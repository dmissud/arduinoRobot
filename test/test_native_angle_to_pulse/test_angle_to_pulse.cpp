#include <ArduinoFake.h>
#include <unity.h>

// Replicate the angleToPulse formula from ServoController.
// Keep in sync with ServoController.h constants:
//   MIN_PULSE = 100, MAX_PULSE = 650, MIN_ANGLE = 0, MAX_ANGLE = 180
// Formula: map(constrain(angle, 0, 180), 0, 180, 100, 650)

static const int MIN_PULSE = 100;
static const int MAX_PULSE = 650;
static const int MIN_ANGLE = 0;
static const int MAX_ANGLE = 180;

// Real implementation of map() (integer arithmetic, same as Arduino)
static long arduino_map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static long arduino_constrain(long x, long lo, long hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

static uint16_t angleToPulse(int angle) {
    return (uint16_t)arduino_map(
        arduino_constrain(angle, MIN_ANGLE, MAX_ANGLE),
        MIN_ANGLE,
        MAX_ANGLE,
        MIN_PULSE,
        MAX_PULSE
    );
}

void setUp(void) {}
void tearDown(void) {}

void test_angle_0_gives_min_pulse(void) {
    TEST_ASSERT_EQUAL_UINT16(100, angleToPulse(0));
}

void test_angle_180_gives_max_pulse(void) {
    TEST_ASSERT_EQUAL_UINT16(650, angleToPulse(180));
}

void test_angle_90_gives_midpoint(void) {
    // (90 - 0) * (650 - 100) / (180 - 0) + 100 = 90 * 550 / 180 + 100 = 375
    TEST_ASSERT_EQUAL_UINT16(375, angleToPulse(90));
}

void test_angle_45_integer_division(void) {
    // (45 * 550) / 180 + 100 = 24750 / 180 + 100 = 137 + 100 = 237
    TEST_ASSERT_EQUAL_UINT16(237, angleToPulse(45));
}

void test_angle_1_precision(void) {
    // (1 * 550) / 180 + 100 = 550 / 180 + 100 = 3 + 100 = 103
    TEST_ASSERT_EQUAL_UINT16(103, angleToPulse(1));
}

void test_negative_angle_clamped_to_min(void) {
    // constrain(-10, 0, 180) = 0 => pulse = 100
    TEST_ASSERT_EQUAL_UINT16(100, angleToPulse(-10));
}

void test_angle_above_max_clamped(void) {
    // constrain(200, 0, 180) = 180 => pulse = 650
    TEST_ASSERT_EQUAL_UINT16(650, angleToPulse(200));
}

int main(int argc, char** argv) {
    UNITY_BEGIN();

    RUN_TEST(test_angle_0_gives_min_pulse);
    RUN_TEST(test_angle_180_gives_max_pulse);
    RUN_TEST(test_angle_90_gives_midpoint);
    RUN_TEST(test_angle_45_integer_division);
    RUN_TEST(test_angle_1_precision);
    RUN_TEST(test_negative_angle_clamped_to_min);
    RUN_TEST(test_angle_above_max_clamped);

    return UNITY_END();
}
