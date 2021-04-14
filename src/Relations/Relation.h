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

namespace DbNodes::Relations {

    class Relation: public QObject
    {
        Q_OBJECT

        public:
            explicit Relation(
                QWidget *parent,
                QString  relationId,
                Dictionaries::RelationTypesDictionary::Type relationTypeId,
                Nodes::Table::ColumnPrt &pkColumn,
                Nodes::Table::ColumnPrt &fkColumn
            );

            Nodes::Table::ColumnPrt getPkColumn();
            Nodes::Table::ColumnPrt getFkColumn();

            [[nodiscard]]
            int getRelationTypeId() const;

            bool checkIsRelationValid();
            const QString &getRelationId();

            void updateRelation(QPainter &painter, QPainterPath &path);
            void raise();

            int getRelationPositionType();
            void setRelationPositionType(const Dictionaries::RelationPositionsDictionary::Type &type);

            Abstract::AbstractRelationView *getAbstractRelationView();

            ~Relation() override;

        public slots:
            void deleteRelation();
            void goToRelationTableSignal();
            void enableRelationType(const Dictionaries::RelationTypesDictionary::Type &relationTypeId);
            void createNodeInWorkArea(Abstract::AbstractNode *node);

        private:
            Abstract::AbstractRelationView *relationView{};

            int relationType{};

            QWidget *parent;

            QString relationId;

            Nodes::Table::ColumnPrt pkColumn;
            Nodes::Table::ColumnPrt fkColumn;

        public: signals:
            void goToRelatedTable(const QString &id);
            void createNodeInWorkAreaSignal(Abstract::AbstractNode *node);

    };

    typedef QPointer<Relations::Relation> RelationPtr;
}

#endif //DBNODES_RELATION_H
