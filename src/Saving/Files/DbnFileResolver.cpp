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
        ProjectParametersObject parameters;

        parameters.setName(workArea->getProjectName());
        parameters.setWorkAreaWidth(workArea->width());
        parameters.setWorkAreaHeight(workArea->height());

        object->setProjectParameters(parameters);
    }

    void DbnFileResolver::fillTables()
    {
        auto tables = workArea->getAllNodes();

        QList<TableObject> tableObjectsList;

        foreach (const NODE_POINTER &table, tables) {
            TableObject tableObject;

            tableObject.setId(table->getTableId());
            tableObject.setName(table->getTableName());
            tableObject.setX(table->x());
            tableObject.setY(table->y());

            fillColumns(tableObject, table);

            tableObjectsList.push_back(tableObject);
        }

        object->setTables(tableObjectsList);
    }

    void DbnFileResolver::fillColumns(TableObject &tableObject, const NODE_POINTER &table)
    {
        auto columns = table->getAllNodeRows();

        QList<ColumnObject> columnObjectList;

        foreach (const NODE_RAW_POINTER &column, columns.toList()) {
            ColumnObject columnObject;

            columnObject.setId(column->getRowId());
            columnObject.setName(column->getRowName());
            columnObject.setColumnType(column->getRowType());
            columnObject.setDbType(column->getRowDbType());
            columnObject.setNullable(column->getRowIsNull());

            columnObjectList.push_back(columnObject);
        }

        tableObject.setColumns(columnObjectList);
    }

    void DbnFileResolver::fillRelations()
    {
        auto relations = workArea->getAllRelations();

        QList<RelationObject> relationObjectsList;

        foreach (const RELATION_POINTER &relation, relations) {
            RelationObject relationObject;

            relationObject.setId(relation->getRelationId());
            relationObject.setFkNodeRawId(relation->getFkNodeRaw()->getRowId());
            relationObject.setPkNodeRawId(relation->getPkNodeRaw()->getRowId());
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
        foreach (const TableObject &tableObject, object->getTables()) {
            NODE_POINTER table = workArea->createNode(
                QPoint(tableObject.getX(), tableObject.getY()),
                tableObject.getId(),
                tableObject.getName()
            );

            loadColumns(tableObject, table);
        }
    }

    void DbnFileResolver::loadColumns(const TableObject &tableObject, NODE_POINTER &table)
    {
        foreach (const ColumnObject &columnObject, tableObject.getColumns()) {
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
        foreach (const RelationObject &relationObject, object->getRelations()) {
            NODE_RAW_POINTER pkNodeRow = workArea->findNodeRow(
                Widgets::WorkArea::GET_PK_NODE_ROWS, relationObject.getPkNodeRawId()
            );

            NODE_RAW_POINTER fkNodeRow = workArea->findNodeRow(
                Widgets::WorkArea::GET_FK_NODE_ROWS, relationObject.getFkNodeRawId()
            );

            workArea->makeRelation(
                relationObject.getId(),
                relationObject.getType(),
                pkNodeRow,
                fkNodeRow
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