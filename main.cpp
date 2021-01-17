#include "MainWindow.h"

#include "QApplication"
#include "QProcessEnvironment"
#include "QDebug"

int main(int argc, char *argv[])
{
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    DbNodes::Widgets::MainWindow w;
    w.show();
    return QApplication::exec();
}
