/*****************************************************************************
 *                                                                           *
 *   ALC2RSF.C                                                      V3.11    *
 *                                                                           *
 *   ALCHEMY II: Room-specific  game structures, definitions, and externs.   *
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

static SHORT ispace(SHORT givmsg);
static SHORT goldck(CHAR *stg);
static SHORT entbath(CHAR sexch);
static VOID honutl(SHORT msgbas);
static SHORT pplant(CHAR *itmstg,SHORT itmnum,SHORT msgbas);
static SHORT istruth(CHAR *stg,CHAR *ans,SHORT truidx);
static VOID poolut(SHORT loc,SHORT off);
static VOID ponped(SHORT pedestal);

struct comcmd saycmds[3]={
     {"answer", NULL},
     {"say",    NULL},
     {"state",  NULL}
};
struct comcmd piccmds[3]={
     {"get",    NULL},
     {"pick",   NULL},
     {"take",   NULL}
};
struct comcmd peecmds[3]={
     {"pee",    NULL},
     {"piss",   NULL},
     {"relieve",NULL}
};
struct comcmd drpcmds[4]={
     {"drop",   NULL},
     {"place",  NULL},
     {"put",    NULL},
     {"stick",  NULL}
};

#define speak()     (a2ccmp(margv[0],saycmds,3) != NULL)
#define pick()      (a2ccmp(margv[0],piccmds,3) != NULL)
#define pee()       (a2ccmp(margv[0],peecmds,3) != NULL)
#define drop()      (a2ccmp(margv[0],drpcmds,4) != NULL)
#define answer()    (strcmp(margv[0],"answer") == 0)
#define give()      (strcmp(margv[0],"give") == 0)
#define ask()       (strcmp(margv[0],"ask") == 0)
#define about()     (sameas(margv[2],"about") == 0)

extern
SHORT bskbal;                      /* basketball status, on or off         */
extern
SHORT lfgopn;                      /* gate at lit forest, open or closed   */
extern
SHORT domes[6];                    /* current status of dome pedestals     */

static SHORT
ispace(SHORT givmsg)
{
     if (alcptr->naobjs < MXPITS) {
          return(1);
     }
     if (givmsg) {
          apmutl(ISPM00);
          apfyou(ISPM01);
     }
     return(0);
}

static SHORT
goldck(CHAR *stg)
{
     SHORT ret;

     bagwrd("to");
     if (give() && sameto(margv[2],"gold") && sameto(margv[3],stg)) {
          if ((ret=(SHORT)atoi(margv[1])) > 0) {
               if (ret <= alcptr->gold) {
                    return(ret);
               }
               apmutl(GCKM00);
               apfyou(GCKM01);
               return(-1);
          }
          apmutl(GCKM02);
          apfyou(GCKM03);
          return(-1);
     }
     return(0);
}

SHORT
amagics(VOID)
{
     SHORT g;

     if ((g=goldck("wizard")) != 0) {
          switch (g) {
          case -1:
               break;
          case 100:
               if (ispace(0)) {
                    alcptr->gold-=g;
                    papobj(alcptr,&alcobs[83]);
                    apmutl(OMSM00);
                    apfyou(OMSM01);
                    break;
               }
          default:
               apmutl(OMSM02);
               apfyou(OMSM03);
          }
          return(1);
     }
     return(0);
}

