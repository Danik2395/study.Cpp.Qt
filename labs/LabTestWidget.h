#pragma once
#include <QTabWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QVBoxLayout>
#include "../Interfaces.h"
#include "ssstl/random.h"

class LabTestWidget : public QWidget, public ILabWindow, public Threaded<int>
{
    Q_OBJECT

public:
    LabTestWidget(QString name = "Lab", QWidget* parent = nullptr) : QWidget(parent), Threaded<int>(parent), name_(name)
    {
        auto* layout  = new QVBoxLayout(this);
        auto* btn     = new QPushButton("Test sleep button", this);
        result_label_ = new QLabel("Test label", this);

        layout->addWidget(btn);
        layout->addWidget(result_label_);

        to_thread(this,
                  btn,
                  &QPushButton::clicked,
                  [this]()->int{ return work_test(); },
                  [this](){ ret_test(); });
    }

    QString get_name(void) const
    {
        return name_;
    }

private:
    QString name_;
    QLabel* result_label_;

    int work_test()
    {
        QThread::sleep(3);
        return ssstl::random<int>(0, 5);
    }

    void ret_test()
    {
        result_label_->setText("Result: " + QString::number(watcher->result()));
    }
};
