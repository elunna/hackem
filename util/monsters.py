import re
import math
from collections import defaultdict
import data
from datetime import date

def process_attacks(attks):
    new_attacks = []
    for atk in attks:
        atk = strip_list(atk.split(","))
        if atk[1] and atk[1] != "AD_PHYS":
            new_attacks.append("{} {}d{} ({})".format(
                data.at_types[atk[0]],
                atk[2],
                atk[3],
                data.ad_types[atk[1]],
            ))
        else:
            new_attacks.append("{} {}d{}".format(
                data.at_types[atk[0]],
                atk[2],
                atk[3],
                data.ad_types[atk[1]],
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
        elif i in data.genflags and i != "G_GENO":
            mondat['gen'].append(data.genflags[i])
            
    if "G_GENO" not in gen:
        mondat['gen'].append("can not be genocided")
            
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
                    result = data.CONFERS[f]
                else:
                    result = (data.FLAG_TO[f])
                if result != "0":
                    flags.append(result)
            
    mondat['flags'] = flags
    
    # difficulty
    mondat['difficulty'] = splitup[-2].strip()
    
    # Color 
    mondat['color'] = splitup[-1].strip()

    return mondat

def write_html(monsters):
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
        file1.write("An automated script scrapes all of this info directly from the Hack'EM source.\n")
        file1.write("\n")
        file1.write("== Hack'EM Monsters ==\n")
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
            for key, val in m.items():
                print("{:15} {}".format(key, val))
            print("--------------------------")
            file1.write("|[[{}]]\n".format(m['name']))
            file1.write("|{}\n".format(data.symbols[m['sym']]))
            file1.write("|{}\n".format(m['base level']))
            file1.write("|{}\n".format(m.get('frequency', 'n/a')))
            file1.write("|{}\n".format(m.get('speed', 'n/a')))
            file1.write("| {}\n".format(m.get('AC', 'n/a')))
            file1.write("| {}\n".format(m.get('MR', 'n/a')))
            file1.write("|{}\n".format(m.get('difficulty', 'n/a')))
            file1.write("|{}\n".format("<br>".join(process_attacks(m['attacks']))))
            file1.write("|{}\n".format("<br>".join(m['flags'])))
            file1.write("|{}\n".format("<br>".join(m['gen'])))
            file1.write("|-\n")
        file1.write("|}\n")

        # Monster analysis
        file1.write("== Analysis ==\n")

        # Count the monsters
        num_monsters = len(monsters)
        file1.write("Total count of monsters: {}\n".format(num_monsters))

        count_dict = defaultdict(int)
        lvl_dict = defaultdict(int)
        diff_dict = defaultdict(int)
        size_dict = defaultdict(int)
    
        for m in monsters:
            lvl_dict[int(m['base level'])] += 1
            diff_dict[int(m['difficulty'])] += 1
            size_dict[m['size']] += 1
    
            for t in data.FLAG_TO.values():
                if t in m['flags']:
                    count_dict[t] += 1
    
            if "amphibious" in m['flags'] or "breathless" in m['flags']:
                count_dict["resists water"] += 1

        file1.write("=== Monster Base Levels ===\n")
        
        # Level table
        file1.write('{|class="prettytable sortable striped"\n')
        file1.write('!Level\n')
        file1.write('!Count\n')
        file1.write('!Percent\n')
        file1.write('|-\n')
        
        for k in sorted(lvl_dict.keys()):
            percent = (lvl_dict[k] / num_monsters) * 100
            file1.write('| {}\n'.format(k))
            file1.write('| {}\n'.format(lvl_dict[k]))
            file1.write('| {:.2f}%\n'.format(percent))
            file1.write('|-\n')
        file1.write('|}\n')

        file1.write("\n")
        
        # Difficulty table
        file1.write('{|class="prettytable sortable striped"\n')
        file1.write('!Difficulty\n')
        file1.write('!Count\n')
        file1.write('!Percent\n')
        file1.write('|-\n')
        for k in sorted(diff_dict.keys()):
            percent = (diff_dict[k] / num_monsters) * 100
            file1.write('| {}\n'.format(k))
            file1.write('| {}\n'.format(diff_dict[k]))
            file1.write('| {:.2f}%\n'.format(percent))
            file1.write('|-\n')
        file1.write('|}\n')


        file1.write("\n")
        
        # Size table
        file1.write('{|class="prettytable sortable striped"\n')
        file1.write('!Size\n')
        file1.write('!Count\n')
        file1.write('!Percent\n')
        file1.write('|-\n')
        for k in data.size_list:
            percent = (size_dict[k] / num_monsters) * 100
            file1.write('| {}\n'.format(k))
            file1.write('| {}\n'.format(size_dict[k]))
            file1.write('| {:.2f}%\n'.format(percent))
            file1.write('|-\n')
        file1.write('|}\n')


        file1.write("\n")
        
        # Flag table
        file1.write('{|class="prettytable sortable striped"\n')
        file1.write('!Flag\n')
        file1.write('!Count\n')
        file1.write('!Percent\n')
        file1.write('|-\n')
        for t in data.FLAG_TO.values():
            percent = (count_dict[t] / num_monsters) * 100
            file1.write('| {}\n'.format(t))
            file1.write('| {}\n'.format(count_dict[t]))
            file1.write('| {:.2f}%\n'.format(percent))
            file1.write('|-\n')
        file1.write('|}\n')

        file1.write("\n")
        
        today = date.today()
        file1.write("Last updated: {}<br>".format(today))
        file1.write("[[Category:HackEM| ]]\n")

def main():
    with open("../src/monst.c", "r") as a_file:
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
                mdict = extract_data(newmon)
                if mdict:
                    monsters.append(mdict)
                newmon = ""
                newmon += stripped_line
            else:
                newmon += stripped_line
                
    write_html(monsters)
    

if __name__ == "__main__":
    main()