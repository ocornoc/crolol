#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <numeric>
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

using approx_table_type = std::array<int64_t, scale>;

static int64_t approx_table_func(int64_t n)
{
	return static_cast<int64_t>(mp::sqrt(static_cast<int128>(n) * scale));
}

static approx_table_type approx_table()
{
	approx_table_type atab;
	std::iota(atab.begin(), atab.end(), 0);
	std::transform(atab.begin(), atab.end(), atab.begin(),
		approx_table_func);
	
	return atab;
}

static const approx_table_type sqrt_approx = approx_table();

// Assumes input is positive.
static int64_t sqrt(int64_t n)
{
	if (n >= scale) return 2*sqrt(n / 4);
	else return sqrt_approx.at(n);
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
		
		for (int64_t i = 0; i < m; i += scale) {
			saferet result = multiply(n, accum);
			if (result.flow != noflow) return result;
			accum = result.val;
		}
		
		const saferet remain = pow(n, m % scale);
		if (remain.flow != noflow) return remain;
		
		return multiply(remain.val, accum);
	} else {
		return pow(sqrt(n), 2 * m);
	}
}
