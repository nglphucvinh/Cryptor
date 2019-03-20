#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include "qstring.h"
#include "qdebug.h"
#include "src/crypto.h"
#include "qdir.h"
#include "qbytearray.h"
#include "qfile.h"
#include "qclipboard.h"
#include "qcombobox.h"
#include "src/mythread.h"
#include "unistd.h"

mythread *mThread = new mythread(1, false, NULL, NULL, NULL, 1, NULL, NULL);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->status->hide();

    ui->input_key->setEnabled(false);
    ui->key_type->setEnabled(false);
    ui->key_sel->setEnabled(false);
    ui->encrypt_button->setEnabled(false);
    ui->decrypt_button->setEnabled(false);

    ui->hash_box->setEnabled(false);
    ui->gen_button->setEnabled(false);
    ui->copy_button->setEnabled(false);
    ui->hash_input->setEnabled(false);
    ui->hash_output->setEnabled(false);

    ui->progressBar->setValue(0);
    ui->progressBar->hide();
    ui->status->setStyleSheet("QLabel {color : red;}");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_radioButton_clicked()
{
    ui->hash_box->setEnabled(false);
    ui->gen_button->setEnabled(false);
    ui->copy_button->setEnabled(false);
    ui->hash_input->setEnabled(false);

    ui->input_key->setEnabled(true);
    ui->key_type->setEnabled(true);
    ui->key_sel->setEnabled(true);
    ui->encrypt_button->setEnabled(true);
    ui->decrypt_button->setEnabled(true);

    ui->progressBar->hide();
    ui->status->hide();

    ui->input_box->setText("/home/bao/qt/btl/a.png");
    ui->input_key->setText("/home/bao/qt/btl/pubkey.pem");
}

void MainWindow::on_radioButton_2_clicked()
{
    ui->input_key->setEnabled(false);
    ui->encrypt->setEnabled(false);
    ui->key_type->setEnabled(false);
    ui->key_sel->setEnabled(false);
    ui->encrypt_button->setEnabled(false);
    ui->decrypt_button->setEnabled(false);

    ui->hash_box->setEnabled(true);
    ui->gen_button->setEnabled(true);
    ui->copy_button->setEnabled(true);
    ui->hash_input->setEnabled(true);

    ui->progressBar->hide();
    ui->status->hide();
}

void MainWindow::on_file_sel_clicked()
{
    QString input_dir = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "/home/",
                "All files (*.*);;Text Files (*.txt)"
                );
    ui->input_box->setText(input_dir);
}

void MainWindow::on_gen_button_clicked()
{
    QByteArray arr = ui->input_box->text().toLatin1();
    unsigned char *in_dir = (unsigned char *) arr.data();

    QString s = ui->hash_input->text();
    QByteArray s_array = s.toLatin1();
    unsigned char *str1 = (unsigned char*) s_array.data();
    int size = strlen((const char*)str1);

    ui->status->setText("");
    ui->status->show();
    ui->hash_output->setText("");

    int isSha1 = !QString::compare(ui->hash_box->currentText(), "SHA-1");

    if(isSha1){
        if(size != 40 && size != 0){
            QString str = QString("Input size must be 20 bytes !!!");
            ui->status->setText(str);
            return;
        }
    }else{
        if(size != 32 && size != 0){
            QString str = QString("Input size must be 16 bytes !!!");
            ui->status->setText(str);
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
            ui->status->setText(str);
            return;
        }
    }

    hash((const char*) in_dir, !isSha1);

    QString hash_dir = QDir::currentPath() + "/_hash.txt";
    QFile f(hash_dir);
    if(!f.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << " Could not open the file for reading";
        return;
    }
    QTextStream in(&f);
    QString text = in.readAll();
    f.close();
    f.remove();

    ui->hash_output->setText(text);

    if(!QString::compare(text, s)){
        QString str = QString("Match !!!");
        ui->status->setText(str);
    }else{
        QString str = QString("Not Match !!!");
        ui->status->setText(str);
    }
}

void MainWindow::on_copy_button_clicked()
{
    QString text = ui->hash_output->text();
    QClipboard *p = QApplication::clipboard();
    p->setText(text);
}

void MainWindow::on_key_sel_clicked()
{
    QString input_dir = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "/home/",
                "All files (*.*);;Text Files (*.txt)"
                );
    ui->input_key->setText(input_dir);
}

void MainWindow::on_encrypt_button_clicked()
{
    ui->status->setText("");
    ui->status->show();

    QString type_crypto = ui->key_type->currentText();
    int key_type;

    if(!QString::compare(type_crypto, "Asymmetric - RSA")){
        key_type = 1;
    }else if(!QString::compare(type_crypto, "Symmetric - DES")){
        key_type = 3;
    }else{
        key_type = 2;
    }

    unsigned char *in_dir, *key_dir;
    qstring_to_uchar(ui->input_box->text(), &in_dir);
    qstring_to_uchar(ui->input_key->text(), &key_dir);

    mThread->type = key_type;
    mThread->Stop = false;
    mThread->path = (const char*) in_dir;
    mThread->out_path = "/home/bao/qt/btl/cipher.txt";
    mThread->key_path = (const char*) key_dir;
    mThread->isEncrypt = 1;
    mThread->bar = ui->progressBar;
    mThread->label = ui->status;

    ui->progressBar->show();
    mThread->start();
}

void MainWindow::on_decrypt_button_clicked()
{
    ui->status->setText("");
    ui->status->show();

    QString type_crypto = ui->key_type->currentText();
    int key_type;

    if(!QString::compare(type_crypto, "Asymmetric - RSA")){
        key_type = 1;
    }else if(!QString::compare(type_crypto, "Symmetric - DES")){
        key_type = 3;
    }else{
        key_type = 2;
    }

    unsigned char *in_dir, *key_dir;
    qstring_to_uchar(ui->input_box->text(), &in_dir);
    qstring_to_uchar(ui->input_key->text(), &key_dir);

    mThread->type = key_type;
    mThread->Stop = false;
    mThread->path = (const char*) in_dir;
    mThread->out_path = "/home/bao/qt/btl/decrypt.png";
    mThread->key_path = (const char*) key_dir;
    mThread->isEncrypt = 0;
    mThread->bar = ui->progressBar;
    mThread->label = ui->status;

    ui->progressBar->show();
    mThread->start();
}

void MainWindow::on_cancel_button_clicked()
{
    mThread->Stop = true;
}
