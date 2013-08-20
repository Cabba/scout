#include <iostream>
#include <fstream>

#include "../scout/scout.hpp"
namespace{

	SCOUT_DEFINE_TEST(TrueTest){
		SCOUT_AssertTrue(1==1);
		SCOUT_AssertTrue(2==2);

		SCOUT_AssertFalse(1==2);
		SCOUT_AssertFalse(2==1);
	}

	SCOUT_DEFINE_TEST(FalseTest){
		SCOUT_AssertTrue(1==2);
		SCOUT_AssertTrue(2==1);

		SCOUT_AssertFalse(1==1);
		SCOUT_AssertFalse(2==2);
	}

} // namespace

int main(){
	// Save the result in a scout.log file
	std::ofstream out("scout.log");
	if( out.is_open() ) scout::evaluate_all_tests(out);
	out.close();

	// Run again the tests and print the result on the standard output
	scout::evaluate_all_tests(std::cout);
	return 0;
}
