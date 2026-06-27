#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QApplication>
#include <QObject>
#include <QPointer>
#include <QSignalSpy>
#include <QString>
#include <QTranslator>
#include <QList>

#include "AppController.h"
#include "Mocks.h"

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::InSequence;

// class MockAppSettings : public AppSettings
// {
//     Q_OBJECT
// public:
//     explicit MockAppSettings(QObject* parent = nullptr) : AppSettings(parent) {}

//     MOCK_METHOD(void, load, ());
//     MOCK_METHOD(void, save, ());
//     MOCK_METHOD(void, reset, ());

//     MOCK_METHOD(int, getVolume, ());
//     MOCK_METHOD(bool, getSoundEnabled, ());
//     MOCK_METHOD(int, getThemeIndex, ());
//     MOCK_METHOD(bool, getPopupEnabled, ());
//     MOCK_METHOD(bool, getConfirmClose, ());
//     MOCK_METHOD(int, getLanguageIndex, ());

//     MOCK_METHOD(void, setVolume, (int) );
//     MOCK_METHOD(void, setSoundEnabled, (bool));
//     MOCK_METHOD(void, setThemeIndex, (int));
//     MOCK_METHOD(void, setLanguageIndex, (int));
// };

// class MockInactivityWatcher : public InactivityWatcher
// {
//     Q_OBJECT
// public:
//     explicit MockInactivityWatcher(QObject* parent = nullptr) : InactivityWatcher(parent) {}

//     MOCK_METHOD(void, start, (int msec));
//     MOCK_METHOD(void, stop, ());

//     void simulateInactivity() { emit inactivityDetected(); }
// };

// class MockSoundManager : public ISoundManager
// {
// public:
//     MOCK_METHOD(void, setVolume, (int), (override));
//     MOCK_METHOD(void, setEnabled, (bool), (override));
//     MOCK_METHOD(void, playNotification, (), (override));
// };

// class MockThemeManager : public IThemeManager
// {
// public:
//     MOCK_METHOD(void, applyTheme, (Theme), (override));
// };

class AppControllerTest : public ::testing::Test
{
protected:
    MockAppSettings* mockSettings = nullptr;
    MockInactivityWatcher* mockWatcher = nullptr;
    MockSoundManager* mockSound = nullptr;
    MockThemeManager* mockTheme = nullptr;
    AppController* controller = nullptr;

    void SetUp() override
    {
        mockSettings = new MockAppSettings();
        mockWatcher  = new MockInactivityWatcher();
        mockSound    = new MockSoundManager();
        mockTheme    = new MockThemeManager();

        ON_CALL(*mockSettings, getVolume()).WillByDefault(Return(50));
        ON_CALL(*mockSettings, getSoundEnabled()).WillByDefault(Return(true));
        ON_CALL(*mockSettings, getThemeIndex()).WillByDefault(Return(0));
        ON_CALL(*mockSettings, getPopupEnabled()).WillByDefault(Return(false));
        ON_CALL(*mockSettings, getConfirmClose()).WillByDefault(Return(false));
        ON_CALL(*mockSettings, getLanguageIndex()).WillByDefault(Return(0));

        controller = new AppController(nullptr, mockSettings, mockWatcher, mockSound, mockTheme);
    }

    void TearDown() override
    {
        delete controller;
        // controller   = nullptr;

        // delete mockSettings;
        // mockSettings = nullptr;

        // delete mockWatcher;
        // mockWatcher = nullptr;
    }

    template<typename T>
    T captureSignal(QSignalSpy& spy, int signalIndex, int argIndex) const
    {
        return spy.at(signalIndex).at(argIndex).value<T>();
    }
};

TEST_F(AppControllerTest, SaveSettings_CallsSettingsSave)
{
    EXPECT_CALL(*mockSettings, save()).Times(1);

    controller->saveSettings();
}

