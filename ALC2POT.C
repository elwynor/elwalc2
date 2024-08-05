/*****************************************************************************
 *                                                                           *
 *   ALC2POT.C                                                      V3.11    *
 *                                                                           *
 *   ALCHEMY II: Object-routine array, potion combination/item routines.     *
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

static VOID creatp(SHORT nmixed);
static VOID nomix(SHORT which);
static VOID nhvpot(SHORT which);
static VOID mtrobl(SHORT which);
static SHORT poblow(struct alcobj *obj);
static struct alcobj *ck2mix(VOID);
static struct alcobj *ck3mix(VOID);
static VOID useait(SHORT mask);
static VOID finger(VOID);
static VOID telepf(VOID);
static VOID zapaho(VOID);
static VOID lightb(VOID);
static VOID firfin(VOID);
static VOID zappro(VOID);
static VOID uglycf(VOID);
static VOID pelvis(VOID);
static VOID nakedf(VOID);
static VOID foot(VOID);
static VOID icey1f(VOID);
static VOID ligbal(VOID);
static VOID threem(SHORT basmsg);
static SHORT ck4pro(SHORT pro,CHAR *aimwht);
static VOID hitall(SHORT damage,SHORT pro,SHORT basmsg,SHORT hitslf,SHORT seemod);
static VOID fkysnd(VOID);
static VOID aimnow(CHAR *aimwht);
static VOID incmht(SHORT max);
static VOID heal(SHORT amt);
static SHORT fndwrd(CHAR *word);

#define FLBDAM      14        /* amount of damage done by lightning bolt   */
#define FFIDAM       8        /* amount of damage done by fire finger      */
#define IC1DAM      12        /* amount of damage done by foot ice bolt    */
#define LBLDAM      16        /* amount of damage done by lightning ball   */
#define FARDAM       4        /* amount of damage done by stink bomb fart  */
#define BTHDAM       8        /* amount of damage done by fire breath      */
#define ICSDAM      10        /* amount of damage done by ice storm        */
#define FRSDAM       8        /* amount of damage done by fire storm       */

static
struct alcobj *mixitm[3],*result;

VOID
amix(VOID)
{
     gi_bagthe();
     bagwrd("potion");
     bagwrd("and");
     bagwrd("with");
     if (margc < 3) {
          gsyner();
          outprf(usrnum);
     }
     else if ((mixitm[0]=fapobj(alcptr,margv[1])) != NULL) {
          tapobj(alcptr,aobjno);
          if (mixitm[0]->flags&POTION) {
               if ((mixitm[1]=fapobj(alcptr,margv[2])) != NULL) {
                    tapobj(alcptr,aobjno);
                    if (mixitm[1]->flags&POTION) {
                         if (margc > 3) {
                              if ((mixitm[2]=fapobj(alcptr,margv[3])) != NULL) {
                                   tapobj(alcptr,aobjno);
                                   if (mixitm[2]->flags&POTION) {
                                        if ((result=ck3mix()) != NULL) {
                                             creatp(3);
                                        }
                                        else {
                                             mtrobl(3);
                                        }
                                   }
                                   else {
                                        nomix(3);
                                   }
                              }
                              else {
                                   nhvpot(3);
                              }
                         }
                         else {
                              if ((result=ck2mix()) != NULL) {
                                   creatp(2);
                              }
                              else {
                                   mtrobl(2);
                              }
                         }
                    }
                    else {
                         nomix(2);
                    }
               }
               else {
                    nhvpot(2);
               }
          }
          else {
               nomix(1);
          }
     }
     else {
          nhvpot(1);
     }
}

static VOID
creatp(SHORT nmixed)
{
     papobj(alcptr,result);
     switch (nmixed) {
     case 2:
          prfmsg(CREP2O,alcptr->altnam,ditutl(result));
          apfoth();
          prfmsg(CREP2U,ditutl(result));
          break;
     case 3:
          prfmsg(CREP3O,alcptr->altnam,ditutl(result));
          apfoth();
          prfmsg(CREP3U,ditutl(result));
          break;
     }
     outprf(usrnum);
}

