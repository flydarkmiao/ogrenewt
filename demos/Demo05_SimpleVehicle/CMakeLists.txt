ADD_EXECUTABLE(Demo05_SimpleVehicle demo05.cpp OgreNewtonApplication.cpp OgreNewtonFrameListener.cpp SimpleVehicle.cpp)

TARGET_LINK_LIBRARIES(Demo05_SimpleVehicle ${OGRE_LIBRARIES} ${CEGUI_LIBRARIES} ${OGRENEWT_LIBRARIES} ${OIS_LIBRARIES} ${CEGUIOGRE_LIBRARIES})

#INSTALL(TARGETS Demo05_SimpleVehicle RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
