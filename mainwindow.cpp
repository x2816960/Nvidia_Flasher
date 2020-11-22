#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>
#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QDir>

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
    confirmFile(sFile);        //确认有没有配置文件.nvidia/.config有则读取，没有则新建
    QFile file(sFile);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream in(&file);    //获取配置文件数据
    QString line = in.readLine();
    qDebug() << line;
    if(line.contains("Jetson nano emmc"))
    {
        ui->actionJetson_nano_B01_EMMC->setChecked(true);
        ui->actionJetson_nano_B01_Sdcard->setChecked(false);
        ui->actionJetson_nx_EMMC->setChecked(false);
        ui->actionJetson_nx_Sdcard->setChecked(false);
        ui->label_board->setText("Nano B01\nEMMC");
        board_type = 0;
    } else if(line.contains("Jetson nano sdcard")){
        ui->actionJetson_nano_B01_EMMC->setChecked(false);
        ui->actionJetson_nano_B01_Sdcard->setChecked(true);
        ui->actionJetson_nx_EMMC->setChecked(false);
        ui->actionJetson_nx_Sdcard->setChecked(false);
        ui->label_board->setText("Nano B01\nSdcard");
        board_type = 1;
    } else if(line.contains("Jetson nx emmc")) {
        ui->actionJetson_nano_B01_EMMC->setChecked(false);
        ui->actionJetson_nano_B01_Sdcard->setChecked(false);
        ui->actionJetson_nx_EMMC->setChecked(true);
        ui->actionJetson_nx_Sdcard->setChecked(false);
        ui->pushButton_3->setEnabled(false);           //Jetson nx未能研究出直接烧录内核的方式，故烧录内核的按钮禁用
        ui->label_board->setText("   NX \nEMMC");
        board_type = 2;
    } else if(line.contains("Jetson nx sdcard")) {
        ui->actionJetson_nano_B01_EMMC->setChecked(false);
        ui->actionJetson_nano_B01_Sdcard->setChecked(false);
        ui->actionJetson_nx_EMMC->setChecked(false);
        ui->actionJetson_nx_Sdcard->setChecked(true);
        ui->pushButton_3->setEnabled(false);           //Jetson nx未能研究出直接烧录内核的方式，故烧录内核的按钮禁用
        ui->label_board->setText("   NX \nSdcard");
        board_type = 3;
    }
    else
    {
        ui->actionJetson_nano_B01_EMMC->setChecked(true);
        ui->actionJetson_nano_B01_Sdcard->setChecked(false);
        ui->actionJetson_nx_EMMC->setChecked(false);
        ui->actionJetson_nx_Sdcard->setChecked(false);
        ui->label_board->setText("Nano B01\nEMMC");
        //deleteOnelineInFile(0,sFile);
        file.write("Jetson nano emmc\n");
        board_type = 0;
    }
    file.close();
    passwd = new Passwd(this);
    passwd->setModal(true);
    passwd->show();
}

bool MainWindow::confirmFile(const QString sFile)
{
    QString sAppPath = QCoreApplication::applicationDirPath();
    QString sFilePath = sAppPath+"/"+sFile;
    QString sPath = sFilePath.left(sFilePath.lastIndexOf('/'));
    //文件是否存在
    if(QFile::exists(sFilePath))
        return true;//文件存在，则确认可以操作
    //文件不存在，先看路径是否存在
    QDir dir(sPath);
    if(!dir.exists(sPath))
    {
        //路径不存在，则创建路径
        if(!dir.mkdir(sPath))
            return false;//路径创建失败就没办法了，通常不会失败
    }
    //到这里需确认路径已经ok，且文件不存在，创建文件
    QFile file(sFilePath);
    //当以Write方式打开时，若文件不存在则自动创建
    if(!file.open(QIODevice::WriteOnly))
        return false;
    file.close();
    return true;
}

