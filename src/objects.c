/* NetHack 3.6	objects.c	$NHDT-Date: 1535422421 2018/08/28 02:13:41 $  $NHDT-Branch: NetHack-3.6.2-beta01 $:$NHDT-Revision: 1.51 $ */
/* Copyright (c) Mike Threepoint, 1989.                           */
/* NetHack may be freely redistributed.  See license for details. */

/*
 * The data in this file is processed twice, to construct two arrays.
 * On the first pass, only object name and object description matter.
 * On the second pass, all object-class fields except those two matter.
 * 2nd pass is a recursive inclusion of this file, not a 2nd compilation.
 * The name/description array is also used by makedefs and lev_comp.
 *
 * #ifndef OBJECTS_PASS_2_
 * # define OBJECT(name,desc,foo,bar,glorkum) name,desc
 * struct objdescr obj_descr[] =
 * #else
 * # define OBJECT(name,desc,foo,bar,glorkum) foo,bar,glorkum
 * struct objclass objects[] =
 * #endif
 * {
 *   { OBJECT("strange object",NULL, 1,2,3) },
 *   { OBJECT("arrow","pointy stick", 4,5,6) },
 *   ...
 *   { OBJECT(NULL,NULL, 0,0,0) }
 * };
 * #define OBJECTS_PASS_2_
 * #include "objects.c"
 */

/* *INDENT-OFF* */
/* clang-format off */

#ifndef OBJECTS_PASS_2_
/* first pass */
struct monst { struct monst *dummy; };  /* lint: struct obj's union */
#include "config.h"
#include "obj.h"
#include "objclass.h"
#include "prop.h"
#include "skills.h"

#else /* !OBJECTS_PASS_2_ */
/* second pass */
#include "color.h"
#define COLOR_FIELD(X) X,
#endif /* !OBJECTS_PASS_2_ */

/* objects have symbols: ) [ = " ( % ! ? + / $ * ` 0 _ . */

/*
 *      Note:  OBJ() and BITS() macros are used to avoid exceeding argument
 *      limits imposed by some compilers.  The ctnr field of BITS currently
 *      does not map into struct objclass, and is ignored in the expansion.
 *      The 0 in the expansion corresponds to oc_pre_discovered, which is
 *      set at run-time during role-specific character initialization.
 */

#ifndef OBJECTS_PASS_2_
/* first pass -- object descriptive text */
#define OBJ(name,desc)  name, desc
#define OBJECT(obj,bits,prp,sym,prob,dly,wt, \
               cost,sdam,ldam,oc1,oc2,nut,color)  { obj }
#define None (char *) 0 /* less visual distraction for 'no description' */

NEARDATA struct objdescr obj_descr[] =
#else
/* second pass -- object definitions */
#define BITS(nmkn,mrg,uskn,ctnr,mgc,chrg,uniq,nwsh,big,tuf,dir,sub,mtrl) \
  nmkn,mrg,uskn,0,mgc,chrg,uniq,nwsh,big,tuf,dir,mtrl,sub /*SCO cpp fodder*/
#define OBJECT(obj,bits,prp,sym,prob,dly,wt,cost,sdam,ldam,oc1,oc2,nut,color) \
  { 0, 0, (char *) 0, bits, prp, sym, dly, COLOR_FIELD(color) prob, wt, \
    cost, sdam, ldam, oc1, oc2, nut }
#ifndef lint
#define HARDGEM(n) (n >= 8)
#else
#define HARDGEM(n) (0)
#endif

NEARDATA struct objclass objects[] =
#endif
{
/* dummy object[0] -- description [2nd arg] *must* be NULL */
OBJECT(OBJ("strange object", None),
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, 0),
       0, ILLOBJ_CLASS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),

/* weapons ... */
#define WEAPON(name,desc,kn,mg,mgc,bi,prob,wt,            \
               cost,sdam,ldam,hitbon,typ,sub,metal,color) \
    OBJECT(OBJ(name,desc),                                           \
           BITS(kn, mg, 1, 0, mgc, 1, 0, 0, bi, 0, typ, sub, metal), \
           0, WEAPON_CLASS, prob, 0, wt,                             \
           cost, sdam, ldam, hitbon, 0, wt, color)
#define PROJECTILE(name,desc,kn,prob,wt,                  \
                   cost,sdam,ldam,hitbon,metal,sub,color) \
    OBJECT(OBJ(name,desc),                                          \
           BITS(kn, 1, 1, 0, 0, 1, 0, 0, 0, 0, PIERCE, sub, metal), \
           0, WEAPON_CLASS, prob, 0, wt,                            \
           cost, sdam, ldam, hitbon, 0, wt, color)
#define BOW(name,desc,kn,bi,prob,wt,cost,hitbon,metal,sub,color) \
    OBJECT(OBJ(name,desc),                                          \
           BITS(kn, 0, 1, 0, 0, 1, 0, 0, bi, 0, 0, sub, metal),     \
           0, WEAPON_CLASS, prob, 0, wt,                            \
           cost, 2, 2, hitbon, 0, wt, color)

#define BULLET(name,app,kn,prob,wt,cost,sdam,ldam,hitbon,ammotyp,typ,metal,sub,color) \
    OBJECT(OBJ(name,app), \
       BITS(kn,1,1,0,0,1,0,0,0,0,typ,sub,metal), 0, \
       WEAPON_CLASS, prob, 0, \
       wt, cost, sdam, ldam, hitbon, ammotyp, wt, color )

#define GUN(name,app,kn,bi,prob,wt,cost,hitbon,ammotyp,metal,sub,color) \
	OBJECT(OBJ(name,app), \
       BITS(kn,0,1,0,0,1,0,0,bi,0,0,sub,metal), 0, \
       WEAPON_CLASS, prob, 0, \
       wt, cost, 2, 2, hitbon, ammotyp, wt, color )

/* Note: for weapons that don't do an even die of damage (ex. 2-7 or 3-18)
   * the extra damage is added on in weapon.c, not here! */

#define P PIERCE
#define S SLASH
#define B WHACK

/* missiles; materiel reflects the arrowhead, not the shaft */
PROJECTILE("arrow", None,
           1, 67, 1, 2, 6, 6, 0,        IRON, -P_BOW, HI_METAL),
PROJECTILE("elven arrow", "runed arrow",
           0, 20, 1, 2, 7, 6, 0,        WOOD, -P_BOW, HI_WOOD),
PROJECTILE("orcish arrow", "crude arrow",
           0, 20, 1, 2, 5, 6, 0,        IRON, -P_BOW, CLR_BLACK),
PROJECTILE("ya", "long arrow",
           0, 15, 1, 4, 7, 7, 1,        WOOD, -P_BOW, HI_WOOD),
PROJECTILE("light arrow", None,                                /* SpliceHack */
           1,  0, 1, 20, 12, 12, 2,     GLASS, -P_BOW, CLR_YELLOW),
PROJECTILE("crossbow bolt", None,
           1, 55, 1, 2, 4, 6, 0,        IRON, -P_CROSSBOW, HI_METAL),

/* missiles that don't use a launcher */
WEAPON("dart", None,
       1, 1, 0, 0, 60,   1,   2,  3,  2, 0, P,   -P_DART, IRON, HI_METAL),
WEAPON("shuriken", "throwing star",
       0, 1, 0, 0, 35,   1,   5,  8,  6, 2, P,   -P_SHURIKEN, IRON, HI_METAL),
WEAPON("boomerang", None,
       1, 1, 0, 0, 15,   5,  20,  9,  9, 0, B,   -P_BOOMERANG, WOOD, HI_WOOD),
WEAPON("chakram", "circular blade",                            /* SpliceHack */
       0, 1, 0, 0, 6,  5,  20, 9, 9, 0, S,   -P_BOOMERANG, SILVER, HI_SILVER),

/* spears [note: javelin used to have a separate skill from spears,
 * because the latter are primarily stabbing weapons rather than
 * throwing ones; but for playability, they've been merged together
 * under spear skill and spears can now be thrown like javelins] */
WEAPON("spear", None,
       1, 1, 0, 0, 52,  30,   3,  6,  8, 0, P,   P_SPEAR, IRON, HI_METAL),
WEAPON("elven spear", "runed spear",
       0, 1, 0, 0, 10,  11,   3,  7,  8, 0, P,   P_SPEAR, WOOD, HI_WOOD),
WEAPON("orcish spear", "crude spear",
       0, 1, 0, 0, 13,  30,   3,  5,  8, 0, P,   P_SPEAR, IRON, CLR_BLACK),
WEAPON("dwarvish spear", "stout spear",
       0, 1, 0, 0, 12,  30,   3,  8,  8, 0, P,   P_SPEAR, IRON, HI_METAL),
WEAPON("javelin", "throwing spear",
       0, 1, 0, 0, 10,  20,   3,  6,  6, 0, P,   P_SPEAR, IRON, HI_METAL),
/* Base weapon for the artifact weapon Gungnir */
WEAPON("atgeir", "bladed spear",                                /* dNethack */
       0, 0, 0, 0,  5,  45,  15,  4, 12, 0, P,   P_SPEAR, IRON, HI_METAL),

/* spearish; doesn't stack, not intended to be thrown */
WEAPON("trident", None,
       1, 0, 0, 0,  8,  20,   5,  6,  4, 0, P,   P_TRIDENT, IRON, HI_METAL),
        /* +1 small, +2d4 large */

/* blades; all stack */
WEAPON("dagger", None,
       1, 1, 0, 0, 33,  10,   4,  4,  3, 2, P,   P_DAGGER, IRON, HI_METAL),
WEAPON("elven dagger", "runed dagger",
       0, 1, 0, 0, 10,   4,   4,  5,  3, 2, P,   P_DAGGER, WOOD, HI_WOOD),
WEAPON("orcish dagger", "crude dagger",
       0, 1, 0, 0, 12,  10,   4,  3,  3, 2, P,   P_DAGGER, IRON, CLR_BLACK),
WEAPON("athame", None,
       1, 1, 0, 0,  0,  10,   4,  4,  3, 2, S,   P_DAGGER, METAL, HI_METAL),
WEAPON("parazonium", "triangular dagger",                      /* SpliceHack */
       0, 1, 0, 0,  1,  12,  15,  6,  4, 1, S,   P_DAGGER, METAL, HI_METAL),
WEAPON("stake", None,                               /* Slash'EM */
       1, 0, 0, 0,  5,  20,  20,  6,  6, 0, P,   P_DAGGER, WOOD, HI_WOOD),
WEAPON("great dagger", None,                               /* Slash'EM */
       1, 0, 0, 0,  0,  20,  30,  6,  7, 2, P,   P_DAGGER, METAL, CLR_BLACK),
WEAPON("pencil", None,
       1, 1, 0, 0,  3,   2,   4,  2,  2, 1, P,   P_DAGGER, WOOD, CLR_YELLOW),
WEAPON("scalpel", None,
       1, 1, 0, 0,  0,   5,   6,  3,  3, 2, S,   P_KNIFE, METAL, HI_METAL),
WEAPON("knife", None,
       1, 1, 0, 0, 20,   5,   4,  3,  2, 0, S, P_KNIFE, IRON, HI_METAL),
WEAPON("stiletto", None,
       1, 1, 0, 0,  5,   5,   4,  3,  2, 0, P, P_KNIFE, IRON, HI_METAL),
/* 3.6: worm teeth and crysknives now stack;
 * when a stack of teeth is enchanted at once, they fuse into one crysknife;
 * when a stack of crysknives drops, the whole stack reverts to teeth */
WEAPON("worm tooth", None,
       1, 1, 0, 0,  0,  20,   2,  2,  2, 0, P,   P_KNIFE, BONE, CLR_WHITE),
WEAPON("crysknife", None,
       1, 1, 0, 0,  0,  20, 100, 20, 30, 3, P,   P_KNIFE, BONE, CLR_WHITE),

/* axes */
WEAPON("axe", None,
       1, 0, 0,  0, 40,  60,   8,  6,  4, 0, S,   P_AXE, IRON, HI_METAL),
WEAPON("throwing axe", None,                                   /* SpliceHack */
       1, 1, 0,  0, 5,   20,  10,  5,  4, 2, S,   P_AXE, IRON, HI_METAL),
WEAPON("dwarvish bearded axe", "broad bearded axe",            /* EvilHack */
       0, 0, 0 , 0, 20,  70,  25,  8, 10, 0, S,   P_AXE, IRON, HI_METAL),
WEAPON("battle-axe", "double-headed axe", /* "double-bitted"? */
       0, 0, 0, 1, 10, 120,  40,  8,  6, 0, S,   P_AXE, IRON, HI_METAL),

/* swords */
WEAPON("short sword", None,
       1, 0, 0, 0,  8,  30,  10,  6,  8, 0, P,   P_SHORT_SWORD, IRON, HI_METAL),
WEAPON("elven short sword", "runed short sword",
       0, 0, 0, 0,  2,  11,  10,  8,  8, 0, P,   P_SHORT_SWORD, WOOD, HI_WOOD),
WEAPON("orcish short sword", "crude short sword",
       0, 0, 0, 0,  3,  30,  10,  5,  8, 0, P,   P_SHORT_SWORD, IRON, CLR_BLACK),
WEAPON("dwarvish short sword", "broad short sword",
       0, 0, 0, 0,  2,  30,  10,  7,  8, 0, P,   P_SHORT_SWORD, IRON, HI_METAL),
WEAPON("gladius", None,                                        /* SpliceHack */
       1, 0, 0, 0,  1,  30,  10, 10,  6, 2, S,   P_SHORT_SWORD, IRON, HI_METAL),
WEAPON("scimitar", "curved sword",
       0, 0, 0, 0, 15,  40,  15,  8,  8, 0, S,   P_SABER, IRON, HI_METAL),
WEAPON("falchion", "heavy sword",                              /* SpliceHack */
       0, 0, 0, 1, 10,  160, 55, 12, 16, 1, S,   P_SABER, IRON, HI_METAL),
WEAPON("orcish scimitar", "crude curved sword",
       0, 0, 0, 0, 15,  40,  15,  6,  8, 0, S,   P_SABER, IRON, CLR_BLACK),
WEAPON("saber", None,
       1, 0, 0, 0,  6,  40,  75,  8,  8, 0, S,   P_SABER, IRON, HI_METAL),
WEAPON("rapier", None,                                           /* Slash'EM */
       1, 0, 0,  0, 10,  30,  40,  6,  8,  0, P,  P_SABER, METAL, CLR_BLACK),
WEAPON("broadsword", None,
       1, 0, 0, 0,  8,  70,  10,  4,  6, 0, S,   P_BROAD_SWORD, IRON, HI_METAL),
        /* +d4 small, +1 large */
WEAPON("elven broadsword", "runed broadsword",
       0, 0, 0, 0,  4,  26,  10,  6,  6, 0, S,   P_BROAD_SWORD, WOOD, HI_WOOD),
        /* +d4 small, +1 large */
