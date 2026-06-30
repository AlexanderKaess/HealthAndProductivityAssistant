#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "AppSettings.h"
#include "AppController.h"
#include "InactivityWatcher.h"
#include "ISoundManager.h"
#include "IThemeManager.h"

class MockAppSettings : public AppSettings
{
    Q_OBJECT
public:
    explicit MockAppSettings(QObject* parent = nullptr) : AppSettings(parent) {}

    MOCK_METHOD(void, load, (), (override));
    MOCK_METHOD(void, save, (), (const, override));
    MOCK_METHOD(void, reset, (), (override));

    MOCK_METHOD(int, getVolume, (), (const, override));
    MOCK_METHOD(bool, getSoundEnabled, (), (const, override));
    MOCK_METHOD(int, getThemeIndex, (), (const, override));
    MOCK_METHOD(bool, getPopupEnabled, (), (const, override));
    MOCK_METHOD(bool, getConfirmClose, (), (const, override));
    MOCK_METHOD(int, getLanguageIndex, (), (const, override));

    MOCK_METHOD(void, setVolume, (int), (override));
    MOCK_METHOD(void, setSoundEnabled, (bool), (override));
    MOCK_METHOD(void, setThemeIndex, (int), (override));
    MOCK_METHOD(void, setLanguageIndex, (int), (override));
};

class MockInactivityWatcher : public InactivityWatcher
{
    Q_OBJECT
public:
    explicit MockInactivityWatcher(QObject* parent = nullptr) : InactivityWatcher(parent) {}

    MOCK_METHOD(void, start, (int msec), (override));
    MOCK_METHOD(void, stop, (), (override));

    void simulateInactivity() { emit inactivityDetected(); }
};

class MockSoundManager : public ISoundManager
{
public:
    MOCK_METHOD(void, setVolume, (int), (override));
    MOCK_METHOD(void, setEnabled, (bool), (override));
    MOCK_METHOD(void, playNotification, (), (override));
};

class MockThemeManager : public IThemeManager
{
public:
    MOCK_METHOD(void, applyTheme, (Theme), (override));
};

class MockAppController : public AppController
{
    Q_OBJECT

public:
    explicit MockAppController(QObject* parent = nullptr) : AppController(parent) {}

    MOCK_METHOD(QList<TimerInfo>, activeTimerInfos, (), (const));
    MOCK_METHOD(void, stopTimer, (TimerDialog*), ());
    MOCK_METHOD(const AppSettings*, getAppSettings, (), (const));
};
