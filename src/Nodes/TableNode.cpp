#include "QContextMenuEvent"
#include "QVBoxLayout"
#include "QMenu"
#include "QAction"
#include "QSizePolicy"
#include "QPair"
#include "QPointer"
#include "QDebug"
#include "QApplication"

#include "TableNode.h"
#include "TableRename.h"
#include "Table/Column.h"
#include "Workarea.h"
#include "RelationTypesDictionary.h"

#include <utility>
#include "../helper.h"

namespace DbNodes::Nodes {

    TableNode::TableNode(QWidget *parent, QString id, QString name)
        : DbNodes::Abstract::AbstractNode(parent), tableName(std::move(name)), tableId(std::move(id))
    {
        setObjectName("TableNode");
        initUI();
        show();
    }

    TableNode::TableNode(QWidget *parent)
        : TableNode(parent, "table:" + Helper::getCurrentTimeMS(), "table")
    {
        openRenameModal();
    }

    void TableNode::initUI()
    {
        setStyleSheet(Helper::getStyleFromFile("table"));

        // Parent layout
        auto *vl = new QVBoxLayout(this);
        vl->setSizeConstraint(QVBoxLayout::SetFixedSize);

        titleLabel = new QLabel(tableName, this);
        titleLabel->setStyleSheet(Helper::getStyleFromFile("tableTitle"));
        titleLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        titleLabel->setFixedWidth(300);

        vl->addSpacing(5);
        vl->addWidget(titleLabel);
        vl->addStretch(5);

        // Layout for PK rows
        pkLayout = new QVBoxLayout(vl->widget());

        // Layout for FK rows
        fkLayout = new QVBoxLayout(vl->widget());

        // Layout for default rows
        columnsLayout = new QVBoxLayout(vl->widget());
        columnsLayout->setSpacing(3);

        vl->addLayout(pkLayout);
        vl->addSpacing(5);
        vl->addLayout(fkLayout);
        vl->addSpacing(5);
        vl->addLayout(columnsLayout);

        vl->addStretch();

        setLayout(vl);
    }

    void TableNode::contextMenuEvent(QContextMenuEvent *event)
    {
        auto *contextMenu = new QMenu();
        contextMenu->setStyleSheet(Helper::getStyleFromFile("tableMenu"));

        QAction* rename = contextMenu->addAction("Rename table");
        // Default Column
        QAction* addColumn = contextMenu->addAction("Add column");
        // PK Column
        QAction* addPkColumn = contextMenu->addAction("Add PK column");
        // FK Column
        QAction* addFkColumn = contextMenu->addAction("Add FK column");
        // Delete table

        Abstract::AbstractNode::createDefaultActions(contextMenu);

        //Define Slots
        connect(rename, &QAction::triggered, this, &TableNode::openRenameModal);
        connect(addColumn, &QAction::triggered, this, [this] {
            this->addColumn(Table::Column::Type::Default);
        });

        connect(addPkColumn, &QAction::triggered, this, [this] {
            this->addColumn(Table::Column::Type::PrimaryKey);
        });

        connect(addFkColumn, &QAction::triggered, this, [this] {
            this->addColumn(Table::Column::Type::ForeignKey);
        });

        auto menuPos = mapToGlobal(event->pos());
        menuPos.setX(menuPos.x() + 5);
        //Set visible menu
        contextMenu->exec(menuPos);
    }

    // Create column of types
    void TableNode::addColumn(Nodes::Table::Column::Type columnType, Table::ColumnPrt column)
    {
        if (!column) column = new Table::Column(getLayoutType(columnType), this, columnType);

        auto *parentWorkArea = dynamic_cast<Widgets::WorkArea*>(parentWidget());

        if (columnType == Table::Column::Type::PrimaryKey)
            pkLayout->addWidget(column);
        else if (columnType == Table::Column::Type::ForeignKey)
            fkLayout->addWidget(column);
        else if (columnType == Table::Column::Type::Default)
            columnsLayout->addWidget(column);

        parentWorkArea->setColumn(column);

        adjustSize();
    }

    void TableNode::addColumnFromFile(
        const QString &id,
        const QString &name,
        const Nodes::Table::Column::Type &type,
        const QString &dbType,
        const bool &isNull
    ) {
        Table::ColumnPrt nodeRow = new Table::Column(getLayoutType(type), this, id, name, type, dbType, isNull);
        addColumn(type, nodeRow);
    }

    Table::ColumnPrtVector TableNode::getAllColumns()
    {
        Table::ColumnPrtVector allColumns;

        foreach (Nodes::Table::Column *column, groupColumns()) {
            allColumns.push_back(Nodes::Table::ColumnPrt(column));
        }

        return allColumns;
    }

    // Named node slot
    void TableNode::setTableName(const QString &newTableName)
    {
        tableName = newTableName;
    }

    // Get table name
    QString TableNode::getTableName()
    {
        return tableName;
    }

    // Get table id
    QString TableNode::getTableId()
    {
        return tableId;
    }

    void TableNode::openRenameModal()
    {
        using namespace DbNodes::Modals;

        auto* tableRenameModal = new TableRename(tableName, this);

        connect(tableRenameModal, &TableRename::pushConfirm, this, [this] (const QString &name) {
            setTableName(name);
            titleLabel->setText(name);
        });
    }

    QVBoxLayout *TableNode::getLayoutType(const Nodes::Table::Column::Type &columnType)
    {
        if (columnType == Table::Column::Type::PrimaryKey)
            return pkLayout;
        else if (columnType == Table::Column::Type::ForeignKey)
            return fkLayout;
        else
            return columnsLayout;
    }

    QList<Table::Column *> TableNode::groupColumns()
    {
        auto columns = findChildren<Table::Column *>();

        QList<Table::Column *> sortedColumns;

        QList<Nodes::Table::Column::Type> layouts({
            Table::Column::Type::PrimaryKey,
            Table::Column::Type::ForeignKey,
            Table::Column::Type::Default
        });

        foreach (const Nodes::Table::Column::Type &layout, layouts) {
            QHash<int, Table::Column *> group;

            foreach (Table::Column *column, columns) {
                if (column->getColumnType() == layout)
                    group.insert(getLayoutType(layout)->indexOf(column), column);
            }

            for (int i = 0; !group.empty(); ++i) {
                sortedColumns.push_back(group.take(i));
            }
        }

        return sortedColumns;
    }

    void TableNode::mousePressEvent(QMouseEvent *event)
    {
        raise();

        foreach (const Relations::RelationPtr &relation, relations) {
            if (relation == nullptr) {
                relations.removeAll(relation);
                continue;
            };

            if (relation->getRelationTypeId() == Dictionaries::RelationTypesDictionary::Type::Link) {
                relation->raise();
            }
        }

        AbstractNode::mousePressEvent(event);

        parentWidget()->update();
    }

    void TableNode::addRelation(const Relations::RelationPtr &relation)
    {
        relations.push_back(relation);
    }
}

