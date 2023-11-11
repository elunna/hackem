/* NetHack 3.6	mcastu.c	$NHDT-Date: 1567418129 2019/09/02 09:55:29 $  $NHDT-Branch: NetHack-3.6 $:$NHDT-Revision: 1.55 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "mextra.h"

extern void demonpet();

/* monster mage spells */
enum mcast_mage_spells {
    MGC_PSI_BOLT = 0,
    MGC_FIRE_BOLT,
    MGC_ICE_BOLT,
    MGC_CURE_SELF,
    MGC_HASTE_SELF,
    MGC_STUN_YOU,
    MGC_DISAPPEAR,
    MGC_WEAKEN_YOU,
    MGC_DESTRY_ARMR,
    MGC_CURSE_ITEMS,
    MGC_AGGRAVATION,
    MGC_ACID_BLAST,
    MGC_SUMMON_MONS,
    MGC_CLONE_WIZ,
    MGC_CANCELLATION,
    MGC_REFLECTION,
    MGC_DEATH_TOUCH,
    MGC_CREATE_POOL,
    MGC_CALL_UNDEAD,
};

/* monster cleric spells */
enum mcast_cleric_spells {
    CLC_OPEN_WOUNDS = 0,
    CLC_CURE_SELF,
    CLC_PROTECTION,
    CLC_CONFUSE_YOU,
    CLC_PARALYZE,
    CLC_VULN_YOU,
    CLC_BLIND_YOU,
    CLC_INSECTS,
    CLC_CURSE_ITEMS,
    CLC_LIGHTNING,
    CLC_FIRE_PILLAR,
    CLC_GEYSER,
    CLC_SUMMON_ELM
};

extern void you_aggravate(struct monst *);

STATIC_DCL void FDECL(cursetxt, (struct monst *, BOOLEAN_P));
STATIC_DCL int FDECL(choose_magic_spell, (struct monst *, int));
STATIC_DCL int FDECL(choose_clerical_spell, (struct monst *, int));
STATIC_DCL int FDECL(m_cure_self, (struct monst *, int));
STATIC_DCL int FDECL(m_destroy_armor, (struct monst *, struct monst *));
STATIC_DCL void FDECL(cast_wizard_spell, (struct monst *, int, int));
STATIC_DCL void FDECL(cast_cleric_spell, (struct monst *, int, int));
STATIC_DCL boolean FDECL(is_undirected_spell, (unsigned int, int));
STATIC_DCL boolean FDECL(spell_would_be_useless, (struct monst *, unsigned int, int));
STATIC_DCL boolean FDECL(uspell_would_be_useless,(unsigned int,int));
STATIC_DCL void FDECL(ucast_wizard_spell, (struct monst *, struct monst *, int, int));
STATIC_DCL void FDECL(ucast_cleric_spell, (struct monst *, struct monst *, int, int));
STATIC_DCL int FDECL(mcastrange, (int));

boolean
is_spellcaster(mtmp)
register struct monst *mtmp;
{
    int i = 0;
    struct attack *mattk;
    mattk = has_erac(mtmp) ? ERAC(mtmp)->mattk : mtmp->data->mattk;

    for (; i < NATTK; i++) {
        if (mattk[i].aatyp == AT_MAGC
            && (mattk[i].adtyp == AD_SPEL
                || mattk[i].adtyp == AD_CLRC)) {
            return TRUE;
        }
    }
    return FALSE;
}

extern const char *const flash_types[]; /* from zap.c */

