#include "mainwindow.h"
#include "qscrollarea.h"
#include "qmenubar.h"
#include "qsizepolicy.h"
#include "qdesktopwidget.h"
#include "iostream"
#include "workarea.h"
#include "qscrollarea.h"
#include "helper.h"
#include <QFileDialog>
#include <qfile.h>
#include <qtextstream.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set minimum size
    this->setMinimumSize(800, 600);
    this->showMaximized();
    this->setStyleSheet(Helper::getStyleFromFile("main"));
    // Insert menu bar into MainWindow
    this->setMenuBar(defineMenuBar());
}

void MainWindow::createWorkArea()
{
    // Create scroll area for work area
    QScrollArea* scrollArea = new QScrollArea(this);

    // Init new work area widget
    this->workArea = new WorkArea(scrollArea);
    scrollArea->setWidget(workArea);
    this->setCentralWidget(scrollArea);
}

QMenuBar* MainWindow::defineMenuBar()
{
    // Create MenuBar for MainWindow
    QMenuBar* menuBar = new QMenuBar(this);

    // Init menu for project
    QMenu* project = menuBar->addMenu("Project");

    // Create new project
    QAction* createProject = project->addAction("Create new Project");
    createProject->setIcon(QIcon(QString(":/imgs/new")));
    createProject->setShortcut(QKeySequence("Ctrl+A"));

    // Open project from file
    QAction* openProject = project->addAction("Open Project");
    openProject->setIcon(QIcon(QString(":/imgs/open")));
    openProject->setShortcut(QKeySequence("Ctrl+D"));

    // Save current project in file
    QAction* saveProject = project->addAction("Save Project");
    saveProject->setIcon(QIcon(QString(":/imgs/save")));
    saveProject->setShortcut(QKeySequence("Ctrl+S"));

    // Save current project in other file
    QAction* saveAsProject = project->addAction("Save as ...");
    saveAsProject->setIcon(QIcon(QString(":/imgs/save_as")));
    saveAsProject->setShortcut(QKeySequence("Ctrl+Shift+S"));

    project->addSeparator();

    // Define slots
    connect(createProject, &QAction::triggered, this, &MainWindow::createWorkArea);
    connect(openProject, &QAction::triggered, this, &MainWindow::openSaveFile);
    connect(saveProject, &QAction::triggered, this, [this] () {
        this->generateSaveFile(MainWindow::SAVE_TYPE_REWRITE_FILE);
    });
    connect(saveAsProject, &QAction::triggered, this, [this] () {
        this->generateSaveFile(MainWindow::SAVE_TYPE_NEW_FILE);
    });

    // return QMenuBar
    return menuBar;
}

