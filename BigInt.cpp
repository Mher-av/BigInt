#include"BigInt.h"
#include<vector>
#include<string.h>

BigInt BigInt::ZERO = BigInt(0);
BigInt BigInt::ONE  = BigInt(1);
BigInt BigInt::TWO  = BigInt(2);
BigInt BigInt::TEN  = BigInt(10);

BigInt::BigInt(){
    digits.clear();
    sign_ = true;
}

BigInt::BigInt(int i_val){
    digits.clear();
    if (i_val >= 0)
        sign_ = true;
    else{
        sign_ = false;
        i_val = -i_val;
    }
    if (i_val == 0){
        digits.push_back(0);
    }
    while (i_val > 0){
        digits.push_back(i_val % 10);
        i_val /= 10;
    }
}

BigInt::BigInt(const char* i_val){
    digits.clear();
    sign_ = true;
    int tmp = 0;
    if (i_val[0] == '-'){
        sign_ = false;
        tmp = 1;
    }
    int len = strlen(i_val);
    for (int i = len - 1; i > 0 + tmp; --i){
        digits.push_back(i_val[i] - '0');
    }
    digits.push_back(i_val[0 + tmp] - '0');
}

BigInt::BigInt(const std::string& i_val){
    digits.clear();
    sign_ = true;
    int tmp = 0;
    if (i_val[0] == '-'){
        sign_ = false;
        tmp = 1;
    }
    for (std::string::size_type i = i_val.size() - 1; i > 0 + tmp; --i){
        digits.push_back(i_val[i] - '0');
    }
    digits.push_back(i_val[0 + tmp] - '0');
}

BigInt BigInt::operator=(const BigInt& i_val) {
    digits.clear();
    sign_ = i_val.sign_;
    for (std::vector<int>::size_type i = 0; i < i_val.digits.size(); ++i) {
        digits.push_back(i_val.digits[i]);
    }
    return *this;
}

BigInt BigInt::operator=(const std::string& i_val){
    digits.clear();
    sign_ = true;
    int tmp = 0;
    if (i_val[0] == '-'){
        sign_ = false;
        tmp = 1;
    }
    for (std::string::size_type i = i_val.size() - 1; i > 0 + tmp; --i) {
        digits.push_back(i_val[i] - '0');
    }
    digits.push_back(i_val[0 + tmp] - '0');
    return *this;
}

BigInt operator+(const BigInt& lhs, const BigInt& rhs) {
    BigInt ret(lhs);
    ret += rhs;
    return ret;
}

BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
    BigInt ret(lhs);
    ret -= rhs;
    return ret;
}

BigInt operator*(const BigInt& lhs, const BigInt& rhs) {
    BigInt ret(lhs);
    ret *= rhs;
    return ret;
}

BigInt operator/(const BigInt& lhs, const BigInt& rhs) {
    BigInt ret(lhs);
    ret /= rhs;
    return ret;
}

BigInt& BigInt::operator+=(const BigInt& i_val){
    if (sign_ == i_val.sign_) {
        while (digits.size() <= i_val.digits.size()){
            digits.push_back(0);
        }
        int carry = 0;
        std::vector<int>::size_type i;
        for (i = 0; i < i_val.digits.size(); ++i){
            digits.at(i) += carry + i_val.digits.at(i);
            if (digits[i] >= 10){
                carry = 1;
                digits[i] -= 10;
            }else
                carry = 0;
        }
        for ( ; i < digits.size(); ++i){
            digits.at(i) += carry + 0;
            if (digits[i] >= 10){
                carry = 1;
                digits[i] -= 10;
            }else
                carry = 0;
        }
    }else{
        if (sign_ == true && i_val.sign_ == false){
            BigInt tmp(i_val);
            tmp.sign_ = true;
            *this -= tmp;
        }else{
            BigInt tmp(*this);
            tmp.sign_ = true;
            *this = i_val - tmp;
        }
    }
    trim();
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& i_val){
    BigInt tmp;
    if (*this < i_val){
        tmp = *this;
        *this = i_val;
        this->sign_ = false;
    }else{
        tmp = i_val;
    }
    int carry = 0;
    std::vector<int>::size_type i;
    for (i = 0; i < tmp.digits.size(); ++i) {
        digits[i] -= carry + tmp.digits[i];
        if (digits[i] < 0){
            carry = 1;
            digits[i] += 10;
        }else
            carry = 0;
    }
    for ( ; i < digits.size(); ++i){
        digits[i] -= carry;
        if (digits[i] < 0){
            carry = 1;
            digits[i] += 10;
        }else
            carry = 0;
    }
    trim();
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& i_val){
    BigInt ret;
    if (this->sign_ == i_val.sign_)
        ret.sign_ = true;
    else
        ret.sign_ = false;
    for (std::vector<int>::size_type i = 0; i < digits.size(); ++i){
        for (std::vector<int>::size_type j = 0; j < i_val.digits.size(); ++j){
            if (i + j < ret.digits.size()){
                ret.digits[i + j] += digits[i] * i_val.digits[j];
            }else{
                ret.digits.push_back(digits[i] * i_val.digits[j]);
            }
        }
    }
    for (std::vector<int>::size_type i = 0; i < ret.digits.size(); ++i){
        if (i + 1 < ret.digits.size()){
            ret.digits[i + 1] += ret.digits[i] / 10;
        }else if (ret.digits[i] >= 10){
            ret.digits.push_back(ret.digits[i] / 10);
        }else{
            break;
        }
        ret.digits[i] %= 10;
    }
    ret.trim();
    *this = ret;
    return *this;
}

