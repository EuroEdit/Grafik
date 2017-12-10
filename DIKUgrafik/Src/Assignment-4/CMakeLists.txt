#-----------------------------------------------------------------------------
# The source files for this directory

MESSAGE ("    Directory: " ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR})

ADD_EXECUTABLE (assignment-4 assignment-4.cpp)
TARGET_LINK_LIBRARIES (assignment-4 DIKUgrafik  glfw3 m ${GRAPHICS_LIBS})