/* feedback when frustrated monster couldn't cast a spell */
STATIC_OVL
void
cursetxt(mtmp, undirected)
struct monst *mtmp;
boolean undirected;
{
    if (canseemon(mtmp) && couldsee(mtmp->mx, mtmp->my)) {
        const char *point_msg; /* spellcasting monsters are impolite */

        if (undirected)
            point_msg = "all around, then curses";
        else if ((Invis && !mon_prop(mtmp, SEE_INVIS)
                  && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                 || is_obj_mappear(&youmonst, STRANGE_OBJECT)
                 || u.uundetected)
            point_msg = "and curses in your general direction";
        else if (Displaced && (mtmp->mux != u.ux || mtmp->muy != u.uy))
            point_msg = "and curses at your displaced image";
        else
            point_msg = "at you, then curses";

        pline("%s points %s.", Monnam(mtmp), point_msg);
    } else if ((!(moves % 4) || !rn2(4))) {
        if (!Deaf)
            Norep("You hear a mumbled curse.");   /* Deaf-aware */
    }
}

/* convert a level based random selection into a specific mage spell;
   inappropriate choices will be screened out by spell_would_be_useless() */
STATIC_OVL int
choose_magic_spell(mtmp, spellval)
struct monst* mtmp;
int spellval;
{
    int i;

    /* for 3.4.3 and earlier, val greater than 22 selected the default spell
     */
    while (spellval > 25 && rn2(25))
        spellval = rn2(spellval);

    /* If we're hurt, seriously consider fixing ourselves a priority */
    if ((mtmp->mhp * 4) <= mtmp->mhpmax)
        spellval = 1;

    switch (spellval) {
    case 25:
    case 24:
        i = rn2(3);
        switch (i) {
        case 2:
            return MGC_FIRE_BOLT;
        case 1:
            return MGC_ICE_BOLT;
        case 0:
        default:
            if (Antimagic || Hallucination)
                return MGC_PSI_BOLT;
        }
        /*FALLTHRU*/
    case 23:
    case 22:
    case 21:
        return MGC_DEATH_TOUCH;
    case 20:
    case 19:
        return MGC_CLONE_WIZ;
    case 18:
    case 17:
        return MGC_SUMMON_MONS;
    case 16:
        return MGC_CANCELLATION;
    case 15:
        return MGC_ACID_BLAST;
    case 14:
        i = rnd(2);
        switch (i) {
        case 1:
            return MGC_AGGRAVATION;
        case 2:
            return MGC_REFLECTION;
        }
        return i;
    case 13:
        return MGC_CREATE_POOL;
    case 12:
    case 11:
        return MGC_CURSE_ITEMS;
    case 10:
        return MGC_CALL_UNDEAD;
    case 9:
    case 8:
        return MGC_DESTRY_ARMR;
    case 7:
    case 6:
        return MGC_WEAKEN_YOU;
    case 5:
    case 4:
        return MGC_DISAPPEAR;
    case 3:
        return MGC_STUN_YOU;
    case 2:
        return MGC_HASTE_SELF;
    case 1:
        return MGC_CURE_SELF;
    case 0:
    default:
        i = rn2(3);
        switch (i) {
        case 2:
            return MGC_FIRE_BOLT;
        case 1:
            return MGC_ICE_BOLT;
        case 0:
        default:
            if (Antimagic || Hallucination)
                return MGC_PSI_BOLT;
        }
        return i;
    }
}

/* convert a level based random selection into a specific cleric spell */
STATIC_OVL int
choose_clerical_spell(mtmp, spellnum)
struct monst* mtmp;
int spellnum;
{
    /* for 3.4.3 and earlier, num greater than 13 selected the default spell
     */
    while (spellnum > 15 && rn2(16))
        spellnum = rn2(spellnum);
    
    /* If we're hurt, seriously consider giving fixing ourselves priority */
    if ((mtmp->mhp * 4) <= mtmp->mhpmax)
        spellnum = 1;

    if (mtmp->data == &mons[PM_ARCH_VILE] && spellnum != 1)
        return CLC_FIRE_PILLAR;
    
    switch (spellnum) {
    case 15:
    case 14:
        if (rn2(3))
            return CLC_OPEN_WOUNDS;
        /*FALLTHRU*/
    case 13:
        if (is_demon(mtmp->data)
            || mtmp->mtame || mtmp->mpeaceful)
            return CLC_FIRE_PILLAR;
        else
            return CLC_SUMMON_ELM;
    case 12:
        return CLC_GEYSER;
    case 11:
        if (mtmp->data == &mons[PM_ASMODEUS])
            return CLC_LIGHTNING;
        else
            return CLC_FIRE_PILLAR;
    case 10:
        return CLC_LIGHTNING;
    case 9:
        return CLC_CURSE_ITEMS;
    case 8:
        if ((is_demon(mtmp->data) && mtmp->data != &mons[PM_LOLTH])
            || mtmp->mtame || mtmp->mpeaceful)
            return CLC_VULN_YOU;
        else
            return CLC_INSECTS;
    case 7:
    case 6:
        return CLC_BLIND_YOU;
    case 5:
        return CLC_VULN_YOU;
    case 4:
        return CLC_PARALYZE;
    case 3:
        return CLC_CONFUSE_YOU;
    case 2:
        return CLC_PROTECTION;
    case 1:
        return CLC_CURE_SELF;
    case 0:
    default:
        return CLC_OPEN_WOUNDS;
    }
}

/* return values:
 * 1: successful spell
 * 0: unsuccessful spell
 */
int
castmu(magr, mattk, thinks_it_foundyou, foundyou)
register struct monst *magr;
register struct attack *mattk;
boolean thinks_it_foundyou;
boolean foundyou;
{
    int dmg, ml = min(magr->m_lev, 50);
    int ret;
    int spellnum = 0;
    struct obj *marm;
    /* Three cases:
     * -- monster is attacking you.  Search for a useful spell.
     * -- monster thinks it's attacking you.  Search for a useful spell,
     *    without checking for undirected.  If the spell found is directed,
     *    it fails with cursetxt() and loss of mspec_used.
     * -- monster isn't trying to attack.  Select a spell once.  Don't keep
     *    searching; if that spell is not useful (or if it's directed),
     *    return and do something else.
     * Since most spells are directed, this means that a monster that isn't
     * attacking casts spells only a small portion of the time that an
     * attacking monster does.
     */
    if ((mattk->adtyp == AD_SPEL || mattk->adtyp == AD_CLRC) && ml) {
        int cnt = 40;

        do {
            spellnum = rn2(ml);
            if (mattk->adtyp == AD_SPEL)
                spellnum = choose_magic_spell(magr, spellnum);
            else
                spellnum = choose_clerical_spell(magr, spellnum);
            /* not trying to attack?  don't allow directed spells */
            if (!thinks_it_foundyou) {
                if (!is_undirected_spell(mattk->adtyp, spellnum)
                    || spell_would_be_useless(magr, mattk->adtyp, spellnum)) {
                    if (foundyou)
                        impossible(
                       "spellcasting monster found you and doesn't know it?");
                    return 0;
                }
                break;
            }
        } while (--cnt > 0
                 && spell_would_be_useless(magr, mattk->adtyp, spellnum));
        if (cnt == 0)
            return 0;
    }

    /* monster unable to cast spells? */
    if (magr->mcan || magr->mspec_used || !ml) {
        cursetxt(magr, is_undirected_spell(mattk->adtyp, spellnum));
        return 0;
    }

    if (mattk->adtyp == AD_SPEL || mattk->adtyp == AD_CLRC) {
        struct obj *obj;
        magr->mspec_used = 4 - magr->m_lev;
        if (magr->mspec_used < 2)
            magr->mspec_used = 2;
        /* many boss-type monsters than have two or more spell attacks
           per turn are never able to fire off their second attack due
           to mspec always being greater than 0. So set to 0 for those
           types of monsters half of the time */
        if (rn2(2)
            && (is_dprince(magr->data) || is_dlord(magr->data)
                || magr->iswiz || magr->isvecna
                || magr->data->msound == MS_LEADER
                || magr->data->msound == MS_NEMESIS
                || magr->data == &mons[PM_ORACLE]
                || magr->data == &mons[PM_HIGH_PRIEST]))
            magr->mspec_used = 0;

        /* Having the EotA in inventory drops mspec to 0 */
        for (obj = magr->minvent; obj; obj = obj->nobj) {
            if (obj->oartifact
                && obj->oartifact == ART_EYE_OF_THE_AETHIOPICA) {
                magr->mspec_used = 0;
                break;
            }
        }
    }

    /* monster can cast spells, but is casting a directed spell at the
       wrong place?  If so, give a message, and return.  Do this *after*
       penalizing mspec_used. */
    if (!foundyou && thinks_it_foundyou
        && !is_undirected_spell(mattk->adtyp, spellnum)) {
        pline("%s casts a spell at %s!",
              canseemon(magr) ? Monnam(magr) : "Something",
              levl[magr->mux][magr->muy].typ == WATER ? "empty water"
                                                      : "thin air");
        return 0;
    }

    if (!magr->mpeaceful)
        nomul(0);
    if (rn2(ml * 10) < (magr->mconf ? 100 : 10)) { /* fumbled attack */
        if (canseemon(magr) && !Deaf)
            pline_The("air crackles around %s.", mon_nam(magr));
        return 0;
    }
    if (canseemon(magr) || !is_undirected_spell(mattk->adtyp, spellnum)) {
        if (magr->mpeaceful
            && magr->ispriest && inhistemple(magr)) {
            ; /* cut down on the temple spam */
        } else {
            pline("%s casts a spell%s!",
                  canseemon(magr) ? Monnam(magr) : "Something",
                  is_undirected_spell(mattk->adtyp, spellnum)
                      ? ""
                      : (Invis && !mon_prop(magr, SEE_INVIS)
                         && (magr->mux != u.ux || magr->muy != u.uy))
                            ? " at a spot near you"
                            : (Displaced
                               && (magr->mux != u.ux || magr->muy != u.uy))
                                  ? " at your displaced image"
                                  : " at you");
        }
    }

    /*
     * As these are spells, the damage is related to the level
     * of the monster casting the spell.
     */
    if (!foundyou) {
        dmg = 0;
        if (mattk->adtyp != AD_SPEL && mattk->adtyp != AD_CLRC) {
            impossible(
              "%s casting non-hand-to-hand version of hand-to-hand spell %d?",
                       Monnam(magr), mattk->adtyp);
            return 0;
        }
    } else if (mattk->damd)
        dmg = d((int) ((ml / 2) + mattk->damn), (int) mattk->damd);
    else
        dmg = d((int) ((ml / 2) + 1), 6);
    if (Half_spell_damage)
        dmg = (dmg + 1) / 2;

    ret = 1;

    switch (mattk->adtyp) {
    case AD_FIRE:
        if (is_demon(magr->data))
            pline("You're enveloped in hellfire!");
        else if (!Underwater)
            pline("You're enveloped in flames.");
        else {
            pline("The flames are quenched by the water around you.");
            dmg = 0;
            break;
        }

        if (how_resistant(FIRE_RES) == 100) {
            shieldeff(u.ux, u.uy);
            if (is_demon(magr->data)) {
                if (Race_if(PM_DEMON) || nonliving(youmonst.data)) {
                    dmg = 0;
                } else {
                    pline_The("hellish flames sear your soul!");
                    dmg = (dmg + 1) / 2;
                }
            } else {
                pline("But you resist the effects.");
                monstseesu(M_SEEN_FIRE);
                dmg = 0;
            }
        } else {
            if (is_demon(magr->data)
                && !(Race_if(PM_DEMON) || nonliving(youmonst.data))) {
                pline_The("hellish flames sear your soul!");
                dmg = resist_reduce(dmg, FIRE_RES) * 2;
            } else {
                dmg = resist_reduce(dmg, FIRE_RES);
            }
        }
        burn_away_slime();
        break;
    case AD_COLD:
        pline("You're covered in frost.");
        if (how_resistant(COLD_RES) == 100) {
            shieldeff(u.ux, u.uy);
            pline("But you resist the effects.");
            monstseesu(M_SEEN_COLD);
            dmg = 0;
        } else {
            dmg = resist_reduce(dmg, COLD_RES);
        }
        break;
    case AD_ACID:
        if (!Underwater)
            pline("You're splashed in acid.");
        else {
            pline("The acid dissipates harmlessly in the water around you.");
            dmg = 0;
            break;
        }
        if (how_resistant(ACID_RES) == 100) {
            shieldeff(u.ux, u.uy);
            pline("But you resist the effects.");
            monstseesu(M_SEEN_ACID);
            dmg = 0;
        } else {
            dmg = d((int) ml / 2 + 1, 8);
        }
        break;
    case AD_MAGM:
        You("are hit by a shower of missiles!");
        dmg = d((int) ml / 2 + 1, 6);
        if (Antimagic) {
            shieldeff(u.ux, u.uy);
            pline("Some missiles bounce off!");
            monstseesu(M_SEEN_MAGR);
            dmg = (dmg + 1) / 2;
        }
        break;
    case AD_PSYC:
        marm = which_armor(magr, W_ARMH);
        pline("Your mind is being attacked!");
        
        if (Psychic_resistance ) {
            shieldeff(u.ux, u.uy);
            pline("You fend off the mental attack!");
            dmg = 0;
        } else if (mindless(youmonst.data)) {
            shieldeff(magr->mx, magr->my);
            You("are unaffected by the psionic energy!");
            dmg = 0;
        } else {
            /* Mon wearing helm of telepathy gets bonus */
            if (marm && marm->otyp == HELM_OF_TELEPATHY)
                dmg += rnd(6) + 2;
            if (marm && marm->otyp == TINFOIL_HAT)
                dmg = 0;
        }
        if (Blind_telepat || Unblind_telepat) {
            dmg *= 2; /* You are more sensitive */
            pline("%s locks on to your telepathic mind!", Monnam(magr));
        }
            
        if (dmg && !rn2(4)) {
            make_confused(HConfusion + d(3, 4), FALSE);
        }
        break;
    case AD_SPEL:   /* wizard spell */
    case AD_CLRC: { /* clerical spell */
        if (mattk->adtyp == AD_SPEL)
            cast_wizard_spell(magr, dmg, spellnum);
        else
            cast_cleric_spell(magr, dmg, spellnum);
        dmg = 0; /* done by the spell casting functions */
        break;
        }
    }
    if (dmg)
        mdamageu(magr, dmg);
    return (ret);
}

STATIC_OVL int
m_cure_self(mtmp, dmg)
struct monst *mtmp;
int dmg;
{
    if (mtmp->mhp < mtmp->mhpmax) {
        if (canseemon(mtmp))
            pline("%s looks better.", Monnam(mtmp));
        /* note: player healing does 6d4; this used to do 1d8 */
        if ((mtmp->mhp += d(3, 6)) > mtmp->mhpmax)
            mtmp->mhp = mtmp->mhpmax;
        dmg = 0;
    }
    return dmg;
}

STATIC_OVL int
m_destroy_armor(magr, mdef)
struct monst *magr, *mdef;
{
    boolean udefend = (mdef == &youmonst),
            uattk = (magr == &youmonst);
    int erodelvl = rnd(3);
    struct obj *oatmp;

    if (udefend ? Antimagic
                : (resists_magm(mdef) || defended(mdef, AD_MAGM))) {
        if (udefend) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_MAGR);
        } else {
            shieldeff(mdef->mx, mdef->my);
        }
        erodelvl = 1;
    }

    oatmp = some_armor(mdef);
    if (oatmp) {
        if (any_quest_artifact(oatmp)) {
            if (udefend || canseemon(mdef)) {
                if (!Blind)
                    pline("%s shines brightly.", The(xname(oatmp)));
                pline("%s is immune to %s destructive magic.",
                      The(xname(oatmp)),
                      uattk ? "your" : s_suffix(mon_nam(magr)));
            }
            return 0;
        } else if (oatmp && oatmp->oprops & ITEM_TOUGH) {
            oatmp->oprops_known |= ITEM_TOUGH;
            pline("%s safely absorbs the magic.", Yname2(oatmp));
            return 0;
        } else if (oatmp->oerodeproof) {
            if (!udefend && !canseemon(mdef)) {
                You("smell something strange.");
            } else if (!Blind) {
                pline("%s glows brown for a moment.", Yname2(oatmp));
            } else {
                pline("%s briefly emits an odd smell.", Yname2(oatmp));
            }
            oatmp->oerodeproof = 0;
            erodelvl--;
        }

        if (greatest_erosion(oatmp) >= MAX_ERODE) {
            if (objects[oatmp->otyp].oc_oprop == DISINT_RES
                || obj_resists(oatmp, 0, 90))
                return 0;
            if (udefend) {
                destroy_arm(oatmp, FALSE);
            } else {
                if (canseemon(mdef)) {
                    const char *action;
                    if (is_cloak(oatmp))
                        action = "crumbles and turns to dust";
                    else if (is_shirt(oatmp))
                        action = "crumbles into tiny threads";
                    else if (is_helmet(oatmp))
                        action = "turns to dust and is blown away";
                    else if (is_gloves(oatmp))
                        action = "vanish";
                    else if (is_boots(oatmp))
                        action = "disintegrate";
                    else if (is_shield(oatmp))
                        action = "crumbles away";
                    else
                        action = "turns to dust";
                    pline("%s %s %s!", s_suffix(Monnam(mdef)), xname(oatmp),
                          action);
                }
                m_useupall(mdef, oatmp);
            }
        } else {
            int erodetype;
            if (is_corrodeable(oatmp))
                erodetype = ERODE_CORRODE;
            else if (is_flammable(oatmp))
                erodetype = ERODE_BURN;
            else if (is_glass(oatmp))
                erodetype = ERODE_FRACTURE;
            else if (is_supermaterial(oatmp))
                erodetype = ERODE_DETERIORATE;
            else
                erodetype = ERODE_ROT;

            while (erodelvl-- > 0) {
                (void) erode_obj(oatmp, (char *) 0, erodetype, EF_NONE);
            }
        }
    } else {
        if (udefend)
            Your("body itches.");
        else if (uattk || canseemon(mdef))
            pline("%s seems irritated.", Monnam(mdef));
    }
    update_inventory();

    return 0;
}

/* monster wizard and cleric spellcasting functions */
/*
   If dmg is zero, then the monster is not casting at you.
   If the monster is intentionally not casting at you, we have previously
   called spell_would_be_useless() and spellnum should always be a valid
   undirected spell.
   If you modify either of these, be sure to change is_undirected_spell()
   and spell_would_be_useless().
 */
