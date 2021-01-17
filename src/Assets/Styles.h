//
// Created by artem on 27.12.2020.
//

#ifndef DBNODES_STYLES_H
#define DBNODES_STYLES_H

#include "QMap"

namespace DbNodes::Assets {

    class Styles
    {
        private:
            QMap<int, QString> styles;
            int currentTheme;

        protected:
            Styles();
            static Styles *instance;

        public:
            static const int THEME_LIGHT = 0;
            static const int THEME_DARK = 1;

            Styles(Styles &other) = delete;
            Styles(Styles &&other) = delete;
            void operator=(const Styles &) = delete;

            static Styles* getInstance();
    };


}

#endif //DBNODES_STYLES_H
