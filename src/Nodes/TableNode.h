#ifndef TABLENODE_H
#define TABLENODE_H

#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPointer>
#include <QLineEdit>
#include <QLabel>

#include "MultipleSelection/Selectable.h"
#include "Table/Column.h"
#include "AbstractNode.h"
#include "Relation.h"
#include "config.h"

namespace DbNodes::Nodes {

    using ColumnType = Nodes::Table::Column::Type;

    class TableNode : public Abstract::AbstractNode
    {
        Q_OBJECT

        public:
            TableNode(QWidget *parent, QString id, QString name);
            explicit TableNode(QWidget *parent = nullptr, const QString& name = "table");

            QString getTableName();
            QString getTableId();

            Table::ColumnPrtVector getAllColumns();

            void createColumn(
                const QString &id,
                const QString &name,
                const ColumnType &type,
                const QString &dbType,
                const bool &isNull = false
            );

            QVBoxLayout *getLayoutType(const ColumnType &columnType);

            void addRelation(const Relations::RelationPtr &relation);
            static QString generateId();

        private:
            QList<Relations::RelationPtr> relations;

            QVBoxLayout* columnsLayout{};
            QVBoxLayout* pkLayout{};
            QVBoxLayout* fkLayout{};

            QString tableName;
            QString tableId;

            QLabel* titleLabel{};

            void initUI();

        protected:
            void contextMenuEvent(QContextMenuEvent *event) override;
            void mousePressEvent(QMouseEvent *event) override;

            QList<Table::Column *> groupColumns();

            void setTableName(const QString &name);
            void openRenameModal();

        public slots:
            // Slots need fully-qualified types
            void addColumn(Nodes::Table::Column::Type columnType, Table::ColumnPrt column = nullptr);

    };

    typedef QPointer<Nodes::TableNode> TablePtr;

}

#endif // TABLENODE_H
