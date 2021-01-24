#include "DeleteArrowButton.h"
#include "../helper.h"
#include "Workarea.h"

namespace DbNodes::Widgets {

    DeleteArrowButton::DeleteArrowButton(const QString &relationName, QWidget *parent): QPushButton(parent)
    {
        setFixedSize(16, 16);
        setText("X");
        relationId = relationName;
        setStyleSheet(Helper::getStyleFromFile("deleteArrowButton"));

        connect(this, &QPushButton::clicked, this, &DeleteArrowButton::deleteRelation);

        show();
    }

    void DeleteArrowButton::deleteRelation()
    {
        auto *workArea = dynamic_cast<WorkArea*>(parentWidget());
        workArea->deleteRelation(relationId);
        workArea->update();
        this->~DeleteArrowButton();
    }

}
