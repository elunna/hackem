/* NetHack 3.6  artilist.h      $NHDT-Date: 1564351548 2019/07/28 22:05:48 $  $NHDT-Branch: NetHack-3.6 $:$NHDT-Revision: 1.20 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2017. */
/* NetHack may be freely redistributed.  See license for details. */

#ifdef MAKEDEFS_C
/* in makedefs.c, all we care about is the list of names */

#define A(nam, typ, s1, s2, mt, atk, dfn, cry, inv, al, cl, rac, \
          cost, clr, mat) nam

static const char *artifact_names[] = {
#else
/* in artifact.c, set up the actual artifact list structure */

#define A(nam, typ, s1, s2, mt, atk, dfn, cry, inv, al, cl, rac, \
          cost, clr, mat)                                        \
    {                                                            \
        typ, nam, s1, s2, mt, atk, dfn, cry, inv, al, cl, rac,   \
        cost, clr, mat                                           \
    }

/* clang-format off */
#define     NO_ATTK     {0,0,0,0}               /* no attack */
#define     NO_DFNS     {0,0,0,0}               /* no defense */
#define     NO_CARY     {0,0,0,0}               /* no carry effects */
#define     DFNS(c)     {0,c,0,0}
#define     CARY(c)     {0,c,0,0}
#define     PHYS(a,b)   {0,AD_PHYS,a,b}         /* physical */
#define     DRLI(a,b)   {0,AD_DRLI,a,b}         /* life drain */
#define     COLD(a,b)   {0,AD_COLD,a,b}
#define     FIRE(a,b)   {0,AD_FIRE,a,b}
#define     ELEC(a,b)   {0,AD_ELEC,a,b}         /* electrical shock */
#define     STUN(a,b)   {0,AD_STUN,a,b}         /* magical attack */
#define     WIND(a,b)   {0,AD_WIND,a,b}         /* wind blast */
#define     DRST(a,b)   {0,AD_DRST,a,b}         /* poison attack */
#define     ACID(a,b)   {0,AD_ACID,a,b}         /* acid attack */
#define     DISE(a,b)   {0,AD_DISE,a,b}         /* disease attack */
#define     DREN(a,b)   {0,AD_DREN,a,b}         /* drains energy */
#define     STON(a,b)   {0,AD_STON,a,b}         /* petrification */
#define     DETH(a,b)   {0,AD_DETH,a,b}         /* special death attack */
#define     PLYS(a,b)   {0,AD_PLYS,a,b}         /* whip binding */
#define     SLEE(a,b)   {0,AD_SLEE,a,b}         /* Sleep attack  */
#define     LOUD(a,b)   {0,AD_LOUD,a,b}         /* Sonic attack  */
#define     WTHR(a,b)   {0,AD_WTHR,a,b}         /* Withering attack  */
#define     MAST(a,b)   {0,AD_MAGM,a,b}         /* magic missile attack  */

#define DEFAULT_MAT 0 /* use base object's default material */
/* clang-format on */

