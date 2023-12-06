//
//  utils.hpp
//  zchat
//
//  Created by reich on 2021/3/11.
//  Copyright © 2021 reich. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#include "wx/wx.h"
#include <string>
#include <iostream>
#include "wx/fswatcher.h"

using namespace std;

#ifdef WIN32
/*打印彩色字
0=黑色   1=蓝色
2=绿色   3=湖蓝色
4=红色   5=紫色
6=黄色   7=白色
8=灰色   9=淡蓝色
10=淡绿色 11=淡浅绿色
12=淡红色 13=淡紫色
14=淡黄色 15=亮白色*/
//ForeColor 字体颜色 BackColor 字体背景颜色
void SetColor(int ForeColor, int BackColor);
//封装一下，方便调用，T表示任何可以被cout输出的类型
template<typename T>
void CoutColorString(T t);
#endif // WIN32
void fflog(int color, wchar_t* format, ...);
void fflog_warn(wchar_t* format, ...);
void fflog_err(wchar_t* format, ...);

static 
wxString GetFSWEventChangeTypeName(int changeType)
{
	switch (changeType)
	{
	case wxFSW_EVENT_CREATE:
		return "CREATE";
	case wxFSW_EVENT_DELETE:
		return "DELETE";
	case wxFSW_EVENT_RENAME:
		return "RENAME";
	case wxFSW_EVENT_MODIFY:
		return "MODIFY";
	case wxFSW_EVENT_ACCESS:
		return "ACCESS";
	case wxFSW_EVENT_ATTRIB:  // Currently this is wxGTK-only
		return "ATTRIBUTE";
#ifdef wxHAS_INOTIFY
	case wxFSW_EVENT_UNMOUNT: // Currently this is wxGTK-only
		return "UNMOUNT";
#endif
	case wxFSW_EVENT_WARNING:
		return "WARNING";
	case wxFSW_EVENT_ERROR:
		return "ERROR";
	}

	return "INVALID_TYPE";
}
enum
{
	ID_OPEN = wxID_HIGHEST + 1,
	ID_SAVE,
	ID_TEXT,
	ID_AVATAR, //avatar ID
	ID_LEFT_SPLITTER,
	ID_SPLITTER_TOP,
	ID_SPLITTER_BOTTOM,
	ID_TEST_BITMAP_BTN,
	ID_INPUTTEXT,
	ID_RICHTEXT,
	ID_RICHINPUT,
	ID_BTN_STARTSTOP,
	ID_BTN_ADDROW,
	ID_BTN_TEST,
	PU_RESTORE,
	PU_NEW_ICON,
	PU_EXIT,
	PU_CHECKMARK,
	PU_SUB1,
	PU_SUB2,
	PU_SUBMAIN,
	RADIO_TARGET_TYPE,
	CUSTOM_LAST_ID // for count
};

// evt queue struct 
// CREATE/DELETE/MODIFY/RENAME    PATH1            PATH2
// so create a struct and save into vector or

typedef struct Action
{
	int actType;
	wxString fromPath;
	wxString toPath;
	Action()
	{
		actType = -1;
		fromPath = "";
	}
}ACTION;

int createNewId();
wxString& ClearHeadTailSpace(wxString &str);
wxString& ClearAllSpace(wxString &str);

#define  LICENSE_STRING "     CPTech LTD.\n\
End - User License Agreement\n\
\n\
Please note the following preliminary terms, which use some of the definitions set out in paragraph 1 below:\n\
\n\
This Agreement, and any new versions, between CPTech Ltd.and You, covers all Your use of Trading Terminal from any computer terminals where Trading Terminal has been installed, by You or by third parties.You can accept this Agreement by clicking on the ACCEPT button or similar buttons or links as may be designated by CPTech Ltd.\n\
\n\
Your agreement with CPTech Ltd.will also include the Additional Terms(as defined below).The Additional Terms shall include, but are not limited to, the termsand policies set out in paragraph 7 below.\
\
In order to use the Trading Terminal, CPTech Ltd.Websiteand Products, You must accept the Additional Terms.You can accept the Additional Terms by(i) clicking to accept or agree where this option is made available to You, or (ii)by actually using the Trading Terminal, MetaQuotes Ltd.Websiteand Products, in which case You acknowledgeand agree that CPTech Ltd.will treat such use as acceptance of the applicable terms.\
"
#endif /* utils_hpp */
