//
//  ImageBtn.cpp
//  zchat
//
//  Created by reich on 2021/2/26.
//  Copyright © 2021 reich. All rights reserved.
//

#include "zcImageBtn.h"

BEGIN_EVENT_TABLE(zcImageBtn, wxControl)
EVT_PAINT(zcImageBtn::OnPaint)
EVT_ENTER_WINDOW(zcImageBtn::OnEnter)
EVT_LEAVE_WINDOW(zcImageBtn::OnLeave)
EVT_LEFT_DOWN(zcImageBtn::OnDown)
EVT_LEFT_DCLICK(zcImageBtn::OnDClick)
EVT_LEFT_UP(zcImageBtn::OnUp)
END_EVENT_TABLE()
 
IMPLEMENT_DYNAMIC_CLASS(zcImageBtn, wxControl)
 
zcImageBtn::zcImageBtn()
{
}
 
zcImageBtn::zcImageBtn(wxWindow* parent, wxWindowID id,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxValidator& validator)
: normal_bitmap_(NULL), down_bitmap_(NULL), enter_bitmap_(NULL)
{
    Create(parent, id, pos, size, style, validator);
    bgColor_ = parent->GetBackgroundColour();
    sid = -1;
}
 
zcImageBtn::~zcImageBtn()
{
    wxDELETE(normal_bitmap_);
    wxDELETE(enter_bitmap_);
    wxDELETE(down_bitmap_);
}
 
bool zcImageBtn::Create(wxWindow* parent, wxWindowID id,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxValidator& validator)
{
    normal_bitmap_ = NULL;
    down_bitmap_ = NULL;
    enter_bitmap_ = NULL;
    disable_bitmap_ = NULL;
 
    if (!wxControl::Create(parent, id, pos, size, style, validator))
    {
        return false;
    }
    //SetBackgroundStyle(wxBG_STYLE_PAINT);
    is_used_bg_ = false;
    return true;
}
 
wxSize zcImageBtn::DoGetBestSize() const
{
    return GetSize();
}
 
void zcImageBtn::DrawExistBitmap(wxDC* dc, wxBitmap* image1,
        wxBitmap* exist_image)
{
    if (image1)
    {
        dc->DrawBitmap(*image1, 0, 0, true);
 
    } else
    {
        dc->DrawBitmap(*exist_image, 0, 0, true);
    }
}
 
void zcImageBtn::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
#ifdef __WIN32__
    DrawBackground(&dc);
#endif
    
    //1.状态控制绘画,好处就是可以调用Refresh连背景一起刷新.
    switch (button_status_)
    {
        case zcImageBtnNormal:
            dc.DrawBitmap(*normal_bitmap_, 0, 0, true);
            break;
        case zcImageBtnEnter:
            if (!enter_bitmap_)
            {
                int width = DoGetBestSize().GetWidth();
                int height = DoGetBestSize().GetHeight();
                wxClientDC dc(this);
                dc.SetPen(*wxRED_PEN);
                dc.SetBrush(*wxTRANSPARENT_BRUSH);
 
                dc.DrawRectangle(0, 0, width, height);
                break;
            }
            dc.DrawBitmap(*enter_bitmap_, 0, 0, true);
            break;
        case zcImageBtnDown:
            DrawExistBitmap(&dc,down_bitmap_,normal_bitmap_);
            break;
        case zcImageBtnUp:
            dc.DrawBitmap(*normal_bitmap_, 0, 0, true);
            break;
        case zcImageBtnLeave:
            dc.DrawBitmap(*normal_bitmap_, 0, 0, true);
            break;
        case zcImageBtnDClick:
            DrawExistBitmap(&dc,down_bitmap_,normal_bitmap_);
            break;
        case zcImageBtnDisable:
            DrawExistBitmap(&dc,disable_bitmap_,normal_bitmap_);
            break;
        default:
            dc.DrawBitmap(*normal_bitmap_, 0, 0, true);
            break;
    }
 
}
 
void zcImageBtn::DrawBackground(wxDC* dc)
{
    if (is_used_bg_)
    {
        dc->DrawBitmap(background_, 0, 0, true);
    } else
    {
        wxBrush brush(bgColor_);
        wxPen pen(bgColor_);
        dc->SetBrush(brush);
        dc->SetPen(pen);
        dc->DrawRectangle(0, 0, GetSize().x, GetSize().y);
    }
}
 
void zcImageBtn::OnEnter(wxMouseEvent& event)
{
    button_status_ = zcImageBtnEnter;
    //Refresh();
    Update();
}
 
void zcImageBtn::OnLeave(wxMouseEvent& event)
{
    if (!IsEnabled())
    {
        return;
    }
    button_status_ = zcImageBtnLeave;
    Refresh();
    Update();
}
 
void zcImageBtn::OnDClick(wxMouseEvent& event)
{
    //button_status_ = zcImageBtnDown;
    //Refresh(false);
    //Update();
}
 
void zcImageBtn::OnDown(wxMouseEvent& event)
{
    button_status_ = zcImageBtnDown;
    Refresh();
    Update();
}
 
void zcImageBtn::OnUp(wxMouseEvent& event)
{
    if (zcImageBtnDown != button_status_)
    {
        return;
    }
    button_status_ = zcImageBtnUp;
    Refresh();
    Update();
    wxCommandEvent myEvent(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
    myEvent.SetEventObject(this);
    GetEventHandler()->ProcessEvent(myEvent);
}
 
zcImageBtn* zcImageBtn::set_normal_bitmap(wxBitmap* bitmap)
{
    normal_bitmap_ = bitmap;
    return this;
}
 
zcImageBtn* zcImageBtn::set_down_bitmap(wxBitmap* bitmap)
{
    down_bitmap_ = bitmap;
    return this;
}
 
zcImageBtn* zcImageBtn::set_enter_bitmap(wxBitmap* bitmap)
{
    enter_bitmap_ = bitmap;
    return this;
}
 
bool zcImageBtn::Enable(bool enable)
{
    if (enable)
    {
        button_status_ = zcImageBtnNormal;
    } else
    {
        button_status_ = zcImageBtnDisable;
    }
 
    Refresh(false);
    Update();
    return wxControl::Enable(enable);
}
 
bool zcImageBtn::Disable()
{
    return Enable(false);
}
 
zcImageBtn* zcImageBtn::set_disable_bitmap(wxBitmap* bitmap)
{
    disable_bitmap_ = bitmap;
    return this;
}
 
zcImageBtn* zcImageBtn::set_background(const wxBitmap& bitmap)
{
    is_used_bg_ = true;
    background_ = bitmap;
    return this;
}
 
bool zcImageBtn::SetBackgroundColour(const wxColour& colour)
{
    is_used_bg_ = false;
    return wxControl::SetBackgroundColour(colour);
}

