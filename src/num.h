#ifndef CROLOL_NUM_H
#define CROLOL_NUM_H

#include <cstdint>
#include <string>

namespace crolol {
	class num;
	
	bool equals(num, num);
	bool lessthan(num, num);
	bool lessequals(num, num);
	
	num abs(num);
	num sqrt(num);
	num sin(num);
	num cos(num);
	num tan(num);
	num asin(num);
	num acos(num);
	num atan(num);
	
	num make_numi(std::int64_t i) noexcept;
	num make_numf(long double ld) noexcept;
}

// In order: addition, subtraction, multiplication, division, modulo, 
// and exponentiation.
crolol::num operator+(crolol::num, crolol::num);
crolol::num operator-(crolol::num, crolol::num);
crolol::num operator*(crolol::num, crolol::num);
crolol::num operator/(crolol::num, crolol::num);
crolol::num operator%(crolol::num, crolol::num);
crolol::num operator^(crolol::num, crolol::num);
// Unary negation.
crolol::num operator-(crolol::num);
// Logical NOT. Returns truthy or falsy.
crolol::num operator!(crolol::num);
// Returns truthy or falsy.
crolol::num operator==(crolol::num, crolol::num);
crolol::num operator!=(crolol::num, crolol::num);
crolol::num operator<(crolol::num, crolol::num);
crolol::num operator<=(crolol::num, crolol::num);
crolol::num operator>(crolol::num, crolol::num);
crolol::num operator>=(crolol::num, crolol::num);
crolol::num operator&&(crolol::num, crolol::num);
crolol::num operator||(crolol::num, crolol::num);
// Prefix/suffix inc/dec.
crolol::num& operator++(crolol::num&);
crolol::num& operator--(crolol::num&);
crolol::num operator++(crolol::num&, int);
crolol::num operator--(crolol::num&, int);

class crolol::num {
	public:
		constexpr num(const std::int64_t bi) noexcept
			: bi(bi) {};
		explicit num(bool) noexcept;
		
		explicit operator std::string() const;
		explicit operator std::int64_t() const;
		explicit operator long double() const;
		explicit operator bool() const;
		
		// In order: addition, subtraction, multiplication, division,
		// and modulo.
		crolol::num& operator+=(crolol::num);
		crolol::num& operator-=(crolol::num);
		crolol::num& operator*=(crolol::num);
		crolol::num& operator/=(crolol::num);
		crolol::num& operator%=(crolol::num);
		
		constexpr num(const num&) noexcept = default;
		constexpr num(num&&) noexcept = default;
		num& operator=(const num&) noexcept = default;
		num& operator=(num&&) noexcept = default;
		~num() noexcept = default;
		
		static num min() noexcept;
		static num max() noexcept;
		static num truthy() noexcept;
		static num falsy() noexcept;
		
		friend num (::operator+)(num, num);
		friend num (::operator-)(num, num);
		friend num (::operator*)(num, num);
		friend num (::operator/)(num, num);
		friend num (::operator%)(num, num);
		friend num (::operator^)(num, num);
		friend num (::operator-)(num);
		friend num (::operator!)(num);
		friend num (::operator==)(num, num);
		friend num (::operator<)(num, num);
		friend num (::operator>)(num, num);
		
		friend bool equals(num, num);
		friend bool lessthan(num, num);
		friend bool lessequals(num, num);
		
		std::int64_t getraw() const noexcept;
		
	private:
		std::int64_t bi = 0;
};

#endif
