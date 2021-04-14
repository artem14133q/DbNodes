set(LIBRARY_NAME Saving)

set(RESOURCES
        SaveManager.cpp
        SaveManager.h
        FileResolver.h
        Files/DbnFileStruct/ProjectObject.h
        Files/DbnFileStruct/ColumnObject.h
        Files/DbnFileStruct/ProjectParametersObject.h
        Files/DbnFileStruct/TableObject.h
        Files/DbnFileStruct/RelationObject.h
        Files/ProjectsListStruct/RootObject.h
        Files/ProjectsListStruct/ProjectDataObject.h
        Files/DbnFileResolver.cpp
        Files/DbnFileResolver.h
        Files/ProjectListFileResolver.cpp
        Files/ProjectListFileResolver.h
        Files/DbnFileStruct/RelationPathPointObject.h)

add_library(${LIBRARY_NAME} STATIC ${RESOURCES})

set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Widgets)

# INCLUDE PROJECT SUBDIRECTORIES
set(PROJECT_DIRECTORIES
        ${SRC}/Abstract
        ${SRC}/Dictionaries
        ${SRC}/Widgets
        ${SRC}/Modals)

# INCLUDE PROJECT LIBS
set(PROJECT_LIBS
        Abstract
        Dictionaries
        Widgets
        Modals)

include_directories(${CMAKE_BINARY_DIR})
target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_DIRECTORIES})
target_link_libraries(${LIBRARY_NAME} ${REQUIRED_LIBS_QUALIFIED} ${PROJECT_LIBS})