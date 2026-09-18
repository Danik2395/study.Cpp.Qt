#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QTabBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <qboxlayout.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <QString>
#include <vector>
#include <functional>
#include <new>
#include "labs/LabTestWidget.h"
#include "labs/Lab1Widget.h"
#include "labs/Lab2Widget.h"
#include "labs/Lab3Widget.h"
#include "labs/Lab4Widget.h"
#include "labs/Lab5Widget.h"

#define PINNED_TAB_INDEX 0
#define MAX_BUTTON_SIZE 100, 100

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent)
    {
        // Create tab widget with this as parent widget
        tab_widget = new QTabWidget(this);
        tab_widget->setTabsClosable(true);
        auto* tab_bar = tab_widget->tabBar();

        // Set tab widget as central (no side widgets yet)
        setCentralWidget(tab_widget);

        // If closed remove tab and send it to safe delete qeueu
        connect(tab_widget, &QTabWidget::tabCloseRequested, this, [this](int index) {
            QWidget* widget_in_tabWidget = tab_widget->widget(index);
            tab_widget->removeTab(index);
            widget_in_tabWidget->deleteLater();
        });

        // Create hub and assign layout to it
        auto* hub_widget = new QWidget();
        auto* hub_layout = new QVBoxLayout(hub_widget);

        tab_widget->addTab(hub_widget, "Hub");

        // Remove cross from hub (the thing done if setTabsClosable isn't set)
        tab_bar->tabButton(PINNED_TAB_INDEX, QTabBar::RightSide)->deleteLater();
        tab_bar->setTabButton(PINNED_TAB_INDEX, QTabBar::RightSide, nullptr);

        buttons_conf = {
            make_button_conf<LabTestWidget>("Test",  parent),
            make_button_conf<Lab1Widget>   ("Lab 1", parent),
            make_button_conf<Lab2Widget>   ("Lab 2", parent),
            make_button_conf<Lab3Widget>   ("Lab 3", parent),
            make_button_conf<Lab4Widget>   ("Lab 4", parent),
            make_button_conf<Lab5Widget>   ("Lab 5", parent),
        };

        for (const auto& btn_conf : buttons_conf)
        {
            // Create instance of the button, assing it to layout (hub_widged), set size
            auto* btn = new QPushButton(btn_conf.name, hub_widget);
            hub_layout->addWidget(btn);
            btn->setMaximumSize(MAX_BUTTON_SIZE);

            // Connect click on this button to lambda.
            // In labmda call the factory to create instance of LabNWidget
            connect(btn,
                    &QPushButton::clicked,
                    this, // Need `this` to Qt to know on what object lifetime to look (with slots not necessary)
                    [this, btn_conf, parent]() {
                    auto* new_lab = btn_conf.factory(btn_conf.name, parent);
                    int index = tab_widget->addTab(new_lab, btn_conf.name);
                    tab_widget->setCurrentIndex(index);

                    // If new_lab is castable to Lab1Widget
                    // connect tap on it btn_exit to lambda with closing this tab
                    if (auto* lab1 = dynamic_cast<Lab1Widget*>(new_lab))
                    {
                        connect(lab1, &Lab1Widget::close_requested, this, [this, lab1](){
                                int lab1_tab_idx = tab_widget->indexOf(lab1);
                                if (lab1_tab_idx != -1)
                                {
                                    tab_widget->removeTab(lab1_tab_idx);
                                    lab1->deleteLater();
                                }
                                });
                    }
                    });
        }

        // Add invisible QSpacer item (min size 0, stretch to the end of layout)
        hub_layout->addStretch(1);
    }

private:
    QTabWidget* tab_widget;

    struct ButtonConfig {
        QString name;
        std::function<QWidget* (const QString&, QWidget*)> factory;
    };

    std::vector<ButtonConfig> buttons_conf;

    template<typename T>
    ButtonConfig make_button_conf(QString&& lab_name, QWidget* lab_parent)
    {
        return {std::move(lab_name), [](const QString& name, QWidget* parent){ return new T(name, parent); }};
    }
};
