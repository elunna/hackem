import monsters
import re
from pprint import pprint
from collections import defaultdict

ITEM_TYPES = (
    "PROJECTILE",
    "WEAPON",
    "BOW",
    "GUN",
    "BULLET",
    "HELM",
    "DRGN_SCALES",
    "ARMOR",
    "CLOAK",
    "SHIELD",
    "GLOVES",
    "BOOTS",
    "RING",
    "AMULET",
    "CONTAINER",
    "TOOL",
    "WEPTOOL",
    "FOOD",
    "POTION",
    "SCROLL",
    "SPELL",
    "WAND",
    "GEM",
    "ROCK",
    "OBJECT",
    "BITS"
    # Some misc cases: spike, boulder, statue, heavy iron ball, iron chain, venom class
    #"OBJECT"
)
CATEGORIES = {
    # We could probably rewrite this to extract this info from the #defines
    "PROJECTILE": ["name","desc","kn","prob","wt","cost","sdam","ldam","hitbon","metal","sub","color"],
    "WEAPON": ["name","desc","kn","mg","mgc","bi","prob","wt","cost","sdam","ldam","hitbon","typ","sub","metal","color"],
    "BOW": ["name","desc","kn","bi","prob","wt","cost","hitbon","metal","sub","color"],
    "GUN": ["name","app","kn","bi","prob","wt","cost","hitbon","ammotyp","metal","sub","color"],
    "BULLET": ["name","app","kn","prob","wt","cost","sdam","ldam","hitbon","ammotyp","typ","metal","sub","color"],
    "HELM": ["name","desc","kn","mgc","power","prob","delay","wt","cost","ac","can","metal","c"],
    "DRGN_SCALES": ["name","mgc","power","cost","color"],
    "ARMOR": ["name","desc","kn","mgc","blk","power","prob","delay","wt","cost","ac","can","sub","metal","c"],
    "CLOAK": ["name","desc","kn","mgc","power","prob","delay","wt","cost","ac","can","metal","c"],
    "SHIELD": ["name","desc","kn","mgc","blk","power","prob","delay","wt","cost","ac","can","metal","c"],
    "GLOVES": ["name","desc","kn","mgc","power","prob","delay","wt","cost","ac","can","metal","c"],
    "BOOTS": ["name","desc","kn","mgc","power","prob","delay","wt","cost","ac","can","metal","c"],
    "RING": ["name","stone","power","cost","prob","mgc","spec","mohs","metal","color"],
    "AMULET": ["name","desc","power","prob"],
    "CONTAINER": ["name","desc","kn","mgc","chg","prob","wt","cost","mat","color"],
    "TOOL": ["name","desc","kn","mrg","mgc","chg","prob","wt","cost","mat","color"],
    "WEPTOOL": ["name","desc","kn","mgc","chg","bi","prob","wt","cost","sdam","ldam","hitbon","typ","sub","mat","clr"],
    "FOOD": ["name","prob","delay","wt","unk","tin","nutrition","color"],
    "POTION": ["name","desc","mgc","power","prob","cost","color"],
    "SCROLL": ["name","text","mgc","prob","cost"],
    "SPELL": ["name","desc","sub","prob","delay","level","mgc","dir","color"],
    "WAND": ["name","typ","prob","cost","mgc","dir","metal","color"],
    "GEM": ["name","desc","prob","wt","gval","nutr","mohs","glass","color"],
    "ROCK": ["name","desc","kn","prob","wt","gval","sdam","ldam","mgc","nutr","mohs","glass","color"],
    #"OBJECT": [],
    #"BITS": [],
}
def junk(text):
    # Skip junk
    if text.startswith(
            ("#ifdef", 
             "#enddef", 
             "#ifndef",
             #"#define", 
             "#undef",
             "#endif", 
             "#else", 
             "#include",
             "#if 0",
             "void", 
             "struct", 
             "NEARDATA",
             # "BITS(",
             "/*", 
             "*", 
             "*/", 
             "};",
             )):
        return True
    return False

def find_parens(s):
    toret = {}
    pstack = []

    for i, c in enumerate(s):
        if c == '(':
            pstack.append(i)
        elif c == ')':
            if len(pstack) == 0:
                raise IndexError("No matching closing parens at: " + str(i))
                # return {}
            toret[pstack.pop()] = i

    if len(pstack) > 0:
        # raise IndexError("No matching opening parens at: " + str(pstack.pop()))
        return {}
    
    return toret

def clean(text):
    text = monsters.stripcomments(text)
    text = re.sub(' +', ' ', text)
    return text
    
def parse_data(items):
    print("PARSING....................................................")
    item_dict = defaultdict(list)
    
    for i in items:
        # Get rid of quotes
        i = i.replace('"', '')
        split = [x.strip() for x in i.split(',')]
        #print(split)
        cat = split[0]
        name = split[1]
        #print(cat)
        #print(name)
        
        if cat in CATEGORIES:
            split.pop(0) # Get rid of the category name
            #zipped = zip(CATEGORIES[cat], split)
            item_dict[cat].append(split)
    return item_dict

def import_data():
    indata = False
    items = []
    newitem = ""

    with open("../src/objects.c", "r") as a_file:
        otyp = None
        for i, line in enumerate(a_file):
            # if i < LINE_START or i > LINE_END:
            #     continue

            sline = line.strip()

            if junk(sline):
                continue
            if not sline:
                continue

            # if sline.startswith(ITEM_TYPES):
            for t in ITEM_TYPES:
                if sline.startswith(t):
                    #print("Record {}".format(i))
                    otyp = t
                    indata = True
                #print("---start scanning")

            if indata:
                #print("Line {}----------------".format(i))
                # print(sline)

                newitem += sline

                p = find_parens(newitem)

                if p: # Found a match!
                    start = min(p.keys())
                    end = p[start]
                    # if len(p) > 1:
                    #     raise ValueError("too many parentheses!")
                    # start, end = p.popitem()
                    #print("start={} end={}".format(start, end))
                    slice = newitem[start + 1: end]
                    #print("Slice: {}".format(slice))
                    newdat = ",".join([otyp, slice])

                    # Final checks
                    
                    if '"' in newdat: # This unfortunately ignores appearances.
                        newdat = clean(newdat)
                        items.append(newdat)

                    #print("Stop {}".format(i))
                    #print(newdat)

                    indata = False
                    newitem = ""
                    otyp = None
    return items

def main():
    items = import_data()
    item_dict = parse_data(items)
    
    print("DISPLAY.................................................")
    
    for v in item_dict['SCROLL']:
        print(v)

    write_html((item_dict))
    
def write_html(item_dict):
    # Writing to file
    with open("item.htm", "w") as file1:
        file1.write("{{otheruses|||monster (disambiguation)}}\n")
        file1.write("This page lists all of the [[items]] that are in [[Hack'EM]].\n")
        file1.write("An automated script scrapes all of this info directly from the Hack'EM source.\n")
        file1.write("\n")

        for k, items in item_dict.items():
            file1.write("\n")
            file1.write("=={}==\n".format(k))
            file1.write('{|class="prettytable sortable"\n')
            for property in CATEGORIES[k]:
                file1.write("!{}\n".format(property))
            
            file1.write("|-\n")
            
            for item in items:           
                for i, prop in enumerate(item):
                    # if name == "None":
                    #     continue
                    
                    # Name needs to be in [[ ]] brackets to be linked
                    if i == 0:
                        file1.write("|[[{}]]\n".format(prop))
                    else:
                        file1.write("| {}\n".format(prop))
                file1.write("|-\n")

            
            file1.write("|}\n")
            
if __name__ == "__main__":
    main()