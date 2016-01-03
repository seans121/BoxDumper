#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pkx.h"

u32 expTable[100][6] = {
  {0, 0, 0, 0, 0, 0},
  {8, 15, 4, 9, 6, 10},
  {27, 52, 13, 57, 21, 33},
  {64, 122, 32, 96, 51, 80},
  {125, 237, 65, 135, 100, 156},
  {216, 406, 112, 179, 172, 270},
  {343, 637, 178, 236, 274, 428},
  {512, 942, 276, 314, 409, 640},
  {729, 1326, 393, 419, 583, 911},
  {1000, 1800, 540, 560, 800, 1250},
  {1331, 2369, 745, 742, 1064, 1663},
  {1728, 3041, 967, 973, 1382, 2160},
  {2197, 3822, 1230, 1261, 1757, 2746},
  {2744, 4719, 1591, 1612, 2195, 3430},
  {3375, 5737, 1957, 2035, 2700, 4218},
  {4096, 6881, 2457, 2535, 3276, 5120},
  {4913, 8155, 3046, 3120, 3930, 6141},
  {5832, 9564, 3732, 3798, 4665, 7290},
  {6859, 11111, 4526, 4575, 5487, 8573},
  {8000, 12800, 5440, 5460, 6400, 10000},
  {9261, 14632, 6482, 6458, 7408, 11576},
  {10648, 16610, 7666, 7577, 8518, 13310},
  {12167, 18737, 9003, 8825, 9733, 15208},
  {13824, 21012, 10506, 10208, 11059, 17280},
  {15625, 23437, 12187, 11735, 12500, 19531},
  {17576, 26012, 14060, 13411, 14060, 21970},
  {19683, 28737, 16140, 15244, 15746, 24603},
  {21952, 31610, 18439, 17242, 17561, 27440},
  {24389, 34632, 20974, 19411, 19511, 30486},
  {27000, 37800, 23760, 21760, 21600, 33750},
  {29791, 41111, 26811, 24294, 23832, 37238},
  {32768, 44564, 30146, 27021, 26214, 40960},
  {35937, 48155, 33780, 29949, 28749, 44921},
  {39304, 51881, 37731, 33084, 31443, 49130},
  {42875, 55737, 42017, 36435, 34300, 53593},
  {46656, 59719, 46656, 40007, 37324, 58320},
  {50653, 63822, 50653, 43808, 40522, 63316},
  {54872, 68041, 55969, 47846, 43897, 68590},
  {59319, 72369, 60505, 52127, 47455, 74148},
  {64000, 76800, 66560, 56660, 51200, 80000},
  {68921, 81326, 71677, 61450, 55136, 86151},
  {74088, 85942, 78533, 66505, 59270, 92610},
  {79507, 90637, 84277, 71833, 63605, 99383},
  {85184, 95406, 91998, 77440, 68147, 106480},
  {91125, 100237, 98415, 83335, 72900, 113906},
  {97336, 105122, 107069, 89523, 77868, 121670},
  {103823, 110052, 114205, 96012, 83058, 129778},
  {110592, 115015, 123863, 102810, 88473, 138240},
  {117649, 120001, 131766, 109923, 94119, 147061},
  {125000, 125000, 142500, 117360, 100000, 156250},
  {132651, 131324, 151222, 125126, 106120, 165813},
  {140608, 137795, 163105, 133229, 112486, 175760},
  {148877, 144410, 172697, 141677, 119101, 186096},
  {157464, 151165, 185807, 150476, 125971, 196830},
  {166375, 158056, 196322, 159635, 133100, 207968},
  {175616, 165079, 210739, 169159, 140492, 219520},
  {185193, 172229, 222231, 179056, 148154, 231491},
  {195112, 179503, 238036, 189334, 156089, 243890},
  {205379, 186894, 250562, 199999, 164303, 256723},
  {216000, 194400, 267840, 211060, 172800, 270000},
  {226981, 202013, 281456, 222522, 181584, 283726},
  {238328, 209728, 300293, 234393, 190662, 297910},
  {250047, 217540, 315059, 246681, 200037, 312558},
  {262144, 225443, 335544, 259392, 209715, 327680},
  {274625, 233431, 351520, 272535, 219700, 343281},
  {287496, 241496, 373744, 286115, 229996, 359370},
  {300763, 249633, 390991, 300140, 240610, 375953},
  {314432, 257834, 415050, 314618, 251545, 393040},
  {328509, 267406, 433631, 329555, 262807, 410636},
  {343000, 276458, 459620, 344960, 274400, 428750},
  {357911, 286328, 479600, 360838, 286328, 447388},
  {373248, 296358, 507617, 377197, 298598, 466560},
  {389017, 305767, 529063, 394045, 311213, 486271},
  {405224, 316074, 559209, 411388, 324179, 506530},
  {421875, 326531, 582187, 429235, 337500, 527343},
  {438976, 336255, 614566, 447591, 351180, 548720},
  {456533, 346965, 639146, 466464, 365226, 570666},
  {474552, 357812, 673863, 485862, 379641, 593190},
  {493039, 367807, 700115, 505791, 394431, 616298},
  {512000, 378880, 737280, 526260, 409600, 640000},
  {531441, 390077, 765275, 547274, 425152, 664301},
  {551368, 400293, 804997, 568841, 441094, 689210},
  {571787, 411686, 834809, 590969, 457429, 714733},
  {592704, 423190, 877201, 613664, 474163, 740880},
  {614125, 433572, 908905, 636935, 491300, 767656},
  {636056, 445239, 954084, 660787, 508844, 795070},
  {658503, 457001, 987754, 685228, 526802, 823128},
  {681472, 467489, 1035837, 710266, 545177, 851840},
  {704969, 479378, 1071552, 735907, 563975, 881211},
  {729000, 491346, 1122660, 762160, 583200, 911250},
  {753571, 501878, 1160499, 789030, 602856, 941963},
  {778688, 513934, 1214753, 816525, 622950, 973360},
  {804357, 526049, 1254796, 844653, 643485, 1005446},
  {830584, 536557, 1312322, 873420, 664467, 1038230},
  {857375, 548720, 1354652, 902835, 685900, 1071718},
  {884736, 560922, 1415577, 932903, 707788, 1105920},
  {912673, 571333, 1460276, 963632, 730138, 1140841},
  {941192, 583539, 1524731, 995030, 752953, 1176490},
  {970299, 591882, 1571884, 1027103, 776239, 1212873},
  {1000000, 600000, 1640000, 1059860, 800000, 1250000}
};

