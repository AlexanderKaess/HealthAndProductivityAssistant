#pragma once

#include <log4cxx/logger.h>
#include <QObject>
#include <QTimer>
#include <QEvent>
#include <QApplication>

class InactivityWatcher : public QObject
{

    Q_OBJECT

public:
    explicit InactivityWatcher(QObject * patent = nullptr);
    ~InactivityWatcher() override;

    void start(int timeOutMs);
    void stop();
    bool isRunning() const { return running; }

signals:
    void inactivityDetected();

private:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void resetTimer();
    QTimer timer{};
    bool running{};
    log4cxx::LoggerPtr logger;
};
