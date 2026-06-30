#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QSignalSpy>
#include <QCoreApplication>
#include <qtest.h>

#include "TimerDialog.h"

class TimerDialogTest : public ::testing::Test
{
protected:
    void SetUp() override {
        timer = new TimerDialog(TimerDialog::POMODORO, nullptr);
    }

    void TearDown() override {
        delete timer;
        timer = nullptr;
    }

    TimerDialog* timer = nullptr;
};

TEST_F(TimerDialogTest, InitialStateIsStopped)
{
    EXPECT_FALSE(timer->isTimerRunning());
}

TEST_F(TimerDialogTest, StartChangesStateToRunning)
{
    timer->start(1000);
    EXPECT_TRUE(timer->isTimerRunning());
    timer->stop();
}

TEST_F(TimerDialogTest, StopChangesStateToStopped)
{
    timer->start(1000);
    timer->stop();
    EXPECT_FALSE(timer->isTimerRunning());
}

TEST_F(TimerDialogTest, RemainingTimeDecreasesAfterStart)
{
    timer->start(5);
    int before = timer->getRemainingTime();
    QTest::qWait(1100);
    int after = timer->getRemainingTime();
    EXPECT_LT(after, before);
    timer->stop();
}

TEST_F(TimerDialogTest, TimerFinishedSignalEmitted)
{
    QSignalSpy spy(timer, &TimerDialog::timerFinished);
    timer->start(1);
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 3000);
}

