/*****************************************************************************
 *                                                                           *
 *   ALC2COM.C (ALCCOM.C)                                           V3.11    *
 *                                                                           *
 *   ALCHEMY II: Computer handling routines.                                 *
 *                                                                           *
 *                                            - S. Brinker 2/28/89           *
 *   Worldgroup 3.2 Port, including bug fixes - R. Hadsall 1/17/2006 and     *
 *                                                         4/2/2021 (v3.1)   *
 *                                                                           *
 *   Major BBS V10 Port                       - R. Hadsall 8/3/2024          *
 *                                                                           *
 * Copyright (C) 2005-2024 Rick Hadsall.  All Rights Reserved.               *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      * 
 * it under the terms of the GNU Affero General Public License as published  *
 * by the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU Affero General Public License for more details.                       *
 *                                                                           *
 * You should have received a copy of the GNU Affero General Public License  *
 * along with this program. If not, see <https://www.gnu.org/licenses/>.     *
 *                                                                           *
 * Additional Terms for Contributors:                                        *
 * 1. By contributing to this project, you agree to assign all right, title, *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies.                              *
 * 2. You grant Rick Hadsall and Elwynor Technologies a non-exclusive,       *
 *    royalty-free, worldwide license to use, reproduce, prepare derivative  *
 *    works of, publicly display, publicly perform, sublicense, and          *
 *    distribute your contributions                                          *
 * 3. You represent that you have the legal right to make your contributions *
 *    and that the contributions do not infringe any third-party rights.     *
 * 4. Rick Hadsall and Elwynor Technologies are not obligated to incorporate *
 *    any contributions into the project.                                    *
 * 5. This project is licensed under the AGPL v3, and any derivative works   *
 *    must also be licensed under the AGPL v3.                               *
 * 6. If you create an entirely new project (a fork) based on this work, it  *
 *    must also be licensed under the AGPL v3, you assign all right, title,  *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies, and you must include these   *
 *    additional terms in your project's LICENSE file(s).                    *
 *                                                                           *
 * By contributing to this project, you agree to these terms.                *
 *                                                                           *
 *****************************************************************************/

#include "alc2.h"

static VOID rdnmsg(VOID);
static VOID who(VOID);
static VOID what(VOID);
static VOID becaus(VOID);
static VOID irou(VOID);
static SHORT fndwrd(CHAR *word);

static
VOID
rdnmsg(VOID)
{
     prfmsg(RDNM00+genrdn(0,5));
}

static
VOID
who(VOID)
{
     if (fndwrd("are") && fndwrd("you?")) {
          prfmsg(WHOMS0);
     }
     else if (fndwrd("is") && fndwrd("this?")) {
          prfmsg(WHOMS0);
     }
     else {
          rdnmsg();
     }
}

static
VOID
what(VOID)
{
     if (fndwrd("are") && fndwrd("you?")) {
          prfmsg(WHOMS0);
     }
     else if (fndwrd("truths?") && fndwrd("life?")) {
          if (alcptr->comstt == 1000) {
               prfmsg(YOUWIN,usaptr->userid);
               alcptr->flags|=WONGAM;
          }
          else {
               prfmsg(TRUQUE);
          }
     }
     else {
          prfmsg(NOQUES);
     }
}

static
VOID
becaus(VOID)
{
     if (margc == 1) {
          prfmsg(BECMSD,alcptr->altnam);
     }
     else {
          rstrin();
          prfmsg(BECMSA,margv[1]);
     }
}

static VOID
irou(VOID)
{
     prfmsg(IMSGEN);
}

static SHORT
fndwrd(CHAR *word)
{
     SHORT i;

     for (i=1 ; i < margc ; i++) {
          if (sameto(margv[i],word)) {
               return(1);
          }
     }
     return(0);
}

struct comcmd c8088a[6]={
     {"because",becaus},
     {"i",      irou},
     {"what",   what},
     {"what?",  rdnmsg},
     {"who",    who},
     {"who?",   rdnmsg}
};


VOID
mr8088(VOID)
{
     struct comcmd *ptr;

     setmbk(acmb);
     if (margc != 1) {
          bagwrd("8088");
          bagwrd("8088,");
     }
     if ((ptr=a2ccmp(margv[0],c8088a,6)) != NULL) {
          //(*(ptr->routine))();
         ptr->routine();
     }
     else {
          rdnmsg();
     }
     outprf(usrnum);
     alcptr->comstt=0;
}

