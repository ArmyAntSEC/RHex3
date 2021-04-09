#include <unity.h>
#include "GetPowerForFreeSpeed.h"

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_function_calculator_addition(void) {
    TEST_ASSERT_EQUAL( 20, GetPowerForFreeSpeed(780) );
    TEST_ASSERT_EQUAL( 24, GetPowerForFreeSpeed(1363) );
    TEST_ASSERT_EQUAL( 32, GetPowerForFreeSpeed(2145) );
    TEST_ASSERT_EQUAL( 48, GetPowerForFreeSpeed(3472) );
    TEST_ASSERT_EQUAL( 64, GetPowerForFreeSpeed(4507) );
    TEST_ASSERT_EQUAL( 128, GetPowerForFreeSpeed(6509) );
    TEST_ASSERT_EQUAL( 255, GetPowerForFreeSpeed(7735) );    
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(test_function_calculator_addition);
    UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);
    
    process();
}

void loop() {
}

#else

int main(int argc, char **argv) {
    process();
    return 0;
}

#endif