cmake_minimum_required(VERSION 3.16)

project(KPA VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    buttonhandlers.cpp
    buttonhandlers.h
    intrfECE0206.h
    ioctlECE0206.h
    library02061.cpp
    library02061.h
    OpenECE0206byIndex.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(KPA
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        data_receiver.h data_receiver.cpp
    )
else()
    if(ANDROID)
        add_library(KPA SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(KPA
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Добавляем библиотеку SetupAPI для Windows
target_link_libraries(KPA PRIVATE Qt${QT_VERSION_MAJOR}::Widgets setupapi)

# Установка свойств для macOS и Windows
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.KPA)
endif()

set_target_properties(KPA PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS KPA
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(KPA)
endif()
