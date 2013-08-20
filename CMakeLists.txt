project(scout-unit-test)
cmake_minimum_required(VERSION 2.8)

add_definitions(
	-std=c++0x
	-Wall
)

set(SOURCE
	scout/scout.hpp
	unit-test/unit_test.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE})

