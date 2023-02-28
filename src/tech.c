/*      SCCS Id: @(#)tech.c    3.2     98/Oct/30        */
/*      Original Code by Warren Cheung (Basis: spell.c, attrib.c) */
/*      Copyright (c) M. Stephenson 1988                          */
/* NetHack may be freely redistributed.  See license for details. */

/* All of the techs from cmd.c are ported here */

#include "hack.h"

/* #define DEBUG */		/* turn on for diagnostics */

static boolean FDECL(procdmg, (struct monst*, int, int));
static boolean FDECL(gettech, (int *));
static boolean FDECL(dotechmenu, (int, int *));
static void NDECL(doblitzlist);
static int FDECL(get_tech_no,(int));
static int FDECL(techeffects, (int));
STATIC_PTR int NDECL(tinker);
STATIC_PTR int NDECL(charge_saber);
STATIC_PTR int NDECL(draw_energy);
static const struct innate_tech * NDECL(role_tech);
static const struct innate_tech * NDECL(race_tech);
static int NDECL(doblitz);
static int NDECL(blitz_chi_strike);
static int NDECL(blitz_e_fist);
static int NDECL(blitz_pummel);
static int NDECL(blitz_g_slam);
static int NDECL(blitz_uppercut);
static int NDECL(blitz_dash);
static int NDECL(blitz_power_surge);
static int NDECL(blitz_spirit_bomb);
static int FDECL(tech_spirittempest, (int));
static int FDECL(tech_surgery, (int));
static int FDECL(tech_healhands, (int));
static int FDECL(tech_kiii, (int));
static int FDECL(tech_calmsteed, (int));
static int FDECL(tech_vanish, (int));
static int FDECL(tech_critstrike, (int));
static int FDECL(tech_cutthroat, (int));
static int FDECL(tech_blessing, (int));
static int FDECL(tech_curse, (int));
static int FDECL(tech_research, (int));
static int FDECL(tech_eviscerate, (int));
static int FDECL(tech_berserk, (int));
static int FDECL(tech_reinforce, (int));
static int FDECL(tech_flurry, (int));
static int FDECL(tech_appraisal, (int));
static int FDECL(tech_practice, (int));
static int FDECL(tech_efist, (int));
static int FDECL(tech_primalroar, (int));
static int FDECL(tech_liquidleap, (int));
static int FDECL(tech_raisezombies, (int));
static int FDECL(tech_callundead, (int));
static int FDECL(tech_revive, (int));
static int FDECL(tech_tinker, (int));
static int FDECL(tech_rage, (int));
static int FDECL(tech_blink, (int));
static int FDECL(tech_drawenergy, (int));
static int FDECL(tech_chiheal, (int));
static int FDECL(tech_pickpocket, (int));
static int FDECL(tech_disarm, (int));
static int FDECL(tech_dazzle, (int));
static int FDECL(tech_drawblood, (int));
static int FDECL(tech_jedijump, (int));
static int FDECL(tech_booze, (int));
static int FDECL(tech_souleater, (int));
static int FDECL(tech_shieldblock, (int));
static int FDECL(tech_telekinesis, (int));
static int FDECL(tech_forcepush, (int));
static int FDECL(tech_chargesaber, (int));
static int FDECL(tech_tumble, (int));
static int FDECL(tech_sunder, (int));
static int FDECL(tech_bloodmagic, (int));

static NEARDATA schar delay;            /* moves left for tinker/energy draw */

/* Duplicated from apply.c */
static const char no_elbow_room[] =
    "don't have enough elbow-room to maneuver.";
/* 
 * Do try to keep the names <= 25 chars long, or else the
 * menu will look bad :B  WAC
 */
 
STATIC_OVL NEARDATA const char *tech_names[] = {
    "no technique",     /* 0 */
    "berserk",          /* 1 */
    "kiii",             /* 2 */
    "research",         /* 3 */
    "surgery",          /* 4 */
    "reinforce memory", /* 5 */
    "missile flurry",   /* 6 */
    "weapon practice",  /* 7 */
    "eviscerate",       /* 8 */
    "healing hands",    /* 9 */
    "calm steed",       /* 10 */
    "turn undead",      /* 11 */
    "vanish",           /* 12 */
    "cutthroat",        /* 13 */
    "blessing",         /* 14 */
    "elemental fist",   /* 15 */
    "primal roar",      /* 16 */
    "liquid leap",      /* 17 */
    "critical strike",  /* 18 */
    "raise zombies",    /* 19 */
    "revivification",   /* 20 */
    "tinker",           /* 21 */
    "rage eruption",    /* 22 */
    "blink",            /* 23 */
    "chi strike",       /* 24 */
    "draw energy",      /* 25 */
    "chi healing",      /* 26 */
    "disarm",           /* 27 */
    "dazzle",           /* 28 */
    "chained blitz",    /* 29 */
    "pummel",           /* 30 */
    "ground slam",      /* 31 */
    "air dash",         /* 32 */
    "power surge",      /* 33 */
    "spirit bomb",      /* 34 */
    "draw blood",       /* 35 */
    /* SLEX techs */
    "world fall",       /* 36 */
    "create ammo",      /* 37 */
    "appraisal",        /* 38 */
    "booze",            /* 39 */
    /* SlashTHEM techs */
    "souleater",        /* 40 */
    "shield block",     /* 41 */
    "jedi jump",        /* 42 */
    "charge saber",     /* 43 */
    "telekinesis",      /* 44 */
    "whistle undead",      /* 45 */
    "spirit tempest",   /* 46 */
    "force push",       /* 47 */
    "curse",            /* 48 */
    "pickpocket",       /* 49 */
    "tumble",           /* 50 */
    "sunder",           /* 51 */
    "blood magic",      /* 52 */
    "break rock",       /* 53 */
    "uppercut",         /* 54 */
    "",            /* 55 */
    ""
};

/* Roles */
static const struct innate_tech
    arc_tech[] = { 
        { 1, T_RESEARCH, 1 },
        { 0, 0, 0 } 
    },
    bar_tech[] = { 
        { 1, T_BERSERK, 1 },
        { 5, T_RAGE, 1 },
        { 0, 0, 0 } 
    },
    cav_tech[] = { 
        { 1, T_PRIMAL_ROAR, 1 },
        { 0, 0, 0 } 
    },
    fla_tech[] = { 
        { 1, T_REINFORCE, 1 },
        { 3, T_POWER_SURGE, 1 },
        { 5, T_DRAW_ENERGY, 1 },
        { 0, 0, 0 } 
    },
    hea_tech[] = { 
        { 1, T_SURGERY, 1 },
        { 0, 0, 0 } 
    },
    ice_tech[] = { 
        { 1, T_REINFORCE, 1 },
        { 5, T_DRAW_ENERGY, 1 },
        { 12, T_POWER_SURGE, 1 },
        { 0, 0, 0 } 
    },
    inf_tech[] = { 
        { 1, T_CURSE, 1 },
        { 1, T_REINFORCE, 1 },
        { 5, T_DRAW_ENERGY, 1 },
        { 0, 0, 0 } 
    },
    jed_tech[] = { 
        { 1, T_JEDI_JUMP, 1 },
        { 5, T_CHARGE_SABER, 1 },
        { 8, T_TELEKINESIS, 1 },
        { 14, T_FORCE_PUSH, 1 },
        { 0, 0, 0 } 
    },
    kni_tech[] = { 
        { 1, T_TURN_UNDEAD, 1 },
        { 1, T_HEAL_HANDS, 1 },
        { 1, T_SHIELD_BLOCK, 1 },
        { 0, 0, 0 } 
    },
        /* 5lo: for Dark Knights */
    drk_tech[] = { 
        { 1, T_SOULEATER, 1 },
        { 1, T_SHIELD_BLOCK, 1 },
        { 0, 0, 0 } 
    },
    mon_tech[] = { 
        { 1, T_PUMMEL, 1 },
        { 1, T_DASH, 1 },
        { 1, T_BLITZ, 1 },
        { 1, T_BREAK_ROCK, 1 },
        { 2, T_CHI_STRIKE, 1 },
        { 4, T_CHI_HEALING, 1 },
        { 6, T_E_FIST, 1 },
        { 8, T_DRAW_ENERGY, 1 },
        { 10, T_G_SLAM, 1 },
        { 13, T_UPPERCUT, 1 },
        { 17, T_SPIRIT_BOMB, 1 },
        { 20, T_POWER_SURGE, 1 },
        { 0, 0, 0 }
    },
    nec_tech[] = { 
        { 1, T_REINFORCE, 1 },
        { 2, T_WHISTLE_UNDEAD, 1 },
        { 3, T_RAISE_ZOMBIES, 1 },
        { 5, T_BLOOD_MAGIC, 1 },
        { 8, T_SOULEATER, 1 },
        { 10, T_POWER_SURGE, 1 },
        { 14, T_REVIVE, 1 },
        { 17, T_SPIRIT_TEMPEST, 1 },
        { 0, 0, 0 } 
    },
    pri_tech[] = { 
        { 1, T_TURN_UNDEAD, 1 },
        { 1, T_BLESSING, 1 },
        { 7, T_SHIELD_BLOCK, 1 },
        { 10, T_HEAL_HANDS, 1 },
        { 30, T_REVIVE, 1 },
        { 0, 0, 0 } 
    },
    pir_tech[] = {
        { 1, T_PRACTICE, 1 },
        { 1, T_TUMBLE, 1 },
        { 5, T_SUNDER, 1 },
        { 0, 0, 0 } 
    },
    ran_tech[] = { 
        { 1, T_FLURRY, 1 },
        { 0, 0, 0 } 
    },
    rog_tech[] = { 
        { 1, T_PICKPOCKET, 1 },
        { 1, T_CRIT_STRIKE, 1 },
        { 15, T_CUTTHROAT, 1 },
        { 0, 0, 0 } 
    },
    sam_tech[] = { 
        { 1, T_KIII, 1 },
        { 0, 0, 0 } 
    },
    tou_tech[] = {
        { 0, 0, 0 } 
    },
    und_tech[] = { 
        { 1, T_TURN_UNDEAD, 1 },
        { 1, T_PRACTICE, 1 },
        { 7, T_SHIELD_BLOCK, 1 },
        { 0, 0, 0 } 
    },
    val_tech[] = { 
        { 1, T_PRACTICE, 1 },
        { 1, T_SHIELD_BLOCK, 1 },
        { 0, 0, 0 } 
    },
    yeo_tech[] = {
        { 1, T_CALM_STEED, 1 },
        { 0, 0, 0 } 
    },
    wiz_tech[] = { 
        { 1, T_REINFORCE, 1 },
        { 3, T_DRAW_ENERGY, 1 },
        { 5, T_POWER_SURGE, 1 },
        { 0, 0, 0 } 
    },
#if 0 
    dru_tech[] = { 
        { 1, T_BOOZE, 1 }, /* drunk */
        { 0, 0, 0 } 
    },
    brd_tech[] = { 
        { 1, T_PRIMAL_ROAR, 1 }, /* bard */
        {  10, T_BOOZE, 1 },
        {  20, T_RAGE, 1 },
        { 0, 0, 0 } 
    },
    nin_tech[] = { 
        { 1, T_VANISH, 1 }, /* ninja */
        { 1, T_CRIT_STRIKE, 1 },
        {  10, T_CUTTHROAT, 1 },
        { 0, 0, 0 } 
    },
    war_tech[] = { 
        { 1, T_PRACTICE, 1 }, /* warrior */
        { 1, T_SHIELD_BLOCK, 1 },
        { 0, 0, 0 } 
    },
    lun_tech[] = { 
        { 1, T_EVISCERATE, 1 }, /* lunatic */
        {  10, T_BERSERK, 1 },
        { 0, 0, 0 } 
    },
#endif
    /* Races */
    dwa_tech[] = { 
        { 1, T_RAGE, 1 },
        { 0, 0, 0 } 
    },
    elf_tech[] = {
        { 0, 0, 0 }
    },
    gno_tech[] = { 
        { 1, T_VANISH, 1 },
        {   7, T_TINKER, 1 },
        { 0, 0, 0 } 
    },
    hob_tech[] = { 
        { 1, T_BLINK, 1 },
        { 0, 0, 0 } 
    },
    tor_tech[] = {
        { 0, 0, 0 } 
    },
    vam_tech[] = { 
        { 1, T_DAZZLE, 1 },
        { 1, T_DRAW_BLOOD, 1 },
        { 0, 0, 0 } 
    },
    dop_tech[] = { 
        { 1, T_LIQUID_LEAP, 1 },
        { 0, 0, 0 } 
    };

#if 0 /* Deferred races */
    lyc_tech[] = { 
        { 1, T_EVISCERATE, 1 },
        {  10, T_BERSERK, 1 },
        { 0, 0, 0 } 
    },
    inc_tech[] = { 
        { 1, T_DRAW_ENERGY, 1 },
        { 0, 0, 0 } 
    };
#endif
/* Local Macros 
 * these give you direct access to the player's list of techs.  
 * Are you sure you don't want to use tech_inuse,  which is the
 * extern function for checking whether a fcn is inuse
 */

#define techt_inuse(tech)       tech_list[tech].t_inuse
#define techtout(tech)        tech_list[tech].t_tout
#define techlev(tech)         (u.ulevel - tech_list[tech].t_lev)
#define techid(tech)          tech_list[tech].t_id
#define techname(tech)        (tech_names[techid(tech)])
#define techlet(tech)  \
        ((char)((tech < 26) ? ('a' + tech) : ('A' + tech - 26)))

/* Helper function to help with tech damage (and double damage) */
static boolean
procdmg(mtmp, amount, type)
struct monst *mtmp;
int amount, type;
{
    if (dbl_dmg())
        amount *= 2;

    if (damage_mon(mtmp, amount, type)) {
        killed(mtmp);
        return TRUE;
    }
    return FALSE;

}
/* Whether you know the tech */
boolean
tech_known(tech)
short tech;
{
    int i;
    for (i = 0; i < MAXTECH; i++) {
            if (techid(i) == tech) 
                 return TRUE;
    }
    return FALSE;
}

/* Called to prematurely stop a technique */
void
aborttech(tech)
int tech;
{
    int i;

    i = get_tech_no(tech);
    if (tech_list[i].t_inuse) {
        switch (tech_list[i].t_id) {
        case T_RAGE:
            u.uhpmax -= tech_list[i].t_inuse - 1;
            if (u.uhpmax < 1)
                u.uhpmax = 0;
            u.uhp -= tech_list[i].t_inuse - 1;
            if (u.uhp < 1)
                u.uhp = 1;
            break;
        case T_POWER_SURGE:
            u.uenmax -= tech_list[i].t_inuse - 1;
            if (u.uenmax < 1)
                u.uenmax = 0;
            u.uen -= tech_list[i].t_inuse - 1;
            if (u.uen < 0)
                u.uen = 0;
            break;
        }
        tech_list[i].t_inuse = 0;
    }
}