static VOID
nomix(SHORT which)
{
     SHORT i;

     for (i=which-1 ; i >= 0 ; i--) {
          papobj(alcptr,mixitm[i]);
     }
     prfmsg(NOMIXO,alcptr->altnam,ahisher(alcptr),nitutl(mixitm[which-1]));
     apfoth();
     prfmsg(NOMIXU,nitutl(mixitm[which-1]));
     outprf(usrnum);
}

static VOID
nhvpot(SHORT which)
{
     SHORT i;

     for (i=which-2 ; i >= 0 ; i--) {
          papobj(alcptr,mixitm[i]);
     }
     apmutl(NHVPOO);
     prfmsg(NHVPOU,margv[which]);
     outprf(usrnum);
}

static VOID
mtrobl(SHORT which)
{
     SHORT blown=0,i;

     for (i=which-1 ; i >= 0 ; i--) {
          if ((mixitm[i]->flags&BLOWUP) && genrdn(0,5) == 5) {
               blown=1;
               if (poblow(mixitm[i])) {      /* stop cycle?  */
                    return;
               }
          }
          else {
               papobj(alcptr,mixitm[i]);
          }
     }
     if (!blown) {
          apmutl(POBLOO);
          prfmsg(POBLOU);
          outprf(usrnum);
     }
}

static SHORT
poblow(struct alcobj *obj)
{
     SHORT retval=0;

     switch (genrdn(0,4)) {
     case 0:
          prfmsg(POBM0O,alcptr->altnam,nitutl(obj));
          apfoth();
          prfmsg(POBM0U,nitutl(obj),PXPDAM);
          outprf(usrnum);
          if (hitap(alcptr,PXPDAM)) {
               retval=1;
          }
          else {
               alcptr->seemod=7;
          }
          break;
     case 1:
          prfmsg(POBM1O,alcptr->altnam);
          apfoth();
          prfmsg(POBM1U,nitutl(obj));
          outprf(usrnum);
          break;
     case 2:
          prfmsg(POBM2O,alcptr->altnam);
          apfoth();
          prfmsg(POBM2U,nitutl(obj));
          outprf(usrnum);
          alcptr->seemod=4;
          break;
     case 3:
          prfmsg(POBM3O,alcptr->altnam);
          apfoth();
          prfmsg(POBM3U,nitutl(obj));
          outprf(usrnum);
          alcptr->naobjs=0;
          retval=1;
     }
     return(retval);
}

static struct alcobj *
ck2mix(VOID)
{
     SHORT *p1ms,*p2ms;

     if (mixitm[0] != mixitm[1]) {
          for (p1ms=mixitm[0]->mix2s ; *p1ms != -1 ; p1ms++) {
               for (p2ms=mixitm[1]->mix2s ; *p2ms != -1 ; p2ms++) {
                    if (*p1ms == *p2ms) {
                         return(&alcobs[*p1ms]);
                    }
               }
          }
     }
     return(NULL);
}

static struct alcobj *
ck3mix(VOID)
{
     SHORT *p1ms,*p2ms,*p3ms;

     if (mixitm[0] != mixitm[1] && mixitm[1] != mixitm[2] && mixitm[0] != mixitm[2]) {
          for (p1ms=mixitm[0]->mix3s ; *p1ms != -1 ; p1ms++) {
               for (p2ms=mixitm[1]->mix3s ; *p2ms != -1 ; p2ms++) {
                    for (p3ms=mixitm[2]->mix3s ; *p3ms != -1 ; p3ms++) {
                         if (*p1ms == *p2ms && *p2ms == *p3ms) {
                              return(&alcobs[*p1ms]);
                         }
                    }
               }
          }
     }
     return(NULL);
}

