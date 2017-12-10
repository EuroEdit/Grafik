#-----------------------------------------------------------------------------
# Search for subdirectories

MESSAGE("Directory: " ${CMAKE_CURRENT_SOURCE_DIR})

SUBDIRLIST (SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
REMOVE (SUBDIRS glm Data CMakeFiles CMakeScripts RASTERMAN.xcodeproj RASTERMAN.build)

#MESSAGE ("Subdirs found: " ${SUBDIRS})

FOREACH (SUBDIR ${SUBDIRS})
    MESSAGE ("Adding subdirectory: " ${SUBDIR})
    ADD_SUBDIRECTORY (${SUBDIR})
ENDFOREACH (SUBDIR)

#-----------------------------------------------------------------------------





