#pragma once

#include <QMainWindow>
#include <log4cxx/logger.h>
#include <TimerDialog.h>

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
    void openTimerDialog(TimerDialog::TimerType &timerType);

    Ui::MainWindow *ui;
    log4cxx::LoggerPtr logger;
    QList<TimerDialog*> activeTimers;
    int completedCount = 0;


};
