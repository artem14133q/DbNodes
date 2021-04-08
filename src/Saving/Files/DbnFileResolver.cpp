//
// Created by artem on 10.03.2021.
//

#include "DbnFileResolver.h"

#include <utility>

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

        foreach (const TABLE_POINTER &table, tables) {
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

    void DbnFileResolver::fillColumns(DbnFileStruct::TableObject &tableObject, const TABLE_POINTER &table)
    {
        auto columns = table->getAllColumns();

        QList<DbnFileStruct::ColumnObject> columnObjectList;

        foreach (const COLUMN_POINTER &column, columns.toList()) {
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

        foreach (const RELATION_POINTER &relation, relations) {
            DbnFileStruct::RelationObject relationObject;

            relationObject.setId(relation->getRelationId());
            relationObject.setFkColumnId(relation->getFkColumn()->getColumnId());
            relationObject.setPkColumnId(relation->getPkColumn()->getColumnId());
            relationObject.setType(relation->getRelationTypeId());
            relationObject.setPosition(relation->getRelationPositionType());

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
            TABLE_POINTER table = workArea->createTable(
                QPoint(tableObject.getX(), tableObject.getY()),
                tableObject.getId(),
                tableObject.getName()
            );

            loadColumns(tableObject, table);
        }
    }

    void DbnFileResolver::loadColumns(const DbnFileStruct::TableObject &tableObject, TABLE_POINTER &table)
    {
        foreach (const DbnFileStruct::ColumnObject &columnObject, tableObject.getColumns()) {
            table->addColumnFromFile(
                columnObject.getId(),
                columnObject.getName(),
                columnObject.getColumnType(),
                columnObject.getDbType(),
                columnObject.getNullable()
            );
        }
    }

    void DbnFileResolver::loadRelations()
    {
        foreach (const DbnFileStruct::RelationObject &relationObject, object->getRelations()) {
            COLUMN_POINTER pkColumn = workArea->findColumn(
                    Widgets::WorkArea::GET_PK_COLUMNS, relationObject.getPkColumnId()
            );

            COLUMN_POINTER fkColumn = workArea->findColumn(
                    Widgets::WorkArea::GET_FK_COLUMNS, relationObject.getFkColumnId()
            );

            workArea->makeRelation(
                relationObject.getId(),
                relationObject.getType(),
                pkColumn,
                fkColumn
            )->setRelationPositionType(
                relationObject.getPosition()
            );
        }
    }

    QString DbnFileResolver::getProjectName()
    {
        return object->getProjectParameters().getName();
    }
}