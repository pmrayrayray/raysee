
#include <QtGui>

#include "hsvdialog.h"



HsvDialog::HsvDialog(QDialog *parent)
    : QDialog(parent)
{
    setupUi(this);



    hueSlider->setRange(-359,359);
    satuSlider->setRange(-255,255);
    valSlider->setRange(-255,255);
    hueSpin->setRange(-359,359);
    satuSpin->setRange(-255,255);
    valSpin->setRange(-255,255);
    connect(hueSlider,SIGNAL(valueChanged(int)),hueSpin,SLOT(setValue(int)));
    connect(satuSlider,SIGNAL(valueChanged(int)),satuSpin,SLOT(setValue(int)));
    connect(valSlider,SIGNAL(valueChanged(int)),valSpin,SLOT(setValue(int)));

    connect(hueSpin,SIGNAL(valueChanged(int)),hueSlider,SLOT(setValue(int)));
    connect(satuSpin,SIGNAL(valueChanged(int)),satuSlider,SLOT(setValue(int)));
    connect(valSpin,SIGNAL(valueChanged(int)),valSlider,SLOT(setValue(int)));

    connect(cancelButton,SIGNAL(clicked()),this,SLOT(resetHsv()));
    connect(okButton,SIGNAL(clicked()),this,SLOT(resetHsv()));
}

void HsvDialog::resetHsv()
{

    hueSlider->setValue(0);
    satuSlider->setValue(0);
    valSlider->setValue(0);

}


