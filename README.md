# Zip-adaptor for use with C++17

Header only library for range-based for loop on a set of heterogeneous containers, any of which may be const or non-const.

## Requirements

* Needs a C++17 compatible compiler
* CMake is suggested for compiling tests 

## Usage

Copy only _zip_adaptor.h_ and _zip_adaptor.inl_ into your projects include directory. 
If desired define _ZIP_NAMESPACE_ to your preferred namespace to find the _zip_adaptor_ in, by default this will be _zip_.
You may also want to define _ZIP_VERBOSE_ to offer a more verbose _zip_adaptor_ creation and avoid name clashing with your project.
Use _zip_adaptor_ as follows:

    #include <zip_adaptor.h>

    // Get the winning team based on pair-wise comparison of player scores
    int getWinningTeam(std::vector<int> scoresA, std::vector<int> scoresB) {
        int winningTeam = 0;
        // If ZIP_VERBOSE is defined instead zip::make_zip_adaptor(...)
        // If ZIP_NAMESPACE is defined instead ZIP_NAMESPACE::zip(...)
        for (auto [a, b] : zip::zip(scoresA, scoresB)) {
            if (a > b) winningTeam--;
            else if (b > 0) winningTeam++;
        }
        return winningTeam;
    }

You may use different types of containers as long as they support range-based for loops. i.e. they support a begin() and end() function:

    #define ZIP_NAMESPACE
    #include <zip_adaptor.h>

    // Joins any type of container into a vector of tuples 
    template<class... T>
    auto join(T&&... containers) {
        // Skipping ZIP_NAMESPACE because it is defined without value
        auto zipped_containers = zip(std::forward<T>(containers)...);

        using zipped_type = decltype(zipped_containers);
        using zipped_value_type = typename zipped_type::value_type;
        
        std::vector<zipped_value_type> joined_containers;
        joined_containers.reserve(zipped_containers.size());
        for (auto zipped : zipped_containers)
            joined_containers.push_back(zipped);
        return joined_containers; 
    }

## Features

Zip-adaptor supports the following features:

* Creating a zipped range from heterogeneous container types
* Creating a zipped range from const as well as non-const container types
* Erasing out of a zipped range
* Throws _std::length_error_ if attempting to zip ranges of different sizes
* Fully constexpr qualified
* Compiles and runs on _MSVC_, _gcc_ and _clang_

Planned features are:

* Creating a zipped range from rvalues as well as lvalues
* Supplying reasonable member typedefs to deal with copying from the zip iterator

## Tests

To compile tests follow the following commands in a bash console opened in the repository:

    mkdir build
    cd build
    cmake .. -G"MSYS Makefiles" # Or another generator of your choice
    cmake --build .
    cmd.exe /c zip_adaptor_test.exe # Or execution appropriate for your platform

Tests were compiled and executed on Windows 10 using **Visual Studio 2017** as well as **clang 7.0.0** and **gcc 8.2.0** with **MSYS2**.