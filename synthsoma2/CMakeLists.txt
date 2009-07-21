
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

link_directories(
  ${Boost_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)


set(synthsoma2_SRCS
  sslogging.cc
  neteventsender.cc
  asioutil.cc
  neteventserver.cc
  eventbus.cc
  databus.cc
  syntheticsoma.cc
  testutil.cc
  databuffer.cc
  netdataserver.cc
  netdatasender.cc
  neteventreceiver.cc
  datasequenceproto.cc
)

ADD_LIBRARY(synthsoma2_test
  ${synthsoma2_SRCS}
)


target_link_libraries(synthsoma2_test
  synthsoma2_devices_test
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}
)


ADD_LIBRARY(synthsoma2  SHARED
  ${synthsoma2_SRCS}
)


target_link_libraries(synthsoma2
  devices
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}
)

add_subdirectory(devices)
