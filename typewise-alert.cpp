#include "typewise-alert.h"
#include <stdio.h>

// Data-driven approach to define limits
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

// Function to get limits based on cooling type
CoolingLimits getLimitsForCoolingType(CoolingType coolingType) {
    for (int i = 0; i < sizeof(coolingLimits) / sizeof(CoolingLimits); ++i) {
        if (coolingLimits[i].coolingType == coolingType) {
            return coolingLimits[i];
        }
    }
    // Return default limits if not found (error case)
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

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
    switch(alertTarget) {
        case TO_CONTROLLER:
            sendToController(breachType);
            break;
        case TO_EMAIL:
            sendToEmail(breachType);
            break;
    }
}

void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
    const char* recipient = "a.b@c.com";
    if (breachType != NORMAL) {
        printf("To: %s\n", recipient);
        printf("Hi, the temperature is %s\n", breachType == TOO_LOW ? "too low" : "too high");
    }
}
