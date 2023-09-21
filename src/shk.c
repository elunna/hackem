/* NetHack 3.6	shk.c	$NHDT-Date: 1571436007 2019/10/18 22:00:07 $  $NHDT-Branch: NetHack-3.6 $:$NHDT-Revision: 1.171 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"

#define PAY_SOME 2
#define PAY_BUY 1
#define PAY_CANT 0 /* too poor */
#define PAY_SKIP (-1)
#define PAY_BROKE (-2)

STATIC_DCL void FDECL(makekops, (coord *));
STATIC_DCL void FDECL(call_kops, (struct monst *, BOOLEAN_P));
STATIC_DCL void FDECL(kops_gone, (BOOLEAN_P));

#define NOTANGRY(mon) ((mon)->mpeaceful)
#define ANGRY(mon) (!NOTANGRY(mon))
#define IS_SHOP(x) (rooms[x].rtype >= SHOPBASE)
#define no_cheat ((ACURR(A_CHA) - rnl(3)) > 7)
#define visible_bad_shirt 	((uarmu && (uarmu->otyp == STRIPED_SHIRT)) && !uarm  && !uarmc)

#define match_shkrace(mon) \
    ((has_erac(mon) && Race_if(ERAC(mon)->rmnum)) \
     || Race_if((mon)->mnum))

#define hostile_shk(mon) \
    ((erac_race_hostile(mon) || u.ualign.type == A_NONE) \
     && !is_izchak(mon, FALSE))

#define muteshk(shkp) \
    ((shkp)->msleeping || !(shkp)->mcanmove \
     || (shkp)->data->msound <= MS_ANIMAL)

extern const struct shclass shtypes[]; /* defined in shknam.c */

STATIC_VAR NEARDATA long int followmsg; /* last time of follow message */
STATIC_VAR const char and_its_contents[] = " and its contents";
STATIC_VAR const char the_contents_of[] = "the contents of ";

STATIC_DCL void FDECL(append_honorific, (char *));
STATIC_DCL long FDECL(addupbill, (struct monst *));
STATIC_DCL void FDECL(pacify_shk, (struct monst *));
STATIC_DCL struct bill_x *FDECL(onbill, (struct obj *, struct monst *,
                                         BOOLEAN_P));
STATIC_DCL struct monst *FDECL(next_shkp, (struct monst *, BOOLEAN_P));
STATIC_DCL long FDECL(shop_debt, (struct eshk *));
STATIC_DCL char *FDECL(shk_owns, (char *, struct obj *));
STATIC_DCL char *FDECL(mon_owns, (char *, struct obj *));
STATIC_DCL void FDECL(clear_unpaid_obj, (struct monst *, struct obj *));
STATIC_DCL void FDECL(clear_unpaid, (struct monst *, struct obj *));
STATIC_DCL long FDECL(check_credit, (long, struct monst *));
STATIC_DCL void FDECL(pay, (long, struct monst *));
STATIC_DCL long FDECL(get_cost, (struct obj *, struct monst *));
STATIC_DCL long FDECL(set_cost, (struct obj *, struct monst *));
STATIC_DCL const char *FDECL(shk_embellish, (struct obj *, long));
STATIC_DCL long FDECL(cost_per_charge, (struct monst *, struct obj *,
                                        BOOLEAN_P));
STATIC_DCL long FDECL(cheapest_item, (struct monst *));
STATIC_DCL int FDECL(dopayobj, (struct monst *, struct bill_x *,
                                struct obj **, int, BOOLEAN_P));
STATIC_DCL long FDECL(stolen_container, (struct obj *, struct monst *,
                                         long, BOOLEAN_P));
STATIC_DCL long FDECL(getprice, (struct obj *, BOOLEAN_P));
STATIC_DCL void FDECL(shk_names_obj, (struct monst *, struct obj *,
                                      const char *, long, const char *));
STATIC_DCL boolean FDECL(inherits, (struct monst *, int, int, BOOLEAN_P));
STATIC_DCL void FDECL(set_repo_loc, (struct monst *));
STATIC_DCL struct obj *FDECL(bp_to_obj, (struct bill_x *));
STATIC_DCL long FDECL(get_pricing_units, (struct obj *));
STATIC_DCL boolean NDECL(angry_shk_exists);
STATIC_DCL void FDECL(rile_shk, (struct monst *));
STATIC_DCL void FDECL(rouse_shk, (struct monst *, BOOLEAN_P));
STATIC_DCL void FDECL(remove_damage, (struct monst *, BOOLEAN_P));
STATIC_DCL void FDECL(sub_one_frombill, (struct obj *, struct monst *));
STATIC_DCL void FDECL(add_one_tobill, (struct obj *, BOOLEAN_P,
                                       struct monst *));
STATIC_DCL void FDECL(dropped_container, (struct obj *, struct monst *,
                                          BOOLEAN_P));
STATIC_DCL void FDECL(add_to_billobjs, (struct obj *));
STATIC_DCL void FDECL(bill_box_content, (struct obj *, BOOLEAN_P, BOOLEAN_P,
                                         struct monst *));
STATIC_DCL boolean FDECL(rob_shop, (struct monst *));
STATIC_DCL void FDECL(deserted_shop, (char *));
STATIC_DCL boolean FDECL(special_stock, (struct obj *, struct monst *,
                                         BOOLEAN_P));
STATIC_DCL const char *FDECL(cad, (BOOLEAN_P));
/* services */
#define NOBOUND         (-1)    /* No lower/upper limit to charge       */
static int NDECL(shk_other_services);
static int FDECL(shk_identify, (const char *, struct monst *, long ident_type));
static int FDECL(shk_uncurse, (const char *, struct monst *));
static int FDECL(shk_appraisal, (const char *, struct monst *));
static int FDECL(shk_weapon_works, (const char *, struct monst *, long svc_type));
static int FDECL(shk_armor_works, (const char *, struct monst *, long svc_type));
static int FDECL(shk_charge, (const char *, struct monst *, char svc_type));
static int FDECL(shk_rumor, (const char *, struct monst *));
static int FDECL(shk_firearms, (const char *, struct monst *));
static int FDECL(shk_tinker, (const char *, struct monst *));
static boolean FDECL(shk_offer_price, (const char *, long, struct monst *));
static void FDECL(shk_smooth_charge, (int *, int, int));
/*
        invariants: obj->unpaid iff onbill(obj) [unless bp->useup]
                    obj->quan <= bp->bquan
 */

static const char *angrytexts[] = { "quite upset", "ticked off", "furious" };

static const char *hatesrace[] = { "scum", "vermin", "peasant", "scalawag",
                                   "wretch", "miscreant", "maggot", "lowlife" };
/*
 *  Transfer money from inventory to monster when paying
 *  shopkeepers, priests, oracle, succubus, and other demons.
 *  Simple with only gold coins.
 *  This routine will handle money changing when multiple
 *  coin types is implemented, only appropriate
 *  monsters will pay change.  (Peaceful shopkeepers, priests
 *  and the oracle try to maintain goodwill while selling
 *  their wares or services.  Angry monsters and all demons
 *  will keep anything they get their hands on.
 *  Returns the amount actually paid, so we can know
 *  if the monster kept the change.
 */
long
money2mon(mon, amount)
struct monst *mon;
long amount;
{
    struct obj *ygold = findgold(invent, TRUE);

    if (amount <= 0) {
        impossible("%s payment in money2mon!", amount ? "negative" : "zero");
        return 0L;
    }
    if (!ygold || ygold->quan < amount) {
        impossible("Paying without %s money?", ygold ? "enough" : "");
        return 0L;
    }

    if (ygold->quan > amount)
        ygold = splitobj(ygold, amount);
    else if (ygold->owornmask)
        remove_worn_item(ygold, FALSE); /* quiver */
    freeinv(ygold);
    add_to_minv(mon, ygold);
    context.botl = 1;
    return amount;
}

/*
 *  Transfer money from monster to inventory.
 *  Used when the shopkeeper pay for items, and when
 *  the priest gives you money for an ale.
 */
void
money2u(mon, amount)
struct monst *mon;
long amount;
{
    struct obj *mongold = findgold(mon->minvent, TRUE);

    if (amount <= 0) {
        impossible("%s payment in money2u!", amount ? "negative" : "zero");
        return;
    }
    if (!mongold || mongold->quan < amount) {
        impossible("%s paying without %s money?", a_monnam(mon),
                   mongold ? "enough" : "");
        return;
    }

    if (mongold->quan > amount)
        mongold = splitobj(mongold, amount);
    obj_extract_self(mongold);

    if (!merge_choice(invent, mongold) && inv_cnt(FALSE) >= 52) {
        You("have no room for the money!");
        dropy(mongold);
    } else {
        addinv(mongold);
        context.botl = 1;
    }
}

STATIC_OVL struct monst *
next_shkp(shkp, withbill)
register struct monst *shkp;
register boolean withbill;
{
    for (; shkp; shkp = shkp->nmon) {
        if (DEADMONSTER(shkp))
            continue;
        if (shkp->isshk && (ESHK(shkp)->billct || !withbill))
            break;
    }

    if (shkp) {
        if (NOTANGRY(shkp)) {
            if (ESHK(shkp)->surcharge)
                pacify_shk(shkp);
        } else {
            if (!ESHK(shkp)->surcharge)
                rile_shk(shkp);
        }
    }
    return shkp;
}

/* called in mon.c */
void
shkgone(mtmp)
struct monst *mtmp;
{
    struct eshk *eshk = ESHK(mtmp);
    struct mkroom *sroom = &rooms[eshk->shoproom - ROOMOFFSET];
    struct obj *otmp;
    char *p;
    int sx, sy;

    /* [BUG: some of this should be done on the shop level */
    /*       even when the shk dies on a different level.] */
    if (on_level(&eshk->shoplevel, &u.uz)) {
        remove_damage(mtmp, TRUE);
        sroom->resident = (struct monst *) 0;
        if (!search_special(ANY_SHOP))
            level.flags.has_shop = 0;

        /* items on shop floor revert to ordinary objects */
        for (sx = sroom->lx; sx <= sroom->hx; sx++)
            for (sy = sroom->ly; sy <= sroom->hy; sy++)
                for (otmp = level.objects[sx][sy]; otmp;
                     otmp = otmp->nexthere)
                    otmp->no_charge = 0;

        /* Make sure bill is set only when the
           dead shk is the resident shk. */
        if ((p = index(u.ushops, eshk->shoproom)) != 0) {
            setpaid(mtmp);
            eshk->bill_p = (struct bill_x *) 0;
            /* remove eshk->shoproom from u.ushops */
            do {
                *p = *(p + 1);
            } while (*++p);
        }
    }
}

void
set_residency(shkp, zero_out)
register struct monst *shkp;
register boolean zero_out;
{
    if (on_level(&(ESHK(shkp)->shoplevel), &u.uz))
        rooms[ESHK(shkp)->shoproom - ROOMOFFSET].resident =
            (zero_out) ? (struct monst *) 0 : shkp;
}

void
replshk(mtmp, mtmp2)
register struct monst *mtmp, *mtmp2;
{
    rooms[ESHK(mtmp2)->shoproom - ROOMOFFSET].resident = mtmp2;
    if (inhishop(mtmp) && *u.ushops == ESHK(mtmp)->shoproom) {
        ESHK(mtmp2)->bill_p = &(ESHK(mtmp2)->bill[0]);
    }
}

/* do shopkeeper specific structure munging -dlc */
void
restshk(shkp, ghostly)
struct monst *shkp;
boolean ghostly;
{
    shkp->data->msound = MS_SELL;
    if (u.uz.dlevel) {
        struct eshk *eshkp = ESHK(shkp);

        if (eshkp->bill_p != (struct bill_x *) -1000)
            eshkp->bill_p = &eshkp->bill[0];
        /* shoplevel can change as dungeons move around */
        /* savebones guarantees that non-homed shk's will be gone */
        if (ghostly) {
            assign_level(&eshkp->shoplevel, &u.uz);
            if (ANGRY(shkp) && strncmpi(eshkp->customer, plname, PL_NSIZ))
                pacify_shk(shkp);
        }
    }
}

/* Clear the unpaid bit on a single object and its contents. */
STATIC_OVL void
clear_unpaid_obj(shkp, otmp)
struct monst *shkp;
struct obj *otmp;
{
    if (Has_contents(otmp))
        clear_unpaid(shkp, otmp->cobj);
    if (onbill(otmp, shkp, TRUE))
        otmp->unpaid = 0;
}

/* Clear the unpaid bit on all of the objects in the list. */
STATIC_OVL void
clear_unpaid(shkp, list)
struct monst *shkp;
struct obj *list;
{
    while (list) {
        clear_unpaid_obj(shkp, list);
        list = list->nobj;
    }
}

/* either you paid or left the shop or the shopkeeper died */
void
setpaid(shkp)
register struct monst *shkp;
{
    register struct obj *obj;
    register struct monst *mtmp;

    clear_unpaid(shkp, invent);
    clear_unpaid(shkp, fobj);
    clear_unpaid(shkp, level.buriedobjlist);
    if (thrownobj)
        clear_unpaid_obj(shkp, thrownobj);
    if (kickedobj)
        clear_unpaid_obj(shkp, kickedobj);
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        clear_unpaid(shkp, mtmp->minvent);
    for (mtmp = migrating_mons; mtmp; mtmp = mtmp->nmon)
        clear_unpaid(shkp, mtmp->minvent);

    while ((obj = billobjs) != 0) {
        obj_extract_self(obj);
        dealloc_obj(obj);
    }
    if (shkp) {
        ESHK(shkp)->billct = 0;
        ESHK(shkp)->credit = 0L;
        ESHK(shkp)->debit = 0L;
        ESHK(shkp)->loan = 0L;
    }
}

STATIC_OVL long
addupbill(shkp)
register struct monst *shkp;
{
    register int ct = ESHK(shkp)->billct;
    register struct bill_x *bp = ESHK(shkp)->bill_p;
    register long total = 0L;

    while (ct--) {
        total += bp->price * bp->bquan;
        bp++;
    }
    return total;
}


STATIC_OVL void
call_kops(shkp, nearshop)
register struct monst *shkp;
register boolean nearshop;
{
    /* Keystone Kops srt@ucla */
    register boolean nokops;
    char kopname[20];
    coord mm;

    Strcpy(kopname, "Keystone Kops");

    if (!shkp)
        return;

    if (!Deaf)
        pline("An alarm sounds!");
    
    if (Is_blackmarket(&u.uz)) {
        nokops = ((mvitals[PM_SOLDIER].mvflags & G_GONE) &&
                  (mvitals[PM_SERGEANT].mvflags & G_GONE) &&
                  (mvitals[PM_LIEUTENANT].mvflags & G_GONE) &&
                  (mvitals[PM_CAPTAIN].mvflags & G_GONE) &&
                  (mvitals[PM_GENERAL].mvflags & G_GONE));

        Strcpy(kopname, "guards");
    } else {
        nokops = ((mvitals[PM_KEYSTONE_KOP].mvflags & G_GONE) &&
                  (mvitals[PM_KOP_SERGEANT].mvflags & G_GONE) &&
                  (mvitals[PM_KOP_LIEUTENANT].mvflags & G_GONE) &&
                  (mvitals[PM_KOP_KAPTAIN].mvflags & G_GONE) &&
                  (mvitals[PM_KOP_KOMMISSIONER].mvflags & G_GONE));
    }

    if (!angry_guards(!!Deaf) && nokops) {
        if (flags.verbose && !Deaf)
            pline("But no one seems to respond to it.");
        return;
    }

    if (nokops)
        return;

    if (nearshop)
        if (!Is_blackmarket(&u.uz)) {
            /* Create swarm around you, if you merely "stepped out" */
            if (flags.verbose)
                pline_The("%s appear!", kopname);
            mm.x = u.ux;
            mm.y = u.uy;
            makekops(&mm);
            return;
        }
    if (flags.verbose)
        pline_The("%s are after you!", kopname);
    
    if (Is_blackmarket(&u.uz)) {
        /* Create swarm near portal (hinders escape) */
        struct trap *trap = ftrap;
        while (trap) {
            if (trap->ttyp == MAGIC_PORTAL) {
                mm.x = trap->tx;
                mm.y = trap->ty;
            }
            trap = trap->ntrap;
        }
    } else {
        /* Create swarm near down staircase (hinders return to level) */
        mm.x = xdnstair;
        mm.y = ydnstair;
    }
    makekops(&mm);
    /* Create swarm near shopkeeper (hinders return to shop) */
    mm.x = shkp->mx;
    mm.y = shkp->my;
    makekops(&mm);
}

void
blkmar_guards(mtmp)
register struct monst *mtmp;
{
    register struct monst *mt;
    boolean mesg_given = FALSE; /* Only give message if assistants peaceful */
    static boolean rlock = FALSE; /* Prevent recursive calls (via wakeup) */

    if (rlock) 
        return;
    rlock = TRUE;

    /* allow black marketeer to leave his shop */
    hot_pursuit(mtmp);

    /* wake up assistants */
    for (mt = fmon; mt; mt = mt->nmon) {
        if (DEADMONSTER(mt)) 
            continue;
        /* non-tame named monsters are presumably
         * black marketeer's assistants */
        else if (mt->mpeaceful &&
                 ((!mt->mtame && has_mname(mt) && MNAME(mt) &&
                   mt->mpeaceful && mt != mtmp) ||
                  mt->data == &mons[PM_ONE_EYED_SAM])) {
            if (!mesg_given) {
                pline("%s calls for help!", noit_Monnam(mtmp));
                mesg_given = TRUE;
                bars_around_portal(FALSE);

                call_kops(mtmp, FALSE);

            }
            wakeup(mt, TRUE);
        }
    }
    rlock = FALSE;
}

/* look for a portal on the level and add or
 * remove iron bars on every adjacent square */
void
bars_around_portal(removebars)
boolean removebars;
{
    int x, y, dx, dy;
    boolean sawit = FALSE;
    struct trap *trap = ftrap;
    while (trap) {
        if (trap->ttyp == MAGIC_PORTAL) break;
        trap = trap->ntrap;
    }
    if (!trap) return;
    if (trap->tx == u.ux && trap->ty == u.uy) return;
    for (dx = -1; dx <= 1; dx++)
        for (dy = -1; dy <= 1; dy++) {
            if (!dx && !dy) continue;
            x = trap->tx + dx;
            y = trap->ty + dy;
            if (removebars) {
                if (levl[x][y].typ == IRONBARS) {
                    dissolve_bars(x, y);
                    if (cansee(x, y))
                        sawit = TRUE;
                }
            } else {
                if (!IS_ROCK(levl[x][y].typ) && levl[x][y].typ != IRONBARS) {
                    levl[x][y].typ = IRONBARS;
                    newsym(x, y);
                    if (cansee(x, y))
                        sawit = TRUE;
                }
            }
        }
    if (sawit) {
        if (removebars)
            pline_The("iron bars rise back into the ceiling.");
        else
            pline("Iron bars drop from the ceiling around the magic portal!");
    }

}

/* x,y is strictly inside shop */
char
inside_shop(x, y)
register xchar x, y;
{
    register char rno;

    rno = levl[x][y].roomno;
    if ((rno < ROOMOFFSET) || levl[x][y].edge || !IS_SHOP(rno - ROOMOFFSET))
        rno = NO_ROOM;
    return rno;
}

void
u_left_shop(leavestring, newlev)
char *leavestring;
boolean newlev;
{
    struct monst *shkp;
    struct eshk *eshkp;

    /*
     * IF player
     * ((didn't leave outright) AND
     *  ((he is now strictly-inside the shop) OR
     *   (he wasn't strictly-inside last turn anyway)))
     * THEN (there's nothing to do, so just return)
     */
    if (!*leavestring && (!levl[u.ux][u.uy].edge || levl[u.ux0][u.uy0].edge))
        return;

    shkp = shop_keeper(*leavestring ? *leavestring : *u.ushops0);
    if (!shkp || !inhishop(shkp))
        return; /* shk died, teleported, changed levels... */

    eshkp = ESHK(shkp);
    if (!eshkp->billct && !eshkp->debit) /* bill is settled */
        return;

    if (!*leavestring && !muteshk(shkp)) {
        /*
         * Player just stepped onto shop-boundary (known from above logic).
         * Try to intimidate him into paying his bill
         */
        if (!Deaf && !muteshk(shkp))
            verbalize(NOTANGRY(shkp) ? "%s!  Please pay before leaving."
                                 : "%s!  Don't you leave without paying!",
                      plname);
        else
            pline("%s %s that you need to pay before leaving%s",
                  Shknam(shkp),
                  NOTANGRY(shkp) ? "points out" : "makes it clear",
                  NOTANGRY(shkp) ? "." : "!");
        return;
    }

    if (rob_shop(shkp)) {
        if (Is_blackmarket(&u.uz))
            blkmar_guards(shkp);
        else
            call_kops(shkp, (!newlev && levl[u.ux0][u.uy0].edge));
    }
}

/* robbery from outside the shop via telekinesis or grappling hook */
void
remote_burglary(x, y)
xchar x, y;
{
    struct monst *shkp;
    struct eshk *eshkp;

    shkp = shop_keeper(*in_rooms(x, y, SHOPBASE));
    if (!shkp || !inhishop(shkp))
        return; /* shk died, teleported, changed levels... */

    eshkp = ESHK(shkp);
    if (!eshkp->billct && !eshkp->debit) /* bill is settled */
        return;

    if (rob_shop(shkp)) {
        if (Is_blackmarket(&u.uz))
            blkmar_guards(shkp);
        else
            call_kops(shkp, FALSE);
        /*[might want to set 2nd arg based on distance from shop doorway]*/
        
    }
}

/* shop merchandise has been taken; pay for it with any credit available;
   return false if the debt is fully covered by credit, true otherwise */
STATIC_OVL boolean
rob_shop(shkp)
struct monst *shkp;
{
    struct eshk *eshkp;
    long total;

    eshkp = ESHK(shkp);
    rouse_shk(shkp, TRUE);
    total = (addupbill(shkp) + eshkp->debit);
    if (eshkp->credit >= total) {
        Your("credit of %ld %s is used to cover your shopping bill.",
             eshkp->credit, currency(eshkp->credit));
        total = 0L; /* credit gets cleared by setpaid() */
    } else {
        You("escaped the shop without paying!");
        total -= eshkp->credit;
    }
    setpaid(shkp);
    if (!total)
        return FALSE;

    /* by this point, we know an actual robbery has taken place */
    eshkp->robbed += total;
    You("stole %ld %s worth of merchandise.", total, currency(total));
    livelog_printf(LL_ACHIEVE, "stole %ld %s worth of merchandise from %s %s",
                   total, currency(total), s_suffix(shkname(shkp)),
                   shtypes[eshkp->shoptype - SHOPBASE].name);
    if (!Role_if(PM_ROGUE) && u.ualign.type != A_NONE) { 
        if (u.ualign.type == A_LAWFUL)
            You_feel("guilty."); /* stealing is unlawful */
        adjalign(-sgn(u.ualign.type));
    }

    hot_pursuit(shkp);
    return TRUE;
}

/* give a message when entering an untended shop (caller has verified that) */
STATIC_OVL void
deserted_shop(enterstring)
/*const*/ char *enterstring;
{
    struct monst *mtmp;
    struct mkroom *r = &rooms[(int) *enterstring - ROOMOFFSET];
    int x, y, m = 0, n = 0;

    for (x = r->lx; x <= r->hx; ++x)
        for (y = r->ly; y <= r->hy; ++y) {
            if (x == u.ux && y == u.uy)
                continue;
            if ((mtmp = m_at(x, y)) != 0) {
                ++n;
                if (sensemon(mtmp) || ((M_AP_TYPE(mtmp) == M_AP_NOTHING
                                        || M_AP_TYPE(mtmp) == M_AP_MONSTER)
                                       && canseemon(mtmp)))
                    ++m;
            }
        }

    if (Blind && !(Blind_telepat || Detect_monsters))
        ++n; /* force feedback to be less specific */

    pline("This shop %s %s.", (m < n) ? "seems to be" : "is",
          !n ? "deserted" : "untended");
}

void
u_entered_shop(enterstring)
char *enterstring;
{
    register int rt;
    register struct monst *shkp;
    register struct eshk *eshkp;
    static char empty_shops[5];

    if (!*enterstring)
        return;

    if (!(shkp = shop_keeper(*enterstring))) {
        if (!index(empty_shops, *enterstring)
            && in_rooms(u.ux, u.uy, SHOPBASE)
                   != in_rooms(u.ux0, u.uy0, SHOPBASE))
            deserted_shop(enterstring);
        Strcpy(empty_shops, u.ushops);
        u.ushops[0] = '\0';
        return;
    }

    eshkp = ESHK(shkp);

    if (!inhishop(shkp)) {
        /* dump core when referenced */
        eshkp->bill_p = (struct bill_x *) -1000;
        if (!index(empty_shops, *enterstring))
            deserted_shop(enterstring);
        Strcpy(empty_shops, u.ushops);
        u.ushops[0] = '\0';
        return;
    }

    eshkp->bill_p = &(eshkp->bill[0]);

    if ((!eshkp->visitct || *eshkp->customer)
        && strncmpi(eshkp->customer, plname, PL_NSIZ)) {
        /* You seem to be new here */
        eshkp->visitct = 0;
        eshkp->following = 0;
        (void) strncpy(eshkp->customer, plname, PL_NSIZ);
        pacify_shk(shkp);
    }

    if (muteshk(shkp) || eshkp->following)
        return; /* no dialog */

    if (Invis) {
        pline("%s senses your presence.", Shknam(shkp));
        if (!Is_blackmarket(&u.uz)) {
            if (!Deaf && !muteshk(shkp)) {
                verbalize("Invisible customers are not welcome!");
            } else {
                pline("%s stands firm as if %s knows you are there.", Shknam(shkp), noit_mhe(shkp));
            }
            return;
        }
    }

    /* Visible striped prison shirt */
    if (!Is_blackmarket(&u.uz) &&
        (uarmu && (uarmu->otyp == STRIPED_SHIRT))
        && !uarm
        && !uarmc) {
        eshkp->pbanned = TRUE;
    }

    rt = rooms[*enterstring - ROOMOFFSET].rtype;

    if (ANGRY(shkp)) {
        if (!Deaf && !muteshk(shkp))
            verbalize("So, %s, you dare return to %s %s?!", plname,
                      s_suffix(shkname(shkp)), shtypes[rt - SHOPBASE].name);
        else
            pline("%s seems %s over your return to %s %s!",
                  Shknam(shkp), angrytexts[rn2(SIZE(angrytexts))],
                  noit_mhis(shkp), shtypes[rt - SHOPBASE].name);
    } else if (eshkp->robbed) {
        if (!Deaf)
            pline("%s mutters imprecations against shoplifters.",
                  Shknam(shkp));
        else
            pline("%s is combing through %s inventory list.",
                  Shknam(shkp), noit_mhis(shkp));
    } else {
        const char *punct = (hostile_shk(shkp) ? "." : "!");

        if (!Deaf && !muteshk(shkp) && !eshkp->pbanned)
            verbalize("%s, %s%s  Welcome%s to %s %s%s", Hello(shkp),
                      hostile_shk(shkp) ? hatesrace[rn2(SIZE(hatesrace))]
                                        : plname, punct,
                      eshkp->visitct++ ? " again" : "",
                      s_suffix(shkname(shkp)),
                      shtypes[rt - SHOPBASE].name, punct);
        else
            You("enter %s %s%s%s",
                s_suffix(shkname(shkp)),
                shtypes[rt - SHOPBASE].name,
                eshkp->visitct++ ? " again" : "", punct);
    }
    /* can't do anything about blocking if teleported in */
    if (!inside_shop(u.ux, u.uy) && !(Is_blackmarket(&u.uz))) {
        boolean should_block;
        int cnt;
        const char *tool;
        struct obj *pick = carrying(PICK_AXE),
                   *mattock = carrying(DWARVISH_MATTOCK);

        if (pick || mattock) {
            cnt = 1;               /* so far */
            if (pick && mattock) { /* carrying both types */
                tool = "digging tool";
                cnt = 2; /* `more than 1' is all that matters */
            } else if (pick) {
                tool = "pick-axe";
                /* hack: `pick' already points somewhere into inventory */
                while ((pick = pick->nobj) != 0)
                    if (pick->otyp == PICK_AXE)
                        ++cnt;
            } else { /* assert(mattock != 0) */
                tool = "mattock";
                while ((mattock = mattock->nobj) != 0)
                    if (mattock->otyp == DWARVISH_MATTOCK)
                        ++cnt;
                /* [ALI] Shopkeeper identifies mattock(s) */
                if (!Blind)
                    makeknown(DWARVISH_MATTOCK);
            }
            if (!Deaf && !muteshk(shkp))
                verbalize(NOTANGRY(shkp) && !hostile_shk(shkp)
                                     ? "Will you please leave your %s%s outside?"
                                     : "Leave the %s%s outside.",
                          tool, plur(cnt));
            else
                pline("%s %s to let you in with your %s%s.",
                      Shknam(shkp),
                      NOTANGRY(shkp) && !hostile_shk(shkp)
                                 ? "is hesitant" : "refuses",
                      tool, plur(cnt));
            should_block = TRUE;
        } else if (eshkp->pbanned && !ANGRY(shkp)) {
            verbalize("I don't sell to your kind here.");
            should_block = TRUE;
        } else if (u.usteed) {
            if (!Deaf && !muteshk(shkp))
                verbalize(NOTANGRY(shkp) && !hostile_shk(shkp)
                                     ? "Will you please leave %s outside?"
                                     : "Leave %s outside.",
                          y_monnam(u.usteed));
            else
                pline("%s %s to let you in while you're riding %s.",
                      Shknam(shkp),
                      NOTANGRY(shkp) && !hostile_shk(shkp)
                                 ? "doesn't want" : "refuses",
                      y_monnam(u.usteed));
            should_block = TRUE;
        } else {
            should_block =
                (Fast && (sobj_at(PICK_AXE, u.ux, u.uy)
                          || sobj_at(DWARVISH_MATTOCK, u.ux, u.uy)));
        }
        if (should_block)
            (void) dochug(shkp); /* shk gets extra move */
    }
    return;
}

