/*
 * MIT License
 *
 * This file is part of the BeeNum (https://github.com/tigertv/BeeNum).
 * Copyright (c) 2020 Max Vetrov(tigertv).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <BeeNum/Brat.h>


namespace BeeNum {

Brat::operator std::string() const {
	return this->toString();
}

std::string Brat::toString() const {
	return base(10);
}

std::string Brat::base(const uint64_t base) const {
	std::string d = denominator.base(base);
	return numerator.base(base) + std::string((d == "1" ? "" : "/"+d));
}

std::string Brat::point(const uint64_t num) const {
    return point(num, 10);
}

#define LISP_PRECISION 10
std::string Brat::lisp_format_string() const {
    std::string s = point(30, 10);
    std::string decimals;
    std::string base;
    bool is_front = true;

    while (!s.empty() && s[s.size() - 1] == '0') {
        s.pop_back();
    }

    while (!s.empty()) {
        char c = s[s.size() - 1];

        if (c == '.') {
            is_front = false;
            s.pop_back();
            continue;
        }

        if (is_front) {
            decimals.insert(std::begin(decimals), c);
        } else {
            base.insert(std::begin(base), c);
        }

        s.pop_back();
    }

    int exponent = 0;

    if (base == "0" || base.empty()) {
        while (decimals.size() > LISP_PRECISION) {
            exponent--;
            decimals.erase(0, 1);
        }

        exponent--;
        base = decimals[0];
        decimals.erase(0, 1);

    } else {
        while (base.size() > LISP_PRECISION) {
            exponent++;
            decimals.insert(decimals.begin(), base[base.size()-1]);
            base.pop_back();
        }
    }

    while (decimals.size() > LISP_PRECISION) {
        decimals.pop_back();
    }

    while ((base == "0" || base.empty()) && exponent < 0 && exponent > -5) {
        decimals.insert(decimals.begin(), '0');
        exponent++;
    }

    return base +
        (decimals.empty() ? "" : "."+decimals) +
        (exponent == 0 ? "" : "e"+std::to_string(exponent));
}

std::string Brat::point(const uint64_t num, const uint64_t base) const {
	Bint a = numerator;
	Bint t = a / denominator;
	a %= denominator;

	if (a < 0) {
		a = -a;
	}

	std::string s = (t == 0 && numerator < 0) ? "-" : "";
	s += t.base(base);
	s = s.substr(0, s.find("::")) + '.';

	uint64_t i;
	// get zeros
	for (i = num; i != 0; --i) {
		a *= base;
		t = a / denominator;
		a %= denominator;
		if (t != 0) break;
		s += '0'; // TODO: replace, it's wrong for alphabet beginning from not '0'
		if (base > 62) s += ':'; // TODO: replace, alphabet's size specific
	}

	// get rest
	Bint b;
	for ( ; i != 0; --i) {
		b *= base;
		b += t;

		a *= base;
		t = a / denominator;
		a %= denominator;
	}

	return s + b.base(base);
}

////////////////////////////////////////////////////////////////////////
//            INPUT-OUTPUT FUNCTIONS
////////////////////////////////////////////////////////////////////////

std::ostream& operator << (std::ostream& strm, const Brat& a) {
	return strm << a.toString();
}
/*
std::istream& operator >> (std::istream& strm, Brat& a) {
    std::string s;
    strm >> s;
    a = s;
    return strm;
}
//*/


} // namespace
