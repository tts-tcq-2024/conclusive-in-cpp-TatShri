#include <assert.h>
#include "typewise-alert.h"

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

// Test sendToController function
void testSendToController() {
    // Capture and verify output for different breach types
    captureOutput(sendToController, TOO_LOW, "feed : 1\n");
    captureOutput(sendToController, TOO_HIGH, "feed : 2\n");
    captureOutput(sendToController, NORMAL, "feed : 0\n");
}

// Test sendToEmail function
void testSendToEmail() {
    // Capture and verify output for different breach types
    captureOutput(sendToEmail, TOO_LOW, "To: a.b@c.com\nHi, the temperature is too low\n");
    captureOutput(sendToEmail, TOO_HIGH, "To: a.b@c.com\nHi, the temperature is too high\n");
    captureOutput(sendToEmail, NORMAL, "");  // No output for NORMAL
}

// Test checkAndAlert function
void testCheckAndAlert() {
    BatteryCharacter batteryChar = {PASSIVE_COOLING};

    // Test sending alerts to controller
    checkAndAlert(TO_CONTROLLER, batteryChar, 20);  
    checkAndAlert(TO_CONTROLLER, batteryChar, 36);  

    // Test sending alerts via email
    checkAndAlert(TO_EMAIL, batteryChar, 20);  
    checkAndAlert(TO_EMAIL, batteryChar, 36);  
}

int main() {
    testInferBreach();
    testClassifyTemperatureBreach();
    testSendToController();
    testSendToEmail();
    testCheckAndAlert();
    printf("All tests passed!\n");
    return 0;
}
