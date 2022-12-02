import monsters
import re

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
    
def main():
    indata = False
    items = []
    newitem = ""
    LINE_START = 125
    LINE_END = 1448
    
    with open("src/objects.c", "r") as a_file:
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
                if t in sline:
                    print("Record {}".format(i))
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
                    items.append(newdat)
                    
                    print("Stop {}".format(i))
                    print(newdat)
                    
                    indata = False
                    newitem = ""
                    otyp = None

    print("")
    print("processing ... ")
    for i in items:
        i = clean(i)
        print(i)
        
        
if __name__ == "__main__":
    main()