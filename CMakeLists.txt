
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(synthsoma2)

find_package(Boost 1.37.0 COMPONENTS thread unit_test_framework filesystem program_options system signals REQUIRED)
find_package(PkgConfig)

# we should handle this with build/release distinctions, but I'm not that l33t. 

SET(CMAKE_CXX_FLAGS "-g -O2 ${CMAKE_CXX_FLAGS}")

pkg_check_modules(SOMADEPS REQUIRED
  somanetwork-1.0>=1.0
  somadspio-1.0>=1.0
)

include_directories(ext-logging/boost)

ENABLE_TESTING()

include_directories(.)
add_subdirectory(synthsoma2)
add_subdirectory(tests)
add_subdirectory(benchmark)
