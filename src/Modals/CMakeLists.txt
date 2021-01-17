set(LIBRARY_NAME Modals)

set(RESOURCES
        NameNewProject.cpp
        NameNewProject.h
        RelationMaker.cpp
        RelationMaker.h
        ConfirmCloseProject.cpp
        ConfirmCloseProject.h
        TableRename.cpp
        TableRename.h)

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