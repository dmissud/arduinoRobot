#include <ArduinoFake.h>
#include <unity.h>

#include "CommandParser.h"

void setUp(void) {
    ArduinoFakeReset();
}

void tearDown(void) {}

// --- Commandes valides ---

void test_parse_valid_servo_command(void) {
    auto result = CommandParser::parse("servo(base,angle90)");
    TEST_ASSERT_TRUE(result.isValid);
    TEST_ASSERT_EQUAL_STRING("servo", result.keyword.c_str());
    TEST_ASSERT_EQUAL_STRING("base", result.name.c_str());
    TEST_ASSERT_EQUAL_STRING("angle90", result.action.c_str());
}

void test_parse_valid_led_command(void) {
    auto result = CommandParser::parse("led(led,on)");
    TEST_ASSERT_TRUE(result.isValid);
    TEST_ASSERT_EQUAL_STRING("led", result.keyword.c_str());
    TEST_ASSERT_EQUAL_STRING("led", result.name.c_str());
    TEST_ASSERT_EQUAL_STRING("on", result.action.c_str());
}

void test_parse_valid_status_command(void) {
    auto result = CommandParser::parse("status(arduino,ok)");
    TEST_ASSERT_TRUE(result.isValid);
    TEST_ASSERT_EQUAL_STRING("status", result.keyword.c_str());
    TEST_ASSERT_EQUAL_STRING("arduino", result.name.c_str());
    TEST_ASSERT_EQUAL_STRING("ok", result.action.c_str());
}

// --- Action vide (valide selon le code : name.length() > 0 suffit) ---

void test_parse_empty_action_is_valid(void) {
    auto result = CommandParser::parse("servo(base,)");
    TEST_ASSERT_TRUE(result.isValid);
    TEST_ASSERT_EQUAL_STRING("servo", result.keyword.c_str());
    TEST_ASSERT_EQUAL_STRING("base", result.name.c_str());
    TEST_ASSERT_EQUAL_STRING("", result.action.c_str());
}

// --- Gestion des espaces ---

void test_parse_inner_spaces_trimmed(void) {
    auto result = CommandParser::parse("servo( base , angle90 )");
    TEST_ASSERT_TRUE(result.isValid);
    TEST_ASSERT_EQUAL_STRING("servo", result.keyword.c_str());
    TEST_ASSERT_EQUAL_STRING("base", result.name.c_str());
    TEST_ASSERT_EQUAL_STRING("angle90", result.action.c_str());
}

void test_parse_keyword_spaces_trimmed(void) {
    auto result = CommandParser::parse("  servo  (base,angle90)");
    TEST_ASSERT_TRUE(result.isValid);
    TEST_ASSERT_EQUAL_STRING("servo", result.keyword.c_str());
}

void test_parse_trailing_space_invalid(void) {
    // Trailing space means endsWith(")") fails
    auto result = CommandParser::parse("servo(base,angle90) ");
    TEST_ASSERT_FALSE(result.isValid);
}

// --- Commandes invalides ---

void test_parse_no_parentheses(void) {
    auto result = CommandParser::parse("servo base angle90");
    TEST_ASSERT_FALSE(result.isValid);
}

void test_parse_no_closing_paren(void) {
    auto result = CommandParser::parse("servo(base,angle90");
    TEST_ASSERT_FALSE(result.isValid);
}

void test_parse_no_comma(void) {
    auto result = CommandParser::parse("servo(base)");
    TEST_ASSERT_FALSE(result.isValid);
}

void test_parse_empty_name_invalid(void) {
    auto result = CommandParser::parse("servo(,angle90)");
    TEST_ASSERT_FALSE(result.isValid);
}

void test_parse_empty_string(void) {
    auto result = CommandParser::parse("");
    TEST_ASSERT_FALSE(result.isValid);
}

// --- Cas limites ---

void test_parse_multiple_commas_first_separates(void) {
    // Only the first comma separates name from action
    auto result = CommandParser::parse("servo(base,a,b,c)");
    TEST_ASSERT_TRUE(result.isValid);
    TEST_ASSERT_EQUAL_STRING("base", result.name.c_str());
    TEST_ASSERT_EQUAL_STRING("a,b,c", result.action.c_str());
}

void test_parse_arbitrary_keyword(void) {
    auto result = CommandParser::parse("foobar(baz,qux)");
    TEST_ASSERT_TRUE(result.isValid);
    TEST_ASSERT_EQUAL_STRING("foobar", result.keyword.c_str());
    TEST_ASSERT_EQUAL_STRING("baz", result.name.c_str());
    TEST_ASSERT_EQUAL_STRING("qux", result.action.c_str());
}

void test_parsed_command_default_invalid(void) {
    CommandParser::ParsedCommand cmd;
    TEST_ASSERT_FALSE(cmd.isValid);
}

void test_parse_keyword_empty_when_paren_first(void) {
    // "(name,action)" => keyword is ""
    auto result = CommandParser::parse("(name,action)");
    TEST_ASSERT_TRUE(result.isValid);
    TEST_ASSERT_EQUAL_STRING("", result.keyword.c_str());
    TEST_ASSERT_EQUAL_STRING("name", result.name.c_str());
    TEST_ASSERT_EQUAL_STRING("action", result.action.c_str());
}

int main(int argc, char** argv) {
    UNITY_BEGIN();

    RUN_TEST(test_parse_valid_servo_command);
    RUN_TEST(test_parse_valid_led_command);
    RUN_TEST(test_parse_valid_status_command);
    RUN_TEST(test_parse_empty_action_is_valid);
    RUN_TEST(test_parse_inner_spaces_trimmed);
    RUN_TEST(test_parse_keyword_spaces_trimmed);
    RUN_TEST(test_parse_trailing_space_invalid);
    RUN_TEST(test_parse_no_parentheses);
    RUN_TEST(test_parse_no_closing_paren);
    RUN_TEST(test_parse_no_comma);
    RUN_TEST(test_parse_empty_name_invalid);
    RUN_TEST(test_parse_empty_string);
    RUN_TEST(test_parse_multiple_commas_first_separates);
    RUN_TEST(test_parse_arbitrary_keyword);
    RUN_TEST(test_parsed_command_default_invalid);
    RUN_TEST(test_parse_keyword_empty_when_paren_first);

    return UNITY_END();
}