SHORT
rocker(VOID)
{
#define LIMESTONE  75
#define LIMEVALUE   4
#define SANDSTONE  77
#define SANDVALUE   2
#define SOAPSTONE  76
#define SOAPVALUE   3
#define COALROCKS  78
#define COALVALUE   1
     bagwrd("to");
     if (give() && margc > 2 && sameto(margv[3],"collector")) {
          if ((aobptr=fapobj(alcptr,margv[1])) != NULL) {
               apmutl(ROCM00);
               switch (aobptr-alcobs) {
               case LIMESTONE:
                    apfyou(ROCM01);
                    tapobj(alcptr,aobjno);
                    alcptr->gold+=LIMEVALUE;
                    break;
               case SANDSTONE:
                    apfyou(ROCM02);
                    tapobj(alcptr,aobjno);
                    alcptr->gold+=SANDVALUE;
                    break;
               case SOAPSTONE:
                    apfyou(ROCM03);
                    tapobj(alcptr,aobjno);
                    alcptr->gold+=SOAPVALUE;
                    break;
               case COALROCKS:
                    apfyou(ROCM04);
                    tapobj(alcptr,aobjno);
                    alcptr->gold+=COALVALUE;
                    break;
               default:
                    apfyou(ROCM05);
               }
          }
          else {
               apmutl(NOHOBJ1);
               apfyou(NOHOBJ2);
          }
          return(1);
     }
     return(0);
}

SHORT
pospla(VOID)
{
     SHORT g;

     if ((g=goldck("po")) != 0) {
          switch (g) {
          case -1:
               break;
          case 3:
               if (ispace(0)) {
                    alcptr->gold-=g;
                    papobj(alcptr,&alcobs[6]);
                    apmutl(POSM00);
                    apfyou(POSM01);
                    break;
               }
          default:
               apmutl(POSM02);
               apfyou(POSM03);
          }
          return(1);
     }
     return(0);
}

SHORT
kyrapr(VOID)
{
     SHORT g;

     if ((g=goldck("kyrapractor")) != 0) {
          switch (g) {
          case -1:
               break;
          default:
               alcptr->gold-=g;
               alcptr->hits+=g;
               if (alcptr->hits > alcptr->maxhits) {
                    alcptr->hits=alcptr->maxhits;
               }
               apmutl(KYPM00);
               apfyou(KYPM01);
          }
          return(1);
     }
     return(0);
}

SHORT
generl(VOID)
{
     SHORT g;

     if ((g=goldck("shopkeeper")) != 0) {
          switch (g) {
          case -1:
               break;
          case 1:
               if (ispace(0)) {
                    alcptr->gold-=g;
                    papobj(alcptr,&alcobs[4]);
                    apmutl(GENM00);
                    apfyou(GENM01);
                    break;
               }
          case 2:
               if (ispace(0)) {
                    alcptr->gold-=g;
                    papobj(alcptr,&alcobs[7]);
                    apmutl(GENM06);
                    apfyou(GENM07);
                    break;
               }
          case 25:
               if (ispace(0)) {
                    alcptr->gold-=g;
                    papobj(alcptr,&alcobs[82]);
                    apmutl(GENM02);
                    apfyou(GENM03);
                    break;
               }
          default:
               apmutl(GENM04);
               apfyou(GENM05);
          }
          return(1);
     }
     return(0);
}

SHORT
jcpunn(VOID)
{
     SHORT g;

     if ((g=goldck("Jc")) != 0) {
          if (g > 0 && g < 9) {
               alcptr->gold-=g;
               alcptr->desc=AMPD01+(g*2)+(usaptr->sex == 'M' ? 0 : 1);
               apmutl(JCPM00);
               apfyou(JCPM01);
          }
          else if (g > 9) {
               apmutl(JCPM02);
               apfyou(JCPM03);
          }
          return(1);
     }
     return(0);
}

SHORT
barten(VOID)
{
     SHORT g;

     if ((g=goldck("bartender")) != 0) {
          switch (g) {
          case -1:
               break;
          case 1:
               apmutl(BARM00);
               apfyou(BARM01);
               break;
          default:
               if (ispace(0)) {
                    alcptr->gold-=g;
                    papobj(alcptr,&alcobs[1]);
                    apmutl(BARM02);
                    apfyou(BARM03);
               }
               else {
                    apmutl(BARM04);
                    apfyou(BARM05);
               }
          }
          if (g > 4) {
               apfyou((SHORT)(BARM06+genrdn(0,5)));
          }
          return(1);
     }
     return(0);
}

