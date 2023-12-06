#include "licenseFrame.h"
#include "mainFrame.h"
#include "wx/notebook.h"
#include "wx/statline.h"
#include "../resources/sample.xpm"
#include "utils.h"

BEGIN_EVENT_TABLE(licenseFrame, wxFrame)
    EVT_SIZE     (                licenseFrame::OnSize)
    //EVT_BUTTON   (wxID_OK,        licenseFrame::OnBtnAccept) // use Bind instead
    EVT_BUTTON  (wxID_CANCEL, licenseFrame::OnBtnRefuse)
END_EVENT_TABLE()

licenseFrame::licenseFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(LICENSE_WIN_W, LICENSE_WIN_H)
	//, wxCAPTION | wxCLOSE_BOX | wxMAXIMIZE_BOX)
	)
{
	SetIcon(wxICON(sample));
	SetMinClientSize(FromDIP(wxSize(LICENSE_WIN_W, LICENSE_WIN_H)));
    Centre();
    
    wxPanel* p = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(LICENSE_WIN_W-20, LICENSE_WIN_H-100));
    
    p->SetBackgroundColour(wxNullColour);
    
    //p->SetBackgroundColour(*wxWHITE);
    //p->SetForegroundColour(*wxBLACK);
//    
//#ifdef __WXMAC__
//	wxTextCtrl* accText = new wxTextCtrl(p, wxID_ANY, "mac");
//#endif
//#ifdef __LINUX__
//	wxTextCtrl* accText = new wxTextCtrl(p, wxID_ANY, "linux");
//#endif
//#ifdef __WIN32__
//	wxTextCtrl* accText = new wxTextCtrl(p, wxID_ANY, "win");
//#endif
//	accText->SetBackgroundColour(*wxWHITE);
//	accText->SetForegroundColour(*wxBLACK);

    wxStaticText* licText = new wxStaticText(p, wxID_ANY, "License Agreement");
	wxFont fontTitle = licText->GetFont();
    fontTitle.SetPointSize(fontTitle.GetPointSize() + 5);
    licText->SetFont(fontTitle);
    zcRichTextCtrl* license_ctrl = new zcRichTextCtrl(p, ID_RICHINPUT, wxEmptyString, wxDefaultPosition, wxSize(180, 240), wxTE_MULTILINE | wxTE_PROCESS_ENTER | wxBORDER_NONE | wxWANTS_CHARS | wxTE_BESTWRAP | wxTE_AUTO_URL);
    license_ctrl->SetBackgroundColour(*wxWHITE);

	wxFont font = license_ctrl->GetFont();
	font.SetPointSize(font.GetPointSize());
    //license_ctrl->SetDefaultStyle(wxTextAttr(*wxBLACK));
    license_ctrl->SetFont(font);
    license_ctrl->SetToolTip("License");
    license_ctrl->SetValue(LICENSE_STRING);
    license_ctrl->SetEditable(false);
    
    wxBoxSizer  *topSizer = new wxBoxSizer( wxVERTICAL );
    topSizer->Add(licText, wxSizerFlags().Proportion(0).Expand().Border(wxALL, 10));

    wxStaticLine* line = new wxStaticLine(p, wxID_ANY, wxDefaultPosition, wxSize(2, 2), wxHORIZONTAL);
    topSizer->Add(line, wxSizerFlags().Expand());

    topSizer->Add(
        license_ctrl,
        wxSizerFlags().Proportion(1).Expand().Border(wxALL, 10));
    
    wxStaticLine* line2 = new wxStaticLine(p, wxID_ANY, wxDefaultPosition, wxSize(2, 2), wxHORIZONTAL);
	topSizer->Add(line2, wxSizerFlags().Expand());

        wxBoxSizer *buttonSizer = new wxBoxSizer( wxHORIZONTAL );

        wxButton* acceptBtn = new wxButton( p, wxID_OK, "Accept", wxDefaultPosition, wxSize(120, 40));
        acceptBtn->SetForegroundColour(*wxGREEN);
        Bind(wxEVT_BUTTON, &licenseFrame::OnBtnAccept, this, wxID_OK);
        wxFont swissFont(wxFontInfo(12).Family(wxFONTFAMILY_SWISS));
        //acceptBtn->SetFont(swissFont);
        buttonSizer->Add(acceptBtn, 0,          // 水平方向不可缩放
        wxALL,     // 四周有空白边
        1 );      // 空白边框大小
    
	    wxButton* refuseBtn = new wxButton(p, wxID_CANCEL, "Refuse", wxDefaultPosition, wxSize(120, 40));
        refuseBtn->SetForegroundColour(*wxRED);
	    buttonSizer->Add(refuseBtn, 0,          // 水平方向不可缩放
		    wxALL,     // 四周有空白边
		    1);      // 空白边框大小

    topSizer->Add(
        buttonSizer,
        0,                // 垂直方向不可缩放
        wxALIGN_RIGHT | wxALL, 10 ); // 无边框并且居中对
    
    //topSizer->Fit( this );          // 调用对话框大
    //topSizer->SetSizeHints( this ); // 设置对话框最小大
    p->SetSizer( topSizer );
    Layout();
}

void licenseFrame::OnBtnAccept(wxCommandEvent& event)
{
    //wxString msg;
    //msg.Printf(wxT("�?%s 聊天�?), wxVERSION_STRING);
    //wxMessageBox(msg, wxT("About Minimal"), wxOK | wxICON_INFORMATION, this);
    
    wxFile licFile;
    licFile.Create("fflicense.txt", true);
    licFile.Write(LICENSE_STRING);
    licFile.Close();

    mainFrame *mFrm = new mainFrame(wxT("test"));
    mFrm->loginFrm = this;
    mFrm->account = wxT("test");
    mFrm->passwd = wxT("123");
    //wxButton *but = new wxButton(login, wxID_OK,wxT("BUT"), wxPoint(100,100));
    mFrm->Show(true);
    
    Close(true);
}

void licenseFrame::OnBtnRefuse(wxCommandEvent& event)
{
	Close(true);
}

void licenseFrame::OnSize(wxSizeEvent& event)
{
    Layout();
}
