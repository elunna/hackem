/* NetHack 3.6	monattk.h	$NHDT-Date: 1432512775 2015/05/25 00:12:55 $  $NHDT-Branch: master $:$NHDT-Revision: 1.11 $ */
/* NetHack may be freely redistributed.  See license for details. */
/* Copyright 1988, M. Stephenson */

#ifndef MONATTK_H
#define MONATTK_H

/*	Add new attack types below - ordering affects experience (exper.c).
 *	Attacks > AT_BUTT are worth extra experience.
 */
#define AT_ANY (-1) /* fake attack; dmgtype_fromattack wildcard */
#define AT_NONE 0   /* passive monster (ex. acid blob) */
#define AT_CLAW 1   /* claw (punch, hit, etc.) */
#define AT_BITE 2   /* bite */
#define AT_KICK 3   /* kick */
#define AT_BUTT 4   /* head butt (ex. a unicorn) */
#define AT_TUCH 5   /* touches */
#define AT_STNG 6   /* sting */
#define AT_HUGS 7   /* crushing bearhug */
#define AT_SPIT 8   /* spits substance - ranged */
#define AT_ENGL 9   /* engulf (swallow or by a cloud) */
#define AT_BREA 10  /* breath - ranged */
#define AT_EXPL 11  /* explodes - proximity */
#define AT_BOOM 12  /* explodes when killed */
#define AT_GAZE 13  /* gaze - ranged */
#define AT_TENT 14  /* tentacles */
#define AT_SCRE 15  /* scream - sonic attack */
#define AT_VOLY 16  /* launch projectiles (ex. manticore spikes) */
#define AT_MULTIPLY	17	/* RJ - multiplies (yes, it's an attack) */

#define AT_WEAP 18  /* uses weapon */
#define AT_MAGC 19  /* uses magic spell(s) */
#define LAST_AT AT_MAGC

/*	Add new damage types below.
 *
 *	Note that 1-10 correspond to the types of attack used in buzz().
 *	Please don't disturb the order unless you rewrite the buzz() code.
 */
#define AD_ANY (-1) /* fake damage; attacktype_fordmg wildcard */
#define AD_PHYS 0   /* ordinary physical */
#define AD_MAGM 1   /* magic missiles */
#define AD_FIRE 2   /* fire damage */
#define AD_COLD 3   /* frost damage */
#define AD_SLEE 4   /* sleep ray */
#define AD_DISN 5   /* disintegration (death ray) */
#define AD_ELEC 6   /* shock damage */
#define AD_DRST 7   /* drains str (poison) */
#define AD_ACID 8   /* acid damage */
#define AD_LOUD 9   /* sound damage */
#define AD_WATR 10  /* water (physical) attack */
#define AD_DRLI 11  /* drains life levels (Vampire) */
#define AD_STUN 12  /* stuns */
/* End of ray attacks */

#define AD_PSYC 13  /* psionic attack */
#define AD_BLND 14  /* blinds (yellow light) */
#define AD_SLOW 15  /* slows */
#define AD_PLYS 16  /* paralyses */
#define AD_DREN 17  /* drains magic energy */
#define AD_LEGS 18  /* damages legs (xan) */
#define AD_STON 19  /* petrifies (Medusa, cockatrice) */
#define AD_STCK 20  /* sticks to you (mimic) */
#define AD_SGLD 21  /* steals gold (leppie) */
#define AD_SITM 22  /* steals item (nymphs) */
#define AD_SEDU 23  /* seduces & steals multiple items */
#define AD_TLPT 24  /* teleports you (Quantum Mech.) */
#define AD_RUST 25  /* rusts armour (Rust Monster)*/
#define AD_CONF 26  /* confuses (Umber Hulk) */
#define AD_DGST 27  /* digests opponent (trapper, etc.) */
#define AD_HEAL 28  /* heals opponent's wounds (nurse) */
#define AD_WRAP 29  /* special "stick" for eels */
#define AD_WERE 30  /* confers lycanthropy */
#define AD_DRDX 31  /* drains dexterity (quasit) */
#define AD_DRCO 32  /* drains constitution */
#define AD_DRIN 33  /* drains intelligence (mind flayer) */
#define AD_DISE 34  /* confers diseases */
#define AD_DCAY 35  /* decays organics (brown Pudding) */
#define AD_SSEX 36  /* Succubus seduction (extended) */
#define AD_HALU 37  /* causes hallucination */
#define AD_DETH 38  /* for Death only */
#define AD_PEST 39  /* for Pestilence only */
#define AD_FAMN 40  /* for Famine only */
#define AD_SLIM 41  /* turns you into green slime */
#define AD_ENCH 42  /* remove enchantment (disenchanter) */
#define AD_CORR 43  /* corrode armor (black pudding) */
#define AD_CNCL 44  /* cancellation */
#define AD_BHED 45  /* beheading (vorpal jabberwock) */
#define AD_LUCK 46  /* affects luck (magical eye) */
#define AD_CLOB 47  /* knock-back attack */
#define AD_POLY 48  /* polymorph the target (genetic engineer) */
#define AD_WTHR 49  /* withering attack (mummies) */
#define AD_PITS 50  /* create pit under target (pit fiend) */
#define AD_WEBS 51  /* entangles target in webbing */
#define AD_CURS 52  /* random curse (ex. gremlin) */
#define AD_CLRC 53  /* random clerical spell */
#define AD_SPEL 54  /* random magic spell */
#define AD_RBRE 55  /* random breath weapon */
#define AD_CALM	56  /* KMH -- calms its enemies (koala) */
#define AD_TCKL	57  /* Tickle (Nightgaunts) */
#define AD_PIER 58  /* Piercing scream (Mobats) */
#define AD_SONG 59  /* Paralyzing song (Harpy) */
#define AD_GIBB 60  /* Gibber chitter-chatter attack (Gibberlings) */
#define AD_SPOR	61	/* generate spore */
#define AD_DSRM 62  /* disarm the player */
#define AD_POTN 63 /* potion effect (gel) */
#define AD_HNGY 64 /* a mini version of FAMN, should fake a higher hunger rating instead  */
#define AD_FEAR 65 /* inflict fear (eye of fear and flame) */
#define AD_WIND 66  /* cause the defender to hurtle */
#define AD_HYDR 67  /* regrow heads (hydra) */
#define AD_QUIL 68  /* quills and spikes (porcupine and manticore) */
#define AD_LARV 69  /* Inject Larvae (Brood Wasp) */
#define AD_LOST 70  /* get lost (yuki-onna) */
#define AD_SAMU 71  /* hits, may steal Amulet (Wizard) */
#define LAST_AD AD_SAMU

/*
 *  Monster to monster attacks.  When a monster attacks another (mattackm),
 *  any or all of the following can be returned.  See mattackm() for more
 *  details.
 */
#define MM_MISS 0x0     /* aggressor missed */
#define MM_HIT 0x1      /* aggressor hit defender */
#define MM_DEF_DIED 0x2 /* defender died */
#define MM_AGR_DIED 0x4 /* aggressor died */
#define MM_EXPELLED 0x8 /* defender was saved by slow digestion */

#endif /* MONATTK_H */
