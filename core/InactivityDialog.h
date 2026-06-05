#pragma once

#include <QDialog>
#include <memory.h>
#include <QApplication>
#include <QScreen>

namespace Ui { class InactivityDialog; }

class InactivityDialog : public QDialog
{

    Q_OBJECT

public:
    explicit InactivityDialog(QWidget* parent = nullptr);
    ~InactivityDialog();

private:
    Ui::InactivityDialog* ui;
};
