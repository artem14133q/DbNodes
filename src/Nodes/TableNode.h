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

namespace DbNodes::Nodes {

    class TableNode : public Abstract::AbstractNode
    {
        Q_OBJECT

        public:
            TableNode(QWidget *parent, QString id, QString name);
            explicit TableNode(QWidget *parent = nullptr);

            QString getTableName();
            QString getTableId();

            Table::ColumnPrtVector getAllColumns();

            void addColumnFromFile(
                const QString &id,
                const QString &name,
                const Nodes::Table::Column::Type &type,
                const QString &dbType,
                const bool &isNull = false
            );

            QVBoxLayout *getLayoutType(const Nodes::Table::Column::Type &columnType);

            void addRelation(const Relations::RelationPtr &relation);

        private:
            QList<Relations::RelationPtr> relations;

            QVBoxLayout* columnsLayout{};
            QVBoxLayout* pkLayout{};
            QVBoxLayout* fkLayout{};

            QString tableName;
            QString tableId;

            QLabel* titleLabel{};

            void initUI();

            #if APP_DEBUG

            static QString debugLayoutType(const Nodes::Table::Column::Type &columnType);
            void debugTables();

            #endif

        protected:
            void contextMenuEvent(QContextMenuEvent *event) override;
            void mousePressEvent(QMouseEvent *event) override;

            QList<Table::Column *> groupColumns();

            void setTableName(const QString &name);
            void openRenameModal();

        protected slots:
            void addColumn(Nodes::Table::Column::Type columnType, Table::ColumnPrt column = nullptr);

    };

    typedef QPointer<Nodes::TableNode> TablePtr;

}

#endif // TABLENODE_H