u8 	calcPkmLevel(u16 species, u32 exp)
{
  u8	xpType = pkData.pkmData[species][11];
  u8 	iterLevel = 1;

  while (iterLevel < 100 && exp >= expTable[iterLevel][xpType])
    iterLevel++;
  return iterLevel;
}

s8	getPIDGender(struct s_pkm *pkm)
{
  u8	gratio = pkData.pkmData[pkm->pkx.species][0xA];

  if (gratio == 255)
    return 2;
  if (gratio == 254)
    gratio++;

  if ((pkm->pkx.personalityID & 0xFF) <= gratio)
    return 1;
  else
    return 0;
}


s8 	setPkmLevel(struct s_pkm *pkm, u8 level)
{
  u8	xpType = pkData.pkmData[pkm->pkx.species][11];

  if (level < 1 || level > 100)
    return -1;
  pkm->pkx.expPoints = expTable[level - 1][xpType];
  pkmRecalc(pkm);
  return 0;
}

s8 	setPkmForm(struct s_pkm *pkm, u8 form)
{
  pkm->pkx.formFlags &= 0x7;
  pkm->pkx.formFlags ^= form << 3;
  pkmRecalc(pkm);
  return 0;
}

s8 	setPkmSpecies(struct s_pkm *pkm, u16 species)
{
  if (species > 721)
    return -1;
  pkm->pkx.species = species;
  setPkmLevel(pkm, pkm->level);
  setPkmAbilityNum(pkm, pkm->pkx.abilityNum);
  setPkmGender(pkm, getPIDGender(pkm));

  if (!(pkm->pkx.individualValues >> 31))
    setNickname((char *)pkData.species[species], pkm);
  pkmRecalc(pkm);
  return 0;
}

