//
//  logPanel.h
//  FileeliF
//
//  Created by reich on 2023/11/9.
//  Copyright © 2023 reich. All rights reserved.
//

#ifndef logPanel_hpp
#define logPanel_hpp

#include "wx/wx.h"
#include "utils.h"
#include "zcPlayGifControl.h"
#include "wx/artprov.h"
#include "wx/htmllbox.h"
#include "wx/fswatcher.h"
#include "wx/dataview.h"
#include "wx/datetime.h"

class logPanel : public wxPanel
{
public:
    wxWindow* _parent;
    
    logPanel(wxWindow *parent,
                 wxWindowID winid = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize);
    void OnStartStop(wxCommandEvent& evt);

    void OnListValueChanged(wxDataViewEvent& event);
    void appendLog(const wxFileSystemWatcherEvent& event);
public:
    wxBoxSizer* m_syncTopSizer;
    wxDataViewListCtrl* m_DataListCtl;

private:
    // Flag used by OnListValueChanged(), see there.
    bool m_eventFromProgram;
    
    DECLARE_EVENT_TABLE()
};

#endif /* logPanel_hpp */
