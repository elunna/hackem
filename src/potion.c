/* NetHack 3.6	potion.c	$NHDT-Date: 1573848199 2019/11/15 20:03:19 $  $NHDT-Branch: NetHack-3.6 $:$NHDT-Revision: 1.167 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2013. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"

boolean notonhead = FALSE;

static NEARDATA int nothing, unkn;
static NEARDATA const char beverages[] = { POTION_CLASS, TOOL_CLASS, 0 };
static NEARDATA const char dippables[] = { POTION_CLASS, 0 };

STATIC_DCL long FDECL(itimeout, (long));
STATIC_DCL long FDECL(itimeout_incr, (long, int));
STATIC_DCL void NDECL(ghost_from_bottle);
STATIC_DCL boolean
FDECL(H2Opotion_dip, (struct obj *, struct obj *, BOOLEAN_P, const char *));
STATIC_DCL void FDECL(dipsink, (struct obj *));
STATIC_DCL void FDECL(quiet_unwear, (struct obj *));

/* force `val' to be within valid range for intrinsic timeout value */
STATIC_OVL long
itimeout(val)
long val;
{
    if (val >= TIMEOUT)
        val = TIMEOUT;
    else if (val < 1)
        val = 0;

    return val;
}

/* increment `old' by `incr' and force result to be valid intrinsic timeout */
STATIC_OVL long
itimeout_incr(old, incr)
long old;
int incr;
{
    return itimeout((old & TIMEOUT) + (long) incr);
}

/* set the timeout field of intrinsic `which' */
void
set_itimeout(which, val)
long *which, val;
{
    *which &= ~TIMEOUT;
    *which |= itimeout(val);
}

/* increment the timeout field of intrinsic `which' */
void
incr_itimeout(which, incr)
long *which;
int incr;
{
    set_itimeout(which, itimeout_incr(*which, incr));
}

/* increase a partial-resistance intrinsic by XX%
 * ...will automatically cap at 100% */
void
incr_resistance(which, incr)
long* which;
int incr;
{
    long oldval = *which & TIMEOUT;

    if (oldval + incr > 100)
        oldval = 100;
    else
        oldval += incr;

    *which &= ~TIMEOUT;
    *which |= (oldval | HAVEPARTIAL);
}

/* decrease a partial-resistance intrinsic by XX% */
void
decr_resistance(which, incr)
long* which;
int incr;
{
    long oldval = *which & TIMEOUT;

    if (oldval - incr < 0)
        oldval = 0;
    else
        oldval -= incr;

    *which &= ~TIMEOUT;
    *which |= (oldval | ((oldval < 1) ? 0 : HAVEPARTIAL));
}

/* Return percent which a player is resistant -- 100% if from external/race/etc. */
int
how_resistant(which)
int which;
{
    int val;

    /* externals and level/race based intrinsics always provide 100%
     * as do monster resistances */
    if (u.uprops[which].extrinsic
        || (u.uprops[which].intrinsic & (FROMEXPER | FROMRACE | FROMFORM))) {
        val = 100;
    } else {
        val = (u.uprops[which].intrinsic & TIMEOUT);
        if (val > 100) {
            val = 100;
            u.uprops[which].intrinsic &= ~TIMEOUT;
            u.uprops[which].intrinsic |= (val | HAVEPARTIAL);
        }
    }

    /* vulnerability will affect things... */
    switch (which) {
    case FIRE_RES:
        if (Vulnerable_fire)
            val -= 50;
        break;
    case COLD_RES:
        if (Vulnerable_cold)
            val -= 50;
        break;
    case SHOCK_RES:
        if (Vulnerable_elec)
            val -= 50;
        break;
    case ACID_RES:
        if (Vulnerable_acid)
            val -= 50;
        break;
    default:
        break;
    }
    return val;
}

/* Handles the damage-reduction shuffle necessary to convert 80% resistance
 * into 20% damage (and keeps the floating-point silliness out of the main lines) */
int
resist_reduce(amount, which)
int amount, which;
{
    float tmp = 100.0f - (float) how_resistant(which);

    tmp /= 100;
    /* debug line */
    /*pline("incoming: %d  outgoing: %d",
            amount, (int) ((float) amount * tmp));*/
    return (int) ((float) amount * tmp);
}

void
make_confused(xtime, talk)
long xtime;
boolean talk;
{
    long old = HConfusion;

    if (Unaware)
        talk = FALSE;

    if (!xtime && old) {
        if (talk)
            You_feel("less %s now.", Hallucination ? "trippy" : "confused");
    }
    if ((xtime && !old) || (!xtime && old))
        context.botl = TRUE;

    set_itimeout(&HConfusion, xtime);
}

void
make_stunned(xtime, talk)
long xtime;
boolean talk;
{
    long old = HStun;

    if (Unaware)
        talk = FALSE;

    if (!xtime && old) {
        if (talk)
            You_feel("%s now.",
                     Hallucination ? "less wobbly" : "a bit steadier");
    }
    if (xtime && !old) {
        if (Stun_resistance) {
            You_feel("a slight itch.");
            return;
        }
        if (talk) {
            if (u.usteed)
                You("wobble in the saddle.");
            else
                You("%s...", stagger(youmonst.data, "stagger"));
        }
    }
    if ((!xtime && old) || (xtime && !old))
        context.botl = TRUE;

    set_itimeout(&HStun, xtime);
}

void
make_afraid(xtime, talk)
long xtime;
boolean talk;
{
    long old = HAfraid;

    if (Unaware)
        talk = FALSE;

    if (!xtime && old) {
        if (talk)
            You_feel("less %s now.", Hallucination ? "frazzled" : "afraid");
    }

    if (xtime && !old) {
        if (talk && u.fearedmon) {
            You("are afraid of %s!", mon_nam(u.fearedmon));
        } else if (talk) {
            You_feel("afraid!");
        }
    }
    if ((xtime && !old) || (!xtime && old))
        context.botl = TRUE;

    set_itimeout(&HAfraid, xtime);
}

void
make_fearless(xtime, talk)
long xtime;
boolean talk;
{
    long old = HFearless;

    if (Unaware)
        talk = FALSE;

    if (!xtime && old) {
        if (talk)
            You_feel("less %s now.", Hallucination ? "heroic" : "brave");
    }

    if (xtime && !old) {
        if (talk && Afraid) {
            You_feel("brave!");
        }
        make_afraid(0L, TRUE);
    }

    if ((xtime && !old) || (!xtime && old))
        context.botl = TRUE;

    set_itimeout(&HFearless, xtime);
}

/* Sick is overloaded with both fatal illness and food poisoning (via
   u.usick_type bit mask), but delayed killer can only support one or
   the other at a time.  They should become separate intrinsics.... */
void
make_sick(xtime, cause, talk, type)
long xtime;
const char *cause; /* sickness cause */
boolean talk;
int type;
{
    struct kinfo *kptr;
    long old = Sick;

#if 0   /* tell player even if hero is unconscious */
    if (Unaware)
        talk = FALSE;
#endif
    if (xtime > 0L) {
        if (Sick_resistance)
            return;
        if (!old) {
            /* newly sick */
            You_feel("deathly sick.");
        } else {
            /* already sick */
            if (talk)
                You_feel("%s worse.", xtime <= Sick / 2L ? "much" : "even");
        }
        set_itimeout(&Sick, xtime);
        u.usick_type |= type;
        context.botl = TRUE;
    } else if (old && (type & u.usick_type)) {
        /* was sick, now not */
        u.usick_type &= ~type;
        if (u.usick_type) { /* only partly cured */
            if (talk)
                You_feel("somewhat better.");
            set_itimeout(&Sick, Sick * 2); /* approximation */
        } else {
            if (talk)
                You_feel("cured.  What a relief!");
            Sick = 0L; /* set_itimeout(&Sick, 0L) */
        }
        context.botl = TRUE;
    }

    kptr = find_delayed_killer(SICK);
    if (Sick) {
        exercise(A_CON, FALSE);
        /* setting delayed_killer used to be unconditional, but that's
           not right when make_sick(0) is called to cure food poisoning
           if hero was also fatally ill; this is only approximate */
        if (xtime || !old || !kptr) {
            int kpfx = ((cause && !strcmp(cause, "#wizintrinsic"))
                        ? KILLED_BY : KILLED_BY_AN);

            delayed_killer(SICK, kpfx, cause);
        }
    } else
        dealloc_killer(kptr);
}

void
make_carrier(xtime, talk)
long xtime;
boolean talk;
{
    long old = LarvaCarrier;

    if (Unaware)
        talk = FALSE;

    set_itimeout(&LarvaCarrier, xtime);
    context.botl = TRUE;
    if (!xtime && old)
        if (talk)
            You_feel("much more yourself.");
}

void
make_slimed(xtime, msg)
long xtime;
const char *msg;
{
    long old = Slimed;

#if 0   /* tell player even if hero is unconscious */
    if (Unaware)
        msg = 0;
#endif
    set_itimeout(&Slimed, xtime);
    if ((xtime != 0L) ^ (old != 0L)) {
        context.botl = TRUE;
        if (msg)
            pline("%s", msg);
    }
    if (!Slimed) {
        dealloc_killer(find_delayed_killer(SLIMED));
        /* fake appearance is set late in turn-to-slime countdown */
        if (U_AP_TYPE == M_AP_MONSTER
            && youmonst.mappearance == PM_GREEN_SLIME) {
            youmonst.m_ap_type = M_AP_NOTHING;
            youmonst.mappearance = 0;
        }
    }
}

/* start or stop petrification */
void
make_stoned(xtime, msg, killedby, killername)
long xtime;
const char *msg;
int killedby;
const char *killername;
{
    long old = Stoned;

#if 0   /* tell player even if hero is unconscious */
    if (Unaware)
        msg = 0;
#endif
    set_itimeout(&Stoned, xtime);
    if ((xtime != 0L) ^ (old != 0L)) {
        context.botl = TRUE;
        if (msg)
            pline("%s", msg);
    }
    if (!Stoned)
        dealloc_killer(find_delayed_killer(STONED));
    else if (!old)
        delayed_killer(STONED, killedby, killername);
}

void
make_vomiting(xtime, talk)
long xtime;
boolean talk;
{
    long old = Vomiting;

    if (Unaware)
        talk = FALSE;

    set_itimeout(&Vomiting, xtime);
    context.botl = TRUE;
    if (!xtime && old)
        if (talk)
            You_feel("much less nauseated now.");
}

static const char vismsg[] = "vision seems to %s for a moment but is %s now.";
static const char eyemsg[] = "%s momentarily %s.";

void
make_blinded(xtime, talk)
long xtime;
boolean talk;
{
    long old = Blinded;
    boolean u_could_see, can_see_now;
    const char *eyes;

    /* we need to probe ahead in case the Eyes of the Overworld
       are or will be overriding blindness */
    u_could_see = !Blind;
    Blinded = xtime ? 1L : 0L;
    can_see_now = !Blind;
    Blinded = old; /* restore */

    if (Unaware)
        talk = FALSE;

    if (can_see_now && !u_could_see) { /* regaining sight */
        if (talk) {
            if (Hallucination)
                pline("Far out!  Everything is all cosmic again!");
            else
                You("can see again.");
        }
    } else if (old && !xtime) {
        /* clearing temporary blindness without toggling blindness */
        if (talk) {
            if (!haseyes(youmonst.data)) {
                strange_feeling((struct obj *) 0, (char *) 0);
            } else if (Blindfolded) {
                eyes = body_part(EYE);
                if (eyecount(youmonst.data) != 1)
                    eyes = makeplural(eyes);
                Your(eyemsg, eyes, vtense(eyes, "itch"));
            } else { /* Eyes of the Overworld */
                Your(vismsg, "brighten", Hallucination ? "sadder" : "normal");
            }
        }
    }

    if (u_could_see && !can_see_now) { /* losing sight */
        if (talk) {
            if (Hallucination)
                pline("Oh, bummer!  Everything is dark!  Help!");
            else
                pline("A cloud of darkness falls upon you.");
        }
        /* Before the hero goes blind, set the ball&chain variables. */
        if (Punished)
            set_bc(0);
    } else if (!old && xtime) {
        /* setting temporary blindness without toggling blindness */
        if (talk) {
            if (!haseyes(youmonst.data)) {
                strange_feeling((struct obj *) 0, (char *) 0);
            } else if (Blindfolded) {
                eyes = body_part(EYE);
                if (eyecount(youmonst.data) != 1)
                    eyes = makeplural(eyes);
                Your(eyemsg, eyes, vtense(eyes, "twitch"));
            } else { /* Eyes of the Overworld */
                Your(vismsg, "dim", Hallucination ? "happier" : "normal");
            }
        }
    }

    set_itimeout(&Blinded, xtime);

    if (u_could_see ^ can_see_now) { /* one or the other but not both */
        toggle_blindness();
    }
}

/* blindness has just started or just ended--caller enforces that;
   called by Blindf_on(), Blindf_off(), and make_blinded() */
void
toggle_blindness()
{
    /* blindness has just been toggled */
    context.botl = TRUE; /* status conditions need update */
    vision_full_recalc = 1; /* vision has changed */
    /* this vision recalculation used to be deferred until moveloop(),
       but that made it possible for vision irregularities to occur
       (cited case was force bolt hitting an adjacent potion of blindness
       and then a secret door; hero was blinded by vapors but then got the
       message "a door appears in the wall" because wall spot was IN_SIGHT) */
    vision_recalc(0);
    if (Blind_telepat || Infravision || context.warntype.obj)
        see_monsters(); /* also counts EWarn_of_mon monsters */
    /*
     * Avoid either of the sequences
     * "Sting starts glowing", [become blind], "Sting stops quivering" or
     * "Sting starts quivering", [regain sight], "Sting stops glowing"
     * by giving "Sting is quivering" when becoming blind or
     * "Sting is glowing" when regaining sight so that the eventual
     * "stops" message matches the most recent "Sting is ..." one.
     */
    blind_glow_warnings();
    /* update dknown flag for inventory picked up while blind */
    if (!Blind)
        learn_unseen_invent();
}

boolean
make_hallucinated(xtime, talk, mask)
long xtime; /* nonzero if this is an attempt to turn on hallucination */
boolean talk;
long mask; /* nonzero if resistance status should change by mask */
{
    long old = Hallucination;
    boolean changed = 0;
    const char *message, *verb;

    if (Unaware)
        talk = FALSE;

    message = (!xtime) ? "Everything %s SO boring now."
                       : "Oh wow!  Everything %s so cosmic!";
    verb = (!Blind) ? "looks" : "feels";

    if (mask) {
        if (HHallucination)
            changed = TRUE;

        if (!xtime)
            EHalluc_resistance |= mask;
        else
            EHalluc_resistance &= ~mask;
    } else {
        if (!EHalluc_resistance && (!!HHallucination != !!xtime))
            changed = TRUE;
        set_itimeout(&HHallucination, xtime);

        /* clearing temporary hallucination without toggling vision */
        if (!changed && !HHallucination && old && talk) {
            if (!haseyes(youmonst.data)) {
                strange_feeling((struct obj *) 0, (char *) 0);
            } else if (Blind) {
                const char *eyes = body_part(EYE);

                if (eyecount(youmonst.data) != 1)
                    eyes = makeplural(eyes);
                Your(eyemsg, eyes, vtense(eyes, "itch"));
            } else { /* Grayswandir */
                Your(vismsg, "flatten", "normal");
            }
        }
    }
    if (changed)
        post_hallucination();
    if (talk && (old != Hallucination))
        pline(message, verb);
    return changed;
}

void
post_hallucination()
{
    /* in case we're mimicking an orange (hallucinatory form
       of mimicking gold) update the mimicking's-over message */
    if (!Hallucination)
        eatmupdate();

    if (u.uswallow) {
        swallowed(0); /* redraw swallow display */
    } else {
        /* The see_* routines should be called *before* the pline. */
        see_monsters();
        see_objects();
        see_traps();
    }

    /* for perm_inv and anything similar
    (eg. Qt windowport's equipped items display) */
    update_inventory();

    context.botl = TRUE;
}

void
make_deaf(xtime, talk)
long xtime;
boolean talk;
{
    long old = HDeaf;
    boolean can_hear_now;

    if (Unaware)
        talk = FALSE;

    set_itimeout(&HDeaf, xtime);
    can_hear_now = !Deaf;
    if ((xtime != 0L) ^ (old != 0L)) {
        context.botl = TRUE;
        if (talk) {
            if (old && can_hear_now)
                You("can hear again.");
            else
                You("are unable to hear anything.");
        }
    }
}

/* set or clear "slippery fingers" */
void
make_glib(xtime)
int xtime;
{
    set_itimeout(&Glib, xtime);
    /* may change "(being worn)" to "(being worn; slippery)" or vice versa */
    if (uarmg)
        update_inventory();
}

void
self_invis_message()
{
    pline("%s %s.",
          Hallucination ? "Far out, man!  You"
                        : "Gee!  All of a sudden, you",
          See_invisible ? "can see right through yourself"
                        : "can't see yourself");
}

STATIC_OVL void
ghost_from_bottle()
{
    struct monst *mtmp = makemon(&mons[PM_GHOST], u.ux, u.uy, NO_MM_FLAGS);

    if (!mtmp) {
        pline("This bottle turns out to be empty.");
        return;
    }
    if (!canspotmon(mtmp)) {
        pline("As you open the bottle, %s emerges.", something);
        return;
    }
    pline("As you open the bottle, an enormous %s emerges!",
          Hallucination ? rndmonnam(NULL) : (const char *) "ghost");
    scary_ghost(mtmp);
}

