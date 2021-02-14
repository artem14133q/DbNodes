set(LIBRARY_NAME Widgets)

set(RESOURCES
        DeleteArrowButton.h
        DeleteArrowButton.cpp
        MainWindow.cpp
        MainWindow.h
        Node.cpp
        Node.h
        Noderow.cpp
        Noderow.h
        Workarea.cpp
        Workarea.h
        StartupWidget.cpp
        StartupWidget.h)

add_library(${LIBRARY_NAME} STATIC ${RESOURCES})

set(REQUIRED_LIBS_QUALIFIED Qt5::Widgets)

# INCLUDE PROJECT SUBDIRECTORIES
set(PROJECT_DIRECTORIES
        ${SRC}/Modals
        ${SRC}/Abstract
        ${SRC}/Settings
        ${SRC}/Relations
        ${SRC}/Dictionaries)

# INCLUDE PROJECT LIBS
set(PROJECT_LIBS
        Modals
        Abstract
        Settings
        Relations
        Dictionaries)

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_DIRECTORIES})
target_link_libraries(${LIBRARY_NAME} ${REQUIRED_LIBS_QUALIFIED} ${PROJECT_LIBS})