SHORT
oracle(VOID)
{
     SHORT g;

     if ((g=goldck("oracle")) != 0) {
          switch (g) {
          case -1:
               return(1);
          case 1:
               apfyou(ORAM01);
               break;
          case 3:
               apfyou(ORAM02);
               break;
          case 4:
               apfyou(ORAM03);
               break;
          case 9:
               apfyou(ORAM04);
               break;
          case 12:
               apfyou(ORAM05);
               break;
          case 18:
               apfyou(ORAM06);
               break;
          case 30:
               apfyou(ORAM07);
               break;
          default:
               apfyou(ORAM08);
               break;
          }
          alcptr->gold-=g;
          apmutl(ORAM00);
          return(1);
     }
     return(0);
}

SHORT
altar(VOID)
{
     if (strcmp(margv[0],"marry") == 0 && margc > 1) {
          if (findap(margv[1])) {
               if (alcptr->flags&MARRID) {
                    apmutl(ALTM02);
                    apfyou(ALTM03);
               }
               else {
                    prfmsg(ALTM04,alcptr->altnam,ahimher(alcptr),
                         alcaux->altnam);
                    apfbt2();
                    prfmsg(ALTM05,alcptr->altnam,ahimher(alcptr));
                    outprf(alcaux->modno);
                    prfmsg(ALTM06,alcaux->alias);
                    outprf(usrnum);
                    strcpy(alcptr->spouse,alcaux->alias);
                    alcptr->flags|=MARRID;
               }
          }
          else {
               apmutl((SHORT)(NOTHRO+genrdn(0,NNHRMSGS)));
               apfyou((SHORT)(NOTHRU+genrdn(0,NNHRMSGS)));
          }
          return(1);
     }
     if (strcmp(margv[0],"pray") == 0) {
          apfyou(ALTM07);
          return(1);
     }
     if (strcmp(margv[0],"offer") == 0 && margc > 1) {
          if ((aobptr=fapobj(alcptr,margv[1])) != NULL) {
               tapobj(alcptr,aobjno);
               apfyou(ALTM08);
          }
          else {
               apfyou(ALTM09);
          }
          apmutl(ALTM10);
          return(1);
     }
     return(0);
}

SHORT
sapool(VOID)
{
     gi_bagthe();
     bagprep();
     bagwrd("puddle");
     if (strcmp(margv[0],"look") == 0 && sameas(margv[1],"sewage") && night) {
          apmutl(SAPM00);
          prfmsg(SAPM01,trucols[alcptr->color]);
          outprf(usrnum);
          return(1);
     }
     return(0);
}

SHORT
omens(VOID)
{
     return(entbath('M'));
}

SHORT
owomens(VOID)
{
     return(entbath('F'));
}

static SHORT
entbath(CHAR sexch)
{
     if (strcmp(margv[0],"e") == 0) {
          if (usaptr->sex != sexch) {
               apmutl(ENBM00);
               apfyou(ENBM01);
          }
          else {
               aeast();
          }
          return(1);
     }
     return(0);
}

SHORT
peeher(VOID)
{
     if (pee()) {
          if (alcptr->peectr == 0) {
               apmutl(PEEM00);
               apfyou(PEEM01);
          }
          else {
               alcptr->peectr=0;
               apmutl(PEEM02);
               apfyou(PEEM03);
          }
          return(1);
     }
     return(0);
}

SHORT
gatlok(VOID)
{
#define FCFDAM 25
     SHORT i;

     if (strcmp(margv[0],"enter") == 0) {
          for (i=0 ; i < NLCMBS ; i++) {
               if (atoi(margv[i+1]) != alcptr->lcombo[i]) {
                    break;
               }
          }
          if (i != NLCMBS) {
               apmutl(LGTM00);
               apfyou(LGTM01);
          }
          else {
               aplptr->east=40;
               aeast();
               aplptr->east=-1;
          }
          return(1);
     }
     if (strcmp(margv[0],"w") == 0) {
          apmutl(LGTM02);
          apfyou(LGTM03);
          hitap(alcptr,FCFDAM);
     }
     return(0);
}

