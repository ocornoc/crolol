#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "../src/num.h"
using args = std::pair<crolol::num, crolol::num>;
using argans = std::pair<args, crolol::num>;
using std::int64_t;

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

static const int64_t min = crolol::num::min().getraw();
static const int64_t max = crolol::num::max().getraw();

args make_args(int64_t n, int64_t m)
{
	return std::make_pair(
		crolol::make_numi(n),
		crolol::make_numi(m));
}

args make_argsf(long double n, long double m)
{
	return std::make_pair(
		crolol::make_numf(n),
		crolol::make_numf(m));
}

argans make_argansn(const args& a, const crolol::num& b)
{
	return std::make_pair(a, b);
}

argans make_argans(const args& a, int64_t b)
{
	return make_argansn(a, crolol::make_numi(b));
}

argans make_argansf(const args& a, long double m)
{
	return std::make_pair(a, crolol::make_numf(m));
}

const static std::vector<argans> add_args = {
	make_argans(make_args(0, 0), 0),
	make_argans(make_args(1, 1), 2),
	make_argans(make_args(1, 2), 3),
	make_argans(make_args(2, 1), 3),
	make_argans(make_args(5, 3), 8),
	make_argans(make_args(-5, 3), -2),
	make_argans(make_args(5, -3), 2),
	make_argans(make_args(-5, -3), -8),
	make_argansf(make_argsf(5.2, 3.1), 8.3),
	make_argansf(make_argsf(-5.2, 3.1), -2.1),
	make_argansf(make_argsf(5.2, -3.1), 2.1),
	make_argansf(make_argsf(-5.2, -3.1), -8.3),
	make_argans(make_args(min, -1), min),
	make_argans(make_args(max, 1), max),
};

