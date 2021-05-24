#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "dseed" for configuration "RelWithDebInfo"
set_property(TARGET dseed APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(dseed PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "D:/Projects/libdseed/out/install/lib/dseed.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "D:/Projects/libdseed/out/install/bin/dseed.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS dseed )
list(APPEND _IMPORT_CHECK_FILES_FOR_dseed "D:/Projects/libdseed/out/install/lib/dseed.lib" "D:/Projects/libdseed/out/install/bin/dseed.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
