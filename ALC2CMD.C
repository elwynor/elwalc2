/*****************************************************************************
 *                                                                           *
 *   ALC2CMD.C                                                      V3.11    *
 *                                                                           *
 *   ALCHEMY II: Main player command parser and command handling routines.   *
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

typedef VOID(*AcmCmdRouNoParam)(VOID);
typedef VOID(*AcmCmdRouWithParam)(SHORT);

struct acmcmd {
     CHAR *word;
     union {
         AcmCmdRouNoParam noParam;
         AcmCmdRouWithParam withParam;
     } routine;
     SHORT asmoff;
};

static VOID aask(VOID);
static VOID asay(VOID);
static VOID awhisp(VOID);
static VOID ayell(VOID);
static VOID amvutl(SHORT dest,SHORT msgnum);
static VOID akiss0(VOID);
static VOID akiss1(VOID);
static VOID akick0(VOID);
static VOID akick1(VOID);
static VOID akissr(SHORT mode,SHORT nasty);
static VOID agold(VOID);
static VOID agiver(VOID);
static VOID agiveg(SHORT amt,SHORT who);
static VOID agiveo(CHAR *givee,CHAR *givwhat);
static VOID aget(VOID);
static VOID agetp(CHAR *who,CHAR *what);
static VOID agetl(CHAR *what);
static VOID alook(VOID);
static VOID ainv(VOID);
static VOID adrop(VOID);
static VOID asit(VOID);
static VOID alie(VOID);
static VOID astand(VOID);
static VOID aundrs(VOID);
static VOID abrief(VOID);
static VOID aubrief(VOID);
static VOID done(VOID);
static VOID ahelp(VOID);
static VOID asetim(VOID);
static VOID asysob(VOID);
static VOID asggol(VOID);
static VOID ahits(VOID);
static VOID asmcmd(SHORT off);
static VOID rubeme(VOID);
static struct acmcmd *a2csch(CHAR *stgptr,struct acmcmd table[],SHORT length);
static VOID mc0msg(VOID);
static SHORT chkacm(VOID);

/* must have a global
     CHAR *comnam

     which will be initialized from a configuration file; this is the "name"
     of the computer.  for our sake, we'll declare it "8088" to avoid
     confusion with a user-id.

     also need a
          SHORT saypfn;  1-4 (1=censor almost everything,4=censor nothing)
     which will determine at what profanity level the game will start
     censoring "broadcast" (from one player to another) messages.
*/

#define GOOD        0
#define BAD         1
#define NMODIFS    10

static
CHAR *comnam={"8088"};
static
SHORT saypfn=3;

CHAR *adverbs[2][NMODIFS]={
     {"tenderly","sympathetically","helpfully","kindly","sadly",
     "surreptitiously","quietly","shyly","seductively","knowingly"},

     {"angrily","sadistically","violently","savagely","hatefully",
     "wildly","grouchily","uncontrollably","silently","scornfully"}
};
CHAR *adjects[2][NMODIFS]={
     {"gentle","warm","loving","soft","very understanding",
     "caring","big","enthusiastic","reluctant","suggestive"},

     {"hard","painful","fierce","sadistic","cold-hearted",
     "uncaring","spiteful","nasty","infuriated","unexpected"}
};

extern CHAR *alcatr;

static
VOID
aask(VOID)
{
     if (margc > 2) {
          if ((aobptr=falobj(aplptr,margv[1])) != NULL && (aobptr->flags&ASKROU)) {
               prfmsg(ASKINO,alcptr->altnam,aheshe(alcptr));
               apfoth();
               (*(obrous[(SHORT)(aobptr-alcobs)].routine))();
          }
          else if (sameas(margv[1],comnam)) {
               /* I answer no questions - this is done on purpose! */
          }
          else {
               asay();
          }
     }
     else {
          asay();
     }
}

static
VOID
asay(VOID)
{
     if (margc == 1) {
          prfmsg(ASAMC0,margv[0]);
     }
     else {
          if (pfnlvl >= saypfn) {
               apmutl((SHORT)(PFNERRO+genrdn(0,NPFNMSGS)));
               prfmsg(PFNERRU+genrdn(0,NPFNMSGS));
          }
          else {
               prf("***\r%s %ss: ",alcptr->altnam,
                   (alcptr->pflags&SBURPF ? "burps" : margv[0]));
               rstrin();
               prf("%s\r",margv[1]);
               apfoth();
               if (alcptr->pflags&SBURPF) {
                    alcptr->pflags&=~SBURPF;
                    prfmsg(BURAWY);
               }
               else {
                    prfmsg(JABAWY);
               }
          }
     }
     outprf(usrnum);
}

static
VOID
awhisp(VOID)
{
     if (margc < 4) {
          gsyner();
     }
     else if (pfnlvl >= saypfn) {
          apmutl((SHORT)(PFNERRO+genrdn(0,NPFNMSGS)));
          prfmsg(PFNERRU+genrdn(0,NPFNMSGS));
     }
     else {
          if (findap(margv[2])) {
               rstrin();
               prfmsg(WHISPU,alcptr->altnam,margv[3]);
               outprf(othusn);
               prfmsg(WHISPO,alcptr->altnam,alcaux->altnam);
               apfbt2();
               prfmsg(WHISPY,alcaux->alias,(usaptr->sex == 'M' ? 'r' : 's'));
          }
          else {
               apmutl((SHORT)(NOTHRO+genrdn(0,NNHRMSGS)));
               prfmsg(NOTHRU+genrdn(0,NNHRMSGS));
          }
     }
     outprf(usrnum);
}

