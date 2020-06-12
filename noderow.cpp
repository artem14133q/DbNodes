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
#include "helper.h"
#include "workarea.h"

NodeRow::NodeRow(QWidget *parent, const int rowType)
    : QWidget(parent),
      rowType_(rowType)
{
    this->rowId_ = "noderow:" + Helper::getCurrentTimeMS();
    this->rowName_ = "coloumn";
    this->rowIsNull_ = false;

    if (this->rowType_ != NodeRow::FK)
        this->rowDbType_ = "integer";
    else
        this->rowDbType_ = "none";

    this->initUi();
    this->show();
}

NodeRow::NodeRow(QWidget *parent, QString rowId, QString rowName,
                     int rowType, QString rowDbType, bool rowIsNull)
    : QWidget(parent),
      rowName_(rowName),
      rowId_(rowId),
      rowDbType_(rowDbType),
      rowIsNull_(rowIsNull),
      rowType_(rowType)
{
    this->initUi();
    this->show();
}

// Get row name. Call in work area
QString NodeRow::getRowName() const
{
    return rowName_;
}

// Private Slot
void NodeRow::setRowName(const QString &newRawName)
{
    rowName_ = newRawName;
}

// Define database types
QStringList NodeRow::initTypes()
{
    QStringList typesList;

    // Numeric types
    typesList.push_back("integer");
    typesList.push_back("tinyint");
    typesList.push_back("bigint");

    /* If NodeRow type is PK return
    only numeric types */
    if (this->rowType_ == NodeRow::PK)
        return typesList;

    // Numeric types
    typesList.push_back("float");
    typesList.push_back("double");

    // Char types
    typesList.push_back("char");
    typesList.push_back("varchar");
    typesList.push_back("text");
    typesList.push_back("longtext");

    // Time types
    typesList.push_back("time");
    typesList.push_back("date");
    typesList.push_back("datetime");
    typesList.push_back("timestamp");
    typesList.push_back("year");

    // Other
    typesList.push_back("binary");
    typesList.push_back("bool");
    typesList.push_back("json");

    return typesList;
}

void NodeRow::initUi()
{
    this->setStyleSheet(Helper::getStyleFromFile("nodeRow"));

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
    QLineEdit* leName = new QLineEdit(this->rowName_, this);
    connect(leName, &QLineEdit::textChanged, this, &NodeRow::setRowName);

    // Width for FK
    if (this->rowType_ == NodeRow::FK)
        leName->setFixedWidth(202);
    // Width for PK
    else if (this->rowType_ == NodeRow::PK)
        leName->setFixedWidth(125);

    leName->setStyleSheet(Helper::getStyleFromFile("nodeRowTitle"));
    hl->addWidget(leName);

    // If FK, not init db types combo box
    if (this->rowType_ != NodeRow::FK) {
        QComboBox* cbTypes = new QComboBox(this);
        QStringList dbTypes = this->initTypes();
        cbTypes->addItems(dbTypes);
        cbTypes->setCurrentIndex(dbTypes.indexOf(this->rowDbType_));
        cbTypes->setStyleSheet(Helper::getStyleFromFile("nodeRowTypes"));
        connect(cbTypes, &QComboBox::textActivated, this, &NodeRow::setRowDbType);

        cbTypes->setFixedWidth(90);
        hl->addWidget(cbTypes);
    }

    // If PK, not init NULL button
    if (this->rowType_ != NodeRow::PK) {
        QCheckBox* isNull = new QCheckBox("NULL", this);
        isNull->setChecked(this->rowIsNull_);
        isNull->setFixedWidth(35);
        isNull->setStyleSheet(Helper::getStyleFromFile("nodeRowIsNull"));
        hl->addWidget(isNull);
        connect(isNull, &QCheckBox::clicked, this, &NodeRow::setRowIsNull);
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

void NodeRow::setRowDbType(const QString &type)
{
    this->rowDbType_ = type;
}

QString NodeRow::getRowDbType()
{
    return this->rowDbType_;
}

bool NodeRow::getRowIsNull()
{
    return this->rowIsNull_;
}

QString NodeRow::getRowId()
{
    return this->rowId_;
}

void NodeRow::setRowIsNull(bool checked)
{
    this->rowIsNull_ = checked;
}
