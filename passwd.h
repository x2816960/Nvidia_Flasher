#ifndef PASSWD_H
#define PASSWD_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class Passwd;
}

class Passwd : public QDialog
{
    Q_OBJECT

public:
    explicit Passwd(QWidget *parent = 0);
    ~Passwd();
    QString userpasswd;
    char *userpasswd_char;
    char *shellcmd(char* cmd, char* buff, int size);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Passwd *ui;
};

#endif // PASSWD_H