static
VOID
ayell(VOID)
{
     CHAR *stgptr;

     if (margc == 1) {
          prfmsg(YELLNW,alcptr->altnam,margv[0],ahisher(alcptr));
          apfoth();
          prfmsg(YELLNWN,margv[0]);
          apfnear();
          prfmsg(YELLNWU,margv[0]);
     }
     else {
          if (pfnlvl >= saypfn) {
               apmutl((SHORT)(PFNERRO+genrdn(0,NPFNMSGS)));
               prfmsg(PFNERRU+genrdn(0,NPFNMSGS));
          }
          else {
               prfmsg(YELLW1,alcptr->altnam,margv[0]);
               rstrin();
               for (stgptr=margv[1] ; *stgptr != '\0' ; stgptr++) {
                    *stgptr=(CHAR)toupper(*stgptr);
               }
               prfmsg(YELLW2,margv[1]);
               apfoth();
               prfmsg(YELLWN,margv[1]);
               apfnear();
               prfmsg(YELLWU);
          }
     }
     outprf(usrnum);
}

VOID
anorth(VOID)
{
     amvutl(aplptr->north,MOVNOR);
}

VOID
asouth(VOID)
{
     amvutl(aplptr->south,MOVSOU);
}

VOID
aeast(VOID)
{
     amvutl(aplptr->east,MOVEAS);
}

VOID
awest(VOID)
{
     amvutl(aplptr->west,MOVWES);
}


static
VOID
amvutl(SHORT dest,SHORT msgnum)
{
     if (dest == -1) {
          apmutl(MESSMO);
          prfmsg(MESSMU);
          outprf(usrnum);
     }
     else if ((alcptr->flags&SITTNG) || (alcptr->flags&LAYING)) {
          apmutl(GETUPO);
          prfmsg(GETUPU);
          outprf(usrnum);
     }
     else {
          xpaloc(msgnum);
          epaloc(dest,msgnum+1);
     }
}

static
VOID
akiss0(VOID)
{
     akissr(0,0);
}

static
VOID
akiss1(VOID)
{
     akissr(1,0);
}

static
VOID
akick0(VOID)
{
     akissr(0,1);
}

static
VOID
akick1(VOID)
{
     akissr(1,1);
}

static
VOID
akissr(SHORT mode,SHORT nasty)
{
     SHORT rndadj;

     rndadj=(SHORT)genrdn(0,NMODIFS);
     if (genrdn(0,10) < 2) {
          if (nasty) {
               nasty=(SHORT)0;
          }
          else {
               nasty=(SHORT)1;
          }
     }
     gi_bagthe();
     if (margc == 1) {
          gsyner();
     }
     else if ((aobptr=fapobj(alcptr,margv[1])) != NULL) {
          if (mode) {
               prfmsg(KISOB1,alcptr->altnam,ahisher(alcptr),nitutl(aobptr),
                             adjects[nasty][rndadj],margv[0]);
          }
          else {
               prfmsg(KISOB2,alcptr->altnam,margv[0],ahisher(alcptr),
                             nitutl(aobptr),adverbs[nasty][rndadj]);
          }
          apfoth();
          prfmsg(KISOB3,margv[0],nitutl(aobptr));
     }
     else if ((aobptr=falobj(aplptr,margv[1])) != NULL) {
          if (aobptr->flags&PERSON) {
               prfmsg(OKISSP,alcptr->altnam,aobptr->name,margv[0],
                             adverbs[nasty][rndadj],margv[0],ahimher(alcptr),
                             adverbs[(nasty ? 0 : 1)][rndadj]);

               apfoth();
               prfmsg(UKISSP,aobptr->name,margv[0],adverbs[(nasty ? 0 : 1)][rndadj]);
          }
          else {
               if (mode) {
                    prfmsg(KISOB4,alcptr->altnam,nitutl(aobptr),objloc(aplptr),
                                  adjects[nasty][rndadj],margv[0]);
               }
               else {
                    prfmsg(KISOB5,alcptr->altnam,margv[0],adjects[nasty][rndadj],
                                  nitutl(aobptr),objloc(aplptr));
               }
               apfoth();
               prfmsg(KISOB6,margv[0],nitutl(aobptr),objloc(aplptr));
          }
     }
     else if (findap(margv[1])) {
          if (mode) {
               if ((sameas(alcaux->userid,alcptr->spouse)
                  || (alcptr->pflags&WETKIS))
                  && (strcmp(margv[0],"kiss") == 0)) {
                    alcptr->pflags&=~WETKIS;
                    prfmsg(WKISS1,ahisher(alcptr),alcptr->altnam);
                    outprf(othusn);
                    prfmsg(WKISS2,ahisher(alcptr),alcptr->altnam,alcaux->altnam,
                                  ahisher(alcaux),ahisher(alcaux));
                    apfbt2();
                    prfmsg(WKISS3,alcaux->alias,
                                  ahisher(alcaux),ahisher(alcaux));
               }
               else {
                    prfmsg(KISSY1,alcptr->altnam,adjects[nasty][rndadj],
                                  margv[0]);
                    outprf(othusn);
                    prfmsg(KISSY2,alcptr->altnam,alcaux->altnam,
                                  adjects[nasty][rndadj],margv[0]);
                    apfbt2();
                    prfmsg(KISSY3,alcaux->altnam);
               }
          }
          else {
               prfmsg(KISSY4,alcptr->altnam,ahisher(alcptr),margv[0],
                             adverbs[nasty][rndadj]);
               outprf(othusn);
               prfmsg(KISSY5,alcptr->altnam,ahisher(alcptr),margv[0],
                             alcaux->altnam,adverbs[nasty][rndadj]);
               apfbt2();
               prfmsg(KISSY6,margv[0],ahimher(alcaux));
          }
     }
     else if (sameas(margv[1],"8088")) {
          if ((alcptr->pflags&WETKIS) && sameas(margv[0],"kiss") &&
              night && alctrl.rmoonc == alcptr->rsolve &&
              alctrl.amoonc == alcptr->asolve && (alcptr->flags&RDYTOS)) {
                    alcptr->pflags&=~WETKIS;
                    alcptr->comstt=1000;
                    setmbk(acmb);
                    apmutl(WINKSO);
                    prfmsg(WINKSU);
          }
          else {
               apmutl(K8088O);
               prfmsg(K8088U,(usaptr->sex == 'M' ? 'r' : 's'),alcptr->altnam,
                             margv[0]);
          }
     }
     else {
          apmutl((SHORT)(NOTHRO+genrdn(0,NNHRMSGS)));
          prfmsg(NOTHRU+genrdn(0,NNHRMSGS));
     }
     outprf(usrnum);
}