WEAPON("long sword", None,
       1, 0, 0, 0, 50,  40,  15,  8, 12, 0, S,   P_LONG_SWORD, IRON, HI_METAL),
WEAPON("elven long sword", "runed long sword",
       0, 0, 0, 0, 10,  15,  40, 10, 12, 0, S,   P_LONG_SWORD, WOOD, HI_WOOD),
WEAPON("orcish long sword", "crude long sword",
       0, 0, 0, 0, 10,  40,  12,  8, 10, 0, S,   P_LONG_SWORD, IRON, CLR_BLACK),
WEAPON("two-handed sword", None,
       1, 0, 0, 1, 22, 150,  50, 12,  6, 0, S,   P_TWO_HANDED_SWORD,
                                                            IRON, HI_METAL),
        /* +2d6 large */
WEAPON("katana", "samurai sword",
       0, 0, 0, 0,  4,  40,  80, 10, 12, 1, S,   P_LONG_SWORD, METAL, HI_METAL),
/* special swords set up for artifacts */
WEAPON("tsurugi", "long samurai sword",
       0, 0, 0, 1,  0,  60, 500, 16,  8, 2, S,   P_TWO_HANDED_SWORD,
                                                            METAL, HI_METAL),
        /* +2d6 large */
WEAPON("runesword", "runed broadsword",
       0, 0, 0, 0,  0,  40, 300,  4,  6, 0, S,   P_BROAD_SWORD, IRON, CLR_BLACK),
        /* +d4 small, +1 large; Stormbringer: +5d2 +d8 from level drain */

/* polearms */
/* spear-type */
WEAPON("partisan", "vulgar polearm",
       0, 0, 0, 1,  5,  80,  10,  6,  6, 0, P,   P_POLEARMS, IRON, HI_METAL),
        /* +1 large */
WEAPON("ranseur", "hilted polearm",
       0, 0, 0, 1,  5,  50,   6,  4,  4, 0, P,   P_POLEARMS, IRON, HI_METAL),
        /* +d4 both */
WEAPON("spetum", "forked polearm",
       0, 0, 0, 1,  5,  50,   5,  6,  6, 0, P,   P_POLEARMS, IRON, HI_METAL),
        /* +1 small, +d6 large */
WEAPON("glaive", "single-edged polearm",
       0, 0, 0, 1,  8,  75,   6,  6, 10, 0, S,   P_POLEARMS, IRON, HI_METAL),
WEAPON("lance", None,
       1, 0, 0, 0,  4, 180,  10,  6,  8, 0, P,   P_LANCE, METAL, HI_METAL),
        /* +2d10 when jousting with lance as primary weapon */
/* axe-type */
WEAPON("halberd", "angled poleaxe",
       0, 0, 0, 1,  8, 150,  10, 10,  6, 0, S, P_POLEARMS, IRON, HI_METAL),
        /* +1d6 large */
WEAPON("bardiche", "long poleaxe",
       0, 0, 0, 1,  4, 120,   7,  4,  4, 0, S,   P_POLEARMS, IRON, HI_METAL),
        /* +1d4 small, +2d4 large */
WEAPON("voulge", "pole cleaver",
       0, 0, 0, 1,  4, 125,   5,  4,  4, 0, S,   P_POLEARMS, IRON, HI_METAL),
        /* +d4 both */
WEAPON("dwarvish mattock", "broad pick",
       0, 0, 0, 1, 13, 120,  50, 12,  8, -1, B,  P_PICK_AXE, IRON, HI_METAL),
/* curved/hooked */
WEAPON("fauchard", "pole sickle",
       0, 0, 0, 1,  6,  60,   5,  6,  8, 0, S, P_POLEARMS, IRON, HI_METAL),
WEAPON("guisarme", "pruning hook",
       0, 0, 0, 1,  6,  80,   5,  4,  8, 0, S,   P_POLEARMS, IRON, HI_METAL),
        /* +1d4 small */
WEAPON("bill-guisarme", "hooked polearm",
       0, 0, 0, 1,  4, 120,   7,  4, 10, 0, S, P_POLEARMS, IRON, HI_METAL),
        /* +1d4 small */
/* other */
WEAPON("lucern hammer", "pronged polearm",
       0, 0, 0, 1,  5, 150,   7,  4,  6, 0, B, P_POLEARMS, IRON, HI_METAL),
        /* +1d4 small */
WEAPON("bec de corbin", "beaked polearm",
       0, 0, 0, 1,  4, 100,   8,  8,  6, 0, P, P_POLEARMS, IRON, HI_METAL),
WEAPON("scythe", "curved polearm",                             /* SpliceHack */
       0, 0, 0, 1,  0,  60,   5,  8, 10, 0, S, P_POLEARMS, IRON, HI_METAL),
        /* +1d4 both */
/* bludgeons */
WEAPON("mace", None,
       1, 0, 0, 0, 30,  30,   5,  6,  6, 0, B,   P_MACE, IRON, HI_METAL),
WEAPON("heavy mace", None,                                     /* EvilHack */
       1, 0, 0, 0, 15,  50,  10, 10, 10, 0, B,   P_MACE, IRON, HI_METAL),
        /* +1 small */
WEAPON("executioner's mace", None, /* Originally B|S */        /* SpliceHack */
       1, 0, 0, 0,  0,  65,  40, 12, 12, 0, B,   P_MACE, IRON, HI_METAL),
        /* +1 small */
/* placeholder for certain special weapons; does not spawn randomly */
WEAPON("rod", "ornate mace",                                   /* EvilHack */
       0, 0, 0, 0,  0,  40, 250,  6,  8, 0, B,   P_MACE, GEMSTONE, CLR_RED),
WEAPON("morning star", None,
       1, 0, 0, 0, 12, 120,  10,  4,  6, 0, B,   P_MORNING_STAR, IRON, HI_METAL),
        /* +d4 small, +1 large */
WEAPON("orcish morning star", "crude morning star",
       0, 0, 0, 0,  8, 120,  10,  4,  6, 0, B,   P_MORNING_STAR, IRON, CLR_BLACK),
        /* +d4 small, +1 large */
WEAPON("war hammer", None,
       1, 0, 0, 0, 15,  50,   5,  4,  4, 0, B,   P_HAMMER, IRON, HI_METAL),
        /* +1 small */
WEAPON("heavy war hammer", None,                               /* EvilHack */
       1, 0, 0, 0, 10,  60,  10,  8, 10, 0, B,   P_HAMMER, IRON, HI_METAL),
WEAPON("club", None,
       1, 0, 0, 0, 12,  30,   3,  6,  3, 0, B,   P_CLUB, WOOD, HI_WOOD),
WEAPON("rubber hose", None,
       1, 0, 0, 0,  0,  20,   3,  4,  3, 0, B,   P_WHIP, PLASTIC, CLR_BROWN),
WEAPON("quarterstaff", "staff",
       0, 0, 0, 1, 11,  40,   5,  6,  6, 0, B,   P_QUARTERSTAFF, WOOD, HI_WOOD),
WEAPON("silver capped staff", None,
       0, 0, 1, 1,  5,  45, 100,  6,  6, 0, B,	 P_QUARTERSTAFF, SILVER, HI_SILVER),
WEAPON("staff of divination", "wormwood staff",                /* EvilHack */
       0, 0, 1, 1,  5,  40, 400,  6,  6, 0, B,   P_QUARTERSTAFF, WOOD, HI_WOOD),
WEAPON("staff of healing", "twisted staff",                    /* EvilHack */
       0, 0, 1, 1,  5,  40, 400,  6,  6, 0, B,   P_QUARTERSTAFF, WOOD, HI_WOOD),
WEAPON("staff of necromancy", "bone-carved staff",             
       0, 0, 1, 1,  5,  40, 400,  6,  6, 0, B,   P_QUARTERSTAFF, BONE, CLR_WHITE),
WEAPON("staff of matter", "etched staff",                      /* EvilHack */
       0, 0, 1, 1,  5,  30, 450,  6,  6, 0, B,   P_QUARTERSTAFF, MITHRIL, HI_SILVER),
WEAPON("staff of escape", "darkwood staff",                    /* EvilHack */
       0, 0, 1, 1,  5,  40, 400,  6,  6, 0, B,   P_QUARTERSTAFF, WOOD, CLR_BLACK),
WEAPON("staff of war", "ironshod staff",                       /* EvilHack */
       0, 0, 1, 1,  5,  50, 425,  6,  6, 2, B,   P_QUARTERSTAFF, IRON, HI_METAL),
/* two-piece */
WEAPON("aklys", "thonged club",
       0, 0, 0, 0,  8,  15,   4,  6,  3, 0, B,   P_CLUB, IRON, HI_METAL),
/* Base for artifact */
WEAPON("baseball bat", None,                                    /* Slash'EM */
	1, 0, 0, 0,  0,  40,  20,  8,  6, 0, B,   P_CLUB, WOOD, HI_WOOD),
WEAPON("flail", None,
       1, 0, 0, 0, 40,  15,   4,  6,  4, 0, B,   P_FLAIL, IRON, HI_METAL),
        /* +1 small, +1d4 large */
/* template for Yeenoghu's artifact weapon; does not spawn randomly */
WEAPON("triple-headed flail", None,                            /* EvilHack */
       1, 0, 0, 1,  0, 150,  30, 12,  6, 3, B,   P_FLAIL, BONE, CLR_WHITE),
        /* +d4 small, +3d6 large */
/* many-piece */
WEAPON("spiked chain", None, /* Originally P|S */              /* SpliceHack */
       1, 0, 0, 1,  3,  50,  80,  6,  4, 1, S,   P_FLAIL, IRON, HI_METAL),

/* misc */
WEAPON("bullwhip", None,
       1, 0, 0, 0,  2,  20,   4,  2,  1, 0, 0,   P_WHIP, LEATHER, CLR_BROWN),
WEAPON("flaming lash", None,                                   /* SpliceHack */
       1, 0, 0, 0,  0, 180,   4, 12, 12, 0, 0,   P_WHIP, DRAGON_HIDE, CLR_RED),
/* Good to-hit and small damage, but low large damage */
WEAPON("fly swatter", None,                                /* Slash'EM */
	1, 0, 0, 0,  2,  10,   3, 10,  2, 2, B,   P_WHIP, PLASTIC, CLR_GREEN),


/* LIGHTSABERS */
/* [WAC]
 * Lightsabers are -3 to hit 
 * Double lightsaber is -4 to hit (only red)
 * DMG is increased: 10.5/15.5
 * green :9 + d3, 13 + d5
 * blue : 8 + d5, 12 + d7
 * red :  6 + d9, 10 + d11
 * red double: 6 + d9 + d9, 10 + d11 + d11  (15/21) in double mode
 */
WEAPON("green lightsaber", "lightsaber",                             /* Slash'EM */
	0, 0, 1, 0,  1, 60, 500, 3,  5, -3, SLASH, P_LIGHTSABER, PLASTIC, HI_METAL),
WEAPON("blue lightsaber",  "lightsaber",                             /* Slash'EM */
	0, 0, 1, 0,  1, 60, 500, 5,  7, -3, SLASH, P_LIGHTSABER, PLATINUM, HI_METAL),
WEAPON("red lightsaber",  "lightsaber",                              /* Slash'EM */
	0, 0, 1, 0,  1, 60, 500, 9,  11, -3, SLASH, P_LIGHTSABER, PLATINUM, HI_METAL),
WEAPON("red double lightsaber",  "double lightsaber",                /* Slash'EM */
	0, 0, 1, 1,  0, 60,1000, 9,  11, -4, SLASH, P_LIGHTSABER, PLATINUM, HI_METAL),

/* launchers */
BOW("bow", None,               1, 1, 24, 30, 60, 0, WOOD, P_BOW, HI_WOOD),
BOW("elven bow", "runed bow",  0, 1, 12, 30, 60, 0, WOOD, P_BOW, HI_WOOD),
BOW("footbow", "warped bow",   0, 0,  0, 60, 60, 0, BONE, P_BOW, CLR_WHITE), /* SpliceHack */
BOW("orcish bow", "crude bow", 0, 1, 12, 30, 60, 0, WOOD, P_BOW, CLR_BLACK),
BOW("yumi", "long bow",        0, 1,  0, 30, 60, 0, WOOD, P_BOW, HI_WOOD),
BOW("sling", None,             1, 0, 40,  3, 20, 0, LEATHER, P_SLING, HI_LEATHER),
BOW("crossbow", None,          1, 1, 45, 50, 40, 0, WOOD, P_CROSSBOW, HI_WOOD),

/* firearms */
GUN("flintlock",            "broken hand-crossbow", 0, 0, 0,  10,   50,  0, WP_BULLET, IRON, P_FIREARM, HI_METAL), /* dNetHack */
GUN("pistol",               "broken hand-crossbow", 0, 0, 0,  20,  100,  0, WP_BULLET, IRON, P_FIREARM, HI_METAL), /* Slash'EM */
GUN("submachine gun",    "strange broken crossbow", 0, 0, 0,  25,  250, -1, WP_BULLET, IRON, P_FIREARM, HI_METAL), /* Slash'EM */
GUN("heavy machine gun", "strange broken crossbow", 0, 1, 0, 200, 2000, -4, WP_BULLET, IRON, P_FIREARM, HI_METAL), /* Slash'EM */
GUN("rifle",                     "broken crossbow", 0, 1, 0,  30,  150,  1, WP_BULLET, IRON, P_FIREARM, HI_METAL), /* Slash'EM */
GUN("assault rifle",             "broken crossbow", 0, 0, 0,  40, 1000, -2, WP_BULLET, IRON, P_FIREARM, HI_METAL),
GUN("sniper rifle",              "broken crossbow", 0, 1, 0,  50, 4000,  4, WP_BULLET, IRON, P_FIREARM, HI_METAL), /* Slash'EM */
GUN("shotgun",                   "broken crossbow", 0, 0, 0,  35,  200,  3,  WP_SHELL, IRON, P_FIREARM, HI_METAL), /* Slash'EM */
GUN("auto shotgun",      "strange broken crossbow", 0, 1, 0,  60, 1500,  1,  WP_SHELL, IRON, P_FIREARM, HI_METAL), /* Slash'EM */

BULLET("bullet",          "pellet", 1, 0, 1,   5,   20, 30, 0, WP_BULLET, P, IRON, -P_FIREARM, HI_METAL), /* Slash'EM */
BULLET("shotgun shell", "red tube", 1, 0, 1,   7,   30, 45, 0, WP_SHELL,  P, IRON, -P_FIREARM, CLR_RED), /* Slash'EM */
BULLET("fire bomb",         "bomb", 0, 0,10,  50,    0,  0, 0, WP_BOMB,   B, IRON, P_NONE, CLR_GREEN), /* Slash'EM */
BULLET("sonic bomb",        "bomb", 0, 0,10,  50,    0,  0, 0, WP_BOMB,   B, IRON, P_NONE, CLR_BLACK), /* Slash'EM */
BULLET("gas bomb",          "bomb", 0, 0,10,  50,    0,  0, 0, WP_BOMB,   B, IRON, P_NONE, CLR_ORANGE), /* Slash'EM */