SHORT
crystb(VOID)
{
     gi_bagthe();
     bagprep();
     if (strcmp(margv[0],"rub") == 0 && sameas(margv[1],"crystal")) {
          apmutl(CRSM00);
          apfyou(CRSM01);
          return(1);
     }
     if (strcmp(margv[0],"look") == 0 && sameas(margv[1],"crystal")) {
          apmutl(CRSM02);
          apfyou(CRSM03);
          return(1);
     }
     return(0);
}

SHORT
scentr(VOID)
{
     if (night && speak() && sameas(margv[1],"dinkdink")) {
          apfyou(SCEM00);
          xpaloc(SCEM01);
          epaloc(17,SCEM02);
          return(1);
     }
     return(0);
}

SHORT
ffdoor(VOID)
{
     if (strcmp(margv[0],"w") == 0) {
          apfyou(FFDM01);
          xpaloc(FFDM02);
          epaloc((SHORT)(genrdn(41,166)),FFDM03);
          return(1);
     }
     return(0);
}

SHORT
honeym(VOID)
{
     if (strcmp(margv[0],"whoopee") == 0) {
          if (margc > 1 && findap(margv[1])) {
               if (alcptr->desc == AMPD01 || alcptr->desc == AFPD01) {
                    if (alcaux->desc == AMPD01 || alcaux->desc == AFPD01) {
                         honutl(HONM00);
                    }
                    else {
                         honutl(HONM03);
                    }
               }
               else {
                    honutl(HONM06);
               }
          }
          else {
               apmutl((SHORT)(NOTHRO+genrdn(0,NNHRMSGS)));
               apfyou((SHORT)(NOTHRU+genrdn(0,NNHRMSGS)));
          }
          return(1);
     }
     return(0);
}

static VOID
honutl(SHORT msgbas)
{
     prfmsg(msgbas,alcptr->altnam,alcaux->altnam);
     apfbt2();
     prfmsg(msgbas+1,alcptr->altnam);
     outprf(alcaux->modno);
     prfmsg(msgbas+2,alcaux->alias);
     outprf(usrnum);
}

SHORT
dicfur(VOID)
{
     if (strcmp(margv[0],"rub") == 0) {
          if (sameto(margv[1],"dicfurs")) {
               apmutl(DICM00);
               apfyou(DICM01);
               return(1);
          }
     }
     return(0);
}

static SHORT
pplant(CHAR *itmstg,SHORT itmnum,SHORT msgbas)
{
     gi_bagthe();
     if (pick() && sameto(margv[1],itmstg)) {
          if (ispace(1)) {
               apmutl(msgbas);
               apfyou(msgbas+1);
               papobj(alcptr,&alcobs[itmnum]);
          }
          return(1);
     }
     return(0);
}

SHORT
barfo(VOID)
{
     return(pplant("barfo",3,BARPMB));
}

SHORT
buffoo(VOID)
{
     return(pplant("buffoon",5,BUFPMB));
}

SHORT
blahpl(VOID)
{
     return(pplant("blah",0,BLAPMB));
}

SHORT
booger(VOID)
{
     return(pplant("booger",2,BOOPMB));
}

SHORT
emeral(VOID)
{
     return(pplant("emerald",80,EMEPMB));
}

SHORT
coaler(VOID)
{
     return(pplant("coalrock",78,COAPMB));
}

SHORT
quartf(VOID)
{
     return(pplant("quartz",79,QUAPMB));
}

SHORT
lime(VOID)
{
     if (strcmp(margv[0],"dig") == 0) {
          if (genrdn(0,10) > 6 && alcptr->naobjs < MXPITS) {
               apmutl(LIMM00);
               apfyou(LIMM01);
               papobj(alcptr,&alcobs[75]);
          }
          else {
               apmutl(LIMM02);
               apfyou(LIMM03);
          }
          return(1);
     }
     return(0);
}

