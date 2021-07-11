//
// Created by artem on 25.06.2021.
//

#include "TableGeneratorUtil.h"
#include "../helper.h"

namespace DbNodes::Utils::TableGenerator {

    /**
     * TableGenerator Constructor
     * @param parentWidget
     * @param name
     */
    TableGeneratorUtil::TableGeneratorUtil(QWidget *parentWidget, const QString& name): AbstractUtil(parentWidget)
    {
        table = new Nodes::TableNode(parentWidget, name);
    }

    /**
     * TableGenerator Constructor
     * @param parentWidget
     * @param table
     */
    TableGeneratorUtil::TableGeneratorUtil(QWidget *parentWidget, Nodes::TablePtr& table):
    AbstractUtil(parentWidget), table(table) {}

    /**
     * TableGenerator Constructor
     * @param table
     */
    TableGeneratorUtil::TableGeneratorUtil(Nodes::TableNode *table):
    AbstractUtil(table), table(QPointer<Nodes::TableNode>(table)) {}

    /**
     * Create instance of TableGeneratorUtil with new table
     * @param parent
     * @param name
     * @return TableGeneratorUtil object
     */
    TableGeneratorUtil *TableGeneratorUtil::makeTable(QWidget *parent, const QString& name)
    {
        return new TableGeneratorUtil(parent, name);
    }

    /**
     * Create instance of TableGeneratorUtil with already created table
     * @param parent
     * @param table
     * @return TableGeneratorUtil object
     */
    TableGeneratorUtil *TableGeneratorUtil::fillTable(QWidget *parent, Nodes::TablePtr& table)
    {
        return new TableGeneratorUtil(parent, table);
    }

    /**
     * Create instance of TableGeneratorUtil with already created table
     * @param table
     * @return
     */
    TableGeneratorUtil *TableGeneratorUtil::fillTable(Nodes::TableNode* table)
    {
        return new TableGeneratorUtil(table);
    }

    /**
     * Create new Column for table with default parameters
     * @param columnType
     * @param name
     * @param dbType
     * @param isNull
     */
    void TableGeneratorUtil::createColumn(
        const ColumnType &columnType,
        const QString &name,
        const QString& dbType,
        const bool isNull
    ) {
        auto databaseType = dbType == nullptr
                ? IndexTypesDictionary::getDictionary().value(determinateColumnType(columnType)).toString()
                : dbType;

        table->createColumn(Table::generateId(), name, columnType, databaseType, isNull);
    }

    /**
     * Return standard database type for current column type
     * @param columnType
     * @return database type
     */
    IndexTypesDictionary::Type TableGeneratorUtil::determinateColumnType(const ColumnType &columnType)
    {
        if (columnType == ColumnType::PrimaryKey) {
            return (IndexTypesDictionary::Type) StandardTypes::Index;
        }

        if (columnType == ColumnType::ForeignKey) {
            return (IndexTypesDictionary::Type) StandardTypes::Foreign;
        }

        return (IndexTypesDictionary::Type) StandardTypes::Default;
    }

    /**
     * Create PrimaryKey column in table
     * @param name
     * @param isNull
     * @return self
     */
    TableGeneratorUtil *TableGeneratorUtil::createIndexColumn(const QString& name, bool isNull)
    {
        createColumn(ColumnType::PrimaryKey, name, nullptr, isNull);

        return this;
    }

    /**
     * Create ForeignKey column in table
     * @param name
     * @param isNull
     * @return self
     */
    TableGeneratorUtil *TableGeneratorUtil::createForeignColumn(const QString& name, bool isNull)
    {
        createColumn(ColumnType::ForeignKey, name, nullptr, isNull);

        return this;
    }

    /**
     * Create content column in table
     * @param name
     * @param dbType
     * @param isNull
     * @return self
     */
    TableGeneratorUtil *TableGeneratorUtil::createDefaultColumn(const QString &name, const QString &dbType, bool isNull)
    {
        createColumn(ColumnType::Default, name, dbType, isNull);

        return this;
    }

    /**
     * Return generated table
     * @return
     */
    Nodes::TablePtr TableGeneratorUtil::getTable()
    {
        return table;
    }
}
