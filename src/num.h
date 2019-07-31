#ifndef CROLOL_NUM_H
#define CROLOL_NUM_H

#include <string>
#include <gmp.h>
#include <gmpxx.h>

namespace crolol {
	class num;
	
	bool equals(const num& lhs, const num& rhs);
	bool lessthan(const num& lhs, const num& rhs);
	bool lessequals(const num& lhs, const num& rhs);
	
	num abs(const num& n);
	num sqrt(const num& n);
	num sin(const num& n);
	num cos(const num& n);
	num tan(const num& n);
	num arcsin(const num& n);
	num arccos(const num& n);
	num arctan(const num& n);
}

// In order: addition, subtraction, multiplication, division, modulo, 
// and exponentiation.
crolol::num operator+(crolol::num lhs, const crolol::num& rhs);
crolol::num operator-(crolol::num lhs, const crolol::num& rhs);
crolol::num operator*(crolol::num lhs, const crolol::num& rhs);
crolol::num operator/(crolol::num lhs, const crolol::num& rhs);
crolol::num operator%(crolol::num lhs, const crolol::num& rhs);
crolol::num operator^(const crolol::num& lhs, const crolol::num& rhs);
// Unary negation.
crolol::num operator-(const crolol::num& n);
// Logical NOT. Returns truthy or falsy.
crolol::num operator!(const crolol::num& n);
// Returns truthy or falsy.
crolol::num operator==(const crolol::num& lhs, const crolol::num& rhs);
crolol::num operator!=(const crolol::num&, const crolol::num&);
crolol::num operator<(const crolol::num&, const crolol::num&);
crolol::num operator<=(const crolol::num&, const crolol::num&);
crolol::num operator>(const crolol::num&, const crolol::num&);
crolol::num operator>=(const crolol::num&, const crolol::num&);
crolol::num operator&&(const crolol::num& lhs, const crolol::num& rhs);
crolol::num operator||(const crolol::num& lhs, const crolol::num& rhs);
// Prefix/suffix inc/dec.
crolol::num& operator++(crolol::num& n);
crolol::num& operator--(crolol::num& n);
crolol::num operator++(crolol::num& n, int);
crolol::num operator--(crolol::num& n, int);

class crolol::num {
	using bigint = mpz_class;
	
	public:
		num(const bigint& bi);
		num(bigint&& bi);
		num(const bool b);
		
		explicit operator std::string() const;
		explicit operator long() const;
		explicit operator unsigned long() const;
		explicit operator double() const;
		explicit operator bool() const;
		
		// In order: addition, subtraction, multiplication, division,
		// and modulo.
		crolol::num& operator+=(const crolol::num&);
		crolol::num& operator-=(const crolol::num&);
		crolol::num& operator*=(const crolol::num&);
		crolol::num& operator/=(const crolol::num&);
		crolol::num& operator%=(const crolol::num&);
		
		num(const num&) = default;
		num(num&&) = default;
		num& operator=(const num&) = default;
		num& operator=(num&&) = default;
		~num() = default;
		
		const static num min;
		const static num max;
		const static num truthy;
		const static num falsy;
		
		friend num (::operator+)(num lhs, const num& rhs);
		friend num (::operator-)(num lhs, const num& rhs);
		friend num (::operator*)(num lhs, const num& rhs);
		friend num (::operator/)(num lhs, const num& rhs);
		friend num (::operator%)(num lhs, const num& rhs);
		friend num (::operator^)(const num& lhs, const num& rhs);
		friend num (::operator-)(const num& n);
		friend num (::operator!)(const num& n);
		friend num (::operator==)(const num& lhs, const num& rhs);
		friend num (::operator<)(const num&, const num&);
		friend num (::operator>)(const num&, const num&);
		friend num& (::operator++)(num& n);
		friend num& (::operator--)(num& n);
		friend num (::operator++)(num& n, int);
		friend num (::operator--)(num& n, int);
		
		friend bool equals(const num& lhs, const num& rhs);
		friend bool lessthan(const num& lhs, const num& rhs);
		friend bool lessequals(const num& lhs, const num& rhs);
		
	private:
		num(const bigint& bi, int);
		
		bigint bi = 0;
};

#endif