#undef P
#undef S
#undef B

#undef WEAPON
#undef PROJECTILE
#undef BOW
#undef BULLET
#undef GUN

/* armor ... */
        /* IRON denotes ferrous metals.
         * METAL denotes stainless steel.
         * Only IRON weapons and armor can rust.
         * Only COPPER (including brass) corrodes.
         * Some creatures are vulnerable to SILVER.
         */
#define ARMOR(name,desc,kn,mgc,blk,power,prob,delay,wt,  \
              cost,ac,can,sub,metal,c)                   \
    OBJECT(OBJ(name, desc),                                         \
           BITS(kn, 0, 1, 0, mgc, 1, 0, 0, blk, 0, 0, sub, metal),  \
           power, ARMOR_CLASS, prob, delay, wt,                     \
           cost, 0, 0, 10 - ac, can, wt, c)
#define HELM(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c)  \
    ARMOR(name, desc, kn, mgc, 0, power, prob, delay, wt,  \
          cost, ac, can, ARM_HELM, metal, c)
#define CLOAK(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c)  \
    ARMOR(name, desc, kn, mgc, 0, power, prob, delay, wt,  \
          cost, ac, can, ARM_CLOAK, metal, c)
#define SHIELD(name,desc,kn,mgc,blk,power,prob,delay,wt,cost,ac,can,metal,c) \
    ARMOR(name, desc, kn, mgc, blk, power, prob, delay, wt, \
          cost, ac, can, ARM_SHIELD, metal, c)
#define GLOVES(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c)  \
    ARMOR(name, desc, kn, mgc, 0, power, prob, delay, wt,  \
          cost, ac, can, ARM_GLOVES, metal, c)
#define BOOTS(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c)  \
    ARMOR(name, desc, kn, mgc, 0, power, prob, delay, wt,  \
          cost, ac, can, ARM_BOOTS, metal, c)

/* helmets */
HELM("dunce cap", "conical hat",
     0, 1,           0,  3, 1,  4,  1, 10, 0, CLOTH, CLR_BLUE),
HELM("cornuthaum", "conical hat",
     0, 1, CLAIRVOYANT,  3, 1,  4, 80, 10, 1, CLOTH, CLR_BLUE),
        /* name coined by devteam; confers clairvoyance for wizards,
           blocks clairvoyance if worn by role other than wizard */
HELM("fedora", None,
     1, 0,           0,  0, 0,  3,  1, 10, 0, CLOTH, CLR_BROWN),
HELM("toque", None,                                            /* EvilHack */
     1, 0,   SONIC_RES,  0, 0,  3,  1, 10, 0, CLOTH, CLR_ORANGE),
HELM("elven helm", "hat",
     0, 0,           0,  6, 1,  3,  8,  9, 0, LEATHER, HI_LEATHER),
HELM("orcish helm", "skull cap",
     0, 0,           0,  6, 1, 30, 10,  9, 0, IRON, CLR_BLACK),
HELM("dwarvish helm", "hard hat",
     0, 0,           0,  6, 1, 40, 20,  8, 0, IRON, HI_METAL),
HELM("plasteel helm", None,
     1, 0,           0,  1, 1,  6, 20,  7, 0, PLASTIC, CLR_WHITE),
HELM("dented pot", None,
     1, 0,           0,  2, 0, 10,  8,  9, 0, IRON, CLR_BLACK),
/* with shuffled appearances... */
HELM("helmet", "plumed helmet",
     0, 0,           0, 10, 1, 30, 10,  9, 0, IRON, HI_METAL),
HELM("helm of brilliance", "etched helmet",
     0, 1,           0,  6, 1, 50, 50,  9, 0, IRON, CLR_GREEN),
HELM("helm of opposite alignment", "crested helmet",
     0, 1,           0,  6, 1, 50, 50,  9, 0, IRON, HI_METAL),
HELM("helm of speed", "winged helmet",                          /* EvilHack */
     0, 1,        FAST,  2, 1, 50, 50,  9, 0, METAL, HI_GOLD),
HELM("tinfoil hat", "fearsome helmet",                     /* SpliceHack/Un */
     0, 1, PSYCHIC_RES,  2, 1, 0, 50,  10, 0, METAL, CLR_YELLOW),
HELM("helm of madness", "top hat",                         /* SpliceHack/Un */
     0, 1, HALLUC,       2, 1, 10, 50,  9, 0, CLOTH, CLR_BLACK),
HELM("helm of telepathy", "visored helmet",
     0, 1,     TELEPAT,  2, 1, 50, 50,  9, 0, IRON, HI_METAL),

/* suits of armor */
/*
 * There is code in obj.h that assumes that the order of the dragon scales is
 * the same as order of dragons defined in monst.c.
 */
#define DRGN_SCALES(name,mgc,power,cost,color)  \
    ARMOR(name, None, 1, mgc, 1, power, 0, 5, 40,  \
          cost, 7, 0, ARM_CLOAK, DRAGON_HIDE, color)
/* For now, only dragons leave these. */
/* 3.4.1: dragon scales left classified as "non-magic"; they confer magical
 * properties but are produced "naturally"; affects use as polypile fodder */
DRGN_SCALES("gray dragon scales",       0, ANTIMAGIC,         700, CLR_GRAY),
DRGN_SCALES("silver dragon scales",     0, REFLECTING,        700, DRAGON_SILVER),
DRGN_SCALES("shimmering dragon scales", 0, DISPLACED,         700, CLR_CYAN),
DRGN_SCALES("deep dragon scales",       0, DRAIN_RES,         700, CLR_MAGENTA),
DRGN_SCALES("red dragon scales",        0, FIRE_RES,          500, CLR_RED),
DRGN_SCALES("white dragon scales",      0, COLD_RES,          500, CLR_WHITE),
DRGN_SCALES("orange dragon scales",     0, SLEEP_RES,         500, CLR_ORANGE),
DRGN_SCALES("black dragon scales",      0, DISINT_RES,        700, CLR_BLACK),
DRGN_SCALES("blue dragon scales",       0, SHOCK_RES,         500, CLR_BLUE),
DRGN_SCALES("green dragon scales",      0, POISON_RES,        500, CLR_GREEN),
DRGN_SCALES("gold dragon scales",       0, 0,                 500, HI_GOLD),
DRGN_SCALES("sea dragon scales",        0, MAGICAL_BREATHING, 500, HI_ZAP),
DRGN_SCALES("yellow dragon scales",     0, ACID_RES,          500, CLR_YELLOW),
DRGN_SCALES("celestial dragon scales",  0, FLYING,           1000, CLR_BRIGHT_MAGENTA),
DRGN_SCALES("chromatic dragon scales",  0, ANTIMAGIC,        1500, CLR_MAGENTA),
#undef DRGN_SCALES
/* other suits */
ARMOR("plate mail", None,
      1, 0, 1,  0, 44, 5, 350, 600,  3, 2,  ARM_SUIT, IRON, HI_METAL),
ARMOR("crystal plate mail", None,
      1, 0, 1,  0,  4, 5, 150, 5000, 3, 2,  ARM_SUIT, GEMSTONE, CLR_WHITE),
ARMOR("splint mail", None,
      1, 0, 1,  0, 62, 5, 300,  80,  4, 1,  ARM_SUIT, IRON, HI_METAL),
/* special suit of armor for giant samurai only (no random spawn) */
ARMOR("large splint mail", None,
      1, 0, 1,  0,  0, 5, 500, 150,  4, 1,  ARM_SUIT, IRON, HI_METAL),
ARMOR("banded mail", None,
      1, 0, 1,  0, 72, 5, 250,  90,  4, 1,  ARM_SUIT, IRON, HI_METAL),
ARMOR("plasteel armor",None,
	1, 0, 1, 0, 5, 5, 150,  80,  4, 0,  ARM_SUIT, PLASTIC, CLR_WHITE),
ARMOR("chain mail", None,
      1, 0, 0,  0, 75, 5, 200,  75,  5, 1,  ARM_SUIT, IRON, HI_METAL),
ARMOR("dwarvish chain mail", None,
      1, 0, 0,  0, 12, 1, 200,  75,  4, 2,  ARM_SUIT, IRON, HI_METAL),
ARMOR("elven chain mail", None,
      1, 0, 0,  0, 15, 1, 200,  75,  5, 2,  ARM_SUIT, COPPER, HI_COPPER),
ARMOR("orcish chain mail", "crude chain mail",
      0, 0, 0,  0, 20, 5, 200,  75,  6, 1,  ARM_SUIT, IRON, CLR_BLACK),
ARMOR("scale mail", None,
      1, 0, 0,  0, 72, 5, 175,  45,  6, 1,  ARM_SUIT, IRON, HI_METAL),
ARMOR("studded armor", None,
      1, 0, 0,  0, 75, 3, 125,  15,  7, 1,  ARM_SUIT, LEATHER, HI_LEATHER),
ARMOR("ring mail", None,
      1, 0, 0,  0, 72, 5, 150, 100,  7, 1,  ARM_SUIT, IRON, HI_METAL),
ARMOR("orcish ring mail", "crude ring mail",
      0, 0, 0,  0, 20, 5, 150,  80,  8, 1,  ARM_SUIT, IRON, CLR_BLACK),
ARMOR("light armor", None,
      1, 0, 0,  0, 82, 3,  75,   5,  8, 1,  ARM_SUIT, LEATHER, HI_LEATHER),
ARMOR("jacket", None,
      1, 0, 0,  0, 12, 0,  30,  10,  9, 0,  ARM_SUIT, LEATHER, CLR_BLACK),

/* Robes */
ARMOR("robe", "red robe",
      0, 0, 0, 0,           1, 1, 40, 25, 9, 0, ARM_SUIT, LEATHER, CLR_RED),
ARMOR("robe of protection", "blue robe",                        /* Slash'EM */
      0, 1, 0, 0,           1, 1, 40, 50, 5, 1, ARM_SUIT, LEATHER, CLR_BLUE),
ARMOR("robe of power", "orange robe",                           /* Slash'EM */
      0, 1, 0, 0,           0, 1, 40, 50, 9, 0, ARM_SUIT, LEATHER, CLR_ORANGE),
ARMOR("robe of weakness", "green robe",                         /* Slash'EM */
      0, 1, 0, 0,           1, 1, 40, 50, 9, 0, ARM_SUIT, LEATHER, CLR_GREEN),

/* shirts */
ARMOR("Hawaiian shirt", None,
      1, 0, 0,  0, 10, 0,   5,   3, 10, 0,  ARM_SHIRT, CLOTH, CLR_MAGENTA),
ARMOR("striped shirt", None,
      1, 0, 0,  0,  0, 0,   5,   2, 10, 0,  ARM_SHIRT, CLOTH, CLR_GRAY),
/* Ruffled shirts are little different from other shirts */
ARMOR("ruffled shirt", None,
      1, 0, 0,  0,  2, 0,   5,   2, 10, 0,  ARM_SHIRT, CLOTH, CLR_WHITE),
ARMOR("T-shirt", None,
      1, 0, 0,  0,  2, 0,   5,   2, 10, 0,  ARM_SHIRT, CLOTH, CLR_WHITE),

/* cloaks */
CLOAK("mummy wrapping", None,
      1, 0,          0,  0, 0,  3,  2, 10, 1,  CLOTH, CLR_GRAY),
        /* worn mummy wrapping blocks invisibility */
CLOAK("elven cloak", "faded pall",
      0, 1,    STEALTH,  8, 0, 10, 60,  9, 1,  CLOTH, CLR_BLACK),
CLOAK("orcish cloak", "coarse mantelet",
      0, 0,          0,  8, 0, 10, 40, 10, 1,  CLOTH, CLR_BLACK),
CLOAK("dwarvish cloak", "hooded cloak",
      0, 0,          0,  8, 0, 10, 50, 10, 1,  CLOTH, HI_CLOTH),
CLOAK("oilskin cloak", "slippery cloak",
      0, 0,          0,  8, 0, 10, 50,  9, 2,  CLOTH, HI_CLOTH),
CLOAK("alchemy smock", "apron",
      0, 1, POISON_RES,  9, 0, 10, 50,  9, 1,  CLOTH, CLR_WHITE),
CLOAK("green coat", None, 
      1, 0,          0,  0, 0, 45, 50,  9, 2,  LEATHER, CLR_BRIGHT_GREEN),
CLOAK("plain cloak", None,
      1, 0,          0,  8, 0, 15, 40,  9, 1,  LEATHER, CLR_BROWN),
/* with shuffled appearances... */
CLOAK("cloak of protection", "tattered cape",                  /* EvilHack */
      0, 1, PROTECTION,  9, 0, 10, 50,  7, 3,  CLOTH, HI_CLOTH),
CLOAK("cloak of invisibility", "opera cloak",
      0, 1,      INVIS, 10, 0, 10, 60,  9, 1,  CLOTH, CLR_BRIGHT_MAGENTA),
CLOAK("cloak of flight", "feathery cloak",                     /* SpliceHack */
      0, 1,      FLYING, 2, 0, 10, 60,  9, 1,  CLOTH, CLR_CYAN),
CLOAK("cloak of magic resistance", "ornamental cope",
      0, 1,  ANTIMAGIC,  1, 0, 10, 60,  9, 1,  CLOTH, CLR_WHITE),
        /*  'cope' is not a spelling mistake... leave it be */
CLOAK("poisonous cloak", "dirty rag",                           /* Slash'EM */
      0, 1,          0,  5, 0, 10, 60, 10, 3, CLOTH, CLR_BLACK),
CLOAK("mana cloak", "funeral shroud",
      0, 1, ENERGY_REGENERATION, 4, 0, 10, 50,  9, 1, CLOTH, CLR_BLACK),
CLOAK("cloak of displacement", "dusty cloak",
      0, 1,  DISPLACED, 10, 0, 10, 50,  9, 1,  CLOTH, HI_CLOTH),

/* shields */
SHIELD("small shield", None,
       1, 0, 0,           0, 4, 0,  30,   3, 9, 0,  WOOD, HI_WOOD),
SHIELD("elven shield", "blue and green shield",
       0, 0, 0,           0, 2, 0,  40,   7, 8, 0,  WOOD, CLR_GREEN),
SHIELD("tower shield", None,                                   /* SpliceHack */
       1, 0, 1,           0, 2, 0, 200,  20, 7, 0,  IRON, HI_METAL),
SHIELD("Uruk-hai shield", "white-handed shield",
       0, 0, 0,           0, 2, 0,  50,   7, 9, 0,  IRON, HI_METAL),