/* Called to teach a new tech.  Level is starting tech level */
void
learntech(tech, mask, tlevel)
short tech;
long mask;
int tlevel;
{
    int i;
    const struct innate_tech *tp;

    i = get_tech_no(tech);
    if (tlevel > 0) {
        if (i < 0) {
            i = get_tech_no(NO_TECH);
            if (i < 0) {
                impossible("No room for new technique?");
                return;
            }
        }
        tlevel = u.ulevel ? u.ulevel - tlevel : 0;
        if (tech_list[i].t_id == NO_TECH) {
            tech_list[i].t_id = tech;
            tech_list[i].t_lev = tlevel;
            tech_list[i].t_inuse = 0; /* not in use */
            tech_list[i].t_intrinsic = 0;
        }
        else if (tech_list[i].t_intrinsic & mask) {
            impossible("Tech already known.");
            return;
        }
        if (mask == FROMOUTSIDE) {
            tech_list[i].t_intrinsic &= ~OUTSIDE_LEVEL;
            tech_list[i].t_intrinsic |= tlevel & OUTSIDE_LEVEL;
        }
        if (tlevel < tech_list[i].t_lev)
            tech_list[i].t_lev = tlevel;
        tech_list[i].t_intrinsic |= mask;
        tech_list[i].t_tout = 0; /* Can use immediately*/
    }
    else if (tlevel < 0) {
        if (i < 0 || !(tech_list[i].t_intrinsic & mask)) {
            impossible("Tech not known.");
            return;
        }
        tech_list[i].t_intrinsic &= ~mask;
        if (!(tech_list[i].t_intrinsic & INTRINSIC)) {
            if (tech_list[i].t_inuse)
                aborttech(tech);
            tech_list[i].t_id = NO_TECH;
            return;
        }
        /* Re-calculate lowest t_lev */
        if (tech_list[i].t_intrinsic & FROMOUTSIDE)
            tlevel = tech_list[i].t_intrinsic & OUTSIDE_LEVEL;
        if (tech_list[i].t_intrinsic & FROMEXPER) {
            for (tp = role_tech(); tp->tech_id; tp++)
                if (tp->tech_id == tech)
                    break;
            if (!tp->tech_id)
                impossible("No inate technique for role?");
            else if (tlevel < 0 || tp->ulevel - tp->tech_lev < tlevel)
                tlevel = tp->ulevel - tp->tech_lev;
        }
        if (tech_list[i].t_intrinsic & FROMRACE) {
            for (tp = race_tech(); tp->tech_id; tp++)
                if (tp->tech_id == tech)
                    break;
            if (!tp->tech_id)
                impossible("No inate technique for race?");
            else if (tlevel < 0 || tp->ulevel - tp->tech_lev < tlevel)
                tlevel = tp->ulevel - tp->tech_lev;
        }
        tech_list[i].t_lev = tlevel;
    }
    else
        impossible("Invalid Tech Level!");
}

/*
 * Return TRUE if a tech was picked, with the tech index in the return
 * parameter.  Otherwise return FALSE.
 */
static boolean
gettech(tech_no)
int *tech_no;
{
    int i, ntechs, idx;
    char ilet, lets[BUFSZ], qbuf[QBUFSZ];

    for (ntechs = i = 0; i < MAXTECH; i++)
        if (techid(i) != NO_TECH) 
            ntechs++;
    if (ntechs == 0)  {
        You("don't know any techniques right now.");
        return FALSE;
    }
    if (flags.menu_style == MENU_TRADITIONAL) {
        if (ntechs == 1)  
            Strcpy(lets, "a");
        else if (ntechs < 27)
            Sprintf(lets, "a-%c", 'a' + ntechs - 1);
        else if (ntechs == 27) 
            Sprintf(lets, "a-z A");
        else 
            Sprintf(lets, "a-z A-%c", 'A' + ntechs - 27);

        for (;;)  {
            Sprintf(qbuf, "Perform which technique? [%s ?]", lets);
            if ((ilet = yn_function(qbuf, (char *)0, '\0')) == '?')
                break;

            if (index(quitchars, ilet))
                return FALSE;

            if (letter(ilet) && ilet != '@') {
                /* in a-zA-Z, convert back to an index */
                if (lowc(ilet) == ilet)     /* lower case */
                    idx = ilet - 'a';
                else
                    idx = ilet - 'A' + 26;

                if (idx < ntechs)
                    for (i = 0; i < MAXTECH; i++)
                        if (techid(i) != NO_TECH) {
                            if (idx-- == 0) {
                                *tech_no = i;
                                return TRUE;
                            }
                        }
            }
            You("don't know that technique.");
        }
    }
    return dotechmenu(PICK_ONE, tech_no);
}

static boolean
dotechmenu(how, tech_no)
int how;
int *tech_no;
{
    winid tmpwin;
    int i, n, len, longest, techs_useable, tlevel;
    char buf[BUFSZ], let = 'a';
    const char *prefix;
    menu_item *selected;
    anything any;

    tmpwin = create_nhwindow(NHW_MENU);
    start_menu(tmpwin);
    any.a_void = 0;         /* zero out all bits */

    techs_useable = 0;

    if (!iflags.menu_tab_sep) {
        /* find the length of the longest tech */
        for (longest = 0, i = 0; i < MAXTECH; i++) {
            if (techid(i) == NO_TECH) 
                continue;
            if ((len = strlen(techname(i))) > longest)
                longest = len;
        }
        Sprintf(buf, "    %-*s Level   Status", longest, "Name");
    } else
        Sprintf(buf, "Name\tLevel\tStatus");

    add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, buf, MENU_UNSELECTED);

    for (i = 0; i < MAXTECH; i++) {
        if (techid(i) == NO_TECH)
            continue;
        tlevel = techlev(i);
        if (!techtout(i) && tlevel > 0) {
            /* Ready to use */
            techs_useable++;
            prefix = "";
            any.a_int = i + 1;
        } else {
            prefix = "    ";
            any.a_int = 0;
        }
#ifdef WIZARD
        if (wizard) 
            if (!iflags.menu_tab_sep)			
                Sprintf(buf, "%s%-*s %2d%c%c%c   %s(%i)",
                        prefix, longest, techname(i), tlevel,
                        tech_list[i].t_intrinsic & FROMEXPER ? 'X' : ' ',
                        tech_list[i].t_intrinsic & FROMRACE ? 'R' : ' ',
                        tech_list[i].t_intrinsic & FROMOUTSIDE ? 'O' : ' ',
                        tech_inuse(techid(i)) ? "Active" :
                        tlevel <= 0 ? "Beyond recall" :
                        !techtout(i) ? "Prepared" : 
                        techtout(i) > 10000 ? "Huge timeout" :
                        techtout(i) > 1000 ? "Not Ready" :
                        techtout(i) > 100 ? "Reloading" : "Soon",
                        techtout(i));
            else
                Sprintf(buf, "%s%s\t%2d%c%c%c\t%s(%i)",
                        prefix, techname(i), tlevel,
                        tech_list[i].t_intrinsic & FROMEXPER ? 'X' : ' ',
                        tech_list[i].t_intrinsic & FROMRACE ? 'R' : ' ',
                        tech_list[i].t_intrinsic & FROMOUTSIDE ? 'O' : ' ',
                        tech_inuse(techid(i)) ? "Active" :
                        tlevel <= 0 ? "Beyond recall" :
                        !techtout(i) ? "Prepared" : 
                        techtout(i) > 10000 ? "Huge timeout" :
                        techtout(i) > 1000 ? "Not Ready" :
                        techtout(i) > 100 ? "Reloading" : "Soon",
                        techtout(i));
        else
#endif
        
        if (!iflags.menu_tab_sep)			
             Sprintf(buf, "%s%-*s %5d   %s",
                    prefix, longest, techname(i), tlevel,
                    tech_inuse(techid(i)) ? "Active" :
                    tlevel <= 0 ? "Beyond recall" :
                    !techtout(i) ? "Prepared" : 
                    techtout(i) > 10000 ? "Huge timeout" :
                    techtout(i) > 1000 ? "Not Ready" :
                    techtout(i) > 100 ? "Reloading" : "Soon");
        else
             Sprintf(buf, "%s%s\t%5d\t%s",
                    prefix, techname(i), tlevel,
                    tech_inuse(techid(i)) ? "Active" :
                    tlevel <= 0 ? "Beyond recall" :
                    !techtout(i) ? "Prepared" : 
                    techtout(i) > 10000 ? "Huge timeout" :
                    techtout(i) > 1000 ? "Not Ready" :
                    techtout(i) > 100 ? "Reloading" : "Soon");

        add_menu(tmpwin, NO_GLYPH, &any,
                 techtout(i) ? 0 : let, 0, ATR_NONE, buf, MENU_UNSELECTED);
        if (let++ == 'z') 
            let = 'A';
    }

    if (!techs_useable) 
        how = PICK_NONE;

    end_menu(tmpwin, how == PICK_ONE ? "Choose a technique" :
                                       "Currently known techniques");

    n = select_menu(tmpwin, how, &selected);
    destroy_nhwindow(tmpwin);
    if (n > 0) {
        int selection = selected[0].item.a_int - 1;
        /* This really shouldn't happen, and yet it does */
        if (selection < 0) {
            free((genericptr_t)selected);
            return FALSE;
        }
        *tech_no = selection;
        free((genericptr_t)selected);
        return TRUE;
    }
    return FALSE;
}

#ifdef DUMP_LOG
void 
dump_techniques()
{

    winid tmpwin;
    int i, n, len, tlevel;
    char buf[BUFSZ];
    const char *prefix;

    if (techid(0) == NO_TECH) {
        dump("", "You didn't know any techniques.");
        dump("", "");
        return;
    }
    dump("", "Techniques known in the end");

    Sprintf(buf, "    %-17s Level  Status", "Name");
    dump("  ",buf);
    for (i = 0; i < MAXTECH; i++) {
        if (techid(i) == NO_TECH)
            continue;
        tlevel = techlev(i);
        if (!techtout(i) && tlevel > 0) {
            /* Ready to use */
            prefix = "";
        } else {
            prefix = "    ";
        }
        if (!iflags.menu_tab_sep) {		
            Sprintf(buf, "%s%-20s %2d%c%c%c   %s(%i)",
                    prefix, techname(i), tlevel,
                    tech_list[i].t_intrinsic & FROMEXPER ? 'X' : ' ',
                    tech_list[i].t_intrinsic & FROMRACE ? 'R' : ' ',
                    tech_list[i].t_intrinsic & FROMOUTSIDE ? 'O' : ' ',
                    tech_inuse(techid(i)) ? "Active" :
                    tlevel <= 0 ? "Beyond recall" :
                    !techtout(i) ? "Prepared" : 
                    techtout(i) > 10000 ? "Huge timeout" :
                    techtout(i) > 1000 ? "Not Ready" :
                    techtout(i) > 100 ? "Reloading" : "Soon",
                    techtout(i));
            dump("  ", buf);
        } else {
            Sprintf(buf, "%s%s\t%2d%c%c%c\t%s(%i)",
                    prefix, techname(i), tlevel,
                    tech_list[i].t_intrinsic & FROMEXPER ? 'X' : ' ',
                    tech_list[i].t_intrinsic & FROMRACE ? 'R' : ' ',
                    tech_list[i].t_intrinsic & FROMOUTSIDE ? 'O' : ' ',
                    tech_inuse(techid(i)) ? "Active" :
                    tlevel <= 0 ? "Beyond recall" :
                    !techtout(i) ? "Prepared" : 
                    techtout(i) > 10000 ? "Huge timeout" :
                    techtout(i) > 1000 ? "Not Ready" :
                    techtout(i) > 100 ? "Reloading" : "Soon",
                    techtout(i));
            dump("  ", buf);
        }
    }
    dump("", "");

} /* dump_techniques */
#endif

static int
get_tech_no(tech)
int tech;
{
    int i;
    for (i = 0; i < MAXTECH; i++) {
        if (techid(i) == tech) {
            return (i);
        }
    }
    return -1;
}

int
dotech()
{
    int tech_no;

    if (gettech(&tech_no))
        return techeffects(tech_no);
    return 0;
}

static NEARDATA const char kits[] = { TOOL_CLASS, 0 };

static struct obj *
use_medical_kit(type, feedback, verb)
int type;
boolean feedback;
const char *verb;
{
    struct obj *obj, *otmp;
    makeknown(MEDICAL_KIT);
    if (!(obj = carrying(MEDICAL_KIT))) {
	if (feedback) 
            You("need a medical kit to do that.");
	return (struct obj *)0;
    }
    for (otmp = invent; otmp; otmp = otmp->nobj)
	if (otmp->otyp == MEDICAL_KIT && otmp != obj)
	    break;
    if (otmp) {	/* More than one medical kit */
	obj = getobj(kits, verb);
	if (!obj)
	    return (struct obj *)0;
    }
    for (otmp = obj->cobj; otmp; otmp = otmp->nobj)
	if (otmp->otyp == type)
	    break;
    if (!otmp) {
	if (feedback)
	    You_cant("find any more %s in %s.",
		    makeplural(simple_typename(type)), yname(obj));
	return (struct obj *)0;
    }
    return otmp;
}


static NEARDATA int dpx, dpy; /* boulder being attacked */
static NEARDATA int breakturns; /* concentration turn */

