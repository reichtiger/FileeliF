//
//  mainFrame.cpp
//  zchat
//
//  Created by reich on 2021/2/19.
//  Copyright 2021 reich. All rights reserved.
//
#include "utils.h"
#include "mainFrame.h"
#include "../resources/sample.xpm"
#include "wx/artprov.h"
#include "wx/taskbar.h"
#include "wx/gifdecod.h"
#include "syncPanel.h"
#include "logPanel.h"
#include <exception>

#ifdef __WIN32__
#include "ProcMgr.h"
#endif

#define MAINFRAME_W 900
#define MAINFRAME_H 600

extern mainFrame* gMainFrm;
const wxString mainFrame::LOG_FORMAT = " %-12s %-36s    %-36s";
wxDEFINE_EVENT(MY_EVENT, wxCommandEvent);//这是事件类型定义，所以不能位于头文件
bool gIsWatching = false;
std::map<wxString, Backups> g_tasksMap;
std::vector<Action> g_actionsVec;

////////////////////////////////////////////
// mainFrame implementation
//
////////////////////////////////////////////
BEGIN_EVENT_TABLE(mainFrame, wxFrame)
EVT_BUTTON(ID_TEST_BITMAP_BTN, mainFrame::OnClickBitmapBtn)
EVT_SIZE(mainFrame::OnSize)
EVT_BUTTON(ID_AVATAR, mainFrame::OnBtnAVATAR)
EVT_CLOSE(mainFrame::OnCloseWindow)
END_EVENT_TABLE()


mainFrame::~mainFrame()
{
    delete m_taskBarIcon;
}
mainFrame::mainFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(MAINFRAME_W, MAINFRAME_H)
		)
{
    m_watcher = nullptr;
    SetIcon(wxICON(sample));
    SetMinClientSize(FromDIP(wxSize(MAINFRAME_W, MAINFRAME_H)));
	SetBackgroundColour(*wxBLACK);
    curPath = wxGetCwd();
    printf("cur path = %s\n", curPath.ToStdString().c_str());
    gIsWatching = false;
    //pp = new movablePanel(this, this);
    //pp->SetBackgroundColour(*wxYELLOW);
    //pp->SetCursor(wxCursor(wxCURSOR_SIZENS));
    //pp->SetSize(FromDIP(wxSize(800, 600)));
    // start layout
	splitterTop = new wxSplitterWindow( this, ID_SPLITTER_TOP, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE | wxSP_NOSASH);
    splitterTop->SetBackgroundColour(*wxLIGHT_GREY);
	wxPanel* p_topbar = new wxPanel(splitterTop);
	//p_topbar->SetMinSize(wxSize(MAINFRAME_W-300, 60));
    p_topbar->SetBackgroundColour(*wxLIGHT_GREY);
	wxBoxSizer* sizer_top = new wxBoxSizer(wxHORIZONTAL);
	p_topbar->SetSizerAndFit(sizer_top);
	
	wxBoxSizer* sizer_main = new wxBoxSizer(wxVERTICAL);
	wxImageList* imageList = new wxImageList(48, 48, true, 1);
	wxBitmap bitmap1(curPath + wxT("/resources/setting-48.png"), wxBITMAP_TYPE_PNG); imageList->Add(bitmap1);
	wxBitmap bitmap2(curPath + wxT("/resources/account-48.png"), wxBITMAP_TYPE_PNG); imageList->Add(bitmap2);
	wxBitmap bitmap3(curPath + wxT("/resources/logs-48.png"), wxBITMAP_TYPE_PNG); imageList->Add(bitmap3);
	wxBitmap bitmap4(curPath + wxT("/resources/about-48.png"), wxBITMAP_TYPE_PNG); imageList->Add(bitmap4);

    m_bookCtrl = new wxListbook(splitterTop, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_LEFT);
	m_bookCtrl->AssignImageList(imageList);
	//m_bookCtrl->SetImages(m_images);
    if ( !m_bookCtrl )
        return;

   // m_bookCtrl->Hide();
    CreateInitialPages(m_bookCtrl);
    sizer_main->Insert(0, m_bookCtrl, wxSizerFlags(5).Expand().Border());
    sizer_main->Show(m_bookCtrl);
    m_bookCtrl->SetBackgroundColour(*wxWHITE);
    //p_mainPanel->SetSizerAndFit(sizer_main);
    sizer_main->Layout();

	Connect(ID_SPLITTER_TOP, wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, (wxObjectEventFunction)&mainFrame::onSplitTopSashPosChanged);
    
    splitterTop->SetSashGravity(0);
    splitterTop->SetMinimumPaneSize(60);
    splitterTop->SplitHorizontally(p_topbar, m_bookCtrl, 60);
	//splitter->Disable();
    Layout();
	CreateStatusBar(2);
    wxFont font(wxFontInfo(11).Family(wxFONTFAMILY_TELETYPE));

	SetStatusText("Welcome to FileeliF!");
    SetStatusText("Version 1.02", 1);
    wxStatusBar* sb = GetStatusBar();
    sb->SetFont(font);
    sb->SetSize(sb->GetBestSize());
	m_taskBarIcon = new zcTaskBarIcon(this);

	// we should be able to show up to 128 characters on Windows
    if (!m_taskBarIcon->SetIcon(wxArtProvider::GetBitmapBundle(wxART_WX_LOGO, wxART_OTHER, wxSize(32, 32)),
		"FileeliF hidden here\n"
		"Call me anytime when you prefer :)\n")) {
        wxLogError("Could not set icon.");
    }

#ifdef __WIN32__
	//PROCESS_INFORMATION pi;
	//createProcessWithAdmin(_T("E:\\Meta5Tester\\MetaTester64.exe"), &pi);
    //createProc();
    //WinExec("E:\\Meta5Tester\\MetaTester64.exe", SW_HIDE);
#endif
}

