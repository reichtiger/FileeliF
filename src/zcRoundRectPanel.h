//
//  zcRoundRectPanel.hpp
//  testwx
//
//  Created by reich on 2021/3/3.
//  Copyright © 2021 reich. All rights reserved.
//

#ifndef zcRoundRectPanel_hpp
#define zcRoundRectPanel_hpp
#include "wx/wx.h"
#include <wx/splitter.h>

#include <wx/richtext/richtextctrl.h>
#include <wx/richtext/richtextstyles.h>
#include <wx/richtext/richtextxml.h>
#include <wx/richtext/richtexthtml.h>
#include <wx/richtext/richtextformatdlg.h>
#include <wx/richtext/richtextsymboldlg.h>
#include <wx/richtext/richtextstyledlg.h>
#include <wx/richtext/richtextprint.h>
#include <wx/richtext/richtextimagedlg.h>

#include "zcImageBtn.h"
#include "zcStaticImage.h"
#include "utils.h"

class zcRichTextCtrl : public wxRichTextCtrl
{
public:
    wxWindow* parent_;
    zcRichTextCtrl(wxWindow* parent
                   ,wxWindowID id
                   ,const wxString& value = wxEmptyString
                   ,const wxPoint& pos = wxDefaultPosition
                   ,const wxSize& size = wxDefaultSize
                   ,long style = wxRE_MULTILINE
                   , const wxValidator& validator = wxDefaultValidator
                   , const wxString& name = wxASCII_STR(wxTextCtrlNameStr)):wxRichTextCtrl(
                    parent, id, value, pos, size,style,validator, name)
    {
        parent_ = parent;
//        Bind(wxEVT_SCROLLWIN_LINEUP, &zcRichTextCtrl::OnScroll, this);
//        Bind(wxEVT_SCROLLWIN_LINEDOWN, &zcRichTextCtrl::OnScroll, this);
    }

//    bool ProcessEvent(wxEvent& event)
//    {
//        static wxEvent* s_lastEvent = NULL;
//
//        // 避免死循环
//        if (& event == s_lastEvent)
//            return false;
//        if (event.IsCommandEvent())
//        {
//            s_lastEvent = & event;
//            bool success = false;
//            success = parent_->GetEventHandler()
//                                    ->ProcessEvent(event);
//            return success;
//        }
//        else
//        {
//            return wxRichTextCtrl::ProcessEvent(event);
//        }
//    }
//    void OnScroll(wxScrollWinEvent &event)
//    {
//        printf("richtext ctrl scrolling.....\n");
//        event.Skip();
//    }

    void onMouseDown(wxMouseEvent& evt)
    {
        //printf("zcRichText mouse down\n");
        wxSize a = GetVirtualSize();
        //printf("mouse down x, y = %d, %d\n", a.x, a.y);
        evt.Skip();
    }
    void onMouseUp(wxMouseEvent& evt)
    {
        //printf("zcRichText mouse up\n");
        evt.Skip();
    }
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------
// ----------------------------------------------
struct stringWithLen {
    int index;
    wxString str;// if str ="", and sLen = 0, means a blank line in richtext
    int sLen;
};

class ChatPanel;

class zcRoundRectPanel : public wxPanel
{
public:
    zcRoundRectPanel(int side, wxString txt, wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = wxASCII_STR(wxPanelNameStr));

    int side_;
    ChatPanel* _parent;
    int isLongMsg;
    wxBoxSizer* topsizer;
    wxString _content;

    zcRichTextCtrl* txtCtrl;
    int curFontSize;
    int txtRealLen;
    int new_height;
    void myDrawText(wxDC &dc, wxString &str, wxRect &rect);
    void OnPaint( wxPaintEvent &event );
    void OnSize( wxSizeEvent& );

    void OnScroll(wxScrollWinEvent &event);
    void onMove(wxMouseEvent& evt);
    void onMouseDown(wxMouseEvent& evt);
    void onMouseUp(wxMouseEvent& evt);
    wxDECLARE_EVENT_TABLE();
};


// ------------------------------------
// class CustomPanel
// which should be extended and implements virtual function for real instance 
// ------------------------------------
class CustomPanel : public wxPanel
{
public:
    CustomPanel() {};
    CustomPanel(wxWindow* parent, wxSize newSize);
    virtual ~CustomPanel();

    int pnl_id;

    zcRoundRectPanel* zcp;
    wxColor curColor;

    virtual void calculateStringLen(wxString&) = 0;
    wxVector<stringWithLen> txtVec;

    virtual void OnSize(wxSizeEvent&) = 0;
    wxDECLARE_EVENT_TABLE();
};

// ----------------------------------------------------------------------------
// ChatPanel classes declaration
// ----------------------------------------------------------------------------
class ChatPanel : public CustomPanel
{
public:
    ChatPanel(wxWindow* parent, int direction, wxSize newSize, wxString txt);
    wxBoxSizer* cp_sizer;
    zcImageBtn* avatarBtn;
    wxString curPath;
    wxColor curColor;

    int newWidth;
    int newHeight;
    int curFontSize;
    int fontWidth;
    int fontHeight;

    void calculateStringLen(wxString&);
    wxVector<stringWithLen> txtVec;

    void OnSize(wxSizeEvent&);
    wxDECLARE_EVENT_TABLE();
};

// ------------------------------------
// class ScrollContainer
// which contains many customized panels in it 
// ------------------------------------
class ScrollContainer : public wxPanel
{
public:
    // ctor
    ScrollContainer(wxWindow *parent,
              wxWindowID winid = wxID_ANY,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxTAB_TRAVERSAL | wxNO_BORDER,
              const wxString& name = wxASCII_STR(wxPanelNameStr));

    ~ScrollContainer(){
        cpsVec.clear();
    }

    wxBoxSizer* m_swSizer;
    wxScrolledWindow* m_sw;
    wxScrolledWindow* getScrollWindow() { return m_sw; }

    wxVector<CustomPanel*> cpsVec;
    unsigned int counter;

    //void addChat(wxString txt);
    void appendItem(CustomPanel* cp);// append custom panel

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);
private:

    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
};
#endif /* zcRoundRectPanel_hpp */
