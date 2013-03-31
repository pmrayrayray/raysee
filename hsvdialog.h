#ifndef HSVDIALOG_H
#define HSVDIALOG_H


#include <QDialog>
#include "ui_hsvdialog.h"


class HsvDialog : public QDialog, public Ui::HsvDialog

{
    Q_OBJECT

public:
    HsvDialog(QDialog *parent = 0);
public slots:
    void resetHsv();



};






#endif // HSVDIALOG_H