/* "Quaffing is like drinking, except you spill more." - Terry Pratchett */
int
dodrink()
{
    register struct obj *otmp;

    if (Hidinshell) {
        You_cant("drink anything while hiding in your shell.");
        return 0;
    }
    if (Strangled) {
        pline("If you can't breathe air, how can you drink liquid?");
        return 0;
    }
    if (uarmh && uarmh->otyp == PLASTEEL_HELM && cursed(uarmh, TRUE)) {
        pline_The("%s covers your whole face.", xname(uarmh));
        return 0;
    }
    
    /* Is there a fountain to drink from here? */
    if (IS_FOUNTAIN(levl[u.ux][u.uy].typ)
        /* not as low as floor level but similar restrictions apply */
        && can_reach_floor(FALSE)) {
        if (yn("Drink from the fountain?") == 'y') {
            drinkfountain();
            return 1;
        }
    }
    /* Or a kitchen sink? */
    if (IS_SINK(levl[u.ux][u.uy].typ)
        /* not as low as floor level but similar restrictions apply */
        && can_reach_floor(FALSE)) {
        if (yn("Drink from the sink?") == 'y') {
            drinksink();
            return 1;
        }
    }
    /* Or a toilet? (Yuck) */
    if (IS_TOILET(levl[u.ux][u.uy].typ)
        /* not as low as floor level but similar restrictions apply */
        && can_reach_floor(FALSE)) {
        if (yn("Drink from the toilet?") == 'y') {
            drinktoilet();
            return 1;
        }
    }
    /* Or a forge? */
    if (IS_FORGE(levl[u.ux][u.uy].typ)
        /* not as low as floor level but similar restrictions apply */
        && can_reach_floor(FALSE)) {
        if (yn("Drink from the forge?") == 'y') {
            drinkforge();
            return 1;
        }
    }
    /* Or are you surrounded by water? */
    if ((((is_puddle(u.ux, u.uy) || is_sewage(u.ux, u.uy))
        && !verysmall(youmonst.data))
        || (is_pool(u.ux, u.uy) && Wwalking))
        && !u.uswallow && can_reach_floor(FALSE)) {
        boolean on_sewage = is_sewage(u.ux, u.uy);
        char prompt[BUFSZ];
        const char *liq;
        liq = hliquid(on_sewage ? "sewage" : "water");
        Sprintf(prompt, "Drink the %s beneath you?", liq);
        if (yn(prompt) == 'y') {
            if (on_sewage) {
                if (Hallucination
                    || yn("Do you really want to drink raw sewage?") == 'y') {
                    pline("This %s is foul!", liq);
                    if (how_resistant(SICK_RES) == 100) {
                        You_feel("mildly nauseous.");
                        losehp(rnd(4), "upset stomach", KILLED_BY_AN);
                    }
                    losestr(resist_reduce(rn1(4, 3), SICK_RES));
                    losehp(resist_reduce(rnd(10), SICK_RES),
                           "contaminated sewage", KILLED_BY);
                    exercise(A_CON, FALSE);
                    return 1;
                }
            } else {
                pline("Do you know what lives in that water?");
            }
            return 1;
        }
    } else if ((Underwater
               || ((is_puddle(u.ux, u.uy) || is_sewage(u.ux, u.uy))
                   && verysmall(youmonst.data)))
               && !u.uswallow) {
        if (yn("Drink the water around you?") == 'y') {
            pline("Do you know what lives in this water?");
            return 1;
        }
    }

    otmp = getobj(beverages, "drink");
    if (!otmp)
        return 0;

    /* quan > 1 used to be left to useup(), but we need to force
       the current potion to be unworn, and don't want to do
       that for the entire stack when starting with more than 1.
       [Drinking a wielded potion of polymorph can trigger a shape
       change which causes hero's weapon to be dropped.  In 3.4.x,
       that led to an "object lost" panic since subsequent useup()
       was no longer dealing with an inventory item.  Unwearing
       the current potion is intended to keep it in inventory.] */
    if (otmp->quan > 1L) {
        otmp = splitobj(otmp, 1L);
        otmp->owornmask = 0L; /* rest of original stuck unaffected */
    } else if (otmp->owornmask) {
        remove_worn_item(otmp, FALSE);
    }
    otmp->in_use = TRUE; /* you've opened the stopper */

    
    if (otmp->otyp == KEG) {
        if (yn("Really drink the entire keg at once?") == 'n') {
            pline("Perhaps not.");
            return 0;
        }
        if (otmp->spe) {
            int quan = 0;
            while (otmp->spe) {
                /* u.uconduct.alcohol++; */
                consume_obj_charge(otmp, TRUE);
                check_unpaid(otmp);
                if (!otmp->cursed)
                    healup(1, 0, FALSE, FALSE);
                if (!otmp->blessed)
                    make_confused(itimeout_incr(HConfusion, d(3, 8)), FALSE);
                u.uhunger += 10 * (2 + bcsign(otmp));
                quan++;
            }
            You("down the entire keg! You are incredibly drunk!");
            if (quan > 5 && !maybe_polyd(is_dwarf(youmonst.data) || is_giant(youmonst.data), 
                Race_if(PM_DWARF))) {
                u.uhp = 0;
                losehp(1, "drinking too much booze", KILLED_BY);
            }
        } else {
            pline("Unfortunately, your keg is dry as a desert.");
            return 0;
        }
        return 1;
    } else if (otmp->oclass != POTION_CLASS) {
        pline(silly_thing_to, "drink");
        return 0;
    }

    if (objdescr_is(otmp, "milky")
        && !(mvitals[PM_GHOST].mvflags & G_GONE)
        && !rn2(POTION_OCCUPANT_CHANCE(mvitals[PM_GHOST].born))) {
        ghost_from_bottle();
        useup(otmp);
        return 1;
    } else if (objdescr_is(otmp, "smoky")
               && !(mvitals[PM_DJINNI].mvflags & G_GONE)
               && !rn2(POTION_OCCUPANT_CHANCE(mvitals[PM_DJINNI].born))) {
        djinni_from_bottle(otmp);
        useup(otmp);
        return 1;
    }
    return dopotion(otmp);
}

int
dopotion(otmp)
register struct obj *otmp;
{
    int retval;

    otmp->in_use = TRUE;
    nothing = unkn = 0;
    if ((retval = peffects(otmp)) >= 0)
        return retval;

    if (nothing) {
        unkn++;
        You("have a %s feeling for a moment, then it passes.",
            Hallucination ? "normal" : "peculiar");
    }
    if (otmp->dknown && !objects[otmp->otyp].oc_name_known) {
        if (!unkn) {
            makeknown(otmp->otyp);
            more_experienced(0, 10);
        } else if (!objects[otmp->otyp].oc_uname)
            docall(otmp);
    }
    useup(otmp);
    return 1;
}

int
peffects(otmp)
register struct obj *otmp;
{
    register int i, ii, lim;
    const char *mod;
    boolean happened;
    
    switch (otmp->otyp) {
    case POT_RESTORE_ABILITY:
        unkn++;
        if (otmp->cursed) {
            pline("Ulch!  This makes you feel mediocre!");
            break;
        } else {
            /* unlike unicorn horn, overrides Fixed_abil */
            pline("Wow!  This makes you feel %s!",
                  (otmp->blessed)
                      ? (unfixable_trouble_count(FALSE) ? "better" : "great")
                      : "good");
            i = rn2(A_MAX); /* start at a random point */
            for (ii = 0; ii < A_MAX; ii++) {
                lim = AMAX(i);
                /* this used to adjust 'lim' for A_STR when u.uhs was
                   WEAK or worse, but that's handled via ATEMP(A_STR) now */
                if (ABASE(i) < lim) {
                    ABASE(i) = lim;
                    context.botl = 1;
                    /* only first found if not blessed */
                    if (!otmp->blessed)
                        break;
                }
                if (++i >= A_MAX)
                    i = 0;
            }

            /* when using the potion (not the spell) also restore lost levels,
               to make the potion more worth keeping around for players with
               the spell or with a unihorn; this is better than full healing
               in that it can restore all of them, not just half, and a
               blessed potion restores them all at once */
            if (otmp->otyp == POT_RESTORE_ABILITY && u.ulevel < u.ulevelmax) {
                do {
                    pluslvl(FALSE);
                } while (u.ulevel < u.ulevelmax && otmp->blessed);
            }
        }
        break;
    case POT_HALLUCINATION:
        if (Halluc_resistance) {
            nothing++;
            break;
        } else if (Hallucination) {
            nothing++;
        }
        (void) make_hallucinated(itimeout_incr(HHallucination,
                                 rn1(otmp->odiluted ? 100 : 200, 600 - 300 * bcsign(otmp))),
                                 TRUE, 0L);
        if ((otmp->blessed && !rn2(3)) || (!otmp->cursed && !rn2(6))) {
            You("perceive yourself...");
            display_nhwindow(WIN_MESSAGE, FALSE);
            enlightenment(MAGICENLIGHTENMENT, ENL_GAMEINPROGRESS);
            Your("awareness re-normalizes.");
            exercise(A_WIS, TRUE);
        }
        break;
    case POT_AMNESIA:
        pline(Hallucination? "This tastes like champagne!" :
                "This liquid bubbles and fizzes as you drink it.");
        
        u.uhunger += 50 + rnd(50); /* You feel refreshed */
        newuhs(FALSE);
        
        if (otmp->blessed && ACURR(A_INT) > 12 && num_spells() > 0) {
            if (yn("Do you want to forget your last spell?") != 'y') {
                goto forget_routine;
            } else {
                forget_spell();
            }
        } else {
forget_routine:
            forget((!otmp->blessed ? ALL_SPELLS : 0) | ALL_MAP);

            if (Hallucination)
                pline("Hakuna matata!");
            else if (!Psychic_resistance)
                You_feel("your memories dissolve.");
        }
        
        /* Blessed amnesia makes you forget lycanthropy, sickness */
        if (otmp->blessed) {
            if (u.ulycn >= LOW_PM && !Race_if(PM_HUMAN_WEREWOLF)) {
                You("forget your affinity to %s!",
                                makeplural(mons[u.ulycn].mname));
                if (youmonst.data == &mons[u.ulycn])
                    you_unwere(FALSE);
                u.ulycn = NON_PM;	/* cure lycanthropy */
            }
            make_sick(0L, (char *) 0, TRUE, SICK_ALL);
        } else
            exercise(A_WIS, FALSE);
        
        /* We also forget we were afraid! */
        make_afraid(0L, TRUE);
        break;
    case POT_WATER:
        if (!otmp->blessed && !otmp->cursed) {
            pline("This tastes like %s.", hliquid("water"));
            u.uhunger += rnd(10);
            newuhs(FALSE);
            break;
        }
        unkn++;
        if (is_undead(youmonst.data) || is_demon(raceptr(&youmonst))
            || u.ualign.type <= A_CHAOTIC) {
            int dice = Uevil_inherently ? 4 : 2;
            if (otmp->blessed) {
                pline("This burns like %s!", hliquid("acid"));
                exercise(A_CON, FALSE);
                if (u.ulycn >= LOW_PM) {
                    Your("affinity to %s disappears!",
                         makeplural(mons[u.ulycn].mname));
                    if (youmonst.data == &mons[u.ulycn])
                        you_unwere(FALSE);
                    set_ulycn(NON_PM); /* cure lycanthropy */
                }
                losehp(Maybe_Half_Phys(d(dice, 6)), "potion of holy water",
                       KILLED_BY_AN);
            } else if (otmp->cursed) {
                You_feel("quite proud of yourself.");
                healup(d(dice, 6), 0, 0, 0);
                if (u.ulycn >= LOW_PM && !Upolyd)
                    you_were();
                exercise(A_CON, TRUE);
            }
        } else {
            if (otmp->blessed) {
                You_feel("full of awe.");
                make_sick(0L, (char *) 0, TRUE, SICK_ALL);
                exercise(A_WIS, TRUE);
                exercise(A_CON, TRUE);
                if (u.ulycn >= LOW_PM)
                    you_unwere(TRUE); /* "Purified" */
                /* make_confused(0L, TRUE); */
            } else {
                if (u.ualign.type == A_LAWFUL) {
                    pline("This burns like %s!", hliquid("acid"));
                    losehp(Maybe_Half_Phys(d(2, 6)), "potion of unholy water",
                           KILLED_BY_AN);
                } else
                    You_feel("full of dread.");
                if (u.ulycn >= LOW_PM && !Upolyd)
                    you_were();
                exercise(A_CON, FALSE);
            }
        }
        break;
    case POT_BOOZE:
        unkn++;
        pline("Ooph!  This tastes like %s%s!",
              otmp->odiluted ? "watered down " : "",
              Hallucination ? "dandelion wine" : "liquid fire");
        if (!otmp->blessed)
            make_confused(itimeout_incr(HConfusion, d(3, 8)), FALSE);
        /* the whiskey makes us feel better */
        if (!otmp->odiluted)
            healup(1, 0, FALSE, FALSE);
        /* u.uhunger += 10 * (2 + bcsign(otmp)); */
        u.uhunger += (otmp->odiluted ? 40 : 130) + 10 * (2 + bcsign(otmp));
        newuhs(FALSE);
        exercise(A_WIS, FALSE);
        if (otmp->cursed) {
            You("pass out.");
            multi = -rnd(15);
            nomovemsg = "You awake with a headache.";
        }
        /* liquid courage */
        make_fearless(itimeout_incr(HFearless, d(8, otmp->blessed ? 6 : 3)), FALSE);
        break;
    case POT_ENLIGHTENMENT:
        if (otmp->cursed) {
            unkn++;
            You("have an uneasy feeling...");
            exercise(A_WIS, FALSE);
        } else {
            if (otmp->blessed) {
                (void) adjattrib(A_INT, 1, FALSE);
                (void) adjattrib(A_WIS, 1, FALSE);
            }
            You_feel("self-knowledgeable...");
            display_nhwindow(WIN_MESSAGE, FALSE);
            enlightenment(MAGICENLIGHTENMENT, ENL_GAMEINPROGRESS);
            pline_The("feeling subsides.");
            exercise(A_WIS, TRUE);
        }
        break;
    case SPE_INVISIBILITY:
        /* spell cannot penetrate mummy wrapping */
        if (BInvis && uarmc->otyp == MUMMY_WRAPPING) {
            You_feel("rather itchy under %s.", yname(uarmc));
            break;
        }
        /* FALLTHRU */
    case POT_INVISIBILITY:
        if (Invis || Blind || BInvis) {
            nothing++;
        } else {
            self_invis_message();
        }
        incr_itimeout(&HInvis, rn1(otmp->odiluted ? 7 : 15, 31 * (otmp->blessed ? rnd(4) : 1)));
        newsym(u.ux, u.uy); /* update position */
        if (otmp->cursed) {
            pline("For some reason, you feel your presence is known.");
            aggravate();
        }
        break;
    case POT_SEE_INVISIBLE: /* tastes like fruit juice in Rogue */
    case POT_FRUIT_JUICE: {
        int msg = Invisible && !Blind;

        unkn++;
        if (otmp->cursed)
            pline("Yecch!  This tastes %s.",
                  Hallucination ? "overripe" : "rotten");
        else
            pline(
                Hallucination
                    ? "This tastes like 10%% real %s%s all-natural beverage."
                    : "This tastes like %s%s.",
                otmp->odiluted ? "reconstituted " : "", fruitname(TRUE));
        if (otmp->otyp == POT_FRUIT_JUICE) {
            u.uhunger += (otmp->odiluted ? 5 : 10) * (2 + bcsign(otmp));
            newuhs(FALSE);
            break;
        }
        if (!otmp->cursed) {
            /* Tell them they can see again immediately, which
             * will help them identify the potion...
             */
            make_blinded(0L, TRUE);
        }
        if (otmp->blessed)
            HSee_invisible |= FROMOUTSIDE;
        else
	    incr_itimeout(&HSee_invisible, rn1(otmp->odiluted ? 50 : 100, 750));
        set_mimic_blocking(); /* do special mimic handling */
        see_monsters();       /* see invisible monsters */
        newsym(u.ux, u.uy);   /* see yourself! */
        if (msg && !Blind) {  /* Blind possible if polymorphed */
            You("can see through yourself, but you are visible!");
            unkn--;
        }
        break;
    }
    case POT_PARALYSIS:
        if (Free_action) {
            You("stiffen momentarily.");
        } else {
            if (Levitation || Is_airlevel(&u.uz) || Is_waterlevel(&u.uz))
                You("are motionlessly suspended.");
            else if (u.usteed)
                You("are frozen in place!");
            else
                Your("%s are frozen to the %s!", makeplural(body_part(FOOT)),
                     surface(u.ux, u.uy));
            nomul(-(rn1(otmp->odiluted ? 5 : 10, 25 - 12 * bcsign(otmp))));
            multi_reason = "frozen by a potion";
            nomovemsg = You_can_move_again;
            exercise(A_DEX, FALSE);
        }
        break;
    case POT_SLEEPING:
        if (how_resistant(SLEEP_RES) == 100 || Free_action) {
            monstseesu(M_SEEN_SLEEP);
            You("yawn.");
        } else {
            You("suddenly fall asleep!");
            fall_asleep(-resist_reduce(rn1(otmp->odiluted ? 5 : 10, 25 - 12 * bcsign(otmp)), SLEEP_RES), TRUE);
        }
        break;
    case POT_MONSTER_DETECTION:
    case SPE_DETECT_MONSTERS:
        if (otmp->blessed) {
            int x, y;

            if (Detect_monsters)
                nothing++;
            unkn++;
            /* after a while, repeated uses become less effective */
            if ((HDetect_monsters & TIMEOUT) >= 300L)
                i = 1;
            else
                i = rn1(otmp->odiluted ? 20 : 40, 21);
            incr_itimeout(&HDetect_monsters, i);
            for (x = 1; x < COLNO; x++) {
                for (y = 0; y < ROWNO; y++) {
                    if (levl[x][y].glyph == GLYPH_INVISIBLE) {
                        unmap_object(x, y);
                        newsym(x, y);
                    }
                    if (MON_AT(x, y))
                        unkn = 0;
                }
            }
            see_monsters();
            if (unkn)
                You_feel("lonely.");
            break;
        }
        if (monster_detect(otmp, 0))
            return 1; /* nothing detected */
        exercise(A_WIS, TRUE);
        break;
    case POT_OBJECT_DETECTION:
    case SPE_DETECT_TREASURE:
        if (object_detect(otmp, 0))
            return 1; /* nothing detected */
        exercise(A_WIS, TRUE);
        break;
    case POT_SICKNESS:
        pline("Yecch!  This stuff tastes like poison.");
        if (otmp->blessed) {
            pline("(But in fact it was mildly stale %s.)", fruitname(TRUE));
            if (!Role_if(PM_HEALER)) {
                /* NB: blessed otmp->fromsink is not possible */
                losehp(1, "mildly contaminated potion", KILLED_BY_AN);
            }
        } else {
            if (how_resistant(POISON_RES) == 100)
                pline("(But in fact it was biologically contaminated %s.)",
                      fruitname(TRUE));
            if (Role_if(PM_HEALER)) {
                monstseesu(M_SEEN_POISON);
                pline("Fortunately, you have been immunized.");
            } else {
                char contaminant[BUFSZ];
                int typ = rn2(A_MAX);

                Sprintf(contaminant, "%s%s",
                        (how_resistant(POISON_RES) == 100) ? "mildly " : "",
                        (otmp->fromsink) ? "contaminated tap water"
                                         : "contaminated potion");
                if (!Fixed_abil) {
                    poisontell(typ, FALSE);
                    (void) adjattrib(typ, -(resist_reduce(rn1(4, 3), POISON_RES) + 1),
                                     1);
                }
                if (how_resistant(POISON_RES) < 100) {
                    if (otmp->fromsink)
                        losehp(resist_reduce(rnd(10) + 5 * !!(otmp->cursed), POISON_RES), contaminant,
                               KILLED_BY);
                    else
                        losehp(resist_reduce(rnd(10) + 5 * !!(otmp->cursed), POISON_RES), contaminant,
                               KILLED_BY_AN);
                } else {
                    /* rnd loss is so that unblessed poorer than blessed */
                    losehp(1 + rn2(2), contaminant,
                           (otmp->fromsink) ? KILLED_BY : KILLED_BY_AN);
                }
                exercise(A_CON, FALSE);
            }
        }
        if (Hallucination) {
            You("are shocked back to your senses!");
            (void) make_hallucinated(0L, FALSE, 0L);
        }
        if (LarvaCarrier) {
            You_feel("as if your body is your own again.");
            make_carrier(0L, FALSE);
        }
        break;
    case POT_CONFUSION:
        if (!Confusion) {
            if (Hallucination) {
                pline("What a trippy feeling!");
                unkn++;
            } else
                pline("Huh, What?  Where am I?");
        } else
            nothing++;
        make_confused(itimeout_incr(HConfusion,
                                    rn1(otmp->odiluted ? 4 : 7, 16 - 8 * bcsign(otmp))),
                      FALSE);
        break;
    case POT_CLAIRVOYANCE:
        /* KMH -- handle cursed, blessed, blocked */
        if (otmp->cursed)
            nothing++;
        else if (!BClairvoyant) {
            if (Hallucination)
                pline("Dude! See-through walls!");
            do_vicinity_map(otmp);
        }
        if (otmp->blessed)
            incr_itimeout(&HClairvoyant, rn1(50, otmp->odiluted ? 100: 250));
        break;
    case POT_ESP:
        /* KMH -- handle cursed, blessed */
        if (otmp->cursed) {
            if (HTelepat)
                mod = "less ";
            else {
                unkn++;
                mod = NULL;
            }
            HTelepat = 0;
        } else if (otmp->blessed) {
            mod = "more ";
            incr_itimeout(&HTelepat, rn1(500, 500));
        } else {
            mod = "more ";
            incr_itimeout(&HTelepat, rn1(250, 250));
        }
        if (mod) {
            You_feel(Hallucination ?
                    "%sin touch with the cosmos." :
                    "%smentally acute.", mod);
        }
        see_monsters();
        break;
    case POT_INVULNERABILITY:
        incr_itimeout(&Invulnerable, 
                      rn1(4, (otmp->odiluted ? 4 : 8) + 4 * bcsign(otmp)));
        You_feel(Hallucination ?
                 "like a super-duper hero!" : "invulnerable!");
        break;
    case POT_GAIN_ABILITY:
        if (otmp->cursed) {
            pline("Ulch!  That potion tasted foul!");
            
            /* [BarclayII] cursed potions randomly decrease your attribute */
            happened = FALSE;
            if (!Fixed_abil) {
                for (i = 0; i < A_MAX; ++i) {
                    if (rn2(2)) {
                        adjattrib(i, -1, 0);
                        happened = TRUE;
                    }
                }
            }
            if (!happened)
                nothing++;
            unkn++;
        } else if (Fixed_abil) {
            nothing++;
        } else {      /* If blessed, increase all; if not, try up to */
            int itmp; /* 6 times to find one which can be increased. */

            i = -1;   /* increment to 0 */
            for (ii = A_MAX; ii > 0; ii--) {
                i = (otmp->blessed ? i + 1 : rn2(A_MAX));
                /* only give "your X is already as high as it can get"
                   message on last attempt (except blessed potions) */
                itmp = (otmp->blessed || ii == 1) ? 0 : -1;
                if (i == A_STR) {
                    if (gainstr(otmp, 1, (itmp == 0)) && !otmp->blessed)
                        break;
                } else {
                    if (adjattrib(i, 1, itmp) && !otmp->blessed)
                        break;
                }
            }
        }
        break;
    case POT_REFLECTION:
        if (otmp->cursed) {
            pline("It\'s like drinking glue!");
            unkn++;
        } else {
            You("are covered in a mirror-like sheen!");
            if (otmp->blessed) {
                set_itimeout(&HReflecting, rn1(50, otmp->odiluted ? 100 : 250));
            } else {
                set_itimeout(&HReflecting, rn1(10, otmp->odiluted ? 50 : 100));
            }
        }
        break;
    case POT_REGENERATION:
        if (otmp->cursed) {
            You("begin to wither away!");
            incr_itimeout(&HWithering, rn1(10, 20));
            unkn++;
            context.botl = TRUE;
        } else {
            You("metabolism kicks into overdrive!");
            if (otmp->blessed) {
                set_itimeout(&HRegeneration, rn1(100,  otmp->odiluted ? 50 : 100));
            } else {
                set_itimeout(&HRegeneration, rn1(50,  otmp->odiluted ? 25 : 50));
            }
        }
        break;
    case POT_SPEED:
        /* skip when mounted; heal_legs() would heal steed's legs */
        if (Wounded_legs && !otmp->cursed && !u.usteed) {
            heal_legs(0);
            unkn++;
            break;
        }
        /* FALLTHRU */
    case SPE_HASTE_SELF:
        speed_up(rn1(10, 100 + 60 * bcsign(otmp)));

        /* non-cursed potion grants intrinsic speed */
        if (otmp->otyp == POT_SPEED
            && !otmp->cursed && !(HFast & INTRINSIC)) {
            Your("quickness feels very natural.");
            HFast |= FROMOUTSIDE;
        }
        break;
    case POT_BLINDNESS:
        if (Blind)
            nothing++;
        make_blinded(itimeout_incr(Blinded,
                                   rn1(otmp->odiluted ? 100 : 200, 250 - 125 * bcsign(otmp))),
                     (boolean) !Blind);
        break;
    case POT_GAIN_LEVEL:
        if (otmp->cursed) {
            unkn++;
            /* they went up a level */
            if ((ledger_no(&u.uz) == 1 && u.uhave.amulet)
                || Can_rise_up(u.ux, u.uy, &u.uz)) {
                const char *riseup = "rise up, through the %s!";

                if (ledger_no(&u.uz) == 1) {
                    d_level newlev;
                    newlev.dnum = astral_level.dnum;
                    newlev.dlevel = dungeons[astral_level.dnum].entry_lev;

                    if (Role_if(PM_INFIDEL)) { /* omg no freebies */
                        You("have an uneasy feeling.");
                        goto no_rise;
                    } else {
                        You(riseup, ceiling(u.ux, u.uy));
                        goto_level(&newlev, FALSE, FALSE, FALSE);
                    }
                } else {
                    register int newlev = depth(&u.uz) - 1;
                    d_level newlevel;

                    get_level(&newlevel, newlev);
                    if (Is_valley(&u.uz)) {
                        /* while Cerberus lives, the effects of a
                           cursed potion of gain level are nullified
                           in the Valley. Killing him restores the effect */
                        if (!u.uevent.ucerberus) {
                            You("have an uneasy feeling.");
                            goto no_rise;
                        } else {
                            You(riseup, ceiling(u.ux, u.uy));
                            goto_level(&newlevel, FALSE, FALSE, FALSE);
                        }
                    } else if (on_level(&newlevel, &u.uz)) {
                        pline("It tasted bad.");
                        break;
                    } else
                        You(riseup, ceiling(u.ux, u.uy));
                    goto_level(&newlevel, FALSE, FALSE, FALSE);
                }
            } else
                You("have an uneasy feeling.");
no_rise:
            break;
        }
        pluslvl(FALSE);
        /* blessed potions place you at a random spot in the
           middle of the new level instead of the low point */
        if (otmp->blessed)
            u.uexp = rndexp(TRUE);
        break;
    case POT_HEALING:
        You_feel("better.");
        healup(d(10 + 2 * bcsign(otmp), 4) / (otmp->odiluted ? 2 : 1),
               !otmp->cursed ? 1 : 0, !!otmp->blessed, !otmp->cursed);
        exercise(A_CON, TRUE);
        break;
    case POT_EXTRA_HEALING:
        You_feel("much better.");
        healup(d(10 + 2 * bcsign(otmp), 8) / (otmp->odiluted ? 2 : 1),
               (otmp->blessed ? 5 : !otmp->cursed ? 2 : 0) / (otmp->odiluted ? 2 : 1),
                !otmp->cursed, TRUE);
        if (otmp->blessed) {
            if (Withering) {
                set_itimeout(&HWithering, (long) 0);
                if (!Withering)
                    You("are no longer withering away.");
            }
        }
        (void) make_hallucinated(0L, TRUE, 0L);
        exercise(A_CON, TRUE);
        exercise(A_STR, TRUE);
        break;
    case POT_FULL_HEALING:
        You_feel("completely healed.");
        healup(400, (4 + 4 * bcsign(otmp)) / (otmp->odiluted ? 2 : 1), !otmp->cursed, TRUE);
        /* Restore one lost level if blessed */
        if (otmp->blessed && !otmp->odiluted && u.ulevel < u.ulevelmax) {
            /* when multiple levels have been lost, drinking
               multiple potions will only get half of them back */
            u.ulevelmax -= 1;
            pluslvl(FALSE);
        }
        if (!otmp->cursed) {
            if (Withering) {
                set_itimeout(&HWithering, (long) 0);
                if (!Withering)
                    You("are no longer withering away.");
            }
        }
        (void) make_hallucinated(0L, TRUE, 0L);
        exercise(A_STR, TRUE);
        exercise(A_CON, TRUE);
        break;
    case POT_LEVITATION:
    case SPE_LEVITATION:
        /*
         * BLevitation will be set if levitation is blocked due to being
         * inside rock (currently or formerly in phazing xorn form, perhaps)
         * but it doesn't prevent setting or incrementing Levitation timeout
         * (which will take effect after escaping from the rock if it hasn't
         * expired by then).
         */
        if (!Levitation && !BLevitation) {
            /* kludge to ensure proper operation of float_up() */
            set_itimeout(&HLevitation, 1L);
            float_up();
            /* This used to set timeout back to 0, then increment it below
               for blessed and uncursed effects.  But now we leave it so
               that cursed effect yields "you float down" on next turn.
               Blessed and uncursed get one extra turn duration. */
        } else /* already levitating, or can't levitate */
            nothing++;

        if (otmp->cursed) {
            /* 'already levitating' used to block the cursed effect(s)
               aside from ~I_SPECIAL; it was not clear whether that was
               intentional; either way, it no longer does (as of 3.6.1) */
            HLevitation &= ~I_SPECIAL; /* can't descend upon demand */
            if (BLevitation) {
                ; /* rising via levitation is blocked */
            } else if ((u.ux == xupstair && u.uy == yupstair)
                    || (sstairs.up && u.ux == sstairs.sx && u.uy == sstairs.sy)
                    || (xupladder && u.ux == xupladder && u.uy == yupladder)) {
                (void) doup();
                /* in case we're already Levitating, which would have
                   resulted in incrementing 'nothing' */
                nothing = 0; /* not nothing after all */
            } else if (has_ceiling(&u.uz)) {
                int dmg = rnd(!uarmh ? 10 : !is_hard(uarmh) ? 6 : 3);

                You("hit your %s on the %s.", body_part(HEAD),
                    ceiling(u.ux, u.uy));
                losehp(Maybe_Half_Phys(dmg), "colliding with the ceiling",
                       KILLED_BY);
                nothing = 0; /* not nothing after all */
            }
        } else if (otmp->blessed) {
            /* at this point, timeout is already at least 1 */
            incr_itimeout(&HLevitation, rn1(otmp->odiluted ? 25 : 50, 250));
            /* can descend at will (stop levitating via '>') provided timeout
               is the only factor (ie, not also wearing Lev ring or boots) */
            HLevitation |= I_SPECIAL;
        } else /* timeout is already at least 1 */
            incr_itimeout(&HLevitation, rn1(otmp->odiluted ? 70 : 140, 10));

        if (Levitation && IS_SINK(levl[u.ux][u.uy].typ))
            spoteffects(FALSE);
        /* levitating blocks flying */
        float_vs_flight();
        break;
    case POT_GAIN_ENERGY: { /* M. Stephenson */
        int num;

        if (otmp->cursed)
            You_feel("lackluster.");
        else
            pline("Magical energies course through your body.");

        /* old: num = rnd(5) + 5 * otmp->blessed + 1;
         *      blessed:  +7..11 max & current (+9 avg)
         *      uncursed: +2.. 6 max & current (+4 avg)
         *      cursed:   -2.. 6 max & current (-4 avg)
         * new: (3.6.0)
         *      blessed:  +3..18 max (+10.5 avg), +9..54 current (+31.5 avg)
         *      uncursed: +2..12 max (+ 7   avg), +6..36 current (+21   avg)
         *      cursed:   -1.. 6 max (- 3.5 avg), -3..18 current (-10.5 avg)
         */
        num = d(otmp->blessed ? 3 : !otmp->cursed ? 2 : 1, 6) / (otmp->odiluted ? 2 : 1);
        if (otmp->cursed)
            num = -num; /* subtract instead of add when cursed */
        u.uenmax += num;
        if (u.uenmax <= 0)
            u.uenmax = 0;
        u.uen += 3 * num;
        if (u.uen > u.uenmax)
            u.uen = u.uenmax;
        else if (u.uen <= 0)
            u.uen = 0;
        context.botl = 1;
        exercise(A_WIS, TRUE);
        break;
    }
    case POT_OIL: { /* P. Winner */
        boolean good_for_you = FALSE;

        if (otmp->lamplit) {
            if (likes_fire(youmonst.data)) {
                pline("Ahh, a refreshing drink.");
                good_for_you = TRUE;
            } else {
                You("burn your %s.", body_part(FACE));
                /* fire damage */
                losehp(resist_reduce(d(2, 4), FIRE_RES) + d(1, 4), "burning potion of oil",
                       KILLED_BY_AN);
            }
        } else if (otmp->cursed)
            pline("This tastes like castor oil.");
        else
            pline("That was smooth!");
        exercise(A_WIS, good_for_you);
        break;
    }
    case POT_ACID:
        if (how_resistant(ACID_RES) > 50) {
            /* Not necessarily a creature who _likes_ acid */
            pline("This tastes %s.", Hallucination ? "tangy" : "sour");
            monstseesu(M_SEEN_ACID);
        } else {
            int dmg;

            pline("This burns%s!",
                  otmp->blessed ? " a little" : otmp->cursed ? " a lot"
                                                             : " like acid");
            dmg = d(otmp->cursed ? 2 : 1, otmp->blessed ? 4 : 8) / (otmp->odiluted ? 2 : 1);
            losehp(Maybe_Half_Phys(dmg), "potion of acid", KILLED_BY_AN);
            exercise(A_CON, FALSE);
        }
        if (Stoned)
            fix_petrification();
        unkn++; /* holy/unholy water can burn like acid too */
        break;
    case POT_POLYMORPH:
        You_feel("a little %s.", Hallucination ? "normal" : "strange");
        if (!Unchanging)
            polyself(0);
        break;
    case POT_BLOOD:
    case POT_VAMPIRE_BLOOD:
        unkn++;
        u.uconduct.unvegan++;
        if (maybe_polyd(is_vampire(youmonst.data), Race_if(PM_VAMPIRIC))) {
            violated_vegetarian();
            if (otmp->cursed)
                pline("Yecch!  This %s.", Hallucination 
                      ? "liquid could do with a good stir" 
                      : "blood has congealed");
            else pline(Hallucination 
                      ? "The %s liquid stirs memories of home." 
                      : "The %s blood tastes delicious.",
                      otmp->odiluted ? "watery" : "thick");
            if (!otmp->cursed)
                lesshungry((otmp->odiluted ? 1 : 2) *
                           (otmp->otyp == POT_VAMPIRE_BLOOD ? 400 :
                            otmp->blessed ? 100 : 30));
            if (otmp->otyp == POT_VAMPIRE_BLOOD && otmp->blessed) {
                int num = newhp();
                if (Upolyd) {
                    u.mhmax += num;
                    u.mh += num;
                } else {
                    u.uhpmax += num;
                    u.uhp += num;
                }
            }
        } else if (otmp->otyp == POT_VAMPIRE_BLOOD) {
            /* [CWC] fix conducts for potions of (vampire) blood -
               doesn't use violated_vegetarian() to prevent
               duplicated "you feel guilty" messages */
            u.uconduct.unvegetarian++;
            if (!Race_if(PM_VAMPIRIC)) {
                if (u.ualign.type == A_LAWFUL || Role_if(PM_MONK)) {
                    You_feel("%sguilty about drinking such a vile liquid.",
                             Role_if(PM_MONK) ? "especially " : "");
                    u.ugangr++;
                    adjalign(-15);
                } else if (u.ualign.type == A_NEUTRAL) {
                    You_feel("guilty.");
                    adjalign(-3);
                }
                exercise(A_CON, FALSE);
            }
            if (Race_if(PM_VAMPIRIC)) {
                if (!Unchanging) 
                    rehumanize();
                break;
            } else if (!Unchanging) {
                int successful_polymorph = FALSE;
                if (otmp->blessed)
                    successful_polymorph = polymon(PM_VAMPIRE_NOBLE);
                else if (otmp->cursed)
                    successful_polymorph = polymon(PM_VAMPIRE_BAT);
                else
                    successful_polymorph = polymon(PM_VAMPIRE);
                if (successful_polymorph)
                    u.mtimedone = 0;	/* "Permament" change */
            }
        } else {
            violated_vegetarian();
            pline("Ugh.  That was vile.");
            make_vomiting(Vomiting+d(10,8), TRUE);
        }
        break;
    default:
        impossible("What a funny potion! (%u)", otmp->otyp);
        return 0;
    }
    return -1;
}

