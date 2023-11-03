/* NetHack 3.6	shknam.c	$NHDT-Date: 1454485432 2016/02/03 07:43:52 $  $NHDT-Branch: NetHack-3.6.0 $:$NHDT-Revision: 1.41 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* NetHack may be freely redistributed.  See license for details. */

/* shknam.c -- initialize a shop */

#include "hack.h"

STATIC_DCL boolean FDECL(stock_room_goodpos, (struct mkroom *, int, int, int, int));
STATIC_DCL boolean FDECL(veggy_item, (struct obj * obj, int));
STATIC_DCL int NDECL(shkveg);
STATIC_DCL void FDECL(mkveggy_at, (int, int));
STATIC_DCL void FDECL(mkshobj_at, (const struct shclass *, int, int,
                                   BOOLEAN_P));
STATIC_DCL void FDECL(nameshk, (struct monst *, const char *const *));
STATIC_DCL int FDECL(shkinit, (const struct shclass *, struct mkroom *, int));
STATIC_DCL void FDECL(stock_blkmar,
                      (const struct shclass *, struct mkroom *, int));
static void FDECL(init_shk_services, (struct monst *));
static void FDECL(maybe_add_svc, (struct monst *, int));
#define VEGETARIAN_CLASS (MAXOCLASSES + 1)

/*
 *  Name prefix codes:
 *      dash          -  female, personal name
 *      underscore    _  female, general name
 *      plus          +  male, personal name
 *      vertical bar  |  male, general name (implied for most of shktools)
 *      equals        =  gender not specified, personal name
 *
 *  Personal names do not receive the honorific prefix "Mr." or "Ms.".
 */

static const char *const shkliquors[] = {
    /* Ukraine */
    "Njezjin", "Tsjernigof", "Ossipewsk", "Gorlowka",
    /* Belarus */
    "Gomel",
    /* N. Russia */
    "Konosja", "Weliki Oestjoeg", "Syktywkar", "Sablja", "Narodnaja", "Kyzyl",
    /* Silezie */
    "Walbrzych", "Swidnica", "Klodzko", "Raciborz", "Gliwice", "Brzeg",
    "Krnov", "Hradec Kralove",
    /* Schweiz */
    "Leuk", "Brig", "Brienz", "Thun", "Sarnen", "Burglen", "Elm", "Flims",
    "Vals", "Schuls", "Zum Loch", 0
};

static const char *const shkbooks[] = {
    /* Eire */
    "Skibbereen",  "Kanturk",   "Rath Luirc",     "Ennistymon",
    "Lahinch",     "Kinnegad",  "Lugnaquillia",   "Enniscorthy",
    "Gweebarra",   "Kittamagh", "Nenagh",         "Sneem",
    "Ballingeary", "Kilgarvan", "Cahersiveen",    "Glenbeigh",
    "Kilmihil",    "Kiltamagh", "Droichead Atha", "Inniscrone",
    "Clonegal",    "Lisnaskea", "Culdaff",        "Dunfanaghy",
    "Inishbofin",  "Kesh",      0
};

static const char *const shkarmors[] = {
    /* Turquie */
    "Demirci",    "Kalecik",    "Boyabai",    "Yildizeli", "Gaziantep",
    "Siirt",      "Akhalataki", "Tirebolu",   "Aksaray",   "Ermenak",
    "Iskenderun", "Kadirli",    "Siverek",    "Pervari",   "Malasgirt",
    "Bayburt",    "Ayancik",    "Zonguldak",  "Balya",     "Tefenni",
    "Artvin",     "Kars",       "Makharadze", "Malazgirt", "Midyat",
    "Birecik",    "Kirikkale",  "Alaca",      "Polatli",   "Nallihan",
    0
};

static const char *const shkwands[] = {
    /* Wales */
    "Yr Wyddgrug", "Trallwng", "Mallwyd", "Pontarfynach", "Rhaeader",
    "Llandrindod", "Llanfair-ym-muallt", "Y-Fenni", "Maesteg", "Rhydaman",
    "Beddgelert", "Curig", "Llanrwst", "Llanerchymedd", "Caergybi",
    /* Scotland */
    "Nairn", "Turriff", "Inverurie", "Braemar", "Lochnagar", "Kerloch",
    "Beinn a Ghlo", "Drumnadrochit", "Morven", "Uist", "Storr",
    "Sgurr na Ciche", "Cannich", "Gairloch", "Kyleakin", "Dunvegan", 0
};

static const char *const shkrings[] = {
    /* Hollandse familienamen */
    "Feyfer",     "Flugi",         "Gheel",      "Havic",   "Haynin",
    "Hoboken",    "Imbyze",        "Juyn",       "Kinsky",  "Massis",
    "Matray",     "Moy",           "Olycan",     "Sadelin", "Svaving",
    "Tapper",     "Terwen",        "Wirix",      "Ypey",
    /* Skandinaviske navne */
    "Rastegaisa", "Varjag Njarga", "Kautekeino", "Abisko",  "Enontekis",
    "Rovaniemi",  "Avasaksa",      "Haparanda",  "Lulea",   "Gellivare",
    "Oeloe",      "Kajaani",       "Fauske",     0
};

static const char *const shkfoods[] = {
    /* Indonesia */
    "Djasinga",    "Tjibarusa",   "Tjiwidej",      "Pengalengan",
    "Bandjar",     "Parbalingga", "Bojolali",      "Sarangan",
    "Ngebel",      "Djombang",    "Ardjawinangun", "Berbek",
    "Papar",       "Baliga",      "Tjisolok",      "Siboga",
    "Banjoewangi", "Trenggalek",  "Karangkobar",   "Njalindoeng",
    "Pasawahan",   "Pameunpeuk",  "Patjitan",      "Kediri",
    "Pemboeang",   "Tringanoe",   "Makin",         "Tipor",
    "Semai",       "Berhala",     "Tegal",         "Samoe",
    0
};

static const char *const shkweapons[] = {
    /* Perigord */
    "Voulgezac",   "Rouffiac",   "Lerignac",   "Touverac",  "Guizengeard",
    "Melac",       "Neuvicq",    "Vanzac",     "Picq",      "Urignac",
    "Corignac",    "Fleac",      "Lonzac",     "Vergt",     "Queyssac",
    "Liorac",      "Echourgnac", "Cazelon",    "Eypau",     "Carignan",
    "Monbazillac", "Jonzac",     "Pons",       "Jumilhac",  "Fenouilledes",
    "Laguiolet",   "Saujon",     "Eymoutiers", "Eygurande", "Eauze",
    "Labouheyre",  0
};

static const char *const shktools[] = {
    /* Spmi */
    "Ymla", "Eed-morra", "Elan Lapinski", "Cubask", "Nieb", "Bnowr Falr",
    "Sperc", "Noskcirdneh", "Yawolloh", "Hyeghu", "Niskal", "Trahnil",
    "Htargcm", "Enrobwem", "Kachzi Rellim", "Regien", "Donmyar", "Yelpur",
    "Nosnehpets", "Stewe", "Renrut", "Senna Hut", "-Zlaw", "Nosalnef",
    "Rewuorb", "Rellenk", "Yad", "Cire Htims", "Y-crad", "Nenilukah",
    "Corsh", "Aned", "Dark Eery", "Niknar", "Lapu", "Lechaim",
    "Rebrol-nek", "AlliWar Wickson", "Oguhmk", "Telloc Cyaj",
#ifdef OVERLAY
    "Erreip", "Nehpets", "Mron", "Snivek", "Kahztiy",
#endif
#ifdef WIN32
    "Lexa", "Niod",
#endif
#ifdef MAC
    "Nhoj-lee", "Evad\'kh", "Ettaw-noj", "Tsew-mot", "Ydna-s", "Yao-hang",
    "Tonbar", "Kivenhoug", "Llardom",
#endif
#ifdef AMIGA
    "Falo", "Nosid-da\'r", "Ekim-p", "Noslo", "Yl-rednow", "Mured-oog",
    "Ivrajimsal",
#endif
#ifdef TOS
    "Nivram",
#endif
#ifdef OS2
    "Nedraawi-nav",
#endif
#ifdef VMS
    "Lez-tneg", "Ytnu-haled",
#endif
    0
};

