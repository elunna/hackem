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
    
    if not text.startswith("MON("):
        return mondat
    
    text = text.lstrip("MON(")
    text = text.rstrip(',')
    text = text.rstrip(')')
    text = text.replace("A(ATTK", "ATTK")
    text = text.replace("NO_ATTK)", "NO_ATTK")
    text = text.replace("))", ")")
    
    parens = extract_allparens((text))
    text = purge_parens(text)
    print("text = {}".format(text))
    print("")
    splitup = split_by_commas(text)
    print("splitup = {}".format(splitup))
    print("splitup[0] = {}".format(splitup[0]))
    print("")
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
    #print(splitup)
    for i in range(-9, -2):
        # print(i)
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
        file1.write("This page lists all the new [[monster]]s that are added in [[Hack'EM]].\n")
        file1.write("\n")
        file1.write('{|class="wikitable sortable" \n')
        file1.write("|-\n")
        file1.write('!scope="col" | Monster\n')
        file1.write('!scope="col" | Symbol\n')
        file1.write('!scope="col" | Level\n')
        file1.write('!scope="col" class="unsortable"| Attacks\n')
        file1.write('!scope="col" class="sortable"| Notes\n')
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
            file1.write("|{}\n".format(mdict['sym']))
            file1.write("|{}\n".format(mdict['base level']))
            file1.write("|{}\n".format("|".join(mdict['attacks'])))
            file1.write("| Notes\n")
            file1.write("|-\n")
        
        file1.write("|}\n")
        file1.write("\n")
        file1.write("==See also==\n")
        file1.write("*[[Item (Hack'EM)]]\n")
        file1.write("\n")
        file1.write("{{hackem-008}}\n")
        file1.write("[[Category:Hack'EM monsters| ]]\n")
    


if __name__ == "__main__":
    main()