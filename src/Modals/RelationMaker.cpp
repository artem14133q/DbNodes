#include "QLabel"
#include "QVector"
#include "QDebug"
#include "QRegExp"

#include "RelationMaker.h"
#include "RelationMakerErrors.h"
#include "Workarea.h"
#include "../helper.h"

namespace DbNodes::Modals {

    RelationMaker::RelationMaker(
            DbNodes::Widgets::NodeRow *fkNodeRaw,
            const QVector<NODE_POINTER> &nodeVector
    ) : Abstract::AbstractModal(fkNodeRaw), fkNodeRawParent(fkNodeRaw), nodeVector(nodeVector)
    {
        setFixedSize(300, 400);
        // Frameless window
        setWindowFlag(Qt::FramelessWindowHint);

        // Disabled MainWindow then relation maker open
        Helper::findParentWidgetRecursive(this, "MainWindow")->setDisabled(true);

        // Enabled this window
        setEnabled(true);

        setStyleSheet(Helper::getStyleFromFile("subWindow outline"));

        initUI();

        currentFkNodeRowId = fkNodeRaw->getRowId();
        filterNode();

        show();
    }

    void RelationMaker::initUI()
    {
        // Styles for ComboBoxes
        QString comboBoxStyle = Helper::getStyleFromFile("relationMakerSelector");

        // Styles for PushButtons
        QString pbStyle = Helper::getStyleFromFile("button");

        auto *searchLabel = new QLabel(this);
        searchLabel->setPixmap(QPixmap(Helper::getIconPath("search", false)));
        searchLabel->move(10, 20);

        search = new QLineEdit(this);
        search->setFixedWidth(240);
        search->move(30, 20);
        search->setStyleSheet(Helper::getStyleFromFile("lineEdit"));

        auto *pbDeleteFilter = new QPushButton("X", this);
        pbDeleteFilter->setFixedWidth(15);
        pbDeleteFilter->setStyleSheet(Helper::getStyleFromFile("nodeRowClose"));
        pbDeleteFilter->move(search->x() + search->width() + 5, 24);
        pbDeleteFilter->setFixedHeight(24);

        auto *tableName = new QLabel("Table:", this);
        tableName->setStyleSheet(Helper::getStyleFromFile("lineEditName"));
        tableName->move(10, 80);
        tableName->setFixedWidth(70);

        // Init for select PK NodeRow
        nodesSelect = new QComboBox(this);
        nodesSelect->setStyleSheet(comboBoxStyle);
        nodesSelect->move(80, 80);
        nodesSelect->setFixedWidth(210);

        auto *pkRowsName = new QLabel("Column:", this);
        pkRowsName->setStyleSheet(Helper::getStyleFromFile("lineEditName"));
        pkRowsName->move(10, 130);
        pkRowsName->setFixedWidth(70);

        nodeRowsOfNode = new QComboBox(this);
        nodeRowsOfNode->setStyleSheet(comboBoxStyle);
        nodeRowsOfNode->move(80, 130);
        nodeRowsOfNode->setFixedWidth(210);

        warningWidget = new QWidget(this);
        warningWidget->setFixedSize(300, 100);
        warningWidget->move(0, 180);
        warningWidget->setStyleSheet("background-color: transparent;");
        warningWidget->hide();

        auto *warningIcon = new QLabel(warningWidget);
        warningIcon->setPixmap(QPixmap(Helper::getIconPath("error", false)));
        warningIcon->move(10, (warningWidget->height() - warningIcon->height()) / 2);

        warningText = new QTextBrowser(warningWidget);
        warningText->setStyleSheet("QTextBrowser{color: #cb2a3c; font: bold; font-size: 18px;}");
        warningText->setFixedSize(warningWidget->width() - warningIcon->width() - 10, 60);
        warningText->move(warningIcon->width(),(warningWidget->height() - warningIcon->height()) / 2 + 10);

        // Exit PushButton
        auto *pbCancel = new QPushButton("Cancel", this);
        pbCancel->setStyleSheet(pbStyle);
        pbCancel->setFixedWidth(70);
        pbCancel->move(140, 360);

        // Confirm to create relation PushButton
        pbCreate = new QPushButton("Create", this);
        pbCreate->setStyleSheet(pbStyle);
        pbCreate->setFixedWidth(70);
        pbCreate->move(220, 360);

        connect(nodesSelect, SIGNAL(activated(int)), this, SLOT(selectNodeByIndex(const int &)));
        connect(nodeRowsOfNode, SIGNAL(activated(int)), this, SLOT(selectNodeRow(const int &)));
        connect(pbCreate, &QPushButton::clicked, this, &RelationMaker::confirm);
        connect(pbCancel, &QPushButton::clicked, this, &RelationMaker::exit);
        connect(search, &QLineEdit::textChanged, this, &RelationMaker::filterNode);
        connect(pbDeleteFilter, &QPushButton::clicked, this, &RelationMaker::deleteFilter);
    }