//如果是5行，nNum最大为4
void MainWindow::DeleteOneline(int nNum, QString &strall)
{
    int nLine=0;
    int Index=0;
    //算出行数nLine
    while(Index!=-1)
    {
        Index=strall.indexOf('\n',Index+1);
        nLine++;
    }

    //如果是直接从位置0开始删除\n算一个字符"abc\nme\ndo" \n的index是3要删除3+1个字符，即index+1个
    if(nNum==0)
    {
        int nIndex=strall.indexOf('\n');
        strall.remove(0,nIndex+1);
    }
    else
    {
        int nTemp=nNum;
        int nIndex=0,nIndex2=0;
        while(nTemp--)
        {
            //
            nIndex=strall.indexOf('\n',nIndex+1);//这里会更新nIndex
            if(nIndex!=-1)//说明是有效的
            {
                nIndex2=strall.indexOf('\n',nIndex+1);
            }
        }
        //删除的行不是最后一行（从nIndex+1这个位置起nIndex2-nIndex个字符全部抹去）
        if(nNum<nLine-1)
        {
            strall.remove(nIndex+1, nIndex2-nIndex);//不用减一
        }
        //删除的是最后一行（从nIndex起始len-nIndex个字符全抹去）
        //不能从nIndex+1处开始，
        else if(nNum==nLine-1)
        {
            int len=strall.length();
            strall.remove(nIndex,len-nIndex);
        }
        else
        {

        }
    }
}

void MainWindow::deleteOnelineInFile(int nNumLine, QString &filename)
{
    QString strall;
    QFile readfile(filename);
    if(readfile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&readfile);
        strall=stream.readAll();
    }
    readfile.close();
    DeleteOneline(nNumLine, strall);

    QFile writefile(filename);
    if(writefile.open(QIODevice::WriteOnly))
    {
        QTextStream wrtstream(&writefile);
        wrtstream<<strall;
    }
    writefile.close();
}

void MainWindow::button_enable()
{
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
}

void MainWindow::button_disable()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
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
        }else if(outStr.contains("\r",Qt::CaseSensitive) == true){
            QStringList list = outStr.split('\r',QString::SkipEmptyParts);

            for(int i = 0; i< list.size();++i)
            {
                QString tmp = list.at(i);

                if(ready_cnt != 0 && tmp.contains("%",Qt::CaseSensitive) == true)
                {
                    txtcur.movePosition(QTextCursor::StartOfLine,QTextCursor::KeepAnchor);
                    txtcur.removeSelectedText();
                }

                ui->textEdit->moveCursor(QTextCursor::End);  //此方式追加是不换行追加
                ui->textEdit->insertPlainText(tmp);       //（在读写指针的位置处插入）
                ui->textEdit->setTextCursor(txtcur);         //
                ready_cnt++;
                //qDebug()<<"tmp ="<< tmp << "ready_cnt =" << ready_cnt;
            }
        }
    }else{
        if(ready_cnt != 0)
        {
            txtcur.movePosition(QTextCursor::StartOfLine,QTextCursor::KeepAnchor);
            txtcur.removeSelectedText();
        }
        ui->textEdit->moveCursor(QTextCursor::End);  //此方式追加是不换行追加
        ui->textEdit->insertPlainText(outStr);       //（在读写指针的位置处插入）
        ui->textEdit->setTextCursor(txtcur);
        ready_cnt = 0;

        if(outStr.contains("successfully") == true)  //烧录成功，解析
        {
            if(outStr.contains("LNX") == true)       //内核成功
            {
                if(QMessageBox::Ok == QMessageBox::information(this,"Information","内核烧录成功!",QMessageBox::Ok))
                {
                    ui->textEdit->clear();      //点击OK后清屏
                    cmd->kill();                //使用完关闭进程
                    button_enable();            //启用所有按钮
                }
            }

            if(outStr.contains("DTB") == true || outStr.contains("kernel-dtb") == true)       //设备树成功
            {
                if(QMessageBox::Ok == QMessageBox::information(this,"Information","设备树烧录成功!",QMessageBox::Ok))
                {
                    ui->textEdit->clear();      //点击OK后清屏
                    cmd->kill();                //使用完关闭进程
                    button_enable();            //启用所有按钮
                }
            }

            if(outStr.contains("t210ref") == true || outStr.contains("t186ref") == true)   //整体固件烧录成功
            {
                if(QMessageBox::Ok == QMessageBox::information(this,"Information","整体固件烧录成功!",QMessageBox::Ok))
                {
                    ui->textEdit->clear();      //点击OK后清屏
                    cmd->kill();                //使用完关闭进程
                    button_enable();            //启用所有按钮
                }
            }
        }

        if(outStr.contains("Error") == true)  //烧录失败，解析
        {
            if(outStr.contains("probing the target") == true)       //板卡未进入烧录模式
            {
                if(QMessageBox::Ok == QMessageBox::information(this,"Information","板卡未进入烧录模式!",QMessageBox::Ok))
                {
                    ui->textEdit->clear();      //点击OK后清屏
                    cmd->kill();                //使用完关闭进程
                    button_enable();            //启用所有按钮
                }
            }

            if(outStr.contains("Return value") == true)       //
            {
                if(QMessageBox::Ok == QMessageBox::information(this,"Information","烧录失败!",QMessageBox::Ok))
                {
                    ui->textEdit->clear();      //点击OK后清屏
                    cmd->kill();                //使用完关闭进程
                    button_enable();            //启用所有按钮
                }
            }
        }
    }
    //ui->textEdit->append(outStr);              //此方法追加内容是换行追加
    //ui->textEdit->append(cmd->readAllStandardOutput().data());
}


