/*****************************************************
* Name:				ScrolledWindowWithSizer.cpp
* Purpose:		To demonstrate an example of how to use
*							wxScrolledWindow within projects.
* Author:			Michael McCarty
* Created:		2009-04-14
* Copyright:
* License:		Not Applicable
*****************************************************/

#include <wx/wx.h>

//helper functions
enum wxbuildinfoformat { short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}


class ScrolledWindowWithSizerApp: public wxApp
{
		public:
				virtual bool OnInit();
};

class ScrolledWindowWithSizerDialog: public wxDialog
{
    public:
        ScrolledWindowWithSizerDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~ScrolledWindowWithSizerDialog();

    private:
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnOkButtonClicked(wxCommandEvent& event);
        void OnAddButtonClicked(wxCommandEvent& event);
        void OnRowButtonClicked(wxCommandEvent& event);

        wxSize tcSize;
        wxSize buttonSize;
        wxSize stSize;
        wxSize swSize;
        wxFlexGridSizer* m_swSizer;
        wxScrolledWindow* m_sw;

        unsigned int counter;

        DECLARE_EVENT_TABLE()
};

//IMPLEMENT_APP(ScrolledWindowWithSizerApp);

bool ScrolledWindowWithSizerApp::OnInit()
{
    bool wxsOK = true;
    wxInitAllImageHandlers();

    if ( wxsOK )
    {
				ScrolledWindowWithSizerDialog Dlg(0);
				SetTopWindow(&Dlg);
				Dlg.ShowModal();
				wxsOK = false;
    }

    return wxsOK;
}

BEGIN_EVENT_TABLE(ScrolledWindowWithSizerDialog,wxDialog)
END_EVENT_TABLE()

ScrolledWindowWithSizerDialog::ScrolledWindowWithSizerDialog(wxWindow* parent,wxWindowID id)
{
		counter = 0;
		tcSize = wxSize(150, -1);
		buttonSize = wxSize(200, -1);
		stSize = wxSize(75, -1);
		swSize = wxSize(-1, 300);
    Create(parent, id, _("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* titleSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* titleST = new wxStaticText(this, wxNewId(), wxT("This is the title section of the dialog"));
    titleSizer->Add(titleST, 0, wxALL|wxCENTER, 5, NULL);

		wxBoxSizer* middleSizer = new wxBoxSizer(wxVERTICAL);

		m_sw = new wxScrolledWindow(this, wxNewId(), wxDefaultPosition, wxDefaultSize, wxVSCROLL);
		m_sw->SetMinSize(wxSize(-1, 200));

		m_swSizer = new wxFlexGridSizer(0, 3, 5, 5);
		m_sw->SetSizer(m_swSizer);

		middleSizer->Add(m_sw, 0, wxALL|wxCENTER|wxGROW, 5, NULL);

		wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		wxButton* OkButton = new wxButton(this, wxID_OK, wxT("OK"));
		OkButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ScrolledWindowWithSizerDialog::OnOkButtonClicked, NULL, this);
		wxButton* AddButton = new wxButton(this, wxNewId(), wxT("Add Row"));
		AddButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ScrolledWindowWithSizerDialog::OnAddButtonClicked, NULL, this);
		buttonSizer->Add(OkButton, 0, wxALL, 5, NULL);
		buttonSizer->Add(AddButton, 0, wxALL, 5, NULL);

		topSizer->Add(titleSizer, 0, wxALL|wxCENTER, 5, NULL);
		topSizer->Add(middleSizer, 0, wxALL|wxCENTER, 5, NULL);
		topSizer->Add(buttonSizer, 0, wxALL|wxCENTER, 5, NULL);

    SetSizer(topSizer);
    topSizer->Fit(this);
    topSizer->SetSizeHints(this);
    Center();
}

ScrolledWindowWithSizerDialog::~ScrolledWindowWithSizerDialog()
{
}

void ScrolledWindowWithSizerDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void ScrolledWindowWithSizerDialog::OnOkButtonClicked(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}

void ScrolledWindowWithSizerDialog::OnAddButtonClicked(wxCommandEvent& event)
{
	counter++;
	wxStaticText* st = new wxStaticText(m_sw, wxID_ANY, wxString::Format("Row %u", counter), wxDefaultPosition, stSize);
	wxTextCtrl* tc = new wxTextCtrl(m_sw, wxID_ANY, wxString::Format("Text in Row %u", counter), wxDefaultPosition, tcSize);
	wxButton* btn = new wxButton(m_sw, counter, wxString::Format("Row %u Button", counter));
	btn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ScrolledWindowWithSizerDialog::OnRowButtonClicked, NULL, this);
	m_sw->GetSizer()->Add(st, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5, NULL);
	m_sw->GetSizer()->Add(tc, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5, NULL);
	m_sw->GetSizer()->Add(btn, 0, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5, NULL);

	// the + 5 in (btn->GetSize().GetHeight() + 5) accounts for the space when creating
	// m_swSizer = new wxFlexGridSizer(0, 3, 5, 5); on the third parameter.
	// The + N should reflect the gaps given for the sizer.  Why it is not + 10, I do not know.  It would make
	// sense since the gap is for both the vertical sides.
	m_sw->SetScrollbars(0, (btn->GetSize().GetHeight() + 5), 0, ((btn->GetSize().GetHeight()) + 5)*counter, 0, 0);

	// For this example, this was the minimum necessary for a clean refresh.
	this->GetSizer()->SetSizeHints(this);
	Layout();
}

void ScrolledWindowWithSizerDialog::OnRowButtonClicked(wxCommandEvent& event)
{
	wxMessageBox(wxString::Format("Row Button %d clicked", event.GetId()));
}
