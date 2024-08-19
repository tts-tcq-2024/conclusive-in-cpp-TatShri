#include <gtest/gtest.h>
#include <assert.h>
#include "typewise-alert.h"

// Test inferBreach function
void testInferBreach() {
    assert(inferBreach(20, 30, 40) == TOO_LOW);
    assert(inferBreach(50, 30, 40) == TOO_HIGH);
    assert(inferBreach(35, 30, 40) == NORMAL);
}

// Test classifyTemperatureBreach function
void testClassifyTemperatureBreach() {
    assert(classifyTemperatureBreach(PASSIVE_COOLING, 20) == NORMAL);
    assert(classifyTemperatureBreach(HI_ACTIVE_COOLING, 50) == TOO_HIGH);
    assert(classifyTemperatureBreach(MED_ACTIVE_COOLING, 39) == NORMAL);
}

// Test checkAndAlert function
void testCheckAndAlert() {
    BatteryCharacter batteryChar = {PASSIVE_COOLING};

    // Redirect output to a buffer for checking controller alerts
    freopen("/dev/null", "w", stdout); // To suppress the output in the test environment
    checkAndAlert(TO_CONTROLLER, batteryChar, 20);
    checkAndAlert(TO_EMAIL, batteryChar, 50);
    freopen("/dev/tty", "w", stdout);  // Reset stdout
}

int main() {
    testInferBreach();
    testClassifyTemperatureBreach();
    testCheckAndAlert();
    printf("All tests passed!\n");
    return 0;
}
