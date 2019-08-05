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
#include "losslessops.h"
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
	if (bi < 0 != rhs.bi < 0 || bi == 0 || rhs.bi == 0)
		bi += rhs.bi;
	else if (bi < 0) {
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
	if ((bi < 0 == rhs.bi < 0) ||
		((rhs.bi == 0 || bi == 0) && rhs.bi != num_min))
		bi -= rhs.bi;
	else if (rhs.bi == num_min) *this += num::max;
	else *this += -rhs.bi;
	
	return *this;
}

num& num::operator*=(num rhs)
{
	const backend::saferet result = backend::multiply(bi, rhs.bi);
	
	switch (result.flow) {
		case backend::badarg: throw std::invalid_argument("Bad mult");
		case backend::underflow: *this = num::min; return *this;
		case backend::overflow: *this = num::max; return *this;
		default: *this = result.val; return *this;
	}
}

num& num::operator/=(num rhs)
{
	const backend::saferet result = backend::divide(bi, rhs.bi);
	
	switch (result.flow) {
		case backend::badarg: throw std::invalid_argument("Bad div");
		case backend::underflow: *this = num::min; return *this;
		case backend::overflow: *this = num::max; return *this;
		default: *this = result.val; return *this;
	}
}

num& num::operator%=(num rhs)
{
	const bool sign = rhs.bi < 0;
	
	bi = abs(*this).bi % abs(rhs).bi;
	
	if (sign) *this = -*this;
	
	return *this;
}

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

num operator/(num lhs, num rhs)
{
	return lhs /= rhs;
}

num operator%(num lhs, num rhs)
{
	return lhs %= rhs;
}

num operator^(num lhs, num rhs)
{
	const backend::saferet result = backend::pow(lhs.bi, rhs.bi);
	int64_t newv;
	
	switch (result.flow) {
		case backend::badarg: throw std::invalid_argument("Bad pow");
		case backend::underflow: newv = num_min; return newv;
		case backend::overflow: newv = num_max; return newv;
		default: newv = result.val; return newv;
	}
}

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

num operator&&(num lhs, num rhs)
{
	return lhs.getraw() && rhs.getraw();
}

num operator||(num lhs, num rhs)
{
	return lhs.getraw() || rhs.getraw();
}

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

num crolol::sqrt(num n)
{
	return n ^ (0.5 * scale);
}
