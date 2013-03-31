#ifndef EDGESETTING_H
#define EDGESETTING_H


#include <QWidget>
#include "ui_edge.h"


class EdgeSetting : public QWidget, public Ui::EdgeSetting
{
    Q_OBJECT

public:
    EdgeSetting(QWidget *parent = 0);


};

#endif // EDGESETTING_H
