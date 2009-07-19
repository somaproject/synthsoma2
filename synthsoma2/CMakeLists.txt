
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

include(FindPythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})

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
  datasequenceproto.cc
)

ADD_LIBRARY(synthsoma2_test
  ${synthsoma2_SRCS}
)


ADD_LIBRARY(pysynthsoma2 SHARED
  ${synthsoma2_SRCS}
  pysynthsoma2.cc
)


target_link_libraries(synthsoma2_test
  synthsoma2_devices_test
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}
)


target_link_libraries(pysynthsoma2
  devices
  ${PYTHON_LIBRARY}
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}


)
set_target_properties(pysynthsoma2 PROPERTIES PREFIX "")

add_subdirectory(devices)
