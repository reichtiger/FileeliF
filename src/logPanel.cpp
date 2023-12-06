//
//  logPanel.cpp
//  zchat
//
//  Created by reich on 2021/2/24.
//  Copyright © 2021 reich. All rights reserved.
//

#include "logPanel.h"
#include "mainFrame.h"

extern mainFrame* gMainFrm;
extern int custom_new_id;
extern bool gIsWatching;
extern std::map<wxString, Backups> g_tasksMap;

BEGIN_EVENT_TABLE(logPanel, wxPanel)

END_EVENT_TABLE()


logPanel::logPanel(wxWindow*parent,
        wxWindowID winid,
        const wxPoint& pos,
        const wxSize& size)
        : wxPanel(parent, wxID_ANY, pos, size)
{
    _parent = parent;
	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);

	m_DataListCtl = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, 0 );
            
    // MyListStoreDerivedModel* page2_model = new MyListStoreDerivedModel();
    // lc->AssociateModel(page2_model);
    // page2_model->DecRef();

    m_DataListCtl->AppendTextColumn( "Index" );

    // We're not limited to convenience column-appending functions, it
    // can also be done fully manually, which allows us to customize
    // the renderer being used.
    /*wxDataViewToggleRenderer* const rendererRadio =
        new wxDataViewToggleRenderer("bool", wxDATAVIEW_CELL_ACTIVATABLE);
    rendererRadio->ShowAsRadio();
    wxDataViewColumn* const colRadio =
        new wxDataViewColumn("Action", rendererRadio, 1);
    m_DataListCtl->AppendColumn(colRadio, "bool");*/

    m_DataListCtl->AppendTextColumn("Action")->SetMinWidth(FromDIP(80));
    m_DataListCtl->AppendTextColumn( "Watcher Path" )->SetMinWidth(FromDIP(200));
	m_DataListCtl->AppendTextColumn( "Backup Path" )->SetMinWidth(FromDIP(200));
    m_DataListCtl->AppendProgressColumn( "Progress" )->SetMinWidth(FromDIP(100));

    wxVector<wxVariant> data;
    for (unsigned int i = 0; i < 10; i++)
    {
        data.clear();
        data.push_back( wxString::Format("%d",long(i+1)));
		data.push_back( wxString("TEST"));
        data.push_back( wxString("From path")); // select a single (random) radio item
        data.push_back( wxString::Format("row %d", i) );
        data.push_back( long(5*i) );

        m_DataListCtl->AppendItem( data );
    }

    m_DataListCtl->Bind(wxEVT_DATAVIEW_ITEM_VALUE_CHANGED, &logPanel::OnListValueChanged, this);

	topSizer->Add(m_DataListCtl, wxSizerFlags(1).Expand().Border(0));

    m_DataListCtl->SetValue(wxString("a"), 3, 3);
	SetSizerAndFit(topSizer);
	Layout();
}

void logPanel::OnListValueChanged(wxDataViewEvent& event)
{
    // Ignore changes coming from our own SetToggleValue() calls below.
	if (m_eventFromProgram)
	{
		m_eventFromProgram = false;
		return;
	}

    const int columnToggle = 1;
    
    // Handle selecting a radio button by unselecting all the other ones.
    //if ( event.GetColumn() == columnToggle )
    //{
    //    const int rowChanged = m_DataListCtl->ItemToRow(event.GetItem());
    //    if ( m_DataListCtl->GetToggleValue(rowChanged, columnToggle) )
    //    {
    //        for ( int row = 0; row < m_DataListCtl->GetItemCount(); ++row )
    //        {
    //            if ( row != rowChanged )
    //            {
    //                m_eventFromProgram = true;
    //                m_DataListCtl->SetToggleValue(false, row, columnToggle);
    //            }
    //        }
    //    }
    //    else // The item was cleared.
    //    {
    //        // Explicitly check it back, we want to always have exactly one
    //        // checked radio item in this column.
    //        m_eventFromProgram = true;
    //        m_DataListCtl->SetToggleValue(true, rowChanged, columnToggle);
    //    }
    //}
}


void logPanel::appendLog(const wxFileSystemWatcherEvent& event)
{
    for (auto row : g_tasksMap)
    {
        wxString srcPath = event.GetPath().GetFullPath();

        int pos = srcPath.find(row.first);
        if (pos == 0) {
			for (auto tarDir : row.second.targetDirsVec)
			{
				wxVector<wxVariant> data;
				data.clear();
				data.push_back(long(m_DataListCtl->GetItemCount()));
				data.push_back(GetFSWEventChangeTypeName(event.GetChangeType()));
				data.push_back(event.GetPath().GetFullPath());
				data.push_back(tarDir);
				data.push_back(long(99));

				m_DataListCtl->AppendItem(data);
			}
        }
    }
	
	
}

