#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <workarea.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void createWorkArea(QString);

    static const int SAVE_TYPE_NEW_FILE = 0;
    static const int SAVE_TYPE_REWRITE_FILE = 1;

private:
    QAction* createProject;
    QAction* openProject;
    QAction* saveProject;
    QAction* saveAsProject;
    QAction* closeProject;

    WorkArea* workArea;
    QString filePath = "";

    QMenuBar* defineMenuBar();

    void generateSaveFile(const int);
    void openSaveFile();
    void createNewProject();
};
#endif // MAINWINDOW_H
