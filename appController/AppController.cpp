#include "AppController.h"
#include "MainWindow.h"
#include "InactivityDialog.h"

#include "SoundManager.h"
#include "ThemeManager.h"

AppController::AppController(QObject* parent,
                             AppSettings* appSettings,
                             InactivityWatcher* inactivityWatcher,
                             ISoundManager* sManager,
                             IThemeManager* tManager)
    : QObject(parent)
    , logger(log4cxx::Logger::getLogger("HealthLogger.AppController"))
{
    settings = appSettings ? appSettings : new AppSettings(this);
    watcher = inactivityWatcher ? inactivityWatcher : new InactivityWatcher(this);
    soundManager = sManager ? sManager : &SoundManager::instance();
    themeManager = tManager ? tManager : &ThemeManager::instance();
    translator = new QTranslator(this);
}

void AppController::start()
{
    LOG4CXX_INFO(logger, "AppController::start");

    settings->load();

    soundManager->setVolume(settings->getVolume());
    soundManager->setEnabled(settings->getSoundEnabled());
    themeManager->applyTheme(static_cast<IThemeManager::Theme>(settings->getThemeIndex()));

    view = new MainWindow(this);
    connectViewSignals();

    view->initFromSettings(settings);

    connect(watcher, &InactivityWatcher::inactivityDetected, this, &AppController::onInactivityDetected);

    view->show();
    emit statusMessage("ready ...", 3000);
}

void AppController::connectViewSignals()
{
    connect(view, &MainWindow::pomodoroRequested, this, [this]{ openTimer(TimerDialog::POMODORO); });
    connect(view, &MainWindow::stayHydratedRequested, this, [this]{ openTimer(TimerDialog::STAYHYDRATET);});
    connect(view, &MainWindow::freshAirRequested, this, [this]{ openTimer(TimerDialog::FRESHAIR); });
    connect(view, &MainWindow::workingHourRequested, this, [this]{ openTimer(TimerDialog::WORKINGHOUR); });
    connect(view, &MainWindow::breakRequested, this, [this]{ openTimer(TimerDialog::BREAK); });
    connect(view, &MainWindow::movementRequested, this, [this]{ openTimer(TimerDialog::MOVEMENT); });

    connect(view, &MainWindow::stopAllTimersRequested, this, &AppController::stopAllTimers);

    connect(view, &MainWindow::saveSettingsRequested, this, &AppController::saveSettings);
    connect(view, &MainWindow::resetSettingsRequested, this, &AppController::resetSettings);
    connect(view, &MainWindow::volumeChangeRequested, this, &AppController::changeVolume);
    connect(view, &MainWindow::themeChangeRequested, this, &AppController::changeTheme);
    connect(view, &MainWindow::languageChangeRequested, this, &AppController::changeLanguage);
    connect(view, &MainWindow::soundEnabledChangeRequested, this, &AppController::changeSoundEnabled);

    connect(view, &MainWindow::monitoringChangeRequested, this, &AppController::setMonitoringEnabled);

    connect(view, &MainWindow::closeRequested, this, &AppController::requestClose);
}

void AppController::openTimer(TimerDialog::TimerType type)
{
    LOG4CXX_INFO(logger, "Open timer: " << static_cast<int>(type));

    QPointer<TimerDialog> dialog = new TimerDialog(type, view);
    dialog->setPopUpNotification(settings->getPopupEnabled());
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    activeTimers.append(dialog);

    connect(dialog, &QObject::destroyed, this, &AppController::onTimerDestroyed);
    connect(dialog, &TimerDialog::timerFinished, this, &AppController::onTimerFinished);

    dialog->show();

    emit activeTimersChanged();
    emit statusMessage(dialog->timerTypeName() + " started ...", 3000);
}

void AppController::stopTimer(TimerDialog* dialog)
{
    if (dialog) {
        dialog->close();
    }
}

void AppController::stopAllTimers()
{
    LOG4CXX_INFO(logger, "Stop all timers");
    cleanUpTimers();

    if (activeTimers.isEmpty()) {
        emit statusMessage("No active timers", 3000);
        return;
    }

    bool confirmed = true;
    if (settings->getPopupEnabled()) {
        auto reply = QMessageBox::question(
            view, "Stop all timers",
            QString("Do you want to stop all %1 timer?")
                .arg(activeTimers.size()));
        confirmed = (reply == QMessageBox::Yes);
    }

    if (confirmed) {
        const auto timers = activeTimers;
        for (const auto& timer : timers)
            if (timer) {
                timer->close();
            }
        emit statusMessage("All timers have stopped", 3000);
    }
}

