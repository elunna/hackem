import re
import math
from datetime import date

at_types = {
    "AT_NONE": "passive",
    "AT_CLAW": "claw",
    "AT_BITE": "bite",
    "AT_KICK": "kick",
    "AT_BUTT": "butt",
    "AT_TUCH": "touch",
    "AT_STNG": "sting",
    "AT_HUGS": "crushing bearhug",
    "AT_SPIT": "spit",
    "AT_ENGL": "engulf",
    "AT_BREA": "breath",
    "AT_EXPL": "explode",
    "AT_BOOM": "explode on death",
    "AT_GAZE": "gaze",
    "AT_TENT": "tentacle",
    "AT_SCRE": "sonic",
    "AT_VOLY": "volley",
    "AT_MULTIPLY": "multiply",
    "AT_WEAP": "weapon",
    "AT_MAGC": "magic",
}

ad_types = {
    "0": 0,
    "AD_PHYS": "physical",
    "AD_MAGM": "magic missile",
    "AD_FIRE": "fire",
    "AD_COLD": "cold",
    "AD_SLEE": "sleep",
    "AD_DISN": "disintegrate",
    "AD_ELEC": "shock",
    "AD_DRST": "poison-str",
    "AD_ACID": "acid",
    "AD_LOUD": "sonic",
    "AD_WATR": "water",
    "AD_PSYC": "psychic",
    "AD_BLND": "blind",
    "AD_STUN": "stun",
    "AD_SLOW": "slow",
    "AD_PLYS": "paralyze",
    "AD_DRLI": "drain life",
    "AD_DREN": "drain energy",
    "AD_LEGS": "wound legs",
    "AD_STON": "petrify",
    "AD_STCK": "sticks",
    "AD_SGLD": "steals gold",
    "AD_SITM": "steals item",
    "AD_SEDU": "seduces",
    "AD_TLPT": "teleport",
    "AD_RUST": "rust",
    "AD_CONF": "confuse",
    "AD_DGST": "digest",
    "AD_HEAL": "heal",
    "AD_WRAP": "wrap",
    "AD_WERE": "lycanthropy",
    "AD_DRDX": "poison-dex",
    "AD_DRCO": "poison-con",
    "AD_DRIN": "drain intelligence",
    "AD_DISE": "disease",
    "AD_DCAY": "decay",
    "AD_SSEX": "seduce",
    "AD_HALU": "hallucination",
    "AD_DETH": "death",
    "AD_PEST": "pestilence",
    "AD_FAMN": "famine",
    "AD_SLIM": "slime",
    "AD_ENCH": "disenchant",
    "AD_CORR": "corrode",
    "AD_CNCL": "cancel",
    "AD_BHED": "behead",
    "AD_LUCK": "drain luck",
    "AD_CLOB": "clobber",
    "AD_POLY": "polymorph",
    "AD_WTHR": "wither",
    "AD_PITS": "create pit",
    "AD_WEBS": "entangle",
    "AD_CURS": "random curse",
    "AD_CLRC": "clerical spell",
    "AD_SPEL": "magic spell",
    "AD_RBRE": "random breath",
    "AD_CALM": "calm",
    "AD_TCKL": "tickle",
    "AD_PIER": "piercing scream",
    "AD_SONG": "paralyzing song",
    "AD_GIBB": "gibberish",
    "AD_SPOR": "spore",
    "AD_DSRM": "disarm",
    "AD_POTN": "random potion",
    "AD_HNGY": "hunger",
    "AD_FEAR": "fear",
    "AD_WIND": "wind",
    "AD_HYDR": "grow heads",
    "AD_QUIL": "quills",
    "AD_LARV": "infect larva",
    "AD_SAMU": "steal amulet",
}
symbols = {
    "S_ANT": "a",
    "S_BLOB": "b",
    "S_COCKATRICE": "c",
    "S_DOG": "d",
    "S_EYE": "e",
    "S_FELINE": "f",
    "S_GREMLIN": "g",
    "S_HUMANOID": "h",
    "S_IMP": "i",
    "S_JELLY": "j",
    "S_KOBOLD": "k",
    "S_LEPRECHAUN": "l",
    "S_MIMIC": "m",
    "S_NYMPH": "n",
    "S_ORC": "o",
    "S_PIERCER": "p",
    "S_QUADRUPED": "q",
    "S_RODENT": "r",
    "S_SPIDER": "s",
    "S_TRAPPER": "t",
    "S_UNICORN": "u",
    "S_VORTEX": "v",
    "S_WORM": "w",
    "S_XAN": "x",
    "S_LIGHT": "y",
    "S_ZOUTHERN": "z",
    "S_ANGEL": "A",
    "S_BAT": "B",
    "S_CENTAUR": "C",
    "S_DRAGON": "D",
    "S_ELEMENTAL": "E",
    "S_FUNGUS": "F",
    "S_GNOME": "G",
    "S_GIANT": "H",
    "S_invisible": "I",
    "S_JABBERWOCK": "J",
    "S_KOP": "K",
    "S_LICH": "L",
    "S_MUMMY": "M",
    "S_NAGA": "N",
    "S_OGRE": "O",
    "S_PUDDING": "P",
    "S_QUANTMECH": "Q",
    "S_RUSTMONST": "R",
    "S_SNAKE": "S",
    "S_TROLL": "T",
    "S_UMBER": "U",
    "S_VAMPIRE": "V",
    "S_WRAITH": "W",
    "S_XORN": "X",
    "S_YETI": "Y",
    "S_ZOMBIE": "Z",
    "S_HUMAN": "@",
    "S_GHOST": " ",
    "S_GOLEM": "`",
    "S_DEMON": "&",
    "S_EEL": ";",
    "S_LIZARD": ":",
    "S_BAD_FOOD": "%",
    "S_BAD_COINS": "$",
    "S_WORM_TAIL": "~",
    "S_MIMIC_DEF": "]",
}

