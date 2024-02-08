
#ifndef IMAGEGRABHANDLER_H
#define IMAGEGRABHANDLER_H

#include <pylon/PylonIncludes.h>

#include <string>

class ImageGrabHandler : public Pylon::CImageEventHandler
{
public:
    
    ImageGrabHandler(std::string fileOutPath) : fileOutPath(fileOutPath) {};

    // Attaches to a camera and configures it for our video output format
    virtual void OnImageEventHandlerRegistered(Pylon::CInstantCamera& camera) override;

    // Adds the passed frame to the video output stream
    virtual void OnImageGrabbed(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& grabResult) override;

private:

    // Used to write video files to disk
    Pylon::CVideoWriter videoWriter;

    // Filename to output to disk
    std::string fileOutPath;
};


#endif // !IMAGEGRABHANDLER_H
