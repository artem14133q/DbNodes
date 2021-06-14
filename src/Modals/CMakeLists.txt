set(LIBRARY_NAME Modals)

set(RESOURCES
        NewProject.cpp
        NewProject.h
        RelationMaker.cpp
        RelationMaker.h
        ConfirmCloseProject.cpp
        ConfirmCloseProject.h
        TableRename.cpp
        TableRename.h
        Finder.cpp
        Finder.h
        Settings.cpp
        Settings.h
        ExceptionModal.cpp
        ExceptionModal.h MinimapSettings.cpp MinimapSettings.h)

add_library(${LIBRARY_NAME} STATIC ${RESOURCES})

set(REQUIRED_LIBS_QUALIFIED Qt5::Widgets)

# INCLUDE PROJECT SUBDIRECTORIES
set(PROJECT_DIRECTORIES
        ${SRC}/Widgets)

# INCLUDE PROJECT LIBS
set(PROJECT_LIBS
        Widgets)

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_DIRECTORIES})
target_link_libraries(${LIBRARY_NAME} ${REQUIRED_LIBS_QUALIFIED} ${PROJECT_LIBS})