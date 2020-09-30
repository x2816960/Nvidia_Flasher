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
    char *userpasswd_char;
    userpasswd = ui->lineEdit->text().toUtf8();

    shellcmd("sudo -S ls Nvidia_flasher", buff, sizeof(buff));
    memset(buff,0,1024);
    userpasswd = userpasswd + '\n';
    QByteArray ba = userpasswd.toLatin1();
    userpasswd_char = ba.data();

    //QTest::qSleep(1000);
    QString str = QLatin1String(buff);
    qDebug() << qPrintable(str.trimmed());  //trimmed移除字符串两端的换行或空白,qPrintable移除引号
}
