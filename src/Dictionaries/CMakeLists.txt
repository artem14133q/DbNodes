set(LIBRARY_NAME Dictionaries)

set(RESOURCES
        DbTableTypesFkDictionary.h
        DbTableTypesDictionary.h
        RelationMakerErrors.h
        StylesDictionary.h
        RelationLinkPositionsDictionary.h
        RelationTypesDictionary.h)

add_library(${LIBRARY_NAME} STATIC ${RESOURCES})

set(REQUIRED_LIBS_QUALIFIED Qt5::Core)

# INCLUDE PROJECT SUBDIRECTORIES
set(PROJECT_DIRECTORIES
        ${SRC}/Abstract)

# INCLUDE PROJECT LIBS
set(PROJECT_LIBS
        Abstract)

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_DIRECTORIES})
target_link_libraries(${LIBRARY_NAME} ${REQUIRED_LIBS_QUALIFIED} ${PROJECT_LIBS})