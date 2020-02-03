cmake_minimum_required(VERSION 3.6)
project(drifter_test)

set(CMAKE_CXX_STANDARD 14)
set(DFTR_SRC_DIR ../src)
set(CINDER_ROOT ${EXTERNAL_DEPS}/Cinder) #CMake build still doesn't work in 9.1 release, building Cinder with CMake generates
#neccesary configuration files
set(CINDER_LIB_DIR ${CINDER_ROOT}/lib/)
set(TEST_INCLUDES ${DFTR_SRC_DIR}/algorithm/tests/dftrGraphTraversalTests.h)
set(TEST_SRCS ${DFTR_SRC_DIR}/algorithm/tests/dftrGraphTraversalTests.cpp)

include("../dependency_config.cmake")
include("${CINDER_ROOT}/proj/cmake/modules/cinderMakeApp.cmake")

ci_make_app(
    APP_NAME    "DrifterTests"
    CINDER_PATH ${CINDER_ROOT}
    SOURCES     ${DFTR_SRCS}
                ${TEST_SRCS}
                ./main.cpp
    INCLUDES    ${DFTR_INCLUDES}
                ${TEST_INCLUDES}
    CINDER_LIB_DIRECTORY ${CINDER_LIB_DIR}
)