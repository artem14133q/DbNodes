//
// Created by artem on 29.04.2021.
//

#ifndef DBNODES_MINIMAPSETTINGS_H
#define DBNODES_MINIMAPSETTINGS_H

#include "AbstractSettingModal.h"
#include "../helper.h"

namespace DbNodes::Modals {

    class MinimapSettings: public Abstract::AbstractSettingModal
    {
        public:
            explicit MinimapSettings(QWidget *parent = nullptr);

        private:
            QPushButton *pbOk{};
            QPushButton *pbApply{};

            void enableConfirm(const bool &enable) override;

            void accept(const bool &close) override;

            void initSettingsUi() override;
    };

}

#endif //DBNODES_MINIMAPSETTINGS_H