static const char *const shklight[] = {
    /* Romania */
    "Zarnesti", "Slanic", "Nehoiasu", "Ludus", "Sighisoara", "Nisipitu",
    "Razboieni", "Bicaz", "Dorohoi", "Vaslui", "Fetesti", "Tirgu Neamt",
    "Babadag", "Zimnicea", "Zlatna", "Jiu", "Eforie", "Mamaia",
    /* Bulgaria */
    "Silistra", "Tulovo", "Panagyuritshte", "Smolyan", "Kirklareli", "Pernik",
    "Lom", "Haskovo", "Dobrinishte", "Varvara", "Oryahovo", "Troyan",
    "Lovech", "Sliven", 0
};

static const char *const shkgeneral[] = {
    /* Suriname */
    "Hebiwerie",    "Possogroenoe", "Asidonhopo",   "Manlobbi",
    "Adjama",       "Pakka Pakka",  "Kabalebo",     "Wonotobo",
    "Akalapi",      "Sipaliwini",
    /* Greenland */
    "Annootok",     "Upernavik",    "Angmagssalik",
    /* N. Canada */
    "Aklavik",      "Inuvik",       "Tuktoyaktuk",  "Chicoutimi",
    "Ouiatchouane", "Chibougamau",  "Matagami",     "Kipawa",
    "Kinojevis",    "Abitibi",      "Maganasipi",
    /* Iceland */
    "Akureyri",     "Kopasker",     "Budereyri",    "Akranes",
    "Bordeyri",     "Holmavik",     0
};

static const char *const shkhealthfoods[] = {
    /* Tibet */
    "Ga'er",    "Zhangmu",   "Rikaze",   "Jiangji",     "Changdu",
    "Linzhi",   "Shigatse",  "Gyantse",  "Ganden",      "Tsurphu",
    "Lhasa",    "Tsedong",   "Drepung",
    /* Hippie names */
    "=Azura",   "=Blaze",    "=Breanna", "=Breezy",     "=Dharma",
    "=Feather", "=Jasmine",  "=Luna",    "=Melody",     "=Moonjava",
    "=Petal",   "=Rhiannon", "=Starla",  "=Tranquilla", "=Windsong",
    "=Zennia",  "=Zoe",      "=Zora",    0
};


static const char *const shkmusic[] = {
    "John", "Paul", "George", "Ringo",
    "Elvis", "Mick", "Keith", "Ron", "Charlie",
    "Joseph", "Franz", "Richard", "Ludwig", "Wolfgang Amadeus",
    "Johann Sebastian",
    "Karlheinz", "Gyorgy",
    "Luciano", "Placido", "Jose", "Enrico",
    "Falco", "_Britney", "_Christina", "_Toni", "_Brandy",
    0
};

static const char *const shkguns[] = {
    /* GTA 3 */
    "Claude", "_Catalina", "Eight-Ball", "Salvatore",
    /* Vice City */
    "Tommy", "Ken", "Lance", "Sonny", "Ricardo",
    /* San Andreas */
    "Carl", "Sean", "Melvin", "Ryder",
    /* 4 */
    "Niko", "Dimitri", "Mikhail", "Vlad",
    /* Chinatown Wars */
    "Huang", "Wu",
    /* GTAV */
    "Michael", "Franklin", "Trevor", "Steve",
    "Weston",
    0
};
static const char *const shkpet[] = {
    /* Albania */
    "Elbasan", "Vlore", "Shkoder", "Berat", "Kavaje", "Pogradec",
    "Sarande", "Peshkopi", "Shijak", "Librazhd", "Tepelene",
    "Fushe-Kruje", "Rreshen",
    0
};

static const char *const shktins[] = {
    /* Sweden */
    "Trosa", "Torshalla", "Morgongava", "Uppsala", "Norrkoping",
    "Nybro", "Alingsas", "Vadstena", "Fagersta", "Skelleftea",
    "Solleftea", "Ystad", "Avesta", "Sala", "Norrtalje",
    0
};

static const char *const shkarchery[] = {
    /* Famous Archers, perhaps? */
    "Robin",      "+William",  "Paris",  "Skadi",
    "Cupid",      "Ullr",      "-Artemis",
    "-Meduka",
    /* Elven names from Tolkien's canon. */
    "+Teleporno", "+Feano", 0
};

static const char *const shkmasks[] = {
    "Happy", 0
};

static const char *const shkjunk[] = {
    /* Silly names, clown names */
    "=Spiffy", "=Bonko", "=Binky", "=Tubby", "=Zippy", "=Jumbo",
    "=Mittens", "=Chuckles", "=Bam Bam", "=Larry", "=Curly",
    "=Moe", "=Zaff", "=Punky", "=Zonko", 0
};

/*
 * To add new shop types, all that is necessary is to edit the shtypes[]
 * array.  See mkroom.h for the structure definition.  Typically, you'll
 * have to lower some or all of the probability fields in old entries to
 * free up some percentage for the new type.
 *
 * The placement type field is not yet used but will be in the near future.
 *
 * The iprobs array in each entry defines the probabilities for various kinds
 * of objects to be present in the given shop type.  You can associate with
 * each percentage either a generic object type (represented by one of the
 * *_CLASS macros) or a specific object (represented by an onames.h define).
 * In the latter case, prepend it with a unary minus so the code can know
 * (by testing the sign) whether to use mkobj() or mksobj().
 */
