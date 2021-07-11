//
// Created by artem on 26.12.2020.
//

#ifndef DBNODES_TABLECONSTRUCTOR_H
#define DBNODES_TABLECONSTRUCTOR_H

#include "AbstractSettingModal.h"

namespace DbNodes::Modals {

    class TableConstructor: public Abstract::AbstractSettingModal
    {
        Q_OBJECT

        private:
            QLineEdit *nameLineEdit{};
            QString tableName;

        public:
            explicit TableConstructor(const QString &name, QWidget* parent = nullptr);

        protected:
            void initSettingsUi() override;
            QVariant getDefaultSetting(const QString &name) override;

            void afterInitUi() override;

            void confirm() override;

        public: signals:
            void pushConfirm(const QHash<QString, QVariant> &settingsMap);
            void pushExit();
    };

}

#endif //DBNODES_TABLECONSTRUCTOR_H