void
healup(nhp, nxtra, curesick, cureblind)
int nhp, nxtra;
register boolean curesick, cureblind;
{
    if (nhp) {
        if (Upolyd) {
            u.mh += nhp;
            if (u.mh > u.mhmax)
                u.mh = (u.mhmax += nxtra);
        } else {
            u.uhp += nhp;
            if (u.uhp > u.uhpmax)
                u.uhp = (u.uhpmax += nxtra);
        }
    }
    if (cureblind) {
        /* 3.6.1: it's debatible whether healing magic should clean off
           mundane 'dirt', but if it doesn't, blindness isn't cured */
        u.ucreamed = 0;
        make_blinded(0L, TRUE);
        /* heal deafness too */
        make_deaf(0L, TRUE);
    }
    if (curesick) {
        make_vomiting(0L, TRUE);
        make_sick(0L, (char *) 0, TRUE, SICK_ALL);
    }
    context.botl = 1;
    return;
}

void
strange_feeling(obj, txt)
struct obj *obj;
const char *txt;
{
    if (flags.beginner || !txt)
        You("have a %s feeling for a moment, then it passes.",
            Hallucination ? "normal" : "strange");
    else
        pline1(txt);

    if (!obj) /* e.g., crystal ball finds no traps */
        return;

    if (obj->dknown && !objects[obj->otyp].oc_name_known
        && !objects[obj->otyp].oc_uname)
        docall(obj);

    useup(obj);
}

const char *bottlenames[] = { "bottle", "phial", "flagon", "carafe",
                              "flask",  "jar",   "vial" };

const char *
bottlename()
{
    return bottlenames[rn2(SIZE(bottlenames))];
}

/* handle item dipped into water potion or steed saddle/barding splashed by same */
STATIC_OVL boolean
H2Opotion_dip(potion, targobj, useeit, objphrase)
struct obj *potion; /* water */
struct obj *targobj; /* item being dipped into the water */
boolean useeit; /* will hero see the glow/aura? */
const char *objphrase; /* "Your widget glows" or "Steed's saddle/barding glows" */
{
    void FDECL((*func), (struct obj *)) = (void (*)(struct obj *)) 0;
    const char *glowcolor = 0;
#define COST_alter (-2)
#define COST_none (-1)
    int costchange = COST_none;
    boolean altfmt = FALSE, res = FALSE;

    if (!potion || potion->otyp != POT_WATER)
        return FALSE;

    if (potion->otyp == POT_AMNESIA) {
        /* Diluting a !ofAmnesia just gives water... */
        Your("%s flat.", aobjnam(potion, "become"));
        potion->odiluted = 0;
        potion->otyp = POT_WATER;
        costchange = COST_alter;
    } else if (potion->blessed) {
        if (targobj->cursed) {
            func = uncurse;
            glowcolor = NH_AMBER;
            costchange = COST_UNCURS;
        } else if (!targobj->blessed) {
            func = bless;
            glowcolor = NH_LIGHT_BLUE;
            costchange = COST_alter;
            altfmt = TRUE; /* "with a <color> aura" */
        }
    } else if (potion->cursed) {
        if (targobj->blessed) {
            func = unbless;
            glowcolor = "brown";
            costchange = COST_UNBLSS;
        } else if (!targobj->cursed) {
            func = curse;
            glowcolor = NH_BLACK;
            costchange = COST_alter;
            altfmt = TRUE;
        }
    } else {
        /* dipping into uncursed water; carried() check skips steed saddle/barding */
        if (carried(targobj)) {
            if (water_damage(targobj, 0, TRUE, u.ux, u.uy) != ER_NOTHING)
                res = TRUE;
        }
    }
    if (func) {
        /* give feedback before altering the target object;
           this used to set obj->bknown even when not seeing
           the effect; now hero has to see the glow, and bknown
           is cleared instead of set if perception is distorted */
        if (useeit) {
            glowcolor = hcolor(glowcolor);
            if (altfmt)
                pline("%s with %s aura.", objphrase, an(glowcolor));
            else
                pline("%s %s.", objphrase, glowcolor);
            iflags.last_msg = PLNMSG_OBJ_GLOWS;
            targobj->bknown = !Hallucination;
        } else {
            /* didn't see what happened:  forget the BUC state if that was
               known unless the bless/curse state of the water is known;
               without this, hero would know the new state even without
               seeing the glow; priest[ess] will immediately relearn it */
            if (!potion->bknown || !potion->dknown)
                targobj->bknown = 0;
            /* [should the bknown+dknown exception require that water
               be discovered or at least named?] */
        }
        /* potions of water are the only shop goods whose price depends
           on their curse/bless state */
        if (targobj->unpaid && targobj->otyp == POT_WATER) {
            if (costchange == COST_alter)
                /* added blessing or cursing; update shop
                   bill to reflect item's new higher price */
                alter_cost(targobj, 0L);
            else if (costchange != COST_none)
                /* removed blessing or cursing; you
                   degraded it, now you'll have to buy it... */
                costly_alteration(targobj, costchange);
        }
        /* finally, change curse/bless state */
        (*func)(targobj);
        res = TRUE;
    }
    return res;
}

