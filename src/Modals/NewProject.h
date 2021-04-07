#ifndef NAMENEWPROJECT_H
#define NAMENEWPROJECT_H

#include "AbstractSettingModal.h"

namespace DbNodes::Modals {

    class NewProject : public Abstract::AbstractSettingModal
    {
        Q_OBJECT

        public:
            explicit NewProject(QWidget *parent = nullptr);

        private:
            QPushButton *createProjectButton{};
            QLineEdit *nameEdit{};

            void initSettingsUi() override;

            QVariant getDefaultSetting(const QString &name) override;

            void validPropertyName(QLineEdit *widget);
            void enableConfirm(const bool &enable) override;

            void afterInitUi() override;

        private slots:
            void confirm() override;
            void exit() override;

        public: signals:
            void createProjectSignal(const VARIANTS_MAP &properties);
    };

}

#endif // NAMENEWPROJECT_H
