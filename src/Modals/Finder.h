//
// Created by artem on 27.01.2021.
//

#ifndef DBNODES_FINDER_H
#define DBNODES_FINDER_H

#include "QListWidget"
#include "QLineEdit"

#include "AbstractModal.h"
#include "Node.h"

namespace DbNodes::Modals {

    class Finder : public Abstract::AbstractModal
    {
        Q_OBJECT

        public:
            explicit Finder(const QList<NODE_POINTER> &nodeVector, QWidget *parent = nullptr);

        signals:
            void selected(const QString &nodeId);

        protected:
            void initUI();
            void exit() override;
            void confirm() override;

            void selectItemNext();
            void selectItemPrevious();
            void selectItemByIndex(const int &index);
            void unselectItems();
            int getSelectionItemIndex();

            bool eventFilter(QObject *obj, QEvent *event) override;

        private:
            QList<NODE_POINTER> nodeVector;
            QHash<QString, NODE_POINTER> filteredNodeList;

            QListWidget *listWidget{};
            QLineEdit *lineEdit{};

            void filterNodes(const QString &filter = "");
    };

}


#endif //DBNODES_FINDER_H
