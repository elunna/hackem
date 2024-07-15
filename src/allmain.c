/* NetHack 3.6	allmain.c	$NHDT-Date: 1555552624 2019/04/18 01:57:04 $  $NHDT-Branch: NetHack-3.6.2-beta01 $:$NHDT-Revision: 1.100 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* NetHack may be freely redistributed.  See license for details. */

/* various code that was replicated in *main.c */

#include "hack.h"
#include <ctype.h>

#ifndef NO_SIGNAL
#include <signal.h>
#endif

#ifdef POSITIONBAR
STATIC_DCL void NDECL(do_positionbar);
#endif
STATIC_DCL void FDECL(regen_hp, (int));
STATIC_DCL void FDECL(interrupt_multi, (const char *));
STATIC_DCL void FDECL(debug_fields, (const char *));
STATIC_DCL void NDECL(init_mchest);
STATIC_DCL void NDECL(pickup_spirits);
STATIC_DCL void NDECL(update_monclock);

#ifdef EXTRAINFO_FN
static long prev_dgl_extrainfo = 0;
#endif

enum monster_generation monclock;

boolean
elf_can_regen()
{
    if (maybe_polyd(is_elf(youmonst.data), Race_if(PM_ELF))) {
        if (uwep && is_iron(uwep)
            && !is_quest_artifact(uwep) && !uarmg)
            return 0;
        if (uarm && is_iron(uarm) && !uarmu)
            return 0;
        if (uarmu && is_iron(uarmu))
            return 0;
        if (uarmc && is_iron(uarmc) && !uarmu && !uarm)
            return 0;
        if (uarmh && is_iron(uarmh)
            && !is_quest_artifact(uarmh))
            return 0;
        if (uarms && is_iron(uarms) && !uarmg)
            return 0;
        if (uarmg && is_iron(uarmg))
            return 0;
        if (uarmf && is_iron(uarmf))
            return 0;
        if (uleft && is_iron(uleft))
            return 0;
        if (uright && is_iron(uright))
            return 0;
        if (uamul && is_iron(uamul) && !is_quest_artifact(uamul)
            && !uarmu && !uarm)
            return 0;
        if (ublindf && is_iron(ublindf))
            return 0;
        if (uchain && is_iron(uchain))
            return 0;
        if (uswapwep && is_iron(uswapwep)
            && u.twoweap && !uarmg)
            return 0;
    }
    return 1;
}

boolean
orc_can_regen()
{
    if (maybe_polyd(is_orc(youmonst.data), Race_if(PM_ORC))) {
        if (uwep && is_mithril(uwep)
            && !is_quest_artifact(uwep) && !uarmg)
            return 0;
        if (uarm && is_mithril(uarm) && !uarmu)
            return 0;
        if (uarmu && is_mithril(uarmu))
            return 0;
        if (uarmc && is_mithril(uarmc) && !uarmu && !uarm)
            return 0;
        if (uarmh && is_mithril(uarmh)
            && !is_quest_artifact(uarmh))
            return 0;
        if (uarms && is_mithril(uarms) && !uarmg)
            return 0;
        if (uarmg && is_mithril(uarmg))
            return 0;
        if (uarmf && is_mithril(uarmf))
            return 0;
        if (uleft && is_mithril(uleft))
            return 0;
        if (uright && is_mithril(uright))
            return 0;
        if (uamul && is_mithril(uamul) && !is_quest_artifact(uamul)
            && !uarmu && !uarm)
            return 0;
        if (ublindf && is_mithril(ublindf))
            return 0;
        if (uchain && is_mithril(uchain))
            return 0;
        if (uswapwep && is_mithril(uswapwep)
            && u.twoweap && !uarmg)
            return 0;
    }
    return 1;
}

boolean
vamp_can_regen()
{
    if (maybe_polyd(is_vampire(youmonst.data), Race_if(PM_VAMPIRIC))) {
        if (uwep && is_silver(uwep)
            && !is_quest_artifact(uwep) && !uarmg)
            return 0;
        if (uarm && is_silver(uarm) && !uarmu)
            return 0;
        if (uarmu && is_silver(uarmu))
            return 0;
        if (uarmc && is_silver(uarmc) && !uarmu && !uarm)
            return 0;
        if (uarmh && is_silver(uarmh)
            && !is_quest_artifact(uarmh))
            return 0;
        if (uarms && is_silver(uarms) && !uarmg)
            return 0;
        if (uarmg && is_silver(uarmg))
            return 0;
        if (uarmf && is_silver(uarmf))
            return 0;
        if (uleft && is_silver(uleft))
            return 0;
        if (uright && is_silver(uright))
            return 0;
        if (uamul && is_silver(uamul) && !is_quest_artifact(uamul)
            && !uarmu && !uarm)
            return 0;
        if (ublindf && is_silver(ublindf))
            return 0;
        if (uchain && is_silver(uchain))
            return 0;
        if (uswapwep && is_silver(uswapwep)
            && u.twoweap && !uarmg)
            return 0;
    }
    return 1;
}

