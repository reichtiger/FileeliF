#ifndef FILEELIF_licenseFrame_H
#define FILEELIF_licenseFrame_H

#include <wx/wx.h>
#include "zcRoundRectPanel.h"

#define LICENSE_WIN_W 500
#define LICENSE_WIN_H 400

class licenseFrame : public wxFrame
{
public:
    licenseFrame(const wxString& title);
    void OnSize(wxSizeEvent& event);
    void OnBtnAccept(wxCommandEvent& event);
    void OnBtnRefuse(wxCommandEvent& event);
private:
    DECLARE_EVENT_TABLE()
};

#endif //FILEELIF_licenseFrame_H
