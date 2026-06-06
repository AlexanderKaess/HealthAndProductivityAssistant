#pragma once

#include <log4cxx/logger.h>
#include <QMainWindow>
#include <QCloseEvent>

#include "AppSettings.h"
#include "../appController/AppController.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//class AppController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(AppController* controller, QWidget* parent = nullptr);
    ~MainWindow();

    void initFromSettings(const AppSettings* settings);

    void cancelClose();

    void acceptClose();

signals:
    //1. QTab
    void pomodoroRequested();
    void stayHydratedRequested();
    void freshAirRequested();
    void workingHourRequested();
    void breakRequested();
    void movementRequested();

    //2. QTab
    void stopAllTimersRequested();

    //3. QTab
    void saveSettingsRequested();
    void resetSettingsRequested();
    void volumeChangeRequested(int percent);
    void themeChangeRequested(int index);
    void languageChangeRequested(int index);
    void soundEnabledChangeRequested(bool enabled);
    void monitoringChangeRequested(bool enabled, int timeoutMinutes);

    void closeRequested();

public slots:
    void onActiveTimersChanged();
    void onStatusMessage(const QString& msg, int timeoutMs);
    void onMonitoringStatusChanged(bool active);
    void onLanguageChanged(const QString& locale);

private:
    void closeEvent(QCloseEvent* event) override;
    void connectUiSignals();
    void refreshTimerTable();

    log4cxx::LoggerPtr logger;
    Ui::MainWindow* ui{nullptr};
    AppController* controller{nullptr};
    bool closeAccepted{false};
};
