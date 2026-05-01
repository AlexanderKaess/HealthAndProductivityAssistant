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
    Q_ENUM(TimerType)

    TimerDialog(TimerType type, QWidget *parent = nullptr);
    ~TimerDialog();

    const int getRemainingTime () { return remainingTime; }
    bool isTimerRunning() const { return timer->isActive(); }

    QString timerTypeName() const;
    QString formattedTime() const;

signals:
    void timerFinished(enum TimerDialog::TimerType type);

private slots:
    void updateTimer();

private:
    void setupTimerType();
    QString getTimerName() const;
    QString getTimerDescription() const;
    int getDefaultTime() const;
    void updateTimeDisplay();

    Ui::TimerDialog *ui;
    log4cxx::LoggerPtr logger;
    TimerType currentType;
    QTimer *timer;
    int remainingTime{};
    bool isRunning{};
};
