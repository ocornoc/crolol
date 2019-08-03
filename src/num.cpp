#include <array>
#include <bitset>
#include <cfenv>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include "num.h"

#pragma STDC FENV_ACCESS on

using namespace crolol;
using bigint = std::int64_t;
using std::int64_t;
using l_lims = std::numeric_limits<bigint>;

constexpr static bigint scale = 10000;
constexpr static bigint num_min = l_lims::lowest();
constexpr static bigint num_max = l_lims::max();
constexpr static bigint num_truthy = 1L * scale;
constexpr static bigint num_falsy = 0L * scale;

constexpr static int64_t rawclamp(int64_t left, int64_t mid, int64_t right)
	noexcept
{
	return mid < left ? left : (mid > right ? right : mid);
}

constexpr static int64_t preclamp(int64_t bi) noexcept
{
	return rawclamp(num_min / scale, bi, num_max / scale);
}

num::num(int64_t bi) noexcept
	: bi(bi)
{}

num::num(bool b) noexcept
	: bi(b ? num_truthy : num_falsy)
{}

num crolol::make_numi(int64_t bi) noexcept
{
	if (preclamp(bi) != bi)
		return bi < 0 ? num_min : num_max;
	else return bi * scale;
}

num crolol::make_numf(long double bf) noexcept
{
	return std::llrint(bf * scale);
}

bool crolol::equals(num lhs, num rhs)
{
	return lhs.bi == rhs.bi;
}

bool crolol::lessthan(num lhs, num rhs)
{
	return lhs.bi < rhs.bi;
}

bool crolol::lessequals(num lhs, num rhs)
{
	return lhs.bi <= rhs.bi;
}

int64_t num::getraw() const noexcept
{
	return bi;
}

const num num::min = num_min;
const num num::max = num_max;
const num num::truthy = num_truthy;
const num num::falsy = num_falsy;

num::operator double() const
{
	return static_cast<double>(bi) / scale;
}

num::operator std::string() const
{
	return std::to_string(static_cast<double>(*this));
}

num::operator int64_t() const
{
	return bi / scale;
}

num::operator bool() const
{
	return bi;
}

num& num::operator+=(num rhs)
{
	// If the sign of lhs and rhs aren't the same, we can always add.
	// Same if either args are 0.
	if (std::signbit(bi) != std::signbit(rhs.bi) or bi == 0 or rhs.bi == 0)
		bi += rhs.bi;
	else if (std::signbit(bi)) {
		if (num_min - bi > rhs.bi)
			bi = num_min;
		else bi += rhs.bi;
	} else {
		if (num_max - bi < rhs.bi)
			bi = num_max;
		else bi += rhs.bi;
	}
	
	return *this;
}

num& num::operator-=(num rhs)
{
	// If sgn(lhs) == sgn(rhs) or rhs or lhs == 0, we can subtract.
	// EXCEPT if rhs == num_min, as that would allow 0-(-2^63)=2^63.
	if ((std::signbit(bi) == std::signbit(rhs.bi)) or
		((rhs.bi == 0 or bi == 0) and rhs.bi != num_min))
		bi -= rhs.bi;
	else if (rhs.bi == num_min) *this += num::max;
	else *this += -rhs.bi;
	
	return *this;
}

num& num::operator*=(num rhs)
{
	bool special = false;
	if (bi == 0 * scale or rhs.bi == 0 * scale) {
		bi = 0 * scale;
		special = true;
	} else if (bi == 1 * scale) {
		bi = rhs.bi;
		special = true;
	} else if (rhs.bi == 1 * scale) return *this;
	else if (bi == -1 * scale) {
		*this = -rhs;
		special = true;
	} else if (rhs.bi == -1 * scale) {
		*this = -*this;
		special = true;
	}
	if (special) return *this;
	
	const bool lbisgn = std::signbit(bi);
	const bool rbisgn = std::signbit(rhs.bi);
	// Determines if we negate the result at the end.
	const bool negate = lbisgn != rbisgn;
	int64_t smaller_orig;
	int64_t bigger_orig;
	
	if (abs(*this).bi < abs(rhs).bi) {
		smaller_orig = bi;
		bigger_orig = rhs.bi;
	} else {
		smaller_orig = rhs.bi;
		bigger_orig = bi;
	}
	
	const int64_t smaller = abs(smaller_orig).bi;
	const int64_t bigger = abs(bigger_orig).bi;
	
	// We know lhs and rhs != 0, 1, -1. Thus, if abs(lhs) >= num_max,
	// lhs * rhs will over/underflow, and vice-versa. abs(lhs) clamps to
	// num_max even if lhs == num_min.
	if (bigger == num_max and smaller >= 1 * scale) {
		bi = bigger_orig;
		*this = negate ? -*this : *this;
	// Check if the result /should/ over/underflow.
	} else if (std::log2(bigger) + std::log2(smaller)
		- 2 * std::log2(scale) >= 63)
		bi = negate ? num_min : num_max;
	else {
		const std::bitset<63> smallbits = smaller;
		num acc = 0;
		std::array<std::bitset<smallbits.size()>, smallbits.size()> arr =
			{bigger * smallbits.test(0)};
		
		for (std::size_t i = 1; i < arr.size(); i++) {
			if (smallbits.test(i)) {
				if (arr[i - 1].test(arr[i - 1].size() - 1))
					arr[i] = num_max;
				else {
					arr[i] = bigger;
					arr[i] <<= i;
				}
			}
		}
		
		for (const auto& bs: arr)
			acc += static_cast<int64_t>(bs.to_ullong());
		
		*this = negate ? -acc : acc;
		bi /= scale;
	}
	
	return *this;
}
/*
num& num::operator/=(num rhs)
{
	//bi = clamp((bi * scale) / (rhs.bi * scale));
	
	return *this;
}

num& num::operator%=(num rhs)
{
	//int sgn = mpz_sgn(rhs.bi.get_mpz_t());
	//bi = clamp(bi % rhs.bi * sgn);
	
	return *this;
}
*/
num operator+(num lhs, num rhs)
{
	return lhs += rhs;
}

num operator-(num lhs, num rhs)
{
	return lhs -= rhs;
}

num operator*(num lhs, num rhs)
{
	return lhs *= rhs;
}
/*
num operator/(num lhs, num rhs)
{
	return lhs /= rhs;
}

num operator%(num lhs, num rhs)
{
	return lhs %= rhs;
}
*/
num operator!(num n)
{
	return n.bi == num_falsy ? num::truthy : num::falsy;
}

num operator==(num lhs, num rhs)
{
	return lhs.bi == rhs.bi;
}

num operator!=(num lhs, num rhs)
{
	return !(lhs == rhs);
}

num operator<(num lhs, num rhs)
{
	return lhs.bi < rhs.bi;
}

num operator>(num lhs, num rhs)
{
	return lhs.bi > rhs.bi;
}

num operator>=(num lhs, num rhs)
{
	return !(lhs < rhs);
}

num operator<=(num lhs, num rhs)
{
	return !(lhs > rhs);
}
/*
num crolol::abs(num n)
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
}*/

num operator-(num n)
{
	return 0 - n;
}

num& operator++(num& n)
{
	return n += 1;
}

num& operator--(num& n)
{
	return n -= 1;
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

num crolol::abs(num n)
{
	return n.getraw() < 0 ? -n : n;
}
