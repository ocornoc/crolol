#ifndef CROLOL_LOSSLESSOPS_H
#define CROLOL_LOSSLESSOPS_H
#include <cstdint>

namespace crolol {
	namespace backend {
		enum flowstatus {noflow, underflow, overflow, badarg};
		
		struct saferet {
			std::int64_t val = 0;
			flowstatus flow = noflow;
		};
		
		saferet multiply(std::int64_t n, std::int64_t m);
		saferet divide(std::int64_t n, std::int64_t m);
		saferet pow(std::int64_t n, std::int64_t m);
	}
}
#endif