void MainWindow::generateSaveFile(const int saveType)
{
    QVector<QPointer<Node>> nodes(this->workArea->getAllNodes());
    QVector<QPair<QString, QStringList>> relations(this->workArea->getAllrelations());

    if (this->filePath == "" || saveType == MainWindow::SAVE_TYPE_NEW_FILE) {
        this->filePath = QFileDialog::getSaveFileName(
                    nullptr,
                    tr("Save File"),
                    "/home/" + qgetenv("USER") + "/new_file.dbn",
                    tr("DbNodes File (*.dbn)"));
    }

    QFile file(this->filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

    QTextStream out(&file);

    out << "#DBNODESFILE#\n";
    out << "::Nodes" << "\n";
    QVectorIterator<QPointer<Node>> saveNodesIterator(nodes);
    while (saveNodesIterator.hasNext()) {
        QPointer<Node> saveNode(saveNodesIterator.next());
        QString row = saveNode->getTableId() + "," +
                saveNode->getTableName() + "," +
                QString::number(saveNode->x()) + "&" +
                QString::number(saveNode->y());
        out << "$" << row << "\n";
    }

    out << "::NodeRows\n";
    saveNodesIterator.toFront();
    while (saveNodesIterator.hasNext()) {
        QPointer<Node> saveNode(saveNodesIterator.next());
        QVector<QPointer<NodeRow>> saveNodeRows(saveNode->getAllNodeRows());
        out << "@" << saveNode->getTableId() << "\n";
        QVectorIterator<QPointer<NodeRow>> saveNodeRowsIterator(saveNodeRows);
        while (saveNodeRowsIterator.hasNext()) {
            QPointer<NodeRow> saveNodeRow(saveNodeRowsIterator.next());
            QString row = saveNodeRow->getRowId() + "," +
                    saveNodeRow->getRowName() + "," +
                    QString::number(saveNodeRow->getRowType()) + "," +
                    saveNodeRow->getRowDbType() + "," +
                    QString::number(saveNodeRow->getRowIsNull());
            out << "$" << row + "\n";
        }
    }

    out << "::Relations\n";
    QVectorIterator<QPair<QString, QStringList>> saveRalationsIterator(this->workArea->getAllrelations());
    while (saveRalationsIterator.hasNext()) {
        QPair<QString, QStringList> saveRelationPair(saveRalationsIterator.next());
        QString saveRelationId(saveRelationPair.first);
        QStringList saveRelationList(saveRelationPair.second);
        out << "@" << saveRelationId << "\n";
        QStringListIterator saveRelationListIterator(saveRelationList);
        while (saveRelationListIterator.hasNext()) {
            out << "$" << saveRelationListIterator.next() << "\n";
        }
    }
    out << "#ENDDBNODESFILE#\n";
}

void MainWindow::openSaveFile()
{
    this->filePath = QFileDialog::getOpenFileName(
                nullptr,
                tr("Open File"),
                "/home/" + qgetenv("USER") + "/file.dbn",
                tr("DbNodes File (*.dbn)"));

    QString fileString("");
    QFile file(this->filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        fileString += line;
    }

    QRegExp filePreg("#DBNODESFILE#\n"
                       "::Nodes\n(\\$\\S+\n){0,}"
                       "::NodeRows\n(@\\S+\n(\\$\\S+\n){0,}){0,}"
                       "::Relations\n(@\\S+\n(\\$\\S+\n){0,}){0,}"
                       "#ENDDBNODESFILE#");
    if (filePreg.indexIn(fileString) == -1)
        return;

    this->createWorkArea();

    QString fileData(filePreg.cap(0));
    QRegExp fileIndicatorPreg("#\\S+#");
    fileData = fileData.replace(fileIndicatorPreg, "");
    fileData = fileData.replace("\n", "");
    QStringList fileModulesData = fileData.split("::");

    QString nodeModule = fileModulesData.at(1);
    nodeModule = nodeModule.replace("Nodes", "");
    QStringList nodes = nodeModule.split("$");
    if (nodes.length() > 0) {
        nodes.removeAt(0);
        foreach (QString node, nodes) {
            QStringList nodeOptions = node.split(",");
            QString nodeId = nodeOptions.at(0);
            QString nodeName = nodeOptions.at(1);
            QStringList coords = nodeOptions.at(2).split("&");
            QPoint pos(coords.at(0).toInt(), coords.at(1).toInt());

            this->workArea->createNodeFromFile(nodeId, nodeName, pos);
        }
    }

    QString nodeRowModule = fileModulesData.at(2);
    nodeRowModule = nodeRowModule.replace("NodeRows", "");
    QStringList nodeRowsClusters = nodeRowModule.split("@");
    if (nodeRowsClusters.length() > 0) {
        nodeRowsClusters.removeAt(0);
        foreach (QString nodeRowCluster, nodeRowsClusters) {
            QStringList nodeRows = nodeRowCluster.split("$");
            if (nodeRows.length() > 0) {
                QString nodeId = nodeRows.takeAt(0);
                foreach (QString nodeRow, nodeRows) {
                    QStringList nodeRowOptions = nodeRow.split(",");
                    QString nodeRowId = nodeRowOptions.at(0);
                    QString nodeRowName = nodeRowOptions.at(1);
                    int nodeRowType = nodeRowOptions.at(2).toInt();
                    QString nodeRowDbType = nodeRowOptions.at(3);
                    bool nodeRowIsNull = nodeRowOptions.at(4).toInt();

                    QVector<QPointer<Node>> nodes = this->workArea->getAllNodes();
                    foreach (QPointer<Node> node, nodes) {
                        if (node->getTableId() == nodeId) {
                            node->addColumnFromFile(nodeRowId, nodeRowName,
                                                    nodeRowType, nodeRowDbType,
                                                    nodeRowIsNull);
                        }
                    }
                }
            }
        }
    }

    QString relationsModule = fileModulesData.at(3);
    relationsModule = relationsModule.replace("Relations", "");
    QStringList relations = relationsModule.split("@");
    if (relations.length() > 0) {
        relations.removeAt(0);
        foreach (QString relation, relations) {
            QStringList nodeRowsItems = relation.split("$");
            if (nodeRowsItems.length() > 2) {
                QString relationId = nodeRowsItems.takeAt(0);
                QString pkNodeRowId = nodeRowsItems.takeFirst();
                QString fkNodeRowId = nodeRowsItems.takeLast();

                QPointer<NodeRow> pkNodeRow = this
                        ->workArea
                        ->findNodeRow(WorkArea::GET_PK_NODE_ROWS, pkNodeRowId);
                QPointer<NodeRow> fkNodeRow = this
                        ->workArea
                        ->findNodeRow(WorkArea::GET_FK_NODE_ROWS, fkNodeRowId);

                this->workArea->makeRelation(relationId, pkNodeRow, fkNodeRow);
            }
        }
    }
}
