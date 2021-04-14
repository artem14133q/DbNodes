set(LIBRARY_NAME Relations)

set(RESOURCES
        RelationProvider.cpp
        RelationProvider.h
        RelationViews/DeleteRelationButton.cpp
        RelationViews/DeleteRelationButton.h
        RelationViews/RelationLink.h
        RelationViews/RelationLink.cpp
        RelationProviders/RelationLinkProvider.cpp
        RelationProviders/RelationLinkProvider.h
        RelationProviders/DeleteRelationButtonProvider.cpp
        RelationProviders/DeleteRelationButtonProvider.h
        RelationFactory.cpp
        RelationFactory.h
        Relation.h
        Relation.cpp RelationViews/Path/PathPoint.cpp RelationViews/Path/PathPoint.h)

add_library(${LIBRARY_NAME} STATIC ${RESOURCES})

set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Widgets)

# INCLUDE PROJECT SUBDIRECTORIES
set(PROJECT_DIRECTORIES
        ${SRC}/Dictionaries
        ${SRC}/Widgets
        ${SRC}/Abstract)

# INCLUDE PROJECT LIBS
set(PROJECT_LIBS
        Dictionaries
        Widgets
        Abstract)

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_DIRECTORIES})
target_link_libraries(${LIBRARY_NAME} ${REQUIRED_LIBS_QUALIFIED} ${PROJECT_LIBS})