VOID
athink(VOID)
{
     bagwrd("of");
     bagwrd("about");
     if (margc == 1) {
          gsyner();
          outprf(usrnum);
     }
     else if ((alcptr->pflags&SCRYFL) && pinalc(margv[1])) {
          apmutl(ATHKM0);
          prfmsg(ATHKM1);
          setmbk(almb);
          prfmsg(alocs[alcaux->curloc].desc+1,"mystically");
          setmbk(amb);
          apfyou(ATHKM2);
          alcptr->pflags&=~SCRYFL;
          prfmsg(ATHKM3);
          outprf(alcaux->modno);
     }
     else {
          apmutl(ATHKM4);
          apfyou(ATHKM5);
     }
}

VOID
adrink(VOID)
{
     if (margc == 1) {
          gsyner();
          outprf(usrnum);
     }
     else if ((aobptr=fapobj(alcptr,margv[1])) == NULL) {
          apmutl(NOHOBJ1);
          apfyou(NOHOBJ2);
     }
     else if (!(aobptr->flags&POTION)) {
          apmutl(NODRKO);
          apfyou(NODRKU);
     }
     else if (alcptr->peectr > 11) {
          apmutl(CNTDP0);
          apfyou(CNTDP1);
     }
     else {
          alcptr->peectr++;
          apmutl(DRKPOT);
          setmbk(admb);
          apfyou(aobptr->desc+1);       /* +1  replaces  + AUXPOM */
          tapobj(alcptr,aobjno);
          if (aobptr->flags&DRKROU) {
               setmbk(amb);
               //(*(obrous[(SHORT)(aobptr-alcobs)].routine))();
               obrous[(SHORT)(aobptr - alcobs)].routine();
          }
     }
}

VOID
apour(VOID)
{
     useait(PORABL);
}

VOID
ashake(VOID)
{
     useait(SHKABL);
}

static VOID
useait(SHORT mask)
{
     if (margc == 1) {
          gsyner();
          outprf(usrnum);
     }
     else if ((aobptr=fapobj(alcptr,margv[1])) == NULL) {
          apmutl(NOHOBJ1);
          apfyou(NOHOBJ2);
     }
     else if (!(aobptr->flags&mask)) {
          apmutl(NOPORO);
          apfyou(NOPORU);
     }
     else {
          if (*margv[0] == 'p') {
               tapobj(alcptr,aobjno);
          }
          //(*(obrous[(SHORT)(aobptr-alcobs)].routine))();
          obrous[(SHORT)(aobptr - alcobs)].routine();
     }
}

VOID
afart(VOID)
{
     bagprep();
     if (margc == 1) {
          apmutl(AFRTM0);
          apfyou(AFRTM1);
     }
     else if (!findap(margv[1])) {
          apmutl((SHORT)(NOTHRO+genrdn(0,NNHRMSGS)));
          apfyou((SHORT)(NOTHRU+genrdn(0,NNHRMSGS)));
     }
     else if (alcptr->pflags&STINKB) {
          threem(AFRTM2);
          alcptr->pflags&=~STINKB;
          hitap(alcaux,FARDAM);
     }
     else {
          apmutl(AFRTM5);
          apfyou(AFRTM6);
     }
}

VOID
asneez(VOID)
{
     bagprep();
     if (margc == 1) {
          apmutl(ASNZM0);
          apfyou(ASNZM1);
     }
     else if (!findap(margv[1])) {
          apmutl((SHORT)(NOTHRO+genrdn(0,NNHRMSGS)));
          apfyou((SHORT)(NOTHRU+genrdn(0,NNHRMSGS)));
     }
     else if (alcptr->pflags&SNEEZF) {
          threem(ASNZM2);
          alcptr->pflags&=~SNEEZF;
          if (aplptr->nlobjs < MXLITS && alcaux->naobjs > 0) {
               prfmsg(ASNZM5,alcaux->apobjs[0]->name);
               outprf(alcaux->modno);
               prfmsg(ASNZM6,alcaux->altnam,nitutl(alcaux->apobjs[0]),ahisher(alcaux));
               apfbt2();
               outprf(usrnum);
               palobj(aplptr,alcaux->apobjs[0]);
               tapobj(alcaux,0);
          }
     }
     else {
          apmutl(ASNZM7);
          apfyou(ASNZM8);
     }
}

