#include "QLabel"
#include "QVector"
#include "QDebug"
#include "QRegExp"

#include "RelationMaker.h"
#include "RelationMakerErrorsDictionary.h"
#include "RelationTypesDictionary.h"
#include "Workarea.h"
#include "../helper.h"

namespace DbNodes::Modals {

    RelationMaker::RelationMaker(
            Nodes::Table::Column *fkColumn,
            const QList<Nodes::TablePtr> &tableVector
    ) : Abstract::AbstractModal(fkColumn), fkColumnParent(fkColumn), tableVector(tableVector)
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

        currentFkColumnId = fkColumn->getColumnId();
        filterTable();

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
        pbDeleteFilter->setStyleSheet(Helper::getStyleFromFile("columnClose"));
        pbDeleteFilter->move(search->x() + search->width() + 5, 24);
        pbDeleteFilter->setFixedHeight(24);

        auto *tableName = new QLabel("Table:", this);
        tableName->setStyleSheet(Helper::getStyleFromFile("lineEditName"));
        tableName->move(10, 80);
        tableName->setFixedWidth(70);

        // Init for select PK Column
        tablesSelect = new QComboBox(this);
        tablesSelect->setStyleSheet(comboBoxStyle);
        tablesSelect->move(80, 80);
        tablesSelect->setFixedWidth(210);

        auto *pkColumnsName = new QLabel("Column:", this);
        pkColumnsName->setStyleSheet(Helper::getStyleFromFile("lineEditName"));
        pkColumnsName->move(10, 130);
        pkColumnsName->setFixedWidth(70);

        columnsOfTable = new QComboBox(this);
        columnsOfTable->setStyleSheet(comboBoxStyle);
        columnsOfTable->move(80, 130);
        columnsOfTable->setFixedWidth(210);

        warningWidget = new QWidget(this);
        warningWidget->setFixedSize(300, 100);
        warningWidget->move(0, 180);
        warningWidget->setStyleSheet("background-color: transparent;");
        warningWidget->hide();

        auto *warningIcon = new QLabel(warningWidget);
        warningIcon->setPixmap(QPixmap(Helper::getIconPath("warning", false)));
        warningIcon->move(10, (warningWidget->height() - warningIcon->height()) / 2);

        warningText = new QTextBrowser(warningWidget);
        warningText->setStyleSheet("QTextBrowser{color: #ffbc33; font: bold; font-size: 18px;}");
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

        connect(tablesSelect, SIGNAL(activated(int)), this, SLOT(selectTableByIndex(const int &)));
        connect(columnsOfTable, SIGNAL(activated(int)), this, SLOT(selectColumn(const int &)));
        connect(pbCreate, &QPushButton::clicked, this, &RelationMaker::confirm);
        connect(pbCancel, &QPushButton::clicked, this, &RelationMaker::exit);
        connect(search, &QLineEdit::textChanged, this, &RelationMaker::filterTable);
        connect(pbDeleteFilter, &QPushButton::clicked, this, &RelationMaker::deleteFilter);
    }

    void RelationMaker::exit()
    {
        tableList.clear();
        columnsOfSelectedTable.clear();

        // Enabled MainWindow
        Helper::findParentWidgetRecursive(this, "MainWindow")->setEnabled(true);

        Abstract::AbstractModal::exit();
    }

    void RelationMaker::selectTable(const Nodes::TablePtr &table)
    {
        columnsOfSelectedTable.clear();
        columnsOfTable->clear();

        foreach (const Nodes::Table::ColumnPrt column, table->getAllColumns().toList()) {
            if (column->getColumnType() == Nodes::Table::Column::Type::PrimaryKey) {
                columnsOfSelectedTable.insert(column->getColumnId(), column);
                columnsOfTable->addItem(column->getColumnName(), column->getColumnId());
            }
        }

        if (!columnsOfSelectedTable.isEmpty()) {
            currentPkColumnId = columnsOfSelectedTable.value(columnsOfSelectedTable.keys().first())->getColumnId();
            columnsOfTable->setCurrentIndex(columnsOfTable->findData(currentPkColumnId));
        }

        showWarningIfPkNotFound(
            columnsOfSelectedTable.isEmpty(),
            Dictionaries::RelationMakerErrorsDictionary::Type::CannotFindPkColumns
        );
    }

    void RelationMaker::selectTableByIndex(const int &index)
    {
        selectTable(tableList.value(tablesSelect->itemData(index).toString()));
    }

    void RelationMaker::selectColumn(const int &index)
    {
        currentPkColumnId = columnsOfSelectedTable.value(columnsOfTable->itemData(index).toString())->getColumnId();
    }

    void RelationMaker::confirm()
    {
        if (!pbCreate->isEnabled()) return;

        using namespace DbNodes::Widgets;

        auto *workArea = dynamic_cast<WorkArea*>(Helper::findParentWidgetRecursive(this, "WorkArea"));

        auto pkColumn = workArea->findColumn(Nodes::Table::Column::Type::PrimaryKey, currentPkColumnId);
        auto fkColumn = workArea->findColumn(Nodes::Table::Column::Type::ForeignKey, currentFkColumnId);

        workArea->makeRelation(
            "relation:" + Helper::getCurrentTimeMS(),
            Dictionaries::RelationTypesDictionary::Type::Path,
            pkColumn,
            fkColumn
        );

        AbstractModal::confirm();
    }

    void RelationMaker::showWarningIfPkNotFound(const bool &enable, const int &errorType)
    {
        if (enable) {
            warningWidget->show();
            warningText->setText(Dictionaries::RelationMakerErrorsDictionary::getValue(errorType).toString());
        } else {
            warningWidget->hide();
        }

        pbCreate->setDisabled(enable);
    }

    void RelationMaker::filterTable(const QString &filter)
    {
        QRegExp regFilter("\\w*" + filter + "\\w*");

        tableList.clear();
        tablesSelect->clear();

        foreach (const Nodes::TablePtr table, tableVector) {
            if (table->getTableId() != fkColumnParent->getTableId() && regFilter.indexIn(table->getTableName()) != -1) {
                tableList.insert(table->getTableId(), table);
                tablesSelect->addItem(table->getTableName(), table->getTableId());
            }
        }

        if (!tableList.isEmpty()) {
            auto table = tableList.values().first();

            selectTable(table);
            tablesSelect->setCurrentIndex(tablesSelect->findData(table->getTableId()));
        } else {
            showWarningIfPkNotFound(
                true,
                Dictionaries::RelationMakerErrorsDictionary::Type::CannotFindTables
            );

            columnsOfSelectedTable.clear();
            columnsOfTable->clear();
        }
    }

    void RelationMaker::deleteFilter()
    {
        search->clear();
        filterTable();
    }
}