void
moveloop(resuming)
boolean resuming;
{
#if defined(MICRO) || defined(WIN32)
    char ch;
    int abort_lev;
#endif
    struct obj *pobj, *aobj;
    int moveamt = 0, wtcap = 0, change = 0;
	int i;
    /* don't make it obvious when monsters will start speeding up */
    boolean monscanmove = FALSE;
    boolean elf_regen = elf_can_regen();
    boolean orc_regen = orc_can_regen();
    boolean vamp_regen = vamp_can_regen();
    
    /* Note:  these initializers don't do anything except guarantee that
            we're linked properly.
    */
    decl_init();
    monst_init();
    objects_init();

    /* if a save file created in normal mode is now being restored in
       explore mode, treat it as normal restore followed by 'X' command
       to use up the save file and require confirmation for explore mode */
    if (resuming && iflags.deferred_X)
        (void) enter_explore_mode();
    
    /* side-effects from the real world */
    flags.moonphase = phase_of_the_moon();
    if (flags.moonphase == FULL_MOON) {
        You("are lucky!  Full moon tonight.");
        change_luck(1);
    } else if (flags.moonphase == NEW_MOON) {
        pline("Be careful!  New moon tonight.");
    }
    flags.friday13 = friday_13th();
    if (flags.friday13) {
        pline("Watch out!  Bad things can happen on Friday the 13th.");
        change_luck(-1);
    }
    for (i = 1; i <= u.ulevel; i++) {
		u.tempulevel = i;
		adjtech(i - 1, i);
	}
	u.tempulevel = 0;
    if (flags.quest_boon) {
        change_luck(1); /* silent */
    }

    if (!resuming) { /* new game */
        context.rndencode = rnd(9000);
        set_wear((struct obj *) 0); /* for side-effects of starting gear */
        (void) pickup(1);      /* autopickup at initial location */
        /* only matters if someday a character is able to start with
           clairvoyance (wizard with cornuthaum perhaps?); without this,
           first "random" occurrence would always kick in on turn 1 */
        context.seer_turn = (long) rnd(30);
        init_mchest();
        u.umovement = NORMAL_SPEED;  /* giants and tortles put their best foot forward */
    }
    context.botlx = TRUE; /* for STATUS_HILITES */
    update_inventory(); /* for perm_invent */
    if (resuming) { /* restoring old game */
        read_engr_at(u.ux, u.uy); /* subset of pickup() */
    }
    if (halloween()) {
        pline("Beware the Undead, for they roam the Dungeons of Doom on All Hallows' Eve!");
    }

    (void) encumber_msg(); /* in case they auto-picked up something */
    if (defer_see_monsters) {
        defer_see_monsters = FALSE;
        see_monsters();
    }
    initrack();

    u.uz0.dlevel = u.uz.dlevel;
    context.move = 0;

    program_state.in_moveloop = 1;

#ifdef WHEREIS_FILE
    touch_whereis();
#endif

    for (;;) {
#ifdef SAFERHANGUP
        if (program_state.done_hup)
            end_of_input();
#endif
        get_nh_event();
#ifdef POSITIONBAR
        do_positionbar();
#endif

        if (context.move) {
            /* actual time passed */
            u.umovement -= NORMAL_SPEED;

            do { /* hero can't move this turn loop */
                wtcap = encumber_msg();
                if (!u.utimestop) {
                    context.mon_moving = TRUE;
                    do {
                        monscanmove = movemon();
                        if (u.umovement >= NORMAL_SPEED)
                            break; /* it's now your turn */
                    } while (monscanmove);
                    context.mon_moving = FALSE;
                } else if (u.umovement < NORMAL_SPEED) {
                    /* If a scroll of time has been read, we want the player's
                     * current turn to be extended. */
                    u.utimestop = FALSE;
                    continue;
                }
                if (!monscanmove && u.umovement < NORMAL_SPEED) {
                    /* both hero and monsters are out of steam this round */
                    struct monst *mtmp;

                    /* set up for a new turn */
                    mcalcdistress(); /* adjust monsters' trap, blind, etc */

                    /* reallocate movement rations to monsters; don't need
                       to skip dead monsters here because they will have
                       been purged at end of their previous round of moving */
                    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
                        mtmp->movement += mcalcmove(mtmp);
                    
                    /* Handle monster spawn rates */
                    update_monclock();

                    if (!rn2(monclock)) {
                        if (u.uevent.invoked && xupstair && rn2(10))
                            (void) makemon((struct permonst *) 0,
                                           xupstair, yupstair,
                                           MM_ADJACENTOK | MM_MPLAYEROK);
                        else if (u.uevent.invoked && sstairs.sx && rn2(10))
                            (void) makemon((struct permonst *) 0,
                                           sstairs.sx, sstairs.sy,
                                           MM_ADJACENTOK | MM_MPLAYEROK);
                        else
                            (void) makemon((struct permonst *) 0, 0, 0,
                                           MM_MPLAYEROK);
                    }
                    
                    /* calculate how much time passed. */
                    if (u.usteed && u.umoved) {
                        /* your speed doesn't augment steed's speed */
                        moveamt = mcalcmove(u.usteed);
                    } else {
                        moveamt = youmonst.data->mmove;

                        if (Very_fast) { /* speed boots, potion, or spell */
                            /* gain a free action on 2/3 of turns */
                            if (rn2(3) != 0)
                                moveamt += NORMAL_SPEED;
                        } else if (Fast) { /* intrinsic */
                            /* gain a free action on 1/3 of turns */
                            if (rn2(3) == 0)
                                moveamt += NORMAL_SPEED;
                        } else if (Slow) {
                            /* average movement noticeably slower */
                            if (rn2(3) != 0)
                                moveamt -= NORMAL_SPEED / 2;
                        }
                        
                        boolean surfing = ((is_damp_terrain(u.ux, u.uy) 
                                || is_lava(u.ux, u.uy))
                                        && !Underwater && using_oprop(ITEM_SURF));
                       
                        /* TECH: Blinking! */
                        if (tech_inuse(T_BLINK) || surfing) {
                            /* Case    Average  Variance
                            * -------------------------
                            * Normal    12         0
                            * Fast      16        12
                            * V fast    20        12
                            * Blinking  24        12
                            * F & B     28        18
                            * V F & B   30        18
                            */
                            moveamt += NORMAL_SPEED * 2 / 3;
                            if (rn2(3) == 0) 
                                moveamt += NORMAL_SPEED / 2;
                        }
                    }

                    if (Afraid && rn2(4)) { /* Afraid of a monster */
                        moveamt += NORMAL_SPEED;
                    }

                    switch (wtcap) {
                    case UNENCUMBERED:
                        break;
                    case SLT_ENCUMBER:
                        moveamt -= (moveamt / 4);
                        break;
                    case MOD_ENCUMBER:
                        moveamt -= (moveamt / 2);
                        break;
                    case HVY_ENCUMBER:
                        moveamt -= ((moveamt * 3) / 4);
                        break;
                    case EXT_ENCUMBER:
                        moveamt -= ((moveamt * 7) / 8);
                        break;
                    default:
                        break;
                    }

                    u.umovement += moveamt;
                    if (u.umovement < 0)
                        u.umovement = 0;
                    settrack();

                    monstermoves++;
                    moves++;

                    /********************************/
                    /* once-per-turn things go here */
                    /********************************/

                    if (Glib)
                        glibr();
                    nh_timeout();
                    run_regions();

                    dgn_growths(TRUE, TRUE);
                    
                    if (u.ublesscnt)
                        u.ublesscnt--;
                    if (flags.time && !context.run)
                        iflags.time_botl = TRUE;
#ifdef EXTRAINFO_FN
                    if ((prev_dgl_extrainfo == 0) || (prev_dgl_extrainfo < (moves + 250))) {
                        prev_dgl_extrainfo = moves;
                        mk_dgl_extrainfo();
                    }
#endif
                    
                    if ((pobj = carrying_arti(ART_TREASURY_OF_PROTEUS))) {
                        if (u.protean > 0)
                            u.protean--;
                        else {
                            if (pobj->cobj)
                                arti_poly_contents(pobj);
                            u.protean = rnz(100) + d(3, 10);
                            update_inventory();
                        }
                    }
                    
                    /* One possible result of prayer is healing.  Whether or
                     * not you get healed depends on your current hit points.
                     * If you are allowed to regenerate during the prayer,
                     * the end-of-prayer calculation messes up on this.
                     * Another possible result is rehumanization, which
                     * requires that encumbrance and movement rate be
                     * recalculated.
                     */
                    if (u.uinvulnerable) {
                        /* for the moment at least, you're in tiptop shape */
                        wtcap = UNENCUMBERED;
                    } else if (!Upolyd ? (u.uhp < u.uhpmax)
                                       : (u.mh < u.mhmax
                                          || youmonst.data->mlet == S_EEL)) {
                        /* maybe heal */
                        regen_hp(wtcap);
                    }

                    /* wither away a bit */
                    if (Withering && !u.uinvulnerable) {
                        int loss = rnd(2) - (Regeneration ? 1 : 0);
                        if (loss >= (Upolyd ? u.mh : u.uhp))
                            You("wither away completely!");
                        losehp(loss, "withered away", NO_KILLER_PREFIX);
                        context.botl = TRUE;
                        interrupt_multi("You are slowly withering away.");
                    }

                    /* moving around while encumbered is hard work */
                    if (wtcap > MOD_ENCUMBER && u.umoved) {
                        if (!(wtcap < EXT_ENCUMBER ? moves % 30
                                                   : moves % 10)) {
                            if (Upolyd && u.mh > 1) {
                                u.mh--;
                                context.botl = TRUE;
                            } else if (!Upolyd && u.uhp > 1) {
                                u.uhp--;
                                context.botl = TRUE;
                            } else {
                                You("pass out from exertion!");
                                exercise(A_CON, FALSE);
                                fall_asleep(-10, FALSE);
                            }
                        }
                    }

                    if (u.uen < u.uenmax 
                        && !Role_if(PM_NECROMANCER)
                        && ((wtcap < MOD_ENCUMBER
                             && (!(moves % ((MAXULEV + 8 - u.ulevel)
                                            * ((Role_if(PM_WIZARD) || Role_if(PM_INFIDEL))
                                                       ? 3 : 4)
                                            / 6)))) || Energy_regeneration
                    /* the Idol grants energy regen to piously unaligned;
                     * it really shouldn't be restricted to Infidels,
                     * but so far we have no other unaligned roles */
                            || (Role_if(PM_INFIDEL) && u.uhave.questart
                               && u.ualign.type == A_NONE
                               && u.ualign.record > rn2(20)))) {
                        u.uen += rn1(
                            (int) (ACURR(A_WIS) + ACURR(A_INT)) / 15 + 1, 1);
                        if (u.uen > u.uenmax)
                            u.uen = u.uenmax;
                        context.botl = TRUE;
                        if (u.uen == u.uenmax)
                            interrupt_multi("You feel full of energy.");
                    }

                    /* Moloch demands regular sacrifices! */
                    if (context.next_moloch_offering <= moves) {
                        if (context.next_moloch_offering == moves
                            && (u.ualign.type == A_NONE
                                || u.ualignbase[A_CURRENT] == A_NONE)) {
                            You_feel("%s urge to perform a sacrifice.",
                                     u.ualign.type == A_NONE ? "an"
                                                             : "a faint");
                            stop_occupation();
                        }
                        if (u.ualign.type == A_NONE
                            && rn2(moves - context.next_moloch_offering
                                   + 1000) >= 1000) {
                            if (!rn2(100) && u.ualign.record > -99) {
                                adjalign(-1);
                                /* give our infidel some feedback every
                                   once in a while */
                                if (!rn2(5))
                                    You_feel("your favor with %s starting to slip.",
                                             u_gname());
                            }
                            if (u.ualign.record < -10 && !rn2(u.ugangr + 1)
                                && rn2(-u.ualign.record + 90) >= 100) {
                                const char *angry = (char *) 0;
                                u.ugangr++;
                                /* avoid repetitive messages */
                                switch (u.ugangr) {
                                /* same values as in cmd.c */
                                case 1:
                                    angry = "";
                                    break;
                                case 4:
                                    angry = "very ";
                                    break;
                                case 7:
                                    angry = "extremely ";
                                    break;
                                }
                                if (angry) {
                                    You_feel("that %s is %sangry at your "
                                             "lack of offerings.", u_gname(),
                                             angry);
                                    stop_occupation();
                                }
                            }
                        }
                    }

                    pickup_spirits();
                    
                    if ((aobj = carrying_arti(ART_NIGHTHORN))) {
                        if (aobj->cursed && !rn2(100))
                            make_afraid((HAfraid & TIMEOUT) + (long) rn1(10, 5), TRUE);
                    }
                        
                    if (!u.uinvulnerable) {
                        if (Teleportation && !rn2(85)) {
                            xchar old_ux = u.ux, old_uy = u.uy;

                            tele();
                            if (u.ux != old_ux || u.uy != old_uy) {
                                if (!next_to_u()) {
                                    check_leash(old_ux, old_uy);
                                }
                                /* clear doagain keystrokes */
                                pushch(0);
                                savech(0);
                            }
                        }
                        /* delayed change may not be valid anymore */
                        if ((change == 1 && !Polymorph)
                            || (change == 2 && u.ulycn == NON_PM))
                            change = 0;
                        if (Polymorph && !rn2(100))
                            change = 1;
                        else if (u.ulycn >= LOW_PM && !Upolyd
                                 && !rn2(80 - (20 * night())))
                            change = 2;
                        if (change && !Unchanging) {
                            if (multi >= 0) {
                                stop_occupation();
                                if (change == 1)
                                    polyself(0);
                                else
                                    you_were();
                                change = 0;
                            }
                        }
                    }

                    if (Searching && multi >= 0)
                        (void) dosearch0(1);
                    if (Warning)
                        warnreveal();
                    mkot_trap_warn();
                    dosounds();
                    do_storms();
                    gethungry();
                    age_spells();
                    exerchk();
                    invault();
                    if (u.uhave.amulet)
                        amulet();
                    if (!rn2(40 + (int) (ACURR(A_DEX) * 3)))
                        u_wipe_engr(rnd(3));
                    if (u.uevent.udemigod && !u.uinvulnerable) {
                        if (u.udg_cnt)
                            u.udg_cnt--;
                        if (!u.udg_cnt) {
                            intervene();
                            u.udg_cnt = rn1(200, (42 + Luck));
                        }
                    }
                    restore_attrib();
                    /* vision will be updated as bubbles move */
                    if (Is_waterlevel(&u.uz) || Is_airlevel(&u.uz))
                        movebubbles();
                    else if (Is_firelevel(&u.uz))
                        fumaroles();

                    /* when immobile, count is in turns */
                    if (multi < 0) {
                        if (++multi == 0) { /* finished yet? */
                            unmul((char *) 0);
                            /* if unmul caused a level change, take it now */
                            if (u.utotype)
                                deferred_goto();
                        }
                    }

                    /* Running around in Gehennom without 100% fire resistance */
                    if (Inhell && !Is_valley(&u.uz))
                        in_hell_effects();

                    /* If wielding/wearing any of the 'banes, make those
                       monsters that they are against hostile should they
                       be tame or peaceful */
     
                    if (uwep || u.twoweap) {
                        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
                            s_level *slev = Is_special(&u.uz);
                            boolean is_shkp = has_eshk(mtmp) && inhishop(mtmp);
                            boolean is_prst = has_epri(mtmp) && inhistemple(mtmp);
                            boolean town_peace = (slev && slev->flags.town);
                            boolean uwep_hates = mon_has_race(mtmp, spec_mh(uwep));
                            boolean uswap_hates = mon_has_race(mtmp, spec_mh(uswapwep));
                            if (!uwep_hates && !uswap_hates)
                                continue;
                            if (!(mtmp->mcansee && m_canseeu(mtmp)))
                                continue;
                            /* Towns are fairly safe zones - keep them that way. */
                            if (town_peace)
                                continue;
                            /* coaligned temple priests will stay civil */
                            if (is_prst && p_coaligned(mtmp))
                                continue;
                            
                            if (is_shkp)
                                ESHK(mtmp)->pbanned = TRUE;
                            
                            else if (mtmp->mpeaceful || mtmp->mtame) {
                                mtmp->mpeaceful = mtmp->mtame = 0;
                                pline("%s %s the %s in your %s...", Monnam(mtmp),
                                       rn2(2) ? "strongly reacts to" : "is aggravated by",
                                       uwep_hates ? xname(uwep) : xname(uswapwep),
                                       body_part(HAND));
                                setmangry(mtmp, FALSE);
                            }
                        }
                    }
                }
            } while (u.umovement < NORMAL_SPEED); /* hero can't move */

            /******************************************/
            /* once-per-hero-took-time things go here */
            /******************************************/

#ifdef STATUS_HILITES
            if (iflags.hilite_delta)
                status_eval_next_unhilite();
#endif
            if (context.bypasses)
                clear_bypasses();
            if (moves >= context.seer_turn) {
                if ((u.uhave.amulet || Clairvoyant) && !In_endgame(&u.uz)
                    && !BClairvoyant)
                    do_vicinity_map((struct obj *) 0);
                /* we maintain this counter even when clairvoyance isn't
                   taking place; on average, go again 30 turns from now */
                context.seer_turn = moves + (long) rn1(31, 15); /*15..45*/
                /* [it used to be that on every 15th turn, there was a 50%
                   chance of farsight, so it could happen as often as every
                   15 turns or theoretically never happen at all; but when
                   a fast hero got multiple moves on that 15th turn, it
                   could actually happen more than once on the same turn!] */
            }
            /* [fast hero who gets multiple moves per turn ends up sinking
               multiple times per turn; is that what we really want?] */
            if (u.utrap && u.utraptype == TT_LAVA)
                sink_into_lava();
            /* when/if hero escapes from lava, he can't just stay there */
            else if (!u.umoved)
                (void) pooleffects(FALSE);
            context.coward = FALSE;

            /* vision while buried or underwater is updated here */
            if (Underwater && !See_underwater) {
                under_water(0);
                docrt();
            } else if (Underwater) {
                docrt();
            } else if (u.uburied) {
                under_ground(0);
            }

        } /* actual time passed */

        /****************************************/
        /* once-per-player-input things go here */
        /****************************************/

        clear_splitobjs();
        find_ac();
        if (!context.mv || Blind) {
            /* redo monsters if hallu or wearing a helm of telepathy */
            if (Hallucination) { /* update screen randomly */
                see_monsters();
                see_objects();
                see_traps();
                if (u.uswallow)
                    swallowed(0);
            } else if (Unblind_telepat) {
                see_monsters();
            } else if (Warning || Warn_of_mon)
                see_monsters();

            if (vision_full_recalc)
                vision_recalc(0); /* vision! */
        }
        if (context.botl || context.botlx) {
            bot();
            curs_on_u();
        } else if (iflags.time_botl) {
            timebot();
            curs_on_u();
        }

        if (elf_regen != elf_can_regen()) {
            if (!Hallucination)
                You_feel("%s.", (elf_regen) ? "itchy" : "relief");
            else
                You_feel("%s.", (elf_can_regen()) ? "magnetic"
                                                  : "like... gnarly dude");
            elf_regen = elf_can_regen();
        }

        if (orc_regen != orc_can_regen()) {
            if (!Hallucination)
                You_feel("%s.", (orc_regen) ? "tingly" : "relief");
            else
                You_feel("%s.", (orc_can_regen()) ? "non-magnetic"
                                                  : "like... whoa");
            orc_regen = orc_can_regen();
        }

        if (vamp_regen != vamp_can_regen()) {
            if (!Hallucination)
                You_feel("%s.", (vamp_regen) ? "itchy" : "relief");
            else
                You_feel("%s.", (vamp_can_regen()) ? "semi-precious" 
                                            : "like you are no longer failing Organic Chemistry");
            
            vamp_regen = vamp_can_regen();
        }
        context.move = 1;

        if (multi >= 0 && occupation) {
#if defined(MICRO) || defined(WIN32)
            abort_lev = 0;
            if (kbhit()) {
                if ((ch = pgetchar()) == ABORT)
                    abort_lev++;
                else
                    pushch(ch);
            }
            if (!abort_lev && (*occupation)() == 0)
#else
            if ((*occupation)() == 0)
#endif
                occupation = 0;
            if (
#if defined(MICRO) || defined(WIN32)
                abort_lev ||
#endif
                monster_nearby()) {
                stop_occupation();
                reset_eat();
            }
#if defined(MICRO) || defined(WIN32)
            if (!(++occtime % 7))
                display_nhwindow(WIN_MAP, FALSE);
#endif
            continue;
        }

        if (iflags.sanity_check || iflags.debug_fuzzer)
            sanity_check();

#ifdef CLIPPING
        /* just before rhack */
        cliparound(u.ux, u.uy);
#endif

        u.umoved = FALSE;

        if (multi > 0) {
            lookaround();
            if (!multi) {
                /* lookaround may clear multi */
                context.move = 0;
                if (flags.time)
                    context.botl = TRUE;
                continue;
            }
            if (context.mv) {
                if (multi < COLNO && !--multi)
                    context.travel = context.travel1 = context.mv =
                        context.run = 0;
                domove();
            } else {
                --multi;
                rhack(save_cm);
            }
        } else if (multi == 0) {
#ifdef MAIL
            ckmailstatus();
#endif
            rhack((char *) 0);
        }
        if (u.utotype)       /* change dungeon level */
            deferred_goto(); /* after rhack() */
        /* !context.move here: multiple movement command stopped */
        else if (flags.time && (!context.move || !context.mv))
            context.botl = TRUE;

        if (vision_full_recalc)
            vision_recalc(0); /* vision! */
        /* when running in non-tport mode, this gets done through domove() */
        if ((!context.run || flags.runmode == RUN_TPORT)
            && (multi && (!context.travel ? !(multi % 7) : !(moves % 7L)))) {
            if (flags.time && context.run)
                context.botl = TRUE;
            /* [should this be flush_screen() instead?] */
            display_nhwindow(WIN_MAP, FALSE);
        }
    }
}

