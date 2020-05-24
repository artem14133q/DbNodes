#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <workarea.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    WorkArea* workArea;

    QMenuBar* defineMenuBar();

private slots:
    void test();

};
#endif // MAINWINDOW_H
