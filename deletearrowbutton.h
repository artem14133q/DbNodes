#ifndef DELETEARROWBUTTON_H
#define DELETEARROWBUTTON_H

#include <QPushButton>
#include <QWidget>

class DeleteArrowButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DeleteArrowButton(
                QString &,
                QWidget *parent = nullptr
            );

private:
    QString relationId;

    void deleteRalation();
};

#endif // DELETEARROWBUTTON_H
