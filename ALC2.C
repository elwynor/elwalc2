/*****************************************************************************
 *                                                                           *
 *   ALC2.C                                                         V3.11    *
 *                                                                           *
 *   ALCHEMY II: Game mainline and The Major BBS module interface.           *
 *                                                                           *
 *                                            - S. Brinker 2/28/89           *
 *   Worldgroup 3.2 Port, including bug fixes - R. Hadsall 1/17/2006 and     *
 *                                                         4/2/2021 (v3.1)   *
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

static SHORT al2lin(VOID);
static SHORT alch2(VOID);
static SHORT alcinj(VOID);
static VOID al2hup(VOID);
static VOID delal2(CHAR *uid);
static VOID al2sdn(VOID);
static VOID bckovr(SHORT number);
static VOID stoobs(VOID);
static VOID retobs(VOID);

INT alcstt;                 /* Alchemy II state number                   */
struct module elwalc2={      /* module interface block                    */
     "",                      /*    name used to refer to this module      */
     al2lin,                  /*    user log-on suplement                  */
     alch2,                   /*    input routine if selected              */
     dfsthn,                  /*    status-input routine                   */
     alcinj,                  /*    "injoth" routine for this module       */
     NULL,                    /*    user logoff supplemental routine       */
     al2hup,                  /*    hang-up routine                        */
     NULL,                    /*    midnight clean-up                      */
     delal2,                  /*    delete account routine                 */
     al2sdn                   /*    system shut-down routine               */
};

struct alctrl alctrl;         /* occurance of game control file            */
struct aloc *alocs,           /* dynamically allocated array of locations  */
            *aplptr;          /* pointer to current player's location stuff*/
struct aplayr *alcarr,        /* base address of dynamic array of players  */
              *alcptr,        /* pointer to alcarr[usrnum] structure       */
              *alcaux,        /* global pointer for "other" player struct. */
              *alcspa;        /* general purpose player structure pointer  */
struct alcobj *alcobs,        /* dynamically allocated array of objects    */
              *aobptr;        /* general purpose pointer for objects       */
SHORT aobjno,                 /* used for an object's array position       */
    aobjn2;                   /* another object array position holder      */
SHORT margvi;                 /* global variable for current margv[] index */

DFAFILE *a2bb,                /* BTRIEVE file pointer for player records   */
        *acbb;                /* BTRIEVE file pointer for control file     */
HMCVFILE amb,                 /* file pointer for general messages         */
         admb,                /* file pointer for description messages     */
         almb,                /* file pointer for location descriptions    */
         acmb;                /* file pointer for computer messages        */

#define PLAYSS 100            /*   "playing" sub-state                     */

CHAR *trucols[NTRUCS]={
     "blue",
     "pink",
     "yellow",
     "green",
     "black",
     "white"
};

CHAR *alcatr;                 /* attribute for displayed ALC II text       */

VOID EXPORT
init__elwalc2(VOID)             /* init of alchemy II                        */
{
     SHORT n;

     stzcpy(elwalc2.descrp,gmdnam("ELWALC2.MDF"),MNMSIZ);
     alcstt=register_module(&elwalc2);
     initializeCommands();
     admb=opnmsg("elwalc2d.mcv");
     almb=opnmsg("elwalc2l.mcv");
     acmb=opnmsg("elwalc2c.mcv");
     alcatr=stgopt(ALCATR);
     amb=opnmsg("elwalc2m.mcv");

     a2bb=dfaOpen("elwalc2u.dat",sizeof(struct aplayr),NULL);
     alcarr=(struct aplayr *)alczer(nterms*sizeof(struct aplayr)); // used to be alczer(n=(nterms*sizeof(struct aplayr))); but value of n was never used and it caused a memcata()!
     for (n=0 ; n < nterms ; n++) {
          alcarr[n].curloc=-1;
     }
     a2dobj();
     a2dloc();
     inautl();
     iactrl();
     mainat();     

     shocst(spr("ELW Alchemy II v%s",VERSION),"(C) Copyright 2024 Elwynor Technologies - www.elwynor.com");
}

static GBOOL
al2lin(VOID)                  /* supplimental logon routine for Alchemy II */
{
     dfaSetBlk(a2bb);
     if (!dfaQueryEQ(usaptr->userid,0)) {
          setmbk(amb);
          prfmsg(PLYAL2);
          outprf(usrnum);
     }
     return(0);
}

