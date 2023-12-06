#include "testFrame.h"
#include "../resources/sample.xpm"
#include "utils.h"

#define WIN_W 500
#define WIN_H 400

BEGIN_EVENT_TABLE(testFrame, wxFrame)
EVT_SIZE(testFrame::OnSize)
//EVT_BUTTON   (wxID_OK,        licenseFrame::OnBtnAccept) // use Bind instead
EVT_CLOSE(testFrame::OnCloseWindow)
END_EVENT_TABLE()

testFrame::testFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(WIN_W, WIN_H)
		//, wxCAPTION | wxCLOSE_BOX | wxMAXIMIZE_BOX)
	)
{
	SetIcon(wxICON(sample));
	SetMinClientSize(FromDIP(wxSize(WIN_W, WIN_H)));
	Centre();

	wxPanel* p = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(WIN_W - 20, WIN_H - 100));

	p->SetBackgroundColour(wxNullColour);
	custom_new_id = CUSTOM_LAST_ID + 1;
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

	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	dirTarget = new TextCtlWithId(p, custom_new_id, "", wxDefaultPosition, wxSize(260, 26)); custom_new_id++;
	dirTarget->SetHint("target dir or left blank");
	dirTarget->SetLabel("adfasdf");
	//dirTarget->SetEditable(false);
	dirTarget->Bind(wxEVT_TEXT, &testFrame::OnText, this);

	const wxSize imageSize(22, 22);
	//wxBitmap bm = wxArtProvider::GetBitmapBundle(wxART_FOLDER, wxART_OTHER, imageSize).GetBitmap(imageSize);
	wxButton* btnTarget = new wxButton(p, custom_new_id);
	btnTarget->SetLabel("&Set Backup Dir");
	btnTarget->SetWindowStyle(btnTarget->GetWindowStyle() & wxCAPTION);
	//Bind(wxEVT_BUTTON, &testFrame::OnAddTargetDir, this, custom_new_id, -1, dirTarget);

	topSizer->Add(dirTarget, wxSizerFlags(0).Align(wxALIGN_TOP | wxALIGN_LEFT | wxEXPAND));
	topSizer->Add(btnTarget, wxSizerFlags(0).Align(wxALIGN_TOP | wxALIGN_LEFT | wxEXPAND));

	p->SetSizer(topSizer);
	Layout();
}

void testFrame::OnCloseWindow(wxCloseEvent& event)
{
	Unbind(wxEVT_BUTTON, &testFrame::OnAddTargetDir, this, event.GetId());
	Destroy();
}
void testFrame::OnSize(wxSizeEvent& event)
{
	Layout();
}

void testFrame::OnText(wxCommandEvent& event)
{
	
}
void testFrame::OnAddTargetDir(wxCommandEvent& event)
{
	TextCtlWithId* t = (TextCtlWithId*)event.GetEventUserData();

	wxString pathname;
	if (pathname.empty())
	{
		// TODO account for adding the files as well
		pathname = wxDirSelector("Choose a folder to backup", wxGetCwd(),
			wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
		if (pathname.empty())
			return;
	}

	// linux not worked
	cout << "cur event id = " << event.GetId() << endl;
	wxWindow* aa = FindWindowById(event.GetId()-1);
	aa->SetLabel(pathname);

	event.Skip();
	//TextCtlWithId* tc = (TextCtlWithId*)(event.GetEventUserData());
	//tc->SetLabel(pathname);// for win
	//tc->Clear();tc->AppendText(pathname);// for linux
	//cout << "lable=" << tc->GetLabel() << endl;
	//tc->GetParent()->Update();
}