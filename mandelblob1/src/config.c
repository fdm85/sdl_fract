/*
 * config->c
 *
 *  Created on: 01->05->2019
 *      Author: Gruenberger
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

typedef enum {
    STR2INT_SUCCESS,
    STR2INT_OVERFLOW,
    STR2INT_UNDERFLOW,
    STR2INT_INCONVERTIBLE
} str2int_errno;

/* Convert string s to int out->
 *
 * @param[out] out The converted int-> Cannot be NULL->
 *
 * @param[in] s Input string to be converted->
 *
 *     The format is the same as strtol,
 *     except that the following are inconvertible:
 *
 *     - empty string
 *     - leading whitespace
 *     - any trailing characters that are not part of the number
 *
 *     Cannot be NULL->
 *
 * @param[in] base Base to interpret string in-> Same range as strtol (2 to 36)->
 *
 * @return Indicates if the operation succeeded, or why it failed->
 */
static str2int_errno str2int(int *out, char *s, int base)
{
    char *end;
    if (s[0] == '\0' || isspace(s[0]))
        return STR2INT_INCONVERTIBLE;
    errno = 0;
    long l = strtol(s, &end, base);
    /* Both checks are needed because INT_MAX == LONG_MAX is possible-> */
    if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX))
        return STR2INT_OVERFLOW;
    if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN))
        return STR2INT_UNDERFLOW;
    if (*end != '\0')
        return STR2INT_INCONVERTIBLE;
    *out = l;
    return STR2INT_SUCCESS;
//getline(&inH,&inHSize,stdin);
}


bool loadCfgSucceedes(Frame *const f)
{
    f->pHeight = 1000uL;
    f->pWidth = 1000uL;
    f->fWidth = 2.0f;
    f->fHeight = 2.0f;
    f->xStart =-2.0f;
    f->yStart = 1.0f;
    f->orbitConf->Nmax = 200uL;
    f->orbitConf->M = 16.0f * 16.0f;
    f->orbitThreads = 16u;
    return true;
}

void updateWidthAndHeight(Frame *const f, double zoom, double xOffset, double yOffset)
{
    f->fWidth *= zoom;
    f->fHeight *= zoom;
    f->xStart -= xOffset;
    f->yStart += yOffset;
}