void MainWindow::on_pushButton_clicked()         //Update DTB button
{
    cmd->start("bash");
    cmd->waitForStarted();
    if(board_type == 0)                          //Jetson nano B01 EMMC
    {
        cmd->write("sudo -S ./flash.sh -r -k DTB jetson-nano-emmc mmcblk0p1\n");
    }else if (board_type == 1)                   //Jetson nano B01 SDCARD
    {
        cmd->write("sudo -S ./flash.sh -r -k DTB jetson-nano-qspi-sd mmcblk0p1\n");
    }else if (board_type == 2)                   //Jetson nx EMMC
    {
        cmd->write("sudo -S ./flash.sh -r -k kernel-dtb p3449-0000+p3668-0001-qspi-emmc mmcblk0p1\n");
    }else if (board_type == 3)                   //Jetson nx Sdcard
    {
        cmd->write("sudo -S ./flash.sh -r -k kernel-dtb p3509-0000+p3668-0000-qspi-sd mmcblk0p1\n");
    }


    button_disable();      //按钮点击后禁用
}

void MainWindow::on_pushButton_3_clicked()
{
    cmd->start("bash");
    cmd->waitForStarted();
    if(board_type == 0)                          //Jetson nano B01 EMMC
    {
        cmd->write("sudo -S ./flash.sh -r -K kernel/Image -k LNX jetson-nano-emmc mmcblk0p1\n");
    }else if (board_type == 1)                   //Jetson nano B01 SDCARD
    {
        cmd->write("sudo -S ./flash.sh -r -K kernel/Image -k LNX jetson-nano-qspi-sd mmcblk0p1\n");
    }else if (board_type == 2)                   //Jetson nx EMMC

    button_disable();      //按钮点击后禁用
}

void MainWindow::on_pushButton_2_clicked()
{
    cmd->start("bash");
    cmd->waitForStarted();
    if(board_type == 0)                          //Jetson nano B01 EMMC
    {
        cmd->write("sudo -S ./flash.sh jetson-nano-emmc mmcblk0p1\n");
    }else if (board_type == 1)                   //Jetson nano B01 SDCARD
    {
        cmd->write("sudo -S ./flash.sh jetson-nano-qspi-sd mmcblk0p1\n");
    }else if (board_type == 2)                   //Jetson nx EMMC
    {
        cmd->write("sudo -S ./flash.sh p3449-0000+p3668-0001-qspi-emmc mmcblk0p1\n");
    }else if (board_type == 3)                   //Jetson nx Sdcard
    {
        cmd->write("sudo -S ./flash.sh p3509-0000+p3668-0000-qspi-sd mmcblk0p1\n");
    }


    button_disable();      //按钮点击后禁用
}

