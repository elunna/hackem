/* NetHack 3.6	role.c	$NHDT-Date: 1578947634 2020/01/13 20:33:54 $  $NHDT-Branch: NetHack-3.7 $:$NHDT-Revision: 1.68 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985-1999. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"

/*** Table of all roles ***/
/* According to AD&D, HD for some classes (ex. Wizard) should be smaller
 * (4-sided for wizards).  But this is not AD&D, and using the AD&D
 * rule here produces an unplayable character.  Thus I have used a minimum
 * of an 10-sided hit die for everything.  Another AD&D change: wizards get
 * a minimum strength of 4 since without one you can't teleport or cast
 * spells. --KAA
 *
 * As the wizard has been updated (wizard patch 5 jun '96) their HD can be
 * brought closer into line with AD&D. This forces wizards to use magic more
 * and distance themselves from their attackers. --LSZ
 *
 * With the introduction of races, some hit points and energy
 * has been reallocated for each race.  The values assigned
 * to the roles has been reduced by the amount allocated to
 * humans.  --KMH
 *
 * God names use a leading underscore to flag goddesses.
 */
const struct Role roles[] = {
    { { "Archeologist", 0 },
      { { "Digger", 0 },
        { "Field Worker", 0 },
        { "Investigator", 0 },
        { "Exhumer", 0 },
        { "Excavator", 0 },
        { "Spelunker", 0 },
        { "Speleologist", 0 },
        { "Collector", 0 },
        { "Curator", 0 } },
      "Quetzalcoatl", "Camaxtli", "Huhetotl", /* Central American */
      "Arc",
      "the College of Archeology",
      "the Tomb of the Toltec Kings",
      PM_ARCHEOLOGIST,
      NON_PM,
      NON_PM,
      PM_LORD_CARNARVON,
      PM_STUDENT,
      PM_MINION_OF_HUHETOTL,
      NON_PM,
      PM_HUMAN_MUMMY,
      S_SNAKE,
      S_MUMMY,
      ART_ITLACHIAYAQUE,
      MH_HUMAN | MH_DWARF | MH_GNOME  | MH_HOBBIT | MH_TORTLE | MH_VAMPIRE,
      ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL,
      /* Str Int Wis Dex Con Cha */
      { 7, 10, 10, 7, 7, 7 },
      { 20, 20, 20, 10, 20, 10 },
      /* Init   Lower  Higher */
      { 11, 0, 0, 8, 1, 0 }, /* Hit points */
      { 3, 0, 0, 1, 0, 1 },
      14, /* Energy */
      10, 5, 0, 2, 10, A_INT, SPE_MAGIC_MAPPING,
      -4 },
    { { "Barbarian", 0 },
      { { "Plunderer", "Plunderess" },
        { "Pillager", 0 },
        { "Bandit", 0 },
        { "Brigand", 0 },
        { "Raider", 0 },
        { "Reaver", 0 },
        { "Slayer", 0 },
        { "Chieftain", "Chieftainess" },
        { "Conqueror", "Conqueress" } },
      "Mitra", "Crom", "Set", /* Hyborian */
      "Bar",
      "the Camp of the Duali Tribe",
      "the Duali Oasis",
      PM_BARBARIAN,
      NON_PM,
      NON_PM,
      PM_PELIAS,
      PM_CHIEFTAIN,
      PM_THOTH_AMON,
      PM_OGRE,
      PM_TROLL,
      S_OGRE,
      S_TROLL,
      ART_RING_OF_P_HUL,
      MH_HUMAN | MH_CENTAUR | MH_DWARF | MH_GIANT | MH_ORC
          | MH_TORTLE | MH_VAMPIRE,
      ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      { 16, 7, 7, 15, 16, 6 },
      { 30, 6, 7, 20, 30, 7 },
      /* Init   Lower  Higher */
      { 14, 0, 0, 10, 2, 0 }, /* Hit points */
      { 1, 0, 0, 1, 0, 1 },
      10, /* Energy */
      10, 14, 0, 0, 8, A_INT, SPE_CAUSE_FEAR, -4 },
    { { "Caveman", "Cavewoman" },
      { { "Troglodyte", 0 },
        { "Aborigine", 0 },
        { "Wanderer", 0 },
        { "Vagrant", 0 },
        { "Wayfarer", 0 },
        { "Roamer", 0 },
        { "Nomad", 0 },
        { "Rover", 0 },
        { "Pioneer", 0 } },
      "Anu", "_Ishtar", "Anshar", /* Babylonian */
      "Cav", "the Caves of the Ancestors", "Annam's Lair",
      PM_CAVEMAN,
      PM_CAVEWOMAN,
      PM_WOLF,
      PM_SHAMAN_KARNOV,
      PM_NEANDERTHAL,
      PM_ANNAM,
      PM_BUGBEAR,
      PM_HILL_GIANT,
      S_HUMANOID,
      S_GIANT,
      ART_SCEPTRE_OF_MIGHT,
      MH_HUMAN | MH_DWARF | MH_GNOME | MH_GIANT,
      ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL,
      /* Str Int Wis Dex Con Cha */
      { 10, 7, 7, 7, 8, 6 },
      { 30, 6, 7, 20, 30, 7 },
      /* Init   Lower  Higher */
      { 14, 0, 0, 8, 2, 0 }, /* Hit points */
      { 1, 0, 0, 1, 0, 1 }, 10, /* Energy */
      0, 12, 0, 1, 8, A_INT, 0, -4 },
    { { "Convict", 0 },
      { { "Detainee", 0 },
        { "Inmate", 0 },
        { "Jail-bird", 0 },
        { "Prisoner", 0 },
        { "Outlaw", 0 },
        { "Crook", 0 },
        { "Desperado", 0 },
        { "Felon", 0 },
        { "Fugitive", 0 } },
      "Ilmater", "Grumbar", "_Tymora",	/* Faerunian */
      "Con",
      "Castle Waterdeep Dungeon",
      "the Warden's Level",
      PM_CONVICT,
      NON_PM,
      PM_SEWER_RAT,
      PM_ROBERT_THE_LIFER,
      PM_INMATE,
      PM_WARDEN_ARIANNA,
      PM_GIANT_BEETLE,
      PM_SOLDIER_ANT,
      S_RODENT,
      S_SPIDER,
      ART_IRON_SPOON_OF_LIBERATION,
      MH_HUMAN | MH_CENTAUR | MH_DWARF | MH_GIANT | MH_GNOME | MH_HOBBIT
          | MH_ILLITHID | MH_ORC | MH_SHIFTER | MH_VAMPIRE,
      ROLE_MALE | ROLE_FEMALE | ROLE_CHAOTIC | ROLE_NORACEALIGN,
      /* Str Int Wis Dex Con Cha */
      { 10, 7, 7, 7, 13, 6 },
      { 20, 20, 10, 20, 20, 10 },
      /* Init   Lower  Higher */
      { 8, 0, 0, 8, 0, 0 },	/* Hit points */
      { 1, 0, 0, 1, 0, 1 },
      10, /* Energy */
      -10, 5, 0, 2, 10, A_INT, SPE_TELEPORT_AWAY, -4 },
    { { "Flame Mage", 0 }, 
      { { "Spark",         0 },   /* WAC was Igniter */
        { "Igniter",       0 },
        { "Broiler",       0 },   /* WAC was Igniter */
        { "Combuster",     0 },   /* WAC was Torcher */
        { "Torcher",       0 },
        { "Scorcher",      0 },   /* WAC was Torcher */
        { "Incinerator",   0 },
        { "Disintegrator", 0 },   /* WAC was Incinerator */
        { "Flame-Master",  0 } },
      "Earth", "Fire", "Ash", /* Special */
      "Fla", "the great Circle of Flame", "the High Ice Mage's Cave",
      PM_FLAME_MAGE, 
      NON_PM, 
      PM_HELL_HOUND_PUP,
      PM_HIGH_FLAME_MAGE, 
      PM_IGNITER,
      PM_HIGH_ICE_MAGE,
      PM_ICE_ELEMENTAL, 
      PM_RUST_MONSTER, 
      S_ELEMENTAL, 
      S_RUSTMONST,
      ART_CANDLE_OF_ETERNAL_FLAME,
      MH_HUMAN | MH_DWARF | MH_ELF | MH_GNOME | MH_GIANT,
      ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL,
      /* Str Int Wis Dex Con Cha */
      {   7, 9,  11,  7,  7,  7 },
      {  12, 15, 20, 20, 20, 10 },
      /* Init   Lower  Higher */
      { 10, 0,  0, 8,  1, 0 },	/* Hit points */
      {  4, 3,  0, 2,  0, 3 }, 12,	/* Energy */
      0, 1, 0, 2, 10, A_WIS, SPE_ENLIGHTEN, -4 },
    { { "Healer", 0 },
      { { "Rhizotomist", 0 },
        { "Empiric", 0 },
        { "Embalmer", 0 },
        { "Dresser", 0 },
        { "Medicus ossium", "Medica ossium" },
        { "Herbalist", 0 },
        { "Magister", "Magistra" },
        { "Physician", 0 },
        { "Chirurgeon", 0 } },
      "_Athena", "Hermes", "Poseidon", /* Greek */
      "Hea",
      "the Temple of Epidaurus",
      "the Temple of Coeus",
      PM_HEALER,
      NON_PM,
      PM_KITTEN,
      PM_HIPPOCRATES,
      PM_ATTENDANT,
      PM_CYCLOPS,
      PM_GIANT_RAT,
      PM_SNAKE,
      S_RODENT,
      S_YETI,
      ART_STAFF_OF_AESCULAPIUS,
      MH_HUMAN | MH_CENTAUR | MH_DWARF | MH_ELF |  MH_GNOME | MH_HOBBIT
          | MH_TORTLE,
      ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL,
      /* Str Int Wis Dex Con Cha */
      { 7, 11, 11, 7, 11, 11 },
      { 15, 20, 20, 15, 25, 5 },
      /* Init   Lower  Higher */
      { 11, 0, 0, 8, 1, 0 }, /* Hit points */
      { 4, 2, 0, 1, 0, 2 },
      20, /* Energy */
      10, 3, -3, 2, 10, A_WIS, SPE_CURE_SICKNESS, -4 },
    { { "Ice Mage", 0 }, 
      { { "Cooler",        0 },   /* WAC was Chiller */
        { "Condenser",     0 },   /* WAC was Chiller */
        { "Chiller",       0 },
        { "Froster",       0 },
        { "Permafroster",  0 },   /* WAC was Froster */
        { "Icer",          0 },   /* WAC was Froster */
        { "Freezer",       0 },
        { "Sublimer",      0 },   /* WAC was Freezer */
        { "Ice-Master",    0 } },
      "Air", "Frost", "Smoke", /* Special */
      "Ice", "the great Ring of Ice", "the Earth Mage's Cave",
      PM_ICE_MAGE, 
      NON_PM, 
      PM_WINTER_WOLF_CUB,
      PM_HIGH_ICE_MAGE, 
      PM_FROSTER, 
      PM_RAGNAROS,
      PM_RUST_MONSTER, 
      PM_XORN, 
      S_RUSTMONST, 
      S_XORN,
      ART_STORM_WHISTLE,
      MH_HUMAN | MH_CENTAUR | MH_ORC | MH_ILLITHID | MH_VAMPIRE,
      ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */  /* Direct copy from Wizard */
      {   7, 10,  7,  7,  7,  7 },
      {  10, 30, 10, 20, 20, 10 },
      /* Init   Lower  Higher */
      { 10, 0,  0, 8,  1, 0 },	/* Hit points */
      {  4, 3,  0, 2,  0, 3 },12,	/* Energy */
      0, 1, 0, 
      2, 10, A_INT, SPE_REFLECTION,    -4 },
    { { "Infidel", 0 },
      { { "Apostate", 0 },
        { "Heathen", 0 },
        { "Heretic", 0 },
        { "Idolater", "Idolatress" },
        { "Cultist", 0 },
        { "Splanchomancer", 0 },
        { "Maleficus", "Malefica" },
        { "Demonologist", 0 },
        { "Heresiarch", 0 } },
      0, 0, 0, /* uses a random role's pantheon */
      "Inf",
      "the Hidden Temple",
      "the Howling Forest",
      PM_INFIDEL,
      NON_PM,
      PM_LESSER_HOMUNCULUS,
      PM_ARCHBISHOP_OF_MOLOCH,
      PM_CULTIST,
      PM_PALADIN,
      PM_AGENT,
      PM_CHAMPION,
      S_DOG,
      S_UNICORN,
      ART_IDOL_OF_MOLOCH,
      MH_HUMAN | MH_CENTAUR | MH_GIANT | MH_ORC | MH_ILLITHID,
      ROLE_MALE | ROLE_FEMALE | ROLE_CHAOTIC, /* actually unaligned */
      /* Str Int Wis Dex Con Cha */
      { 7, 7, 10, 7, 7, 7 },
      { 20, 10, 25, 15, 20, 10 },
      /* Init   Lower  Higher */
      { 10, 0, 0, 8, 1, 0 }, /* Hit points */
      { 4, 3, 0, 1, 0, 2 },
      10, /* Energy */
      10, 3, 1, 2, 10, A_WIS, SPE_FIREBALL, -4 },
    { { "Jedi", 0 }, 
      { { "Youngling",     0 },
        { "Padawan",       0 },
        { "Jedi Apprentice", 0 },
        { "Jedi Knight",    0 },
        { "Jedi Hero",      0 },
        { "Jedi Master",    0 } },
      "the Light Side", "the Force", "the Dark Side",
      "Jed",
      "the Jedi Temple",
      "the Outer Rim",
      PM_JEDI, 
      NON_PM, 
      PM_DROID,
      PM_JEDI_MASTER, 
      PM_PADAWAN, 
      PM_LORD_SIDIOUS,
      PM_STORMTROOPER, 
      PM_STORMTROOPER, 
      S_HUMAN,
      S_HUMAN,
#if 0
	ART_DELUDER, ART_SNAKESKIN
#endif
      ART_LIGHTSABER_PROTOTYPE,
      MH_HUMAN | MH_DWARF | MH_ELF | MH_HOBBIT | MH_TORTLE,
      ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL,
      /* Str Int Wis Dex Con Cha */
      {  10,  7, 14, 12, 10, 14 },
      {  30, 15, 15, 10, 20, 10 },
      /* Init   Lower  Higher */
      { 20, 4,  0, 8,  2, 4 },	/* Hit points */
      {  5, 4,  0, 1,  0, 4 },
      10, /* Energy */
      0, 12 , -1, 2, 10, A_INT, SPE_CHARM_MONSTER, -4 },
    { { "Knight", 0 },
      { { "Gallant", 0 },
        { "Esquire", 0 },
        { "Bachelor", "Bachelorette" },
        { "Sergeant", 0 },
        { "Knight", 0 },
        { "Banneret", 0 },
        { "Chevalier", "Chevaliere" },
        { "Seignieur", "Dame" },
        { "Paladin", 0 } },
      "Lugh", "_Brigit", "Manannan Mac Lir", /* Celtic */
      "Kni",
      "Camelot Castle",
      "the Isle of Glass",
      PM_KNIGHT,
      NON_PM,
      PM_PONY,
      PM_KING_ARTHUR,
      PM_PAGE,
      PM_IXOTH,
      PM_QUASIT,
      PM_OCHRE_JELLY,
      S_IMP,
      S_JELLY,
      ART_MAGIC_MIRROR_OF_MERLIN,
      MH_HUMAN | MH_CENTAUR | MH_DWARF | MH_ELF | MH_ORC ,
      ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      { 13, 7, 14, 8, 10, 17 },
      { 30, 15, 15, 10, 20, 10 },
      /* Init   Lower  Higher */
      { 14, 0, 0, 8, 2, 0 }, /* Hit points */
      { 2, 4, 0, 1, 0, 2 },
      10, /* Energy */
      10, 8, -2, 0, 9, A_WIS, SPE_TURN_UNDEAD, -4 },
    { { "Monk", 0 },
      { { "Candidate", 0 },
        { "Novice", 0 },
        { "Initiate", 0 },
        { "Student of Stones", 0 },
        { "Student of Waters", 0 },
        { "Student of Metals", 0 },
        { "Student of Winds", 0 },
        { "Student of Fire", 0 },
        { "Master", 0 } },
      "Shan Lai Ching", "Chih Sung-tzu", "Huan Ti", /* Chinese */
      "Mon",
      "the Monastery of Chan-Sune",
      "the Monastery of the Earth-Lord",
      PM_MONK,
      NON_PM,
      NON_PM,
      PM_MASTER_PO,
      PM_ABBOT,
      PM_MASTER_KAEN,
      PM_EARTH_ELEMENTAL,
      PM_XORN,
      S_ELEMENTAL,
      S_XORN,
      ART_EYES_OF_THE_OVERWORLD,
      MH_HUMAN | MH_CENTAUR | MH_DWARF | MH_ELF | MH_GIANT
          | MH_TORTLE | MH_SHIFTER,
      ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL
          | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      { 10, 7, 14, 8, 7, 7 },
      { 25, 10, 20, 20, 15, 10 },
      /* Init   Lower  Higher */
      { 12, 0, 0, 8, 1, 0 }, /* Hit points */
      { 7, 2, 0, 2, 0, 2 },
      10, /* Energy */
      10, 8, -2, 2, 20, A_WIS, SPE_RESTORE_ABILITY, -4 },

    { { "Cartomancer", 0 },
      { { "Learner", 0 },
        { "Shuffler", 0 },
        { "Player", 0 },
        { "Reader", 0 },
        { "Strategist", 0 },
        { "Deck Stacker", 0 },
        { "Card Collector", 0 },
        { "Deck Stacker", 0 },
        { "King of Games", "Queen of Games" } },
      "Johnny", "Spike", "Timmy", /* Card player archetypes */
      "Car",
      "the Card Training School",
      "the Great Card Arena",
      PM_CARTOMANCER,
      NON_PM,
      PM_LITTLE_DOG,
      PM_KING_OF_GAMES,
      PM_STUDENT, /* Reusing students to avoid redundancy */
      PM_DAL_ZETHIRE,
      PM_COBRA,
      PM_GUARDIAN_NAGA,
      S_SNAKE,
      S_NAGA,
      ART_HOLOGRAPHIC_VOID_LILY,
      MH_HUMAN | MH_DWARF | MH_ELF | MH_GNOME /*| MH_DEMON | MH_UNDEAD*/,
      ROLE_MALE | ROLE_FEMALE |
      ROLE_LAWFUL | ROLE_NEUTRAL | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      { 7, 10, 7, 7, 7, 7 },
      { 10, 30, 10, 20, 20, 10 },
      /* Init   Lower  Higher */
      { 10, 0, 0, 8, 1, 0 }, /* Hit points */
      { 4, 3, 0, 2, 0, 3 }, 12, /* Energy */
      0, 1, 0, 3, 10, A_INT, SPE_CREATE_MONSTER, -4 },  
    { { "Necromancer", 0 }, 
      { { "Gravedigger",  0 },
        { "Embalmer", 0 },
        { "Mortician", 0 },
        { "Zombie Lord", 0 },
        { "Ghoul Master",0 },
        { "Necromancer", 0 },
        { "Necromancer", 0 },
        { "Undead Master", 0 },
        { "Lich Lord", 0 } },
      "Nyarlathotep", "Zugguthobal", "Gothuulbe", /* Assorted slimy things */
      "Nec", 
      "the Tower of the Dark Lord", 
      "the Lair of Maugneshaagar",
      PM_NECROMANCER, 
      NON_PM, 
      PM_GHOUL,
      PM_DARK_LORD, 
      PM_EMBALMER, 
      PM_MAUGNESHAAGAR,
      PM_NUPPERIBO, 
      PM_MONGBAT, 
      S_BAT, 
      S_IMP,
      ART_GREAT_DAGGER_OF_GLAURGNAA,
      MH_HUMAN | MH_CENTAUR | MH_GIANT | MH_ILLITHID | MH_ORC | MH_VAMPIRE,
      ROLE_MALE | ROLE_FEMALE | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */  /* Direct copy from Wizard */
      {   7, 10,  7,  7,  7,  7 },
      {  10, 30, 10, 20, 20, 10 },
      /* Init   Lower  Higher */
      { 10, 0,  0, 8,  1, 0 },	/* Hit points */
      {  4, 3,  0, 2,  0, 3 },12,	/* Energy */
      0, 1, 0, 2, 10, A_INT, SPE_SUMMON_UNDEAD, -4 },
    { { "Priest", "Priestess" },
      { { "Aspirant", 0 },
        { "Acolyte", 0 },
        { "Adept", 0 },
        { "Priest", "Priestess" },
        { "Curate", 0 },
        { "Canon", "Canoness" },
        { "Lama", 0 },
        { "Patriarch", "Matriarch" },
        { "High Priest", "High Priestess" } },
      0, 0, 0, /* deities from a randomly chosen other role will be used */
      "Pri",
      "the Great Temple",
      "the Temple of Nalzok",
      PM_PRIEST,
      PM_PRIESTESS,
      NON_PM,
      PM_ARCH_PRIEST,
      PM_ACOLYTE,
      PM_NALZOK,
      PM_HUMAN_ZOMBIE,
      PM_WRAITH,
      S_ZOMBIE,
      S_WRAITH,
      ART_MITRE_OF_HOLINESS,
      MH_HUMAN | MH_CENTAUR | MH_DWARF | MH_ELF | MH_GIANT | MH_HOBBIT
          | MH_ILLITHID | MH_ORC | MH_TORTLE | MH_SHIFTER,
      ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL
          | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      {   7, 10, 10,  7,  7,  7 },
      {  15, 20, 20, 15, 20, 10 },
      /* Init   Lower  Higher */
      { 12, 0, 0, 8, 1, 0 }, /* Hit points */
      { 5, 3, 0, 2, 0, 2 },
      10, /* Energy */
      0, 3, -2, 2, 10, A_WIS, SPE_REMOVE_CURSE, -4 },
    /* Pirate is after priest so priest retains lowercase 'p' in character 
     * select */ 
    { { "Pirate", 0 }, 
      { { "Landlubber", 0 },
        { "Swabbie", 0 },
        { "Cut-throat", 0 },
        { "Bosun", 0 },
        { "Second Mate", 0 },
        { "First Mate", 0 },
        { "Captain", 0 },
        { "Pirate Lord", "Pirate Lady" },
        { "Dread Pirate", 0 } },
      "Davy Jones", "_the deep blue sea", "_the storm",
      /* Davy Jones would be chaotic, but we want something all pirates are
        opposed to. */
      "Pir",
      "Tortuga",
      "Shipwreck Island",
      PM_PIRATE,
      NON_PM,
      NON_PM,
      PM_MAYOR_CUMMERBUND,
      PM_PIRATE_CREWMATE,
      PM_BLACKBEARD_S_GHOST,
      PM_SKELETAL_PIRATE,
      PM_SOLDIER,
      S_RODENT,
      S_ELEMENTAL, /* Ghost pirates, soldiers, rats in the food stores, and the occasional storm*/
      ART_TREASURY_OF_PROTEUS,
      MH_HUMAN | MH_GNOME | MH_ILLITHID | MH_ORC | MH_VAMPIRE,
      ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      {   8,  7, 7,  8,  8,  7 },
      {  20, 15, 10, 20, 25, 10 },
      /* Init   Lower  Higher */
      { 10, 0,  0, 8,  1, 0 },	/* Hit points */
      {  1, 0,  0, 1,  0, 1 },
      12,	/* Energy */
      10, 8, 0, 2, 9, A_INT, SPE_HASTE_SELF, -4 },
    /* Note:  Rogue precedes Ranger so that use of `-R' on the command line
       retains its traditional meaning. */
    { { "Rogue", 0 },
      { { "Footpad", 0 },
        { "Cutpurse", 0 },
        { "Rogue", 0 },
        { "Pilferer", 0 },
        { "Robber", 0 },
        { "Burglar", 0 },
        { "Filcher", 0 },
        { "Magsman", "Magswoman" },
        { "Thief", 0 } },
      "Issek", "Mog", "Kos", /* Nehwon */
      "Rog",
      "the Thieves' Guild Hall",
      "the Assassins' Guild Hall",
      PM_ROGUE,
      NON_PM,
      NON_PM,
      PM_MASTER_OF_THIEVES,
      PM_THUG,
      PM_MASTER_ASSASSIN,
      PM_LEPRECHAUN,
      PM_GUARDIAN_NAGA,
      S_NYMPH,
      S_NAGA,
      ART_MASTER_KEY_OF_THIEVERY,
      MH_HUMAN | MH_GNOME | MH_HOBBIT | MH_ORC | MH_SHIFTER | MH_VAMPIRE,
      ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      { 7, 7, 7, 10, 7, 6 },
      { 20, 10, 10, 30, 20, 10 },
      /* Init   Lower  Higher */
      { 10, 0, 0, 8, 1, 0 }, /* Hit points */
      { 1, 0, 0, 1, 0, 1 },
      11, /* Energy */
      10, 8, 0, 1, 9, A_INT, SPE_DETECT_TREASURE, -4 },
    { { "Ranger", 0 },
      {
#if 0 /* OBSOLETE */
        {"Edhel",   "Elleth"},
        {"Edhel",   "Elleth"},         /* elf-maid */
        {"Ohtar",   "Ohtie"},          /* warrior */
        {"Kano",    "Kanie"},          /* commander (Q.) ['a] educated guess,
                                          until further research- SAC */
        {"Arandur"," Aranduriel"}, /* king's servant, minister (Q.) - guess */
        {"Hir",         "Hiril"},      /* lord, lady (S.) ['ir] */
        {"Aredhel",     "Arwen"},      /* noble elf, maiden (S.) */
        {"Ernil",       "Elentariel"}, /* prince (S.), elf-maiden (Q.) */
        {"Elentar",     "Elentari"},   /* Star-king, -queen (Q.) */
        "Solonor Thelandira", "Aerdrie Faenya", "Lolth", /* Elven */
#endif
        { "Tenderfoot", 0 },
        { "Lookout", 0 },
        { "Trailblazer", 0 },
        { "Reconnoiterer", "Reconnoiteress" },
        { "Scout", 0 },
        { "Arbalester", 0 }, /* One skilled at crossbows */
        { "Archer", 0 },
        { "Sharpshooter", 0 },
        { "Marksman", "Markswoman" } },
      "Mercury", "_Venus", "Mars", /* Roman/planets */
      "Ran",
      "Orion's camp",
      "the cave of the wumpus",
      PM_RANGER,
      NON_PM,
      PM_LITTLE_DOG /* Orion & canis major */,
      PM_ORION,
      PM_HUNTER,
      PM_SCORPIUS,
      PM_FOREST_CENTAUR,
      PM_SCORPION,
      S_CENTAUR,
      S_SPIDER,
      ART_LONGBOW_OF_DIANA,
      MH_HUMAN | MH_CENTAUR | MH_GNOME  | MH_HOBBIT | MH_ORC,
      ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      { 13, 13, 13, 9, 13, 7 },
      { 30, 10, 10, 20, 20, 10 },
      /* Init   Lower  Higher */
      { 13, 0, 0, 6, 1, 0 }, /* Hit points */
      { 1, 0, 0, 1, 0, 1 },
      12, /* Energy */
      10, 9, 2, 1, 10, A_INT, SPE_INVISIBILITY, -4 },
    { { "Samurai", 0 },
      { { "Hatamoto", 0 },       /* Banner Knight */
        { "Ronin", 0 },          /* no allegiance */
        { "Ninja", "Kunoichi" }, /* secret society */
        { "Joshu", 0 },          /* heads a castle */
        { "Ryoshu", 0 },         /* has a territory */
        { "Kokushu", 0 },        /* heads a province */
        { "Daimyo", 0 },         /* a samurai lord */
        { "Kuge", 0 },           /* Noble of the Court */
        { "Shogun", 0 } },       /* supreme commander, warlord */
      "_Amaterasu Omikami", "Raijin", "Susanowo", /* Japanese */
      "Sam",
      "the Castle of the Taro Clan",
      "the Shogun's Castle",
      PM_SAMURAI,
      NON_PM,
      PM_LITTLE_DOG,
      PM_LORD_SATO,
      PM_ROSHI,
      PM_ASHIKAGA_TAKAUJI,
      PM_WOLF,
      PM_STALKER,
      S_DOG,
      S_ELEMENTAL,
      ART_TSURUGI_OF_MURAMASA,
      MH_HUMAN | MH_DWARF | MH_GIANT | MH_TORTLE,
      ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL,
      { 10, 8, 7, 10, 17, 6 },
      { 30, 10, 8, 30, 14, 8 },
      /* Init   Lower  Higher */
      { 13, 0, 0, 8, 1, 0 }, /* Hit points */
      { 1, 0, 0, 1, 0, 1 },
      11, /* Energy */
      10, 10, 0, 0, 8, A_INT, SPE_CLAIRVOYANCE, -4 },
    { { "Tourist", 0 },
      { { "Rambler", 0 },
        { "Sightseer", 0 },
        { "Excursionist", 0 },
        { "Peregrinator", "Peregrinatrix" },
        { "Traveler", 0 },
        { "Journeyer", 0 },
        { "Voyager", 0 },
        { "Explorer", 0 },
        { "Adventurer", 0 } },
      "Blind Io", "_The Lady", "Offler", /* Discworld */
      "Tou",
      "Ankh-Morpork",
      "the Thieves' Guild Hall",
      PM_TOURIST,
      NON_PM,
      NON_PM,
      PM_TWOFLOWER,
      PM_GUIDE,
      PM_MASTER_OF_THIEVES,
      PM_GIANT_SPIDER,
      PM_FOREST_CENTAUR,
      S_SPIDER,
      S_CENTAUR,
      ART_YENDORIAN_EXPRESS_CARD,
      MH_HUMAN | MH_GNOME | MH_HOBBIT | MH_TORTLE,
      ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL,
      /* Str Int Wis Dex Con Cha */
      { 7, 10, 6, 7, 7, 10 },
      { 15, 10, 10, 15, 30, 20 },
      /* Init   Lower  Higher */
      { 8, 0, 0, 8, 0, 0 }, /* Hit points */
      { 1, 0, 0, 1, 0, 1 },
      14, /* Energy */
      0, 5, 1, 2, 10, A_INT, SPE_CREATE_FAMILIAR, -4 },
    { { "Undead Slayer", 0 }, 
      { { "Assistant",    0 },
        { "Eliminator",   0 },
        { "Eliminator",   0 },
        { "Exterminator", 0 },
        { "Exterminator", 0 },
        { "Destroyer",   0 },
        { "Vindicator",  0 },
        { "Vindicator",  0 },
        { "Undead Slayer", 0 } },
    "Seeker", "Osiris", "Seth", /* Egyptian */
    "Und", "the Temple of Light", "the Crypt of Dracula",
    PM_UNDEAD_SLAYER, NON_PM, NON_PM,
    PM_VAN_HELSING, PM_EXTERMINATOR, PM_COUNT_DRACULA,
    PM_HUMAN_MUMMY, PM_VAMPIRE, S_MUMMY, S_VAMPIRE,
    ART_STAKE_OF_VAN_HELSING,
    MH_HUMAN | MH_CENTAUR | MH_DWARF | MH_ELF |  MH_GNOME | MH_GIANT
          | MH_HOBBIT | MH_SHIFTER | MH_TORTLE | MH_VAMPIRE,
    ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL,
    /* Str Int Wis Dex Con Cha */ /* Modified from Knight */
    {  13,  7, 14,  8, 10, 10 },
    {  20, 15, 15, 10, 20, 10 },
    /* Init   Lower  Higher */
    { 14, 0,  0, 8,  2, 0 },	/* Hit points */
    {  1, 4,  0, 1,  0, 2 },10,	/* Energy */
    10, 8,-2, 0,  9, A_WIS, SPE_PROTECTION, -4 },
    { { "Valkyrie", 0 },
      { { "Stripling", 0 },
        { "Skirmisher", 0 },
        { "Fighter", 0 },
        { "Man-at-arms", "Woman-at-arms" },
        { "Warrior", 0 },
        { "Swashbuckler", 0 },
        { "Hero", "Heroine" },
        { "Champion", 0 },
        { "Lord", "Lady" } },
      "Tyr", "Odin", "Loki", /* Norse */
      "Val",
      "the Shrine of Destiny",
      "the cave of Surtur",
      PM_VALKYRIE,
      NON_PM,
      NON_PM /*PM_WINTER_WOLF_CUB*/,
      PM_NORN,
      PM_WARRIOR,
      PM_LORD_SURTUR,
      PM_FIRE_ANT,
      PM_FIRE_GIANT,
      S_ANT,
      S_GIANT,
      ART_GJALLAR,
      MH_HUMAN | MH_CENTAUR | MH_DWARF | MH_GIANT ,
      ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL,
      /* Str Int Wis Dex Con Cha */
      { 10, 7, 7, 7, 10, 7 },
      { 30, 6, 7, 20, 30, 7 },
      /* Init   Lower  Higher */
      { 14, 0, 0, 8, 2, 0 }, /* Hit points */
      { 2, 0, 0, 1, 0, 1 },
      10, /* Energy */
      0, 10, -2, 0, 9, A_WIS, SPE_REPAIR_ARMOR, -4 },
    { { "Wizard", 0 },
      { { "Evoker", 0 },
        { "Conjurer", 0 },
        { "Thaumaturge", 0 },
        { "Magician", 0 },
        { "Enchanter", "Enchantress" },
        { "Sorcerer", "Sorceress" },
        { "Warlock", "Witch"},
        { "Wizard", 0 },
        { "Mage", 0 } },
      "Ptah", "Thoth", "Anhur", /* Egyptian */
      "Wiz",
      "the Lonely Tower",
      "the Tower of Darkness",
      PM_WIZARD,
      NON_PM,
      PM_PSEUDODRAGON,
      PM_NEFERET_THE_GREEN,
      PM_APPRENTICE,
      PM_DARK_ONE,
      PM_VAMPIRE_BAT,
      PM_XORN,
      S_BAT,
      S_WRAITH,
      ART_EYE_OF_THE_AETHIOPICA,
      MH_HUMAN  | MH_DWARF | MH_ELF | MH_GIANT | MH_GNOME | MH_HOBBIT
          | MH_ILLITHID | MH_ORC | MH_SHIFTER | MH_TORTLE | MH_VAMPIRE,
      ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      { 7, 10, 7, 7, 7, 7 },
      { 10, 30, 10, 20, 20, 10 },
      /* Init   Lower  Higher */
      { 10, 0, 0, 8, 1, 0 }, /* Hit points */
      { 6, 3, 0, 2, 0, 3 },
      12, /* Energy */
      0, 1, 0, 3, 10, A_INT, SPE_MAGIC_MISSILE, -4 },
    { { "Yeoman", 0 },
	/* Landowner titles, naval ranks and positions */
	/* We intentionally avoid Lieutenant and Captain */
      { { "Usher",          0 },
        { "Steward", "Stewardess" },
        { "Keeper",         0 },
        { "Marshal",        0 },
        { "Master Steward", "Master Stewardess" },
        { "Chamberlain",    0 },
        { "Constable",      0 },
        { "Chancellor",     0 },
        { "Regent",         0} },
	"His Majesty", "His Holiness", "The Commons", /* The three estates */
	"Yeo", "London", "the inner ward",
      PM_YEOMAN, NON_PM, PM_PONY,
      PM_CHIEF_YEOMAN_WARDER, PM_YEOMAN_WARDER, PM_COLONEL_BLOOD,
      PM_RAVEN, PM_WEREWOLF, S_RODENT, S_DOG,
      ART_CROWN_OF_SAINT_EDWARD,
      MH_HUMAN | MH_ELF | MH_HOBBIT | MH_CENTAUR,
      ROLE_MALE | ROLE_LAWFUL | ROLE_NEUTRAL,
      /* Str Int Wis Dex Con Cha */
      {  12,  7, 10, 12, 12,  7 },
      {  20, 15, 15, 10, 20, 10 },
      /* Init   Lower  Higher */
      { 14, 0,  0, 8,  2, 0 },	/* Hit points */
      {  1, 4,  0, 1,  0, 2 },10,	/* Energy */
      10, 8,-2, 0,  9, A_WIS, SPE_KNOCK, -4 },
    
    /* Array terminator */
    { { 0, 0 } }
};