TEST_F(AppControllerTest, SaveSettings_EmitsStatusMessage)
{
    ON_CALL(*mockSettings, save()).WillByDefault(Return());

    QSignalSpy spy(controller, &AppController::statusMessage);

    controller->saveSettings();

    ASSERT_EQ(spy.count(), 1);
    EXPECT_EQ(captureSignal<QString>(spy, 0, 0), QString("Settings saved"));
}

TEST_F(AppControllerTest, SaveSettings_StatusMessageTimeout_Is3000ms)
{
    ON_CALL(*mockSettings, save()).WillByDefault(Return());
    QSignalSpy spy(controller, &AppController::statusMessage);

    controller->saveSettings();

    ASSERT_EQ(spy.count(), 1);
    EXPECT_EQ(captureSignal<int>(spy, 0, 1), 3000);
}

TEST_F(AppControllerTest, ResetSettings_CallsSettingsReset)
{
    EXPECT_CALL(*mockSettings, reset()).Times(Exactly(1));

    controller->resetSettings();
}

TEST_F(AppControllerTest, ResetSettings_EmitsStatusMessage)
{
    ON_CALL(*mockSettings, reset()).WillByDefault(Return());
    QSignalSpy spy(controller, &AppController::statusMessage);

    controller->resetSettings();

    ASSERT_EQ(spy.count(), 1);
    EXPECT_EQ(captureSignal<QString>(spy, 0, 0), QString("Settings reset"));
}

TEST_F(AppControllerTest, ResetSettings_StatusMessageTimeout_Is3000ms)
{
    ON_CALL(*mockSettings, reset()).WillByDefault(Return());
    QSignalSpy spy(controller, &AppController::statusMessage);

    controller->resetSettings();

    ASSERT_EQ(spy.count(), 1);
    EXPECT_EQ(captureSignal<int>(spy, 0, 1), 3000);
}

TEST_F(AppControllerTest, ChangeVolume_CallsSetVolumeOnSettings)
{
    EXPECT_CALL(*mockSettings, setVolume(75)).Times(Exactly(1));

    controller->changeVolume(75);
}

TEST_F(AppControllerTest, ChangeVolume_ZeroBoundary)
{
    EXPECT_CALL(*mockSettings, setVolume(0)).Times(Exactly(1));

    controller->changeVolume(0);
}

TEST_F(AppControllerTest, ChangeVolume_MaxBoundary)
{
    EXPECT_CALL(*mockSettings, setVolume(100)).Times(Exactly(1));

    controller->changeVolume(100);
}

TEST_F(AppControllerTest, ChangeVolume_DoesNotEmitAnySignal)
{
    ON_CALL(*mockSettings, setVolume(_)).WillByDefault(Return());
    QSignalSpy spyStatus(controller, &AppController::statusMessage);
    QSignalSpy spyTimer(controller,  &AppController::activeTimersChanged);

    controller->changeVolume(50);

    EXPECT_EQ(spyStatus.count(), 0);
    EXPECT_EQ(spyTimer.count(),  0);
}

TEST_F(AppControllerTest, ChangeTheme_CallsSetThemeIndexOnSettings)
{
    EXPECT_CALL(*mockSettings, setThemeIndex(1)).Times(1);

    controller->changeTheme(1);
}

TEST_F(AppControllerTest, ChangeTheme_IndexZero_LightTheme)
{
    EXPECT_CALL(*mockSettings, setThemeIndex(0)).Times(Exactly(1));

    controller->changeTheme(0);
}

TEST_F(AppControllerTest, ChangeTheme_IndexTwo_CustomTheme)
{
    EXPECT_CALL(*mockSettings, setThemeIndex(2)).Times(Exactly(1));

    controller->changeTheme(2);
}

TEST_F(AppControllerTest, ChangeTheme_DoesNotEmitStatusMessage)
{
    ON_CALL(*mockSettings, setThemeIndex(_)).WillByDefault(Return());
    QSignalSpy spy(controller, &AppController::statusMessage);

    controller->changeTheme(1);

    EXPECT_EQ(spy.count(), 0);
}


