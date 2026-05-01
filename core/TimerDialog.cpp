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
    QString result = "DEFAULT_TIMER";
    switch (currentType) {
        case POMODORO:
            result = "POMODORO";
            break;
        case STAYHYDRATET:
            result =  "STAYHYDRATET";
            break;
        case FRESHAIR:
            result =  "FRESHAIR";
            break;
        case WORKINGHOUR:
            result =  "WORKINGHOUR";
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
    return "";
}

void TimerDialog::updateTimer() {}