VOID
abreth(VOID)
{
     bagprep();
     if (alcptr->pflags&BREATH) {
          alcptr->pflags&=~BREATH;
          apmutl(BRTHM0);
          apfyou(BRTHM1);
          hitall(BTHDAM,FIRPRO,BRTHM2,0,8);
     }
     else {
          apmutl(BRTHM5);
          apfyou(BRTHM6);
     }
}

VOID
aaim(VOID)
{
     SHORT i;

     bagprep();
     if (margc < 3) {
          gsyner();
          outprf(usrnum);
     }
     else if (sameto(margv[1],"finger")) {
          finger();
     }
     else if (sameto(margv[1],"pelvis")) {
          pelvis();
     }
     else if (sameto(margv[1],"foot")) {
          foot();
     }
     else if ((aobptr=fapobj(alcptr,margv[1])) == NULL) {
          apmutl(NOHOBJ1);
          apfyou(NOHOBJ2);
     }
     else if (aobptr->flags&AIMABL) {
          if ((i=(SHORT)(aobptr-alcobs)) < 0 || i > 102) {
          }
          //(*(obrous[(SHORT)(aobptr-alcobs)].routine))();
          obrous[(SHORT)(aobptr - alcobs)].routine();
     }
     else {
          mr8088();
     }
}

static VOID
finger(VOID)
{
     if (!findap(margv[2])) {
          apmutl((SHORT)(NOTHRO+genrdn(0,NNHRMSGS)));
          apfyou((SHORT)(NOTHRU+genrdn(0,NNHRMSGS)));
     }
     else if (alcptr->pflags&TELEPF) {
          telepf();
     }
     else if (alcptr->pflags&ZAPAHO) {
          zapaho();
     }
     else if (alcptr->pflags&LIGHTB) {
          lightb();
     }
     else if (alcptr->pflags&FIRFIN) {
          firfin();
     }
     else if (alcptr->pflags&ZAPPRO) {
          zappro();
     }
     else if (alcptr->pflags&UGLYCF) {
          uglycf();
     }
     else {
          aimnow("finger");
     }
}

static VOID
telepf(VOID)
{
     INT tmpusn;
     struct aplayr *tmpplr;

     prfmsg(AIMFIN,alcptr->altnam,ahisher(alcptr),alcaux->altnam);
     apfbt2();
     prfmsg(TELM01,alcptr->altnam,ahisher(alcptr));
     outprf(alcaux->modno);
     prfmsg(TELM02,ahimher(alcaux));
     outprf(usrnum);
     tmpplr=alcptr;
     alcptr=alcaux;
     tmpusn=usrnum;
     usrnum=alcptr->modno;
     xpaloc(TELM03);
     epaloc((SHORT)(genrdn(41,166)),TELM04);
     alcptr=tmpplr;
     usrnum=tmpusn;
     alcptr->pflags&=~TELEPF;
}

static VOID
zapaho(VOID)
{
     if (!ck4pro(OTHPRO,"finger")) {
          alcaux->naobjs=0;
          threem(ZAHOM0);
          alcptr->pflags&=~ZAPAHO;
     }
}

static VOID
lightb(VOID)
{
     if (!ck4pro(LIGPRO,"finger")) {
          threem(LIGBM0);
          alcptr->pflags&=~LIGHTB;
          hitap(alcaux,FLBDAM);
     }
}

static VOID
firfin(VOID)
{
     if (!ck4pro(FIRPRO,"finger")) {
          threem(FFINM0);
          alcptr->pflags&=~FIRFIN;
          hitap(alcaux,FFIDAM);
     }
}