/* called when removing a pick-axe or mattock from a container */
void
pick_pick(obj)
struct obj *obj;
{
    struct monst *shkp;

    if (obj->unpaid || !is_pick(obj))
        return;
    shkp = shop_keeper(*u.ushops);
    if (shkp && inhishop(shkp)) {
        static NEARDATA long pickmovetime = 0L;

        /* if you bring a sack of N picks into a shop to sell,
           don't repeat this N times when they're taken out */
        if (moves != pickmovetime) {
            if (!Deaf && !muteshk(shkp))
                verbalize("You sneaky %s!  Get out of here with that pick!",
                      cad(FALSE));
            else
                pline("%s %s your pick!",
                      Shknam(shkp),
                      haseyes(shkp->data) ? "glares at"
                                          : "is dismayed because of");
        }
        pickmovetime = moves;
    }
}

/*
   Decide whether two unpaid items are mergable; caller is responsible for
   making sure they're unpaid and the same type of object; we check the price
   quoted by the shopkeeper and also that they both belong to the same shk.
 */
boolean
same_price(obj1, obj2)
struct obj *obj1, *obj2;
{
    register struct monst *shkp1, *shkp2;
    struct bill_x *bp1 = 0, *bp2 = 0;
    boolean are_mergable = FALSE;

    /* look up the first object by finding shk whose bill it's on */
    for (shkp1 = next_shkp(fmon, TRUE); shkp1;
         shkp1 = next_shkp(shkp1->nmon, TRUE))
        if ((bp1 = onbill(obj1, shkp1, TRUE)) != 0)
            break;
    /* second object is probably owned by same shk; if not, look harder */
    if (shkp1 && (bp2 = onbill(obj2, shkp1, TRUE)) != 0) {
        shkp2 = shkp1;
    } else {
        for (shkp2 = next_shkp(fmon, TRUE); shkp2;
             shkp2 = next_shkp(shkp2->nmon, TRUE))
            if ((bp2 = onbill(obj2, shkp2, TRUE)) != 0)
                break;
    }

    if (!bp1 || !bp2)
        impossible("same_price: object wasn't on any bill!");
    else
        are_mergable = (shkp1 == shkp2 && bp1->price == bp2->price);
    return are_mergable;
}

/*
 * Figure out how much is owed to a given shopkeeper.
 * At present, we ignore any amount robbed from the shop, to avoid
 * turning the `$' command into a way to discover that the current
 * level is bones data which has a shk on the warpath.
 */
STATIC_OVL long
shop_debt(eshkp)
struct eshk *eshkp;
{
    struct bill_x *bp;
    int ct;
    long debt = eshkp->debit;

    for (bp = eshkp->bill_p, ct = eshkp->billct; ct > 0; bp++, ct--)
        debt += bp->price * bp->bquan;
    return debt;
}

/* called in response to the `$' command */
void
shopper_financial_report()
{
    struct monst *shkp, *this_shkp = shop_keeper(inside_shop(u.ux, u.uy));
    struct eshk *eshkp;
    long amt;
    int pass;

    eshkp = this_shkp ? ESHK(this_shkp) : 0;
    if (eshkp && !(eshkp->credit || shop_debt(eshkp))) {
        You("have no credit or debt in here.");
        this_shkp = 0; /* skip first pass */
    }

    /* pass 0: report for the shop we're currently in, if any;
       pass 1: report for all other shops on this level. */
    for (pass = this_shkp ? 0 : 1; pass <= 1; pass++)
        for (shkp = next_shkp(fmon, FALSE); shkp;
             shkp = next_shkp(shkp->nmon, FALSE)) {
            if ((shkp != this_shkp) ^ pass)
                continue;
            eshkp = ESHK(shkp);
            if ((amt = eshkp->credit) != 0)
                You("have %ld %s credit at %s %s.", amt, currency(amt),
                    s_suffix(shkname(shkp)),
                    shtypes[eshkp->shoptype - SHOPBASE].name);
            else if (shkp == this_shkp)
                You("have no credit in here.");
            if ((amt = shop_debt(eshkp)) != 0)
                You("owe %s %ld %s.", shkname(shkp), amt, currency(amt));
            else if (shkp == this_shkp)
                You("don't owe any money here.");
        }
}

int
inhishop(mtmp)
register struct monst *mtmp;
{
    struct eshk *eshkp = ESHK(mtmp);

    return (index(in_rooms(mtmp->mx, mtmp->my, SHOPBASE), eshkp->shoproom)
            && on_level(&eshkp->shoplevel, &u.uz));
}

struct monst *
shop_keeper(rmno)
char rmno;
{
    struct monst *shkp;

    shkp = (rmno >= ROOMOFFSET) ? rooms[rmno - ROOMOFFSET].resident : 0;
    if (shkp) {
        if (has_eshk(shkp)) {
            if (NOTANGRY(shkp)) {
                if (ESHK(shkp)->surcharge)
                    pacify_shk(shkp);
            } else {
                if (!ESHK(shkp)->surcharge)
                    rile_shk(shkp);
            }
        } else {
            /* would have segfaulted on ESHK dereference previously */
            impossible("%s? (rmno=%d, rtype=%d, mnum=%d, \"%s\")",
                       shkp->isshk ? "shopkeeper career change"
                                   : "shop resident not shopkeeper",
                       (int) rmno,
                       (int) rooms[rmno - ROOMOFFSET].rtype,
                       shkp->mnum,
                       /* [real shopkeeper name is kept in ESHK, not MNAME] */
                       has_mname(shkp) ? MNAME(shkp) : "anonymous");
            /* not sure if this is appropriate, because it does nothing to
               correct the underlying rooms[].resident issue but... */
            return (struct monst *) 0;
        }
    }
    return shkp;
}

boolean
tended_shop(sroom)
struct mkroom *sroom;
{
    struct monst *mtmp = sroom->resident;

    return !mtmp ? FALSE : (boolean) inhishop(mtmp);
}

STATIC_OVL struct bill_x *
onbill(obj, shkp, silent)
struct obj *obj;
struct monst *shkp;
boolean silent;
{
    if (shkp) {
        register struct bill_x *bp = ESHK(shkp)->bill_p;
        register int ct = ESHK(shkp)->billct;

        while (--ct >= 0)
            if (bp->bo_id == obj->o_id) {
                if (!obj->unpaid)
                    pline("onbill: paid obj on bill?");
                return bp;
            } else
                bp++;
    }
    if (obj->unpaid && !silent)
        pline("onbill: unpaid obj not on bill?");
    return (struct bill_x *) 0;
}

/* check whether an object or any of its contents belongs to a shop */
boolean
is_unpaid(obj)
struct obj *obj;
{
    return (boolean) (obj->unpaid
                      || (Has_contents(obj) && count_unpaid(obj->cobj)));
}

/* Delete the contents of the given object. */
void
delete_contents(obj)
register struct obj *obj;
{
    register struct obj *curr;

    while ((curr = obj->cobj) != 0) {
        obj_extract_self(curr);
        obfree(curr, (struct obj *) 0);
    }
}

/* called with two args on merge */
void
obfree(obj, merge)
register struct obj *obj, *merge;
{
    register struct bill_x *bp;
    register struct bill_x *bpm;
    register struct monst *shkp;

    if (obj->otyp == LEASH && obj->leashmon)
        o_unleash(obj);
    if (obj->oclass == FOOD_CLASS)
        food_disappears(obj);
    if (obj->oclass == SPBOOK_CLASS)
        book_disappears(obj);
    if (Has_contents(obj))
        delete_contents(obj);
    if (Is_container(obj))
        maybe_reset_pick(obj);

    shkp = 0;
    if (obj->unpaid) {
        /* look for a shopkeeper who owns this object */
        for (shkp = next_shkp(fmon, TRUE); shkp;
             shkp = next_shkp(shkp->nmon, TRUE))
            if (onbill(obj, shkp, TRUE))
                break;
    }
    /* sanity check, in case obj is on bill but not marked 'unpaid' */
    if (!shkp)
        shkp = shop_keeper(*u.ushops);
    /*
     * Note:  `shkp = shop_keeper(*u.ushops)' used to be
     *    unconditional.  But obfree() is used all over
     *    the place, so making its behavior be dependent
     *    upon player location doesn't make much sense.
     */

    if ((bp = onbill(obj, shkp, FALSE)) != 0) {
        if (!merge) {
            bp->useup = 1;
            obj->unpaid = 0; /* only for doinvbill */
            add_to_billobjs(obj);
            return;
        }
        bpm = onbill(merge, shkp, FALSE);
        if (!bpm) {
            /* this used to be a rename */
            /* !merge already returned */
            impossible("obfree: not on bill, %s = (%d,%d,%ld,%d) (%d,%d,%ld,%d)??",
                        "otyp,where,quan,unpaid",
                        obj->otyp, obj->where, obj->quan, obj->unpaid ? 1 : 0,
                        merge->otyp, merge->where, merge->quan,
                            merge->unpaid ? 1 : 0);
            return;
        } else {
            /* this was a merger */
            bpm->bquan += bp->bquan;
            ESHK(shkp)->billct--;
#ifdef DUMB
            {
                /* DRS/NS 2.2.6 messes up -- Peter Kendell */
                int indx = ESHK(shkp)->billct;

                *bp = ESHK(shkp)->bill_p[indx];
            }
#else
            *bp = ESHK(shkp)->bill_p[ESHK(shkp)->billct];
#endif
        }
    } else {
        /* not on bill; if the item is being merged away rather than
           just deleted and has a higher price adjustment than the stack
           being merged into, give the latter the former's obj->o_id so
           that the merged stack takes on higher price; matters if hero
           eventually buys them from a shop, but doesn't matter if hero
           owns them and intends to sell (unless he subsequently buys
           them back) or if no shopping activity ever involves them */
        if (merge && (oid_price_adjustment(obj, obj->o_id)
                      > oid_price_adjustment(merge, merge->o_id)))
            merge->o_id = obj->o_id;
    }
    if (obj->owornmask) {
        impossible("obfree: deleting worn obj (%d: %ld)", obj->otyp,
                   obj->owornmask);
        /* unfortunately at this point we don't know whether worn mask
           applied to hero or a monster or perhaps something bogus, so
           can't call remove_worn_item() to get <X>_off() side-effects */
        setnotworn(obj);
    }
    dealloc_obj(obj);
}

STATIC_OVL long
check_credit(tmp, shkp)
long tmp;
register struct monst *shkp;
{
    long credit = ESHK(shkp)->credit;

    if (credit == 0L) {
        ; /* nothing to do; just 'return tmp;' */
    } else if (credit >= tmp) {
        pline_The("price is deducted from your credit.");
        ESHK(shkp)->credit -= tmp;
        tmp = 0L;
    } else {
        pline_The("price is partially covered by your credit.");
        ESHK(shkp)->credit = 0L;
        tmp -= credit;
    }
    return tmp;
}

STATIC_OVL void
pay(tmp, shkp)
long tmp;
register struct monst *shkp;
{
    long robbed = ESHK(shkp)->robbed;
    long balance = ((tmp <= 0L) ? tmp : check_credit(tmp, shkp));

    if (balance > 0)
        money2mon(shkp, balance);
    else if (balance < 0)
        money2u(shkp, -balance);
    context.botl = 1;
    
    if (!u.uconduct.shk++)
        livelog_printf(LL_CONDUCT, "participated in the Yendorian economy for the first time");
    
    if (robbed) {
        robbed -= tmp;
        if (robbed < 0)
            robbed = 0L;
        ESHK(shkp)->robbed = robbed;
    }
}

/* return shkp to home position */
void
home_shk(shkp, killkops)
register struct monst *shkp;
register boolean killkops;
{
    register xchar x = ESHK(shkp)->shk.x, y = ESHK(shkp)->shk.y;

    (void) mnearto(shkp, x, y, TRUE);
    level.flags.has_shop = 1;
    if (killkops) {
        kops_gone(TRUE);
        pacify_guards();
    }
    after_shk_move(shkp);
}

STATIC_OVL boolean
angry_shk_exists()
{
    register struct monst *shkp;

    for (shkp = next_shkp(fmon, FALSE); shkp;
         shkp = next_shkp(shkp->nmon, FALSE))
        if (ANGRY(shkp))
            return TRUE;
    return FALSE;
}

/* remove previously applied surcharge from all billed items */
STATIC_OVL void
pacify_shk(shkp)
register struct monst *shkp;
{
    NOTANGRY(shkp) = TRUE; /* make peaceful */
    if (ESHK(shkp)->surcharge) {
        register struct bill_x *bp = ESHK(shkp)->bill_p;
        register int ct = ESHK(shkp)->billct;

        ESHK(shkp)->surcharge = FALSE;
        while (ct-- > 0) {
            register long reduction = (bp->price + 3L) / 4L;
            bp->price -= reduction; /* undo 33% increase */
            bp++;
        }
    }
}

/* add aggravation surcharge to all billed items */
STATIC_OVL void
rile_shk(shkp)
register struct monst *shkp;
{
    NOTANGRY(shkp) = FALSE; /* make angry */
    if (!ESHK(shkp)->surcharge) {
        register struct bill_x *bp = ESHK(shkp)->bill_p;
        register int ct = ESHK(shkp)->billct;

        ESHK(shkp)->surcharge = TRUE;
        while (ct-- > 0) {
            register long surcharge = (bp->price + 2L) / 3L;
            bp->price += surcharge;
            bp++;
        }
    }
}

/* wakeup and/or unparalyze shopkeeper */
STATIC_OVL void
rouse_shk(shkp, verbosely)
struct monst *shkp;
boolean verbosely;
{
    if (!shkp->mcanmove || shkp->msleeping) {
        /* greed induced recovery... */
        if (verbosely && canspotmon(shkp))
            pline("%s %s.", Shknam(shkp),
                  shkp->msleeping ? "wakes up" : "can move again");
        shkp->msleeping = 0;
        shkp->mfrozen = 0;
	if (!shkp->mstone || shkp->mstone > 2)
	    shkp->mcanmove = 1;
    }
}

void
make_happy_shk(shkp, silentkops)
register struct monst *shkp;
register boolean silentkops;
{
    boolean wasmad = ANGRY(shkp);
    struct eshk *eshkp = ESHK(shkp);

    pacify_shk(shkp);
    eshkp->following = 0;
    eshkp->robbed = 0L;
    if (!Role_if(PM_ROGUE))
        adjalign(sgn(u.ualign.type));
    if (!inhishop(shkp)) {
        char shk_nam[BUFSZ];
        boolean vanished = canseemon(shkp);

        Strcpy(shk_nam, shkname(shkp));
        if (on_level(&eshkp->shoplevel, &u.uz)) {
            home_shk(shkp, FALSE);
            /* didn't disappear if shk can still be seen */
            if (canseemon(shkp))
                vanished = FALSE;
        } else {
            /* if sensed, does disappear regardless whether seen */
            if (sensemon(shkp))
                vanished = TRUE;
            /* can't act as porter for the Amulet, even if shk
               happens to be going farther down rather than up */
            mdrop_special_objs(shkp);
            /* arrive near shop's door */
            migrate_to_level(shkp, ledger_no(&eshkp->shoplevel),
                             MIGR_APPROX_XY, &eshkp->shd);
            /* dismiss kops on that level when shk arrives */
            eshkp->dismiss_kops = TRUE;
        }
        if (vanished)
            pline("Satisfied, %s suddenly disappears!", shk_nam);
    } else if (wasmad)
        pline("%s calms down.", Shknam(shkp));

    make_happy_shoppers(silentkops);
}

/* called by make_happy_shk() and also by losedogs() for migrating shk */
void
make_happy_shoppers(silentkops)
boolean silentkops;
{
    if (!angry_shk_exists()) {
        kops_gone(silentkops);
        pacify_guards();
    }
}

void
hot_pursuit(shkp)
register struct monst *shkp;
{
    if (!shkp->isshk)
        return;

    rile_shk(shkp);
    (void) strncpy(ESHK(shkp)->customer, plname, PL_NSIZ);
    ESHK(shkp)->following = 1;
}

/* Used when the shkp is teleported or falls (ox == 0) out of his shop, or
   when the player is not on a costly_spot and he damages something inside
   the shop.  These conditions must be checked by the calling function. */
/*ARGSUSED*/
void
make_angry_shk(shkp, ox, oy)
struct monst *shkp;
xchar ox UNUSED; /* <ox,oy> predate 'noit_Monnam()', let alone Shknam() */
xchar oy UNUSED;
{
    struct eshk *eshkp = ESHK(shkp);

    /* all pending shop transactions are now "past due" */
    if (eshkp->billct || eshkp->debit || eshkp->loan || eshkp->credit) {
        eshkp->robbed += (addupbill(shkp) + eshkp->debit + eshkp->loan);
        eshkp->robbed -= eshkp->credit;
        if (eshkp->robbed < 0L)
            eshkp->robbed = 0L;
        /* billct, debit, loan, and credit will be cleared by setpaid */
        setpaid(shkp);
    }

    pline("%s %s!", Shknam(shkp), !ANGRY(shkp) ? "gets angry" : "is furious");
    hot_pursuit(shkp);
}

STATIC_VAR const char
        no_money[] = "Moreover, you%s have no money.",
        not_enough_money[] = "Besides, you don't have enough to interest %s.";

/* delivers the cheapest item on the list */
STATIC_OVL long
cheapest_item(shkp)
register struct monst *shkp;
{
    register int ct = ESHK(shkp)->billct;
    register struct bill_x *bp = ESHK(shkp)->bill_p;
    register long gmin = (bp->price * bp->bquan);

    while (ct--) {
        if (bp->price * bp->bquan < gmin)
            gmin = bp->price * bp->bquan;
        bp++;
    }
    return gmin;
}

int
dopay()
{
    register struct eshk *eshkp;
    register struct monst *shkp;
    struct monst *nxtm, *resident;
    long ltmp;
    long umoney;
    int pass, tmp, sk = 0, seensk = 0;
    boolean paid = FALSE, stashed_gold = (hidden_gold() > 0L);

    multi = 0;

    /* Find how many shk's there are, how many are in
     * sight, and are you in a shop room with one.
     */
    nxtm = resident = 0;
    for (shkp = next_shkp(fmon, FALSE); shkp;
         shkp = next_shkp(shkp->nmon, FALSE)) {
        sk++;
        if (ANGRY(shkp) && distu(shkp->mx, shkp->my) <= 2)
            nxtm = shkp;
        if (canspotmon(shkp))
            seensk++;
        if (inhishop(shkp) && (*u.ushops == ESHK(shkp)->shoproom))
            resident = shkp;
    }

    if (nxtm) {      /* Player should always appease an */
        shkp = nxtm; /* irate shk standing next to them. */
        goto proceed;
    }

    if ((!sk && (!Blind || Blind_telepat)) || (!Blind && !seensk)) {
        There("appears to be no shopkeeper here to receive your payment.");
        return 0;
    }

    if (!seensk) {
        You_cant("see...");
        return 0;
    }

    if (Hidinshell) {
        pline("How do you expect to do that while hiding in your shell?");
        return 0;
    }

    /* The usual case.  Allow paying at a distance when
     * inside a tended shop.  Should we change that?
     */
    if (sk == 1 && resident) {
        shkp = resident;
        goto proceed;
    }

    if (seensk == 1) {
        for (shkp = next_shkp(fmon, FALSE); shkp;
             shkp = next_shkp(shkp->nmon, FALSE))
            if (canspotmon(shkp))
                break;
        if (shkp != resident && distu(shkp->mx, shkp->my) > 2) {
            pline("%s is not near enough to receive your payment.",
                  Shknam(shkp));
            return 0;
        }
    } else {
        struct monst *mtmp;
        coord cc;
        int cx, cy;

        pline("Pay whom?");
        cc.x = u.ux;
        cc.y = u.uy;
        if (getpos(&cc, TRUE, "the creature you want to pay") < 0)
            return 0; /* player pressed ESC */
        cx = cc.x;
        cy = cc.y;
        if (cx < 0) {
            pline("Try again...");
            return 0;
        }
        if (u.ux == cx && u.uy == cy) {
            You("are generous to yourself.");
            return 0;
        }
        mtmp = m_at(cx, cy);
        if (!cansee(cx, cy) && (!mtmp || !canspotmon(mtmp))) {
            You_cant("%s anyone there.", !Blind ? "see" : "sense");
            return 0;
        }
        if (!mtmp) {
            There("is no one there to receive your payment.");
            return 0;
        }
        if (!mtmp->isshk) {
            pline("%s is not interested in your payment.", Monnam(mtmp));
            return 0;
        }
        if (mtmp != resident && distu(mtmp->mx, mtmp->my) > 2) {
            pline("%s is too far to receive your payment.", Shknam(mtmp));
            return 0;
        }
        shkp = mtmp;
    }

    if (!shkp) {
        debugpline0("dopay: null shkp.");
        return 0;
    }
 proceed:
    eshkp = ESHK(shkp);
    ltmp = eshkp->robbed;

    /* wake sleeping shk when someone who owes money offers payment */
    if (ltmp || eshkp->billct || eshkp->debit)
        rouse_shk(shkp, TRUE);

    if (!shkp->mcanmove || shkp->msleeping) { /* still asleep/paralyzed */
        pline("%s %s.", Shknam(shkp),
              rn2(2) ? "seems to be napping" : "doesn't respond");
        return 0;
    }

    if (shkp != resident && NOTANGRY(shkp)) {
        umoney = money_cnt(invent);
        if (!ltmp)
            You("do not owe %s anything.", shkname(shkp));
        else if (!umoney) {
            You("%shave no money.", stashed_gold ? "seem to " : "");
            if (stashed_gold)
                pline("But you have some gold stashed away.");
        } else {
            if (umoney > ltmp) {
                You("give %s the %ld gold piece%s %s asked for.",
                    shkname(shkp), ltmp, plur(ltmp), noit_mhe(shkp));
                pay(ltmp, shkp);
            } else {
                You("give %s all your%s gold.", shkname(shkp),
                    stashed_gold ? " openly kept" : "");
                pay(umoney, shkp);
                if (stashed_gold)
                    pline("But you have hidden gold!");
            }
            if ((umoney < ltmp / 2L) || (umoney < ltmp && stashed_gold))
                pline("Unfortunately, %s doesn't look satisfied.",
                      noit_mhe(shkp));
            else
                make_happy_shk(shkp, FALSE);
        }
        return 1;
    }

    /* ltmp is still eshkp->robbed here */
    if (!eshkp->billct && !eshkp->debit) {
        umoney = money_cnt(invent);
        if (!ltmp && NOTANGRY(shkp)) {
#if 0 /* Don't annoy player about being broke when looking at services */
            You("do not owe %s anything.", shkname(shkp));
            if (!umoney)
                pline(no_money, stashed_gold ? " seem to" : "");
#endif 
            if (eshkp->pbanned) {
                verbalize("I don't service your kind here.");
                return 0;
            }
            if (!shk_other_services())
                return 0;
        } else if (ltmp) {
            pline("%s is after blood, not money!", shkname(shkp));
            if (umoney < ltmp / 2L || (umoney < ltmp && stashed_gold)) {
                if (!umoney)
                    pline(no_money, stashed_gold ? " seem to" : "");
                else
                    pline(not_enough_money, noit_mhim(shkp));
                return 1;
            }
            pline("But since %s shop has been robbed recently,",
                  noit_mhis(shkp));
            pline("you %scompensate %s for %s losses.",
                  (umoney < ltmp) ? "partially " : "", shkname(shkp),
                  noit_mhis(shkp));
            pay(umoney < ltmp ? umoney : ltmp, shkp);
            make_happy_shk(shkp, FALSE);
        } else {
            /* Blackmarket shopkeeper are not easily pacified */
            int peace_offering = (shkp->data == &mons[PM_ONE_EYED_SAM]) 
                                  ? 5000L : 1000L;

            /* shopkeeper is angry, but has not been robbed --
             * door broken, attacked, etc. */
            pline("%s is after your hide, not your money!", Shknam(shkp));
            if (umoney < peace_offering) {
                if (!umoney)
                    pline(no_money, stashed_gold ? " seem to" : "");
                else
                    pline(not_enough_money, noit_mhim(shkp));
                return 1;
            }
            You("try to appease %s by giving %s %d gold pieces.",
                canspotmon(shkp) ? x_monnam(shkp, ARTICLE_THE, "angry", 0, FALSE) : shkname(shkp),
                noit_mhim(shkp), peace_offering);
            pay(peace_offering, shkp);
            if (strncmp(eshkp->customer, plname, PL_NSIZ) || rn2(3))
                make_happy_shk(shkp, FALSE);
            else
                pline("But %s is as angry as ever.", shkname(shkp));
        }
        return 1;
    }
    if (shkp != resident) {
        impossible("dopay: not to shopkeeper?");
        if (resident)
            setpaid(resident);
        return 0;
    }
    /* pay debt, if any, first */
    if (eshkp->debit) {
        long dtmp = eshkp->debit;
        long loan = eshkp->loan;
        char sbuf[BUFSZ];

        umoney = money_cnt(invent);
        Sprintf(sbuf, "You owe %s %ld %s ", shkname(shkp), dtmp,
                currency(dtmp));
        if (loan) {
            if (loan == dtmp)
                Strcat(sbuf, "you picked up in the store.");
            else
                Strcat(sbuf,
                       "for gold picked up and the use of merchandise.");
        } else
            Strcat(sbuf, "for the use of merchandise.");
        pline1(sbuf);
        if (umoney + eshkp->credit < dtmp) {
            pline("But you don't%s have enough gold%s.",
                  stashed_gold ? " seem to" : "",
                  eshkp->credit ? " or credit" : "");
            return 1;
        } else {
            if (eshkp->credit >= dtmp) {
                eshkp->credit -= dtmp;
                eshkp->debit = 0L;
                eshkp->loan = 0L;
                Your("debt is covered by your credit.");
            } else if (!eshkp->credit) {
                money2mon(shkp, dtmp);
                eshkp->debit = 0L;
                eshkp->loan = 0L;
                You("pay that debt.");
                context.botl = 1;
            } else {
                dtmp -= eshkp->credit;
                eshkp->credit = 0L;
                money2mon(shkp, dtmp);
                eshkp->debit = 0L;
                eshkp->loan = 0L;
                pline("That debt is partially offset by your credit.");
                You("pay the remainder.");
                context.botl = 1;
            }
            paid = TRUE;
        }
    }
    /* now check items on bill */
    if (eshkp->billct) {
        register boolean itemize;
        int iprompt;

        umoney = money_cnt(invent);
        if (!umoney && !eshkp->credit) {
            You("%shave no money or credit%s.",
                stashed_gold ? "seem to " : "", paid ? " left" : "");
            return 0;
        }
        if ((umoney + eshkp->credit) < cheapest_item(shkp)) {
            You("don't have enough money to buy%s the item%s you picked.",
                eshkp->billct > 1 ? " any of" : "", plur(eshkp->billct));
            if (stashed_gold)
                pline("Maybe you have some gold stashed away?");
            return 0;
        }

        /* this isn't quite right; it itemizes without asking if the
         * single item on the bill is partly used up and partly unpaid */
        iprompt = (eshkp->billct > 1 ? ynq("Itemized billing?") : 'y');
        itemize = (iprompt == 'y');
        if (iprompt == 'q')
            goto thanks;

        for (pass = 0; pass <= 1; pass++) {
            tmp = 0;
            while (tmp < eshkp->billct) {
                struct obj *otmp;
                register struct bill_x *bp = &(eshkp->bill_p[tmp]);

                /* find the object on one of the lists */
                if ((otmp = bp_to_obj(bp)) != 0) {
                    /* if completely used up, object quantity is stale;
                       restoring it to its original value here avoids
                       making the partly-used-up code more complicated */
                    if (bp->useup)
                        otmp->quan = bp->bquan;
                } else {
                    impossible("Shopkeeper administration out of order.");
                    setpaid(shkp); /* be nice to the player */
                    return 1;
                }
                if (pass == bp->useup && otmp->quan == bp->bquan) {
                    /* pay for used-up items on first pass and others
                     * on second, so player will be stuck in the store
                     * less often; things which are partly used up
                     * are processed on both passes */
                    tmp++;
                } else {
                    switch (dopayobj(shkp, bp, &otmp, pass, itemize)) {
                    case PAY_CANT:
                        return 1; /*break*/
                    case PAY_BROKE:
                        paid = TRUE;
                        goto thanks; /*break*/
                    case PAY_SKIP:
                        tmp++;
                        continue; /*break*/
                    case PAY_SOME:
                        paid = TRUE;
                        if (itemize)
                            bot();
                        continue; /*break*/
                    case PAY_BUY:
                        paid = TRUE;
                        break;
                    }
                    if (itemize)
                        bot();
                    *bp = eshkp->bill_p[--eshkp->billct];
                }
            }
        }
 thanks:
        if (!itemize)
            update_inventory(); /* Done in dopayobj() if itemize. */
    }
    if (!ANGRY(shkp) && paid) {
        const char *punct = (hostile_shk(shkp) ? "." : "!");

        if (!Deaf && !muteshk(shkp)) {
            verbalize("%s for shopping in %s %s%s",
                      hostile_shk(shkp) ? "Yeah, thanks" : "Thank you",
                      s_suffix(shkname(shkp)),
                      shtypes[eshkp->shoptype - SHOPBASE].name, punct);
            if (hostile_shk(shkp))
                verbalize("%s", rn2(2) ? "Move along now..."
                                       : "Feel free to leave anytime.");
        } else {
            pline("%s nods %sat you for shopping in %s %s%s",
                  Shknam(shkp),
                  hostile_shk(shkp) ? "" : "appreciatively ",
                  noit_mhis(shkp),
                  shtypes[eshkp->shoptype - SHOPBASE].name, punct);
        }
    }
    return 1;
}

