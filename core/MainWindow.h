#pragma once

#include <log4cxx/logger.h>
#include <QMainWindow>
#include <QPointer>
#include <QList>

#include "TimerDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// class TimerDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPomodoroTimerClicked();
    void onStayHydratedClicked();
    void onFreshAirTimerClicked();
    void onWorkingHourTimerClicked();
    void onBreakTimerClicked();
    void onMovementTimerClicked();

    void onTimerFinished(TimerDialog::TimerType type);
    void refreshActiveTimers();

private:
    void openTimerDialog(const TimerDialog::TimerType &timerType);
    void cleanUpTimers();

    Ui::MainWindow *ui;
    log4cxx::LoggerPtr logger;
    QList<QPointer<TimerDialog>> activeTimers;
    int completedCount = 0;


};