void MainWindow::on_pushButton_4_clicked()
{
    cmd->start("bash");
    cmd->waitForStarted();
    if(board_type == 0)                          //Jetson nano B01 EMMC
    {
        cmd->write("sudo -S ./flash.sh -r jetson-nano-emmc mmcblk0p1\n");
    }else if (board_type == 1)                   //Jetson nano B01 SDCARD
    {
        cmd->write("sudo -S ./flash.sh -r jetson-nano-qspi-sd mmcblk0p1\n");
    }else if (board_type == 2)                   //Jetson nx EMMC
    {
        cmd->write("sudo -S ./flash.sh -r p3449-0000+p3668-0001-qspi-emmc mmcblk0p1\n");
    }else if (board_type == 3)                   //Jetson nx Sdcard
    {
        cmd->write("sudo -S ./flash.sh -r p3509-0000+p3668-0000-qspi-sd mmcblk0p1\n");
    }

    button_disable();      //按钮点击后禁用
}

void MainWindow::on_actionInformation_triggered()
{
    if(QMessageBox::Ok == QMessageBox::information(this,"Information","APP Version : V2.0.0\nAuthor: XUW <360828046@qq.com>\nRelease time:2020.11.22 +8 11:20",QMessageBox::Ok))
    {

    }
}

void MainWindow::on_actionJetson_nano_B01_EMMC_triggered()
{
    ui->actionJetson_nano_B01_Sdcard->setChecked(false);
    ui->actionJetson_nx_EMMC->setChecked(false);
    ui->actionJetson_nx_Sdcard->setChecked(false);
    ui->pushButton_3->setEnabled(true);
    ui->label_board->setText("Nano B01\nEMMC");

    QFile file(sFile);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    deleteOnelineInFile(0,sFile);
    file.write("Jetson nano emmc\n");
    board_type = 0;
    file.close();
}

void MainWindow::on_actionJetson_nano_B01_Sdcard_triggered()
{
    ui->actionJetson_nano_B01_EMMC->setChecked(false);
    ui->actionJetson_nx_EMMC->setChecked(false);
    ui->actionJetson_nx_Sdcard->setChecked(false);
    ui->pushButton_3->setEnabled(true);
    ui->label_board->setText("Nano B01\nSdcard");

    QFile file(sFile);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    deleteOnelineInFile(0,sFile);
    file.write("Jetson nano sdcard\n");
    board_type = 1;
    file.close();
}

void MainWindow::on_actionJetson_nx_EMMC_triggered()
{
    ui->actionJetson_nano_B01_EMMC->setChecked(false);
    ui->actionJetson_nano_B01_Sdcard->setChecked(false);
    ui->actionJetson_nx_Sdcard->setChecked(false);
    ui->pushButton_3->setEnabled(false);           //Jetson nx未能研究出直接烧录内核的方式，故烧录内核的按钮禁用
    ui->label_board->setText("   Nx\nEMMC");

    QFile file(sFile);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    deleteOnelineInFile(0,sFile);
    file.write("Jetson nx emmc\n");
    board_type = 2;
    file.close();
}

void MainWindow::on_actionJetson_nx_Sdcard_triggered()
{
    ui->actionJetson_nano_B01_EMMC->setChecked(false);
    ui->actionJetson_nano_B01_Sdcard->setChecked(false);
    ui->actionJetson_nx_EMMC->setChecked(false);
    ui->pushButton_3->setEnabled(false);           //Jetson nx未能研究出直接烧录内核的方式，故烧录内核的按钮禁用
    ui->label_board->setText("   Nx\nSdcard");

    QFile file(sFile);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    deleteOnelineInFile(0,sFile);
    file.write("Jetson nx sdcard\n");
    board_type = 3;
    file.close();
}
