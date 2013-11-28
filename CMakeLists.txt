# Minimum CMake version required
cmake_minimum_required(VERSION 2.8.11)

# Global name of the project
project(GMStudioPluginGeoMod2)

# Version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

## These might come in handy:
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_VERBOSE_MAKEFILE on)

# Adjust the CMAKE_MODULE_PATH to include the "local" cmake/Modules directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-DQT_PLUGIN)

set(CMAKE_CXX_FLAGS "-fPIC -std=c++11")

################################
# Find the required Qt libraries

########### Qt 5
find_package(Qt5Core 5.1.1 REQUIRED)

################################
# Find GMlib
find_package(
  GMlib 0.4 EXACT REQUIRED
  PATHS ${GMLIB_DIR}
  NO_DEFAULT_PATH
)

#######
# GMStudio
if( NOT GMSTUDIO_SRC_DIR)
  message( FATAL_ERROR "GMSTUDIO_SRC_DIR not set!!")
endif()

if( NOT GMSTUDIO_BUILD_DIR )
  message( FATAL_ERROR "GMSTUDIO_BUILD_DIR not set!!")
endif()

include_directories(  ${GMSTUDIO_SRC_DIR}/plugincore )
link_directories(     ${GMSTUDIO_BUILD_DIR}/lib )


########
# Qt Moc
set( CMAKE_AUTOMOC on )

##############
# Qt Resources
#QT5_ADD_RESOURCES( RCCS
#  resources/gfx.qrc
#)

# FMod
#include_directories()

add_library( ${PROJECT_NAME} SHARED
  scenario.h
  scenario.cpp
  testtorus.h
  subsurface.h
  subsurface.cpp
  erbssurface.h
  erbssurface.cpp
  Constants.h
  util.h
  util.cpp
  knotvector.h
  knotvector.cpp
  beziersurface.h
  beziersurface.cpp
  equalizervisualizer.h
  equalizervisualizer.cpp

  effects/effect.h
  effects/RotationThingy.h
  effects/PlaneEqualizerEffect.h
#  ${RCCS}
)

#find_package(FMODEX)
#include_directories(${FMOD_INCLUDE_DIR})

######
# link
#qt5_use_modules(${PROJECT_NAME} Core)
#qt5_use_modules(${PROJECT_NAME} Gui)
#qt5_use_modules(${PROJECT_NAME} Widgets)

target_link_libraries( ${CMAKE_PROJECT_NAME} plugincore ${GMLIB_LIBRARIES} Qt5::Core ${FMOD_LIBRARIES} )