STATIC_OVL int
ma_break(VOID_ARGS)
{
    struct obj *bobj, *obj;
    int prob;

    if (distu(dpx, dpy) > 2) {
        Your("qi dissipates in all directions.");
        make_confused(6 + breakturns * P_SKILL(P_MARTIAL_ARTS), FALSE);
        return 0;
    }

    if (breakturns < 40 / P_SKILL(P_MARTIAL_ARTS)) {
        breakturns++;
        return 1; /* still concentrating */
    }

    if ((obj = sobj_at(BOULDER, dpx, dpy)) != 0)
        pline("Focusing your qi, you %s the boulder.",
              rn2(2) ? "strike" : "hit");
    else if ((obj = sobj_at(STATUE, dpx, dpy)) !=0)
        pline("Focusing your qi, you %s the statue.",
              rn2(2) ? "strike" : "hit");

    /* even while blind you can first feel and then image the boulder */
    if (Confusion || Hallucination || Stunned) {
        if ((obj = sobj_at(BOULDER, dpx, dpy)) != 0) {
            if (rn2(2)) {
                You("swing wildly, missing the boulder.");
            } else {
                You("slip, hitting your head against the boulder!");
                losehp(d(1, 4), "face planting into a boulder.",
                       NO_KILLER_PREFIX);
            }
        } else if ((obj = sobj_at(STATUE, dpx, dpy)) != 0) {
            if (rn2(2)) {
                You("swing wildly, missing the statue.");
            } else {
                You("slip, hitting your head against the statue!");
                losehp(d(1, 4), "trying to headbutt a statue.",
                       NO_KILLER_PREFIX);
            }
        }
        return 0;
    }

    prob = 40;
    prob -= u.ulevel;
    prob /= P_SKILL(P_MARTIAL_ARTS);

    /* inherently strong races get a probability bonus,
     * inherently weak races, not so much */
    if (Race_if(PM_GIANT))
        prob -= 3;
    if (Race_if(PM_CENTAUR))
        prob -= 2;
    if (Race_if(PM_ELF))
        prob += 1;

    if (uarmg) {
        switch (uarmg->otyp) {
        case GAUNTLETS_OF_POWER:
        case MUMMIFIED_HAND: /* the Hand of Vecna */
            prob -= 10;
            break;
        case GAUNTLETS_OF_FUMBLING:
            prob *= 4;
            break;
        case GLOVES:
        case GAUNTLETS:
        case GAUNTLETS_OF_DEXTERITY:
        case GAUNTLETS_OF_PROTECTION:
            break;
        default:
            impossible("Unknown type of gloves (%d)", uarmg->otyp);
        }
    }

    if (3 > prob)
        prob = 3; /* don't be always successful without skill */

    if (P_SKILL(P_MARTIAL_ARTS) > P_EXPERT || !rn2(prob)) {
        if ((obj = sobj_at(BOULDER, dpx, dpy)) != 0) {
            fracture_rock(obj);
            pline_The("boulder splits and falls apart.");
        } else if ((obj = sobj_at(STATUE, dpx, dpy)) != 0) {
            break_statue(obj);
            pline_The("statue shatters into pieces.");
        }
        if ((bobj = sobj_at(BOULDER, dpx, dpy)) != 0) {
            /* another boulder here, restack it to the top */
            obj_extract_self(bobj);
            place_object(bobj, dpx, dpy);
        }
        exercise(A_STR, TRUE);
        use_skill(P_MARTIAL_ARTS, 1);
    } else {
        if ((obj = sobj_at(BOULDER, dpx, dpy)) != 0) {
            pline("However, your qi is not focused enough to break the boulder.");
            losehp(d(1, 6), "trying to split a boulder.", KILLED_BY);
        } else if ((obj = sobj_at(STATUE, dpx, dpy)) != 0) {
            pline("However, your qi is not focused enough to break the statue.");
            losehp(d(1, 6), "trying to shatter a statue.", KILLED_BY);
        }
        if (!rn2(5)) {
            You("need more training to reliably focus your qi.");
            use_skill(P_MARTIAL_ARTS, 1);
        }
    }
    return 0; /* done */
}

int
do_breakrock(x, y)
int x, y;
{
    if (P_SKILL(P_MARTIAL_ARTS) < P_SKILLED) {
        You("lack the necessary training to focus your qi.");
        return 0;
    } else {
        You("start channeling your qi.");
        dpx = x;
        dpy = y;
        breakturns = 0;
        set_occupation(ma_break, "channeling your qi", 0);
    }
    return 1;
}

/* gettech is reworked getspell */
/* reworked class special effects code */
/* adapted from specialpower in cmd.c */
static int
techeffects(tech_no)
int tech_no;
{
    /* These variables are used in various techs */
    int t_timeout = 0;

    /* check timeout */
    if (tech_inuse(techid(tech_no))) {
        pline("This technique is already active!");
        return 0;
    }
    if (techtout(tech_no)) {
        You("have to wait %s before using your technique again.",
            (techtout(tech_no) > 100) ?
                    "for a while" : "a little longer");
#ifdef WIZARD
        if (!wizard || (yn("Use technique anyways?") == 'n'))
#endif
        return 0;
    }

    /* switch to the tech and do stuff */
    switch (techid(tech_no)) {
        case T_RESEARCH:
            if (tech_research(get_tech_no(T_RESEARCH)))
                t_timeout = rn1(500, 500);
            break;
        case T_EVISCERATE:
            if (tech_eviscerate(get_tech_no(T_EVISCERATE)))
                t_timeout = rn1(500, 500);
            break;
        case T_BERSERK:
            if (tech_berserk(get_tech_no(T_BERSERK)))
                t_timeout = rn1(1000, 500);
            break;
        case T_REINFORCE:
            if (tech_reinforce(get_tech_no(T_REINFORCE)))
                t_timeout = rn1(500, 500);
           break;
        case T_FLURRY:
            if (tech_flurry(get_tech_no(T_FLURRY)))
                t_timeout = rn1(500, 500);
            break;
        case T_APPRAISAL:
            if (tech_appraisal(get_tech_no(T_APPRAISAL)))
                t_timeout = rn1(500, 500);
            break;
        case T_PRACTICE:
            if (tech_practice(get_tech_no(T_PRACTICE)))
                t_timeout = rn1(500, 500);
            break;
        case T_SURGERY:
            if (tech_surgery(get_tech_no(T_SURGERY)))
                t_timeout = rn1(500, 500);
            break;
        case T_HEAL_HANDS:
            if (tech_healhands(get_tech_no(T_HEAL_HANDS)))
                t_timeout = rn1(500, 500);;
            break;
        case T_KIII:
            if (tech_kiii(get_tech_no(T_KIII)))
                t_timeout = rn1(1000, 500);
            break;
        case T_CALM_STEED:
            if (tech_calmsteed(get_tech_no(T_CALM_STEED)))
                t_timeout = 250;
            break;
        case T_TURN_UNDEAD:
            return turn_undead();
        case T_VANISH:
            if (tech_vanish(get_tech_no(T_VANISH)))
                t_timeout = rn1(500, 500);
            break;
        case T_CRIT_STRIKE:
            if (tech_critstrike(get_tech_no(T_CRIT_STRIKE)))
                t_timeout = rn1(1000, 500);
            break;
        case T_CUTTHROAT:
            if (tech_cutthroat(get_tech_no(T_CUTTHROAT)))
                t_timeout = rn1(1000, 500);
            break;
        case T_BLESSING:
            if (tech_blessing(get_tech_no(T_BLESSING)))
                t_timeout = rn1(500, 500);
            break;
        case T_CURSE:
            if (tech_curse(get_tech_no(T_CURSE)))
                t_timeout = rn1(500, 500);
            break;
        case T_E_FIST: 
            if (tech_efist(get_tech_no(T_E_FIST)))
                t_timeout = rn1(1000, 500);
            break;
        case T_PRIMAL_ROAR:	    	
            if (tech_primalroar(get_tech_no(T_PRIMAL_ROAR)))
                t_timeout = rn1(500, 500);
            break;
        case T_LIQUID_LEAP: {
            if (tech_liquidleap(get_tech_no(T_LIQUID_LEAP)))
                t_timeout = rn1(250, 250);
            break;
        }
        case T_RAISE_ZOMBIES: {
            if (tech_raisezombies(get_tech_no(T_RAISE_ZOMBIES)))
                t_timeout = rn1(500, 500);
            break;
        }
        case T_WHISTLE_UNDEAD: {
            if (tech_callundead(get_tech_no(T_WHISTLE_UNDEAD)))
                t_timeout = rn1(100, 100);
            break;
        }
        case T_REVIVE: 
            if (tech_revive(get_tech_no(T_REVIVE)))
                t_timeout = rn1(1000, 500);
            break;
        case T_TINKER:
            if (tech_tinker(get_tech_no(T_TINKER)))
                t_timeout = rn1(10, 5);
            break;
        case T_RAGE:     	
            if (tech_rage(get_tech_no(T_RAGE)))
                t_timeout = rn1(1000, 500);
            break;	    
        case T_BLINK:
            if (tech_blink(get_tech_no(T_BLINK)))
                t_timeout = rn1(1000, 500);
            break;
        case T_CHI_STRIKE:
            if (!blitz_chi_strike())
                return 0;
            t_timeout = rn1(500, 500);
            break;
        case T_DRAW_ENERGY:
            if (tech_drawenergy(get_tech_no(T_DRAW_ENERGY)))
                t_timeout = rn1(1000, 500);
            break;
        case T_CHI_HEALING:
            if (tech_chiheal(get_tech_no(T_CHI_HEALING)))
                t_timeout = rn1(500, 500);
            break;
        case T_PICKPOCKET:
            return tech_pickpocket(get_tech_no(T_PICKPOCKET));
            /* No timeout */
        case T_DISARM:
            return tech_disarm(get_tech_no(T_DISARM)); /* No timeout */
            break;
        case T_DAZZLE:
            if (tech_dazzle(get_tech_no(T_DAZZLE)))
                t_timeout = rn1(50, 25);
            break;
        case T_BLITZ:
            if (!doblitz()) 
                return 0;
            t_timeout = rn1(1000, 500);
            break;
        case T_PUMMEL:
            /* Called outside of blitz_pummel to work with doblitz */
            if (!getdir((char *)0)) 
                return 0;
            if (!blitz_pummel())
                return 0;
            t_timeout = rn1(500, 500);
            break;
        case T_G_SLAM:
            /* Called outside of blitz_pummel to work with doblitz */
            if (!getdir((char *)0))
                return 0;
            if (!blitz_g_slam())
                return 0;
            t_timeout = rn1(500, 500);
            break;
        case T_UPPERCUT:
            /* Called outside of blitz_pummel to work with doblitz */
            if (!getdir((char *)0))
                return 0;
            if (!blitz_uppercut())
                return 0;
            t_timeout = rn1(500, 500);
            break;
        case T_DASH:
            /* Called outside of blitz_pummel to work with doblitz */
            if (!getdir((char *)0))
                return 0;
            if (!blitz_dash()) 
                return 0;
            t_timeout = rn1(50, 25);
            break;
        case T_POWER_SURGE:
            if (!blitz_power_surge())
                return 0;
            t_timeout = rn1(1000, 500);
            break;            	
        case T_SPIRIT_BOMB:
            /* Called outside of blitz_pummel to work with doblitz */
            
            if (!getdir(NULL))
                return 0;
            if (!blitz_spirit_bomb())
                return 0;
            t_timeout = rn1(250, 250);
            break;
        case T_SPIRIT_TEMPEST:
            if (tech_spirittempest(get_tech_no(T_SPIRIT_TEMPEST)))
                t_timeout = rn1(250, 250);
            break;
        case T_DRAW_BLOOD:
            if (tech_drawblood(get_tech_no(T_DRAW_BLOOD)))
                t_timeout = rn1(500, 500);
            break;
        case T_JEDI_JUMP:
            if (tech_jedijump(get_tech_no(T_JEDI_JUMP)))
                t_timeout = rn1(50, 25);
            break;
        case T_BOOZE:
             if (tech_booze(get_tech_no(T_BOOZE)))
                t_timeout = rn1(500, 500);
            break;
        case T_SOULEATER:
            if (tech_souleater(get_tech_no(T_SOULEATER)))
                t_timeout = rn1(500, 500);
            break;
        case T_SHIELD_BLOCK:
            if (tech_shieldblock(get_tech_no(T_SHIELD_BLOCK)))
                t_timeout = rn1(500, 500);
            break;
        case T_TELEKINESIS: {
            if (tech_telekinesis(get_tech_no(T_TELEKINESIS)))
                t_timeout = 250;
            break;
        }
        case T_FORCE_PUSH: {
            if (tech_forcepush(get_tech_no(T_FORCE_PUSH)))
                t_timeout = rn1(250, 250);
            break;
        }
        case T_CHARGE_SABER:
            if (tech_chargesaber(get_tech_no(T_CHARGE_SABER)))
                t_timeout = rn1(500, 500);
            break;
        case T_TUMBLE:
            if (tech_tumble(get_tech_no(T_TUMBLE)))
                t_timeout = rn1(10, 5);
            break;
        case T_SUNDER:
            if (tech_sunder(get_tech_no(T_SUNDER)))
                t_timeout = rn1(10, 5);
            break;
        case T_BLOOD_MAGIC:
            if (tech_bloodmagic(get_tech_no(T_BLOOD_MAGIC)))
                t_timeout = rn1(250, 250);
            break;
        case T_BREAK_ROCK:
            if (!getdir(NULL))
                return 0;
            int x = u.dx + u.ux;
            int y = u.dy + u.uy;
            return do_breakrock(x, y);
            /* No timeout */
        default:
            pline ("Error!  No such effect (%i)", tech_no);
            return 0;
    }
    techtout(tech_no) = (t_timeout * (100 - techlev(tech_no)) / 100);

    /* By default, action should take a turn */
    return 1;
}

void
shield_block(dam)
int dam;
{
    int i;
    for (i = 0; i < MAXTECH; ++i) {
        if (techid(i) == T_SHIELD_BLOCK)
            break;
    }
    if (i == MAXTECH) {
        impossible("no shield block tech");
        return;
    }
    if (tech_inuse(T_SHIELD_BLOCK)) {
        u.uen -= dam;
        if (u.uen <= 0) {
              u.uen = 0;
              You("can no longer block damage with %s.", 
                  uarms ? yname(uarms) : "a shield");
              for (i = 0; i < MAXTECH; ++i) {
                    if (techid(i) == T_SHIELD_BLOCK) {
                        techt_inuse(i) = 0;
                        break;
                    }
              }
        }
        context.botl = 1;
    }
}

boolean
shield_blockable(mtmp, mattk)
struct monst *mtmp;
struct attack *mattk;
{
    if (!tech_inuse(T_SHIELD_BLOCK) ||
          /* Additional restrictions: */
          !canspotmon(mtmp) 
          || u.uhs >= 3 
          || u.uswallow 
          || u.usleep
          || Fumbling
          || Stunned 
          || Confusion 
          || Afraid
          || Slow
          || Blind 
          || Vomiting)
        return FALSE;
    /* Assuming you took it off or had it stolen while tech is active */
    if (!uarms)
        return FALSE;
    switch (mattk->aatyp) {
    case AT_CLAW:
    case AT_KICK:
    case AT_BITE:
    case AT_STNG:
    case AT_TUCH:
    case AT_BUTT:
    case AT_TENT:
    case AT_WEAP:
        return TRUE;
    default:
        return FALSE;
    }
}

/* Whether or not a tech is in use.
 * 0 if not in use, turns left if in use. Tech is done when techinuse == 1
 */
int
tech_inuse(tech_id)
int tech_id;
{
    int i;

    if (tech_id < 1 || tech_id > MAXTECH) {
        impossible("invalid tech: %d", tech_id);
        return 0;
    }
    for (i = 0; i < MAXTECH; i++) {
        if (techid(i) == tech_id) {
              return techt_inuse(i);
        }
    }
    return 0;
}

