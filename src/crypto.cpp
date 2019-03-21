#include "crypto.h"
#include <QMutex>
#include "qdir.h"
#include "qstring.h"
#include "qbytearray.h"
#include "qtextstream.h"
#include "qdebug.h"
#include "mythread.h"

void progressBar_show(QProgressBar *bar, int value){
    bar->setValue(value);
}

void qstring_to_uchar(QString s, unsigned char **c){
    struct stat st;

    QString _in_dir = QDir::currentPath() + "/_in.txt";
    QByteArray _in_arr = _in_dir.toLatin1();
    unsigned char *__in_dir = (unsigned char*) _in_arr.data();

    QFile f(_in_dir);
    if(f.open(QIODevice::ReadWrite)){
       QTextStream stream(&f);
       stream << s <<endl;
    }
    f.close();

    stat((const char*) __in_dir, &st);
    unsigned int in_size = st.st_size;
    unsigned char *in_dir = (unsigned char*) malloc (in_size*sizeof(unsigned char));

    FILE *fp = fopen((const char*) __in_dir, "r");
    for(unsigned int i = 0; i < in_size; i++){
        in_dir[i] = fgetc(fp);
    }
    in_dir[in_size-1] = '\0';
    fclose(fp);
    f.remove();

    *c = in_dir;
}

int hash(const char *path, int type){
    FILE *fp;
    struct stat st;

    fp = fopen(path, "r");
    if(fp == NULL){
        return -1;
    }

    stat(path, &st);
    unsigned int size = st.st_size;
    unsigned char in[size];
    for(unsigned int i = 0; i < size; i++){
        in[i] = fgetc(fp);
    }
    fclose(fp);

    if(type){
        return md5_hash(in, size);
    }else{
        return sha1_hash(in, size);
    }
}

int md5_hash(unsigned char *in, unsigned int size){
    unsigned char out[16];

    QString _dir = QDir::currentPath() + "/_hash.txt";
    QByteArray _dir_arr = _dir.toLatin1();
    unsigned char *dir = (unsigned char*) _dir_arr.data();

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, in, size);
    MD5_Final(out, &ctx);
    FILE *fp = fopen((const char*) dir, "w");
    for(int i = 0; i < 16; i++){
        fprintf(fp, "%02x", out[i]);
    }
    fclose(fp);
    return 1;
}

int sha1_hash(unsigned char *in, unsigned int size){
    unsigned char out[20];

    QString _dir = QDir::currentPath() + "/_hash.txt";
    QByteArray _dir_arr = _dir.toLatin1();
    unsigned char *dir = (unsigned char*) _dir_arr.data();

    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, in, size);
    SHA1_Final(out, &ctx);
    FILE *fp = fopen((const char*) dir, "w");
    for(int i = 0; i < 20; i++){
        fprintf(fp, "%02x", out[i]);
    }
    fclose(fp);
    return 1;
}

void rsa_return(int value, mythread *mthread){
    QString str;
    switch (value) {
    case -1:
        str = QString("Key file does not exist !!!");
        break;
    case -2:
        str = QString("Cannot create EVP_Key !!!");
        break;
    case -3:
        str = QString("Key file is not a RSA key file !!!");
        break;
    case -4:
        str = QString("Input file does not exist !!!");
        break;
    case -5:
        str = QString("Output directory does not exist !!!");
        break;
    case -6:
        str = QString("Failed to encrypt !!!");
        break;
    case -7:
        str = QString("Invalid input size !!!");
        break;
    case -8:
        str = QString("Failed to decrypt !!!");
        break;
    case 0:
        str = QString("Process is cancelled !!!");
        break;
    case 1:
        str = QString("Successful: ") + mthread->time;
        break;
    default:
        break;
    }
    emit mthread->status(str);
}

