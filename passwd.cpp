#include "passwd.h"
#include "ui_passwd.h"
#include <QTest>

Passwd::Passwd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Passwd)
{
    ui->setupUi(this);
}

Passwd::~Passwd()
{
    delete ui;
}

char* Passwd::shellcmd(char* cmd, char* buff, int size)
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

void Passwd::on_pushButton_clicked()
{
    char buff[1024];
    char cmd[1024];
    char *userpasswd_char;
    userpasswd = ui->lineEdit->text().toUtf8();
    userpasswd = userpasswd + '\n';
    QByteArray ba = userpasswd.toLatin1();
    userpasswd_char = ba.data();

    memset(cmd,0,1024);
    memset(buff,0,1024);

    QString str = QLatin1String(userpasswd_char);
    qDebug() << qPrintable(str.trimmed());  //trimmed移除字符串两端的换行或空白,qPrintable移除引号

    sprintf(cmd,"echo \"%s\" | sudo -S ls Nvidia_flasher",qPrintable(str.trimmed()));
    qDebug() << cmd;
    shellcmd(cmd, buff, sizeof(buff));
    qDebug() << buff;

    QTest::qSleep(1500);

    QString buff_string(buff);

    if(buff_string.contains("Nvidia_flasher",Qt::CaseSensitive) == true) //QT字符串判断是否含有某特定字符串,成功返回true 第二个参数表示是否大小写敏感
    {
        qDebug() << "密码正确";
        this->close();
    }else {
        qDebug() << "密码错误";
        ui->label_2->setText("密码错误 Passwd error!");
    }
}
