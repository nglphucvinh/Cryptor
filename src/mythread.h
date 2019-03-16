#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QProgressBar>
#include <QLabel>

class mythread : public QThread
{
public:
    mythread(int t, bool b, const char *p, const char *out, const char *key,
             int e, QProgressBar *qb, QLabel *la);
    void run();
    bool Stop;
    int type;
    const char *path;
    const char *out_path;
    const char *key_path;
    int isEncrypt;
    QProgressBar *bar;
    QLabel *label;
};

#endif // MYTHREAD_H
