/*****************************************************************************
 *                                                                           *
 *   ALC2.H                                                         V3.11    *
 *                                                                           *
 *   ALCHEMY II: Header file of game structures, definitions, and externs.   *
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

#include "gcomm.h"
#include "majorbbs.h"
#include "bbsutils.h"
#include "gamutils.h"
#include "elwalc2m.h"
#include "elwalc2l.h"
#include "elwalc2d.h"
#include "elwalc2c.h"

#define VERSION "3.11"   /* v3.1 - R. Hadsall 4/2/2021 v3.11 - RH 8/4/2024 */

#define HOURSPD 21600    /* hours per day, in seconds  (hrs*3600)          */
#define NDAYSPM    24    /* number of days per month                       */
#define NMONTHS     6    /* number of months in the year (see amnths[])    */
#define FIRSTYR   750    /* what year (A.D.) the game begins               */
#define NMSTGS      8    /* number of stage in moons cycles                */
#define CTLRTK     30    /* real-time kick interval for game control stuff */
#define RISTIM      0    /* time of alctrl.time when the sun rises/moon set*/
#define SETTIM  10800    /* time of alctrl.time when the sun sets/moon rise*/
#define ALSSIZ     10    /* alias name size for data structure             */

extern
struct alctrl {               /* structure defintion of game control file  */
     CHAR key[4];             /*   key of "key"                            */ // 000-003 KEY
     SHORT time;              /*   time of the day, in seconds 1-21600     */ // 004-005
     SHORT day;               /*   day of the month, 0-23                  */ // 006-007
     SHORT month;             /*   month of the year, 0-5                  */ // 008-009
     SHORT year;              /*   year, starting at 750 A.D.              */ // 010-011
     SHORT rmoonc;            /*   moon cycle of Ristina (0-7)             */ // 012-013
     SHORT amoonc;            /*   moon cycle of Amary (0-7)               */ // 014-015
     SHORT stropn;            /*   strange building is open                */ // 016-017
     SHORT weather;           /*   weather variable of the sun             */ // 018-019
     CHAR spare[56-20];       /*   spare space, decrease when adding       */ // 020-055
} alctrl;                     /* occurance of game control file            */ // size 56

#define NALOCS    225    /* number of locations defined for the game       */
#define MXLITS      6    /* maximum number of items in a game location     */

#define LONGM       1    /* amount added for long description message      */
#define NIGHTM      2    /* amount added for night description message     */
#define GROUNDM     3    /* amount added for ground description message    */

extern
struct aloc {
     SHORT desc;              /*   room's brief description (msg number)   */
     SHORT flags;             /*   room's bit "flags"                      */
     SHORT nlobjs;            /*   number of items currently in the room   */
     struct alcobj *alobjs[MXLITS];/*pointers to items in the room         */
     SHORT north;             /*   location number to the north            */
     SHORT south;             /*   location number to the south            */
     SHORT east;              /*   location number to the east             */
     SHORT west;              /*   location number to the west             */
} *alocs,                     /* dynamically allocated array of locations  */
  *aplptr;                    /* pointer to current player's location stuff*/

typedef SHORT(*LocRou)(VOID);

extern
struct locrou {
    LocRou routine;           /*   room-specific routine address           */
} alrous[NALOCS];

#define SHOSKY      1    /* status of the sky is shown (sun, moons) here   */
#define ALDARK      2    /* this room is always "dark", regardless of sky  */
#define ALLIGH      4    /* this room is always "light", regardless of sky */
#define ALTDSC      8    /* alternate descriptions are now active          */
#define DRKDSC     16    /* alternate descriptions are used for darkness   */
#define CITIES     32    /* city streets, for purpose of lighting          */

