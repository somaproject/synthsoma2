include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

link_directories(
  ${Boost_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

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

target_link_libraries(asioudp
  
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}
  
  )


target_link_libraries(pipeserver
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}
  )


target_link_libraries(blocking_udp
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}
  )



target_link_libraries(async_udp_server
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}
  )

