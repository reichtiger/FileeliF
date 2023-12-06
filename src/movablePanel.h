//
//  movablePanel.hpp
//  zchat
//
//  Created by reich on 2021/2/24.
//  Copyright © 2021 reich. All rights reserved.
//

#ifndef movablePanel_hpp
#define movablePanel_hpp

#include "wx/wx.h"

class movablePanel : public wxPanel
{
public:
    bool dragging;
    int x,y;
    wxPanel* _parent;
    wxPoint mLastPt;
    wxWindow* outer_frame_be_moving;
    
    movablePanel(wxWindow* movFrame,
                 wxWindow *parent,
                 wxWindowID winid = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize);
    
    void onMouseDown(wxMouseEvent& evt);
    void onMouseUp(wxMouseEvent& evt);
    void onMove(wxMouseEvent& evt);
    void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
    
    DECLARE_EVENT_TABLE()
};

#endif /* movablePanel_hpp */