/*
** Called when you don't owe any money.  
 * Called after all checks have been
** made (in shop, not angry shopkeeper, etc.)
*/
static int
shk_other_services()
{
    const char *slang;		/* What shk calls you		*/
    struct monst *shkp;		/* The shopkeeper		*/
    struct permonst *shkdat;
    /*WAC - Windowstuff*/
    winid tmpwin;
    anything any;
    menu_item *selected;
    int n;
    int result = 0;
    
    /* Init the shopkeeper */
    shkp = shop_keeper(/* roomno= */*u.ushops);
    shkdat = &mons[ERAC(shkp)->rmnum];
    
    /* Init your name */
    if (!same_race(youmonst.data, shkdat))
        slang = "ugly";
    else
        slang = flags.female ? "lady" : "buddy";
    
    if (!ESHK(shkp)->services) {
        pline("Sorry %s, I have no services to offer you.", slang);
        return result;
    }
    
    /* Figure out what services he/she offers */
    /*WAC - did this using the windowing system...*/
    any.a_void = 0;         /* zero out all bits */
    tmpwin = create_nhwindow(NHW_MENU);

    start_menu(tmpwin);
    
    /* All shops can identify (some better than others) */
    if (ESHK(shkp)->services & SHK_ID_BASIC) {
        any.a_int = 1;
        add_menu(tmpwin, NO_GLYPH, &any, 'i', 0, ATR_NONE, 
                 "Basic Identify", MENU_ITEMFLAGS_NONE);
    }
    if (ESHK(shkp)->services & SHK_ID_PREMIUM) {
        any.a_int = 2;
        add_menu(tmpwin, NO_GLYPH, &any , 'I', 0, ATR_NONE,
                 "Premier Identify", MENU_ITEMFLAGS_NONE);
    }
    
    if (ESHK(shkp)->services & SHK_ID_WEAPON) {
        any.a_int = 3;
        add_menu(tmpwin, NO_GLYPH, &any , ')', 0, ATR_NONE,
                 "Identify Weapon", MENU_ITEMFLAGS_NONE);
    }
    
    if (ESHK(shkp)->services & SHK_ID_ARMOR) {
        any.a_int = 4;
        add_menu(tmpwin, NO_GLYPH, &any , '[', 0, ATR_NONE,
                 "Identify Armor", MENU_ITEMFLAGS_NONE);
    }
    
    if (ESHK(shkp)->services & SHK_ID_SCROLL) {
        any.a_int = 5;
        add_menu(tmpwin, NO_GLYPH, &any , '?', 0, ATR_NONE,
                 "Identify Scroll", MENU_ITEMFLAGS_NONE);
    }
    
    if (ESHK(shkp)->services & SHK_ID_BOOK) {
        any.a_int = 6;
        add_menu(tmpwin, NO_GLYPH, &any , '+', 0, ATR_NONE,
                 "Identify Spellbook", MENU_ITEMFLAGS_NONE);
    }
    
    if (ESHK(shkp)->services & SHK_ID_POTION) {
        any.a_int = 7;
        add_menu(tmpwin, NO_GLYPH, &any , '!', 0, ATR_NONE,
                 "Identify Potion", MENU_ITEMFLAGS_NONE);
    }
    
    if (ESHK(shkp)->services & SHK_ID_RING) {
        any.a_int = 8;
        add_menu(tmpwin, NO_GLYPH, &any , '=', 0, ATR_NONE,
                 "Identify Ring", MENU_ITEMFLAGS_NONE);
    }
    
    if (ESHK(shkp)->services & SHK_ID_AMULET) {
        any.a_int = 9;
        add_menu(tmpwin, NO_GLYPH, &any , '"', 0, ATR_NONE,
                 "Identify Amulet", MENU_ITEMFLAGS_NONE);
    }
    
    if (ESHK(shkp)->services & SHK_ID_WAND) {
        any.a_int = 10;
        add_menu(tmpwin, NO_GLYPH, &any , '/', 0, ATR_NONE,
                 "Identify Wand", MENU_ITEMFLAGS_NONE);
    }
    
    if (ESHK(shkp)->services & SHK_ID_TOOL) {
        any.a_int = 11;
        add_menu(tmpwin, NO_GLYPH, &any , '(', 0, ATR_NONE,
                 "Identify Tool", MENU_ITEMFLAGS_NONE);
    }
    
    if (ESHK(shkp)->services & SHK_ID_GEM) {
        any.a_int = 12;
        add_menu(tmpwin, NO_GLYPH, &any , '*', 0, ATR_NONE,
                 "Identify Gem/Stone", MENU_ITEMFLAGS_NONE);
    }
    
    if (ESHK(shkp)->services & SHK_ID_FOOD) {
        any.a_int = 13;
        add_menu(tmpwin, NO_GLYPH, &any , '%', 0, ATR_NONE,
                 "Identify Food", MENU_ITEMFLAGS_NONE);
    }
    
    /* All shops can potentially uncurse */
    if (ESHK(shkp)->services & SHK_UNCURSE) {
        any.a_int = 15;
        add_menu(tmpwin, NO_GLYPH, &any , 'u', 0, ATR_NONE,
                 "Uncurse", MENU_ITEMFLAGS_NONE);
    }
    
    /* Weapon-works!  Only a weapon store. */
    if (shk_class_match(WEAPON_CLASS, shkp) == SHK_MATCH) {
        if (ESHK(shkp)->services & SHK_WEP_FIX) {
            any.a_int = 16;
            add_menu(tmpwin, NO_GLYPH, &any , 'f', 0, ATR_NONE,
                     "Fix/Proof Weapon", MENU_UNSELECTED);
        }
        if (ESHK(shkp)->services & SHK_WEP_ENC) {
            any.a_int = 17;
            add_menu(tmpwin, NO_GLYPH, &any , 'e', 0, ATR_NONE,
                     "Enchant Weapon", MENU_UNSELECTED);
        }
        if (ESHK(shkp)->services & SHK_WEP_POI) {
            any.a_int = 18;
            add_menu(tmpwin, NO_GLYPH, &any , 'p', 0, ATR_NONE,
                     "Poison", MENU_ITEMFLAGS_NONE);
        }
    }
    
    /* Armor-works */
    if (shk_class_match(ARMOR_CLASS, shkp) == SHK_MATCH) {
        if ((ESHK(shkp)->services & SHK_ARM_FIX)) {
            any.a_int = 19;
            add_menu(tmpwin, NO_GLYPH, &any , 'f', 0, ATR_NONE,
                     "Fix/Proof Armor", MENU_UNSELECTED);
        }
        if ((ESHK(shkp)->services & SHK_ARM_ENC)) {
            any.a_int = 20;
            add_menu(tmpwin, NO_GLYPH, &any , 'e', 0, ATR_NONE,
                     "Enchant Armor", MENU_UNSELECTED);
        }
    }
    
    /* Basic Charging: / ( = */
    if (ESHK(shkp)->services & SHK_CHG_BAS) {
        any.a_int = 21;
        add_menu(tmpwin, NO_GLYPH, &any , 'c', 0, ATR_NONE,
                 "Basic Charging", MENU_ITEMFLAGS_NONE);
    }
    
    /* Premier/Complete Charging: / ( = */
    if (ESHK(shkp)->services & SHK_CHG_PRE) {
        any.a_int = 22;
        add_menu(tmpwin, NO_GLYPH, &any , 'C', 0, ATR_NONE,
                 "Premier Charging", MENU_ITEMFLAGS_NONE);
    }
    
    /* Rumors */
    if (ESHK(shkp)->services & SHK_RUMOR) {
        any.a_int = 23;
        add_menu(tmpwin, NO_GLYPH, &any, 'r', 0, ATR_NONE, "Rumors",
                 MENU_ITEMFLAGS_NONE);
    }
    /* Firearms training */
    if (ESHK(shkp)->services & SHK_FIREARMS) {
        any.a_int = 24;
        add_menu(tmpwin, NO_GLYPH, &any, 't', 0, ATR_NONE, "Train firearms",
                 MENU_ITEMFLAGS_NONE);
    }
    
    /* Tinker*/
    if (ESHK(shkp)->services & SHK_TINKER) {
        any.a_int = 25;
        add_menu(tmpwin, NO_GLYPH, &any, 'T', 0, ATR_NONE, "Tinker",
                 MENU_ITEMFLAGS_NONE);
    }
    
    end_menu(tmpwin, "Services Available:");
    n = select_menu(tmpwin, PICK_ONE, &selected);
    destroy_nhwindow(tmpwin);
    
    if (n > 0) {
        switch (selected[0].item.a_int) {
        /* Identification services */
        case 1:
            result = shk_identify(slang, shkp, SHK_ID_BASIC);
            break;
        case 2:
            result = shk_identify(slang, shkp, SHK_ID_PREMIUM);
            break;
        case 3:
            result = shk_identify(slang, shkp, SHK_ID_WEAPON);
            break;
        case 4:
            result = shk_identify(slang, shkp, SHK_ID_ARMOR);
            break;
        case 5:
            result = shk_identify(slang, shkp, SHK_ID_SCROLL);
            break;
        case 6:
            result = shk_identify(slang, shkp, SHK_ID_BOOK);
            break;
        case 7:
            result = shk_identify(slang, shkp, SHK_ID_POTION);
            break;
        case 8:
            result = shk_identify(slang, shkp, SHK_ID_RING);
            break;
        case 9:
            result = shk_identify(slang, shkp, SHK_ID_AMULET);
            break;
        case 10:
            result = shk_identify(slang, shkp, SHK_ID_WAND);
            break;
        case 11:
            result = shk_identify(slang, shkp, SHK_ID_TOOL);
            break;
        case 12:
            result = shk_identify(slang, shkp, SHK_ID_GEM);
            break;
        case 13:
            result = shk_identify(slang, shkp, SHK_ID_FOOD);
            break;
        case 14:
            result = shk_appraisal(slang, shkp);
            break;
        case 15:
            result = shk_uncurse(slang, shkp);
            break;
        case 16:
            result = shk_weapon_works(slang, shkp, SHK_WEP_FIX);
            break;
        case 17:
            result = shk_weapon_works(slang, shkp, SHK_WEP_ENC);
            break;
        case 18:
            result = shk_weapon_works(slang, shkp, SHK_WEP_POI);
            break;
        case 19:
            result = shk_armor_works(slang, shkp, SHK_ARM_FIX);
            break;
        case 20:
            result = shk_armor_works(slang, shkp, SHK_ARM_ENC);
            break;
        case 21:
            result = shk_charge(slang, shkp, 'b');
            break;
        case 22:
            result = shk_charge(slang, shkp, 'p');
            break;
        case 23:
            result = shk_rumor(slang, shkp);
            break;
        case 24:
            result = shk_firearms(slang, shkp);
            break;
        case 25:
            result = shk_tinker(slang, shkp);
            break;
        default:
            pline("Unknown Service");
            break;
        }
    }
    free((genericptr_t) selected);
    return result;
}

/* return 2 if used-up portion paid
 *        1 if paid successfully
 *        0 if not enough money
 *       -1 if skip this object
 *       -2 if no money/credit left
 */
STATIC_OVL int
dopayobj(shkp, bp, obj_p, which, itemize)
register struct monst *shkp;
register struct bill_x *bp;
struct obj **obj_p;
int which; /* 0 => used-up item, 1 => other (unpaid or lost) */
boolean itemize;
{
    register struct obj *obj = *obj_p;
    long ltmp, quan, save_quan;
    long umoney = money_cnt(invent);
    int buy;
    boolean stashed_gold = (hidden_gold() > 0L), consumed = (which == 0);

    if (!obj->unpaid && !bp->useup) {
        impossible("Paid object on bill??");
        return PAY_BUY;
    }
    if (itemize && umoney + ESHK(shkp)->credit == 0L) {
        You("%shave no money or credit left.",
            stashed_gold ? "seem to " : "");
        return PAY_BROKE;
    }
    /* we may need to temporarily adjust the object, if part of the
       original quantity has been used up but part remains unpaid  */
    save_quan = obj->quan;
    if (consumed) {
        /* either completely used up (simple), or split needed */
        quan = bp->bquan;
        if (quan > obj->quan) /* difference is amount used up */
            quan -= obj->quan;
    } else {
        /* dealing with ordinary unpaid item */
        quan = obj->quan;
    }
    obj->quan = quan;        /* to be used by doname() */
    obj->unpaid = 0;         /* ditto */
    iflags.suppress_price++; /* affects containers */
    ltmp = bp->price * quan;
    buy = PAY_BUY; /* flag; if changed then return early */

    if (itemize) {
        char qbuf[BUFSZ], qsfx[BUFSZ];

        Sprintf(qsfx, " for %ld %s.  Pay?", ltmp, currency(ltmp));
        (void) safe_qbuf(qbuf, (char *) 0, qsfx, obj,
                         (quan == 1L) ? Doname2 : doname, ansimpleoname,
                         (quan == 1L) ? "that" : "those");
        if (yn(qbuf) == 'n') {
            buy = PAY_SKIP;                         /* don't want to buy */
        } else if (quan < bp->bquan && !consumed) { /* partly used goods */
            obj->quan = bp->bquan - save_quan;      /* used up amount */
            if (!Deaf && !muteshk(shkp)) {
                verbalize("%s for the other %s before buying %s.",
                      (ANGRY(shkp) || hostile_shk(shkp))
                               ? "Pay" : "Please pay",
                      simpleonames(obj), /* short name suffices */
                      save_quan > 1L ? "these" : "this one");
            } else {
                pline("%s %s%s your bill for the other %s first.",
                      Shknam(shkp),
                      (ANGRY(shkp) || hostile_shk(shkp))
                               ? "angrily " : "",
                      nolimbs(shkp->data) ? "motions to" : "points out",
                      simpleonames(obj));
            }
            buy = PAY_SKIP; /* shk won't sell */
        }
    }
    if (buy == PAY_BUY && umoney + ESHK(shkp)->credit < ltmp) {
        You("don't%s have gold%s enough to pay for %s.",
            stashed_gold ? " seem to" : "",
            (ESHK(shkp)->credit > 0L) ? " or credit" : "",
            thesimpleoname(obj));
        buy = itemize ? PAY_SKIP : PAY_CANT;
    }

    if (buy != PAY_BUY) {
        /* restore unpaid object to original state */
        obj->quan = save_quan;
        obj->unpaid = 1;
        iflags.suppress_price--;
        return buy;
    }

    pay(ltmp, shkp);
    shk_names_obj(shkp, obj,
                  consumed ? "paid for %s at a cost of %ld gold piece%s.%s"
                           : "bought %s for %ld gold piece%s.%s",
                  ltmp, "");
    obj->quan = save_quan; /* restore original count */
    /* quan => amount just bought, save_quan => remaining unpaid count */

    iflags.suppress_price--; /* before update_inventory() below */
    if (consumed) {
        if (quan != bp->bquan) {
            /* eliminate used-up portion; remainder is still unpaid */
            bp->bquan = obj->quan;
            obj->unpaid = 1;
            bp->useup = 0;
            buy = PAY_SOME;
        } else { /* completely used-up, so get rid of it */
            obj_extract_self(obj);
            /* assert( obj == *obj_p ); */
            dealloc_obj(obj);
            *obj_p = 0; /* destroy pointer to freed object */
        }
    } else if (itemize) {
        update_inventory(); /* Done just once in dopay() if !itemize. */
    }
    return buy;
}

static struct repo { /* repossession context */
    struct monst *shopkeeper;
    coord location;
} repo;

/* routine called after dying (or quitting) */
boolean
paybill(croaked, silently)
int croaked; /* -1: escaped dungeon; 0: quit; 1: died */
boolean silently; /* maybe avoid messages */
{
    struct monst *mtmp, *mtmp2, *firstshk, *resident, *creditor, *hostile,
        *localshk;
    struct eshk *eshkp;
    boolean taken = FALSE, local;
    int numsk = 0;

    /* if we escaped from the dungeon, shopkeepers can't reach us;
       shops don't occur on level 1, but this could happen if hero
       level teleports out of the dungeon and manages not to die */
    if (croaked < 0)
        return FALSE;
    /* [should probably also return false when dead hero has been
        petrified since shk shouldn't be able to grab inventory
        which has been shut inside a statue] */

    /* suppress message spam from lots of angry black marketeers */
    if (Is_blackmarket(&u.uz)) 
        return FALSE;

    /* this is where inventory will end up if any shk takes it */
    repo.location.x = repo.location.y = 0;
    repo.shopkeeper = 0;

    /*
     * Scan all shopkeepers on the level, to prioritize them:
     * 1) keeper of shop hero is inside and who is owed money,
     * 2) keeper of shop hero is inside who isn't owed any money,
     * 3) other shk who is owed money, 4) other shk who is angry,
     * 5) any shk local to this level, and if none is found,
     * 6) first shk on monster list (last resort; unlikely, since
     * any nonlocal shk will probably be in the owed category
     * and almost certainly be in the angry category).
     */
    resident = creditor = hostile = localshk = (struct monst *) 0;
    for (mtmp = next_shkp(fmon, FALSE); mtmp;
         mtmp = next_shkp(mtmp2, FALSE)) {
        mtmp2 = mtmp->nmon;
        eshkp = ESHK(mtmp);
        local = on_level(&eshkp->shoplevel, &u.uz);
        if (local && index(u.ushops, eshkp->shoproom)) {
            /* inside this shk's shop [there might be more than one
               resident shk if hero is standing in a breech of a shared
               wall, so give priority to one who's also owed money] */
            if (!resident || eshkp->billct || eshkp->debit || eshkp->robbed)
                resident = mtmp;
        } else if (eshkp->billct || eshkp->debit || eshkp->robbed) {
            /* owe this shopkeeper money (might also owe others) */
            if (!creditor)
                creditor = mtmp;
        } else if (eshkp->following || ANGRY(mtmp)) {
            /* this shopkeeper is antagonistic (others might be too) */
            if (!hostile)
                hostile = mtmp;
        } else if (local) {
            /* this shopkeeper's shop is on current level */
            if (!localshk)
                localshk = mtmp;
        }
    }

    /* give highest priority shopkeeper first crack */
    firstshk = resident ? resident
                        : creditor ? creditor
                                   : hostile ? hostile
                                             : localshk;
    if (firstshk) {
        numsk++;
        taken = inherits(firstshk, numsk, croaked, silently);
    }

    /* now handle the rest */
    for (mtmp = next_shkp(fmon, FALSE); mtmp;
         mtmp = next_shkp(mtmp2, FALSE)) {
        mtmp2 = mtmp->nmon;
        eshkp = ESHK(mtmp);
        local = on_level(&eshkp->shoplevel, &u.uz);
        if (mtmp != firstshk) {
            numsk++;
            taken |= inherits(mtmp, numsk, croaked, silently);
            ESHK(mtmp)->pbanned = FALSE; /* Un-ban for bones levels */
        }
        /* for bones: we don't want a shopless shk around */
        if (!local)
            mongone(mtmp);
    }
    return taken;
}

STATIC_OVL boolean
inherits(shkp, numsk, croaked, silently)
struct monst *shkp;
int numsk;
int croaked;
boolean silently;
{
    long loss = 0L;
    long umoney;
    struct eshk *eshkp = ESHK(shkp);
    boolean take = FALSE, taken = FALSE;
    unsigned save_minvis = shkp->minvis;
    int roomno = *u.ushops;
    char takes[BUFSZ];

    /* not strictly consistent; affects messages and prevents next player
       (if bones are saved) from blundering into or being ambused by an
       invisible shopkeeper */
    shkp->minvis = 0;
    /* The simplifying principle is that first-come
       already took everything you had. */
    if (numsk > 1) {
        if (cansee(shkp->mx, shkp->my) && croaked && !silently) {
            takes[0] = '\0';
            if (has_head(shkp->data) && !rn2(2))
                Sprintf(takes, ", shakes %s %s,", noit_mhis(shkp),
                        mbodypart(shkp, HEAD));
            pline("%s %slooks at your corpse%s and %s.", Shknam(shkp),
                  (!shkp->mcanmove || shkp->msleeping) ? "wakes up, " : "",
                  takes, !inhishop(shkp) ? "disappears" : "sighs");
        }
        rouse_shk(shkp, FALSE); /* wake shk for bones */
        taken = (roomno == eshkp->shoproom);
        goto skip;
    }

    /* get one case out of the way: you die in the shop, the */
    /* shopkeeper is peaceful, nothing stolen, nothing owed. */
    if (roomno == eshkp->shoproom && inhishop(shkp) && !eshkp->billct
        && !eshkp->robbed && !eshkp->debit && NOTANGRY(shkp)
        && !eshkp->following && u.ugrave_arise < LOW_PM) {
        taken = (invent != 0);
        if (taken && !silently)
            pline("%s %s inherits all your possessions.",
                  Shknam(shkp),
                  hostile_shk(shkp) ? "disapprovingly"
                                    : "gratefully");
        set_repo_loc(shkp);
        goto clear;
    }

    if (eshkp->billct || eshkp->debit || eshkp->robbed) {
        if (roomno == eshkp->shoproom && inhishop(shkp))
            loss = addupbill(shkp) + eshkp->debit;
        if (loss < eshkp->robbed)
            loss = eshkp->robbed;
        take = TRUE;
    }

    if (eshkp->following || ANGRY(shkp) || take) {
        if (!invent)
            goto skip;
        umoney = money_cnt(invent);
        takes[0] = '\0';
        if (!shkp->mcanmove || shkp->msleeping)
            Strcat(takes, "wakes up and ");
        if (distu(shkp->mx, shkp->my) > 2)
            Strcat(takes, "comes and ");
        Strcat(takes, "takes");

        if (loss > umoney || !loss || roomno == eshkp->shoproom) {
            eshkp->robbed -= umoney;
            if (eshkp->robbed < 0L)
                eshkp->robbed = 0L;
            if (umoney > 0L) {
                money2mon(shkp, umoney);
                context.botl = 1;
            }
            if (!silently)
                pline("%s %s all your possessions.", Shknam(shkp), takes);
            taken = TRUE;
            /* where to put player's invent (after disclosure) */
            set_repo_loc(shkp);
        } else {
            money2mon(shkp, loss);
            context.botl = 1;
            if (!silently)
                pline("%s %s the %ld %s %sowed %s.", Shknam(shkp),
                      takes, loss, currency(loss),
                      strncmp(eshkp->customer, plname, PL_NSIZ) ? "" : "you ",
                      noit_mhim(shkp));
            /* shopkeeper has now been paid in full */
            pacify_shk(shkp);
            eshkp->following = 0;
            eshkp->robbed = 0L;
        }
 skip:
        /* in case we create bones */
        rouse_shk(shkp, FALSE); /* wake up */
        if (!inhishop(shkp))
            home_shk(shkp, FALSE);
    }
 clear:
    shkp->minvis = save_minvis;
    setpaid(shkp);
    return taken;
}

STATIC_OVL void
set_repo_loc(shkp)
struct monst *shkp;
{
    register xchar ox, oy;
    struct eshk *eshkp = ESHK(shkp);

    /* if you're not in this shk's shop room, or if you're in its doorway
        or entry spot, then your gear gets dumped all the way inside */
    if (*u.ushops != eshkp->shoproom || IS_DOOR(levl[u.ux][u.uy].typ)
        || (u.ux == eshkp->shk.x && u.uy == eshkp->shk.y)) {
        /* shk.x,shk.y is the position immediately in
         * front of the door -- move in one more space
         */
        ox = eshkp->shk.x;
        oy = eshkp->shk.y;
        ox += sgn(ox - eshkp->shd.x);
        oy += sgn(oy - eshkp->shd.y);
    } else { /* already inside this shk's shop */
        ox = u.ux;
        oy = u.uy;
    }
    /* finish_paybill will deposit invent here */
    repo.location.x = ox;
    repo.location.y = oy;
    repo.shopkeeper = shkp;
}

/* called at game exit, after inventory disclosure but before making bones;
   shouldn't issue any messages */
void
finish_paybill()
{
    struct monst *shkp = repo.shopkeeper;
    int ox = repo.location.x, oy = repo.location.y;

#if 0 /* don't bother */
    if (ox == 0 && oy == 0)
        impossible("finish_paybill: no location");
#endif
    /* normally done by savebones(), but that's too late in this case */
    unleash_all();
    /* if hero has any gold left, take it into shopkeeper's possession */
    if (shkp) {
        long umoney = money_cnt(invent);

        if (umoney)
            money2mon(shkp, umoney);
    }
    /* transfer rest of the character's inventory to the shop floor */
    drop_upon_death((struct monst *) 0, (struct obj *) 0, ox, oy);
}

/* find obj on one of the lists */
STATIC_OVL struct obj *
bp_to_obj(bp)
register struct bill_x *bp;
{
    register struct obj *obj;
    register unsigned int id = bp->bo_id;

    if (bp->useup)
        obj = o_on(id, billobjs);
    else
        obj = find_oid(id);
    return obj;
}

/*
 * Look for o_id on all lists but billobj.  Return obj or NULL if not found.
 * Its OK for restore_timers() to call this function, there should not
 * be any timeouts on the billobjs chain.
 */
struct obj *
find_oid(id)
unsigned id;
{
    struct obj *obj;
    struct monst *mon, *mmtmp[3];
    int i;

    /* first check various obj lists directly */
    if ((obj = o_on(id, invent)) != 0)
        return obj;
    if ((obj = o_on(id, fobj)) != 0)
        return obj;
    if ((obj = o_on(id, level.buriedobjlist)) != 0)
        return obj;
    if ((obj = o_on(id, migrating_objs)) != 0)
        return obj;

    /* not found yet; check inventory for members of various monst lists */
    mmtmp[0] = fmon;
    mmtmp[1] = migrating_mons;
    mmtmp[2] = mydogs; /* for use during level changes */
    for (i = 0; i < 3; i++)
        for (mon = mmtmp[i]; mon; mon = mon->nmon)
            if ((obj = o_on(id, mon->minvent)) != 0)
                return obj;

    /* not found at all */
    return (struct obj *) 0;
}