#define ANSIZE     18    /* maximum size for player's alterate name        */
#define TRUSIZ     10    /* maximum size of player's "true name"           */
#define NLCMBS      3    /* number of digits for lock combination solve    */
#define MXPITS      6    /* maximum number of items a player can hold      */
#define NPCNTS      6    /* number of counters of player timed flags       */
#define NCSTGS     10    /* number of seestgs[] for aplayr.seemod          */
#define MXHITS    250    /* maximum hit points a character can ever have   */
#define STALOC      0    /* "starting location" for initap()               */
#define STAHTS     10    /* "starting" hit points (and max hit points)     */
#define NTRUCS      6    /* number of different "true colors"              */
#define NTRUTH      8    /* number of "truths" the player must discover    */
                         /*   0 = favorite drink (potion)                  */
                         /*   1 = favorite goddess                         */
                         /*   2 = favorite color                           */
                         /*   3 = favorite pasttime                        */
                         /*   4 = truename                                 */
                         /*   5 = birthmonth                               */
                         /*   6 = sweetheart                               */
                         /*   7 = truth number                             */


#define CINVIS      0    /* "see invisble" counter index                   */
#define INVISI      1    /* "invisbility" counter index                    */
#define FIRPRO      2    /* "fire protection" counter index                */
#define LIGPRO      3    /* "lightning protection" counter index           */
#define ICEPRO      4    /* "ice protection" counter index                 */
#define OTHPRO      5    /* "other protection" counter index               */


#define NCPERT     25    /* maximum number of commands allowed per turn    */

typedef VOID(*AppRou)(VOID);

extern
struct aplayr {
     CHAR userid[UIDSIZ];     /*   player's User-ID; used for BTRIEVE      */ // 000-029 KEY
     CHAR alias[ALSSIZ];      /*   player's alias for within the game      */ // 030-039 KEY
     CHAR altnam[ANSIZE];     /*   either User-ID or "Some unseen force"   */ // 040-057
     SHORT desc;              /*   message number of player's description  */ // 058-059
     SHORT curloc;            /*   player's current location in the game   */ // 060-061
     SHORT prmloc;            /*   player's saved location  in the game    */ // 062-063
     SHORT hits;              /*   player's current hit points             */ // 064-065
     SHORT maxhits;           /*   player's maximum allowable hit poitns   */ // 066-067
     SHORT month;             /*   player's birth month                    */ // 068-069
     SHORT favpot;            /*   player's "favorite potion" index        */ // 070-071
     CHAR trunam[TRUSIZ];     /*   player's random-generated "true name"   */ // 072-081
     SHORT color;             /*   player's "true color" (random)          */ // 082-083
     SHORT lcombo[NLCMBS];    /*   combination for lock (random each user) */ // 084-089
     SHORT rsolve;            /*   ristina's phase required for solve      */ // 090-091
     SHORT asolve;            /*   amary's phase required for solve        */ // 092-093
     SHORT slvloc;            /*   location required for a solution        */ // 094-095
     SHORT stinx;             /*   riddle used for the Stinx               */ // 096-097
     SHORT trunum;            /*   player's "truthnumber"                  */ // 098-099
     SHORT nstate;            /*   number of wrong statements made to kyra */ // 100-101
     SHORT truths[NTRUTH];    /*   truths that the player has gotten       */ // 102-117
     SHORT ntrugt;            /*   number of truths that the player as got */ // 118-119
     SHORT comstt;            /*   for use in mr8088's parser              */ // 120-121
     SHORT naobjs;            /*   number of objects currently held        */ // 122-123
     struct alcobj *apobjs[MXPITS];     /* currently held objects          */ // 124-147
     SHORT gold;              /*   currency in the game                    */ // 148-149
     AppRou approu;           /*   player's once-per-turn called routine   */ // 150-153
     SHORT rouctr;            /*   general purpose counter for (*approu)() */ // 154-155
     SHORT counts[NPCNTS];    /*   various counters (protections, etc.)    */ // 156-167
     LONG flags;              /*   various bit "flags"                     */ // 168-171
     LONG pflags;             /*   various flags set by potions            */ // 172-175
     SHORT seemod;            /*   used for concatenations to "see stgs"   */ // 176-177
     SHORT peectr;            /*   "pee counter" for number of potions     */ // 178-179
     SHORT potctr;            /*   "potion counter" for slot chamber       */ // 180-181
     SHORT macros;            /*   number of commands entered this turn    */ // 182-183
     SHORT modno;             /*   current modem number                    */ // 184-185
     CHAR spouse[ALSSIZ];     /*   user-id of spouse (or \0'd out)         */ // 186-195
     SHORT pickle;            /*   pickle status variable                  */ // 196-197
     CHAR spare[252-198];     /*   spare space, decrease when adding       */ // 198-251
} *alcarr,                    /* base address of dynamic array of players  */ // size 252
  *alcptr,                    /* pointer to alcarr[usrnum] structure       */
  *alcaux,                    /* global pointer for "other" player struct. */
  *alcspa;                    /* general purpose player structure pointer  */