static GBOOL
alch2(VOID)                   /* status routine if selected for Alchemy II */
{
     SHORT i,j=0,len,bad=0;

     alcptr=&alcarr[usrnum];
     aplptr=&alocs[alcptr->prmloc];
     if (sameas(margv[0],"x")) {
          switch(usrptr->substt) {
          case 1:
               setmbk(amb);
               prfmsg(EXIAL2);
               outprf(usrnum);
               btupmt(usrnum,0);
               return(0);
          case PLAYSS:
               setmbk(amb);
               xpaloc(EXIALO);
               prfmsg(EXIAL2);
               btupmt(usrnum,0);
               return(0);
          }
     }
     switch (usrptr->substt) {
     case 0:
          dfaSetBlk(a2bb);
          alcptr->modno=(SHORT)usrnum;
          if (alcptr->flags&LOADED) {
               setmbk(amb);
               prfmsg(CREDITS);
               prf(alcatr);
               outprf(usrnum);
               epaloc(alcptr->prmloc,HERAGN);
               usrptr->substt=PLAYSS;
          }
          else if (dfaAcqEQ(alcptr,usaptr->userid,0)) {
               retobs();
               alcptr->flags|=LOADED;
               alcptr->modno=(SHORT)usrnum;
               setmbk(amb);
               prfmsg(CREDITS);
               prf(alcatr);
               outprf(usrnum);
               epaloc(alcptr->prmloc,WELBAK);
               usrptr->substt=PLAYSS;
          }
          else {
               setmbk(admb);
               btumil(usrnum,ALSSIZ-1);
               usrptr->substt++;
               prf(alcatr);
               prfmsg(GETALS);
               outprf(usrnum);
               break;
          }
          btupmt(usrnum,'>');
          break;
     case 1:
          btumil(usrnum,DFTIMX);
          setmbk(admb);
          strlwr(margv[0]);
          margv[0][0]=(CHAR)toupper(margv[0][0]);
          len=(SHORT)strlen(margv[0]);
          dfaSetBlk(a2bb);
          if (pfnlvl > 0) {
               prfmsg(FOULID);
               outprf(usrnum);
               break;
          }
          if (margc != 1) {
               bad=1;
          }
          else {
               for (i=0 ; i < len ; i++) {
                    if (digalw ? !isalnum(margv[0][i])
                        : !isalpha(margv[0][i])) {
                         bad=1;
                         break;
                    }
               }
          }
          if (bad || len < 3 || len > (ALSSIZ-1)) {
               prfmsg(BADPID);
               prfmsg(B4PLA2);
          }
          else if ((sameas(margv[0],"Sysop") && !sameas(usaptr->userid,"Sysop"))
                   || dfaQueryEQ(margv[0],1)) {
               prfmsg(NTGOOD);
               prfmsg(B4PLA2);
          }
          else {
               btupmt(usrnum,'>');
               initap(alcptr,margv[0]);
               dfaInsert(alcptr);
               prfmsg(GOODPD,margv[0]);
               usrptr->substt++;
          }
          outprf(usrnum);
          break;
     case 2:
          btuxnf(usrnum,0,19);
          btupbc(usrnum,0);
          setmbk(admb);
          prfmsg(INTROA,(usaptr->sex == 'M' ? 'r' : 's'),alcptr->alias);
          outprf(usrnum);
          btuech(usrnum,0);
          usrptr->substt++;
          break;
     case 3:
          if (!(usrptr->flags&ABOIP)) {
               bckovr(34);
          }
          setmbk(admb);
          prfmsg(INTROB);
          outprf(usrnum);
          usrptr->substt++;
          break;
     case 4:
          setmbk(admb);
          prfmsg(INTROC);
          outprf(usrnum);
          usrptr->substt++;
          break;
     case 5:
          if (!(usrptr->flags&ABOIP)) {
               bckovr(33);
          }
          setmbk(admb);
          prfmsg(INTROD);
          outprf(usrnum);
          usrptr->substt++;
          break;
     case 6:
          if (!(usrptr->flags&ABOIP)) {
               bckovr(33);
          }
          setmbk(admb);
          prfmsg(INTROE,(usaptr->sex == 'M' ? "Joe Schmoe" : "Jane Schmane"));
          outprf(usrnum);
          usrptr->substt++;
          break;
     case 7:
          if (!(usrptr->flags&ABOIP)) {
               bckovr(33);
          }
          setmbk(admb);
          prfmsg(INTROF);
          outprf(usrnum);
          btuech(usrnum,1);
          usrptr->substt++;
          break;
     case 8:
          btumil(usrnum,DFTIMX);
          btupmt(usrnum,0);
          btuxmt(usrnum,"\r");
          setmbk(amb);
          epaloc(STALOC,HELNEW);
          usrptr->substt=PLAYSS;
          btupmt(usrnum,'>');
          rstrxf();
          break;
     case PLAYSS:
          setmbk(amb);
          if (alcptr->macros < NCPERT) {
               alcptr->macros++;
               acmhdl();
          }
          else {
               prfmsg(SLODWN);
               outprf(usrnum);
          }
          for (i=0 ; i < nterms ; i++) {
               if (alcarr[i].modno != i && usrnum == i) {
                    j=1;
               }
          }
          if (j) {
               rstrin();
          }
          break;
     }
     return(1);
}