s8 	rerollPID(struct s_pkm *pkm)
{
  pkm->pkx.personalityID = rand();
  pkmRecalc(pkm);
  return 0;
}

s8 	rerollPIDspe(struct s_pkm *pkm, u8 shiny, u8 gender)
{
  u8 	gendercond;
  u8	shinycond;
  for (int i = 0; i < 80000; i++)
  {
    rerollPID(pkm);
    shinycond = 0;
    if (shiny == 2 || pkm->isShiny == shiny)
      shinycond = 1;
    gendercond = 0;
    if (gender == 2 || pkm->gender == gender)
      gendercond = 1;
    if (shinycond && gendercond)
      return 0;
  }
  return -1;
}

s8	isShiny(struct s_pkm *pkm)
{
  u16 trainerSV = (pkm->pkx.trainerID ^ pkm->pkx.trainerSecretID) >> 4;
  u16 pkmSV = ((pkm->pkx.personalityID >> 16) ^ (pkm->pkx.personalityID & 0xFFFF)) >> 4;

  return (pkmSV == trainerSV);
}

s8	setPkmAbilityNum(struct s_pkm *pkm, u8 abilNum)
{
  if (abilNum > 4 || abilNum == 3 || !abilNum)
    return -1;
  u8 offs = abilNum == 4 ? 3 : abilNum;
  pkm->pkx.abilityNum = abilNum;
  pkm->pkx.ability = pkData.pkmData[pkm->pkx.species][5 + offs];
  return 0;
}

s8 	getPkmGender(struct s_pkm *pkm)
{
  return (pkm->pkx.formFlags >> 1) & 0x3;
}

s8 	setPkmGender(struct s_pkm *pkm, u8 gender)
{
  pkm->pkx.formFlags &= 0xF9;
  pkm->pkx.formFlags ^= (gender & 0x3) << 1;
  pkmRecalc(pkm);
  return 0;
}

s8	setu16Name(char *src, u8 *dest)
{
  int	cnt = 0;

  while (src[cnt] && cnt < 12)
  {
    dest[cnt * 2] = src[cnt];
    dest[cnt * 2 + 1] = 0;
    cnt++;
  }
  while (cnt <= 12)
  {
    dest[cnt * 2] = 0;
    dest[cnt * 2 + 1] = 0;
    cnt++;
  }
  return 0;
}

s8	setNickname(char *src, struct s_pkm *pkm)
{
  return setu16Name(src, pkm->pkx.nickname);
}

char 	*getu16Name(char *dst, u8 *src)
{
  int 	cnt = 0;

  while (src[cnt] && cnt < 24)
  {
    dst[cnt / 2] = src[cnt];
    cnt += 2;
  }
  dst[cnt / 2] = 0;
  return dst;
}

char 	*getNickname(char *dst, struct s_pkm *pkm)
{
  return getu16Name(dst, pkm->pkx.nickname);
}

char 	*getOTName(char *dst, struct s_pkm *pkm)
{
  return (getu16Name(dst, pkm->pkx.trainerName));
}

char 	*getHdlName(char *dst, struct s_pkm *pkm)
{
  return (getu16Name(dst, pkm->pkx.handlerName));
}

s8 	setPkmIV(u8 val, u8 stat, struct s_pkm *pkm)
{
  u32 	nval = val;
  u32 	mask = 0xFFFFFFFF;
  mask ^= 0x1F << (5 * stat);
  
  u32 	iv32 = pkm->pkx.individualValues;
  iv32 &= mask;
  iv32 ^= ((nval & 0x1F) << (5 * stat));
  pkm->pkx.individualValues = iv32;
  return 0;
}

