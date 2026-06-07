#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QSignalSpy>
#include <QApplication>
#include <QPushButton>

#include "MainWindow.h"

class MainWindowTest : public ::testing::Test
{
protected:
    void SetUp() override {
        window = std::make_unique<MainWindow>(nullptr);
    }

    std::unique_ptr<MainWindow> window;
};

TEST_F(MainWindowTest, WindowCreatedSuccessfully)
{
    EXPECT_NE(window.get(), nullptr);
}

TEST_F(MainWindowTest, WindowHasCorrectTitle)
{
    EXPECT_FALSE(window->windowTitle().isEmpty());
}

TEST_F(MainWindowTest, AcceptCloseSetsFlagTrue)
{
    window->acceptClose();
    QCloseEvent closeEvent;
    QApplication::sendEvent(window.get(), &closeEvent);
    EXPECT_TRUE(closeEvent.isAccepted());
}

TEST_F(MainWindowTest, CancelCloseSetsFlagFalse)
{
    window->cancelClose();
    QCloseEvent closeEvent;
    QApplication::sendEvent(window.get(), &closeEvent);
    EXPECT_FALSE(closeEvent.isAccepted());
}

TEST_F(MainWindowTest, CloseRequestedSignalEmittedOnCloseEvent)
{
    QSignalSpy spy(window.get(), &MainWindow::closeRequested);
    QCloseEvent closeEvent;
    QApplication::sendEvent(window.get(), &closeEvent);
    EXPECT_EQ(spy.count(), 1);
}
