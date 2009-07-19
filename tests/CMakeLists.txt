include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

link_directories(
  ${Boost_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include(FindPythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})

add_executable(simple_test
  runner.cc
  simple_test.cc
)


add_executable(tsretxbuffer_test
  runner.cc
  tsretxbuffer_test.cc
)

add_executable(datasequenceproto_test
  runner.cc
  datasequenceproto_test.cc
)

add_executable(runtest
  runtest.cc
)

add_executable(neteventsender_test
  runner.cc
  testutil.cc
  neteventsender_test.cc
)

add_executable(netdatasender_test
  runner.cc
  testutil.cc
  netdatasender_test.cc
  )

add_executable(eventbus_test
  runner.cc
  testutil.cc
  eventbus_test.cc
)


add_executable(simpletspike_test
  runner.cc
  simpletspike_test.cc
)


set(testlist 
  simple_test
  tsretxbuffer_test
  neteventsender_test
  eventbus_test
  datasequenceproto_test
  netdatasender_test
  simpletspike_test
  runtest
)

foreach(testname ${testlist})
  
  target_link_libraries(${testname}
    synthsoma2_test
    ${Boost_LIBRARIES} 
    ${SOMADEPS_LIBRARIES}
    ${PYTHON_LIBRARY}
    )
  
  ADD_TEST(${testname} ${testname})

endforeach(testname)

