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
{   // Initialize GUI
    ui->setupUi(this);
    ui->status->hide();

    ui->input_box->setEnabled(false);
    ui->file_sel->setEnabled(false);
    ui->file_sel->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->input_type->setEnabled(false);
    ui->input_type->setStyleSheet("QComboBox {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->input_key->setEnabled(false);
    ui->input_key->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->key_sel->setEnabled(false);
    ui->key_sel->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->key_type->setEnabled(false);
    ui->key_type->setStyleSheet("QComboBox {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->encrypt_button->setEnabled(false);
    ui->encrypt_button->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->decrypt_button->setEnabled(false);
    ui->decrypt_button->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->input_type_2->setEnabled(false);
    ui->input_type_2->setStyleSheet("QComboBox {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");

    ui->hash_box->setEnabled(false);
    ui->hash_box->setStyleSheet("QComboBox {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->gen_button->setEnabled(false);
    ui->gen_button->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->copy_button->setEnabled(false);
    ui->copy_button->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->hash_input->setEnabled(false);
    ui->hash_output->setEnabled(false);

    ui->progressBar->setValue(0);
    ui->progressBar->hide();

    ui->cancel_button->hide();

    ui->status->setStyleSheet("QLabel {color : red;}");
    QObject::connect(mThread, SIGNAL(progress(int)), this, SLOT(change_cancel_button(int)));
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::change_cancel_button(int value)
{
   if(value > 0 && value < 100){
       ui->cancel_button->show();
   }else{
//       ui->status->setStyleSheet("QLabel {color: black}");
       ui->cancel_button->hide();
   }
}

void MainWindow::on_radioButton_clicked()
{
    if(!QString::compare(ui->input_type->currentText(), "File")){
        ui->file_sel->setEnabled(true);
        ui->file_sel->setStyleSheet("QPushButton {color: white; border: black; background-color:"
                                    "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}"
                                    "QPushButton:hover {color: white; background-color: "
                                    "qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");
    }else{
        ui->file_sel->setEnabled(false);
        ui->file_sel->setStyleSheet("QPushButton {color:white; border: black; background-color:"
                 "rgb(37, 56, 72)}");
    }

    ui->input_box->setEnabled(true);

    ui->input_type->setEnabled(true);
    ui->input_type->setStyleSheet("QComboBox {color: white; border: black; background-color: "
                                "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");
    ui->hash_box->setEnabled(false);
    ui->hash_box->setStyleSheet("QComboBox {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->gen_button->setEnabled(false);
    ui->gen_button->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->copy_button->setEnabled(false);
    ui->copy_button->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->hash_input->setEnabled(false);

    ui->input_key->setEnabled(true);
    ui->key_type->setEnabled(true);
    ui->key_type->setStyleSheet("QComboBox {color: white; border: black; background-color: "
                                "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");
    ui->key_sel->setEnabled(true);
    ui->key_sel->setStyleSheet("QPushButton {color: white; border: black; background-color:"
                                "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}"
                                "QPushButton:hover {color: white; background-color: "
                                "qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");
    ui->encrypt_button->setEnabled(true);
    ui->encrypt_button->setStyleSheet("QPushButton {color: white; border: black; background-color:"
                                "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}"
                                "QPushButton:hover {color: white; background-color: "
                                "qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");
    ui->decrypt_button->setEnabled(true);
    ui->decrypt_button->setStyleSheet("QPushButton {color: white; border: black; background-color:"
                                "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}"
                                "QPushButton:hover {color: white; background-color: "
                                "qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");
    ui->input_type_2->setEnabled(true);
    ui->input_type_2->setStyleSheet("QComboBox {color: white; border: black; background-color: "
                                "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");

    ui->progressBar->hide();
    ui->status->hide();

}

void MainWindow::on_radioButton_2_clicked()
{

    if(!QString::compare(ui->input_type->currentText(), "File")){
        ui->file_sel->setEnabled(true);
        ui->file_sel->setStyleSheet("QPushButton {color: white; border: black; background-color:"
                                    "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}"
                                    "QPushButton:hover {color: white; background-color: "
                                    "qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");
    }else{
        ui->file_sel->setEnabled(false);
        ui->file_sel->setStyleSheet("QPushButton {color:white; border: black; background-color:"
                 "rgb(37, 56, 72)}");
    }

    ui->input_box->setEnabled(true);

    ui->input_type->setEnabled(true);
    ui->input_type->setStyleSheet("QComboBox {color: white; border: black; background-color: "
                                "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");
    ui->encrypt->setEnabled(false);

    ui->input_type_2->setEnabled(false);
    ui->input_type_2->setStyleSheet("QComboBox {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->input_key->setEnabled(false);
    ui->input_key->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->key_sel->setEnabled(false);
    ui->key_sel->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->key_type->setEnabled(false);
    ui->key_type->setStyleSheet("QComboBox {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->encrypt_button->setEnabled(false);
    ui->encrypt_button->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
    ui->decrypt_button->setEnabled(false);
    ui->decrypt_button->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");

    ui->hash_box->setEnabled(true);
    ui->hash_box->setStyleSheet("QComboBox {color: white; border: black; background-color: "
                                "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");
    ui->gen_button->setEnabled(true);
    ui->gen_button->setStyleSheet("QPushButton {color: white; border: black; background-color:"
                                "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}"
                                "QPushButton:hover {color: white; background-color: "
                                "qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");
    ui->copy_button->setEnabled(true);
    ui->copy_button->setStyleSheet("QPushButton {color: white; border: black; background-color:"
                                "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}"
                                "QPushButton:hover {color: white; background-color: "
                                "qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");
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
    if(input_dir != NULL)
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

    ui->status->show();
    ui->status->setText("Copied to Clipboard !!!");
}

void MainWindow::on_key_sel_clicked()
{
    QString input_dir = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "/home/",
                "All files (*.*);;Text Files (*.txt)"
                );
    if(input_dir != NULL)
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

    unsigned char *in_dir, *key_dir, *out_dir;

    if(ui->file_sel->isEnabled()){
        qstring_to_uchar(ui->input_box->text(), &in_dir);
    }else{
        unsigned char *s;
        qstring_to_uchar(ui->input_box->text(), &s);

        QString _in_dir = QDir::currentPath() + "/__in.txt";
        qstring_to_uchar(_in_dir, &in_dir);
        FILE *fp = fopen((const char*) in_dir, "w");
        if(fp == NULL){
            printf("kec\n");
            return;
        }
        for(unsigned int i = 0; i < strlen((const char*) s); i++){
            fputc(s[i], fp);
        }
        fclose(fp);
    }

    if(!QString::compare(ui->key_type->currentText(), "Asymmetric - RSA") && !QString::compare(ui->input_type_2->currentText(), "File")){
        qstring_to_uchar(ui->input_key->text(), &key_dir);
    }else if(!QString::compare(ui->input_type_2->currentText(), "Generate")){
        QString _in_dir = QDir::currentPath() + "/__key.txt";
        qstring_to_uchar(_in_dir, &key_dir);
    }else{
        unsigned char *s;
        qstring_to_uchar(ui->input_key->text(), &s);

        QString _in_dir = QDir::currentPath() + "/__key.txt";
        qstring_to_uchar(_in_dir, &key_dir);
        FILE *fp = fopen((const char*) key_dir, "w");
        if(fp == NULL){
            printf("kec\n");
            return;
        }
        for(unsigned int i = 0; i < strlen((const char*) s); i++){
            fputc(s[i], fp);
        }
        fclose(fp);
    }

    QString _out_dir = QDir::currentPath() + "/cipher.cryptor";
    qstring_to_uchar(_out_dir, &out_dir);

    mThread->type = key_type;
    mThread->Stop = false;
    mThread->path = (const char*) in_dir;
    mThread->out_path = (const char*) out_dir;
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

    unsigned char *in_dir, *key_dir, *out_dir;

    if(ui->file_sel->isEnabled()){
        qstring_to_uchar(ui->input_box->text(), &in_dir);
    }else{
        unsigned char *s;
        qstring_to_uchar(ui->input_box->text(), &s);

        QString _in_dir = QDir::currentPath() + "/__in.txt";
        qstring_to_uchar(_in_dir, &in_dir);
        FILE *fp = fopen((const char*) in_dir, "w");
        if(fp == NULL){
            printf("kec\n");
            return;
        }
        for(unsigned int i = 0; i < strlen((const char*) s); i++){
            fputc(s[i], fp);
        }
        fclose(fp);
    }

    if(!QString::compare(ui->key_type->currentText(), "Asymmetric - RSA") && !QString::compare(ui->input_type_2->currentText(), "File")){
        qstring_to_uchar(ui->input_key->text(), &key_dir);
    }else if(!QString::compare(ui->input_type_2->currentText(), "Generate")){
        QString _in_dir = QDir::currentPath() + "/__key.txt";
        qstring_to_uchar(_in_dir, &key_dir);
    }else{
        unsigned char *s;
        qstring_to_uchar(ui->input_key->text(), &s);

        QString _in_dir = QDir::currentPath() + "/__key.txt";
        qstring_to_uchar(_in_dir, &key_dir);
        FILE *fp = fopen((const char*) key_dir, "w");
        if(fp == NULL){
            printf("kec\n");
            return;
        }
        for(unsigned int i = 0; i < strlen((const char*) s); i++){
            fputc(s[i], fp);
        }
        fclose(fp);
    }

    QString _out_dir = QDir::currentPath() + "/decrypt.txt";
    qstring_to_uchar(_out_dir, &out_dir);

    mThread->type = key_type;
    mThread->Stop = false;
    mThread->path = (const char*) in_dir;
    mThread->out_path = (const char*) out_dir;
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

void MainWindow::on_input_type_activated(const QString &arg1)
{
    if(!QString::compare(arg1, "File")){
        ui->file_sel->setEnabled(true);
        ui->file_sel->setStyleSheet("QPushButton {color: white; border: black; background-color:"
                                    "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}"
                                    "QPushButton:hover {color: white; background-color: "
                                    "qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");
    }else{
        ui->file_sel->setEnabled(false);
        ui->file_sel->setStyleSheet("QPushButton {color:white; border: black; background-color:"
                 "rgb(37, 56, 72)}");
    }
}

void MainWindow::on_input_type_2_activated(const QString &arg1)
{
    if(!QString::compare(arg1, "File")){
        ui->key_sel->setEnabled(true);
        ui->key_sel->setStyleSheet("QPushButton {color: white; border: black; background-color:"
                                    "qlineargradient(spread:pad, x1:1, y1:0.738636, x2:0.975124, y2:0, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}"
                                    "QPushButton:hover {color: white; background-color: "
                                    "qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255))}");
        return;
    }else if(!QString::compare(arg1, "Generate") && QString::compare(ui->key_type->currentText(), "Asymmetric - RSA")){
        unsigned char *in_dir;
        QString _in_dir = QDir::currentPath() + "/__key.txt";
        qstring_to_uchar(_in_dir, &in_dir);

        FILE *fp = fopen((const char*) in_dir, "w");
        if(fp == NULL){
            printf("kec\n");
            return;
        }
        int length = !QString::compare(ui->key_type->currentText(), "Symmetric - DES")? 8: 16;
        for(int i = 0; i < length; i++){
            unsigned char c = (unsigned char) (rand()%93 + 33);
            fputc(c, fp);
        }
        fclose(fp);

        QFile f(_in_dir);
        if(!f.open(QIODevice::ReadOnly))
            return;
        QTextStream instream(&f);
        QString line = instream.readLine();
        ui->input_key->setText(line);
    }
    ui->key_sel->setEnabled(false);
    ui->key_sel->setStyleSheet("QPushButton {color:white; border: black; background-color:"
             "rgb(37, 56, 72)}");
}

void MainWindow::on_key_type_activated(const QString &arg1)
{
    ui->input_key->setText("");
    if(!QString::compare(arg1, "Asymmetric - RSA")){
        ui->input_key->setEnabled(false);
    }else{
        ui->input_key->setEnabled(true);
    }
}

void MainWindow::on_progressBar_valueChanged(int value)
{
    if(value>49){
        ui->progressBar->setStyleSheet("QProgressBar{color: white;}"
                                       "QProgressBar:horizontal {border: 1px gray; border-radius: 3px; background: rgb(238, 238, 236);; padding: 1px; text-align: center;}"
                                       "QProgressBar::chunk{background: "
                                       "qlineargradient(spread:pad, x1:0.542, y1:1, x2:1, y2:1, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255));}");
    }
    else{
        ui->progressBar->setStyleSheet("QProgressBar{color: black;}"
                                       "QProgressBar:horizontal {border: 1px gray; border-radius: 3px; background: rgb(238, 238, 236);; padding: 1px; text-align: center;}"
                                       "QProgressBar::chunk{background: "
                                       "qlineargradient(spread:pad, x1:0.542, y1:1, x2:1, y2:1, stop:0.0742574 rgba(83, 118, 186, 255), stop:1 rgba(106, 229, 175, 255));}");
    }
}