/* used when blessed or cursed scroll of light interacts with artifact light;
   if the lit object (Sunsword, shield of light, or gold dragon scales/mail)
   doesn't resist, treat like dipping it in holy or unholy water
   (BUC change, glow message) */
void
impact_arti_light(obj, worsen, seeit)
struct obj *obj; /* wielded Sunsword or worn gold dragon scales/mail */
boolean worsen;  /* True: lower BUC state unless already cursed;
                  * False: raise BUC state unless already blessed */
boolean seeit;   /* True: give "<obj> glows <color>" message */
{
    struct obj *otmp;

    /* if already worst/best BUC it can be, or if it resists, do nothing */
    if ((worsen ? obj->cursed : obj->blessed) || obj_resists(obj, 25, 75))
        return;

    /* curse() and bless() take care of maybe_adjust_light() */
    otmp = mksobj(POT_WATER, TRUE, FALSE);
    if (worsen)
        curse(otmp);
    else
        bless(otmp);
    H2Opotion_dip(otmp, obj, seeit, seeit ? Yobjnam2(obj, "glow") : "");
    dealloc_obj(otmp);
#if 0   /* defer this until caller has used up the scroll so it won't be
         * visible; player was told that it disappeared as hero read it */
    if (carried(obj)) /* carried() will always be True here */
        update_inventory();
#endif
    return;
}

/* potion obj hits monster mon, which might be youmonst; obj always used up 
 * how: How the potion was delivered:
 *      POTHIT_HERO_BASH
 *      POTHIT_HERO_THROW
 *      POTHIT_OTHER_THROW
 *      POTHIT_MONST_THROW
 * */
void
potionhit(mon, obj, how)
struct monst *mon;
struct obj *obj;
int how;
{
    const char *botlnam = bottlename();
    boolean isyou = (mon == &youmonst);
    int distance, tx, ty;
    struct obj *saddle = (struct obj *) 0;
    struct obj *barding = (struct obj *) 0;
    struct obj *ohit = (struct obj *) 0;
    boolean hit_saddle = FALSE, hit_barding = FALSE,
            your_fault = (how <= POTHIT_HERO_THROW 
                    || how == POTHIT_HERO_ENGULF);
    boolean from_engulf = how >= POTHIT_HERO_ENGULF;

    if (isyou) {
        tx = u.ux, ty = u.uy;
        distance = 0;
        pline_The("%s crashes on your %s and breaks into shards.", botlnam,
                  body_part(HEAD));
        losehp(Maybe_Half_Phys(rnd(2)),
               (how == POTHIT_OTHER_THROW) ? "propelled potion" /* scatter */
                                           : "thrown potion",
               KILLED_BY_AN);
    } else {
        tx = mon->mx, ty = mon->my;
        /* sometimes it hits the saddle */
        if (((mon->misc_worn_check & W_SADDLE)
             && (saddle = which_armor(mon, W_SADDLE)))
            && (!rn2(10)
                || (obj->otyp == POT_WATER
                    && ((rnl(10) > 7 && obj->cursed)
                        || (rnl(10) < 4 && obj->blessed) || !rn2(3)))))
            hit_saddle = TRUE;
        /* same chance for hitting barding */
        if (((mon->misc_worn_check & W_BARDING)
             && (barding = which_armor(mon, W_BARDING)))
            && (!rn2(10)
                || (obj->otyp == POT_WATER
                    && ((rnl(10) > 7 && obj->cursed)
                        || (rnl(10) < 4 && obj->blessed) || !rn2(3)))))
            hit_barding = TRUE;
        distance = distu(tx, ty);
        if (from_engulf) {
            ; /* No message */
        } else if (!cansee(tx, ty)) {
            pline("Crash!");
        } else {
            char *mnam = mon_nam(mon);
            char buf[BUFSZ];

            if (hit_saddle && saddle) {
                Sprintf(buf, "%s saddle",
                        s_suffix(x_monnam(mon, ARTICLE_THE, (char *) 0,
                                          (SUPPRESS_IT | SUPPRESS_SADDLE), FALSE)));
            } else if (hit_barding && barding) {
                Sprintf(buf, "%s barding",
                        s_suffix(x_monnam(mon, ARTICLE_THE, (char *) 0,
                                          (SUPPRESS_IT | SUPPRESS_BARDING), FALSE)));
            } else if (has_head(mon->data)) {
                Sprintf(buf, "%s %s", s_suffix(mnam),
                        (notonhead ? "body" : "head"));
            } else {
                Strcpy(buf, mnam);
            }
            pline_The("%s crashes on %s and breaks into shards.", botlnam,
                      buf);
        }
        if (rn2(5) && mon->mhp > 1 && !hit_saddle && !hit_barding)
            damage_mon(mon, 1, AD_PHYS);
    }

    /* oil doesn't instantly evaporate; Neither does a saddle/barding hit */
    if (obj->otyp != POT_OIL && !hit_saddle && !hit_barding 
          && !from_engulf && cansee(tx, ty))
        pline("%s.", Tobjnam(obj, "evaporate"));

    if (isyou) {
        switch (obj->otyp) {
        case POT_OIL:
            if (obj->lamplit) {
                explode_oil(obj, u.ux, u.uy);
            } else {
                pline("Yuck!  You're covered in oil!");
                if (!Glib)
                    make_glib(rn1(5, 5));
                if (obj->dknown)
                    makeknown(POT_OIL);
            }
            break;
        case POT_POLYMORPH:
            You_feel("a little %s.", Hallucination ? "normal" : "strange");
            if (!Unchanging && !Antimagic)
                polyself(0);
            break;
        case POT_ACID:
            if (how_resistant(ACID_RES) < 50) {
                int dmg;

                pline("This burns%s!",
                      obj->blessed ? " a little"
                                   : obj->cursed ? " a lot" : "");
                dmg = d(obj->cursed ? 2 : 1, obj->blessed ? 4 : 8);
                losehp(Maybe_Half_Phys(dmg), "potion of acid", KILLED_BY_AN);
            } else {
                monstseesu(M_SEEN_ACID);
            }
            break;
        case POT_AMNESIA:
            /* Uh-oh! */
            switch (rnd(7)) {
                case 1:
                    if (uarmh)
                        ohit = uarmh;
                    break;
                case 2:
                    if (uarmf)
                        ohit = uarmf;
                    break;
                case 3:
                    /* Gloves sort of protect rings */
                    if (uarmg && rn2(3))
                        ohit = uarmg;
                    else
                        ohit = rn2(2) ? uleft : uright;
                    break;
                case 4:
                    /* Armor layers can protect each-other */
                    if (uarmc)
                        ohit = uarmc;
                    else if (uarm)
                        ohit = uarm;
                    else if (uarmu)
                        ohit = uarmu;
                    break;
                case 5:
                    if (uarms)
                        ohit = uarms;
                    break;
                case 6:
                    if (uwep)
                        ohit = uwep;
                    break;
                case 7:
                    if (uswapwep)
                        ohit = uswapwep;
                    break;
            }
            
            if (ohit && rn2(10 - (ohit->cursed ? 8 : 0))) {
                if (ohit->oprops & ITEM_OILSKIN && !Blind)
                    pline_The("sparkling water slides off %s", 
                              yobjnam(ohit, (char *) 0));
                else
                    amnesia_wet(obj, ohit, your_fault);
            }
            break;
        case POT_HALLUCINATION:
            if (!Halluc_resistance) {
                makeknown(POT_HALLUCINATION);
            }
            break;
        }
    } else if (hit_saddle && saddle) {
        char *mnam, buf[BUFSZ], saddle_glows[BUFSZ];
        boolean affected = FALSE;
        boolean useeit = !Blind && canseemon(mon) && cansee(tx, ty);

        mnam = x_monnam(mon, ARTICLE_THE, (char *) 0,
                        (SUPPRESS_IT | SUPPRESS_SADDLE), FALSE);
        Sprintf(buf, "%s", upstart(s_suffix(mnam)));

        switch (obj->otyp) {
        case POT_WATER:
            Sprintf(saddle_glows, "%s %s", buf, aobjnam(saddle, "glow"));
            affected = H2Opotion_dip(obj, saddle, useeit, saddle_glows);
            break;
        case POT_POLYMORPH:
            /* Do we allow the saddle to polymorph? */
            break;
        }
        if (useeit && !affected)
            pline("%s %s wet.", buf, aobjnam(saddle, "get"));
    } else if (hit_barding && barding) {
        char *mnam, buf[BUFSZ], barding_glows[BUFSZ];
        boolean affected = FALSE;
        boolean useeit = !Blind && canseemon(mon) && cansee(tx, ty);

        mnam = x_monnam(mon, ARTICLE_THE, (char *) 0,
                        (SUPPRESS_IT | SUPPRESS_BARDING), FALSE);
        Sprintf(buf, "%s", upstart(s_suffix(mnam)));

        switch (obj->otyp) {
        case POT_WATER:
            Sprintf(barding_glows, "%s %s", buf, aobjnam(barding, "glow"));
            affected = H2Opotion_dip(obj, barding, useeit, barding_glows);
            break;
        case POT_POLYMORPH:
            /* Do we allow barding to polymorph? */
            break;
        }
        if (useeit && !affected)
            pline("%s %s wet.", buf, aobjnam(barding, "get"));
    } else {
        boolean angermon = your_fault, cureblind = FALSE;

        switch (obj->otyp) {
        case POT_FULL_HEALING:
            cureblind = TRUE;
            if (mon->msick || mon->mdiseased) {
                if (canseemon(mon))
                    pline("%s is no longer ill.", Monnam(mon));
                mon->msick = mon->mdiseased = 0;
            }
            if (mon->mwither) {
                if (canseemon(mon))
                    pline("%s is no longer withering away.", Monnam(mon));
                mon->mwither = 0;
            }
            /*FALLTHRU*/
        case POT_EXTRA_HEALING:
            if (!obj->cursed) {
                cureblind = TRUE;
                if (mon->msick || mon->mdiseased) {
                    if (canseemon(mon))
                        pline("%s is no longer ill.", Monnam(mon));
                    mon->msick = mon->mdiseased = 0;
                }
                if (mon->mwither) {
                    if (canseemon(mon))
                        pline("%s is no longer withering away.", Monnam(mon));
                    mon->mwither = 0;
                }
            }
            /*FALLTHRU*/
        case POT_HEALING:
            if (obj->blessed) {
                cureblind = TRUE;
                if (mon->msick || mon->mdiseased) {
                    if (canseemon(mon))
                        pline("%s is no longer ill.", Monnam(mon));
                    mon->msick = mon->mdiseased = 0;
                }
                if (mon->mwither) {
                    if (canseemon(mon))
                        pline("%s is no longer withering away.", Monnam(mon));
                    mon->mwither = 0;
                }
            }
            if (mon->data == &mons[PM_PESTILENCE])
                goto do_illness;
            /*FALLTHRU*/
        case POT_GAIN_ABILITY:
 do_healing:
            angermon = FALSE;
            if (mon->mhp < mon->mhpmax) {
                mon->mhp = mon->mhpmax;
                if (canseemon(mon))
                    pline("%s looks sound and hale again.", Monnam(mon));
            }
            if (cureblind)
                mcureblindness(mon, canseemon(mon));
            break;
        case POT_RESTORE_ABILITY:
            angermon = FALSE;
            if (mon->mcan) {
                mon->mcan = 0;
                if (canseemon(mon))
                    pline("%s looks revitalized.", Monnam(mon));
            }
            break;
        case POT_SICKNESS:
            if (mon->data == &mons[PM_PESTILENCE])
                goto do_healing;
            if (dmgtype(mon->data, AD_DISE)
                /* won't happen, see prior goto */
                || dmgtype(mon->data, AD_PEST)
                /* most common case */
                || resists_poison(mon)
                || defended(mon, AD_DRST)) {
                if (canseemon(mon))
                    pline("%s looks unharmed.", Monnam(mon));
                break;
            }
 do_illness:
            if ((mon->mhpmax > 3) && !resist(mon, POTION_CLASS, 0, NOTELL))
                mon->mhpmax /= 2;
            if ((mon->mhp > 2) && !resist(mon, POTION_CLASS, 0, NOTELL))
                mon->mhp /= 2;
            if (mon->mhp > mon->mhpmax)
                mon->mhp = mon->mhpmax;
            if (canseemon(mon))
                pline("%s looks rather ill.", Monnam(mon));
            break;
        case POT_CONFUSION:
        case POT_BOOZE:
            if (!resist(mon, POTION_CLASS, 0, NOTELL))
                mon->mconf = TRUE;
            break;
        case POT_INVISIBILITY: {
            boolean sawit = canspotmon(mon);

            angermon = FALSE;
            mon_set_minvis(mon);
            if (sawit && !canspotmon(mon) && cansee(mon->mx, mon->my))
                map_invisible(mon->mx, mon->my);
            break;
        }
        case POT_SLEEPING:
            /* wakeup() doesn't rouse victims of temporary sleep */
            if (sleep_monst(mon, rnd(12), POTION_CLASS)) {
                pline("%s falls asleep.", Monnam(mon));
                slept_monst(mon);
            }
            break;
        case POT_PARALYSIS:
            if (has_free_action(mon)) {
                pline("%s stiffens momentarily.", Monnam(mon));
                break;
            } else if (mon->mcanmove) {
                /* really should be rnd(5) for consistency with players
                 * breathing potions, but...
                 */
                paralyze_monst(mon, rnd(25));
            }
            break;
        case POT_SPEED:
            angermon = FALSE;
            mon_adjust_speed(mon, 1, obj);
            break;
        case POT_BLINDNESS:
            if (haseyes(mon->data) && !mon_perma_blind(mon)) {
                int btmp = 64 + rn2(32)
                            + rn2(32) * !resist(mon, POTION_CLASS, 0, NOTELL);

                btmp += mon->mblinded;
                mon->mblinded = min(btmp, 127);
                mon->mcansee = 0;
            }
            break;
        case POT_WATER:
            if (is_undead(mon->data) || is_demon(mon->data)
                || is_were(mon->data) || is_vampshifter(mon)) {
                if (obj->blessed) {
                    pline("%s %s in pain!", Monnam(mon),
                          is_silent(mon->data) ? "writhes" : "shrieks");
                    if (!is_silent(mon->data))
                        wake_nearto(tx, ty, mon->data->mlevel * 10);
                    damage_mon(mon, d(2, 6), AD_ACID);
                    /* should only be by you */
                    if (DEADMONSTER(mon))
                        killed(mon);
                    else if (is_were(mon->data) && !is_human(mon->data))
                        new_were(mon); /* revert to human */
                } else if (obj->cursed) {
                    angermon = FALSE;
                    if (canseemon(mon))
                        pline("%s looks healthier.", Monnam(mon));
                    mon->mhp += d(2, 6);
                    if (mon->mhp > mon->mhpmax)
                        mon->mhp = mon->mhpmax;
                    if (is_were(mon->data) && is_human(mon->data)
                        && !Protection_from_shape_changers)
                        new_were(mon); /* transform into beast */
                }
            } else if (mon->data == &mons[PM_GREMLIN]) {
                angermon = FALSE;
                (void) split_mon(mon, (struct monst *) 0);
            } else if (mon->data == &mons[PM_IRON_GOLEM]) {
                if (canseemon(mon))
                    pline("%s rusts.", Monnam(mon));
                damage_mon(mon, d(1, 6), AD_PHYS);
                /* should only be by you */
                if (DEADMONSTER(mon))
                    killed(mon);
            }
            break;
        case POT_AMNESIA:
            switch (monsndx(mon->data)) {
                case PM_GREMLIN:
                    /* Gremlins multiply... */
                    mon->mtame = FALSE;	
                    (void)split_mon(mon, (struct monst *)0);
                    break;
                case PM_FLAMING_SPHERE:
                    if (canseemon(mon))
                        pline("%s flickers.", Monnam(mon));
                    damage_mon(mon, d(1, 6), AD_PHYS);
                    break;
                case PM_IRON_GOLEM:
                    if (canseemon(mon))
                        pline("%s rusts.", Monnam(mon));
                    damage_mon(mon, d(1, 6), AD_PHYS);
                    break;
                case PM_WIZARD_OF_YENDOR:
                    if (your_fault) {
                        if (canseemon(mon)) 
                            pline("%s laughs at you!", Monnam(mon));
                        forget(1);
                    }
                    break;
                case PM_MEDUSA:
                    if (canseemon(mon))
                        pline("%s looks like %s's having a bad hair day!", 
                                Monnam(mon), mhe(mon));
                    break;
                case PM_CROESUS:
                    if (canseemon(mon))
                        pline("%s says: 'My gold! I must count my gold!'", 
                            Monnam(mon));
                    break;
                case PM_DEATH:
                    if (canseemon(mon))
                        pline("%s pauses, then looks at you thoughtfully!", 
                            Monnam(mon));
                    break;
                case PM_FAMINE:
                    if (canseemon(mon))
                        pline("%s looks unusually hungry!", Monnam(mon));
                    break;
                case PM_PESTILENCE:
                    if (canseemon(mon))
                        pline("%s looks unusually well!", Monnam(mon));
                    break;
                default:
                    if (mon->data->msound == MS_NEMESIS && canseemon(mon) && your_fault)
                        pline("%s curses your ancestors!", Monnam(mon));
                    else if (mon->isshk) {
                        angermon = FALSE;
                        if (canseemon(mon))
                            pline("%s looks at you curiously!", Monnam(mon));
                        make_happy_shk(mon, FALSE);
                        /* Forget you were banned. */
                        ESHK(mon)->pbanned = FALSE;
                    } else if (!is_covetous(mon->data) && !rn2(4) &&
                            !resist(mon, POTION_CLASS, 0, 0)) {
                        angermon = FALSE;
                        if (canseemon(mon)) {
                            if (mon->msleeping) {
                                wakeup(mon, FALSE);
                                pline("%s wakes up looking bewildered!", 
                                        Monnam(mon));
                            } else {
                                pline("%s looks bewildered!", Monnam(mon));
                            }
                            mon->mpeaceful = TRUE;
                            mon->mtame = FALSE;	
                        }
                    }
                    break;
            }
            /* should only be by you */
            if (DEADMONSTER(mon))
                killed(mon);
            break;
        case POT_OIL:
            if (obj->lamplit)
                explode_oil(obj, tx, ty);
            /* no Glib for monsters */
            break;
        case POT_ACID:
            if (!(resists_acid(mon) || defended(mon, AD_ACID))
                && !resist(mon, POTION_CLASS, 0, NOTELL)) {
                pline("%s %s in pain!", Monnam(mon),
                      is_silent(mon->data) ? "writhes" : "shrieks");
                if (!is_silent(mon->data))
                    wake_nearto(tx, ty, mon->data->mlevel * 10);
                damage_mon(mon, d(obj->cursed ? 2 : 1, obj->blessed ? 4 : 8), AD_ACID);
                if (DEADMONSTER(mon)) {
                    if (your_fault)
                        killed(mon);
                    else
                        monkilled(mon, "", AD_ACID);
                }
            }
            break;
        case POT_REGENERATION:
            if (obj->cursed) {
                pline("%s begins to wither!", Monnam(mon));
                mon->mwither = 1;
            }
            break;
        case POT_POLYMORPH:
            (void) bhitm(mon, obj);
            break;
        /*
        case POT_GAIN_LEVEL:
        case POT_LEVITATION:
        case POT_FRUIT_JUICE:
        case POT_MONSTER_DETECTION:
        case POT_OBJECT_DETECTION:
            break;
        */
        }
        /* target might have been killed */
        if (!DEADMONSTER(mon)) {
            if (angermon)
                wakeup(mon, TRUE);
            else
                mon->msleeping = 0;
        }
    }

    /* Note: potionbreathe() does its own docall() */
    if ((distance == 0 || (distance < 3 && !rn2((1 + ACURR(A_DEX)) / 2)))
        && (!Breathless || haseyes(youmonst.data)))
        potionbreathe(obj);
    else if (obj->dknown && !objects[obj->otyp].oc_name_known
             && !objects[obj->otyp].oc_uname && cansee(tx, ty))
        docall(obj);

    if (*u.ushops && obj->unpaid) {
        struct monst *shkp = shop_keeper(*in_rooms(u.ux, u.uy, SHOPBASE));

        /* neither of the first two cases should be able to happen;
           only the hero should ever have an unpaid item, and only
           when inside a tended shop */
        if (!shkp) /* if shkp was killed, unpaid ought to cleared already */
            obj->unpaid = 0;
        else if (context.mon_moving) /* obj thrown by monster */
            subfrombill(obj, shkp);
        else /* obj thrown by hero */
            (void) stolen_value(obj, u.ux, u.uy, (boolean) shkp->mpeaceful,
                                FALSE);
    }
    obfree(obj, (struct obj *) 0);
}

