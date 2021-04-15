//
// Created by artem on 20.03.2021.
//

#include "QApplication"
#include "QDebug"

#include "Selectable.h"

namespace DbNodes::Utils::MultipleSelection {

    Selectable::Selectable(QObject *object) : AbstractUtil(object) {}

    void Selectable::setClicked(const bool &clicked)
    {
        isClicked = clicked;
    }

    void Selectable::move(const QPoint &delta)
    {
        if (abs(delta.x()) > 3 || abs(delta.y()) > 3) {
            isClicked = false;
        }

        if (parent()->property("selected").toBool() && (delta.x() != 0 || delta.y() != 0)) {
            emit moveSignal(parent(), delta);
        }
    }

    void Selectable::flush()
    {
        if (isClicked) {
            if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
                emit selectCurrentNodeSignal();
            } else if (QApplication::keyboardModifiers() & Qt::AltModifier) {
                emit unSelectCurrentNodeSignal();
            } else {
                emit unSelectNodesSignal();
            }

            isClicked = false;
        }
    }

    void Selectable::setEnable(bool enable)
    {
        isEnabled = enable;
    }

    bool Selectable::enabled()
    {
        return isEnabled;
    }

}