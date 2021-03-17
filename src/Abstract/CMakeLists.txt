set(LIBRARY_NAME Abstract)

set(RESOURCES
        DrawableWidget.h
        DrawableWidget.cpp
        AbstractNode.h
        AbstractNode.cpp
        StructDictionary.h
        AbstractModal.cpp
        AbstractModal.h
        AbstractRelationView.cpp
        AbstractRelationView.h
        JsonObject.cpp
        JsonObject.h)

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