void
tech_timeout()
{
    int i;
    for (i = 0; i < MAXTECH; i++) {
        if (techid(i) == NO_TECH)
              continue;
        /* Check if technique is done */
        if (techt_inuse(i)) {
            if (!(--techt_inuse(i))) {
                switch (techid(i)) {
#if 0 /* Pending lunatic/lycanthrope role/race */
                case T_EVISCERATE:
                    You("retract your claws.");
                    /* You're using bare hands now,  so new msg for next attack */
                    unweapon = TRUE;
                    /* Lose berserk status */
                    repeat_hit = 0;
                    break;
#endif
                case T_BLOOD_MAGIC:
                    The("darkness leaves your heart.");
                    break;
                case T_BERSERK:
                    The("red haze in your mind clears.");
                    break;
                case T_KIII:
                    You("calm down.");
                    break;
                case T_FLURRY:
                    You("relax.");
                    break;
                case T_E_FIST:
                    You("feel the power dissipate.");
                    break;
                case T_RAGE:
                    Your("anger cools.");
                    break;
                case T_POWER_SURGE:
                    pline_The("awesome power within you fades.");
                    break;
                case T_BLINK:
                    You("sense the flow of time returning to normal.");
                    break;
                case T_CHI_STRIKE:
                    You("feel the power in your hands dissipate.");
                    break;
                case T_CHI_HEALING:
                    You("feel the healing power dissipate.");
                    break;
                case T_SOULEATER:
                    if (uwep)
                        pline_The("dark flames surrounding %s dissipate.",
                                  doname(uwep));
                    break;
                case T_SHIELD_BLOCK:
                    if (uarms)
                        You("release the energy from %s.", yname(uarms));
                    break;
                default:
                    break;
                }
            }
            else {
                switch (techid(i)) {
                    /* During the technique */
                case T_RAGE:
                    /* Bleed but don't kill */
                    if (u.uhpmax > 1)
                        u.uhpmax--;
                    if (u.uhp > 1)
                        u.uhp--;
                    break;
                case T_POWER_SURGE:
                    /* Bleed off power.  Can go to zero as 0 power is not fatal */
                    if (u.uenmax > 1)
                        u.uenmax--;
                    if (u.uen > 0)
                        u.uen--;
                    break;
                default:
                    break;
                }
            }
        }

        if (techtout(i) == 1) {
              pline("Your %s technique is ready to be used!", techname(i));
              stop_occupation();
        }
        if (techtout(i) > 0)
              techtout(i)--;
    }
}

void
extend_tech_time(tid, t)
int tid;
int t;
{
    int i;
    for (i = 0; i < MAXTECH; ++i) {
        if (techid(i) == tid) {
              techt_inuse(i) += t;
              break;
        }
    }
}

void
docalm()
{
    int i, tech, n = 0;

    for (i = 0; i < MAXTECH; i++) {
        tech = techid(i);
        if (tech != NO_TECH && techt_inuse(i)) {
             aborttech(tech);
             n++;
        }
    }
    if (n)
        You("calm down.");
}


static const struct 	
innate_tech *
role_tech()
{
    switch (Role_switch) {
    case PM_ARCHEOLOGIST:
        return arc_tech;
    case PM_BARBARIAN:
        return bar_tech;
    case PM_CAVEMAN:
        return cav_tech;
    case PM_FLAME_MAGE:
        return fla_tech;
    case PM_HEALER:
        return hea_tech;
    case PM_ICE_MAGE:
        return ice_tech;
    case PM_INFIDEL:
        return inf_tech;
    case PM_JEDI:
        return jed_tech;
    case PM_KNIGHT:
        if (u.ualign.type == A_CHAOTIC)
             return drk_tech;
        else
             return kni_tech;
    case PM_MONK:
        return mon_tech;
    case PM_NECROMANCER:
        return nec_tech;
    case PM_PIRATE:
        return pir_tech;
    case PM_PRIEST:
        return pri_tech;
    case PM_RANGER:
        return ran_tech;
    case PM_ROGUE:
        return rog_tech;
    case PM_SAMURAI:
        return sam_tech;
    case PM_TOURIST:
        return tou_tech;
    case PM_UNDEAD_SLAYER:
        return und_tech;
    case PM_VALKYRIE:
        return val_tech;
    case PM_WIZARD:
        return wiz_tech;
    case PM_YEOMAN:
        return yeo_tech;
        
#if 0 /* 5lo: Deferred for now */
case PM_DRUNK:
       return (dru_tech);
    case PM_BARD:
        return (brd_tech);
    case PM_LUNATIC:
        return (lun_tech);
    case PM_WARRIOR:
       return (war_tech);
    case PM_NINJA:
        return (nin_tech);
#endif
        
    default:
        return ((struct innate_tech *) 0);
    }
}

static const struct innate_tech *
race_tech()
{
    switch (Race_switch) {
    case PM_DWARF:
        return dwa_tech;
    case PM_ELF:
        return elf_tech;
    case PM_GNOME:
        return gno_tech;
    case PM_HOBBIT:
        return hob_tech;
    case PM_TORTLE:
        return tor_tech;
    case PM_VAMPIRIC:
        return vam_tech;
    case PM_DOPPELGANGER:
        return (dop_tech);
        
#if 0 /* Deferred */
    case PM_DROW:
    case PM_CLOCKWORK_AUTOMATON:
        return (clk_tech);
    case PM_INCANTIFIER:
        return (inc_tech);
#endif
    default:
        return ((struct innate_tech *) 0);
    }
}

void
adjtech(oldlevel,newlevel)
int oldlevel, newlevel;
{
    const struct innate_tech *tech = role_tech(), *rtech = race_tech();
    long mask = FROMEXPER;

    while (tech || rtech) {
        /* Have we finished with the tech lists? */
        if (!tech || !tech->tech_id) {
             /* Try the race intrinsics */
             if (!rtech || !rtech->tech_id)
                break;
             tech = rtech;
             rtech = (struct innate_tech *) 0;
             mask = FROMRACE;
        }
                    
        for (; tech->tech_id; tech++) {
            if (oldlevel < tech->ulevel && newlevel >= tech->ulevel) {
                if (tech->ulevel != 1 && !tech_known(tech->tech_id))
                    You("learn how to perform %s!", tech_names[tech->tech_id]);
                
                learntech(tech->tech_id, mask, tech->tech_lev);
            } else if (oldlevel >= tech->ulevel && newlevel < tech->ulevel
                        && tech->ulevel != 1) {
                    
                learntech(tech->tech_id, mask, -1);
                if (!tech_known(tech->tech_id))
                    You("lose the ability to perform %s!",
                        tech_names[tech->tech_id]);
            }
        }
    }
}

STATIC_PTR int
charge_saber()
{
    int i, tlevel;
    if (delay) {
        delay++;
        return 1;
    }
    for (i = 0; i < MAXTECH; i++) {
        if (techid(i) == NO_TECH)
             continue;
        if (techid(i) != T_CHARGE_SABER)
             continue;
        tlevel = techlev(i);
    }
    if (tlevel >= 10 && !rn2(5)) {
        You("manage to channel the force perfectly!");
        uwep->age += 1500; /* Jackpot! */
    } else
        You("channel the force into %s.", the(xname(uwep)));

    /* yes no return above, it's a bonus :) */
    uwep->age += u.uen * ((techlev(T_CHARGE_SABER) / rnd(10)) + 51); 
    /* improved results by Amy */
    u.uen = 0;
    context.botl = 1;
    return 0;
}


/*WAC tinker code*/
STATIC_PTR int
tinker()
{
    int chance;
    struct obj *otmp = uwep;

    if (delay) { /* not if (delay++), so at end delay == 0 */
        delay++;
        /*WAC a bit of practice so even if you're interrupted
        you won't be wasting your time ;B*/
        return 1; /* still busy */
    }

    if (!uwep)
        return 0;

    You("finish your tinkering.");
    chance = 5;
    /*	chance += PSKILL(P_TINKER); */
    if (rnl(10) < chance) {
        upgrade_obj(otmp);
    } else {
        /* object downgrade  - But for now,  nothing :) */
    }

    setuwep(otmp);
    You("now hold %s!", doname(otmp));
    return 0;
}

/*WAC  draw energy from surrounding objects */
STATIC_PTR int
draw_energy()
{
    int powbonus = 1;
    if (delay) { /* not if (delay++), so at end delay == 0 */
        delay++;
        confdir();
        if (isok(u.ux + u.dx, u.uy + u.dy)) {
             switch ((&levl[u.ux + u.dx][u.uy + u.dy])->typ) {
             case ALTAR: /* Divine power */
                 powbonus = (u.uenmax > 28 ? u.uenmax / 4 : 7);
                 break;
             case THRONE: /* Regal == pseudo divine */
                 powbonus = (u.uenmax > 36 ? u.uenmax / 6 : 6);
                 break;
             case CLOUD:    /* Air */
             case TREE:     /* Earth */
             case LAVAPOOL: /* Fire */
             case ICE:      /* Water - most ordered form */
                 powbonus = 5;
                 break;
             case AIR:
             case MOAT: /* Doesn't freeze */
             case WATER:
                 powbonus = 4;
                 break;
             case POOL: /* Can dry up */
                 powbonus = 3;
                 break;
             case FOUNTAIN:
             case GRASS: /* common */
             case PUDDLE: /* also common */
                 powbonus = 2;
                 break;
             case SINK: /* Cleansing water */
                 if (!rn2(3))
                    powbonus = 2;
                 break;
             case SEWAGE:
             case TOILET: /* Water Power...but also waste! */
                 if (rn2(100) < 50)
                    powbonus = 2;
                 else
                    powbonus = -2;
                 break;
             case GRAVE:
                 if (Role_if(PM_NECROMANCER)) {
                    powbonus = (u.uenmax > 36 ? u.uenmax / 6 : 6);
                    break;
                 }
                 /* FALLTHROUGH */
             case DEADTREE:
                 powbonus = -4;
                 break;
             default:
                break;
             }
        }
        u.uen += powbonus;
        if (u.uen > u.uenmax) {
             delay = 0;
             u.uen = u.uenmax;
        }
        if (u.uen < 1)
             u.uen = 0;
        context.botl = 1;
        return 1; /* still busy */
    }
    You("finish drawing energy from your surroundings.");
    return 0;
}

static const char *Enter_Blitz = "Enter Blitz Command[. to end]: ";

/* Keep commands that reference the same blitz together 
 * Keep the BLITZ_START before the BLITZ_CHAIN before the BLITZ_END
 */
static const struct blitz_tab blitzes[] = {
    { "LLDDR", 5, blitz_chi_strike, T_CHI_STRIKE, BLITZ_START },
    { "RRDDL", 5, blitz_chi_strike, T_CHI_STRIKE, BLITZ_START },
    /*{ "LLDDRDR", 7, blitz_chi_strike, T_CHI_STRIKE, BLITZ_START },*/
    { "RR", 2, blitz_dash, T_DASH, BLITZ_START },
    { "LL", 2, blitz_dash, T_DASH, BLITZ_START },
    { "UURRDDL", 7, blitz_e_fist, T_E_FIST, BLITZ_START },
    { "UULLDDR", 7, blitz_e_fist, T_E_FIST, BLITZ_START },
    /*{ "DDRRDDRR", 8, blitz_power_surge, T_POWER_SURGE, BLITZ_START },*/
    /*{ "DRDRDRDR", 8, blitz_power_surge, T_POWER_SURGE, BLITZ_START },*/
    { ">>>>", 4, blitz_power_surge, T_POWER_SURGE, BLITZ_START },
    { "LRL", 3, blitz_pummel, T_PUMMEL, BLITZ_CHAIN },
    { "RLR", 3, blitz_pummel, T_PUMMEL, BLITZ_CHAIN },
    { "DDDD", 4, blitz_g_slam, T_G_SLAM, BLITZ_END },
    { "DUDUDU", 6, blitz_uppercut, T_UPPERCUT, BLITZ_END },
    { "UUUUD", 5, blitz_spirit_bomb, T_SPIRIT_BOMB, BLITZ_END },
    { "", 0, (void *) 0, 0, BLITZ_END } /* Array terminator */
};

#define MAX_BLITZ 50
#define MIN_CHAIN 2
#define MAX_CHAIN 5

/* parse blitz input */
static int
doblitz()
{
    int i, j, dx, dy, bdone = 0, tech_no;
    char buf[BUFSZ];
    char buf2[BUFSZ];
    char *bp;
    char cmdlist[BUFSZ];
    int blitz_chain[MAX_CHAIN], blitz_num;
    tech_no = (get_tech_no(T_BLITZ));
    int max_moves = (MIN_CHAIN + (techlev(tech_no) / 10));
    techt_inuse(T_BLITZ) = 1;
    if (tech_no == -1) {
        return 0;
    }
    if (uwep || (u.twoweap && uswapwep)) {
        You("can't do this while wielding a weapon!");
        return 0;
    } else if (uarms) {
        You("can't do this while holding a shield!");
        return 0;
    }

    if (u.uen < 10) {
        You("are too weak to attempt this! You need at least 10 points of mana!");
        return 0;
    }

    bp = buf;
    
    if (!getdir((char *) 0))
        return 0;
    if (!u.dx && !u.dy) {
        return 0;
    }

    dx = u.dx;
    dy = u.dy;

    doblitzlist();
    Strcpy(cmdlist, "");
    int res;
    for (i = 0; i < MAX_BLITZ; i++) {
        if (i == 0)
            sprintf(buf2, "%s", Enter_Blitz);
        else
            sprintf(buf2, "[%s]:", cmdlist);
        res = blitzdir(buf2);  /* Get directional input */
        if (!res)
             return 0;
        else if (res < 0) { /* Invalid input, try again */
             i--;
             continue;
        }
        if (!u.dx && !u.dy && !u.dz) {
             break;
        }
        if (u.dx == -1) {
             *(bp) = 'L';
             strcat(cmdlist, "L");
             bp++;
        } else if (u.dx == 1) {
             *(bp) = 'R';
             strcat(cmdlist, "R");
             bp++;
        }
        if (u.dy == -1) {
             *(bp) = 'U';
             strcat(cmdlist, "U");
             bp++;
        } else if (u.dy == 1) {
             *(bp) = 'D';
             strcat(cmdlist, "D");
             bp++;
        }
        if (u.dz == -1) {
             *(bp) = '<';
             /* TODO: Why do these have to be reversed? */
             strcat(cmdlist, "<"); 
             bp++;
        } else if (u.dz == 1) {
             *(bp) = '>';
             /* TODO: Why do these have to be reversed? */
             strcat(cmdlist, ">");
             bp++;
        }
    }
    *(bp) = '.';
    bp++;
    *(bp) = '\0';
    bp = buf;

    /* parse input */
    /* You can't put two of the same blitz in a row */
    blitz_num = 0;
    while (strncmp(bp, ".", 1)) {
        bdone = 0;
        for (j = 0; blitzes[j].blitz_len; j++) {
             int cmd_len = blitzes[j].blitz_len;
             const char *tech_name = tech_names[blitzes[j].blitz_tech];
             if (blitz_num >= MAX_CHAIN || blitz_num >= max_moves) {
                You("went over the maximum allowable commands [%d].",
                         max_moves);
                return 0; /* Trying to chain too many blitz commands */
             }
             else if (!strncmp(bp, blitzes[j].blitz_cmd, cmd_len)) {
                /* Trying to chain in a command you don't know yet */
                if (!tech_known(blitzes[j].blitz_tech)) {
                    pline("You don't know %s yet.", tech_name);
                    return 0;
                }
                if (blitz_num) {
                    /* Check if trying to chain two of the exact same 
                     * commands in a row. */
                    int prev_blitz = blitz_chain[(blitz_num - 1)];
                    if (j == prev_blitz) {
                        You_cant("chain two of the exact same commands [%s] in a row.",
                                 tech_name);
                        return 0;
                    }
                    /* Trying to chain after chain finishing command */
                    if (blitzes[prev_blitz].blitz_type == BLITZ_END) {
                        You_cant("enter more commands after a chain finisher [%s].",
                                 tech_names[blitzes[prev_blitz].blitz_tech]);
                        return 0;
                    }
                    /* Trying to put a chain starter after starting a chain
                     * Note that it's OK to put two chain starters in a row */
                    if ((blitzes[j].blitz_type == BLITZ_START)
                        && (blitzes[prev_blitz].blitz_type != BLITZ_START)) {
                        You_cant("enter a chain starter [%s] after starting a command chain.",
                                 tech_name);
                        return 0;
                    }
                }
                bp += cmd_len;
                blitz_chain[blitz_num] = j;
                blitz_num++;
                bdone = 1;
                break;
             }
        }
        if (!bdone) {
             /*You("stumble!");*/
             pline("Try again.");
             return 0;
        }
    }
    /* Energy is used only if valid sequence is processed. */
    u.uen -= 10;
    
    for (i = 0; i < blitz_num; i++) {
        u.dx = dx;
        u.dy = dy;
        if (!((*blitzes[blitz_chain[i]].blitz_funct)()))
             break;
    }

    /* done */
    return 1;
}

