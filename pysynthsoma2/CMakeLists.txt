
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

include(FindPythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})

link_directories(
  ${Boost_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

ADD_LIBRARY(pysynthsoma2 SHARED
  pysynthsoma2.cc
)

target_link_libraries(pysynthsoma2
  synthsoma2
  ${PYTHON_LIBRARY}
  ${Boost_LIBRARIES} 
  ${SOMADEPS_LIBRARIES}


)

set_target_properties(pysynthsoma2 PROPERTIES PREFIX "")

INSTALL(FILES __init__.py  default.py pysynthsoma2.so 
	       DESTINATION ${PY_SOMA_TARGET}/pysynthsoma2/)

# then for recorder
#INSTALL(FILES 
#  DESTINATION "${PY_SOMA_TARGET}/pysynthsoma2")