/* Returns the price of an arbitrary item in the shop,
   0 if the item doesn't belong to a shopkeeper or hero is not in the shop. */
long
get_cost_of_shop_item(obj, nochrg)
register struct obj *obj;
int *nochrg; /* alternate return value: 1: no charge, 0: shop owned,        */
{            /* -1: not in a shop (so should't be formatted as "no charge") */
    struct monst *shkp;
    struct obj *top;
    xchar x, y;
    boolean freespot;
    long cost = 0L;

    *nochrg = -1; /* assume 'not applicable' */
    if (*u.ushops && obj->oclass != COIN_CLASS
        && obj != uball && obj != uchain
        && get_obj_location(obj, &x, &y, CONTAINED_TOO)
        && *in_rooms(x, y, SHOPBASE) == *u.ushops
        && (shkp = shop_keeper(inside_shop(x, y))) != 0 && inhishop(shkp)) {
        for (top = obj; top->where == OBJ_CONTAINED; top = top->ocontainer)
            continue;
        freespot = (top->where == OBJ_FLOOR
                    && x == ESHK(shkp)->shk.x && y == ESHK(shkp)->shk.y);
        /* no_charge is only set for items on floor or carried by pet inside
         * shop proper; items on freespot are implicitly 'no charge' */
        *nochrg = ((top->where == OBJ_FLOOR || top->where == OBJ_MINVENT)
                   && (obj->no_charge || freespot));

        if (carried(top) ? (int) obj->unpaid : !*nochrg) {
            long per_unit_cost = get_cost(obj, shkp);

            cost = get_pricing_units(obj) * per_unit_cost;
        }
        if (Has_contents(obj) && !freespot)
            cost += contained_cost(obj, shkp, 0L, FALSE, TRUE);
    }
    return cost;
}

STATIC_OVL long
get_pricing_units(obj)
struct obj *obj;
{
    long units = obj->quan;

    if (obj->globby) {
        /* globs must be sold by weight not by volume */
        long unit_weight = (long) objects[obj->otyp].oc_weight,
             wt = (obj->owt > 0) ? (long) obj->owt : (long) weight(obj);

        if (unit_weight)
            units = (wt + unit_weight - 1L) / unit_weight;
    }
    return units;
}

/* decide whether to apply a surcharge (or hypothetically, a discount) to obj
   if it had ID number 'oid'; returns 1: increase, 0: normal, -1: decrease */
int
oid_price_adjustment(obj, oid)
struct obj *obj;
unsigned oid;
{
    int res = 0, otyp = obj->otyp;

    if (!(obj->dknown && objects[otyp].oc_name_known)
        && (obj->oclass != GEM_CLASS || is_worthless_glass(obj))) {
        res = ((oid % 4) == 0); /* id%4 ==0 -> +1, ==1..3 -> 0 */
    }
    return res;
}


/* Relative prices for the different materials.
 * Units for this are much more poorly defined than for weights; the best
 * approximation would be something like "zorkmids per aum".
 * We only care about the ratio of two of these together. */
const int matprices[] = {
     0,
     1, /* LIQUID */
     1, /* WAX */
     1, /* VEGGY */
     3, /* FLESH */
     2, /* PAPER */
     3, /* CLOTH */
     5, /* LEATHER */
     8, /* WOOD */
    20, /* BONE */
   200, /* DRAGON_HIDE - DSM to scale mail */
    10, /* IRON */
    15, /* METAL */
    10, /* COPPER */
    30, /* SILVER */
    60, /* GOLD */
    80, /* PLATINUM */
    50, /* MITHRIL - mithril-coat to regular chain mail */
    10, /* PLASTIC */
    20, /* GLASS */
   500, /* GEMSTONE */
    10  /* MINERAL */
};

/* calculate the value that the shk will charge for [one of] an object */
STATIC_OVL long
get_cost(obj, shkp)
register struct obj *obj;
register struct monst *shkp; /* if angry, impose a surcharge */
{
    long tmp = getprice(obj, FALSE),
         /* used to perform a single calculation even when multiple
            adjustments (unID'd, dunce/tourist, charisma) are made */
         multiplier = 1L, divisor = 1L;

    if (!tmp)
        tmp = 5L;
    /* shopkeeper may notice if the player isn't very knowledgeable -
       especially when gem prices are concerned */
    if (!obj->dknown || !objects[obj->otyp].oc_name_known) {
        if (is_worthless_glass(obj)) {
            int i;
            /* get a value that's 'random' from game to game, but the
               same within the same game */
            boolean pseudorand =
                (((int) ubirthday % obj->otyp) >= obj->otyp / 2);

            /* all gems are priced high - real or not */
            switch (obj->otyp - LAST_GEM) {
            case 1: /* white */
                i = pseudorand ? DIAMOND : OPAL;
                break;
            case 2: /* blue */
                i = pseudorand ? SAPPHIRE : AQUAMARINE;
                break;
            case 3: /* red */
                i = pseudorand ? RUBY : JASPER;
                break;
            case 4: /* yellowish brown */
                i = pseudorand ? AMBER : TOPAZ;
                break;
            case 5: /* orange */
                i = pseudorand ? JACINTH : AGATE;
                break;
            case 6: /* yellow */
                i = pseudorand ? CITRINE : CHRYSOBERYL;
                break;
            case 7: /* black */
                i = pseudorand ? BLACK_OPAL : JET;
                break;
            case 8: /* green */
                i = pseudorand ? EMERALD : JADE;
                break;
            case 9: /* violet */
                i = pseudorand ? AMETHYST : FLUORITE;
                break;
            default:
                impossible("bad glass gem %d?", obj->otyp);
                i = STRANGE_OBJECT;
                break;
            }
            tmp = (long) objects[i].oc_cost;
        } else if (oid_price_adjustment(obj, obj->o_id) > 0) {
            /* unid'd, arbitrarily impose surcharge: tmp *= 4/3 */
            multiplier *= 4L;
            divisor *= 3L;
        }
    }
    /* adjust for different material */
    multiplier *= matprices[obj->material];
    divisor *= matprices[objects[obj->otyp].oc_material];

    if (uarmh && uarmh->otyp == DUNCE_CAP)
        multiplier *= 4L, divisor *= 3L;
    else if ((Role_if(PM_TOURIST) && u.ulevel < (MAXULEV / 2))
             || (uarmu && !uarm && !uarmc)) /* touristy shirt visible */
        multiplier *= 4L, divisor *= 3L;

    if (ACURR(A_CHA) > 18)
        divisor *= 2L;
    else if (ACURR(A_CHA) == 18)
        multiplier *= 2L, divisor *= 3L;
    else if (ACURR(A_CHA) >= 16)
        multiplier *= 3L, divisor *= 4L;
    else if (ACURR(A_CHA) <= 5)
        multiplier *= 2L;
    else if (ACURR(A_CHA) <= 7)
        multiplier *= 3L, divisor *= 2L;
    else if (ACURR(A_CHA) <= 10)
        multiplier *= 4L, divisor *= 3L;
    
    /* tmp = (tmp * multiplier) / divisor [with roundoff tweak] */
    tmp *= multiplier;
    if (divisor > 1L) {
        /* tmp = (((tmp * 10) / divisor) + 5) / 10 */
        tmp *= 10L;
        tmp /= divisor;
        tmp += 5L;
        tmp /= 10L;
    }

    if (tmp <= 0L)
        tmp = 1L;
    /* the artifact prices in artilist[] are also used as a score bonus;
       inflate their shop price here without affecting score calculation */
    if (obj->oartifact)
        tmp *= 4L;

#if 0 /* This exploit has been fixed by other means */
    /* if the player tries to game the system and buy an item
       for less than what they can sell it for, adjust the price
       so the best they can do is break even */
    if (tmp < (objects[obj->otyp].oc_cost / 2))
        tmp = (objects[obj->otyp].oc_cost / 2);
#endif
    
    /* anger surcharge should match rile_shk's, so we do it separately
       from the multiplier/divisor calculation */
    if (shkp && ESHK(shkp)->surcharge)
        tmp += (tmp + 2L) / 3L;

    /* KMH, balance patch -- healthstone replaces rotting/health */
    /* Used UnNetHack pricing instead of slashem's - more fair. */
    if (Is_blackmarket(&u.uz)) {
        if (obj->oclass == RING_CLASS    
            || obj->oclass == AMULET_CLASS   
            || obj->oclass == POTION_CLASS  
            || obj->oclass == SCROLL_CLASS   
            || obj->oclass == SPBOOK_CLASS  
            || obj->oclass == WAND_CLASS     
            || obj->otyp == LUCKSTONE       
            || obj->otyp == LOADSTONE          
            || objects[obj->otyp].oc_magic) {
            tmp *= Role_if(PM_CONVICT) ? 20 : 25;
        } else {
            tmp *= Role_if(PM_CONVICT) ? 12 : 15;
        }
    }
    return tmp;
}

/* returns the price of a container's content.  the price
 * of the "top" container is added in the calling functions.
 * a different price quoted for selling as vs. buying.
 */
long
contained_cost(obj, shkp, price, usell, unpaid_only)
struct obj *obj;
struct monst *shkp;
long price;
boolean usell;
boolean unpaid_only;
{
    register struct obj *otmp, *top;
    xchar x, y;
    boolean on_floor, freespot;

    for (top = obj; top->where == OBJ_CONTAINED; top = top->ocontainer)
        continue;
    /* pick_obj() removes item from floor, adds it to shop bill, then
       puts it in inventory; behave as if it is still on the floor
       during the add-to-bill portion of that situation */
    on_floor = (top->where == OBJ_FLOOR || top->where == OBJ_FREE);
    if (top->where == OBJ_FREE || !get_obj_location(top, &x, &y, 0))
        x = u.ux, y = u.uy;
    freespot = (on_floor && x == ESHK(shkp)->shk.x && y == ESHK(shkp)->shk.y);

    /* price of contained objects; "top" container handled by caller */
    for (otmp = obj->cobj; otmp; otmp = otmp->nobj) {
        if (otmp->oclass == COIN_CLASS)
            continue;

        if (usell) {
            if (saleable(shkp, otmp) && !otmp->unpaid
                && otmp->oclass != BALL_CLASS
                && !(otmp->oclass == FOOD_CLASS && otmp->oeaten)
                && !(Is_candle(otmp)
                     && otmp->age < 20L * (long) objects[otmp->otyp].oc_cost))
                price += set_cost(otmp, shkp);
        } else {
            /* no_charge is only set for floor items (including
               contents of floor containers) inside shop proper;
               items on freespot are implicitly 'no charge' */
            if (on_floor ? (!otmp->no_charge && !freespot)
                         : (otmp->unpaid || !unpaid_only))
                price += get_cost(otmp, shkp) * get_pricing_units(otmp);
        }

        if (Has_contents(otmp))
            price = contained_cost(otmp, shkp, price, usell, unpaid_only);
    }

    return price;
}

/* count amount of gold inside container 'obj' and any nested containers */
long
contained_gold(obj)
struct obj *obj;
{
    register struct obj *otmp;
    register long value = 0L;

    /* accumulate contained gold */
    for (otmp = obj->cobj; otmp; otmp = otmp->nobj)
        if (otmp->oclass == COIN_CLASS)
            value += otmp->quan;
        else if (Has_contents(otmp))
            value += contained_gold(otmp);

    return value;
}

STATIC_OVL void
dropped_container(obj, shkp, sale)
register struct obj *obj;
register struct monst *shkp;
register boolean sale;
{
    register struct obj *otmp;

    /* the "top" container is treated in the calling fn */
    for (otmp = obj->cobj; otmp; otmp = otmp->nobj) {
        if (otmp->oclass == COIN_CLASS)
            continue;

        if (!otmp->unpaid && !(sale && saleable(shkp, otmp)))
            otmp->no_charge = 1;

        if (Has_contents(otmp))
            dropped_container(otmp, shkp, sale);
    }
}

void
picked_container(obj)
register struct obj *obj;
{
    register struct obj *otmp;

    /* the "top" container is treated in the calling fn */
    for (otmp = obj->cobj; otmp; otmp = otmp->nobj) {
        if (otmp->oclass == COIN_CLASS)
            continue;

        if (otmp->no_charge)
            otmp->no_charge = 0;

        if (Has_contents(otmp))
            picked_container(otmp);
    }
}

STATIC_OVL boolean
special_stock(obj, shkp, quietly)
struct obj *obj;
struct monst *shkp;
boolean quietly;
{
    /* for unique situations */
    if (ESHK(shkp)->shoptype == CANDLESHOP
        && obj->otyp == CANDELABRUM_OF_INVOCATION) {
        if (!quietly) {
            if (is_izchak(shkp, TRUE) && !u.uevent.invoked) {
                if (Deaf || muteshk(shkp)) {
                    pline("%s seems %s that you want to sell that.",
                          Shknam(shkp),
                          (obj->spe < 7) ? "horrified" : "concerned");
                } else {
                    verbalize("No thanks, I'd hang onto that if I were you.");
                    if (obj->spe < 7)
                        verbalize(
                             "You'll need %d%s candle%s to go along with it.",
                                (7 - obj->spe), (obj->spe > 0) ? " more" : "",
                                  plur(7 - obj->spe));
                    /* [what if hero is already carrying enough candles?
                       should Izchak explain how to attach them instead?] */
                }
            } else {
                if (!Deaf && !muteshk(shkp))
                    verbalize("I won't stock that.  Take it out of here!");
                else
                    pline("%s shakes %s %s in refusal.",
                          Shknam(shkp), noit_mhis(shkp),
                          mbodypart(shkp, HEAD));
            }
        }
        return TRUE;
    }
    return FALSE;
}

/* calculate how much the shk will pay when buying [all of] an object */
STATIC_OVL long
set_cost(obj, shkp)
register struct obj *obj;
register struct monst *shkp;
{
    long tmp, unit_price = getprice(obj, TRUE), multiplier = 1L, divisor = 1L;

    tmp = get_pricing_units(obj) * unit_price;

    /* adjust for different material */
    multiplier *= matprices[obj->material];
    divisor *= matprices[objects[obj->otyp].oc_material];

    if (obj->globby)
        goto end;

    if (uarmh && uarmh->otyp == DUNCE_CAP)
        divisor *= 3L;
    else if ((Role_if(PM_TOURIST) && u.ulevel < (MAXULEV / 2))
             || (uarmu && !uarm && !uarmc)) /* touristy shirt visible */
        divisor *= 3L;
    else
        divisor *= 2L;

    /* shopkeeper may notice if the player isn't very knowledgeable -
       especially when gem prices are concerned */
    if (!obj->dknown || !objects[obj->otyp].oc_name_known) {
        if (obj->oclass == GEM_CLASS) {
            /* different shop keepers give different prices */
            if (obj->material == GEMSTONE
                || is_worthless_glass(obj)) {
                tmp = (obj->otyp % (6 - shkp->m_id % 3));
                tmp = (tmp + 3) * obj->quan;
            }
        } else if (obj->otyp == WAN_NOTHING) {
            tmp = 100; /* Standard cost */
        } else if (tmp > 1L && !(shkp->m_id % 4)) {
            multiplier *= 3L, divisor *= 4L;
        }
    }

   
    if (tmp >= 1L) {
        /* [see get_cost()] */
        tmp *= multiplier;
        if (divisor > 1L) {
            tmp *= 10L;
            tmp /= divisor;
            tmp += 5L;
            tmp /= 10L;
        }
        /* avoid adjusting nonzero to zero */
        if (tmp < 1L)
            tmp = 1L;
    }

end:
    /* (no adjustment for angry shk here) */
    return tmp;
}

/* unlike alter_cost() which operates on a specific item, identifying or
   forgetting a gem causes all unpaid gems of its type to change value */
void
gem_learned(oindx)
int oindx;
{
    struct obj *obj;
    struct monst *shkp;
    struct bill_x *bp;
    int ct;

    /*
     * Unfortunately, shop bill doesn't have object type included,
     * just obj->oid for each unpaid stack, so we have to go through
     * every bill and every item on that bill and match up against
     * every unpaid stack on the level....
     *
     * Fortunately, there's no need to catch up when changing dungeon
     * levels even if we ID'd or forget some gems while gone from a
     * level.  There won't be any shop bills when arriving; they were
     * either paid before leaving or got treated as robbery and it's
     * too late to adjust pricing.
     */
    for (shkp = next_shkp(fmon, TRUE); shkp;
         shkp = next_shkp(shkp->nmon, TRUE)) {
        ct = ESHK(shkp)->billct;
        bp = ESHK(shkp)->bill;
        while (--ct >= 0) {
            obj = find_oid(bp->bo_id);
            if (!obj) /* shouldn't happen */
                continue;
            if ((oindx != STRANGE_OBJECT) ? (obj->otyp == oindx)
                                          : (obj->oclass == GEM_CLASS))
                bp->price = get_cost(obj, shkp);
            ++bp;
        }
    }
}

/* called when an item's value has been enhanced; if it happens to be
   on any shop bill, update that bill to reflect the new higher price
   [if the new price drops for some reason, keep the old one in place] */
void
alter_cost(obj, amt)
struct obj *obj;
long amt; /* if 0, use regular shop pricing, otherwise force amount;
             if negative, use abs(amt) even if it's less than old cost */
{
    struct bill_x *bp = 0;
    struct monst *shkp;
    long new_price;

    for (shkp = next_shkp(fmon, TRUE); shkp; shkp = next_shkp(shkp, TRUE))
        if ((bp = onbill(obj, shkp, TRUE)) != 0) {
            new_price = !amt ? get_cost(obj, shkp) : (amt < 0L) ? -amt : amt;
            if (new_price > bp->price || amt < 0L) {
                bp->price = new_price;
                update_inventory();
            }
            break; /* done */
        }
    return;
}

/* called from doinv(invent.c) for inventory of unpaid objects */
long
unpaid_cost(unp_obj, include_contents)
struct obj *unp_obj; /* known to be unpaid or contain unpaid */
boolean include_contents;
{
    struct bill_x *bp = (struct bill_x *) 0;
    struct monst *shkp = 0;
    char *shop;
    long amt = 0L;

#if 0   /* if two shops share a wall, this might find wrong shk */
    xchar ox, oy;

    if (!get_obj_location(unp_obj, &ox, &oy, BURIED_TOO | CONTAINED_TOO))
        ox = u.ux, oy = u.uy; /* (shouldn't happen) */
    if ((shkp = shop_keeper(*in_rooms(ox, oy, SHOPBASE))) != 0) {
        bp = onbill(unp_obj, shkp, TRUE);
    } else {
        /* didn't find shk?  try searching bills */
        for (shkp = next_shkp(fmon, TRUE); shkp;
             shkp = next_shkp(shkp->nmon, TRUE))
            if ((bp = onbill(unp_obj, shkp, TRUE)) != 0)
                break;
    }
#endif
    for (shop = u.ushops; *shop; shop++) {
        if ((shkp = shop_keeper(*shop))) {
            if ((bp = onbill(unp_obj, shkp, TRUE)))
                amt = unp_obj->quan * bp->price;
            if (include_contents && Has_contents(unp_obj))
                amt = contained_cost(unp_obj, shkp, amt, FALSE, TRUE);
            if (bp || (!unp_obj->unpaid && amt))
                break;
        }
    }

    /* onbill() gave no message if unexpected problem occurred */
    if (!tech_inuse(T_TELEKINESIS) && (!shkp || (unp_obj->unpaid && !bp)))
        impossible("unpaid_cost: object wasn't on any bill.");
    return amt;
}

STATIC_OVL void
add_one_tobill(obj, dummy, shkp)
struct obj *obj;
boolean dummy;
struct monst *shkp;
{
    struct eshk *eshkp;
    struct bill_x *bp;
    int bct;

    if (!billable(&shkp, obj, *u.ushops, TRUE))
        return;
    eshkp = ESHK(shkp);

    if (eshkp->billct == BILLSZ) {
        You("got that for free!");
        return;
    }

    /* normally bill_p gets set up whenever you enter the shop, but obj
       might be going onto the bill because hero just snagged it with
       a grappling hook from outside without ever having been inside */
    if (!eshkp->bill_p)
        eshkp->bill_p = &(eshkp->bill[0]);

    bct = eshkp->billct;
    bp = &(eshkp->bill_p[bct]);
    bp->bo_id = obj->o_id;
    bp->bquan = obj->quan;
    if (dummy) {              /* a dummy object must be inserted into  */
        bp->useup = 1;        /* the billobjs chain here.  crucial for */
        add_to_billobjs(obj); /* eating floorfood in shop.  see eat.c  */
    } else
        bp->useup = 0;
    bp->price = get_cost(obj, shkp);
    if (obj->globby)
        /* for globs, the amt charged for quan 1 depends on owt */
        bp->price *= get_pricing_units(obj);
    eshkp->billct++;
    obj->unpaid = 1;
}

STATIC_OVL void
add_to_billobjs(obj)
struct obj *obj;
{
    if (obj->where != OBJ_FREE)
        panic("add_to_billobjs: obj not free (%d, %d, %d)",
              obj->where, obj->otyp, obj->invlet);
    if (obj->timed)
        obj_stop_timers(obj);

    obj->nobj = billobjs;
    billobjs = obj;
    obj->where = OBJ_ONBILL;
}

/* recursive billing of objects within containers. */
STATIC_OVL void
bill_box_content(obj, ininv, dummy, shkp)
register struct obj *obj;
register boolean ininv, dummy;
register struct monst *shkp;
{
    register struct obj *otmp;

    if (SchroedingersBox(obj))
        return;
    for (otmp = obj->cobj; otmp; otmp = otmp->nobj) {
        if (otmp->oclass == COIN_CLASS)
            continue;

        /* the "top" box is added in addtobill() */
        if (!otmp->no_charge)
            add_one_tobill(otmp, dummy, shkp);
        if (Has_contents(otmp))
            bill_box_content(otmp, ininv, dummy, shkp);
    }
}

/* shopkeeper tells you what you bought or sold, sometimes partly IDing it */
STATIC_OVL void
shk_names_obj(shkp, obj, fmt, amt, arg)
struct monst *shkp;
struct obj *obj;
const char *fmt; /* "%s %ld %s %s", doname(obj), amt, plur(amt), arg */
long amt;
const char *arg;
{
    char *obj_name, fmtbuf[BUFSZ];
    boolean was_unknown = !obj->dknown;

    obj->dknown = TRUE;
    /* Use real name for ordinary weapons/armor, and spell-less
     * scrolls/books (that is, blank and mail), but only if the
     * object is within the shk's area of interest/expertise.
     */
    if (!objects[obj->otyp].oc_magic && saleable(shkp, obj)
        && (obj->oclass == WEAPON_CLASS || obj->oclass == ARMOR_CLASS
            || obj->oclass == SCROLL_CLASS || obj->oclass == SPBOOK_CLASS
            || obj->otyp == MIRROR)) {
        was_unknown |= !objects[obj->otyp].oc_name_known;
        makeknown(obj->otyp);
    }
    obj_name = doname(obj);
    /* Use an alternate message when extra information is being provided */
    if (was_unknown) {
        Sprintf(fmtbuf, "%%s; you %s", fmt);
        obj_name[0] = highc(obj_name[0]);
        pline(fmtbuf, obj_name, (obj->quan > 1L) ? "them" : "it", amt,
              plur(amt), arg);
    } else {
        You(fmt, obj_name, amt, plur(amt), arg);
    }
}

/* decide whether a shopkeeper thinks an item belongs to her */
boolean
billable(shkpp, obj, roomno, reset_nocharge)
struct monst **shkpp; /* in: non-null if shk has been validated; out: shk */
struct obj *obj;
char roomno;
boolean reset_nocharge;
{
    struct monst *shkp = *shkpp;

    /* if caller hasn't supplied a shopkeeper, look one up now */
    if (!shkp) {
        if (!roomno)
            return FALSE;
        shkp = shop_keeper(roomno);
        if (!shkp || !inhishop(shkp))
            return FALSE;
        *shkpp = shkp;
    }
    /* perhaps we threw it away earlier */
    if (onbill(obj, shkp, FALSE)
        || (obj->oclass == FOOD_CLASS && obj->oeaten))
        return FALSE;
    /* outer container might be marked no_charge but still have contents
       which should be charged for; clear no_charge when picking things up */
    if (obj->no_charge) {
        if (!Has_contents(obj) || (contained_gold(obj) == 0L
                                   && contained_cost(obj, shkp, 0L, FALSE,
                                                     !reset_nocharge) == 0L))
            shkp = 0; /* not billable */
        if (reset_nocharge && !shkp && obj->oclass != COIN_CLASS) {
            obj->no_charge = 0;
            if (Has_contents(obj))
                picked_container(obj); /* clear no_charge */
        }
    }
    return shkp ? TRUE : FALSE;
}

void
addtobill(obj, ininv, dummy, silent)
struct obj *obj;
boolean ininv, dummy, silent;
{
    struct monst *shkp = 0;
    long ltmp, cltmp, gltmp;
    int contentscount;
    boolean container;

    if (!billable(&shkp, obj, *u.ushops, TRUE))
        return;

    if (obj->oclass == COIN_CLASS) {
        costly_gold(obj->ox, obj->oy, obj->quan);
        return;
    } else if (ESHK(shkp)->billct == BILLSZ) {
        if (!silent)
            You("got that for free!");
        return;
    }

    ltmp = cltmp = gltmp = 0L;
    container = Has_contents(obj);

    if (!obj->no_charge) {
        ltmp = get_cost(obj, shkp);
        if (obj->globby)
            ltmp *= get_pricing_units(obj);
    }
    if (obj->no_charge && !container) {
        obj->no_charge = 0;
        return;
    }

    if (container) {
        cltmp = contained_cost(obj, shkp, cltmp, FALSE, FALSE);
        gltmp = contained_gold(obj);

        if (ltmp)
            add_one_tobill(obj, dummy, shkp);
        if (cltmp)
            bill_box_content(obj, ininv, dummy, shkp);
        picked_container(obj); /* reset contained obj->no_charge */

        ltmp += cltmp;

        if (gltmp) {
            costly_gold(obj->ox, obj->oy, gltmp);
            if (!ltmp)
                return;
        }

        if (obj->no_charge)
            obj->no_charge = 0;
        contentscount = count_unpaid(obj->cobj);
    } else { /* !container */
        add_one_tobill(obj, dummy, shkp);
        contentscount = 0;
    }

    if (!Deaf && !muteshk(shkp) && !silent) {
        char buf[BUFSZ];

        if (!ltmp) {
            pline("%s has no interest in %s.", Shknam(shkp), the(xname(obj)));
            return;
        }
        if (!ininv) {
            pline("%s will cost you %ld %s%s.", The(xname(obj)), ltmp,
                  currency(ltmp), (obj->quan > 1L) ? " each" : "");
        } else {
            long save_quan = obj->quan;

            Strcpy(buf, "\"For you, ");
            if (ANGRY(shkp) || hostile_shk(shkp)) {
                Strcat(buf, hatesrace[rn2(SIZE(hatesrace))]);
                Strcat(buf, ";");
            } else {
                append_honorific(buf);
                Strcat(buf, "; only");
            }
            obj->quan = 1L; /* fool xname() into giving singular */
            pline("%s %ld %s %s %s%s.\"", buf, ltmp, currency(ltmp),
                  (save_quan > 1L) ? "per"
                                   : (contentscount && !obj->unpaid)
                                       ? "for the contents of this"
                                       : "for this",
                  xname(obj),
                  (contentscount && obj->unpaid) ? and_its_contents : "");
            obj->quan = save_quan;
        }
    } else if (!silent) {
        if (ltmp)
            pline_The("list price of %s%s%s is %ld %s%s.",
                      (contentscount && !obj->unpaid) ? the_contents_of : "",
                      the(xname(obj)),
                      (contentscount && obj->unpaid) ? and_its_contents : "",
                      ltmp, currency(ltmp), (obj->quan > 1L) ? " each" : "");
        else
            pline("%s does not notice.", Shknam(shkp));
    }
}