SHIELD("orcish shield", "red-eyed shield",
       0, 0, 0,           0, 2, 0,  50,   7, 9, 0,  IRON, CLR_RED),
SHIELD("large shield", None,
       1, 0, 1,           0, 5, 0, 100,  10, 8, 0,  IRON, HI_METAL),
SHIELD("hide shield", None,                                    /* SpliceHack */
       1, 0, 1,           0, 2, 0,  25,   8, 8, 0,  LEATHER, CLR_ORANGE),
SHIELD("dwarvish roundshield", "large round shield",
       0, 0, 0,           0, 2, 0, 100,  10, 8, 0,  IRON, HI_METAL),
SHIELD("shield of reflection", "polished shield",
       0, 1, 0,  REFLECTING, 3, 0,  50,  50, 8, 0,  SILVER, HI_SILVER),
SHIELD("shield of light", "shiny shield",                      /* EvilHack */
       0, 1, 0,           0, 3, 0,  60, 400, 8, 0,  GOLD, CLR_YELLOW),
SHIELD("resonant shield", "humming shield",                    /* SpliceHack */
       0, 1, 0,   SONIC_RES, 3, 0,  50,  50, 8, 0,  SILVER, HI_METAL),
SHIELD("shield of mobility", "slippery shield",                /* EvilHack */
       0, 1, 0, FREE_ACTION, 3, 0,  50, 300, 8, 0,  METAL, HI_METAL),

/* gloves */
/* These have their color but not material shuffled, so the IRON must
 * stay CLR_BROWN (== HI_LEATHER) even though it's normally either
 * HI_METAL or CLR_BLACK.  All have shuffled descriptions.
 */
GLOVES("gloves", "old gloves",
       0, 0,          0, 16, 1, 10,   8, 9, 0,  LEATHER, HI_LEATHER),
GLOVES("gauntlets", "falconry gloves",                          /* EvilHack */
       0, 0,          0, 12, 1, 30,  50, 9, 0,  IRON, CLR_BROWN),
GLOVES("gauntlets of power", "riding gloves",
       0, 1,          0,  8, 1, 30,  50, 9, 0,  IRON, CLR_BROWN),
GLOVES("rogues gloves", "fine gloves",                   /* SpliceHack */
       0, 1,  SEARCHING,  4, 1, 10,  50, 9, 0,  LEATHER, HI_LEATHER),
GLOVES("boxing gloves", "boiled gloves",                      /* SpliceHack */
       0, 1,          0,  4, 1, 10,  50, 9, 0,  LEATHER, HI_LEATHER),
GLOVES("gauntlets of protection", "gardening gloves",           /* EvilHack */
       0, 1, PROTECTION,  4, 1, 10,  60, 8, 3,  CLOTH, CLR_BROWN),
GLOVES("gauntlets of fumbling", "padded gloves",
       0, 1,   FUMBLING,  8, 1, 10,  50, 9, 0,  LEATHER, HI_LEATHER),
GLOVES("gauntlets of swimming", "black gloves",                 /* Slash'EM */
	0, 1,  SWIMMING,   8, 1, 10,  50, 9, 0,  LEATHER, HI_LEATHER),
GLOVES("gauntlets of dexterity", "fencing gloves",
       0, 1,          0,  8, 1, 10,  50, 9, 0,  LEATHER, HI_LEATHER),
GLOVES("plasteel gloves", "white gloves",
       0, 0,          0,  1, 1,  9, 25,  8, 0, PLASTIC, CLR_WHITE),

/* boots */
BOOTS("low boots", "walking shoes",
      0, 0,          0, 25, 2, 10,  8, 9, 0, LEATHER, HI_LEATHER),
BOOTS("plasteel boots", "white boots",
      0, 0,          0,  1, 2, 8,  25, 8, 0, PLASTIC, CLR_WHITE),
BOOTS("dwarvish boots", "hard shoes",
      0, 0,          0,  7, 2, 50, 16, 8, 0, IRON, HI_METAL),
BOOTS("orcish boots", "crude shoes",
      0, 0,          0,  8, 2, 50, 16, 8, 0, IRON, CLR_BLACK),
BOOTS("high boots", "jackboots",
      0, 0,          0, 15, 2, 20, 12, 8, 0, LEATHER, HI_LEATHER),
/* with shuffled appearances... */
BOOTS("speed boots", "combat boots",
      0, 1,       FAST,  8, 2, 20, 50, 9, 0, LEATHER, HI_LEATHER),
BOOTS("water walking boots", "jungle boots",
      0, 1,   WWALKING, 12, 2, 15, 50, 9, 0, LEATHER, HI_LEATHER),
BOOTS("jumping boots", "hiking boots",
      0, 1,    JUMPING, 12, 2, 20, 50, 9, 0, LEATHER, HI_LEATHER),
BOOTS("stomping boots", "steel-toed boots",                    /* SpliceHack */
      0, 1,          0, 12, 2, 20, 50, 9, 0, LEATHER, HI_LEATHER),
BOOTS("elven boots", "mud boots",
      0, 1,    STEALTH, 12, 2, 15,  8, 9, 0, LEATHER, HI_LEATHER),
BOOTS("kicking boots", "buckled boots",
      0, 1,          0, 12, 2, 50,  8, 9, 0, IRON, CLR_BROWN),
        /* CLR_BROWN for same reason as gauntlets of power */
BOOTS("fumble boots", "riding boots",
      0, 1,   FUMBLING, 12, 2, 20, 30, 9, 0, LEATHER, HI_LEATHER),
BOOTS("levitation boots", "snow boots",
      0, 1, LEVITATION, 12, 2, 15, 30, 9, 0, LEATHER, HI_LEATHER),
#undef HELM
#undef CLOAK
#undef SHIELD
#undef GLOVES
#undef BOOTS
#undef ARMOR

/* rings ... */
/* note that prob = 1 for all rings and currently can't be specified
 * per ring without changing the RING definition */
#define RING(name,stone,power,cost,prob,mgc,spec,mohs,metal,color) \
    OBJECT(OBJ(name, stone),                                          \
           BITS(0, 0, spec, 0, mgc, spec, 0, 0, 0,                    \
                HARDGEM(mohs), 0, P_NONE, metal),                     \
           power, RING_CLASS, prob, 0, 3, cost, 0, 0, 0, 0, 15, color)
RING("adornment", "wooden",                      ADORNED,                  100, 1, 1, 1, 2, WOOD, HI_WOOD),
RING("gain constitution", "opal",                0,                        150, 1, 1, 1, 7, MINERAL, HI_MINERAL),
RING("gain dexterity",  "obsidian",              0,                        150, 1, 1, 1, 7, GEMSTONE, CLR_BLACK),  /* Slash'EM */
RING("gain intelligence", "plain",               0,                        150, 1, 1, 1, 7, MINERAL,  HI_MINERAL), /* Slash'EM */
RING("gain strength", "granite",                 0,                        150, 1, 1, 1, 7, MINERAL, HI_MINERAL),  
RING("gain wisdom", "glass",                     0,                        150, 1, 1, 1, 7, GLASS,  CLR_CYAN),   /* Slash'EM */
RING("increase accuracy", "clay",                0,                        150, 1, 1, 1, 4, MINERAL, CLR_RED),
RING("increase damage", "coral",                 0,                        150, 1, 1, 1, 4, MINERAL, CLR_ORANGE),
RING("protection", "black onyx",                 PROTECTION,               100, 1, 1, 1, 7, MINERAL, CLR_BLACK),
        /* 'PROTECTION' intrinsic enhances MC from worn armor by +1,
           regardless of ring's enchantment; wearing a second ring of
           protection (or even one ring of protection combined with
           cloak of protection) doesn't give a second MC boost */
RING("regeneration", "moonstone",                REGENERATION,             200, 1, 1, 0,  6, MINERAL, HI_MINERAL),
RING("searching", "tiger eye",                   SEARCHING,                200, 1, 1, 0,  6, GEMSTONE, CLR_BROWN),
RING("stealth", "jade",                          STEALTH,                  100, 1, 1, 0,  6, GEMSTONE, CLR_GREEN),
RING("sustain ability", "bronze",                FIXED_ABIL,               100, 1, 1, 0,  4, COPPER, HI_COPPER),
RING("levitation", "agate",                      LEVITATION,               200, 1, 1, 0,  7, GEMSTONE, CLR_RED),
RING("hunger", "topaz",                          HUNGER,                   100, 1, 1, 0,  8, GEMSTONE, CLR_CYAN),
RING("aggravate monster", "sapphire",            AGGRAVATE_MONSTER,        150, 1, 1, 0,  9, GEMSTONE, CLR_BLUE),
RING("conflict", "ruby",                         CONFLICT,                 300, 1, 1, 0,  9, GEMSTONE, CLR_RED),
RING("warning", "diamond",                       WARNING,                  100, 1, 1, 0, 10, GEMSTONE, CLR_WHITE),
RING("poison resistance", "pearl",               POISON_RES,               150, 1, 1, 0,  4, BONE, CLR_WHITE),
RING("fire resistance", "iron",                  FIRE_RES,                 200, 1, 1, 0,  5, IRON, HI_METAL),
RING("cold resistance", "brass",                 COLD_RES,                 150, 1, 1, 0,  4, COPPER, HI_COPPER),
RING("psychic resistance", "crystal",            PSYCHIC_RES,              150, 1, 1, 0,  3, GLASS, HI_GLASS),     /* SpliceHack */
RING("sonic resistance",   "embossed",           SONIC_RES,                150, 1, 1, 0,  3, GLASS, HI_GLASS),     /* SpliceHack */
RING("shock resistance", "copper",               SHOCK_RES,                150, 1, 1, 0,  3, COPPER, HI_COPPER),
RING("free action", "twisted",                   FREE_ACTION,              200, 1, 1, 0,  6, METAL, HI_METAL),
RING("slow digestion", "steel",                  SLOW_DIGESTION,           200, 1, 1, 0,  8, METAL, HI_METAL),
RING("teleportation", "silver",                  TELEPORT,                 200, 1, 1, 0,  3, SILVER, HI_SILVER),
RING("teleport control", "gold",                 TELEPORT_CONTROL,         300, 1, 1, 0,  3, GOLD, HI_GOLD),
RING("polymorph", "ivory",                       POLYMORPH,                300, 1, 1, 0,  4, BONE, CLR_WHITE),
RING("polymorph control", "emerald",             POLYMORPH_CONTROL,        300, 1, 1, 0,  8, GEMSTONE, CLR_BRIGHT_GREEN),
RING("invisibility", "wire",                     INVIS,                    150, 1, 1, 0,  5, METAL, HI_METAL),
RING("see invisible", "engagement",              SEE_INVIS,                150, 1, 1, 0,  5, GOLD, HI_METAL),
RING("mood", "ridged",                           0,                        100, 1, 1, 0,  8, IRON, HI_METAL),      /* Slash'EM */
RING("sleeping", "wedding",                      SLEEPY,                   100, 1, 1, 0,  7, GEMSTONE, CLR_WHITE),   /* Slash'EM */
RING("displacement", "neon",                     DISPLACED,                200, 1, 1, 0,  0, PLASTIC, CLR_BLUE),
RING("protection from shape changers", "shiny",  PROT_FROM_SHAPE_CHANGERS, 100, 1, 1, 0,  5, PLATINUM, CLR_BRIGHT_CYAN),

/* Extra descriptions */
RING(None, "quartz",                             0,                        150, 0, 0, 0, 7, GLASS, HI_GLASS),
RING(None, "porcelain",                          0,                        150, 0, 0, 0, 8, GLASS, HI_MINERAL),
RING(None, "ceramic",                            0,                        150, 0, 0, 0, 8, GLASS, HI_MINERAL),
RING(None, "mithril",                            0,                        150, 0, 0, 0, 5, MITHRIL, HI_SILVER),
RING(None, "platinum",                           0,                        150, 0, 0, 0, 3, PLATINUM, HI_METAL),
RING(None, "jacinth",                            0,                        150, 0, 0, 0, 9, GEMSTONE, CLR_ORANGE),
RING(None, "citrine",                            0,                        150, 0, 0, 0, 6, GEMSTONE, CLR_ORANGE),
RING(None, "amber",                              0,                        150, 0, 0, 0, 2, GEMSTONE, CLR_BROWN),
RING(None, "jet",                                0,                        150, 0, 0, 0, 7, GEMSTONE, CLR_BLACK),
RING(None, "chrysoberyl",                        0,                        150, 0, 0, 0, 5, GEMSTONE, CLR_YELLOW),
RING(None, "plastic",                            0,                        150, 0, 0, 0, 1, PLASTIC, CLR_WHITE),

#undef RING

/* amulets ... - THE Amulet comes last because it is special */
#define AMULET(name,desc,power,prob) \
    OBJECT(OBJ(name, desc),                                            \
           BITS(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, P_NONE, METAL),        \
           power, AMULET_CLASS, prob, 0, 20, 150, 0, 0, 0, 0, 20, HI_METAL)
AMULET("amulet of ESP",                "circular", TELEPAT, 120),
AMULET("amulet of drain resistance",     "warped", DRAIN_RES, 60),    /* Slash'EM */
AMULET("amulet of life saving",       "spherical", LIFESAVED, 75),
AMULET("amulet of strangulation",          "oval", STRANGLED, 115),
AMULET("amulet of nausea",           "pentagonal", 0, 100),           /* SpliceHack */
AMULET("amulet of restful sleep",    "triangular", SLEEPY, 115),
AMULET("amulet versus poison",        "pyramidal", POISON_RES, 115),
AMULET("amulet of danger",         "skull-shaped", INFRAVISION, 60),  /* SpliceHack */
AMULET("amulet versus stone",            "lunate", STONE_RES, 60),    /* Slash'EM */
AMULET("amulet of change",               "square", 0, 115),
AMULET("amulet of unchanging",          "concave", UNCHANGING, 60),
AMULET("amulet of reflection",        "hexagonal", REFLECTING, 75),
AMULET("amulet of magical breathing", "octagonal", MAGICAL_BREATHING, 75),
AMULET("amulet of magic resistance",     "oblong", ANTIMAGIC, 30),
        /* +2 AC and +2 MC; +2 takes naked hero past 'warded' to 'guarded' */
AMULET("amulet of guarding",         "perforated", PROTECTION, 75),
        /* cubical: some descriptions are already three dimensional and
         * parallelogrammatical (real word!) would be way over the top */
AMULET("amulet of flying",              "cubical", FLYING, 60),       /* Slash'EM */

/* Extra descriptions */
AMULET(None,                 "convex",     0, 0),
AMULET(None,                 "spiked",     0, 0),
AMULET(None,                 "rectangular", 0, 0),
AMULET(None,                 "elliptic",   0, 0),
/* fixed descriptions; description duplication is deliberate;
 * fake one must come before real one because selection for
 * description shuffling stops when a non-magic amulet is encountered
 */
