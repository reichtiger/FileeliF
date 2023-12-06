reichtiger 2022-1-12
Using CMake to create project .
tested on win OK. so remove win dir and left linux dir.
trying on mac .

in Cmake, richtext must add in find_package before OPTIONAL_COMPONENTS !!
then link OK. compile in xcode or termial both worked 

By reichtiger 2021-2-17

mac下xcode开发 wxWidgets 的配置。
很简单。首先编译wxWidgets, 然后配置wx-config, 具体如下:
1 编译wxWidgets:

To avoid lzma.h not found issue during make :

$ vim ~/.bash_profile

add :

export CPATH=/usr/local/include
export LIBRARY_PATH=/usr/local/lib

then
$ source .bash_profile

now go to compile wxWidgets !!!!

$ cd wxWidgets-3.0.5
$ mkdir wx_build; cd wx_build
$ ../configure --with-cocoa --disable-shared --enable-unicode --with-macosx-version-min=10.10 --with-macosx-sdk=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk

    Or add  --enable-debug

PS: sdk version should be using current Mac OS installed

$ make ( or make -j 4)
$ sudo make install

完毕后欢迎界面。然后可以使用wx-config


------ example on Mac os 10.15.3 with xcode 10.15.sdk

Configured wxWidgets 3.0.5 for `x86_64-apple-darwin19.3.0'

Which GUI toolkit should wxWidgets use?                 osx_cocoa
Should wxWidgets be compiled into single library?       no
Should wxWidgets be linked as a shared library?         no
Should wxWidgets support Unicode?                       yes (using wchar_t)
What level of wxWidgets compatibility should be enabled?
                                     wxWidgets 2.6      no
                                     wxWidgets 2.8      yes
Which libraries should wxWidgets use?
                                     STL                no
                                     jpeg               builtin
                                     png                sys
                                     regex              builtin
                                     tiff               builtin
                                     zlib               sys
                                     expat              sys
                                     libmspack          no
                                     sdl                no
------ end example ---------
                                    
                                    
把wx-config --cppflags 的输出，拷贝到xcode->targets(testwx可执行文件)->Apple Clang - Custom Compiler flags-> Other Cpp Flags 里面
把wx-config --libs 的输出，整个拷贝到 Linking -> Other Linker Flags下面。

然后可以编译程序。调试也可。

-lpng 找不到的错误。在link 库的时候第一行添加 -L/usr/local/lib

richtext 支持 ++++++++++++++++++
add rich text  compile `wx-config --libs std, richtext` output in linker flags +++++++

********** vs 2019 on win 10 ***********************************

Unfortunately you need to manually modify the header to fix the build with MSVS 2019 in 3.0.5 and remove defined(__VISUALC__) || part of the check before wxNEEDS_DECL_BEFORE_TEMPLATE definition in wx/wxcrt.h.

FWIW this problem was fixed since a long time (~6 years) in wx 3.1 and you can compile 3.1.3 or the soon to be released 3.1.4 out of the box with MSVS 2019.

需要在wx/wxcrt.h中删除 "defined(__VISUALC__) ||" 在 wxNEEDS_DECL_BEFORE_TEMPLATE 的定义位置

这样可编译通过, 3.1.4 无此问题

******************* vs2019 编译运行配置************************
include 头文件 $(WXWIN)\include;$(WXWIN)\include\msvc;
LIB path : C:\wxWidgets-3.1.4\lib\vc_lib;
Linker-system中配置，改console-windows

-------不同平台的适配和编译问题---------
代码中宏定义区分平台

#ifdef __WXMAC__
    wxTextCtrl* accText = new wxTextCtrl(p, wxID_ANY, "mac");
#endif
#ifdef __USE_LINUX
    wxTextCtrl* accText = new wxTextCtrl(p, wxID_ANY, "linux");
#endif
#ifdef __WIN32__
    wxTextCtrl* accText = new wxTextCtrl(p, wxID_ANY, "win");
#endif


Makefile中定义具体平台，xcode和vs2019等在配置中定义所在平台。

===========================================================
add 2022-12-5 on wxWidgets3.2.1 with CMake 3-24.0-rc2
===========================================================
1, unzip from wxWidgets3.2.1 , mkdir winbuild
2, CMake open wxDir and build dir=winbuild
3, configure
4, set C++17, samples, configure and generate. 
5, open sln and build Release+x64 , build all and INSTALL ( open VS in Admin mode)
6, wxWidgets will install at C:\Program Files(86) \ wxWidgets
7, open CMake to zchat ,
   set wxWidgets_LIB_DIR = C:\....lib/vc_x64_dll
   set ROOT_dir = C:/Program Files(x86) / wxWidgets
   set wxrc_executable = wxWidgets/bin/wxrc.exe
8. configure and generate . 
Have Fun !