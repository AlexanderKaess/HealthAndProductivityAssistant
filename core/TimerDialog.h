#pragma once

#include <QDialog>
#include <log4cxx/logger.h>

namespace Ui {
class TimerDialog;
}

class TimerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimerDialog(QWidget *parent = nullptr);
    TimerDialog(QWidget *parent = nullptr, QString message = "default");
    ~TimerDialog();

private slots:
    void on_okPushButton_clicked();

    void on_cancelPushButton_clicked();

private:
    Ui::TimerDialog *ui;
    log4cxx::LoggerPtr logger;
};
