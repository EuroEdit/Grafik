#-----------------------------------------------------------------------------
# The main cmakefile


#CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
CMAKE_MINIMUM_REQUIRED (VERSION 3.5)

IF (COMMAND CMAKE_POLICY)
    CMAKE_POLICY (SET CMP0003 NEW)
ENDIF (COMMAND CMAKE_POLICY)

PROJECT(DIKUgrafik)

SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake/glfw)
SET (GLFW3_DIR ${GLFW3_DIR} /usr/local/lib/cmake/glfw)

#-----------------------------------------------------------------------------
# Just debug / release 

SET (CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

#-----------------------------------------------------------------------------
# Setup compiler specific flags for different configuration modes

#SET (TRACE "")
IF (MSVC)
    ADD_DEFINITIONS (-D_SCL_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
ELSE (MSVC)
    #SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    #SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TRACE}")
#    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CMAKE_SOURCE_DIR}/Include")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include")
    #SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -trigraphs")
ENDIF (MSVC)
SET (CMAKE_VERBOSE_MAKEFILE TRUE)

#-----------------------------------------------------------------------------
# Unittest yes/no

# SET (ENABLE_UNIT_TESTS 0 CACHE STRING "Set to 1 if unit tests should be added to project files and 0 otherwise")

# ENABLE_TESTING ()

#-----------------------------------------------------------------------------
# Find OpenGL, Glut, AGL, Fltk, and GLfw

FIND_PACKAGE (PkgConfig REQUIRED)

FIND_PACKAGE (OpenGL)
IF (OPENGL_FOUND)
    SET (GRAPHICS_INCLUDE_DIRS ${GRAPHICS_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
    SET (GRAPHICS_LIBS  ${GRAPHICS_LIBS} ${OPENGL_LIBRARIES})
ENDIF (OPENGL_FOUND)

FIND_PACKAGE (GLEW)
IF (GLEW_FOUND)
    SET (GRAPHICS_INCLUDE_DIRS ${GRAPHICS_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR})
    SET (GRAPHICS_LIBS  ${GRAPHICS_LIBS} ${GLEW_LIBRARIES})
ENDIF(GLEW_FOUND)

FIND_PACKAGE (GLFW3 REQUIRED glfw3)
IF (GLFW3_FOUND)
    MESSAGE ("GLFW3 FOUND: " ${GLFW3_INCLUDE_DIR}  ${GLFW3_LIBRARIES})
    SET (GRAPHICS_INCLUDE_DIRS ${GRAPHICS_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIR})
    SET (GRAPHICS_LIBS ${GRAPHICS_LIBS} ${GLFW3_LIBRARIES})
ENDIF (GLFW3_FOUND)

# FIND_PACKAGE (AGL PATHS AGL)
# IF (AGL_FOUND)
#    SET (GRAPHICS_INCLUDE_DIRS ${GRAPHICS_INCLUDE_DIRS} ${AGL_DIR})
# ENDIF (AGL_FOUND)

#-----------------------------------------------------------------------------
# Check this, and include the libraries you need
SET (GRAPHICS_LIBS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo" ${GRAPHICS_LIBS})
#MESSAGE ("GRAPHICS_LIBS: " ${GRAPHICS_LIBS})

SET (GRAPHICS_INCLUDE_DIRS ${GRAPHICS_INCLUDE_DIRS} glm DIKUgrafik)

#-----------------------------------------------------------------------------
# Macro to define meta-directories

SET (META_DIRS CMakeFiles CMakeScripts Include Include/DIKUgrafik Lib Bin html latex glm Data)
#SET (META_DIRS CMakeFiles CMakeScripts Include Lib Bin html latex glm)

#-----------------------------------------------------------------------------
# Macro to print a list of  subdirectories

MACRO (PRINTLIST HEADER LIST)
    MESSAGE ("${HEADER}")
        FOREACH (DIR ${LIST})
	    MESSAGE ("    " ${DIR})
        ENDFOREACH (DIR)
    MESSAGE ("${HEADER} ... DONE")
ENDMACRO (PRINTLIST)

#-----------------------------------------------------------------------------
# Macro to collect subdirectories

MACRO (SUBDIRLIST RESULT CURDIR)
    FILE (GLOB CHILDREN RELATIVE ${CURDIR} ${CURDIR}/*)
    SET (DIRLIST "")
    FOREACH (CHILD ${CHILDREN})
        IF (IS_DIRECTORY ${CURDIR}/${CHILD})
	    SET(DIRLIST ${DIRLIST} ${CHILD})
    	ENDIF (IS_DIRECTORY ${CURDIR}/${CHILD})
    ENDFOREACH (CHILD)
    #REMOVE (DIRLIST ${META_DIRS})
    SET (${RESULT} ${DIRLIST})
ENDMACRO (SUBDIRLIST)

#-----------------------------------------------------------------------------
# Show the current source directory before cleaning

SUBDIRLIST (SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
#PRINTLIST ("CURRENT SOURCE DIR" "${SUBDIRS}")

#-----------------------------------------------------------------------------
# Show the current source directory after cleaning

REMOVE (SUBDIRS ${META_DIRS})
REMOVE (SUBDIRS glm)
REMOVE (SUBDIRS CMakeFiles)
#PRINTLIST ("CLEANED CURRENT SOURCE DIR" "${SUBDIRS}")

#-----------------------------------------------------------------------------
# Add subdirectories and includes

#MESSAGE ("ADDING SUBDIRECTORIES")
FOREACH (SUBDIR ${SUBDIRS})
#    MESSAGE ("Add Subdirectory ${SUBDIR} - 0")
    ADD_SUBDIRECTORY (${SUBDIR})
#    MESSAGE ("Add Include ${SUBDIR} - 0")
    INCLUDE_DIRECTORIES (${SUBDIR})
ENDFOREACH (SUBDIR)
#MESSAGE ("ADDING SUBDIRECTORIES ... DONE")

#-----------------------------------------------------------------------------
# Do some cleanup!

# Find the test programs - the ones named test*.cpp
FILE (GLOB_RECURSE TEST_SOURCES */test*.cpp)
#PRINTLIST ("TEST SOURCES" "${TEST_SOURCES}")

# Find the test programs - the ones named assignment*.cpp
FILE (GLOB_RECURSE ASSIGNMENT_SOURCES */assignment*.cpp)
#PRINTLIST ("TEST SOURCES" "${ASSIGNMENT_SOURCES}")

# Find all program files - the ones ending in *.cpp
FILE (GLOB_RECURSE SRC_LIB_FILES */*.cpp)
#PRINTLIST ("LIB SOURCES" "${SRC_LIB_FILES}")

# Remove the test files from the library files, and also the *.cpp
# files in the CMakeFiles directories

REMOVE (SRC_LIB_FILES ${TEST_SOURCES})
REMOVE (SRC_LIB_FILES ${ASSIGNMENT_SOURCES})
REMOVE (SRC_LIB_FILES CMakeFiles/*cpp)
#PRINTLIST ("SRC LIB FILES" "${SRC_LIB_FILES}")

FILE (GLOB_RECURSE DUMMY_FILES glm/*.cpp)
# FOREACH (FILE ${DUMMY_FILES})
#     MESSAGE ("REMOVED FROM  LIB_SOURCE: " ${FILE})
# ENDFOREACH (FILE)
REMOVE (SRC_LIB_FILES ${DUMMY_FILES})

# #MESSAGE ("Lib sources: " ${SRC_LIB_FILES})
# FOREACH (FILE ${SRC_LIB_FILES})
#     MESSAGE ("LIB_SOURCE: " ${FILE})
# ENDFOREACH (FILE)

#-----------------------------------------------------------------------------
# Where to put the library

ADD_LIBRARY (DIKUgrafik ${SRC_LIB_FILES})
INSTALL_TARGETS (${CMAKE_SOURCE_DIR}/Lib DIKUgrafik)

#-----------------------------------------------------------------------------
# Where to put the library headers

#FILE (GLOB_RECURSE LIB_HEADERS ${CMAKE_SOURCE_DIR}/*/*.h) 
#INSTALL_FILES (${CMAKE_SOURCE_DIR}/Include/DIKUgrafik FILES ${LIB_HEADERS}) 

#-----------------------------------------------------------------------------
# Include the headers

#INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR}/Include/DIKUgrafik)
INCLUDE_DIRECTORIES (/usr/local/include)

# #-----------------------------------------------------------------------------
# # Install the test programs

# #MESSAGE ("CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})
# # SET (CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
# # MESSAGE ("EXECUTABLES: " ${EXECUTABLES})
# # REMOVE (EXECUTABLES ${SOURCES})
# #MESSAGE ("EXECUTABLES: " ${EXECUTABLES})
# #INSTALL_FILES (/usr/local/bin/test FILES ${EXECUTABLES})
