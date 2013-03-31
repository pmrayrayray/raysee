#ifndef HSLDIALOG_H
#define HSLDIALOG_H


#include <QDialog>
#include "ui_hsldialog.h"


class HslDialog : public QDialog, public Ui::HslDialog

{
    Q_OBJECT

public:
    HslDialog(QDialog *parent = 0);
public slots:
    void resetHsl();



};



#endif // HSLDIALOG_H
