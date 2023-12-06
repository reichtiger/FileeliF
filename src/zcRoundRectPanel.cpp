//
//  zcRoundRectPanel.cpp
//  testwx
//
//  Created by reich on 2021/3/3.
//  Copyright ? 2021 reich. All rights reserved.
//

#include "zcRoundRectPanel.h"
using namespace  std;

BEGIN_EVENT_TABLE(zcRichTextCtrl, wxRichTextCtrl)
EVT_LEFT_DOWN(zcRichTextCtrl::onMouseDown)
EVT_LEFT_UP(zcRichTextCtrl::onMouseUp)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// zcRoundRectPanel implementation
// ----------------------------------------------------------------------------
BEGIN_EVENT_TABLE(zcRoundRectPanel, wxPanel)
EVT_PAINT(zcRoundRectPanel::OnPaint)
EVT_SIZE(zcRoundRectPanel::OnSize)
EVT_MOTION(zcRoundRectPanel::onMove)
EVT_LEFT_DOWN(zcRoundRectPanel::onMouseDown)
EVT_LEFT_UP(zcRoundRectPanel::onMouseUp)
END_EVENT_TABLE()
// frame constructor
zcRoundRectPanel::zcRoundRectPanel(int side, wxString txt, wxWindow *parent,
            wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxString& name)
               : wxPanel(parent,
                         wxID_ANY,
                         wxDefaultPosition,
                         size)
{
    side_ = side;
    _content = txt;
    topsizer = new wxBoxSizer(wxHORIZONTAL);
    _parent = (ChatPanel*)parent;

    txtCtrl = new zcRichTextCtrl(this, ID_RICHTEXT, wxEmptyString, wxDefaultPosition, wxSize(200, 150), wxTE_NO_VSCROLL|wxNO_BORDER|wxTE_BESTWRAP|wxTE_READONLY|wxTE_AUTO_URL);

    topsizer->Add(txtCtrl, 1, wxCENTER | wxEXPAND | wxALL, 5);
    if (side_ % 2 > 0)
        txtCtrl->SetBackgroundColour(wxColor(176, 229, 110));
    else txtCtrl->SetBackgroundColour(*wxWHITE);

    txtCtrl->EnableVerticalScrollbar(false);

    wxFont font = txtCtrl->GetFont();
    curFontSize = font.GetPointSize()+5;
    font.SetPointSize(curFontSize);
    txtCtrl->SetDefaultStyle(wxTextAttr(*wxBLACK));
    txtCtrl->SetFont(font);
    txtCtrl->SetToolTip("...");
    txtCtrl->SetValue(txt);
    txtCtrl->Fit();
    SetSizerAndFit(topsizer);
    //txtCtrl->Disable();
    txtCtrl->Bind(wxEVT_SCROLLWIN_LINEUP, &zcRoundRectPanel::OnScroll, this);
    txtCtrl->Bind(wxEVT_SCROLLWIN_LINEDOWN, &zcRoundRectPanel::OnScroll, this);
}

// 解决滚动问题。richtext中如果鼠标滚动，则disable掉。这样外面scrollwindows才会滚动。
void zcRoundRectPanel::OnScroll(wxScrollWinEvent &event)
{
    txtCtrl->Disable();
    printf("round rect panel scrolling\n");
    //event.Skip();
}

// 当鼠标移动时候，enable控件。
void zcRoundRectPanel::onMove(wxMouseEvent& evt)
{
    txtCtrl->Disable();
    printf("txtCtrl enabled\n");
}

void zcRoundRectPanel::onMouseDown(wxMouseEvent& evt)
{
    txtCtrl->Enable();
    //evt.Skip();
}
void zcRoundRectPanel::onMouseUp(wxMouseEvent& evt)
{
    txtCtrl->Disable();
    //evt.Skip();
}

void zcRoundRectPanel::myDrawText(wxDC &dc, wxString &str, wxRect &rt)
{
    dc.DrawText(str, rt.x, rt.y);
}
void zcRoundRectPanel::OnPaint(wxPaintEvent &event)
{
    wxSize curSize = GetSize();
    //printf("round panel new size= %d, %d\n", curSize.x, curSize.y);
    wxPaintDC dc(this);
    PrepareDC(dc);

    if (side_ % 2 > 0)
        dc.SetBrush(wxColor(176, 229, 110));
    else
        dc.SetBrush(*wxWHITE_BRUSH);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRoundedRectangle(0, 0, curSize.x, curSize.y-5, 5);
}

