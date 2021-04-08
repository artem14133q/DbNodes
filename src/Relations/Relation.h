//
// Created by artem on 11.02.2021.
//

#ifndef DBNODES_RELATION_H
#define DBNODES_RELATION_H

#include "QObject"
#include "QPointer"
#include "QColor"
#include "QPainter"

#include "Table/Column.h"
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
                    int relationTypeId,
                    COLUMN_POINTER &pkColumn,
                    COLUMN_POINTER &fkColumn
            );

            COLUMN_POINTER getPkColumn();
            COLUMN_POINTER getFkColumn();

            [[nodiscard]]
            int getRelationTypeId() const;

            bool checkIsRelationValid();
            const QString &getRelationId();

            void updateRelation(QPainter &painter, QPainterPath &path);
            void raise();

            int getRelationPositionType();
            void setRelationPositionType(const int &type);

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

            COLUMN_POINTER pkColumn;
            COLUMN_POINTER fkColumn;

        public: signals:
            void goToRelatedTable(const QString &id);

    };

}

#endif //DBNODES_RELATION_H
