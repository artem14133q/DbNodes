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
            explicit RelationMaker(Nodes::Table::Column *fkColumn, const QList<Nodes::TablePtr> &tableVector);

        private:
            Nodes::Table::Column *fkColumnParent;
            QString currentPkColumnId, currentFkColumnId;

            QList<Nodes::TablePtr> tableVector;

            QHash<QString, Nodes::TablePtr> tableList;
            QHash<QString, Nodes::Table::ColumnPrt> columnsOfSelectedTable;

            QComboBox *columnsOfTable{}, *tablesSelect{};
            QWidget *warningWidget{};
            QPushButton *pbCreate{};
            QLineEdit *search{};
            QTextBrowser *warningText{};

            void initUI();
            void showWarningIfPkNotFound(const bool &enable, const int &errorType);

        protected:
            void selectTable(const Nodes::TablePtr &table);
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