SHORT
litfor(VOID)
{
     if (!lfgopn && speak() && sameas(margv[1],usaptr->psword)) {
          apfyou(LTFM00);
          lfgopn=1;
          prfmsg(LFGOPN);
          apfloc(59);
          prfmsg(LFGOPN1);
          apfloc(179);
          alocs[59].flags|=ALTDSC;
          alocs[179].flags|=ALTDSC;
          alocs[179].south=59;
          alocs[59].north=179;
          return(1);
     }
     return(0);
}

SHORT
hygine(VOID)
{
#define SOSDAM 5
     gi_bagthe();
     if (strcmp(margv[0],"kick") == 0 && margc != 1) {
          if (sameto(margv[1],"tree") || sameas(margv[1],"hygine")) {
               if (aplptr->nlobjs < MXLITS) {
                    palobj(aplptr,&alcobs[76]);
                    apmutl(HYGM00);
                    apfyou(HYGM01);
                    hitap(alcptr,SOSDAM);
               }
               else {
                    apmutl(HYGM02);
                    apfyou(HYGM03);
               }
               return(1);
          }
     }
     return(0);
}

SHORT
wishwe(VOID)
{
     gi_bagthe();
     bagprep();
     if (drop() && margc > 2 && sameas(margv[2],"well")) {
          apmutl(WIWM00);
          apfyou(WIWM01);
          return(1);
     }
     return(0);
}

SHORT
pstump(VOID)
{
     gi_bagthe();
     bagprep();
     if (drop() && margc > 2 && sameas(margv[2],"stump")) {
          apmutl(PSTM00);
          if (sameas(margv[1],"dick") && usaptr->sex == 'M') {
               apfyou(PSTM02);
               killap(alcptr);
          }
          else {
               apfyou(PSTM01);
          }
          return(1);
     }
     return(0);
}

SHORT
apond(VOID)
{
     gi_bagthe();
     bagprep();
     if (strcmp(margv[0],"jump") == 0 || strcmp(margv[0],"dive") == 0 && margc > 1) {
          if (sameas(margv[1],"pond")) {
               if (alcptr->seemod == 8) {
                    alcptr->seemod=7;
                    apmutl(PONDM0);
                    apfyou(PONDM1);
               }
               else {
                    apmutl(PONDM2);
                    apfyou(PONDM3);
               }
               return(1);
          }
     }
     return(0);
}

SHORT
gigbee(VOID)
{
     if (strcmp(margv[0],"sniff") == 0) {
          alcptr->approu=halluc;
          alcptr->rouctr=3;
          alcptr->seemod=2;
          apmutl(GIGM00);
          apfyou(GIGM01);
          return(1);
     }
     return(0);
}

SHORT
kyrats(VOID)
{
     if (speak() && sameas(margv[1],"my") && margc > 2) {
          if (sameas(margv[2],"favorite")) {
               if (sameas(margv[3],"drink")) {
                    return(istruth(margv[5],alcobs[alcptr->favpot].name,0));
               }
               if (sameas(margv[3],"goddess")) {
                    return(istruth(margv[5],"Kyrannalussi",1));
               }
               if (sameas(margv[3],"color")) {
                    return(istruth(margv[5],trucols[alcptr->color],2));
               }
               if (sameas(margv[3],"pastime")) {
                    return(istruth(margv[5],"whoopee",3));
               }
          }
          if (sameas(margv[2],"truename")) {
               return(istruth(margv[4],alcptr->trunam,4));
          }
          if (sameas(margv[2],"birthmonth")) {
               return(istruth(margv[4],amnths[alcptr->month],5));
          }
          if (sameas(margv[2],"sweetheart")) {
               return(istruth(margv[4],alcptr->spouse,6));
          }
          if (sameas(margv[2],"truthnumber")) {
               return(istruth(margv[4],l2as((LONG)(alcptr->trunum)),7));
          }
          apmutl(KYSM00);
          apfyou(KYSM01);
          return(1);
     }
     if (strcmp(margv[0],"pray") == 0) {
          apmutl(KYSM07);
          apfyou(KYSM08);
          return(1);
     }
     return(0);
}