bool mainFrame::CreateWatcher()
{
    if (m_watcher) {
        fflog_err(L"Watcher already initialized");
        return false;
    }
	m_watcher = new wxFileSystemWatcher();
	m_watcher->SetOwner(this);
	Bind(wxEVT_FSWATCHER, &mainFrame::OnFileSystemEvent, this);
	fflog(10, L"++++++++++++++++watcher created++++++++++++++\n");
    return true;
}

void mainFrame::DeleteWatcher() {
	wxCHECK_RET(m_watcher, "Watcher not initialized");
	fflog_warn(L"----------- watcher removed ------------\n");
	//m_filesList->DeleteAllItems();
	wxDELETE(m_watcher);
}
//void mainFrame::playBackingupGif()
//{
//	wxGIFDecoder* m_gif_decoder = new wxGIFDecoder();
//	wxFileInputStream m_input_stream("");
//	m_gif_decoder->LoadGIF(m_input_stream);
//	int n = m_gif_decoder->GetFrameCount();
//	wxImage image;
//	wxClientDC dc(this);
//	for (int j = 0; j < n; j++)
//	{
//		m_gif_decoder->ConvertToImage(j, &image)
//			wxBimatp bitmap(image);
//		dc.Clear();
//		dc.DrawBitmap(bitmap, 0, 0, true);
//		wxSleep(1);
//	}
//}
void mainFrame::realCloseWindow() 
{ 
    Destroy(); 
}
void mainFrame::OnSize(wxSizeEvent& event)
{
    
	wxSize curClientSize = this->GetClientSize();   // 获得当前客户区高度
    //pp->SetSize(curClientSize.x+1, curClientSize.y+1); // 右侧和底部能露出颜色
	Layout();
}

void mainFrame::OnClickBitmapBtn(wxCommandEvent& event)
{
    printf("clicked\n");
}

wxBitmap mainFrame::GetPositionBackgroundBitmap(int x, int y,
	int width, int height)
{
	wxRect rect;
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	wxBitmap temp = this->bg.GetSubBitmap(rect);
	return temp;
}

void mainFrame::onSplitTopSashPosChanged(wxSplitterEvent& event)
{
    splitterTop->SetSashPosition(60);
}

void mainFrame::OnBtnAVATAR(wxCommandEvent& event)
{
	Close();
}

void mainFrame::OnCloseWindow(wxCloseEvent& event)
{
    Destroy();
    //Show(false); 
}

wxPanel* mainFrame::CreateSyncPage(wxBookCtrlBase* parent)
{
    m_syncPanel = new syncPanel(parent);

#if wxUSE_HELP
    m_syncPanel->SetHelpText("Panel with some Radio Buttons");
#endif
	return m_syncPanel;
}

wxWindow* mainFrame::CreateAccountPage(wxBookCtrlBase* parent)
{
	wxTextCtrl* text = new wxTextCtrl(parent, wxID_ANY, "Account",
		wxDefaultPosition, wxDefaultSize,
		wxTE_MULTILINE);

#if wxUSE_HELP
	text->SetHelpText("Page consisting of just a text control");
#endif

	return text;
}

