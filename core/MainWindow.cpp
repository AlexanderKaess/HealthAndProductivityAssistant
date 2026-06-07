#include "MainWindow.h"
#include "../ui/ui_MainWindow.h"

#include <QTableWidgetItem>
#include <QPushButton>

MainWindow::MainWindow(AppController* controller, QWidget* parent)
    : QMainWindow(parent)
    , logger(log4cxx::Logger::getLogger("HealthLogger.MainWindow"))
    , ui(new Ui::MainWindow)
    , controller(controller)
{
    ui->setupUi(this);
    connectUiSignals();

    // connection of controller and view
    connect(controller, &AppController::activeTimersChanged, this, &MainWindow::onActiveTimersChanged);
    connect(controller, &AppController::statusMessage, this, &MainWindow::onStatusMessage);
    connect(controller, &AppController::monitoringStatusChanged, this, &MainWindow::onMonitoringStatusChanged);
    connect(controller, &AppController::languageChanged, this, &MainWindow::onLanguageChanged);

    // connection of settings-model and view
    const AppSettings* appSettings = controller->getAppSettings();
    connect(appSettings, &AppSettings::volumeChanged,
            this, [this](int v){
                ui->volumeSlider->setValue(v);
                ui->volumeValueLabel->setText(QString("%1%").arg(v));
            });
    connect(appSettings, &AppSettings::soundEnabledChanged, ui->soundCheckBox, &QCheckBox::setChecked);
    connect(appSettings, &AppSettings::popupEnabledChanged, ui->popupCheckBox, &QCheckBox::setChecked);
    connect(appSettings, &AppSettings::confirmCloseChanged, ui->confirmCloseCheckBox, &QCheckBox::setChecked);
    connect(appSettings, &AppSettings::themeIndexChanged, ui->themeComboBox, &QComboBox::setCurrentIndex);
    connect(appSettings, &AppSettings::languageIndexChanged, ui->languageComboBox, &QComboBox::setCurrentIndex);

    auto* refreshTimer = new QTimer(this);
    refreshTimer->setInterval(1000);
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::onActiveTimersChanged);
    refreshTimer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initFromSettings(const AppSettings* settings)
{
    // block signal to prevent feedback from occurring
    QSignalBlocker blocker1(ui->soundCheckBox);
    QSignalBlocker blocker2(ui->popupCheckBox);
    QSignalBlocker blocker3(ui->volumeSlider);
    QSignalBlocker blocker4(ui->themeComboBox);
    QSignalBlocker blocker5(ui->languageComboBox);
    QSignalBlocker blocker6(ui->confirmCloseCheckBox);

    ui->soundCheckBox->setChecked(settings->getSoundEnabled());
    ui->popupCheckBox->setChecked(settings->getPopupEnabled());
    ui->volumeSlider->setValue(settings->getVolume());
    ui->volumeValueLabel->setText(QString("%1%").arg(settings->getVolume()));
    ui->themeComboBox->setCurrentIndex(settings->getThemeIndex());
    ui->languageComboBox->setCurrentIndex(settings->getLanguageIndex());
    ui->confirmCloseCheckBox->setChecked(settings->getConfirmClose());
}

void MainWindow::connectUiSignals()
{
    connect(ui->pomodoroPushButton, &QPushButton::clicked, this, &MainWindow::pomodoroRequested);
    connect(ui->stayHydratedPushButton,&QPushButton::clicked, this, &MainWindow::stayHydratedRequested);
    connect(ui->freshAirPushButton, &QPushButton::clicked, this, &MainWindow::freshAirRequested);
    connect(ui->workingHoursPushButton, &QPushButton::clicked, this, &MainWindow::workingHourRequested);
    connect(ui->breakPushButton, &QPushButton::clicked, this, &MainWindow::breakRequested);
    connect(ui->movementPushButton, &QPushButton::clicked, this, &MainWindow::movementRequested);

    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onActiveTimersChanged);
    connect(ui->stopAllButton, &QPushButton::clicked, this, &MainWindow::stopAllTimersRequested);

    connect(ui->saveSettingsButton, &QPushButton::clicked, this, &MainWindow::saveSettingsRequested);
    connect(ui->resetSettingsButton, &QPushButton::clicked, this, &MainWindow::resetSettingsRequested);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::volumeChangeRequested);
    connect(ui->themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::themeChangeRequested);
    connect(ui->languageComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::languageChangeRequested);
    connect(ui->soundCheckBox, &QCheckBox::toggled, this, &MainWindow::soundEnabledChangeRequested);

    connect(ui->monitoringCheckBox, &QCheckBox::checkStateChanged, this, [this](int state) {
                emit monitoringChangeRequested(
                    state == Qt::Checked,
                    ui->monitoringSpinTimeout->value());
            });
}

void MainWindow::onActiveTimersChanged()
{
    const auto infos = controller->activeTimerInfos();
    ui->activeCountLabel->setText(QString("active timer: %1").arg(infos.size()));
    ui->activeTimersTable->setRowCount(infos.size());

    for (int i = 0; i < infos.size(); ++i) {
        const auto& info = infos[i];
        ui->activeTimersTable->setItem(i, 0, new QTableWidgetItem(info.typeName));
        ui->activeTimersTable->setItem(i, 1, new QTableWidgetItem(info.formattedTime));
        ui->activeTimersTable->setItem(i, 2, new QTableWidgetItem(info.running ? "running" : "stopped"));

        if (!ui->activeTimersTable->cellWidget(i, 3)) {
            auto* button = new QPushButton("stop");
            button->setStyleSheet(
                "background-color:#e74c3c;color:white;"
                "border-radius:3px;padding:4px;");
            TimerDialog* dialog = info.dialog;
            connect(button, &QPushButton::clicked, this,
                    [this, dialog](){
                        controller->stopTimer(dialog);
                    });
            ui->activeTimersTable->setCellWidget(i, 3, button);
        }
    }
}

void MainWindow::onStatusMessage(const QString& msg, int timeoutMs)
{
    statusBar()->showMessage(msg, timeoutMs);
}

void MainWindow::onMonitoringStatusChanged(bool active)
{
    // Set the checkbox without signal feedback
    QSignalBlocker blocker(ui->monitoringCheckBox);
    ui->monitoringCheckBox->setChecked(active);
}

void MainWindow::onLanguageChanged(const QString& /*locale*/)
{
    ui->retranslateUi(this);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    LOG4CXX_INFO(logger, "MainWindow closeEvent");
    closeAccepted = false;
    emit closeRequested();

    if (closeAccepted) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::cancelClose()
{
    closeAccepted = false;
}

void MainWindow::acceptClose() {
    closeAccepted = true;
}
