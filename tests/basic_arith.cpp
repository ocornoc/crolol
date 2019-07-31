#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "../src/num.h"

void success()
{
	std::cout << "Success!" << std::endl << std::endl;
}

void failure()
{
	std::cout << "Failure." << std::endl << std::endl;
}

void report(const bool b)
{
	b ? success() : failure();
}

using args = std::pair<crolol::num, crolol::num>;
using argans = std::pair<args, crolol::num>;

const static std::vector<argans> add_args = {
	std::make_pair(std::make_pair(0, 0), 0),
	std::make_pair(std::make_pair(1, 1), 2),
	std::make_pair(std::make_pair(10, 20), 30)
};

bool add_test()
{
	bool success = true;
	
	for (const argans& arg_and_ans: add_args) {
		const crolol::num lhs = arg_and_ans.first.first;
		const crolol::num rhs = arg_and_ans.first.second;
		const crolol::num ans = arg_and_ans.second;
		const crolol::num result = lhs + rhs;
		
		if (not crolol::equals(ans, result)) {
			success = false;
			
			std::cout << static_cast<std::string>(lhs) << " + "
				<< static_cast<std::string>(rhs)
				<< " should be "
				<< static_cast<std::string>(ans) << ", was "
				<< static_cast<std::string>(result)
				<< std::endl;
		}
	}
	
	std::cout << "Addition: ";
	report(success);
	
	return success;
}

int main()
{
	bool success = true;
	
	success &= add_test();
	
	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