const struct shclass shtypes[] = {
    { "general store", RANDOM_CLASS, 25, D_SHOP,
      { { 100, RANDOM_CLASS },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 } },
      shkgeneral },
    { "used armor dealership", ARMOR_CLASS, 8, D_SHOP,
      { { 90, ARMOR_CLASS },
        { 10, WEAPON_CLASS },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 } },
      shkarmors },
    { "second-hand bookstore", SCROLL_CLASS, 8, D_SHOP,
      { { 90, SCROLL_CLASS },
        { 10, SPBOOK_CLASS },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 } },
      shkbooks },
    { "liquor emporium", POTION_CLASS, 8, D_SHOP,
      { { 100, POTION_CLASS },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 } },
      shkliquors },
    { "antique weapons outlet", WEAPON_CLASS, 8, D_SHOP,
      { { 90, WEAPON_CLASS },
        { 10, ARMOR_CLASS },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 } },
      shkweapons },
    { "delicatessen", FOOD_CLASS, 3, D_SHOP,
      { { 80, FOOD_CLASS },
        { 4, -POT_FRUIT_JUICE },
        { 4, -POT_BOOZE },
        { 4, -POT_WATER },
        { 2, -ICE_BOX },
        { 2, -KEG },
        { 2, -LUMP_OF_ROYAL_JELLY },
        { 1, -MEAT_STICK },
        { 1, -HUGE_CHUNK_OF_MEAT },
        { 0, 0 } },
      shkfoods },
    { "jewelers", RING_CLASS, 5, D_SHOP,
      { { 85, RING_CLASS },
        { 10, GEM_CLASS },
        { 5, AMULET_CLASS },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 } },
      shkrings },
    { "quality apparel and accessories", WAND_CLASS, 5, D_SHOP,
      { { 90, WAND_CLASS },
        { 5, -GLOVES },
        { 5, -ELVEN_CLOAK },
        { 0, 0 } },
      shkwands },
    { "hardware store", TOOL_CLASS, 5, D_SHOP,
      { { 95, TOOL_CLASS },
        { 2, -TOUCHSTONE },
        { 3, -POT_OIL },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 } },
      shktools },
    { "rare books", SPBOOK_CLASS, 5, D_SHOP,
      { { 90, SPBOOK_CLASS },
        { 10, SCROLL_CLASS },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 } },
      shkbooks },
    { "archery emporium", WEAPON_CLASS, 3, D_SHOP,
      { { 15, -BOW },
        { 30, -ELVEN_ARROW },
        { 15, -ELVEN_BOW },
        { 15, -LIGHT_ARROW },
        { 5, -CROSSBOW },
        { 15, -CROSSBOW_BOLT },
        { 5, WEAPON_CLASS } },
      shkarchery },
    { "mask shop", RANDOM_CLASS, 3, D_SHOP,
      { { 95, -MASK },
        { 2, -LENSES },
        { 1, -BLINDFOLD },
        { 2, -GOGGLES },
        { 0, 0} },
      shkmasks },
    { "junk shop", RANDOM_CLASS, 5, D_SHOP,
      { { 30, RANDOM_CLASS },
        { 5, -BASEBALL_BAT },
        { 5, -WAN_WONDER },
        { 5, -FOOTBOW },
        { 5, -WAN_NOTHING },
        { 5, -FLY_SWATTER },
        { 5, -RUBBER_HOSE },
        { 5, -BAG_OF_RATS },
        { 5, -BAGPIPE },
        { 5, -IRON_CHAIN },
        { 5, -TIN_OPENER },
        { 5, -EIGHT_BALL },
        { 5, -SCR_BLANK_PAPER },
        { 5, -SPOON },
        { 4, -FISHING_POLE },
        { 1, -RIN_MOOD } },
        shkjunk },
    { "health food store", FOOD_CLASS, 1, D_SHOP,
        { { 70, VEGETARIAN_CLASS },
          { 20, -POT_FRUIT_JUICE },
          { 4, -POT_HEALING },
          { 3, -POT_FULL_HEALING },
          { 2, -SCR_FOOD_DETECTION },
          { 1, -LUMP_OF_ROYAL_JELLY } },
      shkhealthfoods },
    {"canned food factory", FOOD_CLASS, 1, D_SHOP,
      { {10, -ICE_BOX},         
        {90, -TIN},
        /* shopkeeper will pay for corpses, but they aren't generated */
        /* on the shop floor */
        {0, -CORPSE},
        {0, 0} },
      shktins },
    {"gun store", WEAPON_CLASS, 3, D_SHOP,
      { { 20, -PISTOL },
        { 5, -SUBMACHINE_GUN },
        { 5, -HEAVY_MACHINE_GUN },
        { 5, -SHOTGUN },
        { 5, -AUTO_SHOTGUN },
        { 4, -RIFLE },
        { 3, -ASSAULT_RIFLE },
        { 3, -SNIPER_RIFLE },
        { 3, -FLINTLOCK },
        /* ammo */
        { 25, -BULLET },
        { 13, -SHOTGUN_SHELL },
        { 2, -FIRE_BOMB },
        { 2, -GAS_BOMB },
        { 2, -SONIC_BOMB },
        { 1, -CAN_OF_GREASE },
        { 1, -POT_OIL },
        { 1, -TOWEL },
        { 0, 0} },
      shkguns},
    {"rare instruments", TOOL_CLASS, 2, D_SHOP,
      { { 8, -PEA_WHISTLE },
        { 8, -FLUTE },
        { 8, -TOOLED_HORN },
        { 8, -HARP },
        { 8, -BELL },         
        { 8, -BUGLE },
        { 8, -LEATHER_DRUM },
        { 5, -LOCK_PICK },
        { 5, -LUTE },          
        { 5, -BAGPIPE },
        { 5, -HORN_OF_BLASTING },
        { 4, -T_SHIRT },
        { 3, -MAGIC_WHISTLE },
        { 3, -MAGIC_FLUTE },
        { 3, -FROST_HORN },
        { 3, -FIRE_HORN }, 
        { 3, -HORN_OF_PLENTY },
        { 3, -MAGIC_HARP },
        { 2, -DRUM_OF_EARTHQUAKE },
        { 0, 0} },
      shkmusic },
    {"pet store", FOOD_CLASS, 2, D_SHOP,
      { { 67, -FIGURINE },        
        { 5, -LEASH },
        { 10, -TRIPE_RATION },    
        { 5, -SADDLE },
        { 3, -BARDING },
        { 1, -BARDING_OF_REFLECTION },
        { 1, -SPIKED_BARDING },
        { 5, -PEA_WHISTLE },
        { 3, -MAGIC_WHISTLE } },
      shkpet },
    /* Shops below this point are "unique".  That is they must all have a
     * probability of zero.  They are only created via the special level
     * loader.
     */
    { "lighting store", TOOL_CLASS, 0, D_SHOP,
      { { 27, -WAX_CANDLE },
        { 35, -TALLOW_CANDLE },
        { 5, -TORCH },
        { 5, -LANTERN },
        { 10, -OIL_LAMP },
        { 3, -MAGIC_LAMP },
        { 5, -MAGIC_CANDLE },
        { 5, -POT_OIL },
        { 1, -SCR_LIGHT },
        { 1, -SPE_LIGHT },
        { 2, -WAN_LIGHT }, 
        { 1, -WAN_LIGHTNING }
      },
      shklight },
    { "black market", RANDOM_CLASS, 0, D_SHOP,
      { { 100, RANDOM_CLASS }, 
        { 0, 0 },
        { 0, 0 }, 
        { 0, 0 }, 
        { 0, 0 }, 
        { 0, 0 } }, 
      0 },
    /* sentinel */
    { (char *) 0,
      0,
      0,
      0,
      { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
      0 }
};

#if 0
/* validate shop probabilities; otherwise incorrect local changes could
   end up provoking infinite loops or wild subscripts fetching garbage */
void
init_shop_selection()
{
    register int i, j, item_prob, shop_prob;

    for (shop_prob = 0, i = 0; i < SIZE(shtypes); i++) {
        shop_prob += shtypes[i].prob;
        for (item_prob = 0, j = 0; j < SIZE(shtypes[0].iprobs); j++)
            item_prob += shtypes[i].iprobs[j].iprob;
        if (item_prob != 100)
            panic("item probabilities total to %d for %s shops!",
                  item_prob, shtypes[i].name);
    }
    if (shop_prob != 100)
        panic("shop probabilities total to %d!", shop_prob);
}
#endif /*0*/

/* decide whether an object or object type is considered vegetarian;
   for types, items which might go either way are assumed to be veggy */
STATIC_OVL boolean
veggy_item(obj, otyp)
struct obj *obj;
int otyp; /* used iff obj is null */
{
    int corpsenm;
    char oclass;

    if (obj) {
        /* actual object; will check tin content and corpse species */
        otyp = (int) obj->otyp;
        oclass = obj->oclass;
        corpsenm = obj->corpsenm;
    } else {
        /* just a type; caller will have to handle tins and corpses */
        oclass = objects[otyp].oc_class;
        corpsenm = PM_LICHEN; /* veggy standin */
    }

    if (oclass == FOOD_CLASS) {
        if (objects[otyp].oc_material == VEGGY || otyp == EGG)
            return TRUE;
        if (otyp == TIN && corpsenm == NON_PM) /* implies obj is non-null */
            return (boolean) (obj->spe == 1); /* 0 = empty, 1 = spinach */
        if (otyp == TIN || otyp == CORPSE)
            return (boolean) (corpsenm >= LOW_PM
                              && vegetarian(&mons[corpsenm]));
    }
    return FALSE;
}

STATIC_OVL int
shkveg()
{
    int i, j, maxprob, prob;
    char oclass = FOOD_CLASS;
    int ok[NUM_OBJECTS];

    j = maxprob = 0;
    ok[0] = 0; /* lint suppression */
    for (i = bases[(int) oclass]; i < NUM_OBJECTS; ++i) {
        if (objects[i].oc_class != oclass)
            break;

        if (veggy_item((struct obj *) 0, i)) {
            ok[j++] = i;
            maxprob += objects[i].oc_prob;
        }
    }
    if (maxprob < 1)
        panic("shkveg no veggy objects");
    prob = rnd(maxprob);

    j = 0;
    i = ok[0];
    while ((prob -= objects[i].oc_prob) > 0) {
        j++;
        i = ok[j];
    }

    if (objects[i].oc_class != oclass || !OBJ_NAME(objects[i]))
        panic("shkveg probtype error, oclass=%d i=%d", (int) oclass, i);
    return i;
}

