/*****************************************************************************
 *                                                                           *
 *   ALC2ALC.C                                                      V3.11    *
 *                                                                           *
 *   Copyright (C) 1991-1992 GALACTICOMM, Inc.    All Rights Reserved.       *
 *                                                                           *
 *   ALCHEMY II: Dynamic allocation for location array & object array.       *
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

// v3.1: added +1 to j on the .h file load/match routines for objs/locs as
//       message file constants start at 1 now

#include "alc2.h"

static CHAR *getaln(/*FILE *fp*/);
static VOID masymt(CHAR *hdrfil);
static VOID rdflgs(CHAR *buf,SHORT *flgptr);

#define LINSIZ 80
#define MSGLEN 10
#define LOCOFF 11
#define SYMOFF 8
#define NOBITS 16

static CHAR lbuf[80],*hbuf;
static FILE *lfp; //,*hfp;

static SHORT nlines;
struct a2symtab {
     CHAR symnam[MSGLEN];
} *a2symtab;

static CHAR *
getaln(VOID)
/*FILE *fp;*/
{
     CHAR *retval;

     retval=fgets(lbuf,LINSIZ,lfp);
     return(retval);
}

VOID
a2dloc(VOID)
{
     INT locno;
     INT siz,i,jnk,retval;
     SHORT j;
     INT north,east,south,west;
     INT obidx1,obidx2,obidx3,obidx4,obidx5,obidx6;
     CHAR *locfil={"elwalc2l.lcs"};

     usrnum=-1;
     masymt("elwalc2l.h");
     if ((lfp=fopen(locfil,FOPRA)) == NULL) {
          catastro("ERROR OPENING \"%s\"",locfil);
     }
     alocs=(struct aloc *)alcmem(siz=(NALOCS*sizeof(struct aloc)));
     setmem(alocs,siz,0);
     for (i=0 ; i < NALOCS ; i++) {
          getaln();
          retval=sscanf(&lbuf[0],"Location#: %d\n",&locno);
          if (locno >= NALOCS || locno < 0) {
               fclose(lfp);
               catastro("LOCATION# %d OUT OF RANGE IN \"%s\"!",locno,locfil);
          }
          (VOID)retval;

          hbuf=getaln();
          hbuf+=LOCOFF;
          siz=strlen(hbuf)-1;
          hbuf[siz]='\0';
          for (j=0 ; j < nlines ; j++) {
               if (strcmp(hbuf,a2symtab[j].symnam) == 0) {
                    alocs[locno].desc=j+1; // 04/02/2021 Rick Hadsall: This will start message references at 0 which is incorrect, offsetting by 1 valid value
                    break;
               }
          }
          if (j == nlines) {
               fclose(lfp);
               catastro("UNKNOWN SYMBOL \"%s\" (%d) IN %s!",hbuf,i,locfil);
          }
          hbuf-=LOCOFF;

          getaln();   /* the "guide-line" for the binary flags     */
          hbuf=getaln();
          hbuf+=LOCOFF;
          siz=strlen(hbuf)-1;
          hbuf[siz]='\0';
          rdflgs(hbuf,&alocs[locno].flags);

          getaln();
          retval= sscanf(&lbuf[0], "# of Objs: %d\n", &jnk);
          alocs[locno].nlobjs=(SHORT)jnk;
          (VOID)retval;

          getaln();
          retval = sscanf(&lbuf[0],"Objs' Idx: %d,%d,%d,%d,%d,%d\n",
               &obidx1,&obidx2,&obidx3,&obidx4,&obidx5,&obidx6);
          alocs[locno].alobjs[0]=&alcobs[obidx1];
          alocs[locno].alobjs[1]=&alcobs[obidx2];
          alocs[locno].alobjs[2]=&alcobs[obidx3];
          alocs[locno].alobjs[3]=&alcobs[obidx4];
          alocs[locno].alobjs[4]=&alcobs[obidx5];
          alocs[locno].alobjs[5]=&alcobs[obidx6];
          (VOID)retval;

          getaln();
          retval = sscanf(&lbuf[0],"N:%d S:%d E:%d W:%d\n",
               &north,&south,&east,&west);
          alocs[locno].north=(SHORT)north;
          alocs[locno].south=(SHORT)south;
          alocs[locno].east=(SHORT)east;
          alocs[locno].west=(SHORT)west;
          getaln();
     }
     free(a2symtab);
     fclose(lfp);
     (VOID)retval;
}

