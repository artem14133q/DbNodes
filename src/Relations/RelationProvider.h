//
// Created by artem on 28.02.2021.
//

#ifndef DBNODES_RELATIONPROVIDER_H
#define DBNODES_RELATIONPROVIDER_H

#include "QObject"

#include "AbstractRelationView.h"
#include "Relation.h"
#include "Table/Column.h"

namespace DbNodes::Relations {

    class RelationProvider: public QObject
    {
        private:
            Relation *relation;

            QWidget *parentWidget{};

        protected:
            QHash<QString, QVariant> parametersList;

            QWidget *getParentWidget();

            QVariant getParameter(const QString &parameterName);

            Relation *getRelation();

            void initDefaultsConnects(Abstract::AbstractRelationView *relationView);

        public:
            explicit RelationProvider();

            void setParent(QWidget *parent);
            void setRelation(Relation *relation_);

            void setParameters(const QHash<QString, QVariant> &parameters);

            virtual Abstract::AbstractRelationView *create();

            ~RelationProvider() override;
    };

}

#endif //DBNODES_RELATIONPROVIDER_H
