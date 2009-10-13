/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 Berlinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <SSchwarzer@berlinux-solutions.de>,             *
 *      Matthias Hardt     <MHardt@berlinux-solutions.de>,                 *
 *      Jens Schneider     <pupeider@gmx.de>                               *
 *      Guido Madaus       <GMadaus@berlinux-solutions.de>                 *
 *      Patrick Helterhoff <PHelterhoff@berlinux-solutions.de>             *
 *      René Bählkow       <RBaehlkow@berlinux-solutions.de>               *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 *   Boston, MA 02110-1301 USA                                             *
 ***************************************************************************/

#ifdef __HAVE_MMSCRYPT__
#ifndef MMSCRYPT_H_
#define MMSCRYPT_H_

#include <openssl/evp.h>
#include "mmstools/mmserror.h"

MMS_CREATEERROR(MMSCryptError);

#define MMSCRYPT_DEFAULT_KEY_FILENAME "./.key"

class MMSCrypt {
    private:
        EVP_CIPHER_CTX mmsCtx, userCtx;

        unsigned char* createUserKey(string keyfile);
        unsigned char* getUserKey(string keyfile);

    public:
        MMSCrypt(string keyfile = MMSCRYPT_DEFAULT_KEY_FILENAME);
        ~MMSCrypt();

        unsigned char* encrypt(unsigned char *in, unsigned int size = 0, bool useMMSCtx = false);
        unsigned char* decrypt(unsigned char *in, unsigned int size = 0, bool useMMSCtx = false);
};

#endif /* MMSCRYPT_H_ */
#endif /* __HAVE_MMSCRYPT__ */