#define U_CAN_REGEN() (Regeneration || (Sleepy && u.usleep))

STATIC_OVL void
init_mchest()
{
    mchest = mksobj(HIDDEN_CHEST, FALSE, FALSE);
    mchest->olocked = 1;
    mchest->where = OBJ_SOMEWHERE;
    return;
}

/* maybe recover some lost health (or lose some when an eel out of water) */
STATIC_OVL void
regen_hp(wtcap)
int wtcap;
{
    int efflev, effcon, heal = 0;
    boolean reached_full = FALSE,
            encumbrance_ok = (wtcap < MOD_ENCUMBER || !u.umoved),
            infidel_no_amulet = (u.ualign.type == A_NONE && !u.uhave.amulet
                                 && !u.uachieve.amulet);

    /* periodically let our Infidel know why their hit
       points aren't regenerating if they don't have
       the Amulet in their possession */
    if (infidel_no_amulet && !rn2(20))
        You_feel("unable to rest or heal without the Amulet of Yendor.");

    if (Upolyd) {
        if (u.mh < 1) { /* shouldn't happen... */
            rehumanize();
        } else if (youmonst.data->mlet == S_EEL
                   && !is_damp_terrain(u.ux, u.uy) && !Is_waterlevel(&u.uz)) {
            /* eel out of water loses hp, similar to monster eels;
               as hp gets lower, rate of further loss slows down */
            if (u.mh > 1 && !Regeneration && rn2(u.mh) > rn2(8)
                && (!Half_physical_damage || !(moves % 2L)))
                heal = -1;
        } else if (u.mh < u.mhmax) {
            if ((!Is_valley(&u.uz) || is_undead(youmonst.data))
                && (U_CAN_REGEN() || (encumbrance_ok && !(moves % 20L))))
                heal = 1;
        }
        if (heal && !(Withering && heal > 0)) {
            context.botl = TRUE;
            u.mh += heal;
            reached_full = (u.mh == u.mhmax);
        }

    /* !Upolyd */
    } else {
        /* [when this code was in-line within moveloop(), there was
           no !Upolyd check here, so poly'd hero recovered lost u.uhp
           once u.mh reached u.mhmax; that may have been convenient
           for the player, but it didn't make sense for gameplay...] */
        if (u.uhp < u.uhpmax 
            && elf_can_regen() 
            && orc_can_regen() 
            && vamp_can_regen()
            && (!uarmc || (uarmc && uarmc->otyp != POISONOUS_CLOAK))
            && (encumbrance_ok || U_CAN_REGEN()) && !Is_valley(&u.uz)
            && !infidel_no_amulet) {
            /*
            * KMH, balance patch -- New regeneration code
            * Healthstones have been added, which alter your effective
            * experience level and constitution (-2 cursed, +1 uncursed,
            * +2 blessed) for the basis of regeneration calculations.
            */
            
            efflev = u.ulevel + u.uhealbonus;
            effcon = ACURR(A_CON) + u.uhealbonus;
            heal = 0;
            
            if (efflev > 9) {
                if (!(moves % 3L)) {
                    if (effcon <= 12) {
                        heal = 1;
                    } else {
                        heal = rnd(effcon);
                        if (heal > efflev - 9)
                            heal = efflev - 9;
                    }
                }
            }  
            /* Cursed healthstones might bring our efflev below 0. */
            else if (efflev < 0) {
                if (moves % (long) (22 + (abs(efflev) * 4)) == 0)
                    heal = 1;
            } else {
                if (moves % (long) ((MAXULEV + 12) / (efflev + 2) + 1) == 0)
                    heal = 1;
            }

            /* tortles gain some accelerated regeneration while
               inside their shell */
            if (Hidinshell && !U_CAN_REGEN()) {
                if (!rn2(5))
                    heal += 1;
            }

            if (U_CAN_REGEN() && !heal)
                heal += 1;
            if (Sleepy && u.usleep)
                heal++;
            
            if (!u.uinvulnerable && u.uen > 0 && u.uhp < u.uhpmax 
                && tech_inuse(T_CHI_HEALING)) {
                u.uen--;
                heal++;
            }
            if (heal && !(Withering && heal > 0)) {
                context.botl = TRUE;
                u.uhp += heal;
                if (u.uhp > u.uhpmax)
                    u.uhp = u.uhpmax;
                /* stop voluntary multi-turn activity if now fully healed */
                reached_full = (u.uhp == u.uhpmax);
            }
        }
    }

    if (reached_full)
        interrupt_multi("You are in full health.");
}

