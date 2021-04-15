#ifndef COLUMN_H
#define COLUMN_H

#include "QStringList"
#include "QPair"
#include "QEvent"
#include "QPoint"
#include "QVBoxLayout"
#include "QLabel"
#include "QPushButton"

#include "AbstractNode.h"
#include "IHasPathConnectors.h"
#include "ListMoving/Moving.h"

namespace DbNodes::Nodes::Table {

    class Column : public Abstract::AbstractNode, public Abstract::IHasPathConnectors
    {
        Q_OBJECT

        public:
            enum Type {
                Default,
                PrimaryKey,
                ForeignKey
            };

            explicit Column(
                QVBoxLayout *vb,
                QWidget *parent = nullptr,
                const Column::Type &columnType = Column::Type::Default
            );

            explicit Column(
                QVBoxLayout *vb,
                QWidget *parent,
                const QString &columnId,
                const QString &columnName,
                const Column::Type &columnType,
                const QString &columnDbType,
                const bool &columnIsNull = false
            );

            QString getTableName();
            QString getTableId();
            QString getColumnId();

            Abstract::ParamsForDrawing getDrawParams() override;

            [[nodiscard]] Type getColumnType() const;
            [[nodiscard]] bool getColumnIsNull() const;

            QString getColumnName();
            QString getColumnDbType();

            bool eventFilter(QObject *object, QEvent *event) override;

            void disableFkRelationButton(const bool &disable);

        private:
            Utils::ListMoving::Moving *movingUtil;

            QVBoxLayout *vb;

            QString columnName;
            QString columnId;
            QString columnDbType;
            bool columnIsNull;
            Type columnType;

            QLabel *moveHandle{};
            QPushButton *fkButton{};

            void initUi();

        protected:
            void mouseMoveEvent(QMouseEvent *event) override;
            void mousePressEvent(QMouseEvent *event) override;
            void mouseReleaseEvent(QMouseEvent *event) override;

            [[nodiscard]] QStringList initTypes() const;

            void deleteColumn();
            void setColumnName(const QString &);
            void setColumnDbType(const QString &);
            void setColumnIsNull(bool isNull);
            void openRelationMaker();
    };

    typedef QPointer<Nodes::Table::Column> ColumnPrt;
    typedef QVector<ColumnPrt> ColumnPrtVector;
}

#endif // COLUMN_H
