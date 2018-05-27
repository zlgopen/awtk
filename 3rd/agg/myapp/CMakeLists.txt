PROJECT( MyAggApp )

#modify this!!
SET( AGG_DIR ${antigrain_BINARY_DIR} )

# additional are modified Find routines
SET ( CMAKE_MODULE_PATH "${MyAggApp_BINARY_DIR}" )

CMAKE_MINIMUM_REQUIRED( VERSION 2.4.8 )

# for the moment this decides the platform code.
IF(WIN32)
    ADD_DEFINITIONS( -D_MSWVC_ -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE )
    SET( WIN32GUI WIN32 )    
    INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/font_win32_tt )
    INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/font_freetype )
ENDIF(WIN32)

IF(UNIX)
    ADD_DEFINITIONS( -D__UNIX__  )
    SET( WIN32GUI "" )
    INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/font_freetype )

    FIND_PACKAGE(X11)
    IF(X11_FOUND)  
        INCLUDE_DIRECTORIES(${X11_INCLUDE_DIRS})
        LINK_LIBRARIES(${X11_LIBRARIES})
    ENDIF(X11_FOUND)

ENDIF(UNIX)

##################################################
# Set all includes, flags, libraries, related to Agg
##################################################

FIND_PACKAGE( Agg )
IF( AGG_FOUND )
    #normally the next is needed, but since this very file is in Agg its source, all is already in place
    #INCLUDE(${AGG_USE_FILE})
ELSE( AGG_FOUND )
    MESSAGE( "AGG library was not found" )
ENDIF( AGG_FOUND )

##################################################
# Set all includes, flags, libraries, related to expat
##################################################

IF( agg_USE_EXPAT )

    FIND_PACKAGE( EXPAT )                    

    IF(EXPAT_FOUND)  
        INCLUDE_DIRECTORIES(${EXPAT_INCLUDE_DIRS})
        LINK_LIBRARIES(${EXPAT_LIBRARIES})
    ELSE(EXPAT_FOUND)  
        MESSAGE(SEND_ERROR "expat not found")
    ENDIF(EXPAT_FOUND)
ENDIF( agg_USE_EXPAT )
   
##################################################
# Set all includes, flags, libraries, related to freetype
##################################################

IF( agg_USE_FREETYPE )
    FIND_PACKAGE( Freetype )                    
    IF( FREETYPE_FOUND )
        INCLUDE_DIRECTORIES( ${FREETYPE_INCLUDE_DIRS} )
        LINK_LIBRARIES( ${FREETYPE_LIBRARIES} )
        LINK_DIRECTORIES( ${FREETYPE_LINK_DIR} )
    ELSE( FREETYPE_FOUND )
        MESSAGE(SEND_ERROR "freetype not found")
    ENDIF( FREETYPE_FOUND )
ENDIF( agg_USE_FREETYPE )

ADD_EXECUTABLE( my_demo ${WIN32GUI}
    my_demo.cpp
)

IF( agg_USE_AGG2D )
    ADD_EXECUTABLE( agg2d_demo ${WIN32GUI}
        agg2d_demo.cpp
    )
ENDIF( agg_USE_AGG2D )


