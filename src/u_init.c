/* NetHack 3.6	u_init.c	$NHDT-Date: 1575245094 2019/12/02 00:04:54 $  $NHDT-Branch: NetHack-3.6 $:$NHDT-Revision: 1.60 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2017. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"

struct trobj {
    short trotyp;
    schar trspe;
    char trclass;
    Bitfield(trquan, 6);
    Bitfield(trbless, 2);
};

STATIC_DCL void FDECL(ini_inv, (struct trobj *));
STATIC_DCL void FDECL(knows_object, (int));
STATIC_DCL void FDECL(knows_class, (CHAR_P));
STATIC_DCL boolean FDECL(restricted_spell_discipline, (int));

#define UNDEF_TYP 0
#define UNDEF_SPE '\177'
#define UNDEF_BLESS 2
#define CURSED 3

/*
 *      Initial inventory for the various roles.
 */

struct trobj Archeologist[] = {
#define A_BOOK          4
    /* if adventure has a name...  idea from tan@uvm-gen */
    { BULLWHIP, 2, WEAPON_CLASS, 1, UNDEF_BLESS },
    { JACKET, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { FEDORA, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { FOOD_RATION, 0, FOOD_CLASS, 3, 0 },
    { UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, 1 },        
    { UNDEF_TYP, UNDEF_SPE, SCROLL_CLASS, 2, UNDEF_BLESS },
    { SCR_KNOWLEDGE, UNDEF_SPE, SCROLL_CLASS, 1, UNDEF_BLESS },
    /*{ PICK_AXE, UNDEF_SPE, TOOL_CLASS, 1, UNDEF_BLESS },*/
    { TINNING_KIT, UNDEF_SPE, TOOL_CLASS, 1, UNDEF_BLESS },
    { TOUCHSTONE, 0, GEM_CLASS, 1, 0 },
    { SACK, 0, TOOL_CLASS, 1, 0 },
    { 0, 0, 0, 0, 0 }
};
struct trobj Barbarian[] = {
#define B_MAJOR 0 /* two-handed sword or battle-axe  */
#define B_MINOR 1 /* matched with axe or short sword */
    { TWO_HANDED_SWORD, 0, WEAPON_CLASS, 1, UNDEF_BLESS },
    { AXE, 0, WEAPON_CLASS, 1, UNDEF_BLESS },
    { RING_MAIL, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { FOOD_RATION, 0, FOOD_CLASS, 2, 0 },
    { 0, 0, 0, 0, 0 }
};
static struct trobj Cartomancer[] = {
    { DAGGER, 1, WEAPON_CLASS, 1, UNDEF_BLESS },
    { FEDORA, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { HAWAIIAN_SHIRT, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { SHURIKEN, 0, GEM_CLASS, 60, UNDEF_BLESS },
    { UNDEF_TYP, UNDEF_SPE, SCROLL_CLASS, 4, UNDEF_BLESS },
    { SCR_CREATE_MONSTER, 0, SCROLL_CLASS, 1, UNDEF_BLESS },
    { SCR_CREATE_MONSTER, 1, SCROLL_CLASS, 1, UNDEF_BLESS },
    { SCR_CREATE_MONSTER, 2, SCROLL_CLASS, 1, UNDEF_BLESS },
    { 0, 0, 0, 0, 0 }
};
struct trobj Cave_man[] = {
#define C_AMMO 2
    { CLUB, 1, WEAPON_CLASS, 1, UNDEF_BLESS },
    { SLING, 2, WEAPON_CLASS, 1, UNDEF_BLESS },
    { FLINT, 0, GEM_CLASS, 15, UNDEF_BLESS }, /* trquan is overridden below */
    { SLING_BULLET, 0, GEM_CLASS, 1, UNDEF_BLESS },
    { LIGHT_ARMOR, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { 0, 0, 0, 0, 0 }
};
struct trobj Convict[] = {
    { SPOON, 0, TOOL_CLASS, 1, 0 },
    { ROCK, 0, GEM_CLASS, 1, 0 },
    { STRIPED_SHIRT, 0, ARMOR_CLASS, 1, 0 },
    { 0, 0, 0, 0, 0 }
};
struct trobj Flame_Mage[] = {
#define F_BOOK 9
    /* for dealing with ghosts */
    { QUARTERSTAFF, 1, WEAPON_CLASS, 1, 1 },
    { ROBE, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { FOOD_RATION, 0, FOOD_CLASS, 2, 0 },
    { POT_OIL, UNDEF_SPE, POTION_CLASS, 2, UNDEF_BLESS },
    { SCR_FIRE, UNDEF_SPE, SCROLL_CLASS, 1, 0 },
    { WAN_FIRE, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS },
    { UNDEF_TYP, UNDEF_SPE, RING_CLASS, 1, UNDEF_BLESS },
    { SPE_FIRE_BOLT, UNDEF_SPE, SPBOOK_CLASS, 1, 1 },
    { SPE_FIREBALL, UNDEF_SPE, SPBOOK_CLASS, 1, 1 },
    { UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, 1 },
    { FIRE_BOMB, 0, WEAPON_CLASS, 1, 0 },
    { 0, 0, 0, 0, 0 }
};
struct trobj Healer[] = {
    { SCALPEL, 1, WEAPON_CLASS, 1, 1 },
    { GLOVES, 1, ARMOR_CLASS, 1, UNDEF_BLESS },
    { STETHOSCOPE, 0, TOOL_CLASS, 1, 0 },
    { MEDICAL_KIT, 0, TOOL_CLASS, 1, 0 },   
    { POT_HEALING, 0, POTION_CLASS, 4, UNDEF_BLESS },
    { POT_EXTRA_HEALING, 0, POTION_CLASS, 4, UNDEF_BLESS },
    { WAN_SLEEP, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS },
    /* [Tom] they might as well have a wand of healing, too */        
    { WAN_HEALING, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS },
    /* always blessed, so it's guaranteed readable */
    { SPE_HEALING, 0, SPBOOK_CLASS, 1, 1 },
    { SPE_EXTRA_HEALING, 0, SPBOOK_CLASS, 1, 1 },
    { SPE_STONE_TO_FLESH, 0, SPBOOK_CLASS, 1, 1 },
    { APPLE, 0, FOOD_CLASS, 5, 0 },
    { 0, 0, 0, 0, 0 }
};
struct trobj Ice_Mage[] = {
#define I_BOOK          10
    { QUARTERSTAFF, 1, WEAPON_CLASS, 1, 1 },
    { ROBE, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { FOOD_RATION, 0, FOOD_CLASS, 2, 0 },
    { UNDEF_TYP, UNDEF_SPE, SCROLL_CLASS, 1, UNDEF_BLESS },
    { SCR_ICE, UNDEF_SPE, SCROLL_CLASS, 2, 1 },
    { WAN_COLD, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS },
    { FROST_HORN, UNDEF_SPE, TOOL_CLASS, 1, 1 },
    { UNDEF_TYP, UNDEF_SPE, RING_CLASS, 1, UNDEF_BLESS },
    { SPE_FREEZE_SPHERE, UNDEF_SPE, SPBOOK_CLASS, 1, 1 },
    { SPE_CONE_OF_COLD, UNDEF_SPE, SPBOOK_CLASS, 1, 1 },
    { UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, 1 },
    { 0, 0, 0, 0, 0 }
};
struct trobj Jedi[] = {
    { ROBE, 1, ARMOR_CLASS, 1, UNDEF_BLESS },
    { GLOVES, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { 0, 0, 0, 0, 0 }
};
struct trobj Infidel[] = {
    { DAGGER, 1, WEAPON_CLASS, 1, 0 },
    { AMULET_OF_YENDOR, 0, AMULET_CLASS, 1, 0 },
    { JACKET, 1, ARMOR_CLASS, 1, CURSED },
    { CLOAK_OF_PROTECTION, 0, ARMOR_CLASS, 1, CURSED },
    { POT_WATER, 0, POTION_CLASS, 3, CURSED },
    { SCR_CHARGING, 0, SCROLL_CLASS, 2, 0 },
    { SPE_DRAIN_LIFE, 0, SPBOOK_CLASS, 1, 0 },
    { UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, 0 },
    { FIRE_HORN, UNDEF_SPE, TOOL_CLASS, 1, 0 },
    { OILSKIN_SACK, 0, TOOL_CLASS, 1, 0 },
    { 0, 0, 0, 0, 0 }
};
struct trobj Knight[] = {
    { LONG_SWORD, 1, WEAPON_CLASS, 1, UNDEF_BLESS },
    { LANCE, 1, WEAPON_CLASS, 1, UNDEF_BLESS },
    { PLATE_MAIL, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { HELMET, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { SMALL_SHIELD, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { GAUNTLETS, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { APPLE, 0, FOOD_CLASS, 10, 0 },
    { CARROT, 0, FOOD_CLASS, 10, 0 },
    { 0, 0, 0, 0, 0 }
};
struct trobj Monk[] = {
#define M_BOOK 2
    { GLOVES, 2, ARMOR_CLASS, 1, UNDEF_BLESS },
    { ROBE_OF_POWER, 1, ARMOR_CLASS, 1, UNDEF_BLESS },
    { UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, 1 },
    { UNDEF_TYP, UNDEF_SPE, SCROLL_CLASS, 1, UNDEF_BLESS },
    { POT_HEALING, 0, POTION_CLASS, 3, UNDEF_BLESS },
    { FOOD_RATION, 0, FOOD_CLASS, 3, 0 },
    { APPLE, 0, FOOD_CLASS, 5, UNDEF_BLESS },
    { ORANGE, 0, FOOD_CLASS, 5, UNDEF_BLESS },
    /* Yes, we know fortune cookies aren't really from China.  They were
     * invented by George Jung in Los Angeles, California, USA in 1916.
     */
    { FORTUNE_COOKIE, 0, FOOD_CLASS, 3, UNDEF_BLESS },
    { 0, 0, 0, 0, 0 }
};
struct trobj Necromancer[] = {
#define N_BOOK          7
    { DAGGER, 0, WEAPON_CLASS, 2, UNDEF_BLESS },
    { UNDEF_TYP, UNDEF_SPE, RING_CLASS, 2, UNDEF_BLESS },
    { UNDEF_TYP, UNDEF_SPE, POTION_CLASS, 3, UNDEF_BLESS },
    { UNDEF_TYP, UNDEF_SPE, SCROLL_CLASS, 3, UNDEF_BLESS },
    { SPE_SUMMON_UNDEAD, 0, SPBOOK_CLASS, 1, 1 },
    { SPE_COMMAND_UNDEAD, 0, SPBOOK_CLASS, 1, 1 },
    { SPE_DRAIN_LIFE, 0, SPBOOK_CLASS, 1, 1 },
        /* WAC -- gave him drain life rather than turn undead */
    { UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, 1 },
    { WAN_DRAINING, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS },
        /* KMH -- ...and the matching wand for the inexperienced */
    { WAN_FEAR, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS },
    { PICK_AXE, 1, TOOL_CLASS, 1, 1 }, /* for dealing with ghosts */
    { 0, 0, 0, 0, 0 }
};
struct trobj Priest[] = {
    { MACE, 1, WEAPON_CLASS, 1, 1 },
    { ROBE, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { SMALL_SHIELD, 2, ARMOR_CLASS, 1, UNDEF_BLESS },
    { POT_WATER, 0, POTION_CLASS, 4, 1 }, /* holy water */
    { CLOVE_OF_GARLIC, 0, FOOD_CLASS, 1, 0 },
    { SPRIG_OF_WOLFSBANE, 0, FOOD_CLASS, 1, 0 },
    { UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 2, UNDEF_BLESS },
    { 0, 0, 0, 0, 0 }
};
static struct trobj Pirate[] = {
    { SCIMITAR, 0, WEAPON_CLASS, 1, UNDEF_BLESS },
    { FLINTLOCK, 0, WEAPON_CLASS, 1, 0 },
    { KNIFE, 1, WEAPON_CLASS, 3, 0 },
    { JACKET, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { HIGH_BOOTS, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { CRAM_RATION, 0, FOOD_CLASS, 2, UNDEF_BLESS },
#define PIR_SNACK 6
    { BANANA, 0, FOOD_CLASS, 3, 0 },
#define PIR_RUM 7
    { POT_BOOZE, 0, POTION_CLASS, 3, UNDEF_BLESS },
#define PIR_JEWELRY 8
    { UNDEF_TYP, UNDEF_SPE, RING_CLASS, 1, UNDEF_BLESS },
#define PIR_TOOL 9
    { OILSKIN_SACK, 0, TOOL_CLASS, 1, 0 },
#define PIR_BULLETS 10
    { BULLET,  0, WEAPON_CLASS, 8, 0 },
    { CRAM_RATION, 0, FOOD_CLASS, 2, UNDEF_BLESS },
    { 0, 0, 0, 0, 0 }
};
struct trobj Ranger[] = {
#define RAN_BOW 1
#define RAN_TWO_ARROWS 2
#define RAN_ZERO_ARROWS 3
    { DAGGER, 1, WEAPON_CLASS, 1, UNDEF_BLESS },
    { BOW, 1, WEAPON_CLASS, 1, UNDEF_BLESS },
    { ARROW, 2, WEAPON_CLASS, 50, UNDEF_BLESS },
    { ARROW, 0, WEAPON_CLASS, 30, UNDEF_BLESS },
    { CLOAK_OF_DISPLACEMENT, 2, ARMOR_CLASS, 1, UNDEF_BLESS },
    { CRAM_RATION, 0, FOOD_CLASS, 4, 0 },
    { 0, 0, 0, 0, 0 }
};
struct trobj Rogue[] = {
#define R_DAGGERS 1
#define R_DARTS   2
    { SHORT_SWORD, 1, WEAPON_CLASS, 1, UNDEF_BLESS },
    { DAGGER, 0, WEAPON_CLASS, 10, 0 }, /* quan is variable */
    { DART, 0, WEAPON_CLASS, 25, UNDEF_BLESS },
    { LIGHT_ARMOR, 1, ARMOR_CLASS, 1, UNDEF_BLESS },
    { POT_SICKNESS, 0, POTION_CLASS, 1, 0 },
    { SCR_GOLD_DETECTION, 0, SCROLL_CLASS, 2, 1 },
    { SCR_TELEPORTATION, 0, SCROLL_CLASS, 2, 1 },
    { LOCK_PICK, 0, TOOL_CLASS, 1, 0 },
    { OILSKIN_SACK, 0, TOOL_CLASS, 1, 0 },
    { 0, 0, 0, 0, 0 }
};
struct trobj Samurai[] = {
#define S_ARROWS 3
    { KATANA, 0, WEAPON_CLASS, 1, UNDEF_BLESS },
    { SHORT_SWORD, 3, WEAPON_CLASS, 1, UNDEF_BLESS }, /* wakizashi */
    { YUMI, 0, WEAPON_CLASS, 1, UNDEF_BLESS },
    { YA, 0, WEAPON_CLASS, 25, UNDEF_BLESS }, /* variable quan */
    { SPLINT_MAIL, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { 0, 0, 0, 0, 0 }
};
struct trobj Tourist[] = {
#define T_DARTS 0
    { DART, 2, WEAPON_CLASS, 25, UNDEF_BLESS }, /* quan is variable */
    { UNDEF_TYP, UNDEF_SPE, FOOD_CLASS, 12, 0 },
    { POT_EXTRA_HEALING, 0, POTION_CLASS, 2, UNDEF_BLESS },
    { SCR_MAGIC_MAPPING, 0, SCROLL_CLASS, 6, UNDEF_BLESS },
    { HAWAIIAN_SHIRT, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { EXPENSIVE_CAMERA, UNDEF_SPE, TOOL_CLASS, 1, 0 },
    { CREDIT_CARD, 0, TOOL_CLASS, 1, 0 },
    { LEASH, 0, TOOL_CLASS, 1, 0 },
    { MAGIC_MARKER, 0, TOOL_CLASS, 1, 0 },
    { TIN_OPENER, 0, TOOL_CLASS, 1, 0 },
    { TOWEL, 0, TOOL_CLASS, 1, 0 },
    { 0, 0, 0, 0, 0 }
};
struct trobj UndeadSlayer[] = {
#define U_MAJOR 0       /* wooden stake or platinum longsword for vampires [Blade] */
#define U_MINOR 1       /* silver spear or whip [Castlevania] 25/25% */
                        /* crossbow 50% [Buffy] */
#define U_RANGE 2       /* silver daggers or crossbow bolts */
#define U_MISC  3       /* +1 boots [Buffy can kick] or helmet */
#define U_ARMOR 4       /* Tshirt/leather +1 or chain mail */
	{ STAKE, 0, WEAPON_CLASS, 1, UNDEF_BLESS },
    { SPEAR, 0, WEAPON_CLASS, 1, UNDEF_BLESS },
    { DAGGER, 0, WEAPON_CLASS, 5, UNDEF_BLESS },
	{ HELMET, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
	{ CHAIN_MAIL, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { CLOVE_OF_GARLIC, 0, FOOD_CLASS, 5, 1 },
	{ SPRIG_OF_WOLFSBANE, 0, FOOD_CLASS, 5, 1 },
	{ HOLY_WAFER, 0, FOOD_CLASS, 4, 0 },
	{ POT_WATER, 0, POTION_CLASS, 4, 1 },        /* holy water */
    { TORCH, 0, TOOL_CLASS, 1, 0 },
	{ 0, 0, 0, 0, 0 }
};
struct trobj Valkyrie[] = {
    { SPEAR, 1, WEAPON_CLASS, 1, UNDEF_BLESS },
    { DAGGER, 0, WEAPON_CLASS, 1, UNDEF_BLESS },
    { SMALL_SHIELD, 3, ARMOR_CLASS, 1, UNDEF_BLESS },
    { FOOD_RATION, 0, FOOD_CLASS, 1, 0 },
    { 0, 0, 0, 0, 0 }
};
struct trobj Wizard[] = {
#define W_MULTSTART 2
#define W_MULTEND 6
    { QUARTERSTAFF, 1, WEAPON_CLASS, 1, 1 },
    { CLOAK_OF_MAGIC_RESISTANCE, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { UNDEF_TYP, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS },
    { UNDEF_TYP, UNDEF_SPE, RING_CLASS, 2, UNDEF_BLESS },
    { UNDEF_TYP, UNDEF_SPE, POTION_CLASS, 3, UNDEF_BLESS },
    { UNDEF_TYP, UNDEF_SPE, SCROLL_CLASS, 3, UNDEF_BLESS },
    { SPE_FORCE_BOLT, 0, SPBOOK_CLASS, 1, 1 },
    { UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, UNDEF_BLESS },
    { 0, 0, 0, 0, 0 }
};
struct trobj Yeoman[] = {
    { SHORT_SWORD, 2, WEAPON_CLASS, 1, UNDEF_BLESS },
    { PARTISAN, 1, WEAPON_CLASS, 1, UNDEF_BLESS },
    { GREEN_COAT, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { HIGH_BOOTS, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { GLOVES, 0, ARMOR_CLASS, 1, UNDEF_BLESS },
    { APPLE, 0, FOOD_CLASS, 10, 0 },
    { CARROT, 0, FOOD_CLASS, 10, 0 },
    { POT_WATER, 0, POTION_CLASS, 3, 0 },
    { FISHING_POLE, 0, TOOL_CLASS, 1, 0 },
    { TOOLED_HORN, 0, TOOL_CLASS, 1, 1 },
    { TORCH, 0, TOOL_CLASS, 1, 0 },
    { WHETSTONE, 0, TOOL_CLASS, 1, 1 },
    { 0, 0, 0, 0, 0 }
};

/*
 *      Optional extra inventory items.
 */

struct trobj Tinopener[] = { { TIN_OPENER, 0, TOOL_CLASS, 1, 0 },
                                    { 0, 0, 0, 0, 0 } };
struct trobj Lamp[] = { { OIL_LAMP, 1, TOOL_CLASS, 1, 0 },
                               { 0, 0, 0, 0, 0 } };
struct trobj Torch[] = { { TORCH, 0, TOOL_CLASS, 1, 0 },
                               { 0, 0, 0, 0, 0 } };
struct trobj Blindfold[] = { { BLINDFOLD, 0, TOOL_CLASS, 1, 0 },
                                    { 0, 0, 0, 0, 0 } };
struct trobj Instrument[] = { { FLUTE, 0, TOOL_CLASS, 1, 0 },
                                     { 0, 0, 0, 0, 0 } };
struct trobj Xtra_food[] = { { UNDEF_TYP, UNDEF_SPE, FOOD_CLASS, 2, 0 },
                                    { 0, 0, 0, 0, 0 } };
struct trobj Leash[] = { { LEASH, 0, TOOL_CLASS, 1, 0 },
                                { 0, 0, 0, 0, 0 } };
struct trobj Towel[] = { { TOWEL, 0, TOOL_CLASS, 1, 0 },
                                { 0, 0, 0, 0, 0 } };
struct trobj Wishing[] = { { WAN_WISHING, 3, WAND_CLASS, 1, 0 },
                                  { 0, 0, 0, 0, 0 } };
struct trobj DeathWand[] = { { WAN_DEATH, 1, WAND_CLASS, 1, 0 },
                           { 0, 0, 0, 0, 0 } };
struct trobj Money[] = { { GOLD_PIECE, 0, COIN_CLASS, 1, 0 },
                                { 0, 0, 0, 0, 0 } };
struct trobj Gem[] = { { UNDEF_TYP, 0, GEM_CLASS, 1, 0 },
                              { 0, 0, 0, 0, 0 } };
struct trobj Tinningkit[] = { { TINNING_KIT, UNDEF_SPE, TOOL_CLASS, 1, 0 },
                                     { 0, 0, 0, 0, 0 } };
struct trobj MedicalKit[] = { { MEDICAL_KIT, UNDEF_SPE, TOOL_CLASS, 1, 0 },
                              { 0, 0, 0, 0, 0 } };
struct trobj Pickaxe[] = { { PICK_AXE, 0, TOOL_CLASS, 1, 0 },
                                  { 0, 0, 0, 0, 0 } };
struct trobj Stake[] = { { STAKE, 0, WEAPON_CLASS, 1, 0 },
                           { 0, 0, 0, 0, 0 } };
struct trobj AoMR[] = { { AMULET_OF_MAGIC_RESISTANCE, 0, AMULET_CLASS, 1, 0 },
                               { 0, 0, 0, 0, 0 } };
struct trobj AoUn[] = { { AMULET_OF_UNCHANGING, 0, AMULET_CLASS, 1, CURSED },
                               { 0, 0, 0, 0, 0 } };
struct trobj Oilskin[] = { { OILSKIN_SACK, 0, TOOL_CLASS, 1, 0 },
                                  { 0, 0, 0, 0, 0 } };
struct trobj Goggles[] = { { GOGGLES, 0, TOOL_CLASS, 1, 0 },
                           { 0, 0, 0, 0, 0 } };
struct trobj Lenses[] = { { LENSES, 0, TOOL_CLASS, 1, 0 },
                           { 0, 0, 0, 0, 0 } };
struct trobj GrapplingHook[] = { { GRAPPLING_HOOK, 0, TOOL_CLASS, 1, 0 },
                          { 0, 0, 0, 0, 0 } };
struct trobj GreenSaber[] = { { GREEN_LIGHTSABER, 1, WEAPON_CLASS, 1, UNDEF_BLESS },
                              { 0, 0, 0, 0, 0, } };
struct trobj BlueSaber[] = { { BLUE_LIGHTSABER, 1, WEAPON_CLASS, 1, UNDEF_BLESS },
                             { 0, 0, 0, 0, 0, } };
struct trobj RedSaber[] = { { RED_LIGHTSABER, 1, WEAPON_CLASS, 1, UNDEF_BLESS },
                            { 0, 0, 0, 0, 0, } };

/* race-based substitutions for initial inventory;
   the weaker cloak for elven rangers is intentional--they shoot better */
struct inv_sub {
    short race_pm, item_otyp, subs_otyp;
} inv_subs[] = {
    /* Elves */
    { PM_ELF, DAGGER, ELVEN_DAGGER },
    { PM_ELF, SPEAR, ELVEN_SPEAR },
    { PM_ELF, SHORT_SWORD, ELVEN_SHORT_SWORD },
    { PM_ELF, LONG_SWORD, ELVEN_LONG_SWORD },
    { PM_ELF, BOW, ELVEN_BOW },
    { PM_ELF, ARROW, ELVEN_ARROW },
    { PM_ELF, PLATE_MAIL, ELVEN_CHAIN_MAIL },
    { PM_ELF, HELMET, ELVEN_HELM },
    { PM_ELF, GAUNTLETS, GLOVES },
    /* { PM_ELF, SMALL_SHIELD, ELVEN_SHIELD }, */
    { PM_ELF, CLOAK_OF_DISPLACEMENT, ELVEN_CLOAK },
    { PM_ELF, CRAM_RATION, LEMBAS_WAFER },
    /* Orcs */
    { PM_ORC, DAGGER, ORCISH_DAGGER },
    { PM_ORC, SPEAR, ORCISH_SPEAR },
    { PM_ORC, SHORT_SWORD, ORCISH_SHORT_SWORD },
    { PM_ORC, BOW, ORCISH_BOW },
    { PM_ORC, ARROW, ORCISH_ARROW },
    { PM_ORC, HELMET, ORCISH_HELM },
    { PM_ORC, SMALL_SHIELD, ORCISH_SHIELD },
    { PM_ORC, RING_MAIL, ORCISH_RING_MAIL },
    { PM_ORC, CHAIN_MAIL, ORCISH_CHAIN_MAIL },
    { PM_ORC, CRAM_RATION, TRIPE_RATION },
    { PM_ORC, LEMBAS_WAFER, TRIPE_RATION },
    { PM_ORC, LONG_SWORD, ORCISH_LONG_SWORD },
    /* Dwarves */
    { PM_DWARF, SPEAR, DWARVISH_SPEAR },
    { PM_DWARF, SHORT_SWORD, DWARVISH_SHORT_SWORD },
    { PM_DWARF, HELMET, DWARVISH_HELM },
    /* { PM_DWARF, SMALL_SHIELD, DWARVISH_ROUNDSHIELD }, */
    /* { PM_DWARF, PICK_AXE, DWARVISH_MATTOCK }, */
    { PM_DWARF, LEMBAS_WAFER, CRAM_RATION },
    { PM_DWARF, LONG_SWORD, DWARVISH_BEARDED_AXE },
    { PM_GNOME, BOW, CROSSBOW },
    { PM_GNOME, ARROW, CROSSBOW_BOLT },
    { PM_VAMPIRIC, POT_FRUIT_JUICE, POT_BLOOD },
    { PM_VAMPIRIC, FOOD_RATION, POT_VAMPIRE_BLOOD },
    { PM_VAMPIRIC, CRAM_RATION, POT_VAMPIRE_BLOOD },
    { PM_VAMPIRIC, HOLY_WAFER, POT_VAMPIRE_BLOOD },
    { PM_VAMPIRIC, HELMET, GAUNTLETS },
    { PM_VAMPIRIC, CHAIN_MAIL, JACKET },
    /* Giants have special considerations */
    { PM_GIANT, ROBE, LARGE_ROBE },
    { PM_GIANT, ROBE_OF_POWER, LARGE_ROBE },
    { PM_GIANT, RING_MAIL, HELMET },
    { PM_GIANT, LIGHT_ARMOR, HELMET },
    { PM_GIANT, CLOAK_OF_MAGIC_RESISTANCE, LARGE_ROBE },
    { PM_GIANT, RIN_STEALTH, RIN_SEARCHING },
    { PM_GIANT, SPLINT_MAIL, LARGE_SPLINT_MAIL },
    { PM_GIANT, CHAIN_MAIL, LOW_BOOTS },
    { PM_GIANT, JACKET, LOW_BOOTS },
    { PM_GIANT, CLOAK_OF_PROTECTION, GAUNTLETS_OF_PROTECTION },
    /* Hobbits have a thing for elven gear */
    { PM_HOBBIT, DAGGER, ELVEN_DAGGER },
    { PM_HOBBIT, SPEAR, ELVEN_SPEAR },
    { PM_HOBBIT, SHORT_SWORD, ELVEN_SHORT_SWORD },
    { PM_HOBBIT, BOW, ELVEN_BOW },
    { PM_HOBBIT, ARROW, ELVEN_ARROW },
    { PM_HOBBIT, HELMET, ELVEN_HELM },
    { PM_HOBBIT, CLOAK_OF_DISPLACEMENT, ELVEN_CLOAK },
    { PM_HOBBIT, CRAM_RATION, LEMBAS_WAFER },
    /* Tortles also have special considerations */
    { PM_TORTLE, JACKET, GLOVES },
    { PM_TORTLE, RING_MAIL, TOQUE },
    { PM_TORTLE, SPLINT_MAIL, TOQUE },
    { PM_TORTLE, BATTLE_AXE, TRIDENT },
    { PM_TORTLE, ROBE, TOQUE },
    { PM_TORTLE, ROBE_OF_POWER, TOQUE },
    { PM_TORTLE, HAWAIIAN_SHIRT, TOQUE },
    { PM_TORTLE, SPEAR, TRIDENT }, /* Undead Slayer */
    { PM_TORTLE, HELMET, TOQUE }, /* Undead Slayer */
    { PM_TORTLE, CHAIN_MAIL, GLOVES }, /* Undead Slayer */
    { PM_TORTLE, CLOAK_OF_MAGIC_RESISTANCE, GLOVES },
    /* Centaurs */
    { PM_CENTAUR, HIGH_BOOTS, HELMET },
    { NON_PM, STRANGE_OBJECT, STRANGE_OBJECT }
};

static const struct def_skill Skill_A[] = {
    { P_DAGGER, P_BASIC },
    { P_KNIFE, P_BASIC },
    { P_PICK_AXE, P_EXPERT },
    { P_SHORT_SWORD, P_BASIC },
    { P_SABER, P_EXPERT },
    { P_CLUB, P_BASIC },
    { P_QUARTERSTAFF, P_SKILLED },
    { P_SPEAR, P_EXPERT },
    { P_SLING, P_SKILLED },
    { P_DART, P_BASIC },
    { P_BOOMERANG, P_EXPERT },
    { P_WHIP, P_EXPERT },
    { P_UNICORN_HORN, P_SKILLED },
    { P_ATTACK_SPELL, P_BASIC },
    { P_HEALING_SPELL, P_BASIC },
    { P_DIVINATION_SPELL, P_EXPERT },
    { P_MATTER_SPELL, P_BASIC },
    { P_RIDING, P_BASIC },
    { P_TWO_WEAPON_COMBAT, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_EXPERT },
    { P_SHIELD, P_SKILLED },
    { P_FIREARM, P_SKILLED },
    { P_NONE, 0 }
};
static const struct def_skill Skill_B[] = {
    { P_DAGGER, P_BASIC },
    { P_AXE, P_EXPERT },
    { P_PICK_AXE, P_SKILLED },
    { P_SHORT_SWORD, P_EXPERT },
    { P_BROAD_SWORD, P_SKILLED },
    { P_LONG_SWORD, P_SKILLED },
    { P_TWO_HANDED_SWORD, P_EXPERT },
    { P_SABER, P_SKILLED },
    { P_CLUB, P_SKILLED },
    { P_MACE, P_SKILLED },
    { P_MORNING_STAR, P_SKILLED },
    { P_FLAIL, P_BASIC },
    { P_HAMMER, P_EXPERT },
    { P_SPEAR, P_SKILLED },
    { P_TRIDENT, P_SKILLED },
    { P_BOW, P_BASIC },
    { P_ENCHANTMENT_SPELL, P_BASIC }, /* special spell is cause fear */
    { P_RIDING, P_SKILLED },
    { P_TWO_WEAPON_COMBAT, P_SKILLED },
    { P_BARE_HANDED_COMBAT, P_MASTER },
    { P_SHIELD, P_SKILLED },
    { P_NONE, 0 }
};
static const struct def_skill Skill_Car[] = {
    { P_DAGGER, P_EXPERT },
    { P_KNIFE, P_SKILLED },
    { P_AXE, P_BASIC },
    { P_SHORT_SWORD, P_BASIC },
    { P_CLUB, P_BASIC },
    { P_MACE, P_BASIC },
    { P_QUARTERSTAFF, P_BASIC },
    { P_POLEARMS, P_BASIC },
    { P_SPEAR, P_BASIC },
    { P_TRIDENT, P_BASIC },
    { P_SLING, P_SKILLED },
    { P_DART, P_EXPERT },
    { P_SHURIKEN, P_EXPERT },
    { P_ATTACK_SPELL, P_BASIC },
    { P_HEALING_SPELL, P_BASIC },
    
    { P_DIVINATION_SPELL, P_EXPERT },
    { P_ENCHANTMENT_SPELL, P_SKILLED },
    { P_NECROMANCY_SPELL, P_SKILLED },
    { P_ESCAPE_SPELL, P_SKILLED },
    { P_MATTER_SPELL, P_SKILLED },
    { P_RIDING, P_EXPERT },         /* Card games on motorcycles. */
    { P_BARE_HANDED_COMBAT, P_SKILLED },

   /* { P_WILD_MAGIC, P_EXPERT },*/
    { P_NONE, 0 }
};
static const struct def_skill Skill_C[] = {
    { P_DAGGER, P_BASIC },
    { P_KNIFE, P_SKILLED },
    { P_AXE, P_SKILLED },
    { P_PICK_AXE, P_BASIC },
    { P_CLUB, P_EXPERT },
    { P_MACE, P_EXPERT },
    { P_MORNING_STAR, P_BASIC },
    { P_FLAIL, P_SKILLED },
    { P_HAMMER, P_SKILLED },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_POLEARMS, P_SKILLED },
    { P_SPEAR, P_EXPERT },
    { P_TRIDENT, P_SKILLED },
    { P_BOW, P_SKILLED },
    { P_SLING, P_EXPERT },
    { P_BOOMERANG, P_EXPERT },
    { P_UNICORN_HORN, P_BASIC },
    { P_RIDING, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_MASTER },
    { P_SHIELD, P_BASIC },
    { P_NONE, 0 }
};
static const struct def_skill Skill_Con[] = {
    { P_DAGGER, P_SKILLED },
    { P_KNIFE,  P_EXPERT },
    { P_HAMMER, P_SKILLED },
    { P_PICK_AXE, P_EXPERT },
    { P_CLUB, P_EXPERT },
    { P_MACE, P_BASIC },
    { P_DART, P_SKILLED },
    { P_FLAIL, P_EXPERT },
    { P_SHORT_SWORD, P_BASIC },
    { P_BROAD_SWORD, P_SKILLED },
    { P_SLING, P_SKILLED },
    { P_FIREARM, P_EXPERT },
    { P_ATTACK_SPELL, P_BASIC },
    { P_ESCAPE_SPELL, P_EXPERT },
    { P_RIDING, P_BASIC },
    { P_TWO_WEAPON_COMBAT, P_SKILLED },
    { P_THIEVERY, P_SKILLED },
    { P_SHIELD, P_BASIC },
    { P_NONE, 0 }
};

static const struct def_skill Skill_F[] = {
/*Style: small-med edged weapons, blunt weapons*/
    { P_AXE, P_SKILLED },                   /* for chopping wood. */
    { P_PICK_AXE, P_SKILLED },              /* for digging up coal */
    { P_SHORT_SWORD, P_SKILLED },           /* For access to fire-brand */
    { P_MACE, P_BASIC },                    /* No good reason, just variety.*/
/*  { P_MORNING_STAR, P_BASIC },*/
    { P_CLUB, P_EXPERT },                   /* because clubs/torches can be lit.*/
    { P_HAMMER, P_SKILLED },
    { P_QUARTERSTAFF, P_EXPERT },           /* sac gift is Firewall, a quarterstaff */
    { P_BOW, P_BASIC },
    { P_SLING, P_SKILLED },                 /* Familiar with flint stones */
    { P_WHIP, P_SKILLED },                  /* Potentially for flaming whips */
    { P_UNICORN_HORN, P_SKILLED },
    { P_FIREARM, P_SKILLED },               /* Related to fire */
    
    { P_ATTACK_SPELL, P_SKILLED },
/*  { P_HEALING_SPELL, P_BASIC },*/
    { P_DIVINATION_SPELL, P_EXPERT },
    { P_MATTER_SPELL, P_EXPERT },
    
    { P_BARE_HANDED_COMBAT, P_SKILLED },
    { P_NONE, 0 }
};

static const struct def_skill Skill_H[] = {
    { P_DAGGER, P_SKILLED },
    { P_KNIFE, P_EXPERT },
    { P_SHORT_SWORD, P_SKILLED },
    { P_SABER, P_BASIC },
    { P_CLUB, P_SKILLED },
    { P_MACE, P_BASIC },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_POLEARMS, P_BASIC },
    { P_SPEAR, P_BASIC },
    { P_TRIDENT, P_BASIC },
    { P_SLING, P_SKILLED },
    { P_DART, P_EXPERT },
    { P_SHURIKEN, P_SKILLED },
    { P_UNICORN_HORN, P_EXPERT },
    { P_HEALING_SPELL, P_EXPERT },
    { P_ENCHANTMENT_SPELL, P_SKILLED },
    { P_RIDING, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_BASIC },
    { P_NONE, 0 }
};
static const struct def_skill Skill_I[] = {
    /*Resorts mostly to stabbing weapons*/
    { P_DAGGER, P_EXPERT },
    { P_KNIFE,  P_EXPERT },
    { P_AXE, P_BASIC },
    { P_PICK_AXE, P_BASIC },
    { P_SHORT_SWORD, P_EXPERT },
    { P_BROAD_SWORD, P_BASIC },
    { P_LONG_SWORD, P_BASIC },
    { P_FLAIL, P_BASIC },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_SPEAR, P_BASIC },
    { P_TRIDENT, P_BASIC },
    { P_BOW, P_BASIC },
    { P_SLING, P_SKILLED },
    { P_CROSSBOW, P_BASIC },
    { P_DART, P_BASIC },
    { P_UNICORN_HORN, P_SKILLED },

    { P_ATTACK_SPELL, P_BASIC },
    { P_DIVINATION_SPELL, P_BASIC },
    { P_ENCHANTMENT_SPELL, P_EXPERT },
    { P_ESCAPE_SPELL, P_BASIC },
    { P_MATTER_SPELL, P_EXPERT },

    { P_RIDING, P_EXPERT },
    /*{ P_TWO_WEAPON_COMBAT, P_SKILLED },*/
    { P_BARE_HANDED_COMBAT, P_SKILLED },
    { P_NONE, 0 }
};
static const struct def_skill Skill_Inf[] = {
    { P_DAGGER, P_EXPERT },
    { P_KNIFE, P_EXPERT },
    { P_SHORT_SWORD, P_SKILLED },
    { P_BROAD_SWORD, P_BASIC },
    { P_SABER, P_SKILLED },
    { P_CLUB, P_BASIC },
    { P_HAMMER, P_BASIC },
    { P_QUARTERSTAFF, P_SKILLED },
    { P_POLEARMS, P_SKILLED },
    { P_SPEAR, P_BASIC },
    { P_SLING, P_BASIC },
    { P_CROSSBOW, P_SKILLED },
    { P_DART, P_BASIC },
    { P_WHIP, P_SKILLED },
    { P_ATTACK_SPELL, P_SKILLED },
    { P_MATTER_SPELL, P_SKILLED },
    { P_NECROMANCY_SPELL, P_SKILLED },
    { P_DIVINATION_SPELL, P_SKILLED },
    { P_BARE_HANDED_COMBAT, P_SKILLED },
    { P_RIDING, P_SKILLED },
    { P_NONE, 0 }
};
static const struct def_skill Skill_J[] = {
    /* 5lo: Straight from Jedi patch */
    { P_LIGHTSABER, P_EXPERT },
    { P_SHORT_SWORD, P_BASIC }, 
    { P_BROAD_SWORD, P_BASIC },
    { P_LONG_SWORD, P_SKILLED }, 
    { P_SABER, P_SKILLED },
    { P_FIREARM, P_BASIC },
    { P_HEALING_SPELL, P_BASIC },
    { P_ENCHANTMENT_SPELL, P_SKILLED }, /* special spell is charm monster */
    { P_RIDING, P_SKILLED },
    { P_TWO_WEAPON_COMBAT, P_BASIC }, 
    { P_BARE_HANDED_COMBAT, P_EXPERT },
    { P_NONE, 0 }
};
static const struct def_skill Skill_K[] = {
    { P_DAGGER, P_BASIC },
    { P_KNIFE, P_BASIC },
    { P_AXE, P_EXPERT },
    { P_PICK_AXE, P_BASIC },
    { P_SHORT_SWORD, P_SKILLED },
    { P_BROAD_SWORD, P_EXPERT },
    { P_LONG_SWORD, P_EXPERT },
    { P_TWO_HANDED_SWORD, P_EXPERT },
    { P_SABER, P_SKILLED },
    { P_CLUB, P_BASIC },
    { P_MACE, P_BASIC },
    { P_MORNING_STAR, P_BASIC },
    { P_FLAIL, P_BASIC },
    { P_HAMMER, P_BASIC },
    { P_POLEARMS, P_EXPERT },
    { P_SPEAR, P_EXPERT },
    { P_TRIDENT, P_BASIC },
    { P_LANCE, P_EXPERT },
    { P_BOW, P_BASIC },
    { P_CROSSBOW, P_SKILLED },
    { P_ATTACK_SPELL, P_BASIC },
    { P_HEALING_SPELL, P_BASIC },
    { P_ENCHANTMENT_SPELL, P_SKILLED },
    { P_RIDING, P_EXPERT },
    { P_TWO_WEAPON_COMBAT, P_SKILLED },
    { P_BARE_HANDED_COMBAT, P_EXPERT },
    { P_SHIELD, P_EXPERT },
    { P_NONE, 0 }
};
static const struct def_skill Skill_Mon[] = {
    { P_QUARTERSTAFF, P_EXPERT },
    { P_LIGHTSABER, P_SKILLED },
    { P_SHURIKEN, P_BASIC },
    { P_SPEAR, P_SKILLED },
    { P_TRIDENT, P_SKILLED },
    { P_BROAD_SWORD, P_BASIC },
    { P_ATTACK_SPELL, P_BASIC },
    { P_HEALING_SPELL, P_EXPERT },
    { P_DIVINATION_SPELL, P_BASIC },
    { P_ENCHANTMENT_SPELL, P_SKILLED },
    { P_ESCAPE_SPELL, P_SKILLED },
    { P_MATTER_SPELL, P_BASIC },
    { P_RIDING, P_BASIC },
    { P_MARTIAL_ARTS, P_GRAND_MASTER },
    { P_NONE, 0 }
};
static const struct def_skill Skill_N[] = {
    { P_DAGGER, P_EXPERT },             /* Sac gift and quest artifact */
    { P_AXE, P_BASIC },                 /* For choppin up bodies */
    { P_PICK_AXE, P_EXPERT },            /* For digging up graves */
    { P_POLEARMS, P_BASIC },            /* For scythes */
    { P_MACE, P_BASIC },                /* Executioner's mace, etc */
    { P_CROSSBOW, P_BASIC },
    { P_UNICORN_HORN, P_BASIC },        /* Using a dead animals horn is dark */
    { P_QUARTERSTAFF, P_EXPERT },
    
    { P_ATTACK_SPELL, P_SKILLED },
    { P_NECROMANCY_SPELL, P_EXPERT },
    { P_BARE_HANDED_COMBAT, P_BASIC },
    { P_SHIELD, P_SKILLED },
    { P_NONE, 0 }
};

static const struct def_skill Skill_P[] = {
    { P_CLUB, P_EXPERT },
    { P_MACE, P_EXPERT },
    { P_MORNING_STAR, P_EXPERT },
    { P_HAMMER, P_EXPERT },
    { P_FLAIL, P_EXPERT },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_POLEARMS, P_SKILLED },
    { P_SLING, P_BASIC },
    { P_BOOMERANG, P_BASIC },
    { P_HEALING_SPELL, P_EXPERT },
    { P_DIVINATION_SPELL, P_EXPERT },
    { P_ENCHANTMENT_SPELL, P_EXPERT },
    { P_RIDING, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_BASIC },
    { P_SHIELD, P_SKILLED },
    { P_NONE, 0 }
};
static const struct def_skill Skill_Pir[] = {
    { P_DAGGER, P_SKILLED },
    { P_KNIFE,  P_EXPERT },
    { P_AXE, P_SKILLED },
    { P_SHORT_SWORD, P_BASIC },
    { P_BROAD_SWORD, P_EXPERT },
    { P_LONG_SWORD, P_BASIC },
    { P_SABER, P_EXPERT },
    { P_CLUB, P_BASIC },
    { P_MORNING_STAR, P_SKILLED },
    { P_FLAIL, P_EXPERT },
    { P_SPEAR, P_SKILLED },
    { P_TRIDENT, P_EXPERT },
    { P_CROSSBOW, P_EXPERT },
    { P_DART, P_SKILLED },
    { P_WHIP, P_SKILLED },
    { P_UNICORN_HORN, P_BASIC },
    { P_ATTACK_SPELL, P_BASIC },
    { P_DIVINATION_SPELL, P_BASIC },
    { P_ENCHANTMENT_SPELL, P_BASIC },
    { P_ESCAPE_SPELL, P_SKILLED },
    { P_TWO_WEAPON_COMBAT, P_SKILLED },
    { P_BARE_HANDED_COMBAT, P_EXPERT },
    { P_FIREARM, P_EXPERT },
    { P_NONE, 0 }
};
static const struct def_skill Skill_R[] = {
    { P_DAGGER, P_EXPERT },
    { P_KNIFE, P_EXPERT },
    { P_SHORT_SWORD, P_EXPERT },
    { P_BROAD_SWORD, P_SKILLED },
    { P_LONG_SWORD, P_SKILLED },
    { P_TWO_HANDED_SWORD, P_BASIC },
    { P_SABER, P_SKILLED },
    { P_CLUB, P_SKILLED },
    { P_MACE, P_SKILLED },
    { P_MORNING_STAR, P_BASIC },
    { P_FLAIL, P_BASIC },
    { P_HAMMER, P_BASIC },
    { P_POLEARMS, P_BASIC },
    { P_SPEAR, P_BASIC },
    { P_CROSSBOW, P_EXPERT },
    { P_DART, P_EXPERT },
    { P_SHURIKEN, P_SKILLED },
    { P_DIVINATION_SPELL, P_SKILLED },
    { P_ESCAPE_SPELL, P_SKILLED },
    { P_MATTER_SPELL, P_SKILLED },
    { P_RIDING, P_BASIC },
    { P_TWO_WEAPON_COMBAT, P_EXPERT },
    { P_FIREARM, P_EXPERT },
    { P_THIEVERY, P_MASTER },
    { P_SHIELD, P_BASIC },
    { P_NONE, 0 }
};
static const struct def_skill Skill_Ran[] = {
    { P_DAGGER, P_EXPERT },
    { P_KNIFE, P_SKILLED },
    { P_AXE, P_SKILLED },
    { P_SHORT_SWORD, P_BASIC },
    { P_BROAD_SWORD, P_SKILLED },
    { P_MORNING_STAR, P_BASIC },
    { P_FLAIL, P_SKILLED },
    { P_HAMMER, P_BASIC },
    { P_POLEARMS, P_BASIC },
    { P_SPEAR, P_EXPERT },
    { P_TRIDENT, P_BASIC },
    { P_BOW, P_EXPERT },
    { P_SLING, P_EXPERT },
    { P_CROSSBOW, P_EXPERT },
    { P_DART, P_EXPERT },
    { P_SHURIKEN, P_SKILLED },
    { P_BOOMERANG, P_EXPERT },
    { P_WHIP, P_BASIC },
    { P_HEALING_SPELL, P_BASIC },
    { P_DIVINATION_SPELL, P_EXPERT },
    { P_ESCAPE_SPELL, P_BASIC },
    { P_RIDING, P_BASIC },
    { P_TWO_WEAPON_COMBAT, P_SKILLED },
    { P_BARE_HANDED_COMBAT, P_BASIC },
    { P_SHIELD, P_SKILLED },
    { P_FIREARM, P_EXPERT },
    { P_NONE, 0 }
};
static const struct def_skill Skill_S[] = {
    { P_DAGGER, P_BASIC },
    { P_KNIFE, P_SKILLED },
    { P_SHORT_SWORD, P_EXPERT },
    { P_BROAD_SWORD, P_SKILLED },
    { P_LONG_SWORD, P_EXPERT },
    { P_TWO_HANDED_SWORD, P_EXPERT },
    { P_SABER, P_BASIC },
    { P_FLAIL, P_SKILLED },
    { P_QUARTERSTAFF, P_BASIC },
    { P_POLEARMS, P_SKILLED },
    { P_SPEAR, P_SKILLED },
    { P_LANCE, P_SKILLED },
    { P_BOW, P_EXPERT },
    { P_SHURIKEN, P_EXPERT },
    { P_DIVINATION_SPELL, P_BASIC }, /* special spell is clairvoyance */
    { P_RIDING, P_SKILLED },
    { P_TWO_WEAPON_COMBAT, P_EXPERT },
    { P_MARTIAL_ARTS, P_MASTER },
    { P_NONE, 0 }
};
static const struct def_skill Skill_T[] = {
    { P_DAGGER, P_EXPERT },
    { P_KNIFE, P_SKILLED },
    { P_AXE, P_BASIC },
    { P_PICK_AXE, P_BASIC },
    { P_SHORT_SWORD, P_EXPERT },
    { P_BROAD_SWORD, P_BASIC },
    { P_LONG_SWORD, P_BASIC },
    { P_TWO_HANDED_SWORD, P_BASIC },
    { P_SABER, P_SKILLED },
    { P_MACE, P_BASIC },
    { P_MORNING_STAR, P_BASIC },
    { P_FLAIL, P_BASIC },
    { P_HAMMER, P_BASIC },
    { P_QUARTERSTAFF, P_BASIC },
    { P_POLEARMS, P_BASIC },
    { P_SPEAR, P_BASIC },
    { P_TRIDENT, P_BASIC },
    { P_LANCE, P_BASIC },
    { P_BOW, P_BASIC },
    { P_SLING, P_BASIC },
    { P_CROSSBOW, P_BASIC },
    { P_DART, P_EXPERT },
    { P_SHURIKEN, P_BASIC },
    { P_BOOMERANG, P_EXPERT },
    { P_WHIP, P_BASIC },
    { P_UNICORN_HORN, P_SKILLED },
    { P_DIVINATION_SPELL, P_BASIC },
    { P_ENCHANTMENT_SPELL, P_BASIC },
    { P_ESCAPE_SPELL, P_SKILLED },
    { P_RIDING, P_BASIC },
    { P_TWO_WEAPON_COMBAT, P_SKILLED },
    { P_BARE_HANDED_COMBAT, P_SKILLED },
    { P_SHIELD, P_BASIC },
    { P_FIREARM, P_BASIC },
    { P_NONE, 0 }
};
static const struct def_skill Skill_U[] = {
    { P_DAGGER, P_EXPERT },         /* Stakes and quest artifact */
    { P_LONG_SWORD, P_BASIC },      /* Buffy */
    { P_SHORT_SWORD, P_BASIC },      /* Buffy */
    { P_BROAD_SWORD, P_SKILLED },    /* Buffy */
    { P_CLUB, P_SKILLED },
    { P_MACE, P_SKILLED },
    { P_MORNING_STAR, P_EXPERT },  /* Castlevania */
    { P_FLAIL, P_SKILLED },
    { P_HAMMER, P_SKILLED },
    { P_POLEARMS, P_SKILLED },
    { P_SPEAR, P_EXPERT },          /* starting weapon/artifact */
    { P_FIREARM, P_EXPERT },        /* Buffy? */
    { P_CROSSBOW, P_EXPERT },       /* Dracula movies */
    { P_SHURIKEN, P_BASIC },        /* Meh? */
    { P_BOOMERANG, P_BASIC },       /* Meh? */
    { P_WHIP, P_EXPERT },           /* Castlevania */
    { P_UNICORN_HORN, P_SKILLED },

    { P_ENCHANTMENT_SPELL, P_SKILLED },
    { P_ESCAPE_SPELL, P_SKILLED },
    { P_MATTER_SPELL, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_MASTER }, /* Buffy the Vampire Slayer */
    { P_TWO_WEAPON_COMBAT, P_BASIC },
    { P_SHIELD, P_EXPERT },
    { P_NONE, 0 }
};
static const struct def_skill Skill_V[] = {
    { P_DAGGER, P_EXPERT },
    { P_AXE, P_EXPERT },
    { P_PICK_AXE, P_SKILLED },
    { P_SHORT_SWORD, P_SKILLED },
    { P_BROAD_SWORD, P_SKILLED },
    { P_LONG_SWORD, P_EXPERT },
    { P_TWO_HANDED_SWORD, P_EXPERT },
    { P_SABER, P_BASIC },
    { P_HAMMER, P_EXPERT },
    { P_QUARTERSTAFF, P_BASIC },
    { P_POLEARMS, P_SKILLED },
    { P_SPEAR, P_EXPERT },
    { P_TRIDENT, P_BASIC },
    { P_LANCE, P_SKILLED },
    { P_SLING, P_BASIC },
    { P_MATTER_SPELL, P_BASIC }, /* special spell is repair armor */
    { P_RIDING, P_SKILLED },
    { P_TWO_WEAPON_COMBAT, P_SKILLED },
    { P_BARE_HANDED_COMBAT, P_EXPERT },
    { P_SHIELD, P_MASTER },
    { P_NONE, 0 }
};
static const struct def_skill Skill_W[] = {
    { P_DAGGER, P_EXPERT },
    { P_KNIFE, P_SKILLED },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_SLING, P_SKILLED },
    { P_DART, P_EXPERT },
    { P_ATTACK_SPELL, P_EXPERT },
    { P_HEALING_SPELL, P_SKILLED },
    { P_DIVINATION_SPELL, P_EXPERT },
    { P_ENCHANTMENT_SPELL, P_EXPERT },
    { P_NECROMANCY_SPELL, P_BASIC },
    { P_ESCAPE_SPELL, P_EXPERT },
    { P_MATTER_SPELL, P_EXPERT },
    { P_RIDING, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_BASIC },
    { P_NONE, 0 }
};

static const struct def_skill Skill_Y[] = {
    { P_DAGGER, P_SKILLED },            
    { P_CLUB, P_BASIC },
    { P_KNIFE, P_BASIC },
    { P_AXE, P_SKILLED },               
    { P_SHORT_SWORD, P_EXPERT },
    { P_BROAD_SWORD, P_BASIC },         
    { P_LONG_SWORD, P_SKILLED },
    { P_SABER, P_SKILLED },
    { P_HAMMER, P_BASIC },
    { P_POLEARMS, P_EXPERT },           
    { P_SPEAR, P_BASIC },
    { P_TRIDENT, P_SKILLED },
    { P_BOW, P_EXPERT },
    { P_SLING, P_BASIC },
    { P_FIREARM, P_SKILLED },
    { P_UNICORN_HORN, P_BASIC },

    { P_ENCHANTMENT_SPELL, P_SKILLED },
    { P_ESCAPE_SPELL, P_BASIC },

    { P_RIDING, P_EXPERT },
    { P_TWO_WEAPON_COMBAT, P_SKILLED }, 
    { P_BARE_HANDED_COMBAT, P_SKILLED },
    { P_SHIELD, P_BASIC },
    { P_NONE, 0 }
};

STATIC_OVL void
knows_object(obj)
register int obj;
{
    discover_object(obj, TRUE, FALSE);
    objects[obj].oc_pre_discovered = 1; /* not a "discovery" */
}

/* Know ordinary (non-magical) objects of a certain class,
   like all gems except the loadstone and luckstone. */
STATIC_OVL void
knows_class(sym)
register char sym;
{
    struct obj *o;
    int ct;

    struct obj odummy = { 0 };
    odummy.oclass = sym;
    o = &odummy; /* for use in various obj.h macros */

    /*
     * Note:  the exceptions here can be bypassed if necessary by
     *        calling knows_object() directly.  So an elven ranger,
     *        for example, knows all elven weapons despite the bow,
     *        arrow, and spear limitation below.
     */

    for (ct = bases[(uchar) sym]; ct < bases[(uchar) sym + 1]; ct++) {
        /* not flagged as magic but shouldn't be pre-discovered */
        if (ct == CORNUTHAUM || ct == DUNCE_CAP || ct == ROBE)
            continue;

        if (sym == WEAPON_CLASS) {
            odummy.otyp = ct; /* update 'o' */
            /* arbitrary: only knights and samurai recognize polearms */
            if ((!Role_if(PM_KNIGHT) && !Role_if(PM_SAMURAI)) && is_pole(o))
                continue;
            /* rangers know all launchers (bows, &c), ammo (arrows, &c),
               and spears regardless of race/species, but not other weapons */
            if (Role_if(PM_RANGER)
                && (!is_launcher(o) && !is_ammo(o) && !is_spear(o)))
                continue;
            /* rogues know daggers, regardless of racial variations */
            if (Role_if(PM_ROGUE) && objects[o->otyp].oc_skill != P_DAGGER)
                continue;
            /* Only convicts know about all firearms */
            if (objects[o->otyp].oc_skill == P_FIREARM 
                && !Role_if(PM_CONVICT))
                continue;
            /* Nobody knows about bombs */
            if (is_bomb(o))
                continue;
        }

        if (objects[ct].oc_class == sym && !objects[ct].oc_magic)
            knows_object(ct);
    }
}

void
u_init()
{
    register int i;
    struct u_roleplay tmpuroleplay = u.uroleplay; /* set by rcfile options */

    flags.female = flags.initgend;
    flags.beginner = 1;

    /* zero u, including pointer values --
     * necessary when aborting from a failed restore */
    (void) memset((genericptr_t) &u, 0, sizeof(u));
    u.ustuck = (struct monst *) 0;
    (void) memset((genericptr_t) &ubirthday, 0, sizeof(ubirthday));
    (void) memset((genericptr_t) &urealtime, 0, sizeof(urealtime));

    u.uroleplay = tmpuroleplay; /* restore options set via rcfile */

#if 0  /* documentation of more zero values as desirable */
    u.usick_cause[0] = 0;
    u.uluck  = u.moreluck = 0;
    uarmu = 0;
    uarm = uarmc = uarmh = uarms = uarmg = uarmf = 0;
    uwep = uball = uchain = uleft = uright = 0;
    uswapwep = uquiver = 0;
    u.twoweap = 0;
    u.ublessed = 0;                     /* not worthy yet */
    u.ugangr   = 0;                     /* gods not angry */
    u.ugifts   = 0;                     /* no divine gifts bestowed */
    u.uevent.uhand_of_elbereth = 0;
    u.uevent.uheard_tune = 0;
    u.uevent.uopened_dbridge = 0;
    u.uevent.udemigod = 0;              /* not a demi-god yet... */
    u.udg_cnt = 0;
    u.mh = u.mhmax = u.mtimedone = 0;
    u.uz.dnum = u.uz0.dnum = 0;
    u.utotype = 0;
#endif /* 0 */

    u.uz.dlevel = 1;
    u.uz0.dlevel = 0;
    u.utolev = u.uz;

    u.umoved = FALSE;
    u.umortality = 0;
    u.ugrave_arise = Role_if(PM_PIRATE) ? PM_SKELETAL_PIRATE : NON_PM;

    u.ukinghill = 0;
    u.protean = 0;
    u.utimestop = FALSE;

    u.umonnum = u.umonster = (flags.female && urole.femalenum != NON_PM)
                                 ? urole.femalenum
                                 : urole.malenum;
    u.ulycn = NON_PM;
    set_uasmon();

    u.ulevel = 0; /* set up some of the initial attributes */
                  
    u.uhp = u.uhpmax = newhp();
    if (Role_if(PM_NECROMANCER)) {
        u.uen = 0;
        u.uenmax = 99;
    } else
        u.uen = u.uenmax = newpw();
    u.uspellprot = 0;
    adjabil(0, 1);
    u.ulevel = u.ulevelmax = 1;

    init_uhunger();
    for (i = 0; i <= MAXSPELL; i++)
        spl_book[i].sp_id = NO_SPELL;
    u.ublesscnt = 300; /* no prayers just yet */
    u.ualignbase[A_CURRENT] = u.ualignbase[A_ORIGINAL] = u.ualign.type =
        aligns[flags.initalign].value;

#if defined(BSD) && !defined(POSIX_TYPES)
    (void) time((long *) &ubirthday);
#else
    (void) time(&ubirthday);
#endif

    /*
     *  For now, everyone starts out with a night vision range of 1 and
     *  their xray range disabled.
     */
    u.nv_range = 1;
    u.xray_range = -1;

    /* Scrolls of identify universally known. */
    knows_object(SCR_IDENTIFY);

    /*** Role-specific initializations ***/
    switch (Role_switch) {
    /* rn2(100) > 50 necessary for some choices because some
     * random number generators are bad enough to seriously
     * skew the results if we use rn2(2)...  --KAA
     */
    case PM_ARCHEOLOGIST:
        switch (rnd(5)) {   
        case 1: Archeologist[A_BOOK].trotyp = SPE_DETECT_FOOD; break;
        case 2: Archeologist[A_BOOK].trotyp = SPE_DETECT_MONSTERS; break;
        case 3: Archeologist[A_BOOK].trotyp = SPE_LIGHT; break;
        case 4: Archeologist[A_BOOK].trotyp = SPE_KNOCK; break;
        case 5: Archeologist[A_BOOK].trotyp = SPE_WIZARD_LOCK; break;
        default: break;
        }
        ini_inv(Archeologist);
        ini_inv(Pickaxe);
        if (!rn2(10))
            (rn2(100) > 50 ? ini_inv(Lamp) : ini_inv(Torch));
        else if (!rn2(2))
            ini_inv(Lamp);
        knows_object(SACK);
        knows_object(TOUCHSTONE);
        skill_init(Skill_A);
        break;
    case PM_BARBARIAN:
        if (rn2(100) >= 50) { /* see above comment */
            Barbarian[B_MAJOR].trotyp = BATTLE_AXE;
            Barbarian[B_MINOR].trotyp = SHORT_SWORD;
        } else if (rn2(100) >= 50) {
            Barbarian[B_MAJOR].trotyp = FALCHION;
            Barbarian[B_MINOR].trotyp = SCIMITAR;
        }
        ini_inv(Barbarian);
        if (!rn2(6))
            ini_inv(Torch);
        knows_class(WEAPON_CLASS); /* excluding polearms */
        knows_class(ARMOR_CLASS);
        skill_init(Skill_B);
        break;
    case PM_CAVEMAN:
        u.nv_range = 2;
        Cave_man[C_AMMO].trquan = rn1(11, 10); /* 10..20 */
        ini_inv(Cave_man);
        skill_init(Skill_C);
        break;
    case PM_CARTOMANCER:
        ini_inv(Cartomancer);
        skill_init(Skill_Car);
        knows_object(PLAYING_CARD_DECK);
        knows_object(DECK_OF_FATE);
        break;
    case PM_FLAME_MAGE:
        switch (rnd(2)) {                
            case 1: 
                Flame_Mage[F_BOOK].trotyp = SPE_DETECT_MONSTERS; 
                break;
            case 2: 
                Flame_Mage[F_BOOK].trotyp = SPE_LIGHT; 
                break;
            default: 
                break;
        }
        ini_inv(Flame_Mage);
        if(!rn2(5)) 
            ini_inv(Lamp);
        else if (!rn2(5)) 
            ini_inv(Blindfold);
        skill_init(Skill_F);
        break;
    case PM_ICE_MAGE:
        switch (rnd(2)) {                
        case 1: 
            Ice_Mage[I_BOOK].trotyp = SPE_CONFUSE_MONSTER; 
            break;
        case 2: 
            Ice_Mage[I_BOOK].trotyp = SPE_SLOW_MONSTER; 
            break;
        default: 
            break;
        }
        ini_inv(Ice_Mage);
        if (!rn2(2)) 
            ini_inv(Goggles);
        skill_init(Skill_I);
        break;
    case PM_JEDI:
        /* Random lightsaber at start */
        switch (rnd(3)) {
        case 1:
            ini_inv(RedSaber); 
            break;
        case 2: 
            ini_inv(BlueSaber); 
            break;
        case 3: 
            ini_inv(GreenSaber); 
            break;
        default: 
            break;
        }
        ini_inv(Jedi);
        if (!rn2(2))
            ini_inv(Blindfold);
        skill_init(Skill_J);
        knows_class(WEAPON_CLASS);
        /*knows_class(ARMOR_CLASS);*/

        /* Instead, Jedi will know their enemy. */
        knows_object(PLASTEEL_HELM);
        knows_object(PLASTEEL_ARMOR);
        knows_object(PLASTEEL_GLOVES);
        knows_object(PLASTEEL_BOOTS);
        knows_object(GREEN_LIGHTSABER);
        knows_object(RED_LIGHTSABER);
        knows_object(BLUE_LIGHTSABER);
        knows_object(RED_DOUBLE_LIGHTSABER);
        break;
    case PM_CONVICT:
        ini_inv(Convict);
        knows_object(SKELETON_KEY);
        knows_object(GRAPPLING_HOOK);
        skill_init(Skill_Con);
        u.uhunger = 200;  /* On the verge of hungry */
        u.ualignbase[A_CURRENT] = u.ualignbase[A_ORIGINAL]
            = u.ualign.type = A_CHAOTIC; /* Override racial alignment */
        knows_class(WEAPON_CLASS); /* firearms only */
        break;
    case PM_HEALER:
        u.umoney0 = rn1(1000, 1001);
        ini_inv(Healer);
        if (!rn2(25))
            ini_inv(Lamp);
        /* Naturally familiar with healing items in their work */
        knows_object(POT_SICKNESS);
        knows_object(POT_PARALYSIS);
        knows_object(POT_SLEEPING);
        knows_object(POT_REGENERATION);
        knows_object(POT_RESTORE_ABILITY);
        knows_object(POT_FULL_HEALING);
        knows_object(POT_BLOOD);
        knows_object(HEALTHSTONE);	/* KMH */
        skill_init(Skill_H);
        break;
    case PM_INFIDEL:
        u.umoney0 = rn1(251, 250);
        ini_inv(Infidel);
        knows_object(SCR_CHARGING);
        skill_init(Skill_Inf);
        break;
    case PM_KNIGHT:
        ini_inv(Knight);
        knows_class(WEAPON_CLASS); /* all weapons */
        knows_class(ARMOR_CLASS);
        /* give knights chess-like mobility--idea from wooledge@..cwru.edu */
        HJumping |= FROMOUTSIDE;
        skill_init(Skill_K);
        break;
    case PM_MONK: {
        static short M_spell[] = { SPE_HEALING, SPE_PROTECTION, SPE_SLEEP };

        Monk[M_BOOK].trotyp = M_spell[rn2(90) / 30]; /* [0..2] */
        ini_inv(Monk);
        if (!rn2(4))
            ini_inv(Lamp);
        knows_class(ARMOR_CLASS);
        /* sufficiently martial-arts oriented item to ignore language issue */
        knows_object(SHURIKEN);
        skill_init(Skill_Mon);
        break;
    }
    case PM_NECROMANCER:
        switch (rnd(5)) {
        case 1: Necromancer[N_BOOK].trotyp = SPE_FORCE_BOLT; break;
        case 2: Necromancer[N_BOOK].trotyp = SPE_KNOCK; break;
        case 3: Necromancer[N_BOOK].trotyp = SPE_MAGIC_MISSILE; break;
        case 4: Necromancer[N_BOOK].trotyp = SPE_CREATE_MONSTER; break;
        case 5: Necromancer[N_BOOK].trotyp = SPE_WIZARD_LOCK; break;
        default: break;
        }
        ini_inv(Necromancer);
        knows_class(SPBOOK_CLASS);
        if (!rn2(5)) 
            ini_inv(Blindfold);
        skill_init(Skill_N);
        break;
    case PM_PIRATE:
        u.umoney0 = rnd(300);
        if (!rn2(4))
            Pirate[PIR_SNACK].trotyp = KELP_FROND;
        Pirate[PIR_SNACK].trquan += rn2(4);
        if (rn2(100) < 50)
            Pirate[PIR_JEWELRY].trotyp = RIN_ADORNMENT;
        if (rn2(100) < 50)
            Pirate[PIR_TOOL].trotyp = GRAPPLING_HOOK;
        Pirate[PIR_BULLETS].trquan += rn2(Pirate[PIR_BULLETS].trquan / 2);
        ini_inv(Pirate);
        
        /* From Fyr/YANI/Pirate */
        if (!rn2(10))
            ini_inv(DeathWand);
        
        knows_object(OILSKIN_SACK);
        knows_object(OILSKIN_CLOAK);
        knows_object(GRAPPLING_HOOK);
        skill_init(Skill_Pir);
        break;
    case PM_PRIEST:
        ini_inv(Priest);
        if (!rn2(4))
            ini_inv(Lamp);
        knows_object(POT_WATER);
        skill_init(Skill_P);
        /* KMH, conduct --
         * Some may claim that this isn't agnostic, since they
         * are literally "priests" and they have holy water.
         * But we don't count it as such.  Purists can always
         * avoid playing priests and/or confirm another player's
         * role in their YAAP.
         */
        break;
    case PM_RANGER:
        Ranger[RAN_TWO_ARROWS].trquan = rn1(10, 50);
        Ranger[RAN_ZERO_ARROWS].trquan = rn1(10, 30);
        ini_inv(Ranger);
        knows_class(WEAPON_CLASS); /* bows, arrows, spears only */
        skill_init(Skill_Ran);
        break;
    case PM_ROGUE:
        Rogue[R_DAGGERS].trquan = rn1(10, 6);
        Rogue[R_DARTS].trquan = rn1(10, 25);
        if (rn2(100) < 30) {
            Rogue[R_DAGGERS].trotyp = PISTOL;
            Rogue[R_DAGGERS].trquan = 1;
            Rogue[R_DARTS].trotyp = BULLET;
        }
        u.umoney0 = 0;
        ini_inv(Rogue);
        if (!rn2(5))
            ini_inv(Blindfold);
        knows_object(OILSKIN_SACK);
        knows_class(WEAPON_CLASS); /* daggers only */
        skill_init(Skill_R);
        break;
    case PM_SAMURAI:
        Samurai[S_ARROWS].trquan = rn1(20, 26);
        ini_inv(Samurai);
        if (!rn2(5))
            ini_inv(Blindfold);
        knows_class(WEAPON_CLASS); /* all weapons */
        knows_class(ARMOR_CLASS);
        skill_init(Skill_S);
        break;
    case PM_TOURIST:
        Tourist[T_DARTS].trquan = rn1(20, 21);
        u.umoney0 = rn1(500, 1000);
        ini_inv(Tourist);
        skill_init(Skill_T);
        break;

    case PM_UNDEAD_SLAYER:
        switch (rn2(100) / 25) {
        case 0:	
            /* Pistol and silver bullets */
            UndeadSlayer[U_MINOR].trotyp = PISTOL;
            UndeadSlayer[U_RANGE].trotyp = BULLET;
            UndeadSlayer[U_RANGE].trquan = rn1(10, 30);
            break;
        case 1:	
            /* Crossbow and bolts */
            UndeadSlayer[U_MINOR].trotyp = CROSSBOW;
            UndeadSlayer[U_RANGE].trotyp = CROSSBOW_BOLT;
            UndeadSlayer[U_RANGE].trquan = rn1(10, 30);
            UndeadSlayer[U_MISC].trspe = 1;
            UndeadSlayer[U_ARMOR].trotyp = JACKET;
            UndeadSlayer[U_ARMOR].trspe = 1;
            break;
        case 2:	
            /* Whip and daggers */
            UndeadSlayer[U_MINOR].trotyp = BULLWHIP;
            UndeadSlayer[U_MINOR].trspe = 2;
            break;
        case 3:	
            /* Silver spear and daggers */
            break;
        }

        /* Vampires get a longsword and a trench coat just like Blade */
        if (Race_if(PM_VAMPIRIC)) {
            UndeadSlayer[U_MAJOR].trotyp = LONG_SWORD;
            UndeadSlayer[U_ARMOR].trspe = 0;
            /* Init first so weapon gets index a */
            ini_inv(UndeadSlayer);
            ini_inv(Lenses);
        } else 
            ini_inv(UndeadSlayer);

        knows_class(WEAPON_CLASS);
        knows_class(ARMOR_CLASS);
        skill_init(Skill_U);

        /* Kludge here to trigger Undead Warning */
        HWarn_of_mon = HUndead_warning;
        context.warntype.intrins |= MH_UNDEAD;
        break;
    case PM_VALKYRIE:
        ini_inv(Valkyrie);
        if (!rn2(6))
            ini_inv(Lamp);
        knows_class(WEAPON_CLASS); /* excludes polearms */
        knows_class(ARMOR_CLASS);
        skill_init(Skill_V);
        break;
    case PM_WIZARD:
        ini_inv(Wizard);
        if (Race_if(PM_GIANT) || Race_if(PM_TORTLE))
            ini_inv(AoMR);
        if (!rn2(5))
            ini_inv(Lamp);
        if (!rn2(5))
            ini_inv(Blindfold);
        skill_init(Skill_W);
        
        break;
    case PM_YEOMAN:
        ini_inv(Yeoman);
        knows_class(WEAPON_CLASS);
        knows_class(ARMOR_CLASS);
        skill_init(Skill_Y);
        break;
    default: /* impossible */
        pline("u_init: unknown role %d", Role_switch);
        break;
    }

    /*** Race-specific initializations ***/
    switch (Race_switch) {
    case PM_HUMAN:
        /* Nothing special */
        break;

    case PM_ELF:
        /*
         * Elves are people of music and song, or they are warriors.
         * Non-warriors get an instrument.  We use a kludge to
         * get only non-magic instruments.
         */
        if (Role_if(PM_PRIEST) || Role_if(PM_WIZARD) || Role_if(PM_CARTOMANCER)) {
            static int trotyp[] = { FLUTE,  TOOLED_HORN,       HARP,
                                    BELL,         BUGLE,       LEATHER_DRUM,
                                    LUTE,         BAGPIPE };
            Instrument[0].trotyp = trotyp[rn2(SIZE(trotyp))];
            ini_inv(Instrument);
        }

        /* Elves can recognize all elvish objects */
        knows_object(ELVEN_SHORT_SWORD);
        knows_object(ELVEN_ARROW);
        knows_object(ELVEN_BOW);
        knows_object(ELVEN_SPEAR);
        knows_object(ELVEN_DAGGER);
        knows_object(ELVEN_BROADSWORD);
        knows_object(ELVEN_LONG_SWORD);
        knows_object(ELVEN_CHAIN_MAIL);
        knows_object(ELVEN_HELM);
        knows_object(ELVEN_SHIELD);
        knows_object(ELVEN_BOOTS);
        knows_object(ELVEN_CLOAK);
        
        /* Elves know Elbereth! */
        u.uevent.ulearned_elbereth = 1;
        break;

    case PM_DWARF:
        /* Dwarves can recognize all dwarvish objects */
        knows_object(DWARVISH_SPEAR);
        knows_object(DWARVISH_SHORT_SWORD);
        knows_object(DWARVISH_MATTOCK);
        knows_object(DWARVISH_HELM);
        knows_object(DWARVISH_CHAIN_MAIL);
        knows_object(DWARVISH_CLOAK);
        knows_object(DWARVISH_BOOTS);
        knows_object(DWARVISH_ROUNDSHIELD);

        if (!Role_if(PM_ARCHEOLOGIST) 
            && !Role_if(PM_CONVICT) 
            && !Role_if(PM_NECROMANCER)) {
            if (!rn2(4)) {
                /* Wise dwarves bring their toy to the dungeons */
                ini_inv(Pickaxe);
            }
        }
        break;

    case PM_GNOME:
        break;

    case PM_ILLITHID:
        /* All illithids start with spell of psionic wave. */
        force_learn_spell(SPE_PSIONIC_WAVE);
        break;

    case PM_TORTLE:
        /* if their role lists trident as a trainable skill,
           raise the max proficiency level by one:
          this is set in weapon.c:skill_init */

        if (!rn2(4)) {
            /* in case they want to go for a swim */
            ini_inv(Oilskin);
        }
        break;

    case PM_GIANT: {
        struct trobj RandomGem = Gem[0];
        while (!rn2(3)) {
            int gem = rnd_class(TOPAZ, JADE);
            Gem[0] = RandomGem;
            Gem[0].trotyp = gem;
            ini_inv(Gem);
            knows_object(gem);
        }

        /* Giants know valuable gems from glass, and may recognize a few types of valuable gem. */
        for (i = FIRST_GEM; i <= LUCKSTONE; i++) {
            if ((objects[i].oc_cost <= 1) || (rn2(100) < 5 + ACURR(A_INT)))
                knows_object(i);
        }
        break;
    }
    case PM_HOBBIT:
        /* Hobbits are always hungry; you'd be hard-pressed to come across one that didn't have
         * something to snack on or at least a means to make more food */
        if (!Role_if(PM_CONVICT))
            ini_inv(Xtra_food);
        if (!Role_if(PM_ARCHEOLOGIST) && !Role_if(PM_CONVICT))
            ini_inv(Tinningkit);

        /* If it relates to food, Hobbits know everything about it */
        knows_object(TIN);
        knows_object(TINNING_KIT);
        knows_object(TIN_OPENER);

        /* Like Elves, Hobbits can recognize all elvish objects */
        knows_object(ELVEN_SHORT_SWORD);
        knows_object(ELVEN_ARROW);
        knows_object(ELVEN_BOW);
        knows_object(ELVEN_SPEAR);
        knows_object(ELVEN_DAGGER);
        knows_object(ELVEN_BROADSWORD);
        knows_object(ELVEN_LONG_SWORD);
        knows_object(ELVEN_CHAIN_MAIL);
        knows_object(ELVEN_HELM);
        knows_object(ELVEN_SHIELD);
        knows_object(ELVEN_BOOTS);
        knows_object(ELVEN_CLOAK);
        
        /* Hobbits know Elbereth! */
        u.uevent.ulearned_elbereth = 1;
        break;

    case PM_CENTAUR:
        /* Centaurs know all bow-based projectile weapons */
        knows_object(ELVEN_ARROW);
        knows_object(ELVEN_BOW);
        knows_object(ORCISH_ARROW);
        knows_object(LIGHT_ARROW);
        knows_object(ORCISH_BOW);
        knows_object(ARROW);
        knows_object(BOW);
        knows_object(YA);
        knows_object(YUMI);
        knows_object(FOOTBOW);
        knows_object(CROSSBOW_BOLT);
        knows_object(CROSSBOW);
        break;

    case PM_ORC:
        /* compensate for generally inferior equipment */
        if (!Role_if(PM_WIZARD) 
              && !Role_if(PM_CONVICT)
              && !Role_if(PM_FLAME_MAGE) 
              && !Role_if(PM_ICE_MAGE)
              && !Role_if(PM_NECROMANCER))
            ini_inv(Xtra_food);
        /* Orcs can recognize all orcish objects */
        knows_object(ORCISH_SHORT_SWORD);
        knows_object(ORCISH_ARROW);
        knows_object(ORCISH_BOW);
        knows_object(ORCISH_SPEAR);
        knows_object(ORCISH_DAGGER);
        knows_object(ORCISH_CHAIN_MAIL);
        knows_object(ORCISH_RING_MAIL);
        knows_object(ORCISH_HELM);
        knows_object(ORCISH_SHIELD);
        knows_object(URUK_HAI_SHIELD);
        knows_object(ORCISH_CLOAK);
        knows_object(ORCISH_SCIMITAR);
        knows_object(ORCISH_BOOTS);
        knows_object(ORCISH_MORNING_STAR);
        break;
        
    case PM_VAMPIRIC:
        /* Vampires start off with gods not as pleased, luck penalty */
        adjalign(-5); 
        change_luck(-1);
        knows_object(POT_VAMPIRE_BLOOD);
        knows_object(POT_BLOOD);

        if (!Role_if(PM_HEALER))
            ini_inv(MedicalKit);
        break;

    case PM_DOPPELGANGER:
        /* Con-Dop gets a cursed amulet of unchanging so they can not easily
         * escape the cursed striped shirt and iron ball by #youpoly/liquid leap */
        if (Role_if(PM_CONVICT)) {
            ini_inv(AoUn);
            knows_object(AMULET_OF_UNCHANGING);
        }

        /* Doppelgangers naturally everything know about polymorph */
        knows_object(POT_POLYMORPH);
        knows_object(WAN_POLYMORPH);
        knows_object(SPE_POLYMORPH);
        knows_object(RIN_POLYMORPH);
        break;
        
    default: /* impossible */
        pline("u_init: unknown race %d", Race_switch);
        break;
    }

    if (discover)
        ini_inv(Wishing);

    if (wizard)
        read_wizkit();

    if (u.umoney0)
        ini_inv(Money);
    u.umoney0 += hidden_gold(); /* in case sack has gold in it */

    /* Ensure that Monks don't start with meat. (Tripe is OK, as it's
     * meant as pet food.)
     * Added here mainly for new Giant race that can play the Monk role
     * and will start with extra food much like an Orc.
     **/
    if (Role_if(PM_MONK)) {
        struct obj *otmp;
        for (otmp = invent; otmp; otmp = otmp->nobj) {
            if ((otmp->otyp == TIN) && (!vegetarian(&mons[otmp->corpsenm]))) {
                if (rn2(2)) {
                    otmp->spe = 1;
                    otmp->corpsenm = NON_PM;
                } else {
                    otmp->corpsenm = PM_LICHEN;
                }
            }
        }
        /* If a dwarven monk starts with a pickaxe, don't start wielding it */
        if (uwep)
            setuwep((struct obj *) 0);
    }

    /* Tortles that start with a trident get one that
       is rustproof (currently only barbarian role) */
    if (Race_if(PM_TORTLE)) {
        struct obj *otmp;
        for (otmp = invent; otmp; otmp = otmp->nobj) {
            if (otmp->otyp == TRIDENT) {
                otmp->oerodeproof = 1;
                otmp->rknown = 1;
            }
        }
    }

    find_ac();     /* get initial ac value */
    init_attr(75); /* init attribute values */
    max_rank_sz(); /* set max str size for class ranks */
    /*
     *  Do we really need this?
     */
    for (i = 0; i < A_MAX; i++)
        if (!rn2(20)) {
            register int xd = rn2(7) - 2; /* biased variation */

            (void) adjattrib(i, xd, TRUE);
            if (ABASE(i) < AMAX(i))
                AMAX(i) = ABASE(i);
        }

    /* make sure you can carry all you have - especially for Tourists */
    while (inv_weight() > 0) {
        if (adjattrib(A_STR, 1, TRUE))
            continue;
        if (adjattrib(A_CON, 1, TRUE))
            continue;
        /* only get here when didn't boost strength or constitution */
        break;
    }

    /* For cavemen, extinct monsters are generated. */
    if (Role_if(PM_CAVEMAN) || Role_if(PM_CAVEWOMAN)) {
        mons[PM_VELOCIRAPTOR].geno &= ~(G_NOGEN);
        mons[PM_T_REX].geno &= ~(G_NOGEN);
        mons[PM_COMPSOGNATHUS].geno &= ~(G_NOGEN);
    }
    
    /* If we have at least one spell, force starting Pw to be 5,
       so hero can cast the level 1 spell they should have */
    if (num_spells() && (u.uenmax < 5))
        u.uen = u.uenmax = u.ueninc[u.ulevel] = 5;

    /* Quality-of-Life */
    knows_object(POT_WATER);
    knows_object(SCR_BLANK_PAPER);
    return;
}

/* attack/damage structs for shambler_init() */
int attk_melee_types [] =
    { AT_CLAW, AT_BITE, AT_TUCH, AT_STNG, AT_WEAP };

int attk_spec_types [] =
    { AT_HUGS, AT_SPIT, AT_ENGL, AT_BREA, AT_GAZE,
      AT_MAGC, AT_KICK, AT_BUTT, AT_TENT, AT_SCRE
    };

int damg_melee_types [] =
    { AD_PHYS, AD_FIRE, AD_COLD, AD_SLEE, AD_ELEC,
      AD_ELEC, AD_DRST, AD_ACID, AD_STUN, AD_SLOW,
      AD_PLYS, AD_DRLI, AD_DREN, AD_LEGS, AD_STCK,
      AD_SGLD, AD_SITM, AD_SEDU, AD_TLPT, AD_RUST,
      AD_CONF, AD_DRDX, AD_DRCO, AD_DRIN, AD_DISE,
      AD_DCAY, AD_HALU, AD_ENCH, AD_CORR, AD_BHED,
      AD_POLY, AD_WTHR, AD_WEBS, AD_LOUD, AD_CALM,
      AD_TCKL, AD_LARV /*AD_PITS,*/
    };

int damg_breath_types [] =
    { AD_MAGM, AD_FIRE, AD_COLD, AD_SLEE, AD_ELEC,
      AD_DRST, AD_ACID, AD_LOUD, AD_WATR, AD_DRLI,
	  AD_STUN
	};

int damg_spit_types [] =
    { AD_BLND, AD_ACID, AD_DRST };

int damg_gaze_types [] =
    { AD_FIRE, AD_COLD, AD_SLEE, AD_STUN, AD_SLOW,
      AD_CNCL, AD_DRST, AD_CONF, AD_PLYS, AD_DISN,
      AD_DRLI, AD_LUCK, AD_BLND, AD_TLPT, AD_FEAR,
      AD_SPOR, AD_HNGY
    };

int damg_engulf_types [] =
    { AD_PLYS, AD_DGST, AD_WRAP, AD_POTN, AD_DCAY,
      AD_ACID, AD_DISE, AD_PHYS, AD_BLND, AD_COLD,
      AD_ELEC, AD_DREN, AD_FIRE, AD_DISN, AD_SLIM};

int damg_magic_types [] =
    { AD_SPEL, AD_CLRC, AD_MAGM, AD_FIRE, AD_COLD,
      AD_ACID
    };

int damg_kick_types [] =
    { AD_PHYS, AD_STUN, AD_LEGS, AD_ENCH, AD_CLOB };

int damg_butt_types [] =
    { AD_PHYS, AD_STUN, AD_CONF, AD_CLOB };

int damg_tent_types [] =
    { AD_PHYS, AD_DRST, AD_ACID, AD_STUN, AD_PLYS,
      AD_DRLI, AD_DREN, AD_CONF, AD_DRIN, AD_DISE,
      AD_HALU
    };

int damg_scream_types [] =
    { AD_FEAR, AD_SONG, AD_PIER, AD_LOUD, AD_GIBB };

void
shambler_init()
{
    register int i;
    struct permonst* shambler = &mons[PM_SHAMBLING_HORROR];
    struct attack* attkptr;
    int shambler_attacks;

    /* what a horrible night to have a curse */
    shambler->mlevel += rnd(15) - 3;    /* shuffle level */
    shambler->mmove = rn2(10) + 9;      /* slow to very fast */
    shambler->ac = rn2(31) - 20;        /* any AC */
    shambler->mr = rn2(5) * 25;         /* varying amounts of MR */
    shambler->maligntyp = rn2(21) - 10; /* varying alignment */

    shambler_attacks = rnd(4);
    for (i = 0; i < shambler_attacks; i++) {
        attkptr = &shambler->mattk[i];
        attkptr->aatyp = attk_melee_types[rn2(SIZE(attk_melee_types))];
        attkptr->adtyp = damg_melee_types[rn2(SIZE(damg_melee_types))];
        attkptr->damn = 2 + rn2(5);
        attkptr->damd = 3 + rn2(6);
    }

    shambler_attacks = shambler_attacks + (rnd(9) / 3) - 1;
    for (; i < shambler_attacks; i++) {
        attkptr = &shambler->mattk[i];
        attkptr->aatyp = attk_spec_types[rn2(SIZE(attk_spec_types))];
        attkptr->damn = 2 + rn2(4);
        attkptr->damd = 6 + rn2(3);
        switch (attkptr->aatyp) {
        case AT_BREA:
            attkptr->adtyp = damg_breath_types[rn2(SIZE(damg_breath_types))];
            break;
        case AT_SPIT:
            attkptr->adtyp = damg_spit_types[rn2(SIZE(damg_spit_types))];
            break;
        case AT_GAZE:
            attkptr->adtyp = damg_gaze_types[rn2(SIZE(damg_gaze_types))];
            break;
        case AT_ENGL:
            attkptr->adtyp = damg_engulf_types[rn2(SIZE(damg_engulf_types))];
            break;
        case AT_MAGC:
            attkptr->adtyp = damg_magic_types[rn2(SIZE(damg_magic_types))];
            break;
        case AT_KICK:
            attkptr->adtyp = damg_kick_types[rn2(SIZE(damg_kick_types))];
            break;
        case AT_BUTT:
            attkptr->adtyp = damg_butt_types[rn2(SIZE(damg_butt_types))];
            break;
        case AT_TENT:
            attkptr->adtyp = damg_tent_types[rn2(SIZE(damg_tent_types))];
            break;
        case AT_SCRE:
            attkptr->adtyp = damg_scream_types[rn2(SIZE(damg_scream_types))];
            break;
        case AT_HUGS:
            attkptr->adtyp = AD_PHYS;
            break;
        default:
            attkptr->adtyp = AD_PHYS;
            break;
        }
    }

    shambler->msize = !rn2(6) ? MZ_GIGANTIC
                              : rn2(MZ_HUGE);    /* any size */
    shambler->cwt = 20;                          /* fortunately moot as it's flagged NOCORPSE */
    shambler->cnutrit = 20;                      /* see above */
    shambler->msound = rn2(MS_HUMANOID);         /* any but the specials */
    shambler->mresists = 0;

    for (i = 0; i < rnd(6); i++)
        shambler->mresists |= (1 << rn2(9));     /* physical resistances... */
    for (i = 0; i < rnd(5); i++)
        shambler->mresists |= (0x200 << rn2(9)); /* 'different' resistances, even clumsy */
    shambler->mconveys = 0;                      /* flagged NOCORPSE */

    /*
     * now time for the random flags.  this will likely produce
     * a number of complete trainwreck monsters at first, but
     * every so often something will dial up nasty stuff
     */
    shambler->mflags1 = 0;
    for (i = 0; i < rnd(17); i++)
        shambler->mflags1 |= (1 << rn2(33));    /* rn2() should equal the number of M1_ flags in
                                                 * include/monflag.h */
    shambler->mflags1 &= ~M1_UNSOLID;           /* no ghosts */
    shambler->mflags1 &= ~M1_WALLWALK;          /* no wall-walkers */
    shambler->mflags1 &= ~M1_CONCEAL;           /* no hiders */
    shambler->mflags1 &= ~M1_ACID;              /* will never leave a corpse */
    shambler->mflags1 &= ~M1_POIS;              /* same as above */

    shambler->mflags2 = M2_NOPOLY | M2_HOSTILE; /* Don't let the player be one of these yet. */
    for (i = 0; i < rnd(17); i++)
        shambler->mflags2 |= (1 << rn2(22));    /* rn2() should equal the number of M2_ flags in
                                                 * include/monflag.h */
    shambler->mflags2 &= ~M2_MERC;              /* no guards */
    shambler->mflags2 &= ~M2_PEACEFUL;          /* no peacefuls */
    shambler->mflags2 &= ~M2_PNAME;             /* not a proper name */
    shambler->mflags2 &= ~M2_SHAPESHIFTER;      /* no chameleon types */
    shambler->mflags2 &= ~M2_LORD;              /* isn't royalty */
    shambler->mflags2 &= ~M2_PRINCE;            /* still isn't royalty */
    shambler->mflags2 &= ~M2_DOMESTIC;          /* no taming */

    shambler->mflags3 = 0;
    for (i = 0; i < rnd(5); i++)
        shambler->mflags3 |= (1 << rn2(16));    /* rn2() should equal the number of M3_ flags in
                                                 * include/monflag.h */
    shambler->mflags3 &= ~M3_WANTSALL;
    shambler->mflags3 &= ~M3_COVETOUS;          /* no covetous behavior */
    shambler->mflags3 &= ~M3_WAITMASK;          /* no waiting either */

    return;
}

/* Initialize random elements of dragons. */
void
dragon_init()
{
    /* Which dragons will have the frightful roar? */
    struct permonst* dragon;
    int i;
    for (i = PM_GRAY_DRAGON; i <= PM_YELLOW_DRAGON; i++) {
        if (!rn2(3)) {
            dragon = &mons[i];
            dragon->msound = MS_MEGAROAR;
        }
    }
        
}
/* skills aren't initialized, so we use the role-specific skill lists */
STATIC_OVL boolean
restricted_spell_discipline(otyp)
int otyp;
{
    const struct def_skill *skills;
    int this_skill = spell_skilltype(otyp);

    switch (Role_switch) {
    case PM_ARCHEOLOGIST:
        skills = Skill_A;
        break;
    case PM_BARBARIAN:
        skills = Skill_B;
        break;
    case PM_CAVEMAN:
        skills = Skill_C;
        break;
    case PM_CARTOMANCER:
        skills = Skill_Car;
        break;
    case PM_CONVICT:
        skills = Skill_Con;
        break;
    case PM_FLAME_MAGE:
        skills = Skill_F;
        break;
    case PM_HEALER:
        skills = Skill_H;
        break;
    case PM_INFIDEL:
        skills = Skill_Inf;
        break;
    case PM_JEDI:
        skills = Skill_J;
        break;
    case PM_KNIGHT:
        skills = Skill_K;
        break;
    case PM_MONK:
        skills = Skill_Mon;
        break;
    case PM_PRIEST:
        skills = Skill_P;
        break;
    case PM_RANGER:
        skills = Skill_Ran;
        break;
    case PM_ROGUE:
        skills = Skill_R;
        break;
    case PM_SAMURAI:
        skills = Skill_S;
        break;
    case PM_TOURIST:
        skills = Skill_T;
        break;
    case PM_UNDEAD_SLAYER:
        skills = Skill_U;
        break;
    case PM_VALKYRIE:
        skills = Skill_V;
        break;
    case PM_WIZARD:
        skills = Skill_W;
        break;
    case PM_YEOMAN:
        skills = Skill_Y;
        break;
    default:
        skills = 0; /* lint suppression */
        break;
    }

    while (skills && skills->skill != P_NONE) {
        if (skills->skill == this_skill)
            return FALSE;
        ++skills;
    }
    return TRUE;
}

STATIC_OVL void
ini_inv(origtrop)
register struct trobj *origtrop;
{
    struct obj *obj;
    int otyp, i;
    struct trobj temptrop;
    register struct trobj *trop = &temptrop;
    boolean got_sp1 = FALSE; /* got a level 1 spellbook? */
    memcpy(&temptrop, origtrop, sizeof(struct trobj));

    while (origtrop->trclass) {
        otyp = (int) trop->trotyp;
        if (otyp != UNDEF_TYP) {
            obj = mksobj(otyp, TRUE, FALSE);
        } else { /* UNDEF_TYP */
            static NEARDATA short nocreate = STRANGE_OBJECT;
            static NEARDATA short nocreate2 = STRANGE_OBJECT;
            static NEARDATA short nocreate3 = STRANGE_OBJECT;
            static NEARDATA short nocreate4 = STRANGE_OBJECT;
            /*
             * For random objects, do not create certain overly powerful
             * items: wand of wishing, ring of levitation, or the
             * polymorph/polymorph control combination.  Specific objects,
             * i.e. the discovery wishing, are still OK.
             * Also, don't get a couple of really useless items.  (Note:
             * punishment isn't "useless".  Some players who start out with
             * one will immediately read it and use the iron ball as a
             * weapon.)
             */
            obj = mkobj(trop->trclass, FALSE);
            otyp = obj->otyp;
            while (otyp == WAN_WISHING || otyp == nocreate
                   || otyp == nocreate2 || otyp == nocreate3
                   || otyp == nocreate4 || otyp == RIN_LEVITATION
                   /* 'useless' items */
                   || otyp == POT_HALLUCINATION
                   || otyp == POT_ACID
                   || otyp == SCR_AMNESIA
                   || otyp == SCR_FLOOD
                   || otyp == SCR_FIRE
                   || otyp == SCR_ICE
                   || otyp == SCR_BLANK_PAPER
                   || otyp == SPE_BLANK_PAPER
                   || otyp == RIN_AGGRAVATE_MONSTER
                   || otyp == RIN_HUNGER
                   || otyp == RIN_SLEEPING
                   || otyp == WAN_NOTHING
                   /* Elemental mage stuff */
                   || ((Role_if(PM_FLAME_MAGE) || Role_if(PM_ICE_MAGE))
                       && (otyp == RIN_FIRE_RESISTANCE 
                           || otyp == RIN_COLD_RESISTANCE))

                   /* Necromancers/vampirics start with drain res */
                   || (otyp == AMULET_OF_DRAIN_RESISTANCE
                        && (Role_if(PM_NECROMANCER) || Race_if(PM_VAMPIRIC)))
                   /* vampirics start with regeneration */
                   || (otyp == RIN_REGENERATION && Race_if(PM_VAMPIRIC))
                   /* orcs start with poison resistance */
                   || (otyp == RIN_POISON_RESISTANCE 
                        && (Race_if(PM_ORC) || Race_if(PM_VAMPIRIC)))
                   /* Monks don't use weapons */
                   || (otyp == SCR_ENCHANT_WEAPON && Role_if(PM_MONK))
                   /* wizard patch -- they already have one */
                   || (otyp == SPE_FORCE_BOLT && Role_if(PM_WIZARD))
                   /* same for infidels */
                   || (otyp == SPE_DRAIN_LIFE && Role_if(PM_INFIDEL))
                   /* infidels already have auto-clairvoyance
                      by having the Amulet of Yendor in starting
                      inventory */
                   || (otyp == SPE_CLAIRVOYANCE && Role_if(PM_INFIDEL))
                   /* 5lo -- Doppelgangers shouldn't get ring of poly control. */
                   || (Race_if(PM_DOPPELGANGER) && otyp == RIN_POLYMORPH_CONTROL)
                   /* powerful spells are either useless to
                      low level players or unbalancing; also
                      spells in restricted skill categories */
                   || (obj->oclass == SPBOOK_CLASS
                       && (objects[otyp].oc_level > (got_sp1 ? 3 : 1)
                           || restricted_spell_discipline(otyp)))
                   || otyp == SPE_NOVEL
                   /* items that will be silver for vampirics (rings/wands perhaps)
                    * that can't become iron */
                   || (Race_if(PM_VAMPIRIC) && objects[otyp].oc_material == SILVER
                       && !valid_obj_material(obj, IRON))
                   /* items that will be iron for elves (rings/wands perhaps)
                    * that can't become copper */
                   || (Race_if(PM_ELF) && objects[otyp].oc_material == IRON
                       && !valid_obj_material(obj, COPPER))
                   /* items that will be mithril for orcs (rings/wands perhaps)
                    * that can't become iron */
                   || (Race_if(PM_ORC) && objects[otyp].oc_material == MITHRIL
                       && !valid_obj_material(obj, IRON))) {
                dealloc_obj(obj);
                obj = mkobj(trop->trclass, FALSE);
                otyp = obj->otyp;
            }

            /* Heavily relies on the fact that 1) we create wands
             * before rings, 2) that we create rings before
             * spellbooks, and that 3) not more than 1 object of a
             * particular symbol is to be prohibited.  (For more
             * objects, we need more nocreate variables...)
             */
            switch (otyp) {
            case WAN_POLYMORPH:
            case RIN_POLYMORPH:
            case POT_POLYMORPH:
                nocreate = RIN_POLYMORPH_CONTROL;
                break;
            case RIN_POLYMORPH_CONTROL:
                nocreate = RIN_POLYMORPH;
                nocreate2 = SPE_POLYMORPH;
                nocreate3 = POT_POLYMORPH;
            }
            /* Don't have 2 of the same ring or spellbook */
            if (obj->oclass == RING_CLASS || obj->oclass == SPBOOK_CLASS)
                nocreate4 = otyp;
            /* First spellbook should be level 1 - did we get it? */
            if (obj->oclass == SPBOOK_CLASS && objects[obj->otyp].oc_level == 1)
                got_sp1 = TRUE;
        }

        /* Put post-creation object adjustments that don't depend on whether it
         * was UNDEF_TYP or not after this */

        /* Don't start with +0 or negative rings */
        if (objects[otyp].oc_charged && obj->spe <= 0)
            obj->spe = rne(3);
        
        /* start with maxed lightsaber. 
         * only Jedi start with one, so no need to check Role_if here */
        if (is_lightsaber(obj))
            obj->age = 1500;
        
        /* Don't allow materials to be start scummed for */
        set_material(obj, objects[otyp].oc_material);

        /* Replace iron objects (e.g. Priest's mace) with copper for elves */
        if (Race_if(PM_ELF) && obj->material == IRON
            && valid_obj_material(obj, COPPER))
            set_material(obj, COPPER);

        /* Do the same for orcs and mithril objects */
        if (Race_if(PM_ORC) && obj->material == MITHRIL
            && valid_obj_material(obj, IRON))
            set_material(obj, IRON);

        /* Do the same for vampires and silver objects */
        if (Race_if(PM_VAMPIRE) && obj->material == SILVER
            && valid_obj_material(obj, BONE))
            set_material(obj, BONE);
        
        /* Holy wafers do damage to chaotic players */
        if (u.ualign.type == A_CHAOTIC && obj->otyp == HOLY_WAFER)
            obj->otyp = SPRIG_OF_WOLFSBANE;

        if (urace.malenum != PM_HUMAN) {
            /* substitute race-specific items; this used to be in
               the 'if (otyp != UNDEF_TYP) { }' block above, but then
               substitutions didn't occur for randomly generated items
               (particularly food) which have racial substitutes */
            for (i = 0; inv_subs[i].race_pm != NON_PM; ++i)
                if (inv_subs[i].race_pm == urace.malenum
                    && otyp == inv_subs[i].item_otyp) {
                    debugpline3("ini_inv: substituting %s for %s%s",
                                OBJ_NAME(objects[inv_subs[i].subs_otyp]),
                                (trop->trotyp == UNDEF_TYP) ? "random " : "",
                                OBJ_NAME(objects[otyp]));
                    otyp = obj->otyp = inv_subs[i].subs_otyp;
                    obj->oclass = objects[otyp].oc_class;

                    /* This might have created a bad material combination, such
                     * as a dagger (which was forced to be iron earlier) turning
                     * into an elven dagger, but now remaining iron. Fix this up
                     * here as well. */
                    obj->material = objects[otyp].oc_material;
                    break;
                }
        }

        /* Set up cartomancer cards */
        if (urole.malenum == PM_CARTOMANCER 
              && obj->otyp == SCR_CREATE_MONSTER) {
            do {
                i = rn2(NUMMONS);
            } while ((type_is_pname(&mons[i])
                || (mons[i].geno & G_UNIQ) 
                || (mons[i].geno & G_NOGEN)));
            obj->corpsenm = i;
            if (rn2(2))
                bless(obj);
        }

        /* nudist gets no armor */
        if (u.uroleplay.nudist && obj->oclass == ARMOR_CLASS) {
            dealloc_obj(obj);
            origtrop++;
            memcpy(&temptrop, origtrop, sizeof(struct trobj));
            continue;
        }

        if (trop->trclass == COIN_CLASS) {
            /* no "blessed" or "identified" money */
            obj->quan = u.umoney0;
        } else {
            if (objects[otyp].oc_uses_known)
                obj->known = 1;
            obj->dknown = obj->bknown = obj->rknown = 1;
            if (Is_container(obj) || obj->otyp == STATUE) {
                obj->cknown = obj->lknown = 1;
                obj->otrapped = 0;
            }
            obj->cursed = (trop->trbless == CURSED);
            if (obj->opoisoned && u.ualign.type > A_CHAOTIC)
                obj->opoisoned = 0;
            if (obj->oclass == WEAPON_CLASS || obj->oclass == TOOL_CLASS) {
                obj->quan = (long) trop->trquan;
                trop->trquan = 1;
            } else if (obj->oclass == GEM_CLASS && is_graystone(obj)
                       && obj->otyp != FLINT) {
                obj->quan = 1L;
            }
            if (Role_if(PM_INFIDEL) && obj->oclass == ARMOR_CLASS) {
                /* Infidels are used to playing with fire */
                obj->oerodeproof = 1;
            }
            if (Role_if(PM_FLAME_MAGE)) {
                    /* Flame mages are also used to playing with fire */
                if (obj->oclass == ARMOR_CLASS || obj->otyp == QUARTERSTAFF)
                    obj->oerodeproof = 1;
            }
            
            /* Undead Slayers get special silver weapons.
             * Before the object materials patch this was easy, but 
             * looks like we'll just do it here. */
            if (Role_if(PM_UNDEAD_SLAYER)) {
                if (is_spear(obj)
                      || obj->otyp == DAGGER 
                      || obj->otyp == ELVEN_DAGGER
                      || obj->otyp == TRIDENT
                      || obj->otyp == SLING_BULLET
                      || (is_ammo(obj) && obj->oclass != GEM_CLASS))
                    set_material(obj, SILVER);
                
                if (obj->otyp == JACKET) 
                    set_material(obj, LEATHER);

                if (Race_if(PM_VAMPIRIC) && obj->otyp && obj->otyp == LONG_SWORD)
                    set_material(obj, METAL);
            }
            if (obj->otyp == STRIPED_SHIRT)
                obj->cursed = TRUE;
            if (trop->trspe != UNDEF_SPE)
                obj->spe = trop->trspe;
            if (trop->trbless != UNDEF_BLESS)
                obj->blessed = (trop->trbless == 1);
        }
        /* defined after setting otyp+quan + blessedness */
        obj->owt = weight(obj);
        obj = addinv(obj);

        /* Make the type known if necessary */
        if (OBJ_DESCR(objects[otyp]) && obj->known)
            discover_object(otyp, TRUE, FALSE);
        if (otyp == OIL_LAMP)
            discover_object(POT_OIL, TRUE, FALSE);

        if (obj->oclass == ARMOR_CLASS) {
            /* Validity check here */
            
            if (is_shield(obj) && !uarms && !(uwep && bimanual(uwep))) {
                setworn(obj, W_ARMS);
                /* Prior to 3.6.2 this used to unset uswapwep if it was set, but
                   wearing a shield doesn't prevent having an alternate
                   weapon ready to swap with the primary; just make sure we
                   aren't two-weaponing (academic; no one starts that way) */
                u.twoweap = FALSE;
            } else if (is_helmet(obj) && !uarmh)
                setworn(obj, W_ARMH);
            else if (is_gloves(obj) && !uarmg)
                setworn(obj, W_ARMG);
            else if (is_shirt(obj) && !uarmu)
                setworn(obj, W_ARMU);
            else if (is_cloak(obj) && !uarmc)
                setworn(obj, W_ARMC);
            else if (is_boots(obj) && !uarmf)
                setworn(obj, W_ARMF);
            else if (is_suit(obj) && !uarm)
                setworn(obj, W_ARM);
        }

        if (obj->oclass == WEAPON_CLASS || is_weptool(obj)
            || otyp == TIN_OPENER || otyp == FLINT
            || otyp == ROCK || otyp == SLING_BULLET) {
            if (is_ammo(obj) || is_missile(obj)) {
                if (!uquiver)
                    setuqwep(obj);
            } else if (!uwep && (!uarms || !bimanual(obj))) {
                setuwep(obj);
            } else if (!uswapwep) {
                setuswapwep(obj);
            }
        }
        if (obj->oclass == SPBOOK_CLASS && obj->otyp != SPE_BLANK_PAPER)
            initialspell(obj);
        if (obj->oclass == AMULET_CLASS)
            setworn(obj, W_AMUL);

        if (obj->oclass == TOOL_CLASS && !ublindf
            && (obj->otyp == LENSES || obj->otyp == GOGGLES))
            setworn(obj, W_TOOL);
        

        if (Role_if(PM_UNDEAD_SLAYER) && Race_if(PM_VAMPIRIC) 
                && is_suit(obj)) {
            obj->oprops |= ITEM_FIRE;
            obj->oprops_known |= ITEM_FIRE;
        } else if (Role_if(PM_CARTOMANCER) 
                && obj->otyp == HAWAIIAN_SHIRT && !rn2(2)) {
            obj->oprops |= ITEM_STENCH;
            obj->oprops_known |= ITEM_STENCH;
        }
        /* Object properties cannot be start scummed for. */
        else
            obj->oprops = obj->oprops_known = 0L;

#if !defined(PYRAMID_BUG) && !defined(MAC)
        if (--trop->trquan)
            continue; /* make a similar object */
#else
        if (trop->trquan) { /* check if zero first */
            --trop->trquan;
            if (trop->trquan)
                continue; /* make a similar object */
        }
#endif
        origtrop++;
        memcpy(&temptrop, origtrop, sizeof(struct trobj));
    }
}

/*u_init.c*/
