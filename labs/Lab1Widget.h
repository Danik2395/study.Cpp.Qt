#pragma once
#include <QTabWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <QValidator>
#include <qvalidator.h>
#include "../Interfaces.h"

#define LAB1_HSPACING 30
#define LAB1_VSPACING 20
#define LAB1_EDIT_MAX_SIZE 100, 30
#define LAB1_EXIT_MAX_SIZE 100, 100
#define LAB1_MAX_EMPLOYEES 1000

namespace Lab1
{
    class Call
    {
        int easy, medium, hard;
        int policeman, special_forces, detective;

        public:
        enum Level {
            EASY,
            MEDIUM,
            HARD
        };

        enum Profession
        {
            POLICEMAN,
            SPECIAL_FORCES,
            DETECTIVE
        };

        Call() : easy(0), medium(0), hard(0), policeman(0), special_forces(0), detective(0) {}

        int get_level(Level level);
        int get_level_percent(Level level);
        int get_level_all();

        int get_profession(Profession profession);
        int get_profession_percent(Profession profession);
        int get_profession_all();

        void set_level(Level level, int value);
        void set_profession(Profession profession, int value);
    };
}
using namespace Lab1;

class Lab1Widget : public QWidget, public ILabWindow, public Threaded<void>
{
    Q_OBJECT

signals:
    void close_requested();

public:
    Lab1Widget(QString name = "Lab", QWidget* parent = nullptr) : QWidget(parent), Threaded<void>(parent), name_(name)
    {
        employees_label = new QLabel("Employees: ", this);
        police_label          = new QLabel("Policemans: ", this);
        specialF_label        = new QLabel("Special Forces: ", this);
        detect_label          = new QLabel("Detectives: ", this);

        employees_label->setProperty("call_templ", "Employees: %1");
        police_label->setProperty("call_templ", "Policemans: %1");
        specialF_label->setProperty("call_templ", "Special Forces: %1");
        detect_label->setProperty("call_templ", "Detectives: %1");

        police_edit   = new QLineEdit("", this);
        specialF_edit = new QLineEdit("", this);
        detect_edit   = new QLineEdit("", this);

        auto* employees_validator = new QIntValidator(0, LAB1_MAX_EMPLOYEES, this);
        police_edit->setValidator(employees_validator);
        specialF_edit->setValidator(employees_validator);
        detect_edit->setValidator(employees_validator);

        chbx_police   = new QCheckBox(this);
        chbx_specialF = new QCheckBox(this);
        chbx_detect   = new QCheckBox(this);

        auto* btn_exit = new QPushButton("Exit", this);
        btn_exit->setMaximumSize(LAB1_EXIT_MAX_SIZE);

        auto* vlayout  = new QVBoxLayout(this);

        vlayout->addWidget(employees_label);

        vlayout->addSpacing(LAB1_VSPACING);

        vlayout->addWidget(police_edit);
        auto* police_layout = new QHBoxLayout();
        vlayout->addLayout(police_layout);
        police_layout->addWidget(chbx_police);
        police_layout->addWidget(police_label);
        police_layout->addSpacing(LAB1_HSPACING);
        police_edit->setMaximumSize(LAB1_EDIT_MAX_SIZE);
        police_layout->addStretch(1);

        vlayout->addSpacing(LAB1_VSPACING);

        vlayout->addWidget(specialF_edit);
        auto* specF_layout = new QHBoxLayout();
        vlayout->addLayout(specF_layout);
        specF_layout->addWidget(chbx_specialF);
        specF_layout->addWidget(specialF_label);
        specF_layout->addSpacing(LAB1_HSPACING);
        specialF_edit->setMaximumSize(LAB1_EDIT_MAX_SIZE);
        specF_layout->addStretch(1);

        vlayout->addSpacing(LAB1_VSPACING);

        vlayout->addWidget(detect_edit);
        auto* detect_layout = new QHBoxLayout();
        vlayout->addLayout(detect_layout);
        detect_layout->addWidget(chbx_detect);
        detect_layout->addWidget(detect_label);
        detect_layout->addSpacing(LAB1_HSPACING);
        detect_edit->setMaximumSize(LAB1_EDIT_MAX_SIZE);
        detect_layout->addStretch(1);

        vlayout->addSpacing(LAB1_VSPACING);
        vlayout->addWidget(btn_exit);

        vlayout->addStretch(1);

        // Not slots because it can't get more arguments than signal has sent
        // Properties are available but it's more fuss
        connect(chbx_police,
                &QCheckBox::checkStateChanged,
                this,
                [this](Qt::CheckState check_state){ show_employees(check_state, Call::Profession::POLICEMAN); });

        connect(chbx_specialF,
                &QCheckBox::checkStateChanged,
                this,
                [this](Qt::CheckState check_state){ show_employees(check_state, Call::Profession::SPECIAL_FORCES); });

        connect(chbx_detect,
                &QCheckBox::checkStateChanged,
                this,
                [this](Qt::CheckState check_state){ show_employees(check_state, Call::Profession::DETECTIVE); });

        connect(police_edit,
                &QLineEdit::returnPressed,
                this,
                [this](){ set_employees(Call::Profession::POLICEMAN); });

        connect(specialF_edit,
                &QLineEdit::returnPressed,
                this,
                [this](){ set_employees(Call::Profession::SPECIAL_FORCES); });

        connect(detect_edit,
                &QLineEdit::returnPressed,
                this,
                [this](){ set_employees(Call::Profession::DETECTIVE); });

        connect(btn_exit, &QPushButton::clicked, this, [this](){ emit close_requested(); });
    }

