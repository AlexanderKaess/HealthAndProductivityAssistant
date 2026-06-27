#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QSignalSpy>
#include <QApplication>
#include <QPushButton>
#include <QCloseEvent>

#include "MainWindow.h"
#include "Mocks.h"

using ::testing::Return;
using ::testing::NiceMock;

// class MockAppController : public AppController
// {
//     Q_OBJECT

// public:
//     explicit MockAppController(QObject* parent = nullptr) : AppController(parent) {}

//     MOCK_METHOD(QList<TimerInfo>, activeTimerInfos, (), (const));
//     MOCK_METHOD(void, stopTimer, (TimerDialog*), ());
//     MOCK_METHOD(const AppSettings*, getAppSettings, (), (const));
// };

// class MockAppSettings : public AppSettings
// {
//     Q_OBJECT

// public:
//     explicit MockAppSettings(QObject* parent = nullptr) : AppSettings(parent) {}

//     MOCK_METHOD(int, getVolume, (), (const));
//     MOCK_METHOD(bool, getSoundEnabled, (), (const));
//     MOCK_METHOD(bool, getPopupEnabled, (), (const));
//     MOCK_METHOD(bool, getConfirmClose, (), (const));
//     MOCK_METHOD(int, getThemeIndex, (), (const));
//     MOCK_METHOD(int, getLanguageIndex,(), (const));
// };

class MainWindowTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mockSettings = new NiceMock<MockAppSettings>();

        ON_CALL(*mockSettings, getVolume()).WillByDefault(Return(50));
        ON_CALL(*mockSettings, getSoundEnabled()).WillByDefault(Return(true));
        ON_CALL(*mockSettings, getPopupEnabled()).WillByDefault(Return(true));
        ON_CALL(*mockSettings, getConfirmClose()).WillByDefault(Return(false));
        ON_CALL(*mockSettings, getThemeIndex()).WillByDefault(Return(0));
        ON_CALL(*mockSettings, getLanguageIndex()).WillByDefault(Return(0));

        mockController = new NiceMock<MockAppController>();
        ON_CALL(*mockController, getAppSettings()).WillByDefault(Return(mockSettings));
        ON_CALL(*mockController, activeTimerInfos()).WillByDefault(Return(QList<AppController::TimerInfo>{}));

        window = std::make_unique<MainWindow>(mockController);
    }

    void TearDown() override
    {
        window.reset();
        delete mockController;
        delete mockSettings;
    }

    NiceMock<MockAppController>* mockController{nullptr};
    NiceMock<MockAppSettings>* mockSettings{nullptr};
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
    QObject::connect(window.get(), &MainWindow::closeRequested, window.get(), &MainWindow::acceptClose);

    window->acceptClose();

    QCloseEvent closeEvent;
    QApplication::sendEvent(window.get(), &closeEvent);

    EXPECT_TRUE(closeEvent.isAccepted());
}

TEST_F(MainWindowTest, CancelCloseSetsFlagFalse)
{
    QObject::connect(window.get(), &MainWindow::closeRequested,window.get(), &MainWindow::cancelClose);

    QCloseEvent closeEvent;
    QApplication::sendEvent(window.get(), &closeEvent);

    EXPECT_FALSE(closeEvent.isAccepted());
}

TEST_F(MainWindowTest, CloseRequestedSignalEmittedOnCloseEvent)
{
    QSignalSpy spy(window.get(), &MainWindow::closeRequested);
    QObject::connect(window.get(), &MainWindow::closeRequested,window.get(), &MainWindow::cancelClose);

    QCloseEvent closeEvent;
    QApplication::sendEvent(window.get(), &closeEvent);

    EXPECT_EQ(spy.count(), 1);
}

#include "MainWindowTest.moc"
