#include "mythread.h"
#include "crypto.h"
#include <QMutex>

mythread::mythread(int t, bool b, const char *p, const char *out, const char *key, int e, QProgressBar *qb, QLabel *la) : type(t), Stop(b), path(p), out_path(out), key_path(key), isEncrypt(e), bar(qb), label(la)
{

}

void mythread::run()
{
    switch (this->type) {
    case 1:
        rsa_return(rsa(this->path, this->out_path, this->key_path, this->isEncrypt, this->bar, this->Stop), this->label, this->bar);
        break;
    case 2:
        rsa_return(rsa(this->path, this->out_path, this->key_path, this->isEncrypt, this->bar, this->Stop), this->label, this->bar);
        break;
    case 3:
        aes_return(aes(this->path, this->out_path, this->key_path, this->isEncrypt, this->bar, this->Stop), this->label, this->bar);
        break;
    case 4:
        aes_return(aes(this->path, this->out_path, this->key_path, this->isEncrypt, this->bar, this->Stop), this->label, this->bar);
        break;
    case 5:
        des_return(des(this->path, this->out_path, this->key_path, this->isEncrypt, this->bar, this->Stop), this->label, this->bar);
        break;
    case 6:
        des_return(des(this->path, this->out_path, this->key_path, this->isEncrypt, this->bar, this->Stop), this->label, this->bar);
        break;
    default:
        break;
    }
}
