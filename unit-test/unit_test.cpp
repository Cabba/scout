#include <iostream>
#include <fstream>

#include "../scout/scout.hpp"

SCOUT_DEFINE_TEST(FirstFamily, FirstTest){
	SCOUT_AssertTrue(1==1);
	SCOUT_AssertTrue(2==2);

	SCOUT_AssertFalse(1==2);
	SCOUT_AssertFalse(2==1);
}

SCOUT_DEFINE_TEST(SecondFamily, SecondTest){
	SCOUT_AssertTrue(1==2);
	SCOUT_AssertTrue(2==1);

	SCOUT_AssertFalse(1==1);
	SCOUT_AssertFalse(2==2);
}

// You can disable a single test with the tag 'DISABLE_' (like gunit) ...
SCOUT_DEFINE_TEST(ThirdFamily, DISABLE_FourthTest){
	SCOUT_AssertTrue(true);
}

// The SCOUT_DEFINE_TEST are standard function, so you can put
// all the code you want inside it.
SCOUT_DEFINE_TEST(SecondFamily, ThirdTest){
	for(int i = 0; i < 10; ++i) SCOUT_AssertFalse(false);
}

// ... or an entire family.
SCOUT_DEFINE_TEST(DISABLE_ThirdFamily, FourthTest){
	SCOUT_AssertTrue(true);
}


int main(){
	// Save the result in a scout.log file
	std::ofstream out("scout.log");
	if( out.is_open() ) scout::evaluate_all_tests(out);
	out.close();

	// Run again the tests and print the result on the standard output
	scout::evaluate_all_tests(std::cout);

	return 0;
}
