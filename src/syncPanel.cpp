//
//  syncPanel.cpp
//  zchat
//
//  Created by reich on 2021/2/24.
//  Copyright © 2021 reich. All rights reserved.
//

#include "syncPanel.h"
#include "mainFrame.h"
#include "utils.h"
#include "logPanel.h"

extern mainFrame* gMainFrm;
extern bool gIsWatching;
extern std::map<wxString, Backups> g_tasksMap;
extern std::vector<Action> g_actionsVec;

class RowPanel : public wxPanel
{
public:
	RowPanel(wxWindow* parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxASCII_STR(wxPanelNameStr)) : wxPanel(parent, winid, pos, size, style, name)
	{
		curId = -1;
		m_transType = -1;
		
	}
	~RowPanel() {};

	void updateRelatedBgColor()
	{
		wxWindowList childs = this->GetChildren();
		for (int i = 0; i < childs.GetCount(); i++)
		{
			wxWindow* w = childs[i];
			if (wxDynamicCast(w, wxButton)) {
				w->SetBackgroundColour(this->GetBackgroundColour());
			}

		}
	}

	void takeOnOffControls(bool onoff)
	{
		wxWindowList childs = this->GetChildren();
		for (int i = 0; i < childs.GetCount(); i++)
		{
			wxWindow* w = childs[i];
			if (wxDynamicCast(w, wxPanel) 
				|| wxDynamicCast(w, wxButton) || wxDynamicCast(w, wxRadioBox) 
				|| wxDynamicCast(w, wxTextCtrl) || wxDynamicCast(w, zcImageBtn)
				|| wxDynamicCast(w, wxCheckBox)
				) {
				w->Enable(onoff);
			}

		}
	}
public:
	int curId;
	int m_transType; // 0 is local addr, 1 is remote sftp, etc
	wxString  m_localAddr;
	wxString  m_sftpAddr;
	wxString  m_sftpPort;
	wxString  m_userName;
	wxString  m_password;
	wxString m_remoteDir;
};

class BoxSizerWithInfo : public wxBoxSizer
{
public:
	BoxSizerWithInfo(int orient) : wxBoxSizer(orient)
	{
		_panel = nullptr;
		_parentSizer = nullptr;
		sid = -1;
		m_orient = orient;
		m_totalProportion = 0;

		wxASSERT_MSG(m_orient == wxHORIZONTAL || m_orient == wxVERTICAL,
			wxT("invalid value for wxBoxSizer orientation"));
	}
	wxPanel* _panel;
	BoxSizerWithInfo* _parentSizer;
	int sid;
};


BEGIN_EVENT_TABLE(syncPanel, wxPanel)

END_EVENT_TABLE()


syncPanel::syncPanel(wxWindow*parent,
        wxWindowID winid,
        const wxPoint& pos,
        const wxSize& size)
        : wxPanel(parent, wxID_ANY, pos, size)
{
    _parent = parent;
	curPath = wxGetCwd();
	// global sizer of page
	wxBoxSizer* SyncSizer = new wxBoxSizer(wxVERTICAL);

	// topsizer of rows
	m_syncTopSizer = new wxBoxSizer(wxVERTICAL);
	//wxGridSizer* gridsizer = new wxGridSizer(3, 4, 3);
	//m_syncTopSizer->AddStretchSpacer();
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* btnStartStop = new wxButton(this, ID_BTN_STARTSTOP, "Start Watching");
	Bind(wxEVT_BUTTON, &syncPanel::OnStartStop, this, ID_BTN_STARTSTOP);
	buttonSizer->Add(btnStartStop, 0,          // 水平方向不可缩放
		wxALL,     // 四周有空白边
		10);      // 空白边框大小

	wxButton* btnAddRow = new wxButton(this, ID_BTN_ADDROW, "Add Watching");
	Bind(wxEVT_BUTTON, &syncPanel::OnAddRow, this, ID_BTN_ADDROW);
	buttonSizer->Add(btnAddRow, 0,          // 水平方向不可缩放
		wxALL,     // 四周有空白边
		10);      // 空白边框大小

	wxButton* btnTest = new wxButton(this, ID_BTN_TEST, "Test");
	Bind(wxEVT_BUTTON, &syncPanel::OnTest, this, ID_BTN_TEST);
	buttonSizer->Add(btnTest, 0,          // 水平方向不可缩放
		wxALL,     // 四周有空白边
		10);      // 空白边框大小
//topSizer->Fit( this );          // 调用对话框大
//topSizer->SetSizeHints( this ); // 设置对话框最小大

	SyncSizer->Add(m_syncTopSizer, 1, wxEXPAND, 5);
	SyncSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5);
	SetSizerAndFit(SyncSizer);

	
	for (int i = 0; i < 3; i++)
	{
		addRow();
	}
	Layout();
	Update();

	
}

