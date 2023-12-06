/////////////////////////////////////////////////////////////////////////////
// Name:        samples/vscroll/vstest.cpp
// Purpose:     VScroll wxWidgets sample
// Author:      Vadim Zeitlin
// Modified by: Brad Anderson
// Created:     04/01/98
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
    #include "wx/app.h"
    #include "wx/frame.h"
#endif

// we need to include the headers not included from wx/wx.h explicitly anyhow
#include "wx/vscroll.h"
#include "wx/scrolwin.h"
// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows it is in resources)
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "sample.xpm"
#endif

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

#include "zcRoundRectPanel.h"

// new app
class MyApp2 : public wxApp
{
public:
    // create our main window
    virtual bool OnInit() wxOVERRIDE;
};


class testFrame : public wxFrame
{
public:
    testFrame():wxFrame(NULL, wxID_ANY, "", wxDefaultPosition, wxSize(800, 800))
    {
        wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
        
            wxBoxSizer* middleSizer = new wxBoxSizer(wxVERTICAL);

            mcp = new MainChatPanel(this);
            middleSizer->Add(mcp, 1, wxEXPAND|wxALL, 0, NULL);
            
            wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
        
            wxButton* AddButton = new wxButton(this, wxNewId(), wxT("Add Row"));
            AddButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&testFrame::OnAddRow, NULL, this);
            btnSizer->Add(AddButton, 0, wxALL , 5);
        
        topSizer->Add(middleSizer, 1, wxEXPAND|wxALL, 0, NULL);
        topSizer->Add(btnSizer, 0, wxALIGN_RIGHT, 5, NULL);

        //topSizer->SetSizeHints(this);
        SetSizerAndFit(topSizer);

        Center();
    }
    
    void OnAddRow(wxCommandEvent& event)
    {
        mcp->addChat(wxT("asdf"));
    }
    
    MainChatPanel* mcp;
    
private:
    DECLARE_EVENT_TABLE()
};
       
BEGIN_EVENT_TABLE(testFrame, wxFrame)

END_EVENT_TABLE()

bool MyApp2::OnInit()
{
    wxInitAllImageHandlers();
    testFrame* f = new testFrame();
    
    f->Show();
    return true;
}

IMPLEMENT_APP(MyApp2);
