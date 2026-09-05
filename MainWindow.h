#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QTabBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <qboxlayout.h>
#include "labs/LabTestWidget.h"

#define PINNED_TAB_INDEX 0
#define MAX_BUTTON_SIZE 100, 100

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent)
    {
        tab_widget = new QTabWidget(this);
        tab_widget->setTabsClosable(true);
        auto* tab_bar = tab_widget->tabBar();

        setCentralWidget(tab_widget);

        connect(tab_widget, &QTabWidget::tabCloseRequested, this, [this](int index) {
            QWidget* widget = tab_widget->widget(index);
            tab_widget->removeTab(index);
            widget->deleteLater();
        });

        auto* hub_widget = new QWidget();
        auto* hub_layout = new QVBoxLayout(hub_widget);

        auto* btn_test_lab = new QPushButton("Test", hub_widget);
        hub_layout->addWidget(btn_test_lab);
        btn_test_lab->setMaximumSize(MAX_BUTTON_SIZE);

        auto* btn_dummy = new QPushButton("dummy", hub_widget);
        hub_layout->addWidget(btn_dummy);

        hub_layout->addStretch(1);

        tab_widget->addTab(hub_widget, "Hub");

        tab_bar->tabButton(PINNED_TAB_INDEX, QTabBar::RightSide)->deleteLater();
        tab_bar->setTabButton(PINNED_TAB_INDEX, QTabBar::RightSide, nullptr);

        connect(btn_test_lab, &QPushButton::clicked, this, [this]() {
            auto* new_lab = new LabTestWidget("test_lab");
            int index = tab_widget->addTab(new_lab, new_lab->get_name());
            tab_widget->setCurrentIndex(index);
        });
    }

private:
    QTabWidget* tab_widget;
};
