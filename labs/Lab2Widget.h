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
#include "../ui/ui_Lab2Widget.h"

namespace Ui
{
    // Name of the object from autogen .h file
    class CoolWidget;
}

// Because the ui is not directly a widget
class ImageWidget : public QWidget
{
    Q_OBJECT

    Ui::CoolWidget* cool_widget; // Pointer to access the generated widget

public:
    ImageWidget(QWidget* parent = nullptr) : QWidget(parent), cool_widget(nullptr)
    {
        cool_widget = new Ui::CoolWidget;
        cool_widget->setupUi(this); // Method to build the widget tree on the parent widget
    }

    ~ImageWidget() { delete cool_widget; }
};

#define LAB2_BTN_MAX_SIZE 100, 30

class Lab2Widget : public QWidget, public ILabWindow
{
    Q_OBJECT

signals:
    void close_requested();

public:
    Lab2Widget(QString name = "Lab", QWidget* parent = nullptr) : QWidget(parent), image_widget(nullptr)
                                                                  // , ui(new Ui::CoolWidget)
    {
        auto* btn_create_image = new QPushButton("Show Image", this);
        btn_create_image->setMaximumSize(LAB2_BTN_MAX_SIZE);

        auto* vlayout = new QVBoxLayout(this);
        auto* hlayout = new QHBoxLayout();

        vlayout->addLayout(hlayout);

        hlayout->addStretch(1);
        hlayout->addWidget(btn_create_image);
        hlayout->addStretch(1);

        connect(btn_create_image, &QPushButton::clicked, this, &Lab2Widget::open_image_widget);
    }

    QString get_name(void) const
    {
        return name_;
    }

private:
    QString name_;
    ImageWidget* image_widget;

    void open_image_widget()
    {
        if (image_widget) return;

        image_widget = new ImageWidget(this);

        image_widget->setAttribute(Qt::WA_DeleteOnClose); // On close call destructor
        image_widget->setParent(this, Qt::Dialog); // By default Qt treat it like widget.
                                                   // It will be in left top if this not set (if QWidget initialized with parent).

        connect(image_widget, &QObject::destroyed, this, [this]() { image_widget = nullptr; }); // Set nullptr on close

        image_widget->show();
        image_widget->raise();    // Most up window
        image_widget->setFocus(); // Mouse focus on this window
    }
};