void syncPanel::stopWatcher()
{
	gMainFrm->DeleteWatcher();
	gIsWatching = false;
	g_tasksMap.clear();
	wxWindow* w = FindWindowById(ID_BTN_STARTSTOP);
	if (w) {
		w->SetLabel("Start Watching");
	}

	for (int i = 0; i < m_syncTopSizer->GetItemCount(); i++) {
		RowPanel* bs = (RowPanel*)m_syncTopSizer->GetItem(i)->GetWindow();
		if (bs) {
			bs->takeOnOffControls(!gIsWatching);
		}

	}

	wxWindow* w2 = FindWindowById(ID_BTN_ADDROW);
	if (w2) {
		w2->Enable();
	}
}
void syncPanel::OnStartStop(wxCommandEvent& evt)
{

	if (gIsWatching) {
		stopWatcher();
	}
	else {
		
		wxWindow* w = FindWindowById(ID_BTN_STARTSTOP);
		if (w) {
			w->SetLabel("Stop Watching");
		}
		// anyway, here watcher has created or existed
		gIsWatching = true;
		for (int i = 0; i < m_syncTopSizer->GetItemCount(); i++) {
			RowPanel* bs = (RowPanel*)m_syncTopSizer->GetItem(i)->GetWindow();
			if (bs) {
				// update info into vector
				updateInfo2Tasks(bs);
				bs->takeOnOffControls(!gIsWatching);
			}

		}

		wxWindow* w2 = FindWindowById(ID_BTN_ADDROW);
		if (w2) {
			w2->Disable();
		}
		// start multi-threads task

		//// check m_taskMaps;
		/*for (auto row : g_tasksMap)
		{
			for (auto dir : row.second.targetDirsVec) {
				cout << row.first << " ---> " << dir << endl;
			}
		}*/

		// add into watcher
		gMainFrm->CreateWatcher();
		
		// check m_taskMaps;
		for (auto row : g_tasksMap)
		{
			wxDir dir(row.first);
			g_tasksMap[row.first].dirSize = dir.GetTotalSize(row.first);// itor will not change the value
			fflog(14, L"adding dir %s [ %d ]\n", row.first.wx_str(), g_tasksMap[row.first].dirSize);
			cout << "add watching dir ("<< g_tasksMap[row.first].dirSize << ")==>> " << row.first << endl;
			if (!row.first.empty()) {
				gMainFrm->AddEntry(wxFSWPath_Tree, row.first);
			}
		}

	}

}
#include "wx/dataview.h"
void syncPanel::OnTest(wxCommandEvent& evt)
{
	for (auto act : g_actionsVec)
	{
		//cout << act.actType << ": " << act.fromPath << act.toPath << endl;
		fflog(2, L"%d ", act.actType);
		fflog(14, L"%s\t", act.fromPath.wx_str());
		fflog(11, L"%s\n", act.toPath.wx_str());
	}

	for (auto row : g_tasksMap)
	{
		cout << row.first << ", " << row.second.dirSize << endl;
		//cout << "\033[1;31m " << row.first << ", " << row.second.dirSize << "\033[0m\n";
	}

	//gMainFrm->m_logPanel->m_DataListCtl->SetValue(long(99), 3, 4);
	   
	
}
void syncPanel::OnAddRow(wxCommandEvent& evt)
{
	addRow();
	Layout();
	Update();
}
void syncPanel::addRow()
{
	if (m_syncTopSizer->GetItemCount() > 4)  return;
	
	// a panel in bgPanel , leave all blank some width 

	wxBoxSizer* rowSizer = new wxBoxSizer(wxHORIZONTAL);
	RowPanel* rowPanel = new RowPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	int curCount = m_syncTopSizer->GetItemCount();
	rowPanel->SetId(curCount);

	if (curCount % 2 == 0) {
		rowPanel->SetBackgroundColour(wxColor(245, 255, 250, 30));
	}
	else {
		rowPanel->SetBackgroundColour(wxColor(240, 248, 255, 220));
	}
	
	wxString stringnumber = wxString::Format(wxT("%d"), curCount);
	
	wxBoxSizer* colOneSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* srcUnitSizer = new wxBoxSizer(wxHORIZONTAL);

#ifdef __WIN32__
	TextCtlWithId* srcDir = new TextCtlWithId(rowPanel, createNewId(), "E:\\testdir", wxDefaultPosition, wxSize(260, 26));
#endif
#ifdef __LINUX__
	TextCtlWithId* srcDir = new TextCtlWithId(rowPanel, createNewId(), "/data/testdir", wxDefaultPosition, wxSize(260, 26));
#endif
	srcDir->typeId = 0;
	srcDir->SetEditable(false);
	//srcDir->SetBackgroundColour(rowPanel->GetBackgroundColour());
	srcDir->SetHint("src dir...");
	srcDir->SetToolTip("Watching Directory");

	const wxSize imageSize(22, 22);
	wxBitmap bm = wxArtProvider::GetBitmapBundle(wxART_FOLDER, wxART_OTHER, imageSize).GetBitmap(imageSize);
	wxBitmapButton* srcBtn = new wxBitmapButton(rowPanel, createNewId(), bm);
	srcBtn->SetLabel("&Set Watch Dir");
	srcBtn->SetLabelText("Set Watch Dir");
	srcBtn->SetToolTip("click to set original directory");
	srcBtn->SetWindowStyle(srcBtn->GetWindowStyle() & wxCAPTION);
	Bind(wxEVT_BUTTON, &syncPanel::OnAddSrcTree, this, srcBtn->GetId());

	srcUnitSizer->Add(srcBtn, wxSizerFlags(0).Align(wxALIGN_LEFT | wxALIGN_CENTER_HORIZONTAL));
	srcUnitSizer->Add(srcDir, wxSizerFlags(1).Align(wxALIGN_LEFT | wxEXPAND));

	colOneSizer->Add(srcUnitSizer, wxSizerFlags(0).Align(wxALIGN_TOP | wxALIGN_LEFT | wxEXPAND));


	// add radiobox
	wxArrayString* selection = new wxArrayString;
	selection->Add("RealTime Sync");
	selection->Add("Every Hour");
	selection->Add("Every Day at");
	wxRadioBox* radioBox = new wxRadioBox(rowPanel, wxID_ANY, "Select Sync Mode", wxPoint(0, 0),
		wxSize(600, 70), *selection, 1, wxRA_SPECIFY_ROWS);
	radioBox->SetToolTip("select your backup mode");
	radioBox->SetForegroundColour(*wxBLACK);
	colOneSizer->Add(radioBox, wxSizerFlags(0).Align(wxALIGN_TOP | wxALIGN_LEFT));

	// add checkbox for completeSync or AppendSync
	wxCheckBox* chk = new wxCheckBox(rowPanel, wxID_ANY, "When deleting files from the source directory, \nkeep those files with the same name in the backup directory");
	radioBox->SetToolTip("Completely Sync means target dir will remove the existed file when src dir deleted such file");
	chk->SetForegroundColour(*wxBLACK);
	colOneSizer->Add(chk, wxSizerFlags(0).Align(wxALIGN_TOP | wxALIGN_LEFT));

	rowSizer->Add(colOneSizer, wxSizerFlags(1).Align(wxEXPAND & wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL).Border(wxALL, 10));
	rowSizer->AddSpacer(50);
	wxStaticText* t2 = new wxStaticText(rowPanel, wxID_ANY, " Backup to : ");
	t2->SetForegroundColour(*wxBLACK);
	rowSizer->Add(t2, wxSizerFlags(0).Align(wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL));
	// add target sizer

	wxBoxSizer* allbackupSizer = new wxBoxSizer(wxVERTICAL);
	BoxSizerWithInfo* targetsSizer = new BoxSizerWithInfo(wxVERTICAL);
	targetsSizer->_panel = rowPanel;
	

	wxImage png1(curPath + wxT("/resources/btnAdd-26.png"), wxBITMAP_TYPE_PNG);
	wxBitmap* btnAdd = new wxBitmap(png1);
	wxImage png2(curPath + wxT("/resources/btnAdd-26-diable.png"), wxBITMAP_TYPE_PNG);
	wxBitmap* btnDisable = new wxBitmap(png2);
	zcImageBtn* avatarBtn = new zcImageBtn(rowPanel, createNewId(), wxPoint(0, 0), btnAdd->GetSize());
	avatarBtn->sid = curCount;
	avatarBtn->set_normal_bitmap(btnAdd);
	avatarBtn->set_disable_bitmap(btnDisable);
	avatarBtn->belongSizer = targetsSizer;
	Bind(wxEVT_BUTTON, &syncPanel::OnOpenTargetSetWin, this, avatarBtn->GetId());

	allbackupSizer->Add(targetsSizer, wxSizerFlags(0).Align(wxALIGN_TOP | wxEXPAND));
	allbackupSizer->Add(avatarBtn, wxSizerFlags(0).Align(wxALIGN_TOP));
	allbackupSizer->AddStretchSpacer(1);

	rowSizer->Add(allbackupSizer, wxSizerFlags(1).Align(wxALIGN_TOP | wxEXPAND).Border(wxALL, 10));
	// add remove btn
	rowSizer->AddSpacer(30);

	wxBitmap bitmap_remove(wxGetCwd() + wxT("/resources/recycle-30-red.png"), wxBITMAP_TYPE_PNG);
	wxBitmapButton* btnRemove = new wxBitmapButton(rowPanel, createNewId(), bitmap_remove);
	btnRemove->SetLabel("&Delete");
	btnRemove->SetForegroundColour(*wxRED);
	btnRemove->SetToolTip("Remove current row");
	btnRemove->SetWindowStyle(srcBtn->GetWindowStyle() & wxCAPTION);
	rowSizer->Add(btnRemove, wxSizerFlags(0).Align(wxALIGN_CENTER | wxALIGN_CENTER_HORIZONTAL));
	Bind(wxEVT_BUTTON, &syncPanel::OnRemoveRow, this, btnRemove->GetId());
	rowPanel->SetSizerAndFit(rowSizer);

	m_syncTopSizer->Add(rowPanel, wxSizerFlags().Proportion(0).Expand().Border(wxALL, 0));
	//wxSizerItem* item = m_syncTopSizer->Insert(rowSizer->curId, rowSizer, wxSizerFlags().Proportion(0).Expand().Border(wxALL, 10));
}