/* make a random item for health food store */
STATIC_OVL void
mkveggy_at(sx, sy)
int sx, sy;
{
    struct obj *obj = mksobj_at(shkveg(), sx, sy, TRUE, TRUE);

    if (obj && obj->otyp == TIN)
        set_tin_variety(obj, HEALTHY_TIN);
    return;
}

/* make an object of the appropriate type for a shop square */
STATIC_OVL void
mkshobj_at(shp, sx, sy, mkspecl)
const struct shclass *shp;
int sx, sy;
boolean mkspecl;
{
    struct monst *mtmp;
    struct permonst *ptr;
    int atype;

    /* 3.6 tribute */
    if (mkspecl && (!strcmp(shp->name, "rare books")
                    || !strcmp(shp->name, "second-hand bookstore"))) {
        struct obj *novel = mksobj_at(SPE_NOVEL, sx, sy, FALSE, FALSE);

        if (novel)
            context.tribute.bookstock = TRUE;
        return;
    }

    if (!Is_blackmarket(&u.uz) && rn2(100) < depth(&u.uz) 
        && !MON_AT(sx, sy)
        && (ptr = mkclass(S_MIMIC, 0)) != 0
        && (mtmp = makemon(ptr, sx, sy, NO_MM_FLAGS)) != 0) {
        /* note: makemon will set the mimic symbol to a shop item */
        if (rn2(10) >= depth(&u.uz)) {
            mtmp->m_ap_type = M_AP_OBJECT;
            mtmp->mappearance = STRANGE_OBJECT;
        }
        return;
    } 
    /* black market shops should be a little messy */
    if (Is_blackmarket(&u.uz)) {
        if (!rn2(20)) 
            return; /* leave an empty square */
        if (!rn2(10)) {
            (void) mkobj_at(rn2(5) ? GEM_CLASS : TOOL_CLASS, sx, sy, TRUE);
            return;
        }
    }
    
    atype = get_shop_item((int) (shp - shtypes));
    if (atype == VEGETARIAN_CLASS)
        mkveggy_at(sx, sy);
    else if (atype < 0)
        (void) mksobj_at(-atype, sx, sy, TRUE, TRUE);
    else
        (void) mkobj_at(atype, sx, sy, TRUE);

}

/* extract a shopkeeper name for the given shop type */
STATIC_OVL void
nameshk(shk, nlp)
struct monst *shk;
const char *const *nlp;
{
    int i, trycnt, names_avail;
    const char *shname = 0;
    struct monst *mtmp;
    int name_wanted;
    s_level *sptr;


    if (nlp == shkfoods && In_mines(&u.uz) && Role_if(PM_MONK)
        && (sptr = Is_special(&u.uz)) != 0 && sptr->flags.town) {
        /* special-case override for minetown food store for monks */
        nlp = shkhealthfoods;
        ESHK(shk)->shoptype = FODDERSHOP;
    } 

    if (!nlp) {
        shname = m_monnam(shk);
    } else if (nlp == shklight
               && In_mines(&u.uz) 
               && (sptr = Is_special(&u.uz)) != 0 
               && sptr->flags.town) {
        /* special-case minetown lighting shk */
        shname = "+Izchak";
        shk->female = FALSE;
    } else {
        /* We want variation from game to game, without needing the save
           and restore support which would be necessary for randomization;
           try not to make too many assumptions about time_t's internals;
           use ledger_no rather than depth to keep mine town distinct. */
        int nseed = (int) ((long) ubirthday / 257L) + sysopt.serverseed;

        name_wanted = ledger_no(&u.uz) + (nseed % 13) - (nseed % 5);
        if (name_wanted < 0)
            name_wanted += (13 + 5);
        shk->female = name_wanted & 1;

        for (names_avail = 0; nlp[names_avail]; names_avail++)
            continue;

        for (trycnt = 0; trycnt < 50; trycnt++) {
            if (nlp == shktools) {
                shname = shktools[rn2(names_avail)];
                shk->female = 0; /* reversed below for '_' prefix */
            } else if (nlp == shkmusic) {
                shname = shkmusic[rn2(names_avail)];
            } else if (name_wanted < names_avail) {
                shname = nlp[name_wanted];
            } else if ((i = rn2(names_avail)) != 0) {
                shname = nlp[i - 1];
            } else if (nlp != shkgeneral) {
                nlp = shkgeneral; /* try general names */
                for (names_avail = 0; nlp[names_avail]; names_avail++)
                    continue;
                continue; /* next `trycnt' iteration */
            } else {
                shname = shk->female ? "-Lucrezia" : "+Dirk";
            }
            if (*shname == '_' || *shname == '-' || is_female(r_data(shk)))
                shk->female = 1;
            if (*shname == '|' || *shname == '+' || is_male(r_data(shk)))
                shk->female = 0;

            /* is name already in use on this level? */
            for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
                if (DEADMONSTER(mtmp) || (mtmp == shk) || !mtmp->isshk)
                    continue;
                if (strcmp(ESHK(mtmp)->shknam, shname))
                    continue;
                break;
            }
            if (!mtmp)
                break; /* new name */
        }
    }

    if (Is_blackmarket(&u.uz) && shk->data != &mons[PM_ONE_EYED_SAM]) {
        int num_prefixes;
        const char *prefix = 0;
        /* One-eyed Sam's lackeys idolize her and took nicknames like hers */
        static const char *prefixes[] = {
            "One-armed", "Two-faced", "Three-fingered", "Cross-eyed",
            "Four-toed", "Iron-lunged", "Two-footed", "One-handed",
            "One-legged", "Barefoot", "Cold-blooded", "Cut-throat",
            "Evil-eyed", "Scar-faced", "Five-toothed", "Four-limbed",
            "Eight-fingered", "Color-blind", "Iron-bellied",
            "Silver-tongued", "Crazy-eyed", "Hot-blooded",
            "Green-skinned", "Sharp-eyed", "Hard-nosed", "Dog-eared",
            "Peg-legged", "Knock-knee", "Hook-handed", "One-nostril",
            "Double-chinned", "Double-jointed", "Nine-lives",
            0
        };
        for (num_prefixes = 0; prefixes[num_prefixes]; num_prefixes++)
            continue;

        for (trycnt = 0; trycnt < 50; trycnt++) {
            prefix = prefixes[rn2(num_prefixes)];
            /* is this prefix already used on this level? */
            for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
                if (DEADMONSTER(mtmp) || (mtmp == shk) || !mtmp->isshk) 
                    continue;
                if (!strstr(ESHK(mtmp)->shknam, prefix)) 
                    continue;
                break;
            }
            if (!mtmp) 
                break; /* found an unused prefix */
        }
        snprintf(ESHK(shk)->shknam, PL_NSIZ, "%s %s", prefix, shname);
    } else {
        (void) strncpy(ESHK(shk)->shknam, shname, PL_NSIZ);
    }
    /*(void) strncpy(ESHK(shk)->shknam, shname, PL_NSIZ);*/
    ESHK(shk)->shknam[PL_NSIZ - 1] = 0;
}

void
neweshk(mtmp)
struct monst *mtmp;
{
    if (!mtmp->mextra)
        mtmp->mextra = newmextra();
    if (!ESHK(mtmp))
        ESHK(mtmp) = (struct eshk *) alloc(sizeof(struct eshk));
    (void) memset((genericptr_t) ESHK(mtmp), 0, sizeof(struct eshk));
    ESHK(mtmp)->bill_p = (struct bill_x *) 0;
}

