#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/mythread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_file_sel_clicked();

    void on_gen_button_clicked();

    void on_copy_button_clicked();

    void on_key_sel_clicked();

    void on_encrypt_button_clicked();

    void on_decrypt_button_clicked();

    void on_cancel_button_clicked();

    void on_input_type_activated(const QString &arg1);

    void on_input_type_2_activated(const QString &arg1);

    void on_key_type_activated(const QString &arg1);

    void on_progressBar_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
