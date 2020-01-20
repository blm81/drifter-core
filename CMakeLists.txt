cmake_minimum_required(VERSION 3.6)
project(drifter_core)

set(CMAKE_CXX_STANDARD 14)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CINDER_ROOT ${EXTERNAL_DEPS}/Cinder) #CMake build still doesn't work in 9.1 release, building Cinder with CMake generates
                                         #neccesary configuration files
set(CINDER_LIB_DIR ${CINDER_ROOT}/lib/)

include("${CINDER_ROOT}/proj/cmake/modules/cinderMakeApp.cmake")

#TODO build a library as target for use in specific projects
#instead of executable as ci_make_app does
ci_make_app(
        APP_NAME    "Drifter"
        CINDER_PATH ${CINDER_ROOT}
        SOURCES     src/dftrMsgHandler.cpp
                    src/dftrVideo.cpp
                    src/dftrAudio.cpp
                    src/generative/dftrFauna.cpp
                    src/generative/dftrHabitat.cpp
                    ${EXTERNAL_DEPS}/Cinder-Asio/src/TcpClient.cpp
                    ${EXTERNAL_DEPS}/Cinder-Asio/src/TcpSession.cpp
                    ${EXTERNAL_DEPS}/Cinder-Asio/src/ClientInterface.cpp
                    ${EXTERNAL_DEPS}/Cinder-Asio/src/SessionInterface.cpp
                    ${EXTERNAL_DEPS}/Cinder-Asio/src/DispatcherInterface.cpp
                    src/dftrTcpClient.cpp
                    main.cpp
        INCLUDES    ${SRC_DIR} ${CINDER_ROOT}/include ${EXTERNAL_DEPS}/Cinder-Asio/src
        CINDER_LIB_DIRECTORY ${CINDER_LIB_DIR}
)