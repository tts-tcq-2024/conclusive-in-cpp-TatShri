#include "typewise-alert.h"
#include <stdio.h>

// Define a structure to hold cooling limits
typedef struct {
    CoolingType coolingType;
    int lowerLimit;
    int upperLimit;
} CoolingLimits;

static const CoolingLimits coolingLimits[] = {
    {PASSIVE_COOLING, 0, 35},
    {HI_ACTIVE_COOLING, 0, 45},
    {MED_ACTIVE_COOLING, 0, 40},
};

// Helper function to get limits based on cooling type
static CoolingLimits getLimitsForCoolingType(CoolingType coolingType) {
    for (int i = 0; i < sizeof(coolingLimits) / sizeof(CoolingLimits); ++i) {
        if (coolingLimits[i].coolingType == coolingType) {
            return coolingLimits[i];
        }
    }
    // Default case: return 0, 0 limits if cooling type is unknown
    return (CoolingLimits){coolingType, 0, 0};
}

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
    if(value < lowerLimit) {
        return TOO_LOW;
    }
    if(value > upperLimit) {
        return TOO_HIGH;
    }
    return NORMAL;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    CoolingLimits limits = getLimitsForCoolingType(coolingType);
    return inferBreach(temperatureInC, limits.lowerLimit, limits.upperLimit);
}

// Delegate alert sending to specific functions to reduce complexity
typedef void (*AlertFunction)(BreachType breachType);

static void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breachType);
}

static void sendToEmail(BreachType breachType) {
    const char* recipient = "a.b@c.com";
    if (breachType != NORMAL) {
        printf("To: %s\n", recipient);
        printf("Hi, the temperature is %s\n", breachType == TOO_LOW ? "too low" : "too high");
    }
}

static AlertFunction getAlertFunction(AlertTarget alertTarget) {
    if (alertTarget == TO_CONTROLLER) {
        return sendToController;
    } else {
        return sendToEmail;
    }
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
    AlertFunction alertFunc = getAlertFunction(alertTarget);
    alertFunc(breachType);
}
