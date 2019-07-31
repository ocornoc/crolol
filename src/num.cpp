#include <cfenv>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <gmp.h>
#include <gmpxx.h>
#include "num.h"

#pragma STDC FENV_ACCESS on

using namespace crolol;
using bigint = mpz_class;

using l_lims = std::numeric_limits<long>;

constexpr static long scale = 10000;
constexpr static long num_min = l_lims::lowest();
constexpr static long num_max = l_lims::max();
constexpr static long num_truthy = 1L * scale;
constexpr static long num_falsy = 0L * scale;

static bigint clamp(const bigint& bi)
{
	if (bi > num_max) return num_max;
	else if (bi < num_min) return num_min;
	else return bi;
}

num::num(const bigint& bi)
	: bi(clamp(bi * scale))
{}

num::num(bigint&& bi)
	: bi(std::forward<bigint>(clamp(bi * scale)))
{}

num::num(const bool b)
	: bi(b ? num_truthy : num_falsy)
{}

num::num(const bigint& bi, int)
	: bi(bi)
{}

const num num::min = num_min;
const num num::max = num_max;
const num num::truthy = num_truthy;
const num num::falsy = num_falsy;

num::operator double() const
{
	return static_cast<bigint>(bi).get_d() / scale;
}

num::operator std::string() const
{
	return std::to_string(static_cast<double>(*this));
}

num::operator long() const
{
	return static_cast<double>(*this);
}

num::operator unsigned long() const
{
	return static_cast<double>(*this);
}

num::operator bool() const
{
	return bi != num_falsy;
}

num& num::operator+=(const num& rhs)
{
	bi = clamp(bi + rhs.bi);
	
	return *this;
}

num& num::operator-=(const num& rhs)
{
	bi = clamp(bi - rhs.bi);
	
	return *this;
}

num& num::operator*=(const num& rhs)
{
	bi = clamp((bi * rhs.bi) / (scale * scale));
	
	return *this;
}

num& num::operator/=(const num& rhs)
{
	bi = clamp((bi * scale) / (rhs.bi * scale));
	
	return *this;
}

num& num::operator%=(const num& rhs)
{
	int sgn = mpz_sgn(rhs.bi.get_mpz_t());
	bi = clamp(bi % rhs.bi * sgn);
	
	return *this;
}

num operator+(num lhs, const num& rhs)
{
	return lhs += rhs;
}

num operator-(num lhs, const num& rhs)
{
	return lhs -= rhs;
}

num operator*(num lhs, const num& rhs)
{
	return lhs *= rhs;
}

num operator/(num lhs, const num& rhs)
{
	return lhs /= rhs;
}

num operator%(num lhs, const num& rhs)
{
	return lhs %= rhs;
}

num operator!(const num& n)
{
	return n.bi == num_falsy ? num::truthy : num::falsy;
}

num operator==(const num& lhs, const num& rhs)
{
	return lhs.bi == rhs.bi;
}

num operator!=(const num& lhs, const num& rhs)
{
	return !(lhs == rhs);
}

num operator<(const num& lhs, const num& rhs)
{
	return lhs.bi < rhs.bi;
}

num operator>(const num& lhs, const num& rhs)
{
	return lhs.bi > rhs.bi;
}

num operator>=(const num& lhs, const num& rhs)
{
	return !(lhs < rhs);
}

num operator<=(const num& lhs, const num& rhs)
{
	return !(lhs > rhs);
}

num crolol::abs(const num& n)
{
	return n * (2*(n < 0) - 1);
}

num operator^(const num& lhs, const num& rhs)
{
	std::feclearexcept(FE_ALL_EXCEPT);
	const double approx_lhs = static_cast<double>(lhs) / scale;
	const double approx_rhs = static_cast<double>(rhs) / scale;
	const double approx_pow = std::pow(approx_lhs, approx_rhs);
	
	const int fes = std::fetestexcept(FE_ALL_EXCEPT);
	if (fes & FE_DIVBYZERO)
		throw std::invalid_argument("Division by zero");
	else if (fes & FE_INVALID)
		throw std::invalid_argument("Invalid arguments");
	else if (fes & FE_OVERFLOW or approx_pow >= num_max)
		return num::max;
	else if (fes & FE_UNDERFLOW or approx_pow <= num_min)
		return num::min;
	
	const unsigned long exp = static_cast<unsigned long>(abs(rhs) / scale);
	bigint out_bi;
	mpz_pow_ui(out_bi.get_mpz_t(), lhs.bi.get_mpz_t(), exp);
	bigint scale_bi;
	mpz_pow_ui(scale_bi.get_mpz_t(), static_cast<bigint>(scale).get_mpz_t(), exp - 1);
	out_bi /= scale_bi;
	
	if (abs(rhs) != rhs) {
		out_bi = (scale * scale) / out_bi;
	}
	
	return clamp(out_bi);
}

num operator-(const num& n)
{
	return static_cast<bigint>(-n.bi);
}

num& operator++(num& n)
{
	++n.bi;
	
	return n;
}

num& operator--(num& n)
{
	--n.bi;
	
	return n;
}

num operator++(num& n, int)
{
	num nn = n;
	++n;
	
	return nn;
}

num operator--(num& n, int)
{
	num nn = n;
	--n;
	
	return nn;
}

bool crolol::equals(const num& lhs, const num& rhs)
{
	return lhs.bi == rhs.bi;
}

bool crolol::lessthan(const num& lhs, const num& rhs)
{
	return lhs.bi < rhs.bi;
}

bool crolol::lessequals(const num& lhs, const num& rhs)
{
	return lhs.bi <= rhs.bi;
}
