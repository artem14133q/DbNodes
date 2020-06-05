#include "relationmaker.h"
#include "qcombobox.h"
#include "qlabel.h"
#include "qvector.h"
#include "QVectorIterator"
#include "qpair.h"
#include "mainwindow.h"
#include "qpushbutton.h"
#include "helper.h"

RelationMaker::RelationMaker(QWidget *parent,
                             QVector<QPointer<NodeRow>> pkList,
                             QVector<QPointer<NodeRow>> fkList,
                             QVector<QPair<QString, QVector<QPointer<NodeRow>>>> relations)
    : QMainWindow(parent)
{
    this->setFixedSize(500, 200);
    // Freameless window
    this->setWindowFlag(Qt::FramelessWindowHint);

    // Desabled MainWindow then relation maker open
    this->getMainWindow()->setDisabled(true);
    // Enabled this window
    this->setEnabled(true);

    this->setStyleSheet(Helper::getStyleFromFile("relationMaker"));

    // Delete items in fkList that use
    QVectorIterator<QPointer<NodeRow>> fkListIterator(fkList);
    while (fkListIterator.hasNext()) {
        QPointer<NodeRow> node(fkListIterator.next());
        QVectorIterator<QPair<QString, QVector<QPointer<NodeRow>>>>
                relationIterator(relations);
        while(relationIterator.hasNext()) {
            QPair<QString, QVector<QPointer<NodeRow>>>
                    relationPair(relationIterator.next());
            if (relationPair.second.last() == node) {
                fkList.removeOne(node);
            }
        }
    }

    // Named vectors for QComboBoxes
    this->nameVectors(pkList, pkNames, pkNamedList);
    this->nameVectors(fkList, fkNames, fkNamedList);
    this->initUI();

    // Set default Vlaues in current PK and FK NodeRows
    if (!pkList.isEmpty() && !fkList.isEmpty())
        insertCurrentNodeRows(pkList.at(0), fkList.at(0));

    this->show();
}

void RelationMaker::insertCurrentNodeRows(QPointer<NodeRow> pkNodeRow, QPointer<NodeRow> fkNodeRow)
{

        this->currentPkNodeRow = QPair<QString, QPointer<NodeRow>>(
                                    pkNodeRow->getTableName() + "." + pkNodeRow->getTableName(),
                                    pkNodeRow
                                );

        this->currentFkNodeRow = QPair<QString, QPointer<NodeRow>>(
                                    fkNodeRow->getTableName() + "." + fkNodeRow->getTableName(),
                                    fkNodeRow
                                );
}

QWidget* RelationMaker::getMainWindow()
{
    return this->parentWidget()->parentWidget()->parentWidget()->parentWidget();
}

void RelationMaker::initUI()
{
    // Styles for ComboBoxes
    QString comboBoxStyle = Helper::getStyleFromFile("relationMakerSelector");

    // Styles for PushButtons
    QString pbStyle = Helper::getStyleFromFile("relationMakerButton");

    // Init for select PK NodeRow
    QComboBox* cbPkSelect = new QComboBox(this);
    connect(cbPkSelect, &QComboBox::textActivated, this, [this] (const QString &nodeRowName) {
        this->setCurrentNodeRow(nodeRowName, this->pkNamedList, this->currentPkNodeRow);
    });
    cbPkSelect->insertItems(0, this->pkNames);
    cbPkSelect->setStyleSheet(comboBoxStyle);
    cbPkSelect->move(10, 50);
    cbPkSelect->setFixedWidth(200);

    // Init for select PK NodeRow
    QComboBox* cbFkSelect = new QComboBox(this);
    connect(cbFkSelect, &QComboBox::textActivated, this, [this] (const QString &nodeRowName) {
        this->setCurrentNodeRow(nodeRowName, this->fkNamedList, this->currentFkNodeRow);
    });
    cbFkSelect->insertItems(0, this->fkNames);
    cbFkSelect->setStyleSheet(comboBoxStyle);
    cbFkSelect->move(290, 50);
    cbFkSelect->setFixedWidth(200);

    QLabel* arrow = new QLabel("<-", this);
    arrow->setStyleSheet(Helper::getStyleFromFile("relationMakerArrow"));
    arrow->setFixedWidth(30);
    arrow->move(230, 50);

    // Exit PushButton
    QPushButton* pbCancel = new QPushButton("Cancel", this);
    connect(pbCancel, &QPushButton::clicked, this, &RelationMaker::exit);
    pbCancel->setStyleSheet(pbStyle);
    pbCancel->setFixedWidth(70);
    pbCancel->move(340, 160);

    // Confirm to create relation PushButton
    QPushButton* pbCreate = new QPushButton("Create", this);
    connect(pbCreate, &QPushButton::clicked, this, &RelationMaker::addNodeRows);
    pbCreate->setStyleSheet(pbStyle);
    pbCreate->setFixedWidth(70);
    pbCreate->move(420, 160);
}

// Named NodeRows and set to special list for names
void RelationMaker::nameVectors(QVector<QPointer<NodeRow>> &list,
                                QStringList &names,
                                QVector<QPair<QString, QPointer<NodeRow>>> &namedList)
{
    QVectorIterator<QPointer<NodeRow>> listIterator(list);
    while (listIterator.hasNext()) {
        NodeRow* nodeRow = listIterator.next();
        QString name = nodeRow->getTableName() + "." + nodeRow->getRowName();
        names.push_back(name);
        namedList.push_back(QPair<QString, QPointer<NodeRow>> (name, nodeRow));
    }
}

// Set PK and FK NodeRow to relations list
void RelationMaker::addNodeRows()
{
    QPair<QString, QPointer<NodeRow>> currentPkNodeRow, currentFkNodeRow;
    currentPkNodeRow = this->currentPkNodeRow;
    currentFkNodeRow = this->currentFkNodeRow;

    if (this->currentFkNodeRow.second && this->currentPkNodeRow.second) {
        WorkArea* workArea = static_cast<WorkArea*>(this->parentWidget());

        workArea->makeRelation(currentPkNodeRow.second->getRowId() + "&" +
                               currentFkNodeRow.second->getRowId(),
                               currentPkNodeRow.second,
                               currentFkNodeRow.second);
    }

    this->exit();
}

// Find NodeRow in NodeRows list by name and set to currentNodeRows variables
void RelationMaker::setCurrentNodeRow(const QString &currentNodeRowName,
                                            QVector<QPair<QString, QPointer<NodeRow>>> &namedList,
                                            QPair<QString, QPointer<NodeRow>> &currentNodeRow)
{
    QVectorIterator<QPair<QString, QPointer<NodeRow>>> namedListIterator(namedList);
    while(namedListIterator.hasNext()) {
        QPair<QString, QPointer<NodeRow>> nodePair(namedListIterator.next());

        if (nodePair.first == currentNodeRowName)
            currentNodeRow = nodePair;
    }
}

void RelationMaker::exit()
{
    // Enabled MainWindow
    this->getMainWindow()->setEnabled(true);
    this->~RelationMaker();
}

