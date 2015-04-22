///
/// @file scout.hpp
///
/// @author Federico Cabassi federico.cabassi@gmail.com
/// @date 20/08/2013
///
/// Simple C with Object Unit Test - SCOUT
/// v 0.1.1 - Adding timing features.

#pragma once

#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include <cstdint>
#include <chrono>


/// ////////////
/// SCOUT MACROS
/// ////////////

/// Assert an expression that should be true
#define SCOUT_AssertTrue(expr)\
	evaluate_expr(message_stream, new ExprInfo(ASSERT_TRUE, #expr, expr, __LINE__) );

/// Assert an expression that should be false
#define SCOUT_AssertFalse(expr)\
	evaluate_expr(message_stream, new ExprInfo(ASSERT_FALSE, #expr, expr, __LINE__) );

// TODO In progress 
/// Evaluate the execution time of an expression
#define SCOUT_GetTime(expr)\
	auto start = std::chrono::system_clock::now();\
	expr;\
	auto end = std::chrono::system_clock::now();\
	std::chrono::duration<double> time = end-start;\
	evaluate_expr(message_stream, new ExprInfo(EVAL_TIME, #expr, time.count(), __LINE__) );
	
// TODO
/// Check if the execution of a given expression is under a maximum time 
// #define SCOUT_CheckTime(expr, max_milli_time) 


/// //////////
/// SCOUT CODE
/// //////////

///Define a new test with TESTNAME name.
#define SCOUT_DEFINE_TEST(FAMILYNAME, TESTNAME)\
	namespace scout{ namespace internal{\
	class SCOUT_##FAMILYNAME##_##TESTNAME##_class : public Test{\
	public:\
		SCOUT_##FAMILYNAME##_##TESTNAME##_class(){\
			m_name = #TESTNAME;\
			TestRunner::singleton().add_test(new TestInfo(#TESTNAME, #FAMILYNAME, this));\
		}\
		~SCOUT_##FAMILYNAME##_##TESTNAME##_class(){}\
		virtual void run(std::ostream& message_stream);\
	};\
	SCOUT_##FAMILYNAME##_##TESTNAME##_class SCOUT_##FAMILYNAME##_##TESTNAME##_instance; } }\
	void scout::internal::SCOUT_##FAMILYNAME##_##TESTNAME##_class::run(std::ostream& message_stream)

/// Internal implementation of scout
namespace scout{
	namespace internal{

		typedef std::uint32_t uint;

		// TEST DEFINITION ==========
		class Test{
		protected:
			friend class TestRunner;

			/// Type of expression
			enum ExprType{
				ASSERT_TRUE,
				ASSERT_FALSE,
				EVAL_TIME,
				EVAL_TIME_CONSTR
			};

			/// Info for expression processing
			struct ExprInfo{
				uint m_type;

				std::string m_stringified_expr;
				bool m_flag;
				double m_value;
				uint m_line;

				ExprInfo(ExprType type, const char* stringified_expr, bool flag, int line);
				ExprInfo(ExprType type, const char* stringified_expr, double value, int line);
			};

			/// Info for test gerarchy creation.
			struct TestInfo{
				std::string m_name;
				std::string m_family;
				Test* m_instance;

				TestInfo(const char* name, const char* family, Test* instance);
			};

			int m_failed_expr, m_total_expr;
			std::string m_name;

			Test() : m_failed_expr(0), m_total_expr(0){}
			// Disable copy and assignament
			Test(const Test&) = delete;
			Test& operator=(const Test&) = delete;
			
			virtual ~Test(){}

			/// The user must implement this function when provide a test.
			virtual void run(std::ostream& message_stream) = 0;

			/// Function expression processing
			void evaluate_expr(std::ostream& message_stream, ExprInfo* expression);

		}; // class Test

		// TEST IMPLEMENTATION ======================================================================
		Test::ExprInfo::ExprInfo(ExprType type, const char* stringified_expr, bool flag, int line) :
			m_type(type),
			m_stringified_expr(stringified_expr),
			m_flag(flag),
			m_value(0.0),
			m_line(line){
			// Empty
		}

		Test::ExprInfo::ExprInfo(ExprType type, const char* stringified_expr, double value, int line) :
			m_type(type),
			m_stringified_expr(stringified_expr),
			m_flag(false),
			m_value(value),
			m_line(line){
			// Empty
		}

		Test::TestInfo::TestInfo(const char* name, const char* family, Test* instance) :
			m_name(name),
			m_family(family),
			m_instance(instance){
			// Empty
		}

		/**
		 * @brief Print the message for a processed expression.
		 * @param message_stream The output stream.
		 * @param expr The class containing the expression data.
		 */
		void Test::evaluate_expr(std::ostream& message_stream, Test::ExprInfo* expr){
			if(expr == NULL) return;

			++m_total_expr;

			switch (expr->m_type) {

			case ASSERT_TRUE:
				if(!(expr->m_flag)){
					message_stream << "\t[ERROR] Failed AssertTrue(" << expr->m_stringified_expr.c_str()
								   << ") in line: " << expr->m_line << ".\n";
					++m_failed_expr;
				}
				break;

			case ASSERT_FALSE:
				if(expr->m_flag){
					message_stream << "\t[ERROR] Failed AssertFalse(" << expr->m_stringified_expr.c_str()
								   << ") in line: " << expr->m_line << ".\n";
					++m_failed_expr;
				}
				break;
				
			case EVAL_TIME:
				message_stream 	<< "\t [TIME] expression: <" << expr->m_stringified_expr << ">  at line " << expr->m_line 
								<< " was executed in " << expr->m_value << " seconds" 
								<< " (" << (int)(expr->m_value * 1000) << " milliseconds)."<< std::endl;
				break;
			}

			delete expr;
		}

		// TEST RUNNER DEFINITION ==========
		class TestRunner{
		private:
			static TestRunner* m_singleton;

			/// A test family can be ENABLED or DISABLED
			/// its state is shared between all its tests. 
			enum FamilyState{
				ENABLED,
				DISABLED
			};

			typedef std::vector<Test*> TestVector;
			/// The actual state (ENABLED or DISABLED) and the tests inside a 
			/// family. 
			typedef std::pair<FamilyState, TestVector> TestFamilyData;

			/// Mapping the family with its data, the map ensure 
			/// uniqueness between tests family.
			std::map<std::string, TestFamilyData> m_testmap;

			uint	m_failed_tests,
					m_total_tests;

			TestRunner() :
				m_testmap(), m_failed_tests(0), m_total_tests(0){
				// Empty
			}
			// Disable copy and assignament
			TestRunner(const TestRunner&) = delete;
			TestRunner& operator=(const TestRunner&) = delete;

			void reset();

			bool is_disabled(std::string& name);

		public:
			static TestRunner& singleton();

			void add_test(Test::TestInfo* test);

			void run_tests(std::ostream& message_stream);
		};

		// TEST RUNNER IMPLEMENTATION =============
		TestRunner* TestRunner::m_singleton = NULL;

		TestRunner& TestRunner::singleton(){
			if(m_singleton == NULL) m_singleton = new TestRunner;
			return *m_singleton;
		}

		void TestRunner::add_test(Test::TestInfo* test){
			if( test == NULL ) return;

			std::string family = test->m_family;
			std::string name = test->m_name;
			FamilyState fstate = ENABLED;

			if( is_disabled(name) ){
				delete test;
				return;
			}

			if( is_disabled(family) ){
				fstate = DISABLED;
			}

			if( m_testmap.find(family) == m_testmap.end() ){
				// If new family is found
				std::vector<Test*> tests;
				TestFamilyData res(fstate, tests);
				// If the test is enabled push it inside its family
				if( fstate == ENABLED ) res.second.push_back(test->m_instance);
				m_testmap[family] = res;
			}
			else{
				// if a family is already existent
				if( m_testmap[family].first == ENABLED ){
					m_testmap[family].second.push_back(test->m_instance);
				}
			}

			delete test;
		}

		// TODO: clean this section.
		/// Run all the test in order.
		void TestRunner::run_tests(std::ostream& message_stream){

			auto map_iter = m_testmap.begin();
			for(; map_iter != m_testmap.end(); ++map_iter){

				message_stream << "\n";

				if( map_iter->second.first == DISABLED ){
					message_stream << "[  ALERT   ] Test FAMILY [" << map_iter->first.c_str() << "] DISABLED.\n";
					continue;
				}

				message_stream << "[**********] Executing FAMILY [" << map_iter->first.c_str() << "].\n";
				auto vect = map_iter->second.second;

				for(auto it = vect.begin(); it != vect.end(); ++it){
					Test* test = (*it);

					std::string test_name = (test)->m_name;
					
					if (it != vect.begin() ) message_stream << "[----------]" << std::endl;
					message_stream << "[  RUN     ] Executing TEST <" << test_name.c_str() << ">.\n";

					test->run(message_stream);

					uint failed_expr = test->m_failed_expr;
					uint total_expr = test->m_total_expr;

					if( failed_expr != 0){
						message_stream	<< "[  FAILED  ] Failed " << failed_expr << " expression out of " << total_expr
										<< " in <"<< test_name.c_str() << ">.\n";
						++m_failed_tests;
					}else{
						message_stream << "[  PASSED  ] Executed " << total_expr << " expression correctly.\n";
					}

					++m_total_tests;
				}

				message_stream << "[**********] Test family completed.\n";
			}

			message_stream << "\n[==========] REPORT: FAILED " << m_failed_tests << " TEST OUT OF " << m_total_tests << ".\n";

			reset();
		}

		bool TestRunner::is_disabled(std::string &name){
			std::size_t match = name.find("DISABLE_");
			if(match == std::string::npos ) return false;

			std::cout << "Finded disabled.\n";
			// Remove disable tag
			name.erase(0, 8); // 8 = lenght of 'DISABLE_'
			return true;
		}

		/// Reset the data to initial condition
		void TestRunner::reset(){
			m_failed_tests = 0;
			m_total_tests = 0;

			for(auto map_it = m_testmap.begin(); map_it != m_testmap.end(); ++map_it){
				auto vect = map_it->second.second;
				for(auto it = vect.begin(); it != vect.end(); ++it ){
					// TODO: move this task inside a function in the Test class
					(*it)->m_failed_expr = 0;
					(*it)->m_total_expr = 0;
				}
			}
		}

	} // namespace internal


	// EXTERNAL FUNCTIONS =======================================================================

	/**
	 * @brief Run all the test defined (in order) and print the result on the message_stream.
	 * @param message_stream The output stream.
	 */
	void evaluate_all_tests(std::ostream& message_stream){
		message_stream << "[==========] Simple C with Object Unit Test - SCOUT " << std::endl;
		internal::TestRunner::singleton().run_tests(message_stream);
	}

} // namespace scout


