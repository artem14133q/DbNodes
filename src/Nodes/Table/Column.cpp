#include "Column.h"

#include "QHBoxLayout"
#include "QLineEdit"
#include "QComboBox"
#include "QStringList"
#include "QCheckBox"
#include "QPixmap"
#include "QListView"
#include "QPair"
#include "QDebug"

#include "TableNode.h"
#include "Workarea.h"
#include "../../helper.h"
#include "RelationMaker.h"

#include "DbTableTypesFkDictionary.h"
#include "DbTableTypesDictionary.h"

namespace DbNodes::Nodes::Table {

    Column::Column(QVBoxLayout *vb, QWidget *parent, const Column::Type &columnType)
        : Column(
            vb,
            parent,
            "column:" + Helper::getCurrentTimeMS(),
            "coloumn",
            columnType,
            columnType != ForeignKey ? "integer" : "none",
            false
        ) {}

    Column::Column(
        QVBoxLayout *vb,
        QWidget *parent,
        const QString &columnId,
        const QString &columnName,
        const Column::Type &columnType,
        const QString &columnDbType,
        const bool &columnIsNull
    ): DbNodes::Abstract::AbstractNode(parent),
        vb(vb),
        columnName(const_cast<QString &>(columnName)),
        columnId(const_cast<QString &>(columnId)),
        columnDbType(const_cast<QString &>(columnDbType)),
        columnIsNull(columnIsNull),
        columnType(columnType)
    {
        initUi();
        enableMoveRestrictions(false);
        show();
    }

// Get column name. Call in work area
    QString Column::getColumnName()
    {
        return columnName;
    }

// Private Slot
    void Column::setColumnName(const QString &newColumnName)
    {
        columnName = newColumnName;
    }

// Define database types
    QStringList Column::initTypes() const
    {
        using namespace DbNodes::Dictionaries;

        QStringList typesList;

        foreach (const QVariant &t, DbTableTypesFkDictionary::getAllValues()) {
            typesList.push_back(t.toString());
        }

        /* If Column type is PK return
        only numeric types */
        if (columnType == PrimaryKey)
            return typesList;

        foreach (const QVariant &t, DbTableTypesDictionary::getAllValues()) {
            typesList.push_back(t.toString());
        }

        return typesList;
    }

    void Column::initUi()
    {
        QString styleName;

        if (columnType == PrimaryKey)
            styleName = "pkColumn";
        else if (columnType == ForeignKey)
            styleName = "fkColumn";
        else
            styleName = "column";

        setStyleSheet(Helper::getStyleFromFile(styleName));

        // Parent layout
        auto *hl = new QHBoxLayout(this);
        hl->setSizeConstraint(QHBoxLayout::SetFixedSize);
        hl->setContentsMargins(0, 3, 0, 3);

        // Close PushButton
        auto *pbClose = new QPushButton("X", this);
        connect(pbClose, &QPushButton::clicked, this, &Column::deleteColumn);
        pbClose->setFixedWidth(15);
        pbClose->setStyleSheet(Helper::getStyleFromFile("columnClose"));
        hl->addWidget(pbClose);

        // If row type is PK set key image
        if (columnType == PrimaryKey) {
            auto *key = new QLabel(this);
            key->setStyleSheet(Helper::getStyleFromFile("columnKeyIcon"));
            key->setPixmap(QPixmap(Helper::getIconPath("key")));
            hl->addWidget(key);
        }

        // Name of row
        auto leName = new QLineEdit(columnName, this);
        leName->setCursorPosition(0);
        leName->setToolTip(columnName);
        hl->addWidget(leName);
        connect(leName, &QLineEdit::textChanged, this, &Column::setColumnName);

        QString leRowTitle = "columnTitle";
        // Width for FK
        if (columnType == ForeignKey) {
            leName->setFixedWidth(208);
            leRowTitle = "fkColumnTitle";

            QIcon icon;

            icon.addPixmap(QPixmap(Helper::getIconPath("fk_chain")), QIcon::Normal);
            icon.addPixmap(QPixmap(Helper::getIconPath("fk_chain_disabled")), QIcon::Disabled);

            fkButton = new QPushButton(this);
            fkButton->setFixedHeight(24);
            fkButton->setIcon(icon);
            fkButton->setStyleSheet(Helper::getStyleFromFile("fkChainButton"));

            connect(fkButton, &QPushButton::clicked, this, &Column::openRelationMaker);

            hl->addWidget(fkButton);
        }
        // Width for PK
        else if (columnType == PrimaryKey) {
            leName->setFixedWidth(163);
            leRowTitle = "pkColumnTitle";
        }

        leName->setStyleSheet(Helper::getStyleFromFile(leRowTitle));

        // If FK, not init db types combo box
        if (columnType != ForeignKey) {
            auto cbTypes = new QComboBox(this);
            cbTypes->setView(new QListView());
            QStringList dbTypes = initTypes();
            cbTypes->installEventFilter(this);
            cbTypes->addItems(dbTypes);
            cbTypes->setCurrentIndex(dbTypes.indexOf(columnDbType));
            cbTypes->setStyleSheet(Helper::getStyleFromFile("columnTypes"));
            connect(cbTypes, &QComboBox::currentTextChanged, this, &Column::setColumnDbType);

            cbTypes->setFixedWidth(90);
            hl->addWidget(cbTypes);
        }

        // If PK, not init NULL button
        if (columnType != PrimaryKey) {
            auto isNull = new QCheckBox("NULL", this);
            isNull->setChecked(columnIsNull);
            isNull->setFixedWidth(40);

            QString tableRowIsNullStyle = "columnIsNull";
            if (columnType == ForeignKey) tableRowIsNullStyle = "fkColumnIsNull";
            isNull->setStyleSheet(Helper::getStyleFromFile(tableRowIsNullStyle));

            hl->addWidget(isNull);
            connect(isNull, &QCheckBox::clicked, this, &Column::setColumnIsNull);
        }

        moveHandle = new QLabel(this);
        moveHandle->setPixmap(QPixmap(Helper::getIconPath("dots")));
        moveHandle->setStyleSheet(Helper::getStyleFromFile("dropButton"));

        hl->addWidget(moveHandle);

        setLayout(hl);
    }