static
VOID
agold(VOID)
{
     prfmsg(URGOLD,alcptr->gold,(alcptr->gold == 1 ? "" : "s"));
     outprf(usrnum);
     apmutl(CNTGLD);
}

static
VOID
agiver(VOID)
{
     gi_bagthe();
     bagwrd("pieces");
     if (margc == 5 && sameto(margv[2],"gold") && sameas(margv[3],"to")) {
          agiveg(1,4);
     }
     else if (margc == 4 && sameto(margv[3],"gold")) {
          agiveg(2,1);
     }
     else if (margc == 3) {
          agiveo(margv[1],margv[2]);
     }
     else if (margc == 4 && sameas(margv[2],"to")) {
          agiveo(margv[3],margv[1]);
     }
     else {
          gsyner();
     }
     outprf(usrnum);
}

static
VOID
agiveg(SHORT amt,SHORT who)
{
     SHORT goldps;

     if ((goldps=(SHORT)atoi(margv[amt])) <= 0) {
          gsyner();
     }
     else if (goldps > alcptr->gold) {
          apmutl(NENOGD1);
          prfmsg(NENOGD2);
     }
     else if (!findap(margv[who])) {
          apmutl((SHORT)(NOTHRO+genrdn(0,NNHRMSGS)));
          prfmsg(NOTHRU+genrdn(0,NNHRMSGS));
     }
     else {
          alcptr->gold-=goldps;
          alcaux->gold+=goldps;
          prfmsg(GIVGOL1,alcptr->altnam,goldps,(goldps == 1 ? "" : "s"));
          outprf(othusn);
          prfmsg(GIVGOL2,alcptr->altnam,alcaux->altnam,goldps,
                         (goldps == 1 ? "" : "s"));
          apfbt2();
          prfmsg(GIVGOL3,alcaux->alias);

     }
}

static
VOID
agiveo(CHAR *givee,CHAR *givwhat)
{
     struct alcobj *obj2;

     if (!findap(givee)) {
          apmutl((SHORT)(NOTHRO+genrdn(0,NNHRMSGS)));
          prfmsg(NOTHRU+genrdn(0,NNHRMSGS));
     }
     else if (usrnum == othusn) {
          apmutl(GVOBJ01);
          prfmsg(GVOBJ02);
     }
     else if ((aobptr=fapobj(alcptr,givwhat)) == NULL) {
          apmutl(NOHOBJ1);
          prfmsg(NOHOBJ2);
     }
     else if (alcaux->naobjs >= MXPITS) {
          if (aplptr->nlobjs >= MXLITS) {
               apmutl(GVOBJ03);
               prfmsg(GVOBJ04);
          }
          else if (genrdn(0,10) < 4) {
               tapobj(alcptr,aobjno);
               palobj(aplptr,aobptr);
               prfmsg(GVOBJ05,alcptr->altnam,ahisher(alcptr),nitutl(aobptr));
               apfoth();
               prfmsg(GVOBJ06,nitutl(aobptr));
          }
          else {
               obj2=alcaux->apobjs[0];
               tapobj(alcaux,0);                 /* take from 2nd guy */
               tapobj(alcptr,aobjno);            /* take from 1st guy */
               papobj(alcaux,aobptr);            /* give 2nd's 1st's  */
               prfmsg(GVOBJ07,alcptr->altnam,margv[0],
                              (*margv[0] == 'g' ? "n" : "ed"),
                              ditutl(aobptr),obj2->name);
               outprf(othusn);
               prfmsg(GVOBJ08,alcptr->altnam,margv[0],
                              (*margv[0] == 'g' ? "n" : "ed"),
                              alcaux->altnam,ditutl(aobptr),ahimher(alcaux),
                              ahisher(alcaux),obj2->name);
               apfbt2();
               palobj(aplptr,obj2);
               prfmsg(GVOBJ09,ahimher(alcaux),ahisher(alcaux),obj2->name);
          }
     }
     else {
          tapobj(alcptr,aobjno);                    /* take from 1st guy */
          papobj(alcaux,aobptr);                   /* give 2nd's 1st's  */
          prfmsg(GVOBJ10,alcptr->altnam,margv[0],(*margv[0] == 'g' ? "n" : "ed"),
                         ditutl(aobptr));
          outprf(othusn);
          prfmsg(GVOBJ11,alcptr->altnam,margv[0],
                         (*margv[0] == 'g' ? "n" : "ed"),
                         alcaux->altnam,ditutl(aobptr));
          apfbt2();
          prfmsg(GVOBJ12,ahimher(alcaux));
     }
}

static
VOID
aget(VOID)
{
     gi_bagthe();
     if (margc == 2) {
          agetl(margv[1]);
     }
     else if (margc == 3 && sameas(margn[1]-2,"'s")) {
          *(margn[1]-2)='\0';
          agetp(margv[1],margv[2]);
     }
     else if (margc == 4 && sameto(margv[2],"from")) {
          agetp(margv[3],margv[1]);
     }
     else {
          gsyner();
     }
     outprf(usrnum);
}