void syncPanel::OnAddSrcTree(wxCommandEvent& event)
{
	/*m_gridsizer->AddGrowableRow(1);
	Layout();
	return;*/

	wxString pathname;
	if (pathname.empty())
	{
		// TODO account for adding the files as well
		pathname = wxDirSelector("Choose a folder to watch", wxGetCwd(),
			wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
		if (pathname.empty())
			return;
	}
	cout << "cur event id = " << event.GetId() << endl;
	wxWindow* tc = FindWindowById(event.GetId()-1);
	TextCtlWithId* tc2 = (TextCtlWithId*)tc;
	tc2->SetValue(pathname);

	// tc->SetLabel(pathname);// for win
	// tc->Clear();tc->AppendText(pathname);// for linux
	// cout << "lable=" << tc->GetLabel() << endl;
	// tc->GetParent()->Update();
}

void syncPanel::OnOpenTargetSetWin(wxCommandEvent& event)
{
	zcImageBtn* objBtn = (zcImageBtn*)event.GetEventObject();
	// see if more than 3 targets
	BoxSizerWithInfo* targetsSizer = objBtn->belongSizer;
	int targetsNum = targetsSizer->GetItemCount();
	if (targetsSizer->GetItemCount() > 2) {
		return;
	}


	TargetSetDialog* dlg = new TargetSetDialog("Setting target dir ");
	dlg->ShowModal();

	if (dlg->m_canceled)
		return;
	
	// add new target dir
	RowPanel* tarLinePanel = new RowPanel(targetsSizer->_panel);
	tarLinePanel->curId = targetsNum;
	tarLinePanel->m_transType = dlg->radioBox->GetSelection();
	tarLinePanel->SetBackgroundColour(targetsSizer->_panel->GetBackgroundColour());
	BoxSizerWithInfo* tarLineSizer = new BoxSizerWithInfo(wxHORIZONTAL);
	tarLineSizer->_panel = tarLinePanel;
	tarLineSizer->_parentSizer = targetsSizer;

	TextCtlWithId* dirTarget = new TextCtlWithId(tarLinePanel, createNewId(), "", wxDefaultPosition, wxSize(260, 26));
	dirTarget->SetHint("target dir or left blank");
	dirTarget->typeId = targetsNum+1;

	tarLinePanel->m_transType = dlg->radioBox->GetSelection();
	if (tarLinePanel->m_transType == 0) {
		tarLinePanel->m_localAddr = dlg->localAddr->GetValue();
		dirTarget->SetValue(tarLinePanel->m_localAddr);
	}
	else {
		tarLinePanel->m_sftpAddr = dlg->sftpAddr->GetValue();
		tarLinePanel->m_sftpPort = dlg->sftpPort->GetValue();
		tarLinePanel->m_userName = dlg->userName->GetValue();
		tarLinePanel->m_password = dlg->password->GetValue();
		tarLinePanel->m_remoteDir = dlg->userName->GetValue();

		dirTarget->SetValue("sftp://"+tarLinePanel->m_userName+":"+tarLinePanel->m_password + "@" + tarLinePanel->m_sftpAddr + ":" + tarLinePanel->m_sftpPort +"/" + tarLinePanel->m_remoteDir);
	}
	wxImage png1(curPath + wxT("/resources/btnMinus-26.png"), wxBITMAP_TYPE_PNG);
	wxBitmap* btnImg = new wxBitmap(png1);
	zcImageBtn* btnTarget = new zcImageBtn(tarLinePanel, createNewId(), wxPoint(0, 0), btnImg->GetSize());
	btnTarget->set_normal_bitmap(btnImg);
	btnTarget->belongSizer = targetsSizer;
	btnTarget->outPanel = (RowPanel*)tarLinePanel->GetParent();
	btnTarget->sid = targetsNum;
	Bind(wxEVT_BUTTON, &syncPanel::OnRemoveTargetDir, this, btnTarget->GetId());

	/*const wxSize imageSize(22, 22);
	wxBitmap bm = wxArtProvider::GetBitmapBundle(wxART_FOLDER, wxART_OTHER, imageSize).GetBitmap(imageSize);
	wxBitmapButton* btnTarget = new wxBitmapButton(rowPanel, custom_new_id, bm);
	btnTarget->SetLabel("&Set Backup Dir");
	btnTarget->SetWindowStyle(btnTarget->GetWindowStyle() & wxCAPTION);
	Bind(wxEVT_BUTTON, &syncPanel::OnAddTargetDir, this, custom_new_id, -1, dirTarget);*/

	tarLineSizer->Add(dirTarget, wxSizerFlags(1).Align(wxEXPAND & wxALIGN_CENTER | wxALIGN_CENTER_HORIZONTAL));
	tarLineSizer->Add(btnTarget, wxSizerFlags(0).Align(wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL));
	tarLinePanel->SetSizerAndFit(tarLineSizer);
	targetsSizer->Add(tarLinePanel, wxSizerFlags(1).Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxEXPAND));
	
	delete dlg;
	Layout();
}