#define DRKVIS  0x00000001L   /*   player can see in darkness              */
#define BRIEFF  0x00000002L   /*   player's "brief" descriptions flag      */
#define SITTNG  0x00000004L   /*   player is "sitting down"                */
#define LAYING  0x00000008L   /*   player is "laying/lying down"           */
#define PAIDRM  0x00000010L   /*   player has paid for a room at the Inn   */
#define FEMALE  0x00000020L   /*   player is "female"                      */
#define RDYTOS  0x00000040L   /*   player is ready to solve game w/ 8088   */
#define PSTINX  0x00000080L   /*   player has answered Stinx Riddle        */
#define MARRID  0x00000100L   /*   player is "married"                     */
#define BOOBIE  0x00000200L   /*   player has satisfied the boobie         */
#define WONGAM  0x20000000L   /*   player has solved the game              */
#define LOADED  0x40000000L   /*   player has been "loaded" into memory    */
#define INVISF  0x80000000L   /*   player is invisible                     */

#define SBURPF  0x00000001L   /* super-burp flag (say)                     */
#define SNEEZF  0x00000002L   /* sneeze-attack flag (sneeze)               */
#define STINKB  0x00000004L   /* stink-bomb player with fart flag (fart)   */
#define TELEPF  0x00000008L   /* teleport with finger flag (finger)        */
#define BREATH  0x00000010L   /* fire-breath flag (breathe)                */
#define ICEY1F  0x00000020L   /* ice-attack number 1 (foot)                */
#define NAKEDF  0x00000040L   /* make-naked pelvis attack flag (pelvis)    */
#define ZAPAHO  0x00000080L   /* zap all items held in hand (finger)       */
#define LIGHTB  0x00000100L   /* lightning bolt attack (finger)            */
#define LIGBAL  0x00000200L   /* lightning ball attack (foot)              */
#define FIRFIN  0x00000400L   /* fire-finger attack (finger)               */
#define ZAPPRO  0x00000800L   /* zap-protections attack (finger)           */
#define UGLYCF  0x00001000L   /* make someone ugly (finger)                */
#define SCRYFL  0x00002000L   /* ability to scry someone (think)           */
#define TWALKE  0x00004000L   /* ability to walk through throne forcefield */
#define WETKIS  0x80000000L   /* the super, big wet kiss flag (kiss)       */

#define NAOBJS    103         /* total number of objects in object array   */
#define AONSIZ     12         /* maximum size of object names              */
#define MX2MIX      4         /* max. no. of two-potion potion combos      */
#define MX3MIX      3         /* max. no. of three-potion potion combos    */
                              /**** NOTE: The last entry in each mix array */
                              /****       MUST be set to -1!               */

extern
struct alcobj {
     CHAR name[AONSIZ];       /*   static one-word name                    */
     SHORT desc;              /*   description message (base message)      */
     SHORT flags;             /*   various bit "flags" for the item        */
     SHORT mix2s[MX2MIX];     /*   combinations when mixing two potions    */
     SHORT mix3s[MX3MIX];     /*   combinations when mixing three potions  */
} *alcobs,                    /* dynamically allocated array of objects    */
  *aobptr;                    /* general purpose pointer for objects       */

#define AUXPOM      1         /* amount added for auxillary message        */   */

typedef VOID(*ObjRou)(VOID);

extern
struct objrou {
     ObjRou routine;          /*   address of item's action routine        */
} obrous[NAOBJS];

