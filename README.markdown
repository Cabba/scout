# Scout 

*Scout* or *Simple C with Object Unit Test* is a super-simple library for very basic Unit Testing, it's about 100 lines of code (C++11 required).
It's designed for Unit Test with assertion, and can produce a formatted output on a general `std::ostream`.

## Usage

A very basic example:

	#include "scout.hpp"

	// Make an anonymous namespace for avoid name collision
	namespace{
		SCOUT_DEFINE_TEST(ExampleTest){
			SCOUT_AssertTrue(1==1);
			SCOUT_AssertTrue(2==2);

			SCOUT_AssertFalse(1==2);
			SCOUT_AssertFalse(2==1);
		}
	}

With `SCOUT_DEFINE_TEST(test_name)` you can produce a test named *test_name*.
For now scout have only two type of assertion `SCOUT_AssertTrue` and `SCOUT_AssertFalse`.

For running all the test:

	int main(){
		scout::evaluate_all_tests(std::cout);
		return 0;
	}

## Future changes

*	Add timing feature with `std::chrono`.