void AppController::onTimerDestroyed()
{
    cleanUpTimers();
    emit activeTimersChanged();
}

void AppController::onTimerFinished()
{
    soundManager->playNotification();
}

void AppController::saveSettings()
{
    LOG4CXX_INFO(logger, "Save settings");
    settings->save();
    emit statusMessage("Settings saved", 3000);
}

void AppController::resetSettings()
{
    LOG4CXX_INFO(logger, "Reset settings");
    settings->reset();
    emit statusMessage("Settings reset", 3000);
}

void AppController::changeVolume(int percent)
{
    settings->setVolume(percent);
    soundManager->setVolume(percent);
}

void AppController::changeTheme(int index)
{
    settings->setThemeIndex(index);
    themeManager->applyTheme(static_cast<IThemeManager::Theme>(index));
}

void AppController::changeLanguage(int index)
{
    settings->setLanguageIndex(index);
    const QString locale = (index == 0) ? "de" : "en";
    applyLanguage(locale);
}

void AppController::changeSoundEnabled(bool enabled)
{
    settings->setSoundEnabled(enabled);
    soundManager->setEnabled(enabled);
}

void AppController::applyLanguage(const QString& locale)
{
    qApp->removeTranslator(translator);

    const QString path = (locale == "de")
                             ? ":/i18n/HealthAndProductivityAssistant_de.qm"
                             : ":/i18n/HealthAndProductivityAssistant_en.qm";

    if (!QFile::exists(path)) {
        LOG4CXX_ERROR(logger, "Language file not found: " << path.toStdString());
        return;
    }

    if (translator->load(path)) {
        qApp->installTranslator(translator);
        emit languageChanged(locale);
        LOG4CXX_INFO(logger, "Language applied: " << locale.toStdString());
    } else {
        LOG4CXX_WARN(logger, "Could not load translation: " << path.toStdString());
    }
}

void AppController::setMonitoringEnabled(bool enabled, int timeoutMinutes)
{
    if (enabled) {
        LOG4CXX_INFO(logger, "Monitoring start");
        watcher->start(timeoutMinutes * 60 * 1000);
        emit monitoringStatusChanged(true);
        emit statusMessage("Monitoring active", 5000);
    } else {
        LOG4CXX_INFO(logger, "Monitoring stop");
        watcher->stop();
        emit monitoringStatusChanged(false);
        emit statusMessage("Monitoring inactive", 5000);
    }
}

void AppController::onInactivityDetected()
{
    LOG4CXX_INFO(logger, "Inactivity detected");
    watcher->stop();
    emit monitoringStatusChanged(false);

    auto* dialog = new InactivityDialog(view);
    connect(dialog, &InactivityDialog::finished, this, [this](int) {
        const int timeout = 20;
        setMonitoringEnabled(true, timeout);
    });

    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

void AppController::requestClose()
{
    LOG4CXX_INFO(logger, "Close requested");
    cleanUpTimers();

    if (!activeTimers.isEmpty() && settings->getConfirmClose()) {
        auto reply = QMessageBox::question(
            view, "Timer active",
            QString("%1 timer still running. Stop them?").arg(activeTimers.size()));
        if (reply != QMessageBox::Yes) {
            view->cancelClose();
            return;
        }
    }
    view->acceptClose();
}

void AppController::cleanUpTimers()
{
    activeTimers.erase(
        std::remove_if(activeTimers.begin(), activeTimers.end(),
                       [](const QPointer<TimerDialog>& p){ return p.isNull(); }),
        activeTimers.end());
}

QList<AppController::TimerInfo> AppController::activeTimerInfos() const
{
    QList<TimerInfo> result;
    for (const auto& timer : activeTimers) {
        if (timer) {
            result.append({ timer->timerTypeName(),
                           timer->formattedTime(),
                           timer->isTimerRunning(),
                           timer.data() });
        }
    }
    return result;
}
