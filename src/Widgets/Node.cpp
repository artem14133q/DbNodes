#include "QContextMenuEvent"
#include "QVBoxLayout"
#include "QMenu"
#include "QAction"
#include "QSizePolicy"
#include "QPair"
#include "QPointer"
#include "QDebug"
#include "QApplication"

#include "Node.h"
#include "TableRename.h"
#include "Noderow.h"
#include "Workarea.h"
#include "RelationTypesDictionary.h"

#include <utility>
#include "../helper.h"

namespace DbNodes::Widgets {

    Node::Node(QWidget *parent, QString id, QString name)
        : DbNodes::Abstract::AbstractNode(parent), tableName(std::move(name)), tableId(std::move(id))
    {
        selectable = new Utils::MultipleSelection::Selectable(this);

        setFocusPolicy(Qt::StrongFocus);
        setObjectName("Node");
        initUI();
        show();
    }

    Node::Node(QWidget *parent): Node(parent, "node:" + Helper::getCurrentTimeMS(), "table")
    {
        openRenameModal();
    }

    void Node::initUI()
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
        rowsLayout = new QVBoxLayout(vl->widget());
        rowsLayout->setSpacing(3);

        vl->addLayout(pkLayout);
        vl->addSpacing(5);
        vl->addLayout(fkLayout);
        vl->addSpacing(5);
        vl->addLayout(rowsLayout);

        vl->addStretch();

        setLayout(vl);
    }

    void Node::contextMenuEvent(QContextMenuEvent *event)
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
        connect(deleteTable, &QAction::triggered, this, [this] {this->~Node();});
        connect(addColumn, &QAction::triggered, this, [this] {this->addColumn();});
        connect(rename, &QAction::triggered, this, &Node::openRenameModal);
        connect(addPkColumn, &QAction::triggered, this, [this] {this->addColumn(NodeRow::PK);});
        connect(addFkColumn, &QAction::triggered, this, [this] {this->addColumn(NodeRow::FK);});


        #if APP_DEBUG

        QAction* debugAction = contextMenu->addAction("[DEBUG] - NODE ROW DATA");

        connect(debugAction, &QAction::triggered, this, &Node::debugNodeRows);

        #endif

        auto menuPos = mapToGlobal(event->pos());
        menuPos.setX(menuPos.x() + 5);
        //Set visible menu
        contextMenu->exec(menuPos);
    }

    // Create row of types
    void Node::addColumn(int nodeRowType, NODE_RAW_POINTER row)
    {
        if (!row) row = new NodeRow(getLayoutType(nodeRowType), this, nodeRowType);

        auto *parentWorkArea = dynamic_cast<WorkArea*>(parentWidget());

        if (nodeRowType == NodeRow::PK)
            pkLayout->addWidget(row);
        else if (nodeRowType == NodeRow::FK)
            fkLayout->addWidget(row);
        else
            rowsLayout->addWidget(row);

        parentWorkArea->setNodeRaw(row);

        adjustSize();
    }

    void Node::addColumnFromFile(
        const QString &id,
        const QString &name,
        const int &type,
        const QString &dbType,
        const bool &isNull
    ) {
        QPointer<NodeRow> nodeRow = new NodeRow(getLayoutType(type), this, id, name, type, dbType, isNull);
        addColumn(type, nodeRow);
    }

    QVector<QPointer<NodeRow>> Node::getAllNodeRows()
    {
        NODE_RAW_VECTOR allNodeRows;

        foreach (NodeRow *w, groupNodeRows()) {
            allNodeRows.push_back(QPointer<NodeRow>(w));
        }

        return allNodeRows;
    }

    // Named node slot
    void Node::setTableName(const QString &newTableName)
    {
        tableName = newTableName;
    }

    // Get table name
    QString Node::getTableName()
    {
        return tableName;
    }

    // Get table id
    QString Node::getTableId()
    {
        return tableId;
    }

    void Node::openRenameModal()
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

    QVBoxLayout *Node::getLayoutType(const int &nodeRowType)
    {
        if (nodeRowType == NodeRow::PK)
            return pkLayout;
        else if (nodeRowType == NodeRow::FK)
            return fkLayout;
        else
            return rowsLayout;
    }

    QList<NodeRow *> Node::groupNodeRows()
    {
        auto nodeRows = findChildren<NodeRow *>();

        QList<NodeRow *> sortedNodeRows;
        QList<int> layouts({NodeRow::PK, NodeRow::FK, 0});

        foreach (const int &lt, layouts) {
            QHash<int, NodeRow *> lN;

            foreach (NodeRow *w, nodeRows) {
                if (w->getRowType() == lt) lN.insert(getLayoutType(lt)->indexOf(w), w);
            }

            for (int i = 0; !lN.empty(); ++i) {
                sortedNodeRows.push_back(lN.take(i));
            }
        }

        return sortedNodeRows;
    }

    void Node::mousePressEvent(QMouseEvent *event)
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

    void Node::addRelation(const RELATION_POINTER &relation)
    {
        relations.push_back(relation);
    }

#if APP_DEBUG

    void Node::debugNodeRows()
    {
        qDebug() << "===== DEBUG NODE ROWS DATA ======";

        foreach (NodeRow *w, groupNodeRows()) {
            int index = getLayoutType(w->getRowType())->indexOf(w);

            qDebug() << index
                << (index < 10 ? " " : "")
                << "[ "
                << w->getRowId()
                << " | "
                << debugLayoutType(w->getRowType())
                << " | "
                << w->getRowName()
                << " ]";
        }

        qDebug() << "===== DEBUG NODE ROWS DATA ======";
    }

    QString Node::debugLayoutType(const int &rowType)
    {
        if      (rowType == NodeRow::PK)    return "PK";
        else if (rowType == NodeRow::FK)    return "FK";
        else                                return "RW";
    }

#endif

    void Node::mouseMoveEvent(QMouseEvent *event)
    {
        QPoint delta(event->globalPos() - oldPos);

        selectable->move(delta);

        Abstract::AbstractNode::mouseMoveEvent(event);
    }

    void Node::mouseReleaseEvent(QMouseEvent *event)
    {
        selectable->disable();

        QWidget::mouseReleaseEvent(event);
    }

    Utils::MultipleSelection::Selectable *Node::getSelectable()
    {
        return selectable;
    }

    Node::~Node()
    {
        delete selectable;
    }
}