    void RelationMaker::exit()
    {
        nodeList.clear();
        nodeRowsOfSelectedNode.clear();

        // Enabled MainWindow
        Helper::findParentWidgetRecursive(this, "MainWindow")->setEnabled(true);

        Abstract::AbstractModal::exit();
    }

    void RelationMaker::selectNode(const NODE_POINTER &node)
    {
        nodeRowsOfSelectedNode.clear();
        nodeRowsOfNode->clear();

        foreach (NODE_RAW_POINTER nodeRow, node->getAllNodeRows().toList()) {
            if (nodeRow->getRowType() == Widgets::NodeRow::PK) {
                nodeRowsOfSelectedNode.insert(nodeRow->getRowId(), nodeRow);
                nodeRowsOfNode->addItem(nodeRow->getRowName(), nodeRow->getRowId());
            }
        }

        if (!nodeRowsOfSelectedNode.isEmpty()) {
            currentPkNodeRowId = nodeRowsOfSelectedNode.value(nodeRowsOfSelectedNode.keys().first())->getRowId();
            nodeRowsOfNode->setCurrentIndex(nodeRowsOfNode->findData(currentPkNodeRowId));
        }

        showWarningIfPkNotFound(nodeRowsOfSelectedNode.isEmpty(), CANNOT_FIND_PK_NODE_ROWS);
    }

    void RelationMaker::selectNodeByIndex(const int &index)
    {
        selectNode(nodeList.value(nodesSelect->itemData(index).toString()));
    }

    void RelationMaker::selectNodeRow(const int &index)
    {
        currentPkNodeRowId = nodeRowsOfSelectedNode.value(nodeRowsOfNode->itemData(index).toString())->getRowId();
    }

    void RelationMaker::confirm()
    {
        if (!pbCreate->isEnabled()) return;

        using namespace DbNodes::Widgets;

        auto *workArea = dynamic_cast<WorkArea*>(Helper::findParentWidgetRecursive(this, "WorkArea"));

        auto pkNodeRaw = workArea->findNodeRow(NodeRow::PK, currentPkNodeRowId);
        auto fkNodeRaw = workArea->findNodeRow(NodeRow::FK, currentFkNodeRowId);

        workArea->makeRelation("relation:" + Helper::getCurrentTimeMS(),pkNodeRaw,fkNodeRaw);

        Abstract::AbstractModal::confirm();
    }

    void RelationMaker::showWarningIfPkNotFound(const bool &enable, const int &errorType)
    {
        using namespace DbNodes::Dictionaries;

        if (enable) {
            warningWidget->show();
            warningText->setText(RelationMakerErrors::getValue(errorType).toString());
        } else {
            warningWidget->hide();
        }

        pbCreate->setDisabled(enable);
    }

    void RelationMaker::filterNode(const QString &filter)
    {
        QRegExp regFilter("\\w*" + filter + "\\w*");

        nodeList.clear();
        nodesSelect->clear();

        foreach (NODE_POINTER node, nodeVector.toList()) {
            if (node->getTableId() != fkNodeRawParent->getTableId() && regFilter.indexIn(node->getTableName()) != -1) {
                nodeList.insert(node->getTableId(), node);
                nodesSelect->addItem(node->getTableName(), node->getTableId());
            }
        }

        if (!nodeList.isEmpty()) {
            selectNode(nodeList.values().first());
        } else {
            showWarningIfPkNotFound(true, CANNOT_FIND_NODES);

            nodeRowsOfSelectedNode.clear();
            nodeRowsOfNode->clear();
        }
    }

    void RelationMaker::deleteFilter()
    {
        search->clear();
        filterNode();
    }
}

