/*****************************************************************************
 *                                                                           *
 *   ALC2TIM.C                                                      V3.11    *
 *                                                                           *
 *   ALCHEMY II: Real-time kicks/related routines for game timers/controls.  *
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

static VOID incday(VOID);
static VOID sunrise(SHORT stage);
static VOID outsky(VOID);

#define BRNDAM   2            /* amount of damage for still burning        */

CHAR *amnths[NMONTHS]={
     "Yarno",
     "Zeng",
     "Randia",
     "Foop",
     "Alluva",
     "Varmon"
};

CHAR *phases[NMSTGS]={
     "not visible",
     "waxing",
     "half full",
     "gibbous",
     "full",
     "gibbous",
     "half full",
     "waning"
};

SHORT night=0;           /* global variable for nighttime                  */
SHORT bskbal=0;                    /* basketball status, on or off         */
SHORT lfgopn=0;                    /* gate at lit forest, open or closed   */
SHORT movstg=0;                    /* current stage of the "movie"         */
SHORT domes[6];                    /* current status of dome pedestals     */

#define RISE1 1
#define RISE2 2
#define RISE3 3
#define RISE4 4
#define SET1  5
#define SET2  6
#define SET3  7
#define SET4  8

VOID
iactrl(VOID)
{
     acbb=dfaOpen("elwalc2.dat",sizeof(struct alctrl),NULL);
     if (!dfaAcqEQ(&alctrl,"key",0)) {
          setmem(&alctrl,sizeof(struct alctrl),0);
          strcpy(alctrl.key,"key");
          alctrl.time=1;
          alctrl.year=FIRSTYR;
          alctrl.rmoonc=alctrl.amoonc=4;
          dfaInsert(&alctrl);
     }
     incday();
     alctrl.time=600;
}

VOID
mainat(VOID)
{
     static SHORT genctr=0;
     SHORT time,i,j,rm,domstp=0;

     setmbk(amb);
     time=alctrl.time;
     if (time == RISTIM) {
          sunrise(genctr=RISE1);
          incday();
     }
     else if (time == SETTIM) {
          sunrise(genctr=SET1);
     }
     else if (genctr) {
          sunrise(++genctr);
          if (genctr == RISE4 || genctr == SET4) {
               genctr=0;
          }
     }
     else if (time == HOURSPD) {
          time=-CTLRTK;
     }
     else {
          switch (genrdn(0,5)) {
          case 0:
               if (alocs[56].nlobjs < 4) {
                    palobj(&alocs[56],&alcobs[77]);
                    prfmsg(DSSMSG);
                    apfloc(56);
               }
               break;
          case 1:
               rm=(SHORT)genrdn(41,166);
               if (rndlap(rm)) {
                    prfmsg(BRNM00,alcptr->altnam,ahimher(alcptr));
                    apfoth();
                    prfmsg(BRNM01);
                    outprf(usrnum);
                    if (alcptr->naobjs) {
                         prfmsg(BRNM02,alcptr->altnam,ahisher(alcptr));
                         apfoth();
                         prfmsg(BRNM03,nitutl(alcptr->apobjs[0]));
                         outprf(usrnum);
                         tapobj(alcptr,0);
                    }
               }
               break;
          case 2:
               rm=(SHORT)genrdn(41,166);
               if (rndlap(rm) && uacoff(usrnum)->sex == 'M') {
                    prfmsg(SQUM00,alcptr->altnam,ahisher(alcptr));
                    apfoth();
                    prfmsg(SQUM01);
                    outprf(usrnum);
                    hitap(alcptr,5);
               }
               break;
          case 3:
               rm=(SHORT)genrdn(0,224);
               if (alocs[rm].nlobjs < MXLITS) {
                    palobj(&alocs[rm],&alcobs[genrdn(0,40)]);
                    prfmsg(FAIRYM);
                    apfloc(rm);
               }
               break;
          case 4:
               rm=(SHORT)genrdn(41,166);
               if (rndlap(rm) && alcptr->gold) {
                    alcptr->gold=0;
                    prfmsg(SAXM00,alcptr->altnam,ahisher(alcptr));
                    apfoth();
                    prfmsg(SAXM01);
                    outprf(usrnum);
               }
               break;
          }
     }
     time+=CTLRTK;
     alctrl.time=time;
     for (alcptr=alcarr,usrnum=0 ; usrnum < nterms ; alcptr++,usrnum++) {
          if (alcptr->curloc != -1) {
               alcptr->macros=0;
               for (j=0 ; j < NPCNTS ; j++) {
                    if (alcptr->counts[j]) {
                         if (--alcptr->counts[j] == 0) {
                              switch (j) {
                              case INVISI:
                                   alcptr->flags&=~INVISF;
                                   strcpy(alcptr->altnam,alcptr->alias);
                                   prfmsg(RFINVO,alcptr->alias);
                                   alcptr->curloc=-1;
                                   apfloc(alcptr->prmloc);
                                   alcptr->curloc=alcptr->prmloc;
                                   prfmsg(RFINVU);
                                   break;
                              case CINVIS:
                                   prfmsg(RFCINV);
                                   break;
                              default:
                                   prfmsg(LILPRO);
                              }
                              outprf(usrnum);
                         }
                    }
               }
               if (alcptr->rouctr != 0) {
                   alcptr->approu();// (*(alcptr->approu))();
               }
               if (alcptr->seemod == 8) {
                    apmutl(STBURO);
                    apfyou(STBURU);
                    hitap(alcptr,BRNDAM);
                    continue;
               }
               if (alcptr->pickle) {
                    if (++alcptr->pickle == 3) {
                         apfyou(DBPIKL);
                         killap(alcptr);
                         continue;
                    }
                    else {
                         apfyou(LOPIKL);
                    }
               }
               if (alcptr->peectr >= 12) {
                    if (alcptr->peectr == 22) {
                         apmutl(PEEXPO);
                         prfmsg(PEEXPU);
                         outprf(usrnum);
                         killap(alcptr);
                         continue;
                    }
                    else {
                         prfmsg(MUSPEE,alcptr->peectr-11);
                         outprf(usrnum);
                         alcptr->peectr++;
                    }
               }
          }
     }
     for (i=0 ; i < 6 ; i++) {
          if (domes[i] == 1) {
               domes[i]=0;
               domstp=1;
          }
     }
     if (domstp) {
          //domstp=0;
          prfmsg(PSGLOW);
          apfloc(220);
     }
     if (bskbal) {
          prfmsg(BSKSTP);
          apfloc(212);
          bskbal=0;
     }
     if (lfgopn) {
          prfmsg(LFGCLS);
          apfloc(59);
          apfloc(179);
          alocs[59].flags&=~ALTDSC;
          alocs[179].flags&=~ALTDSC;
          alocs[179].south=-1;
          alocs[59].north=161;
          lfgopn=0;
     }
     prfmsg(MOVIE0+movstg);
     apfloc(30);
     movstg=(movstg > 6 ? 0 : movstg+1);
     rtkick(CTLRTK,mainat);
}