STATIC_OVL
void
cast_wizard_spell(mtmp, dmg, spellnum)
struct monst *mtmp;
int dmg;
int spellnum;
{
    int ml = min(mtmp->m_lev, 50);
    coord mm;
    
    if (dmg == 0 && !is_undirected_spell(AD_SPEL, spellnum)) {
        impossible("cast directed wizard spell (%d) with dmg=0?", spellnum);
        return;
    }

    switch (spellnum) {
    case MGC_DEATH_TOUCH:
        pline("Oh no, %s's using the touch of death!", mhe(mtmp));
        if (Death_resistance || immune_death_magic(youmonst.data)) {
            You("%s.", nonliving(youmonst.data)
                ? "seem no more dead than before"
                : "are unaffected");
        } else {
            if (Hallucination) {
                You("have an out of body experience.");
            } else if (!Antimagic) {
                killer.format = KILLED_BY_AN;
                Strcpy(killer.name, "touch of death");
                done(DIED);
            } else if (Antimagic) {
                dmg = d(8, 6);
                /* Magic resistance or half spell damage will
                   cut this in half */
                if (Antimagic || Half_spell_damage) {
                    shieldeff(u.ux, u.uy);
                    monstseesu(M_SEEN_MAGR);
                    dmg /= 2;
                }
                You_feel("drained...");
                u.uhpmax -= dmg / 3 + rn2(5);
                losehp(dmg, "touch of death", KILLED_BY_AN);
            }
        }
        dmg = 0;
        break;
    /* Imported from slashem-up 
     * Create pool spell can be affected by invisibility and/or
     * displacement.
     */
    case MGC_CREATE_POOL: 
        if (Invisible && !perceives(mtmp->data)
              && (mtmp->mux != u.ux || mtmp->muy != u.uy)
              && zombie_can_dig(mtmp->mux, mtmp->muy)) {
            pline("A pool appears beneath a spot near you!");
            levl[mtmp->mux][mtmp->muy].typ = POOL;
            del_engr_at(mtmp->mux, mtmp->muy);
            water_damage_chain(level.objects[mtmp->mux][mtmp->muy], TRUE, 0, TRUE, mtmp->mux, mtmp->muy);
            spoteffects(FALSE);
        } else if (Displaced 
                   && (mtmp->mux != u.ux || mtmp->muy != u.uy)
                   && zombie_can_dig(mtmp->mux, mtmp->muy)) {
            pline("A pool appears beneath your displaced image!");
            levl[mtmp->mux][mtmp->muy].typ = POOL;
            del_engr_at(mtmp->mux, mtmp->muy);
            water_damage_chain(level.objects[mtmp->mux][mtmp->muy], TRUE, 0, TRUE, mtmp->mux, mtmp->muy);
            spoteffects(FALSE);
        }
        else if (zombie_can_dig(u.ux, u.uy)) {
            pline("A pool appears beneath you!");
            levl[u.ux][u.uy].typ = POOL;
            del_engr_at(u.ux, u.uy);
            water_damage_chain(level.objects[u.ux][u.uy], TRUE, 0, TRUE, u.ux, u.uy);
            spoteffects(FALSE);  /* possibly drown, notice objects */
        }
        else
            pline("Some water comes down from the ceiling.");
        dmg = 0;
        break;
    case MGC_CANCELLATION:
        if (m_canseeu(mtmp) && lined_up(mtmp)) {
            if (canseemon(mtmp))
                pline("%s %s a cancellation spell!", Monnam(mtmp),
                      rn2(2) ? "evokes" : "conjures up");
            else if (!Deaf)
                You_hear("a powerful spell being %s.",
                         rn2(2) ? "spoken" : "uttered");
            (void) cancel_monst(&youmonst, (struct obj *) 0,
                                FALSE, TRUE, FALSE);
        }
        dmg = 0;
        break;
    case MGC_REFLECTION:
        (void) cast_reflection(mtmp);
        dmg = 0;
        break;
    case MGC_ACID_BLAST:
        if (m_canseeu(mtmp) 
              && distu(mtmp->mx, mtmp->my) <= (ml < mcastrange(ml))) {
            pline("%s douses you in a torrent of acid!", Monnam(mtmp));
            explode(u.ux, u.uy, ZT_ACID, d((ml / 2) + 4, 8),
                    MON_CASTBALL, EXPL_ACID);
            if (how_resistant(ACID_RES) == 100) {
                shieldeff(u.ux, u.uy);
                pline_The("acid dissipates harmlessly.");
                monstseesu(M_SEEN_ACID);
            }
            if (rn2(u.twoweap ? 2 : 3))
                acid_damage(uwep);
            if (u.twoweap && rn2(2))
                acid_damage(uswapwep);
            if (rn2(4))
                erode_armor(&youmonst, ERODE_CORRODE);
        } else {
            if (canseemon(mtmp)) {
                pline("%s blasts the %s with %s and curses!",
                      Monnam(mtmp), rn2(2) ? "ceiling"
                                           : "floor", "acid");
            } else {
                You_hear("some cursing!");
            }
        }
        /* damage is handled by explode() */
        dmg = 0;
        break;
    case MGC_CLONE_WIZ:
        if (mtmp->iswiz && context.no_of_wizards == 1) {
            pline("Double Trouble...");
            clonewiz();
            dmg = 0;
        } else
            impossible("bad wizard cloning?");
        break;
    case MGC_SUMMON_MONS: {
        int count = nasty(mtmp, FALSE);

        if (!count) {
            ; /* nothing was created? */
        } else if (mtmp->iswiz) {
            verbalize("Destroy the thief, my pet%s!", plur(count));
        } else {
            boolean one = (count == 1);
            const char *mappear = one ? "A monster appears"
                                      : "Monsters appear";

            /* messages not quite right if plural monsters created but
               only a single monster is seen */
            if (Invis && !mon_prop(mtmp, SEE_INVIS)
                && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                pline("%s %s a spot near you!", mappear,
                      one ? "at" : "around");
            else if (Displaced && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                pline("%s %s your displaced image!", mappear,
                      one ? "by" : "around");
            else
                pline("%s from nowhere!", mappear);
        }
        dmg = 0;
        break;
    }
    case MGC_CALL_UNDEAD:
        mm.x = u.ux;   
        mm.y = u.uy;   
        pline("Undead creatures are called forth from the grave!");   
        mkundead(&mm, FALSE, NO_MINVENT);   
        dmg = 0;   
        break;   
    case MGC_AGGRAVATION:
        You_feel("that monsters are aware of your presence.");
        aggravate();
        dmg = 0;
        break;
    case MGC_CURSE_ITEMS:
        You_feel("as if you need some help.");
        rndcurse();
        dmg = 0;
        break;
    case MGC_DESTRY_ARMR:
        dmg = m_destroy_armor(mtmp, &youmonst);
        break;
    case MGC_WEAKEN_YOU: /* drain strength */
        if (Antimagic) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_MAGR);
            You_feel("momentarily weakened.");
        } else {
            You("suddenly feel weaker!");
            dmg = ml - 6;
            if (Half_spell_damage)
                dmg = (dmg + 1) / 2;
            losestr(rnd(dmg));
            if (u.uhp < 1)
                done_in_by(mtmp, DIED);
        }
        dmg = 0;
        break;
    case MGC_DISAPPEAR: /* makes self invisible */
        if (!mtmp->minvis && !mtmp->invis_blkd) {
            if (canseemon(mtmp))
                pline("%s suddenly %s!", Monnam(mtmp),
                      !See_invisible ? "disappears" : "becomes transparent");
            mon_set_minvis(mtmp);
            if (cansee(mtmp->mx, mtmp->my) && !canspotmon(mtmp))
                map_invisible(mtmp->mx, mtmp->my);
            dmg = 0;
        } else
            impossible("no reason for monster to cast disappear spell?");
        break;
    case MGC_STUN_YOU:
        if (Antimagic || Free_action || Hidinshell) {
            shieldeff(u.ux, u.uy);
            if (!(Stunned || Stun_resistance))
                You_feel("momentarily disoriented.");
            make_stunned(1L, FALSE);
        } else {
            if (!(Stun_resistance))
                You(Stunned ? "struggle to keep your balance." : "reel...");
            dmg = d(ACURR(A_DEX) < 12 ? 6 : 4, 4);
            if (Half_spell_damage)
                dmg = (dmg + 1) / 2;
            make_stunned((HStun & TIMEOUT) + (long) dmg, FALSE);
        }
        dmg = 0;
        break;
    case MGC_HASTE_SELF:
        mon_adjust_speed(mtmp, 1, (struct obj *) 0);
        dmg = 0;
        break;
    case MGC_CURE_SELF:
        dmg = m_cure_self(mtmp, dmg);
        break;
    case MGC_FIRE_BOLT:
    case MGC_ICE_BOLT:
        /* hotwire these to only go off if the critter can see you
         * to avoid bugs WRT the Eyes and detect monsters */
        if (m_canseeu(mtmp) && distu(mtmp->mx, mtmp->my) <= mcastrange(ml)) {
            pline("%s blasts you with %s!", Monnam(mtmp),
                  (spellnum == MGC_FIRE_BOLT) ? "fire" : "ice");
            explode(u.ux, u.uy, (spellnum == MGC_FIRE_BOLT) ? ZT_FIRE 
                                                            : ZT_COLD,
                    resist_reduce(d((ml / 5) + 1, 8),
                    (spellnum == MGC_FIRE_BOLT) ? FIRE_RES : COLD_RES),
                    MON_CASTBALL, (spellnum == MGC_FIRE_BOLT) ? EXPL_FIERY
                                                              : EXPL_FROSTY);
            if (how_resistant((spellnum == MGC_FIRE_BOLT) ? FIRE_RES
                                                          : COLD_RES) == 100) {
                shieldeff(u.ux, u.uy);
                monstseesu((spellnum == MGC_FIRE_BOLT) ? M_SEEN_FIRE
                                                       : M_SEEN_COLD);
                pline("But you resist the effects.");
            }
        } else {
            if (canseemon(mtmp)) {
                pline("%s blasts the %s with %s and curses!",
                      Monnam(mtmp), rn2(2) ? "ceiling" : "floor",
                      (spellnum == MGC_FIRE_BOLT) ? "fire" : "ice");
            } else {
                You_hear("some cursing!");
            }
        }
        /* damage is handled by explode() */
        dmg = 0;
        break;
    case MGC_PSI_BOLT:
        /* prior to 3.4.0 Antimagic was setting the damage to 1--this
           made the spell virtually harmless to players with magic res. */
        if (Antimagic) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_MAGR);
            dmg = (dmg + 1) / 2;
        }
        if (BTelepat) {
            pline_The("spell is blocked%s", dmg > 5 ? "!" : ".");
            return;
        }
        if (dmg <= 5)
            You("get a slight %sache.", body_part(HEAD));
        else if (dmg <= 10)
            Your("brain is on fire!");
        else if (dmg <= 20)
            Your("%s suddenly aches painfully!", body_part(HEAD));
        else
            Your("%s suddenly aches very painfully!", body_part(HEAD));
        break;
    default:
        impossible("mcastu: invalid magic spell (%d)", spellnum);
        dmg = 0;
        break;
    }

    if (dmg)
        mdamageu(mtmp, dmg);
}

const char* vulntext[] = {
        "chartreuse polka-dot",
        "reddish-orange",
        "purplish-blue",
        "coppery-yellow",
        "greenish-mottled",
        "shimmering-stippled"
};