void syncPanel::OnRemoveTargetDir(wxCommandEvent& evt)
{
	zcImageBtn* objBtn = (zcImageBtn*)(evt.GetEventObject());
	RowPanel* parentPanel = (RowPanel*)objBtn->GetParent();
	BoxSizerWithInfo* targetsSizer = objBtn->belongSizer;
	int ind = parentPanel->curId;
	targetsSizer->Remove(ind);
	RowPanel* rp = objBtn->outPanel;
	rp->RemoveChild(parentPanel);
	objBtn->GetParent()->Destroy();
	int targetsNum = targetsSizer->GetItemCount();
	for (int i = 0; i < targetsNum; i++) {
		RowPanel* rp = (RowPanel*)targetsSizer->GetItem(i);
		if (rp)
			rp->curId = i;
	}
	rp->Refresh();
	targetsSizer->Layout();
	Layout();
}
void syncPanel::OnAddTargetDir(wxCommandEvent& event)
{
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
	//cout << "cur event id = " << event.GetId() << endl;
	//wxWindow* aa = FindWindowById(event.GetId() - 1);
	//aa->SetLabel(pathname);

	TextCtlWithId* tc = (TextCtlWithId*)(event.GetEventUserData());
	tc->SetValue(pathname);
	// tc->SetLabel(pathname);// for win
	// tc->Clear();tc->AppendText(pathname);// for linux
	// cout << "lable=" << tc->GetLabel() << endl;
	// tc->GetParent()->Update();
}

