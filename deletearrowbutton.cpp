#include "deletearrowbutton.h"
#include "qpushbutton.h"
#include "helper.h"
#include "workarea.h"

DeleteArrowButton::DeleteArrowButton(QString &relationName, QWidget *parent)
    : QPushButton(parent)
{
    this->setFixedSize(16, 16);
    this->setText("X");
    this->relationId = relationName;
    this->setStyleSheet(Helper::getStyleFromFile("deleteArrowButton"));

    connect(this, &QPushButton::clicked, this, &DeleteArrowButton::deleteRalation);

    this->show();
}

void DeleteArrowButton::deleteRalation()
{
    WorkArea *workArea = static_cast<WorkArea*>(parentWidget());
    workArea->deleteRalation(this->relationId);
    workArea->update();
    this->~DeleteArrowButton();
}