STATIC_OVL
void
cast_cleric_spell(mtmp, dmg, spellnum)
struct monst *mtmp;
int dmg;
int spellnum;
{
    int aligntype, ml = min(mtmp->m_lev, 50);
    static const char *Moloch = "Moloch";
    struct monst *minion = (struct monst *) 0;

    if (dmg == 0 && !is_undirected_spell(AD_CLRC, spellnum)) {
        impossible("cast directed cleric spell (%d) with dmg=0?", spellnum);
        return;
    }

    switch (spellnum) {
    case CLC_SUMMON_ELM:
        aligntype = mon_aligntyp(mtmp);
        minion = summon_minion(aligntype, FALSE);
        if (minion) {
            boolean vassal = (aligntype == A_NONE);
            set_malign(minion);
            if (canspotmon(minion))
                pline("A %s of %s appears!",
                      vassal ? "vassal" : "servant",
                      vassal ? Moloch : aligns[1 - aligntype].noun);
        }
    
        dmg = 0;
        break;
    case CLC_GEYSER:
        /* this is physical damage (force not heat),
         * not magical damage or fire damage
         */
        if (mtmp->data == &mons[PM_ASMODEUS] || mtmp->data == &mons[PM_VECNA]) {
            pline("An avalanche of ice and snow slams into you from nowhere!");
            dmg = d(8, 8);
            if (Half_physical_damage)
                dmg = (dmg + 1) / 2;
            destroy_item(POTION_CLASS, AD_COLD);
        } else {
            pline("A sudden geyser slams into you from nowhere!");
            dmg = d(8, 6);
            if (Half_physical_damage)
                dmg = (dmg + 1) / 2;
            if (u.umonnum == PM_IRON_GOLEM) {
                You("rust!");
                Strcpy(killer.name, "rusted away");
                killer.format = NO_KILLER_PREFIX;
                rehumanize();
                dmg = 0; /* prevent further damage after rehumanization */
                break;
            }
            (void) erode_armor(&youmonst, ERODE_RUST);
        }
        break;
    case CLC_FIRE_PILLAR:
        pline("A pillar of fire strikes all around you!");
        if (how_resistant(FIRE_RES) == 100) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_FIRE);
            dmg = 0;
        } else
            dmg = resist_reduce(d(8, 6), FIRE_RES);
        if (Half_spell_damage)
            dmg = (dmg + 1) / 2;
        burn_away_slime();
        (void) burnarmor(&youmonst);
        destroy_item(SCROLL_CLASS, AD_FIRE);
        destroy_item(POTION_CLASS, AD_FIRE);
        destroy_item(SPBOOK_CLASS, AD_FIRE);
        destroy_item(WEAPON_CLASS, AD_FIRE);
        (void) burn_floor_objects(u.ux, u.uy, TRUE, FALSE);
        break;
    case CLC_LIGHTNING: {
        boolean reflects;

        pline("A bolt of lightning strikes down at you from above!");
        reflects = ureflects("Some of it bounces off your %s%s.", "");
        if (reflects || (how_resistant(SHOCK_RES) == 100)) {
            shieldeff(u.ux, u.uy);
            if (reflects) {
                dmg = resist_reduce(d(4, 6), SHOCK_RES);
                monstseesu(M_SEEN_REFL);
            }
            if (how_resistant(SHOCK_RES) == 100) {
                You("aren't shocked.");
                monstseesu(M_SEEN_ELEC);
                dmg = 0;
            }
        } else {
            dmg = resist_reduce(d(8, 6), SHOCK_RES);
        }
        if (dmg && Half_spell_damage)
            dmg = (dmg + 1) / 2;
        if (!reflects) {
            destroy_item(WAND_CLASS, AD_ELEC);
            destroy_item(RING_CLASS, AD_ELEC);
            (void) flashburn((long) rnd(100));
        }
        break;
    }
    case CLC_CURSE_ITEMS:
        You_feel("as if you need some help.");
        rndcurse();
        dmg = 0;
        break;
    case CLC_INSECTS: {
        /* Try for bugs, and if there are none
           left, go for (sticks to) snakes.  -3. */
        boolean spiders = mtmp->data == &mons[PM_LOLTH];
        boolean spheres = mtmp->data == &mons[PM_BEHOLDER];
        struct permonst *pm = mkclass(spiders ? S_SPIDER
                                      : spheres ? S_EYE :S_ANT, 0);
        struct monst *mtmp2 = (struct monst *) 0;
        char let = (pm ? (spiders ? S_SPIDER
                          : spheres ? S_EYE: S_ANT) : S_SNAKE);
        boolean success = FALSE, seecaster;
        int i, quan, oldseen, newseen;
        coord bypos;
        const char *fmt;

        oldseen = monster_census(TRUE);
        quan = (ml < 2) ? 1 : rnd((int) ml / 2);
        if (quan < 3)
            quan = 3;
        for (i = 0; i <= quan; i++) {
            if (!enexto(&bypos, mtmp->mux, mtmp->muy, mtmp->data))
                break;
            if ((pm = mkclass(let, 0)) != 0
                && (mtmp2 = makemon(pm, bypos.x, bypos.y, MM_ANGRY)) != 0) {
                success = TRUE;
                mtmp2->msleeping = mtmp2->mpeaceful = mtmp2->mtame = 0;
                set_malign(mtmp2);
            }
        }
        newseen = monster_census(TRUE);

        /* not canspotmon(), which includes unseen things sensed via warning
         */
        seecaster = canseemon(mtmp) || tp_sensemon(mtmp) || Detect_monsters;

        fmt = 0;
        if (!seecaster) {
            char *arg; /* [not const: upstart(N==1 ? an() : makeplural())] */
            const char *what = (let == S_SNAKE) ? "snake"
                                                : (let == S_SPIDER) ? "arachnid"
                                                                    : "insect";

            if (newseen <= oldseen || Unaware) {
                /* unseen caster fails or summons unseen critters,
                   or unconscious hero ("You dream that you hear...") */
                You_hear("someone summoning %s.", makeplural(what));
            } else {
                /* unseen caster summoned seen critter(s) */
                arg = (newseen == oldseen + 1) ? an(what) : makeplural(what);
                if (!Deaf)
                    You_hear("someone summoning something, and %s %s.", arg,
                             vtense(arg, "appear"));
                else
                    pline("%s %s.", upstart(arg), vtense(arg, "appear"));
            }

        /* seen caster, possibly producing unseen--or just one--critters;
           hero is told what the caster is doing and doesn't necessarily
           observe complete accuracy of that caster's results (in other
           words, no need to fuss with visibility or singularization;
           player is told what's happening even if hero is unconscious) */
        } else if (!success)
            fmt = "%s casts at a clump of sticks, but nothing happens.";
        else if (let == S_SNAKE)
            fmt = "%s transforms a clump of sticks into snakes!";
        else if (let == S_SPIDER)
            fmt = "%s summons arachnids!";
        else if (let == S_EYE)
            fmt = "%s summons orbs!";
        else if (Invis && !mon_prop(mtmp, SEE_INVIS)
                 && (mtmp->mux != u.ux || mtmp->muy != u.uy))
            fmt = "%s summons insects around a spot near you!";
        else if (Displaced && (mtmp->mux != u.ux || mtmp->muy != u.uy))
            fmt = "%s summons insects around your displaced image!";
        else
            fmt = "%s summons insects!";
        if (fmt)
            pline(fmt, Monnam(mtmp));

        dmg = 0;
        break;
    }
    case CLC_BLIND_YOU:
        /* note: resists_blnd() doesn't apply here */
        if (!Blinded) {
            int num_eyes = eyecount(youmonst.data);
            pline("Scales cover your %s!", (num_eyes == 1)
                                               ? body_part(EYE)
                                               : makeplural(body_part(EYE)));
            make_blinded(Half_spell_damage ? 100L : 200L, FALSE);
            if (!Blind)
                Your1(vision_clears);
            dmg = 0;
        } else
            impossible("no reason for monster to cast blindness spell?");
        break;
    case CLC_PARALYZE:
        if (Antimagic || Free_action) {
            shieldeff(u.ux, u.uy);
            if (multi >= 0)
                You("stiffen briefly.");
            nomul(-1);
            multi_reason = "paralyzed by a monster";
        } else {
            if (multi >= 0)
                You("are frozen in place!");
            dmg = 4 + (int) ml;
            if (Half_spell_damage)
                dmg = (dmg + 1) / 2;
            nomul(-dmg);
            multi_reason = "paralyzed by a monster";
        }
        nomovemsg = 0;
        dmg = 0;
        break;
    case CLC_CONFUSE_YOU:
        if (Antimagic) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_MAGR);
            You_feel("momentarily dizzy.");
        } else {
            boolean oldprop = !!Confusion;

            dmg = (int) ml;
            if (Half_spell_damage)
                dmg = (dmg + 1) / 2;
            make_confused(HConfusion + dmg, TRUE);
            if (Hallucination)
                You_feel("%s!", oldprop ? "trippier" : "trippy");
            else
                You_feel("%sconfused!", oldprop ? "more " : "");
        }
        dmg = 0;
        break;
    case CLC_CURE_SELF:
        dmg = m_cure_self(mtmp, dmg);
        break;
    case CLC_VULN_YOU:
        dmg = rnd(5);
        pline("A %s film oozes over your %s!",
              Blind ? "slimy" : vulntext[dmg], body_part(SKIN));
        switch (dmg) {
        case 1:
            if (mtmp->data == &mons[PM_ASMODEUS] || mtmp->data == &mons[PM_VECNA]) {
                if (Vulnerable_cold)
                    return;
                incr_itimeout(&HVulnerable_cold, rnd(100) + 150);
                You_feel("extremely chilly.");
                break;
            } else {
                if (Vulnerable_fire)
                    return;
                incr_itimeout(&HVulnerable_fire, rnd(100) + 150);
                You_feel("more inflammable.");
                break;
            }
            break;
        case 2:
            if (Vulnerable_cold)
                return;
            incr_itimeout(&HVulnerable_cold, rnd(100) + 150);
            You_feel("extremely chilly.");
            break;
        case 3:
            if (Vulnerable_elec)
                return;
            incr_itimeout(&HVulnerable_elec, rnd(100) + 150);
            You_feel("overly conductive.");
            break;
        case 4:
            if (Vulnerable_acid)
                return;
            incr_itimeout(&HVulnerable_acid, rnd(100) + 150);
            You_feel("easily corrodable.");
            break;
        case 5:
            if (Vulnerable_loud)
                return;
            incr_itimeout(&HVulnerable_loud, rnd(100) + 150);
            You_feel("hyperacute.");
            break;
        default:
            break;
        }
        break;
    case CLC_OPEN_WOUNDS:
        if (Antimagic) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_MAGR);
            dmg = (dmg + 1) / 2;
        }
        if (dmg <= 5)
            Your("skin itches badly for a moment.");
        else if (dmg <= 10)
            pline("Wounds appear on your body!");
        else if (dmg <= 20)
            pline("Severe wounds appear on your body!");
        else
            Your("body is covered with painful wounds!");
        break;
    case CLC_PROTECTION: {
        int natac = find_mac(mtmp) + mtmp->mprotection;
        int loglev = 0;
        int gain = 0;

        dmg = 0;

        for (; ml > 0; ml /= 2)
            loglev++;

        gain = loglev - mtmp->mprotection / (4 - min(3, (10 - natac) / 10));

        if (mtmp->mpeaceful
            && mtmp->ispriest && inhistemple(mtmp)) {
            ; /* cut down on the temple spam */
        } else {
            if (gain && canseemon(mtmp)) {
                if (mtmp->mprotection) {
                    pline_The("%s haze around %s becomes more dense.",
                              hcolor(NH_GOLDEN), mon_nam(mtmp));
                } else {
                    mtmp->mprottime = (mtmp->iswiz || is_prince(mtmp->data)
                                       || mtmp->data->msound == MS_NEMESIS
                                       || mtmp->data->msound == MS_LEADER)
                                       ? 20 : 10;
                    pline_The("air around %s begins to shimmer with a %s haze.",
                              mon_nam(mtmp), hcolor(NH_GOLDEN));
                }
            }
        }
        mtmp->mprotection += gain;
        break;
    }
    default:
        impossible("mcastu: invalid clerical spell (%d)", spellnum);
        dmg = 0;
        break;
    }

    if (dmg)
        mdamageu(mtmp, dmg);
}

STATIC_DCL
boolean
is_undirected_spell(adtyp, spellnum)
unsigned int adtyp;
int spellnum;
{
    if (adtyp == AD_SPEL) {
        switch (spellnum) {
        case MGC_CLONE_WIZ:
        case MGC_SUMMON_MONS:
        case MGC_ACID_BLAST:
        case MGC_AGGRAVATION:
        case MGC_DISAPPEAR:
        case MGC_HASTE_SELF:
        case MGC_CURE_SELF:
        case MGC_CALL_UNDEAD:
        case MGC_FIRE_BOLT:
        case MGC_ICE_BOLT:
        case MGC_CANCELLATION:
        case MGC_REFLECTION:
            return TRUE;
        default:
            break;
        }
    } else if (adtyp == AD_CLRC) {
        switch (spellnum) {
        case CLC_INSECTS:
        case CLC_CURE_SELF:
        case CLC_PROTECTION:
        case CLC_FIRE_PILLAR:
            return TRUE;
        default:
            break;
        }
    }
    return FALSE;
}

STATIC_DCL
boolean
mspell_would_be_useless(mtmp, mdef, adtyp, spellnum)
struct monst *mtmp;
struct monst *mdef;
unsigned int adtyp;
int spellnum;
{
    if (adtyp == AD_SPEL) {
        /* aggravate monsters, etc. won't be cast by peaceful monsters */
        if (mtmp->mpeaceful
            && (spellnum == MGC_AGGRAVATION 
             || spellnum == MGC_SUMMON_MONS
             || spellnum == MGC_CLONE_WIZ 
             || spellnum == MGC_CALL_UNDEAD))
            return TRUE;
        /* haste self when already fast */
        if (mtmp->permspeed == MFAST && spellnum == MGC_HASTE_SELF)
            return TRUE;
        /* invisibility when already invisible */
        if ((mtmp->minvis || mtmp->invis_blkd) && spellnum == MGC_DISAPPEAR)
            return TRUE;
        /* reflection when already reflecting */
        if ((has_reflection(mtmp) || mon_reflects(mtmp, (char *) 0))
            && spellnum == MGC_REFLECTION)
            return TRUE;
        if (mtmp->mpeaceful && !See_invisible && spellnum == MGC_DISAPPEAR)
            return TRUE;
        /* healing when already healed */
        if (mtmp->mhp == mtmp->mhpmax && spellnum == MGC_CURE_SELF)
            return TRUE;
        /* don't summon monsters if it doesn't think you're around */
        if ((!mtmp->iswiz || context.no_of_wizards > 1)
            && (spellnum == MGC_CLONE_WIZ || spellnum == MGC_CALL_UNDEAD))
            return TRUE;
        /* only lichs and necromancers can cast call undead */
        if (spellnum == MGC_CALL_UNDEAD
            && mtmp->data->mlet != S_LICH && mtmp->data != &mons[PM_NECROMANCER])
            return TRUE;
        /* pools can only be created in certain locations and then only
        * rarely unless you're carrying the amulet.
        * Monsters won't cast at you if you are flying or levitating.
        */
        if (spellnum == MGC_CREATE_POOL
             && ((levl[u.ux][u.uy].typ != ROOM && levl[u.ux][u.uy].typ != CORR)
            || Flying
            || Levitation
            || (!u.uhave.amulet && rn2(20))))
            return TRUE;
        /* Don't try to destroy armor if none is being worn */
        if (!(mdef->misc_worn_check & W_ARMOR)
            && spellnum == MGC_DESTRY_ARMR) {
            return TRUE;
        }
        /* Don't waste time zapping resisted spells at the player,
         * and don't blast ourselves with our own explosions */
        if ((m_seenres(mtmp, M_SEEN_FIRE)
             || (distu(mtmp->mx, mtmp->my) < 3 && rn2(5)))
            && spellnum == MGC_FIRE_BOLT) {
            return TRUE;
        }
        if ((m_seenres(mtmp, M_SEEN_COLD)
             || (distu(mtmp->mx, mtmp->my) < 3 && rn2(5)))
            && spellnum == MGC_ICE_BOLT) {
            return TRUE;
        }
        if ((m_seenres(mtmp, M_SEEN_ACID)
             || (distu(mtmp->mx, mtmp->my) < 3 && rn2(5)))
            && spellnum == MGC_ACID_BLAST) {
            return TRUE;
        }
        if ((spellnum == MGC_ICE_BOLT 
            || spellnum == MGC_FIRE_BOLT
            || spellnum == MGC_ACID_BLAST 
            || spellnum == MGC_CANCELLATION)
            && mtmp->mpeaceful) {
            return TRUE;
        }
     } else if (adtyp == AD_CLRC) {
        /* healing when already healed */
        if (mtmp->mhp == mtmp->mhpmax && spellnum == CLC_CURE_SELF)
            return TRUE;
        /* blindness spell on blinded player */
        if ((!haseyes(mdef->data) || mdef->mblinded)
            && spellnum == CLC_BLIND_YOU)
            return TRUE;
        
        if (spellnum == CLC_FIRE_PILLAR && mtmp->mpeaceful)
            return TRUE;
        if (m_seenres(mtmp, M_SEEN_FIRE) && spellnum == CLC_FIRE_PILLAR)
            return TRUE;
        /* Don't let arch-viles spam too much */
        if (spellnum == CLC_FIRE_PILLAR 
            && mtmp->data == &mons[PM_ARCH_VILE] && rn2(30)) {
            return TRUE;
        }
    }
    return FALSE;
}

