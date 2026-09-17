#pragma once
#include <QTabWidget>
#include <QString>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <qlabel.h>
#include <qnamespace.h>
#include <qobject.h>
#include "../Interfaces.h"
#include "classes/City.h"
#include "../ui/ui_Lab4Widget.h"
#include "ssstl/random.h"

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

        connect(city_blocks_ui->chbx_criminality,
                &QCheckBox::checkStateChanged,
                this,
                [this](Qt::CheckState check_state){ show_labels(check_state, CRIME); });

        connect(city_blocks_ui->chbx_most_criminal_blocks,
                &QCheckBox::checkStateChanged,
                this,
                [this](Qt::CheckState check_state){ show_labels(check_state, MOST_BLOCKS); });

        connect(city_blocks_ui->pushButton,
                &QPushButton::clicked,
                this,
                [this](){
                city.gen_city();
                show_labels(city_blocks_ui->chbx_criminality->checkState(), CRIME);
                show_labels(city_blocks_ui->chbx_most_criminal_blocks->checkState(), MOST_BLOCKS);
                });
    }

    ~Lab4Widget() { delete city_blocks_ui; }

    QString get_name(void) const
    {
        return name_;
    }

private:

    QString name_;
    City city;

    enum CITY_DATA_TYPE
    {
        CRIME,
        MOST_BLOCKS
    };

    void show_labels(Qt::CheckState chbx_state, CITY_DATA_TYPE city_data_type)
    {
        QLabel* target_label;
        QString text_to_set;

        switch (city_data_type)
        {
            case CRIME:
                target_label = city_blocks_ui->label_criminality;
                text_to_set = QString::number(city.get_median_criminality());
                break;

            case MOST_BLOCKS:
                target_label = city_blocks_ui->label_most_criminal_blocks;
                text_to_set = QString::number(city.get_most_crime_blocks());
                break;

            default: return;
        }

        if (chbx_state == Qt::Unchecked)
        {
            target_label->setText("");
        }
        else if (chbx_state == Qt::Checked)
        {

            target_label->setText(std::move(text_to_set));
        }
    }
};
