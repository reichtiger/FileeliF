//////////////////////////////////////////////////////////////////////////
// This file and class is for some test code about wxWidgets 
// for example , dynamically created textCtrl and buttons connect with event
// on different platform (win , Linux, mac)
// 
//////////////////////////////////////////////////////////////////////////

#ifndef FILEELIF_testFrame_H
#define FILEELIF_testFrame_H

#include "wx/wx.h"
#include "zcPlayGifControl.h"

class testFrame :
    public wxFrame
{
public:
	testFrame(const wxString& title);
	void OnSize(wxSizeEvent& event);
	void OnAddTargetDir(wxCommandEvent& event);
	void OnText(wxCommandEvent& event);
	void OnCloseWindow(wxCloseEvent& event);
private:
	int custom_new_id;
	TextCtlWithId* dirTarget;
	wxDECLARE_EVENT_TABLE();
};

#endif //FILEELIF_testFrame_H