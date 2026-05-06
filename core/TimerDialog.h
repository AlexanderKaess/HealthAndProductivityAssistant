#pragma once

#include <log4cxx/logger.h>
#include <QDialog>
#include <QTime>
#include <QTimer>
#include <QString>

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

    const int getRemainingTime () { return remainingTimeSec; }
    bool isTimerRunning() const { return timer->isActive(); }

    QString timerTypeName() const;
    QString formattedTime() const;

signals:
    void timerFinished(TimerDialog::TimerType type);
    void timerTick(int remainingTime);

private slots:
    void onStartClicked();
    void onStopClicked();
    void onResetClicked();
    void updateTimerCountDown();

private:
    void closeEvent(QCloseEvent *event) override;
    void setupForTimerType(const TimerType &type);
    void updateDisplay();
    int getDefaultTimeForTimerType(const TimerType &type) const;

    Ui::TimerDialog *ui;
    log4cxx::LoggerPtr logger;
    TimerType currentType;
    QTimer *timer;
    int remainingTimeSec{};
    bool isRunning{};
    int initialSeconds{};
};
