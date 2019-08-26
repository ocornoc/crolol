#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include "losslessops.h"

using namespace crolol;
using namespace backend;
namespace mp = boost::multiprecision;
using std::int64_t;
using slims = std::numeric_limits<int64_t>;
using int128 = mp::int128_t;
using float128 = mp::cpp_bin_float_quad;

static constexpr int64_t slmax = slims::max();
static constexpr int64_t slmin = slims::min();
static constexpr int64_t scale = 1000;
static const float128 slmaxlog2 = mp::log2(static_cast<float128>(slmax) / scale);
static const float128 slminlog2 = mp::log2(-static_cast<float128>(slmax) / scale);

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
	if (n < 0 && m % scale != 0) return make_badarg(m);
	else if (n == 0 && m <= 0) return make_badarg(m);
	else if (n == scale || m == 0) return clamp(scale);
	else if (m == scale) return clamp(n);
	
	const float128 nf = static_cast<float128>(n) / scale;
	const float128 mf = static_cast<float128>(m) / scale;
	const float128 size = mp::log2(mp::abs(nf)) * mf;
	
	if (size > slmaxlog2) return clamp(slmax);
	else if (mp::abs(size) > slminlog2) return clamp(slmin);
	else return clamp(static_cast<int128>(mp::llrint(mp::trunc(mp::pow(nf, mf) * scale))));
}