void
free_eshk(mtmp)
struct monst *mtmp;
{
    if (mtmp->mextra && ESHK(mtmp)) {
        free((genericptr_t) ESHK(mtmp));
        ESHK(mtmp) = (struct eshk *) 0;
    }
    mtmp->isshk = 0;
}

/* create a new shopkeeper in the given room */
STATIC_OVL int
shkinit(shp, sroom, shp_indx)
const struct shclass *shp;
struct mkroom *sroom;
int shp_indx;
{
    register int sh, sx, sy;
    int srace = 0;
    struct monst *shk;
    struct eshk *eshkp;
    long shkmoney; /* Temporary placeholder for Shopkeeper's initial capital */


    /* place the shopkeeper in the given room */
    sh = sroom->fdoor;
    sx = doors[sh].x;
    sy = doors[sh].y;

    /* check that the shopkeeper placement is sane */
    if (sroom->irregular) {
        int rmno = (int) ((sroom - rooms) + ROOMOFFSET);

        if (isok(sx - 1, sy) && !levl[sx - 1][sy].edge
            && (int) levl[sx - 1][sy].roomno == rmno)
            sx--;
        else if (isok(sx + 1, sy) && !levl[sx + 1][sy].edge
                 && (int) levl[sx + 1][sy].roomno == rmno)
            sx++;
        else if (isok(sx, sy - 1) && !levl[sx][sy - 1].edge
                 && (int) levl[sx][sy - 1].roomno == rmno)
            sy--;
        else if (isok(sx, sy + 1) && !levl[sx][sy + 1].edge
                 && (int) levl[sx][sy + 1].roomno == rmno)
            sy++;
        else
            goto shk_failed;
    } else if (sx == sroom->lx - 1) {
        sx++;
    } else if (sx == sroom->hx + 1) {
        sx--;
    } else if (sy == sroom->ly - 1) {
        sy++;
    } else if (sy == sroom->hy + 1) {
        sy--;
    } else {
    shk_failed:
#ifdef DEBUG
        /* Said to happen sometimes, but I have never seen it. */
        /* Supposedly fixed by fdoor change in mklev.c */
        if (wizard) {
            register int j = sroom->doorct;

            pline("Where is shopdoor?");
            pline("Room at (%d,%d),(%d,%d).", sroom->lx, sroom->ly, sroom->hx,
                  sroom->hy);
            pline("doormax=%d doorct=%d fdoor=%d", doorindex, sroom->doorct,
                  sh);
            while (j--) {
                pline("door [%d,%d]", doors[sh].x, doors[sh].y);
                sh++;
            }
            display_nhwindow(WIN_MESSAGE, FALSE);
        }
#endif
        return -1;
    }

    if (MON_AT(sx, sy))
        (void) rloc(m_at(sx, sy), FALSE); /* insurance */

    /* now initialize the shopkeeper monster structure */
    shk = 0;
    if (Is_blackmarket(&u.uz)) {
        if (sroom->rtype == BLACKSHOP) {
            shk = makemon(&mons[PM_ONE_EYED_SAM], sx, sy, MM_ESHK);
        }
    }
    if (!shk) {
        if (!(shk = makemon(&mons[PM_SHOPKEEPER], sx, sy, MM_ESHK))) {
            return -1;
        }
    }

    eshkp = ESHK(shk); /* makemon(...,MM_ESHK) allocates this */
    /* change the shopkeeper to a particular race */
    switch (shtypes[shp_indx].symb) {
	/* armors and weaponry are similar... */
    case ARMOR_CLASS:
    case WEAPON_CLASS:
        switch (rnd(4)) {
        case 1: srace = PM_HUMAN; break;
        case 2: srace = PM_GIANT; break;
        case 3: srace = PM_CENTAUR; break;
        case 4: srace = PM_DWARF; break;
        }
        break;
    case FOOD_CLASS:
        switch (rnd(3)) {
        case 1: srace = PM_DWARF; break;
        case 2: srace = PM_GNOME; break;
        case 3: srace = PM_HUMAN; break;
        }
        break;
    case RING_CLASS:
        srace = PM_NYMPH;
        break;
    /* wands and potions also similar... */
    case WAND_CLASS:
    case POTION_CLASS:
        switch (rnd(8)) {
        case 1: srace = PM_ELF; break;
        case 2: srace = PM_GNOME; break;
        case 3: srace = PM_NYMPH; break;
        case 4: srace = PM_HUMAN; break;
        default:  srace = PM_VAMPIRIC; break;
        }
        break;
    case TOOL_CLASS:
        if (shp->shknms == shklight && In_mines(&u.uz)) {
            /* Izchak is always human.
             * At this point we haven't actually named the shk yet, so we
             * just check if we're a lighting store in the mines. This
             * doesn't actually check to see if we're in Minetown, so
             * this technically makes any lighting store in the Mines have a
             * human shopkeeper. */
            srace = PM_HUMAN;
        } else {
            srace = PM_GNOME;
        }
        break;
    /* and scrolls and books fall to spellcasters */
    case SCROLL_CLASS:
    case SPBOOK_CLASS:
        switch (rnd(5)) {
        case 1: srace = PM_ILLITHID; break;
        case 2: srace = PM_HUMAN; break;
        case 3: srace = PM_GNOME; break;
        case 4: srace = PM_ELF; break;
        case 5: srace = PM_VAMPIRIC; break;
        }
        break;
    default:
        break;
    }

    if (!srace) {
        srace = m_randrace(monsndx(shk->data));
    }
    apply_race(shk, srace);

    shk->isshk = shk->mpeaceful = 1;
    set_malign(shk);
    shk->data->msound = MS_SELL;
    shk->msleeping = 0;
    shk->mtrapseen = ~0; /* we know all the traps already */
    eshkp->shoproom = (schar) ((sroom - rooms) + ROOMOFFSET);
    sroom->resident = shk;
    eshkp->shoptype = sroom->rtype;
    assign_level(&eshkp->shoplevel, &u.uz);
    eshkp->shd = doors[sh];
    eshkp->shk.x = sx;
    eshkp->shk.y = sy;
    eshkp->robbed = eshkp->credit = eshkp->debit = eshkp->loan = 0L;
    eshkp->following = eshkp->surcharge = eshkp->dismiss_kops = FALSE;
    eshkp->billct = eshkp->visitct = 0;
    eshkp->bill_p = (struct bill_x *) 0;
    eshkp->customer[0] = '\0';
    /* WAC init services */
    init_shk_services(shk);
    shkmoney = 2250L + 65L * (long) rnd(100);  /* initial capital */
    /* [CWC] Lets not create the money yet until we see if the
         shk is a black marketeer, else we'll have to create
       another money object, if GOLDOBJ is defined */

    if (shp->shknms == shkrings)
        (void) mongets(shk, TOUCHSTONE);
    nameshk(shk, shp->shknms);
    /* might have changed delicatessen to health food store */
    sroom->rtype = eshkp->shoptype;

    if (Is_blackmarket(&u.uz))
        shkmoney = 7 * shkmoney + rn2(3 * shkmoney);
    
    mkmonmoney(shk, shkmoney);

    if (!strcmp(shkname(shk), "Izchak")) {
        struct obj *otmp;

        otmp = mksobj(LONG_SWORD, FALSE, FALSE);
        if (otmp) {
            create_oprop(otmp, FALSE);
            bless(otmp);
            otmp->spe = rn2(4) + 1;
            otmp->oerodeproof = TRUE;
            (void) mpickobj(shk, otmp);
        }
        otmp = mksobj(SHIELD_OF_REFLECTION, FALSE, FALSE);
        if (otmp) {
            bless(otmp);
            otmp->spe = rn2(4);
            otmp->oerodeproof = TRUE;
            (void) mpickobj(shk, otmp);
        }
        otmp = mksobj(CLOAK_OF_MAGIC_RESISTANCE, FALSE, FALSE);
        if (otmp) {
            bless(otmp);
            otmp->spe = rn2(4);
            otmp->oerodeproof = TRUE;
            (void) mpickobj(shk, otmp);
        }
        otmp = mksobj(CRYSTAL_PLATE_MAIL, FALSE, FALSE);
        if (otmp) {
            bless(otmp);
            otmp->spe = rn2(4);
            otmp->oerodeproof = TRUE;
            (void) mpickobj(shk, otmp);
        }
        otmp = mksobj(RIN_SLOW_DIGESTION, FALSE, FALSE);
        if (otmp) {
            bless(otmp);
            otmp->oerodeproof = TRUE;
            (void) mpickobj(shk, otmp);
        }
        m_dowear(shk, TRUE);
        mon_wield_item(shk);
    }
    /* One-eyed Sam already got her equipment in makemon */
    if (Is_blackmarket(&u.uz) && sroom->rtype != BLACKSHOP) {
        register struct obj *otmp;
        /* black marketeer's equipment */
        
        otmp = mksobj(LONG_SWORD, FALSE, FALSE);
        if (otmp) {
            create_oprop(otmp, FALSE);
            if (otmp->spe < 5) 
                otmp->spe += rnd(5);
            (void) mpickobj(shk, otmp);
        }
        if (!rn2(2)) {
            otmp = mksobj(SHIELD_OF_REFLECTION, FALSE, FALSE);
            mpickobj(shk, otmp);
            if (otmp->spe < 5) 
                otmp->spe += rnd(5);
        }
        if (!rn2(2)) {  /* Replacement for GDSM */
            otmp = mksobj(CLOAK_OF_MAGIC_RESISTANCE, FALSE, FALSE);
            bless(otmp);
            otmp->spe = rn2(4);
            otmp->oerodeproof = TRUE;
            (void) mpickobj(shk, otmp);
        }
        if (!rn2(2)) {  /* Replacement for GDSM */
            otmp = mksobj(CRYSTAL_PLATE_MAIL, FALSE, FALSE);
            bless(otmp);
            otmp->spe = rn2(4);
            otmp->oerodeproof = TRUE;
            (void) mpickobj(shk, otmp);
        }
        if (!rn2(2)) {
            otmp = mksobj(SPEED_BOOTS, FALSE, FALSE);
            mpickobj(shk, otmp);
            if (otmp->spe < 5) 
                otmp->spe += rnd(5);
        }
        if (!rn2(2)) {
            otmp = mksobj(AMULET_OF_LIFE_SAVING, FALSE, FALSE);
            mpickobj(shk, otmp);
        }
        /* wear armor and amulet */
        m_dowear(shk, TRUE);
        otmp = mksobj(SKELETON_KEY, FALSE, FALSE);
        mpickobj(shk, otmp);
        
    }
    return sh;
}

