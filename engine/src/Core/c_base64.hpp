#pragma once
#ifndef __C_BASE64_
#define __C_BASE64_

#include <string>
#include <map>
#include <cstdint>

namespace qe
{
    std::map<char, int> base64_charmap = {
        {'A', 00}, {'B', 01}, {'C', 02}, {'D', 03}, {'E', 04}, {'F', 05}, {'G', 06}, {'H', 07},
        {'I', 010}, {'J', 011}, {'K', 012}, {'L', 013}, {'M', 014}, {'N', 015}, {'O', 016}, {'P', 017},
        {'Q', 020}, {'R', 021}, {'S', 022}, {'T', 023}, {'U', 024}, {'V', 025}, {'W', 026}, {'X', 027},
        {'Y', 030}, {'Z', 031}, {'a', 032}, {'b', 033}, {'c', 034}, {'d', 035}, {'e', 036}, {'f', 037},
        {'g', 040}, {'h', 041}, {'i', 042}, {'j', 043}, {'k', 044}, {'l', 045}, {'m', 046}, {'n', 047},
        {'o', 050}, {'p', 051}, {'q', 052}, {'r', 053}, {'s', 054}, {'t', 055}, {'u', 056}, {'v', 057},
        {'w', 060}, {'x', 061}, {'y', 062}, {'z', 063}, {'0', 064}, {'1', 065}, {'2', 066}, {'3', 067},
        {'4', 070}, {'5', 071}, {'6', 072}, {'7', 073}, {'8', 074}, {'9', 075}, {'+', 076}, {'/', 077}
    };

    std::string DecodeBase64AsChars(std::string chars) {
        std::string decoded;

        uint32_t shift = 2;

        for(size_t i = 1; i < chars.length(); i++) {
            int32_t first = base64_charmap[chars.c_str()[i - 1]];
            int32_t second = base64_charmap[chars.c_str()[i]];

            //printf("Iter: %d,\tFirst: %d, Second: %d, F shift L by %d: %d, S shift R by %d: %d, F s + S s: %d\n", i, first, second, shift, ((first << shift) << 24) >> 24, 6 - shift, ((second >> 6 - shift) << 24) >> 24, ((first << shift) + (second >> (6 - shift)) << 24) >> 24);

            unsigned char ch = ((first << shift) + (second >> (6 - shift)) << 24) >> 24;

            if(/*((first << shift) << 24) >> 24 >= 0 && ((second >> 6 - shift) << 24) >> 24 >= 0*/shift != 8) {
                decoded += std::to_string((int32_t)ch) + ' ';
                //decoded += ch;
            }

            if(shift >= 8) {
                shift = 0;
            }

            shift += 2;
        }

        return decoded;
    }
} // namespace qe


#endif