
#include <QtGui>

#include "edgesetting.h"



EdgeSetting::EdgeSetting(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    ltspinBox->setRange(0,255);
    ltspinBox->setValue(20);
    htspinBox->setRange(0,255);
    htspinBox->setValue(30);
    ksspinBox->setRange(3,7);
    ksspinBox->setSingleStep(2);
    ksspinBox->setValue(3);






}
