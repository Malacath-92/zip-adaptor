cmake_minimum_required (VERSION 3.1)
project (zip_adaptor)

set(CMAKE_CXX_STANDARD 17)

option(ZIP_ADAPTOR_BUILD_TESTS CACHE OFF)

add_library(zip_adaptor INTERFACE)
target_include_directories(zip_adaptor INTERFACE "include")

if(ZIP_ADAPTOR_BUILD_TESTS)
  file(GLOB_RECURSE zip_adaptor_test_HEADERS "test/*.h" "include/*.h" "include/*.inl")
  file(GLOB_RECURSE zip_adaptor_test_SOURCES "test/*.cpp")

  add_executable(zip_adaptor_test ${zip_adaptor_test_SOURCES} ${zip_adaptor_test_HEADERS})
  target_link_libraries(zip_adaptor_test zip_adaptor)

  if(MSVC)
    target_compile_options(zip_adaptor_test PRIVATE /W4 /WX)
  else()
    target_compile_options(zip_adaptor_test PRIVATE -Wall -Wextra -pedantic -Werror)
  endif()
endif()