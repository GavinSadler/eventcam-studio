
#include "FrameDisplay.h"

#include <wx/wx.h>
#include <wx/graphics.h>
#include <opencv2/core/mat.hpp>

FrameDisplay::FrameDisplay(wxWindow* parent)
    : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(640, 480))
{
    this->Bind(wxEVT_PAINT, &FrameDisplay::paint, this);
}

void FrameDisplay::paint(wxPaintEvent& event)
{
    // Create paint DC
    wxPaintDC dc(this);

    // Create graphics context from it
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

    if (gc)
    {
        gc->DrawBitmap(this->buffer, 0, 0, this->buffer.GetWidth(), this->buffer.GetHeight());

        delete gc;
    }
}

void FrameDisplay::fillBuffer(cv::Mat& image)
{
    this->buffer = wxBitmap(reinterpret_cast<char*>(image.data), image.cols, image.rows, image.depth());
    this->Refresh();
}
