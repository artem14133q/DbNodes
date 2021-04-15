//
// Created by artem on 15.04.2021.
//

#ifndef DBNODES_MOVING_H
#define DBNODES_MOVING_H

#include "AbstractUtil.h"
#include "functional"

namespace DbNodes::Utils::ListMoving {

    class Moving: public AbstractUtil
    {
        private:
            bool isMovable = false;
            QWidget *moveHandle;

        public:
            explicit Moving(QWidget *moveHandle, QObject *object = nullptr);

            QWidget *parentWidget();

            void enable();
            bool move();
            void replace(const std::function<void ()> &callback);

            template<class T>
            T *getWidgetUnderMouse(const std::function<bool (T *)> &filter)
            {
                foreach(T *w, parentWidget()->parentWidget()->findChildren<T *>())
                {
                    if (filter(w)) {
                        return w;
                    }
                }

                return nullptr;
            };
    };

}


#endif //DBNODES_MOVING_H
