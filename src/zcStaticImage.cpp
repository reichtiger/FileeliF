//
//  zcStaticImage.cpp
//  zchat
//
//  Created by reich on 2021/3/5.
//  Copyright © 2021 reich. All rights reserved.
//

#include "zcStaticImage.h"

BEGIN_EVENT_TABLE(zcStaticImage, wxControl)
EVT_PAINT(zcStaticImage::OnPaint)
END_EVENT_TABLE()
 
IMPLEMENT_DYNAMIC_CLASS(zcStaticImage, wxControl)
 
zcStaticImage::zcStaticImage()
{
}
 
zcStaticImage::zcStaticImage(wxWindow* parent, wxWindowID id,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxValidator& validator)
{
    Create(parent, id, pos, size, style, validator);
}
 
zcStaticImage::~zcStaticImage()
{
}
 
bool zcStaticImage::Create(wxWindow* parent, wxWindowID id,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxValidator& validator)
{
    if (!wxControl::Create(parent, id, pos, size, style, validator))
    {
        return false;
    }
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    return true;
}

zcStaticImage* zcStaticImage::set_normal_bitmap(wxBitmap* bitmap)
{
    normal_bitmap_ = bitmap;
    return this;
}

void zcStaticImage::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.DrawBitmap(*normal_bitmap_, 0, 0, true);
}
