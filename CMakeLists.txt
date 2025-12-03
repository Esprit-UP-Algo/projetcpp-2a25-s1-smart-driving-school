cmake_minimum_required(VERSION 3.16)
project(tttttetetetetet VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql PrintSupport Charts Network Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql PrintSupport Charts Network Multimedia)

set(PROJECT_SOURCES
    # Main file (merged integration)
    main.cpp
    projetFF.pro

    # Vehicule branch files
    mainwindowV.cpp
    mainwindowV.h
    mainwindowV.ui
    vehicule.h
    vehicule.cpp
    emailsender.h
    emailsender.cpp
    voicerecognition.h
    voicerecognition.cpp
    statschartwidget.h
    statschartwidget.cpp

    # Examens branch files - MAIN UI
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    exam.cpp
    exam.h
    role.h
    logindialog.cpp
    logindialog.h
    logindialog.ui
    connexion.cpp
    connexion.h
    connexion.ui


    # Connection files (shared)
    connection.h
    connection.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(tttttetetetetet
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(tttttetetetetet SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(tttttetetetetet
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link libraries
target_link_libraries(tttttetetetetet PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Multimedia
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.tttttetetetetet)
endif()

set_target_properties(tttttetetetetet PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS tttttetetetetet
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    set_target_properties(tttttetetetetet PROPERTIES
        QT_QML_MODULE_VERSION 1.0
        QT_QML_MODULE_URI tttttetetetetet
    )
    qt_finalize_executable(tttttetetetetet)
endif()
