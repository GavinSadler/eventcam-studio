
#include "ImageGrabHandler.h"

#include <pylon/PylonIncludes.h>

#include <metavision/hal/utils/camera_discovery.h>
#include <metavision/hal/utils/hal_exception.h>
#include <metavision/hal/facilities/i_hw_identification.h>
#include <metavision/hal/facilities/i_trigger_in.h>
#include <metavision/hal/facilities/i_trigger_out.h>
//#include <metavision/hal/facilities/i_ll_biases.h>

#include <metavision/sdk/core/algorithms/periodic_frame_generation_algorithm.h>
#include <metavision/sdk/driver/camera.h>

#include <iostream>
#include <memory>

std::unique_ptr<Pylon::CInstantCamera> connectFrameCamera();
std::unique_ptr<Metavision::Camera> connectEventCamera();
void captureVideo(Metavision::Camera& eventCamera, Pylon::CInstantCamera& frameCamera);
void configureEventCamera(Metavision::Camera& camera);

int main(int argc, char* argv[])
{
    Pylon::PylonInitialize();
    
    auto eventCamera = connectEventCamera();
    //auto frameCamera = connectFrameCamera();

    configureEventCamera(*eventCamera);

    // Use this line to enable the camera trigger output signal
    //eventCamera->get_device().get_facility<Metavision::I_TriggerOut>()->enable();

    //captureVideo(*eventCamera, *frameCamera);

    int eWidth = eventCamera->geometry().width();
    int eHeight = eventCamera->geometry().height();

    Metavision::PeriodicFrameGenerationAlgorithm pfga(eWidth, eHeight);

    eventCamera->cd().add_callback([&](const Metavision::EventCD* begin, const Metavision::EventCD* end) {
        pfga.process_events(begin, end);
    });

    pfga.set_output_callback([&](Metavision::timestamp ts, cv::Mat& image) {
        //if (userInterface->eventCamDisplay != nullptr)
            //userInterface->eventCamDisplay->fillBuffer(image);
    });

    std::cerr << "\nPress enter to exit program...\n";
    while (std::cin.get() != '\n');

    // Releases all pylon resources.
    Pylon::PylonTerminate();

    return 0;
}


std::unique_ptr<Pylon::CInstantCamera> connectFrameCamera()
{
    auto frameCamera = std::make_unique<Pylon::CInstantCamera>();

    do
    {
        try {
            std::cout << "\n=== Attempting to connect to frame camera ===\n";

            frameCamera->Attach(Pylon::CTlFactory::GetInstance().CreateFirstDevice());

            std::cout << "\n=== Connected to frame camera ===\n";

            break;
        }
        catch (const Pylon::GenericException& e)
        {
            std::cout << "\nError when connecting to frame camera: " << e.what() << "\n";
        }

        std::cerr << "\nPress enter to retry...";
        while (std::cin.get() != '\n');

    } while (true);

    return frameCamera;
}

std::unique_ptr<Metavision::Camera> connectEventCamera()
{
    std::unique_ptr<Metavision::Camera> eventCamera;
    
    do
    {
        try {
            std::cout << "\n=== Attempting to connect to event camera ===\n";

            eventCamera = std::make_unique<Metavision::Camera>(Metavision::Camera::from_first_available());

            std::cout << "\n=== Connected to event camera ===\n";

            break;
        }
        catch (const Metavision::CameraException& e)
        {
            std::cout << "\nError when connecting to event camera: " << e.what() << "\n";
        }

        std::cerr << "\nPress enter to retry...";
        while (std::cin.get() != '\n');

    } while (true);

    return eventCamera;
}


void captureVideo(Metavision::Camera& eventCamera, Pylon::CInstantCamera& frameCamera)
{
    try
    {
        std::string dataOutputName;

        std::cout << "Enter data file output name: ";
        std::cin >> dataOutputName;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Will output as " << dataOutputName << ".mp4 and " << dataOutputName << ".raw\n";

        // See ImageGrabHandler.cpp for camera configuration and callback definition
        ImageGrabHandler imageGrabber(dataOutputName + ".mp4");
        frameCamera.RegisterImageEventHandler(&imageGrabber, Pylon::ERegistrationMode::RegistrationMode_Append, Pylon::ECleanup::Cleanup_None);

        std::cerr << "\nPress enter to start recording...\n";
        while (std::cin.get() != '\n');

        eventCamera.start();

        eventCamera.start_recording(dataOutputName + ".raw");
        frameCamera.StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByInstantCamera);

        std::cerr << "\nPress enter to stop capturing...\n";
        while (std::cin.get() != '\n');

        eventCamera.stop_recording();
        eventCamera.stop();

        frameCamera.StopGrabbing();
        frameCamera.DeregisterImageEventHandler(&imageGrabber);
    }
    catch (const Pylon::GenericException& e)
    {
        std::cerr << "\nAn exception occurred.\n"
            << e.what() << "\n";
    }
    catch (const Metavision::CameraException& e)
    {
        std::cerr << "\nAn exception occurred.\n"
            << e.what() << "\n";
    }
}

void configureEventCamera(Metavision::Camera& camera)
{
    // 1 second / 30 frames * 1000 milliseconds / second * 1000 usecond / millisecond
    camera.get_device().get_facility<Metavision::I_TriggerOut>()->set_period(1000.0 * 1000.0 / 30.0);
    //camera.get_device().get_facility<Metavision::I_TriggerOut>()->enable();

    // Mark the clock on the data stream
    camera.get_device().get_facility<Metavision::I_TriggerIn>()->enable(Metavision::I_TriggerIn::Channel::Loopback);

    // Removes noise and increases bandwidth
    camera.get_device().get_facility<Metavision::I_LL_Biases>()->set("bias_fo", 1600);
}
