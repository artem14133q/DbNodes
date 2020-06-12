//Include events
#include "QContextMenuEvent"
#include "QContextMenuEvent"

//Include widgets
#include "qlineedit.h"
#include "QSpacerItem"

//Include laoyts
#include "QVBoxLayout"

//Include Qobjects
#include "qstyleoption.h"
#include "qpainter.h"
#include "qmenu.h"
#include "qaction.h"
#include "qsizepolicy.h"
#include "node.h"
#include "noderow.h"
#include "workarea.h"
#include "qpair.h"
#include "qpointer.h"
#include "helper.h"

Node::Node(QWidget *parent)
    : QWidget(parent)
{
    this->tableId = "node:" + Helper::getCurrentTimeMS();
    this->tableName = "table";
    this->initUI();
    this->show();
}

Node::Node(QWidget *parent, QString id, QString name)
    : QWidget(parent), tableName(name), tableId(id)
{
    this->initUI();
    this->show();
}

void Node::initUI()
{
    this->setStyleSheet(Helper::getStyleFromFile("node"));

    // Parent layout
    QVBoxLayout* vl = new QVBoxLayout(this);
    vl->setSizeConstraint(QVBoxLayout::SetFixedSize);

    // Name node
    QLineEdit* title = new QLineEdit(this->tableName, this);
    connect(title, &QLineEdit::textChanged, this, &Node::setTableName);

    title->setFixedWidth(150);
    title->setStyleSheet(Helper::getStyleFromFile("nodeTitle"));
    title->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    vl->addItem(new QSpacerItem(200, 15, QSizePolicy::Expanding, QSizePolicy::Fixed));
    vl->addWidget(title);
    vl->addItem(new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

    // Layout for PK rows
    this->pkLayout = new QVBoxLayout(vl->widget());

    // Layout for FK rows
    this->fkLayout = new QVBoxLayout(vl->widget());

    // Layout for default rows
    this->rowsLayout = new QVBoxLayout(vl->widget());
    this->rowsLayout->setSpacing(3);

    vl->addLayout(this->pkLayout);
    vl->addLayout(this->fkLayout);
    vl->addLayout(this->rowsLayout);

    vl->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    this->setLayout(vl);
}

// Set old pos before moving
void Node::mousePressEvent(QMouseEvent* event)
{
    this->oldPos = event->globalPos();
}

void Node::mouseMoveEvent(QMouseEvent *event)
{
    QPoint delta(event->globalPos() - this->oldPos);
    this->move(delta.x() + this->x(), delta.y() + this->y());

    if (this->x() < 0)
        this->move(0, this->y());

    if (this->y() < 0)
        this->move(this->x(), 0);

    if ((this->x() + this->width()) > this->parentWidget()->width())
        this->move(this->parentWidget()->width() - this->width(), this->y());

    if ((this->y() + this->height()) > this->parentWidget()->height())
        this->move(this->x(), this->parentWidget()->height() - this->height());

    this->oldPos = event->globalPos();
    this->parentWidget()->update();
}

void Node::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu* contextMenu = new QMenu();
    contextMenu->setStyleSheet(Helper::getStyleFromFile("nodeMenu"));

    // Default NodeRow
    QAction* addColomn = contextMenu->addAction("Add colomn");
    // PK NodeRow
    QAction* addPkColomn = contextMenu->addAction("Add PK colomn");
    // FK NodeRow
    QAction* addFkColomn = contextMenu->addAction("Add FK colomn");
    // Delete table (NodeRow)
    QAction* deleteTable = contextMenu->addAction("Delete Table");

    //Define Slots
    connect(deleteTable, &QAction::triggered, this, [this] {this->~Node();});
    connect(addColomn, &QAction::triggered, this, [this] {this->addColoumn();});
    connect(addPkColomn, &QAction::triggered, this, [this] {this->addColoumn(NodeRow::PK);});
    connect(addFkColomn, &QAction::triggered, this, [this] {this->addColoumn(NodeRow::FK);});

    //Set visible menu
    contextMenu->exec(this->mapToGlobal(event->pos()));
}

void Node::paintEvent(QPaintEvent *)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }

// Create row of types
void Node::addColoumn(const int nodeRowType, QPointer<NodeRow> row)
{
    if (!row)
        row = new NodeRow(this, nodeRowType);

    WorkArea* parentWorkArea = static_cast<WorkArea*>(this->parentWidget());

    if (nodeRowType == NodeRow::PK)
        this->pkLayout->addWidget(row);
    else if (nodeRowType == NodeRow::FK)
        this->fkLayout->addWidget(row);
    else
        this->rowsLayout->addWidget(row);

    parentWorkArea->setNodeRow(row);

    this->nodeRows.push_back(row);
    this->adjustSize();
}

void Node::addColumnFromFile(QString id, QString name, int type,
                             QString dbType, bool isNull)
{
    QPointer<NodeRow> nodeRow = new NodeRow(this, id, name, type, dbType, isNull);
    this->addColoumn(type, nodeRow);
}

void Node::cleanNodeRows()
{
    QVectorIterator<QPointer<NodeRow>> nodeRowsIterator(this->nodeRows);
    while (nodeRowsIterator.hasNext()) {
        QPointer<NodeRow> nodeRow(nodeRowsIterator.next());
        if (!nodeRow)
            this->nodeRows.removeOne(nodeRow);
    }
}

QVector<QPointer<NodeRow>> Node::getAllNodeRows()
{
    this->cleanNodeRows();
    return this->nodeRows;
}

// Named node slot
void Node::setTableName(const QString &newTableName)
{
    this->tableName = newTableName;
}

// Get table name
QString Node::getTableName()
{
    return this->tableName;
}

// Get table id
QString Node::getTableId()
{
    return this->tableId;
}