void syncPanel::OnRemoveRow(wxCommandEvent& evt)
{
	cout << "---------------------" << evt.GetId() << endl;

	wxBitmapButton* btn = (wxBitmapButton*)(evt.GetEventObject());
	RowPanel* rowPanel = (RowPanel*)btn->GetParent();
	if (!rowPanel) return;
	int rowIndex = rowPanel->GetId();
	m_syncTopSizer->Remove(rowIndex);
	RemoveChild(rowPanel);
	rowPanel->Destroy();
	m_syncTopSizer->Layout();
	Refresh();
	cout << "cur index = " << rowIndex << ", count = " << m_syncTopSizer->GetItemCount() << endl;
	// resort the index in sizer for later
	for (int i = 0; i < m_syncTopSizer->GetItemCount(); i++) {
		RowPanel* rp = (RowPanel*)m_syncTopSizer->GetItem(i)->GetWindow();
		if (rp) {
			if (i % 2 == 0)
				rp->SetBackgroundColour(wxColor(245, 255, 250, 30));
			else
				rp->SetBackgroundColour(wxColor(240, 248, 255, 220));
			cout << "from " << rp->GetId() << " reset to " << i << endl;
			rp->SetId(i);
		}
		rp->updateRelatedBgColor();
	}
}

/// ---------------------------------------------------------------------------
/// task info update 
/// 
/// ---------------------------------------------------------------------------