OBJECT(OBJ("cheap plastic imitation of the Amulet of Yendor",
           "Amulet of Yendor"),
       BITS(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, PLASTIC),
       0, AMULET_CLASS, 0, 0, 20, 0, 0, 0, 0, 0, 1, HI_METAL),
OBJECT(OBJ("Amulet of Yendor", /* note: description == name */
           "Amulet of Yendor"),
       BITS(0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, GEMSTONE),
       0, AMULET_CLASS, 0, 0, 20, 30000, 0, 0, 0, 0, 20, HI_METAL),
#undef AMULET

/* tools ... */
/* tools with weapon characteristics come last */
#define TOOL(name,desc,kn,mrg,mgc,chg,prob,wt,cost,mat,color) \
    OBJECT(OBJ(name, desc),                                             \
           BITS(kn, mrg, chg, 0, mgc, chg, 0, 0, 0, 0, 0, P_NONE, mat), \
           0, TOOL_CLASS, prob, 0, wt, cost, 0, 0, 0, 0, wt, color)
#define CONTAINER(name,desc,kn,mgc,chg,prob,wt,cost,mat,color) \
    OBJECT(OBJ(name, desc),                                             \
           BITS(kn, 0, chg, 1, mgc, chg, 0, 0, 0, 0, 0, P_NONE, mat),   \
           0, TOOL_CLASS, prob, 0, wt, cost, 0, 0, 0, 0, wt, color)

           
/* containers */
CONTAINER("large box",        None, 1, 0, 0, 40, 350,   8, WOOD, HI_WOOD),
CONTAINER("chest",            None, 1, 0, 0, 25, 600,  16, WOOD, HI_WOOD),
CONTAINER("iron safe",        None, 1, 0, 0, 10, 900,  50, IRON, HI_METAL),  /* EvilHack/Un */
CONTAINER("crystal chest",    None, 1, 1, 0,  1, 500,  20, GEMSTONE, CLR_WHITE), /* EvilHack */
CONTAINER("ice box",          None, 1, 0, 0,  5, 900,  42, PLASTIC, CLR_WHITE),
CONTAINER("medical kit","white bag",0, 0, 0, 10,  25, 500, LEATHER, HI_LEATHER),
CONTAINER("sack",            "bag", 0, 0, 0, 35,  15,   2, CLOTH, HI_CLOTH),
CONTAINER("oilskin sack",    "bag", 0, 0, 0,  5,  15, 100, CLOTH, HI_CLOTH),
CONTAINER("bag of holding",  "bag", 0, 1, 0, 20,  15, 100, CLOTH, HI_CLOTH),
CONTAINER("bag of rats",     "bag", 0, 1, 1, 15,  15, 100, CLOTH, HI_CLOTH), /* SpliceHack */
CONTAINER("bag of tricks",   "bag", 0, 1, 1, 20,  15, 100, CLOTH, HI_CLOTH),
#undef CONTAINER

/* lock opening tools */
TOOL("skeleton key",       "key", 0, 0, 0, 0,  80,   3,  10, BONE, CLR_WHITE),
TOOL("lock pick",           None, 1, 0, 0, 0,  60,   4,  20, METAL, HI_METAL),
TOOL("credit card",         None, 1, 0, 0, 0,  15,   1,  10, PLASTIC, CLR_WHITE),
/* light sources */
TOOL("tallow candle",   "candle", 0, 1, 0, 0,  20,   2,  10, WAX, CLR_WHITE),
TOOL("wax candle",      "candle", 0, 1, 0, 0,   5,   2,  20, WAX, CLR_WHITE),
TOOL("magic candle",    "candle", 0, 1, 1, 0,   5,   2, 500, WAX, CLR_WHITE),       /* Slash'EM */
TOOL("lantern",             None, 1, 0, 0, 0,  30,  30,  12, COPPER, CLR_YELLOW),
TOOL("oil lamp",          "lamp", 0, 0, 0, 0,  45,  20,  10, COPPER, CLR_YELLOW),
TOOL("magic lamp",        "lamp", 0, 0, 1, 0,  15,  20,1000, COPPER, CLR_YELLOW),
/* other tools */
TOOL("expensive camera",    None, 1, 0, 0, 1,  15,  12, 200, PLASTIC, CLR_BLACK),
TOOL("mirror",   "looking glass", 0, 0, 0, 0,  45,  13,  10, GLASS, HI_SILVER),
TOOL("keg",      "wooden barrel", 1, 0, 0, 1,   5, 100,  50, WOOD,  HI_WOOD),       /* SpliceHack */
TOOL("crystal ball", "glass orb", 0, 0, 1, 1,  15, 150,  60, GLASS, HI_GLASS),
TOOL("eight ball", "plastic orb", 0, 0, 0, 0,   0,  20,  30, PLASTIC, CLR_BLACK),   /* EvilHack */
TOOL("lenses",              None, 1, 0, 0, 0,   4,   3,  80, GLASS, HI_GLASS),
TOOL("goggles",             None, 1, 0, 0, 0,   1,   3,  50, PLASTIC, CLR_BLACK),   /* EvilHack */
TOOL("blindfold",           None, 1, 0, 0, 0,  50,   2,  20, CLOTH, CLR_BLACK),
TOOL("towel",               None, 1, 0, 0, 0,  50,  10,  50, CLOTH, CLR_MAGENTA),
TOOL("saddle",              None, 1, 0, 0, 0,   5, 200, 150, LEATHER, HI_LEATHER),
OBJECT(OBJ("barding", None),                                                        /* EvilHack */
        BITS(1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, IRON),
        0, TOOL_CLASS, 4, 0, 250, 300, 0, 0, 3, 0, 250, HI_METAL),
OBJECT(OBJ("spiked barding", None),                                                 /* EvilHack */
        BITS(1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, IRON),
        0, TOOL_CLASS, 2, 0, 275, 300, 0, 0, 3, 0, 275, HI_METAL),
OBJECT(OBJ("barding of reflection", "polished barding"),                            /* EvilHack */
        BITS(0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, P_NONE, SILVER),
        0, TOOL_CLASS, 1, 0, 275, 600, 0, 0, 3, 0, 275, HI_METAL),
TOOL("leash",               None, 1, 0, 0, 0,  65,  12,  20, LEATHER, HI_LEATHER),
TOOL("stethoscope",         None, 1, 0, 0, 0,  25,   4,  75, IRON, HI_METAL),
TOOL("tinning kit",         None, 1, 0, 0, 1,  20, 100,  30, IRON, HI_METAL),
TOOL("tin opener",          None, 1, 0, 0, 0,  35,   4,  30, IRON, HI_METAL),
TOOL("can of grease",       None, 1, 0, 0, 1,  10,  15,  20, IRON, HI_METAL),
TOOL("figurine",            None, 1, 0, 1, 0,  25,  50,  80, MINERAL, HI_MINERAL),
        /* monster type specified by obj->corpsenm */
TOOL("mask",                None, 1, 0, 1, 0,  10,  20, 150, WOOD, HI_WOOD),        /* SpliceHack */
        /* monster type specified by obj->corpsenm */
TOOL("magic marker",    "marker", 0, 0, 1, 1,   0,   2,  200, PLASTIC, CLR_RED),
/* traps */
TOOL("land mine",           None, 1, 0, 0, 0,   0, 300, 180, IRON, CLR_RED),
TOOL("beartrap",            None, 1, 0, 0, 0,   0, 200,  60, IRON, HI_METAL),
/* instruments */
TOOL("pea whistle",    "whistle", 0, 0, 0, 0, 100,   3,  10, METAL, HI_METAL),
TOOL("magic whistle",  "whistle", 0, 0, 1, 0,  30,   3,  100, METAL, HI_METAL),
TOOL("flute",            "flute", 0, 0, 0, 0,   4,   5,  12, WOOD, HI_WOOD),
TOOL("magic flute",      "flute", 0, 0, 1, 1,   2,   5,  144, WOOD, HI_WOOD),
TOOL("tooled horn",       "horn", 0, 0, 0, 0,   5,  18,  15, BONE, CLR_WHITE),
TOOL("frost horn",        "horn", 0, 0, 1, 1,   2,  18,  200, BONE, CLR_WHITE),
TOOL("fire horn",         "horn", 0, 0, 1, 1,   2,  18,  200, BONE, CLR_WHITE),
TOOL("horn of blasting",  "horn", 0, 0, 1, 1,   2,  18,  200, BONE, CLR_WHITE),      /* SpliceHack */
TOOL("horn of plenty",    "horn", 0, 0, 1, 1,   2,  18,  200, BONE, CLR_WHITE),
        /* horn, but not an instrument */
TOOL("harp",              "harp", 0, 0, 0, 0,   4,  30,  50, WOOD, HI_WOOD),
TOOL("magic harp",        "harp", 0, 0, 1, 1,   2,  30,  200, WOOD, HI_WOOD),
TOOL("bell",                None, 1, 0, 0, 0,   2,  30,  50, COPPER, HI_COPPER),
TOOL("bugle",               None, 1, 0, 0, 0,   4,  10,  15, COPPER, HI_COPPER),
TOOL("lute",                None, 1, 0, 0, 0,  4, 25, 40, WOOD, HI_WOOD),            /* SpliceHack */
TOOL("bagpipe",             None, 1, 0, 0, 0,  2, 35, 75, LEATHER, CLR_GREEN),      /* SpliceHack */
TOOL("leather drum",      "drum", 0, 0, 0, 0,   4,  25,  25, LEATHER, HI_LEATHER),
TOOL("drum of earthquake","drum", 0, 0, 1, 1,   2,  25,  100, LEATHER, HI_LEATHER),


#define WEPTOOL(name,desc,kn,mgc,chg, bi,prob,wt,cost,sdam,ldam,hitbon,typ,sub,mat,clr) \
    OBJECT(OBJ(name, desc),                                             \
           BITS(kn, 0, 1,chg, mgc, 1, 0, 0, bi, 0, hitbon, sub, mat),    \
           0, TOOL_CLASS, prob, 0, wt, cost, sdam, ldam, hitbon, 0, wt, clr)
/* tools useful as weapons */
WEPTOOL("pick-axe", None,              1, 0, 0, 0, 20,  75,  50,  6,  3, 0, WHACK,  P_PICK_AXE, IRON, HI_METAL),
WEPTOOL("spoon", None,                 1, 0, 0, 0,  0,   1,  1,   2,  2, 0, PIERCE, P_KNIFE, IRON, HI_METAL), /* Slash'EM */
WEPTOOL("fishing pole", None,          1, 0, 0, 0,  5,  30,  50,  2,  6, 0, WHACK,  P_POLEARMS, METAL, HI_METAL), /* Slash'EM */
WEPTOOL("grappling hook", "iron hook", 0, 0, 0, 0,  5,  30,  50,  2,  6, 0, WHACK,  P_FLAIL,    IRON, HI_METAL),
WEPTOOL("torch", None,                 1, 0, 0, 0,  0,  25,   8,  5,  2, 0, WHACK,  P_CLUB, WOOD, HI_WOOD), /* Slash'EM */
WEPTOOL("unicorn horn", None,          1, 1, 0, 1,  0,  20, 100,  12, 12, 1, PIERCE, P_UNICORN_HORN, BONE, CLR_WHITE),
        /* 3.4.1: unicorn horn left classified as "magic" */

/* Two pseudo tools. These can never exist outside of medical kits. */
OBJECT(OBJ("bandage", None),
		BITS(1,1,0,0,0,0,0,1,0,0,0,P_NONE,CLOTH), 0,
		TOOL_CLASS, 0, 0, 1, 1, 0, 0, 0, 0, 0, CLR_WHITE),
OBJECT(OBJ("phial", None),
		BITS(1,1,0,0,0,0,0,1,0,0,0,P_NONE,GLASS), 0,
		TOOL_CLASS, 0, 0, 1, 1, 0, 0, 0, 0, 1, HI_GLASS),

/* two unique tools;
 * not artifacts, despite the comment which used to be here
 */
OBJECT(OBJ("Candelabrum of Invocation", "candelabrum"),
       BITS(0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, P_NONE, GOLD),
       0, TOOL_CLASS, 0, 0, 10, 5000, 0, 0, 0, 0, 200, HI_GOLD),
OBJECT(OBJ("Bell of Opening", "engraved silver bell"),
       BITS(0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, P_NONE, SILVER),
       0, TOOL_CLASS, 0, 0, 10, 5000, 0, 0, 0, 0, 50, HI_SILVER),
#undef TOOL
#undef WEPTOOL

/* Comestibles ... */
#define FOOD(name, prob, delay, wt, unk, tin, nutrition, color)         \
    OBJECT(OBJ(name, None),                                       \
           BITS(1, 1, unk, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, tin), 0,     \
           FOOD_CLASS, prob, delay, wt, nutrition / 20 + 5, 0, 0, 0, 0, \
           nutrition, color)
/* All types of food (except tins & corpses) must have a delay of at least 1.
 * Delay on corpses is computed and is weight dependant.
 * Domestic pets prefer tripe rations above all others.
 * Fortune cookies can be read, using them up without ingesting them.
 * Carrots improve your vision.
 * +0 tins contain monster meat.
 * +1 tins (of spinach) make you stronger (like Popeye).
 * Meatballs/sticks/rings are only created from objects via stone to flesh.
 */
/* meat */
FOOD("tripe ration",        140,  2, 10, 0, FLESH, 200, CLR_BROWN),
FOOD("corpse",                0,  1,  0, 0, FLESH,   0, CLR_BROWN),
/* body parts (currently only for base object: the Eye of Vecna) */
FOOD("eyeball",               0,  1,  1, 0, FLESH,   5, CLR_WHITE),
FOOD("severed hand",          0,  1,  0, 0, FLESH,  40, CLR_BROWN),
FOOD("egg",                  85,  1,  1, 1, FLESH,  80, CLR_WHITE),
FOOD("meatball",              0,  1,  1, 0, FLESH,   5, CLR_BROWN),
FOOD("meat stick",            0,  1,  1, 0, FLESH,   5, CLR_BROWN),
FOOD("huge chunk of meat",    0, 20,400, 0, FLESH,2000, CLR_BROWN),
/* special case because it's not mergable */
OBJECT(OBJ("meat ring", None),
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, FLESH),
       0, FOOD_CLASS, 0, 1, 5, 1, 0, 0, 0, 0, 5, CLR_BROWN),
/* pudding 'corpses' will turn into these and combine;
 * must be in same order as the pudding monsters */
