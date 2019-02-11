cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(vulkan-study VERSION 0.0.1)

find_package(Vulkan REQUIRED)
add_executable(vulkan-study 
    src/main.cpp
    )

target_link_libraries(vulkan-study Vulkan::Vulkan)

IF (NOT WIN32)
  # use pkg-config to get the directories and then use these values
  # in the FIND_PATH() and FIND_LIBRARY() calls
  FIND_PACKAGE(PkgConfig)
  PKG_CHECK_MODULES(PKG_X11_XCB QUIET x11-xcb)

  SET(X11_XCB_DEFINITIONS ${PKG_X11_XCB_CFLAGS})

  FIND_PATH(X11_XCB_INCLUDE_DIR NAMES X11/Xlib-xcb.h HINTS ${PKG_X11_XCB_INCLUDE_DIRS})
  FIND_LIBRARY(X11_XCB_LIBRARIES NAMES X11-xcb HINTS ${PKG_X11_XCB_LIBRARY_DIRS})

  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(X11_XCB DEFAULT_MSG X11_XCB_LIBRARIES X11_XCB_INCLUDE_DIR)

  MARK_AS_ADVANCED(X11_XCB_INCLUDE_DIR X11_XCB_LIBRARIES)
ENDIF (NOT WIN32)