#undef U_CAN_REGEN

void
stop_occupation()
{
    if (occupation) {
        if (!maybe_finished_meal(TRUE)) {
            You("stop %s.", occtxt);
            update_inventory(); /* meal weight has changed */
        }
        occupation = 0;
        context.botl = TRUE; /* in case u.uhs changed */
        nomul(0);
        pushch(0);
    } else if (multi >= 0) {
        nomul(0);
    }
}

void
display_gamewindows()
{
    WIN_MESSAGE = create_nhwindow(NHW_MESSAGE);
    if (VIA_WINDOWPORT()) {
        status_initialize(0);
    } else {
        WIN_STATUS = create_nhwindow(NHW_STATUS);
    }
    WIN_MAP = create_nhwindow(NHW_MAP);
    WIN_INVEN = create_nhwindow(NHW_MENU);
    /* in case of early quit where WIN_INVEN could be destroyed before
       ever having been used, use it here to pacify the Qt interface */
    start_menu(WIN_INVEN), end_menu(WIN_INVEN, (char *) 0);

#ifdef MAC
    /* This _is_ the right place for this - maybe we will
     * have to split display_gamewindows into create_gamewindows
     * and show_gamewindows to get rid of this ifdef...
     */
    if (!strcmp(windowprocs.name, "mac"))
        SanePositions();
#endif

    /*
     * The mac port is not DEPENDENT on the order of these
     * displays, but it looks a lot better this way...
     */
#ifndef STATUS_HILITES
    display_nhwindow(WIN_STATUS, FALSE);
#endif
    display_nhwindow(WIN_MESSAGE, FALSE);
    clear_glyph_buffer();
    display_nhwindow(WIN_MAP, FALSE);
}