int rsa(const char *path, const char *out_path, const char *key_path, int isPublic, mythread *mthread){
    clock_t start = clock();
    FILE *fp;
    struct stat st;
    RSA *rsa ;

    fp = fopen(key_path, "r");
    if(fp == NULL){
        return -1;                              //open key file error
    }

    EVP_PKEY* evp_key;

    if(isPublic){
        evp_key = PEM_read_PUBKEY(fp, NULL, NULL, NULL);

    }else{
        evp_key = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
    }
    if(evp_key == NULL){
        return -2;
    }
    rsa = EVP_PKEY_get1_RSA(evp_key);
    if(rsa == NULL){
        return -3;                              //not a rsa key
    }
    fclose(fp);

    unsigned int input_size;
    fp = fopen(path, "r");
    if(fp == NULL){
        return -4;                              //open input file error
    }
    stat(path, &st);
    input_size = st.st_size;
    unsigned char in[input_size];
    for(unsigned int i = 0; i < input_size; i++){
        in[i] = (unsigned char) fgetc(fp);
    }
    fclose(fp);
    fp = NULL;
    fp = fopen(out_path,"w");
    if(fp == NULL){
        return -5;                              //output dir not exist
    }

    if(isPublic){
       return rsa_encrypt(&in[0], input_size, rsa, fp, mthread, start);     //public encrypt
    }else{
       return rsa_decrypt(&in[0], input_size, rsa, fp, mthread, start);     //private decrypt
    }
}

int rsa_encrypt(unsigned char *in, unsigned int in_length, RSA *rsa, FILE *fp, mythread *mthread, clock_t start){
    int block_size = RSA_size(rsa) - 48;
    int block_num = (in_length % block_size == 0) ? in_length/block_size: in_length/block_size + 1;
    for(int i = 0; i < block_num; i++){
        QMutex mutex;
        mutex.lock();
        if(mthread->Stop){
            fclose(fp);
            emit mthread->progress(0);
            return 0;
        }

        int flen = block_size;
        unsigned char temp[RSA_size(rsa)];

        if((i+1) == block_num){
            flen = in_length - block_size*i;
        }
        int en_length = RSA_public_encrypt(flen, in + i*block_size, &temp[0], rsa, RSA_PKCS1_OAEP_PADDING);
        if(en_length == -1)
            return -6;              //fail to encrypt
        for(int j = 0; j < en_length; j++){
            fputc(temp[j], fp);
        }

        mutex.unlock();

        int k = 100*(i+1)/block_num;
        emit mthread->progress(k);
    }

    clock_t end = clock();
    double t = ((double)(end-start)/CLOCKS_PER_SEC);
    mthread->time = QString::number(t) + " s";
    fclose(fp);
    return 1;
}

int rsa_decrypt(unsigned char *in, unsigned int in_length, RSA *rsa, FILE *fp, mythread *mthread, clock_t start){
    int block_size = RSA_size(rsa);
    if(in_length % block_size != 0)
        return -7;                  //input size not correct
    int block_num = in_length/block_size;
    for(int i = 0; i < block_num; i++){
        QMutex mutex;
        mutex.lock();
        if(mthread->Stop){
            fclose(fp);
            emit mthread->progress(0);
            return 0;
        }

        unsigned char temp[block_size];
        int de_length = RSA_private_decrypt(block_size, in + i*block_size, &temp[0], rsa, RSA_PKCS1_OAEP_PADDING);
        if(de_length == -1)
            return -8;              //fail to decrypt
        for(int j = 0; j < de_length; j++){
            fputc(temp[j], fp);
        }

        mutex.unlock();

        int k = 100*(i+1)/block_num;
        emit mthread->progress(k);
    }

    clock_t end = clock();
    double t = ((double)(end-start)/CLOCKS_PER_SEC);
    mthread->time = QString::number(t) + " s";
    fclose(fp);
    return 1;
}


