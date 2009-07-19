include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

link_directories(
  ${Boost_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include(FindPythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})

add_executable(asioudp
  asioudp.cc
)

add_executable(pipeserver
  pipeserver.cc
)

add_executable(blocking_udp
  blocking_udp.cc
)

add_executable(async_udp_server
  async_udp_server.cc
)

add_executable(eventbusbench
  eventbus.cc
)

# add_executable(simplesynthsoma
#   simplesynthsoma.cc
# )

target_link_libraries(asioudp
  
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}
    ${PYTHON_LIBRARY}
  
  )


target_link_libraries(pipeserver
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}
    ${PYTHON_LIBRARY}

  )


target_link_libraries(blocking_udp
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}
    ${PYTHON_LIBRARY}

  )



target_link_libraries(async_udp_server
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}
    ${PYTHON_LIBRARY}

  )

target_link_libraries(eventbusbench
  synthsoma2_test

  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}
  ${PYTHON_LIBRARY}
  
  )


# target_link_libraries(simplesynthsoma
#   synthsoma2_test
#   ${Boost_LIBRARIES} 
#   ${SOMADEPS_LIBRARIES}
#   ${PYTHON_LIBRARY}

#   )
