#ifndef NAMENEWPROJECT_H
#define NAMENEWPROJECT_H

#include <QMainWindow>
#include <QLineEdit>

class NameNewProject : public QMainWindow
{
    Q_OBJECT
public:
    explicit NameNewProject(QWidget *parent = nullptr);

private:
    QString projectName = "";
    QLineEdit *nameInput;

    void initUi();

    void paintEvent(QPaintEvent *event) override;

private slots:
    void setProjectName(const QString &);
    void confirm();
    void cancel();
};

#endif // NAMENEWPROJECT_H