STATIC_OVL boolean
stock_room_goodpos(sroom, rmno, sh, sx, sy)
struct mkroom *sroom;
int rmno, sh, sx,sy;
{
    if (sroom->irregular) {
        if (levl[sx][sy].edge
            || (int) levl[sx][sy].roomno != rmno
            || distmin(sx, sy, doors[sh].x, doors[sh].y) <= 1)
            return FALSE;
    } else if ((sx == sroom->lx && doors[sh].x == sx - 1)
               || (sx == sroom->hx && doors[sh].x == sx + 1)
               || (sy == sroom->ly && doors[sh].y == sy - 1)
               || (sy == sroom->hy && doors[sh].y == sy + 1))
        return FALSE;
    return TRUE;
}

/* stock a newly-created room with objects */
void
stock_room(shp_indx, sroom)
int shp_indx;
register struct mkroom *sroom;
{
    /*
     * Someday soon we'll dispatch on the shdist field of shclass to do
     * different placements in this routine. Currently it only supports
     * shop-style placement (all squares except a row nearest the first
     * door get objects).
     */
    int sx, sy, sh;
    int stockcount = 0, specialspot = 0;
    char buf[BUFSZ];
    int rmno = (int) ((sroom - rooms) + ROOMOFFSET);
    const struct shclass *shp = &shtypes[shp_indx];

    /* first, try to place a shopkeeper in the room */
    if ((sh = shkinit(shp, sroom, shp_indx)) < 0)
        return;

    /* make sure no doorways without doors, and no trapped doors, in shops */
    sx = doors[sroom->fdoor].x;
    sy = doors[sroom->fdoor].y;
    if (levl[sx][sy].doormask == D_NODOOR) {
        levl[sx][sy].doormask = D_ISOPEN;
        newsym(sx, sy);
    }
    if (levl[sx][sy].typ == SDOOR) {
        cvt_sdoor_to_door(&levl[sx][sy]); /* .typ = DOOR */
        newsym(sx, sy);
    }
    if (levl[sx][sy].doormask & D_TRAPPED)
        levl[sx][sy].doormask = D_LOCKED;

    if (levl[sx][sy].doormask == D_LOCKED) {
        register int m = sx, n = sy;

        if (inside_shop(sx + 1, sy))
            m--;
        else if (inside_shop(sx - 1, sy))
            m++;
        if (inside_shop(sx, sy + 1))
            n--;
        else if (inside_shop(sx, sy - 1))
            n++;
        Sprintf(buf, "Closed for inventory");
        make_engr_at(m, n, buf, 0L, DUST);
    }

    if (sroom->rtype == BLACKSHOP) {
        stock_blkmar(shp, sroom, sh);
        level.flags.has_shop = TRUE;
        return;
    }

    if (context.tribute.enabled && !context.tribute.bookstock) {
        /*
         * Out of the number of spots where we're actually
         * going to put stuff, randomly single out one in particular.
         */
        for (sx = sroom->lx; sx <= sroom->hx; sx++)
            for (sy = sroom->ly; sy <= sroom->hy; sy++)
                if (stock_room_goodpos(sroom, rmno, sh, sx,sy))
                    stockcount++;
        specialspot = rnd(stockcount);
        stockcount = 0;
    }

    for (sx = sroom->lx; sx <= sroom->hx; sx++)
        for (sy = sroom->ly; sy <= sroom->hy; sy++)
            if (stock_room_goodpos(sroom, rmno, sh, sx,sy)) {
                stockcount++;
                mkshobj_at(shp, sx, sy,
                           ((stockcount) && (stockcount == specialspot)));
            }

    /*
     * Special monster placements (if any) should go here: that way,
     * monsters will sit on top of objects and not the other way around.
     */

    level.flags.has_shop = TRUE;
}

static void
init_shk_services(shk)
struct monst *shk;
{
    const struct permonst *shkdat = &mons[ERAC(shk)->rmnum];
    ESHK(shk)->services = 0L;



