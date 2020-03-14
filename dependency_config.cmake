set(DFTR_INCLUDES ${DFTR_SRC_DIR}
    ${CINDER_ROOT}/include
    ${EXTERNAL_DEPS}/Cinder-Asio/src)

set(DFTR_SRCS ${DFTR_SRC_DIR}/dftrMsgHandler.cpp
    ${DFTR_SRC_DIR}/dftrTcpClient.cpp
    ${DFTR_SRC_DIR}/dftrVideo.cpp
    ${DFTR_SRC_DIR}/dftrAudio.cpp
    ${DFTR_SRC_DIR}/generative/dftrFauna.cpp
    ${DFTR_SRC_DIR}/generative/dftrHabitat.cpp
    ${DFTR_SRC_DIR}/generative/dftrHabitatTile.cpp
    ${DFTR_SRC_DIR}/algorithm/dftrGraphTraversal.cpp
    ${EXTERNAL_DEPS}/Cinder-Asio/src/TcpClient.cpp
    ${EXTERNAL_DEPS}/Cinder-Asio/src/TcpSession.cpp
    ${EXTERNAL_DEPS}/Cinder-Asio/src/ClientInterface.cpp
    ${EXTERNAL_DEPS}/Cinder-Asio/src/SessionInterface.cpp
    ${EXTERNAL_DEPS}/Cinder-Asio/src/DispatcherInterface.cpp)