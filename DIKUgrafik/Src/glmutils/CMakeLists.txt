#-----------------------------------------------------------------------------
# The source files for this directory

MESSAGE("    Directory: " ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR})

ADD_EXECUTABLE (testglmutils testglmutils.cpp)
TARGET_LINK_LIBRARIES (testglmutils DIKUgrafik)

ADD_EXECUTABLE (testglmutils2d testglmutils2d.cpp)
TARGET_LINK_LIBRARIES (testglmutils2d DIKUgrafik)

ADD_EXECUTABLE (testglmutils3d testglmutils3d.cpp)
TARGET_LINK_LIBRARIES (testglmutils3d DIKUgrafik)
