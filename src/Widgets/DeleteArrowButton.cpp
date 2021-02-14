#include "DeleteArrowButton.h"
#include "../helper.h"

namespace DbNodes::Widgets {

    DeleteArrowButton::DeleteArrowButton(QWidget *parent): QPushButton(parent)
    {
        setFixedSize(16, 16);
        setText("X");
        setStyleSheet(Helper::getStyleFromFile("deleteArrowButton"));

        connect(this, &QPushButton::clicked, this, &DeleteArrowButton::deleteRelation);

        show();
    }

    void DeleteArrowButton::deleteRelation()
    {
        emit clicked();
    }

}
