#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>
#include <QStandardPaths>
#include <cstdio>
#include <exception>
#include <QFileInfo>
#include <QStyledItemDelegate>
#include <QDir>
#include <qnamespace.h>
#include <stdexcept>
#include "Report.h"

class ReadOnlyDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        return nullptr; // Not creating edit
    }
};

class AsciiTableModel : public QSqlTableModel
{
public:
    using QSqlTableModel::QSqlTableModel; // Constructor inharitance

    // Qt::ItemFlags flags(const QModelIndex &index) const override
    // {
    //     Qt::ItemFlags f = QSqlTableModel::flags(index);
    //
    //     if (index.column() != 1)
    //     {
    //         f &= ~Qt::ItemIsEditable; // Set converted string to uneditable
    //     }
    //
    //     return f;
    // }

    bool setData(const QModelIndex& index, const QVariant& value, int role) override
    {
        if (index.column() != 1) return QSqlTableModel::setData(index, value, role);

        if (role == Qt::EditRole)
        {
            int row = index.row();

            std::string ascii_string = value.toString().toStdString();
            QString converted_string;

            try
            {
                converted_string = QString::fromStdString(Report::ascii_to_string(ascii_string));
            }
            catch(std::exception)
            {
                return false;
            }

            QSqlTableModel::setData(this->index(row, 2), converted_string, Qt::EditRole);
        }

        if (!QSqlTableModel::setData(index, value, role)) {
            return false;
        }

        return true;
    };
};

class Ascii_db
{
    QSqlDatabase db;
    AsciiTableModel* model;

    public:
    Ascii_db(QObject* parent)
    {
        QString app_data_path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QString db_path = QDir(app_data_path).filePath("database/ascii_db.db"); // Creade absolute path to db

        QFileInfo db_file_info(db_path);                                          // Get file info
        QDir db_dir = db_file_info.dir();                                         // Get file dir

        if (!db_dir.exists())
        {
            db_dir.mkpath(".");                                                   // mkdir -p
        }

        static const char* ascii_conn = "ascii_conn";
        if (!QSqlDatabase::contains(ascii_conn)) // One connection for app
        {
            db = QSqlDatabase::addDatabase("QSQLITE", ascii_conn);
            db.setDatabaseName(db_path);

            if(!db.open())
            {
                throw std::runtime_error("Ascii database wasn't open.");
            }
        }
        else
        {
            db = QSqlDatabase::database(ascii_conn);
        }

        QSqlQuery query(db);

        query.exec("CREATE TABLE IF NOT EXISTS Ascii_table ("
                    "id integer primary key autoincrement,"
                    "ascii_str text,"
                    "converted_str text"
                    ");"
                );

        model = new AsciiTableModel(parent, db);
        model->setTable("Ascii_table");
        model->select();

        model->setHeaderData(1, Qt::Horizontal, "Ascii string");
        model->setHeaderData(2, Qt::Horizontal, "Converted string");

        model->setEditStrategy(AsciiTableModel::OnFieldChange);
    }

    void add_row()
    {
        model->insertRow(model->rowCount());
    }

    void remove_row(int idx)
    {
        model->removeRow(idx);
    }

    void connect_model_to_table(QTableView* table)
    {
        table->setModel(model);

        ReadOnlyDelegate* readOnlyDelegate = new ReadOnlyDelegate(table);
        table->setItemDelegateForColumn(2, readOnlyDelegate);
        table->setItemDelegateForColumn(0, readOnlyDelegate);
    }
};
