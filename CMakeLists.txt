cmake_minimum_required(VERSION 3.6)
project(drifter_core)

set(CMAKE_CXX_STANDARD 14)
set(DFTR_SRC_DIR ./src)
set(CINDER_ROOT ${EXTERNAL_DEPS}/Cinder) #CMake build still doesn't work in 9.1 release, building Cinder with CMake generates
                                         #neccesary configuration files
set(CINDER_LIB_DIR ${CINDER_ROOT}/lib/)

include("./dependency_config.cmake")
include("${CINDER_ROOT}/proj/cmake/modules/cinderMakeApp.cmake")

#TODO build a library as target for use in specific projects
#instead of executable as ci_make_app does
ci_make_app(
        APP_NAME    "Drifter"
        CINDER_PATH ${CINDER_ROOT}
        SOURCES     ${DFTR_SRCS}
                    main.cpp
        INCLUDES    ${DFTR_INCLUDES}
        CINDER_LIB_DIRECTORY ${CINDER_LIB_DIR}
)