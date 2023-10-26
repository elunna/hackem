import json

# [x] Sort by name
# [x] Sort by damage vs small
# [x] Sort by damage vs large
# Sort by damage vs weight ratio?

# Add materials
# Add role skill maximums and effects on 
# Add firearms ROF/rate
# Classify projectiles and launchers


fields = [
    "name", "skill", "vs_sm", "vs_lg", "tohit", "weight", "bimanual"   
]


def filter_skill(data, skill):
    return [i for i in data if i['skill'] == skill]


def print_data(data):
    print('| {:21} | {:18} | {:6} | {:6} | {:2} |'.format(
        "weapon", "skill", "vs sm", "vs lg", "bi"))

    for i in data:
        print('| {:21} | {:18} | {:.2f} | {:.2f} | {:2} |'.format(
            i['name'],
            i['skill'],
            avg_dmg(i['vs_sm']),
            avg_dmg(i['vs_lg']),
            i['bimanual'],
        ))


def min_dmg(dice):
    return len(dice)


def avg_dmg(dice):
    total = 0
    for d in dice:
        total += (sum(i for i in range(d + 1)) / d)
    return total


def main():
    print("Hack'EM weapons spoiler 1.0")
    
    skills = []
    bimanual = []
    launchers = []
    projectiles = []
    
    with open('weapons.json') as f:
        data = json.load(f)
        #data = sorted(data['weapons'], key=lambda x: x['name'])
        data = sorted(data['weapons'], key=lambda x: avg_dmg(x['vs_lg']))

        for i in data:
            if i['skill'] not in skills:
                skills.append(i['skill'])
            
        #data = filter_skill(data, "P_DAGGER")
    
        print_data(data)


if __name__ == "__main__":
    main()