    /* Instead of offering basic/premier identify services, general
     * stores will have a random selection of item classes they can 
     * identify (at premier level). 
     */
    if (shk_class_match(RANDOM_CLASS, shk) == SHK_GENERAL) {
        while (!rn2(6)) {
            switch (rn2(10)) {
            case 0: maybe_add_svc(shk, SHK_ID_WEAPON); break; 
            case 1: maybe_add_svc(shk, SHK_ID_ARMOR); break; 
            case 2: maybe_add_svc(shk, SHK_ID_SCROLL); break; 
            case 3: maybe_add_svc(shk, SHK_ID_BOOK); break; 
            case 4: maybe_add_svc(shk, SHK_ID_POTION); break; 
            case 5: maybe_add_svc(shk, SHK_ID_RING); break; 
            case 6: maybe_add_svc(shk, SHK_ID_AMULET); break; 
            case 7: maybe_add_svc(shk, SHK_ID_WAND); break; 
            case 8: maybe_add_svc(shk, SHK_ID_TOOL); break; 
            case 9: maybe_add_svc(shk, SHK_ID_GEM); break;
            }
        }
        /* 1 in 10 offer firearms training */
        if (!rn2(10) && P_MAX_SKILL(P_FIREARM) > 0)
            maybe_add_svc(shk, SHK_FIREARMS);
        if (!rn2(7))
            maybe_add_svc(shk, SHK_PROP);
    }
    }

    /* Each shop type offers it's own identify service */
    if (shk_class_match(WEAPON_CLASS, shk) == SHK_MATCH) {
        if (rnd(100) < 75)
            maybe_add_svc(shk, SHK_ID_WEAPON);
        if (!rn2(5))
            maybe_add_svc(shk, SHK_ID_ARMOR);
    } 
    else if (shk_class_match(ARMOR_CLASS, shk) == SHK_MATCH) {
        if (rnd(100) < 75)
            maybe_add_svc(shk, SHK_ID_ARMOR);
        if (!rn2(5))
            maybe_add_svc(shk, SHK_ID_WEAPON);
    } 
    else if (shk_class_match(SCROLL_CLASS, shk) == SHK_MATCH) {
        if (rnd(100) < 75)
            maybe_add_svc(shk, SHK_ID_SCROLL);
        if (!rn2(5))
            maybe_add_svc(shk, SHK_ID_BOOK);
    } 
    else if (shk_class_match(SPBOOK_CLASS, shk) == SHK_MATCH) {
        if (rnd(100) < 75)
            maybe_add_svc(shk, SHK_ID_BOOK);
        if (!rn2(2))
            maybe_add_svc(shk, SHK_ID_SCROLL);
    } 
    else if (shk_class_match(POTION_CLASS, shk) == SHK_MATCH) {
        if (rnd(100) < 50)
            maybe_add_svc(shk, SHK_ID_POTION);
    } 
    else if (shk_class_match(RING_CLASS, shk) == SHK_MATCH) {
        if (!rn2(3))
            maybe_add_svc(shk, SHK_ID_RING);
        if (!rn2(5))
            maybe_add_svc(shk, SHK_ID_AMULET);
        if (!rn2(5))
            maybe_add_svc(shk, SHK_ID_GEM);
        if (!rn2(5))
            maybe_add_svc(shk, SHK_PROP);
    }
    else if (shk_class_match(TOOL_CLASS, shk) == SHK_MATCH) {
        if (!rn2(2))
            maybe_add_svc(shk, SHK_ID_TOOL);
    } 
    else if (shk_class_match(WAND_CLASS, shk) == SHK_MATCH) {
        if (!rn2(2))
            maybe_add_svc(shk, SHK_ID_WAND);
        if (!rn2(5))
            maybe_add_svc(shk, SHK_ID_ARMOR);
    } 
    else if (shk_class_match(FOOD_CLASS, shk) == SHK_MATCH) {
        maybe_add_svc(shk, SHK_ID_FOOD);
        if (!rn2(10))
            maybe_add_svc(shk, SHK_ID_POTION);
    }
    
    /* All shops */
    if (!rn2(3)) 
        maybe_add_svc(shk, SHK_UNCURSE);
    
    /* Weapon shop services */
    if (shk_class_match(WEAPON_CLASS, shk) == SHK_MATCH
            || Is_blackmarket(&u.uz)) {
        if (!rn2(4))
            maybe_add_svc(shk, SHK_WEP_FIX);
        if (!rn2(4))
            maybe_add_svc(shk, SHK_WEP_ENC);
        if (!rn2(4))
            maybe_add_svc(shk, SHK_WEP_POI);
        if (!rn2(5) && P_MAX_SKILL(P_FIREARM) > 0)
            maybe_add_svc(shk, SHK_FIREARMS);
        if (!rn2(5))
            maybe_add_svc(shk, SHK_PROP);
    }
    
    /* Armor shop services */
    if (shk_class_match(ARMOR_CLASS, shk) == SHK_MATCH
            || Is_blackmarket(&u.uz)) {
        if (!rn2(4)) 
            maybe_add_svc(shk, SHK_ARM_FIX);
        if (!rn2(4)) 
            maybe_add_svc(shk, SHK_ARM_ENC);
        if (!rn2(5) && P_MAX_SKILL(P_FIREARM) > 0)
            maybe_add_svc(shk, SHK_FIREARMS);
        if (!rn2(5))
            maybe_add_svc(shk, SHK_PROP);
    }
    
    /* Charging services: If these shops offer charging, they
     * always offer both basic and premium. However, they
     * are restricted to their own specialty of items. */
    if (     (shk_class_match(WAND_CLASS, shk) == SHK_MATCH) 
          || (shk_class_match(TOOL_CLASS, shk) == SHK_MATCH)
          || (shk_class_match(RING_CLASS, shk) == SHK_MATCH)) {
        if (!rn2(4)) {
            maybe_add_svc(shk, SHK_CHG_BAS);
            maybe_add_svc(shk, SHK_CHG_PRE);
        }
    }

    /* General/junk stores can only offer basic charging, 
     * but they can charge anything. */
    if (shk_class_match(RANDOM_CLASS, shk) == SHK_GENERAL && !rn2(4))
        maybe_add_svc(shk, SHK_CHG_BAS);
    
    if (!strcmp(shtypes[ESHK(shk)->shoptype-SHOPBASE].name, "pet store")) {
        if (!(ESHK(shk)->services & SHK_ID_FOOD) && !rn2(2))
            maybe_add_svc(shk, SHK_ID_FOOD);
        if (!(ESHK(shk)->services & SHK_ID_TOOL) && !rn2(4))
            maybe_add_svc(shk, SHK_ID_TOOL);
    }
    
    if (!strcmp(shtypes[ESHK(shk)->shoptype-SHOPBASE].name, "lighting store")) {
        if (!(ESHK(shk)->services & SHK_ID_POTION) && !rn2(10))
            maybe_add_svc(shk,  SHK_ID_POTION);
    }
    if (!strcmp(shtypes[ESHK(shk)->shoptype-SHOPBASE].name, "gun store")) {
        /* Safety first! */
        maybe_add_svc(shk, SHK_FIREARMS);
    }
    
    /* Racial based services: */
    if (is_dwarf(shkdat)) {
        if (!(ESHK(shk)->services & SHK_ID_ARMOR) && !rn2(5))
           maybe_add_svc(shk, SHK_ID_ARMOR);
        if (!(ESHK(shk)->services & SHK_ID_WEAPON) && !rn2(5))
            maybe_add_svc(shk, SHK_ID_WEAPON);
        if (!(ESHK(shk)->services & SHK_ID_GEM) && !rn2(5))
            maybe_add_svc(shk, SHK_ID_GEM);
    }
    else if (is_orc(shkdat)) {
        if (!(ESHK(shk)->services & SHK_WEP_POI) && !rn2(4))
            maybe_add_svc(shk, SHK_WEP_POI);
    }
    else if (is_gnome(shkdat)) {
        if (!(ESHK(shk)->services & SHK_ID_TOOL) && !rn2(4))
            maybe_add_svc(shk, SHK_ID_TOOL);
        if (!rn2(4))
            maybe_add_svc(shk, SHK_TINKER);
    }
    else if (is_giant(shkdat)) {
        if (!(ESHK(shk)->services & SHK_ID_GEM) && !rn2(5))
            maybe_add_svc(shk, SHK_ID_GEM);
    }
    else if (shkdat == &mons[PM_NYMPH]) {
        if (!(ESHK(shk)->services & SHK_ID_RING) && !rn2(10))
            maybe_add_svc(shk, SHK_ID_RING);
        if (!(ESHK(shk)->services & SHK_ID_POTION) && !rn2(5))
            maybe_add_svc(shk, SHK_ID_POTION);
    }
    else if (shkdat == &mons[PM_HOBBIT]) {
        /* LOTR */
        if (!(ESHK(shk)->services & SHK_ID_RING) && !rn2(10))
            maybe_add_svc(shk, SHK_ID_RING);
        if (!(ESHK(shk)->services & SHK_ID_FOOD) && !rn2(2))
            maybe_add_svc(shk, SHK_ID_FOOD);
    }
    else if (is_illithid(shkdat)) {
        if (!(ESHK(shk)->services & SHK_ID_BOOK) && !rn2(4))
            maybe_add_svc(shk, SHK_ID_BOOK);
        if (!(ESHK(shk)->services & SHK_ID_SCROLL) && !rn2(4))
            maybe_add_svc(shk, SHK_ID_SCROLL);
    }

    if (!rn2(20) || Is_blackmarket(&u.uz))
        maybe_add_svc(shk, SHK_RUMOR);
    
    return;
}

