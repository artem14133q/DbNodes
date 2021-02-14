cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(DbNodes VERSION 1.0.0 LANGUAGES CXX)

# ==================================================================
# CONFIGURE CMAKE
# ==================================================================
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-sized-deallocation -Wall")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(APP_DEBUG true)
else()
    set(APP_DEBUG false)
endif()

configure_file(config.h.in config.h @ONLY)

# ==================================================================
# SET OUTPUT BINARY FILES DIRS
# ==================================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
include_directories(${CMAKE_BINARY_DIR})
# ===================================================================
# ROOT PATH OF PROJECT
# ===================================================================
set(SRC ${PROJECT_SOURCE_DIR}/src)

# ====================================================================
# APPEND QT LIBRARIES
# ====================================================================
set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

# ====================================================================
# DEFINE SUBDIRECTORIES OF YOUR PROJECT
# ====================================================================
set(INCLUDE_DIRECTORIES
        src/Abstract
        src/Widgets
        src/Relations
        src/Modals
        src/Settings
        src/Dictionaries)

foreach(LIB_PATH IN LISTS INCLUDE_DIRECTORIES)
    add_subdirectory(${LIB_PATH})
endforeach()

# =====================================================================
# DEFINE RESOURCE FILES OF YOUR PROJECT
# =====================================================================
set(RESOURCES
        main.cpp
        src/helper.cpp
        src/helper.h)

qt5_add_resources(RESOURCES res.qrc)

add_executable(${PROJECT_NAME} ${RESOURCES})

# ====================================================================
# INSTALL TARGETS FOR COMPILED RESOURCES
# ====================================================================

#install(TARGETS ${PROJECT_NAME} DESTINATION bin)
#install(TARGETS ${INCLUDE_PROJECT_LIBS} DESTINATION bin/lib)

#install(FILES config DESTINATION bin)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRECTORIES})

# ====================================================================
# DEFILE LIBS OF YOUR PROJECT
# ====================================================================
file(COPY styles DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(COPY icons DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(COPY dbnodes.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

set(INCLUDE_PROJECT_LIBS
        Widgets)

target_link_libraries(${PROJECT_NAME}
        ${INCLUDE_QT_LIBS}
        ${INCLUDE_PROJECT_LIBS})