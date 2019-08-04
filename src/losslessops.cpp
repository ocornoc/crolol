#include <cstdint>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <boost/multiprecision/cpp_int.hpp>
#include "losslessops.h"

using namespace crolol;
using namespace backend;
namespace mp = boost::multiprecision;
using std::int64_t;
using slims = std::numeric_limits<int64_t>;
using int128 = mp::int128_t;

static constexpr int64_t slmax = slims::max();
static constexpr int64_t slmin = slims::min();
static constexpr int64_t scale = 10000;

static saferet clamp(const int128& n)
{
	saferet out;
	out.val = static_cast<int64_t>(n);
	
	if (n > static_cast<int128>(slmax))
		out.flow = overflow;
	else if (n < static_cast<int128>(slmin))
		out.flow = underflow;

	return out;
}

static saferet make_error(const flowstatus flow)
{
	saferet out;
	out.flow = flow;
	
	return out;
}

static saferet make_badarg(const int64_t val)
{
	saferet out = make_error(badarg);
	out.val = val;
	
	return out;
}

saferet backend::multiply(int64_t n, int64_t m)
{
	return clamp((static_cast<int128>(n) * static_cast<int128>(m))
		/ static_cast<int128>(scale));
}

saferet backend::divide(int64_t n, int64_t m)
{
	if (m == 0) return make_badarg(n);
	else return clamp((static_cast<int128>(n) * scale) / static_cast<int128>(m));
}

saferet backend::pow(int64_t n, int64_t m)
{
	if (n < 0 and std::abs(m) % scale != 0) return make_badarg(m);
	else if (n == 0 and m <= 0) return make_badarg(m);
	else if (m == 0) return clamp(scale);
	else if (n == 0) return clamp(0);
	else if (m < 0) {
		const saferet mul = divide(-scale, m);
		if (mul.flow != noflow) return mul;
		const saferet p = pow(n, mul.val);
		switch (p.flow) {
			case badarg: return p;
			case noflow: return divide(scale, p.val);
			default: return clamp(0);
		}
	} else if (m == scale or n == scale) return clamp(n);
	else if (m < scale) {
		const int128 rescaled_n = static_cast<int128>(n) * scale;
		const int64_t sqrtn = static_cast<int64_t>(mp::sqrt(
			static_cast<int128>(rescaled_n)));
		
		return pow(sqrtn, 2 * m);
	} else {
		const saferet p = pow(n, m - scale);
		if (p.flow == noflow) return multiply(n, p.val);
		else return p;
	}
}
