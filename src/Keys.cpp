#include "Keys.h"

int16_t HIDUsageKeys::hidKeyboard(uint16_t id) {
    if (id < sizeof HIDKeyboard) {
        return HIDKeyboard[id].USBHID;
    }
    return -1;
}

int16_t HIDUsageKeys::hidKeyboard(const char* code) {
    for (int16_t i=0; i < sizeof HIDKeyboard/sizeof HIDKeyboard[0]; i++) {
        if (strcmp(HIDKeyboard[i].name, code) == 0) {
            return HIDKeyboard[i].USBHID;
        }
    }
    return -1;
}

int16_t HIDUsageKeys::hidKeyboardIndex(const char *code)
{
    for (int16_t i = 0; i < sizeof HIDKeyboard / sizeof HIDKeyboard[0]; i++)
    {
        if (strcmp(HIDKeyboard[i].name, code) == 0)
        {
            return i;
        }
    }
    return -1;
}

const char *HIDUsageKeys::hidKeyboardName(uint16_t id)
{
    for (int16_t i = 0; i < sizeof HIDKeyboard / sizeof HIDKeyboard[0]; i++)
    {
        return HIDKeyboard[id].name;
    }
    return "";
}

int16_t HIDUsageKeys::hidConsumer(uint16_t id) {
    if (id < sizeof HIDConsumer) {
        return HIDConsumer[id].USBHID;
    }
    return -1;
}

int16_t HIDUsageKeys::hidConsumer(const char* code) {
    for (int16_t i=0; i < sizeof HIDConsumer/sizeof HIDConsumer[0]; i++) {
        if (strcmp(HIDConsumer[i].name, code) == 0) {
            return HIDConsumer[i].USBHID;
        }
    }
    return -1;
}

int16_t HIDUsageKeys::hidConsumerIndex(const char *code)
{
    for (int16_t i = 0; i < sizeof HIDConsumer / sizeof HIDConsumer[0]; i++)
    {
        if (strcmp(HIDConsumer[i].name, code) == 0)
        {
            return i;
        }
    }
    return -1;
}

const char *HIDUsageKeys::hidConsumerName(uint16_t id)
{
    for (int16_t i = 0; i < sizeof HIDConsumer / sizeof HIDConsumer[0]; i++)
    {
        return HIDConsumer[id].name;
    }
    return "";
}

int16_t HIDUsageKeys::hidAppLauncher(uint16_t id) {
    if (id < sizeof HIDAppLauncher) {
        return HIDAppLauncher[id].USBHID;
    }
    return -1;
}

int16_t HIDUsageKeys::hidAppLauncher(const char* code) {
    for (int16_t i=0; i < sizeof HIDAppLauncher/sizeof HIDAppLauncher[0]; i++) {
        if (strcmp(HIDAppLauncher[i].name, code) == 0) {
            return HIDAppLauncher[i].USBHID;
        }
    }
    return -1;
}

int16_t HIDUsageKeys::hidAppLauncherIndex(const char *code)
{
    for (int16_t i = 0; i < sizeof HIDAppLauncher / sizeof HIDAppLauncher[0]; i++)
    {
        if (strcmp(HIDAppLauncher[i].name, code) == 0)
        {
            return i;
        }
    }
    return -1;
}

const char *HIDUsageKeys::hidAppLauncherName(uint16_t id)
{
    for (int16_t i = 0; i < sizeof HIDAppLauncher / sizeof HIDAppLauncher[0]; i++)
    {
        return HIDAppLauncher[id].name;
    }
    return "";
}

int16_t HIDUsageKeys::hidAppControl(uint16_t id) {
    if (id < sizeof HIDAppControl) {
        return HIDAppControl[id].USBHID;
    }
    return -1;
}

int16_t HIDUsageKeys::hidAppControl(const char* code) {
    for (int16_t i=0; i < sizeof HIDAppControl/sizeof HIDAppControl[0]; i++) {
        if (strcmp(HIDAppControl[i].name, code) == 0) {
            return HIDAppControl[i].USBHID;
        }
    }
    return -1;
}

int16_t HIDUsageKeys::hidAppControlIndex(const char *code)
{
    for (int16_t i = 0; i < sizeof HIDAppControl / sizeof HIDAppControl[0]; i++)
    {
        if (strcmp(HIDAppControl[i].name, code) == 0)
        {
            return i;
        }
    }
    return -1;
}

const char *HIDUsageKeys::hidAppControlName(uint16_t id)
{
    for (int16_t i = 0; i < sizeof HIDAppControl / sizeof HIDAppControl[0]; i++)
    {
        return HIDAppControl[id].name;
    }
    return "";
}

