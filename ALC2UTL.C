/*****************************************************************************
 *                                                                           *
 *   ALC2UTL.C                                                      V3.11    *
 *                                                                           *
 *   ALCHEMY II: Assorted low-level game utils and structures/variables.     *
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

static VOID shwstf(SHORT dark,SHORT shwall);
static SHORT lantrn(VOID);
static CHAR *ssorl(struct aplayr *ptr);
static VOID shwits(SHORT shwall);
static VOID shwaps(VOID);
static SHORT cansee(VOID);

static
struct agputl {               /* utility game-player structure             */
     CHAR userid[ANSIZE];     /* player's name                             */
     SHORT modflg;            /* concatentation index                      */
} *agparr;                    /* array declaration                         */

VOID
inautl(VOID)
{
     agparr=(struct agputl *)alczer(nterms*sizeof(struct agputl));
}

CHAR *
aheshe(struct aplayr *ap)
{
     return(ap->flags&INVISF ? "it" : (ap->flags&FEMALE ? "she" : "he"));
}

CHAR *
ahisher(struct aplayr *ap)
{
     return(ap->flags&INVISF ? "its" : (ap->flags&FEMALE ? "her" : "his"));
}

CHAR *
ahimher(struct aplayr *ap)
{
     return(ap->flags&INVISF ? "it" : (ap->flags&FEMALE ? "her" : "him"));
}

VOID
apfoth(VOID)
{
     SHORT i,loc;
     struct aplayr *utlgp;

     loc=alcptr->curloc;
     if (loc != -1) {
          for (utlgp=alcarr,i=0 ; i < nterms ; utlgp++,i++) {
               if (i != usrnum && utlgp->curloc == loc) {
                    outprf(i);
               }
          }
     }
     clrprf();
}

VOID
apmutl(SHORT msgnum)
{
     prfmsg(msgnum,alcptr->altnam,ahisher(alcptr));
     apfoth();
}

VOID
apfyou(SHORT msgnum)
{
     prfmsg(msgnum);
     outprf(usrnum);
}

VOID
apfbt2(VOID)
{
     SHORT i,loc;
     struct aplayr *utlgp;

     loc=alcptr->curloc;
     if (loc != -1) {
          for (utlgp=alcarr,i=0 ; i < nterms ; utlgp++,i++) {
               if (i != usrnum && i != othusn && utlgp->curloc == loc) {
                    outprf(i);
               }
          }
     }
     clrprf();
}

VOID
apfloc(SHORT loc)
{
     SHORT i;
     struct aplayr *utlgp;

     for (utlgp=alcarr,i=0 ; i < nterms ; utlgp++,i++) {
          if (utlgp->curloc == loc) {
               outprf(i);
          }
     }
     clrprf();
}

VOID
apfnear(VOID)
{
     if (aplptr->north != -1 && aplptr->north != alcptr->curloc) {
          apfloc(aplptr->north);
     }
     if (aplptr->south != -1 && aplptr->south != alcptr->curloc) {
          apfloc(aplptr->south);
     }
     if (aplptr->east != -1 && aplptr->east != alcptr->curloc) {
          apfloc(aplptr->east);
     }
     if (aplptr->west != -1 && aplptr->west != alcptr->curloc) {
          apfloc(aplptr->west);
     }
     clrprf();
}

VOID
apfall(VOID)
{
     SHORT i;
     struct aplayr *utlgp;

     for (utlgp=alcarr,i=0 ; i < nterms ; utlgp++,i++) {
          if (utlgp->curloc != -1) {
               outprf(i);
          }
     }
     clrprf();
}

VOID
xpaloc(SHORT msgnum)
{
     SHORT loc;

     loc=alcptr->prmloc=alcptr->curloc;
     alcptr->curloc=-1;
     prfmsg(msgnum,alcptr->altnam);
     apfloc(loc);
}