static GBOOL
alcinj(VOID)                  /* Alchemy II injoth() routine handler       */
{
     dftinj();
     btuxmn(othusn,alcatr);
     return(1);
}

static VOID
bckovr(SHORT number)                /* back over XX number of spaces             */
{
#define MAXWID 250
     SHORT save,i;

     save=usaptr->scnwid;
     usaptr->scnwid=MAXWID;
     for (i=0 ; i < number ; i++) {
          btuxmt(usrnum,"\b \b");
     }
     usaptr->scnwid=(CHAR)save;
}

VOID
initap(struct aplayr *app,CHAR *plyrid)                 /* prepare to save to disk and set strc.*/
  /* plyrid: alias id pass through           */
{
     struct usracc *uap;
     SHORT i,modno;

     modno=app->modno;
     uap=uacoff(modno);
     setmem(app,sizeof(struct aplayr),0);
     strcpy(app->userid,uap->userid);
     strcpy(app->altnam,uap->userid);
     strcpy(app->alias,plyrid);
     app->desc=(uap->sex == 'M' ? AMPD00 : AFPD00);
     if (uap->sex == 'F') {
          app->flags|=FEMALE;
     }
     app->flags|=LOADED;
     app->curloc=-1;
     app->hits=app->maxhits=STAHTS;
     app->month=alctrl.month;
     app->trunum=(SHORT)genrdn(1,1000);
     app->trunam[0]=(CHAR)(genrdn(0,26) + 'A');
     for (i=(SHORT)1 ; i < TRUSIZ-1 ; i++) {
          app->trunam[i]=(CHAR)(genrdn(0,26) + 'a');
     }
     app->color=(SHORT)genrdn(0,NTRUCS);
     for (i=0 ; i < NLCMBS ; i++) {
          app->lcombo[i]=(SHORT)genrdn(1,10);
     }
     app->favpot= (SHORT)genrdn(50,75);
     app->rsolve= (SHORT)genrdn(0,NMSTGS);
     app->asolve= (SHORT)genrdn(0,NMSTGS);
     app->slvloc= (SHORT)genrdn(0,NALOCS);
     app->stinx = (SHORT)genrdn(0,5);
     app->modno=modno;
}

static
VOID
stoobs(VOID)                  /* pack objects for users                    */
{
     SHORT i;

     for (i=0 ; i < alcptr->naobjs ; i++) {
          alcptr->apobjs[i]=(struct alcobj *)(alcptr->apobjs[i]-alcobs);
     }
}

static
VOID
retobs(VOID)                   /* retrieve objects from users              */
{
     SHORT i;

     for (i=0 ; i < alcptr->naobjs ; i++) {
          alcptr->apobjs[i]=&alcobs[(INT)(alcptr->apobjs[i])];
     }
}

static VOID
al2hup(VOID)                  /* user hang up routine for Alchemy II       */
{
     alcptr=&alcarr[usrnum];
     aplptr=&alocs[alcptr->prmloc];
     if (alcptr->curloc != -1) {
          setmbk(amb);
          apmutl((usaptr->sex == 'M' ? HUPMSGM : HUPMSGF));
          alcptr->curloc=-1;
     }
     if (alcptr->flags&LOADED) {
          alcptr->flags&=~LOADED;
          alcptr->rouctr=0;
          stoobs();
          dfaSetBlk(a2bb);
          if (!dfaAcqEQ(NULL,usaptr->userid,0)) {
               catastro("al2hup error");
          }
          dfaUpdate(alcptr);
     }
     setmem(alcptr,sizeof(struct aplayr),0);
     alcptr->curloc=-1;
}

static VOID
delal2(CHAR *uid)                   /* deletion of account routine for Alchemy II*/
{
     dfaSetBlk(a2bb);
     if (dfaAcqEQ(NULL,uid,0)) {
          dfaDelete();
     }
}

static VOID
al2sdn(VOID)                  /* system shutdown routine for Alchemy II    */
{
     dfaClose(a2bb);
     dfaClose(acbb);
     clsmsg(amb);
     clsmsg(admb);
     clsmsg(almb);
     clsmsg(acmb);
}
