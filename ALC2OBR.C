/*****************************************************************************
 *                                                                           *
 *   ALC2OBR.C                                                      V3.11    *
 *                                                                           *
 *   ALCHEMY II: Object handler routine interface.                           *
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

static VOID nulobj(VOID);

static VOID
nulobj(VOID)
{
}

struct objrou obrous[NAOBJS]={
/* 000 */  {nulobj},
/* 001 */  {pot01},
/* 002 */  {pot02},
/* 003 */  {nulobj},
/* 004 */  {nulobj},
/* 005 */  {pot05},
/* 006 */  {nulobj},
/* 007 */  {pot07},
/* 008 */  {pot08},
/* 009 */  {nulobj},
/* 010 */  {pot10},
/* 011 */  {pot11},
/* 012 */  {pot12},
/* 013 */  {pot13},
/* 014 */  {pot14},
/* 015 */  {pot15},
/* 016 */  {pot16},
/* 017 */  {nulobj},
/* 018 */  {pot18},
/* 019 */  {nulobj},
/* 020 */  {pot20},
/* 021 */  {nulobj},
/* 022 */  {pot22},
/* 023 */  {pot23},
/* 024 */  {pot24},
/* 025 */  {pot25},
/* 026 */  {pot26},
/* 027 */  {pot27},
/* 028 */  {nulobj},
/* 029 */  {pot29},
/* 030 */  {pot30},
/* 031 */  {pot31},
/* 032 */  {pot32},
/* 033 */  {pot33},
/* 034 */  {pot34},
/* 035 */  {pot35},
/* 036 */  {pot36},
/* 037 */  {pot37},
/* 038 */  {pot38},
/* 039 */  {pot39},
/* 040 */  {pot40},
/* 041 */  {pot41},
/* 042 */  {pot42},
/* 043 */  {pot43},
/* 044 */  {pot44},
/* 045 */  {pot45},
/* 046 */  {pot46},
/* 047 */  {pot47},
/* 048 */  {pot48},
/* 049 */  {nulobj},
/* 050 */  {nulobj},
/* 051 */  {pot51},
/* 052 */  {pot52},
/* 053 */  {nulobj},
/* 054 */  {pot54},
/* 055 */  {nulobj},
/* 056 */  {nulobj},
/* 057 */  {nulobj},
/* 058 */  {nulobj},
/* 059 */  {nulobj},
/* 060 */  {nulobj},
/* 061 */  {pot61},
/* 062 */  {nulobj},
/* 063 */  {nulobj},
/* 064 */  {nulobj},
/* 065 */  {nulobj},
/* 066 */  {nulobj},
/* 067 */  {nulobj},
/* 068 */  {nulobj},
/* 069 */  {nulobj},
/* 070 */  {nulobj},
/* 071 */  {nulobj},
/* 072 */  {nulobj},
/* 073 */  {nulobj},
/* 074 */  {nulobj},
/* 075 */  {nulobj},
/* 076 */  {nulobj},
/* 077 */  {nulobj},
/* 078 */  {nulobj},
/* 079 */  {nulobj},
/* 080 */  {nulobj},
/* 081 */  {nulobj},
/* 082 */  {nulobj},
/* 083 */  {nulobj},
/* 084 */  {nulobj},
/* 085 */  {obj085},
/* 086 */  {obj086},
/* 087 */  {obj087},
/* 088 */  {obj088},
/* 089 */  {obj089},
/* 090 */  {obj090},
/* 091 */  {obj091},
/* 092 */  {obj092},
/* 093 */  {obj093},
/* 094 */  {obj094},
/* 095 */  {nulobj},
/* 096 */  {nulobj},
/* 097 */  {nulobj},
/* 098 */  {nulobj},
/* 099 */  {nulobj},
/* 100 */  {nulobj},
/* 101 */  {nulobj},
/* 102 */  {nulobj}
};