static
VOID
agetp(CHAR *who,CHAR *what)
{
     if (!findap(who)) {
          apmutl((SHORT)(NOTHRO+genrdn(0,NNHRMSGS)));
          prfmsg(NOTHRU+genrdn(0,NNHRMSGS));
     }
     else if (usrnum == othusn) {
          apmutl(GETM01);
          prfmsg(GETM02);
     }
     else if ((aobptr=fapobj(alcaux,what)) == NULL) {
          apmutl(GETM03);
          prfmsg(GETM04,alcaux->alias,what);
     }
     else if (alcptr->naobjs >= MXPITS) {
          apmutl(GETM05);
          prfmsg(GETM06);
     }
     else if (genrdn(0,10) < 8) {
          prfmsg(GETM07,alcptr->altnam,margv[0],nitutl(aobptr));
          outprf(othusn);
          prfmsg(GETM08,alcptr->altnam,margv[0],alcaux->altnam,nitutl(aobptr));
          apfbt2();
          prfmsg(GETM09,(usaptr->sex == 'M' ? 'r' : 's'));
     }
     else {
          tapobj(alcaux,aobjno);
          papobj(alcptr,aobptr);
          prfmsg(GETM10,alcptr->altnam,nitutl(aobptr),ahimher(alcptr));
          outprf(othusn);
          prfmsg(GETM11,alcptr->altnam,alcaux->altnam,nitutl(aobptr),
                        ahimher(alcaux));
          apfbt2();
          prfmsg(GETM12,ahisher(alcaux),nitutl(aobptr),ahimher(alcaux));
     }
}

static
VOID
agetl(CHAR *what)
{
     if (findap(margv[1])) {
          prfmsg(GETL01,alcptr->altnam);
          outprf(othusn);
          prfmsg(GETL02,alcptr->altnam,alcaux->altnam);
          apfbt2();
          prfmsg(GETL03,aheshe(alcaux));
     }
     else if ((aobptr=falobj(aplptr,what)) == NULL) {
          apmutl(GETL04);
          prfmsg(GETL05,what,objloc(aplptr));
     }
     else if (!(aobptr->flags&PICKUP)) {
          prfmsg(GETL06,alcptr->altnam,margv[0],nitutl(aobptr));
          apfoth();
          setmbk(admb);
          prfmsg(aobptr->desc+1);
     }
     else if (alcptr->naobjs >= MXPITS) {
          apmutl(GETL07);
          prfmsg(GETL08);
     }
     else {
          talobj(aplptr,aobjno);
          papobj(alcptr,aobptr);
          prfmsg(GETL09,alcptr->altnam,aobptr->name,objloc(aplptr));
          apfoth();
          prfmsg(DUNMSG);
     }
}

static
VOID
alook(VOID)
{
     LONG flags;

     gi_bagthe();
     bagprep();
     if (margc >= 2 && (aobptr=falobj(aplptr,margv[1])) != NULL) {
          prfmsg(LOOK00,alcptr->altnam,aobptr->name,objloc(aplptr));
          apfoth();
          setmbk(admb);
          prfmsg(aobptr->desc);
          outprf(usrnum);
     }
     else if (margc >= 2 && (aobptr=fapobj(alcptr,margv[1])) != NULL) {
          prfmsg(LOOK01,alcptr->altnam,ahisher(alcptr),aobptr->name);
          apfoth();
          setmbk(admb);
          prfmsg(aobptr->desc);
          outprf(usrnum);
     }
     else if (margc >= 2 && findap(margv[1])) {
          if (usrnum != othusn) {
               prfmsg(LOOK02,alcptr->altnam);
               outprf(alcaux->modno);
          }
          prfmsg(LOOK03,alcptr->altnam,alcaux->altnam);
          apfbt2();
          setmbk(admb);
          prfmsg(alcaux->desc,alcaux->alias);
          ainutl(alcaux);
          outprf(usrnum);
     }
     else if (margc >= 2 && sameto(margv[1],"brief")) {
          apmutl(LOOK04);
          flags=alcptr->flags;
          alcptr->flags|=BRIEFF;
          dscloc();
          alcptr->flags=flags;
     }
     else {
          apmutl(LOOK05);
          flags=alcptr->flags;
          alcptr->flags&=~BRIEFF;
          dscloc();
          alcptr->flags=flags;
     }
}

static
VOID
ainv(VOID)
{
     apmutl(AINVMO);
     prfmsg(AINVMU);
     ainutl(alcptr);
     outprf(usrnum);
}

static
VOID
adrop(VOID)
{
     gi_bagthe();
     if (margc >= 2) {
          if ((aobptr=fapobj(alcptr,margv[1])) != NULL) {
               if (aplptr->nlobjs >= MXLITS) {
                    apmutl(DRPM00);
                    prfmsg(DRPM01);
               }
               else {
                    tapobj(alcptr,aobjno);
                    palobj(aplptr,aobptr);
                    prfmsg(DRPM02,alcptr->altnam,ahisher(alcptr),
                           aobptr->name,objloc(aplptr));
                    apfoth();
                    prfmsg(DRPM03);
               }
          }
          else {
               apmutl(NOHOBJ1);
               prfmsg(NOHOBJ2);
          }
     }
     else {
          gsyner();
     }
     outprf(usrnum);
}

static
VOID
asit(VOID)
{
     alcptr->flags&=~LAYING;
     alcptr->flags|=SITTNG;
     apmutl(SSLM00);
     done();
}

static
VOID
alie(VOID)
{
     alcptr->flags&=~SITTNG;
     alcptr->flags|=LAYING;
     apmutl(SSLM01);
     done();
}

static
VOID
astand(VOID)
{
     alcptr->flags&=~LAYING;
     alcptr->flags&=~SITTNG;
     apmutl(SSLM02);
     done();
}

static
VOID
aundrs(VOID)
{
     alcptr->desc=(usaptr->sex == 'M' ? AMPD01 : AFPD01);
     alcptr->seemod=9;
     apmutl(UNDRSO);
     prfmsg(UNDRSU);
     outprf(usrnum);
}

static
VOID
abrief(VOID)
{
     alcptr->flags|=BRIEFF;
     done();
}

static
VOID
aubrief(VOID)
{
     alcptr->flags&=~BRIEFF;
     done();
}

static
VOID
done(VOID)
{
     prfmsg(DUNMSG);
     outprf(usrnum);
}