#define PICKUP  0x0001             /*   item can be picked up              */
#define USEAN   0x0002             /*   attach "n" to single quant. descs. */
#define BLOWUP  0x0004             /*   potion explosive if wrongly mixed  */
#define POTION  0x0008             /*   item is a potion (Drinkable)       */
#define AIMABL  0x0010             /*   item is "aimable"                  */
#define RUBABL  0x0020             /*   item is "rubable"                  */
#define THKABL  0x0040             /*   item is "thinkable"                */
#define SHKABL  0x0080             /*   item is "shakable"                 */
#define DRKROU  0x0100             /*   item has a "drink" routine         */
#define ASKROU  0x0200             /*   itme is "askable"                  */
#define PERSON  0x0400             /*   item is considered a "person"      */
#define PORABL  0x0800             /*   item is "pourable"                 */
#define THRABL  0x1000             /*   item is "throwable"                */

#define PXPDAM 10                  /* amount of damage from potion explos. */

#define LANIDX  82            /* index into object array for lantern       */

#define NERRMSGS    4         /* number of error messages for syntax stuff */
#define NPFNMSGS    3         /* number of messages for profanity          */
#define NNHRMSGS    2         /* number of message for player not here     */

typedef VOID(*ComRou)(VOID);

struct comcmd {
     CHAR *word;              /* command keyword                           */
     ComRou routine;          /* command function                          */
     //VOID (*routine)();
};

extern
SHORT aobjno,                 /* used for an object's array position       */
    aobjn2;                   /* another object array position holder      */
extern
SHORT night;                  /* global variable delcaring "nighttime"     */
extern
SHORT margvi;                 /* global variable for current margv[] index */

extern CHAR *trucols[NTRUCS];
extern CHAR *amnths[NMONTHS];

extern
DFAFILE *a2bb,                /* BTRIEVE file pointer for player records   */
        *acbb;                /* BTRIEVE file pointer for control file     */
extern
HMCVFILE amb,                 /* file pointer for general messages         */
         almb,                /* file pointer for location descriptions    */
         admb,                /* file pointer for description messages     */
         acmb;                /* file pointer for computer messages        */


/* ALC2.C 10/03/92 11.43.10 */
/*VOID EXPORT init__alchemy2(VOID);*/
VOID initap(struct aplayr *app,CHAR *plyrid);

/* ALC2ALC.C 09/03/92 10.22.14 */
VOID a2dloc(VOID);
VOID a2dobj(VOID);

/* ALC2CMD.C 09/03/92 11.48.00 */
VOID initializeCommands(VOID);
VOID gsyner(VOID);
VOID acmhdl(VOID);
VOID anorth(VOID);
VOID asouth(VOID);
VOID aeast(VOID);
VOID awest(VOID);

/* ALC2COM.C 10/03/92 09.34.00 */
VOID mr8088(VOID);

/* ALC2POT.C 10/03/92 10.40.08 */
VOID amix(VOID);
VOID athink(VOID);
VOID adrink(VOID);
VOID apour(VOID);
VOID ashake(VOID);
VOID afart(VOID);
VOID asneez(VOID);
VOID abreth(VOID);
VOID aaim(VOID);
VOID pot01(VOID);
VOID pot02(VOID);
VOID pot05(VOID);
VOID pot07(VOID);
VOID pot08(VOID);
VOID pot10(VOID);
VOID pot11(VOID);
VOID pot12(VOID);
VOID pot13(VOID);
VOID pot14(VOID);
VOID pot15(VOID);
VOID pot16(VOID);
VOID pot18(VOID);
VOID pot20(VOID);
VOID pot22(VOID);
VOID pot23(VOID);
VOID pot24(VOID);
VOID pot25(VOID);
VOID pot26(VOID);
VOID pot27(VOID);
VOID pot29(VOID);
VOID pot30(VOID);
VOID pot31(VOID);
VOID pot32(VOID);
VOID pot33(VOID);
VOID pot34(VOID);
VOID pot35(VOID);
VOID pot36(VOID);
VOID pot37(VOID);
VOID pot38(VOID);
VOID pot39(VOID);
VOID pot40(VOID);
VOID pot41(VOID);
VOID pot42(VOID);
VOID pot43(VOID);
VOID pot44(VOID);
VOID pot45(VOID);
VOID pot46(VOID);
VOID pot47(VOID);
VOID pot48(VOID);
VOID pot51(VOID);
VOID pot52(VOID);
VOID pot54(VOID);
VOID pot61(VOID);
VOID obj085(VOID);
VOID obj086(VOID);
VOID obj087(VOID);
VOID obj088(VOID);
VOID obj089(VOID);
VOID obj090(VOID);
VOID obj091(VOID);
VOID obj092(VOID);
VOID obj093(VOID);
VOID obj094(VOID);

