
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

set(synthsoma2_SRCS
  sslogging.cc
  eventrouter.cc
  neteventsender.cc
  asioutil.cc
  neteventserver.cc
  eventbus.cc
  databus.cc
  syntheticsoma.cc

)

ADD_LIBRARY(synthsoma2_test
  ${synthsoma2_SRCS}
)

target_link_libraries(synthsoma2_test
  synthsoma2_devices_test
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}
)

add_subdirectory(devices)
