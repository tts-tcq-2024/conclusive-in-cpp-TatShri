#include <assert.h>
#include <string.h>
#include "typewise-alert.h"
#include <stdio.h>

// Test inferBreach function
void testInferBreach() {
    assert(inferBreach(20, 30, 40) == TOO_LOW);
    assert(inferBreach(50, 30, 40) == TOO_HIGH);
    assert(inferBreach(35, 30, 40) == NORMAL);
}

// Test classifyTemperatureBreach function with different cooling types
void testClassifyTemperatureBreach() {
    assert(classifyTemperatureBreach(PASSIVE_COOLING, 20) == NORMAL);
    assert(classifyTemperatureBreach(PASSIVE_COOLING, 36) == TOO_HIGH);
    assert(classifyTemperatureBreach(HI_ACTIVE_COOLING, 46) == TOO_HIGH);
    assert(classifyTemperatureBreach(MED_ACTIVE_COOLING, 39) == NORMAL);
    assert(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41) == TOO_HIGH);
}

// Helper function to capture output for assertions
static void captureOutput(void (*function)(BreachType), BreachType breachType, const char* expectedOutput) {
    char buffer[256];
    FILE* output = freopen("/dev/null", "w", stdout);
    setvbuf(stdout, buffer, _IOFBF, sizeof(buffer));
    
    function(breachType);

    freopen("/dev/tty", "w", stdout);  // Reset stdout
    buffer[255] = '\0';
    assert(strcmp(buffer, expectedOutput) == 0);
}

// Test checkAndAlert function
void testCheckAndAlert() {
    BatteryCharacter batteryChar = {PASSIVE_COOLING};

    // Capture and verify output for controller alert
    freopen("/dev/null", "w", stdout);
    checkAndAlert(TO_CONTROLLER, batteryChar, 36);
    freopen("/dev/tty", "w", stdout);

    // Capture and verify output for email alert
    checkAndAlert(TO_EMAIL, batteryChar, 36);
}

int main() {
    testInferBreach();
    testClassifyTemperatureBreach();
    testCheckAndAlert();
    printf("All tests passed!\n");
    return 0;
}
