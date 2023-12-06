//
//  movableFrame.cpp
//  zchat
//
//  Created by reich on 2021/2/22.
//  Copyright © 2021 reich. All rights reserved.
//

#include "movableFrame.h"

BEGIN_EVENT_TABLE(movableFrame, wxFrame)
EVT_LEFT_DOWN(movableFrame::onMouseDown)
EVT_LEFT_UP(movableFrame::onMouseUp)
EVT_MOTION(movableFrame::onMove)
EVT_MOUSE_CAPTURE_LOST(movableFrame::OnMouseCaptureLost)
END_EVENT_TABLE()


movableFrame::movableFrame(wxWindow *parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxString& name)
        : wxFrame(parent, id, title, pos, size, style, name)
{
    movableFrame::_parent = NULL;
    dragging = false;
}

void movableFrame::onMouseDown(wxMouseEvent& evt)
{
    this->CaptureMouse();
    x = evt.GetX();
    y = evt.GetY();
    dragging=true;
    //evt.Veto();
    mLastPt.x = x;
    mLastPt.y = y;
    printf("movableFrame : mouse down\n");
}
void movableFrame::onMouseUp(wxMouseEvent& evt)
{
    dragging=false;
    if (HasCapture()){
        ReleaseMouse();
        printf("movableFrame : mouse up\n");
    }
}
void movableFrame::onMove(wxMouseEvent& evt)
{
    wxPoint mouseOnScreen = wxGetMousePosition();
    int newx = mouseOnScreen.x - x;
    int newy = mouseOnScreen.y - y;
    printf("movableFrame : mouse moving %d, %d\n", newx, newy);
    
    
    if(dragging)
    {
        
        wxPoint pt = evt.GetPosition();
        wxPoint wndLeftTopPt = GetPosition();
        int distanceX = pt.x - mLastPt.x;
        int distanceY = pt.y - mLastPt.y;
          
        wxPoint desPt;
        desPt.x = distanceX + wndLeftTopPt.x;
        desPt.y = distanceY + wndLeftTopPt.y;
        this->Move(desPt);
    }
}

void movableFrame::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    printf("movableFrame : mouse capture lost\n");
    if (HasCapture())
        ReleaseMouse();
}
