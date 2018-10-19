cmake_minimum_required (VERSION 3.1)
project (zip_adaptor)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE zip_adaptor_test_HEADERS "test/*.h" "include/*.h" "include/*.inl")
file(GLOB_RECURSE zip_adaptor_test_SOURCES "test/*.cpp")

add_executable(zip_adaptor_test ${zip_adaptor_test_SOURCES} ${zip_adaptor_test_HEADERS})

include_directories("include")