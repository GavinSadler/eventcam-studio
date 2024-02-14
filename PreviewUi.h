
#ifndef PREVIEWUI_H
#define PREVIEWUI_H

#include "FrameDisplay.h"

#include <wx/wx.h>
#include <opencv2/core/mat.hpp>

class MyApp : public wxApp
{
public:
    MyApp();

    bool OnInit() override;

    //FrameDisplay *eventCamDisplay;
    //FrameDisplay *frameCamDisplay;
private:
    wxFrame* frame;
};

#endif // !PREVIEWUI_H
