//
// Created by artem on 20.03.2021.
//

#ifndef DBNODES_REPOSITORY_H
#define DBNODES_REPOSITORY_H

#include "QWidget"
#include "TableNode.h"

namespace DbNodes::Utils::MultipleSelection {

    class Repository: public QObject
    {
        Q_OBJECT

        private:
            QList<TABLE_POINTER> selectedTables;

            bool mousePressed = false;
            QPoint mouseStartPos;
            QPoint mouseCurrentPos;

            QWidget *parentWidget();

        public:
            explicit Repository(QWidget *parent = nullptr);

            void unSelectTables();
            static void setSelectToTable(const TABLE_POINTER &table, const bool &select);
            void insertTableToSelectionList(const TABLE_POINTER &table);
            void removeTableFromSelectionList(const TABLE_POINTER &table);

            void moveSelectedTable(QObject *table, const QPoint &delta);
            void initDefaultsConnections(const TABLE_POINTER &table);

            void drawSelectionRect(QPainter &painter);

            void start(const QPoint &mousePos);
            void move(const QPoint &mousePos, const QList<TABLE_POINTER> &tables);
            void stop();

            ~Repository() override;
    };

}


#endif //DBNODES_REPOSITORY_H