static VOID
zappro(VOID)
{
     SHORT i;

     threem(ZAPPM0);
     for (i=0 ; i < NPCNTS ; i++) {
          alcaux->counts[i]=0;
     }
     alcptr->pflags&=~ZAPPRO;
}

static VOID
uglycf(VOID)
{
     threem(UGLYM0);
     alcaux->seemod=3;
     alcptr->pflags&=~UGLYCF;
}

static VOID
pelvis(VOID)
{
     if (!findap(margv[2])) {
          apmutl((SHORT)(NOTHRO+genrdn(0,NNHRMSGS)));
          apfyou((SHORT)(NOTHRU+genrdn(0,NNHRMSGS)));
     }
     else if (alcptr->pflags&NAKEDF) {
          nakedf();
     }
     else {
          aimnow("pelvis");
     }
}

static VOID
nakedf(VOID)
{
     threem(NAKDM0);
     alcaux->seemod=9;
     alcaux->desc=(uacoff(alcaux->modno)->sex == 'M' ? AMPD01 : AFPD01);
     alcptr->pflags&=~NAKEDF;
}

static VOID
foot(VOID)
{
     if (!findap(margv[2])) {
          apmutl((SHORT)(NOTHRO+genrdn(0,NNHRMSGS)));
          apfyou((SHORT)(NOTHRU+genrdn(0,NNHRMSGS)));
     }
     else if (alcptr->pflags&ICEY1F) {
          icey1f();
     }
     else if (alcptr->pflags&LIGBAL) {
          ligbal();
     }
     else {
          aimnow("foot");
     }
}

static VOID
icey1f(VOID)
{
     if (!ck4pro(ICEPRO,"foot")) {
          threem(ICEFM0);
          alcptr->pflags&=~ICEY1F;
          hitap(alcaux,IC1DAM);
     }
}

static VOID
ligbal(VOID)
{
     if (!ck4pro(LIGPRO,"foot")) {
          threem(LBALM0);
          alcptr->pflags&=~LIGBAL;
          hitap(alcaux,LBLDAM);
     }
}

static VOID
threem(SHORT basmsg)
{
     prfmsg(basmsg,alcptr->altnam,ahisher(alcptr),alcaux->altnam);
     apfbt2();
     prfmsg(basmsg+1,alcptr->altnam,ahisher(alcptr));
     outprf(alcaux->modno);
     prfmsg(basmsg+2,aheshe(alcaux));
     outprf(usrnum);
}

static SHORT
ck4pro(SHORT pro,CHAR *aimwht)
{
     if (alcaux->counts[pro]) {
          aimnow(aimwht);
          return(1);
     }
     return(0);
}

static VOID
hitall(SHORT damage,SHORT pro,SHORT basmsg,SHORT hitslf,SHORT seemod)
{
     SHORT i;

     for (alcaux=alcarr,i=0 ; i < nterms ; alcaux++,i++) {
          if (alcptr->curloc == alcaux->curloc) {
               if (hitslf || alcaux != alcptr) {
                    if (alcaux->counts[pro]) {
                         prfmsg(basmsg);
                         outprf(i);
                    }
                    else {
                         if (seemod != 0) {
                              alcaux->seemod=seemod;
                         }
                         prfmsg(basmsg+1);
                         outprf(i);
                         prfmsg(basmsg+2,alcaux->altnam);
                         fkysnd();
                         hitap(alcaux,damage);
                    }
               }
          }
     }
}

static VOID
fkysnd(VOID)
{
     INT hldusn;
     struct aplayr *tmpplr;

     tmpplr=alcptr;
     hldusn=usrnum;
     alcptr=alcaux;
     usrnum=alcaux->modno;
     apfoth();
     alcptr=tmpplr;
     usrnum=hldusn;
}