void zcRoundRectPanel::OnSize(wxSizeEvent& event)
{

    if (_parent->txtVec.size() == 0) return;

    int cur_width = txtCtrl->GetSize().x;
    //txtCtrl->SetBackgroundColour(wxColor(200,104, 103));

    if (isLongMsg == 0) {
        new_height = curFontSize + 4;
        wxVector<stringWithLen>::iterator p = _parent->txtVec.begin();
        cur_width = p->sLen;
    }
    else{

        int lines = 0;

        for (wxVector<stringWithLen>::iterator p = _parent->txtVec.begin(); p != _parent->txtVec.end(); p++){
            txtRealLen = p->sLen;
            if (txtRealLen > 0) {
                int cur = (txtRealLen / (cur_width+1) + 1 );
                lines += cur;
                //printf("%ls,totalLen=%d, width=%d, lines=%d\n", p->str.wc_str(), txtRealLen, cur_width, cur);
            }else if (txtRealLen == 0) {
                lines += 1;
                //printf("%ls,totalLen=%d, width=%d, lines=%d\n", p->str.wc_str(), txtRealLen, cur_width, lines);
            }
        }
        //printf("lines======= %d\n", lines);
        new_height = lines  * (curFontSize + 4) + 5;

    }

    txtCtrl->SetMinSize(wxSize(cur_width, new_height));

    Layout();
}


// ---------------------------------------------------------------------------------------------------------------
// ChatPanel implementation
// ---------------------------------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(CustomPanel, wxPanel)
//EVT_SIZE(CustomPanel::OnSize)
END_EVENT_TABLE()
// frame constructor
CustomPanel::CustomPanel(wxWindow* parent, wxSize newSize)
               : wxPanel(parent,
                         wxID_ANY,
                         wxDefaultPosition,
                         newSize
                         ,wxFULL_REPAINT_ON_RESIZE  // win platform
                         )
{
    pnl_id = -1;
    zcp = nullptr;
}
CustomPanel::~CustomPanel()
{

}

// ---------------------------------------------------------------------------------------------------------------
// ChatPanel implementation
// ---------------------------------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(ChatPanel, CustomPanel)
EVT_SIZE(ChatPanel::OnSize)
END_EVENT_TABLE()
// frame constructor
ChatPanel::ChatPanel(wxWindow* parent, int direction, wxSize newSize, wxString txt)
               : CustomPanel(parent,
                         newSize)
{

    curColor = wxColor(wxColor(235,235, 236));
    wxFont font = GetFont();
    curFontSize = font.GetPointSize()+5;

    wxBoxSizer* top_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* cp_sizer = new wxBoxSizer(wxVERTICAL);
    curPath = wxGetCwd();

    wxBoxSizer* chat_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxString pngpath(curPath+wxT("/resources/btn1.png"));
    wxImage png(pngpath, wxBITMAP_TYPE_PNG);
    wxBitmap* normal = new wxBitmap(png);
    avatarBtn = new zcImageBtn(this, ID_TEST_BITMAP_BTN, wxPoint(10,10), normal->GetSize());
    avatarBtn->set_normal_bitmap(normal);
    avatarBtn->SetMinSize(wxSize(40, 40));

    calculateStringLen(txt);
    zcp = new zcRoundRectPanel(direction, txt, this, wxNewId(), wxDefaultPosition, wxDefaultSize);


    // check if it too short msg
    int longMsg = 0;
    int no_1_len = 0;
    for (wxVector<stringWithLen>::iterator p = txtVec.begin(); p != txtVec.end(); p++){
        int lenOfLine = p->sLen;
        if (lenOfLine > no_1_len)
            no_1_len = lenOfLine;

        printf("cur lenOfLine = %d\n", lenOfLine);
        if (lenOfLine > 400) {
            longMsg = 1;
            break;
        }
    }

    zcp->isLongMsg = longMsg;

    if (longMsg == 0) {
        zcp->SetMinSize(wxSize(66, 22));
    }
    wxPanel* p_space = new wxPanel(this, wxNewId(), wxDefaultPosition, wxSize(200,2));
    p_space->SetMinSize(wxSize(200, 2));
//    p_space->SetMaxSize(wxSize(300,20));
    //p_space->SetBackgroundColour(*wxRED);

    if (direction % 2 > 0){

        wxString right_arrow_path(curPath+wxT("/resources/right_arrow.png"));
        wxImage right_arrow(right_arrow_path, wxBITMAP_TYPE_PNG);
        wxBitmap* normal_right = new wxBitmap(right_arrow);
        zcStaticImage* arrow_right = new zcStaticImage(this, wxNewId(), wxPoint(0,0), right_arrow.GetSize());
        arrow_right->set_normal_bitmap(normal_right);

        chat_sizer->Add(p_space, 1-longMsg, wxEXPAND|wxALL, 0);
        chat_sizer->Add(zcp, longMsg, wxEXPAND|wxALL, 0);
        chat_sizer->Add(arrow_right, 0, wxLEFT|wxALL, 0);
        chat_sizer->Add(avatarBtn, 0, wxRIGHT|wxALL, 5);

    }else {

        wxString left_arrow_path(curPath+wxT("/resources/left_arrow.png"));
        wxImage left_arrow(left_arrow_path, wxBITMAP_TYPE_PNG);
        wxBitmap* normal_left = new wxBitmap(left_arrow);
        zcStaticImage* arrow_left = new zcStaticImage(this, wxNewId(), wxPoint(0,0), left_arrow.GetSize());
        arrow_left->set_normal_bitmap(normal_left);

        chat_sizer->Add(avatarBtn, 0, wxLEFT|wxALL, 5);
        chat_sizer->Add(arrow_left, 0, wxLEFT|wxALL, 0);
        chat_sizer->Add(zcp, longMsg, wxEXPAND|wxALL, 0);
        chat_sizer->Add(p_space, 1-longMsg, wxEXPAND|wxALL, 0);
    }

    cp_sizer->Add(chat_sizer, 1, wxEXPAND|wxALL, 0);
    top_sizer->Add(cp_sizer, 1, wxEXPAND|wxALL, 0);
    top_sizer->AddSpacer(8);

    SetSizerAndFit(top_sizer);
    wxSize curS = zcp->GetSize();
    newWidth = curS.x;
    newHeight = curS.y;

}