VOID
epaloc(SHORT loc,SHORT msgnum)
{
     prfmsg(msgnum,alcptr->altnam);
     apfloc(loc);
     aplptr=&alocs[loc];
     alcptr->curloc=alcptr->prmloc=loc;
     dscloc();
}

VOID
dscloc(VOID)
{
     if ((night && !(aplptr->flags&ALLIGH)) || (aplptr->flags&ALDARK)) {
          if (aplptr->flags&CITIES) {
               shwstf(1,1);
          }
          else if (lantrn()) {
               if (alcspa == alcptr) {
                    prfmsg(URLANT);
               }
               else {
                    prfmsg(OLANTR,alcspa->altnam);
               }
               shwstf(1,1);
          }
          else if (alcptr->flags&DRKVIS) {
               prfmsg(MAGSEE);
               shwstf(1,1);
          }
          else {
               if (aplptr->flags&SHOSKY) {
                    prfmsg(SKYLIG);
                    shwstf(1,0);
               }
               else {
                    prfmsg(SEENOT);
                    outprf(usrnum);
               }
          }
     }
     else {
          shwstf(0,1);
     }
}

static VOID
shwstf(SHORT dark,SHORT shwall)
{
     setmbk(almb);
     if (alcptr->flags&BRIEFF) {
          prfmsg(aplptr->desc,ssorl(alcptr));
     }
     else {
          if (((dark) && (aplptr->flags&DRKDSC)) || (aplptr->flags&ALTDSC)) {
               prfmsg(aplptr->desc+NIGHTM,ssorl(alcptr));
          }
          else {
               prfmsg(aplptr->desc+LONGM,ssorl(alcptr));
          }
     }
     shwits(shwall);
     shwaps();
     if (aplptr->flags&SHOSKY) {
          shosky();
     }
     outprf(usrnum);
}

static SHORT
lantrn(VOID)
{
     SHORT i,j;

     for (alcspa=alcarr,i=0 ; i < nterms ; alcspa++,i++) {
          if (alcspa->curloc == alcptr->curloc) {
               for (j=0 ; j < alcspa->naobjs ; j++) {
                    if (alcspa->apobjs[j] == &alcobs[LANIDX]) {
                         return(1);
                    }
               }
          }
     }
     return(0);
}

static CHAR *
ssorl(struct aplayr *ptr)
{
     static CHAR retval[10];

     if (ptr->flags&SITTNG) {
          strcpy(retval,"sitting");
     }
     else if (ptr->flags&LAYING) {
          strcpy(retval,"lying");
     }
     else {
          strcpy(retval,"standing");
     }
     return(retval);
}

#define INIT_STRUCT_TO_ZERO(structure) memset(&structure, 0, sizeof(structure))

static VOID
shwits(SHORT shwall)
{
     struct alcobj **items;
     struct alcobj *shown[MXLITS];
     SHORT nitms,i,citms,j;

     INIT_STRUCT_TO_ZERO(shown);

     if (shwall) {
          items=aplptr->alobjs;
          nitms=aplptr->nlobjs;
          citms=0;
          for (i=0,j=0 ; i < nitms ; i++) {
               if (items[i]->flags&PICKUP) {
                    shown[j]=items[i];
                    j+=1;
                    citms+=1;
               }
          }
          switch (citms) {
          case 0:
               break;
          case 1:
               prf("There is %s lying %s.\r",ditutl(shown[0]),objloc(aplptr));
               break;
          case 2:
               prf("There is %s and ",ditutl(shown[0]));
               prf("%s lying %s.\r",ditutl(shown[1]),objloc(aplptr));
               break;
          default:
               prf("There is ");
               for (i=0 ; i < citms-1 ; i++) {
                    prf("%s, ",ditutl(shown[i]));
               }
               prf("and %s lying %s.\r",ditutl(shown[citms-1]),
                    objloc(aplptr));
          }
     }
     else {
          prf("It is too dark to see if anything is lying %s.\r",objloc(aplptr));
     }
}

