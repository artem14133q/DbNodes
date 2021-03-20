set(LIBRARY_NAME Utils)

set(RESOURCES
        MultipleSelection/Repository.cpp
        MultipleSelection/Repository.h MultipleSelection/Selectable.cpp MultipleSelection/Selectable.h)

add_library(${LIBRARY_NAME} STATIC ${RESOURCES})

set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Widgets)

# INCLUDE PROJECT SUBDIRECTORIES
set(PROJECT_DIRECTORIES
        ${SRC}/Widgets)

# INCLUDE PROJECT LIBS
set(PROJECT_LIBS
        Widgets)

include_directories(${CMAKE_BINARY_DIR})
target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_DIRECTORIES})
target_link_libraries(${LIBRARY_NAME} ${REQUIRED_LIBS_QUALIFIED} ${PROJECT_LIBS})