/* vapors are inhaled or get in your eyes */
void
potionbreathe(obj)
register struct obj *obj;
{
    int i, ii, isdone;
    boolean cureblind = FALSE;
    boolean unambiguous = FALSE; /* if effect is unambiguous, call makeknown */
    boolean breathe = !Breathless;
    boolean cansmell = breathe && olfaction(youmonst.data);
    boolean eyes = eyecount(youmonst.data);
    const char * eyestr =
        (eyes > 1 ? makeplural(body_part(EYE)) : body_part(EYE));
    unsigned already_in_use = obj->in_use;

    if (!breathe) {
        /* currently only acid affects eyes */
        if (eyes && obj->otyp == POT_ACID && how_resistant(ACID_RES) < 50) {
            pline_The("fumes sting your %s.", eyestr);
        }
        else {
            pline_The("vapors don't seem to affect you.");
        }
        return;
    }

    /* after this, can assume the player is breathing the vapors so should be
     * affected by any potion effects; however, they still might not be able to
     * smell them */

    /* potion of unholy water might be wielded; prevent
       you_were() -> drop_weapon() from dropping it so that it
       remains in inventory where our caller expects it to be */
    obj->in_use = 1;

    /* wearing a wet towel protects both eyes and breathing, even when
       the breath effect might be beneficial; we still pass down to the
       naming opportunity in case potion was thrown at hero by a monster */
    switch (Half_gas_damage ? TOWEL : obj->otyp) {
    case TOWEL:
        pline("Some vapor passes harmlessly around you.");
        break;

    case POT_RESTORE_ABILITY:
    case POT_GAIN_ABILITY:
        if (obj->cursed) {
            if (cansmell)
                pline("Ulch!  That potion smells terrible!");
            break;
        } else {
            /* restore 1 point in each lost attribute */
            i = rn2(A_MAX); /* start at a random point */
            for (isdone = ii = 0; !isdone && ii < A_MAX; ii++) {
                if (ABASE(i) < AMAX(i)) {
                    ABASE(i)++;
                    /* only first found if not blessed */
                    isdone = !(obj->blessed);
                    context.botl = 1;
                }
                if (++i >= A_MAX)
                    i = 0;
            }
            if (cansmell) {
                pline("Wow!  That potion smells good!");
            }
        }
        break;
     case POT_GAIN_ENERGY:
        u.uen += rnd(2);
        if (u.uen > u.uenmax) {
            u.uenmax++;
            u.uen = u.uenmax;
        }
        You_feel("a brief rush of magical energy.");
        unambiguous = TRUE;
        break;
    case POT_POLYMORPH:
        You_feel("a change coming over you, but it peters out.");
        unambiguous = TRUE;
        break;
    case POT_FULL_HEALING:
         if (Upolyd)
            u.mh += 10;
        else
            u.uhp += 10;
        cureblind = TRUE;
        /*FALLTHRU*/
    case POT_EXTRA_HEALING:
      if (Upolyd)
            u.mh += 2;
        else
            u.uhp += 2;
        if (!obj->cursed)
            cureblind = TRUE;
        /*FALLTHRU*/
    case POT_HEALING:
        if (Upolyd)
            u.mh++;
        else
            u.uhp++;
        if (u.mh > u.mhmax) {
            u.mh = u.mhmax;
        }
        if (u.uhp > u.uhpmax) {
            u.uhp = u.uhpmax;
        }
        context.botl = 1;
        if (obj->blessed)
            cureblind = TRUE;
        if (cureblind) {
            make_blinded(0L, !u.ucreamed);
            make_deaf(0L, TRUE);
        }
        exercise(A_CON, TRUE);
        You_feel("a little better.");
        break;
    case POT_REGENERATION:
        if (obj->cursed) {
            incr_itimeout(&HWithering, rn1(5, 5));
            exercise(A_CON, FALSE);
            You("start to shrivel up!");
        } else {
            You_feel("a tiny bit better.");
            set_itimeout(&HRegeneration, rn1(5, 5));
            unambiguous = TRUE;
        }
        context.botl = TRUE;
        break;
    case POT_SICKNESS:
        if (!Role_if(PM_HEALER)) {
            if (Upolyd) {
                if (u.mh <= 5)
                    u.mh = 1;
                else
                    u.mh -= 5;
            } else {
                if (u.uhp <= 5)
                    u.uhp = 1;
                else
                    u.uhp -= 5;
            }
            context.botl = 1;
            exercise(A_CON, FALSE);
        }
        if (cansmell) {
            You("smell something diseased.");
            unambiguous = TRUE;
        }
        break;
    case POT_HALLUCINATION: {
        if (!(dmgtype(youmonst.data, AD_HALU)
            || u.umonnum == PM_VIOLET_FUNGUS
            || dmgtype(youmonst.data, AD_STUN))) {
            boolean chg = FALSE;
            if (!Hallucination)
                /* This has a longer effect than other potions because the
                * effect if quaffed is considerably longer than those other
                * potions. Also the case for blindness vapors. */
                chg =
                    make_hallucinated(HHallucination + (long) rn1(20, 20), FALSE, 0L);
            You("%s.", chg ? "are freaked out"
                : "have a momentary vision, but are otherwise unaffected");
            unambiguous = TRUE;
        } else {
            pline("Nothing seems to happen.");
        }
        break;
    }
    case POT_BOOZE:
        /* a whiff of alcohol isn't going to instantly confuse anyone */
        if (cansmell) {
            /* "peculiar odor" or "puff of vapor" message printed before this */
            pline("It smells like alcohol.");
            unambiguous = TRUE;
        }
        break;
    case POT_CONFUSION:
        if (!Confusion)
            You_feel("somewhat dizzy.");
        make_confused(itimeout_incr(HConfusion, rn1(10, 5)), FALSE);
        unambiguous = TRUE;
        break;
    case POT_INVISIBILITY:
        if (!Blind && !Invis) {
            pline("For an instant you %s!",
                  See_invisible ? "could see right through yourself"
                                : "couldn't see yourself");
            unambiguous = TRUE;
        }
        break;
    case POT_PARALYSIS:
        if (!Free_action) {
            pline("%s seems to be holding you.", Something);
            nomul(-rnd(5));
            multi_reason = "frozen by a potion";
            nomovemsg = You_can_move_again;
            exercise(A_DEX, FALSE);
        } else
            You("stiffen momentarily.");
        unambiguous = TRUE;
        break;
    case POT_SLEEPING:
        if (!Free_action && how_resistant(SLEEP_RES) < 100) {
            You_feel("rather tired.");
            nomul(-resist_reduce(rnd(5), SLEEP_RES));
            multi_reason = "sleeping off a magical draught";
            nomovemsg = You_can_move_again;
            exercise(A_DEX, FALSE);
        } else {
            monstseesu(M_SEEN_SLEEP);
            You("yawn.");
        }
        unambiguous = TRUE;
        break;
    case POT_SPEED:
        if (!Fast && !Slow)
            Your("knees seem more flexible now.");
        unambiguous = TRUE;
        incr_itimeout(&HFast, rnd(5));
        exercise(A_DEX, TRUE);
        break;
    case POT_REFLECTION:
        You("are covered in a mirror-like sheen!");
        set_itimeout(&HReflecting, rn1(5, 15));
        unambiguous = TRUE;
        break;
    case POT_ESP:
        You_feel(Hallucination 
                     ? "more in touch with the cosmos." 
                     : "more mentally acute.");
        incr_itimeout(&HTelepat, rn1(5, 15));
        unambiguous = TRUE;
        break;
    case POT_INVULNERABILITY:
        incr_itimeout(&Invulnerable, rn1(2, 4));
        You_feel(Hallucination ? "like a super-duper hero!" : "invulnerable!");
        unambiguous = TRUE;
        break;
    case POT_CLAIRVOYANCE:
        incr_itimeout(&HClairvoyant,  rn1(5, 25));
        break;
    case POT_BLINDNESS:
        if (!Blind && !Unaware) {
            pline("It suddenly gets dark.");
            unambiguous = TRUE;
        }
        make_blinded(itimeout_incr(Blinded, rnd(5)), FALSE);
        if (!Blind && !Unaware)
            Your1(vision_clears);
        break;
    case POT_WATER:
        if (u.umonnum == PM_GREMLIN) {
            (void) split_mon(&youmonst, (struct monst *) 0);
            unambiguous = TRUE;
        } else if (u.ulycn >= LOW_PM) {
            /* vapor from [un]holy water will trigger
               transformation but won't cure lycanthropy */
            if (obj->blessed && youmonst.data == &mons[u.ulycn]) {
                you_unwere(FALSE);
                unambiguous = TRUE;
            } else if (obj->cursed && !Upolyd) {
                you_were();
                unambiguous = TRUE;
            }
        }
        break;
    case POT_AMNESIA:
        if (u.umonnum == PM_GREMLIN)
            (void) split_mon(&youmonst, (struct monst *)0);
        else if (u.umonnum == PM_FLAMING_SPHERE) {
            You("flicker!");
            losehp(d(1, 6),"potion of amnesia", KILLED_BY_AN);
        } else if (u.umonnum == PM_IRON_GOLEM) {
            You("rust!");
            losehp(d(1,6),"potion of amnesia", KILLED_BY_AN);
        }
        You_feel("dizzy!");
        forget(1 + rn2(5));
        unambiguous = TRUE;
        break;
    case POT_ACID:
        if (how_resistant(ACID_RES) > 50) {
            if (cansmell) {
                pline("It smells %s.", Hallucination ? "tangy" : "sour");
                unambiguous = TRUE;
            }
        }
        else {
            pline_The("%s fumes burn your %s and %s!",
                    (Hallucination ? "amaroidal" : "acrid"),
                    (eyes ? eyestr : ""),
                    makeplural(body_part(LUNG)));
            losehp(rnd(2), "acid fumes", KILLED_BY);
            exercise(A_CON, FALSE);
            unambiguous = TRUE;
        }
        break;
    case POT_BLOOD:
    case POT_VAMPIRE_BLOOD:
        if (maybe_polyd(is_vampire(youmonst.data), Race_if(PM_VAMPIRIC))) {
            exercise(A_WIS, FALSE);
            You_feel("a %ssense of loss.",
                     obj->otyp == POT_VAMPIRE_BLOOD ? "terrible " : "");
        } else
            exercise(A_CON, FALSE);
        break;
    case POT_GAIN_LEVEL:
        more_experienced(5, 0);
        /* FALLTHRU */
    case POT_LEVITATION:
        You_feel("slightly elevated.");
        /* Strictly speaking, this is only unambiguous if the player has showexp
         * turned on or has it off but gains a level in the process, but it's
         * probably better not to make the identification here not based on the
         * current options. */
        unambiguous = TRUE;
        break;
    case POT_SEE_INVISIBLE:
        if (!obj->cursed) {
            make_blinded(0L, TRUE);
        }
        if (!See_invisible) {
            You("think you saw something invisible, but it vanished.");
            unambiguous = TRUE;
        }
        break;
    case POT_FRUIT_JUICE:
        if (cansmell) {
            pline("It smells %s.",
                  (obj->cursed ? (Hallucination ? "overripe" : "rotten")
                               : "sweet"));
            unambiguous = TRUE;
        }
        break;
    case POT_MONSTER_DETECTION:
        /* breathing: force uncursed monster detection if blessed */
        obj->blessed = 0;
        if (!monster_detect((struct obj *) 0, 0))
            exercise(A_WIS, TRUE);
        unambiguous = TRUE;
        break;
    case POT_OBJECT_DETECTION:
        /* force uncursed object detection if blessed */
        obj->blessed = 0;
        if (!object_detect(obj, 0))
            exercise(A_WIS, TRUE);
        unambiguous = TRUE;
        break;
    case POT_ENLIGHTENMENT:
        You("have a brief moment of introspection.");
        unambiguous = TRUE;
        break;
    }
    
    if (!already_in_use)
        obj->in_use = 0;
    /* note: no obfree() -- that's our caller's responsibility */
    if (obj->dknown) {
        if (unambiguous)
            makeknown(obj->otyp);
        else if (!objects[obj->otyp].oc_name_known
                 && !objects[obj->otyp].oc_uname)
            docall(obj);
    }
}

/* returns the potion type when o1 is dipped in o2 */
short
mixtype(o1, o2)
register struct obj *o1, *o2;
{
    int o1typ = o1->otyp, o2typ = o2->otyp;
    const char *potion_descr;
    const struct PotionRecipe *recipe;
    
    /* cut down on the number of cases below */
    if (o1->oclass == POTION_CLASS
        && (o2typ == POT_GAIN_LEVEL 
            || o2typ == POT_GAIN_ENERGY
            || o2typ == POT_HEALING 
            || o2typ == POT_EXTRA_HEALING
            || o2typ == POT_FULL_HEALING 
            || o2typ == POT_ENLIGHTENMENT
            || o2typ == POT_FRUIT_JUICE)) {
        /* swap o1 and o2 */
        o1typ = o2->otyp;
        o2typ = o1->otyp;
    }

    for (recipe = potionrecipes; recipe->result_typ; recipe++) {
        if ((o1typ == recipe->typ1 && o2typ == recipe->typ2)) {
            if (recipe->chance == 1 || !rn2(recipe->chance))
                return recipe->result_typ;
        }
    }
    
    /* MRKR: Extra alchemical effects. */
    if (o2->otyp == POT_ACID && o1->oclass == GEM_CLASS) {
        if (o1->otyp == DILITHIUM_CRYSTAL) {     /* white */
            /* explodes - special treatment in dodip */
            /* here we just want to return something non-zero */
            return POT_WATER;
        }
        potion_descr = gem_to_potion(o1->otyp);
        if (potion_descr) {
           return figure_out_potion(potion_descr);
        }
    }
    return STRANGE_OBJECT;
}

/* potion alchemy recipes */
const struct PotionRecipe potionrecipes[] = {
    /* ranged weapons */
    { POT_EXTRA_HEALING,    POT_HEALING, POT_SPEED,         1 },
    { POT_EXTRA_HEALING,    POT_HEALING, POT_GAIN_LEVEL,    1 },
    { POT_EXTRA_HEALING,    POT_HEALING, POT_GAIN_ENERGY,   1 },
    
    { POT_FULL_HEALING,     POT_EXTRA_HEALING, POT_GAIN_LEVEL, 1 },
    { POT_FULL_HEALING,     POT_EXTRA_HEALING, POT_GAIN_ENERGY,1 },
   
    { POT_GAIN_ABILITY,     POT_FULL_HEALING, POT_GAIN_LEVEL,  1 },
    { POT_GAIN_ABILITY,     POT_FULL_HEALING, POT_GAIN_ENERGY, 1 },
    
    { POT_FRUIT_JUICE,      UNICORN_HORN, POT_SICKNESS,      1 },
    { POT_WATER,            UNICORN_HORN, POT_HALLUCINATION, 1 },
    { POT_WATER,            UNICORN_HORN, POT_BLINDNESS,     1 },
    { POT_WATER,            UNICORN_HORN, POT_CONFUSION,     1 },
    { POT_WATER,            UNICORN_HORN, POT_BLOOD,         1 },
    { POT_WATER,            UNICORN_HORN, POT_VAMPIRE_BLOOD, 1 },
    
    { POT_FRUIT_JUICE,      AMETHYST,     POT_BOOZE,         1 },
    
    { POT_ENLIGHTENMENT,    POT_GAIN_LEVEL, POT_CONFUSION,   3 },
        /* If not enlightenment, than booze */
    { POT_BOOZE,            POT_GAIN_LEVEL, POT_CONFUSION,   1 },
    { POT_EXTRA_HEALING,    POT_GAIN_LEVEL, POT_HEALING,     1 },
    { POT_FULL_HEALING,     POT_GAIN_LEVEL, POT_EXTRA_HEALING, 1 },
    { POT_GAIN_ABILITY,     POT_GAIN_LEVEL, POT_FULL_HEALING,1 },
    { POT_SEE_INVISIBLE,    POT_GAIN_LEVEL, POT_FRUIT_JUICE, 1 },
    { POT_HALLUCINATION,    POT_GAIN_LEVEL, POT_BOOZE,       1 },

    { POT_ENLIGHTENMENT,    POT_GAIN_ENERGY, POT_CONFUSION,  3 },
        /* If not enlightenment, than booze */
    { POT_BOOZE,            POT_GAIN_ENERGY, POT_CONFUSION,  1 },
    { POT_EXTRA_HEALING,    POT_GAIN_ENERGY, POT_HEALING,    1 },
    { POT_FULL_HEALING,     POT_GAIN_ENERGY, POT_EXTRA_HEALING, 1 },
    { POT_GAIN_ABILITY,     POT_GAIN_ENERGY, POT_FULL_HEALING,  1 },
    { POT_SEE_INVISIBLE,    POT_GAIN_ENERGY, POT_FRUIT_JUICE,   1 },
    { POT_HALLUCINATION,    POT_GAIN_ENERGY, POT_BOOZE,      1 },
    
    { POT_SICKNESS,         POT_FRUIT_JUICE, POT_SICKNESS,   1 },
    { POT_BLOOD,            POT_FRUIT_JUICE, POT_BLOOD,      1 },
    { POT_VAMPIRE_BLOOD,    POT_FRUIT_JUICE, POT_VAMPIRE_BLOOD, 1 },
    { POT_BOOZE,            POT_FRUIT_JUICE, POT_ENLIGHTENMENT, 1 },
    { POT_BOOZE,            POT_FRUIT_JUICE, POT_SPEED,      1 },
    { POT_SEE_INVISIBLE,    POT_FRUIT_JUICE, POT_GAIN_LEVEL, 1 },
    { POT_SEE_INVISIBLE,    POT_FRUIT_JUICE, POT_GAIN_ENERGY,1 },

    { POT_VAMPIRE_BLOOD,    POT_BLOOD, POT_VAMPIRE_BLOOD, 1 },
    
    { POT_GAIN_LEVEL,       POT_ENLIGHTENMENT, POT_LEVITATION, 3 },
    { POT_BOOZE,            POT_ENLIGHTENMENT, POT_FRUIT_JUICE, 1 },
    { POT_CONFUSION,        POT_ENLIGHTENMENT, POT_BOOZE,       1 },
    
    { ELVEN_DAGGER,         POT_GAIN_ENERGY, KNIFE,         1 },
    { 0, 0, 0, 0 }
};

const char *
gem_to_potion(otyp)
int otyp;
{
    /* Note: you can't create smoky, milky or clear potions */
    switch (otyp) {
    case AGATE:
        return "swirly";
    case AMBER:
        return "amber";  /* yellowish brown */
    case AMETHYST:
        return "magenta"; /* violet */
    case AQUAMARINE:
        return "cyan";
    case BLACK_OPAL:
        return "black"; /* black */
    case CHRYSOBERYL:
        return "golden";
    case CITRINE:
        return "yellow"; /* yellow */
    case EMERALD:
        return "emerald"; /* green */
    case FLUORITE:
        return "white";
    case GARNET:
        return "pink";
    case JACINTH:
        return "orange"; /* orange */
    case JADE:
        return "dark green";
    case JASPER:
        return "puce";
    case JET:
        return "dark";
    case OBSIDIAN:
        return "effervescent";
    case OPAL:
        return "cloudy";
    case RUBY:
        return "ruby"; /* red */
    case SAPPHIRE:
        return "indigo"; /* blue */
    case TOPAZ:
        return "brown";
    case TURQUOISE:
        return "sky blue";
    case DIAMOND: /* won't dissolve */
    default:
        return NULL;
    }
}

