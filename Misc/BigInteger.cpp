// author: 代号4101
// link: https://blog.csdn.net/code4101/article/details/38705155
#include <cstdio>
#include <string>
#include <vector>
#include <cassert>
#include <cstring>
#include <iostream>
#include <algorithm>

struct BigInteger {
    typedef long long LL;
    static const int BASE = 100000000, WIDTH = 8;

    std::vector<int> s;

    BigInteger(LL num = 0) { *this = num; }
    BigInteger(std::string str) { *this = str; }

    BigInteger& clean() { while(!s.back() && s.size() > 1) s.pop_back(); return *this; }

    inline int bsearch(const BigInteger& b, const BigInteger& m) const {
        int L = 0, R = BASE-1, x;
        while (1) {
            x = (L+R)>>1;
            if (b*x <= m) { if (b*(x+1)>m) return x; else L = x; }
            else R = x;
        }
    }
    
    BigInteger& operator = (LL num) {
        s.clear();
        do {
            s.push_back( int(num % BASE) );
            num /= BASE;
        } while (num > 0);
        return *this;
    }
    BigInteger& operator = (const std::string& str) {
        s.clear();
        int x, len = int(str.length() - 1) / WIDTH + 1;
        for (int i = 0; i < len; i++) {
            int end = (int) str.length() - i*WIDTH;
            int start = std::max(0, end - WIDTH);
            sscanf(str.substr(start, end-start).c_str(), "%d", &x);
            s.push_back(x);
        }
        return this->clean();
    }

    BigInteger operator + (const BigInteger& b) const {
        BigInteger c; c.s.clear();
        for (int i = 0, g = 0; ; i++) {
            if (g == 0 && i >= (int) s.size() && i >= (int) b.s.size()) break;
            int x = g;
            if (i < (int) s.size()) x += s[i];
            if (i < (int) b.s.size()) x += b.s[i];
            c.s.push_back(x % BASE);
            g = x / BASE;
        }
        return c;
    }
    BigInteger operator - (const BigInteger& b) const {
        assert(b <= *this);
        BigInteger c; c.s.clear();
        for (int i = 0, g = 0; ; i++) {
            if (g == 0 && i >= (int) s.size() && i >= (int) b.s.size()) break;
            int x = s[i] + g;
            if (i < (int) b.s.size()) x -= b.s[i];
            if (x < 0) { g = -1; x += BASE; } else g = 0;
            c.s.push_back(x);
        }
        return c.clean();
    }
    BigInteger operator * (const BigInteger& b) const {
        int i, j; LL g;
        std::vector<LL> v(s.size() + b.s.size(), 0);
        BigInteger c; c.s.clear();
        for(i = 0; i < (int) s.size(); i++)
			for(j = 0; j < (int) b.s.size(); j++)
				v[i+j] += LL(s[i]) * b.s[j];
        for (i = 0, g = 0; ; i++) {
            if (g ==0 && i >= (int) v.size()) break;
            LL x = v[i] + g;
            c.s.push_back( int(x % BASE) );
            g = x / BASE;
        }
        return c.clean();
    }
    BigInteger operator / (const int& num) {
    	assert(num > 0);
    	BigInteger c;
        c.s.clear(); c.s.resize(s.size()); 
    	LL g = 0;
    	for (int i = (int) s.size()-1; i >= 0; i--) {
    		g = g * BASE + s[i];
    		c.s[i] = int(g / num);
    		g %= num;
        }
    	return c.clean();
    }
    BigInteger operator / (const BigInteger& b) const {
        assert(b > 0);
        BigInteger c = *this;
        BigInteger m;
        for (int i = (int) s.size()-1; i >= 0; i--) {
            m = m * BASE + s[i];
            c.s[i] = bsearch(b, m);
			m -= b*c.s[i];
        }
        return c.clean();
    }
    BigInteger operator % (const BigInteger& b) const {
        BigInteger c = *this;
        BigInteger m;
        for (int i = (int) s.size()-1; i >= 0; i--) {
            m = m * BASE + s[i];
            c.s[i] = bsearch(b, m);
            m -= b*c.s[i];
        }
        return m;
    }

    BigInteger& operator += (const BigInteger& b) { return *this = *this + b; }
    BigInteger& operator -= (const BigInteger& b) { return *this = *this - b; }
    BigInteger& operator *= (const BigInteger& b) { return *this = *this * b; }
    BigInteger& operator /= (const BigInteger& b) { return *this = *this / b; }
    BigInteger& operator %= (const BigInteger& b) { return *this = *this % b; }

    bool operator < (const BigInteger& b) const {
        if (s.size() != b.s.size()) return s.size() < b.s.size();
        for (int i = (int) s.size()-1; i >= 0; i--)
            if (s[i] != b.s[i]) return s[i] < b.s[i];
        return false;
    }

    bool operator >  (const BigInteger& b) const { return b < *this; }
    bool operator <= (const BigInteger& b) const { return !(b < *this); }
    bool operator >= (const BigInteger& b) const { return !(*this < b); }
    bool operator != (const BigInteger& b) const { return b < *this || *this < b; }
    bool operator == (const BigInteger& b) const { return !(b < *this) && !(b > *this); }

    friend std::ostream& operator << (std::ostream &out, const BigInteger& x) {
		out << x.s.back();
		for(int i = (int) x.s.size()-2; i >= 0; i--) {
			char buf[20];
			sprintf(buf, "%08d", x.s[i]);
			for(int j = 0; j < (int) strlen(buf); j++) out << buf[j];
		}
		return out;
	}

	friend std::istream& operator >> (std::istream &in, BigInteger& x) {
		std::string s;
		if(!(in >> s)) return in;
		x = s;
		return in;
	}
};
