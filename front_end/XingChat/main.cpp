#include "login.h"
#include "global.h"

#include <QApplication>
#include <QDir>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(XingChat);

    QString application_path = QCoreApplication::applicationDirPath();
    QString file_name = "config.ini";
    QString config_path = QDir::toNativeSeparators(application_path + QDir::separator() + file_name);

    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host", "localhost").toString();
    QString gate_port = settings.value("GateServer/port", "8080").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "XingChat_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Login w;
    w.show();
    return QCoreApplication::exec();
}
