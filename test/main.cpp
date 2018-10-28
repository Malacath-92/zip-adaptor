#include "runtime.h"

#include <iostream>

// Return values:
//		 0 = tests succeeded
//		-1 = tests failed
int main() {
#ifndef SKIP_RUNTIME_TESTS
	try {
		homogeneous_containers_homogeneous_types_const_iteration();
		std::cout << "homogeneous_containers_homogeneous_types_const_iteration " + succeeded << std::endl;
		homogeneous_containers_heterogeneous_types_const_iteration();
		std::cout << "homogeneous_containers_heterogeneous_types_const_iteration " + succeeded << std::endl;
		heterogeneous_containers_homogeneous_types_const_iteration();
		std::cout << "heterogeneous_containers_homogeneous_types_const_iteration " + succeeded << std::endl;
		heterogeneous_containers_heterogeneous_types_const_iteration();
		std::cout << "heterogeneous_containers_heterogeneous_types_const_iteration " + succeeded << std::endl;

		std::cout << std::endl;
		
		homogeneous_containers_homogeneous_types_non_const_iteration();
		std::cout << "homogeneous_containers_homogeneous_types_non_const_iteration " + succeeded << std::endl;
		homogeneous_containers_heterogeneous_types_non_const_iteration();
		std::cout << "homogeneous_containers_heterogeneous_types_non_const_iteration " + succeeded << std::endl;
		heterogeneous_containers_homogeneous_types_non_const_iteration();
		std::cout << "heterogeneous_containers_homogeneous_types_non_const_iteration " + succeeded << std::endl;
		heterogeneous_containers_heterogeneous_types_non_const_iteration();
		std::cout << "heterogeneous_containers_heterogeneous_types_non_const_iteration " + succeeded << std::endl;

		std::cout << std::endl;

		rvalue_iteration();
		std::cout << "rvalue_iteration " + succeeded << std::endl;

		std::cout << std::endl;

		no_copies_no_moves_iteration();
		std::cout << "no_copies_no_moves_iteration " + succeeded << std::endl;

		std::cout << std::endl;

		exceptions();
		std::cout << "exceptions " + succeeded << std::endl;

		std::cout << std::endl;

		erase_const();
		std::cout << "erase_const " + succeeded << std::endl;
		erase_non_const();
		std::cout << "erase_non_const " + succeeded << std::endl;
	}
	catch(std::logic_error& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
#endif
	return 0;
}