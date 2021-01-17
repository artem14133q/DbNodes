#ifndef DELETEARROWBUTTON_H
#define DELETEARROWBUTTON_H

#include "QPushButton"
#include "QWidget"

namespace DbNodes::Widgets {

    class DeleteArrowButton : public QPushButton
    {
        Q_OBJECT
        public:
            explicit DeleteArrowButton(const QString &relationName, QWidget *parent = nullptr);

        private:
            QString relationId;

            void deleteRelation();
    };


}

#endif // DELETEARROWBUTTON_H
