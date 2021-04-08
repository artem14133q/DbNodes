#ifndef TABLENODE_H
#define TABLENODE_H

#include "QVBoxLayout"
#include "QMouseEvent"
#include "Table/Column.h"
#include "QPointer"
#include "QLabel"
#include "QLineEdit"
#include "config.h"

#include "AbstractNode.h"
#include "Relation.h"
#include "MultipleSelection/Selectable.h"

#define TABLE_POINTER QPointer<Nodes::TableNode>

namespace DbNodes::Nodes {

    class TableNode : public Abstract::AbstractNode
    {
        Q_OBJECT

        public:
            TableNode(QWidget *parent, QString id, QString name);
            explicit TableNode(QWidget *parent = nullptr);

            QString getTableName();
            QString getTableId();

            COLUMN_VECTOR getAllColumns();

            void addColumnFromFile(
                const QString &id,
                const QString &name,
                const int &type,
                const QString &dbType,
                const bool &isNull = false
            );

            QVBoxLayout *getLayoutType(const int &columnType);

            void addRelation(const RELATION_POINTER &relation);

            Utils::MultipleSelection::Selectable *getSelectable();

        private:
            Utils::MultipleSelection::Selectable *selectable;

            QList<RELATION_POINTER> relations;

            QVBoxLayout* columnsLayout{};
            QVBoxLayout* pkLayout{};
            QVBoxLayout* fkLayout{};

            QString tableName;
            QString tableId;

            QLabel* titleLabel{};

            void initUI();

            ~TableNode() override;

            #if APP_DEBUG

            static QString debugLayoutType(const int &columnType);
            void debugTables();

            #endif

        protected:
            void contextMenuEvent(QContextMenuEvent *event) override;
            void mousePressEvent(QMouseEvent *event) override;
            void mouseMoveEvent(QMouseEvent *event) override;
            void mouseReleaseEvent(QMouseEvent *event) override;

            QList<Table::Column *> groupColumns();

            void addColumn(int columnType = 0, QPointer<Table::Column> column = nullptr);
            void setTableName(const QString &name);
            void openRenameModal();
    };

}

#endif // TABLENODE_H