STATIC_OVL void
append_honorific(buf)
char *buf;
{
    /* (chooses among [0]..[3] normally; [1]..[4] after the
       Wizard has been killed or invocation ritual performed) */
    static const char *const honored[] = { "good", "honored", "most gracious",
                                           "esteemed",
                                           "most renowned and sacred" };

    Strcat(buf, honored[rn2(SIZE(honored) - 1) + u.uevent.udemigod]);
    if (is_vampire(youmonst.data))
        Strcat(buf, (flags.female) ? " dark lady" : " dark lord");
    else if (maybe_polyd(is_elf(youmonst.data), Race_if(PM_ELF)))
        Strcat(buf, (flags.female) ? " hiril" : " hir");
    else
        Strcat(buf, !maybe_polyd(is_human(youmonst.data),
                                 Race_if(PM_HUMAN)) ? " creature"
                                                    : (flags.female) ? " lady"
                                                                     : " sir");
}

void
splitbill(obj, otmp)
register struct obj *obj, *otmp;
{
    /* otmp has been split off from obj */
    register struct bill_x *bp;
    register long tmp;
    register struct monst *shkp = shop_keeper(*u.ushops);

    if (!shkp || !inhishop(shkp)) {
        impossible("splitbill: no resident shopkeeper??");
        return;
    }
    bp = onbill(obj, shkp, FALSE);
    if (!bp) {
        impossible("splitbill: not on bill?");
        return;
    }
    if (bp->bquan < otmp->quan) {
        impossible("Negative quantity on bill??");
    }
    if (bp->bquan == otmp->quan) {
        impossible("Zero quantity on bill??");
    }
    bp->bquan -= otmp->quan;

    if (ESHK(shkp)->billct == BILLSZ)
        otmp->unpaid = 0;
    else {
        tmp = bp->price;
        bp = &(ESHK(shkp)->bill_p[ESHK(shkp)->billct]);
        bp->bo_id = otmp->o_id;
        bp->bquan = otmp->quan;
        bp->useup = 0;
        bp->price = tmp;
        ESHK(shkp)->billct++;
    }
}

STATIC_OVL void
sub_one_frombill(obj, shkp)
register struct obj *obj;
register struct monst *shkp;
{
    register struct bill_x *bp;

    if ((bp = onbill(obj, shkp, FALSE)) != 0) {
        register struct obj *otmp;

        obj->unpaid = 0;
        if (bp->bquan > obj->quan) {
            otmp = newobj();
            *otmp = *obj;
            otmp->oextra = (struct oextra *) 0;
            bp->bo_id = otmp->o_id = context.ident++;
            otmp->where = OBJ_FREE;
            otmp->quan = (bp->bquan -= obj->quan);
            otmp->owt = 0; /* superfluous */
            bp->useup = 1;
            add_to_billobjs(otmp);
            return;
        }
        ESHK(shkp)->billct--;
#ifdef DUMB
        {
            /* DRS/NS 2.2.6 messes up -- Peter Kendell */
            int indx = ESHK(shkp)->billct;

            *bp = ESHK(shkp)->bill_p[indx];
        }
#else
        *bp = ESHK(shkp)->bill_p[ESHK(shkp)->billct];
#endif
        return;
    } else if (obj->unpaid) {
        impossible("sub_one_frombill: unpaid object not on bill");
        obj->unpaid = 0;
    }
}

/* recursive check of unpaid objects within nested containers. */
void
subfrombill(obj, shkp)
register struct obj *obj;
register struct monst *shkp;
{
    register struct obj *otmp;

    sub_one_frombill(obj, shkp);

    if (Has_contents(obj))
        for (otmp = obj->cobj; otmp; otmp = otmp->nobj) {
            if (otmp->oclass == COIN_CLASS)
                continue;

            if (Has_contents(otmp))
                subfrombill(otmp, shkp);
            else
                sub_one_frombill(otmp, shkp);
        }
}

STATIC_OVL long
stolen_container(obj, shkp, price, ininv)
struct obj *obj;
struct monst *shkp;
long price;
boolean ininv;
{
    struct obj *otmp;
    struct bill_x *bp;
    long billamt;

    /* the price of contained objects; caller handles top container */
    for (otmp = obj->cobj; otmp; otmp = otmp->nobj) {
        if (otmp->oclass == COIN_CLASS)
            continue;
        billamt = 0L;
        if (!billable(&shkp, otmp, ESHK(shkp)->shoproom, TRUE)) {
            /* billable() returns false for objects already on bill */
            if ((bp = onbill(otmp, shkp, FALSE)) == 0)
                continue;
            /* this assumes that we're being called by stolen_value()
               (or by a recursive call to self on behalf of it) where
               the cost of this object is about to be added to shop
               debt in place of having it remain on the current bill */
            billamt = bp->bquan * bp->price;
            sub_one_frombill(otmp, shkp); /* avoid double billing */
        }

        if (billamt)
            price += billamt;
        else if (ininv ? otmp->unpaid : !otmp->no_charge)
            price += get_pricing_units(otmp) * get_cost(otmp, shkp);

        if (Has_contents(otmp))
            price = stolen_container(otmp, shkp, price, ininv);
    }

    return price;
}

long
stolen_value(obj, x, y, peaceful, silent)
struct obj *obj;
xchar x, y;
boolean peaceful, silent;
{
    long value = 0L, gvalue = 0L, billamt = 0L;
    char roomno = *in_rooms(x, y, SHOPBASE);
    struct bill_x *bp;
    struct monst *shkp = 0;
    boolean was_unpaid;
    long c_count = 0L, u_count = 0L;

    /* gather information for message(s) prior to manipulating bill */
    was_unpaid = obj->unpaid ? TRUE : FALSE;
    if (Has_contents(obj)) {
        c_count = count_contents(obj, TRUE, FALSE, TRUE, FALSE);
        u_count = count_contents(obj, TRUE, FALSE, FALSE, FALSE);
    }

    if (!billable(&shkp, obj, roomno, FALSE)) {
        /* things already on the bill yield a not-billable result, so
           we need to check bill before deciding that shk doesn't care */
        if ((bp = onbill(obj, shkp, FALSE)) != 0) {
            /* shk does care; take obj off bill to avoid double billing */
            billamt = bp->bquan * bp->price;
            sub_one_frombill(obj, shkp);
        }
        if (!bp && !u_count)
            return 0L;
    }

    if (obj->oclass == COIN_CLASS) {
        gvalue += obj->quan;
    } else {
        if (billamt)
            value += billamt;
        else if (!obj->no_charge)
            value += get_pricing_units(obj) * get_cost(obj, shkp);

        if (Has_contents(obj)) {
            boolean ininv =
                (obj->where == OBJ_INVENT || obj->where == OBJ_FREE);

            value += stolen_container(obj, shkp, 0L, ininv);
            if (!ininv)
                gvalue += contained_gold(obj);
        }
    }

    if (gvalue + value == 0L)
        return 0L;

    value += gvalue;

    if (peaceful) {
        boolean credit_use = !!ESHK(shkp)->credit;

        value = check_credit(value, shkp);
        /* 'peaceful' affects general treatment, but doesn't affect
         * the fact that other code expects that all charges after the
         * shopkeeper is angry are included in robbed, not debit */
        if (ANGRY(shkp))
            ESHK(shkp)->robbed += value;
        else
            ESHK(shkp)->debit += value;

        if (!silent) {
            char buf[BUFSZ];
            const char *still = "";

            if (credit_use) {
                if (ESHK(shkp)->credit) {
                    You("have %ld %s credit remaining.", ESHK(shkp)->credit,
                        currency(ESHK(shkp)->credit));
                    return value;
                } else if (!value) {
                    You("have no credit remaining.");
                    return 0;
                }
                still = "still ";
            }
            Sprintf(buf, "%sowe %s %ld %s", still, shkname(shkp),
                    value, currency(value));
            if (u_count) /* u_count > 0 implies Has_contents(obj) */
                Sprintf(eos(buf), " for %s%sits contents",
                        was_unpaid ? "it and " : "",
                        (c_count > u_count) ? "some of " : "");
            else if (obj->oclass != COIN_CLASS)
                Sprintf(eos(buf), " for %s",
                        (obj->quan > 1L) ? "them" : "it");

            You("%s!", buf); /* "You owe <shk> N zorkmids for it!" */
        }
    } else {
        ESHK(shkp)->robbed += value;

        if (!silent) {
            if (canseemon(shkp)) {
                Norep("%s booms: \"%s, you are a thief!\"",
                      Shknam(shkp), plname);
            } else if (!Deaf) {
                Norep("You hear a scream, \"Thief!\"");  /* Deaf-aware */
            }
        }
        hot_pursuit(shkp);
        (void) angry_guards(FALSE);
    }
    return value;
}

/* auto-response flag for/from "sell foo?" 'a' => 'y', 'q' => 'n' */
static char sell_response = 'a';
static int sell_how = SELL_NORMAL;
/* can't just use sell_response='y' for auto_credit because the 'a' response
   shouldn't carry over from ordinary selling to credit selling */
static boolean auto_credit = FALSE;

void
sellobj_state(deliberate)
int deliberate;
{
    /* If we're deliberately dropping something, there's no automatic
       response to the shopkeeper's "want to sell" query; however, if we
       accidentally drop anything, the shk will buy it/them without asking.
       This retains the old pre-query risk that slippery fingers while in
       shops entailed:  you drop it, you've lost it.
     */
    sell_response = (deliberate != SELL_NORMAL) ? '\0' : 'a';
    sell_how = deliberate;
    auto_credit = FALSE;
}

void
sellobj(obj, x, y)
register struct obj *obj;
xchar x, y;
{
    register struct monst *shkp;
    register struct eshk *eshkp;
    long ltmp = 0L, cltmp = 0L, gltmp = 0L, offer, shkmoney;
    boolean saleitem, cgold = FALSE, container = Has_contents(obj);
    boolean isgold = (obj->oclass == COIN_CLASS);
    boolean only_partially_your_contents = FALSE;

    if (!*u.ushops) /* do cheapest exclusion test first */
        return;
    if (!(shkp = shop_keeper(*in_rooms(x, y, SHOPBASE))) || !inhishop(shkp))
        return;
    if (!costly_spot(x, y))
        return;

    if (obj->unpaid && !container && !isgold) {
        sub_one_frombill(obj, shkp);
        return;
    }
    if (container) {
        /* find the price of content before subfrombill */
        cltmp = contained_cost(obj, shkp, cltmp, TRUE, FALSE);
        /* find the value of contained gold */
        gltmp += contained_gold(obj);
        cgold = (gltmp > 0L);
    }

    saleitem = saleable(shkp, obj);
    if (!isgold && !obj->unpaid && saleitem)
        ltmp = set_cost(obj, shkp);

    offer = ltmp + cltmp;

    /* get one case out of the way: nothing to sell, and no gold */
    if (!(isgold || cgold)
        && ((offer + gltmp) == 0L || sell_how == SELL_DONTSELL)) {
        boolean unpaid = is_unpaid(obj);

        if (container) {
            dropped_container(obj, shkp, FALSE);
            if (!obj->unpaid)
                obj->no_charge = 1;
            if (unpaid)
                subfrombill(obj, shkp);
        } else
            obj->no_charge = 1;

        if (!unpaid && (sell_how != SELL_DONTSELL)
            && !special_stock(obj, shkp, FALSE))
            pline("%s seems uninterested.", Shknam(shkp));
        return;
    }

    /* you dropped something of your own - probably want to sell it */
    rouse_shk(shkp, TRUE); /* wake up sleeping or paralyzed shk */
    eshkp = ESHK(shkp);

    if (ANGRY(shkp)) { /* they become shop-objects, no pay */
        if (!Deaf && !muteshk(shkp))
            verbalize("Thank you, scum!");
        else
            pline("%s smirks with satisfaction.", Shknam(shkp));
        subfrombill(obj, shkp);
        return;
    }

    if (eshkp->robbed) { /* shkp is not angry? */
        if (isgold)
            offer = obj->quan;
        else if (cgold)
            offer += cgold;
        if ((eshkp->robbed -= offer < 0L))
            eshkp->robbed = 0L;
        if (offer && !Deaf && !muteshk(shkp))
            verbalize(
  "Thank you for your contribution to restock this recently plundered shop.");
        subfrombill(obj, shkp);
        return;
    }

    if (isgold || cgold) {
        if (!cgold)
            gltmp = obj->quan;

        if (eshkp->debit >= gltmp) {
            if (eshkp->loan) { /* you carry shop's gold */
                if (eshkp->loan >= gltmp)
                    eshkp->loan -= gltmp;
                else
                    eshkp->loan = 0L;
            }
            eshkp->debit -= gltmp;
            Your("debt is %spaid off.", eshkp->debit ? "partially " : "");
        } else {
            long delta = gltmp - eshkp->debit;

            eshkp->credit += delta;
            if (eshkp->debit) {
                eshkp->debit = 0L;
                eshkp->loan = 0L;
                Your("debt is paid off.");
            }
            if (eshkp->credit == delta)
                You("have established %ld %s credit.", delta,
                    currency(delta));
            else
                pline("%ld %s added to your credit; total is now %ld %s.",
                      delta, currency(delta), eshkp->credit,
                      currency(eshkp->credit));
        }

        if (!offer || sell_how == SELL_DONTSELL) {
            if (!isgold) {
                if (container)
                    dropped_container(obj, shkp, FALSE);
                if (!obj->unpaid)
                    obj->no_charge = 1;
                subfrombill(obj, shkp);
            }
            return;
        }
    }

    if ((!saleitem && !(container && cltmp > 0L)) || eshkp->billct == BILLSZ
        || obj->oclass == BALL_CLASS || obj->oclass == CHAIN_CLASS
        || offer == 0L || (obj->oclass == FOOD_CLASS && obj->oeaten)
        || (Is_candle(obj)
            && obj->age < 20L * (long) objects[obj->otyp].oc_cost
               && obj->otyp != MAGIC_CANDLE)) {
        pline("%s seems uninterested%s.", Shknam(shkp),
              cgold ? " in the rest" : "");
        if (container)
            dropped_container(obj, shkp, FALSE);
        obj->no_charge = 1;
        return;
    }

    shkmoney = money_cnt(shkp->minvent);
    if (!shkmoney) {
        char c, qbuf[BUFSZ];
        long tmpcr = ((offer * 9L) / 10L) + (offer <= 1L);

        if (sell_how == SELL_NORMAL || auto_credit) {
            c = sell_response = 'y';
        } else if (sell_response != 'n') {
            pline("%s cannot pay you at present.", Shknam(shkp));
            Sprintf(qbuf, "Will you accept %ld %s in credit for ", tmpcr,
                    currency(tmpcr));
            c = ynaq(safe_qbuf(qbuf, qbuf, "?", obj, doname, thesimpleoname,
                               (obj->quan == 1L) ? "that" : "those"));
            if (c == 'a') {
                c = 'y';
                auto_credit = TRUE;
            }
        } else /* previously specified "quit" */
            c = 'n';

        if (c == 'y') {
            shk_names_obj(
                shkp, obj,
                (sell_how != SELL_NORMAL)
                    ? "traded %s for %ld zorkmid%s in %scredit."
                    : "relinquish %s and acquire %ld zorkmid%s in %scredit.",
                tmpcr, (eshkp->credit > 0L) ? "additional " : "");
            eshkp->credit += tmpcr;
            subfrombill(obj, shkp);
        } else {
            if (c == 'q')
                sell_response = 'n';
            if (container)
                dropped_container(obj, shkp, FALSE);
            if (!obj->unpaid)
                obj->no_charge = 1;
            subfrombill(obj, shkp);
        }
    } else {
        char qbuf[BUFSZ], qsfx[BUFSZ];
        boolean short_funds = (offer > shkmoney), one;

        if (short_funds)
            offer = shkmoney;
        if (!sell_response) {
            long yourc = 0L, shksc;

            if (container) {
                /* number of items owned by shk */
                shksc = count_contents(obj, TRUE, TRUE, FALSE, TRUE);
                /* number of items owned by you (total - shksc) */
                yourc = count_contents(obj, TRUE, TRUE, TRUE, TRUE) - shksc;
                only_partially_your_contents = shksc && yourc;
            }
            /*
               "<shk> offers * for ..." query formatting.
               Normal item(s):
                "... your <object>.  Sell it?"
                "... your <objects>.  Sell them?"
               A container is either owned by the hero, or already
               owned by the shk (!ltmp), or the shk isn't interested
               in buying it (also !ltmp).  It's either empty (!cltmp)
               or it has contents owned by the hero or it has some
               contents owned by the hero and others by the shk.
               (The case where it has contents already entirely owned
               by the shk is treated the same was if it were empty
               since the hero isn't selling any of those contents.)
               Your container and shk is willing to buy it:
                "... your <empty bag>.  Sell it?"
                "... your <bag> and its contents.  Sell them?"
                "... your <bag> and item inside.  Sell them?"
                "... your <bag> and items inside.  Sell them?"
               Your container but shk only cares about the contents:
                "... your item in your <bag>.  Sell it?"
                "... your items in your <bag>.  Sell them?"
               Shk's container:
                "... your item in the <bag>.  Sell it?"
                "... your items in the <bag>.  Sell them?"
              FIXME:
               "your items" should sometimes be "some of your items"
               (when container has some stuff the shk is willing to buy
               and other stuff he or she doesn't care about); likewise,
               "your item" should sometimes be "one of your items".
               That would make the prompting even more verbose so
               living without it might be a good thing.
              FIXME too:
               when container's contents are unknown, plural "items"
               should be used to not give away information.
             */
            Sprintf(qbuf, "%s offers%s %ld gold piece%s for %s%s ",
                    Shknam(shkp), short_funds ? " only" : "", offer,
                    plur(offer),
                    (cltmp && !ltmp)
                        ? ((yourc == 1L) ? "your item in " : "your items in ")
                        : "",
                    obj->unpaid ? "the" : "your");
            one = !ltmp ? (yourc == 1L) : (obj->quan == 1L && !cltmp);
            Sprintf(qsfx, "%s.  Sell %s?",
                    (cltmp && ltmp)
                        ? (only_partially_your_contents
                               ? ((yourc == 1L) ? " and item inside"
                                                : " and items inside")
                               : and_its_contents)
                        : "",
                    one ? "it" : "them");
            (void) safe_qbuf(qbuf, qbuf, qsfx, obj, xname, simpleonames,
                             one ? "that" : "those");
        } else
            qbuf[0] = '\0'; /* just to pacify lint */

        switch (sell_response ? sell_response : nyaq(qbuf)) {
        case 'q':
            sell_response = 'n';
            /*FALLTHRU*/
        case 'n':
            if (container)
                dropped_container(obj, shkp, FALSE);
            if (!obj->unpaid)
                obj->no_charge = 1;
            subfrombill(obj, shkp);
            break;
        case 'a':
            sell_response = 'y';
            /*FALLTHRU*/
        case 'y':
            if (container)
                dropped_container(obj, shkp, TRUE);
            if (!obj->unpaid && !saleitem)
                obj->no_charge = 1;
            subfrombill(obj, shkp);
            pay(-offer, shkp);
            shk_names_obj(shkp, obj,
                          (sell_how != SELL_NORMAL)
                           ? ((!ltmp && cltmp && only_partially_your_contents)
                         ? "sold some items inside %s for %ld gold piece%s.%s"
                         : "sold %s for %ld gold piece%s.%s")
            : "relinquish %s and receive %ld gold piece%s in compensation.%s",
                          offer, "");
            break;
        default:
            impossible("invalid sell response");
        }
    }
}
static int
base_id_charge(obj)
struct obj *obj;
{
    switch (obj->oclass) {        
    case AMULET_CLASS:
        return 375;
        break;
    case WEAPON_CLASS:
        return 40;
        break;
    case ARMOR_CLASS:
        return 50;
        break;
    case FOOD_CLASS:
        return 25;   
        break;
    case SCROLL_CLASS:
        return 150;   
        break;
    case SPBOOK_CLASS:
        return 250;   
        break;
    case POTION_CLASS:
        return 150;   
        break;
    case RING_CLASS:
        return 300;   
        break;
    case WAND_CLASS:
        return 200;   
        break;
    case TOOL_CLASS:
        return 50;   
        break;
    case GEM_CLASS:
        return 500;
        break;
    default:
        return 75;
        break;
    }
}
    
static NEARDATA const char identify_types[] = { ALL_CLASSES, 0 };
static NEARDATA const char weapon_types[] = { WEAPON_CLASS, TOOL_CLASS, 0 };
static NEARDATA const char armor_types[] = { ARMOR_CLASS, 0 };

#define IDENTIFY_TYPES 13
static svc_id_type id_types[IDENTIFY_TYPES] = {
    { SHK_ID_BASIC,     ALL_CLASSES },
    { SHK_ID_PREMIUM,   ALL_CLASSES },
    { SHK_ID_WEAPON,    WEAPON_CLASS },
    { SHK_ID_ARMOR,     ARMOR_CLASS },
    { SHK_ID_SCROLL,    SCROLL_CLASS },
    { SHK_ID_BOOK,      SPBOOK_CLASS },
    { SHK_ID_POTION,    POTION_CLASS },
    { SHK_ID_RING,      RING_CLASS },
    { SHK_ID_AMULET,    AMULET_CLASS },
    { SHK_ID_WAND,      WAND_CLASS },
    { SHK_ID_TOOL,      TOOL_CLASS },
    { SHK_ID_GEM,       GEM_CLASS },
    { SHK_ID_FOOD,      FOOD_CLASS },
};

/*
** FUNCTION shk_identify
**
** Pay the shopkeeper to identify an item.
*/

static int
shk_identify(slang, shkp, ident_type)
const char *slang;
struct monst *shkp;
long ident_type;
{
    register struct obj *obj = ((struct obj *) 0); /* The object to identify */
    int i, charge;                     /* Cost to identify             */
    boolean ripoff = FALSE;         /* Shkp ripping you off?        */

    /* Pick object */
    for (i = 0; i < IDENTIFY_TYPES; i++) {
        if (ident_type != id_types[i].svc_type) {
            continue;
        }
        if (!(obj = getobj(&id_types[i].id_type, "have identified")))
            return 0;
        break;
    }

    /* All specialty identify services default to basic. 
     * We should only see premier identify in the black market */
    if (ident_type == SHK_ID_WEAPON || ident_type == SHK_ID_ARMOR)
        ident_type = SHK_ID_PREMIUM;
    else if (ident_type != SHK_ID_PREMIUM)
        ident_type = SHK_ID_BASIC;
    
    /*
    ** Shopkeeper is ripping you off if:
    ** Basic service and object already known.
    ** Premier service, object known, + know blessed/cursed and
    **      rustproof, etc.
    */
    if (obj->dknown && objects[obj->otyp].oc_name_known) {
        if (ident_type == SHK_ID_BASIC) 
            ripoff = TRUE;
        if (ident_type == SHK_ID_PREMIUM 
              && obj->bknown 
              && obj->rknown 
              && obj->known) 
            ripoff = TRUE;
    }

    /* Compute the charge */
    if (ripoff) {
        if (no_cheat) {
            verbalize("That item's already identified!");
            return 0;
        }
        /* Object already identified: Try and cheat the customer. */
        pline("%s chuckles greedily...", mon_nam(shkp));
        charge = base_id_charge(obj);
    } else if (ident_type == SHK_ID_BASIC)
        charge = base_id_charge(obj);       /* basic */    
    else {
        charge = base_id_charge(obj) * 2;   /* premier */
    }
    
    /* Artifacts cost more to deal with */
    /* KMH -- Avoid floating-point */
    if (obj->oartifact) 
        charge = charge * 3 / 2;
    
    /* Smooth out the charge a bit (lower bound only) */
    shk_smooth_charge(&charge, 25, 750);
    
    /* Go ahead? */
    if (shk_offer_price(slang, charge, shkp) == FALSE) 
        return 0;

    /* Shopkeeper deviousness */
    if (ident_type == SHK_ID_BASIC) {
        if (Hallucination) {
            verbalize("It's a pot of flowers.");
            return 1;
        } else if (Confusion) {
            pline("%s tells you but you forget.", mon_nam(shkp));
            return 1;
        }
    }
    
    if (ident_type == SHK_ID_PREMIUM) {
        identify(obj);
    } else { 
        /* Basic */
        makeknown(obj->otyp);
        obj->dknown = 1;
        prinv((char *)0, obj, 0L); /* Print result */
    }
    update_inventory();
    return 1;
}


/*
** Uncurse an item for the customer
*/
static int
shk_uncurse(slang, shkp)
const char *slang;
struct monst *shkp;
{
    struct obj *obj;                /* The object picked            */
    int charge;                     /* How much to uncurse          */

    /* Pick object */
    if (!(obj = getobj(identify_types, "uncurse")))
        return 0;

    if (obj->bknown && !obj->cursed && !Confusion && !Hallucination) {
        pline("That item is not cursed!");
        return 0;
    }
    /* Charge is same as cost */
    charge = get_cost(obj, shop_keeper(*u.ushops));
            
    /* Artifacts cost more to deal with */
    /* KMH -- Avoid floating-point */
    if (obj->oartifact) charge = charge * 3 / 2;

    /* Smooth out the charge a bit */
    shk_smooth_charge(&charge, 50, 250);

    /* Go ahead? */
    if (shk_offer_price(slang, charge, shkp) == FALSE)
        return 0;

    /* Shopkeeper responses */
    /* KMH -- fixed bknown, curse(), bless(), uncurse() */
    if (!obj->bknown
          && !Role_if(PM_PRIEST)
          && !Role_if(PM_NECROMANCER)
          && !no_cheat) {
        /* Not identified! */
        pline("%s snickers and says \"See, nice and uncursed!\"",
                mon_nam(shkp));
        obj->bknown = FALSE;
    } else if (Confusion) {
        /* Curse the item! */
        You("accidentally ask for the item to be cursed.");
        curse(obj);
    } else if (Hallucination) {
        /*
        ** Let's have some fun:  If you're hallucinating,
        ** then there's a chance for the object to be blessed!
        */
        if (!rn2(4)) {
            pline("Distracted by your blood-shot %s, the shopkeeper accidentally blesses the item!",
                makeplural(body_part(EYE)));
            bless(obj);
        } else {
            You_cant("see straight and point to the wrong item.");
        }
    } else {
        verbalize("All done - safe to handle, now!");
        uncurse(obj);
    }
    update_inventory();
    return 1;
}


/*
**
** Appraise a weapon or armor
*/
static const char basic_damage[] =
    "Basic damage against small foes %s, against large foes %s.";

static int
shk_appraisal(slang, shkp)
const char *slang;
struct monst *shkp;
{
    struct obj *obj;                /* The object picked            */
    int charge;                     /* How much for appraisal       */
    boolean guesswork;              /* Shopkeeper unsure?           */
    char ascii_wsdam[5];            /* Ascii form of damage         */
    char ascii_wldam[5];
    
    /* Pick object */
    if ( !(obj = getobj(weapon_types, "appraise"))) 
        return 0;

    charge = get_cost(obj, shop_keeper(*u.ushops)) / 3;

    /* Smooth out the charge a bit */
    shk_smooth_charge(&charge, 5, 50);
    
    if (shk_class_match(WEAPON_CLASS, shkp) == SHK_MATCH){
        verbalize("Ok, %s, let's see what we have here.", slang);
        guesswork = FALSE;
    } else {
        verbalize("Mind you, I'm not an expert in this field.");
        guesswork = TRUE;
    }
    
    if (shk_offer_price(slang, charge, shkp) == FALSE) 
        return 0;

    /* Shopkeeper deviousness */
    if (Confusion) {
        pline_The("numbers get all mixed up in your head.");
        return 0;
    } else if (Hallucination) {
        You_hear("%s say it'll \"knock 'em dead\"", mon_nam(shkp));
        return 0;
    }

    /* Convert damage to ascii */
    Sprintf(ascii_wsdam, "%d", objects[obj->otyp].oc_wsdam);
    Sprintf(ascii_wldam, "%d", objects[obj->otyp].oc_wldam);

    /* Will shopkeeper be unsure? */
    if (guesswork) {
        switch (rn2(10)) {
        case 1:
            /* Shkp's an idiot */
            verbalize("Sorry, %s, but I'm not certain.", slang);
            break;
        case 2:
            /* Not sure about large foes */
            verbalize(basic_damage, ascii_wsdam, "?");
            break;
        case 3:
            /* Not sure about small foes */
            verbalize(basic_damage, "?", ascii_wldam);
            break;
        default:
            verbalize(basic_damage, ascii_wsdam, ascii_wldam);
            break;
        }
    }
    else {
        verbalize(basic_damage, ascii_wsdam, ascii_wldam);
    }
    return 1;
}


