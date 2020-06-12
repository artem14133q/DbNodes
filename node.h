#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QVBoxLayout>
#include <noderow.h>
#include <QPointer>

class Node : public QWidget
{
    Q_OBJECT
public:
    explicit Node(QWidget *parent = nullptr);
    explicit Node(QWidget *parent, QString id, QString name);

    QString getTableName();
    QString getTableId();

    QVector<QPointer<NodeRow>> getAllNodeRows();
    void addColumnFromFile(QString id, QString name, int type,
                           QString dbType, bool isNull = false);

private:
    QPoint oldPos;
    QVector<QPointer<NodeRow>> nodeRows;
    QVBoxLayout* rowsLayout;
    QVBoxLayout* pkLayout;
    QVBoxLayout* fkLayout;

    QString tableName;
    QString tableId;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

    void initUI();

    void addColoumn(const int nodeRowType = 0,
                    QPointer<NodeRow> nodeRow = nullptr);
    void setTableName(const QString &);
    void cleanNodeRows();
};

#endif // NODE_H
