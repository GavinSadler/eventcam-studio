
#include "ImageGrabHandler.h"

#include <pylon/PylonIncludes.h>

#include <iostream>


void ImageGrabHandler::OnImageEventHandlerRegistered(Pylon::CInstantCamera& camera)
{
    // Print the model name of the camera.
    std::cout << "Using device: " << camera.GetDeviceInfo().GetModelName() << std::endl << std::endl;

    // Open the camera.
    camera.Open();

    // Get the required camera settings.
    auto& nodeMap = camera.GetNodeMap();

    // Used to print settings out
    std::vector<Pylon::CParameter*> settingsReferences;

    // Image format settings
    Pylon::CIntegerParameter width(nodeMap, "Width");
    Pylon::CIntegerParameter height(nodeMap, "Height");
    Pylon::CCommandParameter centerX(nodeMap, "BslCenterX");
    Pylon::CCommandParameter centerY(nodeMap, "BslCenterY");
    Pylon::CEnumParameter pixelFormat(nodeMap, "PixelFormat");

    // Acquisition settings
    Pylon::CFloatParameter acquisitionFrameRate(nodeMap, "AcquisitionFrameRate");
    Pylon::CBooleanParameter acquisitionFrameRateEnable(nodeMap, "AcquisitionFrameRateEnable");
    Pylon::CEnumParameter triggerMode(nodeMap, "TriggerMode");
    Pylon::CEnumParameter triggerSource(nodeMap, "TriggerSource");

    // This vector is responsible for printing out each setting
    settingsReferences.push_back(&width);
    settingsReferences.push_back(&height);
    settingsReferences.push_back(&pixelFormat);
    settingsReferences.push_back(&acquisitionFrameRate);
    settingsReferences.push_back(&acquisitionFrameRateEnable);
    settingsReferences.push_back(&triggerMode);
    settingsReferences.push_back(&triggerSource);

    std::cout << "Current camera settings :" << std::endl;

    for each (auto setting in settingsReferences)
        std::cout << "\t" << (*setting).GetNode()->GetName() << ": " << (*setting).ToString() << std::endl;

    width.TrySetValue(2560);
    height.TrySetValue(1920);
    centerX.Execute();
    centerY.Execute();
    pixelFormat.TrySetValue("RGB8");

    acquisitionFrameRateEnable.TrySetValue(true);
    acquisitionFrameRate.TrySetValue(30.0);
    //triggerMode.TrySetValue("On");
    //triggerSource.TrySetValue("Line 1");

    std::cout << std::endl << "Applied camera settings :" << std::endl;

    for each (auto setting in settingsReferences)
        std::cout << "\t" << (*setting).GetNode()->GetName() << ": " << (*setting).ToString() << std::endl;

    // Grab pixel format information
    Pylon::CPixelTypeMapper pixelTypeMapper(&pixelFormat);
    Pylon::EPixelType pixelType = pixelTypeMapper.GetPylonPixelTypeFromNodeValue(pixelFormat.GetIntValue());

    // Set parameters before opening the video writer.
    videoWriter.SetParameter(
        width.GetValue(),
        height.GetValue(),
        pixelType,
        acquisitionFrameRate.GetValue(), // TODO: This value has to be tied to some other value
        100);

    // Open the video writer.
    videoWriter.Open(fileOutPath.c_str());
}

void ImageGrabHandler::OnImageGrabbed(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& grabResult)
{
    // If required, the grabbed image is converted to the correct format and is then added to the video file.
    // If the orientation of the image does not mach the orientation required for video compression, the
    // image will be flipped automatically to ImageOrientation_TopDown, unless the input pixel type is Yuv420p.
    videoWriter.Add(grabResult);
    const Pylon::IImage& image(grabResult);
    std::cout << "Pixel type: " << image.GetPixelType() << "\n";

    // If images are skipped, writing video frames takes too much processing time.
    std::cout << "Images Skipped = " << grabResult->GetNumberOfSkippedImages() << std::ios_base::boolalpha
        << "; Image has been converted = " << !videoWriter.CanAddWithoutConversion(grabResult)
        << std::endl;
}