FOOD("glob of gray ooze",     0,  2, 20, 0, FLESH,  20, CLR_GRAY),
FOOD("glob of brown pudding", 0,  2, 20, 0, FLESH,  20, CLR_BROWN),
FOOD("glob of green slime",   0,  2, 20, 0, FLESH,  20, CLR_GREEN),
FOOD("glob of blood pudding", 0,  2, 20, 0, FLESH,  20, CLR_RED),
FOOD("glob of gel",           0,  2, 20, 0, FLESH,  20, CLR_BRIGHT_CYAN),
FOOD("glob of moldy pudding", 0,  2, 20, 0, FLESH,  20, CLR_BRIGHT_GREEN),
FOOD("glob of black pudding", 0,  2, 20, 0, FLESH,  20, CLR_BLACK),

/* fruits & veggies */
FOOD("kelp frond",            0,  1,  1, 0, VEGGY,  30, CLR_GREEN),
FOOD("eucalyptus leaf",      10,  1,  1, 0, VEGGY,  1, CLR_GREEN),
FOOD("apple",                10,  1,  2, 0, VEGGY,  50, CLR_RED),
FOOD("orange",               10,  1,  2, 0, VEGGY,  80, CLR_ORANGE),
FOOD("mushroom",              1,  1,  5, 0, VEGGY,  90, CLR_BLACK),          /* Slash'EM */
FOOD("pear",                 10,  1,  2, 0, VEGGY,  50, CLR_BRIGHT_GREEN),
FOOD("asian pear",           1,   1,  2, 0, VEGGY,  75, CLR_BRIGHT_GREEN),   /* Slash'EM */
FOOD("melon",                10,  1,  5, 0, VEGGY, 100, CLR_BRIGHT_GREEN),
FOOD("pineapple",            10,  2,  5, 0, VEGGY,  85, CLR_YELLOW),         /* SpliceHack */
FOOD("banana",               10,  1,  2, 0, VEGGY,  80, CLR_YELLOW),
FOOD("carrot",               12,  1,  2, 0, VEGGY,  50, CLR_ORANGE),
FOOD("sprig of catnip",       3,  1,  1, 0, VEGGY,  40, CLR_GREEN),          /* SpliceHack */
FOOD("sprig of wolfsbane",    7,  1,  1, 0, VEGGY,  40, CLR_GREEN),
FOOD("clove of garlic",       7,  1,  1, 0, VEGGY,  40, CLR_WHITE),
/* name of slime mold is changed based on player's OPTION=fruit:something
 * and bones data might have differently named ones from prior games */
FOOD("slime mold",           75,  1,  5, 0, VEGGY, 250, HI_ORGANIC),

/* people food */
FOOD("lump of royal jelly",   0,  1,  2, 0, VEGGY, 200, CLR_MAGENTA),
FOOD("cream pie",            25,  1, 10, 0, VEGGY, 100, CLR_WHITE),
FOOD("apple pie",             1,  1, 10, 0, VEGGY, 200, CLR_RED),            /* SpliceHack */
FOOD("pumpkin pie",           1,  1, 10, 0, VEGGY, 300, CLR_ORANGE),         /* SpliceHack */
FOOD("sandwich",             10,  1, 10, 0, FLESH, 100, CLR_WHITE),          /* Slash'EM */
FOOD("candy bar",            13,  1,  2, 0, VEGGY, 100, CLR_BRIGHT_BLUE),
FOOD("slice of cake",         6, 10,  2, 0, VEGGY, 900, CLR_RED),            /* SpliceHack */
FOOD("fruitcake",             1, 20, 20, 0, VEGGY, 1000, CLR_BROWN),         /* SpliceHack */
FOOD("fortune cookie",       55,  1,  1, 0, VEGGY,  40, CLR_YELLOW),
FOOD("pancake",              25,  2,  2, 0, VEGGY, 200, CLR_YELLOW),         /* Slash'EM */
FOOD("tortilla",             1,   2,  2, 0, VEGGY,  80, CLR_WHITE),          /* Slash'EM */
FOOD("cheese",               10,  2,  2, 0, FLESH, 250, CLR_YELLOW),         /* Slash'EM */
FOOD("pill",                  1,  1,  1, 0, VEGGY,   1, CLR_BRIGHT_MAGENTA), /* Slash'EM */
FOOD("holy wafer",            7,  1,  1, 0, VEGGY, 150, CLR_WHITE),          
FOOD("lembas wafer",         20,  2,  5, 0, VEGGY, 800, CLR_WHITE),
FOOD("cram ration",          20,  3, 15, 0, VEGGY, 600, HI_ORGANIC),
FOOD("food ration",         380,  5, 20, 0, VEGGY, 800, HI_ORGANIC),
FOOD("K-ration",              0,  1, 10, 0, VEGGY, 400, HI_ORGANIC),
FOOD("C-ration",              0,  1, 10, 0, VEGGY, 300, HI_ORGANIC),
/* tins have type specified by obj->spe (+1 for spinach, other implies
 * flesh; negative specifies preparation method {homemade,boiled,&c})
 * and by obj->corpsenm (type of monster flesh) */
FOOD("tin",                  75,  0, 10, 1, METAL,   0, HI_METAL),
#undef FOOD

/* potions ... */
#define POTION(name,desc,mgc,power,prob,cost,color) \
    OBJECT(OBJ(name, desc),                                             \
           BITS(0, 1, 0, 0, mgc, 0, 0, 0, 0, 0, 0, P_NONE, GLASS),      \
           power, POTION_CLASS, prob, 0, 20, cost, 0, 0, 0, 0, 10, color)
POTION("gain ability",           "ruby",  1, 0, 42, 300, CLR_RED),
POTION("reflection",         "metallic",  1, REFLECTING, 10, 300, CLR_GRAY), /* SpliceHack */
POTION("regeneration",          "purple-red",  1, REGENERATION, 20, 150, CLR_MAGENTA), /* SpliceHack */
POTION("restore ability",        "pink",  1, 0, 40, 100, CLR_BRIGHT_MAGENTA),
POTION("confusion",            "orange",  1, CONFUSION, 42, 100, CLR_ORANGE),
POTION("clairvoyance",    "luminescent",  1, 0, 20, 100, CLR_WHITE),         /* Slash'EM */
POTION("blindness",            "yellow",  1, BLINDED, 40, 150, CLR_YELLOW),
POTION("ESP",                   "muddy",  1, TELEPAT, 20, 150, CLR_BROWN),   /* Slash'EM */
POTION("paralysis",           "emerald",  1, 0, 42, 300, CLR_BRIGHT_GREEN),
POTION("speed",            "dark green",  1, FAST, 42, 200, CLR_GREEN),
POTION("levitation",             "cyan",  1, LEVITATION, 42, 200, CLR_CYAN),
POTION("hallucination",      "sky blue",  1, HALLUC, 40, 100, CLR_CYAN),
POTION("invisibility", "brilliant blue",  1, INVIS, 40, 150, CLR_BRIGHT_BLUE),
POTION("see invisible",       "magenta",  1, SEE_INVIS, 42, 50, CLR_MAGENTA),
POTION("healing",               "amber",  1, 0, 57, 100, CLR_BROWN),
POTION("extra healing",          "puce",  1, 0, 47, 100, CLR_RED),
POTION("gain level",            "milky",  1, 0, 20, 300, CLR_WHITE),
POTION("invulnerability",         "icy",  1, 0,  5, 300, CLR_BRIGHT_BLUE),   /* Slash'EM */
POTION("enlightenment",        "swirly",  1, 0, 20, 200, CLR_ORANGE),
POTION("monster detection",    "bubbly",  1, 0, 40, 150, CLR_WHITE),
POTION("object detection",      "smoky",  1, 0, 42, 150, CLR_GRAY),
POTION("gain energy",          "cloudy",  1, 0, 42, 150, CLR_WHITE),
POTION("sleeping",       "effervescent",  1, 0, 42, 100, CLR_GRAY),
POTION("full healing",          "black",  1, 0, 10, 200, CLR_BLACK),
POTION("polymorph",            "golden",  1, 0, 10, 200, CLR_YELLOW),
POTION("booze",                 "brown",  0, 0, 42,  50, CLR_BROWN),
POTION("sickness",              "fizzy",  0, 0, 42,  50, CLR_CYAN),
POTION("fruit juice",            "dark",  0, 0, 42,  50, CLR_BLACK),
POTION("acid",                  "white",  0, 0, 10, 250, CLR_WHITE),
POTION("oil",                   "murky",  0, 0, 30, 250, CLR_BROWN),
/* fixed description */
POTION("water",                 "clear",  0, 0, 92, 100, CLR_CYAN),
POTION("amnesia",          "sparkling",   1, 0, 16, 100, CLR_CYAN),          /* Slash'EM */
POTION("blood",             "blood-red",  0, 0,  0,  50, CLR_RED),           /* Slash'EM */
POTION("vampire blood",     "blood-red",  1, 0,  0, 300, CLR_RED),           /* Slash'EM */
#undef POTION

/* scrolls ... */
#define SCROLL(name,text,mgc,prob,cost) \
    OBJECT(OBJ(name, text),                                           \
           BITS(0, 1, 0, 0, mgc, 0, 0, 0, 0, 0, 0, P_NONE, PAPER),    \
           0, SCROLL_CLASS, prob, 0, 5, cost, 0, 0, 0, 0, 6, HI_PAPER)
SCROLL("enchant armor",              "ZELGO MER",  1,  65,  80),
SCROLL("destroy armor",         "JUYED AWK YACC",  1,  45, 100),
SCROLL("confuse monster",                 "NR 9",  1,  53, 100),
SCROLL("scare monster",   "XIXAXA XOXAXA XUXAXA",  1,  35, 100),
SCROLL("remove curse",             "PRATYAVAYAH",  1,  65,  80),
SCROLL("enchant weapon",         "DAIYEN FOOELS",  1,  80,  60),
SCROLL("create monster",       "LEP GEX VEN ZEA",  1,  45, 200),
SCROLL("taming",                   "PRIRUTSENIE",  1,  15, 200),
SCROLL("genocide",                  "ELBIB YLOH",  1,  15, 300),
SCROLL("elementalism",          "4OFAE OF9 SCC9",  1,  10, 300), /* SpliceHack/Lethe Patch */
                                   /* Voynich */
SCROLL("light",                 "VERR YED HORRE",  1,  90,  50),
SCROLL("time",                   "TEMP USF UGIT",  1,  15, 300), /* SpliceHack */
                              /* tempus fugit */
SCROLL("teleportation",        "VENZAR BORGAVVE",  1,  55, 100),
SCROLL("gold detection",                 "THARR",  1,  34, 100),
SCROLL("food detection",               "YUM YUM",  1,  25, 100),
SCROLL("knowledge",                   "POTESTAS",  1,  20,  50), /* SpliceHack */
                                     /* power */
SCROLL("identify",                  "KERNOD WEL",  1, 180,  20),
SCROLL("air",                        "TRATS ARL",  1,  35, 200), /* SpliceHack */
                                 /* LRA START */
SCROLL("transmogrify",              "TLON UQBAR",  1,  20, 200), /* SpliceHack */
                                /* Tlon Uqbar */
SCROLL("magic mapping",              "ELAM EBOW",  1,  45, 100),
SCROLL("amnesia",                   "DUAM XNAHT",  1,  35, 200),
SCROLL("flood",                     "AQUE BRAGH",  1,  35, 200),
SCROLL("fire",                  "ANDOVA BEGARIN",  1,  30, 100),
SCROLL("earth",                          "KIRJE",  1,  18, 200),
SCROLL("punishment",            "VE FORBRYDERNE",  1,  15, 300),
SCROLL("charging",                "HACKEM MUCHE",  1,  15, 300),
SCROLL("cloning",               "TOYL ENT RUBLE",  1,   5, 300),  /* SpliceHack */
            /* Fire burn and cauldron bubble. */
SCROLL("stinking cloud",             "VELOX NEB",  1,  15, 300),
SCROLL("acquirement",       "HZLRC KSTSBD MPFNG",  1,  10, 300),
SCROLL("ice",		           "OOBID IBBOB",  1,  10, 200), /* SlashTHEM */
SCROLL("magic detection",        "FOOBIE BLETCH",  1,  25, 300), /* EvilHack */
    /* Extra descriptions, shuffled into use at start of new game.
     * Code in win/share/tilemap.c depends on SCR_MAGIC_DETECTION preceding
     * these and on how many of them there are.  If a real scroll gets added
     * after stinking cloud or the number of extra descriptions changes,
     * tilemap.c must be modified to match.
     */
SCROLL(None,              "TEMOV",  1,   0, 100),
SCROLL(None,         "GARVEN DEH",  1,   0, 100),
SCROLL(None,            "READ ME",  1,   0, 100),
SCROLL(None,      "ETAOIN SHRDLU",  1,   0, 100),
SCROLL(None,        "LOREM IPSUM",  1,   0, 100),
SCROLL(None,              "FNORD",  1,   0, 100), /* Illuminati */
SCROLL(None,            "KO BATE",  1,   0, 100), /* Kurd Lasswitz */
SCROLL(None,      "ABRA KA DABRA",  1,   0, 100), /* traditional incantation */
SCROLL(None,       "ASHPD SODALG",  1,   0, 100), /* Portal */
SCROLL(None,            "ZLORFIK",  1,   0, 100), /* Zak McKracken */
SCROLL(None,      "GNIK SISI VLE",  1,   0, 100), /* Zak McKracken */
SCROLL(None,    "HAPAX LEGOMENON",  1,   0, 100),
SCROLL(None,  "EIRIS SAZUN IDISI",  1,   0, 100), /* Merseburg Incantations */
SCROLL(None,    "PHOL ENDE WODAN",  1,   0, 100), /* Merseburg Incantations */
SCROLL(None,              "GHOTI",  1,   0, 100), /* pronounced as 'fish',
                                                        George Bernard Shaw */
SCROLL(None, "MAPIRO MAHAMA DIROMAT", 1, 0, 100), /* Wizardry */
SCROLL(None,  "VAS CORP BET MANI",  1,   0, 100), /* Ultima */
SCROLL(None,            "XOR OTA",  1,   0, 100), /* Aarne Haapakoski */
SCROLL(None, "STRC PRST SKRZ KRK",  1,   0, 100), /* Czech and Slovak
                                                        tongue-twister */
SCROLL(None,            "NYEHEHE",  1,   0, 100), /* laughing noise */
SCROLL(None,      "ELPHE MONATER",  1,   0, 100), /* famous mistranslation */
SCROLL(None,               "ABAJ",  1,   0, 100), /* famous mistranslation */
SCROLL(None,  "CONG LAT URAT ION",  1,   0, 100), /* famous mistranslation */
SCROLL(None,       "DESUM TETRAL",  1,   0, 100), /* dal aos */
SCROLL(None,            "NHINRL",   1,   0, 100), /* meta */
SCROLL(None,          "ISAY INAY",  1,   0, 100), /* meta */
SCROLL(None,        "PEBCAK RTFM",  1,   0, 100), /* Slang */
SCROLL(None,        "KA TET",  1,   0, 100), /* Dark Tower */
    /* These must come last because they have special fixed descriptions.
     */
