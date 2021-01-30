//
// Created by artem on 30.01.2021.
//

#ifndef DBNODES_MAINSETTINGS_H
#define DBNODES_MAINSETTINGS_H

#include "QSettings"

namespace DbNodes::Settings {

    class MainSettings: public QSettings
    {
        private:
            static MainSettings *instance;
            MainSettings();

        public:
            static MainSettings *getInstance();
            Q_DISABLE_COPY(MainSettings)
    };

}



#endif //DBNODES_MAINSETTINGS_H
