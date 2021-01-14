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
    int ready_cnt = 0;
    void button_enable();
    void button_disable();

    QString sFile = ".nvidia/.config";
    bool confirmFile(const QString sFile);

    void DeleteOneline(int nNum, QString &strall);
    void deleteOnelineInFile(int nNumLine, QString &filename);

    /* board_type 板卡类型：
     * 0：Jetson nano B01 EMMC
     * 1: Jetson nano B01 SDCARD
     * 2: Jetson nx EMMC
     * 3: Jetson nx Sdcard
     */
    int board_type;


private slots:
    void on_readyReadStandardOutput();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_actionInformation_triggered();

    void on_actionJetson_nano_B01_EMMC_triggered();

    void on_actionJetson_nano_B01_Sdcard_triggered();

    void on_actionJetson_nx_EMMC_triggered();

    void on_actionJetson_nx_Sdcard_triggered();

    void on_actionIMG_Load_triggered();

private:
    Ui::MainWindow *ui;
    QProcess *cmd;
    Passwd *passwd;
};

#endif // MAINWINDOW_H
