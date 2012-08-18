# - Try to find libRocketControls
# Once done this will define
#  LIBROCKET_CONTROLS_FOUND - System has libRocketControls
#  LIBROCKET_CONTROLS_INCLUDE_DIRS - The libRocketControls include directories
#  LIBROCKET_CONTROLS_LIBRARIES - The libraries needed to use LibRocketControls
#  LIBROCKET_CONTROLS_DEFINITIONS - Compiler switches required for using LibRocketControls

find_path(LIBROCKET_CONTROLS_INCLUDE_DIR Rocket/Controls.h
          PATH_SUFFIXES Rocket )

find_library(LIBROCKET_CONTROLS_LIBRARY NAMES RocketControls RocketControls_d)

set(LIBROCKET_CONTROLS_LIBRARIES ${LIBROCKET_CONTROLS_LIBRARY} )
set(LIBROCKET_CONTROLS_INCLUDE_DIRS ${LIBROCKET_CONTROLS_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBROCKET_CONTROLS_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibRocketControls DEFAULT_MSG
                                  LIBROCKET_CONTROLS_LIBRARY LIBROCKET_CONTROLS_INCLUDE_DIR)

mark_as_advanced(LIBROCKET_CONTROLS_INCLUDE_DIR LIBROCKET_CONTROLS_LIBRARY )