static
VOID
ahelp(VOID)
{
     setmbk(admb);
     if (margc == 1) {
          prfmsg(HELP);
     }
     else {
          switch (toupper(*margv[1])) {
          case 'A':
               prfmsg(HELPA);
               break;
          case 'C':
               prfmsg(HELPC);
               break;
          case 'N':
               prfmsg(HELPN);
               break;
          case 'P':
               prfmsg(HELPP);
               break;
          case 'T':
               prfmsg(HELPT);
               break;
          default:
               prfmsg(HELP);
          }
     }
     outprf(usrnum);
}

VOID
gsyner(VOID)
{
     apmutl((SHORT)(SYNERRO+genrdn(0,NERRMSGS)));
     prfmsg(SYNERRU+genrdn(0,NERRMSGS));
}

static
VOID
asetim(VOID)
{
     if ((usrptr->flags&MASTER) && margc == 2) {
          if (*margv[1] == 'F') {
               alctrl.amoonc=alctrl.rmoonc=4;
          }
          else {
               alctrl.time=(SHORT)atoi(margv[1]);
          }
          done();
     }
     gsyner();
     outprf(usrnum);
}

static
VOID
asysob(VOID)
{
     SHORT i;

     if (margc == 2) {
          if (alcptr->naobjs < MXPITS) {
               for (i=0 ; i < NAOBJS ; i++) {
                    if (sameto(margv[1],alcobs[i].name)) {
                         papobj(alcptr,&alcobs[i]);
                         done();
                         return;
                    }
               }
          }
     }
     prfmsg(SYNERRU+genrdn(0,NERRMSGS));
     outprf(usrnum);
}

static
VOID
asggol(VOID)
{
     if (margc == 2) {
          alcptr->gold=(SHORT)atoi(margv[1]);
     }
     agold();
}

static
VOID
ahits(VOID)
{
     prf("...You have %d out of %d possible hit points right now.\r",
          alcptr->hits,alcptr->maxhits);
     outprf(usrnum);
}

static
VOID
asmcmd(SHORT off)
{
     apmutl(ASMO00+off);
     prfmsg(ASMU00+off);
     outprf(usrnum);
}

static
VOID
rubeme(VOID)
{
     if (margc == 2 && sameto(margv[1],"emerald")) {
          if ((aobptr=fapobj(alcptr,"emerald")) != NULL) {
               apfyou(EMEMSG);
               xpaloc(EMEMSG1);
               epaloc(65,EMEMSG2);
               return;
          }
     }
     mr8088();
}

#define ANYCMD -1
#define PAYONL -2
#define SYSONL -3

// This initializer works for C99 in VS2022 but not for C89/C90 in Borland 5. 

#ifdef __BUILDV10MODULE
struct acmcmd a2words[]={
     {"?",      .routine.noParam = ahelp,   ANYCMD},
     {"aim",    .routine.noParam = aaim,    PAYONL},
     {"ask",    .routine.noParam = aask,    PAYONL},
     {"blink",  .routine.withParam = asmcmd,  0},
     {"blush",  .routine.withParam = asmcmd,  1},
     {"boo",    .routine.withParam = asmcmd,  2},
     {"bow",    .routine.withParam = asmcmd,  3},
     {"breathe",.routine.noParam = abreth,  PAYONL},
     {"brief",  .routine.noParam = abrief,  ANYCMD},
     {"burp",   .routine.withParam = asmcmd,  4},
     {"cackle", .routine.withParam = asmcmd,  5},
     {"caress", .routine.noParam = akiss0,  PAYONL},
     {"cheer",  .routine.withParam = asmcmd,  6},
     {"chuckle",.routine.withParam = asmcmd,  7},
     {"clap",   .routine.withParam = asmcmd,  8},
     {"comfort",.routine.noParam = akiss0,  PAYONL},
     {"cough",  .routine.withParam = asmcmd,  9},
     {"cry",    .routine.withParam = asmcmd,  10},
     {"dance",  .routine.withParam = asmcmd,  11},
     {"drink",  .routine.noParam = adrink,  PAYONL},
     {"drop",   .routine.noParam = adrop,   PAYONL},
     {"e",      .routine.noParam = aeast,   ANYCMD},
     {"embrace",.routine.noParam = akiss0,  PAYONL},
     {"fart",   .routine.noParam = afart,   PAYONL},
     {"french", .routine.noParam = akiss0,  PAYONL},
     {"frown",  .routine.withParam = asmcmd,  12},
     {"gasp",   .routine.withParam = asmcmd,  13},
     {"get",    .routine.noParam = aget,    PAYONL},
     {"giggle", .routine.withParam = asmcmd,  14},
     {"give",   .routine.noParam = agiver,  PAYONL},
     {"gold",   .routine.noParam = agold,   ANYCMD},
     {"grin",   .routine.withParam = asmcmd,  15},
     {"groan",  .routine.withParam = asmcmd,  16},
     {"grope",  .routine.noParam = akiss0,  PAYONL},
     {"growl",  .routine.withParam = asmcmd,  17},
     {"help",   .routine.noParam = ahelp,   ANYCMD},
     {"hiss",   .routine.withParam = asmcmd,  18},
     {"hits",   .routine.noParam = ahits,   ANYCMD},
     {"hold",   .routine.noParam = akiss0,  PAYONL},
     {"howl",   .routine.withParam = asmcmd,  19},
     {"hug",    .routine.noParam = akiss1,  ANYCMD},
     {"hum",    .routine.withParam = asmcmd,  20},
     {"inv",    .routine.noParam = ainv,    ANYCMD},
     {"kick",   .routine.noParam = akick1,  ANYCMD},
     {"kiss",   .routine.noParam = akiss1,  PAYONL},
     {"laugh",  .routine.withParam = asmcmd,  21},
     {"lick",   .routine.noParam = akiss1,  PAYONL},
     {"lie",    .routine.noParam = alie,    ANYCMD},
     {"look",   .routine.noParam = alook,   ANYCMD},
     {"make",   .routine.noParam = asysob,  SYSONL},
     {"makeg",  .routine.noParam = asggol,  SYSONL},
     {"mix",    .routine.noParam = amix,    PAYONL},
     {"moan",   .routine.withParam = asmcmd,  22},
     {"n",      .routine.noParam = anorth,  ANYCMD},
     {"nod",    .routine.withParam = asmcmd,  23},
     {"pee",    .routine.withParam = asmcmd,  24},
     {"pet",    .routine.noParam = akiss0,  PAYONL},
     {"pinch",  .routine.noParam = akick0,  PAYONL},
     {"piss",   .routine.withParam = asmcmd,  24},
     {"pour",   .routine.noParam = apour,   PAYONL},
     {"pout",   .routine.withParam = asmcmd,  25},
     {"relieve",.routine.withParam = asmcmd,  24},
     {"rub",    .routine.noParam = rubeme,  PAYONL},
     {"s",      .routine.noParam = asouth,  ANYCMD},
     {"say",    .routine.noParam = asay,    ANYCMD},
     {"seduce", .routine.noParam = akiss0,  PAYONL},
     {"settime",.routine.noParam = asetim,  SYSONL},
     {"shake",  .routine.noParam = ashake,  PAYONL},
     {"shrug",  .routine.withParam = asmcmd,  26},
     {"sigh",   .routine.withParam = asmcmd,  27},
     {"sing",   .routine.withParam = asmcmd,  28},
     {"sit",    .routine.noParam = asit,    ANYCMD},
     {"slap",   .routine.noParam = akick1,  PAYONL},
     {"smile",  .routine.withParam = asmcmd,  29},
     {"smirk",  .routine.withParam = asmcmd,  30},
     {"smooch", .routine.noParam = akiss0,  PAYONL},
     {"sneeze", .routine.noParam = asneez,  PAYONL},
     {"snicker",.routine.withParam = asmcmd,  31},
     {"sniff",  .routine.withParam = asmcmd,  32},
     {"sob",    .routine.withParam = asmcmd,  33},
     {"stand",  .routine.noParam = astand,  ANYCMD},
     {"stare",  .routine.noParam = akick1,  PAYONL},
     {"state",  .routine.noParam = asay,    PAYONL},
     {"strangle",.routine.noParam = akick0, PAYONL},
     {"think",  .routine.noParam = athink,  PAYONL},
     {"tickle", .routine.noParam = akiss0,  PAYONL},
     {"touch",  .routine.noParam = akiss0,  PAYONL},
     {"unbrief",.routine.noParam = aubrief, ANYCMD},
     {"undress",.routine.noParam = aundrs,  PAYONL},
     {"w",      .routine.noParam = awest,   ANYCMD},
     {"whisper",.routine.noParam = awhisp,  ANYCMD},
     {"whistle",.routine.withParam = asmcmd,  34},
     {"wink",   .routine.noParam = akiss0,  PAYONL},
     {"yawn",   .routine.withParam = asmcmd,  35},
     {"yell",   .routine.noParam = ayell,   ANYCMD}
};