static void
doblitzlist()
{
    winid tmpwin;
    int i;
    int maxmoves = MIN_CHAIN + (techlev(get_tech_no(T_BLITZ)) / 10);
    char buf[BUFSZ];
    menu_item *selected;
    anything any;

    tmpwin = create_nhwindow(NHW_MENU);
    start_menu(tmpwin);
    any.a_void = 0; /* zero out all bits */

    Sprintf(buf, "%16s %5s %-17s", "[U = Up]", "", "[L = Left]");
    add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, buf, MENU_UNSELECTED);
    Sprintf(buf, "%16s %5s %-17s", "[D = Down]", "", "[R = Right]");
    add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, buf, MENU_UNSELECTED);

    Sprintf(buf, "%-30s %10s   %s", "Name", "Type", "Command");
    add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, buf, MENU_UNSELECTED);
    
    for (i = 0; blitzes[i].blitz_len; i++) {
        if (tech_known(blitzes[i].blitz_tech)) {
             Sprintf(buf, "%-30s %10s   %s",
                 (i && blitzes[i].blitz_tech == blitzes[(i - 1)].blitz_tech
                      ? ""
                      : tech_names[blitzes[i].blitz_tech]),
                 (blitzes[i].blitz_type == BLITZ_START
                      ? "starter"
                      : (blitzes[i].blitz_type == BLITZ_CHAIN
                             ? "chain"
                             : (blitzes[i].blitz_type == BLITZ_END
                                    ? "finisher"
                                    : "unknown"))),
                 blitzes[i].blitz_cmd);

             add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, buf,
                      MENU_UNSELECTED);
        }
    }
    add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, "", MENU_UNSELECTED);
    Sprintf(buf, "You have a maximum of %d moves available.", maxmoves);
    add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, buf, MENU_UNSELECTED);
    
    end_menu(tmpwin, "Currently known blitz manoeuvers");
    select_menu(tmpwin, PICK_NONE, &selected);
    destroy_nhwindow(tmpwin);
    return;
}

static int
blitz_chi_strike()
{
    int tech_no;
    tech_no = (get_tech_no(T_CHI_STRIKE));

    if (tech_no == -1) {
        return 0;
    }

    if (u.uen < 1) {
        You("are too weak to attempt this! You need at least one point of mana!");
        return 0;
    }
    You("feel energy surge through your hands!");
    techt_inuse(tech_no) = techlev(tech_no) + 4;
    return 1;
}

static int
blitz_e_fist()
{
    int tech_no;
    const char *str;

    tech_no = (get_tech_no(T_E_FIST));

    if (tech_no == -1) {
        return 0;
    }

    str = makeplural(body_part(HAND));
    You("focus the powers of the elements into your %s.", str);
    techt_inuse(tech_no) =
        rnd((int) (techlev(tech_no) / 3 + 1)) + d(1, 4) + 2;
    return 1;
}

/* Assumes u.dx, u.dy already set up */
static int
blitz_pummel()
{
    int i = 0, tech_no;
    struct monst *mtmp;
    tech_no = (get_tech_no(T_PUMMEL));

    if (tech_no == -1) {
        return 0;
    }
    if (uwep || (u.twoweap && uswapwep)) {
        You("can't do this while wielding a weapon!");
        return 0;
    } else if (uarms) {
        You("can't do this while holding a shield!");
        return 0;
    }
    if (!u.dx && !u.dy) {
        You("flex your muscles.");
        return 0;
    }
    You("let loose a barrage of blows!");

    if (u.uswallow)
        mtmp = u.ustuck;
    else
        mtmp = m_at(u.ux + u.dx, u.uy + u.dy);

    if (!mtmp) {
        You("strike nothing.");
        return 0;
    }
    context.forcefight = TRUE; 
    if (!attack(mtmp))
        return 0;
    
    
    /* Perform the extra attacks */
    for (i = 0; (i < 4); i++) {
        if (rn2(70) > (techlev(tech_no) + 30))
             break;

        if (u.uswallow)
             mtmp = u.ustuck;
        else
             mtmp = m_at(u.ux + u.dx, u.uy + u.dy);

        if (!mtmp)
             return 1;
        if (!attack(mtmp))
             return 1;
    }

    return 1;
}

/* Assumes u.dx, u.dy already set up */
static int
blitz_g_slam()
{
    int tech_no, tmp;
    struct monst *mtmp;
    struct trap *chasm;

    tech_no = (get_tech_no(T_G_SLAM));

    if (tech_no == -1) {
        return 0;
    }
    if (uwep || (u.twoweap && uswapwep)) {
        You("can't do this while wielding a weapon!");
        return 0;
    } else if (uarms) {
        You("can't do this while holding a shield!");
        return 0;
    }

    if (!u.dx && !u.dy) {
        You("flex your muscles.");
        return 0;
    }
    mtmp = m_at(u.ux + u.dx, u.uy + u.dy);
    if (!mtmp) {
        You("strike nothing.");
        return 0;
    }
    /* Required for the first attack, otherwise nothing happens if we attempt
     * to attack peacefuls. */
    context.forcefight = TRUE; 
    if (!attack(mtmp))
        return 0;
    
    /* Slam the monster into the ground */
    mtmp = m_at(u.ux + u.dx, u.uy + u.dy);
    if (!mtmp || u.uswallow)
        return 1;

    wakeup(mtmp, TRUE);
    
    You("hurl %s downwards...", mon_nam(mtmp));
    if (Is_airlevel(&u.uz) || Is_waterlevel(&u.uz))
        return 1;
    
    tmp = (5 + rnd(6) + (techlev(tech_no) / 5));

    chasm = maketrap(u.ux + u.dx, u.uy + u.dy, PIT);
    if (chasm) {
        if (!is_flyer(mtmp->data) && !is_clinger(mtmp->data))
             mtmp->mtrapped = 1;
        chasm->tseen = 1;
        levl[u.ux + u.dx][u.uy + u.dy].doormask = 0;
        pline("%s slams into the ground, creating a crater!", Monnam(mtmp));
        tmp *= 2;
    }

    mselftouch(mtmp, "Falling, ", TRUE);
    if (!DEADMONSTER(mtmp)) {
        if (procdmg(mtmp, tmp, AD_PHYS)) {
             if (!cansee(u.ux + u.dx, u.uy + u.dy))
                pline("It is destroyed!");
             else {
                You("destroy %s!",
                    mtmp->mtame ? x_monnam(mtmp, ARTICLE_THE, "poor", 
                                           SUPPRESS_SADDLE, FALSE) 
                                : mon_nam(mtmp));
             }
        }
    }

    return 1;
}

/* Assumes u.dx, u.dy already set up */
static int
blitz_uppercut()
{
    int tech_no = (get_tech_no(T_UPPERCUT));
    int tmp;
    struct monst *mtmp;
    struct permonst *mdat;

    if (tech_no == -1) {
        return 0;
    }
    if (uwep || (u.twoweap && uswapwep)) {
        You("can't do this while wielding a weapon!");
        return 0;
    } else if (uarms) {
        You("can't do this while holding a shield!");
        return 0;
    }
    if (!u.dx && !u.dy) {
        You("flex your muscles.");
        return 0;
    }
    mtmp = m_at(u.ux + u.dx, u.uy + u.dy);
    if (!mtmp) {
        You("strike nothing.");
        return 0;
    }
    mtmp = m_at(u.ux + u.dx, u.uy + u.dy);
    if (!mtmp || u.uswallow)
        return 1;
    mdat = mtmp->data;
    
    tmp = (10 + rnd(6) + (techlev(tech_no) / 5)); /* Same as ground slam */
    if (has_head(mtmp->data)) {
        /* Bonus vs things with heads */
        You("wind up your first and clock the %s with an uppercut!", mon_nam(mtmp));
        tmp *= 2;
    } else
        You("slam the %s with your fist!", mon_nam(mtmp));
    
    procdmg(mtmp, tmp, AD_PHYS);
    wakeup(mtmp, TRUE);
    
    /* TODO: Below copied from uhitm, refactor? */
    if (!DEADMONSTER(mtmp) 
          && !biggermonst(mdat) 
          && !thick_skinned(mdat) 
          && !unsolid(mdat)) {
        if (rn2(2)) {
             if (canspotmon(mtmp)) {
                pline("%s %s from your powerful strike!", Monnam(mtmp),
                      makeplural(stagger(mdat, "stagger")));
             }
             if (mhurtle_to_doom(mtmp, tmp, &mdat, FALSE)) {
                ;
                /*already_killed = TRUE;*/
             }
        } else if (!mindless(mtmp->data)) {
             if (canspotmon(mtmp))
                Your("forceful blow knocks %s senseless!", mon_nam(mtmp));
             /* avoid migrating a dead monster */
             if (mtmp->mhp > tmp) {
                mtmp->mconf = 1;
                mdat = mtmp->data; /* in case of a polymorph trap */
                /*if (DEADMONSTER(mon)) already_killed = TRUE;*/
             }
        }
    }
    return 1;
}



/* Assumes u.dx, u.dy already set up */
static int
blitz_dash()
{
    int tech_no;
    tech_no = (get_tech_no(T_DASH));

    if (tech_no == -1) {
        return 0;
    }
    if (!u.dx && !u.dy) {
        You("stretch.");
        return 0;
    }
    if ((!Punished || carried(uball)) && !u.utrap)
        You("dash forwards!");
    hurtle(u.dx, u.dy, 2, FALSE);
    multi = 0; /* No paralysis with dash */
    return 1;
}

static int
blitz_power_surge()
{
    int tech_no, num;
    tech_no = (get_tech_no(T_POWER_SURGE));

    if (tech_no == -1) {
        return 0;
    }

    if (Upolyd) {
        You("cannot tap into your full potential in this form.");
        return 0;
    }
    You("tap into the full extent of your power!");
    num = 50 + (2 * techlev(tech_no));
    techt_inuse(tech_no) = num + 1;
    u.uenmax += num;
    u.uen = u.uenmax;
    return 1;
}

/* Assumes u.dx, u.dy already set up */
static int
blitz_spirit_bomb()
{
    int tech_no, num;
    int sx = u.ux, sy = u.uy, i;
    tech_no = get_tech_no(T_SPIRIT_BOMB);
    if (tech_no == -1) {
        return 0;
    }
    if (uwep || (u.twoweap && uswapwep)) {
        You("can't do this while wielding a weapon!");
        return 0;
    } else if (uarms) {
        You("can't do this while holding a shield!");
        return 0;
    }

    You("gather your energy...");

    if (u.uen < 10) {
        pline("But it fizzles out.");
        u.uen = 0;
    }

    /* hackem: Boosted damage potential */
    num = 10 + d(5, ((techlev(tech_no) / 2) + 1));
    num = (u.uen < num ? u.uen : num);

    u.uen -= num;

    for (i = 0; i < 2; i++) {
        if (!isok(sx, sy) || !cansee(sx, sy) || IS_STWALL(levl[sx][sy].typ)
            || u.uswallow)
             break;

        /* Display the center of the explosion */
        tmp_at(DISP_FLASH, explosion_to_glyph(EXPL_MAGICAL, S_explode5));
        tmp_at(sx, sy);
        delay_output();
        tmp_at(DISP_END, 0);

        sx += u.dx;
        sy += u.dy;
    }
    num = spell_damage_bonus(num); /* hackem bonus */
                                   
    /* Magical Explosion */
    explode(sx, sy, 10, (d(3, 6) + num), SPIRIT_CLASS, EXPL_MAGICAL);
    return 1;
}

static int
tech_spirittempest(tech_no)
int tech_no;
{
    int blasts = (u.ulevel > 20) ? (3 + rn2(4)) : 1;
    int num, failcheck;
    boolean didblast = FALSE;
    /* Restrictions: Must not be impaired */
            
    You("channel the spirits from deep within...");
    if (u.uen < 20) {
        pline("But it fizzles out.");
        u.uen = 0;
        return 0;
    }
    /* hackem: Boosted damage potential */
    num = 20 + d(10, (techlev(tech_no) / 3) + 1);
    
    /* Bonus damage can't go over our available energy */
    num = (u.uen < num ? u.uen : num);
    
    u.uen -= num;
    num = spell_damage_bonus(num); /* hackem bonus */
   
    /* Throwback to the sigil of tempest in SLASH'EM. 
     * Becomes available when we are XP level 21.
     */
    while (blasts--) {
         failcheck = 0;
         do {
            confdir(); /* Random Dir */
            /* 0-2 spaces from our current position */
            u.dx += u.ux + rn2(3); 
            u.dy += u.uy + rn2(3);
            failcheck++;
         } while (failcheck < 3 &&
                  (!cansee(u.dx, u.dy) 
                   || !isok(u.dx, u.dy)
                   || IS_STWALL(levl[u.dx][u.dy].typ)));
         
         if (failcheck >= 3)
            continue;

         explode(u.ux, u.uy, 10, (d(3, 6) + num), SPIRIT_CLASS, EXPL_MAGICAL);
         delay_output();
         didblast = TRUE;
    }
    if (!didblast) {
         explode(u.ux, u.uy, 10, (d(3, 6) + num), SPIRIT_CLASS, EXPL_MAGICAL);
         delay_output();
    }
    return 1;
}

