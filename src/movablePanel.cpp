//
//  movablePanel.cpp
//  zchat
//
//  Created by reich on 2021/2/24.
//  Copyright © 2021 reich. All rights reserved.
//

#include "movablePanel.h"

BEGIN_EVENT_TABLE(movablePanel, wxPanel)
EVT_LEFT_DOWN(movablePanel::onMouseDown)
EVT_LEFT_UP(movablePanel::onMouseUp)
EVT_MOTION(movablePanel::onMove)
EVT_MOUSE_CAPTURE_LOST(movablePanel::OnMouseCaptureLost)
END_EVENT_TABLE()


movablePanel::movablePanel(wxWindow* movFrame,
                           wxWindow *parent,
        wxWindowID winid,
        const wxPoint& pos,
        const wxSize& size)
        : wxPanel(parent, wxID_ANY, pos, size)
{
    movablePanel::_parent = NULL;
    outer_frame_be_moving = movFrame;
    dragging = false;
    
}

void movablePanel::onMouseDown(wxMouseEvent& evt)
{
    this->CaptureMouse();
    x = evt.GetX();
    y = evt.GetY();
    dragging=true;
    //evt.Veto();
    mLastPt.x = x;
    mLastPt.y = y;
    printf("movablePanel : mouse down\n");
}
void movablePanel::onMouseUp(wxMouseEvent& evt)
{
    dragging=false;
    if (HasCapture()){
        ReleaseMouse();
        printf("movablePanel : mouse up\n");
    }
}
void movablePanel::onMove(wxMouseEvent& evt)
{
    if(dragging)
    {
        wxPoint mouseOnScreen = wxGetMousePosition();
        int newx = mouseOnScreen.x - x;
        int newy = mouseOnScreen.y - y;
        //printf("movablePanel : mouse moving %d, %d\n", newx, newy);
        
        wxPoint pt = evt.GetPosition();
        wxPoint wndLeftTopPt = outer_frame_be_moving->GetPosition();
        int distanceX = pt.x - mLastPt.x;
        int distanceY = pt.y - mLastPt.y;
          
        wxPoint desPt;
        desPt.x = distanceX + wndLeftTopPt.x;
        desPt.y = distanceY + wndLeftTopPt.y;
        outer_frame_be_moving->Move(desPt);
    }
}

void movablePanel::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    printf("movablePanel : mouse capture lost\n");
    if (HasCapture())
        ReleaseMouse();
}
