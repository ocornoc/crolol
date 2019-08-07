#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <numeric>
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

// Assumes input is positive.
static int64_t sqrt(int64_t n)
{
	return static_cast<int64_t>(mp::sqrt(static_cast<int128>(n) * scale));
}

saferet backend::pow(int64_t n, int64_t m)
{
	if (n < 0 && m % scale != 0) return make_badarg(m);
	else if (n == 0 && m <= 0) return make_badarg(m);
	else if (n == scale || m == 0) return clamp(scale);
	else if (m == scale) return clamp(n);
	else if (m < 0) {
		const saferet result = pow(n, -m);
		if (result.flow == overflow || result.flow == underflow)
			return clamp(0);
		else if (result.flow == badarg) return result;
		
		return divide(scale, result.val);
	} else if (m > scale) {
		int64_t accum = scale;
		int64_t i = 0;
		
		for (; i < m; i += scale) {
			saferet result = multiply(n, accum);
			if (result.flow != noflow) return result;
			accum = result.val;
		}
		
		const saferet remain = pow(n, m - i);
		if (remain.flow != noflow) return remain;
		
		return multiply(remain.val, accum);
	} else if (m < scale) {
		return pow(sqrt(n), 2 * m);
	} else throw std::invalid_argument("Failed pow, end of cases");
}
