#-----------------------------------------------------------------------------
# The source files for this directory

MESSAGE ("Directory: " ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR})

ADD_EXECUTABLE (testtraceinfo testtraceinfo.cpp)
TARGET_LINK_LIBRARIES (testtraceinfo DIKUgrafik)

