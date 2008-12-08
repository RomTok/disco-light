/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer <SSchwarzer@berlinux-solutions.de>,               *
 *      Matthias Hardt   <MHardt@berlinux-solutions.de>,                   *
 *      Jens Schneider   <pupeider@gmx.de>                                 *
 *      Guido Madaus     <GMadaus@berlinux-solutions.de>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _NO_MMSCRYPT
#include <errno.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/aes.h>

#include "mmstools/mmscrypt.h"
#include "mmstools/mmsfile.h"

unsigned char* MMSCrypt::createUserKey(string keyfile) {
    MMSFile        *file;
    unsigned char  *userKey, *userKeyEnc;
    size_t         numWritten = 0;

    /* generate random key */
    RAND_set_rand_method(RAND_SSLeay());
    RAND_bytes(userKey, EVP_MAX_KEY_LENGTH + EVP_MAX_IV_LENGTH);
    RAND_cleanup();

    userKeyEnc = encrypt(userKey, EVP_MAX_KEY_LENGTH + EVP_MAX_IV_LENGTH, true);

    /* write key to file */
    file = new MMSFile(keyfile, MMSFM_WRITE);
    file->writeBuffer((void*)userKeyEnc, &numWritten, 16, 1);
    delete(file);

    return userKey;
}

unsigned char* MMSCrypt::getUserKey(string keyfile) {
    unsigned char *userKey, *userKeyEnc;
    MMSFile       *file;
    size_t  numRead = 0;

    /* try to open keyfile for reading                   */
    /* if it fails and the filename differs from default */
    /* try the default file                              */
    file = new MMSFile(keyfile);
    if((file->getLastError() != 0) &&
       (keyfile != MMSCRYPT_DEFAULT_KEY_FILENAME))
       file = new MMSFile(MMSCRYPT_DEFAULT_KEY_FILENAME);

    switch(file->getLastError()) {
        case 0 :
            file->readBufferEx((void**)&userKeyEnc, &numRead);
            userKey = decrypt(userKeyEnc, EVP_MAX_KEY_LENGTH + EVP_MAX_IV_LENGTH, true);
            delete(file);
            break;
        case ENOENT :
            delete(file);
            userKey = createUserKey(file->getName());
            break;
        default :
            delete(file);
            throw new MMSCryptError(0, "file " + keyfile + " could not be opened (" + strerror(file->getLastError()) + ")");
    }

    return userKey;
}

MMSCrypt::MMSCrypt(string keyfile) {
    unsigned char mmskey[] = {0x25, 0x04, 0x19, 0x79, 0xaf, 0xfe, 0x1a, 0x3d};
    unsigned char mmsiv[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char *userKey = NULL;

    /* initialise our private cipher context */
    EVP_CIPHER_CTX_init(&mmsCtx);
    EVP_EncryptInit_ex(&mmsCtx, EVP_aes_128_cbc(), 0, mmskey, mmsiv);

    /* initialise user cipher context */
    userKey = getUserKey(keyfile);
    EVP_CIPHER_CTX_init(&userCtx);
    EVP_EncryptInit_ex(&userCtx, EVP_aes_128_cbc(), 0, &userKey[0], &userKey[EVP_MAX_KEY_LENGTH - 1]);

    /* free memory */
    free(userKey);
}

MMSCrypt::~MMSCrypt() {
    EVP_CIPHER_CTX_cleanup(&mmsCtx);
    EVP_CIPHER_CTX_cleanup(&userCtx);
}

unsigned char* MMSCrypt::encrypt(unsigned char *in, unsigned int size, bool useMMSCtx) {
    unsigned char  *out;
    int            inl, tmp, ol = 0;
    EVP_CIPHER_CTX *ctx;

    ((size == 0) ? inl = strlen((char*)in) : inl = size);
    (useMMSCtx ? ctx = &mmsCtx : ctx = &userCtx);

    if(!(out = (unsigned char*)malloc(inl + EVP_CIPHER_CTX_block_size(ctx))))
        throw new MMSCryptError(0, "not enough memory available");

    for(int i = 0; i < inl / 128; i++) {
        if(!EVP_EncryptUpdate(ctx, &out[ol], &tmp, &in[ol], 128))
            throw new MMSCryptError(0, "error while encrypting data");
        ol += tmp;
    }

    if(inl % 128) {
        if(!EVP_EncryptUpdate(ctx, &out[ol], &tmp, &in[ol], inl % 128))
            throw new MMSCryptError(0, "error while encrypting data");
        ol += tmp;
    }

    if(!EVP_EncryptFinal_ex(ctx, &out[ol], &tmp))
        throw new MMSCryptError(0, "error while encrypting data");

    return out;
}

unsigned char* MMSCrypt::decrypt(unsigned char *in, unsigned int size, bool useMMSCtx) {
    unsigned char  *out;
    int            inl, ol;
    EVP_CIPHER_CTX *ctx;

    ((size == 0) ? inl = strlen((char*)in) : inl = size);
    (useMMSCtx ? ctx = &mmsCtx : ctx = &userCtx);

    if(!(out = (unsigned char*)malloc(inl + EVP_CIPHER_CTX_block_size(ctx) + 1)))
        throw new MMSCryptError(0, "not enough memory available");

    EVP_DecryptUpdate(ctx, out, &ol, in, inl);

    /* nothing to decrypt */
    if(!ol) {
        free(out);
        return 0;
    }

    /* null-terminate output */
    out[ol] = 0;
    return out;
}

#endif /* _NO_MMSCRYPT */