STATIC_OVL NEARDATA struct artifact artilist[] = {
#endif /* MAKEDEFS_C */

    /* Artifact cost rationale:
     * 1.  The more useful the artifact, the better its cost.
     * 2.  Quest artifacts are highly valued.
     * 3.  Chaotic artifacts are inflated due to scarcity (and balance).
     */

    /*  dummy element #0, so that all interesting indices are non-zero */
    A("", STRANGE_OBJECT, 0, 0, 0, NO_ATTK, NO_DFNS, NO_CARY, 0, A_NONE,
      NON_PM, NON_PM, 0L, NO_COLOR, DEFAULT_MAT),

        /*** Lawful artifacts ***/

    /* Balmung shreds the armor of opponents. */
    A("Balmung", BROADSWORD, (SPFX_RESTR), 0, 0, 
      PHYS(4, 9), DFNS(AD_FIRE), NO_CARY,
      0, A_LAWFUL, NON_PM, NON_PM, 1000L, CLR_BROWN, GOLD),

    /* Some "worse" sacrifice gifts are needed to avoid making #offer
    *       overpowered. Used to be PM_KNIGHT. */
    A("Carnwennan", KNIFE, (SPFX_RESTR | SPFX_SEARCH | SPFX_STLTH), 0, 0,
      PHYS(3, 8), NO_DFNS, NO_CARY, INVIS, A_LAWFUL,
      NON_PM, NON_PM, 400L, NO_COLOR, DEFAULT_MAT),

    /* From SporkHack. Now a silver mace with an extra property.
       First sacrifice gift for a priest. */
    A("Demonbane", HEAVY_MACE, (SPFX_RESTR | SPFX_WARN | SPFX_DFLAGH), 0, MH_DEMON,
      PHYS(5, 0), NO_DFNS, NO_CARY, FLYING, A_LAWFUL, PM_PRIEST, NON_PM, 3000L,
      CLR_WHITE, SILVER),

    /* From LotR. Provides magic resistance and one level of MC when wielded.
     * Can destroy any balrog in one hit. Warns of any demons nearby.*/
    A("Dramborleg", DWARVISH_BEARDED_AXE,
      (SPFX_RESTR | SPFX_INTEL | SPFX_PROTECT | SPFX_WARN | SPFX_DFLAGH),
      0, MH_DEMON, PHYS(8, 8), DFNS(AD_MAGM), NO_CARY, 0, A_LAWFUL,
      NON_PM, PM_DWARF, 9000L, CLR_RED, MITHRIL),

    A("Excalibur", LONG_SWORD, (SPFX_NOGEN | SPFX_RESTR | SPFX_SEEK
                                | SPFX_DEFN | SPFX_INTEL | SPFX_SEARCH),
      0, 0, PHYS(5, 10), DFNS(AD_DRLI), NO_CARY, 0, A_LAWFUL, PM_KNIGHT, NON_PM,
      4000L, NO_COLOR, DEFAULT_MAT),

    A("Firewall", STAFF_OF_DIVINATION, (SPFX_RESTR | SPFX_ATTK | SPFX_DEFN), 0, 0,
      FIRE(4, 8), DFNS(AD_FIRE), NO_CARY, SEFFECT, A_LAWFUL, 
      PM_FLAME_MAGE, NON_PM, 400L, CLR_RED, DEFAULT_MAT),

    A("Grayswandir", SABER, (SPFX_RESTR | SPFX_HALRES), 0, 0,
      PHYS(5, 0), NO_DFNS, NO_CARY, 0, A_LAWFUL, NON_PM, NON_PM, 8000L,
      NO_COLOR, SILVER),
    
    /* I cannot believe this hasn't been done yet. */
    A("Hand Grenade of Antioch", FIRE_BOMB, (SPFX_RESTR), 0, 0, 
      PHYS(1, 1), NO_DFNS, NO_CARY, 0, A_LAWFUL, NON_PM, NON_PM, 300L, 
      NO_COLOR, DEFAULT_MAT),
    
    /*
     *      Orcrist and Sting have same alignment as elves.
     *
     *      The combination of SPFX_WARN+SPFX_DFLAGH+MH_value will trigger
     *      EWarn_of_mon for all monsters that have the MH_value flag.
     *      Sting and Orcrist will warn of MH_ORC monsters.
     */
    A("Orcrist", ELVEN_BROADSWORD, (SPFX_WARN | SPFX_DFLAGH), 0, MH_ORC,
      PHYS(5, 0), NO_DFNS, NO_CARY, 0, A_LAWFUL, NON_PM, PM_ELF, 2000L,
      CLR_BRIGHT_BLUE, MITHRIL), /* bright blue is actually light blue */
                        
    /* Shield of King Arthur. */
    A("Pridwen", LARGE_SHIELD,
      (SPFX_RESTR | SPFX_HPHDAM), 0, 0,
      NO_ATTK, DFNS(AD_CLOB), NO_CARY, 0, A_LAWFUL, NON_PM, NON_PM, 1500L,
      CLR_MAGENTA, DEFAULT_MAT),

    /* Made of copper so that Elven Yeoman are guaranteed to get it. */
    A("Reaper", HALBERD, SPFX_RESTR, 0, 0,
      PHYS(5,20), NO_DFNS, NO_CARY, 0, A_LAWFUL, PM_YEOMAN, NON_PM, 1000L, 
      NO_COLOR, COPPER),

    A("Skullcrusher", CLUB, SPFX_RESTR, 0, 0,
      PHYS(5, 12), NO_DFNS, NO_CARY, 0, A_LAWFUL, PM_CAVEMAN, NON_PM, 
      2000L, NO_COLOR, MINERAL),

    A("Snickersnee", KATANA, 
      SPFX_RESTR, 0, 0, PHYS(5, 8), NO_DFNS, NO_CARY,
      0, A_LAWFUL, NON_PM, NON_PM, 1200L, NO_COLOR, DEFAULT_MAT),

    A("Kiku-ichimonji", KATANA, /* 5lo: New sacrifice gift for Samurai */
      SPFX_RESTR, 0, 0, PHYS(4, 12), NO_DFNS, NO_CARY, 
      0, A_LAWFUL, PM_SAMURAI, NON_PM, 1200L, NO_COLOR, DEFAULT_MAT),
    
    A("Holy Spear of Light", SPEAR, (SPFX_RESTR | SPFX_INTEL | SPFX_DFLAGH), 0, MH_UNDEAD,
      PHYS(5,10), NO_DFNS, NO_CARY, LIGHT_AREA, A_LAWFUL, PM_UNDEAD_SLAYER, 
      NON_PM, 4000L, NO_COLOR, SILVER),

    A("Sting", ELVEN_DAGGER, (SPFX_WARN | SPFX_DFLAGH), 0, MH_ORC, PHYS(5, 0),
      NO_DFNS, NO_CARY, 0, A_LAWFUL, NON_PM, PM_ELF, 800L, CLR_BRIGHT_BLUE, MITHRIL),
    
    /*
     *      Sunsword from SporkHack was silver in nature, and also warned of nearby undead
     *      Sunsword from SporkHack warned of nearby undead
     */
    A("Sunsword", LONG_SWORD, (SPFX_RESTR | SPFX_WARN | SPFX_DFLAGH), 0, MH_UNDEAD,
      PHYS(5, 0), DFNS(AD_BLND), NO_CARY, 0, A_LAWFUL, NON_PM, NON_PM, 2500L,
      NO_COLOR, GEMSTONE),

    A("Sword of Justice", BROADSWORD, (SPFX_RESTR | SPFX_DALIGN), 0, 0,
      PHYS(5, 12), NO_DFNS, NO_CARY, 0, A_LAWFUL, PM_YEOMAN, NON_PM, 1500L, 
      NO_COLOR, METAL),

    A("Quick Blade", ELVEN_SHORT_SWORD,
      SPFX_RESTR, 0, 0, PHYS(9,2), NO_DFNS, NO_CARY, 0, A_LAWFUL, NON_PM, NON_PM, 
      1000L, NO_COLOR, DEFAULT_MAT),


        /*** Neutral artifacts ***/

    /* This lance does a lot of damage, and also occasionally stuns */
    A("Bradamante\'s Fury", LANCE, (SPFX_RESTR), 0, 0,
      PHYS(5, 12), NO_DFNS, NO_CARY, 0, A_LAWFUL, NON_PM, NON_PM, 800L,
      NO_COLOR, DEFAULT_MAT),
    
    /* Staff of Circe. 1/20 chance of turning target into an animal. */
    A("Circe's Witchstaff", STAFF_OF_MATTER,
      (SPFX_RESTR | SPFX_WARN) , 0, 0,
      PHYS(4, 4), NO_DFNS, NO_CARY, TAMING, A_NEUTRAL, NON_PM, NON_PM, 3500L,
      CLR_GREEN, COPPER),
    
    A("Cleaver", BATTLE_AXE, SPFX_RESTR, 0, 0, PHYS(3, 6), NO_DFNS, NO_CARY,
      0, A_NEUTRAL, PM_BARBARIAN, NON_PM, 1500L, NO_COLOR, DEFAULT_MAT),

    A("Deluder", CLOAK_OF_DISPLACEMENT, (SPFX_RESTR | SPFX_STLTH | SPFX_LUCK), 0, 0,
      NO_ATTK, NO_DFNS, NO_CARY, 0, A_NEUTRAL, PM_JEDI, NON_PM, 
      5000L, NO_COLOR, DEFAULT_MAT),

    A("Disrupter", MACE, (SPFX_RESTR | SPFX_DFLAGH), 0, MH_UNDEAD,
      PHYS(5,30), NO_DFNS, NO_CARY, 0, A_NEUTRAL, NON_PM, NON_PM, 
      500L, NO_COLOR, DEFAULT_MAT),

    A("Gauntlets of Defense", GAUNTLETS_OF_DEXTERITY, SPFX_RESTR, SPFX_HPHDAM, 0,
      NO_ATTK, NO_DFNS, NO_CARY, INVIS, A_NEUTRAL, PM_MONK, NON_PM, 
      5000L, NO_COLOR, DEFAULT_MAT),

    A("Giantslayer", LONG_SWORD, (SPFX_RESTR | SPFX_WARN | SPFX_DFLAGH), 0, MH_GIANT,
      PHYS(5, 0), DFNS(AD_CLOB), NO_CARY, 0, A_NEUTRAL, NON_PM, NON_PM, 1200L,
      NO_COLOR, DEFAULT_MAT),

    /* Gungnir has an insane to-hit bonus. */
    A("Gungnir", ATGEIR, (SPFX_RESTR), 0, 0, 
      PHYS(20, 12), NO_DFNS, NO_CARY, LIGHTNING_BOLT, A_NEUTRAL, 
      PM_VALKYRIE, NON_PM, 4000L, NO_COLOR, METAL),
      
    /* Mesopatamian in origin. Grants stability. against clobber/hurtle.
      Invoke for a scroll of air. 
      Hitting a monster with a tornado does 3d4 bonus damage, takes place 1/3 of the time.
      - Does not affect big monsters.
    */
    A("Imhullu", GLAIVE, (SPFX_RESTR), 0, 0, 
      WIND(4, 5), DFNS(AD_CLOB), NO_CARY, SEFFECT, A_NEUTRAL, 
      NON_PM, NON_PM, 2000L, NO_COLOR, DEFAULT_MAT),

    /* Keolewa from SporkHack - a Hawaiian war club. Buffed. */
    A("Keolewa", CLUB, (SPFX_RESTR | SPFX_ATTK | SPFX_DEFN),
      0, 0, ELEC(5, 8), DFNS(AD_ELEC), NO_CARY, 0, A_NEUTRAL,
      PM_CAVEMAN, NON_PM, 2000L, NO_COLOR, DEFAULT_MAT),
    
    /* Evilhack change: Magic fanfare unbalances victims in addition
     * to doing some damage. */
    A("Magicbane", QUARTERSTAFF, (SPFX_RESTR | SPFX_ATTK | SPFX_DEFN), 0, 0,
      STUN(3, 4), DFNS(AD_MAGM), NO_CARY, 0, A_NEUTRAL, PM_WIZARD, NON_PM,
      3500L, NO_COLOR, DEFAULT_MAT),

    A("Mouser\'s Scalpel", RAPIER, SPFX_RESTR, 0, 0,
      PHYS(5, 1), NO_DFNS, NO_CARY, 0, A_NEUTRAL, NON_PM, NON_PM, 
      600L, NO_COLOR, DEFAULT_MAT),
    
    A("Mirrorbright", SHIELD_OF_REFLECTION, (SPFX_RESTR | SPFX_HALRES | SPFX_REFLECT), 0, 0,
      NO_ATTK, NO_DFNS, NO_CARY, 0, A_NEUTRAL, NON_PM, NON_PM, 
      5000L, NO_COLOR, DEFAULT_MAT),
    
    /*
     *      Mjollnir can be thrown when wielded if hero has 25 Strength
     *      (usually via gauntlets of power but possible with rings of
     *      gain strength).  If the thrower is a Valkyrie, Mjollnir will
     *      usually (99%) return and then usually (separate 99%) be caught
     *      and automatically be re-wielded.  When returning Mjollnir is
     *      not caught, there is a 50:50 chance of hitting hero for damage
     *      and its lightning shock might destroy some wands and/or rings.
     *
     *      Monsters don't throw Mjollnir regardless of strength (not even
     *      fake-player valkyries).
     */
    A("Mjollnir", HEAVY_WAR_HAMMER, /* Mjo:llnir */
      (SPFX_RESTR | SPFX_ATTK), 0, 0, ELEC(5, 24), DFNS(AD_ELEC), NO_CARY, 0,
      A_NEUTRAL, PM_VALKYRIE, NON_PM, 5000L, NO_COLOR, DEFAULT_MAT),

    A("Sword of Balance", SHORT_SWORD, (SPFX_RESTR | SPFX_DALIGN), 0, 0,
      PHYS(2, 5), DFNS(AD_CLOB), NO_CARY, 0, A_NEUTRAL, NON_PM, NON_PM, 
      5000L, NO_COLOR, SILVER),

    A("Vorpal Blade", LONG_SWORD, (SPFX_RESTR | SPFX_BEHEAD | SPFX_WARN | SPFX_DFLAGH),
      0, MH_JABBERWOCK, PHYS(5, 6), NO_DFNS, NO_CARY, 0, A_NEUTRAL, NON_PM, NON_PM, 5000L,
      NO_COLOR, METAL),

    A("Whisperfeet", SPEED_BOOTS, (SPFX_RESTR | SPFX_STLTH | SPFX_LUCK), 0, 0,
      NO_ATTK, NO_DFNS, NO_CARY, 0, A_NEUTRAL, PM_TOURIST, NON_PM, 
      5000L, NO_COLOR, DEFAULT_MAT),
    
    A("Herme's Boots", SPEED_BOOTS, (SPFX_RESTR), 0, 0,
      NO_ATTK, DFNS(AD_DISE), NO_CARY, FLYING, A_NEUTRAL, NON_PM, NON_PM, 
      5000L, NO_COLOR, DEFAULT_MAT),

        /*** Chaotic artifacts ***/

    A("Bat from Hell", BASEBALL_BAT,
      (SPFX_RESTR), 0, 0,
      PHYS(3, 20), NO_DFNS, NO_CARY, 0, A_CHAOTIC, NON_PM, NON_PM, 
      5000L, CLR_RED, IRON),

    /* Yeenoghu's infamous triple-headed flail. A massive weapon reputed to have been created
     * from the thighbone and torn flesh of an ancient god he slew. An extremely lethal artifact */
    A("Butcher", TRIPLE_HEADED_FLAIL,
      (SPFX_NOGEN | SPFX_NOWISH | SPFX_RESTR | SPFX_ATTK | SPFX_INTEL),
      0, 0, STUN(5, 8), NO_DFNS, NO_CARY, 0, A_CHAOTIC,
      NON_PM, NON_PM, 4000L, NO_COLOR, BONE),

    /* Instead of targeting humans - Deathsword now targets cross-aligned and
     * sometimes paralyzes it's victims. */
    A("Deathsword", TWO_HANDED_SWORD,
      (SPFX_RESTR | SPFX_DALIGN), 0, 0,
      PHYS(5, 14), NO_DFNS, NO_CARY, 0, A_CHAOTIC, NON_PM, NON_PM, 
      5000L, NO_COLOR, DEFAULT_MAT),

    A("Deep Freeze", ATHAME, (SPFX_RESTR | SPFX_ATTK | SPFX_DEFN), 0, 0,
      COLD(5, 5), DFNS(AD_COLD), NO_CARY, SEFFECT, A_CHAOTIC, PM_ICE_MAGE, NON_PM, 
      400L, CLR_BLUE, METAL),

    /* From SporkHack, but with a twist. This is the anti-Excalibur. */
    A("Dirge", LONG_SWORD,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_ATTK | SPFX_DEFN | SPFX_INTEL), 0, 0,
      ACID(5, 10), DFNS(AD_ACID), NO_CARY, 0, A_CHAOTIC, NON_PM, NON_PM,
      4000L, NO_COLOR, MITHRIL),

    A("Doomblade", ORCISH_SHORT_SWORD, SPFX_RESTR, 0, 0, PHYS(0, 19), NO_DFNS, NO_CARY,
      0, A_CHAOTIC, NON_PM, NON_PM, 1000L, NO_COLOR, DEFAULT_MAT),

    A("Elfrist", ORCISH_SPEAR, (SPFX_WARN | SPFX_DFLAGH), 0, MH_ELF,
      PHYS(5, 0), NO_DFNS, NO_CARY, 0, A_CHAOTIC, NON_PM, PM_ORC, 
      300L, NO_COLOR, DEFAULT_MAT),

  /* Warns when elves are present, but its damage bonus applies to all targets.
   *      (handled as special case in spec_dbon()). */
    A("Grimtooth", ORCISH_DAGGER, (SPFX_RESTR | SPFX_WARN | SPFX_DFLAGH),
      0, MH_ELF, DISE(5, 6), NO_DFNS, NO_CARY, 0, A_CHAOTIC, PM_ROGUE, PM_ORC, 
      1500L, CLR_RED, DEFAULT_MAT),

   /* from SporkHack - many of the same properties as Stormbringer
    *      Meant to be wielded by Vlad. */
    A("Lifestealer", TWO_HANDED_SWORD,
      (SPFX_NOGEN | SPFX_NOWISH | SPFX_RESTR | SPFX_ATTK | SPFX_DEFN
       | SPFX_INTEL | SPFX_DRLI),
      0, 0, DRLI(5, 2), DFNS(AD_DRLI), NO_CARY, 0, A_CHAOTIC, NON_PM,
      NON_PM, 10000L, NO_COLOR, DEFAULT_MAT),

    /* Convict first artifact weapon. Acts like a luckstone. */
    A("Luck Blade", BROADSWORD, (SPFX_RESTR | SPFX_LUCK), 0, 0,
      PHYS(5, 6), NO_DFNS, NO_CARY, 0, A_CHAOTIC, PM_CONVICT, NON_PM, 3000L,
      NO_COLOR, METAL),

    /*
    *       This bizarre weapon acts as a cursed luckstone regardless of its
    *       BCU status. It gets bonuses to hit and damage based on the opposite
    *       of your luck value. Allows for a very unconventional playstyle,
    *       since getting luck of -13 will give this weapon an automatic +13
    *       to damage and hit, but playing with -13 luck would be really hard.
    */
    A("Luckless Folly", SHORT_SWORD, (SPFX_RESTR | SPFX_NOGEN | SPFX_LUCK), 0,
      0, PHYS(5, 5), NO_DFNS, NO_CARY, 0, A_CHAOTIC, NON_PM, NON_PM, 
      3000L, NO_COLOR, DEFAULT_MAT),

    A("The Marauder's Map", SCR_MAGIC_MAPPING, SPFX_RESTR, 0, 0, NO_ATTK,
      NO_DFNS,	NO_CARY, OBJECT_DET, A_CHAOTIC, PM_PIRATE, NON_PM, 2000L,
      NO_COLOR, DEFAULT_MAT),

    A("Plague", ORCISH_BOW, (SPFX_RESTR | SPFX_DEFN), 0, 0,
      PHYS(5,7), DFNS(AD_DRST), DFNS(AD_DISE), 0, A_CHAOTIC, NON_PM, NON_PM, 
      6000L, NO_COLOR, DEFAULT_MAT),
        /* Auto-poison code in dothrow.c */
    
    /* Grants waterbreathing.
          Invoke for water-walking and an earthquake. */
    A("Poseidon\'s Trident", TRIDENT, (SPFX_RESTR | SPFX_BREATHE), 0, 0, PHYS(3, 7),
      NO_DFNS, NO_CARY, WWALKING, A_CHAOTIC, NON_PM, NON_PM, 1500L, NO_COLOR, GOLD),
    
    A("Reaver",	SCIMITAR, (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL), 0, 0,
      PHYS(5, 8), NO_DFNS, NO_CARY, 0, A_CHAOTIC, PM_PIRATE, NON_PM, 6000L,
      NO_COLOR, DEFAULT_MAT),
    
      /* The energy drain only works if the artifact kills its victim.
       * Also increases sacrifice value while wielded. */
    A("Secespita", KNIFE, (SPFX_RESTR | SPFX_ATTK), 0, 0,
      DREN(8, 8), DFNS(AD_DRST), NO_CARY, 0, A_CHAOTIC, PM_INFIDEL, NON_PM,
      1000L, NO_COLOR, COPPER),

    A("Serpent's Tongue", PARAZONIUM, SPFX_RESTR, 0, 0,
      PHYS(5, 0), NO_DFNS, NO_CARY, 0, A_CHAOTIC, PM_NECROMANCER, NON_PM, 
      400L, NO_COLOR, DEFAULT_MAT),
        /* See artifact.c for special poison damage */
    
    /* Stormbringer only has a 2 because it can drain a level, providing 8 more. */
    A("Stormbringer", RUNESWORD,
      (SPFX_RESTR | SPFX_ATTK | SPFX_DEFN | SPFX_INTEL | SPFX_DRLI),
      0, 0, DRLI(5, 2), DFNS(AD_DRLI), NO_CARY, 0, A_CHAOTIC, NON_PM, NON_PM,
      8000L, NO_COLOR, DEFAULT_MAT),

    A("The End", SCYTHE, (SPFX_RESTR | SPFX_DEFN), 0, 0, COLD(3, 20),
      DFNS(AD_DRLI), NO_CARY, 0, A_CHAOTIC, NON_PM, NON_PM, 
      6000L, NO_COLOR, DEFAULT_MAT),

  /*
   * The Sword of Kas - the sword forged by Vecna and given to his top
   * lieutenant, Kas. */
    A("The Sword of Kas", TWO_HANDED_SWORD, 
      (SPFX_NOGEN | SPFX_NOWISH | SPFX_RESTR | SPFX_ATTK | SPFX_DEFN 
       | SPFX_INTEL | SPFX_DALIGN),
      0, 0, DRST(10, 0), DFNS(AD_STON), NO_CARY, 0, A_CHAOTIC, NON_PM, NON_PM,
      15000L, NO_COLOR, GEMSTONE),

      /*** Unaligned artifacts ***/

    /* The quasi-evil twin of Demonbane, Angelslayer is an unholy trident
     * geared towards the destruction of all angelic beings */
    A("Angelslayer", TRIDENT,
      (SPFX_RESTR | SPFX_ATTK | SPFX_SEARCH | SPFX_HSPDAM | SPFX_WARN | SPFX_DFLAGH),
      0, MH_ANGEL, FIRE(5, 10), NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM,
      5000L, NO_COLOR, DEFAULT_MAT),