STATIC_DCL
boolean
uspell_would_be_useless(adtyp, spellnum)
unsigned int adtyp;
int spellnum;
{
    if (adtyp == AD_SPEL) {
        /* aggravate monsters, etc. won't be cast by peaceful monsters */
        if (spellnum == MGC_CLONE_WIZ)
            return TRUE;
        /* haste self when already fast */
        if (Fast && spellnum == MGC_HASTE_SELF)
            return TRUE;
        /* invisibility when already invisible */
        if ((HInvis & INTRINSIC) && spellnum == MGC_DISAPPEAR)
            return TRUE;
        /* healing when already healed */
        if (u.mh == u.mhmax && spellnum == MGC_CURE_SELF)
            return TRUE;
        /* Call Undead summons hostile undead - no good to us */
        if (spellnum == MGC_CALL_UNDEAD)
            return TRUE;
    } else if (adtyp == AD_CLRC) {
        /* healing when already healed */
        if (u.mh == u.mhmax && spellnum == CLC_CURE_SELF)
            return TRUE;
    }
    return FALSE;
}

/* Some spells are useless under some circumstances. */
STATIC_DCL
boolean
spell_would_be_useless(mtmp, adtyp, spellnum)
struct monst *mtmp;
unsigned int adtyp;
int spellnum;
{
    /* Some spells don't require the player to really be there and can be cast
     * by the monster when you're invisible, yet still shouldn't be cast when
     * the monster doesn't even think you're there.
     * This check isn't quite right because it always uses your real position.
     * We really want something like "if the monster could see mux, muy".
     */
    boolean mcouldseeu = couldsee(mtmp->mx, mtmp->my);

    if (adtyp == AD_SPEL) {
        /* aggravate monsters, etc. won't be cast by peaceful monsters */
        if (mtmp->mpeaceful
            && (spellnum == MGC_AGGRAVATION 
                || spellnum == MGC_SUMMON_MONS
                || spellnum == MGC_CLONE_WIZ 
                || spellnum == MGC_CALL_UNDEAD))
            return TRUE;
        /* haste self when already fast */
        if (mtmp->permspeed == MFAST && spellnum == MGC_HASTE_SELF)
            return TRUE;
        /* invisibility when already invisible */
        if ((mtmp->minvis || mtmp->invis_blkd) && spellnum == MGC_DISAPPEAR)
            return TRUE;
        if ((has_reflection(mtmp) || mon_reflects(mtmp, (char *) 0))
            && spellnum == MGC_REFLECTION)
            return TRUE;
        /* peaceful monster won't cast invisibility if you can't see
           invisible,
           same as when monsters drink potions of invisibility.  This doesn't
           really make a lot of sense, but lets the player avoid hitting
           peaceful monsters by mistake */
        if (mtmp->mpeaceful && !See_invisible && spellnum == MGC_DISAPPEAR)
            return TRUE;
        /* healing when already healed */
        if (mtmp->mhp == mtmp->mhpmax && spellnum == MGC_CURE_SELF)
            return TRUE;
        /* don't summon monsters if it doesn't think you're around */
        if (!mcouldseeu 
            && ((spellnum == MGC_CLONE_WIZ 
                 || spellnum == MGC_SUMMON_MONS
                 || spellnum == MGC_CALL_UNDEAD)
            || (!mtmp->iswiz && spellnum == MGC_CLONE_WIZ)))
            return TRUE;
        /* only lichs and necromancers can cast call undead */
        if (spellnum == MGC_CALL_UNDEAD 
            && mtmp->data->mlet != S_LICH && mtmp->data != &mons[PM_NECROMANCER])
            return TRUE;
        if ((!mtmp->iswiz || context.no_of_wizards > 1)
            && spellnum == MGC_CLONE_WIZ)
            return TRUE;
        /* aggravation (global wakeup) when everyone is already active */
        if (spellnum == MGC_AGGRAVATION) {
            /* if nothing needs to be awakened then this spell is useless
               but caster might not realize that [chance to pick it then
               must be very small otherwise caller's many retry attempts
               will eventually end up picking it too often] */
            if (!has_aggravatables(mtmp))
                return rn2(100) ? TRUE : FALSE;
        }
        /* Don't try to destroy armor if none is being worn */
        if (!wearing_armor() && spellnum == MGC_DESTRY_ARMR) {
            return TRUE;
        }
        if ((m_seenres(mtmp, M_SEEN_FIRE)
             || (distu(mtmp->mx, mtmp->my) < 3 && rn2(5)))
            && spellnum == MGC_FIRE_BOLT) {
            return TRUE;
        }
        if ((m_seenres(mtmp, M_SEEN_COLD)
             || (distu(mtmp->mx, mtmp->my) < 3 && rn2(5)))
            && spellnum == MGC_ICE_BOLT) {
            return TRUE;
        }
        if ((m_seenres(mtmp, M_SEEN_ACID)
             || (distu(mtmp->mx, mtmp->my) < 3 && rn2(5)))
            && spellnum == MGC_ACID_BLAST) {
            return TRUE;
        }
        if ((spellnum == MGC_ICE_BOLT 
             || spellnum == MGC_FIRE_BOLT
             || spellnum == MGC_ACID_BLAST 
             || spellnum == MGC_CANCELLATION) 
            && (mtmp->mpeaceful || u.uinvulnerable)) {
            return TRUE;
        }
    } else if (adtyp == AD_CLRC) {
        /* summon insects/sticks to snakes won't be cast by peaceful monsters
         */
        if (mtmp->mpeaceful && spellnum == CLC_INSECTS)
            return TRUE;
        /* healing when already healed */
        if (mtmp->mhp == mtmp->mhpmax && spellnum == CLC_CURE_SELF)
            return TRUE;
        /* don't summon insects if it doesn't think you're around */
        if (!mcouldseeu && spellnum == CLC_INSECTS)
            return TRUE;
        /* blindness spell on blinded player */
        if (Blinded && spellnum == CLC_BLIND_YOU)
            return TRUE;

        if (m_seenres(mtmp, M_SEEN_FIRE) && spellnum == CLC_FIRE_PILLAR) {
            return TRUE;
        }
        if (spellnum == CLC_FIRE_PILLAR && (mtmp->mpeaceful || u.uinvulnerable))
            return TRUE;
        /* Don't let arch-viles spam too much */
        if (spellnum == CLC_FIRE_PILLAR
            && mtmp->data == &mons[PM_ARCH_VILE] && rn2(30)) {
            return TRUE;
        }
    }
    return FALSE;
}


/* monster uses spell against player (ranged) */
int
buzzmu(mtmp, mattk)
register struct monst *mtmp;
register struct attack *mattk;
{
    /* don't print constant stream of curse messages for 'normal'
       spellcasting monsters at range */
    if (mattk->adtyp >= AD_PSYC)
        return 0;

    if (mtmp->mcan) {
        cursetxt(mtmp, FALSE);
        return 0;
    }
    if (lined_up(mtmp) && !rn2(7)) {
        nomul(0);
        if (mattk->adtyp && (mattk->adtyp <= MAX_ZT)) { /* no cf unsigned > 0 */
            if (canseemon(mtmp))
                pline("%s zaps you with a %s!", Monnam(mtmp),
                      flash_types[AD_to_ZT(mattk->adtyp)]);
            buzz(ZT_MONSPELL(AD_to_ZT(mattk->adtyp)), (int) mattk->damn, mtmp->mx,
                 mtmp->my, sgn(tbx), sgn(tby));
        } else
            impossible("Monster spell %d cast", mattk->adtyp - 1);
    }
    return 1;
}

/* monster uses spell against monster (ranged) */
int
buzzmm(mtmp, mdef, mattk)
register struct monst *mtmp;
register struct monst *mdef;
register struct attack *mattk;
{
    /* don't print constant stream of curse messages for 'normal'
       spellcasting monsters at range */
    if (mattk->adtyp > AD_PSYC)
        return 0;

    if (mtmp->mcan) {
        cursetxt(mtmp, FALSE);
        return 0;
    }
    if (mlined_up(mtmp, mdef, FALSE) && rn2(3)) {
        nomul(0);
        if (mattk->adtyp && (mattk->adtyp <= MAX_ZT)) { /* no cf unsigned > 0 */
            if (canseemon(mtmp))
                pline("%s zaps %s with a %s!", Monnam(mtmp),
                      mon_nam(mdef), flash_types[AD_to_ZT(mattk->adtyp)]);
            dobuzz(ZT_MONSPELL(AD_to_ZT(mattk->adtyp)), (int) mattk->damn, mtmp->mx,
                   mtmp->my, sgn(tbx), sgn(tby), FALSE);
        } else
            impossible("Monster spell %d cast", mattk->adtyp - 1);
    }
    return 1;
}

int
castmm(magr, mdef, mattk)
struct monst *magr;
struct monst *mdef;
struct attack *mattk;
{
    int dmg, ml = min(magr->m_lev, 50);
    int ret;
    int spellnum = 0;

    /* guard against casting another spell attack
       at an already dead monster; some monsters
       have multiple AT_MAGC attacks */
    if (mdef->mhp <= 0)
        return 0;

    /* Currently the droid's ranged attack - not melee. */
    if (mattk->adtyp == AD_ELEC)
        return 0; 
    
    if ((mattk->adtyp == AD_SPEL || mattk->adtyp == AD_CLRC) && ml) {
        int cnt = 40;

        do {
            spellnum = rn2(ml);
            if (mattk->adtyp == AD_SPEL)
                spellnum = choose_magic_spell(magr, spellnum);
            else
                spellnum = choose_clerical_spell(magr, spellnum);
        /* not trying to attack?  don't allow directed spells */
        } while (--cnt > 0
                 && mspell_would_be_useless(magr, mdef, mattk->adtyp, spellnum));
        if (cnt == 0)
            return 0;
    }

    /* monster unable to cast spells? */
    if (magr->mcan || magr->mspec_used || !ml) {
        if (canseemon(magr)) {
            if (is_undirected_spell(mattk->adtyp, spellnum))
                pline("%s points all around, then curses.",
                      Monnam(magr));
            else
                pline("%s points at %s, then curses.",
                      Monnam(magr), mon_nam(mdef));
        } else if ((!(moves % 4) || !rn2(4))) {
            if (!Deaf)
                Norep("You hear a mumbled curse.");
        }
        return 0;
    }

