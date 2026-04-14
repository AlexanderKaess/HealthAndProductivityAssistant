#include "TimerDialog.h"
#include "../ui/ui_TimerDialog.h"

TimerDialog::TimerDialog(QWidget *parent, QString message)
    : QDialog(parent)
    , ui(new Ui::TimerDialog)
    , logger(log4cxx::Logger::getLogger("HealthLogger.TimerDialog"))
{
    LOG4CXX_INFO(logger, "TimerDialog started...");
    ui->setupUi(this);
    ui->label->setText(message);
}

TimerDialog::~TimerDialog()
{
    delete ui;
}

void TimerDialog::on_okPushButton_clicked()
{
    LOG4CXX_INFO(logger, "ok button clicked");
    close();
}

void TimerDialog::on_cancelPushButton_clicked()
{
    LOG4CXX_INFO(logger, "cancel button clicked");
    close();
}