VOID initializeCommands(VOID) { }

#else
struct acmcmd a2words[95];

VOID initializeCommands(VOID) {
    a2words[0].word = "?";
    a2words[0].routine.noParam = ahelp;
    a2words[0].asmoff = ANYCMD;

    a2words[1].word = "aim";
    a2words[1].routine.noParam = aaim;
    a2words[1].asmoff = PAYONL;

    a2words[2].word = "ask";
    a2words[2].routine.noParam = aask;
    a2words[2].asmoff = PAYONL;

    a2words[3].word = "blink";
    a2words[3].routine.withParam = asmcmd;
    a2words[3].asmoff = 0;

    a2words[4].word = "blush";
    a2words[4].routine.withParam = asmcmd;
    a2words[4].asmoff = 1;

    a2words[5].word = "boo";
    a2words[5].routine.withParam = asmcmd;
    a2words[5].asmoff = 2;

    a2words[6].word = "bow";
    a2words[6].routine.withParam = asmcmd;
    a2words[6].asmoff = 3;

    a2words[7].word = "breathe";
    a2words[7].routine.noParam = abreth;
    a2words[7].asmoff = PAYONL;

    a2words[8].word = "brief";
    a2words[8].routine.noParam = abrief;
    a2words[8].asmoff = ANYCMD;

    a2words[9].word = "burp";
    a2words[9].routine.withParam = asmcmd;
    a2words[9].asmoff = 4;

    a2words[10].word = "cackle";
    a2words[10].routine.withParam = asmcmd;
    a2words[10].asmoff = 5;

    a2words[11].word = "caress";
    a2words[11].routine.noParam = akiss0;
    a2words[11].asmoff = PAYONL;

    a2words[12].word = "cheer";
    a2words[12].routine.withParam = asmcmd;
    a2words[12].asmoff = 6;

    a2words[13].word = "chuckle";
    a2words[13].routine.withParam = asmcmd;
    a2words[13].asmoff = 7;

    a2words[14].word = "clap";
    a2words[14].routine.withParam = asmcmd;
    a2words[14].asmoff = 8;

    a2words[15].word = "comfort";
    a2words[15].routine.noParam = akiss0;
    a2words[15].asmoff = PAYONL;

    a2words[16].word = "cough";
    a2words[16].routine.withParam = asmcmd;
    a2words[16].asmoff = 9;

    a2words[17].word = "cry";
    a2words[17].routine.withParam = asmcmd;
    a2words[17].asmoff = 10;

    a2words[18].word = "dance";
    a2words[18].routine.withParam = asmcmd;
    a2words[18].asmoff = 11;

    a2words[19].word = "drink";
    a2words[19].routine.noParam = adrink;
    a2words[19].asmoff = PAYONL;

    a2words[20].word = "drop";
    a2words[20].routine.noParam = adrop;
    a2words[20].asmoff = PAYONL;

    a2words[21].word = "e";
    a2words[21].routine.noParam = aeast;
    a2words[21].asmoff = ANYCMD;

    a2words[22].word = "embrace";
    a2words[22].routine.noParam = akiss0;
    a2words[22].asmoff = PAYONL;

    a2words[23].word = "fart";
    a2words[23].routine.noParam = afart;
    a2words[23].asmoff = PAYONL;

    a2words[24].word = "french";
    a2words[24].routine.noParam = akiss0;
    a2words[24].asmoff = PAYONL;

    a2words[25].word = "frown";
    a2words[25].routine.withParam = asmcmd;
    a2words[25].asmoff = 12;

    a2words[26].word = "gasp";
    a2words[26].routine.withParam = asmcmd;
    a2words[26].asmoff = 13;

    a2words[27].word = "get";
    a2words[27].routine.noParam = aget;
    a2words[27].asmoff = PAYONL;

    a2words[28].word = "giggle";
    a2words[28].routine.withParam = asmcmd;
    a2words[28].asmoff = 14;

    a2words[29].word = "give";
    a2words[29].routine.noParam = agiver;
    a2words[29].asmoff = PAYONL;

    a2words[30].word = "gold";
    a2words[30].routine.noParam = agold;
    a2words[30].asmoff = ANYCMD;

    a2words[31].word = "grin";
    a2words[31].routine.withParam = asmcmd;
    a2words[31].asmoff = 15;

    a2words[32].word = "groan";
    a2words[32].routine.withParam = asmcmd;
    a2words[32].asmoff = 16;

    a2words[33].word = "grope";
    a2words[33].routine.noParam = akiss0;
    a2words[33].asmoff = PAYONL;

    a2words[34].word = "growl";
    a2words[34].routine.withParam = asmcmd;
    a2words[34].asmoff = 17;

    a2words[35].word = "help";
    a2words[35].routine.noParam = ahelp;
    a2words[35].asmoff = ANYCMD;

    a2words[36].word = "hiss";
    a2words[36].routine.withParam = asmcmd;
    a2words[36].asmoff = 18;

    a2words[37].word = "hits";
    a2words[37].routine.noParam = ahits;
    a2words[37].asmoff = ANYCMD;

    a2words[38].word = "hold";
    a2words[38].routine.noParam = akiss0;
    a2words[38].asmoff = PAYONL;

    a2words[39].word = "howl";
    a2words[39].routine.withParam = asmcmd;
    a2words[39].asmoff = 19;

    a2words[40].word = "hug";
    a2words[40].routine.noParam = akiss1;
    a2words[40].asmoff = ANYCMD;

    a2words[41].word = "hum";
    a2words[41].routine.withParam = asmcmd;
    a2words[41].asmoff = 20;

    a2words[42].word = "inv";
    a2words[42].routine.noParam = ainv;
    a2words[42].asmoff = ANYCMD;

    a2words[43].word = "kick";
    a2words[43].routine.noParam = akick1;
    a2words[43].asmoff = ANYCMD;

    a2words[44].word = "kiss";
    a2words[44].routine.noParam = akiss1;
    a2words[44].asmoff = PAYONL;

    a2words[45].word = "laugh";
    a2words[45].routine.withParam = asmcmd;
    a2words[45].asmoff = 21;

    a2words[46].word = "lick";
    a2words[46].routine.noParam = akiss1;
    a2words[46].asmoff = PAYONL;

    a2words[47].word = "lie";
    a2words[47].routine.noParam = alie;
    a2words[47].asmoff = ANYCMD;

    a2words[48].word = "look";
    a2words[48].routine.noParam = alook;
    a2words[48].asmoff = ANYCMD;

    a2words[49].word = "make";
    a2words[49].routine.noParam = asysob;
    a2words[49].asmoff = SYSONL;

    a2words[50].word = "makeg";
    a2words[50].routine.noParam = asggol;
    a2words[50].asmoff = SYSONL;

    a2words[51].word = "mix";
    a2words[51].routine.noParam = amix;
    a2words[51].asmoff = PAYONL;

    a2words[52].word = "moan";
    a2words[52].routine.withParam = asmcmd;
    a2words[52].asmoff = 22;

    a2words[53].word = "n";
    a2words[53].routine.noParam = anorth;
    a2words[53].asmoff = ANYCMD;

    a2words[54].word = "nod";
    a2words[54].routine.withParam = asmcmd;
    a2words[54].asmoff = 23;

    a2words[55].word = "pee";
    a2words[55].routine.withParam = asmcmd;
    a2words[55].asmoff = 24;

    a2words[56].word = "pet";
    a2words[56].routine.noParam = akiss0;
    a2words[56].asmoff = PAYONL;

    a2words[57].word = "pinch";
    a2words[57].routine.noParam = akick0;
    a2words[57].asmoff = PAYONL;

    a2words[58].word = "piss";
    a2words[58].routine.withParam = asmcmd;
    a2words[58].asmoff = 24;

    a2words[59].word = "pour";
    a2words[59].routine.noParam = apour;
    a2words[59].asmoff = PAYONL;

    a2words[60].word = "pout";
    a2words[60].routine.withParam = asmcmd;
    a2words[60].asmoff = 25;

    a2words[61].word = "relieve";
    a2words[61].routine.withParam = asmcmd;
    a2words[61].asmoff = 24;

    a2words[62].word = "rub";
    a2words[62].routine.noParam = rubeme;
    a2words[62].asmoff = PAYONL;

    a2words[63].word = "s";
    a2words[63].routine.noParam = asouth;
    a2words[63].asmoff = ANYCMD;

    a2words[64].word = "say";
    a2words[64].routine.noParam = asay;
    a2words[64].asmoff = ANYCMD;

    a2words[65].word = "seduce";
    a2words[65].routine.noParam = akiss0;
    a2words[65].asmoff = PAYONL;

    a2words[66].word = "settime";
    a2words[66].routine.noParam = asetim;
    a2words[66].asmoff = SYSONL;

    a2words[67].word = "shake";
    a2words[67].routine.noParam = ashake;
    a2words[67].asmoff = PAYONL;

    a2words[68].word = "shrug";
    a2words[68].routine.withParam = asmcmd;
    a2words[68].asmoff = 26;

    a2words[69].word = "sigh";
    a2words[69].routine.withParam = asmcmd;
    a2words[69].asmoff = 27;

    a2words[70].word = "sing";
    a2words[70].routine.withParam = asmcmd;
    a2words[70].asmoff = 28;

    a2words[71].word = "sit";
    a2words[71].routine.noParam = asit;
    a2words[71].asmoff = ANYCMD;

    a2words[72].word = "slap";
    a2words[72].routine.noParam = akick1;
    a2words[72].asmoff = PAYONL;

    a2words[73].word = "smile";
    a2words[73].routine.withParam = asmcmd;
    a2words[73].asmoff = 29;

    a2words[74].word = "smirk";
    a2words[74].routine.withParam = asmcmd;
    a2words[74].asmoff = 30;

    a2words[75].word = "smooch";
    a2words[75].routine.noParam = akiss0;
    a2words[75].asmoff = PAYONL;

    a2words[76].word = "sneeze";
    a2words[76].routine.noParam = asneez;
    a2words[76].asmoff = PAYONL;

    a2words[77].word = "snicker";
    a2words[77].routine.withParam = asmcmd;
    a2words[77].asmoff = 31;

    a2words[78].word = "sniff";
    a2words[78].routine.withParam = asmcmd;
    a2words[78].asmoff = 32;

    a2words[79].word = "sob";
    a2words[79].routine.withParam = asmcmd;
    a2words[79].asmoff = 33;

    a2words[80].word = "stand";
    a2words[80].routine.noParam = astand;
    a2words[80].asmoff = ANYCMD;

    a2words[81].word = "stare";
    a2words[81].routine.noParam = akick1;
    a2words[81].asmoff = PAYONL;

    a2words[82].word = "state";
    a2words[82].routine.noParam = asay;
    a2words[82].asmoff = PAYONL;

    a2words[83].word = "strangle";
    a2words[83].routine.noParam = akick0;
    a2words[83].asmoff = PAYONL;

    a2words[84].word = "think";
    a2words[84].routine.noParam = athink;
    a2words[84].asmoff = PAYONL;

    a2words[85].word = "tickle";
    a2words[85].routine.noParam = akiss0;
    a2words[85].asmoff = PAYONL;

    a2words[86].word = "touch";
    a2words[86].routine.noParam = akiss0;
    a2words[86].asmoff = PAYONL;

    a2words[87].word = "unbrief";
    a2words[87].routine.noParam = aubrief;
    a2words[87].asmoff = ANYCMD;

    a2words[88].word = "undress";
    a2words[88].routine.noParam = aundrs;
    a2words[88].asmoff = PAYONL;

    a2words[89].word = "w";
    a2words[89].routine.noParam = awest;
    a2words[89].asmoff = ANYCMD;

    a2words[90].word = "whisper";
    a2words[90].routine.noParam = awhisp;
    a2words[90].asmoff = ANYCMD;

    a2words[91].word = "whistle";
    a2words[91].routine.withParam = asmcmd;
    a2words[91].asmoff = 34;

    a2words[92].word = "wink";
    a2words[92].routine.noParam = akiss0;
    a2words[92].asmoff = PAYONL;

    a2words[93].word = "yawn";
    a2words[93].routine.withParam = asmcmd;
    a2words[93].asmoff = 35;

    a2words[94].word = "yell";
    a2words[94].routine.noParam = ayell;
    a2words[94].asmoff = ANYCMD;

}
#endif

