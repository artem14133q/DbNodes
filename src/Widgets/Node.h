#ifndef NODE_H
#define NODE_H

#include "QVBoxLayout"
#include "QMouseEvent"
#include "Noderow.h"
#include "QPointer"
#include "QLabel"
#include "QLineEdit"
#include "config.h"

#include "AbstractNode.h"
#include "Relation.h"

#define NODE_POINTER QPointer<Widgets::Node>

namespace DbNodes::Widgets {

    class Node : public Abstract::AbstractNode
    {
        Q_OBJECT

        public:
            Node(QWidget *parent, QString id, QString name);
            explicit Node(QWidget *parent = nullptr);

            QString getTableName();
            QString getTableId();

            NODE_RAW_VECTOR getAllNodeRows();

            void addColumnFromFile(
                const QString &id,
                const QString &name,
                const int &type,
                const QString &dbType,
                const bool &isNull = false
            );

            QVBoxLayout *getLayoutType(const int &nodeRowType);

            void addRelation(const RELATION_POINTER &relation);

        private:
            QList<RELATION_POINTER> relations;

            QVBoxLayout* rowsLayout{};
            QVBoxLayout* pkLayout{};
            QVBoxLayout* fkLayout{};

            QString tableName;
            QString tableId;

            QLabel* titleLabel{};

            void initUI();

            #if APP_DEBUG

            static QString debugLayoutType(const int &rowType);
            void debugNodeRows();

            #endif

        protected:
            void contextMenuEvent(QContextMenuEvent *event) override;
            void mousePressEvent(QMouseEvent *event) override;
            QList<NodeRow *> groupNodeRows();

            void addColumn(int nodeRowType = 0, QPointer<NodeRow> nodeRow = nullptr);
            void setTableName(const QString &name);
            void openRenameModal();
    };

}

#endif // NODE_H