void
newgame()
{
    int i;
    /* Add this flag here to activate rr fuzzing */
    /* iflags.debug_fuzzer = 1; */
    
#ifdef MFLOPPY
    gameDiskPrompt();
#endif

    context.botlx = TRUE;
    context.ident = 1;
    context.stethoscope_move = -1L;
    context.warnlevel = 1;
    context.next_attrib_check = 600L; /* arbitrary first setting */
    context.tribute.enabled = TRUE;   /* turn on 3.6 tributes    */
    context.tribute.tributesz = sizeof(struct tribute_info);
    context.inf_aligns = rn2(6);      /* randomness for the Infidel role */
    context.next_moloch_offering = 6000; /* give a grace period before
                                          * the first sacrifice */

    /* Extra entropy added to sysopt.serverseed */
    sysopt.serverseed += rn2(8000000);

    for (i = LOW_PM; i < NUMMONS; i++)
        mvitals[i].mvflags = mons[i].geno & G_NOCORPSE;

    init_objects(); /* must be before u_init() */

    flags.pantheon = -1; /* role_init() will reset this */
    role_init();         /* must be before init_dungeons(), u_init(),
                          * and init_artifacts() */

    init_dungeons();  /* must be before u_init() to avoid rndmonst()
                       * creating odd monsters for any tins and eggs
                       * in hero's initial inventory */
    init_artifacts(); /* before u_init() in case $WIZKIT specifies
                       * any artifacts */
    u_init();

    shambler_init();
    dragon_init();

#ifndef NO_SIGNAL
    (void) signal(SIGINT, (SIG_RET_TYPE) done1);
#endif
#ifdef NEWS
    if (iflags.news)
        display_file(NEWS, FALSE);
#endif
    load_qtlist();          /* load up the quest text info */
    /* quest_init();  --  Now part of role_init() */

    mklev();
    u_on_upstairs();
    if (wizard)
        obj_delivery(FALSE); /* finish wizkit */
    vision_reset();          /* set up internals for level (after mklev) */
    check_special_room(FALSE);

    if (MON_AT(u.ux, u.uy))
        mnexto(m_at(u.ux, u.uy));
    (void) makedog();
    docrt();

    if (Role_if(PM_CONVICT)) {
        setworn(mkobj(CHAIN_CLASS, TRUE), W_CHAIN);
        setworn(mkobj(BALL_CLASS, TRUE), W_BALL);
        uball->spe = 1;
        placebc();
        newsym(u.ux, u.uy);
    }

    if (flags.legacy) {
        flush_screen(1);
        if (Role_if(PM_CONVICT)) {
            com_pager(199);
        } else {
            com_pager(1);
        }
    }

    urealtime.realtime = 0L;
    urealtime.start_timing = getnow();
#ifdef INSURANCE
    save_currentstate();
#endif
    program_state.something_worth_saving++; /* useful data now exists */

    /* Success! */
    welcome(TRUE);
    return;
}

