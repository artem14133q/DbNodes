#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <workarea.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    static const int SAVE_TYPE_NEW_FILE = 0;
    static const int SAVE_TYPE_REWRITE_FILE = 1;

private:
    WorkArea* workArea;
    QString filePath = "";

    QMenuBar* defineMenuBar();

    void generateSaveFile(const int);
    void openSaveFile();

private slots:
    void test();

};
#endif // MAINWINDOW_H
