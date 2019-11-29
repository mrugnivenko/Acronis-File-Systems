#include <cstdint>
#include <bitset>
#include <iostream>
#include <vector>
#include <stdexcept>
#define ERROR -1
#define OK 0

#define ONE_BYTE 0x7f
#define TWO_BYTES 0x7ff
#define THREE_BYTES 0xffff
#define FOUR_BYTES 0x10ffff

std::vector<uint8_t>to_utf8(const std::vector<uint32_t> &x);
std::vector<uint32_t>from_utf8(const std::vector<uint8_t> &x);