/* show "welcome [back] to nethack" message at program startup */
void
welcome(new_game)
boolean new_game; /* false => restoring an old game */
{
    char buf[BUFSZ];
    boolean currentgend = Upolyd ? u.mfemale : flags.female;

    /* skip "welcome back" if restoring a doomed character */
    if (!new_game && Upolyd && ugenocided()) {
        /* death via self-genocide is pending */
        pline("You're back, but you still feel %s inside.", udeadinside());
        return;
    }

    /*
     * The "welcome back" message always describes your innate form
     * even when polymorphed or wearing a helm of opposite alignment.
     * Alignment is shown unconditionally for new games; for restores
     * it's only shown if it has changed from its original value.
     * Sex is shown for new games except when it is redundant; for
     * restores it's only shown if different from its original value.
     */
    *buf = '\0';
    if (new_game || u.ualignbase[A_ORIGINAL] != u.ualignbase[A_CURRENT])
        Sprintf(eos(buf), " %s", align_str(u.ualignbase[A_ORIGINAL]));
    if (!urole.name.f
        && (new_game
                ? (urole.allow & ROLE_GENDMASK) == (ROLE_MALE | ROLE_FEMALE)
                : currentgend != flags.initgend))
        Sprintf(eos(buf), " %s", genders[currentgend].adj);

    Sprintf(eos(buf), " %s %s", urace.adj,
            (currentgend && urole.name.f) ? urole.name.f : urole.name.m);
    
    if (u.ualign.type == A_NONE)
        pline(new_game ? "%s %s, welcome to Hack'EM!  You are an%s."
                       : "%s %s, the%s, welcome back to Hack'EM!",
              Hello((struct monst *) 0), plname, buf);
    else
        pline(new_game ? "%s %s, welcome to Hack'EM!  You are a%s."
                       : "%s %s, the%s, welcome back to Hack'EM!",
              Hello((struct monst *) 0), plname, buf);
}

