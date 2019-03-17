#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qstring.h"
#include "crypto.h"
#include "qevent.h"
#include "mythread.h"
#include "qclipboard.h"
#include "qlineedit.h"
#include <QDebug>
#include "qstring.h"
#include "qcombobox.h"
#include "qbytearray.h"
#include "qfile.h"

mythread *mThread1 = new mythread(1, false, "/home/bao/qt/gui/a.png", "/home/bao/qt/gui/cipher.txt", "/home/bao/qt/gui/pubkey.pem",
                 1, NULL, NULL);

mythread *mThread2 = new mythread(2, false, "/home/bao/qt/gui/cipher.txt", "/home/bao/qt/gui/decrypt.png", "/home/bao/qt/gui/privkey.pem",
                 0, NULL, NULL);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->label->hide();
    ui->label_2->hide();

    mThread1->bar = ui->progressBar;
    mThread1->label = ui->label;
    mThread2->bar = ui->progressBar;
    mThread2->label = ui->label;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->label->hide();
    ui->progressBar->setValue(0);
    ui->progressBar->show();
    mThread1->Stop = false;
    mThread1->start();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->label->hide();
    ui->progressBar->setValue(0);
    ui->progressBar->show();
    mThread2->Stop = false;
    mThread2->start();
}


void MainWindow::on_pushButton_3_clicked()
{
    mThread1->Stop = true;
    mThread2->Stop = true;
}

void MainWindow::on_pushButton_4_clicked()
{
    QString s = ui->lineEdit->text();
    QByteArray s_array = s.toLatin1();
    unsigned char *str1 = (unsigned char*) s_array.data();
    int size = strlen((const char*)str1);

    ui->label_2->setText("");
    ui->label_2->show();
    ui->lineEdit_2->setText("");

    int isSha1 = !QString::compare(ui->comboBox->currentText(), "sha1");
    if(isSha1){
        if(size != 40 && size != 0){
            QString str = QString("Input size must be 20 bytes !!!");
            ui->label_2->setText(str);
            ui->label_2->setStyleSheet("QLabel {color : red;}");
            return;
        }
    }else{
        if(size != 32 && size != 0){
            QString str = QString("Input size must be 16 bytes !!!");
            ui->label_2->setText(str);
            ui->label_2->setStyleSheet("QLabel {color : red;}");
            return;
        }
    }
    for(int i = 0; i < size; i++){
        int k = str1[i] == '0'|| str1[i] == '1'|| str1[i] == '2'|| str1[i] == '3'|| str1[i] == '4'|| str1[i] == '5'||
            str1[i] == '6'|| str1[i] == '7'|| str1[i] == '8'|| str1[i] == '9'|| str1[i] == 'a'|| str1[i] == 'b'||
            str1[i] == 'c'|| str1[i] == 'd'|| str1[i] == 'e'|| str1[i] == 'f'|| str1[i] == 'A'|| str1[i] == 'B'||
            str1[i] == 'C'|| str1[i] == 'D'|| str1[i] == 'E'|| str1[i] == 'F';
        if(!k){
            QString str = QString("Input size must be hex !!!");
            ui->label_2->setText(str);
            ui->label_2->setStyleSheet("QLabel {color : red;}");
            return;
        }
    }

    hash("/home/bao/qt/gui/a.png", !isSha1);

    QFile f("/home/bao/qt/gui/_hash.txt");
    if(!f.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << " Could not open the file for reading";
        return;
    }
    QTextStream in(&f);
    QString text = in.readAll();
    f.close();
    ui->lineEdit_2->setText(text);

    if(!QString::compare(text, s)){
        QString str = QString("Match !!!");
        ui->label_2->setText(str);
        ui->label_2->setStyleSheet("QLabel {color : green;}");
    }else{
        QString str = QString("Not Match !!!");
        ui->label_2->setText(str);
        ui->label_2->setStyleSheet("QLabel {color : red;}");
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QString text = ui->lineEdit_2->text();
    QClipboard *p = QApplication::clipboard();
    p->setText(text);
}
