# - Locate Agg libraries
# This module defines
#  AGG_LIBRARIES, the library to link against
#  AGG_FOUND, if false, do not try to link to AGG
#  AGG_INCLUDE_DIRS, where to find headers.
#
# $AGG_DIR is an environment variable that would
# correspond to the install directory on e.g. windows.

# Created by Klaas Holwerda. 
# Search only if the location is not already known.

  SET( AGG_DIR_BIN AGG_DIR_BIN-NOTFOUND CACHE  INTERNAL "" )

  #
  # Look for an installation or build tree.
  #
  FIND_PATH(AGG_DIR_BIN AggConfig.cmake
    # Look for an environment variable AGG_DIR.
    $ENV{AGG_DIR}/bin
    ${AGG_DIR}/bin

    # Look in search path.
    $ENV{PATH}

    NO_DEFAULT_PATH 
    DOC "WXART2D_DIR found"]
    
    # Help the user find it if we cannot.
    DOC "The Agg bin dir"
  )
  
  
  FIND_PATH(AGG_DIR_BIN AggConfig.cmake

    # Look in standard UNIX install locations.
    /usr/local/bin
    /usr/local/lib/agg
    /usr/lib/agg
    /usr/local/include/agg
    /usr/local/include
    /usr/include
    /usr/local/agg
    /usr/X11R6/include

    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Agg\\antigrain 0.1.1]/bin"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Agg_is1;Inno Setup: App Path]/bin"
 
    # Read from the CMakeSetup registry entries.  It is likely that
    # AGG will have been recently built.
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild1]/bin
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild2]/bin
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild3]/bin
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild4]/bin
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild5]/bin
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild6]/bin
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild7]/bin
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild8]/bin
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild9]/bin
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild10]/bin

    # Help the user find it if we cannot.
    DOC "The Agg bin dir"
  )

# If AGG was found, load the configuration file to get the rest of the
# settings.
IF( AGG_DIR_BIN )

  # Check if AGG was built using CMake
    IF(EXISTS ${AGG_DIR_BIN}/AggConfig.cmake)
    SET(AGG_BUILT_WITH_CMAKE 1)
    ENDIF(EXISTS ${AGG_DIR_BIN}/AggConfig.cmake)

  IF(AGG_BUILT_WITH_CMAKE)

        INCLUDE(${AGG_DIR_BIN}/AggConfig.cmake)
    # at this point  AGG_LIBRARIES AGG_INCLUDE_DIRS etc. are set .

  ELSE(AGG_BUILT_WITH_CMAKE)
  
    # oh no! Oke lets do things the hard way.
  
    FIND_PATH(AGG_INCLUDE_DIR agg_config.h 
      ${AGG_DIR}/include
      NO_DEFAULT_PATH
    )

    SET( AGG_INCLUDE_DIRS ${AGG_INCLUDE_DIRS} ${AGG_INCLUDE_DIR} )

    FIND_LIBRARY(AGG_LIBRARY_agg
      NAMES agg
      PATHS
      ${AGG_DIR}
      NO_DEFAULT_PATH
      PATH_SUFFIXES lib64 lib
    )

    SET( AGG_LIBRARIES ${AGG_LIBRARIES} ${AGG_LIBRARY_agg} )

    FIND_LIBRARY(AGG_LIBRARY_aggctrl
      NAMES aggctrl
      PATHS
      ${AGG_DIR}
      NO_DEFAULT_PATH
      PATH_SUFFIXES lib64 lib
    )

    SET( AGG_LIBRARIES ${AGG_LIBRARIES} ${AGG_LIBRARY_aggctrl} )

    FIND_LIBRARY(AGG_LIBRARY_aggfontfreetype
      NAMES aggfontfreetype
      PATHS
      ${AGG_DIR}
      NO_DEFAULT_PATH
      PATH_SUFFIXES lib64 lib
    )

    SET( AGG_LIBRARIES ${AGG_LIBRARIES} ${AGG_LIBRARY_aggfontfreetype} )

    FIND_LIBRARY(AGG_LIBRARY_aggplatform
      NAMES aggplatform
      PATHS
      ${AGG_DIR}
      NO_DEFAULT_PATH
      PATH_SUFFIXES lib64 lib
    )

    SET( AGG_LIBRARIES ${AGG_LIBRARIES} ${AGG_LIBRARY_aggplatform} )

    FIND_LIBRARY(AGG_LIBRARY_gpc
      NAMES gpc
      PATHS
      ${AGG_DIR}
      NO_DEFAULT_PATH
      PATH_SUFFIXES lib64 lib
    )

    SET( AGG_LIBRARIES ${AGG_LIBRARIES} ${AGG_LIBRARY_gpc} )
    SET( AGG_INCLUDE_DIRS ${AGG_INCLUDE_DIR} )
  
  ENDIF(AGG_BUILT_WITH_CMAKE)
	
ENDIF( AGG_DIR_BIN)



#MESSAGE ( "AGG_INCLUDE_DIRS => ${AGG_INCLUDE_DIRS}" )
#MESSAGE ( "AGG_LIBRARIES => ${AGG_LIBRARIES}" )
#MESSAGE ( "AGG_DIR_BIN => ${AGG_DIR_BIN}" )

# handle the QUIETLY and REQUIRED arguments and set AGG_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Agg  DEFAULT_MSG  AGG_DIR_BIN AGG_LIBRARIES  AGG_INCLUDE_DIRS)

MARK_AS_ADVANCED( AGG_DIR_BIN AGG_LIBRARIES AGG_INCLUDE_DIRS)
