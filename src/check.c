/*
 * libpwquality main API code for quality checking
 *
 * See the end of the file for Copyright and License Information
 */
 
#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <crack.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

#include <curl/curl.h>
#include <openssl/sha.h>

#include "pwquality.h"


#define RANGE_PREFIX_LENGTH 5

static char*
generate_api_url(const char *const pwdhash) {
    static char url[48] = { 
        'h','t','t','p','s',':','/','/',
        'a','p','i','.',
        'p','w','n','e','d','p','a','s','s','w','o','r','d','s','.',
        'c','o','m','/',
        'r','a','n','g','e','/',
        '\0'
    };
    static const size_t len = strlen(url);
    
    for(int i = 0; i < RANGE_PREFIX_LENGTH; ++i) url[len+i] = pwdhash[i];
    url[len + RANGE_PREFIX_LENGTH] = '\0';
    
    return url;
}

/* Check if the password appears in the Pwned Passwords database.
 * Return value can be one of:
 *   X  > 0 : the password was pwned X times
 *   X == 0 : the password was not pwned
 *   X  < 0 : an error occurred
 */
int
pwpwned_check(
    const char *password,
    void **auxerror
) {
    CURL *curl = curl_easy_init();
    if(curl == NULL) return PWNED_ERR_CURL;
}

/*
 * Copyright (c) Cristian Gafton <gafton@redhat.com>, 1996.
 *                                              All rights reserved
 * Copyright (c) Red Hat, Inc, 2011, 2015
 * Copyright (c) Tomas Mraz <tm@t8m.info>, 2011, 2015
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, and the entire permission notice in its entirety,
 *    including the disclaimer of warranties.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * ALTERNATIVELY, this product may be distributed under the terms of
 * the GNU General Public License version 2 or later, in which case the
 * provisions of the GPL are required INSTEAD OF the above restrictions.
 *
 * THIS SOFTWARE IS PROVIDED `AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The following copyright was appended for the long password support
 * added with the libpam 0.58 release:
 *
 * Modificaton Copyright (c) Philip W. Dalrymple III <pwd@mdtsoft.com>
 *       1997. All rights reserved
 *
 * THE MODIFICATION THAT PROVIDES SUPPORT FOR LONG PASSWORD TYPE CHECKING TO
 * THIS SOFTWARE IS PROVIDED `AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
