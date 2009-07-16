include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

set(devices_SRCS
  timer.cc
  testdevice.cc

)

ADD_LIBRARY(synthsoma2_devices_test
  ${devices_SRCS}
)

target_link_libraries(synthsoma2_devices_test
  ${Boost_LIBRARIES} ${SOMADEPS_LIBRARIES})
