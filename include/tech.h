/*      SCCS Id: @(#)tech.h    3.2     98/Oct/30                  */
/* Original Code by Warren Cheung                                 */
/* Copyright 1986, M. Stephenson				  */
/* NetHack may be freely redistributed.  See license for details. */

#ifndef TECH_H
#define TECH_H

/* An array of this struct holds your current techs */
struct tech {
    /* tech id Unique number for the tech see the #defines below */
    int t_id;    
    /* power level: This is the level you were when you got the tech */
    xchar t_lev; 
    /* timeout - time until tech can be used again */
    int t_tout;  
    /* time till effect stops */
    int t_inuse;        
    /* Source(s) */
    long t_intrinsic;        
#define OUTSIDE_LEVEL TIMEOUT /* Level "from outside" started at */
};

struct innate_tech {
    schar   ulevel; /* gains tech at level */
    short   tech_id; /* the tech unique ID*/ 
    int	tech_lev; /* starts at level */
};

struct blitz_tab {
    
    const char *blitz_cmd; /* the typed command */
    const int blitz_len;   /* The length of blitz_cmd */
    int NDECL(
        (*blitz_funct));  /* function called when the command is executed */
    const int blitz_tech; /* the tech designation - determines tech needed
         		       * to access this blitz and the name of the blitz
                           */
    const int blitz_type; /* the type of blitz chain */
#define BLITZ_START 0     /* Starts the chain */
#define BLITZ_CHAIN 1     /* Goes anywhere in the chain (usually middle) */
#define BLITZ_END 2       /* Finishes the chain */
};


#define NO_TECH 	0
#define T_BERSERK 	1
#define T_KIII 		2
#define T_RESEARCH 	3
#define T_SURGERY 	4
#define T_REINFORCE 	5
#define T_FLURRY 	6
#define T_PRACTICE 	7
#define T_EVISCERATE 	8
#define T_HEAL_HANDS 	9
#define T_CALM_STEED 	10
#define T_TURN_UNDEAD 	11
#define T_VANISH 	12
#define T_CUTTHROAT 	13
#define T_BLESSING 	14
#define T_E_FIST 	15
#define T_PRIMAL_ROAR 	16
#define T_LIQUID_LEAP 	17
#define T_CRIT_STRIKE 	18
#define T_RAISE_ZOMBIES 19
#define T_REVIVE 	20
#define T_TINKER 	21
#define T_RAGE	 	22
#define T_BLINK	 	23
#define T_CHI_STRIKE 	24
#define T_DRAW_ENERGY 	25
#define T_CHI_HEALING 	26
#define T_DISARM 	27
#define T_DAZZLE 	28
#define T_BLITZ 	29
#define T_PUMMEL 	30
#define T_G_SLAM 	31
#define T_DASH	 	32
#define T_POWER_SURGE 	33
#define T_SPIRIT_BOMB 	34
#define T_DRAW_BLOOD	35
/* Added by Slash'EM Extended */
#define T_WORLD_FALL	36
#define T_CREATE_AMMO	37
#define T_APPRAISAL	38
#define T_BOOZE		39
/* Added by SlashTHEM */
#define T_SOULEATER 	40
#define T_SHIELD_BLOCK  41
#define T_JEDI_JUMP	42
#define T_CHARGE_SABER	43
#define T_TELEKINESIS	44
#define T_CALL_UNDEAD	45
#define T_SPIRIT_TEMPEST 46
#define T_FORCE_PUSH    47
#define T_SHELL         48
#define T_PICKPOCKET    49
#define T_TUMBLE        50
#define T_SUNDER        51
 
#define MAXTECH 52

#endif /* TECH_H */
