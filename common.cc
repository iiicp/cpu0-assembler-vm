#include "common.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include <bitset>
#include <sstream>
#include "llvm/Support/raw_ostream.h"
namespace cpu0 {
unsigned bits(unsigned n, int from, int to) {
    unsigned int numBits = to - from + 1;
    unsigned int mask = ((1 << numBits) - 1) << from;
    unsigned int result = (n & mask) >> from;
    return result;
}

int signbits(int n, int from, int to) {
    unsigned int numBits = to - from + 1;
    int mask = ((1 << numBits) - 1) << from;
    int result = (n & mask) >> from;

    // 检查最高位是否为负数位
    if (result & (1 << (numBits - 1))) {
        result |= ~((1 << numBits) - 1); // 进行符号扩展
    }

    return result;
}

std::string dup(char ch, int n) {
    return std::string(n, ch);
}

std::string fill(char ch, std::string o, int len) {
    if (abs(len) <= o.size()) {
        return o;
    }
    if (len >= 0) {
        return o + dup(ch, len - o.size());
    } else {
        return dup(ch, -len - o.size()) + o;
    }
}

std::string fill(char ch, int n, int len) {
    return fill(ch, std::to_string(n), len);
}

static std::string convertToBase(int n, int base) {
    // 处理0的情况
    if (n == 0) {
        return "0";
    }

    std::string result;
    
    // 对于有符号整数，处理负数情况
    unsigned int abs_n = static_cast<unsigned int>(n);

    while (abs_n > 0) {
        int remainder = abs_n % base;
        if (remainder < 10) {
            result += '0' + remainder;  // 数字部分
        } else {
            result += 'A' + (remainder - 10);  // 字母部分
        }
        abs_n /= base;
    }

    // 反转结果字符串得到正确的顺序
    std::reverse(result.begin(), result.end());

    return result;
}

std::string base(int n, int b, int len) {
    std::string str = convertToBase(n, b);

    return dup('0', len - str.length()) + str;
}

std::string hex(int n, int len) {
    std::string s = convertToBase(n, 16);
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    if (n < 0) {
        std::string t = fill('F', s, -len);
        return t.substr(t.size()-len);
    }else {
        std::string t = fill('0', s, -len);
        return t.substr(t.size()-len);
    }
}

std::string str2hex(llvm::StringRef str) {
    std::string ret = "";
    for (int i = 0; i < str.size(); ++i) {
        ret += hex(str[i], 2);
    }
    return ret;
}

void test() {
    std::cout << "bits(0xF3A4, 4, 7) = " << hex(bits(0xF3A4, 4, 7), 4) << std::endl;
    std::cout << "dup('x', 3) = " << dup('x', 3) << std::endl;
    std::cout << "fill('0', 35, -5) = " << fill('0', 35, -5) << std::endl;
    std::cout << "base(100, 16, 5) = " << base(100, 16, 5) << std::endl;
    std::cout << "hex(-100, 6) = " << hex(-100, 6) << std::endl;
    std::cout << "str2hex(Hello!) = " << str2hex("Hello!") << std::endl;
}
}