int
potion_to_gem(potion_otyp)
int potion_otyp;
{
    const char *result;
    int i;
    for (i = bases[GEM_CLASS]; i <= LAST_GEM; i++) {
        result = gem_to_potion(i);
        if (result && !strcmp(result, OBJ_DESCR(objects[potion_otyp])))
            return i;
    }
    return 0;
}

int
figure_out_potion(pot_descr)
const char *pot_descr;
{
    int typ;

    /* find a potion that matches the description */
    for (typ = bases[POTION_CLASS];
         objects[typ].oc_class == POTION_CLASS; typ++) {
        if (strcmp(pot_descr, OBJ_DESCR(objects[typ])) == 0) {
            return typ;
        }
    }
    return 0;
}

short
mold_to_potion(corpsenm)
short corpsenm;
{
    if (corpsenm == PM_BROWN_MOLD)
        return POT_BLOOD;
    else if (corpsenm == PM_GREEN_MOLD)
        return POT_ACID;
    else if (corpsenm == PM_YELLOW_MOLD)
        return POT_CONFUSION;
    else if (corpsenm == PM_RED_MOLD)
        return POT_BOOZE; /* it "tastes like liquid fire" */
    else if (corpsenm == PM_VIOLET_FUNGUS)
        return POT_HALLUCINATION;
    else if (corpsenm == PM_GRAY_FUNGUS)
        return POT_AMNESIA;
    else if (corpsenm == PM_ORANGE_MOLD)
        return POT_SLEEPING;
    else if (corpsenm == PM_BLACK_MOLD)
        return POT_SICKNESS;
    return 0;
}

/* Bills an object that's about to be downgraded, assuming that's not already
 * been done */
STATIC_OVL void
pre_downgrade_obj(obj, used)
register struct obj *obj;
boolean *used;
{
    boolean dummy = FALSE;

    if (!used)
        used = &dummy;
    if (!*used)
        Your("%s for a moment.", aobjnam(obj, "sparkle"));
    if (obj->unpaid && costly_spot(u.ux, u.uy) && !*used) {
        You("damage it, you pay for it.");
        bill_dummy_object(obj);
    }
    *used = TRUE;
}

/* Implements the downgrading effect of potions of amnesia and Lethe water */
STATIC_OVL void
downgrade_obj(obj, nomagic, used)
register struct obj *obj;
int nomagic;	/* The non-magical object to downgrade to */
boolean *used;
{
    pre_downgrade_obj(obj, used);
    obj->otyp = nomagic;
    obj->spe = 0;
    obj->owt = weight(obj);
    context.botl = TRUE;
}

/* returns TRUE if something happened (potion should be used up)
    - Amnesia is a bit complicated
    - It gets things wet
    - It cancels objects
    - It downgrades objects
    - It disenchants items
    - It removes properties
    - It strips blessed/cursed status
    - It removes fooproofing
*/
boolean
amnesia_wet(potion, targobj, ourfault)
register struct obj *targobj;
struct obj *potion; /* potion of amnesia */
boolean ourfault;
{
    char Your_buf[BUFSZ];
    boolean used = FALSE;
    int res;
    
    if (!potion || potion->otyp != POT_AMNESIA)
        return FALSE;
    if (snuff_lit(targobj))
        return TRUE;

    if (targobj->greased) {
        /*TODO: Fix this so &youmonst is victim instead */
        grease_protect(targobj, (char *)0, &youmonst);
        return FALSE;
    }

    /* Sustainable items are immune to draining. */
    if (targobj && targobj->oprops & ITEM_SUSTAIN) {
        targobj->oprops_known |= ITEM_SUSTAIN;
        return FALSE;
    }
    
    (void) Shk_Your(Your_buf, targobj);

    /* (Rusting shop goods ought to be charged for.) */
    switch (targobj->oclass) {
    case POTION_CLASS:
        if (targobj->otyp == POT_AMNESIA)
            return FALSE;

        /* Create more amnesia potions by dipping water into them.
         * Curiously, the opposite is not true - dipping amnesia
         * into water sources converts it to water! */
        if (targobj->otyp == POT_WATER) {
            Your("%s to sparkle.", aobjnam(targobj, "start"));
            targobj->odiluted = 0;
            targobj->otyp = POT_AMNESIA;
            used = TRUE;
            break;
            return FALSE;
        }

        if (targobj->otyp == POT_ACID) {
            pline("It boils vigorously!");
            You("are caught in the explosion!");
            losehp(how_resistant(ACID_RES) > 50 ? rnd(5) : rnd(10),
                   "elementary chemistry", KILLED_BY);

            You_feel("a momentary lapse of reason!");
            forget(2 + rn2(3));

            makeknown(targobj->otyp);
            used = TRUE;
            break;
        }
        pline("%s%s completely.", Your_buf, aobjnam(targobj, "dilute"));
        if (targobj->unpaid && costly_spot(u.ux, u.uy)) {
            You("dilute it, you pay for it.");
            bill_dummy_object(targobj);
        }

        /* Unlike cancelling, amnesia can dilute anything (except itself)
         * That's why we are not using cancel_item() here */
        targobj->odiluted = 0;
        targobj->blessed = FALSE;
        targobj->cursed = FALSE;
        targobj->otyp = POT_WATER;
        used = TRUE;
        break;
    case SCROLL_CLASS:
        if (targobj->otyp != SCR_BLANK_PAPER && targobj->otyp != SCR_AMNESIA) {
            if (!Blind) {
                pline_The("scroll%s %s.", targobj->quan == 1L ? "" : "s",
                          otense(targobj, "fade"));
            }
            if (targobj->unpaid && costly_spot(u.ux, u.uy)) {
                You("erase it, you pay for it.");
                bill_dummy_object(targobj);
            }
            cancel_item(targobj);
            used = TRUE;
        }
        break;
    case SPBOOK_CLASS:
        if (targobj->otyp == SPE_BLANK_PAPER) {
            break;
        } else if (targobj->otyp == SPE_BOOK_OF_THE_DEAD) {
            pline("%s suddenly heats up; steam rises and it remains dry.",
                  The(xname(targobj)));
        } else {
            if (!Blind) {
                boolean oq1 = targobj->quan == 1L;
                pline_The("spellbook%s %s.",
                          oq1 ? "" : "s", otense(targobj, "fade"));
            }
            if (targobj->unpaid) {
                subfrombill(targobj, shop_keeper(*u.ushops));
                You("erase it, you pay for it.");
                bill_dummy_object(targobj);
            }
            cancel_item(targobj);
        }
        used = TRUE;
        break;
    case GEM_CLASS:
        if (targobj->otyp == LUCKSTONE
                        || targobj->otyp == LOADSTONE
                        || targobj->otyp == HEALTHSTONE
                        || targobj->otyp == TOUCHSTONE)
            downgrade_obj(targobj, FLINT, &used);
        break;
    case TOOL_CLASS:
        /* Artifacts aren't downgraded by amnesia */
        if (!targobj->oartifact) {
            switch (targobj->otyp) {
            case MAGIC_LAMP:
                downgrade_obj(targobj, OIL_LAMP, &used);
                break;
            case MAGIC_CANDLE:
                downgrade_obj(targobj, rn2(2)? WAX_CANDLE : TALLOW_CANDLE, &used);
                break;
            case DRUM_OF_EARTHQUAKE:
                downgrade_obj(targobj, LEATHER_DRUM, &used);
                break;
            case MAGIC_WHISTLE:
                downgrade_obj(targobj, PEA_WHISTLE, &used);
                break;
            case MAGIC_FLUTE:
                downgrade_obj(targobj, FLUTE, &used);
                break;
            case MAGIC_HARP:
                downgrade_obj(targobj, HARP, &used);
                break;
            case CRYSTAL_BALL:
                downgrade_obj(targobj, EIGHT_BALL, &used);
                break;
            case FIRE_HORN:
            case FROST_HORN:
            case HORN_OF_BLASTING:
            case HORN_OF_PLENTY:
                downgrade_obj(targobj, TOOLED_HORN, &used);
                break;
            case MAGIC_MARKER:
                if (targobj->spe > 0) {
                    pre_downgrade_obj(targobj, &used);
                    if ((targobj->spe -= (3 + rn2(10))) < 0)
                        targobj->spe = 0;
                }
                break;
            case BAG_OF_RATS:
            case BAG_OF_TRICKS:
                /* Bags of holding are kept safe somehow... */
                downgrade_obj(targobj, SACK, &used);
                break;
            case MASK:
                /* Masks are magical and therefore subject to amnesia. */
                pre_downgrade_obj(targobj, &used);
                if (targobj->where == OBJ_INVENT)
                    Your("%s transforms into something ordinary!", xname(targobj));
                else
                    pline_The("%s transforms into something ordinary!", xname(targobj));
                targobj->corpsenm = -1;
            }
        }

        /* The only other tools that can be affected are pick axes and
         * unicorn horns... */
        if (!is_weptool(targobj))
            break;
        /* Drop through for disenchantment and rusting... */
        /* fall through */

    case WAND_CLASS:
    case RING_CLASS:
    case ARMOR_CLASS:
    case WEAPON_CLASS:
    /* Just "fall through" to generic rustprone check for now. */
    /* fall through */
    default:
        if (artifact_wet(targobj))
            return TRUE;

        if (targobj->spe > 0) {
            pre_downgrade_obj(targobj, &used);
            drain_item(targobj, ourfault);
            if (targobj->spe < 0)
                targobj->spe = 0;
        } else if (targobj->age > 0) {
            if (is_lightsaber(targobj))
                targobj->age -= rn1(450, 300);
            if (targobj->age < 0)
                targobj->age = 0;
        }
        /* We're able to use the water_damage function, but some things still
         * need handling specifically. This has to come after most of the
         * cancel effects, otherwise the messages and effects don't really
         * make sense for this potion. */
        res = water_damage(targobj, 0, TRUE, u.ux, u.uy);

        if (res == ER_DAMAGED) {
            used = TRUE;
            if (targobj->unpaid && costly_spot(u.ux, u.uy) && !used) {
                You("damage it, you pay for it.");
                /*bill_dummy_object(targobj);*/
            }
        } else if (res == ER_DESTROYED)
            return TRUE;
        break;
    }
    /* !ofAmnesia might strip away fooproofing... */
    if (targobj->oerodeproof && !rn2(13)) {
        pre_downgrade_obj(targobj, &used);
        targobj->oerodeproof = FALSE;
    }

    /* !ofAmnesia might strip away properties... */
    if (targobj->oprops && !rn2(13)) {
        pre_downgrade_obj(targobj, &used);
        targobj->oprops = 0L;
        targobj->oprops_known = 0L;
    }

    if (targobj->cursed || targobj->blessed) {
        if (targobj->blessed || targobj->otyp == POT_WATER)
            pre_downgrade_obj(targobj, &used);
        else if (!used) {
            Your("%s for a moment.", aobjnam(targobj, "sparkle"));
            used = TRUE;
        }
        uncurse(targobj);
        unbless(targobj);
    }
    if (targobj->otyp == EGG) {
        /* sterilized */
        kill_egg(targobj);
        targobj->corpsenm = NON_PM;
        if (!used) {
            Your("%s for a moment.", aobjnam(targobj, "sparkle"));
            used = TRUE;
        }
    }

    if (used)
        update_inventory();
    else
        pline("%s%s wet.", Your_buf, aobjnam(targobj,"get"));

    return used;
}