wxWindow* mainFrame::CreateLogsPage(wxBookCtrlBase* parent)
{
    m_logPanel = new logPanel(parent);
    
	return m_logPanel;
}

wxWindow* mainFrame::CreateAboutPage(wxBookCtrlBase* parent)
{
    wxPanel* p = new wxPanel(parent);

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* statsizer = new wxStaticBoxSizer(
		new wxStaticBox(p, wxID_ANY, "  About FileeliF "), wxTOP);
	statsizer->Add(
		new wxStaticText(p, wxID_ANY, "FileeliF is a totally free file backup tool. Dear users, you can store important files, videos, photos, etc. in other hard drive paths, or on the hard drive path under remote SFTP. So that important data can have multiple backups. Safety first. If you have any comments during use, please feel free to submit them at any time.Email address : asdf@sadf.com, or submit directly : ", wxDefaultPosition, wxSize(400, 300)),
		wxSizerFlags().Border(wxALL, 30));
    topSizer->Add(
		statsizer,
		wxSizerFlags(1).Expand().Border(wxALL, 10));

	

    p->SetSizerAndFit(topSizer);

	return p;
}

void mainFrame::updateTaskList()
{
	wxMutexLocker lock(m_mutex);
	if (lock.IsOk())
	{
		// do something
	}
	else
	{
		/*... we have not been able to
			... acquire the mutex, fatal error*/
	}
}

int mainFrame::GetIconIndex(wxBookCtrlBase* bookCtrl)
{
	if (bookCtrl)
	{
		const int nImages = bookCtrl->GetImageCount();
		if (nImages > 0)
		{
			return bookCtrl->GetPageCount() % nImages;
		}
	}

	return -1;
}

void mainFrame::CreateInitialPages(wxBookCtrlBase* p)
{
	// Create and add some panels to the notebook

	wxWindow* page = CreateSyncPage(p);
	p->AddPage(page, SETTING_PAGE_NAME, false, GetIconIndex(p));

	page = CreateAccountPage(p);
	p->AddPage(page, ACCOUNT_PAGE_NAME, false, GetIconIndex(p));

	page = CreateLogsPage(p);
	p->AddPage(page, LOGS_PAGE_NAME, false, GetIconIndex(p));

	page = CreateAboutPage(p);
	p->AddPage(page, ABOUT_PAGE_NAME, false, GetIconIndex(p));

	p->SetSelection(0);
}

// ----------------------------------------------------------------------------
// file system related 
// ----------------------------------------------------------------------------
void mainFrame::AddEntry(wxFSWPathType type, wxString filename)
{
    
    wxCHECK_RET(m_watcher, "Watcher not initialized");

    wxLogDebug("Adding %s: '%s'",
               filename,
               type == wxFSWPath_Dir ? "directory" : "directory tree");

    wxString prefix;
    bool ok = false;

    // This will tell wxFileSystemWatcher whether to dereference symlinks
    wxFileName fn = wxFileName::DirName(filename);
    if (!m_followLinks)
    {
        fn.DontFollowLink();
    }

    switch ( type )
    {
        case wxFSWPath_Dir:
            ok = m_watcher->Add(fn);
            prefix = "Dir:  ";
            break;

        case wxFSWPath_Tree:
            ok = m_watcher->AddTree(fn);
            prefix = "Tree: ";
            break;

        case wxFSWPath_File:
        case wxFSWPath_None:
            wxFAIL_MSG( "Unexpected path type." );
    }

    if (!ok)
    {
        wxLogError("Error adding '%s' to watched paths", filename);
        return;
    }

    //cout << wxFileName::DirName(filename).GetFullPath() << endl;
    // Prepend 'prefix' to the filepath, partly for display
    // but mostly so that OnRemove() can work out the correct way to remove it
    //m_filesList->InsertItem(m_filesList->GetItemCount(), prefix + wxFileName::DirName(filename).GetFullPath());
}

void mainFrame::OnFileSystemEvent(wxFileSystemWatcherEvent& event)
{
    // TODO remove when code is rock-solid
    //wxLogTrace(wxTRACE_FSWATCHER, "*** %s ***", event.ToString());
    LogEvent(event);
}

