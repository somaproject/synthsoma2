include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

set(devices_SRCS
  timer.cc
  testdevice.cc
  simpletspike.cc
  audiodevice.cc
  dspboard.cc
)

ADD_LIBRARY(synthsoma2_devices_test
  ${devices_SRCS}
)


ADD_LIBRARY(devices SHARED
  ${devices_SRCS}

)

target_link_libraries(synthsoma2_devices_test
  ${Boost_LIBRARIES} ${SOMADEPS_LIBRARIES})

target_link_libraries(devices
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}

)


# set_target_properties(devices PROPERTIES PREFIX "")
