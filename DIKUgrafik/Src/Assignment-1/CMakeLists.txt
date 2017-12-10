#-----------------------------------------------------------------------------
# The source files for this directory

MESSAGE ("    Directory: " ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR})

ADD_EXECUTABLE (assignment-1 assignment-1.cpp)
TARGET_LINK_LIBRARIES (assignment-1 DIKUgrafik  glfw3 m ${GRAPHICS_LIBS})