static VOID
aimnow(CHAR *aimwht)
{
     prfmsg(CK4PM0,alcptr->altnam,ahisher(alcptr),aimwht,alcaux->altnam);
     apfbt2();
     prfmsg(CK4PM1,alcptr->altnam,ahisher(alcptr),aimwht);
     outprf(alcaux->modno);
     prfmsg(CK4PM2,ahimher(alcaux));
     outprf(usrnum);
}

VOID
pot01(VOID)
{
     alcptr->pflags|=SBURPF;
}

VOID
pot02(VOID)
{
     alcptr->pflags|=SNEEZF;
}

VOID
pot05(VOID)
{
     alcptr->macros=-30;
}

VOID
pot07(VOID)
{
     incmht(20);
}

VOID
pot08(VOID)
{
     heal(5);
}

VOID
pot10(VOID)
{
     killap(alcptr);
}

VOID
pot11(VOID)
{
     alcptr->pflags|=STINKB;
}

VOID
pot12(VOID)
{
     alcptr->rouctr=5;
     alcptr->approu=hiccup;
}

VOID
pot13(VOID)
{
     CHAR *camnth();

     prfmsg(DADATE,alctrl.day,camnth(),alctrl.year);
     outprf(usrnum);
}

VOID
pot14(VOID)
{
     alcptr->counts[FIRPRO]=8;
}

VOID
pot15(VOID)
{
     prfmsg(THISRM,alcptr->curloc);
     outprf(usrnum);
}

VOID
pot16(VOID)
{
     alcptr->pflags|=UGLYCF;
}

VOID
pot18(VOID)
{
     alcptr->counts[CINVIS]=6;
}

VOID
pot20(VOID)
{
     if (alcptr->spouse[0] != '\0') {
          prfmsg(SPODRM,alcptr->spouse);
     }
     else {
          prfmsg(NSPOTD);
     }
     outprf(usrnum);
}

VOID
pot22(VOID)
{
     incmht(30);
}

VOID
pot23(VOID)
{
     alcptr->flags&=~SITTNG;
     alcptr->flags|=LAYING;
     apmutl(SSLM01);
}

VOID
pot24(VOID)
{
     alcptr->counts[LIGPRO]=8;
}

VOID
pot25(VOID)
{
     btuinj(usrnum,RING);
}

VOID
pot26(VOID)
{
     alcptr->pflags|=TELEPF;
}

VOID
pot27(VOID)
{
     alcptr->pflags|=FIRFIN;
}

VOID
pot29(VOID)
{
     apmutl(PICNOS);
     alcptr->seemod=(usaptr->sex == 'M' ? 5 : 6);
}

VOID
pot30(VOID)
{
     alcptr->pflags|=BREATH;
}

VOID
pot31(VOID)
{
     alcptr->pflags|=WETKIS;
}

VOID
pot32(VOID)
{
     alcptr->pflags|=SCRYFL;
}

VOID
pot33(VOID)
{
     alcptr->counts[OTHPRO]=8;
}

VOID
pot34(VOID)
{
     apmutl(ICSM00);
     hitall(ICSDAM,ICEPRO,ICSM01,1,0);
}

VOID
pot35(VOID)
{
     apmutl(FRSM00);
     hitall(FRSDAM,FIRPRO,FRSM01,1,0);
}

VOID
pot36(VOID)
{
     papobj(alcptr,&alcobs[genrdn(0,50)]);
}

VOID
pot37(VOID)
{
     alcptr->maxhits-=8;
     hitap(alcptr,8);
}

VOID
pot38(VOID)
{
     alcptr->pflags|=LIGHTB;
}

VOID
pot39(VOID)
{
     apmutl(GONINV);
     apfyou(UGOINV);
     alcptr->flags|=INVISF;
     alcptr->counts[INVISI]=6;
     strcpy(alcptr->altnam,"An Unseen Being");
}

VOID
pot40(VOID)
{
     alcptr->pflags|=ZAPPRO;
}

VOID
pot41(VOID)
{
     heal(30);
}

