cmake_minimum_required(VERSION 3.2)
set(PROJECTNAME "FileeliF")
set(SRCDIR "src")
PROJECT(${PROJECTNAME})

# =========== turn on C++17 ===========
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# =========== finish: turn on C++17 ===========

find_package(CURL REQUIRED) 
IF(CURL_FOUND)
  INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
  SET(requiredlibs ${requiredlibs} ${CURL_LIBRARIES} )
  MESSAGE("found CURL includePath = ${CURL_INCLUDE_DIR} and libsPath = ${CURL_LIBRARIES}")
ELSE(CURL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
ENDIF(CURL_FOUND)

SET(CURL_BASE "C:\\Program Files\ (x86)\\CURL")

SET(wxWidgets_USE_LIBS)
set( Srcs 
    ${SRCDIR}/main.cpp
    ${SRCDIR}/licenseFrame.cpp
    ${SRCDIR}/mainFrame.cpp
    ${SRCDIR}/movableFrame.cpp
    ${SRCDIR}/movablePanel.cpp 
    ${SRCDIR}/zcImageBtn.cpp 
    ${SRCDIR}/utils.cpp 
    ${SRCDIR}/zcRoundRectPanel.cpp 
    ${SRCDIR}/zcStaticImage.cpp
    ${SRCDIR}/zcPlayGifControl.cpp
    ${SRCDIR}/testFrame.cpp
    ${SRCDIR}/syncPanel.cpp
    ${SRCDIR}/logPanel.cpp
)
set( Hdrs
    ${SRCDIR}/licenseFrame.h
    ${SRCDIR}/mainFrame.h
    ${SRCDIR}/movableFrame.h
    ${SRCDIR}/movablePanel.h 
    ${SRCDIR}/zcImageBtn.h 
    ${SRCDIR}/utils.h 
    ${SRCDIR}/zcRoundRectPanel.h 
    ${SRCDIR}/zcStaticImage.h
    ${SRCDIR}/zcPlayGifControl.h
    ${SRCDIR}/testFrame.h
    ${SRCDIR}/syncPanel.h
    ${SRCDIR}/logPanel.h
)

find_package(wxWidgets REQUIRED 
    OPTIONAL_COMPONENTS 
    net 
    msw 
    zlib 
    png 
    tiff 
    xml
    xrc
    expat
    x11
    propgrid
    osx
    html
    generic
    gl core 
    base 
    richtext
    
)

if(WIN32)

    set (ProcManager
        ${SRCDIR}/ProcMgr.h
        ${SRCDIR}/ProcMgr.cpp
    )
    SET(wxWidgets_LIBRARIES_D ${wxWidgets_LIBRARIES})
    list(APPEND wxWidgets_LIBRARIES
    wxmsw32u_core.lib wxmsw32u_richtext.lib wxbase32u.lib wxmsw32u_html.lib
    libcurl_imp.lib
    )
    list(APPEND wxWidgets_LIBRARIES_D
    wxmsw32ud_core.lib wxmsw32ud_richtext.lib wxbase32ud.lib wxmsw32ud_html.lib
    libcurl-d_imp.lib
    )
elseif(UNIX)
    list(APPEND wxWidgets_LIBRARIES
    libwx_baseu-3.2.a      libwx_gtk3u_core-3.2.a      libwx_gtk3u_richtext-3.2.a
    libwx_baseu_net-3.2.a  libwx_gtk3u_html-3.2.a      libwx_gtk3u_stc-3.2.a
    libwx_baseu_xml-3.2.a  libwx_gtk3u_propgrid-3.2.a  libwx_gtk3u_xrc-3.2.a
    libwx_gtk3u_adv-3.2.a  libwx_gtk3u_qa-3.2.a        libwxscintilla-3.2.a
    -lgthread-2.0 -pthread -lX11 -lSM -lgtk-3 -lgdk-3 -lpangocairo-1.0 -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lXtst -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lharfbuzz -lfontconfig -lfreetype -L/usr/local/lib -lwxscintilla-3.2 -lexpat -lpcre2-32 -lpng -ljpeg -L/usr/local/lib -ltiff -ljbig -lz -lm
  )
endif(WIN32) 

message('wxLibDir === ${wxWidgets_LIBRARIES}')
link_directories(${wxWidgets_LIB_DIR} ${CURL_LIBRARIES} "${CURL_BASE}\\lib")

if(WIN32)
  add_executable(${PROJECTNAME} WIN32 ${Srcs} ${ProcManager} ${Hdrs})
  target_compile_options(${PROJECTNAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/Zi /Od >
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-g>
    )
  set_property(TARGET ${PROJECTNAME} PROPERTY VS_DPI_AWARE "PerMonitor") # high DPI set for vs
  SET_TARGET_PROPERTIES(
       ${PROJECTNAME} 
       PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")
  target_include_directories(${PROJECTNAME} PUBLIC ${wxWidgets_INCLUDE_DIRS} "${CURL_BASE}\\include")
elseif(MACOS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
  target_include_directories(${PROJECTNAME} PUBLIC ${wxWidgets_INCLUDE_DIRS})
  add_executable(${PROJECTNAME} ${Srcs} ${CPPFLAGS} )
elseif(UNIX)
  add_executable(${PROJECTNAME} ${Srcs} )
endif(WIN32)

message('CPPFLAGS==== ${CPPFLAGS}')
include(${wxWidgets_USE_FILE})
if(WIN32)
target_link_libraries(${PROJECTNAME} PRIVATE 
    #debug  ${wxWidgets_LIBRARIES_D} "${CURL_BASE}\\lib\\libcurl-d_imp.lib" # for debug compile 
    optimized  ${wxWidgets_LIBRARIES} "${CURL_BASE}\\lib\\libcurl_imp.lib" # for release mode compile
    )
elseif(UNIX)
target_link_libraries(${PROJECTNAME} PRIVATE ${wxWidgets_LIBRARIES} ${CURL_LIBRARIES})
endif(WIN32)

# IF(wxWidgets_FOUND)
#   INCLUDE("${wxWidgets_USE_FILE}")
#   ADD_EXECUTABLE(${PROJECTNAME} WIN32  src/licenseFrame.cpp)
#   # and for each of your dependant executable/library targets:
#   TARGET_LINK_LIBRARIES(${PROJECTNAME} ${wxWidgets_LIBRARIES})
# ELSE(wxWidgets_FOUND)
#   # For convenience. When we cannot continue, inform the user
#   MESSAGE("wxWidgets not found!")
# ENDIF(wxWidgets_FOUND)