    if (mattk->adtyp == AD_SPEL || mattk->adtyp == AD_CLRC) {
        struct obj *obj;
        magr->mspec_used = 4 - magr->m_lev;
        if (magr->mspec_used < 2)
            magr->mspec_used = 2;
        /* many boss-type monsters than have two or more spell attacks
           per turn are never able to fire off their second attack due
           to mspec always being greater than 0. So set to 0 for those
           types of monsters, either sometimes or all of the time
           depending on how powerful they are or what their role is */
        if (rn2(3) /* mspec 0 two thirds of the time */
            && (is_dlord(magr->data)
                || magr->data->msound == MS_LEADER
                || magr->data->msound == MS_NEMESIS
                || magr->data == &mons[PM_ORACLE]
                || magr->data == &mons[PM_HIGH_PRIEST]))
            magr->mspec_used = 0;

        if (is_dprince(magr->data) || magr->iswiz || magr->isvecna)
            /* mspec 0 always */
            magr->mspec_used = 0;

        /* Having the EotA in inventory drops mspec to 0 */
        for (obj = magr->minvent; obj; obj = obj->nobj) {
            if (obj->oartifact
                && obj->oartifact == ART_EYE_OF_THE_AETHIOPICA) {
                mdef->mspec_used = 0;
                break;
            }
        }
    }

    if (rn2(ml * 10) < (mdef->mconf ? 100 : 20)) {	/* fumbled attack */
        if (canseemon(mdef) && !Deaf)
            pline_The("air crackles around %s.", mon_nam(mdef));
        return 0;
    }

    if (canseemon(mdef)
        && !is_undirected_spell(mattk->adtyp, spellnum))
        pline("%s casts a spell at %s!", Monnam(magr), mon_nam(mdef));

    if (canseemon(magr)
        && is_undirected_spell(mattk->adtyp, spellnum))
        pline("%s casts a spell!", Monnam(magr));

    if (mattk->damd)
        dmg = d((int) ((ml / 2) + mattk->damn), (int) mattk->damd);
    else
        dmg = d((int) ((ml / 2) + 1), 6);

    ret = 1;

    switch (mattk->adtyp) {
    case AD_FIRE:
        if (canseemon(mdef)) {
            if (is_demon(magr->data))
                pline("%s is enveloped in hellfire!", Monnam(mdef));
            else if (!mon_underwater(mdef))
                pline("%s is enveloped in flames.", Monnam(mdef));
            else {
                pline("The flames are quenched by the water around %s.",
                      mon_nam(mdef));
                dmg = 0;
                break;
            }
        }
        
        if (resists_fire(mdef) || defended(mdef, AD_FIRE)) {
            shieldeff(mdef->mx, mdef->my);
            
            if (is_demon(magr->data)) {
                if (!(nonliving(mdef->data) || is_demon(mdef->data))) {
                    if (canseemon(mdef))
                        pline_The("hellish flames sear %s soul!",
                                  s_suffix(mon_nam(mdef)));
                    dmg = (dmg + 1) / 2;
                } else {
                    dmg = 0;
                }
            } else {
                if (canseemon(mdef))
                    pline("But %s resists the effects.", mhe(mdef));
                dmg = 0;
            }
        }
        break;
    case AD_COLD:
        if (canseemon(mdef))
            pline("%s is covered in frost.", Monnam(mdef));
        if (resists_cold(mdef) || defended(mdef, AD_COLD)) {
            shieldeff(mdef->mx, mdef->my);
            if (canseemon(mdef))
                pline("But %s resists the effects.", mhe(mdef));
            dmg = 0;
        }
        break;
    case AD_WIND:
        You("are blasted by wind!");
        hurtle(u.ux - magr->mx, u.uy - magr->my, dmg, TRUE);
        dmg = 0;
        break;
    case AD_ACID:
        if (canseemon(mdef)) {
            if (!mon_underwater(mdef))
                pline("%s is covered in acid.", Monnam(mdef));
            else {
                pline("The acid dissipates harmlessly in the water around %s.",
                      mon_nam(mdef));
                dmg = 0;
                break;
            }
        }
        if (resists_acid(mdef) || defended(mdef, AD_ACID)) {
            shieldeff(mdef->mx, mdef->my);
            if (canseemon(mdef))
                pline("But %s resists the effects.", mhe(mdef));
            dmg = 0;
        }
        break;
    case AD_MAGM:
        if (canseemon(mdef))
            pline("%s is hit by a shower of missiles!", Monnam(mdef));
        dmg = d((int) ml / 2 + 1, 6);
        if (resists_magm(mdef) || defended(mdef, AD_MAGM)) {
            shieldeff(mdef->mx, mdef->my);
            if (canseemon(mdef))
                pline("Some missiles bounce off!");
            dmg = (dmg + 1) / 2;
        }
        break;
    case AD_SPEL:   /* wizard spell */
    case AD_CLRC: { /* clerical spell */
        /* aggravation is a special case;
         * it's undirected but should still target the
         * victim so as to aggravate you */
        if (is_undirected_spell(mattk->adtyp, spellnum)
            /* 'undirected-but-not-really' spells: */
            && (mattk->adtyp == AD_SPEL
                /* magic spells */
                ? (spellnum != MGC_AGGRAVATION
                    && spellnum != MGC_SUMMON_MONS)
                /* clerical spells */
                : (spellnum != CLC_INSECTS))) {
            if (mattk->adtyp == AD_SPEL)
                cast_wizard_spell(magr, dmg, spellnum);
            else
               cast_cleric_spell(magr, dmg, spellnum);
        } else if (mattk->adtyp == AD_SPEL) {
            ucast_wizard_spell(magr, mdef, dmg, spellnum);
        } else {
            ucast_cleric_spell(magr, mdef, dmg, spellnum);
        }
        dmg = 0; /* done by the spell casting functions */
        break;
        }
    }

    if (dmg) {
        if (damage_mon(mdef, dmg, mattk->adtyp))
            monkilled(mdef, "", mattk->adtyp);
    }

    return (ret);
}

int
castum(mdef, mattk)
struct monst *mdef;
struct attack *mattk;
{
    int dmg, ml = mons[u.umonnum].mlevel;
    int ret;
    int spellnum = 0;
    boolean directed = FALSE;

    /* guard against casting another spell attack
       at an already dead monster; some monsters
       have multiple AT_MAGC attacks */
    if (mdef->mhp <= 0)
        return 0;

    if ((mattk->adtyp == AD_SPEL || mattk->adtyp == AD_CLRC) && ml) {
        int cnt = 40;

        do {
            spellnum = rn2(ml);
            if (mattk->adtyp == AD_SPEL)
                spellnum = choose_magic_spell(mdef, spellnum);
            else
                spellnum = choose_clerical_spell(mdef, spellnum);
            /* not trying to attack?  don't allow directed spells */
            if (!mdef || mdef->mhp < 1) {
                /* Kludge here because uspell_would_be_useless doesn't take 
                 * the targeted monster as a parameter */
                if (spellnum == MGC_CREATE_POOL 
                    && (levl[u.ux][u.uy].typ == ROOM || levl[u.ux][u.uy].typ == CORR)
                    && (!is_flyer(mdef->data) && !is_floater(mdef->data) && !amphibious(mdef->data)))
                    break;
                if (is_undirected_spell(mattk->adtyp, spellnum)
                    && !uspell_would_be_useless(mattk->adtyp, spellnum)) {
                break;
            }
        }

    } while (--cnt > 0
             && ((!mdef && !is_undirected_spell(mattk->adtyp, spellnum))
                 || uspell_would_be_useless(mattk->adtyp, spellnum)));
        if (cnt == 0) {
            You("have no spells to cast right now!");
            return 0;
        }
    }

    if (spellnum == MGC_AGGRAVATION && !mdef) {
        /* choose a random monster on the level */
        int j = 0, k = 0;

        for (mdef = fmon; mdef; mdef = mdef->nmon) {
            if (!mdef->mtame && !mdef->mpeaceful)
                j++;
        }

        if (j > 0) {
            k = rn2(j);
            for (mdef = fmon; mdef; mdef = mdef->nmon) {
                if (!mdef->mtame && !mdef->mpeaceful) {
                    if (--k < 0)
                        break;
                }
            }
        }
    }

    directed = mdef && !is_undirected_spell(mattk->adtyp, spellnum);

    /* unable to cast spells? */
    if (u.uen < ml) {
        if (directed)
            You("point at %s, then curse.", mon_nam(mdef));
        else
            You("point all around, then curse.");
        return 0;
    }

    if (mattk->adtyp == AD_SPEL || mattk->adtyp == AD_CLRC) {
        u.uen -= ml;
    }

    if (rn2(ml * 10) < (Confusion ? 100 : 20)) { /* fumbled attack */
        pline_The("air crackles around you.");
        return 0;
    }

    You("cast a spell%s%s!", directed ? " at " : "",
        directed ? mon_nam(mdef) : "");

    /* As these are spells, the damage is related to the level
     * of the monster casting the spell. */
    if (mattk->damd)
        dmg = d((int) ((ml / 2) + mattk->damn), (int) mattk->damd);
    else
        dmg = d((int) ((ml / 2) + 1), 6);

    ret = 1;

    switch (mattk->adtyp) {
    case AD_FIRE:
        if (canseemon(mdef)) {
            if (is_demon(youmonst.data))
                pline("%s is enveloped in hellfire!", Monnam(mdef));
            else if (!mon_underwater(mdef))
                pline("%s is enveloped in flames.", Monnam(mdef));
            else {
                pline("The flames are quenched by the water around %s.",
                      mon_nam(mdef));
                dmg = 0;
                break;
            }
        }
      
        if (resists_fire(mdef) || defended(mdef, AD_FIRE)) {
            shieldeff(mdef->mx, mdef->my);
            if (is_demon(youmonst.data)) {
                if (!(nonliving(mdef->data) || is_demon(mdef->data))) {
                    if (canseemon(mdef))
                        pline_The("hellish flames sear %s soul!",
                                  s_suffix(mon_nam(mdef)));
                    dmg = (dmg + 1) / 2;
                } else {
                    dmg = 0;
                }
            } else {
                if (canseemon(mdef))
                    pline("But %s resists the effects.", mhe(mdef));
                dmg = 0;
            }
        }
        break;
    case AD_COLD:
        if (canseemon(mdef))
            pline("%s is covered in frost.", Monnam(mdef));
        if (resists_cold(mdef) || defended(mdef, AD_COLD)) {
            shieldeff(mdef->mx, mdef->my);
            if (canseemon(mdef))
                pline("But %s resists the effects.", mhe(mdef));
            dmg = 0;
        }
        break;
    case AD_ACID:
        if (canseemon(mdef)) {
            if (!mon_underwater(mdef))
                pline("%s is covered in acid.", Monnam(mdef));
            else {
                pline("The acid dissipates harmlessly in the water around %s.",
                      mon_nam(mdef));
                dmg = 0;
                break;
            }
        }
        if (resists_acid(mdef) || defended(mdef, AD_ACID)) {
            shieldeff(mdef->mx, mdef->my);
            if (canseemon(mdef))
                pline("But %s resists the effects.", mhe(mdef));
            dmg = 0;
        }
        break;
    case AD_MAGM:
        if (canseemon(mdef))
            pline("%s is hit by a shower of missiles!", Monnam(mdef));
        dmg = d((int)ml / 2 + 1, 6);
        if (resists_magm(mdef) || defended(mdef, AD_MAGM)) {
            shieldeff(mdef->mx, mdef->my);
            if (canseemon(mdef))
                pline("Some missiles bounce off!");
            dmg = (dmg + 1) / 2;
        }
        break;
    case AD_PSYC: {
        struct obj *pseudo = mksobj(SPE_PSIONIC_WAVE, FALSE, FALSE);
        bhitm(mdef, pseudo);
        obfree(pseudo, NULL);
        break;
    }
    case AD_SPEL:   /* wizard spell */
    case AD_CLRC: { /* clerical spell */
        if (mattk->adtyp == AD_SPEL)
            ucast_wizard_spell(&youmonst, mdef, dmg, spellnum);
        else
            ucast_cleric_spell(&youmonst, mdef, dmg, spellnum);
        dmg = 0; /* done by the spell casting functions */
        break;
        }
    }

    if (dmg) {
        if (damage_mon(mdef, dmg, mattk->adtyp))
            killed(mdef);
    }

    return (ret);
}

 extern NEARDATA const int nasties[];

