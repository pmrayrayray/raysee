
#include <QtGui>

#include "rgbdialog.h"



RgbDialog::RgbDialog(QDialog *parent)
    : QDialog(parent)
{
    setupUi(this);

    redSlider->setRange(-255,255);
    greenSlider->setRange(-255,255);
    blueSlider->setRange(-255,255);
    redSpin->setRange(-255,255);
    greenSpin->setRange(-255,255);
    blueSpin->setRange(-255,255);
    connect(redSlider,SIGNAL(valueChanged(int)),redSpin,SLOT(setValue(int)));
    connect(greenSlider,SIGNAL(valueChanged(int)),greenSpin,SLOT(setValue(int)));
    connect(blueSlider,SIGNAL(valueChanged(int)),blueSpin,SLOT(setValue(int)));

    connect(redSpin,SIGNAL(valueChanged(int)),redSlider,SLOT(setValue(int)));
    connect(greenSpin,SIGNAL(valueChanged(int)),greenSlider,SLOT(setValue(int)));
    connect(blueSpin,SIGNAL(valueChanged(int)),blueSlider,SLOT(setValue(int)));


    connect(cancelButton,SIGNAL(clicked()),this,SLOT(resetRgb()));
    connect(okButton,SIGNAL(clicked()),this,SLOT(resetRgb()));


}

void RgbDialog::resetRgb()
{

    redSlider->setValue(0);
    greenSlider->setValue(0);
    blueSlider->setValue(0);

}

