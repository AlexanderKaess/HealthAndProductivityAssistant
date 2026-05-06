#include <QPointer>
#include <QCloseEvent>

#include "MainWindow.h"
#include "../ui/ui_MainWindow.h"
#include "TimerDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , logger(log4cxx::Logger::getLogger("HealthLogger.MainWindow"))
{
    ui->setupUi(this);
    setWindowTitle("HealthAndProductivityAssistant");
    LOG4CXX_INFO(logger, "Mainwindow started ...");

    // Connect buttons to slots
    connect(ui->pomodoroPushButton,&QPushButton::clicked,this,&MainWindow::onPomodoroTimerClicked);
    connect(ui->stayHydratedPushButton,&QPushButton::clicked,this,&MainWindow::onStayHydratedClicked);
    connect(ui->freshAirPushButton,&QPushButton::clicked,this,&MainWindow::onFreshAirTimerClicked);
    connect(ui->workingHoursPushButton,&QPushButton::clicked,this,&MainWindow::onWorkingHourTimerClicked);
    connect(ui->breakPushButton, &QPushButton::clicked, this, &MainWindow::onBreakTimerClicked);
    connect(ui->movementPushButton,&QPushButton::clicked,this,&MainWindow::onMovementTimerClicked);

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
    refreshActiveTimers();
}

void MainWindow::saveSettings() {}

void MainWindow::resetSettings() {}

void MainWindow::onVolumeChanged(int value) {
    ui->volumeValueLabel->setText(QString("%1%").arg(value));
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
    ui->completedCountLabel->setText(QString("done: %1").arg(completedCount));

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

void MainWindow::cleanUpTimers() {
    // remove all destroyed TimerDialogs
    activeTimers.erase(
        std::remove_if(activeTimers.begin(), activeTimers.end(),
                       [](const QPointer<TimerDialog> &p){ return p.isNull();}),
        activeTimers.end());
}
