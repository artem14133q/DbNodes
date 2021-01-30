#ifndef NODEROW_H
#define NODEROW_H

#include "QStringList"
#include "QPair"
#include "QEvent"
#include "QPoint"
#include "QVBoxLayout"
#include "QLabel"
#include "QPushButton"

#include "AbstractNode.h"

#define NODE_RAW_POINTER QPointer<DbNodes::Widgets::NodeRow>
#define NODE_RAW_VECTOR QVector<NODE_RAW_POINTER>

namespace DbNodes::Widgets {

    class NodeRow : public Abstract::AbstractNode
    {
        Q_OBJECT

        public:
            explicit NodeRow(QVBoxLayout *vb, QWidget *parent = nullptr, const int &rowType = 0);

            explicit NodeRow(
                    QVBoxLayout *vb,
                    QWidget *parent,
                    const QString &rowId,
                    const QString &rowName,
                    const int &rowType,
                    const QString &rowDbType,
                    const bool &rowIsNull = false
                );

            static const int PK = 1;
            static const int FK = 2;

            QString getTableName();
            QString getTableId();
            QString getRowId();
            QPair<QPoint, int> getGlobalPos();

            [[nodiscard]] int getRowType() const;
            [[nodiscard]] bool getRowIsNull() const;

            QString getRowName();
            QString getRowDbType();

            bool eventFilter(QObject *object, QEvent *event) override;
            NodeRow *getNodeRowUnderMouse();

            void disableFkRelationButton(const bool &disable);

        private:
            bool isMovable = false;

            QVBoxLayout *vb;

            QString rowName;
            QString rowId;
            QString rowDbType;
            bool rowIsNull;
            int rowType;

            QLabel *moveHandle{};
            QPushButton *fkButton{};

        protected:
            void mouseMoveEvent(QMouseEvent *event) override;
            void mousePressEvent(QMouseEvent *event) override;
            void mouseReleaseEvent(QMouseEvent *event) override;

            [[nodiscard]] QStringList initTypes() const;

            void initUi();
            void deleteNodeRow();
            void setRowName(const QString &);
            void setRowDbType(const QString &);
            void setRowIsNull(bool);
            void openRelationMaker();
    };

}

#endif // NODEROW_H
