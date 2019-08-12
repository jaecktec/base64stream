#include <iostream>
#include <functional>
#include "src/Base64Stream.h"

namespace test_util {
    static const std::string base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";


    static inline bool is_base64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

    std::string base64_encode(char const *bytes_to_encode, int in_len) {
        std::string ret;
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        while (in_len--) {
            char_array_3[i++] = *(bytes_to_encode++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (i = 0; (i < 4); i++)
                    ret += base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (j = 0; (j < i + 1); j++)
                ret += base64_chars[char_array_4[j]];

            while ((i++ < 3))
                ret += '=';

        }

        return ret;

    }
}

typedef struct {
    uint8_t *expectedOutput;
    uint8_t inputLength;
    int counter;
    uint8_t *input;
    int expectedLength;
    std::string encodedInput;
} current_test_args_t;

#define TEST_STRING_1 "Hallo welt!1232 HANS WURasdaST"
#define TEST_STRING_2 "Hallo welt!1232 HANS WURasdaST!"
#define TEST_STRING_3 "Hallo welt!1232 HANS WURasdaST!!"

int test(current_test_args_t testArgs) {
    for (int i = 0; i < testArgs.expectedLength; i++) {
        std::cout << testArgs.expectedOutput[i];
    }
    std::cout << std::endl;


    Base64Stream encoder(
            reinterpret_cast<void *const *>(&testArgs),
            [](uint8_t param2, void *const *arg) {
                auto *testArgs = (current_test_args_t *) arg;

                uint8_t expected = testArgs->expectedOutput[testArgs->counter];
                std::cout << param2;
                if (expected != param2) {
                    std::cerr << std::endl << param2 << " not equal " << expected << " at pos " << testArgs->counter
                              << std::endl;
                    throw std::exception();
                }
                testArgs->counter++;
            });

    for (int j = 0; j < testArgs.inputLength; j++) {
        encoder.write_byte(testArgs.input[j]);
    }
    encoder.finalise();
    std::cout << std::endl;
    if (testArgs.counter != testArgs.expectedLength) {
        std::cerr << "expected length of '" << testArgs.expectedLength << "' but got '" << testArgs.counter << "'"
                  << std::endl;
        return -1;
    }

    return 0;
}

int test_1() {
    current_test_args_t testArgs;
    testArgs.input = (uint8_t *) TEST_STRING_1;
    testArgs.inputLength = sizeof(TEST_STRING_1);
    testArgs.encodedInput = test_util::base64_encode(reinterpret_cast<const char *>(testArgs.input),
                                                     testArgs.inputLength);
    testArgs.expectedOutput = reinterpret_cast<uint8_t *>(&testArgs.encodedInput[0]);
    testArgs.expectedLength = testArgs.encodedInput.length();
    testArgs.counter = 0;

    return test(testArgs);
}
int test_2() {
    current_test_args_t testArgs;
    testArgs.input = (uint8_t *) TEST_STRING_2;
    testArgs.inputLength = sizeof(TEST_STRING_2);
    testArgs.encodedInput = test_util::base64_encode(reinterpret_cast<const char *>(testArgs.input),
                                                     testArgs.inputLength);
    testArgs.expectedOutput = reinterpret_cast<uint8_t *>(&testArgs.encodedInput[0]);
    testArgs.expectedLength = testArgs.encodedInput.length();
    testArgs.counter = 0;

    return test(testArgs);
}
int test_3() {
    current_test_args_t testArgs;
    testArgs.input = (uint8_t *) TEST_STRING_3;
    testArgs.inputLength = sizeof(TEST_STRING_3);
    testArgs.encodedInput = test_util::base64_encode(reinterpret_cast<const char *>(testArgs.input),
                                                     testArgs.inputLength);
    testArgs.expectedOutput = reinterpret_cast<uint8_t *>(&testArgs.encodedInput[0]);
    testArgs.expectedLength = testArgs.encodedInput.length();
    testArgs.counter = 0;

    return test(testArgs);
}

int main(int, char**) {
    if (test_1() != -1) {
        return -1;
    }
    if (test_2() != -1) {
        return -1;
    }
    if (test_3() != -1) {
        return -1;
    }
    return 0;
}