#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Matplot++::matplot" for configuration "Release"
set_property(TARGET Matplot++::matplot APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Matplot++::matplot PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libmatplot.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libmatplot.dylib"
  )

list(APPEND _cmake_import_check_targets Matplot++::matplot )
list(APPEND _cmake_import_check_files_for_Matplot++::matplot "${_IMPORT_PREFIX}/lib/libmatplot.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