#if 0 
    /* Bag of the Hesperides - this is the magical bag obtained by Perseus
   * from the Hesperides (nymphs) to contain and transport Medusa's head.
   * The bag naturally repels water, and it has greater weight reduction
   * than a regular bag of holding. Found at the end of the Ice Queen branch
   * with the captive pegasus.
   */
    A("Bag of the Hesperides", BAG_OF_HOLDING,
      (SPFX_NOGEN | SPFX_NOWISH | SPFX_RESTR), SPFX_PROTECT, 0,
      NO_ATTK, NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM,
      8000L, NO_COLOR, DRAGONHIDE),
#endif
    
    /* Warning vs angels; grants 25 charisma while wielded; special paralysis hit*/
    A("Chains of Malcanthet", SPIKED_CHAIN, 
      (SPFX_RESTR | SPFX_WARN | SPFX_DFLAGH), 0, MH_ANGEL,
      PLYS(6, 10), NO_DFNS, NO_CARY, 0, A_NONE,
      NON_PM, NON_PM, 800L, CLR_RED, DEFAULT_MAT),

    /* Dragonbane is no longer a weapon, but a pair of magical gloves made from
     * the scales of a long dead ancient dragon. The gloves afford much of the
     * same special abilities as the weapon did, but swaps fire resistance for
     * resistance. Other dragons will be able to sense the power of these gloves
     * and will be affected accordingly.
     */
    A("Dragonbane", GLOVES,
      (SPFX_RESTR | SPFX_WARN | SPFX_DFLAGH | SPFX_REFLECT), 0, MH_DRAGON,
      NO_ATTK, DFNS(AD_ACID), NO_CARY, 0, A_NONE, NON_PM, NON_PM, 3000L,
      NO_COLOR, DRAGON_HIDE),

    A("Drowsing Rod", STAFF_OF_HEALING, (SPFX_RESTR), 0, 0,
      SLEE(5, 8), DFNS(AD_SLEE), NO_CARY, 0, A_NONE, PM_HEALER, NON_PM, 500L,
      CLR_MAGENTA, COPPER),

    A("Fire Brand", SHORT_SWORD, (SPFX_RESTR | SPFX_ATTK | SPFX_DEFN), 0, 0,
      FIRE(5, 0), DFNS(AD_FIRE), NO_CARY, 0, A_NONE, NON_PM, NON_PM, 3000L,
      NO_COLOR, METAL),

    A("Frost Brand", SHORT_SWORD, (SPFX_RESTR | SPFX_ATTK | SPFX_DEFN), 0, 0,
      COLD(5, 0), DFNS(AD_COLD), NO_CARY, 0, A_NONE, NON_PM, NON_PM, 3000L,
      NO_COLOR, METAL),

    /* Based on the bindings used to trap Fenrir. This hook can grapple
     *       things regardless of size, and has a massive range. */
    A("Gleipnir", GRAPPLING_HOOK, (SPFX_RESTR), 0, 0, PHYS(5, 8),
      NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 
      1700L, NO_COLOR, DEFAULT_MAT),
    
    A("The Master Sword", LONG_SWORD, 
      (SPFX_NOGEN | SPFX_NOWISH | SPFX_RESTR | SPFX_ATTK), 0, 0, 
      MAST(3, 3), NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 1000L, 
      NO_COLOR, GEMSTONE),
    
    /* Attacks deal double damage, but at the cost of hallucination.
    *      This effect is only active while the user can see. */
    A("Mystic Eyes", LENSES, (SPFX_RESTR | SPFX_SEARCH), 0, 0, 
      NO_ATTK, NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 
      2000L, NO_COLOR, GEMSTONE),
    
    /* Created by fusing werebane and trollsbane. Original idea by Spicy. 
            Prevents all monster regen. */
    A("Mortality Dial", EXECUTIONER_S_MACE, (SPFX_RESTR | SPFX_REGEN | SPFX_NOGEN | SPFX_RESTR), 
      0, 0, PHYS(10, 10), NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 
      5000L, NO_COLOR, DEFAULT_MAT),

    A("Ogresmasher", HEAVY_WAR_HAMMER, (SPFX_RESTR | SPFX_WARN | SPFX_DFLAGH), 0, MH_OGRE,
      PHYS(5, 0), NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 1200L,
      NO_COLOR, DEFAULT_MAT),

    /* Quarterstaff that grants teleport control, and also greatly increases
     *      spellcasting ability (as a robe). */
    A("Origin", STAFF_OF_ESCAPE,
      (SPFX_RESTR | SPFX_TCTRL), 0, 0,
      PHYS(2, 6), NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 
      500L, CLR_MAGENTA, DEFAULT_MAT),

    /* The Eye of Vecna, which Vecna will sometimes death drop
       before the rest of his body crumbles to dust */
    A("The Eye of Vecna", EYEBALL,
      (SPFX_NOGEN | SPFX_NOWISH | SPFX_RESTR | SPFX_INTEL),
      (SPFX_XRAY | SPFX_ESP | SPFX_HSPDAM),
      0, NO_ATTK, NO_DFNS, CARY(AD_COLD), DEATH_MAGIC, A_NONE,
      NON_PM, NON_PM, 50000L, NO_COLOR, DEFAULT_MAT),

    /* The Hand of Vecna, another possible artifact that Vecna
       might drop once destroyed */
    A("The Hand of Vecna", MUMMIFIED_HAND,
      (SPFX_NOGEN | SPFX_NOWISH | SPFX_RESTR | SPFX_INTEL | SPFX_REGEN
       | SPFX_HPHDAM),
      0, 0, NO_ATTK, DFNS(AD_DISE), NO_CARY, DEATH_MAGIC, A_NONE,
      NON_PM, NON_PM, 50000L, CLR_BLACK, FLESH),

    /* Blinding mace. */
    A("Sunspot", MACE, (SPFX_RESTR | SPFX_BLIND), 0, 0,
      PHYS(5, 5), DFNS(AD_BLND), NO_CARY, 0, A_NONE, NON_PM, NON_PM, 
      2000L, NO_COLOR, DEFAULT_MAT),

    A("Thiefbane", TWO_HANDED_SWORD,
      (SPFX_NOGEN | SPFX_NOWISH | SPFX_RESTR | SPFX_BEHEAD | SPFX_WARN), 0, 0, 
      PHYS(5, 1), NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 
      1500L, NO_COLOR, PLATINUM),
    
    /* Resisted by very few monsters, but is a morning star and also
    *       happens to destroy tons of items. 
     *      (Previously known as Sonicboom in SpliceHack) */
    A("Thunderstruck", MORNING_STAR, (SPFX_RESTR | SPFX_ATTK | SPFX_DEFN), 0, 0,
      LOUD(5, 20), DFNS(AD_LOUD), NO_CARY, 0, A_NONE, NON_PM, NON_PM, 3000L,
      NO_COLOR, METAL),

    A("Trollsbane", MORNING_STAR, (SPFX_RESTR | SPFX_WARN | SPFX_DFLAGH | SPFX_REGEN), 0, MH_TROLL,
      PHYS(5, 10), NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 1000L,
      NO_COLOR, DEFAULT_MAT),

    A("Wallet of Perseus", BAG_OF_HOLDING, SPFX_RESTR, SPFX_PROTECT, 0,
      NO_ATTK, NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM,
      10000L, NO_COLOR, DRAGON_HIDE),
    
    A("Werebane", SABER, (SPFX_RESTR | SPFX_WARN | SPFX_DFLAGH), 0, MH_WERE,
      PHYS(5, 10), DFNS(AD_WERE), NO_CARY, 0, A_NONE, NON_PM, NON_PM, 1500L,
      NO_COLOR, SILVER),

    /*** Alignment quest artifacts ***/
    A("The Key Of Access", SKELETON_KEY,
      (SPFX_RESTR | SPFX_NOWISH | SPFX_NOGEN), 0, 0, 
      NO_ATTK, NO_DFNS, NO_CARY,
      CREATE_PORTAL, A_LAWFUL, NON_PM, NON_PM, 200L, NO_COLOR, GEMSTONE),
    
    A("The Staff of Rot", STAFF_OF_NECROMANCY,
      (SPFX_RESTR | SPFX_ATTK), 0, 0,
      WTHR(6, 6), NO_DFNS, NO_CARY,
      WITHER, A_CHAOTIC, NON_PM, NON_PM, 5000L, NO_COLOR, BONE),

    /* Xanathar's eyestalk ring of proof against detection and location */
    A("Xanathar's Ring of Proof", RIN_SEE_INVISIBLE,
      (SPFX_NOGEN | SPFX_NOWISH | SPFX_RESTR), SPFX_WARN, 0,
      NO_ATTK, DFNS(AD_MAGM), NO_CARY,
      INVIS, A_NEUTRAL, NON_PM, NON_PM, 2000L, NO_COLOR, DEFAULT_MAT),
    
    /*
     *      The artifacts for the quest dungeon, all self-willed.
     */

    /*
     *      At first I was going to add the artifact shield that was made for the
     *      Archeologist quest in UnNetHack, but then decided to do something unique.
     *      Behold Xiuhcoatl, a dark wooden spear-thrower that does fire damage much like
     *      Mjollnir does electrical damage. From Aztec lore, Xiuhcoatl is an atlatl,
     *      which is an ancient device that was used to throw spears with great force
     *      and even greater distances. Xiuhcoatl will return to the throwers hand much like
     *      Mjollnir, but requires high dexterity instead of strength to handle properly.
     */
    A("Xiuhcoatl", ATLATL,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_ATTK | SPFX_DEFN), SPFX_ESP, 0,
      FIRE(5, 24), DFNS(AD_FIRE), NO_CARY, LEVITATION, A_LAWFUL, PM_ARCHEOLOGIST,
      NON_PM, 3500L, NO_COLOR, DEFAULT_MAT),

