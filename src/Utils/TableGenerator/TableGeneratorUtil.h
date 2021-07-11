//
// Created by artem on 25.06.2021.
//

#ifndef DBNODES_TABLEGENERATORUTIL_H
#define DBNODES_TABLEGENERATORUTIL_H

#include "QObject"
#include "AbstractUtil.h"
#include "DbTableTypesIndexDictionary.h"
#include "TableNode.h"

namespace DbNodes::Utils::TableGenerator
{
    using ColumnType = Nodes::Table::Column::Type;
    using Column = Nodes::Table::Column;
    using Table = Nodes::TableNode;
    using IndexTypesDictionary = Dictionaries::DbTableTypesIndexDictionary;

    class TableGeneratorUtil: public AbstractUtil
    {
        private:
            Nodes::TablePtr table;

        protected:
            enum StandardTypes {
                Index = IndexTypesDictionary::Bigint,
                Foreign = IndexTypesDictionary::Bigint,
                Default = IndexTypesDictionary::Integer,
            };

            IndexTypesDictionary::Type determinateColumnType(const ColumnType& columnType);

        public:
            inline static const QString DEFAULT_COLUMN_NAME = "column";

            explicit TableGeneratorUtil(QWidget *parentWidget, const QString& name);
            explicit TableGeneratorUtil(QWidget *parentWidget, Nodes::TablePtr& table);
            explicit TableGeneratorUtil(Nodes::TableNode* table);

            static TableGeneratorUtil *makeTable(QWidget *parentWidget, const QString& name);
            static TableGeneratorUtil *fillTable(QWidget *parentWidget, Nodes::TablePtr& table);
            static TableGeneratorUtil *fillTable(Nodes::TableNode* table);

            void createColumn(
                const ColumnType& columnType,
                const QString& name = TableGeneratorUtil::DEFAULT_COLUMN_NAME,
                const QString& dbType = nullptr,
                bool isNull = false
            );

            TableGeneratorUtil *createIndexColumn(const QString& name, bool isNull = false);

            TableGeneratorUtil *createForeignColumn(const QString& name, bool isNull = false);

            TableGeneratorUtil *createDefaultColumn(
                    const QString& name,
                    const QString& dbType = nullptr,
                    bool isNull = false
            );

            Nodes::TablePtr getTable();
    };

}

#endif //DBNODES_TABLEGENERATORUTIL_H
