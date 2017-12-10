#-----------------------------------------------------------------------------
# The source files for this directory

MESSAGE("    Directory: " ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR})

ADD_EXECUTABLE (testcamera testcamera.cpp)
TARGET_LINK_LIBRARIES (testcamera DIKUgrafik  glfw3 m ${GRAPHICS_LIBS})