void aes_return(int value, mythread *mthread){
    QString str;
    switch (value) {
    case -1:
        str = QString("Key file does not exist !!!");
          break;
    case -2:
        str = QString("Key size is larger than 16 bytes !!!");
          break;
    case -3:
        str = QString("Input file does not exist !!!");
          break;
    case -4:
        str = QString("Output directory does not exist !!!");

        break;
    case -5:
        str = QString("Invalid input size !!!");

        break;
    case 0:
        str = QString("Process is cancelled !!!");

        break;
    case 1:
        str = QString("Successful: ") + mthread->time;

        break;
    default:

        break;
    }
    emit mthread->status(str);
}


int aes(const char *path, const char *out_path, const char *key_path, int isEncrypt, mythread *mthread){
    clock_t start = clock();
    FILE *fp;
    struct stat st;
    AES_KEY key_aes;

    fp = fopen(key_path, "r");
    if(fp == NULL){
        return -1;          //key dir not exist
    }
    stat(key_path, &st);
    if(st.st_size > 16){
        return -2;          //key size > 16 bytes
    }

    unsigned char key[16];
    memset(key, 0, 16);
    for(int i = 0; i < st.st_size; i++){
        key[i] = (unsigned char) fgetc(fp);
    }
    fclose(fp);

    if(isEncrypt){
        AES_set_encrypt_key((const unsigned char*) &key[0], 128, &key_aes);
    }else{
        AES_set_decrypt_key((const unsigned char*) &key[0], 128, &key_aes);
    }

    int in_size;
    fp = fopen(path, "r");
    if(fp == NULL){
        return -3;          //input file not exist
    }
    stat(path, &st);
    in_size = st.st_size;
    unsigned char in[in_size];
    for(int i = 0; i < in_size; i++){
        in[i] = (unsigned char) fgetc(fp);
    }
    fclose(fp);

    fp = fopen(out_path, "w");
    if(fp == NULL){
        return -4;         //output dir not exist
    }

    if(isEncrypt){
        return aes_encrypt(&in[0], in_size, &key_aes, fp, mthread, start);
    }else{
        return aes_decrypt(&in[0], in_size, &key_aes, fp, mthread, start);
    }
}

int aes_encrypt(unsigned char *in, unsigned int in_length, AES_KEY *aes_key, FILE *fp, mythread *mthread, clock_t start){
    unsigned int k = 0;
    unsigned char out[16];
    while(k < in_length){
        QMutex mutex;
        mutex.lock();
        if(mthread->Stop){
            fclose(fp);
            emit mthread->progress(0);
            return 0;
        }
        AES_encrypt((const unsigned char *) &in[k], &out[0], aes_key);
        for(int i = 0; i < 16; i++){
            fputc(out[i], fp);
        }
        k = k + 16;

        mutex.unlock();

        int m = 100*k/in_length + 1;
        emit mthread->progress(m);

    }

    clock_t end = clock();
    double t = ((double)(end-start)/CLOCKS_PER_SEC);
    mthread->time = QString::number(t) + " s";
    fclose(fp);
    return 1;
}

int aes_decrypt(unsigned char *in, unsigned int in_length, AES_KEY *aes_key, FILE *fp, mythread *mthread, clock_t start){
    if(in_length % 16 != 0){
        return -5;          //invalid input size
    }
    unsigned int k = 0;
    unsigned char out[16];
    while(k < in_length){
        QMutex mutex;
        mutex.lock();
        if(mthread->Stop){
            fclose(fp);
            emit mthread->progress(0);
            return 0;
        }

        AES_decrypt((const unsigned char *) &in[k], &out[0], aes_key);
        for(int i = 0; i < 16; i++){
            fputc(out[i], fp);
        }
        k = k + 16;

        mutex.unlock();

        int m = 100*k/in_length + 1;
        emit mthread->progress(m);
    }

    clock_t end = clock();
    double t = ((double)(end-start)/CLOCKS_PER_SEC);
    mthread->time = QString::number(t) + " s";
    fclose(fp);
    return 1;
}

