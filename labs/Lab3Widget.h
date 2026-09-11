#pragma once
#include <QTabWidget>
#include <QWidget>
#include <QPushButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qboxlayout.h>
#include <qnamespace.h>
#include <qobject.h>
#include <new>
#include <qpushbutton.h>
#include "../Interfaces.h"
#include "../ui/ui_Lab3Widget_panel.h"
#include "../ui/ui_Lab3Widget_show.h"

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

    friend class Lab3Widget;

public:
    Prison_Show_Widget(QWidget* parent = nullptr) : QWidget(parent), prison_show_ui(nullptr)
    {
        prison_show_ui = new Ui::Prison_Show;
        prison_show_ui->setupUi(this); // Method to build the widget tree on the parent widget
    }

    ~Prison_Show_Widget() { delete prison_show_ui; }
};

class Lab3Widget : public QWidget, public ILabWindow
{
    Q_OBJECT

    QString name_;
    Prison_Show_Widget* prison_show_widget;
    Ui::Prison_Panel* prison_panel_ui;

signals:
    void close_requested();

public:
    Lab3Widget(QString name = "Lab", QWidget* parent = nullptr) : QWidget(parent), prison_show_widget(nullptr)
    {
        prison_panel_ui = new Ui::Prison_Panel;
        prison_panel_ui->setupUi(this);

        connect(prison_panel_ui->btn_random_gen,
                &QPushButton::clicked,
                this,
                &Lab3Widget::randomgen_prison);

        connect(prison_panel_ui->btn_show_prison,
                &QPushButton::clicked,
                this,
                &Lab3Widget::open_prison_show_widget);
    }

    ~Lab3Widget() { delete prison_panel_ui; }

    QString get_name(void) const
    {
        return name_;
    }

private:

    void open_prison_show_widget()
    {
        if (prison_show_widget) return;

        prison_show_widget = new Prison_Show_Widget(this);

        prison_show_widget->setAttribute(Qt::WA_DeleteOnClose);
        prison_show_widget->setParent(this, Qt::Dialog);

        connect(prison_show_widget, &QObject::destroyed, this, [this]() { prison_show_widget = nullptr; });

        prison_show_widget->show();
        prison_show_widget->raise();
        prison_show_widget->setFocus();
    }

    void randomgen_prison()
    {

    }
};

