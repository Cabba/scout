///
/// @file scout.hpp
///
/// @author Federico Cabassi federico.cabassi@gmail.com
/// @date 20/08/2013
///
/// Simple C with Object Unit Test - SCOUT
/// Copyright (C) 2013  Federico Cabassi
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <fstream>
#include <vector>

///Define a new test with TESTNAME name.
#define SCOUT_DEFINE_TEST(TESTNAME)\
	class SCOUT_##TESTNAME : public scout_internal::Test{\
	public:\
		SCOUT_##TESTNAME(){\
			std::cout << "[ CREATION ] Test SCOUT_" << #TESTNAME << "." << std::endl;\
			name = #TESTNAME;\
			scout_internal::TestExecutor::singleton().add_test(this);\
		}\
		~SCOUT_##TESTNAME(){}\
		virtual void run(std::ostream& message_stream);\
	};\
	namespace SCOUT_internal{ SCOUT_##TESTNAME TESTNAME##_##instance; }\
	void SCOUT_##TESTNAME::run(std::ostream& message_stream)

/// Assert an expression that should be true
#define SCOUT_AssertTrue(expr)\
	do{++total_expr;\
	if(!(expr)){ message_stream << "\t[ERROR] Failed AssertTrue(" << #expr << ") in line: " << __LINE__ << "." << std::endl;\
	++failed_expr;}}while(false)

/// Assert an expression that should be false
#define SCOUT_AssertFalse(expr)\
	do{++total_expr;\
	if(expr){ message_stream << "\t[ERROR] Failed AssertFalse(" << #expr << ") in line: " << __LINE__ << "." << std::endl;\
	++failed_expr;}}while(false)

namespace scout_internal{

	class Test
	{
	public:
		int failed_expr, total_expr;
		std::string name;

		Test() : failed_expr(0), total_expr(0){}
		virtual ~Test(){}
		virtual void run(std::ostream& message_stream) = 0;
	};

	class TestExecutor
	{
	private:
		static TestExecutor* m_singleton;
		std::vector<Test*> m_tests;

		int m_failed_tests;

		TestExecutor() : m_tests(), m_failed_tests(0) {}

		void shutdown();

	public:
		static TestExecutor& singleton();

		void add_test(Test* test);

		void run_tests(std::ostream& message_stream);
	};

	TestExecutor* TestExecutor::m_singleton = NULL;

	TestExecutor& TestExecutor::singleton(){
		if(m_singleton == NULL) m_singleton = new TestExecutor;
		return *m_singleton;
	}

	void TestExecutor::add_test(Test* test){
		m_tests.push_back( test );
	}

	/// Run all the test in order.
	void TestExecutor::run_tests(std::ostream& message_stream){
		auto end = m_tests.end();
		for(auto it = m_tests.begin(); it != end; ++it){
			auto test = (*it);

			message_stream << "\n[  RUN     ] Executing test <" << test->name << ">" << std::endl;
			test->run(message_stream);

			if(test->failed_expr != 0){
				message_stream << "[  FAILED  ] Failed " << test->failed_expr << " expression out of " << test->total_expr
							   << " in <"<< test->name << ">." << std::endl;
				++m_failed_tests;
			}else{
				message_stream << "[  PASSED  ] Executed " << test->total_expr << " correctly." << std::endl;
			}
		}

		message_stream << "\n[==========] REPORT: FAILED " << m_failed_tests << " TEST OUT OF " << m_tests.size() << "." << std::endl;

		shutdown();
	}

	void TestExecutor::shutdown(){
		m_tests.clear();
	}

} // namespace scout_internal


namespace scout{

	/// Run all the test defined in order and print the result on the message_stream std::ostream.
	void execute_tests(std::ostream& message_stream){
		message_stream << "\n[==========] Simple C with Object Unit Test - SCOUT " << std::endl;
		scout_internal::TestExecutor::singleton().run_tests(message_stream);
	}

} // namespace scout