#ifdef POSITIONBAR
STATIC_DCL void
do_positionbar()
{
    static char pbar[COLNO];
    char *p;

    p = pbar;
    /* up stairway */
    if (upstair.sx
        && (glyph_to_cmap(level.locations[upstair.sx][upstair.sy].glyph)
                == S_upstair
            || glyph_to_cmap(level.locations[upstair.sx][upstair.sy].glyph)
                   == S_upladder)) {
        *p++ = '<';
        *p++ = upstair.sx;
    }
    if (sstairs.sx
        && (glyph_to_cmap(level.locations[sstairs.sx][sstairs.sy].glyph)
                == S_upstair
            || glyph_to_cmap(level.locations[sstairs.sx][sstairs.sy].glyph)
                   == S_upladder)) {
        *p++ = '<';
        *p++ = sstairs.sx;
    }

    /* down stairway */
    if (dnstair.sx
        && (glyph_to_cmap(level.locations[dnstair.sx][dnstair.sy].glyph)
                == S_dnstair
            || glyph_to_cmap(level.locations[dnstair.sx][dnstair.sy].glyph)
                   == S_dnladder)) {
        *p++ = '>';
        *p++ = dnstair.sx;
    }
    if (sstairs.sx
        && (glyph_to_cmap(level.locations[sstairs.sx][sstairs.sy].glyph)
                == S_dnstair
            || glyph_to_cmap(level.locations[sstairs.sx][sstairs.sy].glyph)
                   == S_dnladder)) {
        *p++ = '>';
        *p++ = sstairs.sx;
    }

    /* hero location */
    if (u.ux) {
        *p++ = '@';
        *p++ = u.ux;
    }
    /* fence post */
    *p = 0;

    update_positionbar(pbar);
}
#endif

STATIC_DCL void
interrupt_multi(msg)
const char *msg;
{
    if (multi > 0 && !context.travel && !context.run) {
        nomul(0);
        if (flags.verbose && msg)
            Norep("%s", msg);
    }
}

/*
 * Argument processing helpers - for xxmain() to share
 * and call.
 *
 * These should return TRUE if the argument matched,
 * whether the processing of the argument was
 * successful or not.
 *
 * Most of these do their thing, then after returning
 * to xxmain(), the code exits without starting a game.
 *
 */

static struct early_opt earlyopts[] = {
    {ARG_DEBUG, "debug", 5, TRUE},
    {ARG_VERSION, "version", 4, TRUE},
    {ARG_SHOWPATHS, "showpaths", 9, FALSE},
    {ARG_LISTROLES, "listroles", 9, FALSE},
#ifdef WIN32
    {ARG_WINDOWS, "windows", 4, TRUE},
#endif
};

#ifdef WIN32
extern int FDECL(windows_early_options, (const char *));
#endif

/*
 * Returns:
 *    0 = no match
 *    1 = found and skip past this argument
 *    2 = found and trigger immediate exit
 */

int
argcheck(argc, argv, e_arg)
int argc;
char *argv[];
enum earlyarg e_arg;
{
    int i, idx;
    boolean match = FALSE;
    char *userea = (char *)0;
    const char *dashdash = "";

    for (idx = 0; idx < SIZE(earlyopts); idx++) {
        if (earlyopts[idx].e == e_arg)
            break;
    }
    if ((idx >= SIZE(earlyopts)) || (argc <= 1))
            return FALSE;

    for (i = 0; i < argc; ++i) {
        if (argv[i][0] != '-')
            continue;
        if (argv[i][1] == '-') {
            userea = &argv[i][2];
            dashdash = "-";
        } else {
            userea = &argv[i][1];
        }
        match = match_optname(userea, earlyopts[idx].name,
                              earlyopts[idx].minlength,
                              earlyopts[idx].valallowed);
        if (match) break;
    }

