# @file	    FindVTK.cmake
# @brief	CMake script for finding VTK.
# @author	WUZHUOBIN
# @version	1.0
# @since	Aug.23.2018
# @date 	Aug.28.2018
# @log
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			Copyright (c) WUZHUOBIN 											    #
#			All rights reserved.												    #
#			See Copyright.txt													    #
#			This software is distributed WITHOUT ANY WARRANTY; without even		    #
#			the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR	    #
#			PURPOSE.  See the above copyright notice for more information.		    #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
# LIBRARIES
list(
    APPEND
    PROJECT_LIBRARY
    ${VTK_LIBRARIES}
)