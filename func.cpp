#include "typedefs.h"

int set_bits(Long val){
    val = static_cast<ULong>(val) - ((static_cast<ULong>(val) >> 1) & 0x5555555555555555L);
    val = (static_cast<ULong>(val) & 0x3333333333333333L) + ((static_cast<ULong>(val) >> 2) & 0x3333333333333333L);
    val = (static_cast<ULong>(val) + (static_cast<ULong>(val) >> 4)) & 0x0f0f0f0f0f0f0f0fL;
    val = static_cast<ULong>(val) + (static_cast<ULong>(val) >> 8);
    val = static_cast<ULong>(val) + (static_cast<ULong>(val) >> 16);
    val = static_cast<ULong>(val) + (static_cast<ULong>(val) >> 32);
    return (int)val & 0x7f;
}

int trailing_count_2(Long val) { // zeros following least order set bit
    int acc, temp;
    if (val == 0) return 64;
    int new_val = 63;
    temp = (int)val;
    if (temp != 0) {
        new_val = new_val - 32;
        acc = temp;
    } else {
        acc = (int)(static_cast<ULong>(val) >> 32);
    }
    temp = acc <<16;
    if (temp != 0) {
        new_val = new_val - 16;
        acc = temp;
    }
    temp = acc << 8;
    if (temp != 0) {
        new_val = new_val - 8;
        acc = temp;
    }
    temp = acc << 4;
    if (temp != 0) {
        new_val = new_val - 4;
        acc = temp;
    }
    temp = acc << 2;
    if (temp != 0) {
        new_val = new_val - 2;
        acc = temp;
    }
    return new_val - (static_cast<ULong>(acc << 1) >> 31);
}

int leading_count(Long val){ //Zeros before highest order set bit
    val |= static_cast<ULong>(val) >> 1;
    val |= static_cast<ULong>(val) >> 2;
    val |= static_cast<ULong>(val) >> 4;
    val |= static_cast<ULong>(val) >> 8;
    val |= static_cast<ULong>(val) >> 16;
    val |= static_cast<ULong>(val) >> 32;
    return set_bits(~val);
}

int trailing_count(Long val){
    return set_bits((val & -val) - 1);
}

Long reversed(Long val) {
    val = (static_cast<ULong>(val) & 0x5555555555555555L) << 1 | (static_cast<ULong>(val) >> 1) & 0x5555555555555555L;
    val = (static_cast<ULong>(val) & 0x3333333333333333L) << 2 | (static_cast<ULong>(val) >> 2) & 0x3333333333333333L;
    val = (static_cast<ULong>(val) & 0x0f0f0f0f0f0f0f0fL) << 4 | (static_cast<ULong>(val) >> 4) & 0x0f0f0f0f0f0f0f0fL;
    val = (static_cast<ULong>(val) & 0x00ff00ff00ff00ffL) << 8 | (static_cast<ULong>(val) >> 8) & 0x00ff00ff00ff00ffL;
    val = (val << 48) | ((val & 0xffff0000L) << 16) | ((static_cast<ULong>(val) >> 16) & 0xffff0000L) | (static_cast<ULong>(val) >> 48);
    return val;
}