cmake_minimum_required(VERSION 3.5)

project(DGP LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

set(SOURCE_FILES
    dgp.cpp
    dgp.h
    includes.h
    version.h
)

set(QT_FILES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Resources.qrc
)

if (WIN32)
    ENABLE_LANGUAGE(RC)
    set(SOURCE_FILES resources.rc ${SOURCE_FILES})
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(DGP-Qt SHARED
    ${SOURCE_FILES}
    ${QT_FILES}
  )
else()
  add_executable(DGP-Qt
      ${SOURCE_FILES}
      ${QT_FILES}
  )

endif()
add_executable(DGP-CLI
    DGP-CLI.cpp
    ${SOURCE_FILES}
)

target_link_libraries(DGP-Qt PRIVATE Qt5::Widgets)

# generate proper GUI program on specified platform
if(WIN32) # Check if we are on Windows
        target_compile_definitions(DGP-Qt PRIVATE QT=1)
        if(MSVC) # Check if we are using the Visual Studio compiler
                set_target_properties(DGP-Qt PROPERTIES
                        WIN32_EXECUTABLE YES
                        LINK_FLAGS "/ENTRY:mainCRTStartup"

                )
        elseif(CMAKE_COMPILER_IS_GNUCXX)
                        # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
        else()
                message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
        endif(MSVC)
elseif(APPLE)
        set_target_properties(DGP-Qt PROPERTIES
                        MACOSX_BUNDLE YES
        )
elseif(UNIX)
        # Nothing special required
else()
        message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
endif(WIN32)
