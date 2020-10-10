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
    QPalette palette = ui->textEdit->palette();  //
    palette.setColor(QPalette::Base,Qt::black);
    palette.setColor(QPalette::Text,Qt::white);
    ui->textEdit->setPalette(palette);
    ui->textEdit->setTextInteractionFlags(Qt::NoTextInteraction);  //textedit控件禁止鼠标选中文本方法

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
    //txtcur.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);//读取一行
    //QString qstr = txtcur.selectedText();

    QString outStr = QString::fromLocal8Bit(cmd->readAllStandardOutput().data());
    QTextCursor txtcur = ui->textEdit->textCursor();

    if(outStr.contains("\r",Qt::CaseSensitive) == true) //QT字符串判断是否含有某特定字符串,成功返回true 第二个参数表示是否大小写敏感)
    {
        if(outStr.contains("\r\n",Qt::CaseSensitive) == true) //QT字符串判断是否含有某特定字符串,成功返回true 第二个参数表示是否大小写敏感)
        {
            ui->textEdit->moveCursor(QTextCursor::End);  //此方式追加是不换行追加
            ui->textEdit->insertPlainText(outStr);       //（在读写指针的位置处插入）
            ui->textEdit->setTextCursor(txtcur);
        }else {
            QStringList list = outStr.split('\r',QString::SkipEmptyParts);

            for(int i = 0; i< list.size();++i)
            {
                QString tmp = list.at(i);

                if(ready_cnt != 0)
                {
                    txtcur.movePosition(QTextCursor::StartOfLine,QTextCursor::KeepAnchor);
                    txtcur.removeSelectedText();
                }

                ui->textEdit->moveCursor(QTextCursor::End);  //此方式追加是不换行追加
                ui->textEdit->insertPlainText(tmp);       //（在读写指针的位置处插入）
                ui->textEdit->setTextCursor(txtcur);         //
                ready_cnt++;
                qDebug()<<"tmp ="<< tmp;
            }
        }
    }else{
        ui->textEdit->moveCursor(QTextCursor::End);  //此方式追加是不换行追加
        ui->textEdit->insertPlainText(outStr);       //（在读写指针的位置处插入）
        ui->textEdit->setTextCursor(txtcur);
    }



    //ui->textEdit->append(outStr);              //此方法追加内容是换行追加
    //ui->textEdit->append(cmd->readAllStandardOutput().data());
}


void MainWindow::on_pushButton_clicked()
{
    cmd->start("bash");
    cmd->waitForStarted();
    cmd->write("sudo -S ./flash.sh -r -k DTB jetson-nano-emmc mmcblk0p1\n");
}

void MainWindow::on_pushButton_3_clicked()
{
    cmd->start("bash");
    cmd->waitForStarted();
    cmd->write("sudo -S ./flash.sh -r -K kernel/Image -k LNX jetson-nano-emmc mmcblk0p1\n");
}

void MainWindow::on_pushButton_2_clicked()
{
    cmd->start("bash");
    cmd->waitForStarted();
    cmd->write("sudo -S ./flash.sh jetson-nano-emmc mmcblk0p1\n");
}

void MainWindow::on_pushButton_4_clicked()
{
    cmd->start("bash");
    cmd->waitForStarted();
    cmd->write("sudo -S ./flash.sh -r jetson-nano-emmc mmcblk0p1\n");

}