#define CARSIZ (sizeof(a2words)/sizeof(struct acmcmd))

static struct acmcmd *
a2csch(CHAR *stgptr,struct acmcmd table[],SHORT length)
{
     SHORT cond;
     struct acmcmd *low,*mid,*high;

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

static VOID
mc0msg(VOID)
{
     static SHORT count=0;

     prf(alcatr);
     if (usrptr->flags&INJOIP) {
          prfmsg(MC0INJ);
     }
     else {
          prfmsg(MC0M00+count);
          count++;
          if (count > 4) {
               count=0;
          }
     }
     outprf(usrnum);
}

VOID
acmhdl(VOID)
{
     CHAR *mv0ptr;

     if (!margc) {
          mc0msg();
     }
     else {
          for (mv0ptr=margv[0] ; *mv0ptr != '\0' ; mv0ptr++) {
               *mv0ptr=(CHAR)tolower(*mv0ptr);
          }
          aplptr=&alocs[alcptr->curloc];
          //if (!(*(alrous[alcptr->curloc].routine))()) {
          if (!alrous[alcptr->curloc].routine()) {
               if (!chkacm()) {
                    mr8088();
               }
          }
     }
}

static SHORT
chkacm(VOID)
{
     struct acmcmd *cmd;

     if ((cmd=a2csch(margv[0],a2words,CARSIZ)) != NULL) {
          if ((cmd->asmoff == PAYONL && !hasmkey(AL2KEY)) &&
              (!usrptr->flags&MASTER)) {
               prfmsg(PTOPLY);
               outprf(usrnum);
          }
          else if (cmd->asmoff == SYSONL && !(usrptr->flags&MASTER)) {
               prfmsg(ONLYSY);
               outprf(usrnum);
          }
          else if (cmd->asmoff > ANYCMD) {
               //(*(cmd->routine))(cmd->asmoff);
              cmd->routine.withParam(cmd->asmoff);
          }
          else {
               //(*(cmd->routine))();
              cmd->routine.noParam();
          }
          return(1);
     }
     return(0);
}
