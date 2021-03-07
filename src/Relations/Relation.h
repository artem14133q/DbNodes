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
#include "AbstractRelationView.h"

#define RELATION_POINTER QPointer<Relations::Relation>

namespace DbNodes::Relations {

    class Relation: public QObject
    {
        Q_OBJECT

        public:
            explicit Relation(
                QWidget *parent,
                QString  relationId,
                const int relationTypeId,
                NODE_RAW_POINTER &pkNodeRaw,
                NODE_RAW_POINTER &fkNodeRaw
            );

            NODE_RAW_POINTER getPkNodeRaw();
            NODE_RAW_POINTER getFkNodeRaw();

            [[nodiscard]]
            int getRelationTypeId() const;

            bool checkIsRelationValid();
            const QString &getRelationId();

            void updateRelation(QPainter &painter, QPainterPath &path);
            void raise();

            ~Relation() override;

        public slots:
            void deleteRelation();
            void goToRelationTableSignal();
            void enableRelationType(const int &relationTypeId);

        private:
            Abstract::AbstractRelationView *relationView{};

            int relationType{};

            QWidget *parent;

            QString relationId;

            NODE_RAW_POINTER pkNodeRaw;
            NODE_RAW_POINTER fkNodeRaw;

        public: signals:
            void goToRelatedTable(const QString &id);

    };

}

#endif //DBNODES_RELATION_H
