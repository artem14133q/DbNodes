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
        selectable = new Utils::MultipleSelection::Selectable(this);

        setFocusPolicy(Qt::StrongFocus);
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
        setStyleSheet(Helper::getStyleFromFile("node"));

        // Parent layout
        auto *vl = new QVBoxLayout(this);
        vl->setSizeConstraint(QVBoxLayout::SetFixedSize);

        titleLabel = new QLabel(tableName, this);
        titleLabel->setStyleSheet(Helper::getStyleFromFile("nodeTitle"));
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
        contextMenu->setStyleSheet(Helper::getStyleFromFile("nodeMenu"));

        QAction* rename = contextMenu->addAction("Rename table");
        // Default NodeRow
        QAction* addColumn = contextMenu->addAction("Add column");
        // PK NodeRow
        QAction* addPkColumn = contextMenu->addAction("Add PK column");
        // FK NodeRow
        QAction* addFkColumn = contextMenu->addAction("Add FK column");
        // Delete table (NodeRow)
        QAction* deleteTable = contextMenu->addAction("Delete Table");


        //Define Slots
        connect(deleteTable, &QAction::triggered, this, [this] {this->~TableNode();});
        connect(addColumn, &QAction::triggered, this, [this] {this->addColumn();});
        connect(rename, &QAction::triggered, this, &TableNode::openRenameModal);
        connect(addPkColumn, &QAction::triggered, this, [this] {this->addColumn(Table::Column::PK);});
        connect(addFkColumn, &QAction::triggered, this, [this] {this->addColumn(Table::Column::FK);});


        #if APP_DEBUG

        QAction* debugAction = contextMenu->addAction("[DEBUG] - COLUMN DATA");

        connect(debugAction, &QAction::triggered, this, &TableNode::debugTables);

        #endif

        auto menuPos = mapToGlobal(event->pos());
        menuPos.setX(menuPos.x() + 5);
        //Set visible menu
        contextMenu->exec(menuPos);
    }

    // Create column of types
    void TableNode::addColumn(int columnType, COLUMN_POINTER column)
    {
        if (!column) column = new Table::Column(getLayoutType(columnType), this, columnType);

        auto *parentWorkArea = dynamic_cast<Widgets::WorkArea*>(parentWidget());

        if (columnType == Table::Column::PK)
            pkLayout->addWidget(column);
        else if (columnType == Table::Column::FK)
            fkLayout->addWidget(column);
        else
            columnsLayout->addWidget(column);

        parentWorkArea->setColumn(column);

        adjustSize();
    }

    void TableNode::addColumnFromFile(
        const QString &id,
        const QString &name,
        const int &type,
        const QString &dbType,
        const bool &isNull
    ) {
        QPointer<Table::Column> nodeRow = new Table::Column(getLayoutType(type), this, id, name, type, dbType, isNull);
        addColumn(type, nodeRow);
    }

    QVector<QPointer<Table::Column>> TableNode::getAllColumns()
    {
        COLUMN_VECTOR allNodeRows;

        foreach (Table::Column *w, groupColumns()) {
            allNodeRows.push_back(QPointer<Table::Column>(w));
        }

        return allNodeRows;
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

        QWidget *mainWindow = Helper::findParentWidgetRecursive(this, "MainWindow");

        tableRenameModal->move(
            mainWindow->x() + mainWindow->width() / 2 - tableRenameModal->width() / 2,
            mainWindow->y() + mainWindow->height() / 2 - tableRenameModal->height() / 2
        );

        connect(tableRenameModal, &TableRename::pushConfirm, this, [this] (const QString &name) {
                setTableName(name);
                titleLabel->setText(name);
        });
    }

    QVBoxLayout *TableNode::getLayoutType(const int &columnType)
    {
        if (columnType == Table::Column::PK)
            return pkLayout;
        else if (columnType == Table::Column::FK)
            return fkLayout;
        else
            return columnsLayout;
    }

    QList<Table::Column *> TableNode::groupColumns()
    {
        auto nodeRows = findChildren<Table::Column *>();

        QList<Table::Column *> sortedNodeRows;
        QList<int> layouts({Table::Column::PK, Table::Column::FK, 0});

        foreach (const int &lt, layouts) {
            QHash<int, Table::Column *> lN;

            foreach (Table::Column *w, nodeRows) {
                if (w->getColumnType() == lt) lN.insert(getLayoutType(lt)->indexOf(w), w);
            }

            for (int i = 0; !lN.empty(); ++i) {
                sortedNodeRows.push_back(lN.take(i));
            }
        }

        return sortedNodeRows;
    }

    void TableNode::mousePressEvent(QMouseEvent *event)
    {
        raise();
        selectable->setClicked(true);

        foreach (const RELATION_POINTER &relation, relations) {
            if (relation == nullptr) {
                relations.removeAll(relation);
                continue;
            };

            if (relation->getRelationTypeId() == RELATION_TYPE_LINK) {
                relation->raise();
            }
        }

        AbstractNode::mousePressEvent(event);
    }

    void TableNode::addRelation(const RELATION_POINTER &relation)
    {
        relations.push_back(relation);
    }

#if APP_DEBUG

    void TableNode::debugTables()
    {
        qDebug() << "===== DEBUG NODE ROWS DATA ======";

        foreach (Table::Column *w, groupColumns()) {
            int index = getLayoutType(w->getColumnType())->indexOf(w);

            qDebug() << index
                << (index < 10 ? " " : "")
                << "[ "
                << w->getColumnId()
                << " | "
                << debugLayoutType(w->getColumnType())
                << " | "
                << w->getColumnName()
                << " ]";
        }

        qDebug() << "===== DEBUG NODE ROWS DATA ======";
    }

    QString TableNode::debugLayoutType(const int &columnType)
    {
        if      (columnType == Table::Column::PK)      return "PK";
        else if (columnType == Table::Column::FK)      return "FK";
        else                                        return "RW";
    }

#endif

    void TableNode::mouseMoveEvent(QMouseEvent *event)
    {
        QPoint delta(event->globalPos() - oldPos);

        selectable->move(delta);

        Abstract::AbstractNode::mouseMoveEvent(event);
    }

    void TableNode::mouseReleaseEvent(QMouseEvent *event)
    {
        selectable->disable();

        QWidget::mouseReleaseEvent(event);
    }

    Utils::MultipleSelection::Selectable *TableNode::getSelectable()
    {
        return selectable;
    }

    TableNode::~TableNode()
    {
        delete selectable;
    }
}

