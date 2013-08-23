# Scout 

*Scout* or *Simple C with Objects Unit Test* is a super-simple library for very basic Unit Testing, it's very few lines of code (C++11 required).

It's designed for Unit Test with assertion, and can produce a formatted output on a general `std::ostream`.

## Features
 
- *Automatic tests discovery*: you have to define the test only once in your code, the API takes care of the rest.

- *Test family and single test disabling*: is possiblo to disable one test, or en entire suite with the tag `DISABLE_` before the test name or the family name.

- *Automatic test grouping*: all the test are grouped by family and executed in family definition order, even if the definition order is not consecutive.

## Usage

### Test definition

A very basic example:

	#include "scout.hpp"
	
	SCOUT_DEFINE_TEST(TestFamily, ExampleTest){
		SCOUT_AssertTrue(1==1);
		SCOUT_AssertTrue(2==2);

		SCOUT_AssertFalse(1==2);
		SCOUT_AssertFalse(2==1);
	}


With `SCOUT_DEFINE_TEST(test_family, test_name)` you can produce a test named *test_name*.
For now scout have only two type of assertion `SCOUT_AssertTrue` and `SCOUT_AssertFalse`.

### Disabling

You can disable a single test with:

	SCOUT_DEFINE_TEST(TestFamily, DISABLE_ExampleTest){
		// ...
	}

or a test family with the same tag:
	
	SCOUT_DEFINE_TEST(DISABLE_TestFamily, ExampleTest){
		// ...
	}

If you disable a family all the tests of its class are disabled.

### Run the test

For running all the tests:

	int main(){
		scout::evaluate_all_tests(std::cout);
		return 0;
	}

## Output example

This is an example of the output produced:

	[==========] Simple C with Object Unit Test - SCOUT 

	[----------] Executing FAMILY [FirstFamily].
	[  RUN     ] Executing TEST <FirstTest>.
	[  PASSED  ] Executed 4 expression correctly.
	[----------] Test family completed.

	[----------] Executing FAMILY [SecondFamily].
	[  RUN     ] Executing TEST <SecondTest>.
		[ERROR] Failed AssertTrue(1==2) in line: 15.
		[ERROR] Failed AssertTrue(2==1) in line: 16.
		[ERROR] Failed AssertFalse(1==1) in line: 18.
		[ERROR] Failed AssertFalse(2==2) in line: 19.
	[  FAILED  ] Failed 4 expression out of 4 in <SecondTest>.
	[  RUN     ] Executing TEST <ThirdTest>.
	[  PASSED  ] Executed 10 expression correctly.
	[----------] Test family completed.

	[  ALERT   ] Test FAMILY [ThirdFamily] DISABLED.

	[==========] REPORT: FAILED 1 TEST OUT OF 3.

## Future changes

*	Add timing feature with `std::chrono`.