int16_t HIDUsageKeys::hidInternal(uint16_t id)
{
    if (id < sizeof InternalUsage)
    {
        return InternalUsage[id].USBHID;
    }
    return -1;
}

int16_t HIDUsageKeys::hidInternal(const char *code)
{
    for (int16_t i = 0; i < sizeof InternalUsage / sizeof InternalUsage[0]; i++)
    {
        if (strcmp(InternalUsage[i].name, code) == 0)
        {
            return InternalUsage[i].USBHID;
        }
    }
    return -1;
}

int16_t HIDUsageKeys::hidInternalIndex(const char *code)
{
    for (int16_t i = 0; i < sizeof InternalUsage / sizeof InternalUsage[0]; i++)
    {
        if (strcmp(InternalUsage[i].name, code) == 0)
        {
            return i;
        }
    }
    return -1;
}

const char *HIDUsageKeys::hidInternalName(uint16_t id)
{
    for (int16_t i = 0; i < sizeof InternalUsage / sizeof InternalUsage[0]; i++)
    {
        return InternalUsage[id].name;
    }
    return "";
}

int16_t HIDUsageKeys::getKey(uint8_t type, uint16_t id) {
    switch(type) {
        case TYPE_KEYBOARD:
            return instance().hidKeyboard(id);
        case TYPE_CONSUMER:
            return instance().hidConsumer(id);
        case TYPE_APP_LAUNCHER:
            return instance().hidAppLauncher(id);
        case TYPE_APP_CONTROL:
            return instance().hidAppControl(id);
        case TYPE_INTERNAL:
            return instance().hidInternal(id);
    }
    return -1;
}

int16_t HIDUsageKeys::getKey(const char *type, const char *code)
{
    if (strcmp(type, "KEYBOARD") == 0)
    {
        return instance().hidKeyboard(code);
    }
    else if (strcmp(type, "CONSUMER") == 0)
    {
        return instance().hidConsumer(code);
    }
    else if (strcmp(type, "APP_LAUNCHER") == 0)
    {
        return instance().hidAppLauncher(code);
    }
    else if (strcmp(type, "APP_CONTROL") == 0)
    {
        return instance().hidAppControl(code);
    }
    else if (strcmp(type, "INTERNAL") == 0)
    {
        return instance().hidInternal(code);
    }
    return -1;
}

int16_t HIDUsageKeys::getKeyIndex(uint8_t type, const char *code)
{
    switch (type)
    {
    case TYPE_KEYBOARD:
        return instance().hidKeyboardIndex(code);
    case TYPE_CONSUMER:
        return instance().hidConsumerIndex(code);
    case TYPE_APP_LAUNCHER:
        return instance().hidAppLauncherIndex(code);
    case TYPE_APP_CONTROL:
        return instance().hidAppControlIndex(code);
    case TYPE_INTERNAL:
        return instance().hidInternalIndex(code);
    }
    return -1;
}

const char *HIDUsageKeys::getKeyName(uint8_t type, int16_t id)
{
    switch (type)
    {
    case TYPE_KEYBOARD:
        return instance().hidKeyboardName(id);
    case TYPE_CONSUMER:
        return instance().hidConsumerName(id);
    case TYPE_APP_LAUNCHER:
        return instance().hidAppLauncherName(id);
    case TYPE_APP_CONTROL:
        return instance().hidAppControlName(id);
    case TYPE_INTERNAL:
        return instance().hidInternalName(id);
    }
    return "unknown";
}

const char *HIDUsageKeys::getKeyType(uint8_t type)
{
    switch (type)
    {
    case TYPE_KEYBOARD:
        return "TYPE_KEYBOARD";
    case TYPE_CONSUMER:
        return "TYPE_CONSUMER";
    case TYPE_APP_LAUNCHER:
        return "TYPE_APP_LAUNCHER";
    case TYPE_APP_CONTROL:
        return "TYPE_APP_CONTROL";
    case TYPE_INTERNAL:
        return "TYPE_INTERNAL";
    }
    return "unknown";
}

const uint8_t HIDUsageKeys::getKeyTypeId(const char *type)
{
    if (strcmp(type, "KEYBOARD") == 0)
    {
        return TYPE_KEYBOARD;
    }
    if (strcmp(type, "CONSUMER") == 0)
    {
        return TYPE_CONSUMER;
    }
    if (strcmp(type, "APP_LAUNCHER") == 0)
    {
        return TYPE_APP_LAUNCHER;
    }
    if (strcmp(type, "APP_CONTROL") == 0)
    {
        return TYPE_APP_CONTROL;
    }
    if (strcmp(type, "INTERNAL") == 0)
    {
        return TYPE_INTERNAL;
    }
    return 0;
}