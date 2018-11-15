# @file	    TargetQtTest.cmake
# @brief	CMake script for finding QtTest and creating testing targets. If 
#           FindQtCoreQtWidget.cmake is not included, this script will find QtCore itself 
#           and enable CMAKE_AUTOMOC function.
# @author	WUZHUOBIN
# @version	1.1
# @since	Aug.23.2018
# @date 	Sep.03.2018
# @log
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			Copyright (c) WUZHUOBIN 											    #
#			All rights reserved.												    #
#			See Copyright.txt													    #
#			This software is distributed WITHOUT ANY WARRANTY; without even		    #
#			the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR	    #
#			PURPOSE.  See the above copyright notice for more information.		    #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
if(BUILD_TESTING)
# Qt5::Test
    find_package(Qt5Core REQUIRED)
    find_package(Qt5Test REQUIRED)
    list(
        APPEND
        PROJECT_LIBRARY
        Qt5::Core
    )
    set(CMAKE_AUTOMOC ON)
# Enable CTest.
    include(CTest)
    file(GLOB PROJECT_TEST_SRC ${PROJECT_SOURCE_DIR}/Testing/*.c*)
    foreach(ONE ${PROJECT_TEST_SRC})
        get_filename_component(FILE_NAME ${ONE} NAME_WE)
        add_executable(
            ${FILE_NAME}
            ${ONE}
        )
        if(TARGET ${PROJECT_NAME})
            target_link_libraries(
                ${FILE_NAME}
                ${PROJECT_NAME}
                ${PROJECT_LIBRARY}
                Qt5::Test
            )
        else()
            target_link_libraries(
                ${FILE_NAME}
                ${PROJECT_LIBRARY}
                Qt5::Test
            )
        endif()
        target_include_directories(
            ${FILE_NAME}
            PUBLIC
            ${PROJECT_INCLUDE}
        )
# Providing ability to do CTest
        add_test(
            NAME
            ${FILE_NAME}
            COMMAND
            ${FILE_NAME}
        )
# EXPORT test
    install(
        TARGETS ${FILE_NAME}
        RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		INCLUDES DESTINATION include
    )
    endforeach()
endif()