/* monster wizard and cleric spellcasting functions */
/*
  If dmg is zero, then the monster is not casting at you.
  If the monster is intentionally not casting at you, we have previously
  called spell_would_be_useless() and spellnum should always be a valid
  undirected spell.
  If you modify either of these, be sure to change is_undirected_spell()
  and spell_would_be_useless().
*/
STATIC_OVL
void
ucast_wizard_spell(magr, mdef, dmg, spellnum)
struct monst *magr, *mdef;
int dmg;
int spellnum;
{
    boolean resisted = FALSE;
    boolean yours = (magr == &youmonst);

    if (dmg == 0 && !is_undirected_spell(AD_SPEL, spellnum)) {
        impossible("cast directed wizard spell (%d) with dmg = 0?", spellnum);
        return;
    }

    if (mdef && mdef->mhp < 1) {
        impossible("monster already dead?");
        return;
    }

    switch (spellnum) {
    case MGC_DEATH_TOUCH:
        if (!mdef || mdef->mhp < 1) {
            impossible("touch of death with no mdef");
            return;
        }
        if (yours)
            pline("You're using the touch of death!");
        else if (canseemon(magr)) {
            char buf[BUFSZ];
            Sprintf(buf, "%s%s",
                    mdef->mtame ? "Oh no, " : "", mhe(magr));
            if (!mdef->mtame)
                *buf = highc(*buf);

            pline("%s's using the touch of death!", buf);
        }

        resisted =
            ((resist(mdef, 0, 0, FALSE)
              && rn2(mons[yours ? u.umonnum : magr->mnum].mlevel) <= 12)
             || resists_magm(mdef) || defended(mdef, AD_MAGM));
        if (immune_death_magic(mdef->data) || is_vampshifter(mdef)) {
            if (yours || canseemon(mdef))
                pline("%s %s.", Monnam(mdef),
                      nonliving(mdef->data)
                          ? "seems no more dead than before"
                          : "is unaffected");
        } else if (!resisted) {
            mdef->mhp = -1;
            if (yours)
                killed(mdef);
            else 
                monkilled(mdef, "", AD_SPEL);
            return;
        } else {
            if (resisted)
                shieldeff(mdef->mx, mdef->my);
            if (yours || canseemon(mdef)) {
                if (mdef->mtame)
                    pline("Lucky for %s, it didn't work!", mon_nam(mdef));
                else
                    pline("Well.  That didn't work...");
            }
        }
        dmg = 0;
        break;
    case MGC_CANCELLATION:
        if (!mdef || mdef->mhp < 1) {
            impossible("cancellation with no mdef");
            return;
        }
        if (yours)
            You("%s a cancellation spell!",
                rn2(2) ? "evoke" : "conjure up");
        else if (canseemon(magr))
            pline("%s %s a cancellation spell!", Monnam(magr),
                  rn2(2) ? "evokes" : "conjures up");
        (void) cancel_monst(mdef, (struct obj *) 0, FALSE, TRUE, FALSE);
        break;
    case MGC_REFLECTION:
        if (!mdef || mdef->mhp < 1) {
            impossible("reflection with no mdef");
            return;
        }
        if (yours)
            (void) cast_reflection(&youmonst);
        else
            (void) cast_reflection(mdef);
        break;
    case MGC_ACID_BLAST:
        if (!mdef || mdef->mhp < 1) {
            impossible("acid blast with no mdef");
            return;
        }
        if (yours || canseemon(mdef))
            You("douse %s in a torrent of acid!", mon_nam(mdef));
        explode(mdef->mx, mdef->my, ZT_ACID, d((u.ulevel / 2) + 4, 8),
                WAND_CLASS, EXPL_ACID);
        if (resists_acid(mdef) || defended(mdef, AD_ACID)) {
            shieldeff(mdef->mx, mdef->my);
            if (canseemon(mdef))
                pline("But the acid dissipates harmlessly.");
        }
        if (rn2(4))
            erode_armor(mdef, ERODE_CORRODE);
        /* damage is handled by explode() */
        dmg = 0;
        break;
    case MGC_SUMMON_MONS: {
        int count = 0;
        register struct monst *mpet;

        if (!rn2(10) && Inhell) {
            if (yours)
                demonpet();
            else
                msummon(magr);
        } else {
            register int i, j;
            int makeindex, tmp = (u.ulevel > 3) ? u.ulevel / 3 : 1;
            const char *mappear;
            coord bypos;

            if (mdef)
                bypos.x = mdef->mx, bypos.y = mdef->my;
            else if (yours)
                bypos.x = u.ux, bypos.y = u.uy;
            else
                bypos.x = magr->mx, bypos.y = magr->my;

            for (i = rnd(tmp); i > 0; --i) {
                for (j = 0; j < 20; j++) {
                    do {
                        makeindex = pick_nasty(mons[u.umonnum].difficulty - 1);
                    } while (attacktype(&mons[makeindex], AT_MAGC)
                             && mons[makeindex].difficulty >= mons[u.umonnum].difficulty);
                        if (yours
                            && !enexto(&bypos, u.ux, u.uy, &mons[makeindex]))
                            continue;
                        if (!yours
                            && !enexto(&bypos, magr->mx, magr->my, &mons[makeindex]))
                            continue;
                    if ((mpet = makemon(&mons[makeindex], bypos.x, bypos.y,
                        (yours || magr->mtame) ? MM_EDOG : NO_MM_FLAGS)) != 0) {
                        mpet->msleeping = 0;
                        if (yours || magr->mtame)
                            initedog(mpet);
                        else if (magr->mpeaceful)
                            mpet->mpeaceful = 1;
                        else
                            mpet->mpeaceful = mpet->mtame = 0;
                        set_malign(mpet);
                    } else /* GENOD? */
                        mpet = makemon((struct permonst *)0,
                                       bypos.x, bypos.y, NO_MM_FLAGS);
                    if (mpet && (u.ualign.type == 0
                        || mon_aligntyp(mpet) == 0
                        || sgn(mon_aligntyp(mpet)) == sgn(u.ualign.type))) {
                        count++;
                        break;
                    }
                }
            }
            mappear = (count == 1) ? "A monster appears" : "Monsters appear";
            if (yours || canseemon(mdef))
                pline("%s from nowhere!", mappear);
        }
        dmg = 0;
        break;
    }
    case MGC_AGGRAVATION:
        if (!mdef || mdef->mhp < 1) {
            You_feel("lonely.");
            return;
        }
        you_aggravate(mdef);
        dmg = 0;
        break;
    case MGC_CURSE_ITEMS:
        if (!mdef || mdef->mhp < 1) {
            impossible("curse spell with no mdef");
            return;
        }
        if (yours || canseemon(mdef))
            You_feel("as though %s needs some help.", mon_nam(mdef));
        mrndcurse(mdef);
        dmg = 0;
        break;
    case MGC_DESTRY_ARMR:
        if (!mdef || mdef->mhp < 1) {
            impossible("destroy spell with no mdef");
            return;
        }
        dmg = m_destroy_armor(magr, mdef);
        break;
    case MGC_WEAKEN_YOU: /* drain strength */
        if (!mdef || mdef->mhp < 1) {
            impossible("weaken spell with no mdef");
            return;
        }
        if (resist(mdef, 0, 0, FALSE)) {
            shieldeff(mdef->mx, mdef->my);
            pline("%s looks momentarily weakened.", Monnam(mdef));
        } else {
            if (mdef->mhp < 1) {
                impossible("trying to drain monster that's already dead");
                return;
            }
            if (yours || canseemon(mdef))
                pline("%s suddenly seems weaker!", Monnam(mdef));
            
            /* monsters don't have strength, so drain max hp instead */
            mdef->mhpmax -= dmg;
            
            if (damage_mon(mdef, dmg, AD_DRLI)) {
                if (yours)
                    killed(mdef);
                else monkilled(mdef, "", AD_SPEL);
            }
        }
        dmg = 0;
        break;
    case MGC_DISAPPEAR: /* makes self invisible */
        if (!yours) {
            impossible("ucast disappear but not yours?");
            return;
        }
        if (!(HInvis & INTRINSIC)) {
            HInvis |= FROMOUTSIDE;
            if (!Blind && !BInvis)
                self_invis_message();
            dmg = 0;
        } else
            impossible("no reason for player to cast disappear spell?");
        break;
    case MGC_STUN_YOU:
        if (!mdef || mdef->mhp < 1) {
            impossible("stun spell with no mdef");
            return;
        }
        if (resist(mdef, 0, 0, FALSE)) {
            shieldeff(mdef->mx, mdef->my);
            if (yours || canseemon(mdef) 
                  || resists_stun(mdef->data) || defended(mdef, AD_STUN))
                pline("%s seems momentarily disoriented.", Monnam(mdef));
        } else {
            if (yours || canseemon(mdef)) {
                if (mdef->mstun)
                    pline("%s struggles to keep %s balance.",
                          Monnam(mdef), mhis(mdef));
                else
                    pline("%s reels...", Monnam(mdef));
            }
            mdef->mstun = 1;
        }
        dmg = 0;
        break;
    case MGC_HASTE_SELF:
        if (!yours) {
            impossible("ucast haste but not yours?");
            return;
        }
        if (!(HFast & INTRINSIC)) {
            You("are suddenly moving faster.");
            HFast |= INTRINSIC;
        }
        dmg = 0;
        break;
    case MGC_CURE_SELF:
        if (!yours) {
            impossible("ucast healing but not yours?");
        } else if (u.mh < u.mhmax) {
            You_feel("better.");
            u.mh += dmg;
            if ((u.mh += d(3, 6)) > u.mhmax)
                u.mh = u.mhmax;
            context.botl = 1;
            dmg = 0;
        }
        break;
    case MGC_FIRE_BOLT:
    case MGC_ICE_BOLT:
        if (!mdef || mdef->mhp < 1) {
            impossible("bolt spell with no mdef");
            return;
        }
        if (yours || canseemon(mdef)) {
            You("blast %s with %s!", mon_nam(mdef),
                (spellnum == MGC_FIRE_BOLT) ? "fire" : "ice");
            explode(mdef->mx, mdef->my,
                    (spellnum == MGC_FIRE_BOLT) ? ZT_FIRE : ZT_COLD,
                    d((u.ulevel / 5) + 1, 8), WAND_CLASS, 1);
            if (spellnum == MGC_FIRE_BOLT
                && (resists_fire(mdef) || defended(mdef, AD_FIRE))) {
                shieldeff(mdef->mx, mdef->my);
                pline("But %s seems unaffected by the fire.", mon_nam(mdef));
            } else if (spellnum == MGC_ICE_BOLT
                && (resists_cold(mdef) || defended(mdef, AD_COLD))) {
                shieldeff(mdef->mx, mdef->my);
                pline("But %s seems unaffected by the cold.", mon_nam(mdef));
            }
            if (spellnum == MGC_FIRE_BOLT) {
                (void) burnarmor(mdef);
                destroy_mitem(mdef, SCROLL_CLASS, AD_FIRE);
                destroy_mitem(mdef, POTION_CLASS, AD_FIRE);
                destroy_mitem(mdef, SPBOOK_CLASS, AD_FIRE);
                destroy_mitem(mdef, WEAPON_CLASS, AD_FIRE);
                (void) burn_floor_objects(mdef->mx, mdef->my, TRUE, FALSE);
            }
        }
        /* damage is handled by explode() */
        dmg = 0;
        break;
    case MGC_PSI_BOLT:
        if (!mdef || mdef->mhp < 1) {
            impossible("psibolt spell with no mdef");
            return;
        }
        if (resist(mdef, 0, 0, FALSE)) {
            shieldeff(mdef->mx, mdef->my);
            dmg = (dmg + 1) / 2;
        }
        if (yours || canseemon(mdef))
            pline("%s %s%s", Monnam(mdef),
                  can_flollop(mdef->data) ? "flollops" : "winces",
                  (dmg <= 5) ? "." : "!");
        break;
    case MGC_CREATE_POOL:
        if (!mdef || mdef->mhp < 1) {
            impossible("create pool spell with no mdef");
            return;
        }
        if (zombie_can_dig(mdef->mx, mdef->my)) {
            if (yours || canseemon(mdef)) {
                pline("A pool appears beneath %s!", mon_nam(mdef));
            }
            levl[mdef->mx][mdef->my].typ = POOL;
            del_engr_at(mdef->mx, mdef->my);
            (void) mintrap(mdef);
            water_damage_chain(level.objects[mdef->mx][mdef->my], TRUE, 0, TRUE, mdef->mx, mdef->my);
            spoteffects(FALSE);  /* possibly drown, notice objects */
        }
        break;
    default:
        impossible("ucastm: invalid magic spell (%d)", spellnum);
        dmg = 0;
        break;
    }

    if (dmg) {
        if (damage_mon(mdef, dmg, AD_SPEL)) {
            if (yours)
                killed(mdef);
            else
                monkilled(mdef, "", AD_SPEL);
        }
    }
}

