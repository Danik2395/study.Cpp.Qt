#pragma once
#include <QTabWidget>
#include <QWidget>
#include <QPushButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QValidator>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qnamespace.h>
#include <qobject.h>
#include <new>
#include <qpushbutton.h>
#include "../Interfaces.h"
#include "../ui/ui_Lab3Widget_panel.h"
#include "../ui/ui_Lab3Widget_show.h"
#include "classes/Prison.h"

constexpr size_t PRISON_CELLS_COUNT = 15;
constexpr int LAB3_MAX_CRIMINALS = 10;

namespace Ui
{
    class Prison_Panel;
    class Prison_Show;
}

// Because the ui is not directly a widget
class Prison_Show_Widget : public QWidget
{
    Q_OBJECT

    Ui::Prison_Show* prison_show_ui;

public:
    Prison_Show_Widget(Prison* prison, QWidget* parent = nullptr) :
        QWidget(parent),
        prison_show_ui(nullptr),
        prison(prison)
    {
        prison_show_ui = new Ui::Prison_Show;
        prison_show_ui->setupUi(this); // Method to build the widget tree on the parent widget
        label_criminals_cnt = prison_show_ui->label_criminals_cnt;
        label_crouded_cells_nums = prison_show_ui->label_crouded_cells_nums;
        label_exp_criminals_cell_nums = prison_show_ui->label_exp_criminals_cell_nums;

        label_criminals_cnt->setProperty("criminal_templ", label_criminals_cnt->text() + QString(" %1"));
        label_crouded_cells_nums->setProperty("criminal_templ", label_crouded_cells_nums->text() + QString(" %1"));
        label_exp_criminals_cell_nums->setProperty("criminal_templ", label_exp_criminals_cell_nums->text() + QString(" %1"));
        update_labels();
    }

    ~Prison_Show_Widget() { delete prison_show_ui; }

    void update_labels()
    {
        QString text_count_to_set;

        text_count_to_set = QString::number(prison->get_criminals_count());
        label_criminals_cnt->setText(label_criminals_cnt->property("criminal_templ").toString().arg(text_count_to_set));

        auto create_num_comma_str = [&text_count_to_set](ssstl::Vector<int>&& nums){
            text_count_to_set.clear();
            for (int num : nums)
            {
                text_count_to_set += " " + QString::number(num) + ", ";
            }
            if (!text_count_to_set.isEmpty()) text_count_to_set.chop(2);
        };

        create_num_comma_str(prison->get_crouded_cells_num());
        label_crouded_cells_nums->setText(label_crouded_cells_nums->property("criminal_templ").toString().arg(text_count_to_set));

        create_num_comma_str(prison->get_exp_cells_num());
        label_exp_criminals_cell_nums->setText(label_exp_criminals_cell_nums->property("criminal_templ").toString().arg(text_count_to_set));
    }

private:

    Prison* prison;
    QLabel* label_criminals_cnt;
    QLabel* label_crouded_cells_nums;
    QLabel* label_exp_criminals_cell_nums;
};

class Lab3Widget : public QWidget, public ILabWindow, Threaded<void>
{
    Q_OBJECT

    QString name_;
    Prison_Show_Widget* prison_show_widget;
    Ui::Prison_Panel* prison_panel_ui;

signals:
    void close_requested();

public:
    Lab3Widget(QString name = "Lab", QWidget* parent = nullptr) :
        QWidget(parent),
        prison_show_widget(nullptr),
        prison(PRISON_CELLS_COUNT)
    {
        prison_panel_ui = new Ui::Prison_Panel;
        prison_panel_ui->setupUi(this);

        auto* employees_validator = new QIntValidator(0, LAB3_MAX_CRIMINALS, this);
        prison_panel_ui->edit_cell_11->setValidator(employees_validator);
        prison_panel_ui->edit_cell_12->setValidator(employees_validator);
        prison_panel_ui->edit_cell_13->setValidator(employees_validator);
        prison_panel_ui->edit_cell_14->setValidator(employees_validator);

        connect(prison_panel_ui->btn_random_gen,
                &QPushButton::clicked,
                this,
                [this](){
                prison.random_fill();
                if (!open_prison_show_widget())
                {
                    prison_show_widget->update_labels();
                }
                });

        connect(prison_panel_ui->btn_show_prison,
                &QPushButton::clicked,
                this,
                &Lab3Widget::open_prison_show_widget);

        connect(prison_panel_ui->edit_cell_11,
                &QLineEdit::returnPressed,
                this,
                [this](){ set_special_cell_cnt_panel(Prison::FRST); });

        connect(prison_panel_ui->edit_cell_12,
                &QLineEdit::returnPressed,
                this,
                [this](){ set_special_cell_cnt_panel(Prison::SEC); });

        connect(prison_panel_ui->edit_cell_13,
                &QLineEdit::returnPressed,
                this,
                [this](){ set_special_cell_cnt_panel(Prison::THRD); });

        connect(prison_panel_ui->edit_cell_14,
                &QLineEdit::returnPressed,
                this,
                [this](){ set_special_cell_cnt_panel(Prison::FRTH); });
    }

    ~Lab3Widget() { delete prison_panel_ui; }

    QString get_name(void) const
    {
        return name_;
    }

private:

    Prison prison;

    // Return 1 if widget exists
    // Return 0 if doesn't
    int open_prison_show_widget()
    {
        if (prison_show_widget) return 0;

        prison_show_widget = new Prison_Show_Widget(&prison, this);

        prison_show_widget->setAttribute(Qt::WA_DeleteOnClose);
        prison_show_widget->setParent(this, Qt::Dialog);

        connect(prison_show_widget, &QObject::destroyed, this, [this]() { prison_show_widget = nullptr; });

        // prison_show_widget->init_prison();

        prison_show_widget->show();
        prison_show_widget->raise();
        prison_show_widget->setFocus();

        return 1;
    }

    void set_special_cell_cnt_panel(Prison::Spec_Cells_Num cell_num)
    {
        QLineEdit* target_edit;

        switch (cell_num)
        {
            case Prison::FRST: target_edit = prison_panel_ui->edit_cell_11;   break;
            case Prison::SEC:  target_edit = prison_panel_ui->edit_cell_12;   break;
            case Prison::THRD: target_edit = prison_panel_ui->edit_cell_13;   break;
            case Prison::FRTH: target_edit = prison_panel_ui->edit_cell_14;   break;
            default: return;
        }

        bool ok_int = false;
        int criminals_count = target_edit->text().toInt(&ok_int);
        if (ok_int)
        {
            prison.set_special_cell_cnt(cell_num, criminals_count);

            if (prison_show_widget)
            {
                prison_show_widget->update_labels();
            }
        }

        target_edit->clear();
    }
};
