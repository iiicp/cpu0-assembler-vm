#pragma once
#include <string>
#include "llvm/ADT/StringRef.h"

namespace cpu0 {
/**
 * 取 [from, to] bit
 */
unsigned bits(unsigned word, int from, int to);

/**
 * 取 [from, to] bit
 */
int signbits(int word, int from, int to);

/**
 * dup('x', 3) = 'xxx'
 */
std::string dup(char ch, int n);

/**
 * fill(' ', 35, 5) = '35   ';
 * fill('0', 35, -5) = '00035';
 */
std::string fill(char ch, std::string n, int len);

std::string fill(char ch, int n, int len);

/**
 * base(31, 16, 5) = '0001F'
 */
std::string base(int n, int b, int len);

/**
 * hex(31, 5) = '0001F'
 * hex(-3, 5) = 'FFFFD'
 */
std::string hex(int n, int len);

/**
 * str2hex("hello!") = '48656C6C6F21'
 */
std::string str2hex(llvm::StringRef str);

void test();
}