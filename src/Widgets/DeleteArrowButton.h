#ifndef DELETEARROWBUTTON_H
#define DELETEARROWBUTTON_H

#include "QPushButton"
#include "QWidget"
#include "QPointer"

namespace DbNodes::Widgets {

    class DeleteArrowButton : public QPushButton
    {
        Q_OBJECT
        public:
            explicit DeleteArrowButton(QWidget *parent = nullptr);

        private:
            void deleteRelation();

        public: signals:
            void clicked();
    };


}

#endif // DELETEARROWBUTTON_H
