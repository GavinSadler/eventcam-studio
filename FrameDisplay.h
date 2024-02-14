
#ifndef FRAMEDISPLAY_H
#define FRAMEDISPLAY_H

#include <wx/wx.h>
#include <opencv2/core/mat.hpp>

class FrameDisplay : public wxWindow
{
public:
    FrameDisplay(wxWindow* parent);

    void fillBuffer(cv::Mat& image);

private:
    // Handles painting to the FrameDisplay
    void paint(wxPaintEvent& event);

    wxBitmap buffer;
};

#endif // !FRAMEDISPLAY_H