static SHORT
istruth(CHAR *stg,CHAR *ans,SHORT truidx)
{
     if (sameas(stg,ans) && alcptr->truths[truidx] != 1) {
          apmutl(KYSM02);
          if (++alcptr->ntrugt == NTRUTH) {
               prfmsg(KYSM06,alcptr->alias,alcptr->trunam);
          }
          else {
               prfmsg(KYSM03,alcptr->alias,alcptr->ntrugt);
          }
          outprf(usrnum);
          alcptr->truths[truidx]=1;
          return(1);
     }
     if (++alcptr->nstate > 3) {
          apmutl(KYSM04);
          apfyou(KYSM05);
          killap(alcptr);
          return(1);
     }
     apmutl(KYSM00);
     apfyou(KYSM01);
     return(1);
}

SHORT
toxic(VOID)
{
     if (strcmp(margv[0],"push") == 0 || strcmp(margv[0],"shove") == 0) {
          if (margc > 1 && findap(margv[1])) {
               prfmsg(TOXM00,alcptr->altnam,alcaux->altnam);
               apfbt2();
               prfmsg(TOXM01,alcptr->altnam);
               outprf(alcaux->modno);
               prfmsg(TOXM02,ahimher(alcaux));
               outprf(usrnum);
               alcaux->seemod=4;
          }
          else {
               apmutl((SHORT)(NOTHRO+genrdn(0,NNHRMSGS)));
               apfyou((SHORT)(NOTHRU+genrdn(0,NNHRMSGS)));
          }
          return(1);
     }
     return(0);
}

SHORT
bludra(VOID)
{
     if (strcmp(margv[0],"hug") == 0 || strcmp(margv[0],"comfort") == 0) {
          if (sameto(margv[1],"dragon")) {
               if (alcptr->naobjs < MXPITS) {
                    apmutl(BLUM00);
                    apfyou(BLUM01);
                    papobj(alcptr,&alcobs[81]);
               }
               else {
                    apmutl(BLUM00);
                    apfyou(BLUM02);
               }
               return(1);
          }
     }
     return(0);
}

static
CHAR *stxans[5]={
     "whoopee",
     "triangle",
     "Kyrannalussi",
     "Meling",
     "emerald"
};

SHORT
stinx(VOID)
{
     if (!(alcptr->flags&PSTINX) && strcmp(margv[0],"e") == 0) {
          apmutl(STNM00);
          prfmsg(STNM01);
          prfmsg(SRIDL0+alcptr->stinx);
          outprf(usrnum);
          return(1);
     }
     if (answer() && margc > 1) {
          if (sameas(margv[1],stxans[alcptr->stinx])) {
               alcptr->flags|=PSTINX;
               apmutl(STNM02);
               apfyou(STNM03);
          }
          else {
               apmutl(STNM04);
               apfyou(STNM05);
               killap(alcptr);
          }
          return(1);
     }
     return(0);
}

SHORT
wheel(VOID)
{
     SHORT i;

     if (strcmp(margv[0],"spin") == 0 && sameas(margv[1],"wheel")) {
          apmutl(WHLOTH);
          apfyou((SHORT)(WHLM00+(i=(SHORT)genrdn(0,12))));
          switch (i) {
          case 0:
               killap(alcptr);
               break;
          case 1:
               alcptr->naobjs=0;
               break;
          case 2:
               alcptr->gold=0;
               break;
          case 3:
               xpaloc(WHLMXP);
               epaloc((SHORT)(genrdn(41,166)),WHLMEP);
               break;
          default:
               if (i > 9 && alcptr->naobjs < MXPITS) {
                    apfyou(WGETPO);
                    papobj(alcptr,&alcobs[genrdn(0,50)]);
               }
               break;
          }
          return(1);
     }
     return(0);
}