/*
** FUNCTION shk_weapon_works
**
** Perform ops on weapon for customer
*/
static int
shk_weapon_works(slang, shkp, svc_type)
const char *slang;
struct monst *shkp;
long svc_type;
{
    struct obj *obj;
    int charge;

    /* Pick weapon */
    if (svc_type == SHK_WEP_FIX || svc_type == SHK_WEP_ENC)
        obj = getobj(weapon_types, "improve");
    else
        obj = getobj(weapon_types, "poison");
    
    if (!obj)
        return 0;

    /* Check if you asked for a non weapon tool to be improved */
    if (obj->oclass == TOOL_CLASS && !is_weptool(obj))
        pline("%s grins greedily...", mon_nam(shkp));
    
    switch (svc_type) {
    case SHK_WEP_FIX:
        verbalize("This'll leave your %s untouchable!", xname(obj));

        /* Costs more the more eroded it is (oeroded 0-3 * 2) */
        charge = 500 * (obj->oeroded + obj->oeroded2 + 1);
        if (obj->oeroded + obj->oeroded2 > 2)
            verbalize("This thing's in pretty sad condition, %s.", slang);

        /* Another warning if object is naturally rustproof */
        if (obj->oerodeproof || !is_damageable(obj))
            pline("%s gives you a suspiciously happy smile...", mon_nam(shkp));

        /* Artifacts cost more to deal with */
        if (obj->oartifact)
            charge = charge * 3 / 2;

        /* Smooth out the charge a bit */
        shk_smooth_charge(&charge, 200, 1500);

        if (shk_offer_price(slang, charge, shkp) == FALSE)
            return 0;

        /* Have some fun, but for this $$$ it better work. */
        if (Confusion)
            You("fall over in appreciation.");
        else if (Hallucination)
            Your("tin roof, un-rusted!");

        obj->oeroded = obj->oeroded2 = 0;
        obj->rknown = TRUE;
        obj->oerodeproof = TRUE;
        update_inventory();
        break;

    case SHK_WEP_ENC:
        verbalize("Guaranteed not to harm your weapon, or your money back!");
        /*
        ** The higher the enchantment, the more costly!
        ** Gets to the point where you need to rob fort Ludios
        ** in order to get it to +5!!
        */
        charge = (obj->spe + 1) * (obj->spe + 1) * 625;

        if (obj->spe < 0)
            charge = 100;

        /* Artifacts cost more to deal with */
        if (obj->oartifact)
            charge *= 2;

        /* Smooth out the charge a bit (lower bound only) */
        shk_smooth_charge(&charge, 50, NOBOUND);

        if (shk_offer_price(slang, charge, shkp) == FALSE)
            return 0;
        if (obj->spe + 1 > 5) {
            verbalize("I can't enchant this any higher!");
            charge = 0;
            break;
        }
        /* Have some fun! */
        if (Confusion)
            Your("%s unexpectedly!", aobjnam(obj, "vibrate"));
        else if (Hallucination)
            Your("%s to evaporate into thin air!", aobjnam(obj, "seem"));
        /* ...No actual vibrating and no evaporating */

        if (obj->otyp == WORM_TOOTH) {
            obj->otyp = CRYSKNIFE;
            Your("weapon seems sharper now.");
            obj->cursed = 0;
            update_inventory();
            break;
        }

        obj->spe++;
        update_inventory();
        break;
    case SHK_WEP_POI:
        verbalize("Just imagine what poisoned %s can do!", xname(obj));
        charge = 10 * obj->quan;
        if (shk_offer_price(slang, charge, shkp) == FALSE)
            return 0;
        obj->opoisoned = TRUE;
            update_inventory();
        break;
    default:
        impossible("Unknown Weapon Enhancement");
        return 0;
    }
    pline("%s", doname(obj));
    return 1;
}


/*
** Perform ops on armor for customer
*/
static int
shk_armor_works(slang, shkp, svc_type)
const char *slang;
struct monst *shkp;
long svc_type;
{
    struct obj *obj;
    int charge;

    /* Pick armor */
    if ( !(obj = getobj(armor_types, "improve")))
        return 0;

    switch (svc_type) {
    case SHK_ARM_FIX:
        if (!flags.female && is_human(youmonst.data))
            verbalize("They'll call you the man of stainless steel!");

        /* Costs more the more rusty it is (oeroded 0-3) */
        charge = 300 * (obj->oeroded + 1);
        if (obj->oeroded > 2)
            verbalize("Yikes!  This thing's a mess!");

        /* Artifacts cost more to deal with */
        /* KMH -- Avoid floating-point */
        if (obj->oartifact)
            charge = charge * 3 / 2;

        /* Smooth out the charge a bit */
        shk_smooth_charge(&charge, 100, 1000);

        if (shk_offer_price(slang, charge, shkp) == FALSE)
            return 0;

        /* Have some fun, but for this $$$ it better work. */
        if (Confusion)
            You("forget how to put your %s back on!", xname(obj));
        else if (Hallucination)
            You("mistake your %s for a pot and...", xname(obj));

        obj->oeroded = 0;
        obj->rknown = TRUE;
        obj->oerodeproof = TRUE;
        update_inventory();
        break;

    case SHK_ARM_ENC:
        if (Is_dragon_scales(obj)) {
            verbalize("Sorry I don't handle dragon scales yet.");
            return 0;
        }
        verbalize("Nobody will ever hit on you again.");

        /* Higher enchantment levels cost more. */
        charge = (obj->spe + 1) * (obj->spe + 1) * 500;

        if (obj->spe < 0)
            charge = 100;

        /* Artifacts cost more to deal with */
        if (obj->oartifact)
            charge *= 2;

        /* Smooth out the charge a bit */
        shk_smooth_charge(&charge, 50, NOBOUND);

        if (shk_offer_price(slang, charge, shkp) == FALSE)
            return 0;
        if (obj->spe + 1 > 3) {
            verbalize("I can't enchant this any higher!");
            charge = 0;
            break;
        }

        if (Hallucination)
            Your("%s looks dented.", xname(obj));
        
        obj->spe++;
        adj_abon(obj, 1);
        update_inventory();
        break;

    default:
        pline("Unknown Armor Enhancement");
        return 0;
    }
    pline("%s", doname(obj));
    return 1;
}


/*
** FUNCTION shk_charge
**
** Charge something (for a price!)
*/
#if 0 /* TODO: Not using these until we grok getobj */
static const char wand_types[] = { WAND_CLASS, 0 };
static const char tool_types[] = { TOOL_CLASS, 0 };
static const char ring_types[] = { RING_CLASS, 0 };
static const char spbook_types[] = { SPBOOK_CLASS, 0 };
#endif
static const char all_count[] = { ALLOW_COUNT, ALL_CLASSES, 0 };

static int
shk_charge(const char *slang, struct monst *shkp, char svc_type)
{
    struct obj *obj = NULL; /* The object picked            */
    struct obj *tobj;       /* Temp obj                     */
    int charge;             /* How much to charge customer  */
    char invlet;            /* Inventory letter             */

    /* What type of shop are we? */
    obj = getobj(all_count, "charge");
    if (!obj) 
        return 0;
    
    /* Verify the item we picked matches the shop type */
    if (shk_class_match(WAND_CLASS, shkp) == SHK_MATCH 
            && obj->oclass != WAND_CLASS ) {
        verbalize("I only can charge wands.");
        return 0;
    }
    else if (shk_class_match(TOOL_CLASS, shkp) == SHK_MATCH 
             && obj->oclass != TOOL_CLASS) {
        verbalize("I only can charge tools.");
        return 0;
    }
    else if (shk_class_match(RING_CLASS, shkp) == SHK_MATCH 
             && obj->oclass != RING_CLASS) {
        verbalize("I only can charge rings.");
        return 0;
    }
#if 0 /* Disabled charging spellbooks */
    else if (shk_class_match(SPBOOK_CLASS, shkp) == SHK_MATCH 
             && obj->oclass != SPBOOK_CLASS) {
        verbalize("I only can charge spellbooks.");
        return 0;
    }
#endif
        
    /*
    ** Wand shops can offer special service!
    ** Extra charges (for a lot of extra money!)
    */

    /* Compute charge */
    if (svc_type == 'b')
        charge = 300;
    else
        charge = 1000;

    /* Wands of wishing should be hard to get recharged */
    if (obj->otyp == WAN_WISHING)
        charge *= 3;
    else /* Smooth out the charge a bit */
        shk_smooth_charge(&charge, 100, 1000);
    
    if (shk_offer_price(slang, charge, shkp) == FALSE)
        return 0;

    /* Shopkeeper deviousness */
    if ((Confusion || Hallucination) && !no_cheat) {
        pline("%s says it's charged and pushes you toward the door.", Monnam(shkp));
        return 1;
    }
    
    invlet = obj->invlet;
    recharge(obj, (svc_type == 'b') ? 0 : 1, &youmonst);

    /*
    ** Did the object blow up?  We need to check this in a way that has 
     * nothing to do with dereferencing the obj pointer. We saved the inventory 
     * letter of this item; now cycle through all objects and see if there is
     * an object with that letter.
    */
    for (obj = 0, tobj = invent; tobj; tobj=tobj->nobj) {
        if (tobj->invlet == invlet) {
            obj = tobj;
            break;
        }
    }
    if (!obj) {
        verbalize("Oops!  Sorry about that...");
        return 1;
    }

    /* Wands get special treatment */
    if (obj->oclass == WAND_CLASS) {
        /* Wand of wishing? */
        if (obj->otyp == WAN_WISHING) {
            /* Premier gives you ONE more charge */
            if (svc_type == 'p') 
                obj->spe++;
            verbalize("Since you'll have everything you always wanted,");
            verbalize("...How about loaning me some money?");
            money2mon(shkp, money_cnt(invent));
            makeknown(obj->otyp);
            bot();
        } else {
            /*
            ** Basic: recharge() will have given 1 charge.
            ** Premier: recharge() will have given 5-10, say.
            ** Add a few more still.
            */
            if (obj->spe < 16) 
                obj->spe += rn1(5, 5);
            else if (obj->spe < 20) 
                obj->spe += 1;
        }
    }
    update_inventory();
    return 1;
}

static int
shk_rumor(slang, shkp)
const char *slang;
struct monst *shkp;
{
    long charge = 25;                     /* Rumor Cost */
                                    
    verbalize("I heard some juicy stuff the other day...");
    /* Go ahead? */
    if (shk_offer_price(slang, charge, shkp) == FALSE) 
        return 0;
    
    if (Hallucination) {
        verbalize("You. Are. Here.");
    } else if (Confusion) {
        pline("%s tells you something... but you forget.", mon_nam(shkp));
    } else
        outrumor(0, BY_OTHER);
    return 1;
}



static int
shk_firearms(slang, shkp)
const char *slang;
struct monst *shkp;
{
    int charge = 9999;
    int weptype = weapon_type(uwep);
    int maxoffered;
    int progress = P_ADVANCE(weptype);
    int threshold = practice_needed_to_advance(P_SKILL(weptype));
    int required = practice_needed_to_advance(P_SKILL(weptype)) - progress;
    
    /* Max skill store can grant */
    if (!strcmp(shtypes[ESHK(shkp)->shoptype-SHOPBASE].name, "gun store")) {
        maxoffered = practice_needed_to_advance(P_SKILLED);
    } else if (shk_class_match(RANDOM_CLASS, shkp) == SHK_GENERAL) {
        maxoffered = practice_needed_to_advance(P_UNSKILLED);
    } else if (shk_class_match(WEAPON_CLASS, shkp) 
             || shk_class_match(ARMOR_CLASS, shkp)) {
        maxoffered = practice_needed_to_advance(P_BASIC);
    } else {
        maxoffered = 0;
    }

    if (can_advance(weptype, FALSE)) {
        verbalize("You should advance your skill before training more.");
        return 0;
    } else if (!uwep || (weapon_type(uwep) == P_NONE)) {
        verbalize("You are not wielding a weapon!");
        return 0;
    } else if (weapon_type(uwep) != P_FIREARM) {
        verbalize("You are not wielding a firearm!");
        return 0;
    } else if  (Confusion || Stunned || Hallucination || Afraid) {
        verbalize("You should not be wielding a firearm right now!");
        return 0;
    } else if (P_SKILL(weptype) >= P_MAX_SKILL(weptype)) {
        verbalize("You cannot increase your skill in %s.", weapon_descr(uwep));
        return 0;
    } else if (!can_practice(weptype)) {
        verbalize("You cannot learn much about %s right now.", weapon_descr(uwep));
        return 0;
    } else if (progress == threshold) {
        verbalize("You are unable to learn more at this time!");
        return 0;
    } else if (progress >= maxoffered) {
        verbalize("I have nothing to teach you!");
        return 0;
    }
    
    /* Determine skill level and charge:
     * Unskilled -> Basic = $500    (General stores will offer this)
     * Basic -> Skilled = $1000     (Weapon/Armor shops can offer this)
     * Skilled -> Expert = $2000    (Only available at gun shops)
     */
    if (progress < practice_needed_to_advance(P_UNSKILLED))
        charge = 500;
    else if (progress < practice_needed_to_advance(P_BASIC))
        charge = 1000; 
    else if (progress < practice_needed_to_advance(P_SKILLED))
        charge = 2000;
    else {
        impossible("Weird firearms skill level!");
    }
    /* Go ahead? */
    if (shk_offer_price(slang, charge, shkp) == FALSE) 
        return 0;

    if (not_fully_identified(uwep)) {
        pline("%s examines %s.", mon_nam(shkp), doname(uwep));
        if (rnd(15) <= ACURR(A_INT)) {
            makeknown(uwep->otyp);
            uwep->known = TRUE;
            verbalize("This is %s", doname(uwep));
            update_inventory();
        } else
            pline("Unfortunately, nothing new turns up.");
    }
    
    pline("%s trains you intensely in the way of %s.", doname(uwep), mon_nam(shkp));
    use_skill(weptype, required);
    return 1;
}

/*
**
** Pay the shopkeeper to tinker an item.
*/
static int
shk_tinker(slang, shkp)
const char *slang;
struct monst *shkp;
{
    register struct obj *obj;
    int charge = 2000; /* Gnomes are greedy pricks */
    int res = 0;
    /* Pick object */
    if (!(obj = getobj(identify_types, "have tinkered")))
        return 0;
    
    if (ACURR(A_INT) < 13) {
       charge += 1000;
    } 
    if (ACURR(A_INT) < 18) {
        charge += 500;
    }
    if (!Race_if(PM_GNOME))
        charge *= 2;
    
    /* Artifacts cannot be tinkered */
    if (obj->oartifact) {
        verbalize("That item is as good as it'll get!");
        return 0;
    }
    
    /* Test if this object yields anything */
    if (!obj2upgrade(obj->otyp)) {
        verbalize("I can't upgrade that object.");
        return 0;
    }
    /* Smooth out the charge a bit (lower bound only) */
    shk_smooth_charge(&charge, 25, 7500);
    
    /* Go ahead? */
    if (shk_offer_price(slang, charge, shkp) == FALSE) 
        return 0;

    /* Shopkeeper deviousness */
    if (Hallucination) {
        verbalize("It's Very Special now.");
        return 1;
    } else if (Confusion) {
        pline("%s dunks the thing in some water and hands it back to you.", mon_nam(shkp));
        (void) water_damage(obj, NULL, TRUE, shkp->mx, shkp->my);
        return 1;
    }
    
    obj = upgrade_obj(obj, &res);
    if (res != 0) {
        if (res == 1) {
            /* The object was upgraded */
            verbalize("Hmm!");
            prinv((char *)0, obj, 0L);
        } else
            verbalize("Huh.");
        update_inventory();
    }
    return 1;
}

/*
** Tell customer how much it'll cost, ask if he wants to pay,
** and deduct from $$ if agreable.
*/
static boolean
shk_offer_price(slang, charge, shkp)
const char *slang;
long charge;
struct monst *shkp;
{
    char sbuf[BUFSZ];
    long credit = ESHK(shkp)->credit;

    /* Ask y/n if player wants to pay */
    Sprintf(sbuf, "It'll cost you %ld zorkmid%s.  Interested?",
            charge, plur(charge));

    if (yn(sbuf) != 'y') {
        verbalize("It's your call, %s.", slang);
        return FALSE;
    }

    /* Player _wants_ to pay, but can he? */
    /* WAC -- Check the credit:  but don't use check_credit
     * since we don't want to charge him for part of it if he can't pay for all 
     * of it 
     */
    if (charge > (money_cnt(invent) + credit)) {  
        verbalize("Cash on the spot, %s, and you ain't got the dough!", slang);
        return FALSE;
    }
    
    if (!u.uconduct.shk++)
        livelog_printf(LL_CONDUCT, "participated in the Yendorian economy for the first time");
    
    /* Charge the customer */
    charge = check_credit(charge, shkp); /* Deduct the credit first */

    if (charge > 0)
        money2mon(shkp, charge);
    bot();

    return TRUE;
}


/*
** Smooth out the lower/upper bounds on the price to get something
** done.  Make sure that it (1) varies depending on charisma and
** (2) is constant.
*/
static void
shk_smooth_charge(pcharge, lower, upper)
int *pcharge;
int lower;
int upper;
{
    int charisma;
    int bonus;

    /* KMH -- Avoid using floating-point arithmetic */
    
    if (ACURR(A_CHA) > 21)
        *pcharge *= 7;
    else if (ACURR(A_CHA) > 20)
        *pcharge *= 8;
    else if (ACURR(A_CHA) > 18)
        *pcharge *= 9;
    else if (ACURR(A_CHA) > 17)
        *pcharge *= 10;
    else if (ACURR(A_CHA) > 16)
        *pcharge *= 11;
    else if (ACURR(A_CHA) > 14)
        *pcharge *= 12;
    else if (ACURR(A_CHA) > 12)
        *pcharge *= 14;
    else if (ACURR(A_CHA) > 10)
        *pcharge *= 15;
    else if (ACURR(A_CHA) > 8)
        *pcharge *= 16;
    else if (ACURR(A_CHA) > 7)
        *pcharge *= 17;
    else if (ACURR(A_CHA) > 6)
        *pcharge *= 18;
    else if (ACURR(A_CHA) > 5)
        *pcharge *= 19;
    else if (ACURR(A_CHA) > 4)
        *pcharge *= 20;
    else
        *pcharge *= 21;
    *pcharge /= 10;

    if (Is_blackmarket(&u.uz))
        *pcharge *= 3;
    /* Skip upper stuff? */
    if (upper == NOBOUND)
        goto check_lower;

    /* This should give us something like a charisma of 5 to 25. */
    /*charisma = ABASE(A_CHA) + ABON(A_CHA) + ATEMP(A_CHA);*/
    charisma = ACURR(A_CHA);
    /* Now: 0 to 10 = 0.  11 and up = 1 to whatever. */
    if (charisma <= 10)
        charisma = 0;
    else
        charisma -= 10;

    /* Charismatic players get smaller upper bounds */
    bonus = ((upper / 50) * charisma);

    /* Adjust upper.  Upper > lower! */
    upper -= bonus;
    upper = (upper >= lower) ? upper : lower;

    /* Ok, do the min/max stuff */
    if (*pcharge > upper)
        *pcharge = upper;
check_lower:
    if (*pcharge < lower)
        *pcharge = lower;
}

int
doinvbill(mode)
int mode; /* 0: deliver count 1: paged */
{
#ifdef __SASC
    void sasc_bug(struct obj *, unsigned);
#endif
    struct monst *shkp;
    struct eshk *eshkp;
    struct bill_x *bp, *end_bp;
    struct obj *obj;
    long totused;
    char *buf_p;
    winid datawin;

    shkp = shop_keeper(*u.ushops);
    if (!shkp || !inhishop(shkp)) {
        if (mode != 0)
            impossible("doinvbill: no shopkeeper?");
        return 0;
    }
    eshkp = ESHK(shkp);

    if (mode == 0) {
        /* count expended items, so that the `I' command can decide
           whether to include 'x' in its prompt string */
        int cnt = !eshkp->debit ? 0 : 1;

        for (bp = eshkp->bill_p, end_bp = &eshkp->bill_p[eshkp->billct];
             bp < end_bp; bp++)
            if (bp->useup
                || ((obj = bp_to_obj(bp)) != 0 && obj->quan < bp->bquan))
                cnt++;
        return cnt;
    }

    datawin = create_nhwindow(NHW_MENU);
    putstr(datawin, 0, "Unpaid articles already used up:");
    putstr(datawin, 0, "");

    totused = 0L;
    for (bp = eshkp->bill_p, end_bp = &eshkp->bill_p[eshkp->billct];
         bp < end_bp; bp++) {
        obj = bp_to_obj(bp);
        if (!obj) {
            impossible("Bad shopkeeper administration.");
            goto quit;
        }
        if (bp->useup || bp->bquan > obj->quan) {
            long oquan, uquan, thisused;

            oquan = obj->quan;
            uquan = (bp->useup ? bp->bquan : bp->bquan - oquan);
            thisused = bp->price * uquan;
            totused += thisused;
            iflags.suppress_price++; /* suppress "(unpaid)" suffix */
            /* Why 'x'?  To match `I x', more or less. */
            buf_p = xprname(obj, (char *) 0, 'x', FALSE, thisused, uquan);
            iflags.suppress_price--;
            putstr(datawin, 0, buf_p);
        }
    }
    if (eshkp->debit) {
        /* additional shop debt which has no itemization available */
        if (totused)
            putstr(datawin, 0, "");
        totused += eshkp->debit;
        buf_p = xprname((struct obj *) 0, "usage charges and/or other fees",
                        GOLD_SYM, FALSE, eshkp->debit, 0L);
        putstr(datawin, 0, buf_p);
    }
    buf_p = xprname((struct obj *) 0, "Total:", '*', FALSE, totused, 0L);
    putstr(datawin, 0, "");
    putstr(datawin, 0, buf_p);
    display_nhwindow(datawin, FALSE);
 quit:
    destroy_nhwindow(datawin);
    return 0;
}

STATIC_OVL long
getprice(obj, shk_buying)
register struct obj *obj;
boolean shk_buying;
{
    register long tmp = (long) objects[obj->otyp].oc_cost;

    if (obj->oartifact) {
        tmp = arti_cost(obj);
        if (shk_buying)
            tmp /= 4;
    }
    
    switch (obj->oclass) {
    case FOOD_CLASS:
        /* simpler hunger check, (2-4)*cost */
        if (u.uhs >= HUNGRY && !shk_buying)
            tmp *= (long) u.uhs;
        if (obj->oeaten)
            tmp = 0L;
        break;
    case WAND_CLASS:
        if (obj->spe == -1)
            tmp = 0L;
        break;
    case POTION_CLASS:
        if (obj->otyp == POT_WATER && !obj->blessed && !obj->cursed)
            tmp = 0L;
        break;
    case ARMOR_CLASS:
        if (Is_dragon_scaled_armor(obj))
            tmp += ((3 * objects[obj->dragonscales].oc_cost) / 2L);
        /* FALLTHRU */
    case WEAPON_CLASS:
        if (obj->spe > 0)
            tmp += 10L * (long) obj->spe;
        if (is_bomb(obj) && obj->oarmed) 
            tmp = 0L;
        break;
    case TOOL_CLASS:
        if (Is_candle(obj)
            && obj->age < 20L * (long) objects[obj->otyp].oc_cost) {
            tmp /= 2L;
        } else if (obj->otyp == TORCH) {
            if (obj->age == 0) {
                tmp = 0L;
            } else if (obj->age < 25) {
                tmp /= 4L;
            } else if (obj->age < 50) {
                tmp /= 2L;
            }
        }
        break;
    }
    return tmp;
}

/* shk catches thrown pick-axe */
struct monst *
shkcatch(obj, x, y)
register struct obj *obj;
register xchar x, y;
{
    register struct monst *shkp;

    if (!(shkp = shop_keeper(inside_shop(x, y))) || !inhishop(shkp))
        return 0;

    if (shkp->mcanmove && !shkp->msleeping
        && (*u.ushops != ESHK(shkp)->shoproom || !inside_shop(u.ux, u.uy))
        && dist2(shkp->mx, shkp->my, x, y) < 3
        /* if it is the shk's pos, you hit and anger him */
        && (shkp->mx != x || shkp->my != y)) {
        if (mnearto(shkp, x, y, TRUE) == 2 && !Deaf && !muteshk(shkp))
            verbalize("Out of my way, scum!");
        if (cansee(x, y)) {
            pline("%s nimbly%s catches %s.", Shknam(shkp),
                  (x == shkp->mx && y == shkp->my) ? "" : " reaches over and",
                  the(xname(obj)));
            if (!canspotmon(shkp))
                map_invisible(x, y);
            delay_output();
            mark_synch();
        }
        subfrombill(obj, shkp);
        (void) mpickobj(shkp, obj);
        return shkp;
    }
    return (struct monst *) 0;
}

void
add_damage(x, y, cost)
register xchar x, y;
long cost;
{
    struct damage *tmp_dam;
    char *shops;

    if (IS_DOOR(levl[x][y].typ)) {
        struct monst *mtmp;

        /* Don't schedule for repair unless it's a real shop entrance */
        for (shops = in_rooms(x, y, SHOPBASE); *shops; shops++)
            if ((mtmp = shop_keeper(*shops)) != 0
                && x == ESHK(mtmp)->shd.x && y == ESHK(mtmp)->shd.y)
                break;
        if (!*shops)
            return;
    }
    for (tmp_dam = level.damagelist; tmp_dam; tmp_dam = tmp_dam->next)
        if (tmp_dam->place.x == x && tmp_dam->place.y == y) {
            tmp_dam->cost += cost;
            tmp_dam->when = monstermoves; /* needed by pay_for_damage() */
            return;
        }
    tmp_dam = (struct damage *) alloc((unsigned) sizeof *tmp_dam);
    (void) memset((genericptr_t) tmp_dam, 0, sizeof *tmp_dam);
    tmp_dam->when = monstermoves;
    tmp_dam->place.x = x;
    tmp_dam->place.y = y;
    tmp_dam->cost = cost;
    tmp_dam->typ = levl[x][y].typ;
    tmp_dam->next = level.damagelist;
    level.damagelist = tmp_dam;
    /* If player saw damage, display as a wall forever */
    if (cansee(x, y))
        levl[x][y].seenv = SVALL;
}

/*
 * Do something about damage. Either (!croaked) try to repair it, or
 * (croaked) just discard damage structs for non-shared locations, since
 * they'll never get repaired. Assume that shared locations will get
 * repaired eventually by the other shopkeeper(s). This might be an erroneous
 * assumption (they might all be dead too), but we have no reasonable way of
 * telling that.
 */
