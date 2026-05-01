#include "TimerDialog.h"
#include "../ui/ui_TimerDialog.h"

TimerDialog::TimerDialog(TimerType type, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TimerDialog)
    , logger(log4cxx::Logger::getLogger("HealthLogger.TimerDialog"))
    , currentType(type)
    , remainingTime(0)
    , isRunning(false)
{
    LOG4CXX_INFO(logger, "TimerDialog started...");
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TimerDialog::updateTimer);
}

TimerDialog::~TimerDialog()
{
    delete ui;
}

QString TimerDialog::timerTypeName() const {
    switch (currentType) {
    case POMODORO:
        return "POMODORO";
        break;
    case STAYHYDRATET:
        return "STAYHYDRATET";
        break;
    case FRESHAIR:
        return "FRESHAIR";
        break;
    case WORKINGHOUR:
        return "WORKINGHOUR";
        break;
    case BREAK:
        return "BREAK";
        break;
    case MOVEMENT:
        return "MOVEMENT";
        break;
    }
    return "TIMER";
}

QString TimerDialog::formattedTime() const {
    return "";
}

void TimerDialog::updateTimer() {}
