#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "passwd.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();
    char *shellcmd(char* cmd, char* buff, int size);
    int root_flag;  //是否是root用户，1为root,0为非root
    int ready_cnt = 0;

private slots:
    void on_readyReadStandardOutput();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QProcess *cmd;
    Passwd *passwd;
};

#endif // MAINWINDOW_H