#ifdef MAIL
SCROLL("mail",          "stamped",  0,   0,   0),
#endif
SCROLL("blank paper", "unlabeled",  0,  28,  60),
#undef SCROLL

/* spellbooks ... */
    /* Expanding beyond 52 spells would require changes in spellcasting
     * or imposition of a limit on number of spells hero can know because
     * they are currently assigned successive letters, a-zA-Z, when learned.
     * [The existing spell sorting capability could conceivably be extended
     * to enable moving spells from beyond Z to within it, bumping others
     * out in the process, allowing more than 52 spells be known but keeping
     * only 52 be castable at any given time.]
     */
#define SPELL(name,desc,sub,prob,delay,level,mgc,dir,color)  \
    OBJECT(OBJ(name, desc),                                             \
           BITS(0, 0, 0, 0, mgc, 0, 0, 0, 0, 0, dir, sub, PAPER),       \
           0, SPBOOK_CLASS, prob, delay, level * 5 + 30,                \
           level * 100, 0, 0, 0, level, 20, color)
/* Spellbook description normally refers to book covers (primarily color).
 * Parchment and vellum would never be used for such, but rather than
 * eliminate those, finagle their definitions to refer to the pages
 * rather than the cover.  They are made from animal skin (typically of
 * a goat or sheep) and books using them for pages generally need heavy
 * covers with straps or clamps to tightly close the book in order to
 * keep the pages flat.  (However, a wooden cover might itself be covered
 * by a sheet of parchment, making this become less of an exception.  Also,
 * changing the internal composition from paper to leather makes eating a
 * parchment or vellum spellbook break vegetarian conduct, as it should.) */
#define PAPER LEATHER /* override enum for use in SPELL() expansion */
SPELL("dig",             "parchment",     P_MATTER_SPELL,      20,  6, 3, 1, RAY, HI_LEATHER),
SPELL("magic missile",   "vellum",        P_ATTACK_SPELL,      35,  2, 2, 1, RAY, HI_LEATHER),
#undef PAPER /* revert to normal material */
SPELL("fireball",        "ragged",        P_MATTER_SPELL,      20,  4, 4, 1, RAY, HI_PAPER),
SPELL("cone of cold",    "dog eared",     P_MATTER_SPELL,      10,  7, 4, 1, RAY, HI_PAPER),
SPELL("sleep",           "mottled",       P_ENCHANTMENT_SPELL, 30,  1, 1, 1, RAY, HI_PAPER),
SPELL("finger of death", "stained",       P_ATTACK_SPELL,       5, 10, 7, 1, RAY, HI_PAPER),
SPELL("lightning",       "electric blue", P_ATTACK_SPELL,      10,  8, 4, 1, RAY, CLR_BRIGHT_BLUE),  /* Slash'EM */
SPELL("poison blast",    "olive green",   P_ATTACK_SPELL,      10,  2, 4, 1, RAY, CLR_GREEN),        /* Slash'EM */
SPELL("acid blast",      "acid green",    P_ATTACK_SPELL,       5,  9, 6, 1, RAY, CLR_BRIGHT_GREEN), /* Slash'EM */
SPELL("sonicboom",       "ghostly",       P_MATTER_SPELL,       5,  7, 4, 1, RAY, CLR_BLACK),        /* SpliceHack */
/* Water spell here? */
SPELL("psionic wave",    "worn",          P_ATTACK_SPELL,       0,  1, 1, 1, IMMEDIATE, CLR_MAGENTA), /* EvilHack */
    /* Don't disturb the order of the spells above. zap.c depends on it. */

SPELL("force bolt",      "red",           P_ATTACK_SPELL,      35,  2, 1, 1, IMMEDIATE, CLR_RED),
SPELL("drain life",      "velvet",        P_NECROMANCY_SPELL,  10,  2, 2, 1, IMMEDIATE, CLR_MAGENTA),
SPELL("summon undead",   "black",         P_NECROMANCY_SPELL,  10,  7, 5, 1, IMMEDIATE, CLR_BLACK),  /* Slash'EM */
SPELL("command undead",  "dark",          P_NECROMANCY_SPELL,  10,  7, 3, 1, IMMEDIATE, CLR_BRIGHT_GREEN),  /* Slash'EM */
SPELL("knock",           "pink",          P_MATTER_SPELL,      35,  1, 1, 1, IMMEDIATE, CLR_BRIGHT_MAGENTA),
SPELL("wizard lock",     "dark green",    P_MATTER_SPELL,      30,  3, 2, 1, IMMEDIATE, CLR_GREEN),
SPELL("polymorph",       "silver",        P_MATTER_SPELL,      10,  8, 6, 1, IMMEDIATE, HI_SILVER),
SPELL("repair armor",    "platinum",      P_MATTER_SPELL,      20,  6, 3, 1, IMMEDIATE, HI_PAPER), /* EvilHack */
SPELL("reflection",	     "decrepit",      P_MATTER_SPELL,      15,  3, 5, 1, IMMEDIATE, CLR_BROWN),  /* EvilHack */
SPELL("flame sphere",    "canvas",        P_MATTER_SPELL,      15,  2, 1, 1, NODIR, CLR_BROWN),   /* Slash'EM */
SPELL("freeze sphere",   "hardcover",     P_MATTER_SPELL,      15,  2, 1, 1, NODIR, CLR_BROWN),   /* Slash'EM */
SPELL("cancellation",    "shining",       P_MATTER_SPELL,      15,  8, 7, 1, IMMEDIATE, CLR_WHITE),
/* Fire Bolt is the Flame Mage's special spell - not randomly generated. */
SPELL("fire bolt",       "feathered",     P_MATTER_SPELL,       0,  2, 1, 1, IMMEDIATE, CLR_YELLOW),
SPELL("slow monster",    "light green",   P_ENCHANTMENT_SPELL, 30,  2, 1, 1, IMMEDIATE, CLR_BRIGHT_GREEN),
SPELL("cause fear",      "light blue",    P_ENCHANTMENT_SPELL,  25,  3, 3, 1, NODIR, CLR_BRIGHT_BLUE),
SPELL("charm monster",   "magenta",       P_ENCHANTMENT_SPELL, 15,  3, 4, 1, IMMEDIATE, CLR_MAGENTA),
SPELL("confuse monster", "orange",        P_ENCHANTMENT_SPELL, 30,  2, 1, 1, IMMEDIATE, CLR_ORANGE),

SPELL("haste self",      "purple",        P_ESCAPE_SPELL,      30,  4, 3, 1, NODIR, CLR_MAGENTA),
SPELL("levitation",      "tan",           P_ESCAPE_SPELL,      20,  4, 4, 1, NODIR, CLR_BROWN),
SPELL("teleport away",   "gold",          P_ESCAPE_SPELL,      15,  6, 5, 1, IMMEDIATE, HI_GOLD),
SPELL("passwall",        "ochre",         P_ESCAPE_SPELL,       5,  7, 5, 1, NODIR, CLR_YELLOW),   /* Slash'EM */
SPELL("invisibility",    "dark brown",    P_ESCAPE_SPELL,      25,  5, 2, 1, NODIR, CLR_BROWN),
SPELL("jumping",         "thin",          P_ESCAPE_SPELL,      20,  3, 2, 1, IMMEDIATE, HI_PAPER),

SPELL("light",           "cloth",         P_DIVINATION_SPELL,  35,  1, 1, 1, NODIR, HI_CLOTH),
SPELL("detect monsters", "leathery",      P_DIVINATION_SPELL,  30,  1, 2, 1, NODIR, HI_LEATHER),
SPELL("detect food",     "cyan",          P_DIVINATION_SPELL,  30,  3, 1, 1, NODIR, CLR_CYAN),
SPELL("detect unseen",   "violet",        P_DIVINATION_SPELL,  20,  4, 2, 1, NODIR, CLR_MAGENTA),
SPELL("detect treasure", "gray",          P_DIVINATION_SPELL,  20,  5, 4, 1, NODIR, CLR_GRAY),
SPELL("clairvoyance",    "dark blue",     P_DIVINATION_SPELL,  15,  3, 3, 1, NODIR, CLR_BLUE),
SPELL("enlighten",       "faded",         P_DIVINATION_SPELL,  15,  5, 4, 1, NODIR, CLR_GRAY),
SPELL("identify",        "bronze",        P_DIVINATION_SPELL,  20,  6, 3, 1, NODIR, HI_COPPER),
SPELL("magic mapping",   "dusty",         P_DIVINATION_SPELL,  15,  7, 5, 1, NODIR, HI_PAPER),

SPELL("healing",         "white",         P_HEALING_SPELL,     35,  2, 1, 1, IMMEDIATE, CLR_WHITE),
SPELL("cure blindness",  "yellow",        P_HEALING_SPELL,     20,  2, 2, 1, IMMEDIATE, CLR_YELLOW),
SPELL("extra healing",   "plaid",         P_HEALING_SPELL,     25,  5, 3, 1, IMMEDIATE, CLR_GREEN),
SPELL("restore ability", "light brown",   P_HEALING_SPELL,     25,  5, 2, 1, IMMEDIATE, CLR_BROWN),
SPELL("stone to flesh",  "thick",         P_HEALING_SPELL,     15,  1, 3, 1, IMMEDIATE, HI_PAPER),
SPELL("cure sickness",   "indigo",        P_HEALING_SPELL,     30,  3, 3, 1, IMMEDIATE, CLR_BLUE),

SPELL("create monster",  "turquoise",     P_ENCHANTMENT_SPELL, 30,  3, 2, 1, NODIR, CLR_BRIGHT_CYAN),
SPELL("remove curse",    "wrinkled",      P_ENCHANTMENT_SPELL, 25,  5, 3, 1, NODIR, HI_PAPER),
SPELL("turn undead",     "copper",        P_NECROMANCY_SPELL,  15,  8, 6, 1, IMMEDIATE, HI_COPPER),
SPELL("create familiar", "glittering",    P_ENCHANTMENT_SPELL, 10,  7, 6, 1, NODIR, CLR_WHITE),
SPELL("protection",      "dull",          P_ENCHANTMENT_SPELL, 15,  3, 1, 1, NODIR, HI_PAPER),

SPELL(None,         "deep",        P_NONE, 0, 0, 0, 1, 0, HI_SILVER),
SPELL(None,         "big",         P_NONE, 0, 0, 0, 1, 0, CLR_BLACK),
SPELL(None,         "fuzzy",       P_NONE, 0, 0, 0, 1, 0, CLR_YELLOW),
SPELL(None,         "spotted",     P_NONE, 0, 0, 0, 1, 0, CLR_WHITE),
SPELL(None,         "colorful",    P_NONE, 0, 0, 0, 1, 0, HI_PAPER),
SPELL(None,         "long",        P_NONE, 0, 0, 0, 1, 0, HI_PAPER),
SPELL(None,         "rainbow",     P_NONE, 0, 0, 0, 1, 0, HI_PAPER),
SPELL(None,         "tattered",    P_NONE, 0, 0, 0, 1, 0, HI_PAPER),
SPELL(None,         "wide",        P_NONE, 0, 0, 0, 1, 0, HI_PAPER),
SPELL(None,         "left-handed", P_NONE, 0, 0, 0, 1, 0, HI_PAPER),
SPELL(None,         "psychedelic", P_NONE, 0, 0, 0, 1, 0, CLR_BRIGHT_MAGENTA),
SPELL(None,         "spiral-bound",P_NONE, 0, 0, 0, 1, 0, HI_PAPER),
SPELL(None,         "stapled",     P_NONE, 0, 0, 0, 1, 0, HI_PAPER),
SPELL(None,         "stylish",     P_NONE, 0, 0, 0, 1, 0, HI_PAPER),
SPELL(None,         "tartan",      P_NONE, 0, 0, 0, 1, 0, CLR_RED),
SPELL(None,         "chartreuse",  P_NONE, 0, 0, 0, 1, 0, CLR_GREEN),
SPELL(None,         "crimson",     P_NONE, 0, 0, 0, 1, 0, CLR_RED),
SPELL(None,         "charcoal",    P_NONE, 0, 0, 0, 1, 0, CLR_BLACK),
/* books with fixed descriptions */
SPELL("blank paper", "plain", P_NONE, 15, 0, 0, 0, 0, HI_PAPER),
/* tribute book for 3.6 */
OBJECT(OBJ("novel", "paperback"),
       BITS(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, PAPER),
       0, SPBOOK_CLASS, 0, 0, 0, 20, 0, 0, 0, 1, 20, CLR_BRIGHT_BLUE),
/* a special, one of a kind, spellbook */
OBJECT(OBJ("Book of the Dead", "papyrus"),
       BITS(0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, P_NONE, PAPER),
       0, SPBOOK_CLASS, 0, 0, 20, 10000, 0, 0, 0, 7, 20, HI_PAPER),
#undef SPELL

/* wands ... */
/* sum of all probabilty values equals 3000 - wands
 * of wishing are 3 times less likely to randomly spawn
 * versus all other kinds */
#define WAND(name,typ,prob,cost,mgc,dir,metal,color) \
    OBJECT(OBJ(name, typ),                                              \
           BITS(0, 0, 1, 0, mgc, 1, 0, 0, 0, 0, dir, P_NONE, metal),    \
           0, WAND_CLASS, prob, 0, 7, cost, 0, 0, 0, 0, 30, color)
WAND("light",           "glass", 285, 100, 1, NODIR, GLASS, HI_GLASS),
WAND("secret door detection",
                        "balsa", 150, 150, 1, NODIR, WOOD, HI_WOOD),
WAND("enlightenment", "crystal",  58, 150, 1, NODIR, GLASS, HI_GLASS),
WAND("healing",        "bamboo",  60, 150, 1, IMMEDIATE, WOOD, CLR_YELLOW),         /* Slash'EM */
WAND("create monster",  "maple", 135, 200, 1, NODIR, WOOD, HI_WOOD),
WAND("fear",            "rusty",  25, 200, 1, NODIR, IRON, CLR_RED),                /* Slash'EM */
WAND("wishing",          "pine",   2, 500, 1, NODIR, WOOD, HI_WOOD),
WAND("nothing",           "oak",  75, 500, 0, IMMEDIATE, WOOD, HI_WOOD),
WAND("wonder",        "plastic",  5, 100, 0, IMMEDIATE, PLASTIC, CLR_RED),          /* SpliceHack */
WAND("wind",            "green", 15, 300, 1, IMMEDIATE, PLASTIC,                    /* SpliceHack */
                                                            CLR_BRIGHT_GREEN),
WAND("striking",        "ebony", 225, 150, 1, IMMEDIATE, WOOD, HI_WOOD),

