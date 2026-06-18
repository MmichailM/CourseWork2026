#include "Solver.h"

vector<int> Solver::utf8_to_codepoints(const string &str) {
    vector<int> res;
    for (size_t i = 0; i < str.size(); ) {
        unsigned char c = str[i];
        int codepoint = 0;
        int len = 0;

        if (c <= 0x7F) { codepoint = c; len = 1; }           // 1 байт (английские буквы, цифры)
        else if ((c & 0xE0) == 0xC0) { codepoint = c & 0x1F; len = 2; } // 2 байта (кириллица)
        else if ((c & 0xF0) == 0xE0) { codepoint = c & 0x0F; len = 3; } // 3 байта
        else if ((c & 0xF8) == 0xF0) { codepoint = c & 0x07; len = 4; } // 4 байта
        else { i++; continue; } // Некорректный байт

        if (i + len > str.size()) break;

        for (int j = 1; j < len; j++) codepoint = (codepoint << 6) | (str[i + j] & 0x3F);
        res.push_back(codepoint);
        i += len;
    }
    return res;
}
