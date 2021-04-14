//
// Created by artem on 30.01.2021.
//

#ifndef DBNODES_MAINSETTINGS_H
#define DBNODES_MAINSETTINGS_H

#include "QSettings"
#include "QVariant"

#include "utility"

namespace DbNodes::Settings {

    class MainSettings: public QSettings
    {
        public:
            typedef std::function<void (const QVariant &)> ConnectorCallback;

        private:
            QHash<QString, ConnectorCallback> callbacks;

            static MainSettings *instance;
            MainSettings();

            void resolveCallback(const QString &key, const QVariant &value);
            bool has(const QString &key);

        public:
            static MainSettings *getInstance();
            Q_DISABLE_COPY(MainSettings)

            void subscribe(const QString &settingName, const ConnectorCallback &callback);
            void unBind(const QString &settingName);

            static QVariant get(const QString &key);
            static void set(const QString &key, const QVariant &value);
    };

}



#endif //DBNODES_MAINSETTINGS_H
