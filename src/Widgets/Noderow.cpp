#include "Noderow.h"

#include "QHBoxLayout"
#include "QLineEdit"
#include "QComboBox"
#include "QStringList"
#include "QCheckBox"
#include "QPixmap"
#include "QPair"
#include "QDebug"

#include "Node.h"
#include "Workarea.h"
#include "../helper.h"
#include "RelationMaker.h"

#include "DbTableTypesFkDictionary.h"
#include "DbTableTypesDictionary.h"

namespace DbNodes::Widgets {

    NodeRow::NodeRow(
            QVBoxLayout *vb,
            QWidget *parent,
            const int &rowType)
            : NodeRow(
                    vb,
                    parent,
                    "noderow:" + Helper::getCurrentTimeMS(),
                    "coloumn",
                    rowType,
                    rowType != NodeRow::FK ? "integer" : "none",
                    false) {}

    NodeRow::NodeRow(
            QVBoxLayout *vb,
            QWidget *parent,
            const QString &rowId,
            const QString &rowName,
            const int &rowType,
            const QString &rowDbType,
            const bool &rowIsNull)
            : DbNodes::Abstract::AbstractNode(parent),
                vb(vb),
                rowName(const_cast<QString &>(rowName)),
                rowId(const_cast<QString &>(rowId)),
                rowDbType(const_cast<QString &>(rowDbType)),
                rowIsNull(rowIsNull),
                rowType(rowType)
    {
        initUi();
        enableMoveRestrictions(false);
        show();
    }

// Get row name. Call in work area
    QString NodeRow::getRowName()
    {
        return rowName;
    }

// Private Slot
    void NodeRow::setRowName(const QString &newRawName)
    {
        rowName = newRawName;
    }

// Define database types
    QStringList NodeRow::initTypes() const
    {
        using namespace DbNodes::Dictionaries;

        QStringList typesList;

        foreach (const QVariant &t, DbTableTypesFkDictionary::getAllValues()) {
            typesList.push_back(t.toString());
        }

        /* If NodeRow type is PK return
        only numeric types */
        if (rowType == NodeRow::PK)
            return typesList;

        foreach (const QVariant &t, DbTableTypesDictionary::getAllValues()) {
            typesList.push_back(t.toString());
        }

        return typesList;
    }

    void NodeRow::initUi()
    {
        QString styleName;

        if (rowType == NodeRow::PK) styleName = "pkNodeRow";
        else if (rowType == NodeRow::FK) styleName = "fkNodeRow";
        else styleName = "nodeRow";

        setStyleSheet(Helper::getStyleFromFile(styleName));

        // Parent layout
        auto *hl = new QHBoxLayout(this);
        hl->setSizeConstraint(QHBoxLayout::SetFixedSize);
        hl->setContentsMargins(0, 3, 0, 3);

        // Close PushButton
        auto *pbClose = new QPushButton("X", this);
        connect(pbClose, &QPushButton::clicked, this, &NodeRow::deleteNodeRow);
        pbClose->setFixedWidth(15);
        pbClose->setStyleSheet(Helper::getStyleFromFile("nodeRowClose"));
        hl->addWidget(pbClose);

        // If row type is PK set key image
        if (rowType == NodeRow::PK) {
            auto *key = new QLabel(this);
            key->setStyleSheet(Helper::getStyleFromFile("nodeRowKeyIcon"));
            key->setPixmap(QPixmap(Helper::getIconPath("key", false)));
            hl->addWidget(key);
        }

        // Name of row
        auto *leName = new QLineEdit(rowName, this);
        leName->setCursorPosition(0);
        leName->setToolTip(rowName);
        hl->addWidget(leName);
        connect(leName, &QLineEdit::textChanged, this, &NodeRow::setRowName);

        QString leRowTitle = "nodeRowTitle";
        // Width for FK
        if (rowType == NodeRow::FK) {
            leName->setFixedWidth(208);
            leRowTitle = "fkNodeRowTitle";

            QIcon icon;

            icon.addPixmap(QPixmap(Helper::getIconPath("fk_chain", false)), QIcon::Normal);
            icon.addPixmap(QPixmap(Helper::getIconPath("fk_chain_disabled", false)), QIcon::Disabled);

            fkButton = new QPushButton(this);
            fkButton->setFixedHeight(24);
            fkButton->setIcon(icon);
            fkButton->setStyleSheet(Helper::getStyleFromFile("fkChainButton"));

            connect(fkButton, &QPushButton::clicked, this, &NodeRow::openRelationMaker);

            hl->addWidget(fkButton);
        }
        // Width for PK
        else if (rowType == NodeRow::PK) {
            leName->setFixedWidth(160);
            leRowTitle = "pkNodeRowTitle";
        }

        leName->setStyleSheet(Helper::getStyleFromFile(leRowTitle));

        // If FK, not init db types combo box
        if (rowType != NodeRow::FK) {
            auto *cbTypes = new QComboBox(this);
            QStringList dbTypes = initTypes();
            cbTypes->installEventFilter(this);
            cbTypes->addItems(dbTypes);
            cbTypes->setCurrentIndex(dbTypes.indexOf(rowDbType));
            cbTypes->setStyleSheet(Helper::getStyleFromFile("nodeRowTypes"));
            connect(cbTypes, &QComboBox::currentTextChanged, this, &NodeRow::setRowDbType);

            cbTypes->setFixedWidth(90);
            hl->addWidget(cbTypes);
        }

        // If PK, not init NULL button
        if (rowType != NodeRow::PK) {
            auto *isNull = new QCheckBox("NULL", this);
            isNull->setChecked(rowIsNull);
            isNull->setFixedWidth(35);

            QString nodeRowIsNullStyle = "nodeRowIsNull";
            if (rowType == NodeRow::FK) nodeRowIsNullStyle = "fkNodeRowIsNull";
            isNull->setStyleSheet(Helper::getStyleFromFile(nodeRowIsNullStyle));

            hl->addWidget(isNull);
            connect(isNull, &QCheckBox::clicked, this, &NodeRow::setRowIsNull);
        }

        moveHandle = new QLabel(this);
        moveHandle->setPixmap(QPixmap(Helper::getIconPath("menu", false)));
        moveHandle->setStyleSheet(Helper::getStyleFromFile("dropButton"));

        hl->addWidget(moveHandle);

        setLayout(hl);
    }

