#ifndef NAMENEWPROJECT_H
#define NAMENEWPROJECT_H

#include "QLineEdit"
#include "QObject"
#include "QEvent"

#include "AbstractModal.h"

namespace DbNodes::Modals {

    class NameNewProject : public Abstract::AbstractModal
    {
        Q_OBJECT

        public:
            explicit NameNewProject(QWidget *parent = nullptr);

        private:
            QString projectName = "";
            QLineEdit *nameInput{};

            void initUi();
            void setProjectName(const QString &);

        private slots:
            void confirm() override;
            void exit() override;
    };

}

#endif // NAMENEWPROJECT_H
