import re
import math

# Need parse_lvl
# Need parse_gen
# Need parse_attk
# Need parse_siz
# Need parse_flags

def stripcomments(text):
    return re.sub('//.*?\n|/\*.*?\*/', '', text, flags=re.S)

def extract_data(text):
    mondat = {}
    #splitup = re.split(r',(?![^()]*\))', text)
    splitup = re.split(r',\s*(?![^()]*\))', text)
    # Parse these things
    # Split commas not in parentheses?
    # Name
    mondat['name'] = splitup[0]
    # Symbol
    mondat['sym'] = splitup[1]
    # LVL - Always has ()
    mondat['sym'] = splitup[2]
    # GEN FLAGS - sometimes has ()
    mondat['gen'] = splitup[3]
    # Attacks - always has nested ()
    mondat['attacks'] = splitup[4]

    s = splitup[5]
    commasplit = s.split(',')
    # print(re.search(r'\((.*?)\)', s).group(1))
    
    # Size - Always has ()
    mondat['siz'] = s[s.find("(")+1:s.find(")")]
    
    # M1
    mondat['mr1'] = commasplit[-9]
    # M2
    mondat['mr2'] = commasplit[-8]
    # M3
    mondat['flg1'] = commasplit[-7]
    # M4
    mondat['flg2'] = commasplit[-6]
    # flg3, 
    mondat['flg3'] = commasplit[-5]
    # flg4,
    mondat['flg4'] = commasplit[-4]
    # mflg, 
    mondat['mflg'] = commasplit[-3]
    # difficulty
    mondat['difficulty'] = commasplit[-2]
    # Color 
    mondat['color'] = commasplit[-1]

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
    
    for key, val in extract_data(monsters[0]).items():
        print(key, val)



if __name__ == "__main__":
    main()