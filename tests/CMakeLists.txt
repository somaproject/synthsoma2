include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

link_directories(
  ${Boost_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

add_executable(simple_test
  runner.cc
  simple_test.cc
)

add_executable(eventrouter_test
  runner.cc
  eventrouter_test.cc
)

add_executable(tsretxbuffer_test
  runner.cc
  tsretxbuffer_test.cc
)

add_executable(runtest
  runtest.cc
)

add_executable(neteventsender_test
  runner.cc
  neteventsender_test.cc
)

set(testlist 
  simple_test
  eventrouter_test
  tsretxbuffer_test
  neteventsender_test
  runtest
)

foreach(testname ${testlist})
  
  target_link_libraries(${testname}
    synthsoma2_test
    ${Boost_LIBRARIES} 
    ${SOMADEPS_LIBRARIES}

    )
  
  ADD_TEST(${testname} ${testname})

endforeach(testname)

