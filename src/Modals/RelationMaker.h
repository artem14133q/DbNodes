#ifndef RELATIONMAKER_H
#define RELATIONMAKER_H

#include "QMainWindow"
#include "QVector"
#include "QPointer"
#include "QHash"
#include "QComboBox"
#include "QPushButton"
#include "QLineEdit"
#include "QTextBrowser"

#include "Node.h"
#include "Noderow.h"

#define NODE_POINTER QPointer<DbNodes::Widgets::Node>
#define NODE_RAW_POINTER QPointer<DbNodes::Widgets::NodeRow>

namespace DbNodes::Modals {

    class RelationMaker : public QMainWindow
    {
        Q_OBJECT

        public:
            explicit RelationMaker(
                    DbNodes::Widgets::NodeRow *fkNodeRaw,
                    const QVector<NODE_POINTER> &nodeVector
                );

        private:
            DbNodes::Widgets::NodeRow *fkNodeRawParent;
            QString currentPkNodeRowId, currentFkNodeRowId;

            QVector<NODE_POINTER> nodeVector;

            QHash<QString, NODE_POINTER> nodeList;
            QHash<QString, NODE_RAW_POINTER> nodeRowsOfSelectedNode;

            QComboBox *nodeRowsOfNode{}, *nodesSelect{};
            QWidget *warningWidget{};
            QPushButton *pbCreate{};
            QLineEdit *search{};
            QTextBrowser *warningText{};

            void exit();
            void initUI();
            void showWarningIfPkNotFound(const bool &enable, const int &errorType);

        protected:
            void selectNode(const NODE_POINTER &node);
            void filterNode(const QString &filter = "");
            void deleteFilter();
            void makeRelation();

        protected slots:
            void selectNodeByIndex(const int &index);
            void selectNodeRow(const int &index);
    };

}

#endif // RELATIONMAKER_H
