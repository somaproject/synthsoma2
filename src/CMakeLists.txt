
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

set(synthsoma2_SRCS
  sslogging.cc

)

ADD_LIBRARY(synthsoma2_test
  ${synthsoma2_SRCS}
)

target_link_libraries(synthsoma2_test
  ${Boost_LIBRARIES} ${SOMADEPS_LIBRARIES})