void ChatPanel::OnSize(wxSizeEvent& event)
{
    wxSize curSize = GetSize();
    //zcp->SetMinSize(wxSize(curSize.x, zcp->new_height));
    Layout();
}

void ChatPanel::calculateStringLen(wxString &str)
{
    std::wstring tempStr = L"";
    std::wstring oldStr = str.wc_str();
    int cur_line_len = 0;

    stringWithLen* sl = nullptr;
    int count = 0;
    for(int i = 0; i < str.length(); ++i)
    {
        // 获取每个字符的第一个字节
        unsigned char* pCh = (unsigned char*)&oldStr[i];

        if (i == 0) {
            sl = new stringWithLen;
            sl->index = count;
            sl->sLen = 0;
            sl->str = L"";
        }else if (*pCh == '\n') {
            wxString mystring(tempStr);
            sl->str = mystring;
            sl->sLen = cur_line_len;

            txtVec.push_back(*sl);
            count++;
            sl = new stringWithLen;
            sl->index = count;
            tempStr = L"";
            cur_line_len = 0;

        }
        if (*pCh == ' '){
            tempStr += oldStr[i];
            cur_line_len += curFontSize/4;
        }
        // 数字
        else if((*pCh >= 0x30 && *pCh <= 0x39) && *(pCh + 1) == 0x00)
        {
            tempStr += oldStr[i];
            cur_line_len += curFontSize/2+1;
        }
        // 大写字母
        else if((*pCh >= 0x41 && *pCh <=0x5A)  && *(pCh + 1) == 0x00)
        {
            tempStr += oldStr[i];
            cur_line_len += curFontSize;
        }
        // 小写字母
        else if((*pCh >= 0x61 && *pCh <= 0x7A)  && *(pCh + 1) == 0x00)
        {
            tempStr += oldStr[i];
            cur_line_len += curFontSize/2;
        }
        // 希腊文字
        else if((*pCh >= 0xB1 && *pCh <= 0xC9) && *(pCh + 1) == 0x03)
        {
            tempStr += oldStr[i];
            cur_line_len += curFontSize + 2;
        }
        // 中文
        else if (((*pCh >= 0) && (*pCh <= 0xff)) && (*(pCh + 1) >= 0x4e && *(pCh + 1) <= 0x9f))
        {
            tempStr += oldStr[i];
            cur_line_len += curFontSize + 3;
        }
        else{
            cur_line_len += curFontSize / 2;
        }
    }

    if (tempStr != L"") {
        // the last one
        wxString mystring(tempStr);
        sl->str = mystring;
        sl->sLen = cur_line_len;
        txtVec.push_back(*sl);

    }
    //int x, y;
    //GetTextExtent(str, &x, &y);
    //printf("x=%d, y= %d\n", x, y);
}



