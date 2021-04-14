//
// Created by artem on 31.01.2021.
//

#ifndef DBNODES_SETTINGS_H
#define DBNODES_SETTINGS_H

#include "QVBoxLayout"
#include "QPushButton"
#include "QLabel"

#include "AbstractSettingModal.h"
#include "DrawableWidget.h"

namespace DbNodes::Modals {

    class Settings : public Abstract::AbstractSettingModal
    {
        public:
            explicit Settings(QWidget *parent = nullptr);

        private:
            QPushButton *pbApply{};
            QPushButton *pbOk{};

            void initSettingsUi() override;

            void accept(const bool &close) override;

        protected:
            void enableConfirm(const bool &enable) override;
    };

}


#endif //DBNODES_SETTINGS_H