void syncPanel::updateInfo2Tasks(RowPanel* rp)
{

	wxWindowList childs = rp->GetChildren();

	wxString srcDir = "";
	Backups bks;
	for (int i = 0; i < childs.GetCount(); i++)
	{
		wxWindow* w = childs[i];
		if (wxDynamicCast(w, TextCtlWithId)) {
			TextCtlWithId* t = (TextCtlWithId*)w;
			t->Update();

			wxString p = t->GetValue();
			wxString path = p.Trim();

			//cout << t->typeId << " = " << path << endl;

			if (path.Length() > 252) return;

			if (t->typeId == 0) {
				srcDir = path;
			}
			else {
				if (!path.compare("")) continue;
				bks.targetDirsVec.push_back(path);
			}
		}

		if (wxDynamicCast(w, RowPanel)) {
			RowPanel* tarLinePanel = (RowPanel*)w;
			if (tarLinePanel->m_transType < 0) continue;

			wxWindowList targets = tarLinePanel->GetChildren();
			for (int i = 0; i < targets.GetCount(); i++)
			{
				wxWindow* w = targets[i];
				if (wxDynamicCast(w, TextCtlWithId)) {
					TextCtlWithId* t = (TextCtlWithId*)w;
					t->Update();

					wxString p = t->GetValue();
					wxString path = p.Trim();

					//cout << tarLinePanel->m_transType << " = " << path << endl;

					if (path.Length() > 252) return;

					if (!path.compare("")) continue;
					bks.targetDirsVec.push_back(path);
				}
			}
		}

		if (wxDynamicCast(w, wxRadioBox)) {
			wxRadioBox* rb = (wxRadioBox*)w;
			//cout << rb->GetString(rb->GetSelection()) << endl;
			bks.eSyncPeriod = (SyncPeriod)rb->GetSelection();
		}
	}

	if (!srcDir.compare(""))
		return;
	auto iter = g_tasksMap.find(srcDir);
	if (iter == g_tasksMap.end())
	{
		g_tasksMap.insert(make_pair(srcDir, bks));
	}

    
}

// ----------------------------------------------------------------------------
// test dialog
// ----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(TargetSetDialog, wxDialog)
//EVT_RADIOBOX(RADIO_TARGET_TYPE, TargetSetDialog::onRadioChanged)
wxEND_EVENT_TABLE()