/* #dip command */
int
dodip()
{
    static const char Dip_[] = "Dip ";
    register struct obj *potion, *obj;
    struct obj *singlepotion;
    uchar here;
    char allowall[2];
    short mixture;
    char qbuf[QBUFSZ], obuf[QBUFSZ];
    const char *shortestname; /* last resort obj name for prompt */

    allowall[0] = ALL_CLASSES;
    allowall[1] = '\0';
    if (!(obj = getobj(allowall, "dip")))
        return 0;
    if (inaccessible_equipment(obj, "dip", FALSE))
        return 0;
    if (Hidinshell) {
        You_cant("dip anything while hiding in your shell.");
        return 0;
    }

    shortestname = (is_plural(obj) || pair_of(obj)) ? "them" : "it";
    /*
     * Bypass safe_qbuf() since it doesn't handle varying suffix without
     * an awful lot of support work.  Format the object once, even though
     * the fountain and pool prompts offer a lot more room for it.
     * 3.6.0 used thesimpleoname() unconditionally, which posed no risk
     * of buffer overflow but drew bug reports because it omits user-
     * supplied type name.
     * getobj: "What do you want to dip <the object> into? [xyz or ?*] "
     */
    Strcpy(obuf, short_oname(obj, doname, thesimpleoname,
                             /* 128 - (24 + 54 + 1) leaves 49 for <object> */
                             QBUFSZ - sizeof "What do you want to dip \
 into? [abdeghjkmnpqstvwyzBCEFHIKLNOQRTUWXZ#-# or ?*] "));

    here = levl[u.ux][u.uy].typ;
    /* Is there a fountain to dip into here? */
    if (IS_FOUNTAIN(here)) {
        Sprintf(qbuf, "%s%s into the fountain?", Dip_,
                flags.verbose ? obuf : shortestname);
        /* "Dip <the object> into the fountain?" */
        if (yn(qbuf) == 'y') {
            dipfountain(obj);
            return 1;
        }
    } else if (IS_SINK(here)) {
        /* currently can only pour potions into sinks */
        if (obj->oclass == POTION_CLASS) {
            Sprintf(qbuf, "Pour %s into the sink?",
                    flags.verbose? obuf : shortestname);
            if (yn(qbuf) == 'y') {
                dipsink(obj);
                return 1;
            }
        }
        /* if not potion, do regular dip */
    } else if (IS_TOILET(here)) {
        Sprintf(qbuf, "%s%s into the toilet?", Dip_,
                flags.verbose ? obuf : shortestname);
        /* "Dip <the object> into the toilet?" */
        if (yn(qbuf) == 'y') {
            diptoilet(obj);
            return 1;
        }
    } else if (IS_FORGE(here)) {
        Sprintf(qbuf, "%s%s into the forge?", Dip_,
                flags.verbose ? obuf : shortestname);
        /* "Dip <the object> into the forge?" */
        if (yn(qbuf) == 'y') {
            dipforge(obj);
            return 1;
        }
    } else if (is_damp_terrain(u.ux, u.uy)) {
        const char *pooltype = waterbody_name(u.ux, u.uy);

        Sprintf(qbuf, "%s%s into the %s?", Dip_,
                flags.verbose ? obuf : shortestname, pooltype);
        /* "Dip <the object> into the {pool, moat, &c}?" */
        if (yn(qbuf) == 'y') {
            if (Levitation) {
                floating_above(pooltype);
            } else if (u.usteed && !is_swimmer(u.usteed->data)
                       && P_SKILL(P_RIDING) < P_BASIC) {
                rider_cant_reach(); /* not skilled enough to reach */
            } else if (IS_SEWAGE(here)) {
                if (rn2(3)) {
                    You_see("something moving under the surface and abruptly pull away your %s.",
                        xname(obj));
                } else if (obj_resists(obj, 0, 0) || obj == uball
                           || (is_worn(obj) && obj->cursed)) {
                    pline("A creature hiding under the surface tries to grab your %s, but cannot!",
                          xname(obj));
                } else {
                    pline("Without warning, a creature hiding in the muck snatches away your %s!",
                          xname(obj));
                    useupall(obj);
                }
            } else {
                if (obj->otyp == POT_ACID)
                    obj->in_use = 1;
                if (water_damage(obj, 0, TRUE, u.ux, u.uy) != ER_DESTROYED
                    && obj->in_use)
                    useup(obj);
                if (IS_PUDDLE(here) && !rn2(3)
                    && !On_stairs(u.ux, u.uy)) { /* if shallow water forms on stairs,
                                                    don't run risk of removing stairs */
                    /* shallow water isn't an endless resource like a pool/moat */
                    levl[u.ux][u.uy].typ = ROOM;
                    newsym(u.ux, u.uy);
                    maybe_unhide_at(u.ux, u.uy);
                    if (cansee(u.ux, u.uy))
                        pline_The("puddle dries up.");
                }
            }
            return 1;
        }
    }

    /* "What do you want to dip <the object> into? [xyz or ?*] " */
    Sprintf(qbuf, "dip %s into", flags.verbose ? obuf : shortestname);
    potion = getobj(dippables, qbuf);
    if (!potion)
        return 0;
    if (potion == obj && potion->quan == 1L) {
        pline("That is a potion bottle, not a Klein bottle!");
        return 0;
    }
    potion->in_use = TRUE; /* assume it will be used up */
    if (potion->otyp == POT_WATER) {
        boolean useeit = !Blind || (obj == ublindf && Blindfolded_only);
        const char *obj_glows = Yobjnam2(obj, "glow");

        if (H2Opotion_dip(potion, obj, useeit, obj_glows))
            goto poof;
    } else if (potion->otyp == POT_AMNESIA) {
        if (amnesia_wet(potion, obj, TRUE))
            goto poof;
    } else if (obj->otyp == POT_POLYMORPH || potion->otyp == POT_POLYMORPH) {
        /* some objects can't be polymorphed */
        if (obj->otyp == potion->otyp /* both POT_POLY */
            || obj->otyp == WAN_POLYMORPH 
            || obj->otyp == SPE_POLYMORPH
            || obj == uball 
            || obj == uskin
            || obj_resists(obj->otyp == POT_POLYMORPH ? 
                potion : obj, 5, 95)) {
            pline1(nothing_happens);
        } else {
            short save_otyp = obj->otyp;
            short save_dknown = obj->dknown;

            /* KMH, conduct */
            if(!u.uconduct.polypiles++)
                livelog_printf(LL_CONDUCT, "polymorphed %s first item", uhis());

            obj = poly_obj(obj, STRANGE_OBJECT);

            /*
             * obj might be gone:
             *  poly_obj() -> set_wear() -> Amulet_on() -> useup()
             * if obj->otyp is worn amulet and becomes AMULET_OF_CHANGE.
             */
            if (!obj) {
                if (potion->dknown)
                    makeknown(POT_POLYMORPH);
                return 1;
            } else if (obj->otyp != save_otyp || obj->dknown != save_dknown) {
                if (potion->dknown)
                    makeknown(POT_POLYMORPH);
                useup(potion);
                prinv((char *) 0, obj, 0L);
                return 1;
            } else {
                pline("Nothing seems to happen.");
                goto poof;
            }
        }
        potion->in_use = FALSE; /* didn't go poof */
        return 1;
    } else if (obj->oclass == POTION_CLASS && obj->otyp != potion->otyp) {
        int amt = (int) obj->quan;
        boolean magic;

        mixture = mixtype(obj, potion);

        magic = (mixture != STRANGE_OBJECT) ? objects[mixture].oc_magic
            : (objects[obj->otyp].oc_magic || objects[potion->otyp].oc_magic);
        Strcpy(qbuf, "The"); /* assume full stack */
        if (amt > (magic ? 3 : 7)) {
            /* trying to dip multiple potions will usually affect only a
               subset; pick an amount between 3 and 8, inclusive, for magic
               potion result, between 7 and N for non-magic */
            if (magic)
                amt = rnd(min(amt, 8) - (3 - 1)) + (3 - 1); /* 1..6 + 2 */
            else
                amt = rnd(amt - (7 - 1)) + (7 - 1); /* 1..(N-6) + 6 */

            if ((long) amt < obj->quan) {
                obj = splitobj(obj, (long) amt);
                Sprintf(qbuf, "%ld of the", obj->quan);
            }
        }
        /* [N of] the {obj(s)} mix(es) with [one of] {the potion}... */
        pline("%s %s %s with %s%s...", qbuf, simpleonames(obj),
              otense(obj, "mix"), (potion->quan > 1L) ? "one of " : "",
              thesimpleoname(potion));
        /* get rid of 'dippee' before potential perm_invent updates */
        useup(potion); /* now gone */
        /* Mixing potions is dangerous...
           KMH, balance patch -- acid is particularly unstable */
        if (obj->cursed || obj->otyp == POT_ACID || !rn2(10)) {
            /* it would be better to use up the whole stack in advance
               of the message, but we can't because we need to keep it
               around for potionbreathe() [and we can't set obj->in_use
               to 'amt' because that's not implemented] */
            int dmg = (amt + rnd(9)) * (Acid_resistance ? 1 : 2);
            obj->in_use = 1;
            pline("%sThey explode!", !Deaf ? "BOOM!  " : "");
            wake_nearto(u.ux, u.uy, (BOLT_LIM + 1) * (BOLT_LIM + 1));
            exercise(A_STR, FALSE);
            if (!Breathless || haseyes(youmonst.data))
                potionbreathe(obj);
            useupall(obj);
            losehp(dmg, /* not physical damage */
                   "alchemic blast", KILLED_BY_AN);
            return 1;
        }

        obj->blessed = obj->cursed = obj->bknown = 0;
        if (Blind || Hallucination)
            obj->dknown = 0;

        if (mixture != STRANGE_OBJECT) {
            obj->otyp = mixture;
        } else {
            struct obj *otmp;

            switch (obj->odiluted ? 1 : rnd(8)) {
            case 1:
                obj->otyp = POT_WATER;
                break;
            case 2:
            case 3:
                obj->otyp = POT_SICKNESS;
                break;
            case 4:
                otmp = mkobj(POTION_CLASS, FALSE);
                obj->otyp = otmp->otyp;
                obfree(otmp, (struct obj *) 0);
                break;
            default:
                useupall(obj);
                pline_The("mixture %sevaporates.",
                          !Blind ? "glows brightly and " : "");
                return 1;
            }
        }
        obj->odiluted = (obj->otyp != POT_WATER);

        if (obj->otyp == POT_WATER && !Hallucination) {
            pline_The("mixture bubbles%s.", Blind ? "" : ", then clears");
        } else if (!Blind) {
            pline_The("mixture looks %s.",
                      hcolor(OBJ_DESCR(objects[obj->otyp])));
        }

        /* this is required when 'obj' was split off from a bigger stack,
           so that 'obj' will now be assigned its own inventory slot;
           it has a side-effect of merging 'obj' into another compatible
           stack if there is one, so we do it even when no split has
           been made in order to get the merge result for both cases;
           as a consequence, mixing while Fumbling drops the mixture */
        freeinv(obj);
        (void) hold_another_object(obj, "You drop %s!", doname(obj),
                                   (const char *) 0);
        return 1;
    }

    if (potion->otyp == POT_ACID && obj->otyp == CORPSE
        && obj->corpsenm == PM_LICHEN) {
        pline("%s %s %s around the edges.", The(cxname(obj)),
              otense(obj, "turn"), Blind ? "wrinkled"
                                   : potion->odiluted ? hcolor(NH_ORANGE)
                                     : hcolor(NH_RED));
        potion->in_use = FALSE; /* didn't go poof */
        if (potion->dknown
            && !objects[potion->otyp].oc_name_known
            && !objects[potion->otyp].oc_uname)
            docall(potion);
        return 1;
    }

    if (potion->otyp == POT_WATER && obj->otyp == TOWEL) {
        pline_The("towel soaks it up!");
        /* wetting towel already done via water_damage() in H2Opotion_dip */
        goto poof;
    }

    if (is_poisonable(obj)) {
        if (potion->otyp == POT_SICKNESS && !obj->opoisoned) {
            char buf[BUFSZ];

            if (potion->quan > 1L)
                Sprintf(buf, "One of %s", the(xname(potion)));
            else
                Strcpy(buf, The(xname(potion)));
            pline("%s forms a coating on %s.", buf, the(xname(obj)));
            obj->opoisoned = TRUE;
            goto poof;
        } else if (obj->opoisoned && (potion->otyp == POT_HEALING
                                      || potion->otyp == POT_EXTRA_HEALING
                                      || potion->otyp == POT_FULL_HEALING)) {
            pline("A coating wears off %s.", the(xname(obj)));
            obj->opoisoned = 0;
            goto poof;
        }
    }

    if (potion->otyp == POT_ACID) {
        if (erode_obj(obj, 0, ERODE_CORRODE, EF_GREASE | EF_DESTROY) != ER_NOTHING)
            goto poof;
    }

    if (potion->otyp == POT_OIL) {
        boolean wisx = FALSE;

        if (potion->lamplit) { /* burning */
            fire_damage(obj, TRUE, u.ux, u.uy);
        } else if (potion->cursed) {
            pline_The("potion spills and covers your %s with oil.",
                      fingers_or_gloves(TRUE));
            make_glib((int) (Glib & TIMEOUT) + d(2, 10));
        } else if (obj->otyp == CLUB) {
            /* From DNetHack */
            You("make a torch from your club and the oil.");
            obj->otyp = TORCH;
            obj->age = (long) rn1(300, 600);
            obj->oclass = TOOL_CLASS;
            obj->owt = weight(obj);
            if (potion->dknown && !objects[potion->otyp].oc_name_known)
                makeknown(potion->otyp);
            goto poof;
        } else if (obj->oclass != WEAPON_CLASS && !is_weptool(obj)) {
            /* the following cases apply only to weapons */
            goto more_dips;
            /* Oil removes rust and corrosion, but doesn't unburn.
             * Arrows, etc are classed as metallic due to arrowhead
             * material, but dipping in oil shouldn't repair them.
             */
        } else if ((!is_rustprone(obj) && !is_corrodeable(obj))
                   || is_ammo(obj) || (!obj->oeroded && !obj->oeroded2)) {
            /* uses up potion, doesn't set obj->greased */
            if (!Blind)
                pline("%s %s with an oily sheen.", Yname2(obj),
                      otense(obj, "gleam"));
            else /*if (!uarmg)*/
                pline("%s %s oily.", Yname2(obj), otense(obj, "feel"));
        } else {
            pline("%s %s less %s.", Yname2(obj),
                  otense(obj, !Blind ? "are" : "feel"),
                  (obj->oeroded && obj->oeroded2)
                      ? "corroded and rusty"
                      : obj->oeroded ? "rusty" : "corroded");
            if (obj->oeroded > 0)
                obj->oeroded--;
            if (obj->oeroded2 > 0)
                obj->oeroded2--;
            wisx = TRUE;
        }
        exercise(A_WIS, wisx);
        if (potion->dknown)
            makeknown(potion->otyp);
        useup(potion);
        return 1;
    } else if (potion->otyp == POT_GAIN_LEVEL) {
        int res = 0;
        obj = upgrade_obj(obj, &res);
        if (res != 0) {
            if (res == 1) {
                /* The object was upgraded */
                pline("Hmm!  You don't recall dipping that into the potion.");
                
                if (carried(obj))
                    prinv((char *) 0, obj, 0L);
                /* Wisdom on successful upgrades */
                exercise(A_WIS, TRUE);
            } /* else potion exploded */
            if (!objects[potion->otyp].oc_name_known
                && !objects[potion->otyp].oc_uname)
                docall(potion);
            useup(potion);
            update_inventory();
            return 1;
        }
    } else if (potion->otyp == POT_INVULNERABILITY) {
        if (obj->oartifact)
            ; /* Already pretty indestructible */
        else if (obj->oprops & ITEM_TOUGH)
            ; /* Already has toughness */
        else if (is_missile(obj) || is_ammo(obj))
            ; /* Ammo and missiles not eligible */
        else if (obj->oclass != WEAPON_CLASS 
                && obj->oclass != ARMOR_CLASS
                && obj->oclass != AMULET_CLASS
                && obj->oclass != TOOL_CLASS
                && obj->oclass != RING_CLASS)
            ; /* Some items should not be indestructible: 
 *              * food, potions, scrolls, etc */
        else {
            obj->oprops |= ITEM_TOUGH;
            obj->oprops_known |= ITEM_TOUGH;
            pline("%s covered by a %s %s %s!", Yobjnam2(obj, "are"),
                  obj->cursed ? "mottled" : "shimmering",
                  hcolor(obj->cursed ? NH_BLACK : NH_GOLDEN),
                  obj->cursed ? "glow"
                          : (is_shield(obj) ? "layer" : "shield"));
            useup(potion);
            update_inventory();
            return 1;
        }
    }
     /* removing erosion from items */
     else if (potion->otyp == POT_RESTORE_ABILITY && !potion->cursed
             && erosion_matters(obj) && (obj->oeroded || obj->oeroded2)) {
        obj->oeroded = obj->oeroded2 = 0;
        pline("%s as good as new!", Yobjnam2(obj, Blind ? "feel" : "look"));
        if (potion->dknown)
            makeknown(POT_RESTORE_ABILITY);
        useup(potion);
        update_inventory();
        return 1;
    }
    /* no return here, go for Interesting... message */
 more_dips:
        
    /* Allow filling of MAGIC_LAMPs to prevent identification by player */
    if ((obj->otyp == OIL_LAMP || obj->otyp == MAGIC_LAMP)
        && (potion->otyp == POT_OIL)) {
        /* Turn off engine before fueling, turn off fuel too :-)  */
        if (obj->lamplit || potion->lamplit) {
            useup(potion);
            explode(u.ux, u.uy, ZT_SPELL(ZT_FIRE), d(6, 6), 0, EXPL_FIERY);
            exercise(A_WIS, FALSE);
            return 1;
        }
        /* Adding oil to an empty magic lamp renders it into an oil lamp */
        if ((obj->otyp == MAGIC_LAMP) && obj->spe == 0) {
            obj->otyp = OIL_LAMP;
            obj->age = 0;
        }
        if (obj->age > (MAX_LAMP_FUEL * 2 / 3)) {
            pline("%s %s full.", Yname2(obj), otense(obj, "are"));
            potion->in_use = FALSE; /* didn't go poof */
        } else {
            You("fill %s with oil.", yname(obj));
            check_unpaid(potion);        /* Yendorian Fuel Tax */
            /* burns more efficiently in a lamp than in a bottle;
               diluted potion provides less benefit but we don't attempt
               to track that the lamp now also has some non-oil in it */
            obj->age += (!potion->odiluted ? 4L : 3L) * potion->age / 2L;
            if (obj->age > MAX_LAMP_FUEL)
                obj->age = MAX_LAMP_FUEL;
            useup(potion);
            exercise(A_WIS, TRUE);
        }
        makeknown(POT_OIL);
        obj->spe = 1;
        update_inventory();
        return 1;
    }

    potion->in_use = FALSE; /* didn't go poof */
    if ((obj->otyp == UNICORN_HORN || obj->oclass == GEM_CLASS)
        && (mixture = mixtype(obj, potion)) != STRANGE_OBJECT) {
        char oldbuf[BUFSZ], newbuf[BUFSZ];
        short old_otyp = potion->otyp;
        boolean old_dknown = FALSE;
        boolean more_than_one = potion->quan > 1L;

        oldbuf[0] = '\0';
        if (potion->dknown) {
            old_dknown = TRUE;
            Sprintf(oldbuf, "%s ", hcolor(OBJ_DESCR(objects[potion->otyp])));
        }
        /* with multiple merged potions, split off one and
           just clear it */
        if (potion->quan > 1L) {
            singlepotion = splitobj(potion, 1L);
        } else
            singlepotion = potion;

        /* MRKR: Gems dissolve in acid to produce new potions */
        if (obj->oclass == GEM_CLASS && potion->otyp == POT_ACID) {
            struct obj *singlegem = (obj->quan > 1L ?  splitobj(obj, 1L) : obj);
            
            if (potion->otyp == POT_ACID &&
                (obj->otyp == DILITHIUM_CRYSTAL || potion->cursed || !rn2(30))) {
                /* Just to keep them on their toes */

                if (Hallucination && obj->otyp == DILITHIUM_CRYSTAL) {
                    /* Thanks to Robin Johnson */
                    pline("Warning, Captain!  The warp core has been breached!");
                }
                pline("BOOM! %s explodes!", The(xname(singlegem)));
                if (obj->otyp == DILITHIUM_CRYSTAL) {
                    tele();
                }
                exercise(A_STR, FALSE);
                if (!Breathless || haseyes(youmonst.data)) {
                    potionbreathe(singlepotion);
                }
                useup(singlegem);
                useup(singlepotion);
                /* MRKR: an alchemy smock ought to be */
                /* some protection against this: */
                losehp(how_resistant(ACID_RES) > 50 ? rnd(5) : rnd(10), "alchemic blast", KILLED_BY_AN);

                return 1;
            }

            pline("%s dissolves in %s.", The(xname(singlegem)), the(xname(singlepotion)));
            makeknown(POT_ACID);
            useup(singlegem);
        }
        
        costly_alteration(singlepotion, COST_NUTRLZ);
        singlepotion->otyp = mixture;
        singlepotion->blessed = singlepotion->cursed = 0;
        singlepotion->odiluted = 0;
        singlepotion->bknown = FALSE;

        if (Blind) {
            singlepotion->dknown = FALSE;
        } else {
            singlepotion->dknown = !Hallucination;
            *newbuf = '\0';
            if (mixture == POT_WATER && singlepotion->dknown)
                Sprintf(newbuf, "clears");
            else if (!Blind)
                Sprintf(newbuf, "turns %s",
                        hcolor(OBJ_DESCR(objects[mixture])));
            if (*newbuf)
                pline_The("%spotion%s %s.", oldbuf,
                          more_than_one ? " that you dipped into" : "",
                          newbuf);
            else
                pline("Something happens.");

            if (old_dknown
                && !objects[old_otyp].oc_name_known
                && !objects[old_otyp].oc_uname) {
                struct obj fakeobj;

                fakeobj = zeroobj;
                fakeobj.dknown = 1;
                fakeobj.otyp = old_otyp;
                fakeobj.oclass = POTION_CLASS;
                docall(&fakeobj);
            }
        }
        obj_extract_self(singlepotion);
        singlepotion = hold_another_object(singlepotion,
                                           "You juggle and drop %s!",
                                           doname(singlepotion),
                                           (const char *) 0);
        nhUse(singlepotion);
        update_inventory();
        return 1;
    }

    if (obj->otyp == CORPSE && potion->otyp == POT_FRUIT_JUICE) {
        short result = mold_to_potion(obj->corpsenm);
        if (result) {
            /* MRKR: Molds and fungi have various medicinal properties */
            /* AOS: This code could possibly be merged into the stuff above,
                 * but ultimately I felt that there would be too much of the
                 * above code that we have to make an exception for in this
                 * case. The dipped potion isn't actually changing right now,
                 * and its dknown, beatitude etc aren't affected. */
            pline("%s dissolves and the liquid begins fizzing %s.",
                  The(cxname_singular(obj)),
                  (objdescr_is(potion, "fizzy")
                   || objdescr_is(potion, "effervescent"))
                      ? "even more"
                      : "slowly");

            /* order matters: split first, then start the timer, then try to
                 * hold_another_object.  Previously this segfaulted when
                 * hold_another_object was called first, the hero dropped the
                 * potion, and then the timer tried to start on its NULL return
                 * value. */
            if (potion->quan > 1)
                singlepotion = splitobj(potion, 1);
            else
                singlepotion = potion;

            singlepotion->corpsenm = obj->corpsenm;

            if (obj->cursed)
                /* placeholder for "turn into sickness instead" */
                singlepotion->corpsenm = PM_PESTILENCE;

            useup(obj);
            obj_extract_self(singlepotion);
            start_timer(50 + rn2(50), TIMER_OBJECT, FERMENT,
                        obj_to_any(singlepotion));
            costly_alteration(singlepotion, COST_FERMENT);
            singlepotion =
                hold_another_object(singlepotion, "You juggle and drop %s!",
                                    doname(singlepotion), NULL);
            update_inventory();
            return 1;

            /* other corpses won't do anything special and will
             * just fall through to "Interesting..." below.
             **/
        }
    }

    pline("Interesting...");
    return 1;

 poof:
    if (potion->dknown
        && !objects[potion->otyp].oc_name_known
        && !objects[potion->otyp].oc_uname)
        docall(potion);
    useup(potion);
    return 1;
}

#define UNUSED_PARAM(x) (void)(x)

void
ferment(arg, timeout)
anything* arg;
long timeout;
{
    struct obj* potion = arg->a_obj;
    short corpsenm = potion->corpsenm;
    boolean need_newsym = FALSE;
    xchar x, y;
    short new_otyp;
    UNUSED_PARAM(timeout); /* Silence compiler warning */
                     
    if (!potion) {
        impossible("null potion fermenting?");
        return;
    }

    /* if it has been transformed in the meantime, silently do nothing */
    if (potion->otyp != POT_FRUIT_JUICE)
        return;
    new_otyp = mold_to_potion(corpsenm);

    if (!new_otyp) {
        /* Denotes that something went wrong in the mixing
         * and the potion should turn into sickness instead
         * of whatever it was going to */
        new_otyp = POT_SICKNESS;
    }

    /* corpsenm = 0 actually means giant ant, but the default value of corpsenm
     * on potions appears to be 0. For now this doesn't have any effect because
     * giant ants can't ferment, but this seems wrong... */
    potion->corpsenm = 0;
    if (get_obj_location(potion, &x, &y, 0) && !Blind && cansee(x, y)) {
        /* OBJ_MINVENT used to be shown here, but on the principle that we
         * generally can't see what's in a monster's inventory at a glance,
         * don't show anything that happens to be fermenting there. */
        if (potion->where == OBJ_INVENT) {
            pline("%s %s.", Yobjnam2(potion, "turn"),
                  hcolor(OBJ_DESCR(objects[new_otyp])));
        }
        else if (potion->where == OBJ_FLOOR) {
            You_see("%s turn %s.", an(xname(potion)),
                    hcolor(OBJ_DESCR(objects[new_otyp])));
            /* can't newsym here because otyp has to change after the message is
             * printed */
            need_newsym = TRUE;
        }
    }

    potion->otyp = new_otyp;
    if (need_newsym)
        newsym(x, y);
}


/* Dip an object into a sink. */
void
    dipsink(obj)
        struct obj * obj;
{
    /* currently can't dip non-potions into sinks */
    if (obj->oclass != POTION_CLASS) {
        impossible("non-potion dipped into sink");
        return;
    }
    if (obj->otyp == POT_POLYMORPH) {
        polymorph_sink();
        makeknown(POT_POLYMORPH);
        useup(obj);
        return;
    }
    You("pour the %s down the drain, creating a puff of vapor.",
          xname(obj));
    potionbreathe(obj);
    useup(obj);
}

/* *monp grants a wish and then leaves the game */
void
mongrantswish(monp)
struct monst **monp;
{
    struct monst *mon = *monp;
    int mx = mon->mx, my = mon->my, glyph = glyph_at(mx, my);

    /* remove the monster first in case wish proves to be fatal
       (blasted by artifact), to keep it out of resulting bones file */
    mongone(mon);
    *monp = 0; /* inform caller that monster is gone */
    /* hide that removal from player--map is visible during wish prompt */
    tmp_at(DISP_ALWAYS, glyph);
    tmp_at(mx, my);
    /* grant the wish */
    makewish();
    /* clean up */
    tmp_at(DISP_END, 0);
}

void
djinni_from_bottle(obj)
struct obj *obj;
{
    struct monst *mtmp;
    int chance;

    if (!(mtmp = makemon(&mons[PM_DJINNI], u.ux, u.uy, NO_MM_FLAGS))) {
        pline("It turns out to be empty.");
        return;
    }

    if (!Blind) {
        pline("In a cloud of smoke, %s emerges!", a_monnam(mtmp));
        pline("%s speaks.", Monnam(mtmp));
    } else {
        You("smell acrid fumes.");
        pline("%s speaks.", Something);
    }

    chance = rn2(5);
    if (obj->blessed)
        chance = 0;
    else if (obj->cursed)
        chance = (chance == 0) ? rn2(4) : 4;
    /* 0,1,2,3,4:  b=80%,5,5,5,5; nc=20%,20,20,20,20; c=5%,5,5,5,80 */