SHORT
mirror(VOID)
{
     gi_bagthe();
     bagwrd("is");
     bagwrd("on");
     bagwrd("of");
     if (speak() && sameto(margv[1],"mirror,") && sameto(margv[2],"mirror,")) {
          if (sameto(margv[3],"wall,") && sameto(margv[5],"fairest") &&
              sameto(margv[6],"them") && sameto(margv[7],"all?")) {
               prfmsg(MIRANS,alcptr->trunam);
               outprf(usrnum);
               return(1);
          }
     }
     return(0);
}

SHORT
throne(VOID)
{
     if (strcmp(margv[0],"e") == 0) {
          if (alcptr->pflags&TWALKE) {
               aeast();
          }
          else {
               apfyou(THRM00);
               killap(alcptr);
          }
          return(1);
     }
     return(0);
}

SHORT
lkdoor(VOID)
{
     gi_bagthe();
     bagwrd("on");
     if (strcmp(margv[0],"knock") == 0 && sameas(margv[1],"door")) {
          apfyou(LKDM00);
          xpaloc(LKDM01);
          epaloc(210,LKDM02);
          return(1);
     }
     return(0);
}

SHORT
apools(VOID)
{
     gi_bagthe();
     bagwrd("in");
     bagwrd("into");
     if (strcmp(margv[0],"jump") == 0) {
          if (sameas(margv[1],"pool") && margc == 2) {
               apfyou(POOL00);
               apmutl(POOL01);
          }
          else if (sameas(margv[2],"pool") && margc == 3) {
               switch (toupper(*margv[1])) {
               case 'B':
                    if (toupper(margv[1][2]) == 'U') {
                         poolut(169,0);
                    }
                    else {
                         poolut(170,4);
                    }
                    break;
               case 'P':
                    poolut(176,1);
                    break;
               case 'W':
                    poolut(65,5);
                    break;
               case 'G':
                    poolut(55,3);
                    break;
               case 'Y':
                    poolut(19,2);
                    break;
               default:
                    apfyou(POOL02);
                    apmutl(POOL01);
               }
          }
          else {
               apfyou(POOL03);
               apmutl(POOL01);
          }
          return(1);
     }
     return(0);
}

static VOID
poolut(SHORT loc,SHORT off)
{
     if (off == alcptr->color) {
          loc=alcptr->slvloc;
     }
     xpaloc(POOL04+off);
     epaloc(loc,POOL10+off);
}

SHORT
basket(VOID)
{
     gi_bagthe();
     if (strcmp(margv[0],"rub") == 0 && sameto(margv[1],"basketball")) {
          bskbal=1;
          apfyou(BSKM00);
          apmutl(BSKM01);
          return(1);
     }
     return(0);
}

SHORT
boobie(VOID)
{
     if (strcmp(margv[0],"n") == 0) {
          if (alcptr->flags&BOOBIE) {
               anorth();
          }
          else {
               apmutl(BOOB00);
               apfyou(BOOB01);
               killap(alcptr);
          }
          return(1);
     }
     if ((strcmp(margv[0],"suck") == 0 ||
         strcmp(margv[0],"fondle") == 0 ||
         strcmp(margv[0],"lick") == 0) &&
         (sameto(margv[1],"breasts") || sameto(margv[1],"boobies"))) {
          apmutl(BOOB02);
          apfyou(BOOB03);
          killap(alcptr);
          return(1);
     }
     if (speak() && sameas(margv[1],"trojan")) {
          apmutl(BOOB04);
          apfyou(BOOB05);
          alcptr->flags|=BOOBIE;
          return(1);
     }
     return(0);
}

SHORT
wdome(VOID)
{
     SHORT i;

     gi_bagthe();
     if (margc > 3 && drop() && (aobptr=fapobj(alcptr,margv[1])) != NULL) {
          if (margc != 5 || !sameas(margv[4],"pedestal")) {
               apfyou(PEDHUH);
          }
          else {
               switch (toupper(margv[3][0])) {
               case 'B':
                    if (toupper(margv[3][2]) == 'U') {
                         ponped(0);
                    }
                    else {
                         ponped(1);
                    }
                    break;
               case 'G':
                    ponped(2);
                    break;
               case 'P':
                    ponped(3);
                    break;
               case 'Y':
                    ponped(4);
                    break;
               case 'W':
                    ponped(5);
                    break;
               default:
                    apfyou(NOTPED);
                    return(1);
               }
               for (i=0 ; i < 6 ; i++) {
                    if (domes[i] == 0) {
                         break;
                    }
               }
               if (i == 6) {
                    xpaloc(GOPICX);
                    epaloc(221,GOPICE);
                    alcptr->pickle=1;
                    for (i=0 ; i < 6 ; i++) {
                         domes[i]=0;
                    }
               }
          }
          return(1);
     }
     return(0);
}

