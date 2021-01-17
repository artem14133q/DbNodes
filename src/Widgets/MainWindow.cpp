#include "MainWindow.h"
#include "qmenubar.h"
#include "qdesktopwidget.h"
#include "iostream"
#include "Workarea.h"
#include "../helper.h"
#include "QFileDialog"
#include "qfile.h"
#include "qtextstream.h"
#include "NameNewProject.h"
#include "QApplication"
#include "SettingsManager.h"
#include "QDebug"

namespace DbNodes::Widgets {

    MainWindow::MainWindow(QWidget *parent)
            : QMainWindow(parent)
    {
        // Set minimum size
        setMinimumSize(800, 600);
        setObjectName("MainWindow");
        setStyleSheet(Helper::getStyleFromFile("main"));
        setWindowIcon(QIcon(":/icons/128"));
        // Insert menu bar into MainWindow
        setMenuBar(defineMenuBar());

        startupWidget = new StartupWidget(this);

        scrollArea = new QScrollArea(this);
        scrollArea->hide();
        setCentralWidget(scrollArea);

        showMaximized();
    }

    void MainWindow::setTitle(const QString &name, const QString &path)
    {
        setWindowTitle("[" + path + "] @ " + name + " - DbNodes");
    }

    void MainWindow::createWorkArea(const QString &projectName)
    {
        // Init new work area widget
        workArea = new WorkArea(scrollArea, projectName);
        setTitle(projectName, "~");
        scrollArea->setWidget(workArea);
        scrollArea->show();

        startupWidget->hide();

        saveProject->setEnabled(true);
        saveAsProject->setEnabled(true);
        closeProject->setEnabled(true);
    }

    void MainWindow::closeCurrentProject(const int &closeProjectStatus)
    {
        if (closeProjectStatus == PROJECT_NOT_CLOSED) return;

        if (closeProjectStatus == PROJECT_CLOSE_AND_SAVE) {
            generateSaveFile(SAVE_TYPE_NEW_FILE);
        }

        scrollArea->hide();
        scrollArea->setWidget(nullptr);
        delete workArea;
        setWindowTitle("DbNodes");
        filePath = "";

        startupWidget->show();

        saveProject->setEnabled(false);
        saveAsProject->setEnabled(false);
        closeProject->setEnabled(false);
    }

    QMenuBar* MainWindow::defineMenuBar()
    {
        // Create MenuBar for MainWindow
        auto *menuBar = new QMenuBar(this);

        // Init menu for project
        auto *project = menuBar->addMenu("Project");

        // Create new project
        createProject = project->addAction("Create new Project");
//        createProject->setIcon(QIcon(":/imgs/new"));
        createProject->setShortcut(QKeySequence("Ctrl+A"));

        // Open project from file
        openProject = project->addAction("Open Project");
//        openProject->setIcon(QIcon(":/imgs/open"));
        openProject->setShortcut(QKeySequence("Ctrl+D"));

        // Close opened project
        closeProject = project->addAction("Close Project");
//        closeProject->setIcon(QIcon(":/imgs/close"));
        closeProject->setDisabled(true);
        closeProject->setShortcut(QKeySequence("Ctrl+W"));

        project->addSeparator();

        // Save current project in file
        saveProject = project->addAction("Save Project");
//        saveProject->setIcon(QIcon(":/imgs/save"));
        saveProject->setDisabled(true);
        saveProject->setShortcut(QKeySequence("Ctrl+S"));

        // Save current project in other file
        saveAsProject = project->addAction("Save as ...");
//        saveAsProject->setIcon(QIcon(":/imgs/save_as"));
        saveAsProject->setDisabled(true);
        saveAsProject->setShortcut(QKeySequence("Ctrl+Shift+S"));

        project->addSeparator();

        // Open project from file
        exit = project->addAction("Exit");
//        openProject->setIcon(QIcon(":/imgs/open"));
        exit->setShortcut(QKeySequence("Ctrl+Q"));

        // Define slots
        connect(closeProject, &QAction::triggered, this, [this] () {
            using namespace DbNodes::Modals;

            auto* window = openConfirmCloseProjectModal();
            connect(window, &ConfirmCloseProject::pushConfirm, this, &MainWindow::closeCurrentProject);
        });

        connect(createProject, &QAction::triggered, this, &MainWindow::createNewProject);
        connect(openProject, &QAction::triggered, this, &MainWindow::openSaveFile);

        connect(saveProject, &QAction::triggered, this, [this] () {
            generateSaveFile(SAVE_TYPE_REWRITE_FILE);
        });

        connect(saveAsProject, &QAction::triggered, this, [this] () {
            generateSaveFile(SAVE_TYPE_NEW_FILE);
        });

        connect(exit, &QAction::triggered, this, &MainWindow::close);

        // return QMenuBar
        return menuBar;
    }