#if 0 /* Replaced by Xiuhcoatl */
    A("The Orb of Detection", CRYSTAL_BALL,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL), (SPFX_ESP | SPFX_HSPDAM), 0,
      NO_ATTK, NO_DFNS, CARY(AD_MAGM), INVIS, A_LAWFUL, PM_ARCHEOLOGIST,
      NON_PM, 2500L, NO_COLOR, DEFAULT_MAT),
#endif

#if 0 /* Replaced by The Ring of P'hul */
    A("The Heart of Ahriman", LUCKSTONE,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL), SPFX_STLTH, 0,
      /* this stone does double damage if used as a projectile weapon */
      PHYS(5, 0), NO_DFNS, NO_CARY, LEVITATION, A_NEUTRAL, PM_BARBARIAN,
      NON_PM, 2500L, NO_COLOR, DEFAULT_MAT),
#endif

    A("The Sceptre of Might", ROD,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_DALIGN), 0, 0, PHYS(5, 0),
      DFNS(AD_MAGM), NO_CARY, CONFLICT, A_LAWFUL, PM_CAVEMAN, NON_PM, 2500L,
      NO_COLOR, METAL),

#if 0 /* OBSOLETE */
    A("The Palantir of Westernesse", CRYSTAL_BALL,
      (SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL),
      (SPFX_ESP|SPFX_REGEN|SPFX_HSPDAM), 0,
      NO_ATTK, NO_DFNS, NO_CARY,
      TAMING, A_CHAOTIC, NON_PM , PM_ELF, 8000L, NO_COLOR, DEFAULT_MAT),