/* Dark Knight */
const struct Role align_roles[] = {
    { { "Dark Knight", 0 },
      { { "Sniveler", 0 },
        { "Pawn", 0 },
        { "Brute", 0 },
        { "Mercenary", 0 },
        { "Blackguard", 0 },
        { "Turncoat", 0 },
        { "Knave", "Vixen" },
        { "Dark Baron", "Dark Baroness" },
        { "Dark Paladin", 0 } },
      "Lugh", "_Brigit", "Manannan Mac Lir", /* Celtic */
      "Kni",
      "Camelot Castle",
      "the Isle of Glass",
      PM_KNIGHT,
      NON_PM,
      PM_PONY,
      PM_KING_ARTHUR,
      PM_PAGE,
      PM_IXOTH,
      PM_QUASIT,
      PM_OCHRE_JELLY,
      S_IMP,
      S_JELLY,
      ART_MAGIC_MIRROR_OF_MERLIN,
      MH_HUMAN | MH_DWARF | MH_ORC | MH_CENTAUR,
      ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      { 13, 7, 14, 8, 10, 17 },
      { 30, 15, 15, 10, 20, 10 },
      /* Init   Lower  Higher */
      { 14, 0, 0, 8, 2, 0 }, /* Hit points */
      { 1, 4, 0, 1, 0, 2 },
      10, /* Energy */
      10, 8, -2, 0, 9, A_WIS, SPE_TURN_UNDEAD, -4 },
    /* Array terminator */
    { { 0, 0 } }
};