    QString get_name(void) const
    {
        return name_;
    }

private:
    QString name_;
    QLabel* employees_label;
    QLabel* police_label;
    QLabel* specialF_label;
    QLabel* detect_label;

    QLineEdit* police_edit;
    QLineEdit* specialF_edit;
    QLineEdit* detect_edit;

    QCheckBox* chbx_police;
    QCheckBox* chbx_specialF;
    QCheckBox* chbx_detect;

    Call call;

    void set_employees(Call::Profession prof)
    {
        QLineEdit* target_edit;

        switch (prof)
        {
            case Call::Profession::POLICEMAN:      target_edit = police_edit;   break;
            case Call::Profession::SPECIAL_FORCES: target_edit = specialF_edit; break;
            case Call::Profession::DETECTIVE:      target_edit = detect_edit;   break;
            default: return;
        }

        bool ok_int = false;
        int employee_count = target_edit->text().toInt(&ok_int);
        if (ok_int)
        {
            call.set_profession(prof, employee_count);
            employees_label->setText(employees_label->property("call_templ").toString().arg(call.get_level_all()));
            update_employees();
        }

        target_edit->clear();
    }

    void show_employees(Qt::CheckState check_state, Call::Profession prof)
    {
        QLabel* show_label;
        QString text_to_set = "";

        switch (prof)
        {
            case Call::Profession::POLICEMAN:      show_label = police_label;   break;
            case Call::Profession::SPECIAL_FORCES: show_label = specialF_label; break;
            case Call::Profession::DETECTIVE:      show_label = detect_label;   break;
            default: return;
        }

        if (check_state == Qt::Unchecked)
        {
            text_to_set = "";
        }
        else if (check_state == Qt::Checked)
        {
            text_to_set = QString::number(call.get_profession_percent(prof));
        }

        show_label->setText(show_label->property("call_templ").toString().arg(text_to_set));
    }

    void update_employees()
    {
        auto police_check = chbx_police->checkState();
        auto specF_check = chbx_specialF->checkState();
        auto detect_check = chbx_detect->checkState();

        show_employees(police_check, Call::Profession::POLICEMAN);
        show_employees(specF_check, Call::Profession::SPECIAL_FORCES);
        show_employees(detect_check, Call::Profession::DETECTIVE);

    }
};