TargetSetDialog::~TargetSetDialog()
{
	cout << "deleting TargetSetDialog .." << endl;
	
}
TargetSetDialog::TargetSetDialog(const wxString& title)
	:wxDialog(NULL, wxID_ANY, title, wxPoint(220, 220), wxSize(600, 500))
{
	wxPanel* p = new wxPanel(this, -1);
	m_canceled = false;
	Bind(wxEVT_CLOSE_WINDOW, &TargetSetDialog::OnCloseWindow, this);
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

	// add radiobox
	wxArrayString* selection = new wxArrayString;
	selection->Add("Local Path");
	selection->Add("Remote SFTP Path");
	radioBox = new wxRadioBox(p, RADIO_TARGET_TYPE, "Select Path Type", wxDefaultPosition, wxDefaultSize, *selection, 1, wxRA_SPECIFY_ROWS);
	radioBox->SetToolTip("select backup target type");
	radioBox->SetForegroundColour(*wxBLACK);
	radioBox->Bind(wxEVT_RADIOBOX, &TargetSetDialog::onRadioChanged, this, RADIO_TARGET_TYPE);

	topSizer->Add(radioBox, wxSizerFlags(0).Align(wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL | wxEXPAND).Border(10));
	wxGridSizer* grid_sizer = new wxGridSizer(2,10,10);
	label_lp = new wxStaticText(p, wxID_ANY, "Local Path :");
	label_rp = new wxStaticText(p, wxID_ANY, "SFTP Path :");
	localAddr = new wxTextCtrl(p, wxID_ANY, "C:\\");
	sftpAddr = new wxTextCtrl(p, wxID_ANY, ""); sftpAddr->SetHint("127.0.0.1");
	labelPort = new wxStaticText(p, wxID_ANY, "Port:" );
	sftpPort = new wxTextCtrl(p, wxID_ANY, "1234", wxDefaultPosition, wxSize(100, 30));
	lable_username = new wxStaticText(p, wxID_ANY, "Username:");
	userName = new wxTextCtrl(p, wxID_ANY, "abc", wxPoint(100, 180), wxSize(80, 30));
	label_pwd = new wxStaticText(p, wxID_ANY, "Password:");
	password = new wxTextCtrl(p, wxID_ANY, "mypassword", wxPoint(100, 180), wxSize(80, 30), wxTE_PASSWORD);

	grid_sizer->Add(label_lp, wxSizerFlags(0).Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL ).Border(5));  
	grid_sizer->Add(localAddr, wxSizerFlags(1).Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxEXPAND));
	grid_sizer->Add(label_rp, wxSizerFlags(0).Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL ));
	grid_sizer->Add(sftpAddr, wxSizerFlags(1).Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxEXPAND));
	grid_sizer->Add(labelPort, wxSizerFlags(0).Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL ));
	grid_sizer->Add(sftpPort, wxSizerFlags(0).Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxEXPAND));
	grid_sizer->Add(lable_username, wxSizerFlags(0).Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL ));
	grid_sizer->Add(userName, wxSizerFlags(1).Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxEXPAND));
	grid_sizer->Add(label_pwd, wxSizerFlags(0).Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL ));
	grid_sizer->Add(password, wxSizerFlags(1).Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxEXPAND));

	topSizer->Add(grid_sizer, wxSizerFlags(1).Align(wxALIGN_LEFT | wxEXPAND).Border(5));

	wxButton* okButton = new wxButton(p, wxID_OK, "OK", wxDefaultPosition, wxSize(100, 30));
	topSizer->Add(okButton, wxSizerFlags(0).Align(wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL));
	topSizer->AddSpacer(10);
	p->SetSizerAndFit(topSizer);
	Layout();

	// init status
	label_lp->Enable(true);
	localAddr->Enable(true);
	label_rp->Enable(false);
	sftpAddr->Enable(false);
	labelPort->Enable(false);
	sftpPort->Enable(false);
	lable_username->Enable(false);
	userName->Enable(false);
	label_pwd->Enable(false);
	password->Enable(false);
}

void TargetSetDialog::OnCloseWindow(wxCloseEvent& event)
{
	cout << "closing" << endl;
	m_canceled = true;
	Destroy();
}

void TargetSetDialog::onRadioChanged(wxCommandEvent& event)
{
	cout << radioBox->GetSelection() << endl;
	switch (radioBox->GetSelection())
	{
	case 0:
	{
		label_lp->Enable(true);
		localAddr->Enable(true);
		label_rp->Enable(false);
		sftpAddr->Enable(false);
		labelPort->Enable(false);
		sftpPort->Enable(false);
		lable_username->Enable(false);
		userName->Enable(false);
		label_pwd->Enable(false);
		password->Enable(false);
	}break;
	case 1:
	{
		label_lp->Enable(false);
		localAddr->SetValue(""); localAddr->Enable(false);
		label_rp->Enable(true);
		sftpAddr->Enable(true);
		labelPort->Enable(true);
		sftpPort->Enable(true);
		lable_username->Enable(true);
		userName->Enable(true);
		label_pwd->Enable(true);
		password->Enable(true);
	}break;
	default:
		break;
	}
}