# ==============================================================================
# Indigo CMake File
# ==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# ==============================================================================
# conditional setup ------------------------------------------------------------
# ==============================================================================

SET(LIBS_SHARED       ON  CACHE BOOL "Should the shared version of the library be built?")
SET(LIBS_STATIC       OFF CACHE BOOL "Should the static version of the library be built?")

# ==============================================================================
# project setup ----------------------------------------------------------------
# ==============================================================================

# define project version -------------------------
SET(INDIGO_VERSION 1.1)

# what languages we use --------------------------
ENABLE_LANGUAGE(C)
ENABLE_LANGUAGE(CXX)

# ==============================================================================
# external dependencies --------------------------------------------------------
# ==============================================================================

IF(NOT DEFINED COMPOSITE_PROJECT)
    SET(DEVELOPMENT_ROOT "$ENV{DEVELOPMENT_ROOT}")
ENDIF(NOT DEFINED COMPOSITE_PROJECT)

IF(NOT DEVELOPMENT_ROOT)
    MESSAGE(FATAL_ERROR "DEVELOPMENT_ROOT variable is not set!")
ENDIF(NOT DEVELOPMENT_ROOT)

# ==============================================================================
# external/internal libraries --------------------------------------------------
# ==============================================================================

# zlib -------------------------------------------
SET(ZLIB_ROOT ${DEVELOPMENT_ROOT}/thirdparty/zlib/1.2.7)
INCLUDE_DIRECTORIES(${ZLIB_ROOT}/src)
LINK_DIRECTORIES(${ZLIB_ROOT}/lib)
SET(ZLIB_LIB zlib)

# ==============================================================================
# output directories -----------------------------------------------------------
# ==============================================================================

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

# ==============================================================================
# rpaths -----------------------------------------------------------------------
# ==============================================================================

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

# ==============================================================================
# compiler setups --------------------------------------------------------------
# ==============================================================================

IF(NOT DEFINED COMPOSITE_PROJECT)
    # C options
    SET(CMAKE_C_FLAGS_RELEASE "-O2")
    SET(CMAKE_C_FLAGS_DEBUG "-O0 -g")
    # C++ options
    SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
    SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

    #SET(CMAKE_BUILD_TYPE Release)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT DEFINED COMPOSITE_PROJECT)

# ==============================================================================
# project subdirectories  ------------------------------------------------------
# ==============================================================================

ADD_SUBDIRECTORY(src)



