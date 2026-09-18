#pragma once
#include <QWidget>
#include <QString>
#include <qabstractitemmodel.h>
#include <QSqlTableModel>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include "../Interfaces.h"
#include "../ui/ui_Lab5Widget.h"
#include "classes/Ascii_db.h"

namespace Ui
{
    class Lab5Widget_ui;
}

class Lab5Widget : public QWidget, public ILabWindow
{
    Q_OBJECT

public:
    Lab5Widget(QString name = "Lab", QWidget* parent = nullptr) :
        QWidget(parent),
        name_(name),
        ascii_db(this),
        current_row(-1)
    {
        auto lab5_ui = Ui::Lab5Widget_ui(); // Ascii_db could throw runtime_error
        lab5_ui.setupUi(this);
        ascii_db.connect_model_to_table(lab5_ui.table_ascii);

        // connect(lab5_ui.table_ascii, &QTableView::clicked, this, [this](const QModelIndex& idx){ current_row = idx.row(); });
        connect(lab5_ui.table_ascii->selectionModel(), &QItemSelectionModel::currentChanged,
                this, [this](const QModelIndex& current, const QModelIndex& previous) {
                if (current.isValid()) {
                current_row = current.row();
                }
                });

        connect(lab5_ui.btn_add,    &QPushButton::clicked, this, [this](){ ascii_db.add_row(); });
        connect(lab5_ui.btn_remove, &QPushButton::clicked, this, [this](){
                if (current_row == -1) return;
                ascii_db.remove_row(current_row);
                current_row = -1;
                });
    }

    QString get_name(void) const
    {
        return name_;
    }

private:
    QString name_;
    Ascii_db ascii_db;
    int current_row;
};