// ----------------------------------------------------------------------------
// MainChatPanel implementation
// ----------------------------------------------------------------------------
BEGIN_EVENT_TABLE(ScrollContainer, wxPanel)
EVT_SIZE(ScrollContainer::OnSize)
END_EVENT_TABLE()
// frame constructor
ScrollContainer::ScrollContainer(wxWindow *parent,
                             wxWindowID winid,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style,
                             const wxString& name)
               : wxPanel(parent,
                         winid,
                         wxDefaultPosition,
                         wxSize(600, 500))
{

    counter = 0;

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

        wxBoxSizer* middleSizer = new wxBoxSizer(wxVERTICAL);

        m_sw = new wxScrolledWindow(this, wxNewId(), wxDefaultPosition, wxDefaultSize, wxVSCROLL);
        m_sw->SetMinSize(wxSize(700, 500));
        m_sw->SetBackgroundColour(wxColor(235,235, 236));

        // 每次滚动10个象素
        //int pixelsPerUnixY = 1;
        //int noUnitsY = 1;
        //m_sw->SetScrollbars(0, pixelsPerUnixY, 0, noUnitsY);

        m_swSizer = new wxBoxSizer(wxVERTICAL);
        m_sw->SetSizer(m_swSizer);
        //m_swSizer->AddStretchSpacer();
        middleSizer->Add(m_sw, 1, wxEXPAND|wxALL, 0, NULL);

    topSizer->Add(middleSizer, 1, wxEXPAND|wxALL, 0, NULL);
    //topSizer->SetSizeHints(this);
    Center();
    //for (int i = 0; i < 2; i++) addChat(txt);

    SetSizerAndFit(topSizer);

}

// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

void ScrollContainer::appendItem(CustomPanel* cp)
{
    cp->pnl_id = counter;
    m_sw->GetSizer()->Insert(counter, cp, 0, wxEXPAND | wxRIGHT,  0);
    //m_sw->GetSizer()->Add(cp, 1, wxEXPAND, 5, NULL);
    m_sw->SetScrollbars(0, (cp->GetSize().GetHeight() + 5), 0, ((cp->GetSize().GetHeight()) + 5)*counter, 0, 0);
    this->GetSizer()->SetSizeHints(this);

    m_sw->Scroll(wxPoint(0, cp->GetSize().y));

    cpsVec.push_back(cp);
    counter++;
    Layout();
    Update();
}

// void ScrollContainer::addChat(wxString text)
// {
//     CustomPanel* cp = new ChatPanel(getScrollWindow(), counter, wxDefaultSize, text);
//     cp->SetMinSize(wxSize(200, cp->curFontSize+30));
//     m_sw->GetSizer()->Insert(counter, cp, 0, wxEXPAND | wxRIGHT,  0);
//     //m_sw->GetSizer()->Add(cp, 1, wxEXPAND, 5, NULL);
//     m_sw->SetScrollbars(0, (cp->GetSize().GetHeight() + 5), 0, ((cp->GetSize().GetHeight()) + 5)*counter, 0, 0);
//     //this->GetSizer()->SetSizeHints(this);

//     m_sw->Scroll(wxPoint(0, cp->GetSize().y));

//     cpsVec.push_back(cp);
//     counter++;
//     Layout();
//     Update();

// }

void ScrollContainer::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void ScrollContainer::OnSize(wxSizeEvent& event)
{
    //wxVector<CustomPanel*>::iterator cp = cpsVec.begin();
    //for (cp = cpsVec.begin(); cp != cpsVec.end(); cp++){
    //    zcRoundRectPanel* zcp = (*cp)->zcp;
    //    if (!zcp) break;
    //    wxSize ss = zcp->GetSize();
    //    (*cp)->SetMinSize(wxSize(ss.x, zcp->new_height + 5));
    //}
	wxVector<CustomPanel*>::iterator cp = cpsVec.begin();
	for (cp = cpsVec.begin(); cp != cpsVec.end(); cp++) {
		wxSize ss = (*cp)->GetSize();
		(*cp)->SetMinSize(wxSize(ss.x, ss.y));
	}
    Layout();
}