int
do_pickpocket(mon)
struct monst *mon;
{
    if (!Role_if(PM_ROGUE)) {
         impossible("Attempting pickpocket technique as non-rogue.");
         return 0;
    }
    if (Upolyd) {
         You("must be in your natural form to pickpocket.");
         return 0;
    }
    if (uwep) {
         You_cant("do this while holding a weapon");
         return 0;
    }
    /* Exercise by default, decrease with failure  */
    use_skill(P_THIEVERY, 1);
    
    /* Izchak is off-limits */
    if (mon->isshk
        && !strcmp(shkname(mon), "Izchak")) {
         You("find yourself unable to steal from %s.",
             mon_nam(mon));
         use_skill(P_THIEVERY, -1);
         return 0;
    }
    /* pets are also off-limits, since #loot can be
       used to give your pets as much as they can carry.
       would be an easy way to abuse thievery and train
       the skill without risk */
    if (mon->mtame) {
         You_cant("bring yourself to steal from %s.",
                  mon_nam(mon));
         use_skill(P_THIEVERY, -1);
         return 0;
    }
    /* engulfed? ummm no */
    if (u.uswallow) {
         pline("What exactly were you planning on stealing?  Its stomach?");
         use_skill(P_THIEVERY, -1);
         return 0;
    }
    if (mon->minvent != 0) {
         You("%s to %s %s.",
             rn2(2) ? "try" : "attempt",
             rn2(2) ? "steal from" : "pickpocket",
             mon_nam(mon));
         steal_it(mon, &youmonst.data->mattk[0]);
    } else if (mon->minvent == 0) {
         pline("%s has nothing for you to %s.",
               Monnam(mon), rn2(2) ? "steal" : "pickpocket");
         /* prevent skill from incrementing - nothing was stolen */
         use_skill(P_THIEVERY, -1);
    }
    return 1;
}


int
tech_surgery(tech_no)
int tech_no;
{
    struct obj *otmp;
    if (Hallucination || Stunned || Confusion) {
         You("are in no condition to perform surgery!");
         return 0;
    }
    if (Sick || Slimed) {
         if (carrying(SCALPEL)) {
            pline("Using %s (ow!), you cure your infection!", 
                  yname(carrying(SCALPEL)));
            make_sick(0L, (char *)0, TRUE, SICK_ALL);
            Slimed = 0;
            if (Upolyd) {
                u.mh -= 5;
                if (u.mh < 1)
                    rehumanize();
            } else if (u.uhp > 6)
                u.uhp -= 5;
            else
                u.uhp = 1;
            context.botl = TRUE;
            return 1;
         } else 
            pline("If only you had a scalpel...");
    }
    
    if (Upolyd ? u.mh < u.mhmax : u.uhp < u.uhpmax) {
         otmp = use_medical_kit(BANDAGE, FALSE,
                                "dress your wounds with");
         if (otmp) {
            pline("Using %s, you dress your wounds.", yname(otmp));
            check_unpaid(otmp);
            if (otmp->quan > 1L) {
                otmp->quan--;
                otmp->ocontainer->owt = weight(otmp->ocontainer);
            } else {
                obj_extract_self(otmp);
                obfree(otmp, (struct obj *)0);
            }
            healup(techlev(tech_no) * (rnd(2) + 1) + rn1(5, 5), 0, FALSE, FALSE);
         } else {
            You("strap your wounds as best you can.");
            healup(techlev(tech_no) + rn1(5, 5), 0, FALSE, FALSE);
         }
         context.botl = TRUE;
         return 1;
    } else 
         You("don't need your healing powers!");
    return 0;
}

int
tech_healhands(tech_no)
int tech_no;
{
    if (Slimed) {
         Your("body is on fire!");
         burn_away_slime();
         return 1;
    } else if (Sick) {
         You("lay your %s on the foul sickness...",
             makeplural(body_part(HAND)));
         make_sick(0L, (char*)0, TRUE, SICK_ALL);
         return 1;
    } else if (LarvaCarrier) {
         You("lay your %s on your %s...", 
             makeplural(body_part(HAND)),
             body_part(STOMACH));
         make_carrier(0L, FALSE);
         You_feel("as if your body is your own again.");
         return 1;
    } else if (Upolyd ? u.mh < u.mhmax : u.uhp < u.uhpmax) {
         pline("A warm glow spreads through your body!");
         healup(techlev(tech_no) * 4, 0, FALSE, FALSE);
         return 1;
    } 
    pline1(nothing_happens);
    return 0;
}

int
tech_kiii(tech_no)
int tech_no;
{
    You("scream \"KIIILLL!\"");
    aggravate();
    techt_inuse(tech_no) = rnd((int) (techlev(tech_no) / 6 + 1)) + 2;
    return 1;
}

int
tech_calmsteed(tech_no)
int tech_no;
{
    if (u.usteed) {
         You("attempt to calm your steed.");
         tamedog(u.usteed, (struct obj *) 0);
         
         if (u.usteed->mtame >= 10) {
            pline("%s looks tame enough.", Monnam(u.usteed));
         } else {
            u.usteed->mtame += rnd((techlev(tech_no) + 2) / 3);
            pline("%s gets tamer.", Monnam(u.usteed));
            
            return 1;
         }
    } else
         Your("technique is only effective when riding a monster.");
    return 0;
}

int
tech_vanish(tech_no)
int tech_no;
{
    if (Invisible && Fast) {
         You("are already quite nimble and undetectable.");
         return 0;
    }
    techt_inuse(tech_no) = rn1(50, 50) + techlev(tech_no);

    if (!Invisible)
         pline("In a puff of smoke,  you disappear!");
    if (!Fast)
         You("feel more nimble!");
    incr_itimeout(&HInvis, techt_inuse(tech_no));
    incr_itimeout(&HFast, techt_inuse(tech_no));
    newsym(u.ux, u.uy); /* update position */
    return 1;
}

/* Base damage is always something, though it may be reduced to zero 
 * if the hero is hampered. However, since techlev will never be zero, 
 * stiking vital organs will always do _some_ damage.
 */
int
tech_critstrike(tech_no)
int tech_no;
{
    struct monst *mtmp;
    if (!getdir((char *)0)) 
         return 0;
    if (!u.dx && !u.dy) {
         /* Hopefully a mistake ;B */
         You("decide against that idea.");
         return 0;
    }
    mtmp = m_at(u.ux + u.dx, u.uy + u.dy);
    if (!mtmp) {
         You("perform a flashy twirl!");
         return 0;
    } else {
         int oldhp = mtmp->mhp;
         int tmp;
         
         if (!attack(mtmp)) 
            return 0;
         if (!DEADMONSTER(mtmp) 
               && mtmp->mhp < oldhp 
               && !noncorporeal(mtmp->data) 
               && !unsolid(mtmp->data)) {
            You("strike %s vital organs!", s_suffix(mon_nam(mtmp)));
            tmp = mtmp->mhp > 1 ? mtmp->mhp / 2 : 1;
            if (!humanoid(mtmp->data) 
                || is_golem(mtmp->data) 
                || mtmp->data->mlet == S_CENTAUR) {
                You("are hampered by the differences in anatomy.");
                tmp /= 2;
            }
            tmp += techlev(tech_no);
            procdmg(mtmp, tmp, AD_PHYS);
         }
    }
    return 1;
}


int
tech_cutthroat(tech_no)
int tech_no;
{
    struct monst *mtmp;
    if (!uwep) {
         You("can't perform that without a weapon.");
         return 0;
    }
    if (!is_blade(uwep)) {
         You("need a blade to perform cutthroat!");
         return 0;
    }
    if (!getdir((char *)0)) 
         return 0;
    if (!u.dx && !u.dy) {
         /* Hopefully a mistake ;B */
         pline("Things may be going badly, but that's extreme.");
         return 0;
    }
    mtmp = m_at(u.ux + u.dx, u.uy + u.dy);
    if (!mtmp) {
         You("attack...nothing!");
         return 0;
    } else {
         int oldhp = mtmp->mhp;
         
         if (!attack(mtmp)) 
            return 0;
         if (!DEADMONSTER(mtmp) && mtmp->mhp < oldhp) {
            if (!has_head(mtmp->data) || u.uswallow)
                You_cant("perform cutthroat on %s!", mon_nam(mtmp));
            else {
                int tmp = 0;

                if (rn2(5) < (techlev(tech_no)/10 + 1)) {
                    You("sever %s head!", s_suffix(mon_nam(mtmp)));
                    tmp = mtmp->mhp;
                } else {
                    You("hurt %s badly!", s_suffix(mon_nam(mtmp)));
                    tmp = mtmp->mhp / 2;
                }
                tmp += techlev(tech_no);
                procdmg(mtmp, tmp, AD_PHYS);

            }
         }
    }
    return 1;
}

int
tech_blessing(tech_no)
int tech_no;
{
    char allowall[2], Your_buf[BUFSZ];
    struct obj *obj;
    const char *str;
    allowall[0] = ALL_CLASSES; 
    allowall[1] = '\0';
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    if (!(obj = getobj(allowall, "bless"))) 
         return 0;
    
    pline("An aura of holiness surrounds your hands!");
    if (!Blind) 
         (void) Shk_Your(Your_buf, obj);
    if (obj->cursed) {
         if (!Blind)
            pline("%s%s %s.",Your_buf, aobjnam(obj, "softly glow"),
                  hcolor(NH_AMBER));
         uncurse(obj);
         obj->bknown = 1;
    } else if (!obj->blessed) {
         if (!Blind) {
            str = hcolor(NH_LIGHT_BLUE);
            pline("%s%s with a%s %s aura.", Your_buf,
                  aobjnam(obj, "softly glow"), 
                  index(vowels, *str) ? "n" : "", str);
         }
         bless(obj);
         obj->bknown = 1;
    } else {
         if (obj->bknown) {
            pline ("That object is already blessed!");
            return 0;
         }
         obj->bknown = 1;
         pline("The aura fades.");
    }
    return 1;
}


int
tech_curse(tech_no)
int tech_no;
{
    char allowall[2], Your_buf[BUFSZ];
    struct obj *obj;
    const char *str;
    allowall[0] = ALL_CLASSES; 
    allowall[1] = '\0';
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    if (!(obj = getobj(allowall, "curse"))) 
         return 0;
    
    pline("An aura of evil surrounds your hands!");
    if (!Blind) 
         (void) Shk_Your(Your_buf, obj);
    if (obj->blessed) {
         if (!Blind)
            pline("%s%s %s.",Your_buf, aobjnam(obj, "darkly glow"),
                  hcolor(NH_AMBER));
         obj->blessed = 0;
         unbless(obj);
         obj->bknown = 1;
    } else if (!obj->cursed) {
         if (!Blind) {
            str = hcolor(NH_RED);
            pline("%s%s with a%s %s aura.", Your_buf,
                  aobjnam(obj, "harshly glow"), 
                  index(vowels, *str) ? "n" : "", str);
         }
         curse(obj);
         obj->bknown = 1;
    } else {
         if (obj->bknown) {
            pline ("That object is already cursed!");
            return 0;
         }
         obj->bknown = 1;
         pline("The aura fades.");
    }
    return 1;
}

/* WAC stolen from the spellcasters...'A' can identify from
 * historical research
 * */
int
tech_research(tech_no)
int tech_no;
{
    if (Hallucination || Stunned || Confusion) {
         You("can't concentrate right now!");
         return 0;
    } else if ((ACURR(A_INT) + ACURR(A_WIS)) < rnd(60)) {
         pline("Nothing in your pack looks familiar.");
         return 1;
    } else if (invent) {
         You("examine your possessions.");
         identify_pack((int) ((techlev(tech_no) / 10) + 1), FALSE);
         return 1;
    } else {
         /* KMH -- fixed non-compliant string */
         You("are already quite familiar with the contents of your pack.");
         return 0;
    }
}


int
tech_eviscerate(tech_no)
int tech_no;
{
    /* only when empty handed, in human form */
    if (Upolyd || uwep || uarmg) {
         You_cant("do this while %s!", 
                  Upolyd ? "polymorphed" :
                  uwep ? "holding a weapon" : "wearing gloves");
         return 0;
    }
    Your("fingernails extend into claws!");
    aggravate();
    techt_inuse(tech_no) = d(2, 4) + techlev(tech_no) / 5 + 2;
    return 1;
}

int
tech_berserk(tech_no)
int tech_no;
{
    You("fly into a berserk rage!");
    techt_inuse(tech_no) = d(2, 8) +
                           (techlev(tech_no) / 5) + 2;
    incr_itimeout(&HFast, techt_inuse(tech_no));
    return 1;
}


int
tech_reinforce(tech_no)
int tech_no;
{
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    /* WAC spell-users can study their known spells*/
    if (Hallucination || Stunned || Confusion) {
         You("can't concentrate right now!");
    } else {
         You("concentrate...");
         if (studyspell()) 
            return 1;
    }
    return 0;
}

int
tech_flurry(tech_no)
int tech_no;
{
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    Your("%s %s become blurs as they reach for your quiver!",
         uarmg ? "gloved" : "bare",      /* Del Lamb */
         makeplural(body_part(HAND)));
    techt_inuse(tech_no) = rnd((int) (techlev(tech_no) / 6 + 1)) + 2;
    return 1;
}

int
tech_appraisal(tech_no)
int tech_no;
{
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    if (!uwep) {
         You("are not wielding anything!");
    } else if (weapon_type(uwep) == P_NONE) {
         You("examine %s.", doname(uwep));
         uwep->known = TRUE;
         You("discover it is %s", doname(uwep));
         return 1;
    } else {
         You("examine %s.", doname(uwep));
         uwep->known = TRUE;
         You("discover it is %s", doname(uwep));
         return 1;
    }
    return 0;
}

int
tech_practice(tech_no)
int tech_no;
{
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    if (!uwep || (weapon_type(uwep) == P_NONE)) {
         You("are not wielding a weapon!");
         return 0;
    } else if (uwep->known == TRUE) {
         practice_weapon();
    } else {
         if (not_fully_identified(uwep)) {
            You("examine %s.", doname(uwep));
            if (rnd(15) <= ACURR(A_INT)) {
                makeknown(uwep->otyp);
                uwep->known = TRUE;
                You("discover it is %s", doname(uwep));
            } else
                pline("Unfortunately, you didn't learn anything new.");
         } 
         /*WAC Added practicing code - in weapon.c*/
         practice_weapon();
    }
    return 1;
}

int
tech_efist(tech_no)
int tech_no;
{
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    blitz_e_fist();
#if 0
    str = makeplural(body_part(HAND));
    You("focus the powers of the elements into your %s", str);
    techt_inuse(tech_no) = rnd((int) (techlev(tech_no) / 3 + 1)) + d(1, 4) + 2;
#endif
    return 0;
}

