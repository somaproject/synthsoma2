
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(synthsoma)

IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE RelWithDebInfo)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

SET(LIBRARY_VERSION "2.0")
SET(LIBRARY_SOVERSION "0")


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

SET(Boost_ADDITIONAL_VERSIONS "1.38" "1.38.0" "1.39" "1.39.0")

find_package(Boost 1.37.0 COMPONENTS thread unit_test_framework filesystem program_options system signals python REQUIRED)
find_package(PkgConfig)

# we should handle this with build/release distinctions, but I'm not that l33t. 

SET(CMAKE_CXX_FLAGS "-g -O2 -fPIC ${CMAKE_CXX_FLAGS}")

pkg_check_modules(SOMADEPS REQUIRED
  somanetwork-1.0>=1.0
  somadspio-1.0>=1.0
)

include_directories(ext-logging/boost)


if(CMAKE_INSTALL_PREFIX) 
  set(PY_SOMA_TARGET "${CMAKE_INSTALL_PREFIX}/lib/python2.6/soma/")
else(CMAKE_INSTALL_PREFIX)
  set(PY_SOMA_TARGET "${PYTHON_SITE_PACKAGES_DIR}/soma/")
endif(CMAKE_INSTALL_PREFIX)


INSTALL(FILES __init__.py  
	       DESTINATION ${PY_SOMA_TARGET})

MESSAGE(STATUS "The Soma synthsoma module will be installed in " ${PY_SOMA_TARGET})

ENABLE_TESTING()

include_directories(.)
add_subdirectory(synthsoma2)
add_subdirectory(tests)
add_subdirectory(benchmark)
add_subdirectory(pysynthsoma2)
add_subdirectory(somasynth)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/synthsoma-2.0.pc.in 
  ${CMAKE_CURRENT_BINARY_DIR}/synthsoma-2.0.pc)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/synthsoma-2.0.pc 
  DESTINATION lib/pkgconfig)
