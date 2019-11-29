#include <bitset>
#include <iostream>
#include <iconv.h>
#include <stdexcept>
#include "converter.hpp"

#define ERROR_MSG "transformation FAILED"
#define ERROR -1

using namespace std;

std::u32string u32_string = U"¢";


int main(int argc, char const *argv[])
{

    // from u32string to vector<uint32_t>
    std::vector<uint32_t> original_string;
    for (int i = 0; i < u32_string.length(); i++)
    {
        original_string.push_back(static_cast<uint32_t>(u32_string[i]));
    }

    // show UTF-32
    cout << endl << "Original string (UTF-32):" << endl;
    for (int i = 0; i < original_string.size(); ++i)
        cout << original_string[i] << " - " << bitset<32>(original_string[i]) << endl;

    // convertation UTF-32 -> UTF-8
    std::vector<uint8_t> utf8_string;
    try{
        utf8_string = to_utf8(original_string);
    } catch (const runtime_error& error) {
        cerr << error.what() << endl;
        return ERROR;
    }

    // show UTF-8
    cout << endl << "String converted to UTF-8:" << endl;
    for (int i = 0; i < utf8_string.size(); ++i)
        cout << bitset<8>(utf8_string[i]) << endl;


    // convertation UTF-8 -> UTF-32
    std::vector<uint32_t> utf32_string;
    try {
        utf32_string = from_utf8(utf8_string);
    } catch (const std::runtime_error& error) {
        cerr << error.what() << endl;
        return ERROR;
    }
    // show converted UTF-32
    cout << endl << "String converted back to UTF-32:" << endl;
    for (int i = 0; i < utf32_string.size(); ++i)
        cout << utf32_string[i] << " - " << bitset<32>(utf32_string[i]) << endl;

    // UTF-32 check
    if (original_string != utf32_string) {
        cerr << "transformation from utf8 FAILED" << endl;
        return ERROR;
    }
    else
        cout << endl << "Everything is correct" << endl;

    return 0;
}
