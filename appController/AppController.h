#pragma once

#include <log4cxx/logger.h>
#include <QObject>
#include <QPointer>
#include <QList>
#include <QTranslator>
#include <QApplication>
#include <QMessageBox>
#include <QFile>

#include "AppSettings.h"
#include "TimerDialog.h"
#include "InactivityWatcher.h"
#include "SoundManager.h"
#include "ThemeManager.h"
#include "InactivityDialog.h"

class MainWindow;

class AppController : public QObject
{
    Q_OBJECT

public:
    explicit AppController(QObject* parent = nullptr);

    void start();

    AppSettings* getAppSettings() const { return settings; }
    InactivityWatcher* getInactivityWatcher() { return watcher; }
    QList<QPointer<TimerDialog>> getActiveTimers() {return activeTimers; }
    int getActiveTimerCount()const { return activeTimers.size(); }

    struct TimerInfo {
        QString typeName;
        QString formattedTime;
        bool running;
        TimerDialog* dialog;
    };
    QList<TimerInfo> activeTimerInfos() const;

public slots:
    void openTimer(TimerDialog::TimerType type);
    void stopAllTimers();
    void stopTimer(TimerDialog* dialog);

    void saveSettings();
    void resetSettings();
    void changeVolume(int percent);
    void changeTheme(int index);
    void changeLanguage(int index);
    void changeSoundEnabled(bool enabled);

    void setMonitoringEnabled(bool enabled, int timeoutMinutes = 20);
    void requestClose();

    void onTimerDestroyed();
    void onTimerFinished();
    void onInactivityDetected();

signals:
    void activeTimersChanged();
    void statusMessage(const QString& msg, int timeoutMs = 3000);
    void monitoringStatusChanged(bool active);
    void inactivityDetected();
    void timerFinishedNotification(const QString& timerName);
    void languageChanged(const QString& locale);

private:
    void cleanUpTimers();
    void applyLanguage(const QString& locale);
    void connectViewSignals();

    log4cxx::LoggerPtr logger;
    AppSettings* settings { nullptr };
    InactivityWatcher* watcher { nullptr };
    QTranslator* translator { nullptr };
    MainWindow* view { nullptr };
    QList<QPointer<TimerDialog>> activeTimers{};
};