genflags = {
    "G_NOCORPSE": "does not leave a corpse",
    "G_GENO": "can be annihilated",
    "G_LGROUP": "appears in large groups",
    "G_SGROUP": "appears in small groups",
    "G_NOGEN": "generated only specially",
    "G_HELL": "only appears in Gehennom",
    "G_NOHELL": "never generated in Gehennom",
    "G_UNIQ": "unique",
    "G_VLGROUP": "appears in very large groups"
}
CONFERS = {
    "MR_FIRE": "confers fire res",
    "MR_COLD": "confers cold res",
    "MR_SLEEP": "confers sleep res",
    "MR_DISINT": "confers disintegration res",
    "MR_ELEC": "confers shock res",
    "MR_POISON": "confers poison res",
    "MR_ACID": "confers acid res",
    "MR_STONE": '0',
}
FLAG_TO = {
    "MR_FIRE": "resists fire",
    "MR_COLD": "resists cold",
    "MR_SLEEP": "resists sleep",
    "MR_DISINT": "resists disintegration",
    "MR_ELEC": "resist shock",
    "MR_POISON": "resists poison",
    "MR_ACID": "resists acid",
    "MR_STONE": "resists petrification",
    "MR_PSYCHIC": "resists psychic",
    "MR_CLOB": "resists clobber",
    "MR_SONIC": "resists sonic",
    "MR2_SEE_INVIS": "sees invisible",
    "MR2_LEVITATE": "levitates",
    "MR2_WATERWALK": "walks on water",
    "MR2_MAGBREATH": "magical breathing",
    "MR2_DISPLACED": "displaced",
    "MR2_STRENGTH": "strong",
    "MR2_FUMBLING": "fumbles",
    "MR2_TELEPATHY": "telepathic",
    "MR2_JUMPING": "jumps",
    "MR2_REFLECTION": "reflective",
    "M1_FLY": "flies",
    "M1_SWIM": "swims",
    "M1_AMORPHOUS": "amorphous",
    "M1_WALLWALK": "phases",
    "M1_CLING": "clings",
    "M1_TUNNEL": "tunnels",
    "M1_NEEDPICK": "needs a pickaxe",
    "M1_CONCEAL": "conceals",
    "M1_HIDE": "hides",
    "M1_AMPHIBIOUS": "amphibious",
    "M1_BREATHLESS": "breathless",
    "M1_NOTAKE": "doesn't pick up items",
    "M1_NOEYES": "eyeless",
    "M1_NOHANDS": "handless",
    "M1_NOLIMBS": "limbless",
    "M1_NOHEAD": "headless",
    "M1_MINDLESS": "mindless",
    "M1_HUMANOID": "humanoid",
    "M1_ANIMAL": "animal",
    "M1_SLITHY": "slithy",
    "M1_UNSOLID": "unsolid",
    "M1_THICK_HIDE": "thick hide",
    "M1_OVIPAROUS": "oviparous",
    "M1_REGEN": "regenerates",
    "M1_SEE_INVIS": "sees invisible",
    "M1_TPORT": "teleports",
    "M1_TPORT_CNTRL": "teleport control",
    "M1_ACID": "acidic",
    "M1_POIS": "poisonous",
    "M1_CARNIVORE": "carnivore",
    "M1_HERBIVORE": "herbivore",
    "M1_OMNIVORE": "omnivore",
    "M1_METALLIVORE": "eats metal",
    "M2_NOPOLY": "cannot be polymorphed into",
    "M2_JUMPER": "jumps",
    "M2_LORD": "lord",
    "M2_PRINCE": "prince",
    "M2_MINION": "minion",
    "M2_MERC": "mercenary",
    "M2_SHAPESHIFTER": "shapeshifts",
    "M2_MALE": "male",
    "M2_FEMALE": "female",
    "M2_NEUTER": "neuter",
    "M2_PNAME": "named",
    "M2_HOSTILE": "hostile",
    "M2_PEACEFUL": "peaceful",
    "M2_DOMESTIC": "domestic",
    "M2_WANDER": "wanders",
    "M2_STALK": "stalks you",
    "M2_NASTY": "nasty",
    "M2_STRONG": "strong",
    "M2_ROCKTHROW": "throws rocks",
    "M2_GREEDY": "greedy",
    "M2_JEWELS": "collects jewels",
    "M2_COLLECT": "collects",
    "M2_FLANK": "outflanks",
    "M2_MAGIC": "magical",
    "M3_WANTSAMUL": "wants the amulet",
    "M3_WANTSBELL": "wants the bell",
    "M3_WANTSBOOK": "wants the book",
    "M3_WANTSCAND": "wants the candelabrum",
    "M3_WANTSARTI": "wants the quest artifact",
    "M3_WANTSALL": "wants all",
    "M3_WAITFORU": "waits for you",
    "M3_CLOSE": "keeps close",
    "M3_COVETOUS": "covetous",
    "M3_WAITMASK": "waits",
    "M3_INFRAVISION": "has infravision",
    "M3_INFRAVISIBLE": "is infravisible",
    "M3_DISPLACES": "displaces",
    "M3_SKITTISH": "skittish",
    "M3_ACCURATE": "accurate",
    "M3_BERSERK": "berserks",
    "M3_TRAITOR": "can betray",
    "M3_NOTAME": "untameable",
    "M4_VULNERABLE_FIRE": "vulnerable to fire",
    "M4_VULNERABLE_COLD": "vulnerable to cold",
    "M4_VULNERABLE_ELEC": "vulnerable to shock",
    "M4_VULNERABLE_ACID": "vulnerable to acid",
    "M4_STATIONARY": "doesn't move",
    "MH_HUMAN": "human",
    "MH_ELF": "elvish",
    "MH_DWARF": "dwarvish",
    "MH_GNOME": "gnomish",
    "MH_ORC": "orcish",
    "MH_GIANT": "giant",
    "MH_HOBBIT": "hobbitish",
    "MH_CENTAUR": "centaur",
    "MH_ILLITHID": "illithid",
    "MH_TORTLE": "tortle",
    "MH_VAMPIRE": "vampire",
    "MH_UNDEAD": "undead",
    "MH_WERE": "lycanthrope",
    "MH_DEMON": "demon",
    "MH_DRAGON": "dragon",
    "MH_ANGEL": "angelic",
    "MH_OGRE": "ogre",
    "MH_TROLL": "troll",
    "MH_GNOLL": "gnoll",
    "MH_JABBERWOCK": "jabberwock",
    "0L": "0",
}