    bool Column::eventFilter(QObject *object, QEvent *event) {
        if (event->type() == QEvent::Wheel && qobject_cast<QComboBox*>(object))
        {
            event->ignore();
            return true;
        }

        return QWidget::eventFilter(object, event);
    }

    // Get parent table name
    QString Column::getTableName()
    {
        auto table = dynamic_cast<TableNode *>(parentWidget());

        return table->getTableName();
    }

    QString Column::getTableId()
    {
        auto table = dynamic_cast<TableNode *>(parentWidget());

        return table->getTableId();
    }

    Column::Type Column::getColumnType() const
    {
        return columnType;
    }

    void Column::deleteColumn()
    {
        deleteLater();
    }

    void Column::setColumnDbType(const QString &type)
    {
        columnDbType = type;
    }

    QString Column::getColumnDbType()
    {
        return columnDbType;
    }

    bool Column::getColumnIsNull() const
    {
        return columnIsNull;
    }

    QString Column::getColumnId()
    {
        return columnId;
    }

    void Column::setColumnIsNull(bool checked)
    {
        columnIsNull = checked;
    }

    void Column::mouseMoveEvent(QMouseEvent *event)
    {
        if (isMovable) {
            parentWidget()->parentWidget()->update();
            DbNodes::Abstract::AbstractNode::mouseMoveEvent(event);
        }
    }

    void Column::mousePressEvent(QMouseEvent *event)
    {
        if (moveHandle->geometry().contains(mapFromGlobal(QCursor::pos()))) {
            setStyleSheet(styleSheet() + "QWidget{border: 1px solid #f36026;}");
            isMovable = true;
            raise();
        }

        DbNodes::Abstract::AbstractNode::mousePressEvent(event);
    }

    void Column::mouseReleaseEvent(QMouseEvent *event)
    {
        if (isMovable) {
            auto *anotherColumn = getColumnUnderMouse();

            if (anotherColumn) vb->insertWidget(vb->indexOf(anotherColumn), this);
            else vb->insertWidget(vb->indexOf(this), this);

            isMovable = false;
            setStyleSheet(styleSheet().split("QWidget{border: 1px solid #f36026;}")[0]);
        }

        parentWidget()->parentWidget()->update();
        DbNodes::Abstract::AbstractNode::mouseReleaseEvent(event);
    }

    Column *Column::getColumnUnderMouse()
    {
        foreach(Column *w, parentWidget()->findChildren<Column *>())
        {
            if (w->geometry().contains(parentWidget()->mapFromGlobal(QCursor::pos()))
                && w->getColumnType() == columnType
                && vb->indexOf(w) != vb->indexOf(this)) {
                return w;
            }
        }

        return nullptr;
    }

    void Column::openRelationMaker()
    {
        using namespace DbNodes::Modals;

        auto *workArea = dynamic_cast<Widgets::WorkArea*>(parentWidget()->parentWidget());

        auto *relationMaker = new RelationMaker(this, workArea->getAllTables());

        auto globalPos = mapToGlobal(fkButton->pos());

        relationMaker->move(globalPos.x() + fkButton->width() + 2,globalPos.y() + fkButton->height() + 2);
    }

    void Column::disableFkRelationButton(const bool &disable)
    {
        fkButton->setDisabled(disable);
    }

    Abstract::ParamsForDrawing Column::getDrawParams()
    {
        return Abstract::ParamsForDrawing(
            {parentWidget()->x(), parentWidget()->y() + y() + height() / 2},
            parentWidget()->width()
        );
    }
}
