#include "QMenu"

#include "DeleteRelationButton.h"
#include "RelationTypesDictionary.h"
#include "../../helper.h"

namespace DbNodes::Relations {

    DeleteRelationButton::DeleteRelationButton(
        const COLUMN_POINTER &pkColumn,
        const COLUMN_POINTER &fkColumn,
        QWidget *parent
    ):  Abstract::AbstractRelationView(parent, pkColumn, fkColumn) {
        setFixedSize(16, 16);
        setStyleSheet("QWidget{border: 0; border-radius: 8px;}");

        color = QColor();
        color.setRgb(47, 167, 227);

        initUI();
    }

    void DeleteRelationButton::initUI()
    {
        auto delButton = new QPushButton("X", this);
        delButton->setFixedSize(16, 16);
        delButton->move(0, 0);
        delButton->setStyleSheet(Helper::getStyleFromFile("deleteArrowButton"));
        connect(delButton, &QPushButton::clicked, this, &DeleteRelationButton::deleteRelation);
    }

    void DeleteRelationButton::deleteRelation()
    {
        emit clickedDelete();
    }

    void DeleteRelationButton::contextMenuEvent(QContextMenuEvent *event)
    {
        auto menuPos = mapToGlobal(event->pos());
        menuPos.setX(menuPos.x() + 5);
        //Set visible menu
        createContextMenu()->exec(menuPos);
    }

    void DeleteRelationButton::updateRelation(QPainter &painter, QPainterPath &path)
    {
        painter.setPen(QPen(color, 2, Qt::SolidLine, Qt::FlatCap));

        int *pkBuf = pkColumn->dataForPaint();
        int *fkBuf = fkColumn->dataForPaint();

        if (pkBuf[0] < fkBuf[0]) {
            pkBuf[0] += pkBuf[2];

            move(fkBuf[0] - width() - 2, fkBuf[1] - height() / 2);

            fkBuf[0] -= width() + 2;
        } else {
            fkBuf[0] += fkBuf[2];

            move(fkBuf[0] + 2, fkBuf[1] - height() / 2);

            fkBuf[0] += width();
        }

        int cP12_x = pkBuf[0] + (fkBuf[0] - pkBuf[0]) / 2;

        path.moveTo(pkBuf[0], pkBuf[1]);
        path.cubicTo(cP12_x, pkBuf[1], cP12_x, fkBuf[1], fkBuf[0], fkBuf[1]);

        delete pkBuf;
        delete fkBuf;

        painter.drawPath(path);
    }

    int DeleteRelationButton::getCurrentTypeId()
    {
        return RELATION_TYPE_PATH;
    }
}
