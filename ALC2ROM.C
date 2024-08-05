/*****************************************************************************
 *                                                                           *
 *   ALC2ROM.C                                                      V3.11    *
 *                                                                           *
 *   ALCHEMY II: Room handler routine interface.                             *
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

static SHORT nulloc(VOID);

static SHORT
nulloc(VOID)
{
     return(0);
}

struct locrou alrous[NALOCS]={
/* 000 */  {nulloc},
/* 001 */  {nulloc},
/* 002 */  {nulloc},
/* 003 */  {barten},
/* 004 */  {nulloc},
/* 005 */  {nulloc},
/* 006 */  {generl},
/* 007 */  {nulloc},
/* 008 */  {nulloc},
/* 009 */  {oracle},
/* 010 */  {nulloc},
/* 011 */  {nulloc},
/* 012 */  {nulloc},
/* 013 */  {altar},
/* 014 */  {nulloc},
/* 015 */  {kyrapr},
/* 016 */  {nulloc},
/* 017 */  {nulloc},
/* 018 */  {nulloc},
/* 019 */  {rocker},
/* 020 */  {nulloc},
/* 021 */  {amagics},
/* 022 */  {nulloc},
/* 023 */  {nulloc},
/* 024 */  {sapool},
/* 025 */  {nulloc},
/* 026 */  {nulloc},
/* 027 */  {nulloc},
/* 028 */  {nulloc},
/* 029 */  {nulloc},
/* 030 */  {nulloc},
/* 031 */  {nulloc},
/* 032 */  {pospla},
/* 033 */  {owomens},
/* 034 */  {omens},
/* 035 */  {peeher},
/* 036 */  {peeher},
/* 037 */  {nulloc},
/* 038 */  {jcpunn},
/* 039 */  {gatlok},
/* 040 */  {crystb},
/* 041 */  {nulloc},
/* 042 */  {nulloc},
/* 043 */  {nulloc},
/* 044 */  {ffdoor},
/* 045 */  {nulloc},
/* 046 */  {wishwe},
/* 047 */  {kyrats},
/* 048 */  {nulloc},
/* 049 */  {honeym},
/* 050 */  {nulloc},
/* 051 */  {nulloc},
/* 052 */  {blahpl},
/* 053 */  {dicfur},
/* 054 */  {nulloc},
/* 055 */  {lime},
/* 056 */  {nulloc},
/* 057 */  {barfo},
/* 058 */  {pstump},
/* 059 */  {litfor},
/* 060 */  {apond},
/* 061 */  {buffoo},
/* 062 */  {hygine},
/* 063 */  {booger},
/* 064 */  {scentr},
/* 065 */  {emeral},
/* 066 */  {nulloc},
/* 067 */  {gigbee},
/* 068 */  {nulloc},
/* 069 */  {nulloc},
/* 070 */  {nulloc},
/* 071 */  {nulloc},
/* 072 */  {nulloc},
/* 073 */  {nulloc},
/* 074 */  {nulloc},
/* 075 */  {nulloc},
/* 076 */  {nulloc},
/* 077 */  {nulloc},
/* 078 */  {nulloc},
/* 079 */  {nulloc},
/* 080 */  {nulloc},
/* 081 */  {nulloc},
/* 082 */  {nulloc},
/* 083 */  {nulloc},
/* 084 */  {nulloc},
/* 085 */  {nulloc},
/* 086 */  {nulloc},
/* 087 */  {nulloc},
/* 088 */  {nulloc},
/* 089 */  {nulloc},
/* 090 */  {nulloc},
/* 091 */  {nulloc},
/* 092 */  {nulloc},
/* 093 */  {nulloc},
/* 094 */  {nulloc},
/* 095 */  {nulloc},
/* 096 */  {nulloc},
/* 097 */  {nulloc},
/* 098 */  {nulloc},
/* 099 */  {nulloc},
/* 100 */  {nulloc},
/* 101 */  {nulloc},
/* 102 */  {nulloc},
/* 103 */  {nulloc},
/* 104 */  {nulloc},
/* 105 */  {nulloc},
/* 106 */  {nulloc},
/* 107 */  {nulloc},
/* 108 */  {nulloc},
/* 109 */  {nulloc},
/* 110 */  {nulloc},
/* 111 */  {nulloc},
/* 112 */  {nulloc},
/* 113 */  {nulloc},
/* 114 */  {nulloc},
/* 115 */  {nulloc},
/* 116 */  {nulloc},
/* 117 */  {nulloc},
/* 118 */  {nulloc},
/* 119 */  {nulloc},
/* 120 */  {nulloc},
/* 121 */  {nulloc},
/* 122 */  {nulloc},
/* 123 */  {nulloc},
/* 124 */  {nulloc},
/* 125 */  {nulloc},
/* 126 */  {nulloc},
/* 127 */  {nulloc},
/* 128 */  {nulloc},
/* 129 */  {nulloc},
/* 130 */  {nulloc},
/* 131 */  {nulloc},
/* 132 */  {nulloc},
/* 133 */  {nulloc},
/* 134 */  {nulloc},
/* 135 */  {nulloc},
/* 136 */  {nulloc},
/* 137 */  {nulloc},
/* 138 */  {nulloc},
/* 139 */  {nulloc},
/* 140 */  {nulloc},
/* 141 */  {nulloc},
/* 142 */  {nulloc},
/* 143 */  {nulloc},
/* 144 */  {nulloc},
/* 145 */  {nulloc},
/* 146 */  {nulloc},
/* 147 */  {nulloc},
/* 148 */  {nulloc},
/* 149 */  {nulloc},
/* 150 */  {nulloc},
/* 151 */  {nulloc},
/* 152 */  {nulloc},
/* 153 */  {nulloc},
/* 154 */  {nulloc},
/* 155 */  {nulloc},
/* 156 */  {nulloc},
/* 157 */  {nulloc},
/* 158 */  {nulloc},
/* 159 */  {nulloc},
/* 160 */  {nulloc},
/* 161 */  {nulloc},
/* 162 */  {nulloc},
/* 163 */  {nulloc},
/* 164 */  {nulloc},
/* 165 */  {nulloc},
/* 166 */  {nulloc},
/* 167 */  {nulloc},
/* 168 */  {toxic},
/* 169 */  {bludra},
/* 170 */  {coaler},
/* 171 */  {nulloc},
/* 172 */  {nulloc},
/* 173 */  {nulloc},
/* 174 */  {nulloc},
/* 175 */  {stinx},
/* 176 */  {nulloc},
/* 177 */  {nulloc},
/* 178 */  {quartf},
/* 179 */  {nulloc},
/* 180 */  {nulloc},
/* 181 */  {wheel},
/* 182 */  {nulloc},
/* 183 */  {nulloc},
/* 184 */  {nulloc},
/* 185 */  {nulloc},
/* 186 */  {nulloc},
/* 187 */  {nulloc},
/* 188 */  {nulloc},
/* 189 */  {nulloc},
/* 190 */  {nulloc},
/* 191 */  {nulloc},
/* 192 */  {nulloc},
/* 193 */  {nulloc},
/* 194 */  {nulloc},
/* 195 */  {nulloc},
/* 196 */  {nulloc},
/* 197 */  {nulloc},
/* 198 */  {nulloc},
/* 199 */  {nulloc},
/* 200 */  {mirror},
/* 201 */  {nulloc},
/* 202 */  {nulloc},
/* 203 */  {nulloc},
/* 204 */  {nulloc},
/* 205 */  {nulloc},
/* 206 */  {nulloc},
/* 207 */  {nulloc},
/* 208 */  {nulloc},
/* 209 */  {lkdoor},
/* 210 */  {apools},
/* 211 */  {throne},
/* 212 */  {basket},
/* 213 */  {nulloc},
/* 214 */  {nulloc},
/* 215 */  {boobie},
/* 216 */  {nulloc},
/* 217 */  {cofmoo},
/* 218 */  {mrslot},
/* 219 */  {nulloc},
/* 220 */  {wdome},
/* 221 */  {pickle},
/* 222 */  {nulloc},
/* 223 */  {pnkcor},
/* 224 */  {cofcre}
};
