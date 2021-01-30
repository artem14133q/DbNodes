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
#include "AbstractModal.h"

namespace DbNodes::Modals {

    class RelationMaker : public Abstract::AbstractModal
    {
        Q_OBJECT

        public:
            explicit RelationMaker(Widgets::NodeRow *fkNodeRaw, const QVector<NODE_POINTER> &nodeVector);

        private:
            Widgets::NodeRow *fkNodeRawParent;
            QString currentPkNodeRowId, currentFkNodeRowId;

            QVector<NODE_POINTER> nodeVector;

            QHash<QString, NODE_POINTER> nodeList;
            QHash<QString, NODE_RAW_POINTER> nodeRowsOfSelectedNode;

            QComboBox *nodeRowsOfNode{}, *nodesSelect{};
            QWidget *warningWidget{};
            QPushButton *pbCreate{};
            QLineEdit *search{};
            QTextBrowser *warningText{};

            void initUI();
            void showWarningIfPkNotFound(const bool &enable, const int &errorType);

        protected:
            void selectNode(const NODE_POINTER &node);
            void filterNode(const QString &filter = "");
            void deleteFilter();
            void confirm() override;
            void exit() override;

        protected slots:
            void selectNodeByIndex(const int &index);
            void selectNodeRow(const int &index);
    };

}

#endif // RELATIONMAKER_H
