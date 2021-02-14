//
// Created by artem on 11.02.2021.
//

#include "QPen"

#include "Relation.h"

#include <utility>

namespace DbNodes::Relations {

    Relation::Relation(
        QWidget *parent,
        QString relationId,
        NODE_RAW_POINTER &pkNodeRaw,
        NODE_RAW_POINTER &fkNodeRaw
    ):  QObject(parent),
        relationId(std::move(relationId)),
        pkNodeRaw(pkNodeRaw),
        fkNodeRaw(fkNodeRaw)
    {
        color = QColor();
        color.setRgb(47, 167, 227);

        deletePathButton = new Widgets::DeleteArrowButton(parent);

        connect(deletePathButton, &Widgets::DeleteArrowButton::clicked, this, [this] () {
            this->~Relation();
        });
    }

    NODE_RAW_POINTER Relation::getPkNodeRaw()
    {
        return pkNodeRaw;
    }

    NODE_RAW_POINTER Relation::getFkNodeRaw()
    {
        return fkNodeRaw;
    }

    void Relation::paintPathLine(QPainter &painter, QPainterPath &painterPath)
    {
        painter.setPen(QPen(color, 2, Qt::SolidLine, Qt::FlatCap));

        int *pkBuf = pkNodeRaw->dataForPaint();
        int *fkBuf = fkNodeRaw->dataForPaint();

        if (pkBuf[0] < fkBuf[0]) {
            pkBuf[0] += pkBuf[2];

            deletePathButton->move(
                fkBuf[0] - deletePathButton->width() - 2,
                fkBuf[1] - deletePathButton->height() / 2
            );

            fkBuf[0] -= deletePathButton->width() + 2;
        } else {
            fkBuf[0] += fkBuf[2];

            deletePathButton->move(
                fkBuf[0] + 2,
                fkBuf[1] - deletePathButton->height() / 2
            );

            fkBuf[0] += deletePathButton->width();
        }

        int cP12_x = pkBuf[0] + (fkBuf[0] - pkBuf[0]) / 2;

        painterPath.moveTo(pkBuf[0], pkBuf[1]);
        painterPath.cubicTo(cP12_x, pkBuf[1], cP12_x, fkBuf[1], fkBuf[0], fkBuf[1]);

        delete pkBuf;
        delete fkBuf;

        painter.drawPath(painterPath);
    }

    bool Relation::checkIsRelationValid()
    {
        return pkNodeRaw && fkNodeRaw;
    }

    Relation::~Relation()
    {
        delete deletePathButton;
        dynamic_cast<QWidget *>(parent())->update();

        if (fkNodeRaw) {
            fkNodeRaw->disableFkRelationButton(false);
        }

        deleteLater();
    }

    const QString &Relation::getRelationId()
    {
        return relationId;
    }
}
