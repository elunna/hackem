import re
import math

# Need parse_lvl
# Need parse_gen
# Need parse_attk
# Need parse_siz
# Need parse_flags

# Extract from parentheses

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
    text = text.lstrip("MON(")
    text = text.rstrip(',')
    text = text.rstrip(')')
    text = text.replace("A(ATTK", "ATTK")
    text = text.replace("NO_ATTK)", "NO_ATTK")
    
    parens = extract_allparens((text))
    text = purge_parens(text)

    splitup = split_by_commas(text)
    
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
    gen = parens.pop(0)
    mondat['gen'] = strip_list(gen.split('|'))


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

    # Misc Flags
    # M1, M2, M3, M4, flg3, flg4, mflg, 
    flags = []
    for i in range(-9, -2):
        for f in splitup[i].split('|'):
            f = f.strip()
            if f != '0':
                flags.append(f)
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

        for line in a_file:
            stripped_line = line.rstrip()
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
    monsters.pop(0)
    monsters.pop(0)
    monsters.pop(0)
    monsters.pop(0)
    
    for m in range(50):
        for key, val in extract_data(monsters[m]).items():
            print("{:15} {}".format(key, val))
        print("--------------------------")



if __name__ == "__main__":
    main()