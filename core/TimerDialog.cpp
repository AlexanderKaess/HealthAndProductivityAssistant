#include <QMessageBox>
#include <QCloseEvent>
#include "TimerDialog.h"
#include "../ui/ui_TimerDialog.h"

TimerDialog::TimerDialog(TimerType type, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TimerDialog)
    , logger(log4cxx::Logger::getLogger("HealthLogger.TimerDialog"))
    , timer(new QTimer(this))
    , currentType(type)
    , remainingTimeSec(0)
    , isRunning(false)
    , initialSeconds(0)
{
    LOG4CXX_INFO(logger, "TimerDialog started ...");
    ui->setupUi(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &TimerDialog::updateTimerCountDown);

    setupForTimerType(type);

    connect(ui->startButton, &QPushButton::clicked, this, &TimerDialog::onStartClicked );
}

TimerDialog::~TimerDialog()
{
    delete ui;
}

QString TimerDialog::timerTypeName() const {
    QString result = "DEFAULT_TIMER";
    switch (currentType) {
        case POMODORO:
            result = "POMODORO";
            break;
        case STAYHYDRATET:
            result = "STAYHYDRATET";
            break;
        case FRESHAIR:
            result = "FRESHAIR";
            break;
        case WORKINGHOUR:
            result = "WORKINGHOUR";
            break;
        case BREAK:
            result = "BREAK";
            break;
        case MOVEMENT:
            result = "MOVEMENT";
            break;
    }

    LOG4CXX_INFO(logger, "TimerType is: " <<  result.toStdString());
    return result;
}

QString TimerDialog::formattedTime() const {
    int hours = remainingTimeSec / 3600;
    int minutes = (remainingTimeSec % 3600) / 60;
    int seconds = remainingTimeSec % 60;
    QString result = QString::number(hours) + ":" + QString::number(minutes) + ":" + QString::number(seconds);
    return result;
}

void TimerDialog::onStartClicked() {
    LOG4CXX_INFO(logger, "Start timer clicked ...");
    if (!timer->isActive()) {
        LOG4CXX_INFO(logger, "Timer is not activ, start timer now");
        if (ui->minutesSpinBox) {
            int minutes = ui->minutesSpinBox->value();
            if ( remainingTimeSec == initialSeconds) {
                initialSeconds = minutes * 60;
                remainingTimeSec = initialSeconds;
            }
        }
        timer->start();
        ui->startButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
    }
}

void TimerDialog::onStopClicked() {
    LOG4CXX_INFO(logger, "Stop timer clicked ...");
    timer->stop();
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
}

void TimerDialog::onResetClicked() {
    LOG4CXX_INFO(logger, "Reset timer clicked ...");
    timer->stop();
    remainingTimeSec = initialSeconds;

    updateDisplay();

    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
}

void TimerDialog::updateTimerCountDown() {
    if (remainingTimeSec > 0) {
        LOG4CXX_INFO(logger, "Update timer");
        remainingTimeSec--;
        updateDisplay();
        emit timerTick(remainingTimeSec);
    } else {
        LOG4CXX_INFO(logger, "Timer stopped");
        timer->stop();
        QApplication::beep();
        emit timerFinished(currentType);
        QMessageBox::information(this, "Timer stopped",
                                 QString("%1-Timer has expired").arg(timerTypeName()));
        ui->startButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
    }
}

void TimerDialog::closeEvent(QCloseEvent *event) {
    if (timer->isActive()) {
        auto reply = QMessageBox::question(this, "Timer is running",
                                           "The timer is still running. Do you want to close it??",
                                           QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes) {
            event->ignore();
            return;
        }
        timer->stop();
    }
    event->accept();
}

void TimerDialog::setupForTimerType(const TimerType &type) {
    setWindowTitle(timerTypeName() + " Timer");
    initialSeconds = getDefaultTimeForTimerType(type);
    remainingTimeSec = initialSeconds;

    if (ui->titleLabel){
        ui->titleLabel->setText(timerTypeName());
    }
    if (ui->minutesSpinBox){
        ui->minutesSpinBox->setValue(initialSeconds / 60);
    }

    updateDisplay();
}

void TimerDialog::updateDisplay() {
    LOG4CXX_INFO(logger, "Update display of timer");
    if (ui->timeDisplayLabel){
        ui->timeDisplayLabel->setText(formattedTime());
    }
}

int TimerDialog::getDefaultTimeForTimerType(const TimerType &type) const {
    int result{};
    switch (type) {
    case POMODORO:
        result = 25 * 60;
        break;
    case STAYHYDRATET:
        result = 45 * 60;
        break;
    case FRESHAIR:
        result = 60 * 60;
        break;
    case WORKINGHOUR:
        result = 8 * 3600;
        break;
    case BREAK:
        result = 5 * 60;
        break;
    case MOVEMENT:
        result = 30 * 60;
        break;
    }

    LOG4CXX_INFO(logger, "Default time for  is: " <<  std::to_string(result));
    return result;
}
