//
//  zcPlayGifConrol.hpp
//  FileeliF
//
//  Created by hzhao on 2023/2/10.
//  Copyright © 2023 hzhao. All rights reserved.
//

#ifndef zcPlayGifConrol_h
#define zcPlayGifConrol_h

#include "wx/wx.h"
using namespace std;

class zcPlayGifConrol : public wxControl
{
	DECLARE_DYNAMIC_CLASS(zcPlayGifConrol)
	DECLARE_EVENT_TABLE()
public:
	zcPlayGifConrol();
	virtual ~zcPlayGifConrol();

	zcPlayGifConrol(wxWindow* parent, wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxBORDER_NONE,
		const wxValidator& validator = wxDefaultValidator);
	bool Create(wxWindow* parent, wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxSUNKEN_BORDER,
		const wxValidator& validator = wxDefaultValidator);

	wxBitmap* normal_bitmap_;

	void OnPaint(wxPaintEvent& event);
};


// ----------------------------------------------
// self textCtrl
// ----------------------------------------------

class TextCtlWithId : public wxTextCtrl
{
	DECLARE_DYNAMIC_CLASS(TextCtlWithId)
	DECLARE_EVENT_TABLE()
public:
	TextCtlWithId() {}
	TextCtlWithId(wxWindow* parent,
		wxWindowID id,
		const wxString& value = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxASCII_STR(wxTextCtrlNameStr)
	) :wxTextCtrl( parent,
		 id,
		 value,
		pos,
		size,
		 style,
		 validator ,
		 name)
	{

	}
	virtual ~TextCtlWithId() {
		cout << "deleting TextctlWithId " << this << endl;
	}

public:
	int typeId; // 0-3, 0 = src dir , 1-3 = target
};
#endif /* zcPlayGifConrol_hpp */