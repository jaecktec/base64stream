//
// Created by jaecktec.eu on 2019-08-12.
//

#include "Base64Stream.h"

void Base64Stream::write_byte(unsigned char byte){
    int a3Counter = this->inputLenCounter % 3;
    this->a3[a3Counter] = byte;

    if (a3Counter == 2) {
        this->a3_to_a4();
        for (int i : this->a4) {
            callback(Base64Internals::b64_alphabet[i], args);
        }
    }
    this->inputLenCounter++;
}

void Base64Stream::finalise() {
    int padding = this->inputLenCounter % 3;
    if (padding == 0) return;
    for (int j = padding; j < 3; j++) {
        a3[j] = 0;
    }
    a3_to_a4();
    for (int k = 0; k <= padding; k++) {
        callback(Base64Internals::b64_alphabet[a4[k]], args);
    }
    for (int j = padding + 1; j < 4; j++) {
        callback('=', args);
    }
}

void Base64Stream::a3_to_a4() {
    a4[0] = (a3[0] & 0xfc) >> 2;
    a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
    a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
    a4[3] = (a3[2] & 0x3f);
}