CHAR *
ditutl(struct alcobj *itmptr)
{
     static CHAR retval[4][40];
     static SHORT cycle=0;

     cycle=((cycle+1)&3);
     if (itmptr->flags&USEAN) {
          strcpy(retval[cycle],"an ");
     }
     else {
          strcpy(retval[cycle],"a ");
     }
     strcat(retval[cycle],itmptr->name);
     if (itmptr->flags&POTION) {
          strcat(retval[cycle]," potion");
     }
     return(retval[cycle]);
}

CHAR *
nitutl(struct alcobj *itm)
{
     static CHAR retval[4][40];
     static SHORT cycle=0;

     cycle=((cycle+1)&3);
     strcpy(retval[cycle],itm->name);
     if (itm->flags&POTION) {
          strcat(retval[cycle]," potion");
     }
     return(retval[cycle]);
}

CHAR *
objloc(struct aloc *lp)
{
     static CHAR stg[60];

     if (prfptr != prfbuf) {
          outprf(usrnum);
     }
     setmbk(almb);
     prfmsg(lp->desc+GROUNDM);
     strcpy(stg,prfbuf);
     clrprf();
     setmbk(amb);
     return(stg);
}

static
CHAR *seestg[NCSTGS]={
     "",
     " (slightly transparent)",         /* 1 */
     " (looking rather drunk)",         /* 2 */
     " (looking REALLY ugly)",          /* 3 */
     " (emanating a nasty odor)",       /* 4 */
     " (picking his nose)",             /* 5 */
     " (picking her nose)",             /* 6 */
     " (slightly smoking)",             /* 7 */
     " (currently on fire)",            /* 8 */
     " (stark naked)"                   /* 9 */
};

static VOID
shwaps(VOID)
{
     SHORT i,ncngps;
     struct aplayr *locogp;

     for (i=0,ncngps=0,locogp=alcarr ; i < nterms ; i++,locogp++) {
          if (locogp->curloc == alcptr->curloc && locogp != alcptr) {
               if (locogp->flags&INVISF) {
                    if (alcptr->counts[CINVIS]) {
                         strcpy(agparr[ncngps].userid,locogp->alias);
                         agparr[ncngps].modflg=1;
                         ncngps++;
                    }
               }
               else {
                    strcpy(agparr[ncngps].userid,locogp->altnam);
                    agparr[ncngps].modflg=locogp->seemod;
                    ncngps++;
               }
          }
     }
     switch (ncngps) {
     case 0:
          break;
     case 1:
          prf("%s%s is here.\r",agparr[0].userid,seestg[agparr[0].modflg]);
          break;
     case 2:
          prf("%s%s and %s%s are here.\r",agparr[0].userid,seestg[agparr[0].modflg],
                                          agparr[1].userid,seestg[agparr[1].modflg]);
          break;
     default:
          for (i=0 ; i < ncngps-1 ; i++) {
               prf("%s%s, ",agparr[i].userid,seestg[agparr[i].modflg]);
          }
          prf("and %s%s are here.\r",agparr[i].userid,seestg[agparr[i].modflg]);
     }
}

VOID
ainutl(struct aplayr *who)
{
     struct alcobj **gpobjs;
     SHORT nobjs=0,i;

     (VOID)nobjs;
     gpobjs=who->apobjs;
     switch (nobjs=who->naobjs) {
     case 0:
          prf("nothing");
          break;
     case 1:
          prf("%s",ditutl(gpobjs[0]));
          break;
     case 2:
          prf("%s and %s",ditutl(gpobjs[0]),ditutl(gpobjs[1]));
          break;
     default:
          for (i=0 ; i < nobjs-1 ; i++) {
               prf("%s, ",ditutl(gpobjs[i]));
          }
          prf("and %s",ditutl(gpobjs[i]));
     }
     prf(".\r");
}