void des_return(int value, mythread *mthread){
    QString str;
    switch (value) {
    case -1:
        str = QString("Key file does not exist !!!");

        break;
    case -2:
        str = QString("Key size is larger than 8 bytes !!!");

        break;
    case -3:
        str = QString("Input file does not exist !!!");

        break;
    case -4:
        str = QString("Output directory does not exist !!!");

        break;
    case -5:
        str = QString("Invalid input size !!!");

        break;
    case 0:
        str = QString("Process is cancelled !!!");

        break;
    case 1:
        str = QString("Successful: ") + mthread->time;

        break;
    default:
        break;
    }

    mthread->status(str);
}

int des(const char *path, const char *out_path, const char *key_path, int isEncrypt, mythread *mthread){
    clock_t start = clock();
    FILE *fp;
    struct stat st;

    fp = fopen(key_path, "r");
    if(fp == NULL){
        return -1;          //key file not exist
    }
    stat(key_path, &st);
    if(st.st_size > 8){
        return -2;          //invalid key size
    }

    unsigned char key[8];
    memset(key, 0, 8);
    for(int i = 0; i < st.st_size; i++){
        key[i] = (unsigned char) fgetc(fp);
    }
    fclose(fp);

    DES_key_schedule key_schedule;
    DES_set_key((DES_cblock *) key, &key_schedule);


    unsigned int in_size;
    fp = fopen(path, "r");
    if(fp == NULL){
        return -3;          //input file not exist
    }
    stat(path, &st);
    in_size = st.st_size;
    unsigned char in[in_size];
    for(unsigned int i = 0; i < in_size; i++){
        in[i] = (unsigned char) fgetc(fp);
    }
    fclose(fp);

    fp = fopen(out_path, "w");
    if(fp == NULL){
        return -4;          //output dir not exist
    }


    if(isEncrypt){
        return des_encrypt(in, in_size, &key_schedule, fp, mthread, start);
    }else{
        return des_decrypt(in, in_size, &key_schedule, fp, mthread, start);
    }
}

int des_encrypt(unsigned char *in, unsigned int in_length, DES_key_schedule *key_schedule, FILE *fp, mythread *mthread, clock_t start){
    unsigned int k = 0;
    unsigned char out[8];
    while(k < in_length){
        QMutex mutex;
        mutex.lock();
        if(mthread->Stop){
            fclose(fp);
            mthread->progress(0);
            return 0;
        }

        DES_ecb_encrypt((DES_cblock *) &in[k], (DES_cblock *) out, key_schedule, DES_ENCRYPT);
        for(int i = 0; i < 8; i++){
            fputc(out[i], fp);
        }
        k = k + 8;

        mutex.unlock();

        int m = 100*k/in_length + 1;
        mthread->progress(m);
    }

    clock_t end = clock();
    double t = ((double)(end-start)/CLOCKS_PER_SEC);
    mthread->time = QString::number(t) + " s";
    fclose(fp);
    return 1;
}

int des_decrypt(unsigned char *in, unsigned int in_length, DES_key_schedule *key_schedule, FILE *fp, mythread *mthread, clock_t start){
    unsigned int k = 0;
    unsigned char out[8];
    if(in_length % 8 != 0){
        return -5;          //invalid input size
    }
    while(k < in_length){
        QMutex mutex;
        mutex.lock();
        if(mthread->Stop){
            fclose(fp);
            mthread->progress(0);
            return 0;
        }

        DES_ecb_encrypt((DES_cblock *) &in[k], (DES_cblock *) out, key_schedule, DES_DECRYPT);
        for(int i = 0; i < 8; i++){
            fputc(out[i], fp);
        }
        k = k + 8;

        mutex.unlock();

        int m = 100*k/in_length + 1;
        mthread->progress(m);
    }

    clock_t end = clock();
    double t = ((double)(end-start)/CLOCKS_PER_SEC);
    mthread->time = QString::number(t) + " s";
    fclose(fp);
    return 1;
}