/* ALC2RSF.C 10/03/92 10.53.46 */
SHORT amagics(VOID);
SHORT rocker(VOID);
SHORT pospla(VOID);
SHORT kyrapr(VOID);
SHORT generl(VOID);
SHORT jcpunn(VOID);
SHORT barten(VOID);
SHORT oracle(VOID);
SHORT altar(VOID);
SHORT sapool(VOID);
SHORT omens(VOID);
SHORT owomens(VOID);
SHORT peeher(VOID);
SHORT gatlok(VOID);
SHORT crystb(VOID);
SHORT scentr(VOID);
SHORT ffdoor(VOID);
SHORT honeym(VOID);
SHORT dicfur(VOID);
SHORT barfo(VOID);
SHORT buffoo(VOID);
SHORT blahpl(VOID);
SHORT booger(VOID);
SHORT emeral(VOID);
SHORT coaler(VOID);
SHORT quartf(VOID);
SHORT lime(VOID);
SHORT litfor(VOID);
SHORT hygine(VOID);
SHORT wishwe(VOID);
SHORT pstump(VOID);
SHORT apond(VOID);
SHORT gigbee(VOID);
SHORT kyrats(VOID);
SHORT toxic(VOID);
SHORT bludra(VOID);
SHORT stinx(VOID);
SHORT wheel(VOID);
SHORT mirror(VOID);
SHORT throne(VOID);
SHORT lkdoor(VOID);
SHORT apools(VOID);
SHORT basket(VOID);
SHORT boobie(VOID);
SHORT wdome(VOID);
SHORT pickle(VOID);
SHORT mrslot(VOID);
SHORT pnkcor(VOID);
SHORT cofcre(VOID);
SHORT cofmoo(VOID);

/* ALC2TIM.C 10/03/92 10.56.48 */
VOID iactrl(VOID);
VOID mainat(VOID);
VOID shosky(VOID);
CHAR *camnth(VOID);
VOID hiccup(VOID);
VOID halluc(VOID);

/* ALC2UTL.C 10/03/92 11.39.48 */
VOID inautl(VOID);
CHAR *aheshe(struct aplayr *ap);
CHAR *ahisher(struct aplayr *ap);
CHAR *ahimher(struct aplayr *ap);
VOID apfoth(VOID);
VOID apmutl(SHORT msgnum);
VOID apfyou(SHORT msgnum);
VOID apfbt2(VOID);
VOID apfloc(SHORT loc);
VOID apfnear(VOID);
VOID apfall(VOID);
VOID xpaloc(SHORT msgnum);
VOID epaloc(SHORT loc,SHORT msgnum);
VOID dscloc(VOID);
CHAR *ditutl(struct alcobj *itmptr);
CHAR *nitutl(struct alcobj *itm);
CHAR *objloc(struct aloc *lp);
VOID ainutl(struct aplayr *who);
SHORT findap(CHAR *stg);
SHORT pinalc(CHAR *stg);
struct alcobj *fapobj(struct aplayr *gp,CHAR *stg);
struct alcobj *falobj(struct aloc *loc,CHAR *stg);
VOID tapobj(struct aplayr *gp,SHORT objino);
VOID talobj(struct aloc *loc,SHORT objino);
VOID papobj(struct aplayr *gp,struct alcobj *obj);
VOID palobj(struct aloc *loc,struct alcobj *obj);
SHORT hitap(struct aplayr *who,SHORT damage);
VOID killap(struct aplayr *who);
struct comcmd *a2ccmp(CHAR *stgptr,struct comcmd table[],SHORT length);
SHORT rndlap(SHORT loc);