int
tech_primalroar(tech_no)
int tech_no;
{
    struct monst *mtmp;
    int i, j;
    You("let out a bloodcurdling roar!");
    aggravate();
    techt_inuse(tech_no) = d(4, 6) + (techlev(tech_no)) + 12;
    incr_itimeout(&HFast, techt_inuse(tech_no));
    
    for (i = -5; i <= 5; i++) {
         for (j = -5; j <= 5; j++) {
            if (isok(u.ux + i, u.uy + j)
                && (mtmp = m_at(u.ux + i, u.uy + j))) {
                if (mtmp->mtame != 0 && !mtmp->msummoned) {
                    /* Change from SLASH'EM: Instead of the temporary poly,
                     * we fully heal your close pets. */
                    mtmp->mhp = mtmp->mhpmax;
                }
            }
         }
    }
    return 1;
}

int
tech_liquidleap(tech_no)
int tech_no;
{
    struct monst *mtmp = NULL;
    coord cc;
    int dx, dy, sx, sy, range;
    
    pline("Where do you want to leap to?");
    cc.x = sx = u.ux;
    cc.y = sy = u.uy;

    getpos(&cc, TRUE, "the desired position");
    if (cc.x == -10)
         return 0; /* user pressed esc */

    dx = cc.x - u.ux;
    dy = cc.y - u.uy;
    /* allow diagonals */
    if (dx && dy && dx != dy && dx != -dy) {
         You("can only leap in straight lines!");
         return 0;
    } else if (distu(cc.x, cc.y) > 19 + techlev(tech_no)) {
         pline("Too far!");
         return 0;
    } else if (m_at(cc.x, cc.y) || !isok(cc.x, cc.y) ||
               IS_ROCK(levl[cc.x][cc.y].typ) ||
               sobj_at(BOULDER, cc.x, cc.y) ||
               closed_door(cc.x, cc.y)) {
         You_cant("flow there!"); /* MAR */
         return 0;
    } else {
         You("liquify!");
         if (Punished) {
            You("slip out of the iron chain.");
            unpunish();
         }
         if (u.utrap) {
            switch (u.utraptype) {
            case TT_BEARTRAP: 
                You("slide out of the bear trap.");
                break;
            case TT_PIT:
                You("leap from the pit!");
                break;
            case TT_WEB:
                You("flow through the web!");
                break;
            case TT_LAVA:
                You("separate from the lava!");
                u.utrap = 0;
                break;
            case TT_INFLOOR:
                u.utrap = 0;
                You("ooze out of the floor!");
            }
            u.utrap = 0;
         }
         /* Fry the things in the path ;B */
         if (dx) 
            range = dx;
         else
            range = dy;
         if (range < 0) 
            range = -range;
                
         dx = sgn(dx);
         dy = sgn(dy);
                
         while (range-- > 0) {
            int tmp_invul = 0;
                    
            if (!Invulnerable)
                Invulnerable = tmp_invul = 1;
            sx += dx; sy += dy;
            tmp_at(DISP_BEAM, zapdir_to_glyph(dx, dy, AD_ACID-1));
            tmp_at(sx,sy);
            delay_output(); /* wait a little */
            if ((mtmp = m_at(sx, sy)) != 0) {
                int chance;
                        
                chance = rn2(20);
                if (!chance || (3 - chance) > AC_VALUE(find_mac(mtmp)))
                    break;
                setmangry(mtmp, TRUE);
                You("catch %s in your acid trail!", mon_nam(mtmp));
                if (!resists_acid(mtmp)) {
                    int tmp = 1;
                    /* Need to add a to-hit */
                    tmp += d(2, 4);
                    tmp += rn2((int) (techlev(tech_no) / 5 + 1));
                    if (!Blind) 
                        pline_The("acid burns %s!", mon_nam(mtmp));
                    procdmg(mtmp, tmp, AD_ACID);
                } else if (!Blind) 
                    pline_The("acid doesn't affect %s!", mon_nam(mtmp));
            }
            /* Clean up */
            tmp_at(DISP_END, 0);
            if (tmp_invul) 
                Invulnerable = 0;
         }

         /* A little Sokoban guilt... */
         if (In_sokoban(&u.uz)) {
            change_luck(-1);
            pline("You cheater!");
         }
         You("reform!");
         teleds(cc.x, cc.y, FALSE);
         nomul(-1);
         multi_reason = "liquid leaping";
         nomovemsg = "";
    }
    return 1;
}

int
tech_raisezombies(tech_no)
int tech_no;
{
    /* This is passed to tamedog, reusing SPE_ANIMATE_DEAD instead of 
     * adding another case.
     * */
    struct obj* pseudo = mksobj(SPE_COMMAND_UNDEAD, FALSE, FALSE);
    struct monst *mtmp = NULL;
    struct obj *obj, *otmp;
    int i, j;

    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    You("chant the ancient curse...");
    for (i = -1; i <= 1; i++) {
         for (j = -1; j <= 1; j++) {
            int corpsenm;
            
            if (!isok(u.ux + i, u.uy + j))
                continue;
            for (obj = level.objects[u.ux + i][u.uy + j]; obj;
                 obj = otmp) {
                otmp = obj->nexthere;

                if (obj->otyp != CORPSE)
                    continue;
                /* Only generate undead */
                corpsenm = mon_to_zombie(obj->corpsenm);
                if (corpsenm != -1 && !cant_create(&corpsenm, TRUE)) {
                    /* Maintain approx. proportion of oeaten to cnutrit so that the zombie's HP relate roughly to how much of the original corpse was left.
                     */
                    if (obj->oeaten)
                        obj->oeaten =
                            eaten_stat(mons[corpsenm].cnutrit, obj);
                    obj->corpsenm = corpsenm;
                    mtmp = revive(obj, TRUE);
                    if (mtmp) {
                        if (!resist(mtmp, SPBOOK_CLASS, 0, TELL)) {
                            tamedog(mtmp, pseudo);
                            You("dominate %s!", mon_nam(mtmp));
                        } else {
                            setmangry(mtmp, FALSE);
                        }
                    }
                }
            }
         }
    }
    obfree(pseudo, NULL);
    /* You need to recover */
    nomul(-2);
    multi_reason = "recovering from an attempt to raise zombies";
    nomovemsg = "";
    return 1;
}

int
tech_callundead(tech_no)
int tech_no;
{
    register struct monst *nextmon;
    struct monst *mtmp = NULL;
    int pet_cnt = 0, omx, omy;
            
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    for (mtmp = fmon; mtmp; mtmp = nextmon) {
         nextmon = mtmp->nmon; /* trap might kill mon */
         if (DEADMONSTER(mtmp) || !is_undead(mtmp->data))
            continue;
         /* steed is already at your location, so not affected;
            this avoids trap issues if you're on a trap location */
         if (mtmp == u.usteed)
            continue;
         if (mtmp->mtame) {
            if (mtmp->mtrapped) {
                /* no longer in previous trap (affects mintrap) */
                mtmp->mtrapped = 0;
                fill_pit(mtmp->mx, mtmp->my);
            }
            omx = mtmp->mx, omy = mtmp->my;
            mnexto(mtmp);
            if (mtmp->mx != omx || mtmp->my != omy) {
                mtmp->mundetected = 0; /* reveal non-mimic hider */
                if (canspotmon(mtmp))
                    ++pet_cnt;
                if (mintrap(mtmp) == 2)
                    change_luck(-1);
            }
         }
    }
    return 1;
}

int
tech_revive(tech_no)
int tech_no;
{
    /* This is passed to tamedog, reusing SPE_ANIMATE_DEAD instead of 
     * adding another case.
     * */
    struct obj* pseudo = mksobj(SPE_COMMAND_UNDEAD, FALSE, FALSE);
    struct monst *mtmp = NULL;
    struct obj *obj;
    int num;
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    if (u.uswallow) {
         You(no_elbow_room);
         return 0;
    }
    num = 100 - techlev(tech_no); /* WAC make this depend on mon? */
    if ((Upolyd && u.mh <= num) || (!Upolyd && u.uhp <= num)){
         You("don't have the strength to perform revivification!");
         return 0;
    }
    
    if (!(obj = floorfood("revive", 1)))
         return 0;
            
    mtmp = revive(obj, TRUE);
    if (mtmp) {
         if (Is_blackmarket(&u.uz))
            setmangry(mtmp, FALSE);
         else {
            if (mtmp->isshk)
                make_happy_shk(mtmp, FALSE);
            else if (!resist(mtmp, SPBOOK_CLASS, 0, TELL)) {
                pseudo = mksobj(SPE_COMMAND_UNDEAD, FALSE, FALSE);
                tamedog(mtmp, pseudo);
                mtmp->mrevived = TRUE;
                obfree(pseudo, NULL);
            }
         }
    }
    if (Upolyd) 
         u.mh -= num;
    else 
         u.uhp -= num;
    return 1;
}


int
tech_tinker(tech_no)
int tech_no;
{
    if (Blind) {
         You("can't do any tinkering if you can't see!");
         return 0;
    }
    if (!uwep) {
         You("aren't holding an object to work on!");
         return 0;
    }
    You("are holding %s.", doname(uwep));
    if (yn("Start tinkering on this?") != 'y') 
         return 0;
    You("start working on %s", doname(uwep));
    delay = -150 + techlev(tech_no);
    set_occupation(tinker, "tinkering", 0);
    return 1;
}


int
tech_rage(tech_no)
int tech_no;
{
    int num;
    if (Upolyd) {
         You("cannot focus your anger!");
         return 0;
    }
    You("feel the anger inside you erupt!");
    num = 50 + (4 * techlev(tech_no));
    techt_inuse(tech_no) = num + 1;
    u.uhpmax += num;
    u.uhp += num;
    return 1;
}

int
tech_blink(tech_no)
int tech_no;
{
    You("feel the flow of time slow down.");
    techt_inuse(tech_no) = rnd(techlev(tech_no) + 1) + 2;
    return 1;
}

int
tech_drawenergy(tech_no)
int tech_no;
{
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    if (u.uen == u.uenmax) {
         if (Hallucination) 
            You("are fully charged!");
         else 
            You("cannot hold any more energy!");
         return 0;           		
    }
    You("begin drawing energy from your surroundings!");
    delay = -15;
    set_occupation(draw_energy, "drawing energy", 0);         
    return 1;
}


int
tech_chiheal(tech_no)
int tech_no;
{
    if (u.uen < 1) {
         You("are too weak to attempt this! You need at least one point of mana!");
         return 0;
    }
    You("direct your internal energy to restoring your body!");
    techt_inuse(tech_no) = techlev(tech_no) * 2 + 4;
    return 1;
}

int
tech_pickpocket(tech_no)
int tech_no;
{
    struct monst *mtmp = NULL;
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    if (!getdir(NULL))
         return 0;
    mtmp = m_at(u.ux + u.dx, u.uy + u.dy);
    if (!mtmp) {
         pline("No one is there!");
         return 0;
    }
    /* No timeout */
    return do_pickpocket(mtmp);
}

int
tech_disarm(tech_no)
int tech_no;
{
    struct monst *mtmp = (struct monst *) 0;
    struct obj *obj;
    int num;
    if (P_SKILL(weapon_type(uwep)) == P_NONE) {
         You("aren't wielding a proper weapon!");
         return 0;
    }
    if ((P_SKILL(weapon_type(uwep)) < P_SKILLED) || (Blind)) {
         You("aren't capable of doing this!");
         return 0;
    }
    if (u.uswallow) {
         pline("What do you think %s is?  A sword swallower?",
               mon_nam(u.ustuck));
         return 0;
    }
    
    if (!getdir((char *)0)) 
         return 0;
    if (!u.dx && !u.dy) {
         /* Hopefully a mistake ;B */
         pline("Why don't you try wielding something else instead.");
         return 0;
    }
    mtmp = m_at(u.ux + u.dx, u.uy + u.dy);
    if (!mtmp || !canspotmon(mtmp)) {
         You("don't see anything there!");
         return 0;
    }
    obj = MON_WEP(mtmp);   /* can be null */
    if (!obj) {
         You_cant("disarm an unarmed foe!");
         return 0;
    }
    /* Blindness dealt with above */
    if (!mon_visible(mtmp) ) {
         You_cant("see %s weapon!", s_suffix(mon_nam(mtmp)));
         return 0;
    }
    num = ((rn2(techlev(tech_no) + 15)) 
           * (P_SKILL(weapon_type(uwep)) - P_SKILLED + 1)) / 10;

    You("attempt to disarm %s...", mon_nam(mtmp));
    /* WAC can't yank out cursed items */
    if (num > 0 && (!Fumbling || !rn2(10)) && !obj->cursed) {
         int roll;
         obj_extract_self(obj);
         possibly_unwield(mtmp, FALSE);
         setmnotwielded(mtmp, obj);
         roll = rn2(num + 1);
         if (roll > 3) 
            roll = 3;
                
         switch (roll) {
         case 2:
            /* to floor near you */
            You("knock %s %s to the %s!", s_suffix(mon_nam(mtmp)),
                xname(obj), surface(u.ux, u.uy));
            if (obj->otyp == CRYSKNIFE && (!obj->oerodeproof || !rn2(10))) {
                obj->otyp = WORM_TOOTH;
                obj->oerodeproof = 0;
            }
            place_object(obj, u.ux, u.uy);
            stackobj(obj);
            break;
         case 3:
#if 0
            if (!rn2(25)) {
                /* proficient at disarming, but maybe not
                   so proficient at catching weapons */
                int hitu, hitvalu;

                hitvalu = 8 + obj->spe;
                hitu = thitu(hitvalu,
                        dmgval(obj, &youmonst),
                        obj, xname(obj));
                if (hitu)
                    pline("%s hits you as you try to snatch it!",
                            The(xname(obj)));
                place_object(obj, u.ux, u.uy);
                stackobj(obj);
                break;
            }
#endif /* 0 */
            /* right into your inventory */
            You("snatch %s %s!", s_suffix(mon_nam(mtmp)), xname(obj));
            if (obj->otyp == CORPSE 
                && touch_petrifies(&mons[obj->corpsenm]) 
                && !uarmg && !Stone_resistance 
                && !(poly_when_stoned(youmonst.data)
                     && polymon(PM_STONE_GOLEM))) {
                char kbuf[BUFSZ];

                Sprintf(kbuf, "%s corpse", an(mons[obj->corpsenm].mname));
                pline("Snatching %s is a fatal mistake.", kbuf);
                instapetrify(kbuf);
            }
            obj = hold_another_object(obj, "You drop %s!",
                                      doname(obj), (const char *)0);
            break;
         default:
            /* to floor beneath mon */
            You("knock %s from %s grasp!", the(xname(obj)),
                s_suffix(mon_nam(mtmp)));
            if (obj->otyp == CRYSKNIFE &&
                (!obj->oerodeproof || !rn2(10))) {
                obj->otyp = WORM_TOOTH;
                obj->oerodeproof = 0;
            }
            place_object(obj, mtmp->mx, mtmp->my);
            stackobj(obj);
            break;
         }
    } else if (mtmp->mcanmove && !mtmp->msleeping)
         pline("%s evades your attack.", Monnam(mtmp));
    else
         You("fail to dislodge %s %s.", s_suffix(mon_nam(mtmp)),
             xname(obj));
    wakeup(mtmp, TRUE);
    if (!mtmp->mcanmove && !rn2(10)) {
         mtmp->mcanmove = 1;
         mtmp->mfrozen = 0;
    }
    return 1;
}

