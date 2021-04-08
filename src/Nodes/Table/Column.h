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

#define COLUMN_POINTER QPointer<Nodes::Table::Column>
#define COLUMN_VECTOR QVector<COLUMN_POINTER>

namespace DbNodes::Nodes::Table {

    class Column : public Abstract::AbstractNode
    {
        Q_OBJECT

        public:
            explicit Column(QVBoxLayout *vb, QWidget *parent = nullptr, const int &rowType = 0);

            explicit Column(
                QVBoxLayout *vb,
                QWidget *parent,
                const QString &columnId,
                const QString &columnName,
                const int &columnType,
                const QString &columnDbType,
                const bool &columnIsNull = false
            );

            static const int PK = 1;
            static const int FK = 2;

            QString getTableName();
            QString getTableId();
            QString getColumnId();

            int* dataForPaint();

            [[nodiscard]] int getColumnType() const;
            [[nodiscard]] bool getColumnIsNull() const;

            QString getColumnName();
            QString getColumnDbType();

            bool eventFilter(QObject *object, QEvent *event) override;
            Column *getColumnUnderMouse();

            void disableFkRelationButton(const bool &disable);

        private:
            bool isMovable = false;

            QVBoxLayout *vb;

            QString columnName;
            QString columnId;
            QString columnDbType;
            bool columnIsNull;
            int columnType;

            QLabel *moveHandle{};
            QPushButton *fkButton{};

        protected:
            void mouseMoveEvent(QMouseEvent *event) override;
            void mousePressEvent(QMouseEvent *event) override;
            void mouseReleaseEvent(QMouseEvent *event) override;

            [[nodiscard]] QStringList initTypes() const;

            void initUi();
            void deleteColumn();
            void setColumnName(const QString &);
            void setColumnDbType(const QString &);
            void setColumnIsNull(bool);
            void openRelationMaker();
    };

}

#endif // COLUMN_H
