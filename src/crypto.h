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
#include <QProgressBar>
#include <QString>
#include <QColor>
#include <QLabel>

void progressBar_show(QProgressBar *bar, int value);

void rsa_return(int value, QLabel *label, QProgressBar *bar);
void aes_return(int value, QLabel *label, QProgressBar *bar);
void des_return(int value, QLabel *label, QProgressBar *bar);

int aes(const char *path, const char *out_path, const char *key_path, int isEncrypt, QProgressBar *bar, bool &mStop);
int aes_encrypt(unsigned char *in, unsigned int in_length, AES_KEY *aes_key, FILE *fp, QProgressBar *bar, bool &mStop);
int aes_decrypt(unsigned char *in, unsigned int in_length, AES_KEY *aes_key, FILE *fp, QProgressBar *bar, bool &mStop);

int rsa(const char *path, const char *out_path, const char *key_path, int isPublic, QProgressBar *bar, bool &mStop);
int rsa_encrypt(unsigned char *in, unsigned int in_length, RSA *rsa, FILE *fp, QProgressBar *bar, bool &mStop);
int rsa_decrypt(unsigned char *in, unsigned int in_length, RSA *rsa, FILE *fp, QProgressBar *bar, bool &mStop);


int des(const char *path, const char *out_path, const char *key_path, int isEncrypt, QProgressBar *bar, bool &mStop);
int des_encrypt(unsigned char *in, unsigned int in_length, DES_key_schedule *key_schedule, FILE *fp, QProgressBar *bar, bool &mStop);
int des_decrypt(unsigned char *in, unsigned int in_length, DES_key_schedule *key_schedule, FILE *fp, QProgressBar *bar, bool &mStop);

#endif // CRYPTO_H
