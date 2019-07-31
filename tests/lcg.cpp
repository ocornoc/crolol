#include <array>
#include <iostream>
#include <string>
#include "../src/num.h"

const static crolol::num mod = 9;
const static crolol::num mult = 4;
const static crolol::num incr = 1;
const static crolol::num seed = 0;
static crolol::num lcg_state = seed;

crolol::num prng()
{
	crolol::num oldv = lcg_state;
	lcg_state = (mult * lcg_state + incr) % mod;
	
	return oldv;
}

static std::array<crolol::num, 10> should_bes = {0, 1, 5, 3, 4, 8, 6, 7, 2, 0};

int main()
{
	for (const crolol::num& should_be: should_bes) {
		const crolol::num ans = prng();
		
		if (not crolol::equals(should_be, ans)) {
			std::cout << static_cast<std::string>(ans)
				<< " should have been "
				<< static_cast<std::string>(should_be)
				<< std::endl;
			
			return EXIT_FAILURE;
		}
	}
	
	return EXIT_SUCCESS;
}
