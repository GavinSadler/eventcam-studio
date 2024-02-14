

#include "PreviewUi.h"

#include "FrameDisplay.h"

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/frame.h>

MyApp::MyApp()
{
    this->frame = new wxFrame(nullptr, -1, "EventCam Studio", wxPoint(50, 50), wxSize(800, 600));

    //wxBoxSizer* bSizer = new wxBoxSizer(wxHORIZONTAL);
    
    //this->eventCamDisplay = new FrameDisplay(frame);
    //this->frameCamDisplay = new FrameDisplay(frame);

    //bSizer->Add(this->eventCamDisplay);
    //bSizer->Add(this->frameCamDisplay);

    //this->frame->SetSizer(bSizer);
    //this->frame->Layout();
}

bool MyApp::OnInit()
{   
    this->frame->Show(true);

    return true;
}
