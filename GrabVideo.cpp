
#define DISABLE_GRABVIDEO_CPP
#ifndef DISABLE_GRABVIDEO_CPP

// Utility_GrabVideo.cpp
/*
    Note: Before getting started, Basler recommends reading the "Programmer's Guide" topic
    in the pylon C++ API documentation delivered with pylon.
    If you are upgrading to a higher major version of pylon, Basler also
    strongly recommends reading the "Migrating from Previous Versions" topic in the pylon C++ API documentation.

    This sample illustrates how to create a video file in MP4 format.
*/

// Include files to use the pylon API.
#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif

// Include files for Metavision video writing
#include <metavision/sdk/core/utils/video_writer.h>
#include <metavision/sdk/core/utils/cv_video_recorder.h>
#include <opencv2/core.hpp>

//#define AVI_WRITER
//#define VIDEO_WRITER
#define METAVISION_VIDEO_RECORDER

using std::cout, std::cerr, std::cin, std::endl;

// The maximum number of images to be grabbed.
static const uint32_t c_countOfImagesToGrab = 100;
// When this amount of image data has been written, the grabbing is stopped.
static const int64_t c_maxImageDataBytesThreshold = 50 * 1024 * 1024;


int main(int /*argc*/, char* /*argv*/[])
{
    // The exit code of the sample application.
    int exitCode = 0;

    // Before using any pylon methods, the pylon runtime must be initialized.
    Pylon::PylonInitialize();

    try
    {
#ifdef VIDEO_WRITER
        // Check if CVideoWriter is supported and all DLLs are available.
        if (!Pylon::CVideoWriter::IsSupported())
        {
            cout << "VideoWriter is not supported at the moment. Please install the pylon Supplementary Package for MPEG-4 which is available on the Basler website." << endl;
            // Releases all pylon resources.
            Pylon::PylonTerminate();
            // Return with error code 1.
            return 1;
        }

        // Create a video writer object.
        Pylon::CVideoWriter videoWriter;

        // The quality used for compressing the video.
        const uint32_t cQuality = 10;
#endif
#ifdef AVI_WRITER
        // Create an AVI writer object.
        Pylon::CAviWriter aviWriter;

        // The AVI writer supports the output formats PixelType_Mono8,
        // PixelType_BGR8packed, and PixelType_BGRA8packed.
        Pylon::EPixelType aviPixelType = Pylon::PixelType_BGR8packed;
#endif

        // The frame rate used for playing the video (playback frame rate).
        const int cFramesPerSecond = 20;

        // Create an instant camera object with the first camera device found.
        Pylon::CInstantCamera camera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());

        // Print the model name of the camera.
        cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;

        // Open the camera.
        camera.Open();

        // Get the required camera settings.
        Pylon::CIntegerParameter width(camera.GetNodeMap(), "Width");
        Pylon::CIntegerParameter height(camera.GetNodeMap(), "Height");
        Pylon::CEnumParameter pixelFormat(camera.GetNodeMap(), "PixelFormat");

        // Optional: Depending on your camera or computer, you may not be able to save
        // a video without losing frames. Therefore, we limit the resolution:
        width.TrySetValue(640, Pylon::EIntegerValueCorrection::IntegerValueCorrection_Nearest);
        height.TrySetValue(480, Pylon::EIntegerValueCorrection::IntegerValueCorrection_Nearest);

#ifdef VIDEO_WRITER
        // Map the pixelType
        Pylon::CPixelTypeMapper pixelTypeMapper(&pixelFormat);
        Pylon::EPixelType pixelType = pixelTypeMapper.GetPylonPixelTypeFromNodeValue(pixelFormat.GetIntValue());

        // Set parameters before opening the video writer.
        videoWriter.SetParameter(
            (uint32_t)width.GetValue(),
            (uint32_t)height.GetValue(),
            pixelType,
            cFramesPerSecond,
            cQuality);

        // Open the video writer.
        videoWriter.Open("_TestVideo.mp4");
#endif
#ifdef AVI_WRITER
        if (pixelFormat.IsReadable())
        {
            // If the camera produces Mono8 images use Mono8 for the AVI file.
            if (pixelFormat.GetValue() == "Mono8")
            {
                aviPixelType = Pylon::PixelType_Mono8;
            }
        }

        // Optionally set up compression options.
        Pylon::SAviCompressionOptions* pCompressionOptions = NULL;
        // Uncomment the two code lines below to enable AVI compression.
        // A dialog will be shown for selecting the codec.
        //SAviCompressionOptions compressionOptions( "MSVC", true);
        //pCompressionOptions = &compressionOptions;

        // Open the AVI writer.
        aviWriter.Open(
            "_TestAvi.avi",
            cFramesPerSecond,
            aviPixelType,
            (uint32_t)width.GetValue(),
            (uint32_t)height.GetValue(),
            Pylon::ImageOrientation_BottomUp, // Some compression codecs will not work with top down oriented images.
            pCompressionOptions);
#endif
#ifdef METAVISION_VIDEO_RECORDER
        Metavision::CvVideoRecorder metaWriter = Metavision::CvVideoRecorder(
            "_TestMeta.mp4",
            Metavision::VideoWriter::fourcc('H', '2', '6', '4'),
            cFramesPerSecond,
            cv::Size((uint32_t)width.GetValue(), (uint32_t)height.GetValue()),
            true
        );

        metaWriter.start();

        Pylon::CImageFormatConverter fc;
        fc.OutputPixelFormat = Pylon::PixelType_BGR8packed;
        Pylon::CPylonImage image;
#endif
        // Start the grabbing of c_countOfImagesToGrab images.
        // The camera device is parameterized with a default configuration which
        // sets up free running continuous acquisition.
        camera.StartGrabbing(c_countOfImagesToGrab, Pylon::GrabStrategy_LatestImages);


        cout << "Please wait. Images are being grabbed." << endl;

        // This smart pointer will receive the grab result data.
        Pylon::CGrabResultPtr ptrGrabResult;

        // Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // when c_countOfImagesToGrab images have been retrieved.
        while (camera.IsGrabbing())
        {
            // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
            camera.RetrieveResult(5000, ptrGrabResult, Pylon::TimeoutHandling_ThrowException);

            // Image grabbed successfully?
            if (ptrGrabResult->GrabSucceeded())
            {
                // Access the image data.
                cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
                cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
                const uint8_t* pImageBuffer = (uint8_t*)ptrGrabResult->GetBuffer();
                cout << "Gray value of first pixel: " << (uint32_t)pImageBuffer[0] << endl << endl;

#ifdef PYLON_WIN_BUILD
                // Display the grabbed image.
                Pylon::DisplayImage(1, ptrGrabResult);
#endif

#ifdef VIDEO_WRITER
                // If required, the grabbed image is converted to the correct format and is then added to the video file.
                // If the orientation of the image does not mach the orientation required for video compression, the
                // image will be flipped automatically to ImageOrientation_TopDown, unless the input pixel type is Yuv420p.
                videoWriter.Add(ptrGrabResult);

                // If images are skipped, writing video frames takes too much processing time.
                cout << "Images Skipped = " << ptrGrabResult->GetNumberOfSkippedImages() << std::ios_base::boolalpha
                    << "; Image has been converted = " << !videoWriter.CanAddWithoutConversion(ptrGrabResult)
                    << endl;

                // Check whether the image data size limit has been reached to avoid the video file becoming too large.
                if (c_maxImageDataBytesThreshold < videoWriter.BytesWritten.GetValue())
                {
                    cout << "The image data size limit has been reached." << endl;
                    break;
                }
#endif
#ifdef AVI_WRITER
                // If required, the grabbed image is converted to the correct format and is then added to the AVI file.
                // The orientation of the image taken by the camera is top down.
                // The bottom up orientation is specified to apply when opening the Avi Writer. That is why the image is
                // always converted before it is added to the AVI file.
                // To maximize frame rate try to avoid image conversion (see the CanAddWithoutConversion() method).
                aviWriter.Add(ptrGrabResult);

                // If images are skipped, writing AVI frames takes too much processing time.
                cout << "Images Skipped = " << ptrGrabResult->GetNumberOfSkippedImages() << std::ios_base::boolalpha
                    << "; Image has been converted = " << !aviWriter.CanAddWithoutConversion(ptrGrabResult)
                    << endl;

                // Check whether the image data size limit has been reached to avoid the AVI File to get too large.
                // The size returned by GetImageDataBytesWritten() does not include the sizes of the AVI file header and AVI file index.
                // See the documentation for GetImageDataBytesWritten() for more information.
                if (c_maxImageDataBytesThreshold < aviWriter.GetImageDataBytesWritten())
                {
                    cout << "The image data size limit has been reached." << endl;
                    break;
                }
#endif
#ifdef METAVISION_VIDEO_RECORDER
                // Access the image data.
                fc.Convert(image, ptrGrabResult);
                cv::Mat cv_img = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t*)image.GetBuffer());

                metaWriter.write(cv_img);

                // If images are skipped, writing AVI frames takes too much processing time.
                cout << "Images Skipped = " << ptrGrabResult->GetNumberOfSkippedImages() << std::ios_base::boolalpha
                    << endl;
#endif
            }
            else
            {
                cout << "Error: " << std::hex << ptrGrabResult->GetErrorCode() << std::dec << " " << ptrGrabResult->GetErrorDescription() << endl;
            }
        }
    }
    catch (const Pylon::GenericException& e)
    {
        // Error handling.
        cerr << "An exception occurred." << endl
            << e.GetDescription() << endl;
        exitCode = 1;
    }

    // Comment the following two lines to disable waiting on exit.
    cerr << endl << "Press enter to exit." << endl;
    while (cin.get() != '\n');

    // Releases all pylon resources.
    Pylon::PylonTerminate();

    return exitCode;
}

#endif