void
maybe_add_svc(shkp, svc)
struct monst *shkp;
int svc;
{
    if (ESHK(shkp)->services & svc)
        return; /* Already has it! */
    ESHK(shkp)->services |= svc;
}

/* does shkp's shop stock this item type? */
boolean
saleable(shkp, obj)
struct monst *shkp;
struct obj *obj;
{
    int i, shp_indx = ESHK(shkp)->shoptype - SHOPBASE;
    const struct shclass *shp = &shtypes[shp_indx];
    
    if (obj->oprops & ITEM_STENCH)
        return FALSE;
    if (shp->symb == RANDOM_CLASS)
        return TRUE;
    for (i = 0; i < SIZE(shtypes[0].iprobs) && shp->iprobs[i].iprob; i++) {
        /* pseudo-class needs special handling */
        if (shp->iprobs[i].itype == VEGETARIAN_CLASS) {
            if (veggy_item(obj, 0))
                return TRUE;
        } else if ((shp->iprobs[i].itype < 0)
                       ? shp->iprobs[i].itype == -obj->otyp
                       : shp->iprobs[i].itype == obj->oclass)
            return TRUE;
    }
    /* not found */
    return FALSE;
}

/* positive value: class; negative value: specific object type */
int
get_shop_item(type)
int type;
{
    const struct shclass *shp = shtypes + type;
    register int i, j;

    /* select an appropriate object type at random */
    for (j = rnd(100), i = 0; (j -= shp->iprobs[i].iprob) > 0; i++)
        continue;

    return shp->iprobs[i].itype;
}

/* version of shkname() for beginning of sentence */
char *
Shknam(mtmp)
struct monst *mtmp;
{
    char *nam = shkname(mtmp);

    /* 'nam[]' is almost certainly already capitalized, but be sure */
    nam[0] = highc(nam[0]);
    return nam;
}

/* shopkeeper's name, without any visibility constraint; if hallucinating,
   will yield some other shopkeeper's name (not necessarily one residing
   in the current game's dungeon, or who keeps same type of shop) */
char *
shkname(mtmp)
struct monst *mtmp;
{
    char *nam;
    unsigned save_isshk = mtmp->isshk;

    mtmp->isshk = 0; /* don't want mon_nam() calling shkname() */
    /* get a modifiable name buffer along with fallback result */
    nam = noit_mon_nam(mtmp);
    mtmp->isshk = save_isshk;

    if (!mtmp->isshk) {
        impossible("shkname: \"%s\" is not a shopkeeper.", nam);
    } else if (!has_eshk(mtmp)) {
        panic("shkname: shopkeeper \"%s\" lacks 'eshk' data.", nam);
    } else {
        const char *shknm = ESHK(mtmp)->shknam;

        if (Hallucination && !program_state.gameover) {
            const char *const *nlp;
            int num;

            /* count the number of non-unique shop types;
               pick one randomly, ignoring shop generation probabilities;
               pick a name at random from that shop type's list */
            for (num = 0; num < SIZE(shtypes); num++)
                if (shtypes[num].prob == 0)
                    break;
            if (num > 0) {
                nlp = shtypes[rn2(num)].shknms;
                for (num = 0; nlp[num]; num++)
                    continue;
                if (num > 0)
                    shknm = nlp[rn2(num)];
            }
        }
        /* strip prefix if present */
        if (!letter(*shknm))
            ++shknm;
        Strcpy(nam, shknm);
    }
    return nam;
}

boolean
shkname_is_pname(mtmp)
struct monst *mtmp;
{
    const char *shknm = ESHK(mtmp)->shknam;

    return (boolean) (*shknm == '-' || *shknm == '+' || *shknm == '=');
}

boolean
is_izchak(shkp, override_hallucination)
struct monst *shkp;
boolean override_hallucination;
{
    const char *shknm;

    if (Hallucination && !override_hallucination)
        return FALSE;
    if (!shkp->isshk)
        return FALSE;
    /* outside of town, Izchak becomes just an ordinary shopkeeper */
    if (!in_town(shkp->mx, shkp->my))
        return FALSE;
    shknm = ESHK(shkp)->shknam;
    /* skip "+" prefix */
    if (!letter(*shknm))
        ++shknm;
    return (boolean) !strcmp(shknm, "Izchak");
}


/* stock a newly-created black market with objects */
static void
stock_blkmar(shp, sroom, sh)
const struct shclass *shp UNUSED;
register struct mkroom *sroom;
register int sh;
{
    /*
     * Someday soon we'll dispatch on the shdist field of shclass to do
     * different placements in this routine. Currently it only supports
     * shop-style placement (all squares except a row nearest the first
     * door get objects).
     */
    /* [max] removed register int cl,  char buf[bufsz] */
    int sx, sy, first = 0, next = 0, total, partial, blkmar_size;
    /* int blkmar_gen[NUM_OBJECTS+2]; */
    int *clp, *lastclp = NULL;
    int goodcl[12];

    goodcl[ 0] = WEAPON_CLASS;
    goodcl[ 1] = ARMOR_CLASS;
    goodcl[ 2] = RING_CLASS;
    goodcl[ 3] = AMULET_CLASS;
    goodcl[ 4] = TOOL_CLASS;
    goodcl[ 5] = FOOD_CLASS;
    goodcl[ 6] = POTION_CLASS;
    goodcl[ 7] = SCROLL_CLASS;
    goodcl[ 8] = SPBOOK_CLASS;
    goodcl[ 9] = WAND_CLASS;
    goodcl[10] = GEM_CLASS;
    goodcl[11] = 0;

    /* for (i=0; i < NUM_OBJECTS; i++) {
       blkmar_gen[i] = 0;
       } */

    total = 0;
    for (clp = goodcl; *clp != 0; clp++)  {
        lastclp = clp;
        first = bases[*clp];
        /* this assumes that luckstone & loadstone comes just after the gems */
        next = (*clp==GEM_CLASS) ? (LOADSTONE+1) : bases[(*clp)+1];
        total += next-first;
    }
    if (total == 0)
        return;

    if (sroom->hx-sroom->lx < 2)
        return;
    clp = goodcl - 1;
    partial = 0;

    blkmar_size = (sroom->hx-sroom->lx+1) * (sroom->hy-sroom->ly+1);
    for (sx = sroom->lx+1; sx <= sroom->hx; sx++) {
        if (sx == sroom->lx + 1 ||
            ((sx - sroom->lx - 2) * total) / (sroom->hx - sroom->lx - 1) > partial) {
            clp++;
            if (clp > lastclp)
                clp = lastclp;
            first = bases[*clp];
            next = (*clp==GEM_CLASS) ? (LOADSTONE+1) : bases[(*clp)+1];
            partial += next-first;
        }

        for (sy = sroom->ly; sy <= sroom->hy; sy++) {
            if ((sx == sroom->lx && doors[sh].x == sx-1) ||
                (sx == sroom->hx && doors[sh].x == sx+1) ||
                (sy == sroom->ly && doors[sh].y == sy-1) ||
                (sy == sroom->hy && doors[sh].y == sy+1) ||
                /* the Blackmarket has 400 items on average */
                (!rnf(400, blkmar_size)))
                continue;

            mkobj_at(RANDOM_CLASS, sx, sy, TRUE);
        }
    }

    /*
     * Special monster placements (if any) should go here: that way,
     * monsters will sit on top of objects and not the other way around.
     */
}


/*shknam.c*/
