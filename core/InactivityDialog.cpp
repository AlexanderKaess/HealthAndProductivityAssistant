#include "InactivityDialog.h"
#include "../ui/ui_InactivityDialog.h"

InactivityDialog::InactivityDialog(QWidget* parent)
    : QDialog(parent, Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint)
    , ui(new Ui::InactivityDialog)
{
    ui->setupUi(this);
    connect(ui->okPushButton, &QPushButton::clicked, this, &InactivityDialog::accept);

    if (const QScreen* screen = QApplication::primaryScreen())
    {
        const QRect geo = screen->availableGeometry();
        move(geo.center() - rect().center());
    }
}

InactivityDialog::~InactivityDialog()
{
    delete ui;
}
