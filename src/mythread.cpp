#include "mythread.h"
#include "crypto.h"
#include <QMutex>
#include "qdebug.h"
#include "qobject.h"
mythread::mythread(int t, bool b, const char *p, const char *out, const char *key, int e, QProgressBar *ba, QLabel *lab)
    : type(t), Stop(b), path(p), out_path(out), key_path(key), isEncrypt(e), bar(ba), label(lab)
{

}

void mythread::run()
{
    switch (this->type) {
    case 1:
        QObject::connect(this, SIGNAL(progress(int)), this->bar, SLOT(setValue(int)));
        QObject::connect(this, SIGNAL(status(QString)), this->label, SLOT(setText(QString)));

        rsa_return(rsa(this->path, this->out_path, this->key_path, this->isEncrypt, this), this);

        QObject::disconnect(this, SIGNAL(progress(int)), this->bar, SLOT(setValue(int)));
        QObject::disconnect(this, SIGNAL(status(QString)), this->label, SLOT(setText(QString)));
        break;
    case 2:
        QObject::connect(this, SIGNAL(progress(int)), this->bar, SLOT(setValue(int)));
        QObject::connect(this, SIGNAL(status(QString)), this->label, SLOT(setText(QString)));

        aes_return(aes(this->path, this->out_path, this->key_path, this->isEncrypt, this), this);

        QObject::disconnect(this, SIGNAL(progress(int)), this->bar, SLOT(setValue(int)));
        QObject::disconnect(this, SIGNAL(status(QString)), this->label, SLOT(setText(QString)));
        break;
    case 3:
        QObject::connect(this, SIGNAL(progress(int)), this->bar, SLOT(setValue(int)));
        QObject::connect(this, SIGNAL(status(QString)), this->label, SLOT(setText(QString)));

        des_return(des(this->path, this->out_path, this->key_path, this->isEncrypt, this), this);

        QObject::disconnect(this, SIGNAL(progress(int)), this->bar, SLOT(setValue(int)));
        QObject::disconnect(this, SIGNAL(status(QString)), this->label, SLOT(setText(QString)));
    default:
        break;
    }
}