    switch (chance) {
    case 0:
        verbalize("I am in your debt.  I will grant one wish!");
        /* give a wish and discard the monster (mtmp set to null) */
        mongrantswish(&mtmp);
        break;
    case 1:
        /* if the player is trying to play petless, make it safe
           for them to rub lamps */
        if (u.uconduct.pets) {
            verbalize("Thank you for freeing me!");
            (void) tamedog(mtmp, (struct obj *) 0);
            break;
        }
        /* FALLTHRU */
    case 2:
        verbalize("You freed me!");
        mtmp->mpeaceful = TRUE;
        set_malign(mtmp);
        break;
    case 3:
        verbalize("It is about time!");
        if (canspotmon(mtmp))
            pline("%s vanishes.", Monnam(mtmp));
        mongone(mtmp);
        break;
    default:
        verbalize("You disturbed me, fool!");
        mtmp->mpeaceful = FALSE;
        set_malign(mtmp);
        if (!rn2(5)) {
            verbalize("I think I'll keep this wish for myself!");
            mmake_wish(mtmp);
        }
        break;
    }
}

/* clone a gremlin or mold (2nd arg non-null implies heat as the trigger);
   hit points are cut in half (odd HP stays with original) */
struct monst *
split_mon(mon, mtmp)
struct monst *mon,  /* monster being split */
             *mtmp; /* optional attacker whose heat triggered it */
{
    struct monst *mtmp2;
    char reason[BUFSZ];

    reason[0] = '\0';
    if (mtmp)
        Sprintf(reason, " from %s heat",
                (mtmp == &youmonst) ? the_your[1]
                                    : (const char *) s_suffix(mon_nam(mtmp)));

    if (mon == &youmonst) {
        mtmp2 = cloneu();
        if (mtmp2) {
            mtmp2->mhpmax = u.mhmax / 2;
            u.mhmax -= mtmp2->mhpmax;
            context.botl = 1;
            You("multiply%s!", reason);
        }
    } else {
        mtmp2 = clone_mon(mon, 0, 0);
        if (mtmp2) {
            mtmp2->mhpmax = mon->mhpmax / 2;
            mon->mhpmax -= mtmp2->mhpmax;
            if (canspotmon(mon))
                pline("%s multiplies%s!", Monnam(mon), reason);
        }
    }
    return mtmp2;
}


/* KMH, balance patch -- idea by Dylan O'Donnell <dylanw@demon.net>
 * The poor hacker's polypile.  This includes weapons, armor, and tools.
 * To maintain balance, magical categories (amulets, scrolls, spellbooks,
 * potions, rings, and wands) should NOT be supported.
 * Polearms are not currently implemented.
 */
struct obj *
upgrade_obj(obj, res)
struct obj *obj;
int *res;
/* res = 1 if something happened (potion should be used up) 
 * res = 0 if nothing happened (default)
 * res = -1 if object exploded (potion should be used up) 
 */
{
    short otyp = obj->otyp, otyp2;
    xchar ox, oy;
    long owornmask;
    struct obj *otmp;
    int newtyp;
    boolean split1off, explodes = FALSE, dropped = FALSE;
    *res = 0;

    /* Check to see if object is valid */
    if (!obj)
        return obj;

    (void) snuff_lit(obj);
    if (obj->oartifact)
        /* WAC -- Could have some funky fx */
        return obj;

    newtyp = obj2upgrade(obj->otyp);
    if (!newtyp)
        return obj;
    
    /* Only one item can be upgraded at a time */
    split1off = (obj->quan > 1L);
    if (split1off) {
        obj = splitobj(obj, 1L);

        if (carried(obj)) {
            freeinv(obj);
            if (inv_cnt(FALSE) >= 52) {
                sellobj_state(SELL_DONTSELL);
                dropy(obj);
                sellobj_state(SELL_NORMAL);
                dropped = TRUE;
            } else {
                obj->nomerge = 1; /* used to prevent merge */
                obj = addinv(obj);
                obj->nomerge = 0;
            }
        }
    }
    
    if (obj->otyp == LEASH && obj->leashmon)
        o_unleash(obj);

    /* Convert it */
    obj->otyp = newtyp;
    
    /* These still need special handling */
    switch (newtyp) {
    case MAGIC_FLUTE:
    case HORN_OF_PLENTY:
    case FROST_HORN:
    case FIRE_HORN:
    case HORN_OF_BLASTING:
    case MAGIC_HARP:
        obj->spe = rn1(5,10); 
        obj->known = 0;
        break;
    case TINNING_KIT:
        obj->spe = rn1(30,70);
        obj->known = 0;
        break;
    case CRYSTAL_BALL:
        obj->spe = rn1(10, 3);
        /*if (obj->spe > chg) obj->spe = chg;*/
        obj->known = 0;
        break;
    case SADDLE:
        obj->corpsenm = -1;
        break;
    case LEASH:
        obj->corpsenm = 0;
        break;
    }
    
    if ((!carried(obj) || obj->unpaid) &&
            get_obj_location(obj, &ox, &oy, BURIED_TOO | CONTAINED_TOO) &&
            costly_spot(ox, oy)) {
        char objroom = *in_rooms(ox, oy, SHOPBASE);
        register struct monst *shkp = shop_keeper(objroom);

        if ((!obj->no_charge
                 || (Has_contents(obj)
                 && (contained_cost(obj, shkp, 0L, FALSE, FALSE) != 0L)))
                 && inhishop(shkp)) {

            if (shkp->mpeaceful) {
                if(*u.ushops
                        && *in_rooms(u.ux, u.uy, 0)
                        == *in_rooms(shkp->mx, shkp->my, 0)
                        && !costly_spot(u.ux, u.uy)) {
                    make_angry_shk(shkp, ox, oy);
                } else {
                    pline("%s gets angry!", Monnam(shkp));
                    hot_pursuit(shkp);
                }
            } else 
                Norep("%s is furious!", Monnam(shkp));
            otyp2 = obj->otyp;
            obj->otyp = otyp;
            /*
             * [ALI] When unpaid containers are upgraded, the
             * old container is billed as a dummy object, but
             * it's contents are unaffected and will remain
             * either unpaid or not as appropriate.
             */
            otmp = obj->cobj;
            obj->cobj = NULL;
            if (costly_spot(u.ux, u.uy) && objroom == *u.ushops)
                bill_dummy_object(obj);
            else
                (void) stolen_value(obj, ox, oy, FALSE, FALSE);
            obj->otyp = otyp2;
            obj->cobj = otmp;
        }
    }

    /* The object was transformed */
    obj->owt = weight(obj);
    obj->oclass = objects[obj->otyp].oc_class;
    if (!objects[obj->otyp].oc_uses_known)
        obj->known = 1;
    
    if (carried(obj)) {
        if (obj == uskin) 
            rehumanize();
        owornmask = obj->owornmask;;
        quiet_unwear(obj);
        otyp2 = obj->otyp;
        obj->otyp = otyp;
        remove_worn_item(obj, TRUE);
        obj->otyp = otyp2;
        obj->owornmask = owornmask;
        setworn(obj, obj->owornmask);
        set_wear(obj);
    }
    
     if ((obj->otyp == BAG_OF_HOLDING || Bad_bag(obj)) 
          && Has_contents(obj)) {
        
        /* In the strange case that an upgraded sack has charged... */
        if (Bad_bag(obj))
            explodes = TRUE;
        else {
            for (otmp = obj->cobj; otmp; otmp = otmp->nobj) {
                if (mbag_explodes(otmp, 0)) {
                    explodes = TRUE;
                    break;
                }
            }
        }
        
        if (explodes) {
            You("are blasted by a magical explosion!");
            do_boh_explosion(obj, (obj->where == OBJ_FLOOR));
            if (carried(obj))
                useup(obj);
            else
                useupf(obj, obj->quan);
            livelog_printf(LL_ACHIEVE, "just blew up %s %s", uhis(),
                           "bag of holding");
            losehp(Maybe_Half_Phys(d(8, 10)),
                   "exploding magical bag", KILLED_BY_AN);
            
            *res = -1;
            return (struct obj *) 0;
        }
    }
    /* Check if the new object fits the old material.
     * If not... */
    if (!valid_obj_material(obj, obj->material)) {
        init_obj_material(obj);
    }
    if (dropped)
        pline("You drop %s.", doname(obj));
    
    *res = 1;
    return obj;
}

void
quiet_unwear(obj)
struct obj *obj;
{
    long owornmask = obj->owornmask;;
    
    /* Quietly remove worn item if no longer compatible --ALI */
    if (owornmask & W_ARM && !is_suit(obj))
        owornmask &= ~W_ARM;
    if (owornmask & W_ARMC && !is_cloak(obj))
        owornmask &= ~W_ARMC;
    if (owornmask & W_ARMH && !is_helmet(obj))
        owornmask &= ~W_ARMH;
    if (owornmask & W_ARMS && !is_shield(obj))
        owornmask &= ~W_ARMS;
    if (owornmask & W_ARMG && !is_gloves(obj))
        owornmask &= ~W_ARMG;
    if (owornmask & W_ARMF && !is_boots(obj))
        owornmask &= ~W_ARMF;
    if (owornmask & W_ARMU && !is_shirt(obj))
        owornmask &= ~W_ARMU;
    if (owornmask & W_TOOL 
            && obj->otyp != BLINDFOLD 
            && obj->otyp != TOWEL
            && obj->otyp != MASK
            && obj->otyp != GOGGLES
            && obj->otyp != LENSES)
        owornmask &= ~W_TOOL;
}

int
obj2upgrade(int otyp)
{
    /* Guidelines:
     * - Use an ascending upgrade path where items follow that trend,
     *   and then wrap around from best to worst item.
     * - If two items are appropriate - pair them up so they can be
     *   upgraded back and forth. There should be no one-way upgrades,
     *   each path can go both ways.
     *   Ex: A junky item and a valuable item pair, or two like items.
     *
     * Not everything should be upgradable. Some items don't have good
     * pairs or upgrade paths, and should stand alone. Some items are also
     * quite powerful and some should stay unique because they have a
     * thematical purpose.
     * 
     * Because we also have forges, we also have to watch the interplay
     * between the two. 
     * */
    switch (otyp) {
    /* weapons */
    
    case ORCISH_ARROW: return ARROW;
    case ARROW: return ELVEN_ARROW;
    case ELVEN_ARROW: return ORCISH_ARROW;
    
    case YA: return LIGHT_ARROW;
    case LIGHT_ARROW: return YA;
    
    case BOOMERANG: return CHAKRAM;
    case CHAKRAM: return BOOMERANG;
    
    case ORCISH_SPEAR: return SPEAR;
    case SPEAR: return ELVEN_SPEAR;
    case ELVEN_SPEAR: return DWARVISH_SPEAR;
    case DWARVISH_SPEAR: return ORCISH_SPEAR;
    
    case ORCISH_DAGGER: return DAGGER;
    case DAGGER: return ELVEN_DAGGER;
    case ELVEN_DAGGER: return STAKE;
    case STAKE: return GREAT_DAGGER;
    case GREAT_DAGGER: return ORCISH_DAGGER;
    
    case KNIFE: return STILETTO;
    case STILETTO: return KNIFE;
    
    case AXE: return THROWING_AXE;
    case THROWING_AXE: return AXE;
    
    case DWARVISH_BEARDED_AXE: return BATTLE_AXE;
    case BATTLE_AXE: return DWARVISH_BEARDED_AXE;

    case PICK_AXE: return DWARVISH_MATTOCK;
    case DWARVISH_MATTOCK: return PICK_AXE;

    case ORCISH_SHORT_SWORD: return SHORT_SWORD;
    case SHORT_SWORD: return ELVEN_SHORT_SWORD;
    case ELVEN_SHORT_SWORD: return DWARVISH_SHORT_SWORD;
    case DWARVISH_SHORT_SWORD: return ORCISH_SHORT_SWORD;
    
    case ORCISH_SCIMITAR: return SCIMITAR;
    case SCIMITAR: return ORCISH_SCIMITAR;

    case SABER: return RAPIER;
    case RAPIER: return SABER;

    case BROADSWORD: return ELVEN_BROADSWORD;
    case ELVEN_BROADSWORD: return BROADSWORD;

    case ORCISH_LONG_SWORD: return LONG_SWORD;
    case LONG_SWORD: return ELVEN_LONG_SWORD;
    case ELVEN_LONG_SWORD: return ORCISH_LONG_SWORD;
    
    case MACE: return HEAVY_MACE;
    case HEAVY_MACE: return MACE;

    case MORNING_STAR: return ORCISH_MORNING_STAR;
    case ORCISH_MORNING_STAR: return MORNING_STAR;

    case WAR_HAMMER: return HEAVY_WAR_HAMMER;
    case HEAVY_WAR_HAMMER: return WAR_HAMMER;
    
    case CLUB: return AKLYS;
    case AKLYS: return CLUB;

    case ORCISH_BOW: return BOW;
    case BOW: return ELVEN_BOW;
    case ELVEN_BOW: return ORCISH_BOW;
        
    case QUARTERSTAFF: return SILVER_CAPPED_STAFF;
    case SILVER_CAPPED_STAFF: return QUARTERSTAFF;
    
    /* Magical staves are split up into pairs */
    case STAFF_OF_ESCAPE: return STAFF_OF_MATTER;
    case STAFF_OF_MATTER: return STAFF_OF_ESCAPE;
    
    case STAFF_OF_WAR: return STAFF_OF_DIVINATION;
    case STAFF_OF_DIVINATION: return STAFF_OF_WAR; 
    
    case STAFF_OF_NECROMANCY: return STAFF_OF_HEALING; 
    case STAFF_OF_HEALING: return STAFF_OF_NECROMANCY;

    /* helms */
    case CORNUTHAUM: return DUNCE_CAP;
    case DUNCE_CAP: return CORNUTHAUM;

    case FEDORA: return ELVEN_HELM;
    case ELVEN_HELM: return FEDORA;
    
    case ORCISH_HELM: return DWARVISH_HELM;
    case DWARVISH_HELM: return ORCISH_HELM;

    case HELM_OF_MADNESS: return HELM_OF_BRILLIANCE;
    case HELM_OF_BRILLIANCE: return HELM_OF_MADNESS;

    case HELM_OF_TELEPATHY: return TINFOIL_HAT;
    case TINFOIL_HAT: return HELM_OF_TELEPATHY;
    
    /* armor */
    case DWARVISH_CHAIN_MAIL: return ELVEN_CHAIN_MAIL;
    case ELVEN_CHAIN_MAIL: return DWARVISH_CHAIN_MAIL;
    
    case CHAIN_MAIL: return ORCISH_CHAIN_MAIL;
    case ORCISH_CHAIN_MAIL: return CHAIN_MAIL;
    
    case SPLINT_MAIL: return LARGE_SPLINT_MAIL;
    case LARGE_SPLINT_MAIL: return SPLINT_MAIL;
    
    case RING_MAIL: return ORCISH_RING_MAIL;
    case ORCISH_RING_MAIL: return RING_MAIL;
    
    case LIGHT_ARMOR: return JACKET;
    case JACKET: return LIGHT_ARMOR;

    /* robes */
    case ROBE: return LARGE_ROBE;
    case LARGE_ROBE: return ROBE;
    
    case ROBE_OF_WEAKNESS: return ROBE_OF_POWER;
    case ROBE_OF_POWER: return ROBE_OF_PROTECTION;
    case ROBE_OF_PROTECTION: return ROBE_OF_WEAKNESS;

    /* shirts */
    case STRIPED_SHIRT: return T_SHIRT;
    case T_SHIRT: return STRIPED_SHIRT;
        
    /* cloaks */
    case MUMMY_WRAPPING: return PLAIN_CLOAK;
    case PLAIN_CLOAK: return MUMMY_WRAPPING;
    
    case ORCISH_CLOAK: return DWARVISH_CLOAK;
    case DWARVISH_CLOAK: return ELVEN_CLOAK;
    case ELVEN_CLOAK: return ORCISH_CLOAK;
    
    case POISONOUS_CLOAK: return ALCHEMY_SMOCK;
    case ALCHEMY_SMOCK: return POISONOUS_CLOAK;
    
    /* shields */
    case ORCISH_SHIELD: return DWARVISH_ROUNDSHIELD;
    case DWARVISH_ROUNDSHIELD: return ORCISH_SHIELD;
    
    case LARGE_SHIELD: return TOWER_SHIELD;
    case TOWER_SHIELD: return LARGE_SHIELD;
    
    case SMALL_SHIELD: return ELVEN_SHIELD;
    case ELVEN_SHIELD: return SMALL_SHIELD;
    
    /* gloves */
    case GAUNTLETS_OF_FUMBLING: return GAUNTLETS_OF_DEXTERITY;
    case GAUNTLETS_OF_DEXTERITY: return GAUNTLETS_OF_FUMBLING;
    
    /* boots */
    case LOW_BOOTS: return HIGH_BOOTS;
    case HIGH_BOOTS: return DWARVISH_BOOTS;
    
    case FUMBLE_BOOTS: return ELVEN_BOOTS;
    case ELVEN_BOOTS: return FUMBLE_BOOTS;
    
    case LEVITATION_BOOTS: return WATER_WALKING_BOOTS;
    case WATER_WALKING_BOOTS: return LEVITATION_BOOTS;
    
    /* tools */
    
    case SACK: return OILSKIN_SACK;
    
    case OILSKIN_SACK: return BAG_OF_HOLDING;
    case BAG_OF_TRICKS: return BAG_OF_HOLDING;
    case BAG_OF_RATS: return BAG_OF_HOLDING;
    
    case BAG_OF_HOLDING: return SACK;
    
    case SKELETON_KEY: return LOCK_PICK;
    case LOCK_PICK: return SKELETON_KEY;
        
    case MIRROR: return EXPENSIVE_CAMERA;
    case EXPENSIVE_CAMERA: return MIRROR;

    case EIGHT_BALL: return CRYSTAL_BALL;
    case CRYSTAL_BALL: return EIGHT_BALL;
    
    case LENSES: return GOGGLES;
    case GOGGLES: return LENSES;

    case TOWEL: return BLINDFOLD;
    case BLINDFOLD: return TOWEL;

    case LEASH: return SADDLE;
    case SADDLE: return LEASH;

    case TIN_OPENER: return TINNING_KIT;
    case TINNING_KIT: return TIN_OPENER;
    
    case PEA_WHISTLE: return MAGIC_WHISTLE;
    case MAGIC_WHISTLE: return PEA_WHISTLE;
        
    case FLUTE: return MAGIC_FLUTE;
    case MAGIC_FLUTE: return FLUTE;
        
    case TOOLED_HORN: return HORN_OF_PLENTY;
    case HORN_OF_PLENTY: return TOOLED_HORN;

    case FIRE_HORN: return FROST_HORN;
    case FROST_HORN: return FIRE_HORN;

    case HARP: return MAGIC_HARP;
    case MAGIC_HARP: return HARP;
    
    case LEATHER_DRUM: return DRUM_OF_EARTHQUAKE;
    case DRUM_OF_EARTHQUAKE: return LEATHER_DRUM;
    
    case WHETSTONE: return LUCKSTONE;
    case LUCKSTONE: return WHETSTONE;
    
    case LOADSTONE: return HEALTHSTONE;
    case HEALTHSTONE: return LOADSTONE;
    }
    /* This object is not upgradable */
    return 0;
}

/* Character becomes very fast temporarily. */
void
speed_up(duration)
long duration;
{
    /* will fix intrinsic 'slow' */
    if (Slow) {
        HSlow = 0;
        if (!ESlow)
            You("no longer feel sluggish.");
    }

    if (!Very_fast && !Slow) {
        You("are suddenly moving %sfaster.", Fast ? "" : "much ");
    } else if (!Slow) {
        Your("%s get new energy.", makeplural(body_part(LEG)));
    }

    exercise(A_DEX, TRUE);
    incr_itimeout(&HFast, duration);
}

/*potion.c*/