#endif
    A("The Candle of Eternal Flame", MAGIC_CANDLE,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL), 
      (SPFX_WARN | SPFX_TCTRL | SPFX_EREGEN), 
      0, NO_ATTK, NO_DFNS, CARY(AD_FIRE), SUMMON_FIRE_ELEMENTAL, A_LAWFUL, 
      PM_FLAME_MAGE, NON_PM, 5000L, NO_COLOR, DEFAULT_MAT),
    
    A("The Staff of Aesculapius", QUARTERSTAFF,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_ATTK | SPFX_INTEL | SPFX_DRLI
       | SPFX_REGEN),
      0, 0, DRLI(5, 0), DFNS(AD_DRLI), NO_CARY, HEALING, A_NEUTRAL, PM_HEALER,
      NON_PM, 5000L, NO_COLOR, DEFAULT_MAT),
    
    A("The Storm Whistle", MAGIC_WHISTLE,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL), (SPFX_WARN | SPFX_TCTRL), 0,
      NO_ATTK, NO_DFNS, CARY(AD_COLD), SUMMON_WATER_ELEMENTAL,
      A_NEUTRAL, PM_ICE_MAGE, NON_PM, 1000L, NO_COLOR, DEFAULT_MAT),

    A("The Lightsaber Prototype", RED_LIGHTSABER,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_REFLECT), 0, 0,
      PHYS(5, 10), NO_DFNS, NO_CARY,
      ENERGY_BOOST, A_LAWFUL, PM_JEDI, NON_PM, 3500L, NO_COLOR, DEFAULT_MAT),
    
    A("The Magic Mirror of Merlin", MIRROR,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_SPEAK | SPFX_REFLECT),
      (SPFX_REFLECT | SPFX_ESP | SPFX_HSPDAM), 0,
      NO_ATTK, NO_DFNS, NO_CARY, 0, A_LAWFUL, PM_KNIGHT, NON_PM, 1500L,
      NO_COLOR, DEFAULT_MAT),

    A("The Eyes of the Overworld", LENSES,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_XRAY), 0, 0, NO_ATTK,
      DFNS(AD_MAGM), NO_CARY, ENLIGHTENING, A_NEUTRAL, PM_MONK, NON_PM,
      2500L, NO_COLOR, DEFAULT_MAT),
    
    A("The Great Dagger of Glaurgnaa", GREAT_DAGGER,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_ATTK | SPFX_INTEL | SPFX_DRLI | SPFX_DALIGN), 
      0, 0, DRLI(8, 4), DFNS(AD_MAGM), NO_CARY, ENERGY_BOOST, A_CHAOTIC, 
      PM_NECROMANCER, NON_PM, 5000L, NO_COLOR, DEFAULT_MAT),
    
    A("The Mitre of Holiness", HELM_OF_BRILLIANCE,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_DFLAGH | SPFX_INTEL | SPFX_PROTECT), 0,
      MH_UNDEAD, NO_ATTK, NO_DFNS, CARY(AD_FIRE), ENERGY_BOOST, A_LAWFUL,
      PM_PRIEST, NON_PM, 2000L, NO_COLOR, METAL),

    A("The Treasury of Proteus", CHEST,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL), SPFX_LUCK, 0, NO_ATTK, NO_DFNS,
      CARY(AD_MAGM), 0,	A_CHAOTIC, PM_PIRATE, NON_PM, 
      2500L, NO_COLOR, DEFAULT_MAT),
    
    /* If playing a gnomish ranger, the player receives the 'Crossbow of Carl',
       otherwise rangers will receive the Longbow of Diana. Exact same properties
       between the two artifacts */
    A("The Longbow of Diana", BOW,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_REFLECT), SPFX_ESP, 0,
      PHYS(5, 0), NO_DFNS, NO_CARY, CREATE_AMMO, A_CHAOTIC, PM_RANGER, NON_PM,
      4000L, NO_COLOR, DEFAULT_MAT),

    A("The Crossbow of Carl", CROSSBOW,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_REFLECT), SPFX_ESP, 0,
      PHYS(5, 0), NO_DFNS, NO_CARY, CREATE_AMMO, A_CHAOTIC, PM_RANGER, NON_PM,
      4000L, NO_COLOR, DEFAULT_MAT),

    /* MKoT has an additional carry property if the Key is not cursed (for
       rogues) or blessed (for non-rogues):  #untrap of doors and chests
       will always find any traps and disarming those will always succeed */
    A("The Master Key of Thievery", SKELETON_KEY,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_SPEAK),
      (SPFX_WARN | SPFX_TCTRL | SPFX_HPHDAM), 0, NO_ATTK, NO_DFNS, NO_CARY,
      UNTRAP, A_CHAOTIC, PM_ROGUE, NON_PM, 3500L, NO_COLOR, DEFAULT_MAT),

    A("The Tsurugi of Muramasa", TSURUGI,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_BEHEAD | SPFX_LUCK
       | SPFX_PROTECT | SPFX_HPHDAM), 0, 0,
      PHYS(3, 8), NO_DFNS, NO_CARY, 0, A_LAWFUL, PM_SAMURAI, NON_PM,
      6000L, NO_COLOR, DEFAULT_MAT),

    A("The Platinum Yendorian Express Card", CREDIT_CARD,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_DEFN),
      (SPFX_ESP | SPFX_HSPDAM), 0, NO_ATTK, NO_DFNS, CARY(AD_MAGM),
      CHARGE_OBJ, A_NEUTRAL, PM_TOURIST, NON_PM, 7000L, NO_COLOR, PLATINUM),

    /* Provides warning vs vampires. Still does bonus damage vs all.
    */
    A("The Stake of Van Helsing", WOODEN_STAKE,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_WARN | SPFX_DFLAGH), 
      0, MH_VAMPIRE,
      PHYS(5, 12), DFNS(AD_MAGM), NO_CARY, 0, A_LAWFUL, 
      PM_UNDEAD_SLAYER, NON_PM, 5000L, CLR_RED, DEFAULT_MAT),

