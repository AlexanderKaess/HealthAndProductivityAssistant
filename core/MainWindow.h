#pragma once

#include <log4cxx/logger.h>
#include <QMainWindow>
#include <QPointer>
#include <QList>
#include <QMessageBox>
#include <QTranslator>
#include <QLocale>

#include "TimerDialog.h"
#include "InactivityWatcher.h"
#include "ThemeManager.h"
#include "SoundManager.h"
#include "InactivityDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //1. QTab
    void onPomodoroTimerClicked();
    void onStayHydratedClicked();
    void onFreshAirTimerClicked();
    void onWorkingHourTimerClicked();
    void onBreakTimerClicked();
    void onMovementTimerClicked();

    //2. QTab
    void refreshActiveTimers();
    void stopAllTimers();
    void onTimerFinished(TimerDialog::TimerType type);

    //3.QTab
    void saveSettings();
    void resetSettings();
    void onVolumeChanged(int value);
    void onThemeChanged(int index);
    void onLanguageChanged(int index);
    void onMonitoringChanged();
    void onInactivityDetected();

private:
    void closeEvent(QCloseEvent *event) override;
    void openTimerDialog(const TimerDialog::TimerType &timerType);
    void loadSettings();
    void connectSignals();
    void cleanUpTimers();
    void applyLanguage(const QString &localLanguage);
    void monitoringUpdateStatus(bool monitoringActive);

    Ui::MainWindow *ui;
    log4cxx::LoggerPtr logger;
    QList<QPointer<TimerDialog>> activeTimers;
    QTimer *refreshTimer{};
    int completedCount{};
    QTranslator *translator = nullptr;
    InactivityWatcher* watcher = nullptr;
};
