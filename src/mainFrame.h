//
//  mainFrame.hpp
//  zchat
//
//  Created by reich on 2021/2/19.
//  Copyright © 2021 reich. All rights reserved.
//
/*
  mv file from dir1 -> dir2 , event is DELETE/CREATE , create is immediately happen
  delete dir tree, the delete of dir root is on the end.

 */

#ifndef zchat_mainFrame_h
#define zchat_mainFrame_h

#include "wx/wx.h"
#include <wx/log.h>
#include "wx/taskbar.h"
#include <wx/splitter.h>
#include <wx/msgdlg.h>
#include <wx/filefn.h>
#include <wx/vector.h>

#include "wx/image.h"
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/mstream.h"
#include "wx/wfstream.h"

#include "wx/notebook.h"
#include "wx/listbook.h"
#include "licenseFrame.h"
#include "movableFrame.h"
#include "movablePanel.h"

#include "zcImageBtn.h"
#include "zcPlayGifControl.h"
#include "wx/vscroll.h"

#include "wx/fswatcher.h"
#include "wx/listctrl.h"
#include "wx/cmdline.h"
#include "wx/imaglist.h"
#include "wx/artprov.h"
#include "wx/cshelp.h"
#include "wx/utils.h"

#include <map>
#include <vector>

class mainFrame;
class RowPanel;
class logPanel;

enum SyncPeriod
{
	E_SYNC_REALTIME,
	E_SYNC_PERHOUR,
	E_SYNC_SECSOFDAY
};
typedef struct Backups
{
	std::vector<wxString> targetDirsVec;
	wxULongLong		dirSize;
	SyncPeriod		eSyncPeriod;
	bool			bSyncMode;// 只增量同步0，还是完全同步1(删除也跟)
	unsigned long	iSecsOfDay;
	Backups()
	{
		eSyncPeriod = SyncPeriod::E_SYNC_REALTIME;
		bSyncMode = true;
		iSecsOfDay = 0;
		dirSize = 0;
		targetDirsVec.clear();

	}
}BACKUPS;


#define SETTING_PAGE_NAME			"Setting"
#define ACCOUNT_PAGE_NAME           "Account"
#define LOGS_PAGE_NAME          	"Logs"
#define ABOUT_PAGE_NAME             "About"


class MyThread : public wxThread
{
public:
	MyThread(mainFrame* handler)
		: wxThread(wxTHREAD_DETACHED)
	{
		m_pHandler = handler;
	}

	~MyThread();
protected:
	virtual ExitCode Entry();
	mainFrame* m_pHandler;

};
class zcTaskBarIcon : public wxTaskBarIcon
{
public:
#if defined(__WXOSX__) && wxOSX_USE_COCOA
	zcTaskBarIcon(wxFrame* frame, wxTaskBarIconType iconType = wxTBI_DEFAULT_TYPE)
		: wxTaskBarIcon(iconType) {
}
#else
	zcTaskBarIcon(wxFrame* frame) { }
#endif
	void OnLeftButtonUp(wxTaskBarIconEvent&);
	void OnMenuRestore(wxCommandEvent&);
	void OnMenuExit(wxCommandEvent&);
	void OnMenuSetNewIcon(wxCommandEvent&);
	void OnMenuCheckmark(wxCommandEvent&);
	void OnMenuUICheckmark(wxUpdateUIEvent&);
	void OnMenuSub(wxCommandEvent&);
	virtual wxMenu* CreatePopupMenu() wxOVERRIDE;
	wxDECLARE_EVENT_TABLE();
};

// ----------------------------------------------------------------------------
// zcTaskBarIcon declaration
// ----------------------------------------------------------------------------

wxDECLARE_EVENT(MY_EVENT, wxCommandEvent);

/**********************************************************************************

 mainFrame declaration

***********************************************************************************/

class mainFrame : public wxFrame
{
public:
	mainFrame(const wxString& title);
	~mainFrame();
	void OnBtnAVATAR(wxCommandEvent& event);

	zcTaskBarIcon* m_taskBarIcon;

	licenseFrame* loginFrm;
	wxSplitterWindow* splitterTop;
	wxString curPath;
	wxString account;
	wxString passwd;
	wxBitmap bg;
    movablePanel* pp;
	void OnClickBitmapBtn(wxCommandEvent&);
	zcImageBtn* avatarBtn;
	wxBitmap GetPositionBackgroundBitmap(int x, int y, int width, int height);
	void OnSize(wxSizeEvent& event);
	void onSplitTopSashPosChanged(wxSplitterEvent& event);
	void OnCloseWindow(wxCloseEvent& event);
	void realCloseWindow();
	wxPanel* m_syncPanel;
	logPanel* m_logPanel;

	wxBookCtrlBase *m_bookCtrl;
	wxBookCtrlBase::Images m_images;
	// file system related
	wxTextCtrl *m_evtConsole;         // events console
    wxListView *m_filesList;          // list of watched paths
    wxFileSystemWatcher* m_watcher;   // file system watcher
    bool m_followLinks;               // should symlinks be dereferenced
    const static wxString LOG_FORMAT; // how to format events
	wxGridSizer* m_gridsizer;
	wxBoxSizer* m_syncTopSizer;
	void CreateInitialPages(wxBookCtrlBase* parent);
	wxPanel* CreateSyncPage(wxBookCtrlBase* parent);
	wxWindow* CreateAccountPage(wxBookCtrlBase* parent);
	wxWindow* CreateLogsPage(wxBookCtrlBase* parent);
	wxWindow* CreateAboutPage(wxBookCtrlBase* parent);

	int GetIconIndex(wxBookCtrlBase* bookCtrl);
	
	// Add an entry of the specified type asking the user for the filename if
    // the one passed to this function is empty.
    void AddEntry(wxFSWPathType type, wxString filename = wxString());
	void addEventRecord(const wxFileSystemWatcherEvent& event);

	wxMutex m_mutex;
	void updateTaskList();
	Action m_lastAct;
	void OnFileSystemEvent(wxFileSystemWatcherEvent& event);
	void LogEvent(const wxFileSystemWatcherEvent& event);
	
	bool CreateWatcher();
	void DeleteWatcher();
private:
	DECLARE_EVENT_TABLE()
};

#endif /* zchat_mainFrame_h */
