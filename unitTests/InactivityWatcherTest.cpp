#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QSignalSpy>
#include <QApplication>
#include <QMouseEvent>
#include <QTest>

#include "InactivityWatcher.h"

class InactivityWatcherTest : public ::testing::Test
{
protected:
    void SetUp() override {
        watcher = std::make_unique<InactivityWatcher>(nullptr);
    }

    std::unique_ptr<InactivityWatcher> watcher;
};

TEST_F(InactivityWatcherTest, WatcherCreatedSuccessfully)
{
    EXPECT_NE(watcher.get(), nullptr);
}

TEST_F(InactivityWatcherTest, StartDoesNotCrash)
{
    EXPECT_NO_THROW(watcher->start(5000));
}

TEST_F(InactivityWatcherTest, StopDoesNotCrash)
{
    watcher->start(5000);
    EXPECT_NO_THROW(watcher->stop());
}
