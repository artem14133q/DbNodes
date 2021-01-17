set(LIBRARY_NAME Settings)

set(RESOURCES
        Parser.cpp
        Parser.h
        SettingsManager.cpp
        SettingsManager.h
        Writer.cpp
        Writer.h)

add_library(${LIBRARY_NAME} STATIC ${RESOURCES})

set(REQUIRED_LIBS_QUALIFIED Qt5::Core)

# INCLUDE PROJECT SUBDIRECTORIES
set(PROJECT_DIRECTORIES)

# INCLUDE PROJECT LIBS
set(PROJECT_LIBS)

include_directories(${CMAKE_BINARY_DIR})
target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_DIRECTORIES})
target_link_libraries(${LIBRARY_NAME} ${REQUIRED_LIBS_QUALIFIED} ${PROJECT_LIBS})