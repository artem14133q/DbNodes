//
// Created by artem on 10.03.2021.
//

#include "DbnFileResolver.h"
#include "RelationViews/Path/PathPoint.h"
#include "RelationTypesDictionary.h"
#include "RelationViews/DeleteRelationButton.h"

namespace DbNodes::Saving {

    DbnFileResolver::DbnFileResolver(SaveManager *saveManager, Widgets::WorkArea *workArea)
        : FileResolver(saveManager), workArea(workArea) {}

    QByteArray DbnFileResolver::toJson()
    {
        fillProjectParameters();
        fillTables();
        fillRelations();

        return object->toJson();
    }

    void DbnFileResolver::fillProjectParameters()
    {
        DbnFileStruct::ProjectParametersObject parameters;

        parameters.setName(workArea->getProjectName());
        parameters.setWorkAreaWidth(workArea->width());
        parameters.setWorkAreaHeight(workArea->height());

        object->setProjectParameters(parameters);
    }

    void DbnFileResolver::fillTables()
    {
        auto tables = workArea->getAllTables();

        QList<DbnFileStruct::TableObject> tableObjectsList;

        foreach (const Nodes::TablePtr &table, tables) {
            DbnFileStruct::TableObject tableObject;

            tableObject.setId(table->getTableId());
            tableObject.setName(table->getTableName());
            tableObject.setX(table->x());
            tableObject.setY(table->y());

            fillColumns(tableObject, table);

            tableObjectsList.push_back(tableObject);
        }

        object->setTables(tableObjectsList);
    }

    void DbnFileResolver::fillColumns(DbnFileStruct::TableObject &tableObject, const Nodes::TablePtr &table)
    {
        auto columns = table->getAllColumns();

        QList<DbnFileStruct::ColumnObject> columnObjectList;

        foreach (const Nodes::Table::ColumnPrt &column, columns.toList()) {
            DbnFileStruct::ColumnObject columnObject;

            columnObject.setId(column->getColumnId());
            columnObject.setName(column->getColumnName());
            columnObject.setColumnType(column->getColumnType());
            columnObject.setDbType(column->getColumnDbType());
            columnObject.setNullable(column->getColumnIsNull());

            columnObjectList.push_back(columnObject);
        }

        tableObject.setColumns(columnObjectList);
    }

    void DbnFileResolver::fillRelations()
    {
        auto relations = workArea->getAllRelations();

        QList<DbnFileStruct::RelationObject> relationObjectsList;

        foreach (const Relations::RelationPtr &relation, relations) {
            DbnFileStruct::RelationObject relationObject;

            relationObject.setId(relation->getRelationId());
            relationObject.setFkColumnId(relation->getFkColumn()->getColumnId());
            relationObject.setPkColumnId(relation->getPkColumn()->getColumnId());
            relationObject.setType(relation->getRelationTypeId());
            relationObject.setPosition(relation->getRelationPositionType());

            fillRelationPathPoints(relationObject, relation);

            relationObjectsList.push_back(relationObject);
        }

        object->setRelations(relationObjectsList);
    }

    void DbnFileResolver::fromJson()
    {
        loadProjectParameters();
        loadTables();
        loadRelations();
    }

    void DbnFileResolver::loadProjectParameters()
    {
        auto projectParametersObject = object->getProjectParameters();

        workArea->setProjectName(projectParametersObject.getName());

        workArea->setFixedSize(
            projectParametersObject.getWorkAreaWidth(),
            projectParametersObject.getWorkAreaHeight()
        );
    }

    void DbnFileResolver::loadTables()
    {
        foreach (const DbnFileStruct::TableObject &tableObject, object->getTables()) {
            Nodes::TablePtr table = workArea->createTable(
                QPoint(tableObject.getX(), tableObject.getY()),
                tableObject.getId(),
                tableObject.getName()
            );

            loadColumns(tableObject, table);
        }
    }

    void DbnFileResolver::loadColumns(const DbnFileStruct::TableObject &tableObject, Nodes::TablePtr &table)
    {
        foreach (const DbnFileStruct::ColumnObject &columnObject, tableObject.getColumns()) {
            table->createColumn(
                columnObject.getId(),
                columnObject.getName(),
                (Nodes::Table::Column::Type) columnObject.getColumnType(),
                columnObject.getDbType(),
                columnObject.getNullable()
            );
        }
    }

    void DbnFileResolver::loadRelations()
    {
        foreach (const DbnFileStruct::RelationObject &relationObject, object->getRelations()) {
            Nodes::Table::ColumnPrt pkColumn = workArea->findColumn(
                    Widgets::WorkArea::GET_PK_COLUMNS, relationObject.getPkColumnId()
            );

            Nodes::Table::ColumnPrt fkColumn = workArea->findColumn(
                    Widgets::WorkArea::GET_FK_COLUMNS, relationObject.getFkColumnId()
            );

            auto relation = workArea->makeRelation(
                relationObject.getId(),
                (Dictionaries::RelationTypesDictionary::Type) relationObject.getType(),
                pkColumn,
                fkColumn
            );

            relation->setRelationPositionType(
                (Dictionaries::RelationPositionsDictionary::Type) relationObject.getPosition()
            );

            loadRelationPathPoints(relationObject, relation);
        }
    }

    QString DbnFileResolver::getProjectName()
    {
        return object->getProjectParameters().getName();
    }

    void DbnFileResolver::fillRelationPathPoints(
        DbnFileStruct::RelationObject &relationObject,
        const QPointer<Relations::Relation> &relation
    ) {
        if (relation->getRelationTypeId() != Dictionaries::RelationTypesDictionary::Type::Path) {
            return;
        }

        QList<DbnFileStruct::RelationPathPointObject> relationPathPointObjectList;

        auto deleteRelationButton = dynamic_cast<Relations::DeleteRelationButton *>(relation->getAbstractRelationView());

        foreach (const Relations::Path::RelationPathPointPtr &pathPointPtr, deleteRelationButton->getPoints()) {
            DbnFileStruct::RelationPathPointObject relationPathPointObject;

            relationPathPointObject.setX(pathPointPtr->x());
            relationPathPointObject.setY(pathPointPtr->y());

            relationPathPointObjectList.push_front(relationPathPointObject);
        }

        relationObject.setPathPoints(relationPathPointObjectList);
    }

    void DbnFileResolver::loadRelationPathPoints(
        const DbnFileStruct::RelationObject &relationObject,
        const QPointer<Relations::Relation> &relation
    ) {
        if (relation->getRelationTypeId() != Dictionaries::RelationTypesDictionary::Type::Path) {
            return;
        }

        auto deleteRelationButton = dynamic_cast<Relations::DeleteRelationButton *>(relation->getAbstractRelationView());

        foreach (const DbnFileStruct::RelationPathPointObject &pointObject, relationObject.getPathPoints()) {
            deleteRelationButton->createPoint(QPoint(pointObject.getX(), pointObject.getY()));
        }
    }
}