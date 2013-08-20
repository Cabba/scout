#include <iostream>
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


void scout_unit_test(){
	scout::execute_tests(std::cout);
}


int main(){
	scout_unit_test();
	return 0;
}