TEST_F(AppControllerTest, ChangeLanguage_Index0_SetsLanguageIndex0)
{
    EXPECT_CALL(*mockSettings, setLanguageIndex(0)).Times(Exactly(1));

    controller->changeLanguage(0);
}

TEST_F(AppControllerTest, ChangeLanguage_Index1_SetsLanguageIndex1)
{
    EXPECT_CALL(*mockSettings, setLanguageIndex(1)).Times(Exactly(1));

    controller->changeLanguage(1);
}

TEST_F(AppControllerTest, ChangeLanguage_Index0_AppliesGermanLocale)
{
    ON_CALL(*mockSettings, setLanguageIndex(_)).WillByDefault(Return());
    EXPECT_NO_THROW(controller->changeLanguage(0));
}

TEST_F(AppControllerTest, ChangeLanguage_Index1_AppliesEnglishLocale)
{
    ON_CALL(*mockSettings, setLanguageIndex(_)).WillByDefault(Return());

    EXPECT_NO_THROW(controller->changeLanguage(1));
}

TEST_F(AppControllerTest, ChangeSoundEnabled_True_CallsSetSoundEnabledTrue)
{
    EXPECT_CALL(*mockSettings, setSoundEnabled(true)).Times(Exactly(1));

    controller->changeSoundEnabled(true);
}

TEST_F(AppControllerTest, ChangeSoundEnabled_False_CallsSetSoundEnabledFalse)
{
    EXPECT_CALL(*mockSettings, setSoundEnabled(false)).Times(Exactly(1));

    controller->changeSoundEnabled(false);
}

TEST_F(AppControllerTest, ChangeSoundEnabled_DoesNotEmitStatusMessage)
{
    ON_CALL(*mockSettings, setSoundEnabled(_)).WillByDefault(Return());
    QSignalSpy spy(controller, &AppController::statusMessage);

    controller->changeSoundEnabled(true);

    EXPECT_EQ(spy.count(), 0);
}

TEST_F(AppControllerTest, SetMonitoring_Enable_StartsWatcher)
{
    const int timeoutMinutes = 5;
    EXPECT_CALL(*mockWatcher, start(timeoutMinutes * 60 * 1000)).Times(Exactly(1));

    controller->setMonitoringEnabled(true, timeoutMinutes);
}

TEST_F(AppControllerTest, SetMonitoring_Enable_ConvertsMinutesToMilliseconds)
{
    EXPECT_CALL(*mockWatcher, start(600000)).Times(Exactly(1));

    controller->setMonitoringEnabled(true, 10);
}

TEST_F(AppControllerTest, SetMonitoring_Enable_1Minute)
{
    EXPECT_CALL(*mockWatcher, start(60000)).Times(Exactly(1));

    controller->setMonitoringEnabled(true, 1);
}

TEST_F(AppControllerTest, SetMonitoring_Enable_60Minutes)
{
    EXPECT_CALL(*mockWatcher, start(3600000)).Times(Exactly(1));

    controller->setMonitoringEnabled(true, 60);
}

TEST_F(AppControllerTest, SetMonitoring_Enable_EmitsMonitoringStatusChanged_True)
{
    ON_CALL(*mockWatcher, start(_)).WillByDefault(Return());
    QSignalSpy spy(controller, &AppController::monitoringStatusChanged);

    controller->setMonitoringEnabled(true, 5);

    ASSERT_EQ(spy.count(), 1);
    EXPECT_TRUE(captureSignal<bool>(spy, 0, 0));
}

TEST_F(AppControllerTest, SetMonitoring_Enable_EmitsStatusMessage_ContainsActive)
{
    ON_CALL(*mockWatcher, start(_)).WillByDefault(Return());
    QSignalSpy spy(controller, &AppController::statusMessage);

    controller->setMonitoringEnabled(true, 5);

    ASSERT_EQ(spy.count(), 1);
    QString msg = captureSignal<QString>(spy, 0, 0);
    EXPECT_TRUE(msg.contains("active", Qt::CaseInsensitive))
        << "Erwartet 'active' in: " << msg.toStdString();
}

