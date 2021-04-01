#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(OpenCMSG4ver1)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find HepMC (required package)
#
find_package(HepMC REQUIRED)

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED)
if(ROOT_FOUND)
  message(STATUS "Found ROOT: ${ROOT_INCLUDE_DIRS} compiled with ${ROOT_CXX_FLAGS}")
endif()
include(${ROOT_USE_FILE})

#----------------------------------------------------------------------------
# Find Pythia6 (optional package)
#
find_package(Pythia6 QUIET)
if(PYTHIA6_FOUND)
  message(STATUS "G4 Examples: Pythia6 found. --> HepMCEx01 example with Pythia6 enabled.") 
  add_definitions(-DG4LIB_USE_PYTHIA) 
else()
  set(PYTHIA6_LIBRARIES "")  
endif()

#----------------------------------------------------------------------------
# GCC COMPILER FLAGS
#
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(GCC_COMPILE_FLAGS "-Wshadow")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
					${HEPMC_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(OpenCMSG4 OpenCMSG4.cc ${sources} ${headers})
target_link_libraries(OpenCMSG4 ${Geant4_LIBRARIES}
						${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES}
						${PYTHIA6_LIBRARIES} ${ROOT_LIBRARIES} gfortran)	

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build OpenCMSG4. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(OpenCMSG4_SCRIPTS
  exampleOpenCMSG4.in 
  exampleOpenCMSG4.out 
  icons.mac
  gui.mac
  run.png
  init.mac 
  init_vis.mac 
  vis.mac
  example_MyPythia.dat
  exampleRun_particle.mac
  exampleRun_hepmc.mac
  readTrackerPixel.cc
  decays.txt
  ProcessList.txt
  PhysicsConfiguration.txt
  particles.txt  )

foreach(_script ${OpenCMSG4_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(OpenCMSG4ver1 DEPENDS OpenCMSG4)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS OpenCMSG4 DESTINATION bin)

