#include "noderow.h"
#include "qstyleoption.h"
#include "qpainter.h"
#include "QHBoxLayout"
#include "qlineedit.h"
#include "qcombobox.h"
#include "QStringList"
#include "qcheckbox.h"
#include "qpushbutton.h"
#include "qpixmap.h"
#include "qlabel.h"
#include "node.h"
#include "qpair.h"
#include "qdebug.h"
#include "helper.h"
#include "workarea.h"

NodeRow::NodeRow(QWidget *parent, const int rowType)
    : QWidget(parent),
      rowType_(rowType)
{
    this->initUi();
    this->setStyleSheet(Helper::getStyleFromFile("nodeRow"));
    this->show();
}

// Get row name. Call in work area
QString NodeRow::getRowName() const
{
    return rowName;
}

// Private Slot
void NodeRow::setRowName(const QString &newRawName)
{
    rowName = newRawName;
}

// Define database types
QStringList NodeRow::initTypes()
{
    QStringList* typesList = new QStringList();

    // Numeric types
    typesList->push_back("integer");
    typesList->push_back("tinyint");
    typesList->push_back("bigint");

    /* If NodeRow type is PK return
    only numeric types */
    if (this->rowType_ == NodeRow::PK)
        return *typesList;

    // Numeric types
    typesList->push_back("float");
    typesList->push_back("double");

    // Char types
    typesList->push_back("char");
    typesList->push_back("varchar");
    typesList->push_back("text");
    typesList->push_back("longtext");

    // Time types
    typesList->push_back("time");
    typesList->push_back("date");
    typesList->push_back("datetime");
    typesList->push_back("timestamp");
    typesList->push_back("year");

    // Other
    typesList->push_back("binary");
    typesList->push_back("bool");
    typesList->push_back("json");

    return *typesList;
}

void NodeRow::initUi()
{
    // Parent layout
    QHBoxLayout* hl = new QHBoxLayout(this);
    hl->setSizeConstraint(QHBoxLayout::SetFixedSize);
    hl->setContentsMargins(0, 3, 0, 3);

    // Close PushButton
    QPushButton* pbClose = new QPushButton("X", this);
    connect(pbClose, &QPushButton::clicked, this, &NodeRow::deleteNodeRow);
    pbClose->setFixedWidth(15);
    pbClose->setStyleSheet(Helper::getStyleFromFile("nodeRowClose"));
    hl->addWidget(pbClose);

    // If row type is PK set key image
    if (this->rowType_ == NodeRow::PK) {
        QLabel* key = new QLabel(this);
        key->setStyleSheet(Helper::getStyleFromFile("nodeRowKeyIcon"));
        key->setPixmap(QPixmap(":/imgs/key"));
        hl->addWidget(key);
    }

    // Name of row
    QLineEdit* leName = new QLineEdit("coloumn", this);
    connect(leName, &QLineEdit::textChanged, this, &NodeRow::setRowName);

    // Width for FK
    if (this->rowType_ == NodeRow::FK)
        leName->setFixedWidth(219);
    // Width for PK
    else if (this->rowType_ == NodeRow::PK)
        leName->setFixedWidth(100);

    leName->setStyleSheet(Helper::getStyleFromFile("nodeRowTitle"));
    hl->addWidget(leName);

    // If FK, not init db types combo box
    if (this->rowType_ != NodeRow::FK) {
        QComboBox* cbTypes = new QComboBox(this);
        cbTypes->insertItems(0, this->initTypes());
        cbTypes->setStyleSheet(Helper::getStyleFromFile("nodeRowTypes"));

        cbTypes->setFixedWidth(90);
        hl->addWidget(cbTypes);
    }

    // If PK, not init NULL button
    if (this->rowType_ != NodeRow::PK) {
        QCheckBox* isNull = new QCheckBox("NULL", this);
        isNull->setFixedWidth(35);
        isNull->setStyleSheet(Helper::getStyleFromFile("nodeRowIsNull"));
        hl->addWidget(isNull);
    }

    this->setLayout(hl);
}

// Get parent node name
QString NodeRow::getTableName()
{
    Node* node = static_cast<Node*>(this->parentWidget());

    return node->getTableName();
}

void NodeRow::paintEvent(QPaintEvent *)
{
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// Get pos in work area
QPair<QPoint, int> NodeRow::getGlobalPos()
{
    Node* parentNode = static_cast<Node*>(this->parentWidget());

    return QPair<QPoint, int>(QPoint(parentNode->x(),
                                     parentNode->y() + this->y() + this->height()/2),
                                     parentNode->width());
}

int NodeRow::getRowType()
{
    return this->rowType_;
}

void NodeRow::deleteNodeRow()
{
    WorkArea* workArea = static_cast<WorkArea*>(parentWidget()->parentWidget());
    this->~NodeRow();
    // Update work area when relation delete
    workArea->update();
}