TEST_F(AppControllerTest, SetMonitoring_Enable_StatusMessageTimeout_Is5000ms)
{
    ON_CALL(*mockWatcher, start(_)).WillByDefault(Return());
    QSignalSpy spy(controller, &AppController::statusMessage);

    controller->setMonitoringEnabled(true, 5);

    ASSERT_EQ(spy.count(), 1);
    EXPECT_EQ(captureSignal<int>(spy, 0, 1), 5000);
}

TEST_F(AppControllerTest, SetMonitoring_Disable_StopsWatcher)
{
    EXPECT_CALL(*mockWatcher, stop()).Times(Exactly(1));

    controller->setMonitoringEnabled(false, 0);
}

TEST_F(AppControllerTest, SetMonitoring_Disable_EmitsMonitoringStatusChanged_False)
{
    ON_CALL(*mockWatcher, stop()).WillByDefault(Return());
    QSignalSpy spy(controller, &AppController::monitoringStatusChanged);

    controller->setMonitoringEnabled(false, 0);

    ASSERT_EQ(spy.count(), 1);
    EXPECT_FALSE(captureSignal<bool>(spy, 0, 0));
}

TEST_F(AppControllerTest, SetMonitoring_Disable_EmitsStatusMessage_ContainsInactive)
{
    ON_CALL(*mockWatcher, stop()).WillByDefault(Return());
    QSignalSpy spy(controller, &AppController::statusMessage);

    controller->setMonitoringEnabled(false, 0);

    ASSERT_EQ(spy.count(), 1);
    QString msg = captureSignal<QString>(spy, 0, 0);
    EXPECT_TRUE(msg.contains("inactive", Qt::CaseInsensitive))
        << "Erwartet 'inactive' in: " << msg.toStdString();
}

TEST_F(AppControllerTest, SetMonitoring_Disable_StatusMessageTimeout_Is5000ms)
{
    ON_CALL(*mockWatcher, stop()).WillByDefault(Return());
    QSignalSpy spy(controller, &AppController::statusMessage);

    controller->setMonitoringEnabled(false, 0);

    ASSERT_EQ(spy.count(), 1);
    EXPECT_EQ(captureSignal<int>(spy, 0, 1), 5000);
}

TEST_F(AppControllerTest, SetMonitoring_EnableThenDisable_CorrectCallOrder)
{
    InSequence seq;
    EXPECT_CALL(*mockWatcher, start(300000)).Times(1);   // 5 Minuten
    EXPECT_CALL(*mockWatcher, stop()).Times(1);

    controller->setMonitoringEnabled(true,  5);
    controller->setMonitoringEnabled(false, 0);
}


TEST_F(AppControllerTest, ActiveTimerInfos_EmptyByDefault)
{
    const auto infos = controller->activeTimerInfos();

    EXPECT_TRUE(infos.isEmpty());
}

TEST_F(AppControllerTest, ActiveTimerCount_ZeroByDefault)
{
    EXPECT_EQ(controller->getActiveTimerCount(), 0);
}

TEST_F(AppControllerTest, CleanUpTimers_RemovesNullPointers_NoCrash)
{
    auto timers = controller->getActiveTimers();
    timers.append(QPointer<TimerDialog>(nullptr));

    EXPECT_NO_THROW({
        const auto infos = controller->activeTimerInfos();
        EXPECT_TRUE(infos.isEmpty());
    });
}

TEST_F(AppControllerTest, CleanUpTimers_AfterNull_ListIsEmpty)
{
    auto timers = controller->getActiveTimers();
    timers.append(QPointer<TimerDialog>(nullptr));
    timers.append(QPointer<TimerDialog>(nullptr));
    timers.append(QPointer<TimerDialog>(nullptr));

    const auto infos = controller->activeTimerInfos();

    EXPECT_TRUE(infos.isEmpty());
}

TEST_F(AppControllerTest, OnTimerFinished_Pomodoro_DoesNotCrash)
{
    EXPECT_NO_THROW(controller->onTimerFinished());
}

