#include "TimerDialog.h"
#include "../ui/ui_TimerDialog.h"

TimerDialog::TimerDialog(TimerType type, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TimerDialog)
    , logger(log4cxx::Logger::getLogger("HealthLogger.TimerDialog"))
    , currentType(type)
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

void TimerDialog::updateTimer() {}