static VOID
incday(VOID)
{
     if ((++alctrl.day) == NDAYSPM) {
          alctrl.day=0;
          if ((++alctrl.month) == NMONTHS) {
               alctrl.month=0;     /* index in amnths[] */
               alctrl.year++;
          }
     }
     if (((alctrl.day+1)%2) == 0) {
          if ((++alctrl.rmoonc) == NMSTGS) {
               alctrl.rmoonc=0;
          }
     }
     if (((alctrl.day+1)%3) == 0) {
          if ((++alctrl.amoonc) == NMSTGS) {
               alctrl.amoonc=0;
          }
     }
}

static VOID
sunrise(SHORT stage)
{
     prfmsg(SETRIS+(stage-1));
     outsky();
     if (stage == SET4) {
          prfmsg(CGSCLS);          /* close city gates */
          apfloc(0);
          apfloc(1);
          alocs[0].east=-1;
          alocs[1].west=-1;
          if (alctrl.amoonc == alctrl.rmoonc && alctrl.amoonc == 4) {
               alctrl.stropn=1;
               prfmsg(SBBOPN);     /* open strange building */
               apfloc(28);
               apfloc(39);
               alocs[28].south=39;
               alocs[39].north=28;
          }
          night=1;
     }
     if (stage == RISE4) {
          prfmsg(CGSOPN);          /* open city gates */
          apfloc(0);
          apfloc(1);
          alocs[0].east=1;
          alocs[1].west=0;
          if (alctrl.stropn) {
               prfmsg(SBBCLS);     /* close strange building */
               apfloc(28);
               apfloc(39);
               alocs[28].south=-1;
               alocs[39].north=-1;
               alctrl.stropn=0;
          }
          night=0;
          dfaSetBlk(acbb);
          dfaAcqEQ(NULL,"key",0);
          dfaUpdate(&alctrl);
     }
}

static VOID
outsky(VOID)
{
     SHORT i;
     struct aplayr *tmpplr;

     for (tmpplr=alcarr,i=0 ; i < nterms ; tmpplr++,i++) {
          if (tmpplr->curloc != -1 && (alocs[tmpplr->curloc].flags&SHOSKY)) {
               outprf(i);
          }
     }
     clrprf();
}

VOID
shosky(VOID)
{
     setmbk(amb);
     if (alctrl.time < SETTIM) {
          prfmsg(SUNSHI);
     }
     else {
          if (alctrl.amoonc == alctrl.rmoonc) {
               prfmsg(BOTHAM,phases[alctrl.amoonc]);
          }
          else {
               prfmsg(MOONAM,phases[alctrl.amoonc],phases[alctrl.rmoonc]);
          }
     }
}

CHAR *
camnth(VOID)
{
     return(amnths[alctrl.month]);
}

VOID
hiccup(VOID)
{
     apmutl(HICCUO);
     apfyou(HICCUU);
     alcptr->rouctr--;
}

VOID
halluc(VOID)
{
     apmutl(GIGM05);
     apfyou(GIGM01+alcptr->rouctr);
     alcptr->rouctr--;
     if (alcptr->rouctr == 0) {
          alcptr->seemod=0;
     }
}