IGNORE_LIST = [
    "needs a pickaxe",
    "doesn't pick up items",
    "eyeless",
    "handless",
    "limbless",
    "headless",
    "mindless",
    "humanoid",
    "animal",
    "slithy",
    "carnivore",
    "herbivore",
    "omnivore",
    "eats metal",
    "male",
    "female",
    "neuter",
    "named",
    "hostile",
    "peaceful",
    "strong",
    "accurate",
    "greedy",
    "collects jewels",
    "collects",
    "oviparous",
    "magical",
    "has infravision",
    "is infravisible",
    "wanders",
    "skittish",
    "thick hide",
]
def process_attacks(attks):
    new_attacks = []
    for atk in attks:
        atk = strip_list(atk.split(","))
        if atk[1] and atk[1] != "AD_PHYS":
            new_attacks.append("{} {}d{} ({})".format(
                at_types[atk[0]],
                atk[2],
                atk[3],
                ad_types[atk[1]],
            ))
        else:
            new_attacks.append("{} {}d{}".format(
                at_types[atk[0]],
                atk[2],
                atk[3],
                ad_types[atk[1]],
            ))
    return new_attacks

def stripcomments(text):
    return re.sub('//.*?\n|/\*.*?\*/', '', text, flags=re.S)

def extract_parens(text):
    return text[text.find("(") + 1:text.find(")")]

