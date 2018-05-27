# - Find expat
# Find the native EXPAT headers and libraries.
#
#  EXPAT_INCLUDE_DIRS - where to find expat.h, etc.
#  EXPAT_LIBRARIES    - List of libraries when using expat.
#  EXPAT_FOUND        - True if expat found.

# Look for the header file.
FIND_PATH( EXPAT_INCLUDE_DIR expat.h
	$ENV{EXPAT_DIR}/Source/lib
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\expat_is1;Inno Setup: App Path]/Source/lib"
)

# Look for the library.
FIND_LIBRARY( EXPAT_LIBRARY NAMES expat libexpat
    PATHS
	$ENV{EXPAT_DIR}/Bin
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\expat_is1;Inno Setup: App Path]/bin"
)

#MESSAGE ( "EXPAT_INCLUDE_DIR => ${EXPAT_INCLUDE_DIR}" )
#MESSAGE ( "EXPAT_LIBRARY => ${EXPAT_LIBRARY}" )

# handle the QUIETLY and REQUIRED arguments and set EXPAT_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(EXPAT DEFAULT_MSG EXPAT_LIBRARY EXPAT_INCLUDE_DIR)

# Copy the results to the output variables.
IF(EXPAT_FOUND)
  SET(EXPAT_LIBRARIES ${EXPAT_LIBRARY})
  SET(EXPAT_INCLUDE_DIRS ${EXPAT_INCLUDE_DIR})
ELSE(EXPAT_FOUND)
  SET(EXPAT_LIBRARIES)
  SET(EXPAT_INCLUDE_DIRS)
ENDIF(EXPAT_FOUND)

MARK_AS_ADVANCED(EXPAT_INCLUDE_DIR EXPAT_LIBRARY)