bool add_test()
{
	bool success = true;
	
	for (const argans& arg_and_ans: add_args) {
		const crolol::num lhs = arg_and_ans.first.first;
		const crolol::num rhs = arg_and_ans.first.second;
		const crolol::num ans = arg_and_ans.second;
		const crolol::num result = lhs + rhs;
		
		if (!crolol::equals(ans, result)) {
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

const static std::vector<argans> sub_args = {
	make_argans(make_args(0, 0), 0),
	make_argans(make_args(1, 1), 0),
	make_argans(make_args(1, 2), -1),
	make_argans(make_args(2, 1), 1),
	make_argans(make_args(5, 3), 2),
	make_argans(make_args(-5, 3), -8),
	make_argans(make_args(5, -3), 8),
	make_argans(make_args(-5, -3), -2),
	make_argansf(make_argsf(5.2, 3.1), 2.1),
	make_argansf(make_argsf(-5.2, 3.1), -8.3),
	make_argansf(make_argsf(5.2, -3.1), 8.3),
	make_argansf(make_argsf(-5.2, -3.1), -2.1),
	make_argans(make_args(min, 1), min),
	make_argans(make_args(max, -1), max),
};

bool sub_test()
{
	bool success = true;
	
	for (const argans& arg_and_ans: sub_args) {
		const crolol::num lhs = arg_and_ans.first.first;
		const crolol::num rhs = arg_and_ans.first.second;
		const crolol::num ans = arg_and_ans.second;
		const crolol::num result = lhs - rhs;
		
		if (!crolol::equals(ans, result)) {
			success = false;
			
			std::cout << static_cast<std::string>(lhs) << " - "
				<< static_cast<std::string>(rhs)
				<< " should be "
				<< static_cast<std::string>(ans) << ", was "
				<< static_cast<std::string>(result)
				<< std::endl;
		}
	}
	
	std::cout << "Subtraction: ";
	report(success);
	
	return success;
}

const static std::vector<argans> mul_args = {
	make_argans(make_args(0, 0), 0),
	make_argans(make_args(1, 1), 1),
	make_argans(make_args(1, 2), 2),
	make_argans(make_args(2, 1), 2),
	make_argans(make_args(5, 3), 15),
	make_argans(make_args(-5, 3), -15),
	make_argans(make_args(5, -3), -15),
	make_argans(make_args(-5, -3), 15),
	make_argansf(make_argsf(5.2, 3.1), 16.12),
	make_argansf(make_argsf(-5.2, 3.1), -16.12),
	make_argansf(make_argsf(5.2, -3.1), -16.12),
	make_argansf(make_argsf(-5.2, -3.1), 16.12),
	make_argans(make_args(min, 1), min),
	make_argans(make_args(min, -1), max),
	make_argans(make_args(max, 1), max),
	make_argansn(make_args(max, -1), -max),
};

bool mul_test()
{
	bool success = true;
	
	for (const argans& arg_and_ans: mul_args) {
		const crolol::num lhs = arg_and_ans.first.first;
		const crolol::num rhs = arg_and_ans.first.second;
		const crolol::num ans = arg_and_ans.second;
		const crolol::num result = lhs * rhs;
		
		if (!crolol::equals(ans, result)) {
			success = false;
			
			std::cout << static_cast<std::string>(lhs) << " * "
				<< static_cast<std::string>(rhs)
				<< " should be "
				<< static_cast<std::string>(ans) << ", was "
				<< static_cast<std::string>(result)
				<< std::endl;
		}
	}
	
	std::cout << "Multiplication: ";
	report(success);
	
	return success;
}

const static std::vector<argans> div_args = {
	make_argans(make_args(1, 1), 1),
	make_argansf(make_args(1, 2), 0.5),
	make_argans(make_args(2, 1), 2),
	make_argansf(make_args(5, 3), 1.6666),
	make_argansf(make_args(-5, 3), -1.6666),
	make_argansf(make_args(5, -3), -1.6666),
	make_argansf(make_args(-5, -3), 1.6666),
	make_argansf(make_argsf(5.2, 3.1), 1.6774),
	make_argansf(make_argsf(-5.2, 3.1), -1.6774),
	make_argansf(make_argsf(5.2, -3.1), -1.6774),
	make_argansf(make_argsf(-5.2, -3.1), 1.6774),
	make_argans(make_args(min, 1), min),
	make_argans(make_args(min, -1), max),
	make_argans(make_args(max, 1), max),
	make_argansn(make_args(max, -1), -max),
};

bool div_test()
{
	bool success = true;
	
	for (const argans& arg_and_ans: div_args) {
		const crolol::num lhs = arg_and_ans.first.first;
		const crolol::num rhs = arg_and_ans.first.second;
		const crolol::num ans = arg_and_ans.second;
		const crolol::num result = lhs / rhs;
		
		if (!crolol::equals(ans, result)) {
			success = false;
			
			std::cout << static_cast<std::string>(lhs) << " / "
				<< static_cast<std::string>(rhs)
				<< " should be "
				<< static_cast<std::string>(ans) << ", was "
				<< static_cast<std::string>(result)
				<< std::endl;
		}
	}
	
	std::cout << "Division: ";
	report(success);
	
	return success;
}

const static std::vector<argans> pow_args = {
	make_argans(make_args(1, 1), 1),
	make_argans(make_args(1, 2), 1),
	make_argans(make_args(2, 1), 2),
	make_argans(make_args(5, 3), 125),
	make_argans(make_args(-5, 3), -125),
	make_argansf(make_args(5, -3), 0.008),
	make_argansf(make_args(-5, -3), -0.008),
	make_argansf(make_argsf(5.2, 3.1), 165.8098),
	make_argansf(make_argsf(5.2, -3.1), 0.006),
	make_argans(make_args(min, 1), min),
	make_argans(make_args(min, -1), 0),
	make_argans(make_args(max, 1), max),
	make_argans(make_args(max, -1), 0),
};

bool pow_test()
{
	bool success = true;
	
	for (const argans& arg_and_ans: pow_args) {
		const crolol::num lhs = arg_and_ans.first.first;
		const crolol::num rhs = arg_and_ans.first.second;
		const crolol::num ans = arg_and_ans.second;
		const crolol::num result = lhs ^ rhs;
		
		if (!crolol::equals(ans, result)) {
			success = false;
			
			std::cout << static_cast<std::string>(lhs) << " ^ "
				<< static_cast<std::string>(rhs)
				<< " should be "
				<< static_cast<std::string>(ans) << ", was "
				<< static_cast<std::string>(result)
				<< std::endl;
		}
	}
	
	std::cout << "Power: ";
	report(success);
	
	return success;
}

const static std::vector<argans> mod_args = {
	make_argans(make_args(1, 1), 0),
	make_argans(make_args(1, 2), 1),
	make_argans(make_args(2, 1), 0),
	make_argans(make_args(5, 3), 2),
	make_argans(make_args(-5, 3), 2),
	make_argans(make_args(5, -3), -2),
	make_argans(make_args(-5, -3), -2),
	make_argansf(make_argsf(5.2, 3.1), 2.1),
	make_argansf(make_argsf(-5.2, 3.1), 2.1),
	make_argansf(make_argsf(5.2, -3.1), -2.1),
	make_argansf(make_argsf(-5.2, -3.1), -2.1),
	make_argansf(make_args(min, 1), 0.5807),
	make_argansf(make_args(min, -1), -0.5807),
	make_argansf(make_args(max, 1), 0.5807),
	make_argansf(make_args(max, -1), -0.5807),
};

bool mod_test()
{
	bool success = true;
	
	for (const argans& arg_and_ans: mod_args) {
		const crolol::num lhs = arg_and_ans.first.first;
		const crolol::num rhs = arg_and_ans.first.second;
		const crolol::num ans = arg_and_ans.second;
		const crolol::num result = lhs % rhs;
		
		if (!crolol::equals(ans, result)) {
			success = false;
			
			std::cout << static_cast<std::string>(lhs) << " % "
				<< static_cast<std::string>(rhs)
				<< " should be "
				<< static_cast<std::string>(ans) << ", was "
				<< static_cast<std::string>(result)
				<< std::endl;
		}
	}
	
	std::cout << "Modulo: ";
	report(success);
	
	return success;
}

int main()
{
	bool success = true;
	
	success &= add_test();
	success &= sub_test();
	success &= mul_test();
	success &= div_test();
	success &= pow_test();
	success &= mod_test();
	
	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