static VOID
ponped(SHORT pedestal)
{
     struct pedob {
          SHORT objidx;
          CHAR *color;
     } pedobs[6]={
          {81,   "blue"},
          {78,   "black"},
          {75,   "green"},
          {79,   "pink"},
          {62,   "yellow"},
          {80,   "white"}
     };

     tapobj(alcptr,aobjno);
     apfyou(PONPM0);
     if (aobptr == &alcobs[pedobs[pedestal].objidx]) {
          prfmsg(PONPM1,pedobs[pedestal].color);
          apfloc(alcptr->curloc);
          domes[pedestal]=1;
     }
}

SHORT
pickle(VOID)
{
     gi_bagthe();
     if ((speak() || strcmp(margv[0],"yell") == 0) && sameas(margv[1],"hold")
       && sameas(margv[2],"pickle") && alcptr->naobjs < MXPITS) {
          apfyou(UGPIKL);
          papobj(alcptr,&alcobs[74]);
          alcptr->pickle=0;
          xpaloc(GOPICX);
          epaloc(221,GOPICE);
          return(1);
     }
     return(0);
}

SHORT
mrslot(VOID)
{
     gi_bagthe();
     bagwrd("potion");
     if (margc > 3 && drop() && (aobptr=fapobj(alcptr,margv[1])) != NULL &&
        sameas(margv[3],"slot")) {
          tapobj(alcptr,aobjno);
          apmutl(MRSLM0);
          if (aobptr-alcobs == alcptr->potctr) {
               if (++alcptr->potctr == 75) {
                    papobj(alcptr,&alcobs[84]);
                    apfyou(MRSLM1);
               }
               else {
                    apfyou(MRSLM2);
               }
          }
          else {
               apfyou(MRSLM3);
          }
          return(1);
     }
     return(0);
}

SHORT
pnkcor(VOID)
{
     gi_bagthe();
     if (margc == 4 &&strcmp(margv[0],"aim") == 0 && sameas(margv[1],"wand")
        && sameas(margv[3],"wall")) {
          if ((aobptr=fapobj(alcptr,margv[1])) != NULL) {
               apfyou(PNKCM0);
               xpaloc(PNKCM1);
               epaloc(224,PNKCM2);
               return(1);
          }
     }
     return(0);
}

SHORT
cofcre(VOID)
{
     if (sameas(margv[0],"Kyra,") && sameas(margv[1],"I") &&
         sameas(margv[2],"love") && sameto(margv[3],"you,") &&
         sameas(margv[4],"across") && sameas(margv[5],"time") &&
         sameas(margv[6],"and") && sameto(margv[7],"space,") &&
         sameas(margv[8],"forever.")) {
          apfyou(COFCM0);
          xpaloc(COFCM1);
          epaloc(223,COFCM2);
          return(1);
     }
     return(0);
}

SHORT
cofmoo(VOID)
{
     gi_bagthe();
     if (margc > 3 && drop() && (aobptr=fapobj(alcptr,margv[1])) != NULL &&
        sameas(margv[3],"niche")) {
          apmutl(COFMM0);
          if (aobptr == &alcobs[84]) {
               tapobj(alcptr,aobjno);
               alcptr->asolve=alcptr->rsolve=4;        /* full moon   */
               apfyou(COFMM1);
          }
          else {
               apfyou(COFMM2);
          }
          return(1);
     }
     return(0);
}

