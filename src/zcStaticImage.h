//
//  zcStaticImage.hpp
//  zchat
//
//  Created by hzhao on 2021/3/5.
//  Copyright © 2021 hzhao. All rights reserved.
//

#ifndef zcStaticImage_h
#define zcStaticImage_h

#include "wx/wx.h"
 
class zcStaticImage : public wxControl
{
    DECLARE_DYNAMIC_CLASS(zcStaticImage)
    DECLARE_EVENT_TABLE()
public:
    zcStaticImage();
    virtual ~zcStaticImage();
 
    zcStaticImage(wxWindow* parent, wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxBORDER_NONE,
            const wxValidator& validator = wxDefaultValidator);
    bool Create(wxWindow* parent, wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxSUNKEN_BORDER,
            const wxValidator& validator = wxDefaultValidator);
 
    wxBitmap* normal_bitmap_;
    zcStaticImage* set_normal_bitmap(wxBitmap* );
    void OnPaint(wxPaintEvent& event);
};

#endif /* zcImageCtrl_hpp */
