//
//  utils.cpp
//  zchat
//
//  Created by reich on 2021/3/11.
//  Copyright © 2021 reich. All rights reserved.
//

#include "utils.h"
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
void SetColor(int ForeColor, int BackColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ForeColor + BackColor * 0x10);
}
//封装一下，方便调用，T表示任何可以被cout输出的类型
template<typename T>
void CoutColorString(T t)
{
	SetColor(0, 5);
	cout << t;
	//printf("%s", t);
	SetColor(15, 0);
}
#endif //WIN32
void fflog(int color, wchar_t* format, ...)
{
#ifdef WIN32
	SetColor(color, 0);
#endif
	va_list args;
	va_start(args, format);
	vwprintf(format, args);
	va_end(args);
#ifdef WIN32
	SetColor(15, 0);
#endif
}
void fflog_warn(wchar_t* format, ...)
{
#ifdef WIN32
	SetColor(0, 14);
#endif
	va_list args;
	va_start(args, format);
	vwprintf(format, args);
	va_end(args);
#ifdef WIN32
	SetColor(15, 0);
#endif
}

void fflog_err(wchar_t* format, ...)
{
#ifdef WIN32
	SetColor(14, 4);
#endif
	va_list args;
	va_start(args, format);
	vwprintf(format, args);
	va_end(args);
#ifdef WIN32
	SetColor(15, 0);
#endif
}

int custom_new_id = CUSTOM_LAST_ID + 1; // extern in other files

int createNewId()
{
    custom_new_id += 1;
    return custom_new_id;
}
wxString& ClearHeadTailSpace(wxString &str)
{
    //std::string sv = str.ToStdString();
    //printf("text=%s\n", sv.c_str());
    
    if (str.empty())
    {
        return str;
    }
    str.erase(0,str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
    return str;
}


//去掉字符串中的全部空格
wxString& ClearAllSpace(wxString &str)
{
    int index = 0;
    if( !str.empty())
    {
        while( (index = str.find(" ",index)) != wxString::npos)
        {
            str.erase(index,1);
        }
    }
    return str;
}