    if (match) {
        const char *extended_opt = index(userea, ':');

        if (!extended_opt)
            extended_opt = index(userea, '=');
        switch(e_arg) {
        case ARG_DEBUG:
            if (extended_opt) {
                extended_opt++;
                debug_fields(extended_opt);
            }
            return 1;
        case ARG_VERSION: {
            boolean insert_into_pastebuf = FALSE;

            if (extended_opt) {
                extended_opt++;
                if (match_optname(extended_opt, "paste", 5, FALSE)) {
                    insert_into_pastebuf = TRUE;
                } else {
                    raw_printf(
                   "-%sversion can only be extended with -%sversion:paste.\n",
                               dashdash, dashdash);
                    return TRUE;
                }
            }
            early_version_info(insert_into_pastebuf);
            return 2;
        }
        case ARG_SHOWPATHS: {
            return 2;
        }
        case ARG_LISTROLES: {
            listroles();
            return 2;
        }
#ifdef WIN32
        case ARG_WINDOWS: {
            if (extended_opt) {
                extended_opt++;
                return windows_early_options(extended_opt);
            }
        }
#endif
        default:
            break;
        }
    };
    return FALSE;
}

/*
 * These are internal controls to aid developers with
 * testing and debugging particular aspects of the code.
 * They are not player options and the only place they
 * are documented is right here. No gameplay is altered.
 *
 * test             - test whether this parser is working
 * ttystatus        - TTY:
 * immediateflips   - WIN32: turn off display performance
 *                    optimization so that display output
 *                    can be debugged without buffering.
 */
STATIC_OVL void
debug_fields(opts)
const char *opts;
{
    char *op;
    boolean negated = FALSE;

    while ((op = index(opts, ',')) != 0) {
        *op++ = 0;
        /* recurse */
        debug_fields(op);
    }
    if (strlen(opts) > BUFSZ / 2)
        return;


    /* strip leading and trailing white space */
    while (isspace((uchar) *opts))
        opts++;
    op = eos((char *) opts);
    while (--op >= opts && isspace((uchar) *op))
        *op = '\0';

    if (!*opts) {
        /* empty */
        return;
    }
    while ((*opts == '!') || !strncmpi(opts, "no", 2)) {
        if (*opts == '!')
            opts++;
        else
            opts += 2;
        negated = !negated;
    }
    if (match_optname(opts, "test", 4, FALSE))
        iflags.debug.test = negated ? FALSE : TRUE;
#ifdef TTY_GRAPHICS
    if (match_optname(opts, "ttystatus", 9, FALSE))
        iflags.debug.ttystatus = negated ? FALSE : TRUE;
#endif
#ifdef WIN32
    if (match_optname(opts, "immediateflips", 14, FALSE))
        iflags.debug.immediateflips = negated ? FALSE : TRUE;
#endif
    return;
}

/* Calculate the "wishing pain" luck penalty for wishes */
int
wishluck()
{
    /* Don't let this affect the fuzzer */
    if (iflags.debug_fuzzer || wizard)
        return 0;
    if (u.uconduct.wishes < 3)
        return 0;
    
    /* For synchronizing monster spawn rates with wishes */
    update_monclock();
    
    /* Technically we can only get the player to -10, 
     * so we force them to carry a virtual cursed luckstone. */
    if (u.uconduct.wishes < 13)
        return -((u.uconduct.wishes - 2));
    else
        return -10;
}

STATIC_OVL void
pickup_spirits()
{
    /* Autopickup spirits for Necromancer */
    if (Role_if(PM_NECROMANCER)) {
        struct obj *obj;
        /* Carrying the necro quest arti triples the range */
        int off = carrying_arti(ART_GREAT_DAGGER_OF_GLAURGNAA) ? 3 : 1;
        /* Loop through the surrounding squares */
        int x, y;
        for (x = u.ux - off; x <= u.ux + off; x++) {
            for (y = u.uy - off; y <= u.uy + off; y++) {
                if (!isok(x, y) || !OBJ_AT(x, y))
                    continue;

                /* count the objects here */
                for (obj = level.objects[x][y]; obj; obj = obj->nexthere) {
                    if (obj->otyp == SPIRIT) {
                        /* Don't allow auto-pickup of spirits in shops unless
                         * the shop is abandoned. */
                        if (costly_spot(x, y))
                            continue;

                        pickup_object(obj, obj->quan, TRUE);
                        newsym_force(x, y);
                        break;
                    }
                }
            }
        }
    }
}

STATIC_OVL void 
update_monclock()
{
    /* From SporkHack & UnNetHack (modified)
     * Vanilla generates a critter every 70-ish turns.
     * The rate accelerates to every 50 or so below the Castle,
     * and 'round every 25 turns once you've done the Invocation.
     * 
     * (From EvilHack)
     * We will push it even further.  Monsters post-Invocation
     * will almost always appear on the stairs (if present), and
     * much more frequently; this, along with the extra intervene()
     * calls, should certainly make it seem like you're wading back
     * through the teeming hordes.
     *
     * Aside from that, a more general clock should be put on things;
     * after about 50,000 turns, the frequency rate of appearance
     * and difficulty of monsters generated will slowly increase until
     * it reaches the point it will be at as if you were post-Invocation.
     *
     * The rate increases linearly with turns.  The rule of thumb is that
     * at turn x the rate is approximately (x / 30.0000) times the normal
     * rate.  Maximal rate is 8x the normal rate.
     * -------------------------------------------------------------------
     * In Hack'EM - we are going to put Yet Another Twist on monster generation
     * and use it to "punish" wishes. For every wish the player makes, the 
     * rate will increase! This lets the player have their wishes at the cost
     * of a harder game later.
     */
	 int wishminus2 = u.uconduct.wishes - 2;
    monclock = MIN_MONGEN_RATE;

	if (wishminus2 > 0) {
/* Don't let wishes influence the fuzzer */
    if (!iflags.debug_fuzzer && !wizard) {
        for (long i = 2; i <= 7; i++) {
            if (monclock > MIN_MONGEN_RATE / i && (wishminus2 >= i)) {
                monclock = MIN_MONGEN_RATE / i;
            }
        }
        if (wishminus2 >= 8L) {
            monclock = MAX_MONGEN_RATE;
        }
    }
    /* make sure we don't fall off the bottom */
    if (monclock < MAX_MONGEN_RATE)
        monclock = MAX_MONGEN_RATE;
    if (monclock > MIN_MONGEN_RATE)
        monclock = MIN_MONGEN_RATE;
	}
}
/*allmain.c*/
