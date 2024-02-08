
#include "ImageGrabHandler.h"

#include <pylon/PylonIncludes.h>

#include <metavision/hal/utils/camera_discovery.h>
#include <metavision/hal/utils/hal_exception.h>
#include <metavision/hal/facilities/i_hw_identification.h>

#include <metavision/sdk/driver/camera.h>

#include <iostream>
#include <memory>


int main(int argc, char* argv[])
{
    Pylon::PylonInitialize();
    
    Metavision::Camera eventCamera;
    Pylon::CInstantCamera frameCamera;

    do
    {
        try {
            std::cout << "\n=== Attempting to connect to frame camera ===\n";

            frameCamera.Attach(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
            
            std::cout << "\n=== Connected to frame camera ===\n";

            std::cout << "\n=== Attempting to connect to event camera ===\n";

            eventCamera = Metavision::Camera::from_first_available();
            
            std::cout << "\n=== Connected to event camera ===\n";

            break;
        }
        catch (const Pylon::GenericException& e)
        {
            std::cout << "\nError when connecting to frame camera: " << e.what() << "\n";
        }
        catch (const Metavision::CameraException& e)
        {
            std::cout << "\nError when connecting to event camera: " << e.what() << "\n";
        }

        std::cerr << "\nPress enter to retry...";
        while (std::cin.get() != '\n');

    } while (true);

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

    // Comment the following two lines to disable waiting on exit.
    std::cerr << "\nPress enter to exit program...\n";
    while (std::cin.get() != '\n');

    // Releases all pylon resources.
    Pylon::PylonTerminate();

    return 0;
}