STATIC_OVL
void
ucast_cleric_spell(magr, mdef, dmg, spellnum)
struct monst *magr, *mdef;
int dmg;
int spellnum;
{
    int aligntype;
    static const char *Moloch = "Moloch";
    boolean yours = (magr == &youmonst);
    struct monst *minion = (struct monst *) 0;

    if (dmg == 0 && !is_undirected_spell(AD_CLRC, spellnum)) {
        impossible("cast directed cleric spell (%d) with dmg=0?", spellnum);
        return;
    }

    if (mdef && mdef->mhp < 1) {
        impossible("monster already dead?");
        return;
    }

    switch (spellnum) {
    case CLC_SUMMON_ELM:
        if (!mdef || mdef->mhp < 1) {
            impossible("summon elemental spell with no mdef");
            return;
        }

        aligntype = yours ? u.ualign.type : mon_aligntyp(magr);
        minion = summon_minion(aligntype, FALSE);
        if (minion && canspotmon(minion))
            pline("A %s of %s appears!",
                  aligntype == A_NONE ? "vassal" : "servant",
                  aligntype == A_NONE ? Moloch : aligns[1 - aligntype].noun);
        dmg = 0;
        break;
    case CLC_GEYSER:
        /* this is physical damage, not magical damage */
        if (!mdef || mdef->mhp < 1) {
            impossible("geyser spell with no mdef");
            return;
        }
        if (yours || canseemon(mdef))
            pline("A sudden geyser slams into %s from nowhere!",
                  mon_nam(mdef));
        dmg = d(8, 6);
        (void) erode_armor(mdef, ERODE_RUST);
        break;
    case CLC_FIRE_PILLAR:
        if (!mdef || mdef->mhp < 1) {
            impossible("fire pillar spell with no mdef");
            return;
        }
        if (yours || canseemon(mdef))
            pline("A pillar of fire strikes all around %s!",
                  mon_nam(mdef));
        if (resists_fire(mdef) || defended(mdef, AD_FIRE)) {
            shieldeff(mdef->mx, mdef->my);
            dmg = 0;
        } else
            dmg = d(8, 6);
        (void) burnarmor(mdef);
        destroy_mitem(mdef, SCROLL_CLASS, AD_FIRE);
        destroy_mitem(mdef, POTION_CLASS, AD_FIRE);
        destroy_mitem(mdef, SPBOOK_CLASS, AD_FIRE);
        destroy_mitem(mdef, WEAPON_CLASS, AD_FIRE);
        (void) burn_floor_objects(mdef->mx, mdef->my, TRUE, FALSE);
        break;
    case CLC_LIGHTNING: {
        boolean reflects;
        if (!mdef || mdef->mhp < 1) {
            impossible("lightning spell with no mdef");
            return;
        }

        if (yours || canseemon(mdef))
            pline("A bolt of lightning strikes down at %s from above!",
                  mon_nam(mdef));
        reflects = mon_reflects(mdef, "It bounces off %s %s.");
        if (reflects || resists_elec(mdef) || defended(mdef, AD_ELEC)) {
            shieldeff(u.ux, u.uy);
            dmg = 0;
            if (reflects)
                break;
        } else
            dmg = d(8, 6);
        destroy_mitem(mdef, WAND_CLASS, AD_ELEC);
        destroy_mitem(mdef, RING_CLASS, AD_ELEC);
        break;
    }
    case CLC_CURSE_ITEMS:
        if (!mdef || mdef->mhp < 1) {
            impossible("curse spell with no mdef");
            return;
        }
        if (yours || canseemon(mdef))
            You_feel("as though %s needs some help.", mon_nam(mdef));
        mrndcurse(mdef);
        dmg = 0;
        break;
    case CLC_INSECTS: {
        /* Try for insects, and if there are none
           left, go for (sticks to) snakes.  -3. */
        struct permonst *pm = mkclass(S_ANT, 0);
        struct monst *mdef2 = (struct monst *) 0;
        char let = (pm ? S_ANT : S_SNAKE);
        boolean success;
        int i;
        coord bypos;
        int quan;

        if (!mdef || mdef->mhp < 1) {
            impossible("insect spell with no mdef");
            return;
        }

        if (mdef)
            bypos.x = mdef->mx, bypos.y = mdef->my;
        else if (yours)
            bypos.x = u.ux, bypos.y = u.uy;
        else
            bypos.x = magr->mx, bypos.y = magr->my;

        quan = (mons[u.umonnum].mlevel < 2) ? 1 :
         	rnd(mons[u.umonnum].mlevel / 2);
        if (quan < 3) quan = 3;
            success = pm ? TRUE : FALSE;
        for (i = 0; i <= quan; i++) {
            if (!enexto(&bypos, mdef->mx, mdef->my, mdef->data))
                break;
            if ((pm = mkclass(let, 0)) != 0
                && (mdef2 = makemon(pm, bypos.x, bypos.y, NO_MM_FLAGS)) != 0) {
                success = TRUE;
                mdef2->msleeping = 0;
                if (yours || magr->mtame)
                    (void) tamedog(mdef2, (struct obj *) 0);
                else if (magr->mpeaceful)
                    mdef2->mpeaceful = 1;
                else
                    mdef2->mpeaceful = 0;
                set_malign(mdef2);
            }
        }

        if (yours) {
            if (!success)
                You("cast at a clump of sticks, but nothing happens.");
            else if (let == S_SNAKE)
                You("transforms a clump of sticks into snakes!");
            else
                You("summon insects!");
        } else if (canseemon(mdef)) {
            if (!success)
                pline("%s casts at a clump of sticks, but nothing happens.",
                      Monnam(magr));
            else if (let == S_SNAKE)
                pline("%s transforms a clump of sticks into snakes!",
                      Monnam(magr));
            else
                pline("%s summons insects!", Monnam(magr));
        }
        dmg = 0;
        break;
    }
    case CLC_BLIND_YOU:
        if (!mdef || mdef->mhp < 1) {
            impossible("blindness spell with no mdef");
            return;
        }
        /* note: resists_blnd() doesn't apply here */
        if (!mdef->mblinded
            && haseyes(mdef->data)) {
            if (!resists_blnd(mdef)) {
                int num_eyes = eyecount(mdef->data);
                if (yours || canseemon(mdef))
                    pline("Scales cover %s %s!", s_suffix(mon_nam(mdef)),
                          (num_eyes == 1) ? "eye" : "eyes");
                mdef->mblinded = 127;
            }
            dmg = 0;
        } else if (!yours)
            impossible("no reason for monster to cast blindness spell?");
        break;
    case CLC_PARALYZE:
        if (!mdef || mdef->mhp < 1) {
            impossible("paralysis spell with no mdef");
            return;
        }
        if (resist(mdef, 0, 0, FALSE)) {
            shieldeff(mdef->mx, mdef->my);
            if (yours || canseemon(mdef))
                pline("%s stiffens briefly.", Monnam(mdef));
        } else {
            if (yours || canseemon(mdef))
                pline("%s is frozen in place!", Monnam(mdef));
            dmg = 4 + mons[u.umonnum].mlevel;
            mdef->mcanmove = 0;
            mdef->mfrozen = dmg;
        }
        dmg = 0;
        break;
    case CLC_CONFUSE_YOU:
        if (!mdef || mdef->mhp < 1) {
            impossible("confusion spell with no mdef");
            return;
        }
        if (resist(mdef, 0, 0, FALSE)) {
            shieldeff(mdef->mx, mdef->my);
            if (yours || canseemon(mdef))
                pline("%s seems momentarily dizzy.", Monnam(mdef));
        } else {
            if (yours || canseemon(mdef))
                pline("%s seems %sconfused!", Monnam(mdef),
                      mdef->mconf ? "more " : "");
            mdef->mconf = 1;
        }
        dmg = 0;
        break;
    case CLC_CURE_SELF:
        if (!yours) {
            impossible("ucast healing but not yours?");
        } else if (u.mh < u.mhmax) {
            You_feel("better.");
            u.mh += dmg;
            /* note: player healing does 6d4; this used to do 1d8 */
            if ((u.mh += d(3, 6)) > u.mhmax)
                u.mh = u.mhmax;
            context.botl = 1;
            dmg = 0;
        }
        break;
    case CLC_VULN_YOU:
        if (!mdef || mdef->mhp < 1) {
            impossible("vulnerable spell with no mdef");
            return;
        }

        if (yours || canseemon(mdef)) {
            dmg = rnd(5);
            pline("A %s film oozes over %s exterior!",
                  Blind ? "slimy" : vulntext[dmg], mhis(mdef));
            switch (dmg) {
            case 1:
                if ((mdef->data->mflags4 & M4_VULNERABLE_FIRE) != 0
                    || mdef->vuln_fire)
                    return;
                mdef->vuln_fire = rnd(100) + 150;
                pline("%s is more inflammable.", Monnam(mdef));
                break;
            case 2:
                if ((mdef->data->mflags4 & M4_VULNERABLE_COLD) != 0
                    || mdef->vuln_cold)
                    return;
                mdef->vuln_cold = rnd(100) + 150;
                pline("%s is extremely chilly.", Monnam(mdef));
                break;
            case 3:
                if ((mdef->data->mflags4 & M4_VULNERABLE_ELEC) != 0
                    || mdef->vuln_elec)
                    return;
                mdef->vuln_elec = rnd(100) + 150;
                pline("%s is overly conductive.", Monnam(mdef));
                break;
            case 4:
                if ((mdef->data->mflags4 & M4_VULNERABLE_ACID) != 0
                    || mdef->vuln_acid)
                    return;
                mdef->vuln_acid = rnd(100) + 150;
                pline("%s is easily corrodable.", Monnam(mdef));
                break;
            case 5:
                if ((mdef->data->mflags4 & M4_VULNERABLE_LOUD) != 0
                    || mdef->vuln_loud)
                    return;
                mdef->vuln_loud = rnd(100) + 150;
                pline("%s is hyperacute.", Monnam(mdef));
                break;
            default:
                break;
            }
        }
        dmg = 0;
        break;
    case CLC_OPEN_WOUNDS:
        if (!mdef || mdef->mhp < 1) {
            impossible("wound spell with no mdef");
            return;
        }
        if (resist(mdef, 0, 0, FALSE)) {
            shieldeff(mdef->mx, mdef->my);
            dmg = (dmg + 1) / 2;
        }
        /* not canseemon; if you can't see it you don't know it was wounded */
        if (yours) {
            if (dmg <= 5)
                pline("%s looks itchy!", Monnam(mdef));
            else if (dmg <= 10)
                pline("Wounds appear on %s!", mon_nam(mdef));
            else if (dmg <= 20)
                pline("Severe wounds appear on %s!", mon_nam(mdef));
            else
                pline("%s is covered in wounds!", Monnam(mdef));
        }
        break;
    case CLC_PROTECTION:
        if (!mdef || mdef->mhp < 1) {
            impossible("protection with no mdef");
            return;
        }
        if (yours)
            (void) cast_protection();
        break;
    default:
        impossible("ucastm: invalid clerical spell (%d)", spellnum);
        dmg = 0;
        break;
    }

    if (dmg) {
        if (damage_mon(mdef, dmg, AD_CLRC)) {
            if (yours)
                killed(mdef);
            else
                monkilled(mdef, "", AD_CLRC);
        }
    }
}

/* Figure out how far a spellcaster can reach with a spell based on their level */
STATIC_OVL int
mcastrange(ml) 
int ml;
{
    if (ml < 5) 
        return 9;   /* ~3 spaces */
    if (ml < 7)
        return 16;  /* ~4 spaces */
    if (ml < 9)
        return 25;  /* ~5 spaces */
    if (ml < 11)
        return 49;  /* ~7 spaces */
    return 192;     /* ~13 spaces */
}

/*mcastu.c*/