SHORT
findap(CHAR *stg)
{
     for (alcaux=alcarr,othusn=0 ; othusn < nterms ; alcaux++,othusn++) {
          if (alcaux->curloc == alcptr->curloc) {
               if (sameto(stg,alcaux->alias) && cansee()) {
                    return(1);
               }
          }
     }
     return(0);
}

static SHORT
cansee(VOID)
{
     if (!(alcaux->flags&INVISF) || (alcptr->counts[CINVIS])) {
          return(1);
     }
     return(0);
}

SHORT
pinalc(CHAR *stg)
{
     for (alcaux=alcarr,othusn=0 ; othusn < nterms ; alcaux++,othusn++) {
          if (alcaux->curloc != -1 && sameas(stg,alcaux->alias)) {
               return(1);
          }
     }
     return(0);
}

struct alcobj *
fapobj(struct aplayr *gp,CHAR *stg)
{
     for (aobjno=0 ; aobjno < gp->naobjs ; aobjno++) {
          if (sameto(stg,gp->apobjs[aobjno]->name)) {
               return(gp->apobjs[aobjno]);
          }
     }
     return(NULL);
}

struct alcobj *
falobj(struct aloc *loc,CHAR *stg)
{
     for (aobjno=0 ; aobjno < loc->nlobjs ; aobjno++) {
          if (sameto(stg,loc->alobjs[aobjno]->name)) {
               return(loc->alobjs[aobjno]);
          }
     }
     return(NULL);
}

VOID
tapobj(struct aplayr *gp,SHORT objino)
{
     if (--gp->naobjs != objino) {
          gp->apobjs[objino]=gp->apobjs[gp->naobjs];
     }
}

VOID
talobj(struct aloc *loc,SHORT objino)
{
     loc->nlobjs-=1;
     if (loc->nlobjs != objino) {
          loc->alobjs[objino]=loc->alobjs[loc->nlobjs];
     }
}

VOID
papobj(struct aplayr *gp,struct alcobj *obj)
{
     gp->apobjs[(gp->naobjs)++]=obj;
}

VOID
palobj(struct aloc *loc,struct alcobj *obj)
{
     loc->alobjs[(loc->nlobjs)++]=obj;
}

SHORT
hitap(struct aplayr *who,SHORT damage)
{
     who->hits-=damage;
     if (who->hits <= 0) {
          killap(who);
          return(1);
     }
     return(0);
}

VOID
killap(struct aplayr *who)
{
     SHORT loc;
     INT tmpusn;
     struct aplayr *tmpplr;
     CHAR plyhld[ALSSIZ];

     loc=who->curloc;
     setmbk(amb);
     prfmsg(PDIEDU);
     outprf(who->modno);
     strcpy(plyhld,who->alias);
     initap(who,plyhld);
     prfmsg(PDIEDO,who->userid);
     apfloc(loc);
     tmpplr=alcptr;
     tmpusn=usrnum;
     alcptr=who;
     usrnum=who->modno;
     epaloc(STALOC,HELNEW);
     alcptr=tmpplr;
     usrnum=tmpusn;
}

struct comcmd *
a2ccmp(CHAR *stgptr,struct comcmd table[], SHORT length)
{
     SHORT cond;
     struct comcmd *low,*mid,*high;

     low=&table[0];
     high=&table[length-1];
     while (low <= high) {
          mid=low+((SHORT)(high-low))/2;
          if ((cond=(SHORT)strcmp(stgptr,mid->word)) < 0) {
               if (mid == low) {
                    break;
               }
               high=mid-1;
          }
          else if (cond > 0) {
               if (mid == high) {
                    break;
               }
               low=mid+1;
          }
          else {
               return(mid);
          }
     }
     return(NULL);
}

SHORT
rndlap(SHORT loc)
{
     for (alcptr=alcarr,usrnum=0 ; usrnum < nterms ; alcptr++,usrnum++) {
          if (alcptr->curloc == loc) {
               return(1);
          }
     }
     return(0);
}