STATIC_OVL
void
remove_damage(shkp, croaked)
struct monst *shkp;
boolean croaked;
{
    struct damage *tmp_dam, *tmp2_dam;
    struct obj *shk_inv = shkp->minvent;
    boolean did_repair = FALSE, saw_door = FALSE, saw_floor = FALSE,
            stop_picking = FALSE, doorway_trap = FALSE, skip_msg = FALSE;
    int saw_walls = 0, saw_untrap = 0, feedback;
    char trapmsg[BUFSZ];

    feedback = !croaked; /* 1 => give feedback, 0 => don't or already did */
    tmp_dam = level.damagelist;
    tmp2_dam = 0;
    while (tmp_dam) {
        register xchar x = tmp_dam->place.x, y = tmp_dam->place.y;
        char shops[5];
        int disposition;
        unsigned old_doormask = 0;

        disposition = 0;
        Strcpy(shops, in_rooms(x, y, SHOPBASE));
        if (index(shops, ESHK(shkp)->shoproom)) {
            if (IS_DOOR(levl[x][y].typ))
                old_doormask = levl[x][y].doormask;

            if (croaked) {
                disposition = (shops[1]) ? 0 : 1;
            } else if (stop_picking) {
                disposition = repair_damage(shkp, tmp_dam, &feedback, FALSE);
            } else {
                /* Defer the stop_occupation() until after repair msgs */
                if (closed_door(x, y))
                    stop_picking = picking_at(x, y);
                disposition = repair_damage(shkp, tmp_dam, &feedback, FALSE);
                if (!disposition)
                    stop_picking = FALSE;
            }
        }

        if (!disposition) {
            tmp2_dam = tmp_dam;
            tmp_dam = tmp_dam->next;
            continue;
        }

        if (disposition > 1) {
            did_repair = TRUE;
            if (cansee(x, y)) {
                if (IS_WALL(levl[x][y].typ)) {
                    saw_walls++;
                } else if (IS_DOOR(levl[x][y].typ)
                           /* an existing door here implies trap removal */
                           && !(old_doormask & (D_ISOPEN | D_CLOSED))) {
                    saw_door = TRUE;
                } else if (disposition == 3) { /* untrapped */
                    saw_untrap++;
                    if (IS_DOOR(levl[x][y].typ))
                        doorway_trap = TRUE;
                } else {
                    saw_floor = TRUE;
                }
            }
        }

        tmp_dam = tmp_dam->next;
        if (!tmp2_dam) {
            free((genericptr_t) level.damagelist);
            level.damagelist = tmp_dam;
        } else {
            free((genericptr_t) tmp2_dam->next);
            tmp2_dam->next = tmp_dam;
        }
    }
    if (!did_repair)
        return;

    trapmsg[0] = '\0'; /* not just lint suppression... */
    shk_inv = (shkp->minvent != shk_inv) ? shkp->minvent : 0;
    if (saw_untrap == 1 && shk_inv
        && (shk_inv->otyp == BEARTRAP || shk_inv->otyp == LAND_MINE)
        && canseemon(shkp)) {
        pline("%s untraps %s.", Shknam(shkp), ansimpleoname(shk_inv));
        /* we've already reported this trap (and know it's the only one) */
        saw_untrap = 0;
        skip_msg = !(saw_walls || saw_door || saw_floor);
    } else if (saw_untrap) {
        Sprintf(trapmsg, "%s trap%s",
                (saw_untrap > 3) ? "several" : (saw_untrap > 1) ? "some"
                                                                : "a",
                plur(saw_untrap));
        Sprintf(eos(trapmsg), " %s", vtense(trapmsg, "are"));
        Sprintf(eos(trapmsg), " removed from the %s",
                (doorway_trap && saw_untrap == 1) ? "doorway" : "floor");
    }

    if (skip_msg) {
        ; /* already gave an untrap message which covered the only repair */
    } else if (saw_walls) {
        char wallbuf[BUFSZ];

        Sprintf(wallbuf, "section%s", plur(saw_walls));
        pline("Suddenly, %s %s of wall %s up!",
              (saw_walls == 1) ? "a" : (saw_walls <= 3) ? "some" : "several",
              wallbuf, vtense(wallbuf, "close"));

        if (saw_door)
            pline_The("shop door reappears!");
        if (saw_floor)
            pline_The("floor is repaired!");
        if (saw_untrap)
            pline("%s!", upstart(trapmsg));
    } else {
        if (saw_door || saw_floor || saw_untrap)
            pline("Suddenly, %s%s%s%s%s!",
                  saw_door ? "the shop door reappears" : "",
                  (saw_door && saw_floor) ? " and " : "",
                  saw_floor ? "the floor damage is gone" : "",
                  ((saw_door || saw_floor) && *trapmsg) ? " and " : "",
                  trapmsg);
        /* FIXME:
         *  these messages aren't right if the unseen repairs were only
         *  for trap removal (except for hole and possibly trap door).
         */
        else if (inside_shop(u.ux, u.uy) == ESHK(shkp)->shoproom)
            You_feel("more claustrophobic than before.");
        else if (!Deaf && !rn2(10))
            Norep("The dungeon acoustics noticeably change.");
    }
    if (stop_picking)
        stop_occupation();
}

/*
 * 0: repair postponed, 1: silent repair (no messages), 2: normal repair
 * 3: untrap
 */
int
repair_damage(shkp, tmp_dam, once, catchup)
struct monst *shkp;
struct damage *tmp_dam;
int *once;
boolean catchup; /* restoring a level */
{
    xchar x, y;
    xchar litter[9];
    struct monst *mtmp;
    struct obj *otmp;
    struct trap *ttmp;
    int i, k, ix, iy, disposition = 1;

    if ((monstermoves - tmp_dam->when) < REPAIR_DELAY)
        return 0;
    if (shkp->msleeping || !shkp->mcanmove || ESHK(shkp)->following)
        return 0;
    x = tmp_dam->place.x;
    y = tmp_dam->place.y;
    if (!IS_ROOM(tmp_dam->typ)) {
        if ((x == u.ux && y == u.uy && !Passes_walls)
            || (x == shkp->mx && y == shkp->my)
            || ((mtmp = m_at(x, y)) != 0 && !passes_walls(mtmp->data)))
            return 0;
    }
    ttmp = t_at(x, y);
    if (ttmp && x == u.ux && y == u.uy && !Passes_walls)
        return 0;

    if (once && *once) {
        boolean shk_closeby = (distu(shkp->mx, shkp->my)
                               <= (BOLT_LIM / 2) * (BOLT_LIM / 2));

        /* this is suboptimal if we eventually give a "shk untraps"
           message for the only repair, but perhaps the shop repair
           incantation means that shk's untrap attempt will never fail */
        if (canseemon(shkp))
            pline("%s whispers %s.", Shknam(shkp),
                  shk_closeby ? "an incantation" : "something");
        else if (!Deaf && shk_closeby)
            You_hear("someone muttering an incantation.");
        *once = 0;
    }
    if (ttmp) {
        if ((ttmp->ttyp == LANDMINE || ttmp->ttyp == BEAR_TRAP)
            && dist2(x, y, shkp->mx, shkp->my) <= 2) {
            /* convert to an object */
            otmp = mksobj((ttmp->ttyp == LANDMINE) ? LAND_MINE : BEARTRAP,
                          TRUE, FALSE);
            otmp->quan = 1L;
            otmp->owt = weight(otmp);
            (void) mpickobj(shkp, otmp);
        }
        deltrap(ttmp);
        if (cansee(x, y))
            newsym(x, y);
        if (!catchup)
            disposition = 3;
    }
    if (IS_ROOM(tmp_dam->typ)
        || (tmp_dam->typ == levl[x][y].typ
            && (!IS_DOOR(tmp_dam->typ) || levl[x][y].doormask > D_BROKEN)))
        /* no terrain fix necessary (trap removal or manually repaired) */
        return disposition;

    /* door or wall repair; trap, if any, is now gone;
       restore original terrain type and move any items away */
    levl[x][y].typ = tmp_dam->typ;
    if (IS_DOOR(tmp_dam->typ))
        levl[x][y].doormask = D_CLOSED; /* arbitrary */

    (void) memset((genericptr_t) litter, 0, sizeof litter);
#define NEED_UPDATE 1
#define OPEN 2
#define INSHOP 4
#define horiz(i) ((i % 3) - 1)
#define vert(i) ((i / 3) - 1)
    k = 0; /* number of adjacent shop spots */
    if (level.objects[x][y] && !IS_ROOM(levl[x][y].typ)) {
        for (i = 0; i < 9; i++) {
            ix = x + horiz(i);
            iy = y + vert(i);
            if (i == 4 || !isok(ix, iy) || !ZAP_POS(levl[ix][iy].typ))
                continue;
            litter[i] = OPEN;
            if (inside_shop(ix, iy) == ESHK(shkp)->shoproom) {
                litter[i] |= INSHOP;
                ++k;
            }
        }
    }
    /* placement below assumes there is always at least one adjacent
       spot; the 'k' check guards against getting stuck in an infinite
       loop if some irregularly shaped room breaks that assumption */
    if (k > 0) {
        /* Scatter objects haphazardly into the shop */
        if (Punished && !u.uswallow
            && ((uchain->ox == x && uchain->oy == y)
                || (uball->ox == x && uball->oy == y))) {
            /*
             * Either the ball or chain is in the repair location.
             * Take the easy way out and put ball&chain under hero.
             *
             * FIXME: message should be reworded; this might be the
             * shop's doorway rather than a wall, there might be some
             * other stuff here which isn't junk, and "your junk" has
             * a slang connotation which could be applicable if hero
             * has Passes_walls ability.
             */
            if (!Deaf && !muteshk(shkp))
                verbalize("Get your junk out of my wall!");
            unplacebc(); /* pick 'em up */
            placebc();   /* put 'em down */
        }
        while ((otmp = level.objects[x][y]) != 0)
            /* Don't mess w/ boulders -- just merge into wall */
            if (otmp->otyp == BOULDER || otmp->otyp == ROCK) {
                obj_extract_self(otmp);
                obfree(otmp, (struct obj *) 0);
            } else {
                int trylimit = 50;

                /* otmp must be moved otherwise level.objects[x][y] will
                   never become Null and while-loop won't terminate */
                do {
                    i = rn2(9);
                } while (--trylimit && !(litter[i] & INSHOP));
                if ((litter[i] & (OPEN | INSHOP)) != 0) {
                    ix = x + horiz(i);
                    iy = y + vert(i);
                } else {
                    /* we know shk isn't at <x,y> because repair
                       is deferred in that situation */
                    ix = shkp->mx;
                    iy = shkp->my;
                }
                remove_object(otmp);
                place_object(otmp, ix, iy);
                litter[i] |= NEED_UPDATE;
            }
    }
    if (catchup)
        return 1; /* repair occurred while off level so no messages */

    block_point(x, y);
    if (cansee(x, y)) {
        if (IS_WALL(tmp_dam->typ))
            /* player sees actual repair process, so KNOWS it's a wall */
            levl[x][y].seenv = SVALL;
        newsym(x, y);
    }
    for (i = 0; i < 9; i++)
        if (litter[i] & NEED_UPDATE)
            newsym(x + horiz(i), y + vert(i));

    if (disposition < 3)
        disposition = 2;
    return disposition;
#undef NEED_UPDATE
#undef OPEN
#undef INSHOP
#undef vert
#undef horiz
}

/*
 * shk_move: return 1: moved  0: didn't  -1: let m_move do it  -2: died
 */
int
shk_move(shkp)
struct monst *shkp;
{
    xchar gx, gy, omx, omy;
    int udist;
    schar appr;
    struct eshk *eshkp = ESHK(shkp);
    int z;
    boolean uondoor = FALSE, satdoor, avoid = FALSE, badinv;

    omx = shkp->mx;
    omy = shkp->my;

    /* Random advertising to passersby. */
    if (!ANGRY(shkp) && inhishop(shkp) && !*u.ushops 
          && !rn2(7) && m_canseeu(shkp)) {
        shk_holler(shkp);
    }

    if (inhishop(shkp))
        remove_damage(shkp, FALSE);

    if ((udist = distu(omx, omy)) < 3 && (shkp->data != &mons[PM_GRID_BUG]
                                          || (omx == u.ux || omy == u.uy))) {
        if (ANGRY(shkp) || (Conflict && !resist_conflict(shkp))) {
            if (Displaced)
                Your("displaced image doesn't fool %s!", shkname(shkp));
            (void) mattacku(shkp);
            return 0;
        }
        if (eshkp->following) {
            if (strncmp(eshkp->customer, plname, PL_NSIZ)) {
                if (!Deaf && !muteshk(shkp))
                    verbalize("%s, %s!  I was looking for %s.", Hello(shkp),
                              plname, eshkp->customer);
                eshkp->following = 0;
                return 0;
            }
            if (moves > followmsg + 4) {
                if (!Deaf && !muteshk(shkp))
                    verbalize("%s, %s!  Didn't you forget to pay?",
                              Hello(shkp), plname);
                else
                    pline("%s holds out %s upturned %s.",
                          Shknam(shkp), noit_mhis(shkp),
                          mbodypart(shkp, HAND));
                followmsg = moves;
                if (!rn2(9)) {
                    pline("%s doesn't like customers who don't pay.",
                          Shknam(shkp));
                    rile_shk(shkp);
                }
            }
            if (udist < 2)
                return 0;
        }
    }

    appr = 1;
    gx = eshkp->shk.x;
    gy = eshkp->shk.y;
    satdoor = (gx == omx && gy == omy);
    if (eshkp->following || ((z = holetime()) >= 0 && z * z <= udist)) {
        /* [This distance check used to apply regardless of
            whether the shk was following, but that resulted in
            m_move() sometimes taking the shk out of the shop if
            the player had fenced him in with boulders or traps.
            Such voluntary abandonment left unpaid objects in
            invent, triggering billing impossibilities on the
            next level once the character fell through the hole.] */
        if (udist > 4 && eshkp->following && !eshkp->billct)
            return -1; /* leave it to m_move */
        gx = u.ux;
        gy = u.uy;
    } else if (ANGRY(shkp)) {
        /* Move towards the hero if the shopkeeper can see him. */
        if (shkp->mcansee && m_canseeu(shkp)) {
            gx = u.ux;
            gy = u.uy;
        }
        avoid = FALSE;
    } else {
#define GDIST(x, y) (dist2(x, y, gx, gy))
        if ((!Is_blackmarket(&u.uz) 
                && (Invis || u.usteed) 
                && !inside_shop(u.ux, u.uy))) {
            avoid = FALSE;
        } else {
            uondoor = (u.ux == eshkp->shd.x && u.uy == eshkp->shd.y);
            if (uondoor) {
                badinv = (!Is_blackmarket(&u.uz) &&
                    (carrying(PICK_AXE) || carrying(DWARVISH_MATTOCK)
                     || eshkp->pbanned
                     || (Fast && (sobj_at(PICK_AXE, u.ux, u.uy)
                                  || sobj_at(DWARVISH_MATTOCK, u.ux, u.uy)))));
                if (satdoor && badinv)
                    return 0;
                avoid = !badinv;
            } else {
                avoid = (*u.ushops && distu(gx, gy) > 8);
                badinv = FALSE;
            }

            if (((!eshkp->robbed && !eshkp->billct && !eshkp->debit) || avoid)
                && GDIST(omx, omy) < 3) {
                if (!badinv && !onlineu(omx, omy))
                    return 0;
                if (satdoor)
                    appr = gx = gy = 0;
            }
        }
    }

    z = move_special(shkp, inhishop(shkp), appr, uondoor, avoid, omx, omy, gx,
                     gy);
    if (z > 0)
        after_shk_move(shkp);

    return z;
}

/* called after shopkeeper moves, in case themove causes re-entry into shop */
void
after_shk_move(shkp)
struct monst *shkp;
{
    struct eshk *eshkp = ESHK(shkp);

    if (eshkp->bill_p == (struct bill_x *) -1000 && inhishop(shkp)) {
        /* reset bill_p, need to re-calc player's occupancy too */
        eshkp->bill_p = &eshkp->bill[0];
        check_special_room(FALSE);
    }
}

/* for use in levl_follower (mondata.c) */
boolean
is_fshk(mtmp)
register struct monst *mtmp;
{
    return (boolean) (mtmp->isshk && ESHK(mtmp)->following);
}

/* You are digging in the shop. */
void
shopdig(fall)
register int fall;
{
    register struct monst *shkp = shop_keeper(*u.ushops);
    int lang;
    const char *grabs = "grabs";

    if (!shkp)
        return;

    /* 0 == can't speak, 1 == makes animal noises, 2 == speaks */
    lang = 0;
    if (shkp->msleeping || !shkp->mcanmove || is_silent(shkp->data))
        ; /* lang stays 0 */
    else if (shkp->data->msound <= MS_ANIMAL)
        lang = 1;
    else if (shkp->data->msound >= MS_HUMANOID)
        lang = 2;

    if (!inhishop(shkp)) {
        if (Role_if(PM_KNIGHT)) {
            You_feel("like a common thief.");
            adjalign(-sgn(u.ualign.type));
        }
        return;
    }

    if (!fall) {
        if (lang == 2) {
            if (!Deaf && !muteshk(shkp)) {
                if (u.utraptype == TT_PIT)
                    verbalize(
                        "Be careful, %s, or you might fall through the floor.",
                        hostile_shk(shkp) ? hatesrace[rn2(SIZE(hatesrace))]
                                          : flags.female ? "madam" : "sir");
                else
                    verbalize("%s%s, do not damage the floor here!",
                        hostile_shk(shkp) ? "Hey " : "",
                        hostile_shk(shkp) ? hatesrace[rn2(SIZE(hatesrace))]
                                          : flags.female ? "Madam" : "Sir");
            }
        }
        if (Role_if(PM_KNIGHT)) {
            You_feel("like a common thief.");
            adjalign(-sgn(u.ualign.type));
        }
    } else if (!um_dist(shkp->mx, shkp->my, 5)
               && !shkp->msleeping && shkp->mcanmove
               && (ESHK(shkp)->billct || ESHK(shkp)->debit)) {
        register struct obj *obj, *obj2;

        if (nolimbs(shkp->data)) {
            grabs = "knocks off";
#if 0
            /* This is what should happen, but for balance
             * reasons, it isn't currently.
             */
            if (lang == 2)
                pline("%s curses %s inability to grab your backpack!",
                      Shknam(shkp), noit_mhim(shkp));
            rile_shk(shkp);
            return;
#endif
        }
        if (distu(shkp->mx, shkp->my) > 2) {
            mnexto(shkp);
            /* for some reason the shopkeeper can't come next to you */
            if (distu(shkp->mx, shkp->my) > 2) {
                if (lang == 2)
                    pline("%s curses you in anger and frustration!",
                          Shknam(shkp));
                else if (lang == 1)
                    growl(shkp);
                rile_shk(shkp);
                return;
            } else
                pline("%s %s, and %s your backpack!", Shknam(shkp),
                      makeplural(locomotion(shkp->data, "leap")), grabs);
        } else
            pline("%s %s your backpack!", Shknam(shkp), grabs);

        for (obj = invent; obj; obj = obj2) {
            obj2 = obj->nobj;
            if ((obj->owornmask & ~(W_SWAPWEP | W_QUIVER)) != 0
                || (obj == uswapwep && u.twoweap)
                || (obj->otyp == LEASH && obj->leashmon))
                continue;
            if (obj == current_wand)
                continue;
            setnotworn(obj);
            freeinv(obj);
            subfrombill(obj, shkp);
            (void) add_to_minv(shkp, obj); /* may free obj */
        }
    }
}

STATIC_OVL void
makekops(mm)
coord *mm;
{
    const int TYPES = 5;
    short k_mndx[TYPES];
    int k_cnt[TYPES], cnt, mndx, k;
    if (Is_blackmarket(&u.uz)) {
        cnt = 7 + rnd(5);
        k_mndx[0] = PM_SOLDIER;
        k_mndx[1] = PM_SERGEANT;
        k_mndx[2] = PM_LIEUTENANT;
        k_mndx[3] = PM_CAPTAIN;
        k_mndx[4] = PM_GENERAL;
    } else {
        cnt = abs(depth(&u.uz)) + rnd(5);
        k_mndx[0] = PM_KEYSTONE_KOP;
        k_mndx[1] = PM_KOP_SERGEANT;
        k_mndx[2] = PM_KOP_LIEUTENANT;
        k_mndx[3] = PM_KOP_KAPTAIN;
        k_mndx[4] = PM_KOP_KOMMISSIONER;
    }

    k_cnt[0] = cnt;
    k_cnt[1] = (cnt / 3) + 1; /* at least one sarge */
    k_cnt[2] = (cnt / 6);     /* maybe a lieutenant */
    k_cnt[3] = (cnt / 9);     /* and maybe a kaptain */
    k_cnt[4] = (cnt / 12);     /* and maybe a general */

    for (k = 0; k < TYPES; k++) {
        if ((cnt = k_cnt[k]) == 0)
            break;
        mndx = k_mndx[k];
        if (mvitals[mndx].mvflags & G_GONE)
            continue;

        while (cnt--)
            if (enexto(mm, mm->x, mm->y, &mons[mndx]))
                (void) makemon(&mons[mndx], mm->x, mm->y, NO_MM_FLAGS);
    }
}

void
pay_for_damage(dmgstr, cant_mollify)
const char *dmgstr;
boolean cant_mollify;
{
    register struct monst *shkp = (struct monst *) 0;
    char shops_affected[5];
    boolean uinshp = (*u.ushops != '\0');
    char qbuf[80];
    xchar x, y;
    boolean dugwall = (!strcmp(dmgstr, "dig into")    /* wand */
                       || !strcmp(dmgstr, "damage")); /* pick-axe */
    boolean animal, pursue;
    struct damage *tmp_dam, *appear_here = 0;
    long cost_of_damage = 0L;
    unsigned int nearest_shk = (ROWNO * ROWNO) + (COLNO * COLNO),
                 nearest_damage = nearest_shk;
    int picks = 0;

    for (tmp_dam = level.damagelist; tmp_dam; tmp_dam = tmp_dam->next) {
        char *shp;

        if (tmp_dam->when != monstermoves || !tmp_dam->cost)
            continue;
        cost_of_damage += tmp_dam->cost;
        Strcpy(shops_affected,
               in_rooms(tmp_dam->place.x, tmp_dam->place.y, SHOPBASE));
        for (shp = shops_affected; *shp; shp++) {
            struct monst *tmp_shk;
            unsigned int shk_distance;

            if (!(tmp_shk = shop_keeper(*shp)))
                continue;
            if (tmp_shk == shkp) {
                unsigned int damage_distance =
                    distu(tmp_dam->place.x, tmp_dam->place.y);

                if (damage_distance < nearest_damage) {
                    nearest_damage = damage_distance;
                    appear_here = tmp_dam;
                }
                continue;
            }
            if (!inhishop(tmp_shk))
                continue;
            shk_distance = distu(tmp_shk->mx, tmp_shk->my);
            if (shk_distance > nearest_shk)
                continue;
            if ((shk_distance == nearest_shk) && picks) {
                if (rn2(++picks))
                    continue;
            } else
                picks = 1;
            shkp = tmp_shk;
            nearest_shk = shk_distance;
            appear_here = tmp_dam;
            nearest_damage = distu(tmp_dam->place.x, tmp_dam->place.y);
        }
    }

    if (!cost_of_damage || !shkp)
        return;

    animal = (shkp->data->msound <= MS_ANIMAL);
    pursue = FALSE;
    x = appear_here->place.x;
    y = appear_here->place.y;

    /* not the best introduction to the shk... */
    (void) strncpy(ESHK(shkp)->customer, plname, PL_NSIZ);

    /* if the shk is already on the war path, be sure it's all out */
    if (ANGRY(shkp) || ESHK(shkp)->following) {
        hot_pursuit(shkp);
        return;
    }

    /* if the shk is not in their shop.. */
    if (!*in_rooms(shkp->mx, shkp->my, SHOPBASE)) {
        if (!cansee(shkp->mx, shkp->my))
            return;
        pursue = TRUE;
        goto getcad;
    }

    if (uinshp) {
        if (um_dist(shkp->mx, shkp->my, 1)
            && !um_dist(shkp->mx, shkp->my, 3)) {
            pline("%s leaps towards you!", Shknam(shkp));
            mnexto(shkp);
        }
        pursue = um_dist(shkp->mx, shkp->my, 1);
        if (pursue)
            goto getcad;
    } else {
        /*
         * Make shkp show up at the door.  Effect:  If there is a monster
         * in the doorway, have the hero hear the shopkeeper yell a bit,
         * pause, then have the shopkeeper appear at the door, having
         * yanked the hapless critter out of the way.
         */
        if (MON_AT(x, y)) {
            if (!animal) {
                if (!Deaf && !muteshk(shkp)) {
                    You_hear("an angry voice:");
                    verbalize("Out of my way, scum!");
                }
                wait_synch();
#if defined(UNIX) || defined(VMS)
#if defined(SYSV) || defined(ULTRIX) || defined(VMS)
                (void)
#endif
                    sleep(1);
#endif
            } else {
                growl(shkp);
            }
        }
        (void) mnearto(shkp, x, y, TRUE);
    }

    if ((um_dist(x, y, 1) && !uinshp) || cant_mollify
        || (money_cnt(invent) + ESHK(shkp)->credit) < cost_of_damage
        || !rn2(50)) {
 getcad:
        if (muteshk(shkp)) {
            if (animal && shkp->mcanmove && !shkp->msleeping)
                yelp(shkp);
        } else if (pursue || uinshp || !um_dist(x, y, 1)) {
            if (!Deaf)
                verbalize("How dare you %s my %s?", dmgstr,
                          dugwall ? "shop" : "door");
            else
                pline("%s is %s that you decided to %s %s %s!",
                      Shknam(shkp), angrytexts[rn2(SIZE(angrytexts))],
                      dmgstr, noit_mhis(shkp), dugwall ? "shop" : "door");
        } else {
            if (!Deaf) {
                pline("%s shouts:", Shknam(shkp));
                verbalize("Who dared %s my %s?", dmgstr,
                          dugwall ? "shop" : "door");
            } else {
                pline("%s is %s that someone decided to %s %s %s!",
                      Shknam(shkp), angrytexts[rn2(SIZE(angrytexts))],
                      dmgstr, noit_mhis(shkp), dugwall ? "shop" : "door");
            }
        }
        hot_pursuit(shkp);
        return;
    }

    if (Invis)
        Your("invisibility does not fool %s!", shkname(shkp));
    Sprintf(qbuf, "%sYou did %ld %s worth of damage!%s  Pay?",
            !animal ? cad(TRUE) : "", cost_of_damage,
            currency(cost_of_damage), !animal ? "\"" : "");
    if (yn(qbuf) != 'n') {
        cost_of_damage = check_credit(cost_of_damage, shkp);
        if (cost_of_damage > 0L) {
            money2mon(shkp, cost_of_damage);
            context.botl = 1;
        }
        pline("Mollified, %s accepts your restitution.", shkname(shkp));
        /* move shk back to his home loc */
        home_shk(shkp, FALSE);
        pacify_shk(shkp);
    } else {
        if (!animal) {
            if (!Deaf && !muteshk(shkp))
                verbalize("Oh, yes!  You'll pay!");
            else
                pline("%s lunges %s %s toward your %s!",
                      Shknam(shkp), noit_mhis(shkp),
                      mbodypart(shkp, HAND), body_part(NECK));
        } else
            growl(shkp);
        hot_pursuit(shkp);
        if (u.ualign.type != A_NONE)
            adjalign(-sgn(u.ualign.type));
    }
}

/* called in dokick.c when we kick an object that might be in a store */
boolean
costly_spot(x, y)
register xchar x, y;
{
    struct monst *shkp;
    struct eshk *eshkp;

    if (!level.flags.has_shop)
        return FALSE;
    shkp = shop_keeper(*in_rooms(x, y, SHOPBASE));
    if (!shkp || !inhishop(shkp))
        return FALSE;
    eshkp = ESHK(shkp);
    return  (boolean) (inside_shop(x, y)
                       && !(x == eshkp->shk.x && y == eshkp->shk.y));
}

/* called by dotalk(sounds.c) when #chatting; returns obj if location
   contains shop goods and shopkeeper is willing & able to speak */
struct obj *
shop_object(x, y)
register xchar x, y;
{
    register struct obj *otmp;
    register struct monst *shkp;

    if (!(shkp = shop_keeper(*in_rooms(x, y, SHOPBASE))) || !inhishop(shkp))
        return (struct obj *) 0;

    for (otmp = level.objects[x][y]; otmp; otmp = otmp->nexthere)
        if (otmp->oclass != COIN_CLASS)
            break;
    /* note: otmp might have ->no_charge set, but that's ok */
    return (otmp && costly_spot(x, y)
            && NOTANGRY(shkp) && shkp->mcanmove && !shkp->msleeping)
               ? otmp
               : (struct obj *) 0;
}

/* give price quotes for all objects linked to this one (ie, on this spot) */
void
price_quote(first_obj)
register struct obj *first_obj;
{
    register struct obj *otmp;
    char buf[BUFSZ], price[40];
    long cost = 0L;
    int cnt = 0;
    boolean contentsonly = FALSE;
    winid tmpwin;
    struct monst *shkp = shop_keeper(inside_shop(u.ux, u.uy));

    tmpwin = create_nhwindow(NHW_MENU);
    putstr(tmpwin, 0, "Fine goods for sale:");
    putstr(tmpwin, 0, "");
    for (otmp = first_obj; otmp; otmp = otmp->nexthere) {
        if (otmp->oclass == COIN_CLASS)
            continue;
        cost = (otmp->no_charge || otmp == uball || otmp == uchain) ? 0L
                 : get_cost(otmp, shkp ? shkp : (struct monst*)0);
        contentsonly = !cost;
        if (Has_contents(otmp))
            cost += contained_cost(otmp, shkp, 0L, FALSE, FALSE);
        if (otmp->globby)
            cost *= get_pricing_units(otmp);  /* always quan 1, vary by wt */
        if (!cost) {
            Strcpy(price, "no charge");
            contentsonly = FALSE;
        } else {
            Sprintf(price, "%ld %s%s", cost, currency(cost),
                    (otmp->quan) > 1L ? " each" : "");
        }
        Sprintf(buf, "%s%s, %s", contentsonly ? the_contents_of : "",
                doname(otmp), price);
        putstr(tmpwin, 0, buf), cnt++;
    }
    if (cnt > 1) {
        display_nhwindow(tmpwin, TRUE);
    } else if (cnt == 1) {
        if (!cost) {
            /* "<doname(obj)>, no charge" */
            pline("%s!", upstart(buf)); /* buf still contains the string */
        } else {
            /* print cost in slightly different format, so can't reuse buf;
               cost and contentsonly are already set up */
            Sprintf(buf, "%s%s", contentsonly ? the_contents_of : "",
                    doname(first_obj));
            pline("%s, price %ld %s%s%s", upstart(buf), cost, currency(cost),
                  (first_obj->quan > 1L) ? " each" : "",
                  contentsonly ? "." : shk_embellish(first_obj, cost));
        }
    }
    destroy_nhwindow(tmpwin);
}

