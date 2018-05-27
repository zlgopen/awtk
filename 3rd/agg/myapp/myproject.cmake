# \file myproject.cmake
# \author Klaas Holwerda
#
#     Copyright: 2008 (c) Klaas Holwerda
#

CONFIGURE_FILE(${antigrain_SOURCE_DIR}/bin/FindAgg.cmake
               ${antigrain_BINARY_DIR}/myapp/FindAgg.cmake
               @ONLY )

CONFIGURE_FILE(${antigrain_SOURCE_DIR}/bin/FindEXPAT.cmake
${antigrain_BINARY_DIR}/myapp/FindExpat.cmake
@ONLY )

CONFIGURE_FILE(${antigrain_SOURCE_DIR}/bin/FindFreetype.cmake
               ${antigrain_BINARY_DIR}/myapp/FindFreetype.cmake
               @ONLY )

CONFIGURE_FILE(${antigrain_SOURCE_DIR}/myapp/CMakeLists.txt.in
               ${antigrain_BINARY_DIR}/myapp/CMakeLists.txt
               @ONLY )
CONFIGURE_FILE(${antigrain_SOURCE_DIR}/myapp/my_demo.cpp
               ${antigrain_BINARY_DIR}/myapp/my_demo.cpp
               @ONLY )
CONFIGURE_FILE(${antigrain_SOURCE_DIR}/myapp/agg2d_demo.cpp
               ${antigrain_BINARY_DIR}/myapp/agg2d_demo.cpp
               @ONLY )