    void MainWindow::createNewProject()
    {
        auto window = new DbNodes::Modals::NameNewProject(this);
        window->move(x() + width() / 2 - window->width() / 2, y() + height() / 2 - window->height() / 2);
    }

    void MainWindow::generateSaveFile(const int &saveType)
    {
        QVector<QPointer<Node>> nodes(workArea->getAllNodes());

        if (filePath == "" || saveType == SAVE_TYPE_NEW_FILE)
            filePath = QFileDialog::getSaveFileName(
                    nullptr,
                    tr("Save File"),
                    "/home/" + qgetenv("USER") + "/new_file.dbn",
                    tr("DbNodes File (*.dbn)"));

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);

        out << "#DBNODESFILE#\n";
        out << "Options\n";
        out << "$" << workArea->getProjectName() << "\n";
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
        QVectorIterator<QPair<QString, QStringList>> saveRelationsIterator(workArea->getAllRelations());
        while (saveRelationsIterator.hasNext()) {
            QPair<QString, QStringList> saveRelationPair(saveRelationsIterator.next());
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

        if (filePath == "")
            return;

        QString fileString("");
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            fileString += line;
        }

        QRegExp fileReg("#DBNODESFILE#\n"
                        "Options\n(\\$\\S+\n){0,}"
                        "::Nodes\n(\\$\\S+\n){0,}"
                        "::NodeRows\n(@\\S+\n(\\$\\S+\n){0,}){0,}"
                        "::Relations\n(@\\S+\n(\\$\\S+\n){0,}){0,}"
                        "#ENDDBNODESFILE#");

        if (fileReg.indexIn(fileString) == -1)
            return;

        QString fileData(fileReg.cap(0));
        QRegExp fileIndicatorReg("#\\S+#");
        fileData = fileData.replace(fileIndicatorReg, "");
        fileData = fileData.replace("\n", "");
        QStringList fileModulesData = fileData.split("::");

        QString optionsModule = fileModulesData.at(0);
        optionsModule = optionsModule.replace("Options", "");
        QStringList options = optionsModule.split("$");
        if (options.length() > 0) {
            options.removeAt(0);

            createWorkArea(options.at(0));
            setTitle(options.at(0), filePath);
        }

        QString nodeModule = fileModulesData.at(1);
        nodeModule = nodeModule.replace("Nodes", "");
        QStringList nodeList = nodeModule.split("$");
        if (nodeList.length() > 0) {
            nodeList.removeAt(0);

            foreach (QString node, nodeList) {
                QStringList nodeOptions = node.split(",");
                const QString& nodeId = nodeOptions.at(0);
                const QString& nodeName = nodeOptions.at(1);
                QStringList cords = nodeOptions.at(2).split("&");
                QPoint pos(cords.at(0).toInt(), cords.at(1).toInt());

                workArea->createNodeFromFile(nodeId, nodeName, pos);
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
                        const QString& nodeRowId = nodeRowOptions.at(0);
                        const QString& nodeRowName = nodeRowOptions.at(1);
                        int nodeRowType = nodeRowOptions.at(2).toInt();
                        const QString& nodeRowDbType = nodeRowOptions.at(3);
                        bool nodeRowIsNull = nodeRowOptions.at(4).toInt();

                        QVector<QPointer<Node>> nodes = workArea->getAllNodes();
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

                    QPointer<NodeRow> pkNodeRow = workArea
                            ->findNodeRow(WorkArea::GET_PK_NODE_ROWS, pkNodeRowId);
                    QPointer<NodeRow> fkNodeRow = workArea
                            ->findNodeRow(WorkArea::GET_FK_NODE_ROWS, fkNodeRowId);

                    workArea->makeRelation(relationId, pkNodeRow, fkNodeRow);
                }
            }
        }
    }

    void MainWindow::closeEvent(QCloseEvent *event)
    {
        if (workArea != nullptr) {
            event->ignore();
        } else {
            event->accept();
            return;
        }

        using namespace DbNodes::Modals;

        auto * window = openConfirmCloseProjectModal();
        window->activateWindow();
        connect(window, &ConfirmCloseProject::pushConfirm, this, [this] (const int &status) {
            MainWindow::closeCurrentProject(status);

            if (status != PROJECT_NOT_CLOSED) QApplication::exit();
        });
    }

    DbNodes::Modals::ConfirmCloseProject* MainWindow::openConfirmCloseProjectModal()
    {
        auto *window = new DbNodes::Modals::ConfirmCloseProject(this);
        window->move(
                x() + width() / 2 - window->width() / 2,
                y() + height() / 2 - window->height() / 2
        );

        return window;
    }

    void MainWindow::paintEvent(QPaintEvent * event)
    {
        if (workArea == nullptr) {
            startupWidget->move(width() / 2 - startupWidget->width() / 2,
                                height() / 2 - startupWidget->height() / 2);
        }

        QMainWindow::paintEvent(event);
    }

}
