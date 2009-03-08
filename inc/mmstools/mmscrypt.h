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
