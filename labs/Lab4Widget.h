#pragma once
#include <QTabWidget>
#include <QString>
#include <QLabel>
#include <qobject.h>
#include "../Interfaces.h"
#include "classes/City.h"
#include "../ui/ui_Lab4Widget.h"

namespace Ui
{
    class City_Blocks_ui;
}

class Lab4Widget : public QWidget, public ILabWindow
{
    Q_OBJECT
    Ui::City_Blocks_ui* city_blocks_ui;

signals:
    void close_requested();

public:
    Lab4Widget(QString name = "Lab", QWidget* parent = nullptr) : QWidget(parent), name_(name)
    {
        city_blocks_ui = new Ui::City_Blocks_ui;
        city_blocks_ui->setupUi(this);
    }

    QString get_name(void) const
    {
        return name_;
    }

private:
    QString name_;

};