WAND("make invisible", "marble", 135, 150, 1, IMMEDIATE, MINERAL, HI_MINERAL),
WAND("slow monster",      "tin", 150, 150, 1, IMMEDIATE, METAL, HI_METAL),
WAND("speed monster",   "brass", 150, 150, 1, IMMEDIATE, COPPER, HI_COPPER),
WAND("undead turning", "copper", 150, 150, 1, IMMEDIATE, COPPER, HI_COPPER),
WAND("draining",       "ceramic",  15, 175, 1, IMMEDIATE, GLASS,  HI_MINERAL),    /* Slash'EM */
WAND("polymorph",      "silver", 135, 200, 1, IMMEDIATE, SILVER, HI_SILVER),
WAND("cancellation", "platinum", 135, 200, 1, IMMEDIATE, PLATINUM, CLR_WHITE),
WAND("teleportation", "iridium", 135, 200, 1, IMMEDIATE, METAL, CLR_BRIGHT_CYAN),
WAND("create horde",   "black",    5, 300, 1, NODIR,     IRON,  CLR_BLACK),      /* Slash'EM */
WAND("extra healing",  "bronze",  30, 300, 1, IMMEDIATE, COPPER,   CLR_YELLOW),     /* Slash'EM */
WAND("opening",          "zinc",  75, 150, 1, IMMEDIATE, METAL, HI_METAL),
WAND("locking",      "aluminum",  75, 150, 1, IMMEDIATE, METAL, HI_METAL),
WAND("probing",       "uranium",  90, 150, 1, IMMEDIATE, METAL, HI_METAL),
/* Ray wands have to come last, and in this order. */
/* This is extremely kludgy, but that's what zap.c expects. */
WAND("digging",          "iron", 165, 150, 1, RAY, IRON,  HI_METAL),
WAND("magic missile",   "steel", 150, 150, 1, RAY, METAL, HI_METAL),
WAND("fire",        "hexagonal", 120, 175, 1, RAY, GLASS,  HI_METAL),
WAND("cold",            "short", 120, 175, 1, RAY, IRON,  HI_METAL),
WAND("sleep",           "runed", 150, 175, 1, RAY, IRON,  HI_METAL),
WAND("death",            "long",  15, 500, 1, RAY, IRON,  HI_METAL),
WAND("lightning",      "curved", 120, 175, 1, RAY, IRON,  HI_METAL),
WAND("poison gas",  "octagonal",  20, 175, 1, RAY, GLASS,  HI_METAL),
WAND("corrosion",     "twisted",  20, 175, 1, RAY, WOOD,  HI_WOOD), /* SpliceHack */
WAND("noise",        "titanium",  20, 175, 1, RAY, METAL,  HI_METAL),
WAND("deluge",      "driftwood",  10, 175, 1, RAY, WOOD,  HI_WOOD), /* SpliceHack */
/* extra descriptions, shuffled into use at start of new game */
WAND(None,             "spiked",  0, 150, 1, 0, IRON, HI_METAL),
WAND(None,            "jeweled",  0, 150, 1, 0, IRON, HI_MINERAL),
WAND(None,        "forked",    0, 150, 1, 0,         WOOD,     HI_WOOD),
WAND(None,        "quartz",    0, 150, 1, 0,         GLASS,    HI_GLASS),
WAND(None,        "walnut",    0, 150, 1, 0,         WOOD,     HI_WOOD),
WAND(None,        "mahogany",  0, 150, 1, 0,         WOOD,     HI_WOOD),
WAND(None,        "cedar",     0, 150, 1, 0,         WOOD,     HI_WOOD),
WAND(None,        "chrome",    0, 150, 1, 0,         METAL,    HI_SILVER),
WAND(None,        "nickel",    0, 150, 1, 0,         METAL,    HI_METAL),
WAND(None,        "mithril",   0, 150, 1, 0,         MITHRIL,  HI_SILVER),
WAND(None,        "grooved",   0, 150, 1, 0,         WOOD,     HI_WOOD),
WAND(None,        "bent",      0, 150, 1, 0,         IRON,     HI_METAL),
WAND(None,        "bone",      0, 150, 1, 0,         BONE,     CLR_WHITE),
WAND(None,        "alabaster", 0, 150, 1, 0,         MINERAL,  CLR_BROWN),
WAND(None,        "orichalcum",0, 150, 1, 0,         METAL,    HI_METAL),
WAND(None,        "electrum",  0, 150, 1, 0,         METAL,    HI_GOLD),
WAND(None,          "holly",   0, 150, 1, 0, WOOD, HI_WOOD),
WAND(None,          "pliable", 0, 150, 1, 0, PLASTIC, HI_WOOD),
WAND(None,     "dragon-bone",  0, 150, 1, 0, BONE, CLR_WHITE),
WAND(None,          "golden",  0, 150, 1, 0, GOLD, HI_GOLD),
WAND(None,          "gilded",  0, 150, 1, 0, GOLD, HI_GOLD),

WAND(None,            "thin",  0, 150, 1, 0, IRON, HI_METAL),
WAND(None,        "porcelain", 0, 150, 1, 0,         GLASS,    CLR_WHITE),
#undef WAND

/* coins ... - so far, gold is all there is */
#define COIN(name,prob,metal,worth) \
    OBJECT(OBJ(name, None),                                        \
           BITS(0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, metal),    \
           0, COIN_CLASS, prob, 0, 1, worth, 0, 0, 0, 0, 0, HI_GOLD)
COIN("gold piece", 1000, GOLD, 1),
#undef COIN

/* gems ... - includes stones and rocks but not boulders */
#define GEM(name,desc,prob,wt,gval,nutr,mohs,glass,color) \
    OBJECT(OBJ(name, desc),                                             \
           BITS(0, 1, 0, 0, 0, 0, 0, 0, 0,                              \
                HARDGEM(mohs), 0, -P_SLING, glass),                     \
           0, GEM_CLASS, prob, 0, 1, gval, 3, 3, 0, 0, nutr, color)
#define ROCK(name,desc,kn,prob,wt,gval,sdam,ldam,mgc,nutr,mohs,glass,color) \
    OBJECT(OBJ(name, desc),                                             \
           BITS(kn, 1, 0, 0, mgc, 0, 0, 0, 0,                           \
                HARDGEM(mohs), 0, -P_SLING, glass),                     \
           0, GEM_CLASS, prob, 0, wt, gval, sdam, ldam, 0, 0, nutr, color)
GEM("dilithium crystal", "white",  2, 1, 4500, 15,  5, GEMSTONE, CLR_WHITE),
GEM("diamond",           "white",  3, 1, 4000, 15, 10, GEMSTONE, CLR_WHITE),
GEM("ruby",                "red",  4, 1, 3500, 15,  9, GEMSTONE, CLR_RED),
GEM("jacinth",          "orange",  3, 1, 3250, 15,  9, GEMSTONE, CLR_ORANGE),
GEM("sapphire",           "blue",  4, 1, 3000, 15,  9, GEMSTONE, CLR_BLUE),
GEM("black opal",        "black",  3, 1, 2500, 15,  8, GEMSTONE, CLR_BLACK),
GEM("emerald",           "green",  5, 1, 2500, 15,  8, GEMSTONE, CLR_GREEN),
GEM("turquoise",         "green",  6, 1, 2000, 15,  6, GEMSTONE, CLR_GREEN),
GEM("citrine",          "yellow",  4, 1, 1500, 15,  6, GEMSTONE, CLR_YELLOW),
GEM("aquamarine",        "green",  6, 1, 1500, 15,  8, GEMSTONE, CLR_GREEN),
GEM("amber",   "yellowish brown",  8, 1, 1000, 15,  2, GEMSTONE, CLR_BROWN),
GEM("topaz",   "yellowish brown", 10, 1,  900, 15,  8, GEMSTONE, CLR_BROWN),
GEM("jet",               "black",  6, 1,  850, 15,  7, GEMSTONE, CLR_BLACK),
GEM("opal",              "white", 12, 1,  800, 15,  6, GEMSTONE, CLR_WHITE),
GEM("chrysoberyl",      "yellow",  8, 1,  700, 15,  5, GEMSTONE, CLR_YELLOW),
GEM("garnet",              "red", 12, 1,  700, 15,  7, GEMSTONE, CLR_RED),
GEM("amethyst",         "violet", 14, 1,  600, 15,  7, GEMSTONE, CLR_MAGENTA),
GEM("jasper",              "red", 15, 1,  500, 15,  7, GEMSTONE, CLR_RED),
GEM("fluorite",         "violet", 15, 1,  400, 15,  4, GEMSTONE, CLR_MAGENTA),
GEM("obsidian",          "black",  9, 1,  200, 15,  6, GEMSTONE, CLR_BLACK),
GEM("agate",            "orange", 12, 1,  200, 15,  6, GEMSTONE, CLR_ORANGE),
GEM("jade",              "green", 10, 1,  300, 15,  6, GEMSTONE, CLR_GREEN),
GEM("worthless piece of white glass", "white",
    74, 1, 0, 6, 5, GLASS, CLR_WHITE),
GEM("worthless piece of blue glass", "blue",
    74, 1, 0, 6, 5, GLASS, CLR_BLUE),
GEM("worthless piece of red glass", "red",
    73, 1, 0, 6, 5, GLASS, CLR_RED),
GEM("worthless piece of yellowish brown glass", "yellowish brown",
    73, 1, 0, 6, 5, GLASS, CLR_BROWN),
GEM("worthless piece of orange glass", "orange",
    73, 1, 0, 6, 5, GLASS, CLR_ORANGE),
GEM("worthless piece of yellow glass", "yellow",
    73, 1, 0, 6, 5, GLASS, CLR_YELLOW),
GEM("worthless piece of black glass", "black",
    73, 1, 0, 6, 5, GLASS, CLR_BLACK),
GEM("worthless piece of green glass", "green",
    74, 1, 0, 6, 5, GLASS, CLR_GREEN),
GEM("worthless piece of violet glass", "violet",
    74, 1, 0, 6, 5, GLASS, CLR_MAGENTA),

/* Placement note: there is a wishable subrange for
 * "gray stones" in the o_ranges[] array in objnam.c
 * that is currently everything between luckstones and flint
 * (inclusive).
 */
ROCK("luckstone", "gray",       0,  10,  10, 60, 3, 3, 1, 10, 7, MINERAL, CLR_GRAY),
ROCK("healthstone", "gray",     0,   8,  10, 60, 3, 3, 1, 10, 7, MINERAL, CLR_GRAY),  /* Slash'EM */
ROCK("loadstone", "gray",       0,  10, 500,  1, 3, 3, 1, 10, 6, MINERAL, CLR_GRAY),
ROCK("touchstone", "gray",      0,   8,  10, 45, 3, 3, 1, 10, 6, MINERAL, CLR_GRAY),
ROCK("whetstone", "gray",       0,   3,  10, 45, 3, 3, 1, 10, 7, MINERAL, CLR_GRAY),  /* Slash'EM */
ROCK("flint", "gray",           0,  10,   1,  1, 6, 6, 0, 10, 7, MINERAL, CLR_GRAY),
ROCK("sling bullet", "shiny",   0,  30,   1, 10, 6, 8, 0, 10, 7, IRON, HI_METAL),
ROCK("rock", None,              1, 100,  10,  0, 3, 3, 0, 10, 7, MINERAL, CLR_GRAY),
#undef GEM
#undef ROCK

/* miscellaneous ... */
/* Note: boulders and rocks are not normally created at random; the
 * probabilities only come into effect when you try to polymorph them.
 * Boulders weigh more than MAX_CARR_CAP; statues use corpsenm to take
 * on a specific type and may act as containers (both affect weight).
 */
OBJECT(OBJ("boulder", None),
       BITS(1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, P_NONE, MINERAL), 0,
       ROCK_CLASS, 100, 0, 6000, 0, 20, 20, 0, 0, 2000, HI_MINERAL),
OBJECT(OBJ("statue", None),
       BITS(1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, P_NONE, MINERAL), 0,
       ROCK_CLASS, 900, 0, 2500, 0, 20, 20, 0, 0, 2500, CLR_WHITE),

OBJECT(OBJ("heavy iron ball", None),
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHACK, P_FLAIL, IRON), 0,
       BALL_CLASS, 1000, 0, 480, 10, 25, 25, 0, 0, 200, HI_METAL),
        /* +d4 when "very heavy" */
OBJECT(OBJ("iron chain", None),
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHACK, P_NONE, IRON), 0,
       CHAIN_CLASS, 1000, 0, 120, 0, 4, 4, 0, 0, 200, HI_METAL),
        /* +1 both l & s */

/* Venom is normally a transitory missile (spit by various creatures)
 * but can be wished for in wizard mode so could occur in bones data.
 */
OBJECT(OBJ("blinding venom", "splash of venom"),
       BITS(0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, P_NONE, LIQUID), 0,
       VENOM_CLASS, 500, 0, 1, 0, 0, 0, 0, 0, 0, HI_ORGANIC),
OBJECT(OBJ("acid venom", "splash of venom"),
       BITS(0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, P_NONE, LIQUID), 0,
       VENOM_CLASS, 500, 0, 1, 0, 6, 6, 0, 0, 0, HI_ORGANIC),
OBJECT(OBJ("snowball", None),                                  /* EvilHack */
       BITS(1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, P_NONE, LIQUID), 0,
       VENOM_CLASS, 500, 0, 1, 0, 6, 6, 0, 0, 0, HI_GLASS),
        /* +d6 small or large */
/* manticore spikes - counts as WEPTOOL? */
OBJECT(OBJ("spike", None),                                    /* SpliceHack */
      BITS(1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, P_NONE, BONE), 0,
      VENOM_CLASS, 0, 0, 1, 0, 6, 6, 0, 0, 0, HI_ORGANIC),

OBJECT(OBJ("spirit", None),
        /*  nmkn,mrg,uskn,ctnr,mgc,chrg,uniq,nwsh,big,tuf,dir,sub,   mtrl) \ */
       BITS(1,   1,  0,   0,   0,  0,   0,   1,   0,  0,  0,  P_NONE,LIQUID), 
       /* prp,sym,          prob,dly,wt,cost,sdam,ldam,oc1,oc2,nut,color */
          0,  SPIRIT_CLASS, 500, 0,  1, 0,   0,   0,   0,  0,  0,  HI_GLASS),


/* fencepost, the deadly Array Terminator -- name [1st arg] *must* be NULL */
OBJECT(OBJ(None, None),
       BITS(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, 0), 0,
       ILLOBJ_CLASS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
}; /* objects[] */

#ifndef OBJECTS_PASS_2_

/* perform recursive compilation for second structure */
#undef OBJ
#undef OBJECT
#define OBJECTS_PASS_2_
#include "objects.c"

/* clang-format on */
/* *INDENT-ON* */

void NDECL(objects_init);

/* dummy routine used to force linkage */
void
objects_init()
{
    return;
}

#endif /* !OBJECTS_PASS_2_ */

/*objects.c*/
