#include <QPointer>
#include <QCloseEvent>
#include <QSettings>

#include "MainWindow.h"
#include "../ui/ui_MainWindow.h"
#include "TimerDialog.h"
#include "ThemeManager.h"
#include "SoundManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , logger(log4cxx::Logger::getLogger("HealthLogger.MainWindow"))
    , refreshTimer(new QTimer(this))
{
    ui->setupUi(this);
    setWindowTitle("HealthAndProductivityAssistant");
    LOG4CXX_INFO(logger, "Mainwindow started ...");

    connectSignals();
    loadSettings();

    refreshTimer->setInterval(1000);
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::refreshActiveTimers);
    refreshTimer->start();
    refreshActiveTimers();

    // set the current index to 1 = dark
    ui->themeComboBox->setCurrentIndex(1);

    statusBar()->showMessage("ready ...", 3000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPomodoroTimerClicked()
{
    LOG4CXX_INFO(logger, "TimerDialog::POMODORO selected ...");
    openTimerDialog(TimerDialog::TimerType::POMODORO);
}

void MainWindow::onStayHydratedClicked() {
    LOG4CXX_INFO(logger, "TimerDialog::STAYHYDRATET selected ...");
    openTimerDialog(TimerDialog::TimerType::STAYHYDRATET);
}

void MainWindow::onFreshAirTimerClicked() {
    LOG4CXX_INFO(logger, "TimerDialog::FRESHAIR selected ...");
    openTimerDialog(TimerDialog::TimerType::FRESHAIR);
}

void MainWindow::onWorkingHourTimerClicked() {
    LOG4CXX_INFO(logger, "TimerDialog::WORKINGHOUR selected ...");
    openTimerDialog(TimerDialog::TimerType::WORKINGHOUR);
}

void MainWindow::onBreakTimerClicked() {
    LOG4CXX_INFO(logger, "TimerDialog::BREAK selected ...");
    openTimerDialog(TimerDialog::TimerType::BREAK);
}

void MainWindow::onMovementTimerClicked() {
    LOG4CXX_INFO(logger, "TimerDialog::MOVEMENT selected ...");
    openTimerDialog(TimerDialog::TimerType::MOVEMENT);
}

void MainWindow::onTimerFinished(TimerDialog::TimerType type){
    completedCount++;
    SoundManager::instance().playNotification();
    refreshActiveTimers();
}

void MainWindow::saveSettings() {
    QSettings s("MyCompany", "HealthProductivityApp");
    s.setValue("sound",          ui->soundCheckBox->isChecked());
    s.setValue("popup",          ui->popupCheckBox->isChecked());
    s.setValue("volume",         ui->volumeSlider->value());
    s.setValue("theme",          ui->themeComboBox->currentIndex());
    s.setValue("language",       ui->languageComboBox->currentIndex());
    s.setValue("confirmClose",   ui->confirmCloseCheckBox->isChecked());

    QMessageBox::information(this, "Settings", "Settings safed!");
    ui->statusbar->showMessage("Settings safed", 3000);
}

void MainWindow::resetSettings() {
    auto reply = QMessageBox::question(this, "Reset", "Reset all settings?");
    if (reply != QMessageBox::Yes) {
        return;
    }

    ui->soundCheckBox->setChecked(true);
    ui->popupCheckBox->setChecked(true);
    ui->volumeSlider->setValue(75);
    ui->themeComboBox->setCurrentIndex(0);
    ui->languageComboBox->setCurrentIndex(0);
    ui->confirmCloseCheckBox->setChecked(true);
    ui->statusbar->showMessage("Settings reset", 3000);
}

void MainWindow::onVolumeChanged(int value) {
    ui->volumeValueLabel->setText(QString("%1%").arg(value));
    SoundManager::instance().setVolume(value);
}

void MainWindow::onThemeChanged(int index) {
    ThemeManager::instance().applyTheme(static_cast<ThemeManager::Theme>(index));
}

void MainWindow::onLanguageChanged(int index) {
    const QString localLanguage = (index == 0) ? "de" : "en";
    applyLanguage(localLanguage);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    LOG4CXX_INFO(logger, "Close ...");
    cleanUpTimers();

    if (!activeTimers.isEmpty() && ui->confirmCloseCheckBox->isChecked()) {
        auto reply = QMessageBox::question(this, "Timer activ",
                                           QString("%1 timer still running. Are you want to top them?").arg(activeTimers.size()));
        if (reply != QMessageBox::Yes) {
            event->ignore();
            return;
        }
    }
    event->accept();
}

void MainWindow::refreshActiveTimers() {
    LOG4CXX_INFO(logger, "Refresh active timer ...");
    cleanUpTimers();

    ui->activeCountLabel->setText(QString("active timer: %1").arg(activeTimers.size()));

    ui->activeTimersTable->setRowCount(activeTimers.size());
    for (int index = 0; index < activeTimers.size(); ++index) {
        TimerDialog *timerDialog = activeTimers[index];
        if (!timerDialog) {
            continue;
        }

        ui->activeTimersTable->setItem(index, 0, new QTableWidgetItem(timerDialog->timerTypeName()));
        ui->activeTimersTable->setItem(index, 1, new QTableWidgetItem(timerDialog->formattedTime()));
        ui->activeTimersTable->setItem(index, 2, new QTableWidgetItem(timerDialog->isTimerRunning() ? "running" : "break"));

        if (!ui->activeTimersTable->cellWidget(index, 3)) {
            QPushButton *stopBtn = new QPushButton("stop");
            stopBtn->setStyleSheet("background-color:#e74c3c;color:white;border-radius:3px;padding:4px;");
            connect(stopBtn, &QPushButton::clicked, timerDialog, &QDialog::close);
            ui->activeTimersTable->setCellWidget(index, 3, stopBtn);
        }
    }
}

void MainWindow::stopAllTimers() {
    LOG4CXX_INFO(logger, "Stop all time");
    cleanUpTimers();

    if (activeTimers.isEmpty()) {
        QMessageBox::information(this, "Info", "No active timers");
        return;
    }

    auto reply = QMessageBox::question(this, "Stop all timers",
                                       QString("Do you wanna stop all %1 timer?").arg(activeTimers.size()));

    if (reply == QMessageBox::Yes) {
        const auto timers = activeTimers;
        for (const auto &timer : timers) {
            if (timer) {
                timer->close();
            }
        }
        ui->statusbar->showMessage("All timers have stopped", 3000);
    }
}

void MainWindow::openTimerDialog(const TimerDialog::TimerType &timerType) {
    LOG4CXX_INFO(logger, "Open timer dialog");
    QPointer<TimerDialog> dialog = new TimerDialog(timerType, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    activeTimers.append(dialog);

    // Remove pointer from list before Qt deletes the object
    connect(dialog, &TimerDialog::destroyed, this, [this, dialog]() {
        activeTimers.removeOne(dialog);
    });

    connect(dialog, &TimerDialog::timerFinished, this, &MainWindow::onTimerFinished);

    dialog->show();
    refreshActiveTimers();
    ui->statusbar->showMessage(dialog->timerTypeName() + " started ...", 3000);
}

void MainWindow::loadSettings() {
    QSettings s("MyCompany", "HealthProductivityApp");
    ui->soundCheckBox->setChecked(s.value("sound", true).toBool());
    ui->popupCheckBox->setChecked(s.value("popup", true).toBool());
    ui->volumeSlider->setValue(s.value("volume", 75).toInt());
    ui->themeComboBox->setCurrentIndex(s.value("theme", 0).toInt());
    ui->languageComboBox->setCurrentIndex(s.value("language", 0).toInt());
    ui->confirmCloseCheckBox->setChecked(s.value("confirmClose", true).toBool());

    SoundManager::instance().setVolume(ui->volumeSlider->value());
    SoundManager::instance().setEnabled(ui->themeComboBox->currentIndex());
}

void MainWindow::connectSignals() {
    LOG4CXX_INFO(logger, "Connect signals and slots");
    connect(ui->pomodoroPushButton,&QPushButton::clicked,this,&MainWindow::onPomodoroTimerClicked);
    connect(ui->stayHydratedPushButton,&QPushButton::clicked,this,&MainWindow::onStayHydratedClicked);
    connect(ui->freshAirPushButton,&QPushButton::clicked,this,&MainWindow::onFreshAirTimerClicked);
    connect(ui->workingHoursPushButton,&QPushButton::clicked,this,&MainWindow::onWorkingHourTimerClicked);
    connect(ui->breakPushButton, &QPushButton::clicked, this, &MainWindow::onBreakTimerClicked);
    connect(ui->movementPushButton,&QPushButton::clicked,this,&MainWindow::onMovementTimerClicked);

    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::refreshActiveTimers);
    connect(ui->stopAllButton, &QPushButton::clicked, this, &MainWindow::stopAllTimers);

    connect(ui->saveSettingsButton, &QPushButton::clicked, this, &MainWindow::saveSettings);
    connect(ui->resetSettingsButton, &QPushButton::clicked, this, &MainWindow::resetSettings);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::onVolumeChanged);

    connect(ui->themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onThemeChanged);
    connect(ui->soundCheckBox, &QCheckBox::toggled, &SoundManager::instance(), &SoundManager::setEnabled);
    connect(ui->languageComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onLanguageChanged);
}

void MainWindow::cleanUpTimers() {
    LOG4CXX_INFO(logger, "Remove all destroyed timers from active timers Qlist");
    activeTimers.erase(
        std::remove_if(activeTimers.begin(), activeTimers.end(),
                       [](const QPointer<TimerDialog> &p){ return p.isNull();}),
        activeTimers.end());
}

void MainWindow::applyLanguage(const QString &localLanguage) {
    qApp->removeTranslator(translator);
    if(translator->load(QString(":/translations/HealthAndProductivityAssistant_%1.ts").arg(localLanguage))){
        qApp->installTranslator(translator);
    }
}
