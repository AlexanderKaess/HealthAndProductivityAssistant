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

    MOCK_METHOD(void, load, ());
    MOCK_METHOD(void, save, ());
    MOCK_METHOD(void, reset, ());

    MOCK_METHOD(int, getVolume, ());
    MOCK_METHOD(bool, getSoundEnabled, ());
    MOCK_METHOD(int, getThemeIndex, ());
    MOCK_METHOD(bool, getPopupEnabled, ());
    MOCK_METHOD(bool, getConfirmClose, ());
    MOCK_METHOD(int, getLanguageIndex, ());

    MOCK_METHOD(void, setVolume, (int) );
    MOCK_METHOD(void, setSoundEnabled, (bool));
    MOCK_METHOD(void, setThemeIndex, (int));
    MOCK_METHOD(void, setLanguageIndex, (int));
};

class MockInactivityWatcher : public InactivityWatcher
{
    Q_OBJECT
public:
    explicit MockInactivityWatcher(QObject* parent = nullptr) : InactivityWatcher(parent) {}

    MOCK_METHOD(void, start, (int msec));
    MOCK_METHOD(void, stop, ());

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
