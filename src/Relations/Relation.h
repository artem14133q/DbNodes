//
// Created by artem on 11.02.2021.
//

#ifndef DBNODES_RELATION_H
#define DBNODES_RELATION_H

#include "QObject"
#include "QPointer"
#include "QColor"
#include "QPainter"

#include "Noderow.h"
#include "DeleteArrowButton.h"

#define RELATION_POINTER QPointer<Relations::Relation>

namespace DbNodes::Relations {

    class Relation: public QObject
    {
        public:
            explicit Relation(
                QWidget *parent,
                QString  relationId,
                NODE_RAW_POINTER &pkNodeRaw,
                NODE_RAW_POINTER &fkNodeRaw
            );

            NODE_RAW_POINTER getPkNodeRaw();
            NODE_RAW_POINTER getFkNodeRaw();

            void paintPathLine(QPainter &painter, QPainterPath &painterPath);
            bool checkIsRelationValid();
            const QString &getRelationId();

            ~Relation() override;

        private:
            QString relationId;
            QColor color;

            Widgets::DeleteArrowButton *deletePathButton;

            NODE_RAW_POINTER pkNodeRaw;
            NODE_RAW_POINTER fkNodeRaw;

    };

}

#endif //DBNODES_RELATION_H