void mainFrame::addEventRecord(const wxFileSystemWatcherEvent& event)
{
	Action act;
	act.actType = event.GetChangeType();
	act.fromPath = event.GetPath().GetFullPath();
	act.toPath = event.GetNewPath().GetFullPath();
	m_lastAct = act;
	g_actionsVec.push_back(act);
}
void mainFrame::LogEvent(const wxFileSystemWatcherEvent& event)
{
	int type = event.GetChangeType();
	if (type == 16) return;

	std::map<wxString, BACKUPS>::iterator it = g_tasksMap.begin();
	wxDir dir(it->first);
	wxULongLong dirOldSize = it->second.dirSize;
	wxULongLong dirCurSize = dir.GetTotalSize(it->first);
    int sizeChangedFlag = 15;
	if (dirCurSize != dirOldSize) {
        sizeChangedFlag = 4;
	}
    wxString entry = wxString::Format(LOG_FORMAT,
                            GetFSWEventChangeTypeName(type),
                            event.GetPath().GetFullPath(),
                            event.GetNewPath().GetFullPath());

    fflog(sizeChangedFlag, L"EVENT [% d - % d] : % s\n", dirOldSize, dirCurSize, entry.wx_str());

    m_logPanel->appendLog(event);
	it->second.dirSize = dirCurSize;

    if (sizeChangedFlag == 4) { // need to record
		if (m_lastAct.actType > 0) {
			addEventRecord(event);
		}
		else {
            addEventRecord(event);
		}
    }
    else {
        // some other actions need to record
        if (type == wxFSW_EVENT_CREATE || type == wxFSW_EVENT_DELETE || type == wxFSW_EVENT_RENAME)
        {
            addEventRecord(event);
        }
    }

    
    
}


// ----------------------------------------------------------------------------
// zcTaskBarIcon implementation
// ----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(zcTaskBarIcon, wxTaskBarIcon)
EVT_MENU(PU_RESTORE, zcTaskBarIcon::OnMenuRestore)
EVT_MENU(PU_EXIT, zcTaskBarIcon::OnMenuExit)
EVT_MENU(PU_CHECKMARK, zcTaskBarIcon::OnMenuCheckmark)
EVT_UPDATE_UI(PU_CHECKMARK, zcTaskBarIcon::OnMenuUICheckmark)
EVT_TASKBAR_LEFT_UP(zcTaskBarIcon::OnLeftButtonUp)
EVT_MENU(PU_SUB1, zcTaskBarIcon::OnMenuSub)
EVT_MENU(PU_SUB2, zcTaskBarIcon::OnMenuSub)
wxEND_EVENT_TABLE()

void zcTaskBarIcon::OnMenuRestore(wxCommandEvent&)
{
    gMainFrm->Show(true);
}

void zcTaskBarIcon::OnMenuExit(wxCommandEvent&)
{
    gMainFrm->realCloseWindow();
}

static bool check = true;

void zcTaskBarIcon::OnMenuCheckmark(wxCommandEvent&)
{
	check = !check;
}

void zcTaskBarIcon::OnMenuUICheckmark(wxUpdateUIEvent& event)
{
	event.Check(check);
}

void zcTaskBarIcon::OnMenuSub(wxCommandEvent&)
{
	wxMessageBox("You clicked on a submenu!");
}

// Overridables
wxMenu* zcTaskBarIcon::CreatePopupMenu()
{
#ifdef __WXOSX__
    gMainFrm->Show(true);
    return nullptr;
#else
    wxMenu* menu = new wxMenu;
    menu->Append(PU_RESTORE, "restore main window");
   /* menu->AppendSeparator();
    menu->Append(PU_NEW_ICON, "change new icon");
    menu->AppendSeparator();
    menu->AppendCheckItem(PU_CHECKMARK, "Test &check mark");
    menu->AppendSeparator();
    wxMenu* submenu = new wxMenu;
    submenu->Append(PU_SUB1, "One submenu");
    submenu->AppendSeparator();
    submenu->Append(PU_SUB2, "Another submenu");
    menu->Append(PU_SUBMAIN, "Submenu", submenu);*/
    /* OSX has built-in quit menu for the dock menu, but not for the status item */

#ifdef __WXOSX__
    if (OSXIsStatusItem())
#endif
    menu->AppendSeparator();
    menu->Append(PU_EXIT, "E&xit");

    return menu;

#endif

}

void zcTaskBarIcon::OnLeftButtonUp(wxTaskBarIconEvent&)
{
    gMainFrm->Show(true);
    //gMainFrm->SetWindowStyle(gMainFrm->GetWindowStyle() | wxSTAY_ON_TOP);
    
}
