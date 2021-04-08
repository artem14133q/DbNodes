#ifndef RELATIONMAKER_H
#define RELATIONMAKER_H

#include "QMainWindow"
#include "QVector"
#include "QPointer"
#include "QHash"
#include "QComboBox"
#include "QPushButton"
#include "QLineEdit"
#include "QTextBrowser"

#include "../Nodes/TableNode.h"
#include "Table/Column.h"
#include "AbstractModal.h"

namespace DbNodes::Modals {

    class RelationMaker : public Abstract::AbstractModal
    {
        Q_OBJECT

        public:
            explicit RelationMaker(Nodes::Table::Column *fkColumn, const QList<TABLE_POINTER> &tableVector);

        private:
            Nodes::Table::Column *fkColumnParent;
            QString currentPkColumnId, currentFkColumnId;

            QList<TABLE_POINTER> tableVector;

            QHash<QString, TABLE_POINTER> tableList;
            QHash<QString, COLUMN_POINTER> columnsOfSelectedTable;

            QComboBox *columnsOfTable{}, *tablesSelect{};
            QWidget *warningWidget{};
            QPushButton *pbCreate{};
            QLineEdit *search{};
            QTextBrowser *warningText{};

            void initUI();
            void showWarningIfPkNotFound(const bool &enable, const int &errorType);

        protected:
            void selectTable(const TABLE_POINTER &table);
            void filterTable(const QString &filter = "");
            void deleteFilter();
            void confirm() override;
            void exit() override;

        protected slots:
            void selectTableByIndex(const int &index);
            void selectColumn(const int &index);
    };

}

#endif // RELATIONMAKER_H
