//
//  ImageBtn.hpp
//  zchat
//
//  Created by reich on 2021/2/26.
//  Copyright © 2021 reich. All rights reserved.
//

#ifndef ZCImageBtn_hpp
#define ZCImageBtn_hpp

#include "wx/wx.h"

enum zcImageBtnStatus
{
    zcImageBtnNormal,
    zcImageBtnEnter,
    zcImageBtnDown,
    zcImageBtnUp,
    zcImageBtnLeave,
    zcImageBtnDClick,
    zcImageBtnDisable
};
 
class BoxSizerWithInfo;
class RowPanel;

class zcImageBtn : public wxControl
{
    DECLARE_DYNAMIC_CLASS(zcImageBtn)
    DECLARE_EVENT_TABLE()
public:
    zcImageBtn();
    virtual ~zcImageBtn();
 
    zcImageBtn(wxWindow* parent, wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxBORDER_NONE,
            const wxValidator& validator = wxDefaultValidator);
    bool Create(wxWindow* parent, wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxSUNKEN_BORDER,
            const wxValidator& validator = wxDefaultValidator);
 
    wxSize DoGetBestSize() const;
    void OnPaint(wxPaintEvent& event);
    virtual void OnEnter(wxMouseEvent& event);
    virtual void OnLeave(wxMouseEvent& event);
    virtual void OnDown(wxMouseEvent& event);
    virtual void OnDClick(wxMouseEvent& event);
    virtual void OnUp(wxMouseEvent& event);
    virtual bool Enable(bool enable = true);
    virtual bool Disable();
    /**
     * 设置正常图片
     *
     * @param bitmap
     */
    zcImageBtn* set_normal_bitmap(wxBitmap* bitmap);
 
    /**
     * 1.设置按钮按下时的切换图片
     */
    zcImageBtn* set_down_bitmap(wxBitmap* bitmap);
 
    /**
     * 1.设置按钮按经过时的切换图片
     */
    zcImageBtn* set_enter_bitmap(wxBitmap* bitmap);
 
    /**
     * 1.设置Disable图片.
     *
     * @param bitmap
     * @return this
     */
    zcImageBtn* set_disable_bitmap(wxBitmap* bitmap);
 
    zcImageBtn* set_background(const wxBitmap& bitmap);
 
    bool SetBackgroundColour(const wxColour& colour);
 
    BoxSizerWithInfo* belongSizer;
    RowPanel* outPanel;
    int sid;
protected:
    void DrawExistBitmap(wxDC* dc,wxBitmap* image1,wxBitmap* exist_image);
 
private:
    wxBitmap background_;
    bool is_used_bg_;
    wxColor bgColor_;
    
    wxBitmap* normal_bitmap_;
    wxBitmap* down_bitmap_;
    wxBitmap* enter_bitmap_;
    wxBitmap* disable_bitmap_;
 
    int button_status_;
    wxString text_;
    wxFont text_font_;
 
    void DrawBackground(wxDC* dc);
 
};

#endif /* ImageBtn_hpp */
