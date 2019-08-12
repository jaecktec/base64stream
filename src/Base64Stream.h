//
// Created by jaecktec.eu on 2019-08-12.
//

#ifndef BASE64STREAM_BASE64STREAM_HPP
#define BASE64STREAM_BASE64STREAM_HPP

#include <functional>

namespace Base64Internals
{
    const char b64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz"
                                "0123456789+/";
}

class Base64Stream
{
private:
    unsigned char a3[3] = {0x00, 0x00, 0x00};
    unsigned char a4[4] = {0x00, 0x00, 0x00, 0x00};
    unsigned int inputLenCounter = 0;
    void a3_to_a4();
    const std::function<void(uint8_t, void *const *)> &callback;
    void *const *args;

public:
    Base64Stream(
            void *const *args,
            const std::function<void(uint8_t, void *const *)> &callback)
            :callback(callback), args(args){}

    void write_byte(unsigned char byte);
    void finalise();
};

#endif //BASE64STREAM_BASE64STREAM_HPP
