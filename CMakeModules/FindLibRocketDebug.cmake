# - Try to find libRocketDebugger
# Once done this will define
#  LIBROCKET_DEBUGGER_FOUND - System has libRocketDebugger
#  LIBROCKET_DEBUGGER_INCLUDE_DIRS - The libRocketDebugger include directories
#  LIBROCKET_DEBUGGER_LIBRARIES - The libraries needed to use LibRocketDebugger
#  LIBROCKET_DEBUGGER_DEFINITIONS - Compiler switches required for using LibRocketDebugger

find_path(LIBROCKET_DEBUGGER_INCLUDE_DIR Rocket/Debugger.h
          PATH_SUFFIXES Rocket )

find_library(LIBROCKET_DEBUGGER_LIBRARY NAMES RocketDebugger RocketDebugger_d)

set(LIBROCKET_DEBUGGER_LIBRARIES ${LIBROCKET_DEBUGGER_LIBRARY} )
set(LIBROCKET_DEBUGGER_INCLUDE_DIRS ${LIBROCKET_DEBUGGER_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBROCKET_DEBUGGER_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibRocketDebugger DEFAULT_MSG
                                  LIBROCKET_DEBUGGER_LIBRARY LIBROCKET_DEBUGGER_INCLUDE_DIR)

mark_as_advanced(LIBROCKET_DEBUGGER_INCLUDE_DIR LIBROCKET_DEBUGGER_LIBRARY )