VOID
pot42(VOID)
{
     alcptr->pflags|=ZAPAHO;
}

VOID
pot43(VOID)
{
     apmutl(UBCORV);
     alcptr->counts[INVISI]=8;
     strcpy(alcptr->altnam,"Corvadam");
}

VOID
pot44(VOID)
{
     apmutl(UBKYRA);
     alcptr->counts[INVISI]=8;
     strcpy(alcptr->altnam,"Kyrannalussi");
}

VOID
pot45(VOID)
{
     apmutl(UBMELL);
     alcptr->counts[INVISI]=8;
     strcpy(alcptr->altnam,"Lady Meling");
}

VOID
pot46(VOID)
{
     incmht(40);
}

VOID
pot47(VOID)
{
     apmutl(UBSHAR);
     alcptr->counts[INVISI]=8;
     strcpy(alcptr->altnam,"Sharibor");
}

VOID
pot48(VOID)
{
     alcptr->pflags|=ICEY1F;
}

VOID
pot51(VOID)
{
     alcptr->pflags|=LIGBAL;
}

VOID
pot52(VOID)
{
     alcptr->counts[FIRPRO]=8;
     alcptr->counts[LIGPRO]=8;
     alcptr->counts[ICEPRO]=8;
     alcptr->counts[OTHPRO]=8;
}

VOID
pot54(VOID)
{
     alcptr->pflags|=NAKEDF;
}

VOID
pot61(VOID)
{
     prfmsg(TRUCHN,trucols[alcptr->color]);
     outprf(usrnum);
}

static VOID
incmht(SHORT max)
{
     if (alcptr->maxhits < max) {
          alcptr->maxhits=max;
     }
}

static VOID
heal(SHORT amt)
{
     alcptr->hits+=amt;
     if (alcptr->hits > alcptr->maxhits) {
          alcptr->hits=alcptr->maxhits;
     }
}

VOID
obj085(VOID)
{
     if (fndwrd("potions?")) {
          apfyou(OB85M0);
     }
     else if (fndwrd("truths?")) {
          apfyou(OB85M1);
     }
     else if (fndwrd("colors?")) {
          apfyou(OB85M2);
     }
     else {
          apfyou(OB85M3);
     }
}

VOID
obj086(VOID)
{
     apfyou(OB86M0);
}

VOID
obj087(VOID)
{
     apfyou(OB87M0);
}

VOID
obj088(VOID)
{
     if (fndwrd("truths?")) {
          apfyou(OB88M0);
     }
     else if (fndwrd("sex?")) {
          apfyou(OB88M1);
     }
     else {
          apfyou(OB88M2);
     }
}

VOID
obj089(VOID)
{
     if (fndwrd("truths?")) {
          apfyou(OB89M0);
     }
     else {
          apfyou(OB89M1);
     }
}

VOID
obj090(VOID)
{
     apfyou(OB90M0);
}

VOID
obj091(VOID)
{
     if (fndwrd("sex?")) {
          apfyou(OB91M0);
     }
     else if (fndwrd("potions?")) {
          apfyou(OB91M1);
     }
     else {
          apfyou(OB91M2);
     }
}

VOID
obj092(VOID)
{
     apfyou(OB92M0);
}

VOID
obj093(VOID)
{
     apfyou(OB93M0);
}

VOID
obj094(VOID)
{
     if (fndwrd("kyrannalussi?")) {
          apfyou(OB94M0);
     }
     else if (fndwrd("potions?")) {
          apfyou(OB94M1);
     }
     else if (fndwrd("truths?")) {
          apfyou(OB94M2);
     }
     else if (fndwrd("love?")) {
          apfyou(OB94M3);
     }
     else {
          apfyou(OB94M4);
     }
}

static SHORT
fndwrd(CHAR *word)
{
     SHORT i;

     for (i=2 ; i < margc ; i++) {
          if (sameto(margv[i],word)) {
               return(1);
          }
     }
     return(0);
}
