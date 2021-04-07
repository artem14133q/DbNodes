//
// Created by artem on 21.01.2021.
//

#include "AbstractModal.h"
#include "QKeyEvent"

namespace DbNodes::Abstract {

    AbstractModal::AbstractModal(QWidget *parent) : QMainWindow(parent)
    {
        installEventFilter(this);
    }

    void AbstractModal::exit()
    {
        deleteLater();
    }

    void AbstractModal::confirm()
    {
        exit();
    }

    bool AbstractModal::eventFilter(QObject *obj, QEvent *event)
    {
        if (event->type() == QEvent::KeyPress) {
            auto *keyEvent = dynamic_cast<QKeyEvent *>(event);

            switch (keyEvent->key()) {
                case Qt::Key_Return:

                    if (acceptOnEnter) {
                        accept(true);
                    } else {
                        confirm();
                    }

                    return true;

                case Qt::Key_Escape:
                    exit(); return true;
            }
        }

        return QObject::eventFilter(obj, event);
    }

    void AbstractModal::closeEvent(QCloseEvent *event)
    {
        event->ignore();
        exit();
    }

    void AbstractModal::setAcceptOnEnter(const bool &enable)
    {
        acceptOnEnter = enable;
    }

    void AbstractModal::accept(const bool &close)
    {
        if (close) confirm();
    }
}
