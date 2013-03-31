
#include <QtGui>

#include "hsldialog.h"



HslDialog::HslDialog(QDialog *parent)
    : QDialog(parent)
{
    setupUi(this);



    hueSlider->setRange(-359,359);
    satuSlider->setRange(-255,255);
    lightSlider->setRange(-255,255);
    hueSpin->setRange(-359,359);
    satuSpin->setRange(-255,255);
    lightSpin->setRange(-255,255);
    connect(hueSlider,SIGNAL(valueChanged(int)),hueSpin,SLOT(setValue(int)));
    connect(satuSlider,SIGNAL(valueChanged(int)),satuSpin,SLOT(setValue(int)));
    connect(lightSlider,SIGNAL(valueChanged(int)),lightSpin,SLOT(setValue(int)));

    connect(hueSpin,SIGNAL(valueChanged(int)),hueSlider,SLOT(setValue(int)));
    connect(satuSpin,SIGNAL(valueChanged(int)),satuSlider,SLOT(setValue(int)));
    connect(lightSpin,SIGNAL(valueChanged(int)),lightSlider,SLOT(setValue(int)));

    connect(cancelButton,SIGNAL(clicked()),this,SLOT(resetHsl()));
    connect(okButton,SIGNAL(clicked()),this,SLOT(resetHsl()));
}

void HslDialog::resetHsl()
{

    hueSlider->setValue(0);
    satuSlider->setValue(0);
    lightSlider->setValue(0);

}
