//
//  movableFrame.hpp
//  zchat
//
//  Created by reich on 2021/2/22.
//  Copyright © 2021 reich. All rights reserved.
//

#ifndef movableFrame_hpp
#define movableFrame_hpp

#include "wx/wx.h"

class movableFrame : public wxFrame
{
public:
    bool dragging;
    int x,y;
    wxFrame* _parent;
    wxPoint mLastPt;
    
    movableFrame(wxWindow *parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        //long style = wxDEFAULT_FRAME_STYLE,
        long style = wxNO_BORDER,
        const wxString& name = wxASCII_STR(wxFrameNameStr));
    void onMouseDown(wxMouseEvent& evt);
    void onMouseUp(wxMouseEvent& evt);
    void onMove(wxMouseEvent& evt);
    void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
    DECLARE_EVENT_TABLE()
};

#endif /* movableFrame_hpp */