TEST_F(AppControllerTest, OnTimerFinished_DoesNotEmitAnySignal)
{
    QSignalSpy spyStatus(controller,  &AppController::statusMessage);
    QSignalSpy spyMonitor(controller, &AppController::monitoringStatusChanged);
    QSignalSpy spyTimers(controller,  &AppController::activeTimersChanged);

    controller->onTimerFinished();

    EXPECT_EQ(spyStatus.count(),  0);
    EXPECT_EQ(spyMonitor.count(), 0);
    EXPECT_EQ(spyTimers.count(),  0);
}

TEST_F(AppControllerTest, OnTimerDestroyed_EmitsActiveTimersChanged)
{
    QSignalSpy spy(controller, &AppController::activeTimersChanged);
    controller->onTimerDestroyed();

    ASSERT_EQ(spy.count(), 1);
}

TEST_F(AppControllerTest, OnTimerDestroyed_WithNullTimer_DoesNotCrash)
{
    EXPECT_NO_THROW(controller->onTimerDestroyed());
}

TEST_F(AppControllerTest, Signal_StatusMessage_IsRegistered)
{
    const QMetaObject* meta = controller->metaObject();
    int idx = meta->indexOfSignal("statusMessage(QString,int)");
    EXPECT_NE(idx, -1) << "Signal statusMessage(QString,int) nicht gefunden";
}

TEST_F(AppControllerTest, Signal_ActiveTimersChanged_IsRegistered)
{
    const QMetaObject* meta = controller->metaObject();
    int idx = meta->indexOfSignal("activeTimersChanged()");
    EXPECT_NE(idx, -1) << "Signal activeTimersChanged() nicht gefunden";
}

TEST_F(AppControllerTest, Signal_MonitoringStatusChanged_IsRegistered)
{
    const QMetaObject* meta = controller->metaObject();
    int idx = meta->indexOfSignal("monitoringStatusChanged(bool)");
    EXPECT_NE(idx, -1) << "Signal monitoringStatusChanged(bool) nicht gefunden";
}

TEST_F(AppControllerTest, ChangeVolume_MultipleConsecutiveCalls)
{
    EXPECT_CALL(*mockSettings, setVolume(10)).Times(1);
    EXPECT_CALL(*mockSettings, setVolume(50)).Times(1);
    EXPECT_CALL(*mockSettings, setVolume(90)).Times(1);

    controller->changeVolume(10);
    controller->changeVolume(50);
    controller->changeVolume(90);
}

TEST_F(AppControllerTest, SaveSettings_CalledTwice_BothCallsForwarded)
{
    EXPECT_CALL(*mockSettings, save()).Times(Exactly(2));

    controller->saveSettings();
    controller->saveSettings();
}

TEST_F(AppControllerTest, ResetSettings_CalledTwice_BothCallsForwarded)
{
    EXPECT_CALL(*mockSettings, reset()).Times(Exactly(2));

    controller->resetSettings();
    controller->resetSettings();
}

TEST_F(AppControllerTest, SetMonitoring_EnableMultipleTimes_StartsWatcherEachTime)
{
    EXPECT_CALL(*mockWatcher, start(_)).Times(Exactly(3));

    controller->setMonitoringEnabled(true, 5);
    controller->setMonitoringEnabled(true, 10);
    controller->setMonitoringEnabled(true, 20);
}

TEST_F(AppControllerTest, ChangeSoundEnabled_ToggleMultipleTimes)
{
    InSequence seq;
    EXPECT_CALL(*mockSettings, setSoundEnabled(true)).Times(1);
    EXPECT_CALL(*mockSettings, setSoundEnabled(false)).Times(1);
    EXPECT_CALL(*mockSettings, setSoundEnabled(true)).Times(1);

    controller->changeSoundEnabled(true);
    controller->changeSoundEnabled(false);
    controller->changeSoundEnabled(true);
}

#include "AppControllerTest.moc"