#if 0 /* Replaced by Gjallar */
    A("The Orb of Fate", CRYSTAL_BALL,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_LUCK),
      (SPFX_WARN | SPFX_HPHDAM), 0, NO_ATTK, NO_DFNS, NO_CARY,
      LEV_TELE, A_NEUTRAL, PM_VALKYRIE, NON_PM, 3500L, NO_COLOR, DEFAULT_MAT),
#endif

    A("Gjallar", TOOLED_HORN,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_LUCK),
      (SPFX_WARN | SPFX_HPHDAM), 0, NO_ATTK, NO_DFNS, NO_CARY,
      LEV_TELE, A_NEUTRAL, PM_VALKYRIE, NON_PM, 5000L, NO_COLOR, BONE),

    A("The Eye of the Aethiopica", AMULET_OF_ESP,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL), (SPFX_EREGEN | SPFX_HSPDAM), 0,
      NO_ATTK, DFNS(AD_MAGM), NO_CARY, CREATE_PORTAL, A_NEUTRAL, PM_WIZARD,
      NON_PM, 4000L, NO_COLOR, DEFAULT_MAT),
    
    A("The Crown of Saint Edward", HELM_OF_TELEPATHY,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL), (SPFX_HSPDAM), 0,
      NO_ATTK, DFNS(AD_MAGM), NO_CARY, 0,  
      A_LAWFUL, PM_YEOMAN, NON_PM, 5000L, NO_COLOR, DEFAULT_MAT),
    
    /*
     *      Based loosely off of the Ring of P'hul - from 'The Lords of Dus' series
     *      by Lawrence Watt-Evans. This is another one of those artifacts that would
     *      just be ridiculous if its full power were realized in-game. In the series,
     *      it deals out death and disease. Here it will protect the wearer from a
     *      good portion of that. Making this the quest artifact for the Barbarian role.
     *      This artifact also corrects an oversight from vanilla, that no chaotic-based
     *      artiafcts conferred magic resistance, a problem that was compounded if our
     *      hero is in a form that can't wear body armor or cloaks. So, we make the
     *      Barbarian artifact chaotic (why it was neutral before is a bit confusing
     *      to me as most vanilla Barbarian race/role combinations are chaotic).
     */
    A("The Ring of P\'hul", RIN_FREE_ACTION,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_DEFN), 0, 0,
      NO_ATTK, DFNS(AD_MAGM), CARY(AD_DISE), 0, A_CHAOTIC, PM_BARBARIAN,
      NON_PM, 5000L, NO_COLOR, GEMSTONE),

