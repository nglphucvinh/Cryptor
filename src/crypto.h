#ifndef CRYPTO_H
#define CRYPTO_H

#include <string.h>
#include <stdio.h>
#include <openssl/des.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <QProgressBar>
#include <QString>
#include <QColor>
#include <QLabel>
#include "mythread.h"
#include "time.h"

void progressBar_show(QProgressBar *bar, int value);

void qstring_to_uchar(QString s, unsigned char **c);

int hash(const char *path, int type);
int md5_hash(unsigned char *in, unsigned int size);
int sha1_hash(unsigned char *in, unsigned int size);

void rsa_return(int value, mythread *mthread);
void aes_return(int value, mythread *mthread);
void des_return(int value, mythread *mthread);

int aes(const char *path, const char *out_path, const char *key_path, int isEncrypt, mythread *mthread);
int aes_encrypt(unsigned char *in, unsigned int in_length, AES_KEY *aes_key, FILE *fp, mythread *mthread, clock_t start);
int aes_decrypt(unsigned char *in, unsigned int in_length, AES_KEY *aes_key, FILE *fp, mythread *mthread, clock_t start);

int rsa(const char *path, const char *out_path, const char *key_path, int isPublic, mythread *mthread);
int rsa_encrypt(unsigned char *in, unsigned int in_length, RSA *rsa, FILE *fp, mythread *mthread, clock_t start);
int rsa_decrypt(unsigned char *in, unsigned int in_length, RSA *rsa, FILE *fp, mythread *mthread, clock_t start);


int des(const char *path, const char *out_path, const char *key_path, int isEncrypt, mythread *mthread);
int des_encrypt(unsigned char *in, unsigned int in_length, DES_key_schedule *key_schedule, FILE *fp, mythread *mthread, clock_t start);
int des_decrypt(unsigned char *in, unsigned int in_length, DES_key_schedule *key_schedule, FILE *fp, mythread *mthread, clock_t start);

#endif // CRYPTO_H
