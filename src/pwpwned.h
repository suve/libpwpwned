/*
 * libpwpwned main API code header
 *
 * Copyright (c) Red Hat, Inc, 2011
 * Copyright (c) Tomas Mraz <tm@t8m.info>, 2011
 *
 * See the end of the file for the License Information
 */

#ifndef PWPWNED_H
#define PWPWNED_H

#ifdef __cplusplus
extern "C" {
#endif

#define PWNED_ERR_SUCCESS   0 /* implicit, not used in the library code */
#define PWNED_ERR_NOMEM    -1
#define PWNED_ERR_CURL     -2
#define PWNED_ERR_RESOLV   -3
#define PWNED_ERR_CONNECT  -4
#define PWNED_ERR_APIERR   -5

/* Check the password according to the settings.
 * It returns either score <0-100>, negative error number,
 * and possibly also auxiliary error information that must be
 * passed into pwquality_strerror() function.
 * The old password is optional and can be NULL.
 * The user is used for checking the password against user name
 * and potentially other passwd information and can be NULL.
 * The auxerror can be NULL - in that case the auxiliary error information
 * is not returned.
 * Not passing the *auxerror into pwquality_strerror() can lead to memory leaks.
 * The score depends on PWQ_SETTING_MIN_LENGTH. If it is set higher,
 * the score for the same passwords will be lower. */ 
int
pwpwned_check(const char *password, void **auxerror);

/* Translate the error code and auxiliary message into a localized
 * text message.
 * If buf is NULL it uses an internal static buffer which
 * makes the function non-reentrant in that case.
 * The returned pointer is not guaranteed to point to the buf. */
const char *
pwquality_strerror(char *buf, size_t len, int errcode, void *auxerror);

#ifdef __cplusplus
}
#endif

#endif /* PWPWNED_H */

/*
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
 */