def strip_list(l):
    return [i.strip() for i in l]

def split_by_commas(text):
    #splitup = re.split(r',(?![^()]*\))', text)
    return re.split(r',\s*(?![^()]*\))', text)


# Remove any parentheses(with content)
def purge_parens(text):
    return re.sub(r'\(.*?\)', '', text)

def is_integer(num):
    """ Returns True if the num argument is an integer, and False if it is not. """
    try:
        num = float(num)
    except ValueError:
        return False

    return num.is_integer()

def extract_allparens(text):
    """ Looks through a string and extracts all text contained in parentheses,
        and returns a list with a string for each parentheses found. Discards
        the parens.
    """
    #  Stackoverflow: re.findall('\(.*?\)',s)
    #  parens = re.findall(r'\(.*?\)+', text)    # This keeps the outer parens

    # Note: Regex doesn't like newlines, text should be cleaned of them before this.
    parens = re.findall(r'\((.*?)\)+', text)  # Discard the parentheses
    return parens


def extract_data(text):
    mondat = {}
    # Initial cleanup
    text = text.strip()
    
    if not text.startswith("MON("):
        return mondat
    
    text = text.lstrip("MON(")
    text = text.rstrip(',')
    text = text.rstrip(')')
    text = text.replace("A(ATTK", "ATTK")
    text = text.replace("NO_ATTK)", "NO_ATTK")
    text = text.replace("))", ")")
    text = text.replace("ATTK", "")
    
    parens = extract_allparens((text))
    text = purge_parens(text)
    #print("text = {}".format(text))
    #print("")
    splitup = split_by_commas(text)
    #print("splitup = {}".format(splitup))
    #print("splitup[0] = {}".format(splitup[0]))
    #print("")
    # Name
    
    mondat['name'] = splitup[0].strip("\"")
    
    # Symbol
    mondat['sym'] = splitup[1]
    
    # LVL - Always has ()
    # 5 fields: base level, speed, AC, MR, alignment)
    # lvl = extract_parens(splitup[2]).split(',')
    lvl = strip_list(parens.pop(0).split(','))
    mondat['base level'] = lvl[0]
    mondat['speed'] = lvl[1]
    mondat['AC'] = lvl[2]
    mondat['MR'] = lvl[3]
    mondat['alignment'] = lvl[4]

    # GEN FLAGS - sometimes has ()
    # gen = extract_parens(splitup[3]).split(',')[0]
    gen = strip_list(parens.pop(0).split('|'))
    mondat['gen'] = []
    for i in gen:
        if is_integer(i):
            mondat['frequency'] = int(i)
        elif i in genflags and i != "G_GENO":
            mondat['gen'].append(genflags[i])
            
    if "G_GENO" not in gen:
        mondat['gen'].append("can not be annihilated")
            
    # Size - Always has ()
    # Fields: weight, nut, sound, size
    siz = strip_list(parens.pop(-1).split(','))
    mondat['weight'] = siz[0]
    mondat['nutrition'] = siz[1]
    mondat['sound'] = siz[2]
    mondat['size'] = siz[3]

    # Attacks - always has nested ()
    # Anything left over is attacks.
    mondat['attacks'] = parens
    print(parens)
    
    # Misc Flags
    # M1, M2, M3, M4, flg3, flg4, mflg, 
    flags = []
    #print(splitup)
    for i in range(-9, -2):
        # print(i)
        for f in splitup[i].split('|'):
            f = f.strip()
            result = 0
            if f != '0':
                if i == -8:
                    result = CONFERS[f]
                else:
                    result = (FLAG_TO[f])
                if result != "0" and result not in IGNORE_LIST:
                    flags.append(result)
            
    mondat['flags'] = flags
    
    # difficulty
    mondat['difficulty'] = splitup[-2].strip()
    
    # Color 
    mondat['color'] = splitup[-1].strip()

    return mondat

