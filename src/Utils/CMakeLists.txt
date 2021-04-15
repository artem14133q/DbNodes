set(LIBRARY_NAME Utils)

set(RESOURCES
        MultipleSelection/Repository.cpp
        MultipleSelection/Repository.h
        MultipleSelection/Selectable.cpp
        MultipleSelection/Selectable.h RelationPath/Render.cpp RelationPath/Render.h AbstractUtil.cpp AbstractUtil.h ListMoving/Moving.cpp ListMoving/Moving.h)

add_library(${LIBRARY_NAME} STATIC ${RESOURCES})

set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Widgets)

# INCLUDE PROJECT SUBDIRECTORIES
set(PROJECT_DIRECTORIES
        ${SRC}/Nodes)

# INCLUDE PROJECT LIBS
set(PROJECT_LIBS
        Nodes)

include_directories(${CMAKE_BINARY_DIR})
target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_DIRECTORIES})
target_link_libraries(${LIBRARY_NAME} ${REQUIRED_LIBS_QUALIFIED} ${PROJECT_LIBS})