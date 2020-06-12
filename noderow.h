#ifndef NODEROW_H
#define NODEROW_H

#include <QWidget>
#include <QStringList>
#include <QPair>

class NodeRow : public QWidget
{
Q_OBJECT
public:
    explicit NodeRow(QWidget *parent = nullptr, const int rowType = 0);

    explicit NodeRow(QWidget *parent, QString rowId, QString rowName,
                     int rowType, QString rowDbType, bool rowIsNull = false);

    static const int PK = 1;
    static const int FK = 2;

    QString getTableName();
    QString getTableId();
    QString getRowId();
    QPair<QPoint, int> getGlobalPos();
    int getRowType();
    bool getRowIsNull();

    QString getRowName() const;
    QString getRowDbType();

private:
    QString rowName_;
    QString rowId_;
    QString rowDbType_;
    bool rowIsNull_;
    const int rowType_;

    QStringList initTypes();

    void initUi();
    void deleteNodeRow();
    void setRowName(const QString &);
    void setRowDbType(const QString &);
    void setRowIsNull(bool);

    void paintEvent(QPaintEvent *event) override;
};

#endif // NODEROW_H
