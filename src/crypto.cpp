#include "crypto.h"

void progressBar_show(QProgressBar *bar, int value){
    bar->setValue(value);
}

void rsa_return(int value, QLabel *label, QProgressBar *bar){
    QString str;
    label->show();
    switch (value) {
    case -1:
        str = QString("Key file does not exist !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -2:
        str = QString("Cannot create EVP_Key !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -3:
        str = QString("Key file is not a RSA key file !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -4:
        str = QString("Input file does not exist !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -5:
        str = QString("Output directory does not exist !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -6:
        str = QString("Failed to encrypt !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -7:
        str = QString("Invalid input size !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -8:
        str = QString("Failed to decrypt !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case 1:
        str = QString("Successful !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : blue;}");
        break;
    default:
        bar->hide();
        label->hide();
        break;
    }
}

int rsa(const char *path, const char *out_path, const char *key_path, int isPublic, QProgressBar *bar){
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
       return rsa_encrypt(&in[0], input_size, rsa, fp, bar);     //public encrypt
    }else{
       return rsa_decrypt(&in[0], input_size, rsa, fp, bar);     //private decrypt
    }
}

int rsa_encrypt(unsigned char *in, unsigned int in_length, RSA *rsa, FILE *fp, QProgressBar *bar){
    int block_size = RSA_size(rsa) - 48;
    int block_num = (in_length % block_size == 0) ? in_length/block_size: in_length/block_size + 1;
    for(int i = 0; i < block_num; i++){
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
        progressBar_show(bar, 100*(i+1)/block_num);
    }
    fclose(fp);
    return 1;
}

int rsa_decrypt(unsigned char *in, unsigned int in_length, RSA *rsa, FILE *fp, QProgressBar *bar){
    int block_size = RSA_size(rsa);
    if(in_length % block_size != 0)
        return -7;                  //input size not correct
    int block_num = in_length/block_size;
    for(int i = 0; i < block_num; i++){
        unsigned char temp[block_size];
        int de_length = RSA_private_decrypt(block_size, in + i*block_size, &temp[0], rsa, RSA_PKCS1_OAEP_PADDING);
        if(de_length == -1)
            return -8;              //fail to decrypt
        for(int j = 0; j < de_length; j++){
            fputc(temp[j], fp);
        }
        progressBar_show(bar, 100*(i+1)/block_num);
    }
    fclose(fp);
    return 1;
}


void aes_return(int value, QLabel *label, QProgressBar *bar){
    QString str;
    label->show();
    switch (value) {
    case -1:
        str = QString("Key file does not exist !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -2:
        str = QString("Key size is larger than 16 bytes !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -3:
        str = QString("Input file does not exist !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -4:
        str = QString("Output directory does not exist !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -5:
        str = QString("Invalid input size !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case 1:
        str = QString("Successful !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : blue;}");
        break;
    default:
        bar->hide();
        label->hide();
        break;
    }
}


int aes(const char *path, const char *out_path, const char *key_path, int isEncrypt, QProgressBar *bar){
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
        return aes_encrypt(&in[0], in_size, &key_aes, fp, bar);
    }else{
        return aes_decrypt(&in[0], in_size, &key_aes, fp, bar);
    }
}

int aes_encrypt(unsigned char *in, unsigned int in_length, AES_KEY *aes_key, FILE *fp, QProgressBar *bar){
    unsigned int k = 0;
    unsigned char out[16];
    while(k < in_length){
        AES_encrypt((const unsigned char *) &in[k], &out[0], aes_key);
        for(int i = 0; i < 16; i++){
            fputc(out[i], fp);
        }
        progressBar_show(bar, 100*k/in_length + 1);
        k = k + 16;
    }
    fclose(fp);
    return 1;
}

int aes_decrypt(unsigned char *in, unsigned int in_length, AES_KEY *aes_key, FILE *fp, QProgressBar *bar){
    if(in_length % 16 != 0){
        return -5;          //invalid input size
    }
    unsigned int k = 0;
    unsigned char out[16];
    while(k < in_length){
        AES_decrypt((const unsigned char *) &in[k], &out[0], aes_key);
        for(int i = 0; i < 16; i++){
            fputc(out[i], fp);
        }
        progressBar_show(bar, 100*k/in_length + 1);
        k = k + 16;
    }
    fclose(fp);
    return 1;
}

void des_return(int value, QLabel *label, QProgressBar *bar){
    QString str;
    label->show();
    switch (value) {
    case -1:
        str = QString("Key file does not exist !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -2:
        str = QString("Key size is larger than 8 bytes !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -3:
        str = QString("Input file does not exist !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -4:
        str = QString("Output directory does not exist !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case -5:
        str = QString("Invalid input size !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : red;}");
        bar->hide();
        break;
    case 1:
        str = QString("Successful !!!");
        label->setText(str);
        label->setStyleSheet("QLabel {color : blue;}");
        break;
    default:
        bar->hide();
        label->hide();
        break;
    }
}

int des(const char *path, const char *out_path, const char *key_path, int isEncrypt, QProgressBar *bar){
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
        return des_encrypt(in, in_size, &key_schedule, fp, bar);
    }else{
        return des_decrypt(in, in_size, &key_schedule, fp, bar);
    }
}

int des_encrypt(unsigned char *in, unsigned int in_length, DES_key_schedule *key_schedule, FILE *fp, QProgressBar *bar){
    unsigned int k = 0;
    unsigned char out[8];
    while(k < in_length){
        DES_ecb_encrypt((DES_cblock *) &in[k], (DES_cblock *) out, key_schedule, DES_ENCRYPT);
        for(int i = 0; i < 8; i++){
            fputc(out[i], fp);
        }
        progressBar_show(bar, 100*k/in_length + 1);
        k = k + 8;
    }
    fclose(fp);

    return 1;
}

int des_decrypt(unsigned char *in, unsigned int in_length, DES_key_schedule *key_schedule, FILE *fp, QProgressBar *bar){
    unsigned int k = 0;
    unsigned char out[8];
    if(in_length % 8 != 0){
        return -5;          //invalid input size
    }
    while(k < in_length){
        DES_ecb_encrypt((DES_cblock *) &in[k], (DES_cblock *) out, key_schedule, DES_DECRYPT);
        for(int i = 0; i < 8; i++){
            fputc(out[i], fp);
        }
        progressBar_show(bar, 100*k/in_length + 1);
        k = k + 8;
    }
    fclose(fp);

    return 1;
}
