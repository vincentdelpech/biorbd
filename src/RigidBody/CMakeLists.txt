set(MASTER_PROJECT_NAME ${PROJECT_NAME})
project(${MASTER_PROJECT_NAME}_rigidbody)
set(RIGIDBODY_MODULE_NAME ${MASTER_PROJECT_NAME}_rigidbody PARENT_SCOPE)

# Add the relevant files
set(SRC_LIST_MODULE
    ${CMAKE_CURRENT_SOURCE_DIR}/Bone.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BoneCaracteristics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BoneMesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Contacts.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GeneralizedCoordinates.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GeneralizedTorque.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMU.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMUs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Joints.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Markers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NodeBone.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Patch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Integrator.cpp
)
if (NOT SKIP_KALMAN)
    list(APPEND SRC_LIST_MODULE
        ${CMAKE_CURRENT_SOURCE_DIR}/KalmanRecons.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/KalmanReconsIMU.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/KalmanReconsMarkers.cpp
    )
endif()

# Create the library
if (BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED ${SRC_LIST_MODULE})
else()
    add_library(${PROJECT_NAME} STATIC ${SRC_LIST_MODULE})
endif()

# Add the dependencies for insuring build order
target_link_libraries(${PROJECT_NAME} ${RBDL_LIBRARY} ${MASTER_PROJECT_NAME}_utils)
add_dependencies(${PROJECT_NAME} ${MASTER_PROJECT_NAME}_utils)

# Add the include
target_include_directories(${PROJECT_NAME} PUBLIC
    ${RBDL_INCLUDE_DIR}
    ${RBDL_INCLUDE_DIR}/..
    ${Boost_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIR}
    ${DLIB_INCLUDE_DIR}
    ${IPOPT_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
)

# Installation
install(
    TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${${MASTER_PROJECT_NAME}_LIB_FOLDER}
    RUNTIME DESTINATION ${${MASTER_PROJECT_NAME}_BIN_FOLDER}
    LIBRARY DESTINATION ${${MASTER_PROJECT_NAME}_LIB_FOLDER}
)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH ${${MASTER_PROJECT_NAME}_BIN_FOLDER})
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
