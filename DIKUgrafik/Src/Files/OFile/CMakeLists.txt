#-----------------------------------------------------------------------------
# The source files for this directory

MESSAGE ("    Directory: " ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR})

ADD_EXECUTABLE (testofile testofile.cpp)
TARGET_LINK_LIBRARIES (testofile DIKUgrafik glfw3 m ${GRAPHICS_LIBS})


