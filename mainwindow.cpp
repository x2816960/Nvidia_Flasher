#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cmd = new QProcess(this);

    /* 将QProcess的输出打印到界面 */
    connect(cmd,&QProcess::readyReadStandardOutput,this,&MainWindow::on_readyReadStandardOutput);
    QPalette palette = ui->textEdit->palette();
    palette.setColor(QPalette::Base,Qt::black);
    palette.setColor(QPalette::Text,Qt::white);
    ui->textEdit->setPalette(palette);
}

MainWindow::~MainWindow()
{
    delete ui;

    if (cmd->state() == QProcess::Running)
    {
        cmd->terminate();
        cmd->waitForFinished();
    }
}

void MainWindow::init()
{
    char buff[1024];

    shellcmd("whoami", buff, sizeof(buff));
    QString str = QLatin1String(buff);
    qDebug() << qPrintable(str.trimmed());  //trimmed移除字符串两端的换行或空白,qPrintable移除引号
    QString root = "root";
    if(qPrintable(str.trimmed()) != root)   //当前用户如果非root，则弹出密码输入框
    {
        passwd = new Passwd(this);
        passwd->setModal(true);
        passwd->show();
        root_flag = 0;
    }
    else
    {
        root_flag = 1;
    }
}

char* MainWindow::shellcmd(char* cmd, char* buff, int size)
{
    char temp[256];
    FILE* fp = NULL;
    int offset = 0;
    int len;

    fp = popen(cmd, "r");
    if(fp == NULL)
    {
        return NULL;
    }

    while(fgets(temp, sizeof(temp), fp) != NULL)
    {
        len = strlen(temp);
        if(offset + len < size)
         {
            strcpy(buff+offset, temp);
            offset += len;
         }
        else
        {
            buff[offset] = 0;
            break;
         }
    }

    if(fp != NULL)
    {
        pclose(fp);
    }

    return buff;
}

void MainWindow::on_readyReadStandardOutput()
{
    QString outStr = QString::fromLocal8Bit(cmd->readAllStandardOutput());
    ui->textEdit->append(outStr);
}


void MainWindow::on_pushButton_clicked()
{
    QStringList arguments;

    arguments<< "d";
    cmd->setProgram("nmcli");
    cmd->setArguments(arguments);
    cmd->start();
}

void MainWindow::on_pushButton_3_clicked()
{
    qDebug() << passwd->userpasswd;
}
