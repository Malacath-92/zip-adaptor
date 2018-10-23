cmake_minimum_required (VERSION 3.1)
project (zip_adaptor)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE zip_adaptor_test_HEADERS "test/*.h" "include/*.h" "include/*.inl")
file(GLOB_RECURSE zip_adaptor_test_SOURCES "test/*.cpp")

add_executable(zip_adaptor_test ${zip_adaptor_test_SOURCES} ${zip_adaptor_test_HEADERS})

if(MSVC)
  target_compile_options(zip_adaptor_test PRIVATE /W4 /WX)
else()
  target_compile_options(zip_adaptor_test PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

include_directories("include")