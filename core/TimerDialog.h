#pragma once

#include <QDialog>
#include <QTime>
#include <QTimer>
#include <log4cxx/logger.h>

namespace Ui {
class TimerDialog;
}

class TimerDialog : public QDialog
{
    Q_OBJECT

public:
    enum TimerType {
        POMODORO,
        STAYHYDRATET,
        FRESHAIR,
        WORKINGHOUR,
        BREAK,
        MOVEMENT };

    TimerDialog(TimerType type, QWidget *parent = nullptr);
    ~TimerDialog();

private slots:

    void updateTimer();

private:
    Ui::TimerDialog *ui;
    log4cxx::LoggerPtr logger;
    TimerType currentType;
    QTimer *timer;
    QTime remainingTime;
    bool isRunning;

    void setupTimerType();
    QString getTimerName() const;
    QString getTimerDescription() const;
    int getDefaultTime() const;
    void updateTimeDisplay();
};
