#ifndef RGBDIALOG_H
#define RGBDIALOG_H


#include <QDialog>
#include "ui_rgbdialog.h"


class RgbDialog : public QDialog, public Ui::RgbDialog
{
    Q_OBJECT


public:
    RgbDialog(QDialog *parent = 0);

public slots:
    void resetRgb();


};


#endif // RGBDIALOG_H