VOID
a2dobj(VOID)
{
     INT n;
     INT siz, i;
     SHORT j;
     INT mix1,mix2,mix3,mix4;
     CHAR *objfil={"elwalc2o.obs"};

     usrnum=-1;
     masymt("elwalc2d.h");
     if ((lfp=fopen(objfil,FOPRA)) == NULL) {
          catastro("ERROR OPENING \"%s\"",objfil);
     }
     
     alcobs=(struct alcobj *)alcmem(siz=(NAOBJS*sizeof(struct alcobj)));
     setmem(alcobs,siz,0);
     for (i=0 ; i < NAOBJS ; i++) {
          getaln();
          sscanf(&lbuf[0],"ObjectNum: %d\n",&n);
          if (n >= NAOBJS || n < 0) {
               fclose(lfp);
               catastro("OBJECT# %d OUT OF RANGE IN \"%s\"!",n,objfil);
          }

          hbuf=getaln();
          hbuf+=LOCOFF;
          siz=strlen(hbuf)-1;
          hbuf[siz]='\0';
          strcpy(alcobs[n].name,hbuf);
          hbuf-=LOCOFF;

          hbuf=getaln();
          hbuf+=LOCOFF;
          siz=strlen(hbuf)-1;
          hbuf[siz]='\0';
          for (j=0 ; j < nlines ; j++) {
               if (strcmp(hbuf,a2symtab[j].symnam) == 0) {
                    alcobs[n].desc=j+1;  // 04/02/2021 Rick Hadsall: This will start message references at 0 which is incorrect, offsetting by 1 valid value
                    break;
               }
          }
          if (j == nlines) {
               fclose(lfp);
               catastro("UNKNOWN SYMBOL \"%s\" IN %s!",hbuf,objfil);
          }
          hbuf-=LOCOFF;

          getaln();    //the "guide-line" for the binary flags     
          hbuf=getaln();
          hbuf+=LOCOFF;
          siz=strlen(hbuf)-1;
          hbuf[siz]='\0';
          rdflgs(hbuf,&alcobs[n].flags);

          getaln();
          sscanf(&lbuf[0],"Obj'sMx2s: %d,%d,%d,%d\n",
               &mix1,&mix2,&mix3,&mix4);
          alcobs[n].mix2s[0]=(SHORT)mix1;
          alcobs[n].mix2s[1]=(SHORT)mix2;
          alcobs[n].mix2s[2]=(SHORT)mix3;
          alcobs[n].mix2s[3]=(SHORT)mix4;

          getaln();
          sscanf(&lbuf[0],"Obj'sMx3s: %d,%d,%d\n",
               &mix1,&mix2,&mix3);
          alcobs[n].mix3s[0]=(SHORT)mix1;
          alcobs[n].mix3s[1]=(SHORT)mix2;
          alcobs[n].mix3s[2]=(SHORT)mix3;

          getaln();
     }
     free(a2symtab);
     fclose(lfp); 
}

static VOID
masymt(CHAR *hdrfil)
{
     SHORT i,n,siz;
     CHAR *jnkbuf;

     if ((lfp=fopen(hdrfil,FOPRA)) == NULL) {
          catastro("ERROR OPENING \"%s\"",hdrfil);
     }
     for (nlines=0 ; getaln() != NULL ; nlines++) {
     }
     nlines-=2;
     a2symtab=(struct a2symtab *)alcmem(siz=nlines*sizeof(struct a2symtab));
     setmem(a2symtab,siz,0);
     rewind(lfp);
     getaln();
     getaln();
     for (i=0 ; i < nlines ; i++) {
          hbuf=getaln();
          hbuf+=SYMOFF;
          for (jnkbuf=hbuf,n=0 ; *jnkbuf != ' ' && n < MSGLEN ; jnkbuf++,n++) {
          }
          *jnkbuf='\0';
          strcpy(a2symtab[i].symnam,hbuf);
          hbuf-=SYMOFF;
     }
     fclose(lfp);
}

static VOID
rdflgs(CHAR *buf,SHORT *flgptr)
{
     SHORT i;
     USHORT flgbts[16]={
          1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768U
     };

     for (i=0 ; i < 16 ; i++,buf++) {
          if (*buf == '1') {
               *flgptr|=flgbts[i];
          }
     }
}
