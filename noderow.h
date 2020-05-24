#ifndef NODEROW_H
#define NODEROW_H

#include <QWidget>
#include <QStringList>
#include <QPair>

class NodeRow : public QWidget
{
    Q_OBJECT
    public:
        explicit NodeRow(QWidget *parent = nullptr,
                         const int rowType = 0);

        static const int PK = 1;
        static const int FK = 2;

        QString getTableName();
        QPair<QPoint, int> getGlobalPos();

        QString getRowName() const;

    private:
        QString rowName = "coloumn";
        const int rowType_;

        QStringList initTypes();

        void initUi();
        void deleteNodeRow();
        void setRowName(const QString &);

        void paintEvent(QPaintEvent *event) override;
};

#endif // NODEROW_H
