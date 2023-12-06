//
//  syncPanel.h
//  FileeliF
//
//  Created by reich on 2023/11/9.
//  Copyright © 2023 reich. All rights reserved.
//

#ifndef syncPanel_hpp
#define syncPanel_hpp

#include "wx/wx.h"
#include "utils.h"
#include "zcPlayGifControl.h"
#include "wx/artprov.h"

#ifdef WIN32
/*打印彩色字
0=黑色   1=蓝色
2=绿色   3=湖蓝色
4=红色   5=紫色
6=黄色   7=白色
8=灰色   9=淡蓝色
10=淡绿色 11=淡浅绿色
12=淡红色 13=淡紫色
14=淡黄色 15=亮白色*/
//ForeColor 字体颜色 BackColor 字体背景颜色
void SetColor(int ForeColor, int BackColor);
//封装一下，方便调用，T表示任何可以被cout输出的类型
template<typename T>
void CoutColorString(T t, int ForeColor, int BackColor=0);
#endif // WIN32

class TargetSetDialog : public wxDialog
{
public:
	TargetSetDialog(const wxString& title);
    ~TargetSetDialog();
    void onRadioChanged(wxCommandEvent& event);
    void OnCloseWindow(wxCloseEvent& event);
    void OnSave(wxCommandEvent& evt);
public:
    wxRadioBox* radioBox;
    wxStaticText* label_lp;
    wxStaticText* label_rp;
    wxTextCtrl* localAddr;
    wxTextCtrl* sftpAddr;
    wxStaticText* labelPort;
    wxTextCtrl* sftpPort;
    wxStaticText* lable_username;
    wxTextCtrl* userName;
    wxStaticText* label_pwd;
    wxTextCtrl* password;

    bool m_canceled;
	wxDECLARE_EVENT_TABLE();
};


class RowPanel;
class syncPanel : public wxPanel
{
public:
    wxWindow* _parent;
    
    syncPanel(wxWindow *parent,
                 wxWindowID winid = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize);
    ~syncPanel(){};
    void OnAddRow(wxCommandEvent& evt);
    void addRow();
    void OnAddSrcTree(wxCommandEvent& event);
	void OnAddTargetDir(wxCommandEvent& event);
	void OnRemoveRow(wxCommandEvent& evt);
    void OnStartStop(wxCommandEvent& evt);
    void stopWatcher();
    void updateInfo2Tasks(RowPanel* rp);
    void OnOpenTargetSetWin(wxCommandEvent& event);
    void OnRemoveTargetDir(wxCommandEvent& evt);
    void OnTest(wxCommandEvent& evt);

public:
    wxBoxSizer* m_syncTopSizer;
    wxString curPath;
    DECLARE_EVENT_TABLE()
};

#endif /* syncPanel_hpp */