#if 0 /* --hackem: Disabled */
    /* Convict role quest artifact. Provides magic resistance when carried,
     * invoke to phase through walls like a xorn.
     */
    A("The Iron Ball of Liberation", HEAVY_IRON_BALL,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL),
      (SPFX_STLTH | SPFX_SEARCH | SPFX_WARN | SPFX_FREE), 0,
      NO_ATTK, NO_DFNS, CARY(AD_MAGM), PHASING,
      A_CHAOTIC, PM_CONVICT, NON_PM, 10000L, NO_COLOR, DEFAULT_MAT),
#endif
    
    /* Convict role quest artifact (from dnh)
     * has a +1d5 to-hit bonus and does double damage to all monsters. 
     * When carried, it confers automatic searching, free action, and luck, 
     * when wielded, it confers stealth. 
     * Invoking allows you to phase through walls for a limited time, 
     * Applying The Iron Spoon of Liberation allows you to dig; 
     * It can also be used to engrave with the same quality and speed as an athame.
     * Also - As a spoon, Convicts wielding it can make sneak attacks similar to a Rogue.
     */
    /* --hackem: I added stealth to the carry effects, since we don't have
     * some of these extra fields in dnh. */
    A("The Iron Spoon of Liberation", SPOON,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_STLTH), 
      (SPFX_SEARCH | SPFX_SEEK | SPFX_LUCK), 0,
      PHYS(5,0), NO_DFNS, CARY(AD_PLYS), PHASING, 
      A_CHAOTIC, PM_CONVICT, NON_PM, 5000L, NO_COLOR, DEFAULT_MAT),
    
    /* Infidel role quest artifact. Confers energy regeneration,
     * but only to those in good standing with Moloch. */
    
    A("The Idol of Moloch", FIGURINE,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL), SPFX_HSPDAM, 0,
      NO_ATTK, NO_DFNS, CARY(AD_MAGM), CHANNEL, A_CHAOTIC, PM_INFIDEL, NON_PM,
      4000L, NO_COLOR, DEFAULT_MAT),

    /*
     *  terminator; otyp must be zero
     */
    A(0, 0, 0, 0, 0, NO_ATTK, NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 0L,
      0, 0) /* 0 is CLR_BLACK rather than NO_COLOR but it doesn't matter here */

}; /* artilist[] (or artifact_names[]) */

#undef A

#ifndef MAKEDEFS_C
#undef NO_ATTK
#undef NO_DFNS
#undef DFNS
#undef PHYS
#undef DRLI
#undef COLD
#undef FIRE
#undef ELEC
#undef STUN
#undef WIND
#undef DRST
#undef ACID
#undef DISE
#undef DREN
#undef STON
#undef DETH
#undef PLYS
#undef LOUD
#undef WTHR
#endif

/*artilist.h*/
