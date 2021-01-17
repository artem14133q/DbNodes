#ifndef NAMENEWPROJECT_H
#define NAMENEWPROJECT_H

#include "QMainWindow"
#include "QLineEdit"
#include "QObject"
#include "QEvent"
#include "QKeyEvent"

namespace DbNodes::Modals {

    class NameNewProject : public QMainWindow
    {
        Q_OBJECT
        public:
            explicit NameNewProject(QWidget *parent = nullptr);

        private:
            QString projectName = "";
            QLineEdit *nameInput{};

            void initUi();

            void keyPressEvent(QKeyEvent *) override;
            bool eventFilter(QObject *, QEvent *) override;

        private slots:
            void setProjectName(const QString &);
            void confirm();
            void cancel();
    };

}

#endif // NAMENEWPROJECT_H