def main():
    with open("src/monst.c", "r") as a_file:
        monsters = []
        newmon = ""
        skipping = False;
        for line in a_file:
            stripped_line = line.strip()
            
            # Skip junk
            if stripped_line.startswith(
                    ("#ifdef", "#enddef", "#ifndef", 
                     "#define", "#endif", "#if 0",
                     "void", "struct", "NEARDATA",
                    "/*", "*", "*/", "};",
                     )):
                continue
            if "/* parser skip*/" in stripped_line:
                continue
            # if stripped_line.endswith(r" \\"):
            #     continue
                
            if not stripped_line:
                continue
                
            if "MON(" in stripped_line:
                # Save current and Start a new listing

                newmon = stripcomments(newmon)
                newmon = re.sub(' +', ' ', newmon)
                monsters.append(newmon)
                newmon = ""
                newmon += stripped_line
            else:
                newmon += stripped_line

        # for m in monsters:
        #     print(m)
        #     print("")

    # remove some of the early chafe
    # monsters.pop(0)
    # monsters.pop(0)
    # monsters.pop(0)
    # monsters.pop(0)

    # Writing to file
    with open("monsters.htm", "w") as file1:
        # Writing data to a file
        file1.write("{{otheruses|||monster (disambiguation)}}\n")
        file1.write("This page lists all of the [[monster]]s that are in [[Hack'EM]].\n")
        file1.write("\n")
        file1.write("Some of these monsters might vary from their representations in vanilla NetHack or in other variants.\n")
        file1.write("Note that most of the information presented here can be obtained about any creature in Hack'EM by "
                    "using the far look command. The information displayed by this command is considered the single "
                    "source of truth about monsters.\n")
        file1.write("\n")
        file1.write("\n")
        file1.write('{|class="wikitable sortable" \n')
        file1.write("|-\n")
        file1.write('!scope="col" | Monster\n')
        file1.write('!scope="col" | Sym\n')
        file1.write('!scope="col" | Lvl\n')
        file1.write('!scope="col" | Freq\n')
        file1.write('!scope="col" | Speed\n')
        file1.write('!scope="col" | AC\n')
        file1.write('!scope="col" | MC\n')
        file1.write('!scope="col" | Diff\n')
        file1.write('!scope="col" class="unsortable"| Attacks\n')
        file1.write('!scope="col" class="unsortable"| Properties\n')
        file1.write('!scope="col" class="unsortable"| Notes\n')
        file1.write("\n")
        
        file1.write("|-\n")
        file1.write("\n")

        for i, m in enumerate(monsters):
            print("Monster #{}".format(i))
            print(m)
            print("")
            mdict =  extract_data(m)
            if not mdict:
                continue
                
            for key, val in mdict.items():
                print("{:15} {}".format(key, val))
            print("--------------------------")
            file1.write("|[[{}]]\n".format(mdict['name']))
            file1.write("|{}\n".format(symbols[mdict['sym']]))
            file1.write("|{}\n".format(mdict['base level']))
            file1.write("|{}\n".format(mdict.get('frequency', 'n/a')))
            file1.write("|{}\n".format(mdict.get('speed', 'n/a')))
            file1.write("| {}\n".format(mdict.get('AC', 'n/a')))
            file1.write("| {}\n".format(mdict.get('MR', 'n/a')))
            file1.write("|{}\n".format(mdict.get('difficulty', 'n/a')))
            file1.write("|{}\n".format("<br>".join(process_attacks(mdict['attacks']))))
            file1.write("|{}\n".format("<br>".join(mdict['flags'])))
            file1.write("|{}\n".format("<br>".join(mdict['gen'])))
            file1.write("|-\n")
        
        file1.write("|}\n")
        file1.write("\n")
        file1.write("==See also==\n")
        file1.write("*[[Item (Hack'EM)]]\n")
        file1.write("\n")
        today = date.today()
        file1.write("Last updated: {}<br>".format(today))
        file1.write("{{hackem-008}}\n")
        file1.write("[[Category:Hack'EM monsters| ]]\n")
    


if __name__ == "__main__":
    main()