STATIC_OVL const char *
shk_embellish(itm, cost)
register struct obj *itm;
long cost;
{
    if (!rn2(3)) {
        register int o, choice = rn2(5);

        if (choice == 0)
            choice = (cost < 100L ? 1 : cost < 500L ? 2 : 3);
        switch (choice) {
        case 4:
            if (cost < 10L)
                break;
            else
                o = itm->oclass;
            if (o == FOOD_CLASS)
                return ", gourmets' delight!";
            if (objects[itm->otyp].oc_name_known
                    ? objects[itm->otyp].oc_magic
                    : (o == AMULET_CLASS || o == RING_CLASS || o == WAND_CLASS
                       || o == POTION_CLASS || o == SCROLL_CLASS
                       || o == SPBOOK_CLASS))
                return ", painstakingly developed!";
            return ", superb craftsmanship!";
        case 3:
            return ", finest quality.";
        case 2:
            return ", an excellent choice.";
        case 1:
            return ", a real bargain.";
        default:
            break;
        }
    } else if (itm->oartifact) {
        return ", one of a kind!";
    }
    return ".";
}

/* First 4 supplied by Ronen and Tamar, remainder by development team */
const char *Izchak_speaks[] = {
    "%s says: 'These shopping malls give me a headache.'",
    "%s says: 'Slow down.  Think clearly.'",
    "%s says: 'You need to take things one at a time.'",
    "%s says: 'I don't like poofy coffee... give me Colombian Supremo.'",
    "%s says that getting the devteam's agreement on anything is difficult.",
    "%s says that he has noticed those who serve their deity will prosper.",
    "%s says: 'Don't try to steal from me - I have friends in high places!'",
    "%s says: 'You may well need something from this shop in the future.'",
    "%s comments about the Valley of the Dead as being a gateway."
};

void
shk_chat(shkp)
struct monst *shkp;
{
    struct eshk *eshk;
    long shkmoney;

    if (!shkp->isshk) {
        /* The monster type is shopkeeper, but this monster is
           not actually a shk, which could happen if someone
           wishes for a shopkeeper statue and then animates it.
           (Note: shkname() would be "" in a case like this.) */
        pline("%s asks whether you've seen any untended shops recently.",
              Monnam(shkp));
        /* [Perhaps we ought to check whether this conversation
           is taking place inside an untended shop, but a shopless
           shk can probably be expected to be rather disoriented.] */
        return;
    }

    eshk = ESHK(shkp);
    if (ANGRY(shkp) || hostile_shk(shkp)) {
        pline("%s %s how much %s dislikes %s customers.",
              Shknam(shkp),
              (!Deaf && !muteshk(shkp)) ? "mentions" : "indicates",
              noit_mhe(shkp), eshk->robbed ? "non-paying" : "rude");
    } else if (eshk->following) {
        if (strncmp(eshk->customer, plname, PL_NSIZ)) {
            if (!Deaf && !muteshk(shkp))
                verbalize("%s %s!  I was looking for %s.",
                      Hello(shkp), plname, eshk->customer);
            eshk->following = 0;
        } else {
            if (!Deaf && !muteshk(shkp))
                verbalize("%s %s!  Didn't you forget to pay?",
                          Hello(shkp), plname);
            else
                pline("%s taps you on the %s.",
                      Shknam(shkp), body_part(ARM));
        }
    } else if (eshk->billct) {
        register long total = addupbill(shkp) + eshk->debit;

        pline("%s %s that your bill comes to %ld %s.",
              Shknam(shkp),
              (!Deaf && !muteshk(shkp)) ? "says" : "indicates",
              total, currency(total));
    } else if (eshk->debit) {
        pline("%s %s that you owe %s %ld %s.",
              Shknam(shkp),
              (!Deaf && !muteshk(shkp)) ? "reminds you" : "indicates",
              noit_mhim(shkp), eshk->debit, currency(eshk->debit));
    } else if (eshk->credit) {
        pline("%s encourages you to use your %ld %s of credit.",
              Shknam(shkp), eshk->credit, currency(eshk->credit));
    } else if (eshk->robbed) {
        pline("%s %s about a recent robbery.",
              Shknam(shkp),
              (!Deaf && !muteshk(shkp)) ? "complains" : "indicates concern");
    } else if ((shkmoney = money_cnt(shkp->minvent)) < 50L) {
        pline("%s %s that business is bad.",
              Shknam(shkp),
              (!Deaf && !muteshk(shkp)) ? "complains" : "indicates");
    } else if (shkmoney > 4000) {
        pline("%s %s that business is good.",
              Shknam(shkp),
              (!Deaf && !muteshk(shkp)) ? "says" : "indicates");
    } else if (is_izchak(shkp, FALSE)) {
        if (!Deaf && !muteshk(shkp))
            pline(Izchak_speaks[rn2(SIZE(Izchak_speaks))], shkname(shkp));
    } else {
        if (!Deaf && !muteshk(shkp))
            pline("%s talks about the problem of shoplifters.", Shknam(shkp));
    }
}

STATIC_OVL void
kops_gone(silent)
boolean silent;
{
    register int cnt = 0;
    register struct monst *mtmp, *mtmp2;

    for (mtmp = fmon; mtmp; mtmp = mtmp2) {
        mtmp2 = mtmp->nmon;
        if (mtmp->data->mlet == S_KOP) {
            if (canspotmon(mtmp))
                cnt++;
            mongone(mtmp);
        }
    }
    if (cnt && !silent)
        pline_The("Kop%s (disappointed) vanish%s into thin air.",
                  plur(cnt), (cnt == 1) ? "es" : "");
}

STATIC_OVL long
cost_per_charge(shkp, otmp, altusage)
struct monst *shkp;
struct obj *otmp;
boolean altusage; /* some items have an "alternate" use with different cost */
{
    long tmp = 0L;

    if (!shkp || !inhishop(shkp))
        return 0L; /* insurance */
    tmp = get_cost(otmp, shkp);

    /* The idea is to make the exhaustive use of an unpaid item
     * more expensive than buying it outright.
     */
    if (otmp->otyp == MAGIC_LAMP) { /* 1 */
        /* normal use (ie, as light source) of a magic lamp never
           degrades its value, but not charging anything would make
           identification too easy; charge an amount comparable to
           what is charged for an ordinary lamp (don't bother with
           angry shk surcharge) */
        if (!altusage)
            tmp = (long) objects[OIL_LAMP].oc_cost;
        else
            tmp += tmp / 3L;                 /* djinni is being released */
    } else if (otmp->otyp == MAGIC_MARKER) { /* 70 - 100 */
        /* No way to determine in advance how many charges will be
         * wasted.  So, arbitrarily, one half of the price per use.
         */
        tmp /= 2L;
    } else if (otmp->otyp == BAG_OF_TRICKS /* 1 - 20 */
               || otmp->otyp == BAG_OF_RATS
               || otmp->otyp == MEDICAL_KIT
               || otmp->otyp == HORN_OF_PLENTY) {
        /* altusage: emptying of all the contents at once */
        if (!altusage)
            tmp /= 5L;
    } else if (otmp->otyp == CRYSTAL_BALL               /* 1 - 5 */
               || otmp->otyp == OIL_LAMP                /* 1 - 10 */
               || otmp->otyp == LANTERN
               || (otmp->otyp >= MAGIC_FLUTE
                   && otmp->otyp <= DRUM_OF_EARTHQUAKE) /* 5 - 9 */
               || otmp->oclass == WAND_CLASS) {         /* 3 - 11 */
        if (otmp->spe > 1)
            tmp /= 4L;
    } else if (otmp->otyp == TORCH) {
	    tmp /= 2L;	
	} else if (otmp->oclass == SPBOOK_CLASS) {
        tmp -= tmp / 5L;
    } else if (otmp->otyp == CAN_OF_GREASE || otmp->otyp == TINNING_KIT
               || otmp->otyp == EXPENSIVE_CAMERA) {
        tmp /= 10L;
    } else if (otmp->otyp == POT_OIL) {
        tmp /= 5L;
    }
    return tmp;
}

/* Charge the player for partial use of an unpaid object.
 *
 * Note that bill_dummy_object() should be used instead
 * when an object is completely used.
 */
void
check_unpaid_usage(otmp, altusage)
struct obj *otmp;
boolean altusage;
{
    struct monst *shkp;
    const char *fmt, *arg1, *arg2;
    char buf[BUFSZ];
    long tmp;

    if (!otmp->unpaid || !*u.ushops
        || (otmp->spe <= 0 && objects[otmp->otyp].oc_charged
        && otmp->otyp != TORCH))
        return;
    if (!(shkp = shop_keeper(*u.ushops)) || !inhishop(shkp))
        return;
    if ((tmp = cost_per_charge(shkp, otmp, altusage)) == 0L)
        return;

    arg1 = arg2 = "";
    if (otmp->oclass == SPBOOK_CLASS) {
        fmt = "%sYou owe%s %ld %s.";
        Sprintf(buf, "This is no free library, %s!  ", cad(FALSE));
        arg1 = rn2(2) ? buf : "";
        arg2 = ESHK(shkp)->debit > 0L ? " an additional" : "";
    } else if (otmp->otyp == POT_OIL) {
        fmt = "%s%sThat will cost you %ld %s (Yendorian Fuel Tax).";
    } else if (altusage && (otmp->otyp == BAG_OF_TRICKS
                            || otmp->otyp == BAG_OF_RATS
                            || otmp->otyp == HORN_OF_PLENTY)) {
        fmt = "%s%sEmptying that will cost you %ld %s.";
        if (!rn2(3))
            arg1 = "Whoa!  ";
        if (!rn2(3))
            arg1 = "Watch it!  ";
    } else {
        fmt = "%s%sUsage fee, %ld %s.";
        if (!rn2(3))
            arg1 = "Hey!  ";
        if (!rn2(3))
            arg2 = "Ahem.  ";
    }

    if (!Deaf && !muteshk(shkp)) {
        verbalize(fmt, arg1, arg2, tmp, currency(tmp));
        exercise(A_WIS, TRUE); /* you just got info */
    }
    ESHK(shkp)->debit += tmp;
}

/* for using charges of unpaid objects "used in the normal manner" */
void
check_unpaid(otmp)
struct obj *otmp;
{
    check_unpaid_usage(otmp, FALSE); /* normal item use */
}

void
costly_gold(x, y, amount)
register xchar x, y;
register long amount;
{
    register long delta;
    register struct monst *shkp;
    register struct eshk *eshkp;

    if (!costly_spot(x, y))
        return;
    /* shkp now guaranteed to exist by costly_spot() */
    shkp = shop_keeper(*in_rooms(x, y, SHOPBASE));

    eshkp = ESHK(shkp);
    if (eshkp->credit >= amount) {
        if (eshkp->credit > amount)
            Your("credit is reduced by %ld %s.", amount, currency(amount));
        else
            Your("credit is erased.");
        eshkp->credit -= amount;
    } else {
        delta = amount - eshkp->credit;
        if (eshkp->credit)
            Your("credit is erased.");
        if (eshkp->debit)
            Your("debt increases by %ld %s.", delta, currency(delta));
        else
            You("owe %s %ld %s.", shkname(shkp), delta, currency(delta));
        eshkp->debit += delta;
        eshkp->loan += delta;
        eshkp->credit = 0L;
    }
}

/* used in domove to block diagonal shop-exit */
/* x,y should always be a door */
boolean
block_door(x, y)
register xchar x, y;
{
    register int roomno = *in_rooms(x, y, SHOPBASE);
    register struct monst *shkp;

    if (roomno < 0 || !IS_SHOP(roomno))
        return FALSE;
    if (!IS_DOOR(levl[x][y].typ))
        return FALSE;
    if (roomno != *u.ushops)
        return FALSE;

    if (!(shkp = shop_keeper((char) roomno)) || !inhishop(shkp))
        return FALSE;

    if (shkp->mx == ESHK(shkp)->shk.x && shkp->my == ESHK(shkp)->shk.y
        /* Actually, the shk should be made to block _any_
         * door, including a door the player digs, if the
         * shk is within a 'jumping' distance.
         */
        && ESHK(shkp)->shd.x == x
        && ESHK(shkp)->shd.y == y
        && shkp->mcanmove && !shkp->msleeping
        && (ESHK(shkp)->debit || ESHK(shkp)->billct || ESHK(shkp)->robbed)) {
        pline("%s%s blocks your way!", Shknam(shkp),
              Invis ? " senses your motion and" : "");
        return TRUE;
    }
    return FALSE;
}

/* used in domove to block diagonal shop-entry;
   u.ux, u.uy should always be a door */
boolean
block_entry(x, y)
register xchar x, y;
{
    register xchar sx, sy;
    register int roomno;
    register struct monst *shkp;

    if (!(IS_DOOR(levl[u.ux][u.uy].typ)
          && levl[u.ux][u.uy].doormask == D_BROKEN))
        return FALSE;

    roomno = *in_rooms(x, y, SHOPBASE);
    if (roomno < 0 || !IS_SHOP(roomno))
        return FALSE;
    if (!(shkp = shop_keeper((char) roomno)) || !inhishop(shkp))
        return FALSE;

    if (ESHK(shkp)->shd.x != u.ux || ESHK(shkp)->shd.y != u.uy)
        return FALSE;

    sx = ESHK(shkp)->shk.x;
    sy = ESHK(shkp)->shk.y;

    if (shkp->mx == sx && shkp->my == sy && shkp->mcanmove && !shkp->msleeping
        && (x == sx - 1 || x == sx + 1 || y == sy - 1 || y == sy + 1)
        && (Invis || 
            (!Is_blackmarket(&u.uz) &&
            (carrying(PICK_AXE) 
            || carrying(DWARVISH_MATTOCK)))
            || u.usteed)) {
        pline("%s%s blocks your way!", Shknam(shkp),
              Invis ? " senses your motion and" : "");
        return TRUE;
    }
    return FALSE;
}

/* "your " or "Foobar's " (note the trailing space) */
char *
shk_your(buf, obj)
char *buf;
struct obj *obj;
{
    if (!shk_owns(buf, obj) && !mon_owns(buf, obj))
        Strcpy(buf, the_your[carried(obj) ? 1 : 0]);
    return strcat(buf, " ");
}

char *
Shk_Your(buf, obj)
char *buf;
struct obj *obj;
{
    (void) shk_your(buf, obj);
    *buf = highc(*buf);
    return buf;
}

STATIC_OVL char *
shk_owns(buf, obj)
char *buf;
struct obj *obj;
{
    struct monst *shkp;
    xchar x, y;

    if (get_obj_location(obj, &x, &y, 0)
        && (obj->unpaid || (obj->where == OBJ_FLOOR && !obj->no_charge
                            && costly_spot(x, y)))) {
        shkp = shop_keeper(inside_shop(x, y));
        return strcpy(buf, shkp ? s_suffix(shkname(shkp)) : the_your[0]);
    }
    return (char *) 0;
}

STATIC_OVL char *
mon_owns(buf, obj)
char *buf;
struct obj *obj;
{
    if (obj->where == OBJ_MINVENT)
        return strcpy(buf, s_suffix(y_monnam(obj->ocarry)));
    return (char *) 0;
}

STATIC_OVL const char *
cad(altusage)
boolean altusage; /* used as a verbalized exclamation:  \"Cad! ...\" */
{
    const char *res = 0;

    switch (is_demon(raceptr(&youmonst)) ? 3 : poly_gender()) {
    case 0:
        res = "cad";
        break;
    case 1:
        res = "minx";
        break;
    case 2:
        res = "beast";
        break;
    case 3:
        res = "fiend";
        break;
    default:
        impossible("cad: unknown gender");
        res = "thing";
        break;
    }
    if (altusage) {
        char *cadbuf = mon_nam(&youmonst); /* snag an output buffer */

        /* alternate usage adds a leading double quote and trailing
           exclamation point plus sentence separating spaces */
        Sprintf(cadbuf, "\"%s!  ", res);
        cadbuf[1] = highc(cadbuf[1]);
        res = cadbuf;
    }
    return res;
}

#ifdef __SASC
void
sasc_bug(struct obj *op, unsigned x)
{
    op->unpaid = x;
}
#endif

/*
 * The caller is about to make obj_absorbed go away.
 *
 * There's no way for you (or a shopkeeper) to prevent globs
 * from merging with each other on the floor due to the
 * inherent nature of globs so it irretrievably becomes part
 * of the floor glob mass. When one glob is absorbed by another
 * glob, the two become indistinguishable and the remaining
 * glob object grows in mass, the product of both.
 *
 * billing admin, player compensation, shopkeeper compensation
 * all need to be considered.
 *
 * Any original billed item is lost to the absorption so the
 * original billed amount for the object being absorbed must
 * get added to the cost owing for the absorber, and the
 * separate cost for the object being absorbed goes away.
 *
 * There are four scenarios to deal with:
 *     1. shop_owned glob merging into shop_owned glob
 *     2. player_owned glob merging into shop_owned glob
 *     3. shop_owned glob merging into player_owned glob
 *     4. player_owned glob merging into player_owned glob
 */
void
globby_bill_fixup(obj_absorber, obj_absorbed)
struct obj *obj_absorber, *obj_absorbed;
{
    int x = 0, y = 0;
    struct bill_x *bp, *bp_absorber = (struct bill_x *) 0;
    struct monst *shkp = 0;
    struct eshk *eshkp;
    long amount, per_unit_cost = set_cost(obj_absorbed, shkp);
    boolean floor_absorber = (obj_absorber->where == OBJ_FLOOR);

    if (!obj_absorber->globby)
        impossible("globby_bill_fixup called for non-globby object");

    if (floor_absorber) {
        x = obj_absorber->ox, y = obj_absorber->oy;
    }
    if (obj_absorber->unpaid) {
        /* look for a shopkeeper who owns this object */
        for (shkp = next_shkp(fmon, TRUE); shkp;
             shkp = next_shkp(shkp->nmon, TRUE))
            if (onbill(obj_absorber, shkp, TRUE))
                break;
    } else if (obj_absorbed->unpaid) {
        if (obj_absorbed->where == OBJ_FREE
             && floor_absorber && costly_spot(x, y)) {
            shkp = shop_keeper(*in_rooms(x, y, SHOPBASE));
        }
    }
    /* sanity check, in case obj is on bill but not marked 'unpaid' */
    if (!shkp)
        shkp = shop_keeper(*u.ushops);
    if (!shkp)
        return;
    bp_absorber = onbill(obj_absorber, shkp, FALSE);
    bp = onbill(obj_absorbed, shkp, FALSE);
    eshkp = ESHK(shkp);

    /**************************************************************
     * Scenario 1. Shop-owned glob absorbing into shop-owned glob
     **************************************************************/
    if (bp && (!obj_absorber->no_charge
               || billable(&shkp, obj_absorber, eshkp->shoproom, FALSE))) {
        /* the glob being absorbed has a billing record */
        amount = bp->price;
        eshkp->billct--;
#ifdef DUMB
        {
            /* DRS/NS 2.2.6 messes up -- Peter Kendell */
            int indx = eshkp->billct;

            *bp = eshkp->bill_p[indx];
        }
#else
        *bp = eshkp->bill_p[eshkp->billct];
#endif
        clear_unpaid_obj(shkp, obj_absorbed);

        if (bp_absorber) {
            /* the absorber has a billing record */
            bp_absorber->price += amount;
        } else {
            /* the absorber has no billing record */
            ;
        }
        return;
    }
    /**************************************************************
     * Scenario 2. Player-owned glob absorbing into shop-owned glob
     **************************************************************/
    if (!bp_absorber && !bp && !obj_absorber->no_charge) {
        /* there are no billing records */
        amount = get_pricing_units(obj_absorbed) * per_unit_cost;
        if (saleable(shkp, obj_absorbed)) {
            if (eshkp->debit >= amount) {
                if (eshkp->loan) { /* you carry shop's gold */
                   if (eshkp->loan >= amount)
                        eshkp->loan -= amount;
                   else
                        eshkp->loan = 0L;
                }
                eshkp->debit -= amount;
                pline_The("donated %s %spays off your debt.",
                          obj_typename(obj_absorbed->otyp),
                          eshkp->debit ? "partially " : "");
            } else {
                long delta = amount - eshkp->debit;

                eshkp->credit += delta;
                if (eshkp->debit) {
                    eshkp->debit = 0L;
                    eshkp->loan = 0L;
                    Your("debt is paid off.");
                }
                if (eshkp->credit == delta)
                    pline_The("%s established %ld %s credit.",
                              obj_typename(obj_absorbed->otyp),
                              delta, currency(delta));
                else
                    pline_The("%s added %ld %s %s %ld %s.",
                              obj_typename(obj_absorbed->otyp),
                              delta, currency(delta),
                              "to your credit; total is now",
                              eshkp->credit, currency(eshkp->credit));
            }
        }
        return;
    } else if (bp_absorber) {
        /* absorber has a billing record */
        bp_absorber->price += per_unit_cost * get_pricing_units(obj_absorbed);
        return;
    }
    /**************************************************************
     * Scenario 3. shop_owned glob merging into player_owned glob
     **************************************************************/
    if (bp && (obj_absorber->no_charge
               || (floor_absorber && !costly_spot(x, y)))) {
        amount = bp->price;
        bill_dummy_object(obj_absorbed);
        verbalize("You owe me %ld %s for my %s that you %s with your%s",
                  amount, currency(amount), obj_typename(obj_absorbed->otyp),
                  (ANGRY(shkp) || hostile_shk(shkp))
                           ? "had the audacity to mix" : "just mixed",
                  (ANGRY(shkp) || hostile_shk(shkp))
                           ? " stinking batch!" : "s.");
        return;
    }
    /**************************************************************
     * Scenario 4. player_owned glob merging into player_owned glob
     **************************************************************/

    return;
}


#define CRYNUMBER 5

const char* armor_wares[] = {
        "Any %s would love these!  Finest quality!",
        "Fit for a Knight, but they'll last for weeks!",
        "It's dangerous 'round here these days... better wear something safe!",
        "Hey, %s, I've got something here that'll fit you perfectly!",
        "Guaranteed safety or double your money back!"
};
const char* scroll_wares[] = {
        "Large print available!",
        "'ere now, this isn't a library; get lost, you freeloader!",
        "Waterproof ink upon request!  ... for a small surcharge.",
        "Curses removed, gold detected, and weapons enchanted, at your whim!",
        "If you can read, %s, you'll want some of these!"
};
const char* potion_wares[] = {
        "All drinks locally brewed.",
        "Booze on ice!  Getcher booze on ice!",
        "Come on, %s.  You know you're thirsty.",
        "Ahhh, it'll put hair on yer chest!",
        "Lowest percentage of cursed items around!"
};
const char* weapon_wares[] = {
        "Sharpest weapons around! On sale, today only!",
        "We sell 'em, you stab 'em!",
        "Guaranteed to not dull for ten fights or your money back!",
        "Look, %s, with a face like that you'll be in a lot of fights.  Better buy something now.",
        "You'll never slash the same again after one of ours!"
};
const char* food_wares[] = {
        "Gitchore luvverly orinjes!",
        "Fresh fish! So fresh it'll grab yer naughty bits!",
        "Sausage inna bun!  Hot sausage!",
        "C'mon, you don't want to explore on an empty stomach, do ya?",
        "Genuine pig parts, these. So good most pigs don't even know they got 'em."
};
const char* ring_wares[] = {
        "Well, you seem like a fine, discerning young %s; come look at this.",
        "Special sparklies for a special %s, perhaps?",
        "Once you put one of ours on, you'll never want to take it off!",
        "Our bands never break or melt!",
        "Shiny, isn't it?"
};
const char* wand_wares[] = {
        "Credit available for valued customers!",
        "Crafted out of only the finest materials.",
        "Straightest zaps anywhere!  100%% money back guarantee (less usage)!",
        "Our wands explode less than all others!",
        "New EZ-BREAK feature on these in case of emergency!"
};
const char* tool_wares[] = {
        "Lifetime warrenty on all of our products!",
        "Tins opened, faces wiped, gazes reflected; your one-stop shop!",
        "How you gonna carry all your stuff without a bag, %s?",
        "Must be hard kickin' all those doors down, I bet a key would help...",
        "Only tools wouldn't buy our tools!"
};
const char* book_wares[] = {
        "Large print available!",
        "'ere now, this isn't a library; get lost, you freeloader!",
        "Mental magnificence for the scholarly IN-clined!",
        "Credit available for valued customers!",
        "'Banned' section now open! (I.D. required)"
};
const char* candle_wares[] = {
        "We'll light the way for you.",
        "You've got a long way down yet, %s.  Be sure you're ready.",
        "Let us be the light in your darkness!",
        "You know, I hear some of these old lamps might be... magic.",
        "Be a shame if you missed anything because you didn't see it!",
        /* The below only appears if you're in minetown. */
        "Hey, %s! Best candles in Minetown! You'll need 'em later, count on it!"
};

const char* tin_wares[] = {
        /* Some of these may have to change assuming we ever
         * add the health food store added in Nethack 3.7
         */
        "All corpses naturally sourced.",
        "100%% Organic and preservative free!",
        "Guaranteed fresh!",
        "Corpses kept cool by our patented ice boxes.",
        "Botulism free or your gold back...maybe."
};

const char* black_market_wares[] = {
        /* Good chance nobody will hear these, but doing them anyway */
        "No cheapskates.",
        "Best to heed the warnings outside.",
        "Only criminals get a discount here.  Otherwise, pay up.",
        "Unless you have a death wish, don't even bother stealing.",
        "You might need to rob Fort Ludios to afford things here."
};

const char* music_wares[] = {
        "All instruments cleaned and tuned!",
        "We only carry the finest of instruments.",
        "The favorite store of bards and musicians!",
        "Practice perfect pitch with this powerful paraphernalia.",
        "Become your own one %s band with our selection!"
};

const char* pet_wares[] = {
        "Find your new companion here!",
        "Remember to keep your pet on a leash!",
        "Fresh snacks for your pet!",
        "Pet had an unfortunate accident? Get a new one here!",
        "Don't explore the dungeon alone, take a friend with you!"
};

const char* general_wares[] = {
        "We buy and sell nearly anything.",
        "Feel free to browse our wares, %s.",
        "You'll find anything here!",
        "Feel free to unload your old equipment here for gold.",
        "We'll take what you don't want!"
};

const char* gun_wares[] = { /* GTA Ammunation quotes */
        "Best not to mention where you got these, okay?",
        "I didn't sell you these, right?",
        "No ID Required, friend.",
        "The choice of the professional.",
        "Guns and ammo, all top quality!"
};

void
shk_holler(shkp)
struct monst* shkp;
{
    /* Don't yell from too far away... */
    if (distu(shkp->mx, shkp->my) > 12)
        return;
        
    /* You're either banned from the store or you're going to be banned because of the shirt */
    if (ESHK(shkp)->shoptype != BLACKSHOP && (ESHK(shkp)->pbanned || visible_bad_shirt)) {
        verbalize("Bugger off, you filthy little %s. Don't come begging around here!", urace.noun);
        return;
    }
    switch (ESHK(shkp)->shoptype) {
        default:
            verbalize(general_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case ARMORSHOP:
            verbalize(armor_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case SCROLLSHOP:
            verbalize(scroll_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case POTIONSHOP:
            verbalize(potion_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case WEAPONSHOP:
            verbalize(weapon_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case FOODSHOP:
            verbalize(food_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case RINGSHOP:
            verbalize(ring_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case WANDSHOP:
            verbalize(wand_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case TOOLSHOP:
            verbalize(tool_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case BOOKSHOP:
            verbalize(book_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case CANDLESHOP:
            verbalize(candle_wares[rn2(Is_minetn_level(&u.uz) ? CRYNUMBER + 1 : CRYNUMBER)], urace.noun);
            break;
        case INSTRUMENTSHOP:
            verbalize(music_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case BLACKSHOP:
            verbalize(black_market_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case TINSHOP:
            verbalize(tin_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case PETSHOP:
            verbalize(pet_wares[rn2(CRYNUMBER)], urace.noun);
            break;
        case GUNSHOP:
            verbalize(gun_wares[rn2(CRYNUMBER)], urace.noun);
            break;
    }
}

/*shk.c*/