/* Gnomish Ranger */
const struct Role race_roles[] = {
    { { "Ranger", 0 },
      {
        { "Tenderfoot", 0 },
        { "Lookout", 0 },
        { "Trailblazer", 0 },
        { "Reconnoiterer", "Reconnoiteress" },
        { "Scout", 0 },
        { "Arbalester", 0 }, /* One skilled at crossbows */
        { "Archer", 0 },
        { "Sharpshooter", 0 },
        { "Marksman", "Markswoman" } },
      "Mercury", "_Venus", "Mars", /* Roman/planets */
      "Ran",
      "Orion's camp",
      "the cave of the wumpus",
      PM_RANGER,
      NON_PM,
      PM_LITTLE_DOG /* Orion & canis major */,
      PM_ORION,
      PM_HUNTER,
      PM_SCORPIUS,
      PM_FOREST_CENTAUR,
      PM_SCORPION,
      S_CENTAUR,
      S_SPIDER,
      ART_CROSSBOW_OF_CARL,
      MH_HUMAN | MH_ELF | MH_GNOME | MH_ORC | MH_HOBBIT
          | MH_CENTAUR,
      ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      { 13, 13, 13, 9, 13, 7 },
      { 30, 10, 10, 20, 20, 10 },
      /* Init   Lower  Higher */
      { 13, 0, 0, 6, 1, 0 }, /* Hit points */
      { 1, 0, 0, 1, 0, 1 },
      12, /* Energy */
      10, 9, 2, 1, 10, A_INT, SPE_INVISIBILITY, -4 },
    /* Array terminator */
    { { 0, 0 } }
};

/* The player's role, created at runtime from initial
 * choices.  This may be munged in role_init().
 */
