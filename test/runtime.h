#include "zip_adaptor.h"

#include <string>

const auto succeeded = std::string("\033[32msucceeded\033[0m");
const auto failed = std::string("\033[31mfailed\033[0m");

#ifndef SKIP_RUNTIME_TESTS

void homogeneous_containers_homogeneous_types_const_iteration();
void homogeneous_containers_heterogeneous_types_const_iteration();
void heterogeneous_containers_homogeneous_types_const_iteration();
void heterogeneous_containers_heterogeneous_types_const_iteration();

void homogeneous_containers_homogeneous_types_non_const_iteration();
void homogeneous_containers_heterogeneous_types_non_const_iteration();
void heterogeneous_containers_homogeneous_types_non_const_iteration();
void heterogeneous_containers_heterogeneous_types_non_const_iteration();

void no_copies_no_moves_iteration();

void exceptions();

#endif