u8 	getPkmIV(u32 individualValues, u8 stat)
{
  return (individualValues >> (5 * stat)) & 0x1F;
}

u16 	calcPkmStat(u16 species, u8 IV, u8 EV, u8 nature, u8 level, u8 stat, u8 form)
{
  if (form && form <= pkData.pkmData[species][0xC])
    species = 721 + pkData.pkmData[species][0xD] + form - 1;
  u8 	baseStat = pkData.pkmData[species][stat];
  u8 	mult = 10;
  u16 	final;

  if (stat == HP)
    final = 10 + ((2 * baseStat) + IV + EV / 4 + 100) * level / 100;
  else if (stat <= SPD)
    final = 5 + (2 * baseStat + IV + EV / 4) * level / 100; 
  else
    final = 0;

  if (nature / 5 + 1 == stat)
    mult++;
  if (nature % 5 + 1 == stat)
    mult--;
  final = final * mult / 10;
  return final;
}

s8 	pkmRecalc(struct s_pkm *pkm)
{
  pkm->level = calcPkmLevel(pkm->pkx.species, pkm->pkx.expPoints);
  for (int i = 0; i < 6; i++)
    pkm->stat[i] = calcPkmStat(pkm->pkx.species, getPkmIV(pkm->pkx.individualValues, i),
			      pkm->pkx.effortValues[i], pkm->pkx.nature, pkm->level, i,
			      pkm->pkx.formFlags >> 3);
  pkm->isShiny = isShiny(pkm);
  pkm->gender = getPkmGender(pkm);
  return 0;
}

//Encryption

Result 	shuffleArray(u8 *array, u8 sv)
{
  u8 aloc[] = { 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3 };
  u8 bloc[] = { 1, 1, 2, 3, 2, 3, 0, 0, 0, 0, 0, 0, 2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2 };
  u8 cloc[] = { 2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2, 0, 0, 0, 0, 0, 0, 3, 2, 3, 2, 1, 1 };
  u8 dloc[] = { 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0 };

  u8 ord[] = {aloc[sv], bloc[sv], cloc[sv], dloc[sv]};
  u8 pkmcpy[232];
  u8 tmp[56];

  memcpy(pkmcpy, array, 232);
  for (int b = 0; b < 4; b++)
  {
    memcpy(tmp, pkmcpy + 8 + 56 * ord[b], 56);
    memcpy(array + 8 + 56 * b, tmp, 56);
  }
  return 0;	
}

u32 seedStep(u32 seed)
{
  u32 a = 0x41C64E6D;
  u32 c = 0x00006073;

  return ((seed * a + c) & 0xFFFFFFFF);
}

Result decryptPokemon(u8 *enc, u8 *dec)
{
  memcpy(dec, enc, 232);

  u32 pv = *(u32 *)dec;
  u8 sv = (((pv & 0x3E000) >> 0xD) % 24);

  u32 seed = pv;
  u16 tmp;
  for (int i = 8; i < 232; i += 2)
  {
    memcpy(&tmp, dec + i, 2);
    tmp ^= (seedStep(seed) >> 16);
    seed = seedStep(seed);
    memcpy(dec + i, &tmp, 2);
  }
  shuffleArray(dec, sv);
  return (0);
}

Result 	encryptPokemon(u8 *dec, u8 *enc)
{
  u32 	pv = *(u32 *)dec;
  u32 	sv = (((pv & 0x3E000) >> 0xD) % 24);
  u16 	tmp;

  memcpy(enc, dec, 232);
  for (int i = 0; i < 11; i++)
    shuffleArray(enc, sv);

  u32 	seed = pv;
  for (int i = 8; i < 232; i += 2)
  {
    memcpy(&tmp, enc + i, 2);
    tmp ^= (seedStep(seed) >> 16);
    seed = seedStep(seed);
    memcpy(enc + i, &tmp, 2);
  }
  return 0;
}