BigInt& BigInt::operator/=(const BigInt& i_val){
    if (*this == i_val){
        *this = BigInt::ONE;
        return *this;
    }
    std::cout << *this << std::endl;
    std::cout << i_val << std::endl;

    BigInt ret(0);
    if (sign_ == i_val.sign_)
        ret.sign_ = true;
    else
        ret.sign_ = false;

    BigInt divider(i_val);

    this->sign_ = true;
    divider.sign_ = true;

    if (*this < divider){
        *this = BigInt::ZERO;
        return *this;
    }

    int cnt = 0;
    while (*this > BigInt::ZERO){
        if (*this >= divider){
            *this -= divider;
            ret += pow(BigInt::TEN, cnt);
            divider *= BigInt::TEN;
            cnt++;
        }else{
            divider = i_val;
            cnt = 0;
            if (*this < divider){
                ret.trim();
                *this = ret;
                return *this;
            }
        }
        divider.sign_ = true;
    }

    ret.trim();
    *this = ret;
    return *this;
}
BigInt& BigInt::operator++() {
    *this += BigInt::ONE;
    return *this;
}

BigInt& BigInt::operator--() {
    *this -= BigInt::ONE;
    return *this;
}

const BigInt BigInt::operator++(int) {
    BigInt temp = *this;
    ++(*this);
    return BigInt(temp);
}

const BigInt BigInt::operator--(int) {
    BigInt temp = *this;
    --(*this);
    return BigInt(temp);
}

BigInt& BigInt::operator+() {
    return *this;
}

BigInt& BigInt::operator-() {
    this->sign_ = !(this->sign_);
    return *this;
}

bool operator>(const BigInt& lhs, const BigInt& rhs) {
    return (lhs != rhs) && (rhs < lhs);
}

bool operator>=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs < rhs);
}

bool operator<(const BigInt& lhs, const BigInt& rhs) {
    if (lhs.sign_ == false && rhs.sign_ == true)
        return true;
    if (lhs.sign_ == true && rhs.sign_ == false)
        return false;
    if (lhs.sign_){
        if (lhs.digits.size() < rhs.digits.size())
            return true;
        else if (lhs.digits.size() > rhs.digits.size())
            return false;
    }else{
        if (lhs.digits.size() < rhs.digits.size())
            return false;
        else if (lhs.digits.size() > rhs.digits.size())
            return true;
    }
    for (std::vector<int>::size_type i = lhs.digits.size() - 1; i > 0; --i) {
        if (lhs.digits[i] < rhs.digits[i]){
            if (lhs.sign_)
                return true;
            else
                return false;
        }else if (lhs.digits[i] > rhs.digits[i]){
            if (lhs.sign_)
                return false;
            else
                return true;
        }
    }
    if (lhs.digits[0] < rhs.digits[0]){
        if (lhs.sign_)
            return true;
        else
            return false;
    }
    return false;
}

bool operator<=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs > rhs);
}

bool operator==(const BigInt& lhs, const BigInt& rhs) {
    if (lhs.sign_ != rhs.sign_
        || lhs.digits.size() != rhs.digits.size())
        return false;
    for (std::vector<int>::size_type i = 0; i < lhs.digits.size(); i++){
        if (lhs.digits.at(i) != rhs.digits.at(i))
            return false;
    }
    return true;
}

bool operator!=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs == rhs);
}


std::istream& operator>>(std::istream& in, BigInt& o_val) {
    std::string tmp;
    in >> tmp;
    o_val = tmp;
    return in;
}

std::ostream& operator<<(std::ostream& out, const BigInt& i_val) {
    if (i_val.digits.size() == 0){
        out << "0";
    }else{
        if (i_val.sign_ == false)
            out << '-';
        for (std::vector<int>::size_type i = i_val.digits.size() - 1; i > 0; --i) {
            out << i_val.digits[i];
        }
        out << i_val.digits[0];
    }
    return out;
}

BigInt pow(const BigInt& i_val, const BigInt& i_exp) {
    if (i_exp == BigInt::ZERO || i_val == BigInt::ONE){
        return BigInt::ONE;
    }
    if (i_val == BigInt::ZERO){
        return BigInt::ZERO;
    }

    BigInt i(0);
    BigInt ret(i_val);
    BigInt exp(i_exp);

    exp -= BigInt::ONE;

    while (i < exp){
        ret *= i_val;
        ++i;
    }

    return ret;
}

BigInt pow(const BigInt& i_val, const int i_exp) {
    BigInt exp(i_exp);
    return pow(i_val, exp);
}

std::string BigInt::toString()
{
	std::string str;
	if (sign_ == false)
		str.append("-");
	for (auto it = digits.rbegin(); it != digits.rend(); ++it)
		str.append(std::to_string(*it));
		return str;
}

void BigInt::trim() {
    while (digits.size() > 1 && digits[digits.size() - 1] == 0)
        digits.pop_back();
}