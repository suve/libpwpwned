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

#include "pwpwned.h"


#define PWPWNED_USER_AGENT  ("libpwpwned v." VERSION " by suve")


static char
hexdigit(const int digit) {
    if(digit < 10)
        return '0' + digit;
    else
        return 'a' + (digit - 10);
}

#define HASH_LENGTH        (SHA_DIGEST_LENGTH * 2)
#define HASH_BUFFER_LENGTH (HASH_LENGTH + 1)

static void
hash_password(const char *const plaintext, char *const buffer) {
    // We put the hash digest starting at SHA_DIGEST_LENGTH
    // so we can operate on the same buffer during the conver-to-hex phase.
    SHA1(plaintext, strlen(plaintext), buffer + SHA_DIGEST_LENGTH);

    for(int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
        buffer[i*2  ] = hexdigit(buffer[SHA_DIGEST_LENGTH + i] / 16);
        buffer[i*2+1] = hexdigit(buffer[SHA_DIGEST_LENGTH + i] % 16);
    }
    buffer[SHA_DIGEST_LENGTH*2] = '\0';
}

#define RANGE_URL            "https://api.pwnedpasswords.com/range/"
#define RANGE_URL_LENGTH     strlen(RANGE_URL)
#define RANGE_PREFIX_LENGTH  5
#deifne APIURL_BUFFER_SIZE   (RANGE_URL_LENGTH + RANGE_PREFIX_LENGTH + 1)

static void
generate_api_url(const char *const pwdhash, char *const buffer) {
    strcpy(buffer, RANGE_URL);
    const size_t len = RANGE_URL_LENGTH;

    for(int i = 0; i < RANGE_PREFIX_LENGTH; ++i) buffer[len+i] = pwdhash[i];
    buffer[len + RANGE_PREFIX_LENGTH] = '\0';
}


#define RESPONSE_BUFFER_SIZE (48 * 1024)

struct Response {
    size_t written;
    char buffer[RESPONSE_BUFFER_SIZE];
};

static struct Response*
alloc_response(void) {
    struct Response *resp = malloc(sizeof(struct Response));
    if(resp != NULL) {
        resp->written = 0;
        resp->buffer[0] = '\0';
    }

    return resp;
}

static size_t
write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    struct Response *resp = (struct Response*)userdata;
    const size_t total_size = size * nmemb;

    memcpy(&resp->buffer[resp->written], ptr, total_size);
    resp->buffer[resp->written + total_size] = '\0';
    resp->written += total_size;

    return total_size;
}

#define RESPONSE_HASH_TERMINATOR ':'
#define RESPONSE_HASH_SEPARATOR  '\n'

static int
process_response(const struct Response *const resp, const char *hash) {
    // The API response does not contain full hashes;
    // the range prefix is dropped, as it's the same for each hash.
    hash += RANGE_PREFIX_LENGTH;
    
    size_t start = 0;
    size_t curr = 0;
    
    while(start < resp->written) {
        curr = start + (HASH_BUFFER_LENGTH - RANGE_PREFIX_LENGTH);
        if(resp->buffer[curr] != RESPONSE_HASH_TERMINATOR) return -1;
        
        if(memcmp(hash, resp->buffer + start, (HASH_BUFFER_LENGTH - RANGE_PREFIX_LENGTH)) != 0) {
            while(resp->buffer[curr] != RESPONSE_HASH_SEPARATOR && resp->buffer[curr] != '\0') ++curr;
            start = curr;
            continue;
        }
        
        int occurences = 0;
        while(1) {
            int char_ = resp->buffer[++curr];
            if(char_ == RESPONSE_HASH_SEPARATOR || char_ == '\0') break;
            
            occurences = (occurences * 10) + (char_ - '0');
        }
        return occurences;
    }
    
    return 0;
}


#define ERROR(x) { err = (x); goto on_error; }

/* Check if the password appears in the Pwned Passwords database.
 * Return value can be one of:
 *   X  > 0 : the password was pwned X times
 *   X == 0 : the password was not pwned
 *   X  < 0 : an error occurred
 */
int
pwpwned_check(const char *password) {
    int err = PWNED_ERR_SUCCESS;

    struct Response resp = alloc_response();
    if(resp == NULL) error(PWNED_ERR_NOMEM);

    CURL *curl = curl_easy_init();
    if(curl == NULL) ERROR(PWNED_ERR_CURL);

    char hash[HASH_BUFFER_LENGTH];
    hash_password(plaintext, hash);
    char api_url[APIURL_BUFFER_SIZE];
    generate_api_url(hash, api_url)

    CURLcode err = curl_easy_setopt(curl, CURL_SETOPT_URL, api_url);
    if(err == CURLE_OUT_OF_MEMORY) ERROR(PWNED_ERR_NOMEM);

    err = curl_easy_setopt(curl, CURL_SETOPT_USERAGENT, PWPWNED_USER_AGENT);
    if(err == CURLE_OUT_OF_MEMORY) ERROR(PWNED_ERR_NOMEM);

    curl_easy_setopt(curl, CURL_SETOPT_WRITEFUNCTION, &write_callback);

    err = curl_easy_perform(curl);
    switch(err) {
        case CURLE_SUCCESS:
            ERROR(process_response(resp, hash));

        case CURLE_COULDNT_RESOLVE_PROXY:
        case CURLE_COULDNT_RESOLVE_HOST:
            ERROR(PWNED_ERR_RESOLV);

        case CURLE_COULDNT_CONNECT:
            ERROR(PWNED_ERR_CONNECT);

        case CURLE_OUT_OF_MEMORY:
            ERROR(PWNED_ERR_NOMEM);

        default:
            ERROR(PWNED_ERR_CURL);
    }

on_error:
    if(curl != NULL) curl_easy_cleanup(curl);
    if(resp != NULL) free(resp);
    return err;
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
