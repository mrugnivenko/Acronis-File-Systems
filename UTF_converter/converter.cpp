#include "converter.hpp"
using namespace std;

//return last 6 number to create 8 bit number
static inline uint8_t get_last_6_number(uint32_t x)
{
    return 0b10000000 + (uint8_t)(x & 0b00111111);
}

// if start is 10 then it's a simple block
static inline bool find_last_6_number(uint8_t x)
{
    return ((x & (1 << 7)) && !(x & (1 << 6)));
}


int utf32_to_utf8 (uint32_t utf32_symbol, vector<uint8_t> &result_vector) {
    //we need to know how many sockets are in number
    //the simplest task -  the last 6 numbers are our uint8_t
    if (utf32_symbol <= (uint32_t)ONE_BYTE)
        result_vector.push_back((uint8_t)utf32_symbol);
    //new level - whe have to devide our into 2 parts and do some magic
    else if (utf32_symbol <= (uint32_t)TWO_BYTES) {
        result_vector.push_back((uint8_t)(0b11000000 + (uint8_t)(utf32_symbol >> 1*6)));
        result_vector.push_back(get_last_6_number(utf32_symbol));
    }
    //new level - whe have to devide into 3 parts and do some magic
    else if (utf32_symbol <= (uint32_t)THREE_BYTES) {
        result_vector.push_back((uint8_t)(0b11100000 + (uint8_t)(utf32_symbol >> 2*6)));
        result_vector.push_back(get_last_6_number(utf32_symbol >> 1*6));
        result_vector.push_back(get_last_6_number(utf32_symbol));
    }
    //the latest level - whe have to devide into 4 parts and do some magic
    else if (utf32_symbol <= (uint32_t)FOUR_BYTES) {
        result_vector.push_back((uint8_t)(0b11110000 + (uint8_t)(utf32_symbol >> 3*6)));
        result_vector.push_back(get_last_6_number(utf32_symbol >> 2*6));
        result_vector.push_back(get_last_6_number(utf32_symbol >> 1*6));
        result_vector.push_back(get_last_6_number(utf32_symbol));
    }
    //if there are errors
    else {
        throw runtime_error("Transformation FAILED");
    }

    return OK;
}
// do transformation for every symbol of our string
vector<uint8_t> to_utf8(const vector<uint32_t> &utf32_string) {
    vector<uint8_t> result_vector;
    for (int i = 0; i < utf32_string.size(); ++i) {
        try {
            utf32_to_utf8(utf32_string[i], result_vector);
        } catch (const std::runtime_error& error) {
            throw error;
        }
    }

    return result_vector;
}
//reverse operation
vector<uint32_t> from_utf8(const vector<uint8_t> &utf8_string) {
    vector<uint32_t> result_vector;
    uint32_t t = 0;
    //it’s logical that we need to start with the latest level
    for (int i = 0; i < utf8_string.size();)
    {
        //fourth block
        if ((utf8_string[i] >> 3) == 0b00011110)  {
            t += ((uint32_t)utf8_string[i++] & 0b00000111) << 3*6;
            for (int j = 0; j < 3; ++j) {
                if (find_last_6_number(utf8_string[i]))
                    t += ((uint32_t)utf8_string[i++] & 0b00111111)
                            << (2 - j)*6;
                else {
                    throw runtime_error("Expected 4 bytes");
                }
            }
        }
            //third block
        else if ((utf8_string[i] >> 4) == 0b0001110) {
            t += ((uint32_t)utf8_string[i++] & 0b00001111) << 2*6;
            for (int j = 0; j < 2; ++j) {
                if (find_last_6_number(utf8_string[i]))
                    t += ((uint32_t)utf8_string[i++] & 0b00111111)
                            << (1 - j)*6;
                else {
                   throw runtime_error("Expected 3 bytes");
                }
            }
        }
        //second  block
        else if ((utf8_string[i] >> 5) == 0b000110) {
            t += ((uint32_t)utf8_string[i++] & 0b00011111) << 1*6;
            if (find_last_6_number(utf8_string[i]))
                t += ((uint32_t)utf8_string[i++] & 0b00111111);
            else {
               throw runtime_error("Expected 2 bytes");
            }
        }
        else
            // first block
            t = (uint32_t)utf8_string[i++];
        result_vector.push_back(t);
        t = 0;
    }

    return result_vector;
}
