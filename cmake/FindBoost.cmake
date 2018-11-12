# @file	    FindBoost.cmake
# @brief	CMake script for finding Boost.
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
# Boost
find_package(Boost REQUIRED)
# LIBRARIES
list(
    APPEND
    PROJECT_LIBRARY
    ${Boost_LIBRARIES}
)
# INCLUDE
list(
    APPEND
    PROJECT_INCLUDE
    ${Boost_INCLUDE_DIRS}
)