struct Role urole = {
    { "Undefined", 0 },
    { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
      { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    "L", "N", "C",
    "Xxx", "home", "locate",
    NON_PM, NON_PM, NON_PM, NON_PM, NON_PM, NON_PM, NON_PM, NON_PM,
    0, 0, 0, 0, 0,
    /* Str Int Wis Dex Con Cha */
    { 7, 7, 7, 7, 7, 7 },
    { 20, 15, 15, 20, 20, 10 },
    /* Init   Lower  Higher */
    { 10, 0, 0, 8, 1, 0 }, /* Hit points */
    { 2, 0, 0, 2, 0, 3 },
    14, /* Energy */
     0, 10, 0, 0, 4, A_INT, 0, -3 
};

/* Table of all races */
const struct Race races[] = {
    {
        "human", "human", "humanity", "Hum",
        { "man", "woman" },  /* individual as a noun */
        PM_HUMAN,   /* PM_ as a male monster */
        NON_PM,     /* as a female (NON_PM == same) */
        PM_HUMAN_MUMMY, /* PM_ as a mummy */
        PM_HUMAN_ZOMBIE, /* PM_ as a zombie */
        MH_HUMAN | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL
            | ROLE_CHAOTIC, /* allowed variations */
        MH_HUMAN,   /* your own race's bit mask */
        0,          /* always peaceful */
        MH_GNOME | MH_ORC | MH_CENTAUR | MH_ILLITHID, /* always hostile */
        /*    Str    Int Wis Dex Con Cha */
        {      3,     3,  3,  3,  3,  3 }, /* min allowable */
        { STR18(100), 18, 18, 18, 18, 18 },/* max allowable */
        /* Init   Lower  Higher */
        { 2, 0, 0, 2, 1, 0 }, /* Hit points */
        { 1, 0, 2, 0, 2, 0 }  /* Energy */
    },
    {
        "elf", "elven", "elvenkind", "Elf",
        { 0, 0 },   /* individual as a noun */
        PM_ELF,     /* PM_ as a male monster */
        NON_PM,     /* as a female (NON_PM == same) */
        PM_ELF_MUMMY, /* PM_ as a mummy */
        PM_ELF_ZOMBIE, /* PM_ as a zombie */
        MH_ELF | ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_LAWFUL, /* allowed variations */
        MH_ELF,     /* your own race's bit mask */
        MH_ELF,     /* always peaceful */
        MH_ORC | MH_ILLITHID, /* always hostile */
        /*    Str    Int Wis Dex Con Cha */
        {      3,     3,  3,  3,  3,  3 }, /* min allowable */
        { 18, 20, 20, 20, 16, 18 },        /* max allowable */
        /* Init   Lower  Higher */
        { 1, 0, 0, 1, 1, 0 }, /* Hit points */
        { 2, 0, 3, 0, 3, 0 }  /* Energy */
    },
    {
        "dwarf", "dwarven", "dwarvenkind", "Dwa",
        { 0, 0 },   /* individual as a noun */
        PM_DWARF,   /* PM_ as a male monster */
        NON_PM,     /* as a female (NON_PM == same) */
        PM_DWARF_MUMMY, /* PM_ as a mummy */
        PM_DWARF_ZOMBIE, /* PM_ as a zombie */
        MH_DWARF | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL, /* allowed variations */
        MH_DWARF,   /* your own race's bit mask */
        MH_DWARF | MH_GNOME, /* always peaceful */
        MH_ORC | MH_ILLITHID | MH_GIANT, /* always hostile */
        /*    Str    Int Wis Dex Con Cha */
        {      3,     3,  3,  3,  3,  3 }, /* min allowable */
        { STR18(100), 16, 16, 20, 20, 16 },/* max allowable */
        /* Init   Lower  Higher */
        { 4, 0, 0, 3, 2, 0 }, /* Hit points */
        { 0, 0, 0, 0, 0, 0 }  /* Energy */
    },
    {
        "gnome", "gnomish", "gnomehood", "Gno",
        { 0, 0 },   /* individual as a noun */
        PM_GNOME,   /* PM_ as a male monster */
        NON_PM,     /* as a female (NON_PM == same) */
        PM_GNOME_MUMMY, /* PM_ as a mummy */
        PM_GNOME_ZOMBIE, /* PM_ as a zombie */
        MH_GNOME | ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_CHAOTIC, /* allowed variations */
        MH_GNOME,   /* your own race's bit mask */
        MH_DWARF | MH_GNOME, /* always peaceful */
        MH_HUMAN | MH_ORC | MH_GIANT | MH_ILLITHID, /* always hostile */
        /*    Str    Int Wis Dex Con Cha */
        {      3,     3,  3,  3,  3,  3 }, /* min allowable */
        { STR18(50), 19, 18, 18, 18, 18 }, /* max allowable */
        /* Init   Lower  Higher */
        { 1, 0, 0, 1, 0, 0 }, /* Hit points */
        { 2, 0, 2, 0, 2, 0 }  /* Energy */
    },
    {
        "orc", "orcish", "orcdom", "Orc",
        { 0, 0 },   /* individual as a noun */
        PM_ORC,     /* PM_ as a male monster */
        NON_PM,     /* as a female (NON_PM == same) */
        PM_ORC_MUMMY, /* PM_ as a mummy */
        PM_ORC_ZOMBIE, /* PM_ as a zombie */
        MH_ORC | ROLE_MALE | ROLE_FEMALE | ROLE_CHAOTIC, /* allowed variations */
        MH_ORC,     /* your own race's bit mask */
        0,          /* always peaceful */
        MH_HUMAN | MH_ELF | MH_DWARF | MH_GNOME | MH_HOBBIT
            | MH_TORTLE, /* always hostile */
        /*    Str    Int Wis Dex Con Cha */
        {      3,     3,  3,  3,  3,  3 }, /* min allowable */
        { STR18(50), 16, 16, 18, 19, 16 }, /* max allowable */
        /* Init   Lower  Higher */
        { 1, 0, 0, 1, 0, 0 }, /* Hit points */
        { 1, 0, 1, 0, 1, 0 }  /* Energy */
    },
    {
        "giant", "giant", "giant-kind", "Gia",
        { 0, 0 },   /* individual as a noun */
        PM_GIANT,   /* PM_ as a male monster */
        NON_PM,     /* as a female (NON_PM == same) */
        PM_GIANT_MUMMY, /* PM_ as a mummy */
        PM_GIANT_ZOMBIE, /* PM_ as a zombie */
        MH_GIANT | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL
            | ROLE_CHAOTIC, /* allowed variations */
        MH_GIANT,   /* your own race's bit mask */
        MH_GIANT,   /* always peaceful */
        MH_HUMAN | MH_DWARF | MH_GNOME | MH_ORC | MH_ILLITHID, /* always hostile */
        /*    Str    Int Wis Dex Con Cha */
        {      3,     3,  3,  3,  3,  3 }, /* min allowable */
        { STR19(25), 14, 18, 14, 25, 16 }, /* max allowable */
        /* Init   Lower  Higher */
        { 4, 3, 1, 4, 3, 3 }, /* Hit points */
        { 1, 0, 1, 0, 1, 0 }  /* Energy */
    },
    {
        "hobbit", "hobbit", "halfling", "Hob",
        { 0, 0 },   /* individual as a noun */
        PM_HOBBIT,  /* PM_ as a male monster */
        NON_PM,     /* as a female (NON_PM == same) */
        PM_HOBBIT_MUMMY, /* PM_ as a mummy */
        PM_HOBBIT_ZOMBIE, /* PM_ as a zombie */
        MH_HOBBIT | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL, /* allowed variations */
        MH_HOBBIT,  /* your own race's bit mask */
        MH_HOBBIT | MH_TORTLE, /* always peaceful */
        MH_ORC | MH_ILLITHID, /* always hostile */
        /*    Str    Int Wis Dex Con Cha */
        {      3,     3,  3,  3,  3,  3 }, /* min allowable */
        { 16, 16, 20, 20, 20, 16 },        /* max allowable */
        /* Init   Lower  Higher */
        { 2, 0, 0, 2, 1, 0 }, /* Hit points */
        { 2, 0, 2, 1, 2, 0 }  /* Energy */
    },
    {
        "centaur", "centaurian", "centaurian", "Cen",
        { 0, 0 }, /* individual as a noun */
        PM_CENTAUR, /* PM_ as a male monster */
        NON_PM,     /* as a female (NON_PM == same) */
        PM_CENTAUR_MUMMY,     /* PM_ as a mummy */
        PM_CENTAUR_ZOMBIE,     /* PM_ as a zombie */
        MH_CENTAUR | ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_CHAOTIC, /* allowed variations */
        MH_CENTAUR, /* your own race's bit mask */
        MH_CENTAUR, /* always peaceful */
        MH_HUMAN | MH_DWARF | MH_GNOME | MH_ILLITHID, /* always hostile */
        /*    Str    Int Wis Dex Con Cha */
        {      3,     3,  3,  3,  3,  3 }, /* min allowable */
        { STR19(20), 12, 14, 20, 18, 16 }, /* max allowable */
        /* Init   Lower  Higher */
        { 3, 3, 1, 2, 2, 2 }, /* Hit points */
        { 1, 0, 1, 0, 1, 0 }  /* Energy */
    },
    {
        "illithid", "illithid", "illithid-kind", "Ith",
        { 0, 0 }, /* individual as a noun */
        PM_ILLITHID, /* PM_ as a male monster */
        NON_PM,     /* as a female (NON_PM == same) */
        NON_PM,     /* PM_ as a mummy */
        NON_PM,     /* PM_ as a zombie */
        MH_ILLITHID | ROLE_MALE | ROLE_FEMALE | ROLE_CHAOTIC, /* allowed variations */
        MH_ILLITHID, /* your own race's bit mask */
        MH_ILLITHID, /* always peaceful */
        MH_HUMAN | MH_ELF | MH_DWARF | MH_GNOME | MH_HOBBIT
            | MH_GIANT | MH_CENTAUR | MH_ORC | MH_TORTLE, /* always hostile */
        /*    Str    Int Wis Dex Con Cha */
        {      3,     3,  3,  3,  3,  3 }, /* min allowable */
        { 10, 22, 22, 20, 12, 16 },        /* max allowable */
        /* Init   Lower  Higher */
        { 2, 0, 1, 1, 1, 0 }, /* Hit points */
        { 3, 0, 3, 0, 4, 0 }  /* Energy */
    },
    {
        "tortle", "tortle", "tortle", "Trt",
        { 0, 0 },   /* individual as a noun */
        PM_TORTLE,  /* PM_ as a male monster */
        NON_PM,     /* as a female (NON_PM == same) */
        NON_PM,     /* PM_ as a mummy */
        NON_PM,     /* PM_ as a zombie */
        MH_TORTLE | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL
            | ROLE_NEUTRAL, /* allowed variations */
        MH_TORTLE, /* your own race's bit mask */
        MH_TORTLE | MH_HOBBIT, /* always peaceful */
        MH_ORC | MH_ILLITHID,  /* always hostile */
        /*    Str    Int Wis Dex Con Cha */
        {      3,     3,  3,  3,  3,  3 }, /* min allowable */
        { STR19(19), 18, 20, 10, 18, 14 }, /* max allowable */
        /* Init   Lower  Higher */
        { 2, 0, 0, 2, 1, 0 }, /* Hit points */
        { 2, 0, 2, 1, 2, 0 }  /* Energy */
    },
    {	"vampire", "vampiric", "vampirehood", "Vam",
        {0, 0},     /* individual as a noun */
        PM_VAMPIRIC, /* PM_ as a male monster */
        NON_PM,     /* ...or as a female (NON_PM == same) */
        PM_HUMAN_MUMMY, /* PM_ as a mummy */
        PM_HUMAN_ZOMBIE, /* PM_ as a zombie */
        MH_VAMPIRE | ROLE_MALE | ROLE_FEMALE | ROLE_CHAOTIC 
            | ROLE_NEUTRAL, /* allowed variations */
        MH_VAMPIRE, /* your own race's bit mask */
        0,          /* always peaceful */
        MH_ELF | MH_GNOME | MH_DWARF | MH_ORC, /* always hostile */
        /*    Str    Int Wis Dex Con Cha */
        {      4,     0,  0,  4,  3,  4 }, /* min allowable */
        { STR19(19), 18, 18, 20, 20, 20 }, /* max allowable */
        /* Init   Lower  Higher */
        {  3, 0,  0, 3,  2, 0 },	/* Hit points */
        {  3, 0,  4, 0,  4, 0 }		/* Energy */
    },
    {	"doppelganger", "doppelganger", "doppelganger-kind", "Dop",
        {0, 0}, /* individual as a noun */
        PM_DOPPELGANGER,    /* PM_ as a male monster */
        NON_PM,             /* ...or as a female (NON_PM == same) */
        PM_HUMAN_MUMMY,     /* PM_ as a mummy */
        PM_HUMAN_ZOMBIE,    /* PM_ as a zombie */
        MH_SHIFTER | ROLE_MALE | ROLE_FEMALE 
            | ROLE_NEUTRAL | ROLE_CHAOTIC, /* allowed variations */
        MH_SHIFTER,        /* your own race's bit mask */
        MH_WERE,          /* always peaceful */
        MH_ELF | MH_GNOME | MH_DWARF | MH_HOBBIT | MH_HUMAN | MH_ORC 
            | MH_TORTLE | MH_GIANT | MH_CENTAUR | MH_ILLITHID, /* always hostile */
        /*    Str     Int Wis Dex Con Cha */
        {      1,      3,  3,  1,  1,  1 },
        { STR18(100), 20, 20, 20, 20, 15 },
        /* Init   Lower  Higher */
        {  0, 0,  0, 1,  1, 0 },	/* Hit points */
        {  7, 0,  5, 0,  5, 0 }		/* Energy */
    },
    /* Array terminator */
    { 0, 0, 0, 0 }
};

/* Special race for crowned Infidels. */
struct Race race_demon = {
    "demon",
    "demonic",
    "demonkind",
    "Dem",
    { 0, 0 },
    PM_DEMON,
    NON_PM,
    NON_PM,
    NON_PM,
    MH_DEMON | ROLE_MALE | ROLE_FEMALE, /* not available at start */
    MH_DEMON,
    MH_DEMON,
    MH_HUMAN | MH_ELF | MH_DWARF | MH_GNOME | MH_HOBBIT
        | MH_GIANT | MH_CENTAUR | MH_TORTLE,
    /*  Str    Int Wis Dex Con Cha */
    { 3, 3, 3, 3, 3, 3 },
    { STR18(100), 18, 18, 20, 20, 18 },
    /* Init   Lower  Higher */
    { 8, 0, 0, 8, 4, 0 }, /* Hit points */
    { 4, 0, 6, 0, 6, 0 }  /* Energy */
};

/* The player's race, created at runtime from initial
 * choices.  This may be munged in role_init().
 */
struct Race urace = {
    "something",
    "undefined",
    "something",
    "Xxx",
    { 0, 0 },
    NON_PM,
    NON_PM,
    NON_PM,
    NON_PM,
    0,
    0,
    0,
    0,
    /*    Str     Int Wis Dex Con Cha */
    { 3, 3, 3, 3, 3, 3 },
    { STR18(100), 18, 18, 18, 18, 18 },
    /* Init   Lower  Higher */
    { 2, 0, 0, 2, 1, 0 }, /* Hit points */
    { 1, 0, 2, 0, 2, 0 }  /* Energy */
};

/* Table of all genders */
const struct Gender genders[] = {
    { "male", "he", "him", "his", "Mal", ROLE_MALE },
    { "female", "she", "her", "her", "Fem", ROLE_FEMALE },
    { "neuter", "it", "it", "its", "Ntr", ROLE_NEUTER }
};

/* Table of all alignments */
const struct Align aligns[] = {
    { "law", "lawful", "Law", ROLE_LAWFUL, A_LAWFUL },
    { "balance", "neutral", "Neu", ROLE_NEUTRAL, A_NEUTRAL },
    { "chaos", "chaotic", "Cha", ROLE_CHAOTIC, A_CHAOTIC },
    { "evil", "unaligned", "Una", 0, A_NONE }
};

/* Filters */
static struct {
    boolean roles[SIZE(roles)];
    short mask;
} rfilter;

STATIC_DCL int NDECL(randrole_filtered);
STATIC_DCL char *FDECL(promptsep, (char *, int));
STATIC_DCL int FDECL(role_gendercount, (int));
STATIC_DCL int FDECL(race_alignmentcount, (int));

/* used by str2XXX() */
static char NEARDATA randomstr[] = "random";

/* Inf are listed as chaotic above, but actually are unaligned. */
int
special_alignment(rolenum, alignnum)
int rolenum, alignnum;
{
    if (rolenum >= 0 && roles[rolenum].malenum == PM_INFIDEL)
        return 3; /* aligns[unaligned] */
    return alignnum;
}

boolean
validrole(rolenum)
int rolenum;
{
    return (boolean) (rolenum >= 0 && rolenum < SIZE(roles) - 1);
}

int
randrole(for_display)
boolean for_display;
{
    int res = SIZE(roles) - 1;

    if (for_display)
        res = rn2_on_display_rng(res);
    else
        res = rn2(res);
    return res;
}

STATIC_OVL int
randrole_filtered()
{
    int i, n = 0, set[SIZE(roles)];

    /* this doesn't rule out impossible combinations but attempts to
       honor all the filter masks */
    for (i = 0; i < SIZE(roles) - 1; ++i) /* -1: avoid terminating element */
        if (ok_role(i, ROLE_NONE, ROLE_NONE, ROLE_NONE)
            && ok_race(i, ROLE_RANDOM, ROLE_NONE, ROLE_NONE)
            && ok_gend(i, ROLE_NONE, ROLE_RANDOM, ROLE_NONE)
            && ok_align(i, ROLE_NONE, ROLE_NONE, ROLE_RANDOM))
            set[n++] = i;
    return n ? set[rn2(n)] : randrole(FALSE);
}

int
str2role(str)
const char *str;
{
    int i, len;

    /* Is str valid? */
    if (!str || !str[0])
        return ROLE_NONE;

    /* Match as much of str as is provided */
    len = strlen(str);
    for (i = 0; roles[i].name.m; i++) {
        /* Does it match the male name? */
        if (!strncmpi(str, roles[i].name.m, len))
            return i;
        /* Or the female name? */
        if (roles[i].name.f && !strncmpi(str, roles[i].name.f, len))
            return i;
        /* Or the filecode? */
        if (!strcmpi(str, roles[i].filecode))
            return i;
    }

    if ((len == 1 && (*str == '*' || *str == '@'))
        || !strncmpi(str, randomstr, len))
        return ROLE_RANDOM;

    /* Couldn't find anything appropriate */
    return ROLE_NONE;
}

boolean
validrace(rolenum, racenum)
int rolenum, racenum;
{
    /* Assumes validrole */
    return (boolean) (racenum >= 0 && racenum < SIZE(races) - 1
                      && (roles[rolenum].mhrace & races[racenum].selfmask));
}

int
randrace(rolenum)
int rolenum;
{
    int i, n = 0;

    /* Count the number of valid races */
    for (i = 0; races[i].noun; i++)
        if (roles[rolenum].mhrace & races[i].selfmask)
            n++;

    /* Pick a random race */
    /* Use a factor of 100 in case of bad random number generators */
    if (n)
        n = rn2(n * 100) / 100;
    for (i = 0; races[i].noun; i++)
        if (roles[rolenum].mhrace & races[i].selfmask) {
            if (n)
                n--;
            else
                return i;
        }

    /* This role has no permitted races? */
    return rn2(SIZE(races) - 1);
}

int
str2race(str)
const char *str;
{
    int i, len;

    /* Is str valid? */
    if (!str || !str[0])
        return ROLE_NONE;

    /* Match as much of str as is provided */
    len = strlen(str);
    for (i = 0; races[i].noun; i++) {
        /* Does it match the noun? */
        if (!strncmpi(str, races[i].noun, len))
            return i;
        /* Or the filecode? */
        if (!strcmpi(str, races[i].filecode))
            return i;
    }

    if ((len == 1 && (*str == '*' || *str == '@'))
        || !strncmpi(str, randomstr, len))
        return ROLE_RANDOM;

    /* Couldn't find anything appropriate */
    return ROLE_NONE;
}

boolean
validgend(rolenum, racenum, gendnum)
int rolenum, racenum, gendnum;
{
    /* Assumes validrole and validrace */
    return (boolean) (gendnum >= 0 && gendnum < ROLE_GENDERS
                      && (roles[rolenum].allow & races[racenum].allow
                          & genders[gendnum].allow & ROLE_GENDMASK));
}

int
randgend(rolenum, racenum)
int rolenum, racenum;
{
    int i, n = 0;

    /* Count the number of valid genders */
    for (i = 0; i < ROLE_GENDERS; i++)
        if (roles[rolenum].allow & races[racenum].allow & genders[i].allow
            & ROLE_GENDMASK)
            n++;

    /* Pick a random gender */
    if (n)
        n = rn2(n);
    for (i = 0; i < ROLE_GENDERS; i++)
        if (roles[rolenum].allow & races[racenum].allow & genders[i].allow
            & ROLE_GENDMASK) {
            if (n)
                n--;
            else
                return i;
        }

    /* This role/race has no permitted genders? */
    return rn2(ROLE_GENDERS);
}

int
str2gend(str)
const char *str;
{
    int i, len;

    /* Is str valid? */
    if (!str || !str[0])
        return ROLE_NONE;

    /* Match as much of str as is provided */
    len = strlen(str);
    for (i = 0; i < ROLE_GENDERS; i++) {
        /* Does it match the adjective? */
        if (!strncmpi(str, genders[i].adj, len))
            return i;
        /* Or the filecode? */
        if (!strcmpi(str, genders[i].filecode))
            return i;
    }
    if ((len == 1 && (*str == '*' || *str == '@'))
        || !strncmpi(str, randomstr, len))
        return ROLE_RANDOM;

    /* Couldn't find anything appropriate */
    return ROLE_NONE;
}

boolean
validalign(rolenum, racenum, alignnum)
int rolenum, racenum, alignnum;
{
    /* Assumes validrole and validrace */
    return (boolean) (alignnum >= 0 && alignnum < ROLE_ALIGNS
                      && (roles[rolenum].allow & aligns[alignnum].allow)
                      && ((races[racenum].allow & aligns[alignnum].allow)
                          || (roles[rolenum].allow & ROLE_NORACEALIGN)));
}

int
randalign(rolenum, racenum)
int rolenum, racenum;
{
    int i, n = 0;

    /* Count the number of valid alignments */
    for (i = 0; i < ROLE_ALIGNS; i++)
        if (validalign(rolenum, racenum, i))
            n++;

    /* Pick a random alignment */
    if (n)
        n = rn2(n);
    for (i = 0; i < ROLE_ALIGNS; i++)
        if (validalign(rolenum, racenum, i)) {
            if (n)
                n--;
            else
                return i;
        }

    /* This role/race has no permitted alignments? */
    return rn2(ROLE_ALIGNS);
}

int
str2align(str)
const char *str;
{
    int i, len;

    /* Is str valid? */
    if (!str || !str[0])
        return ROLE_NONE;

    /* Match as much of str as is provided */
    len = strlen(str);
    for (i = 0; i < ROLE_ALIGNS; i++) {
        /* Does it match the adjective? */
        if (!strncmpi(str, aligns[i].adj, len))
            return i;
        /* Or the filecode? */
        if (!strcmpi(str, aligns[i].filecode))
            return i;
    }
    if ((len == 1 && (*str == '*' || *str == '@'))
        || !strncmpi(str, randomstr, len))
        return ROLE_RANDOM;

    /* Couldn't find anything appropriate */
    return ROLE_NONE;
}

/* is rolenum compatible with any racenum/gendnum/alignnum constraints? */
boolean
ok_role(rolenum, racenum, gendnum, alignnum)
int rolenum, racenum, gendnum, alignnum;
{
    int i;
    short allow;

    if (rolenum >= 0 && rolenum < SIZE(roles) - 1) {
        if (rfilter.roles[rolenum])
            return FALSE;
        allow = roles[rolenum].allow;
        if (racenum >= 0 && racenum < SIZE(races) - 1
            && !(roles[rolenum].mhrace & races[racenum].selfmask))
            return FALSE;
        if (gendnum >= 0 && gendnum < ROLE_GENDERS
            && !(allow & genders[gendnum].allow & ROLE_GENDMASK))
            return FALSE;
        if (alignnum >= 0 && alignnum < ROLE_ALIGNS
            && !(allow & aligns[alignnum].allow & ROLE_ALIGNMASK))
            return FALSE;
        return TRUE;
    } else {
        /* random; check whether any selection is possible */
        for (i = 0; i < SIZE(roles) - 1; i++) {
            if (rfilter.roles[i])
                continue;
            allow = roles[i].allow;
            if (racenum >= 0 && racenum < SIZE(races) - 1
                && !(roles[i].mhrace & races[racenum].selfmask))
                continue;
            if (gendnum >= 0 && gendnum < ROLE_GENDERS
                && !(allow & genders[gendnum].allow & ROLE_GENDMASK))
                continue;
            if (alignnum >= 0 && alignnum < ROLE_ALIGNS
                && !(allow & aligns[alignnum].allow & ROLE_ALIGNMASK))
                continue;
            return TRUE;
        }
        return FALSE;
    }
}

/* pick a random role subject to any racenum/gendnum/alignnum constraints */
/* If pickhow == PICK_RIGID a role is returned only if there is  */
/* a single possibility */
int
pick_role(racenum, gendnum, alignnum, pickhow)
int racenum, gendnum, alignnum, pickhow;
{
    int i;
    int roles_ok = 0, set[SIZE(roles)];

    for (i = 0; i < SIZE(roles) - 1; i++) {
        if (ok_role(i, racenum, gendnum, alignnum)
            && ok_race(i, (racenum >= 0) ? racenum : ROLE_RANDOM,
                       gendnum, alignnum)
            && ok_gend(i, racenum,
                       (gendnum >= 0) ? gendnum : ROLE_RANDOM, alignnum)
            && ok_align(i, racenum,
                        gendnum, (alignnum >= 0) ? alignnum : ROLE_RANDOM))
            set[roles_ok++] = i;
    }
    if (roles_ok == 0 || (roles_ok > 1 && pickhow == PICK_RIGID))
        return ROLE_NONE;
    return set[rn2(roles_ok)];
}

/* is racenum compatible with any rolenum/gendnum/alignnum constraints? */
boolean
ok_race(rolenum, racenum, gendnum, alignnum)
int rolenum, racenum, gendnum, alignnum;
{
    int i;
    short allow;

    if (racenum >= 0 && racenum < SIZE(races) - 1) {
        if (rfilter.mask & races[racenum].selfmask)
            return FALSE;
        allow = races[racenum].allow;
        if (rolenum >= 0 && rolenum < SIZE(roles) - 1) {
            if (!(allow & roles[rolenum].mhrace & races[racenum].selfmask))
                return FALSE;
            if (roles[rolenum].allow & ROLE_NORACEALIGN) {
                /* If the role overrides racial alignments,
                   replace the allowed race alignments with allowed role
                   alignments */
                allow &= ~ROLE_ALIGNMASK;
                allow |= (roles[rolenum].allow & ROLE_ALIGNMASK);
            }
        }
        if (gendnum >= 0 && gendnum < ROLE_GENDERS
            && !(allow & genders[gendnum].allow & ROLE_GENDMASK))
            return FALSE;
        if (alignnum >= 0 && alignnum < ROLE_ALIGNS
            && !(allow & aligns[alignnum].allow & ROLE_ALIGNMASK))
            return FALSE;
        return TRUE;
    } else {
        /* random; check whether any selection is possible */
        for (i = 0; i < SIZE(races) - 1; i++) {
            if (rfilter.mask & races[i].selfmask)
                continue;
            allow = races[i].allow;
            if (rolenum >= 0 && rolenum < SIZE(roles) - 1) {
                if (!(roles[rolenum].mhrace & races[i].selfmask))
                    continue;
                if (roles[rolenum].allow & ROLE_NORACEALIGN) {
                    allow &= ~ROLE_ALIGNMASK;
                    allow |= (roles[rolenum].allow & ROLE_ALIGNMASK);
                }
            }
            if (gendnum >= 0 && gendnum < ROLE_GENDERS
                && !(allow & genders[gendnum].allow & ROLE_GENDMASK))
                continue;
            if (alignnum >= 0 && alignnum < ROLE_ALIGNS
                && !(allow & aligns[alignnum].allow & ROLE_ALIGNMASK))
                continue;
            return TRUE;
        }
        return FALSE;
    }
}

/* Pick a random race subject to any rolenum/gendnum/alignnum constraints.
   If pickhow == PICK_RIGID a race is returned only if there is
   a single possibility. */
int
pick_race(rolenum, gendnum, alignnum, pickhow)
int rolenum, gendnum, alignnum, pickhow;
{
    int i;
    int races_ok = 0;

    for (i = 0; i < SIZE(races) - 1; i++) {
        if (ok_race(rolenum, i, gendnum, alignnum))
            races_ok++;
    }
    if (races_ok == 0 || (races_ok > 1 && pickhow == PICK_RIGID))
        return ROLE_NONE;
    races_ok = rn2(races_ok);
    for (i = 0; i < SIZE(races) - 1; i++) {
        if (ok_race(rolenum, i, gendnum, alignnum)) {
            if (races_ok == 0)
                return i;
            else
                races_ok--;
        }
    }
    return ROLE_NONE;
}

/* is gendnum compatible with any rolenum/racenum/alignnum constraints? */
/* gender and alignment are not comparable (and also not constrainable) */
boolean
ok_gend(rolenum, racenum, gendnum, alignnum)
int rolenum, racenum, gendnum;
int alignnum UNUSED;
{
    int i;
    short allow;

    if (gendnum >= 0 && gendnum < ROLE_GENDERS) {
        if (rfilter.mask & genders[gendnum].allow)
            return FALSE;
        allow = genders[gendnum].allow;
        if (rolenum >= 0 && rolenum < SIZE(roles) - 1
            && !(allow & roles[rolenum].allow & ROLE_GENDMASK))
            return FALSE;
        if (racenum >= 0 && racenum < SIZE(races) - 1
            && !(allow & races[racenum].allow & ROLE_GENDMASK))
            return FALSE;
        return TRUE;
    } else {
        /* random; check whether any selection is possible */
        for (i = 0; i < ROLE_GENDERS; i++) {
            if (rfilter.mask & genders[i].allow)
                continue;
            allow = genders[i].allow;
            if (rolenum >= 0 && rolenum < SIZE(roles) - 1
                && !(allow & roles[rolenum].allow & ROLE_GENDMASK))
                continue;
            if (racenum >= 0 && racenum < SIZE(races) - 1
                && !(allow & races[racenum].allow & ROLE_GENDMASK))
                continue;
            return TRUE;
        }
        return FALSE;
    }
}

/* pick a random gender subject to any rolenum/racenum/alignnum constraints */
/* gender and alignment are not comparable (and also not constrainable) */
/* If pickhow == PICK_RIGID a gender is returned only if there is  */
/* a single possibility */
int
pick_gend(rolenum, racenum, alignnum, pickhow)
int rolenum, racenum, alignnum, pickhow;
{
    int i;
    int gends_ok = 0;

    for (i = 0; i < ROLE_GENDERS; i++) {
        if (ok_gend(rolenum, racenum, i, alignnum))
            gends_ok++;
    }
    if (gends_ok == 0 || (gends_ok > 1 && pickhow == PICK_RIGID))
        return ROLE_NONE;
    gends_ok = rn2(gends_ok);
    for (i = 0; i < ROLE_GENDERS; i++) {
        if (ok_gend(rolenum, racenum, i, alignnum)) {
            if (gends_ok == 0)
                return i;
            else
                gends_ok--;
        }
    }
    return ROLE_NONE;
}

/* is alignnum compatible with any rolenum/racenum/gendnum constraints? */
/* alignment and gender are not comparable (and also not constrainable) */
boolean
ok_align(rolenum, racenum, gendnum, alignnum)
int rolenum, racenum;
int gendnum UNUSED;
int alignnum;
{
    int i;
    short allow;

    if (alignnum >= 0 && alignnum < ROLE_ALIGNS) {
        if (rfilter.mask & aligns[alignnum].allow)
            return FALSE;
        allow = aligns[alignnum].allow;
        if (rolenum >= 0 && rolenum < SIZE(roles) - 1) {
            if (!(allow & roles[rolenum].allow & ROLE_ALIGNMASK))
                return FALSE;
            if (roles[rolenum].allow & ROLE_NORACEALIGN)
                return TRUE;
        }
        if (racenum >= 0 && racenum < SIZE(races) - 1
            && !(allow & races[racenum].allow & ROLE_ALIGNMASK))
            return FALSE;
        return TRUE;
    } else {
        /* random; check whether any selection is possible */
        for (i = 0; i < ROLE_ALIGNS; i++) {
            if (rfilter.mask & aligns[i].allow)
                continue;
            allow = aligns[i].allow;
            if (rolenum >= 0 && rolenum < SIZE(roles) - 1) {
                if (!(allow & roles[rolenum].allow & ROLE_ALIGNMASK))
                    continue;
                if (roles[rolenum].allow & ROLE_NORACEALIGN)
                    return TRUE;
            }
            if (racenum >= 0 && racenum < SIZE(races) - 1
                && !(allow & races[racenum].allow & ROLE_ALIGNMASK))
                continue;
            return TRUE;
        }
        return FALSE;
    }
}

/* Pick a random alignment subject to any rolenum/racenum/gendnum constraints;
   alignment and gender are not comparable (and also not constrainable).
   If pickhow == PICK_RIGID an alignment is returned only if there is
   a single possibility. */
int
pick_align(rolenum, racenum, gendnum, pickhow)
int rolenum, racenum, gendnum, pickhow;
{
    int i;
    int aligns_ok = 0;

    for (i = 0; i < ROLE_ALIGNS; i++) {
        if (ok_align(rolenum, racenum, gendnum, i))
            aligns_ok++;
    }
    if (aligns_ok == 0 || (aligns_ok > 1 && pickhow == PICK_RIGID))
        return ROLE_NONE;
    aligns_ok = rn2(aligns_ok);
    for (i = 0; i < ROLE_ALIGNS; i++) {
        if (ok_align(rolenum, racenum, gendnum, i)) {
            if (aligns_ok == 0)
                return i;
            else
                aligns_ok--;
        }
    }
    return ROLE_NONE;
}

void
rigid_role_checks()
{
    int tmp;

    /* Some roles are limited to a single race, alignment, or gender and
     * calling this routine prior to XXX_player_selection() will help
     * prevent an extraneous prompt that actually doesn't allow
     * you to choose anything further. Note the use of PICK_RIGID which
     * causes the pick_XX() routine to return a value only if there is one
     * single possible selection, otherwise it returns ROLE_NONE.
     *
     */
    if (flags.initrole == ROLE_RANDOM) {
        /* If the role was explicitly specified as ROLE_RANDOM
         * via -uXXXX-@ or OPTIONS=role:random then choose the role
         * in here to narrow down later choices.
         */
        flags.initrole = pick_role(flags.initrace, flags.initgend,
                                   flags.initalign, PICK_RANDOM);
        if (flags.initrole < 0)
            flags.initrole = randrole_filtered();
    }
    if (flags.initrace == ROLE_RANDOM
        && (tmp = pick_race(flags.initrole, flags.initgend,
                            flags.initalign, PICK_RANDOM)) != ROLE_NONE)
        flags.initrace = tmp;
    if (flags.initalign == ROLE_RANDOM
        && (tmp = pick_align(flags.initrole, flags.initrace,
                             flags.initgend, PICK_RANDOM)) != ROLE_NONE)
        flags.initalign = tmp;
    if (flags.initgend == ROLE_RANDOM
        && (tmp = pick_gend(flags.initrole, flags.initrace,
                            flags.initalign, PICK_RANDOM)) != ROLE_NONE)
        flags.initgend = tmp;

    if (flags.initrole != ROLE_NONE) {
        if (flags.initrace == ROLE_NONE)
            flags.initrace = pick_race(flags.initrole, flags.initgend,
                                       flags.initalign, PICK_RIGID);
        if (flags.initalign == ROLE_NONE)
            flags.initalign = pick_align(flags.initrole, flags.initrace,
                                         flags.initgend, PICK_RIGID);
        if (flags.initgend == ROLE_NONE)
            flags.initgend = pick_gend(flags.initrole, flags.initrace,
                                       flags.initalign, PICK_RIGID);
    }
}

boolean
setrolefilter(bufp)
const char *bufp;
{
    int i;
    boolean reslt = TRUE;

    if ((i = str2role(bufp)) != ROLE_NONE && i != ROLE_RANDOM)
        rfilter.roles[i] = TRUE;
    else if ((i = str2race(bufp)) != ROLE_NONE && i != ROLE_RANDOM)
        rfilter.mask |= races[i].selfmask;
    else if ((i = str2gend(bufp)) != ROLE_NONE && i != ROLE_RANDOM)
        rfilter.mask |= genders[i].allow;
    else if ((i = str2align(bufp)) != ROLE_NONE && i != ROLE_RANDOM)
        rfilter.mask |= aligns[i].allow;
    else
        reslt = FALSE;
    return reslt;
}

boolean
gotrolefilter()
{
    int i;

    if (rfilter.mask)
        return TRUE;
    for (i = 0; i < SIZE(roles); ++i)
        if (rfilter.roles[i])
            return TRUE;
    return FALSE;
}

void
clearrolefilter()
{
    int i;

    for (i = 0; i < SIZE(roles); ++i)
        rfilter.roles[i] = FALSE;
    rfilter.mask = 0;
}

#define BP_ALIGN 0
#define BP_GEND 1
#define BP_RACE 2
#define BP_ROLE 3
#define NUM_BP 4

STATIC_VAR char pa[NUM_BP], post_attribs;

STATIC_OVL char *
promptsep(buf, num_post_attribs)
char *buf;
int num_post_attribs;
{
    const char *conjuct = "and ";

    if (num_post_attribs > 1 && post_attribs < num_post_attribs
        && post_attribs > 1)
        Strcat(buf, ",");
    Strcat(buf, " ");
    --post_attribs;
    if (!post_attribs && num_post_attribs > 1)
        Strcat(buf, conjuct);
    return buf;
}

STATIC_OVL int
role_gendercount(rolenum)
int rolenum;
{
    int gendcount = 0;

    if (validrole(rolenum)) {
        if (roles[rolenum].allow & ROLE_MALE)
            ++gendcount;
        if (roles[rolenum].allow & ROLE_FEMALE)
            ++gendcount;
        if (roles[rolenum].allow & ROLE_NEUTER)
            ++gendcount;
    }
    return gendcount;
}

STATIC_OVL int
race_alignmentcount(racenum)
int racenum;
{
    int aligncount = 0;

    if (racenum != ROLE_NONE && racenum != ROLE_RANDOM) {
        if (races[racenum].allow & ROLE_CHAOTIC)
            ++aligncount;
        if (races[racenum].allow & ROLE_LAWFUL)
            ++aligncount;
        if (races[racenum].allow & ROLE_NEUTRAL)
            ++aligncount;
    }
    return aligncount;
}

char *
root_plselection_prompt(suppliedbuf, buflen, rolenum, racenum, gendnum,
                        alignnum)
char *suppliedbuf;
int buflen, rolenum, racenum, gendnum, alignnum;
{
    int k, gendercount = 0, aligncount = 0;
    char buf[BUFSZ];
    static char err_ret[] = " character's";
    boolean donefirst = FALSE;

    if (!suppliedbuf || buflen < 1)
        return err_ret;

    /* initialize these static variables each time this is called */
    post_attribs = 0;
    for (k = 0; k < NUM_BP; ++k)
        pa[k] = 0;
    buf[0] = '\0';
    *suppliedbuf = '\0';

    /* How many alignments are allowed for the desired race? */
    if (racenum != ROLE_NONE && racenum != ROLE_RANDOM)
        aligncount = race_alignmentcount(racenum);

    if (alignnum != ROLE_NONE && alignnum != ROLE_RANDOM
        && ok_align(rolenum, racenum, gendnum, alignnum)) {
        /* if race specified, and multiple choice of alignments for it */
        if ((racenum >= 0) && (aligncount > 1)) {
            if (donefirst)
                Strcat(buf, " ");
            Strcat(buf, aligns[special_alignment(rolenum, alignnum)].adj);
            donefirst = TRUE;
        } else {
            if (donefirst)
                Strcat(buf, " ");
            Strcat(buf, aligns[special_alignment(rolenum, alignnum)].adj);
            donefirst = TRUE;
        }
    } else {
        /* in case we got here by failing the ok_align() test */
        if (alignnum != ROLE_RANDOM)
            alignnum = ROLE_NONE;
        /* if alignment not specified, but race is specified
           and only one choice of alignment for that race then
           don't include it in the later list */
        if ((((racenum != ROLE_NONE && racenum != ROLE_RANDOM)
              && ok_race(rolenum, racenum, gendnum, alignnum))
             && (aligncount > 1))
            || (racenum == ROLE_NONE || racenum == ROLE_RANDOM)) {
            pa[BP_ALIGN] = 1;
            post_attribs++;
        }
    }
    /* <your lawful> */

    /* How many genders are allowed for the desired role? */
    if (validrole(rolenum))
        gendercount = role_gendercount(rolenum);

    if (gendnum != ROLE_NONE && gendnum != ROLE_RANDOM) {
        if (validrole(rolenum)) {
            /* if role specified, and multiple choice of genders for it,
               and name of role itself does not distinguish gender */
            if ((rolenum != ROLE_NONE) && (gendercount > 1)
                && !roles[rolenum].name.f) {
                if (donefirst)
                    Strcat(buf, " ");
                Strcat(buf, genders[gendnum].adj);
                donefirst = TRUE;
            }
        } else {
            if (donefirst)
                Strcat(buf, " ");
            Strcat(buf, genders[gendnum].adj);
            donefirst = TRUE;
        }
    } else {
        /* if gender not specified, but role is specified
                and only one choice of gender then
                don't include it in the later list */
        if ((validrole(rolenum) && (gendercount > 1))
            || !validrole(rolenum)) {
            pa[BP_GEND] = 1;
            post_attribs++;
        }
    }
    /* <your lawful female> */

    if (racenum != ROLE_NONE && racenum != ROLE_RANDOM) {
        if (validrole(rolenum)
            && ok_race(rolenum, racenum, gendnum, alignnum)) {
            if (donefirst)
                Strcat(buf, " ");
            Strcat(buf, (rolenum == ROLE_NONE) ? races[racenum].noun
                                               : races[racenum].adj);
            donefirst = TRUE;
        } else if (!validrole(rolenum)) {
            if (donefirst)
                Strcat(buf, " ");
            Strcat(buf, races[racenum].noun);
            donefirst = TRUE;
        } else {
            pa[BP_RACE] = 1;
            post_attribs++;
        }
    } else {
        pa[BP_RACE] = 1;
        post_attribs++;
    }
    /* <your lawful female gnomish> || <your lawful female gnome> */

    if (validrole(rolenum)) {
        if (donefirst)
            Strcat(buf, " ");
        if (gendnum != ROLE_NONE) {
            if (gendnum == 1 && roles[rolenum].name.f)
                Strcat(buf, roles[rolenum].name.f);
            else
                Strcat(buf, roles[rolenum].name.m);
        } else {
            if (roles[rolenum].name.f) {
                Strcat(buf, roles[rolenum].name.m);
                Strcat(buf, "/");
                Strcat(buf, roles[rolenum].name.f);
            } else
                Strcat(buf, roles[rolenum].name.m);
        }
        donefirst = TRUE;
    } else if (rolenum == ROLE_NONE) {
        pa[BP_ROLE] = 1;
        post_attribs++;
    }

    if ((racenum == ROLE_NONE || racenum == ROLE_RANDOM)
        && !validrole(rolenum)) {
        if (donefirst)
            Strcat(buf, " ");
        Strcat(buf, "character");
        donefirst = TRUE;
    }
    /* <your lawful female gnomish cavewoman> || <your lawful female gnome>
     *    || <your lawful female character>
     */
    if (buflen > (int) (strlen(buf) + 1)) {
        Strcpy(suppliedbuf, buf);
        return suppliedbuf;
    } else
        return err_ret;
}

/* utility function to list valid role/race/align/gender combos, for external
 * scoreboards and role-selection generators
 */
void
listroles()
{
    /* output is 3-character filecodes separated by hyphens */
    /* rog-orc-mal-cha */
    int rol, rac, gen, ali;
    for (rol = 0; roles[rol].filecode; rol++)
        for (rac = 0; races[rac].filecode; rac++)
            for (gen = 0; gen < ROLE_GENDERS; gen++)
                for (ali = 0; ali < ROLE_ALIGNS; ali++)
                    if (ok_role(rol, rac, gen, ali)
                        && ok_race(rol, rac, gen, ali)
                        && ok_gend(rol, rac, gen, ali)
                        && ok_align(rol, rac, gen, ali))
                        raw_printf("%s-%s-%s-%s", roles[rol].filecode,
                                   races[rac].filecode, genders[gen].filecode,
                                   aligns[ali].filecode);
}

char *
build_plselection_prompt(buf, buflen, rolenum, racenum, gendnum, alignnum)
char *buf;
int buflen, rolenum, racenum, gendnum, alignnum;
{
    const char *defprompt = "Shall I pick a character for you? [ynaq] ";
    int num_post_attribs = 0;
    char tmpbuf[BUFSZ], *p;

    if (buflen < QBUFSZ)
        return (char *) defprompt;

    Strcpy(tmpbuf, "Shall I pick ");
    if (racenum != ROLE_NONE || validrole(rolenum))
        Strcat(tmpbuf, "your ");
    else
        Strcat(tmpbuf, "a ");
    /* <your> */

    (void) root_plselection_prompt(eos(tmpbuf), buflen - strlen(tmpbuf),
                                   rolenum, racenum, gendnum, alignnum);
    /* "Shall I pick a character's role, race, gender, and alignment for you?"
       plus " [ynaq] (y)" is a little too long for a conventional 80 columns;
       also, "pick a character's <anything>" sounds a bit stilted */
    strsubst(tmpbuf, "pick a character", "pick character");
    Sprintf(buf, "%s", s_suffix(tmpbuf));
    /* don't bother splitting caveman/cavewoman or priest/priestess
       in order to apply possessive suffix to both halves, but do
       change "priest/priestess'" to "priest/priestess's" */
    if ((p = strstri(buf, "priest/priestess'")) != 0
        && p[sizeof "priest/priestess'" - sizeof ""] == '\0')
        strkitten(buf, 's');

    /* buf should now be:
     *    <your lawful female gnomish cavewoman's>
     * || <your lawful female gnome's>
     * || <your lawful female character's>
     *
     * Now append the post attributes to it
     */
    num_post_attribs = post_attribs;
    if (!num_post_attribs) {
        /* some constraints might have been mutually exclusive, in which case
           some prompting that would have been omitted is needed after all */
        if (flags.initrole == ROLE_NONE && !pa[BP_ROLE])
            pa[BP_ROLE] = ++post_attribs;
        if (flags.initrace == ROLE_NONE && !pa[BP_RACE])
            pa[BP_RACE] = ++post_attribs;
        if (flags.initalign == ROLE_NONE && !pa[BP_ALIGN])
            pa[BP_ALIGN] = ++post_attribs;
        if (flags.initgend == ROLE_NONE && !pa[BP_GEND])
            pa[BP_GEND] = ++post_attribs;
        num_post_attribs = post_attribs;
    }
    if (num_post_attribs) {
        if (pa[BP_RACE]) {
            (void) promptsep(eos(buf), num_post_attribs);
            Strcat(buf, "race");
        }
        if (pa[BP_ROLE]) {
            (void) promptsep(eos(buf), num_post_attribs);
            Strcat(buf, "role");
        }
        if (pa[BP_GEND]) {
            (void) promptsep(eos(buf), num_post_attribs);
            Strcat(buf, "gender");
        }
        if (pa[BP_ALIGN]) {
            (void) promptsep(eos(buf), num_post_attribs);
            Strcat(buf, "alignment");
        }
    }
    Strcat(buf, " for you? [ynaq] ");
    return buf;
}

#undef BP_ALIGN
#undef BP_GEND
#undef BP_RACE
#undef BP_ROLE
#undef NUM_BP

void
plnamesuffix()
{
    char *sptr, *eptr;
    int i;

    /* some generic user names will be ignored in favor of prompting */
    if (sysopt.genericusers) {
        if (*sysopt.genericusers == '*') {
            *plname = '\0';
        } else {
            i = (int) strlen(plname);
            if ((sptr = strstri(sysopt.genericusers, plname)) != 0
                && (sptr == sysopt.genericusers || sptr[-1] == ' ')
                && (sptr[i] == ' ' || sptr[i] == '\0'))
                *plname = '\0'; /* call askname() */
        }
    }

    do {
        if (!*plname)
            askname(); /* fill plname[] if necessary, or set defer_plname */

        /* Look for tokens delimited by '-' */
        if ((eptr = index(plname, '-')) != (char *) 0)
            *eptr++ = '\0';
        while (eptr) {
            /* Isolate the next token */
            sptr = eptr;
            if ((eptr = index(sptr, '-')) != (char *) 0)
                *eptr++ = '\0';

            /* Try to match it to something */
            if ((i = str2role(sptr)) != ROLE_NONE)
                flags.initrole = i;
            else if ((i = str2race(sptr)) != ROLE_NONE)
                flags.initrace = i;
            else if ((i = str2gend(sptr)) != ROLE_NONE)
                flags.initgend = i;
            else if ((i = str2align(sptr)) != ROLE_NONE)
                flags.initalign = i;
        }
    } while (!*plname && !iflags.defer_plname);

    /* commas in the plname confuse the record file, convert to spaces */
    for (sptr = plname; *sptr; sptr++) {
        if (*sptr == ',')
            *sptr = ' ';
    }
}

/* show current settings for name, role, race, gender, and alignment
   in the specified window */
void
role_selection_prolog(which, where)
int which;
winid where;
{
    static const char NEARDATA choosing[] = " choosing now",
                               not_yet[] = " not yet specified",
                               rand_choice[] = " random";
    char buf[BUFSZ];
    int r, c, g, a, allowmask;

    r = flags.initrole;
    c = flags.initrace;
    g = flags.initgend;
    a = flags.initalign;
    if (r >= 0) {
        allowmask = roles[r].allow;
        if (roles[r].mhrace == MH_HUMAN)
            c = 0; /* races[human] */
        else if (c >= 0 && !(roles[r].mhrace & races[c].selfmask))
            c = ROLE_RANDOM;
        if ((allowmask & ROLE_GENDMASK) == ROLE_MALE)
            g = 0; /* role forces male (hypothetical) */
        else if ((allowmask & ROLE_GENDMASK) == ROLE_FEMALE)
            g = 1; /* role forces female (valkyrie) */
        if ((allowmask & ROLE_ALIGNMASK) == AM_LAWFUL)
            a = 0; /* aligns[lawful] */
        else if ((allowmask & ROLE_ALIGNMASK) == AM_NEUTRAL)
            a = 1; /* aligns[neutral] */
        else if ((allowmask & ROLE_ALIGNMASK) == AM_CHAOTIC)
            a = 2; /* aligns[chaotic] */
    }
    if (c >= 0) {
        allowmask = (r >= 0) && (roles[r].allow & ROLE_NORACEALIGN)
                    ? roles[r].allow
                    : races[c].allow;
        if ((allowmask & ROLE_ALIGNMASK) == AM_LAWFUL)
            a = 0; /* aligns[lawful] */
        else if ((allowmask & ROLE_ALIGNMASK) == AM_NEUTRAL)
            a = 1; /* aligns[neutral] */
        else if ((allowmask & ROLE_ALIGNMASK) == AM_CHAOTIC)
            a = 2; /* aligns[chaotic] */
        /* [c never forces gender] */
    }
    a = special_alignment(r, a); /* special case (Infidel) */
    /* [g and a don't constrain anything sufficiently
       to narrow something down to a single choice] */

    Sprintf(buf, "%12s ", "name:");
    Strcat(buf, (which == RS_NAME) ? choosing : !*plname ? not_yet : plname);
    putstr(where, 0, buf);
    Sprintf(buf, "%12s ", "role:");
    Strcat(buf, (which == RS_ROLE) ? choosing : (r == ROLE_NONE)
                                                    ? not_yet
                                                    : (r == ROLE_RANDOM)
                                                          ? rand_choice
                                                          : roles[r].name.m);
    if (r >= 0 && roles[r].name.f) {
        /* distinct female name [caveman/cavewoman, priest/priestess] */
        if (g == 1)
            /* female specified; replace male role name with female one */
            Sprintf(index(buf, ':'), ": %s", roles[r].name.f);
        else if (g < 0)
            /* gender unspecified; append slash and female role name */
            Sprintf(eos(buf), "/%s", roles[r].name.f);
    }
    putstr(where, 0, buf);
    Sprintf(buf, "%12s ", "race:");
    Strcat(buf, (which == RS_RACE) ? choosing : (c == ROLE_NONE)
                                                    ? not_yet
                                                    : (c == ROLE_RANDOM)
                                                          ? rand_choice
                                                          : races[c].noun);
    putstr(where, 0, buf);
    Sprintf(buf, "%12s ", "gender:");
    Strcat(buf, (which == RS_GENDER) ? choosing : (g == ROLE_NONE)
                                                      ? not_yet
                                                      : (g == ROLE_RANDOM)
                                                            ? rand_choice
                                                            : genders[g].adj);
    putstr(where, 0, buf);
    Sprintf(buf, "%12s ", "alignment:");
    Strcat(buf, (which == RS_ALGNMNT) ? choosing : (a == ROLE_NONE)
                                                       ? not_yet
                                                       : (a == ROLE_RANDOM)
                                                             ? rand_choice
                                                             : aligns[a].adj);
    putstr(where, 0, buf);
}

/* add a "pick alignment first"-type entry to the specified menu */
void
role_menu_extra(which, where, preselect)
int which;
winid where;
boolean preselect;
{
    static NEARDATA const char RS_menu_let[] = {
        '=',  /* name */
        '?',  /* role */
        '/',  /* race */
        '\"', /* gender */
        '[',  /* alignment */
    };
    anything any;
    char buf[BUFSZ];
    const char *what = 0, *constrainer = 0, *forcedvalue = 0;
    int f = 0, r, c, g, a, i, allowmask;

    r = flags.initrole;
    c = flags.initrace;
    switch (which) {
    case RS_NAME:
        what = "name";
        break;
    case RS_ROLE:
        what = "role";
        f = r;
        for (i = 0; i < SIZE(roles); ++i)
            if (i != f && !rfilter.roles[i])
                break;
        if (i == SIZE(roles)) {
            constrainer = "filter";
            forcedvalue = "role";
        }
        break;
    case RS_RACE:
        what = "race";
        f = flags.initrace;
        c = ROLE_NONE; /* override player's setting */
        if (r >= 0) {
            allowmask = roles[r].mhrace;
            if (allowmask == MH_HUMAN)
                c = 0; /* races[human] */
            if (c >= 0) {
                constrainer = "role";
                forcedvalue = races[c].noun;
            } else if (f >= 0
                       && (allowmask & ~rfilter.mask) == races[f].selfmask) {
                /* if there is only one race choice available due to user
                   options disallowing others, race menu entry is disabled */
                constrainer = "filter";
                forcedvalue = "race";
            }
        }
        break;
    case RS_GENDER:
        what = "gender";
        f = flags.initgend;
        g = ROLE_NONE;
        if (r >= 0) {
            allowmask = roles[r].allow & ROLE_GENDMASK;
            if (allowmask == ROLE_MALE)
                g = 0; /* genders[male] */
            else if (allowmask == ROLE_FEMALE)
                g = 1; /* genders[female] */
            if (g >= 0) {
                constrainer = "role";
                forcedvalue = genders[g].adj;
            } else if (f >= 0
                       && (allowmask & ~rfilter.mask) == genders[f].allow) {
                /* if there is only one gender choice available due to user
                   options disallowing other, gender menu entry is disabled */
                constrainer = "filter";
                forcedvalue = "gender";
            }
        }
        break;
    case RS_ALGNMNT:
        what = "alignment";
        f = flags.initalign;
        a = ROLE_NONE;
        if (r >= 0) {
            allowmask = roles[r].allow & ROLE_ALIGNMASK;
            if (allowmask == AM_LAWFUL)
                a = 0; /* aligns[lawful] */
            else if (allowmask == AM_NEUTRAL)
                a = 1; /* aligns[neutral] */
            else if (allowmask == AM_CHAOTIC)
                a = 2; /* aligns[chaotic] */
            a = special_alignment(r, a); /* special case (Infidel) */
            if (a >= 0)
                constrainer = "role";
        }
        if (c >= 0 && !constrainer
            && !(r >= 0 && roles[r].allow & ROLE_NORACEALIGN)) {
            allowmask = races[c].allow & ROLE_ALIGNMASK;
            if (allowmask == AM_LAWFUL)
                a = 0; /* aligns[lawful] */
            else if (allowmask == AM_NEUTRAL)
                a = 1; /* aligns[neutral] */
            else if (allowmask == AM_CHAOTIC)
                a = 2; /* aligns[chaotic] */
            if (a >= 0)
                constrainer = "race";
        }
        if (f >= 0 && !constrainer
            && (ROLE_ALIGNMASK & ~rfilter.mask) == aligns[f].allow) {
            /* if there is only one alignment choice available due to user
               options disallowing others, algn menu entry is disabled */
            constrainer = "filter";
            forcedvalue = "alignment";
        }
        if (a >= 0)
            forcedvalue = aligns[a].adj;
        break;
    }

    any = zeroany; /* zero out all bits */
    if (constrainer) {
        any.a_int = 0;
        /* use four spaces of padding to fake a grayed out menu choice */
        Sprintf(buf, "%4s%s forces %s", "", constrainer, forcedvalue);
        add_menu(where, NO_GLYPH, &any, 0, 0, ATR_NONE, buf,
                 MENU_UNSELECTED);
    } else if (what) {
        any.a_int = RS_menu_arg(which);
        Sprintf(buf, "Pick%s %s first", (f >= 0) ? " another" : "", what);
        add_menu(where, NO_GLYPH, &any, RS_menu_let[which], 0, ATR_NONE, buf,
                 MENU_UNSELECTED);
    } else if (which == RS_filter) {
        char setfiltering[40];

        any.a_int = RS_menu_arg(RS_filter);
        Sprintf(setfiltering, "%s role/race/&c filtering",
                gotrolefilter() ? "Reset" : "Set");
        add_menu(where, NO_GLYPH, &any, '~', 0, ATR_NONE,
                 setfiltering, MENU_UNSELECTED);
    } else if (which == ROLE_RANDOM) {
        any.a_int = ROLE_RANDOM;
        add_menu(where, NO_GLYPH, &any, '*', 0, ATR_NONE, "Random",
                 preselect ? MENU_SELECTED : MENU_UNSELECTED);
    } else if (which == ROLE_NONE) {
        any.a_int = ROLE_NONE;
        add_menu(where, NO_GLYPH, &any, 'q', 0, ATR_NONE, "Quit",
                 preselect ? MENU_SELECTED : MENU_UNSELECTED);
    } else {
        impossible("role_menu_extra: bad arg (%d)", which);
    }
}

/*
 *      Special setup modifications here:
 *
 *      Unfortunately, this is going to have to be done
 *      on each newgame or restore, because you lose the permonst mods
 *      across a save/restore.  :-)
 *
 *      1 - The Rogue Leader is the Tourist Nemesis.
 *      2 - Priests start with a random alignment - convert the leader and
 *          guardians here.
 *      3 - Priests also get their set of deities from a randomly chosen role.
 *      4 - [obsolete] Elves can have one of two different leaders,
 *          but can't work it out here because it requires hacking the
 *          level file data (see sp_lev.c).
 *
 * This code also replaces quest_init().
 */
void
role_init()
{
    int alignmnt, malignmnt;
    struct permonst *pm;

    /* Strip the role letter out of the player name.
     * This is included for backwards compatibility.
     */
    plnamesuffix();

    /* Check for a valid role.  Try flags.initrole first. */
    if (!validrole(flags.initrole)) {
        /* Try the player letter second */
        if ((flags.initrole = str2role(pl_character)) < 0)
            /* None specified; pick a random role */
            flags.initrole = randrole_filtered();
    }

    /* We now have a valid role index.  Copy the role name back. */
    /* This should become OBSOLETE */
    Strcpy(pl_character, roles[flags.initrole].name.m);
    pl_character[PL_CSIZ - 1] = '\0';

    /* Check for a valid race */
    if (!validrace(flags.initrole, flags.initrace))
        flags.initrace = randrace(flags.initrole);

    /* Check for a valid gender.  If new game, check both initgend
     * and female.  On restore, assume flags.female is correct. */
    if (flags.pantheon == -1) { /* new game */
        if (!validgend(flags.initrole, flags.initrace, flags.female))
            flags.female = !flags.female;
    }
    if (!validgend(flags.initrole, flags.initrace, flags.initgend))
        /* Note that there is no way to check for an unspecified gender. */
        flags.initgend = flags.female;

    /* Check for a valid alignment */
    if (!validalign(flags.initrole, flags.initrace, flags.initalign))
        /* Pick a random alignment */
        flags.initalign = randalign(flags.initrole, flags.initrace);
    /* Infidels are actually unaligned */
    flags.initalign = special_alignment(flags.initrole, flags.initalign);
    alignmnt = malignmnt = aligns[flags.initalign].value;
    if (alignmnt != A_NONE)
        malignmnt *= 3;

    /* Initialize urole and urace */
    urole = roles[flags.initrole];
    if (u.uevent.uhand_of_elbereth == 4) { /* crowned as a demon */
        urace = race_demon;
        /* mental faculties are not changed by demonization */
        urace.attrmax[A_INT] = races[flags.initrace].attrmax[A_INT];
        urace.attrmax[A_WIS] = races[flags.initrace].attrmax[A_WIS];
    } else
        urace = races[flags.initrace];

    /* kick it over to alternate-alignment role */
    if (alignmnt == A_CHAOTIC && Role_if(PM_KNIGHT)) {
        urole = align_roles[0];
    }

    if (Race_if(PM_GNOME) && Role_if(PM_RANGER)) {
        urole = race_roles[0];
    }

    /* Fix up the quest leader */
    if (urole.ldrnum != NON_PM) {
        pm = &mons[urole.ldrnum];
        pm->msound = MS_LEADER;
        pm->mflags2 |= (M2_PEACEFUL);
        pm->mflags3 |= M3_CLOSE;
        pm->maligntyp = malignmnt;
        /* if gender is random, we choose it now instead of waiting
           until the leader monster is created */
        quest_status.ldrgend =
            is_neuter(pm) ? 2 : is_female(pm) ? 1 : is_male(pm)
                                                        ? 0
                                                        : (rn2(100) < 50);
    }

    /* Fix up the quest guardians */
    if (urole.guardnum != NON_PM) {
        pm = &mons[urole.guardnum];
        pm->mflags2 |= (M2_PEACEFUL);
        pm->maligntyp = malignmnt;
    }

    /* Fix up the quest nemesis */
    if (urole.neminum != NON_PM) {
        pm = &mons[urole.neminum];
        pm->msound = MS_NEMESIS;
        pm->mflags2 &= ~(M2_PEACEFUL);
        pm->mflags2 |= (M2_NASTY | M2_STALK | M2_HOSTILE);
        pm->mflags3 &= ~(M3_CLOSE);
        pm->mflags3 |= M3_WANTSARTI | M3_WAITFORU;
        /* if gender is random, we choose it now instead of waiting
           until the nemesis monster is created */
        quest_status.nemgend = is_neuter(pm) ? 2 : is_female(pm) ? 1
                                   : is_male(pm) ? 0 : (rn2(100) < 50);
    }

    /* Enable special Inf enemies */
    if (Role_if(PM_INFIDEL)) {
        if (urole.enemy1num != NON_PM)
            mons[urole.enemy1num].geno &= ~G_NOGEN;
        if (urole.enemy2num != NON_PM)
            mons[urole.enemy2num].geno &= ~G_NOGEN;
    }

    /* Fix up the god names */
    if (flags.pantheon == -1) {             /* new game */
        flags.pantheon = flags.initrole;    /* use own gods */
        while (!roles[flags.pantheon].lgod) /* unless they're missing */
            flags.pantheon = randrole(FALSE);
    }
    if (!urole.lgod) {
        urole.lgod = roles[flags.pantheon].lgod;
        urole.ngod = roles[flags.pantheon].ngod;
        urole.cgod = roles[flags.pantheon].cgod;
    }
    /* 0 or 1; no gods are neuter, nor is gender randomized */
    quest_status.godgend = !strcmpi(align_gtitle(alignmnt), "goddess");

#if 0
/*
 * Disable this fixup so that mons[] can be const.  The only
 * place where it actually matters for the hero is in set_uasmon()
 * and that can use mons[race] rather than mons[role] for this
 * particular property.  Despite the comment, it is checked--where
 * needed--via instrinsic 'Infravision' which set_uasmon() manages.
 */
    /* Fix up infravision */
    if (mons[urace.malenum].mflags3 & M3_INFRAVISION) {
        /* although an infravision intrinsic is possible, infravision
         * is purely a property of the physical race.  This means that we
         * must put the infravision flag in the player's current race
         * (either that or have separate permonst entries for
         * elven/non-elven members of each class).  The side effect is that
         * all NPCs of that class will have (probably bogus) infravision,
         * but since infravision has no effect for NPCs anyway we can
         * ignore this.
         */
        mons[urole.malenum].mflags3 |= M3_INFRAVISION;
        if (urole.femalenum != NON_PM)
            mons[urole.femalenum].mflags3 |= M3_INFRAVISION;
    }
#endif /*0*/

    /* Artifacts are fixed in hack_artifacts() */

    /* Success! */
    return;
}

const char *
Hello(mtmp)
struct monst *mtmp;
{
    switch (Role_switch) {
    case PM_KNIGHT:
        return "Salutations"; /* Olde English */
    case PM_SAMURAI:
        return (mtmp && mtmp->data == &mons[PM_SHOPKEEPER])
                    ? "Irasshaimase"
                    : "Konnichi wa"; /* Japanese */
    case PM_PIRATE:
        return "Ahoy"; /* Pirate */
    case PM_TOURIST:
        return "Aloha"; /* Hawaiian */
    case PM_VALKYRIE:
        return
#ifdef MAIL
               (mtmp && mtmp->data == &mons[PM_MAIL_DAEMON]) ? "Hallo" :
#endif
               "Velkommen"; /* Norse */
    default:
        return "Hello";
    }
}

const char *
Goodbye()
{
    switch (Role_switch) {
    case PM_KNIGHT:
        return "Fare thee well"; /* Olde English */
    case PM_SAMURAI:
        return "Sayonara"; /* Japanese */
    case PM_TOURIST:
        return "Aloha"; /* Hawaiian */
    case PM_VALKYRIE:
        return "Farvel"; /* Norse */
    default:
        return "Goodbye";
    }
}

/* role.c */
