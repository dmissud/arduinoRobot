#include <ArduinoFake.h>
#include <unity.h>

#include "Application.h"
#include "CommandParser.h"
#include "Component.h"

using namespace fakeit;

// Concrete mock component for testing
class MockComponent : public Component {
public:
    String m_keyword;
    bool m_processResult;
    bool m_processCalled;

    MockComponent(const String& keyword, bool processResult)
        : m_keyword(keyword), m_processResult(processResult), m_processCalled(false) {}

    String getKeywords() const override {
        return m_keyword;
    }

    bool processCommand(const CommandParser::ParsedCommand& command) override {
        m_processCalled = true;
        return m_processResult;
    }
};

void setUp(void) {
    ArduinoFakeReset();
    When(OverloadedMethod(ArduinoFake(Serial), println, size_t(const char*))).AlwaysReturn(0);
    When(OverloadedMethod(ArduinoFake(Serial), println, size_t(const String&))).AlwaysReturn(0);
}

void tearDown(void) {}

// --- addComponent tests ---

void test_add_component_returns_true(void) {
    Application app;
    MockComponent comp("servo", true);
    TEST_ASSERT_TRUE(app.addComponent(&comp));
}

void test_add_component_refuses_after_max(void) {
    Application app;
    MockComponent components[MAX_COMPONENTS] = {
        {"c0", true}, {"c1", true}, {"c2", true}, {"c3", true}, {"c4", true},
        {"c5", true}, {"c6", true}, {"c7", true}, {"c8", true}, {"c9", true}
    };

    for (int i = 0; i < MAX_COMPONENTS; i++) {
        TEST_ASSERT_TRUE(app.addComponent(&components[i]));
    }

    MockComponent extra("extra", true);
    TEST_ASSERT_FALSE(app.addComponent(&extra));
}

// --- run / routing tests ---

void test_run_routes_to_matching_component(void) {
    Application app;
    MockComponent comp("servo", true);
    app.addComponent(&comp);

    CommandParser::ParsedCommand cmd;
    cmd.keyword = "servo";
    cmd.name = "base";
    cmd.action = "angle90";
    cmd.isValid = true;

    app.run(cmd);

    TEST_ASSERT_TRUE(comp.m_processCalled);
}

void test_run_skips_non_matching_component(void) {
    Application app;
    MockComponent led("led", true);
    MockComponent servo("servo", true);
    app.addComponent(&led);
    app.addComponent(&servo);

    CommandParser::ParsedCommand cmd;
    cmd.keyword = "servo";
    cmd.name = "base";
    cmd.action = "angle90";
    cmd.isValid = true;

    app.run(cmd);

    TEST_ASSERT_FALSE(led.m_processCalled);
    TEST_ASSERT_TRUE(servo.m_processCalled);
}

void test_run_unrecognized_prints_message(void) {
    Application app;

    CommandParser::ParsedCommand cmd;
    cmd.keyword = "unknown";
    cmd.name = "x";
    cmd.action = "y";
    cmd.isValid = true;

    app.run(cmd);

    Verify(OverloadedMethod(ArduinoFake(Serial), println, size_t(const char*))).Once();
}

void test_run_fallthrough_on_reject(void) {
    // First component matches keyword but rejects (returns false),
    // second component with same keyword accepts.
    Application app;
    MockComponent rejecter("servo", false);
    MockComponent accepter("servo", true);
    app.addComponent(&rejecter);
    app.addComponent(&accepter);

    CommandParser::ParsedCommand cmd;
    cmd.keyword = "servo";
    cmd.name = "base";
    cmd.action = "angle90";
    cmd.isValid = true;

    app.run(cmd);

    TEST_ASSERT_TRUE(rejecter.m_processCalled);
    TEST_ASSERT_TRUE(accepter.m_processCalled);
}

int main(int argc, char** argv) {
    UNITY_BEGIN();

    RUN_TEST(test_add_component_returns_true);
    RUN_TEST(test_add_component_refuses_after_max);
    RUN_TEST(test_run_routes_to_matching_component);
    RUN_TEST(test_run_skips_non_matching_component);
    RUN_TEST(test_run_unrecognized_prints_message);
    RUN_TEST(test_run_fallthrough_on_reject);

    return UNITY_END();
}