    bool NodeRow::eventFilter(QObject *object, QEvent *event) {
        if (event->type() == QEvent::Wheel && qobject_cast<QComboBox*>(object))
        {
            event->ignore();
            return true;
        }

        return QWidget::eventFilter(object, event);
    }

// Get parent node name
    QString NodeRow::getTableName()
    {
        Node* node = dynamic_cast<Node*>(parentWidget());

        return node->getTableName();
    }

    QString NodeRow::getTableId()
    {
        Node* node = dynamic_cast<Node*>(parentWidget());

        return node->getTableId();
    }

// Get pos in work area
    QPair<QPoint, int> NodeRow::getGlobalPos()
    {
        Node* parentNode = dynamic_cast<Node*>(parentWidget());

        return QPair<QPoint, int>(
                QPoint(parentNode->x(),parentNode->y() + y() + height()/2),
                parentNode->width()
            );
    }

    int NodeRow::getRowType() const
    {
        return rowType;
    }

    void NodeRow::deleteNodeRow()
    {
        auto* workArea = dynamic_cast<WorkArea*>(parentWidget()->parentWidget());
        this->~NodeRow();
        // Update work area when relation delete
        workArea->update();
    }

    void NodeRow::setRowDbType(const QString &type)
    {
        rowDbType = type;
    }

    QString NodeRow::getRowDbType()
    {
        return rowDbType;
    }

    bool NodeRow::getRowIsNull() const
    {
        return rowIsNull;
    }

    QString NodeRow::getRowId()
    {
        return rowId;
    }

    void NodeRow::setRowIsNull(bool checked)
    {
        rowIsNull = checked;
    }

    void NodeRow::mouseMoveEvent(QMouseEvent *event)
    {
        if (isMovable) {
            parentWidget()->parentWidget()->update();
            DbNodes::Abstract::AbstractNode::mouseMoveEvent(event);
        }
    }

    void NodeRow::mousePressEvent(QMouseEvent *event)
    {
        if (moveHandle->geometry().contains(mapFromGlobal(QCursor::pos()))) {
            setStyleSheet(styleSheet() + "QWidget{border: 1px solid #f36026;}");
            isMovable = true;
            raise();
        }

        DbNodes::Abstract::AbstractNode::mousePressEvent(event);
    }

    void NodeRow::mouseReleaseEvent(QMouseEvent *event)
    {
        if (isMovable) {
            auto *anotherNodeRow = getNodeRowUnderMouse();

            if (anotherNodeRow) vb->insertWidget(vb->indexOf(anotherNodeRow), this);
            else vb->insertWidget(vb->indexOf(this), this);

            isMovable = false;
            setStyleSheet(styleSheet().split("QWidget{border: 1px solid #f36026;}")[0]);
        }

        parentWidget()->parentWidget()->update();
        DbNodes::Abstract::AbstractNode::mouseReleaseEvent(event);
    }

    NodeRow *NodeRow::getNodeRowUnderMouse()
    {
        foreach(NodeRow *w, parentWidget()->findChildren<NodeRow *>())
        {
            if (w->geometry().contains(parentWidget()->mapFromGlobal(QCursor::pos()))
                && w->getRowType() == rowType
                && vb->indexOf(w) != vb->indexOf(this)) {
                return w;
            }
        }

        return nullptr;
    }

    void NodeRow::openRelationMaker()
    {
        using namespace DbNodes::Modals;

        auto* workArea = dynamic_cast<WorkArea*>(parentWidget()->parentWidget());

        auto *relationMaker = new RelationMaker(this, workArea->getAllNodes());

        auto globalPos = mapToGlobal(fkButton->pos());

        relationMaker->move(globalPos.x() + fkButton->width() + 2,globalPos.y() + fkButton->height() + 2);
    }

    void NodeRow::disableFkRelationButton(const bool &disable)
    {
        fkButton->setDisabled(disable);
    }
}