int
tech_dazzle(tech_no)
int tech_no;
{
    struct monst *mtmp = (struct monst *) 0;
    int i;
    
    /* Short range stun attack */
    if (Blind) {
         You("can't see anything!");
         return 0;
    }
    if (!getdir((char *)0)) 
         return 0;
    if (!u.dx && !u.dy) {
         /* Hopefully a mistake ;B */
         You("can't see yourself!");
         return 0;
    }
    for (i = 0; (i  <= ((techlev(tech_no) / 8) + 1) 
                 && isok(u.ux + (i * u.dx), u.uy + (i * u.dy))); i++) {
         mtmp = m_at(u.ux + (i * u.dx), u.uy + (i * u.dy));
         if (mtmp && canseemon(mtmp)) 
            break;
    }
    if (!mtmp || !canseemon(mtmp)) {
         You("fail to make eye contact with anything!");
         return 0;
    }
    You("stare at %s.", mon_nam(mtmp));
    if (!haseyes(mtmp->data))
         pline("..but %s has no eyes!", mon_nam(mtmp));
    else if (!mtmp->mcansee)
         pline("..but %s cannot see you!", mon_nam(mtmp));
    if ((rn2(6) + rn2(6) + (techlev(tech_no) - mtmp->m_lev)) > 10) {
         You("dazzle %s!", mon_nam(mtmp));
         mtmp->mcanmove = 0;
         mtmp->mfrozen = rnd(10);
    } else {
         pline("%s breaks the stare!", Monnam(mtmp));
    }
    return 1;
}

int
tech_drawblood(tech_no)
int tech_no;
{
    struct obj *obj, *otmp;
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    /* ALI: Otherwise we get problems with what we create:
         * potions of vampire blood would no longer be appropriate. */
    if (!maybe_polyd(is_vampire(youmonst.data), Race_if(PM_VAMPIRIC))) {
         You("must be in your natural form to draw blood.");
         return 0;
    }
    obj = use_medical_kit(PHIAL, TRUE, "draw blood with");
    if (!obj)
         return 0;
    if (u.ulevel <= 1) {
         You_cant("seem to find a vein.");
         return 0;
    }
    check_unpaid(obj);
    pline("Using %s, you draw off a phial of your blood.", 
          yname(carrying(MEDICAL_KIT)));
    losexp("drawing blood");
    if (u.uexp > 0)
         u.uexp = newuexp(u.ulevel - 1);
    otmp = mksobj(POT_VAMPIRE_BLOOD, FALSE, FALSE);
    otmp->cursed = obj->cursed;
    otmp->blessed = obj->blessed;
            
    if (obj->quan > 1L)
         obj->quan--;
    else {
         obj_extract_self(obj);
         obfree(obj, (struct obj *)0);
    }
    (void) hold_another_object(
        otmp, "You fill, but have to drop, %s!", 
        doname(otmp), (const char *)0);
    return 1;
}

int
tech_jedijump(tech_no)
int tech_no;
{
    if (u.uen < 25){
         You("can't channel the force around you. Jedi jumps require 25 points of mana!");
         return 0;
    }
    if (!jump((techlev(tech_no) / 5) + 1))
         return 0;
    u.uen -= 25;
    return 1;
}

int
tech_booze(tech_no)
int tech_no;
{
    struct obj *udrink;
    int caughtW;
    You("procure some refreshing drinks.");
    make_confused(HConfusion + d(3,8), FALSE);
    /*healup(Role_if(PM_DRUNK) ? rnd(20 + u.ulevel) : 1, 0, FALSE, FALSE);*/
    healup(rnd(20 + u.ulevel), 0, FALSE, FALSE);
    u.uhunger += 20;
    /*if (Race_if(PM_CLOCKWORK_AUTOMATON)) u.uhunger += 200;*/
    /*if (Role_if(PM_DRUNK))*/
     u.uhunger += 100;
    newuhs(FALSE);
    exercise(A_WIS, FALSE);
    
    caughtW = 0;
    
    while (caughtW == 0) {
         udrink = mksobj(POT_BOOZE, TRUE, FALSE);
         udrink ->known = udrink ->dknown = 1;
         dropy(udrink);
         stackobj(udrink);
         if (techlev(tech_no) < rnd(40)) 
            caughtW++;
    }
    return 1;
}

int
tech_souleater(tech_no)
int tech_no;
{
    struct monst *mtmp;
    int num;
    num = Upolyd ? (u.mhmax / 2) : (u.uhpmax / 2);
    
    if ((!Upolyd && u.uhp <= num) || (Upolyd && u.mh <= num)) {
         You("don't have the strength to invoke Souleater! Requires at least %i HP!",
             num + 1);
         return 0;
    }
    if (!uwep) {
         You("need a weapon to channel your dark energy into!");
         return 0;
    }
    You("unleash a burst of dark energy!");
            
    /* Works against anything in line of sight...except flan */
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
         if (couldsee(mtmp->mx, mtmp->my) && !is_flan(mtmp->data)) {
            pline("%s screams in agony!", Monnam(mtmp));
            damage_mon(mtmp, mtmp->mhp / (dbl_dmg() ? 2 : 4), AD_PHYS);
            if (mtmp->mhp < 1)
                mtmp->mhp = 1;
         }
    }
    /* Instead of being limited to Soulthief (which Hack'EM is not
             * importing - we will allow any artifact that drains life. */
    int weapon_modifier = (attacks(AD_DRLI, uwep) ? 5 : 1);
                                   
    techt_inuse(tech_no) = (techlev(tech_no) / 2) + weapon_modifier;
    /* See uhitm.c in particular for the extra damage */
    pline("Dark flames flow from %s.", doname(uwep));
    if (Upolyd)
         u.mh -= num;
    else
         u.uhp -= num;
    context.botl = TRUE;
    return 1;
}

int
tech_shieldblock(tech_no)
int tech_no;
{
    if (!uarms) {
         You_cant("block attacks without a shield.");
         return 0;
    }
    You("inject your energy into %s shield.", yname(uarms));
    techt_inuse(tech_no) = d(3, 4) + techlev(tech_no);
    return 1;
}

int
tech_telekinesis(tech_no)
int tech_no;
{
    struct obj *otmp;
    struct trap *ttrap;
    coord cc;
    cc.x = u.ux;
    cc.y = u.uy;
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    do {
         pline("Apply telekinesis where?");
         if (getpos(&cc, TRUE, "apply telekinesis") < 0) {
            You("release the force energy back into your surroundings.");
            return 0;
         }
         if (!cansee(cc.x, cc.y)) {
            You_cant("see what's there!");
            continue;
         }
         if ((ttrap = t_at(cc.x, cc.y)) && ttrap->tseen
             && yn("Handle the trap here?") == 'y') {
            if (tele_disarm(ttrap, cc.x, cc.y)) {
                return 1;
            }
            else
                continue;
         } else if ((otmp = level.objects[cc.x][cc.y]) != 0) {
            char buf[BUFSZ];
            sprintf(buf, "Pick up %s?", the(xname(otmp)));
            if (yn(buf) == 'n')
                continue;
            else {
                You("pick up an object from the %s.", surface(cc.x, cc.y));
                (void) pickup_object(otmp, 1L, TRUE);
                newsym(cc.x, cc.y);
                return 1;
            }
         } else {
            You_cant("do anything %sthere.", ttrap ? "else " : "");
         }
    } while (TRUE);
    
}


/* Basically a wand of wind effect for the Jedi 
* Scale the hurtle distance a little with the techlevel.
* Used inside engulfer expels you.
* Using while conf/stunned fires in random dir.
* Can't use while afraid/weak.
 */
int
tech_forcepush(tech_no)
int tech_no;
{
    struct monst *mon;
    int i, sx = u.ux, sy = u.uy;
    int range = 1 + (techlev(tech_no) / 3);
    /* Need at least one free hand! */
    if (u.twoweap && uswapwep) {
         You("can't do this while two-weaponing!");
         return 0;
    } else if (uarms) {
         You("can't do this while holding a shield!");
         return 0;
    }
    if (!getdir(NULL))
         return 0;

    
    if (tech_no == -1) {
         return 0;
    }
    if (range > 3)
         range = 3;
    
    if (u.uswallow && u.ustuck) {
         mon = u.ustuck;
         You("blast a hole in %s!", mon_nam(mon));
         expels(mon, mon->data, TRUE);
    } else {
         for (i = 0; i < 3; i++) {
            if (!isok(sx, sy) || IS_STWALL(levl[sx][sy].typ))
                break;
            if ((mon = m_at(sx, sy)) != 0) {
                pline("%s gets blasted by the force!", Monnam(mon));
                mhurtle(mon, mon->mx - u.ux, mon->my - u.uy,
                        1 + rnd(techlev(1 + tech_no) / 3));
                break;
            }
            sx += u.dx;
            sy += u.dy;
         }
    }
    return 1;
}

int
tech_chargesaber(tech_no)
int tech_no;
{
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    if (!uwep || !is_lightsaber(uwep)) {
         You("are not holding a lightsaber!");
         return 0;
    }
    if (u.uen < 5) {
         You("lack the concentration to charge %s. You need at least 5 points of mana!",
             the(xname(uwep)));
         return 0;
    }
    You("start charging %s.", the(xname(uwep)));
    delay = -10;
    set_occupation(charge_saber, "charging", 0);
    return 1;
}


int
tech_tumble(tech_no)
int tech_no;
{
    int roll;
    struct monst *mtmp;
    struct trap *trtmp;
    int tx, ty;
    int tumbleskill = (techlev(tech_no) / 5) + 1;
    if (u.uswallow) {
         You("tumble in place.");
         return 1;
    }
    if (u.utrap) {
         You("cannot tumble until you extricate yourself.");
         return 0;
    }
    if (Fumbling) {
         You("slip and land on your %s!", body_part(HEAD));
         losehp(Maybe_Half_Phys(rnd(20)), "ill-advised gymnastics",
                KILLED_BY);
         return 1;
    }
    roll = rn2(6);
    if (tumbleskill <= P_UNSKILLED || roll > tumbleskill) {
         You(Hallucination ? "are too busy tripping!" : "trip!");
         return 1;
    }
    if (!getdir((char *)0) || !isok(u.ux + u.dx, u.uy + u.dy)) 
         return 0;
    if (!u.dx && !u.dy) {
         You("do a sommersault.");
         return 1;
    }
    mtmp = m_at(u.ux + u.dx, u.uy + u.dy);
    if (!mtmp || !canspotmon(mtmp)) {
         You("don't see anyone to tumble past in that direction.");
         return 0;
    }
    You("tumble past %s.", mon_nam(mtmp));
    tx = u.ux;
    ty = u.uy;
    u.ux = mtmp->mx;
    u.uy = mtmp->my;
    remove_monster(mtmp->mx, mtmp->my);
    place_monster(mtmp, tx, ty);
    trtmp = t_at(u.ux, u.uy);
    if (trtmp) dotrap(trtmp, FORCETRAP);
    if (roll > tumbleskill) {
         nomul(-rnd(2));
         multi_reason = "recovering from a tumble";
         nomovemsg = "You recover from your tumble.";
    }
    newsym(u.ux, u.uy);
    newsym(mtmp->mx, mtmp->my);
    return 1;
}

int
tech_sunder(tech_no)
int tech_no;
{
    struct obj *otmp;
    struct monst *mtmp;
    int roll;
    int skill = (techlev(tech_no) / 5) + 2; /* +2 so it starts at basic */
    skill = (skill > 3) ? 3 : skill;
    if (skill <= P_UNSKILLED) {
         You("are not skilled enough to attempt to sunder objects.");
         return 0;
    }
    if (u.uswallow) {
         pline("Try attacking, instead.");
         return 0;
    }
    if (!getdir((char *)0) || !isok(u.ux + u.dx, u.uy + u.dy)) return 0;
    if (!u.dx && !u.dy) {
         if (uwep) You("cannot break your own weapon!");
         else pline("Break your %s? You need those!", makeplural(body_part(HAND)));
    }
    mtmp = m_at(u.ux + u.dx, u.uy + u.dy);
    if (!mtmp || !canspotmon(mtmp)) {
         You("don't see anyone with equipment to sunder!");
         return 0;
    }
    if (!MON_WEP(mtmp)) {
         pline("%s does not have a weapon for you to sunder.", Monnam(mtmp));
         return 0;
    }
    roll = rn2(6);
    otmp = MON_WEP(mtmp);
    if (roll >= (skill - (otmp->oartifact ? 2 : 0))) {
         pline("%d %d", roll, skill);
         You("fail to sunder %s's weapon.", mon_nam(mtmp));
         return 1;
    }

    You("sunder %s's %s!", mon_nam(mtmp), xname(otmp));
    if (touch_petrifies(&mons[otmp->corpsenm])) {
         if (!uwep && !Stone_resistance) {
            Sprintf(killer.name, "sundering %s with an unarmed strike",
                    killer_xname(otmp));
            instapetrify(killer.name);
         } else if (!uwep) {
            uwep->material = STONE;
         }
         update_inventory();
         context.botl = 1;
    }
    setmangry(mtmp, TRUE);
    m_useup(mtmp, otmp);
    return 1;
}

int
tech_bloodmagic(tech_no)
int tech_no;
{
    if (tech_no == -1) { /* Prevent unused var warning */
         return 0;
    }
    You("invite a dark power into your heart!");
    techt_inuse(tech_no) = (int) (techlev(tech_no) * 6 + 1) + 2;
    return 1;
}

#ifdef DEBUG
void
wiz_debug_cmd() /* in this case, allow controlled loss of techniques */
{
    int tech_no, id, n = 0;
    long mask;
    if (gettech(&tech_no)) {
        id = techid(tech_no);
        if (id == NO_TECH) {
             impossible("Unknown technique ([%d])?", tech_no);
             return;
        }
        mask = tech_list[tech_no].t_intrinsic;
        if (mask & FROMOUTSIDE)
             n++;
        if (mask & FROMRACE)
             n++;
        if (mask & FROMEXPER)
             n++;
        if (!n) {
             impossible("No intrinsic masks set (0x%lX).", mask);
             return;
        }
        n = rn2(n);
        if (mask & FROMOUTSIDE && !n--)
             mask = FROMOUTSIDE;
        if (mask & FROMRACE && !n--)
             mask = FROMRACE;
        if (mask & FROMEXPER && !n--)
             mask = FROMEXPER;
        learntech(id, mask, -1);
        if (!tech_known(id))
             You("lose the ability to perform %s.", tech_names[id]);
    }
}

#endif /* DEBUG */
