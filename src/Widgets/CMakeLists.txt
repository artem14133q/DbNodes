set(LIBRARY_NAME Widgets)

set(RESOURCES
        MainWindow.cpp
        MainWindow.h
        Workarea.cpp
        Workarea.h
        StartupWidget/MainWidget.cpp
        StartupWidget/MainWidget.h
        StartupWidget/Menu.cpp
        StartupWidget/Menu.h
        StartupWidget/Project.cpp
        StartupWidget/Project.h
        StartupWidget/Hints.cpp
        StartupWidget/Hints.h
        Menus/RecentMenu.cpp
        Menus/RecentMenu.h)

add_library(${LIBRARY_NAME} STATIC ${RESOURCES})

set(REQUIRED_LIBS_QUALIFIED Qt5::Widgets)

# INCLUDE PROJECT SUBDIRECTORIES
set(PROJECT_DIRECTORIES
        ${SRC}/Modals
        ${SRC}/Abstract
        ${SRC}/Settings
        ${SRC}/Relations
        ${SRC}/Dictionaries
        ${SRC}/Saving
        ${SRC}/Utils
        ${SRC}/Nodes)

# INCLUDE PROJECT LIBS
set(PROJECT_LIBS
        Modals
        Abstract
        Settings
        Relations
        Dictionaries
        Saving
        Utils
        Nodes)

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_DIRECTORIES})
target_link_libraries(${LIBRARY_NAME} ${REQUIRED_LIBS_QUALIFIED} ${PROJECT_LIBS})