```
  _    _            _    ______ __  __ 
 | |  | |          | |  |  ____|  \/  |
 | |__| | __ _  ___| | _| |__  | \  / |
 |  __  |/ _` |/ __| |/ /  __| | |\/| |
 | |  | | (_| | (__|   <| |____| |  | |
 |_|  |_|\__,_|\___|_|\_\______|_|  |_|
```                                    

# Hack'EM

## Table of Contents:

- New Monsters
- Monster changes
- Dragons and Dragon Scales
- New Weapons
- New Items
- Spellbooks and spells
- New Artifacts
- Dungeon Features
- Mechanics and Quality of Life Features
- Dungeon Changes/Special Rooms
- Role changes
- Race changes
- Shops/Shopkeeper Services

## Monster changes

### New Monsters (imported and created)

- (a) giant tick
    - Can appear in main dungeon; increased difficulty by 7.
- (a) giant flea
    - Can appear in main dungeon; increased difficulty by 7.
- (a) giant louse
    - Can appear in main dungeon; increased difficulty by 7.
- (a) giant fly
- (a) giant mosquito
- (a) bullet ant
    - can occupy antholes.
- (a) snow ant
    - can occupy antholes.
- (a) queen ant
- (a) giant cockroach
- (a) tsetse fly
- (a) giant praying mantis
- (a) yellow jacket
- (a) black wasp
- (a) giant wasp
- (a) spitting beetle
- (a) migo drone
    - Added claw attack
- (a) migo warrior
    - Added claw attack
- (a) migo queen
    - Added claw attack
- (a) assassin bug
    - Added BERSERK
- (a) killer beetle
- (a) gibberslug
- (a) ichneumon larva
- (a) giant ichneumon
- (a) locust
- (b) jiggling blob
- (b) lava blob
- (b) static blob
- (b) burbling blob
- (c) basilisk (already in evil and stronger.)
    - Previously occupied the ':' slot
    - higher level, has more attacks than SlashEM
    - Added sleep res
    - Added M1_OVIPAROUS to basilisk.
- (c) chicken
- (c) cockatoo
- (c) parrot
- (d) pit bull
- (d) guard dog
- (d) dingo puppy
- (d) large dingo
- (d) barghest
    - Made amphibious; Added Berserk
- (d) death dog
- (d) rabid dog
- (d) rabid wolf
    - Added Berserk
- (d) wolverine
    - Added Berserk
- (d) shadow wolf
    - Made drain & death resistant.
    - Can darken lit areas when moving
    - Added infravision
- (d) mist wolf
    - Made cold resistant
    - Each mist wolf can exhale clouds of thick mist - purely defensive
- (d) nova fox
    - Renamed from vulpenferno in splicehack
    - Bumped lvl up to 16, diff to 19. 
    - Added resistance to fire/shock, and grants fire/shk res.
- (d) Cerberus
    - guards the gates to Gehennom.
    - Can regrow heads while fighting
- (e) glowing eye
    - Added skittish behavior
- (e) bloodshot eye
    - Added skittish behavior
- (e) blinking eye
    - Added skittish behavior
- (e) weeping eye
- (e) corrosive sphere
- (e) concussive sphere
- (e) whirling sphere
- (e) beholder
    - Removed M1_NOHEAD | M1_MINDLESS (viewable with telepathy now)
    - Cannot tame
- (e) magical eye
    - Cannot tame
- (f) kamadan
    - raised difficulty to 13
- (f) displacer beast (already present in evil)
- (f) caterwaul
- (f) saber-toothed cat
    - Saber-tooth cats can grow up into saber-toothed tigers
    - Made a little weaker than the tigers.
- (f) hellcat
- (f) serval
- (f) serpopard
    - Increased to lvl 27, diff 30.
    - Both claw attacks are now 6d9.
- (f) fat cat
- (f) lion
- (f/@) werepanther 
- (f/@) weretiger 
- (g) statue gargoyle
    - Made immune to death magic
- (g) galltrit
    - Only generate in Hell.
    - Using ratman squeaker tile from SpliceHack.
- (g) jermlaine
    - Gets infravision and are skittish.
    - Start with potions of acid, oil, or others.
    - Jermlaine can start with bullwhips or grappling hooks.
    - Sometimes generated riding rats.
    - Using ratman tile from SpliceHack.
- (g) telamon(caryatid, loadbearer)
- (h) hobbit pickpocket
- (h) dwarf thief
    - Added Berserk 
- (h) duergar
    - Added Berserk
- (h) deep one
- (h) deeper one
- (h) deepest one
- (h) planar pirate
- (h) alhoon
    - Traitorous
- (i) dretch
- (i) rutterkin
- (i) nupperibo
    - Added a drain life bite 
- (i) blood imp
    - Added a drain life bite 
- (i) redcap
    - Added berserk, traitor property
- (i) dust devil
- (j) clear jelly
    - added vulnerable to fire (matches existing j)
    - Added passive acid attack
- (j) yellow jelly
    - added vulnerable to fire (matches existing j)
    - Added stun touch
- (j) orange jelly 
    - added vulnerable to fire (matches existing j)
    - Added passive sleep attack
    - Added touch sleep attack
- (j) rancid jelly
    - added vulnerable to fire (matches existing j)
- (k) swamp kobold
    - Added swimming 
- (k) rock kobold
    - sometimes start with a sling and some rocks.
- (k) kobold warrior
- (k) Kroo the Kobold King
- (l) leprechaun wizard
- (m) killer mimic
    - Reduced speed by half, now speed 6. 
    - These are really mean, so watch out for these in minetown!
- (n) pixie
- (n) brownie
    - made brownies vegetarian.
    - Brownies sometimes get short swords.
    - increased difficulty to 18
- (n) quickling
- (n) lampad
    - Doubled difficulty to 22
    - Usually bears torches 
- (n) thriae
    - changed from 'a' to yellow 'n' (they are bee nymphs)
    - Upped level to 14, difficulty to 27.
- (n) Aphrodite - (Already in evil with her own level)
- (n) yuki-onna
    - Replace AD_LOST attack with fear gaze attack.
    - Added cold resistance
    - doubled difficulty to 14
- (n) dream thief (oneroi, oneroi)
    - Replaced AD_MEMR attack with spellcasting.
    - increased difficulty to 25
- (n) ice nymph
- (o) war orc
- (o) great orc
- (o) Grund the Orc King
- (o) snow orc
    - can throw snowballs like snow golems
- (o) demon orc
- (o) goblin shaman
- (o) goblin outrider
- (o) goblin-captain
- (p) diamond piercer
- (p) god piercer
- (q) lamb
    - Removed "always peaceful" flag
- (q) sheep
    - Removed "always peaceful" flag
- (q) goat
- (q) cow
    - Removed "always peaceful" flag
- (q) bull
- (q) scramper
- (q) squealer
- (q) mangler
- (q) Jumbo the Elephant
- (q) juggernaut
    - Buffed significantly: Now is NASTY
    - Removed G_NOHELL tag from juggernaut so they can appear anywhere.
    - Added fire/poison/shock/sleep/acid/stone/death/drain/sickness resistances
    - Added infravision, regeneration, M1_MINDLESS
    - Changed juggernaut MR to 70, AC to -4,  speed to 15
- (q) catoblepas
    - Specified they have 2 tusks/horns
- (q) manticore
- (q) landshark
- (q) pig
- (q) feral hog
- (q) llama
- (q) black bear
    - Raised level to 7. Added berserk.
- (q/@) werebear
- (q) grizzly bear
    - Changed color to red to avoid clashing with manticore
- (q) chimera
    - fire-breathing 3d6
- (q) hippo
- (q) hellbear
- (q) woolly mammoth
- (r) rabbit
    - Can be tamed with carrots (not rabid rabbits though.)
- (r) black rat
    - cannot be genocided.
- (r) rabid rabbit
    - Added berserk
- (r) pack rat
    - Made a little tougher
    - Now level 2; Added M2_COLLECT, M2_STRONG, M2_FLANK
- (r) hellrat
- (r) giant badger
    - Changed to rodent class to match honey badger, was 'q' in slashem
    - added M1_SWIM to all badgers.
    - Badgers leave occasional poison gas clouds (they are stinky...)
    - Made badgers immune to poison gas (immune their own stink)
    - Added berserk
- (r) honey badger
  - Ignores Elbereth
- (r) skunk
- (r) hedgehog
    - Previously occupied the 'q' slot
    - Removed the "always peaceful" flag.
- (r) enormous rat
- (r) rodent of unusual size
- (s) jumping spider
- (s) recluse spider
- (s) barking spider
- (s) carrion crawler
- (s) nickelpede
    - centipedes grow into nickelpede
    - nickelpedes grow into giant centipedes
- (s) giant centipede
- (s) giant scorpion
- (s) phase spider
    - Made skittish and teleport more like tengus.
- (s) Girtab
- (s) Shelob
- (s) monstrous spider
- (s/@) werespider 	
- (u) selkie
- (u) lesser nightmare
- (u) nightmare
- (u) cauchemar
- (u) pegasus
- (u) Pale horse
- (u) White Horse
- (u) Black horse
- (u) Red horse
- (w) giant leech
- (w) larva
    - Larvas can infest corpses and create maggots
- (w) maggot
    - Can infest corpses and create further maggots or giant flies.
    - Can also create a Worm That Walks by infesting another maggot corpse. 
- (w) dung worm
    - Made poisonous to eat
- (w) acid worm
- (w) bloodworm
- (w) tunnel worm
- (w) rot worm
    - Eating a rot worm causes food poisoning
- (w) neothelid
    - Traitorous
- (w) hellminth
    - Type of maggot that does not generate randomly - only dropped by a dying Worm that Walks.
    - Can infest corpses to create a gibberslug (75% change) or a new Worm that Walks (25% chance.)
    - Cannot be genocided.
- (x) spark bug
- (x) arc bug
- (x) lightning bug
- (y) will-o'-the-wisp
    - Spawns in swamp rooms and Juiblex's level.
- (z) echidna
    - Added M3_INFRAVISIBLE
- (z) platypus
- (z) koala
- (z) wombat
- (z) Tasmanian devil
    - Added Berserk
- (z) wallaby
    - Can jump
- (z) wallaroo
    - Can jump
- (z) kangaroo
    - Can jump
- (z) drop bear
- (A) movanic deva
    - Added fire/shock/death magic resistance. Has big_wings
- (A) monadic deva
    - Added fire/shock/death magic resistance. Has big_wings
    - Made breathless.
- (A) astral deva
    - Added fire/shock/death magic resistance. Has big_wings
- (A) planetar
- (A) solar
- (A) archangel
- (A) Eldritch Ki-rin
- (A) lamassu
    - has 2 horns
    - Replaced one of the claw attacks with a butt attack.
- (B) rhumbat
- (B) athol
    - Added "ahool!" sounds that aggravate monsters.
    - Alert to presence of athols, you hear a distant howl
    - Increased their difficulty
- (B) hellbat
- (B) mongbat
- (B) mobat
    - Mobats collect treasure
    - Added piercing screams that cause pain
- (B) harpy
    - can start with jewelery.
    - uses primitive weapons, big bones/clubs, sometimes bows
    - Added song attack that can paralyze from a distance.
    - Changed #chat voice to MS_SEDUCE (could be better...)
- (B) byakhee
- (B) nightgaunt
    - Sometimes get tridents, spears or javelins
- (B) zoo bat
- (B) gaol bat
- (B) phoenix
    - Phoenix eggs hatch instantly when exposed to lava or fire.
- (D) baby deep dragon
- (D) deep dragons
    - Added AD_DRLI to passive attacks (AT_NONE)
    - Baby/adult deep dragons resist poison and drain magic
- (D) baby shimmering dragon (already in EvilHack)
    - Gets a passive stun attack
- (D) shimmering dragon 	 (already in EvilHack)
    - Gets a passive stun attack
- (D) gold dragon
- (D) baby gold dragon
- (D) sea dragon
- (D) baby sea dragon
- (D) wyvern
- (D) hydra
    - Added poisonous breath, passive poison 
    - Imported multiple heads mechanic from splicehack.
- (D) fell beast
- (D) pseudodragon
- (D) elder pseudodragon
- (D) ancient pseudodragon
- (D) Father Dagon
    - Not used yet
- (D) Mother Hydra
    - Not used yet
- (E) mud elemental
    - Added engulf attack (like water elemental)
- (E) acid elemental
    - Added acid engulf (like shoggoth)
- (E) ice elemental
    - Spits snowballs
- (E) magma elemental
    - Added engulf attack 
- (F) gray fungus
    - Has a speed of 1 so they can move.
    - Added a 1d4 touch attack (From SlashTHEM)
- (F) disgusting mold
- (F) black mold
    - Disabled to make room for swamp ferns.
- (F) brown moldier
- (F) yellow moldier
- (F) green moldier
- (F) red moldier
- (F) shrieker
    - Added sonic resistance
- (F) screamer
    - Has a sonic touch attack, but removed the "touches you" msg.
- (F) dungeon fern/dungeon fern sprout
    - added acid res
    - Changed color to White
    - Every time a fern sprout releases a spore, it gains a level. (Enables sprouts to grow up into adult ferns)
- (F) arctic fern/arctic fern sprout
    - Changed color to blue (spores to bright blue not to confuse with floating eyes)
- (F) blazing fern/blazing fern sprout
- (F) swamp fern/swamp fern sprout
    - Changed from cyan to black
    - Added swamp ferns to mine/cave river generation (if there is sewage)
- (e) dungeon fern spore
- (e) arctic fern spore
- (e) blazing fern spore
- (e) swamp fern spore
- (e) fern spore
- (F) volatile mushroom
- (G) gnome thief
- (G) deep gnome 
- (G) gnome warrior
- (G) Ruggo the Gnome King
- (G) gnoll
- (G) gnoll warrior
- (G) gnoll chieftain
- (G) gnoll shaman
- (G) marrashi
- (H) hill giant shaman
- (H) The Largest Giant
- (H) hecatoncheire (Hundred Handed One)
- (H) elder minotaur
    - Traitorous
- (J) vorpal jabberwock (evil already had with the beheading attack!)
- (J) snark
- (J) boojum
- (L) eye of fear and flame
- (L) worm that walks
    - When destroyed, generates up to 10 hellminth maggots. These maggots can infest other corpses to create more worms that walk.
- (M) troll mummy 
    - Added regen and stalking
- (M) adherer
- (M) hobbit mummy
- (M) centaur mummy
    - A Hack'EM original!
- (N) ha-naga
- (O) ogre mage
- (O) shadow ogre
    - Gave a frequency of 2; let them appear in the main dungeon 
    - Made poison, drain & death resistant and vulnerable to fire. 
    - darkens lit areas as they move.
- (P) moldy pudding 
    - added vulnerability to fire
- (P) shoggoth
    - Limited to one corrosion attack.
- (P) giant shoggoth
    - Limited to one corrosion attack.
- (P) blood pudding
    - specified not vegetarian
- (P) gel
- (Q) genetic engineer (already in evil)
- (Q) alchemist
    - Added skittish behavior
- (S) king cobra
- (S) asphynx
- (S) cerastes
- (S) amphisbaena
- (S) giant anaconda
- (S/@) weresnake
- (T) two-headed troll
- (T) black troll 
- (T) cave troll
    - Buffed some of the attacks.
- (T) grave troll
- (U) water hulk
    - Added vulnerability to shock
- (U) umbral hulk
    - they have a darkness movement, litroom_mon
- (U) hunger hulk
- (U) slumber hulk
- (U) shambling horror
    - Traitorous
- (V) fire vampire
- (V) star vampire
- (V) vampire mage (already in evil with minor changes)
- (V) nosferatu
    - Instead of were_summoning rats, summons wolves
- (W) wight
- (W) bodak
- (W) banshee
- (W) slaughter wight
    - A HackEM original!
    - Added infravision, ACCURATE, BERSERK, and STRONG. -8 AC
    - Has 2 powerful melee slams: (2d18 drain energy/clobber)
    - Also has drain life attacks.
    - Releases a nasty death wail when slain. 2d18 sonic damage, Stuns
    - Replaced slaughter wight tile with vomitous ghoul tile from SpliceHack.
- (Y) zruty
    - Changed zruty from z to Y (to accomodate zouthern animals)
- (Y) lemur
- (Y) baby owlbear
- (Z) hobbit zombie
- (Z) ghoul mage
- (Z) ghast
    - Ghasts gain a level for every corpse they eat.
    - Ghast's bite attack now scales with their level.
- (Z) ghoul queen
- (Z) revenant
- (Z) gug
- (Z) draugr
    - Added zombie bite attack, and vulnerable to fire.
- (Z) vomitous ghoul
- (Z) skeletal pirate
- (Z) zombie dragon
- (Z) centaur zombie
    - A Hack'EM original!
- (`) wax golem
    - can be created from wax poly.
    - Wax golems have a small chance of death dropping a magic candle
- (`) plastic golem
    - Can drop credit cards and fake amulets.
- (`) snow golem
- (`) ruby golem
- (`) diamond golem
- (`) sapphire golem
- (`) steel golem 
    - Added vulnerability to acid
    - Subject to the same rust vulnerability as iron golems (they are basically iron)
- (`) crystal golem
    - Added reflection to the higher level golems.
    - Changes color every turn
- (@) gibberling
    - Hates light
    - Gibberlings get a sonic attack: Stuns, confuses, etc (Similar to a cursed unihorn)
    - Can infect monsters/players with gibberslug larva.
- (@) grimlock
    - Hates light and can stalk you.
    - can start with Bone Clubs, bone aklys, axes or orcish spears.
    - added a claw attack
- (@) elven wizard
- (@) mugger
- (@) black marketeer 
    - added accurate behavior
- (@) general
- (@) bandit
- (@) peasant
    - Can start with a scythe
- (@) bards
- (@) musicians
    - (for village; used apprentice tile)
- (@) street thug leader (for village)
- (@) street thug 
    - (for village; use pirate crewmate tile)
- (@) black marketeer 
- (@) Charon
- ( ) shadow 
    - Added M3_INFRAVISION
- ( ) spectre
- (&) lava demon
- (&/d) weredemon
    - Wereform is a hell hound.
- (&) spined devil
    - Spined devils have big wings - flies
    - can get tridents, halberds.
    - Added a weapon attack
- (&) bearded devil
    - Added BERSERK and resists cold
    - Bearded devils get glaives.
    - Bearded devils gets a poisonous beard attack  (1d13 weapon)
- (&) bar-lgura
    - Added shock resistance.
    - Bar-Lgura are telepath, can jump, see inv
- (&) chasme
    - Added telepathy, see invisible, resistance to acid, cold, and shock.
    - Raised MR to 70
- (&) babau
    - Babau have 1 horn and a thick leathery hide.
    - Added a passive corrosion attack (from their acidic greasy hide)
    - Babau usually use two-handed swords or spears.
- (&) nabassu
    - Nabassu have a thick hide and a pair of forehead horns
    - Added M3_DISPLACES
    - Nabassu can cause victims(you) to rise as a ghast. 
    - Nabassu get a gaze life drain attack
    - Nabassu darken their environment when moving
- (&) armanite
  - fast demon from splicehack
- (&) damned pirate
- (&) spibrawulf
- (&) Cthulhu
    - Put Cthulhu in the middle of sanctum
    - Cthulhu cannot teleport (as in UnNethack)
- (;) mind flayer larva
- (;) giant crab
- (;) merfolk
    - Combination of Splice and SlashTHEM versions
- (;) swordfish
- (;) sea horse
- (;) giant squid 
    - Previously "thing from below" in splice
- (:) giant turtle
- (:) frost salamander
- (:) gila monster
    - Made vulnerable to cold
- (:) rhaumbusun
- (:) komodo dragon
    - Made vulnerable to cold
    - Komodo dragons can swim and can hiss
    - Added MR20 (~MC1 of chain mail)
- (:) velociraptor
- (:) T-Rex
- (%) bad egg
    - vulnerable to fire - "cooking"!
- (%) killer tripe ration
    - vulnerable to fire
- (%) killer food ration
    - vulnerable to fire
- ($) pile of killer coins
    - leaves piles of coins (similar to gold golems)
- ($) large pile of killer coins
    - leaves piles of coins
- ($) huge pile of killer coins
    - leaves piles of coins
- (@) flame mage
- (@) necromancer
- (@) ice mage
- (@) undead slayer
- (@) yeoman
- (@) High Flame Mage
- (@) High Ice Mage
- (@) Dark Lord
- (@) Van Helsing
- (@) Chief Yeoman Warder
- (@) Water Mage
- (@) Ragnaros (Replaces the Earth Mage, slashem-up)
- (@) Maugneshaagar
- (@) Count Dracula
- (@) Colonel Blood
- (@) igniter
- (@) froster
- (@) embalmer
- (@) exterminator
- (@) Yeoman Warder

### Monster changes:

- [x] Zombies 
  - probably the most significant monster change (from EvilHack) and one of the key changes that makes this variant so difficult.
  - Zombies now have two different attack types, one being a poisonous claw attack (effects and frequency are the same as a bite from a rabid rat), the other being a disease-inducing bite attack to the head. The latter is a very serious threat, as the ability to cure oneself from being terminally ill are almost non-existent in the early game. 
  - If the player becomes ill from a zombie bite, they only have a few turns to cure themselves, or they will die and turn into a zombie. 
  - This zombie can be encountered by other players if a bones file is generated. 
  - There is a very small chance that the player will be able to continue playing if they are turned into a zombie;  however, they will be stuck in that form for the remainder of the game. 
  - A zombie's bite attack also has a chance of causing amnesia. 
  - Eating a zombie corpse will make the player ill and cause them to start turning into a zombie (tins of zombie meat 'cure' the meat and make it safe to eat). 
  - Attacking zombies at range and wearing a piece of armor that protects your head are highly recommended. 
  - Zombies can make other monsters terminally ill as well - the same methods used by the player to cure themselves of a terminal illness can be used/applied on monsters or pets also.

- [x] Dragons:
    - Significantly enhanced
    - spawn with much more hit points (especially in the end game),
    - can engulf creatures smaller than it and digest them
    - can now bellow a frightful roar that causes Afraid status.
    - To balance out engulf and roaring mechanics - only half of dragons are capable of roaring, and I removed the engulf attack from about half of the dragons.
    - Also, in order to offset their powerful abilities, dragons periodically slumber. Running from a dragon and hoping it decides to rest is a valid strategy. (From Splice)
    - A couple of things will now protect you from dragon roars:
        - Being a knight
        - Wearing Dragonbane or Pridwen
        - Being confused

- [x] Elementals:
    - All elemental monsters can engulf creatures smaller than it
    - can be zapped and transformed
    - Ex: Zap water elemental with / of Cold to turn it into Ice Elemental
- [x] Were-monsters: summon wider (and more dangerous) diversity of monsters
- [x] Rabid monsters can no longer be tamed
- [x] Elves are lawful instead of chaotic
- [x] All orcs are immune to poison
    - Wargs can now be ridden by player orcs, and subsequently can only be tamed by the same.
- [x] Nymphs and mermaids will introduce themselves when seducing you (splice)
- [x] Nymphs now hate iron.
- [x] Imps now hate iron.
- [x] All Fungi/migos: now hate copper and receive an extra d6 damage from copper.
- [x] Copper does extra damage vs monsters with disease, decay, and pestilense attacks.
- [x] All Zouthern animals now have a small chance of death-dropping boomerangs.
- [x] Gnolls:
    - Evil already had gnolls, so I disabled the Evil gnolls in favor of the slashem versions - and used the G symbol instead)
    - Updated to match evil weight/size, poison res, infravisible, berzerk.
- [x] Priests, soldiers, the watch, and player monsters can also be a variety of different races.
- [x] Shopkeepers:
    - usually spawn get a shotgun and shells
    - Doubled their base level (compared to vanilla) (from SlashEM)
- [x] Player monsters:
    - can be encountered throughout the game.
    - They covet the Amulet of Yendor, and attempt to steal it.
    - Any monster, should it get a hold of the Amulet of Yendor, can and will sacrifice it on an endgame altar.
- [x] Stone golems, gargoyles, winged gargoyles: Now vulnerable to digging rays, halves their HP.
- [x] Rats: Can tamed with cheese
- [x] gelatinous cube and green slime now have engulfing attacks
- [x] Baby gray dragons: get MR20 (twice the other baby dragons)
- [x] Gnome Kings: Raised to lvl 9, gnome warriors to 7.
- [x] Gnomes outside the Mines have a chance of generating with a few gems
- [x] Dwarves have a 6/7 chance to be generated with a potion of booze (Outside of the mines)
- [x] Hobbits may carry some healthy snacks
- [x] Ice Devil gets an additional slow attack.
- [x] Rock trolls are now stoning resistant (from xnh)
- [x] Olog Hai are now poison resistant
- [x] Keystone Kops:
  - can no longer be genocided. 
  - may generate with pistols, bullets and dagger/knife.
- [x] Lichens: Got a weak slow touch attack.
- [x] Leprechauns will steal anything made of gold
- [x] mind flayers/master mind flayers color changed to bright magenta.
- [x] Decay attacks instakill paper golems
- [x] Tigers are now orange. 
    - Tigers rarely drop a tiger eye ring when killed (xnh)
- [x] Mumakil and mastodons/woolly mammoth:
    - can attack with their trunks, grabbing onto and crushing their target. 
    - difficultly level increased 
- [x] Salamanders: can grab onto the player and pull them into lava.
- [x] Ghosts: 
    - All incorporeal monsters can only be hit with blessed, silver, or bone objects
    - can also turn invisible and reappear - scaring (and paralyzing) the player time.
    - Prevent ghosts from being renamed (xnh)
- [x] Skeletons rarely drop skeleton keys (xnh)
- [x] Wraith corpses 
  - nerfed (from slashem) - New effects:
    - 10% lose exp; 10% max HP/Pw decreases;
    - 10% max HP/Pw increases.
    - 50% Gain xp; 20% nothing happens.
- [x] trappers and lurkers above mindless (now they cannot be detected with ESP)
- [x] The Oracle sometimes gets a potion of hallucination (xnh)

### Monster Mechanic Changes:

- [x] Mold/Fungus ressurection
    - (ported from Splicehack, which ported from xnh)
    - The ressurection rates have been tapered back a bit, so it's not as annoying as in Slash'EM - but there are more F monsters that can spawn (including moldiers at deeper levels)
- [x] Shape-changing monsters (except for vampires and were-creatures)
    - will revert back to their original form when their polymorphed form dies.
    - They cannot change form again once this happens.
- [x] Updated nasties lists with slashem monsters. 
  - Added gugs, slaughter wights, devas, revenants 
- [x] Some monsters ride other monsters as steeds.
- [x] Imps and others cussing the hero wake up nearby monsters
- [x] Removed "Hits as +x weapon" property from slashem monsters
- [x] Removed "Requires +x weapon to be hit" property from slashem monsters
- [x] BERSERK behavior from evil:
    - Added to many of the imported monsters as felt appropriate
- [x] Many new monster attacks
- [x] Oily Corpses patch
    - There is a 20% chance upon starting to eat a corpse of certain slippery monsters (e.g. most slithy, non-snakelike monsters and most blobby monsters) that your fingers get slippery. Tins of them are not slippery unless the tin preparation method is slippery.
- [x] Monster traitor property (M3_TRAITOR): 
    - If tame, these monsters have a chance to betray you.
    - In slashem this used to depend on the monsters HP being lower than yours, but removed that condition.
      - I also increased the chance of betraying significantly each turn.
      - Added the traitor property to all monsters that had it in SlashEM/THEM, and related new imported monsters.
- [x] Implemented Flanking (M2_FLANK) from SpliceHack.
    - Monsters with flanking are "tactical"
    - Monsters gain a large bonus to hit when flanking the player (located on opposite sides of the player's position).
    - Flanking scales with monster level (used to be a flat +4 AC penalty):
        AC penalty = 4 + (mdiffulty - 4) / 2
    - I applied FLANKING from SpliceHack to many appropriate monsters in hackem:
- [x] Monster hit point generation is now determined by their size as well as what level they are.
- [x] They also have the same armor class damage reduction that players enjoy.
- [x] Intelligent monsters
    - can use many more items against the player or for their own benefit, 
    - including stashing objects into bags and taking them back out, 
    - activating any figurines it may have, 
    - unlocking and looting locked containers,
    - wearing most rings,
    - reading scrolls of remove curse to uncurse items in its inventory, 
    - scrolls of charging to recharge wands in its possession, 
    - and utilizing powerful wands against the player.
- [x] Player monsters:
  - now technically their own class of monster and are separate from how the actual player is defined in src/monst.c, 
  - and were purposely left out of the new monsters table as it would unnecessarily inflate that list.
  - Player monsters differ here from vanilla NetHack in that they can spawn outside of the Astral Plane, are yellow in color, and are kitted out in player-like gear starting at level 10, and then can be kitted out with ascension-level gear starting at level 20 
  - their hit dice and difficulty level roughly matches that of the actual player (this is based off of what experience level the actual player is when the player monster spawns). 
  - Their abilities closely match their role as well (wizards and priests can cast spells, rogues can steal items from inventory, etc). 
  - Player monster Knights spawn riding either a horse or a warhorse. 
  - Player monsters in EvilHack covet the Amulet of Yendor and will attempt to steal it, and if they steal it while on the Astral Plane, will attempt to sacrifice it themselves. As you can imagine, if that happens, it is a game-ending event.

### Monster spawn rates

- Simplified a bit from EvilHack:
    - Monster spawn rates increase to 4x normal once you've performed the invocation, plus:
    - all denizens of the dungeon are aware of what has happened, 
    - Nasties will spawn on each levels upstairs

### Unique Monster Changes:

Medusa:
- enhanced; no longer an easy kill. 
- boosted hit points and level 
- spawns wielding an enchanted orcish bow with a quiver full of enchanted and poisoned orcish arrows. 
- The snakes on her head can attack (poisonous bite) 
- her own bite causes slow-stoning. 
- Her gaze is still of the instant petrification type. 
- Having reflection is no longer a guaranteed kill, she resists most of the time.
- When defeated, a pegasus will arise from her body (From SlashEM)

demon lords/princes encountered in Gehennom,
- along with which lair they may appear in, has been completely randomized. 
- Your experience in Gehennom can be a very different experience per game.
- Baalzebub, Geryon, Dispater are back to being a princes (demoted from lords).


Juiblex:
- enhanced, and has many more acid-based attacks at its disposal.

Vlad the Impaler:
 - can cast wizard-based monster spells, and wields the artifact weapon Lifestealer.

The Wizard of Yendor:
- can use any artifact weapons or armor that he steals against the player.
- Color changed to bright magenta.
- I reduced the chance Rodney gets an "oMR to 1 in 13 times (changed from Evil)
    
The Riders:
- have had their hit points increased significantly, and each Rider has its own steed which it rides, each with unique attacks and abilities.
- Riders only get 40 + d(10, 8) HP instead of 100 + d(10, 8) (changed from Evil)

Dragons and Dragon Scales

	EvilHack uses the dtsund patch for dragon scales, which means that dragon scale
	mail no longer exists - but we can apply dragon scales to any armor. Since
	robes are now body armor - we can even created dragon scaled robes! Evil 
	introduced some passive effects, I have expanded these so that every dragon 
	scale provides them. I have also switched around or added new abilities.
	Most of these abilities are from other forks, but some are my own addition.


| Dragon | Primary    | Secondary  | Passive             |
|--------|------------|------------|---------------------|
| Black		 | Disintegration res | slow digestion		 | Disintegration (!!) |
| Blue		 | Shock res	 | Speed			   | Shock (!!)          |
| Deep		 | Drain res			 | Stable (hurtling res)	 | Drain life          |
| Gold		 | Light 			  | Automatic searching	 | Blinding            |
| Gray		 | Magic res			 | Cancel protection		 | Cancellation (!!)   |
| Green		 | Poison res		 | Sickness res		 | Poison              |
| Orange		 | Sleep res			 | Free action			 | Slow               |
| Red			 | Fire res			 | +1d6 melee damage 	 | Fire (!!)           |
| Shimmering | Displacement		 | Stealth, warn, see inv | Confuse/Stun/slow    |
| Silver	| Reflection		 | Blinding resistance	 | Scaring             |
| Sea		 | Magical breathing	 | Swimming		 | Water/rust (!!)     |
| White	| Cold res			 | Freeze water/lava walk | Cold (!!)           |
| Yellow | Acid res			 | Stoning resistance	 | Acid (!!)           |

!! - Risks item destruction from the type of attack

- Most of the passive attacks only trigger 33% of the time.
- Black DSM also protects from engulfing digest attacks (But not from gelatinous cubes because they have a engulf "wrap" attack)
- Blue scales passive shock is normally 1d6, but is capable of a critical hit for 2d24 
- Acid scales passive acid is normally 1d3, but is capable of a critical hit for (level+1)d6

## Item Changes:

### New Weapons:

- [x] footbow/warped bow
- [x] light arrow
    - Found only in archery shops
- [x] great dagger	
- [x] parazonium
- [x] chakram (metal based boomerang)
    - Made chakram dmg 10 vs small, 9 vs lg, and +1 tohit. 
    - Weight 25
    - Decreased probability to half of boomerang.
    - Splice had chakram's base material as silver - this seemed a bit too powerful - changed base material to IRON, but can be any shiny material so still a 20% chance of being silver.
    - Chakrams only deal 1-2 damage if used as a melee weapon.
- [x] throwing axe
    - Does 1 less damage vs sm than a regular axe, but counts as a throwing weapon so can be stacked.
    - Lowered the weight of throwing axes to 20, +2 to-hit.
- [X] gladius
- [x] falchion (heavy sword)
    - Barbarians sometimes start with a falcion/scimitar
- [x] spiked chain
- [x] flaming lash
    - Added fire damage
- [x] rapier
    - Rapiers can be generated randomly.
- [x] baseball bat
    - baseball bats are only generated in junk shops.
- [x] fly swatter 
    - uses WHIP skill instead of PADDLE
- [x] scythe
    - Death gets a scythe
- [x] executioner's mace
    - Powerful but doesn't generate randomly.
    - The worm that walks can genenerate with one.
- [x] stake
    - "wooden stake" renamed to "stake" so object materials make more sense.
    - Added better bonuses for stakes vs vampires 
    - dagger skill, undead slayer and artifact-stake bonuses stack)
    - Give monsters 20% instakill rate for wooden stakes
    - Added to-hit bonus of +1
    - Design decision: I'm deviating from slashem here - vampires should not be vulnerable to stakes
        while in different forms (like Vampire Bats). Makes rings of protection from shapechangers more valuable.
- [x] atgeir
    - Spear: Base item for Gungnir
    - 1d4 + 1d4 versus small; 1d12 versus large monsters.
- [x] atlatl
    - Base item for Xiuhcoatl
    - 1d8 versus small; 1d12 versus large monsters.

New magical staves: These were introduced in Evil and each one grants a large bonus to a specific spell school

- All deal standard quarterstaff damage.
- [x] staff of divination (DIVINATION school)
- [x] staff of healing (HEALING school)
- [x] staff of necromancy (NECROMANCY school)
    - (Replaced the staff of holiness)
- [x] staff of matter (MATTER school)
- [x] staff of escape (ESCAPE school)
- [x] staff of war (ATTACK school)

### Firearms:

- [x] pistol
- [x] submachine gun 
    - (apply for different firing modes)
- [x] heavy machine gun
    - Decreased weight of from 500 to 200.
- [x] rifle
- [x] sniper rifle
- [x] assault rifle
- [x] shotgun
    - Increased range to 5 (from 3)
- [x] auto shotgun
    - (apply for different firing modes)
    - Increased range to 4 (from 3)
    - Increased to-hit from 0 to 1.
- [x] bullet 
    - (Silver bullets are covered by object materials)
- [x] shotgun shell
    - Now weighs only 7.
- [x] bombs (previously known as grenades)
    - [x] fire bombs
    - [x] gas bomb
    - [x] sonic bomb (Hack'EM!
    - All bombs can be ignited by sources of fire (fire rays, explosions, etc). This will start their normal timer. 
    - (In SlashEM, grenade explosions would be calculated and happen instantaneously)
    - Bombs now insta-explode when you throw them (as they do for monsters)
    - Now weigh only 10.
	
### Armor:

- [x] light armor
    - Replaces leather armor as part of the object materials system.
- [x] shimmering dragon scales (already in evil)
- [x] robe of protection
- [x] robe of power
- [x] robe of weakness

### Cloaks:

- [x] plain cloak
    - Replaces the leather cloak as part of the object materials system.
- [x] poisonous cloak
- [x] cloak of flight
- [SKIPPED] lab coat (Evil/vanilla has alchemy smock)

### Helms:

- [x] toque
    - Grants sonic resistance
- [x] tinfoil hat
    - Grants Psychic Resistance, protection against brain eating.
- [x] helm of speed
    - Grants very fast speed.
- [x] helm Of madness (HackEM)
    - Causes hallucination, grants +3 charisma while worn. 
    - Auto-curses on wear. Usually generated cursed.

### Gloves:

- [x] gauntlets
    - just regular iron-based gloves.
- [x] gauntlets of protection
    - base item cloth, provides MC3.
- [x] gauntlets of swimming
- [x] rogues gloves (splice)
- [x] boxing gloves (splice)
    - Increased chance of stunning from 1 in 50 to 1 in 25.

### Shield:

- [x] shield of light
    - gives off light when worn. (radius based on its BUC status)
    - base material is gold
- [x] shield of mobility
    - confers free action when worn. 
    - base material is steel
- [x] tower shield
- [x] hide shield
- [x] resonant shield

### Boots:

- [x] stomping boots
- [x] Orcish boots
    - much like dwarvish boots, but of lesser quality.

### Barding:

- [x] barding: 
    - armor for steeds.
    - applied to your steed just as you apply a saddle
    - it's 'one size fits all'. 
    - The material determines how much extra AC it gives your steed
- [x] spiked barding: 
    - Protects you and steed from engulfing attack
- [x] barding of reflection: 
    - fully reflective just like a shield of reflection.

### Food:

- [x] asian pear
    - Added asian pear to possible tree fruits
- [x] mushroom
    - Mushrooms can now propogate if left on the dungeon floor. 
- [x] sandwich
- [x] tortilla
    - Made vegetarian, not vegan.
- [x] cheese
- [x] pill
    - Make case 4 ("tastes like vitamins") only give 80 nutrition, but also grants gain ability (like fountains, blessed if high luck)
    - Pills always cure larval infection.
- [x] holy wafer
- [x] pineapple
    - Gains bonuses when used as a thrown weapon. 
    - can upgrade to random bomb types via dipping in potion of gain level.
- [x] fruitcake
  - Provides a LOT of nutrition (1000))
  - 18 extra damage when thrown.
  - Eating a cursed fruit does 1d6 dmg
- [x] apple pie
- [x] pumpkin pie
- [x] slice of cake
- [x] sprig of catnip (was "pinch of catnip")
    - Can tame any f

### Amulets:

- [x] amulet of flying (already in evil)
- [x] amulet of drain resistance
- [x] amulet versus stone
    - The amulet provides permanent and durable resistance 
    - in SlashEM it would change BUC status when it protected you and disintegrate when cursed.
- [x] amulet of nausea
- [x] amulet of danger
    - Provides infravision, 
    - greatly increases dungeon difficulty. Usually cursed.
- [x] Amulet of magic resistance
    - provides magic resistance when worn.

### Rings:

- [x] mood ring
    - Now this also glows bright green if your alignment is pious 
    - It glows red only if your alignment is negative.
- [x] ring of sleeping
- [x] ring of gain dexterity
- [x] ring of gain intelligence
- [x] ring of gain wisdom
- [x] ring of sonic resistances
    - Immune to sonic damage
- [x] ring of psychic resistance

### Potions:

- [x] potion of ESP
    - Made blessed potion of ESP temporary (250-350 turns)
- [x] potion of invulnerability
- [x] potion of clairvoyance
- [x] potion of amnesia
    - Amnesia potions can now be anything - since the amnesia mechanic was nerfed.
    - Cures afraid status (you forget what you were afraid of!)
- [x] potion of blood
- [x] potion of vampire blood
- [x] potion of regeneration
- [x] potion of reflection

### Wands:

- [x] wand of healing
- [x] wand of extra healing
- [x] wand of draining
- [x] wand of fear
    - Changed to an omni wand that behaves as the cause fear spell, 
    - removed engrave msg, added cursed effect.
    - Breaking causes you to panic in fear.
    - Cursed wands of fear zaps you with fear 80% of the time in addition 
      to other negative effects. If monsters are fleeing, they stop. If 
      monsters are paralyzed or sleeping, they wake up and are able to move.
    - Monsters are able to zap you with wands of fear (as a defensive action).
- [x] wand of create horde 
    - generates with WAY less charges (1-3) and very rare
    - Players zapping this create 7-11 instead of 7-13 monsters.
    - Monsters zapping this create 6-10 instead of 10-14 monsters.
- [x] wand of wind
    - Renamed wand of windstorm to Wand of Wind
    - hurtles you in a random direction when zapped at self.
- [x] wand of water
    - In splicehack this wand was an immediate beam type wand.
    - Converted it to a ray based wand instead (similar to the sea dragon breath)
    - does 6d8 vs non-amphibious monsters. (consider suffocation part of the damage)
    - Amphibious/breathless monsters are immune to water blasts.
    - Fiery monsters take extra damage
    - The ray can destroy forges.
- [x] wand of acid
- [x] wand of poison gas
- [x] wand of wonder (splice implementation)
- [x] wand of sonics
    - Immune to sonic damage
    - Can be used to destroy iron bars or drawbridges. 
    If you destroy iron bars, they send 1-3 iron chains flying.

### Scrolls:

- [x] scroll of time
- [x] scroll of knowledge
- [x] scroll of transmogrify
    - Previously known as "scroll of change material"
- [x] scroll of elementalism
- [x] scroll of air
    - Confused: Creates 1-3 air elemental(s) if cursed, 
      or 1-3 whirling spheres if non-cursed.
- [x] scroll of ice (them)
    - Upgraded: Now in addition to freezing floor tiles, these can freeze pools, puddles, and sewage. They also destroy lava and fire traps.
    - Most magical traps will be converted to ice traps.
    - Inflicts cold damage to monsters within a 5 square radius if blessed, otherwise 3. 
    - Blessed does 6d6 cold (doesn't target you), uncursed deals 2d3 to everything in the radius, and cursed deals 1d3.
    - Confused: Instead of removing lava, this now creates 1-3 freezing spheres or ice elementals.
    - Waterproof like the scroll of flood.
- [x] scroll of cloning
    - Clones an item in your inventory
    - Confused cloning will clone yourself.
    - If polymorphed, you clone your monster type into a tame clone.
    - Confused cursed = hostile clone
    - Confused uncursed = peaceful clone
    - Confused blessed = tame clone
    - Monsters can read scrolls of cloning, but it always clones them - not objects.
- [x] scroll of magic detection
    - detects magical objects on the current level and in your inventory. 
    - An uncursed scroll will label such items as 'magical'
    - a blessed scroll will identify fully those same items (except beatitude).
- [x] scroll of flood (un)
    - Remove possibility of instadeath
    - Scrolls of Flood resist blanking
    - In addition to the "Saved by the bell!" grave with a bell on top, include a "Apres moi, le deluge." grave with a scroll of flood on top. Idea by tedav.

### Tools/Weapon Tools:

- [x] lute
  - When played, has a small chance of ending confusion effects.
- [x] bagpipe
  - When played, will aggravate monsters unless you have high dexterity and experience level.
- [x] horn of blasting 
  - Deals sonic damage when applied, similar to the Fire and frost horn.
  - Immune to sonic damage
- [x] fishing pole
  - Can be used on water sources to fish for stuff.
  - Can also grab small objects out of water.
- [x] magic candle
  - Similar to a magic lamp, but not capable to granting wishes
- [x] keg
  - Contains a very large number of potions of booze. 
  - Quaffing from the keg will consume a single potion; 
  - applying the keg will consume all the potions, potentially killing the player via alcohol poisoning.
- [x] masks
  - Can be worn on the face like a blindfold. 
  - When worn, you turn into the type of creature the mask is of. 
  - A mask's BUC status degrades when taken off. 
  - Cursed masks will break and damage you when used.
- [x] Eight ball (plastic Orb)
  - base item for the artifact Magic 8-Ball. Does not appear randomly.
- [x] torch
  - (The slashem implementation of torches was a bit cumbersome for the player, you could only have a lit torch by wielding it. I have tried to address that.)
  - Removed requirement for torches to be wielded -(lamps don't need to be wielded, so why would torches?)
  - Unwielding does not snuff the torch.
  - Applying does not auto-wield the torch
  - Dwarves in mines frequently get lit torches now.
  - Putting lit torches into containers auto-snuffs.
  - Clubs can be dipped into oil and turned into torches (from dnh)
- [x] Spoons
    - 1d2 vs small, 1d1 vs large.
    - Convicts starting weapon (uses knife skill)
    - Base item for Iron Spoon Of Liberation.
    - Spoons don't generate randomly.
[x] Lightsabers:
- [x] green lightsaber
- [x] blue lightsaber
- [x] red lightsaber
- [x] double-red-light-saber
- Lightsabers count as weapon/tools
- Lit lightsabers act as a light source.
- We can force locks and break down doors with #force
- We can also cut through iron bars and open iron safes.
- Putting a lit lightsaber into a bag/container snuffs it.


### Gems:

- [x] healthstone
    - Fixed a bug from SlashEM where healthstones would not work under level 10.
- [x] whetstone
    - Made whetstones not suck!
    - We can now apply a stack of whetstones to a stack of weapons. (Before this was limited to applying 1 whetstone to a single object, which was cumbersome and annoying to work with.)
    - Can use (a)pply or #rub with whetstones.
    - Removed the extra penalty for artifacts (how can artifacts resist sharpening?)
    - Whetstones can now remove corrosion (in addition to rust)
    - A blessed whetstone can uncurse a weapon
    - Blessed Whetstones can enchant non-corroded, uncursed, +0 weapons up to +1. (You need to pass a coin flip after the first roll)
    - Cursed whetstones can erode/negatively enchant weapons. (Only takes 1 turn though)
    - Allow using potions of water, roll 1d7 to see if it's used up.
    - You can use whetstones in shallow water, pools, moats, sewage, rust traps, sinks, and toilets.
    - Whetstones took a long to use (too long), lowered the time a little.
    - Cavemen get mega bonus for whetstones
    - Updated usage and repair messages
    - Removed the side effects of fountains, sinks, toilets. In SlashEM, using a whetstone on one of these would also activate the quaffing effects of those features. This duplicates code and also adds too much complexity - after all, we're just using a tiny amount of water to wet the whetstone, would a bunch of Water Moccasins pop out because of that? 


- Added more upgrade paths for gray stones. 
- luckstones and whetstones can be upgraded into healthstones and vice versa. 
- flintstones upgrade into luck/whetstones.

### Containers:
- [x] bag of rats
    - Similar to a bag of tricks, but produces large amounts of rats instead.
- [x] Iron safe
    - A secure container that can only be 'cracked' using a stethoscope. 
    - Cannot be physically locked again once it's been unlocked. 
    - Can appear randomly.
- [x] Crystal Chest
    - cannot be locked, unlocked or forced open by any physical means. 
    - Can only open or lock by magical means. 
    - immune to all other forms of magic
    - cannot be probed, cancelled, or polymorphed. 
- [x] Coffins
    - Coffins can be found in Vlad's tower (or graves) that contain vampires
    - Not actual objects - used the UnNetHack hack that turns large boxes into coffins using +4 enchantment.

### Specific item changes (from Vanilla)

#### Wands

- **Wresting behavior**
    - Went back slightly to vanilla wand wresting behavior
    - blessed = 1 in 7 chance of wresting
    - uncursed = 1 in 23
    - cursed = 1 in 121
- [x] wands of wishing
    - are more rare to find randomly, 
    - are always generated charged once already (1:x). 
    - The Castle wand is still created as having never been charged (0:x).
- [x] wands of death
    - can be recharged only once.
- [x] wands of speed monster
    - Now only grants temporary speed(xnh)
    - Same for potions, they don't grant permanant "fast" intrinsic either.
- [x] wands of striking/force bolt spell:
    - knocks its target back one space with 16+ damage).
- [x] cursed wands of light
    - Cursed WoL cast darkness instead of light.
    - In Evil, zapping around peaceful monsters could anger them. I have removed this mechanic except for when you break a WoL.
- [x] wands of nothing
    - have a base price of 500
- [x] wands of make invisible
    - Invisibility from wands of invisibility is now temporary (Evil)
    - cursed wands will make its target visible if already invisible, 
    - has a 50% chance of aggravating monsters if zapped at yourself while invisible. 
    - If the target is visible, nothing happens.
- [x] wand of digging
    - are no longer restricted to digging a single square on maze levels. (includes spells of digging)
    - zapping a boulder with a wand of digging will vaporize it; doing this in Sokoban will incur a -1 Luck penalty. 
    - Rays from a wand of digging will also halve the current HP of "stony" monsters, including xorns, earth elementals, statue gargoyles and stone golems; strangely, this does not include regular gargoyles.
- [x] Most golems now drop more items related to their material (ie: rope golems drop leashes, etc)
- [x] Angels and Aleaxes can generate with harps.

#### Scrolls

- [x] scroll of genocide -> renamed to scroll of anhillation
    - Changed to UnNetHack behavior
    - Now kills a single monster species on level (uncursed) or globally (blessed)
    - Anhillations from thrones are dungeon-wide
- [x] blessed scroll of teleport
    - Let's you control your destination as if you had teleport control.
- [x] Scrolls of gold detection: 
    - confused reading shows only real traps, not magic portals.
- [x] Confused scroll of identify gives enlightenment

#### Other magic items

- [x] Diluted Potions
    - Evil added diluted effects for many existing potions, I have tried to fill out these effects with the new potions (and some of the old ones) to keep dilution behavior consistent among all the potions.
- [x] booze:
    - drinking it gives 130 points of nutrition if uncursed, 140 if blessed, and 120 if cursed.From dnh
- [x] Acid potions are immune to being destroyed by freezing (xnh)
- [x] Oil is undilutable (xnh)
- [x] Potions of hallucination can give enlightenment (xnh)
- [x] ring of free action
    - 50% chance (with a luck bonus) of slipping free with free action
- [x] Amulets of life saving
    - do not work if cursed or worn by a non-living creature. 
    - This applies to monsters as well as the player.
- [x] Magic markers
    - now never generate randomly, nor will they appear in a player's starting inventory (except the tourist who gets a 0:0 one)
    - Also cannot be polypiled. 
    - Can wish for, find one in a bones pile, looting the High Priest's crystal chest in the Sanctum, or choosing the 'tool' as the Sokoban prize.
    - Increased price to 200 (xnh)
- [x] Magic lamps
    - Changed price to 1000
    - can be wished for. 
    - Cursed magic lamps give off darkness instead of light (same radius); 
    - the aura of darkness overrides any light source.
- [x] Exploding bags of holding
    - scatter their contents in a roughly 10 by 10 area. 
    - Breakable items are subject to breakage from the force of the explosion.
- [x] bag of tricks patch (from UnNethack)
    - The bag of tricks has a bunch of new random effects on apply:
    - When the BoT is empty however, we can use it like a normal bag!
    - When the BoT has 0 charges, it is safe to put in a Bag of Holding.
    - Adjusted BoT #tip behavior - it stops when it reaches a non-monster effect.
      (This is a compromize between BoT behavior between UnNethack and Evil.)
- [x] Unicorn horns:
    - no longer cure attribute loss;
    - they can now be wielded in one hand.
- [x] luckstones:
    -  will slow down your luck timeout based on its beatitude;
    - a blessed luckstone no longer halts the timeout entirely.
- [x] Most magical tools have had their prices increased (xnh)

#### non-magic items

- [x] Pick-axe: 
    - Changed weight to 75 aum (From Slash'EM)
    - Digging down doors with pickaxe is not quiet anymore.
- [x] crysknife: 
    - Changed sm/lg damage to d20/d30 damage (From SlashEM)
    - Long worms only drop a worm tooth 1 in 20 times.
- [x] fedora
    - Now acts as luckstone and grants +1 charisma
- [x] iron shoes were renamed to 'dwarvish boots'.
- [x] Robes occupy the body armor slot instead of the robe slot (SlashEM)
  - This means that dragon scaled robes are possible in HackEM!
- [x] Towels weigh more than blindfolds
- [x] readable conical hats: For a Tourist, 1⁄3 of conical hats can be read; a dunce cap will be labeled "DUNCE". Reading a dunce cap autoidentifies it. For a Tourist, 1⁄3 of conical hats can be read. A cornuthaum will be labeled "WIZZARD". (3.7)
- [x] Free Fortune Cookie Patch; there is a 50% chance for a szechuan tin to contain a free fortune cookie (UnNetHack)
- [x] Adjust flint stone weight from 4 to 1
- [x] Sling buffs: strength bonus, potential to instakill H

### General item changes

- [x] Many body armor types have had their weights reduced.
- [x] Many types of potions will have their effects diminished if diluted.
- [x] Any item or object that is susceptible to being rusted, corroded, rotted, burned, or fractured can be completely destroyed via those methods unless fixed.
- [x] Any item or object can be fixed (wielding the object and reading a scroll of enchant weapon while confused) if its material allows for it.
- [x] Monsters can drink restore ability to cure cancellation, or you can hit a monster with a potion of restore ability to forcibly uncancel them.
- [x] Object materials – many objects and items can spawn made from something other than its base material.
- [x] Object properties – various weapons and armor are sometimes created with magical properties.
- [x] Changed chances of item randomly spawning with two properties to 1 in 666
    - (In Evil the chances are 1 in 100000! K2 what?!)
- [x] All edged weapons can be poisoned (ex: daggers, swords)
- [x] Removed cursed weapons reverse hitting (was in EvilHack)
    - Cursed weapons will miss 25% of the time now instead of 20% to compensate.
- [x] Launchers now contribute to damage.


[x] 1/40 of random non-artifact weapons will be generated erodeproof (xnh)
[x] 1/40 of random non-artifact items will be generated as pre-eroded (xnh)
[x] Grease will generate on an erodable object every once in a great while (xnh)



- [x] breakable locking tools (keys/lockpicks/credit cards) 
  - non-blessed have a 5% chance of breaking with each use, whether locking or unlocking.
  - Rogues have only have a 1 in 60 chance of breaking a non-blessed lock pick.
  - Tourist have only have a 1 in 40 chance of breaking a non-blessed credit card.
  - cursed have a 20% chance of breaking each use.
  - Artifact locking tools are not affected.

- [x] All musical instruments can now break 
    - Artifact instruments are safe from breaking on apply.
    - If you are fumbling, afraid, or the instrument is cursed, there is a 1 in 8 chance it will break when you attempt to play it.
    - Uncursed instruments have a 1 in 50 chance of breaking.
    - Blessed instruments won't break
    - If you are an elf, you will never break an instrument this way.

Imported some appearance effects from dNetHack:

- [x] Combat boots grant +1 AC and attack
- [x] Mud boots make you more resistant to wrap attacks (dnh)
- [x] Hiking boots grant +100 to carrying capacity
- [x] Jungle boots prevent many types of leg injury
- [x] padded gloves +1 AC 
- [x] old gloves can't be eroded
- [x] fencing gloves +2 attack with one handed weapon and no shield

## Spells/Spellcasting

### Spellbooks:

- [x] spellbook of acid blast
- [x] spellbook of command undead
    - Lowered command undead spell difficulty to 4
- [x] spellbook of enlighten
- [x] spellbook of flame sphere
    - Evil made this a level 2 spell
- [x] spellbook of freeze sphere
    - Made a level 1 spell (as in slashem) - levels out early game a bit more for ice mages.
- [x] spellbook of lightning 
- [x] spellbook of passwall 
    - Moved to ESCAPE category
    - Lowered spell level slightly to 5
- [x] spellbook of poison blast
    - Reduced to level 2 so the infidel has a decent starting spell at an affordable price.
- [x] spellbook of reflection
    - Creates a temporary reflective bubble around the caster, 
    - acts exactly the same as an amulet of reflection (or the like)
- [x] spellbook of repair armor
    - repairs one level of damage to any worn piece of armor. 
    - Will not make armor items fixed. 
    - Works as a foil to the new monster spell 'destroy armor'
- [x] spellbook of sonicboom
- [x] spellbook of summon undead
- [x] spellbook of invisiblity - now a level 1 spell (to compensate for the lack of permament intrinsic invisibility)

- [x] spellbook of fire bolt (new Hack'EM special!)
  - Fire Bolt is only available to the Flame Mage and will not generate randomly.
  - 1d10 fire damage (avg 5.5).
  - A fire attack (similar to force bolt) that doesn't reflect.
  - Can hit multiple targets, destroys doors, trees, webs, fountains
  - Destroys straw/paper golems
  - Damage scales with level:
    - Level 1 = 1d10 fire damage
    - Level 4 = 1d10 + 1d8
    - Level 8 = 1d10 + 2d8
    - Level 12 = 1d10 + 3d8
    - Matter at Skilled adds +1d4
    - Matter at Expert adds +1d4

### Spellcasting:

- [x] Got rid of the BODY spell class in SlashEM, moved to the Escape spells
- [x] Got rid of the PROTECTION spell class in SlashEM, moved to the CLERIC spells
- [x] Got rid of the CLERIC spell class and created a NECROMANCY spell class to support the new spin on the Necromancer role. The spells from the CLERIC school have been distributed among the other schools as appropriate.
- [x] Increased available max spells by 10 (Added 0-9 for spell list letters)
- [x] Spellbook weights are now based on their level
- [x] Spellcasting while wearing body armor will negatively affect your spell success rate.
  - The exception to this is crystal plate mail, which does not adversely affect spellcasting in any way.
- [x] Casting any spell successfully adds +500 turns to your memory of it (from slashem)

### Spell changess:

- [x] spellbook of Confuse monster – level 2 → 1
- [x] spellbook of cure sickness
    - is now directional 
    - can be cast at other monsters as well as yourself.
- [x] spellbook of charm monster – level 3 → 5
- [x] spellbook of cone of cold
    - changed from an attack spell to a Matter spell.
- [x] spellbook of Detect food – level 2 → 1
- [x] spellbook of Detect monsters – level 1 → 3
- [x] spellbook of Detect unseen – level 3 → 2
- [x] spellbook of Dig – level 5 → 3
- [x] spellbook of fireball 
    - changed from an attack spell to a Matter spell
- [x] spellbooks of flame sphere, firebolt are immune to fire.
- [x] spellbook of Invisibility – level 4 → 3
- [x] spellbook of Jumping – level 1 → 2
- [x] spellbook of Restore ability – level 4 → 2
- [x] spellbook of Sleep – level 1 → 2
- [x] spellbook of teleport away:
    - Lowered to level 5

### Monster Mage spells

- [x] Touch of death
    - if you have MR you'll take damage (8d6) and your maximum hit point level can be reduced slightly.
    - Having MR and half spell damage together can reduce those effects even more.
- [x] Cancellation (new)
    - Acts just like having a wand of cancellation zapped at the player.
    - The spellcasting monster must be lined up with the player before it can cast this spell.
- [x] Acid blast (new)
    - A powerful area of effect spell, does acid damage to its target.
    - The damage output is dependent on the level of the monster casting it.
    - The acid from this spell also has a chance of eroding any unprotected weapons or armor in open inventory.
- [x] Destroy armor
    - Vanilla behavior – if the player didn't have magic resistance, one piece of armor would be completely destroyed.
    - In HackEM, any piece of worn armor can have its fixed status removed, and then deteriorated.
    - Even armor that is normally erodeproof is affected.
    - Having MR keeps the erosion level at one per cast, otherwise 1-3 levels per cast.
    - Armor-based quest artifacts are immune to this spell, as is crystal plate mail.
- [x] Fire bolt	(new)
    - A small area of effect spell, this spell explodes a small fireball upon its target.
    - Any flammable objects in open inventory are subject to being burned.
    - low-level spell, any spellcaster has access to it.
- [x] Ice bolt (new)
    - A small area of effect spell, this spell explodes a small ice storm upon its target.
    - Any non-protected objects in open inventory are subject to being frozen.
    - low-level spell, any spellcaster has access to it.
- [x] Reflection (new)
    - Creates a shimmering globe around the caster, granting them reflection for several turns.
- [x] Create Pool (from SlashEM)
    - Affected by displacement and invisibility now.
- [x] Call Undead (from SlashEM)
- [x] Summon Monsters/Nasties 
    - Enemies have to see you in order to cast this spell now.
    - Implemented fix #K669 - 'nasty' monster summoning. 
    - chain summoning nerf. tldr: "If a replacement creature is a spellcaster it now has to have lower difficulty than the summoner."

[x] Spellcasters now have their spell range limited by their level (gnomish mages are level 3 and can only cast fire bolt in a radius of 2-3 tiles instead of 13)


### Monster Clerical spells:

- [x] Summon minion	(new)
    - Summon a type of minion based on its alignment.
    - Demons that can cast clerical spells are not given access to this spell (they can already demon-gate).
- [x] Geyser
    - A powerful spell that causes a large geyser of water to slam into its target, causing physical damage (8d6).
    - What's new: can now rust any unprotected worn armor.
- [x] Vulnerability	 (new)
    - It's target temporarily becomes vulnerable to either fire, cold, shock or acid-based attacks,
      dropping its resistance (if any exists) by 50%.

[x] Fix unstoning monster spell timeout. Changed timeout for monster spell of stone to flesh to timeout 2d7 instead of rn2(7).

## Artifacts:

**Design decision #1:**

    In SlashEM, artifacts grant a base damage bonus - should we adjust so that they deal closer to that on average? Currently, we'll say no and see how things play out. (Except for Doomblade which I specifically want close to slashem levels.)

**Design decision #2:** 

    Any roles that had 2 guaranteed altar sacrifice gifts have been reduced to just 1. Having 2 just seems excessive, and most of the time, the second gift was just a barrier to overcome to get to better stuff (example: Deathsword for Barbarian, or even Deluder for Wizards)
    
- [x] Players can twoweapon with an artifact in each hand, if those artifacts happen to get along with each other.
- [x] When sacrificing, your deity may gift you a regular piece of gear instead of an artifact.
- [x] Added much stronger artifact blasting (6d6 to 8d10) (from Slashem)
- [x] If wielding/wearing any of the 'banes, taming becomes impossible */
- [x] Wishing for artifacts: 
    - Instead of being based off of how many artifacts exist in the game total (vanilla NetHack method), success is based off of how many artifacts the player has wished for (successful or not). 
    - ALSO - the odds are high that it comes with its owner...

### New artifacts
- [x] Balmung
    - (destruction proof, but destroys armor like a boss
- [x] Bat from Hell
    - Rogues no longer receive this as a guaranteed sacrifice gift.
- [x] Bradamante's Fury
- [x] Carnwennan
- [x] Chains of Malcanthet
    - Currently cannot wish for.
    - Changed to target Angels instead of demons
- [x] Circe's Witchstaff
    - Changed to a STAFF_OF_MATTER;
    - poly attack upped to 10%; can change enemies into pigs or feral hogs.
- [x] Deathsword
    - Barbs no longer receive this as a guaranteed sacrifice gift.
    - Deathsword causes hostility from humans
    - Deathsword occasionally knocks the opponents unconscious.
    - Changed from targeting vs Human to vs cross-aligned.
- [x] Deluder
    - Wizards no longer receive this as a guaranteed sacrifice gift.
- [x] Deep Freeze
    - freezes the ground and summons freeze spheres.
    - Only Ice Mages can invoke this.
- [x] Disrupter
    - Switched Disrupter from being a priest sac gift to being Undead Slayer sac gift.
    - Disrupter causes hostility to undead.
- [x] Doomblade
    - Deals an extra d19 to make up for solid +10
    - Shows an appropriate message for tiny monsters.
- [x] Drowsing Rod (Hack'EM)
    - (Staff of Healing) +d5 to-hit, +d8 dmg
    - This is meant to simulate the original drow attack from slashem, so 19 in 20 times the sleep attack triggers for 2d4 turns.
    - It's a gas vapor attack so breathless monsters are not affected.
    - Grants sleep res
    - Healer's first sac gift.
- [x] Elfrist
    - Warns against elves.
    - Elfrist causes hostility to elves
- [x] Firewall
    - Changed into a Staff of Divination
    - Firewall can generate Fire elementals (moved from Candle)
    - Only Flame Mages can invoke this.
- [x] Gauntlets of Defense (Monk sac gift)
- [x] Gleipnir
- [x] Gungnir
    - Upped to +20 tohit, +1d12 from dnh.
    - When a player creates Gungnir, it now converts to an atgeir (type of spear)
- [x] Hand Grenade of Antioch
    - This will not be lit accidentally by sources of fire!
- [x] Imhullu
- [x] Luckblade (already in evil)
- [x] Luckless Folly
- [x] Master Sword, The
    - +3 to-hit and +1d3 dmg)
    - Was a "named" silver long sword in the village level,
    - shoots magic missiles when you are at full health?
    - 10% chance of melee magic missile attack
    - Cannot be wished for. The master sword now has a 75% chance of appearing in the middle of the Town branch.
- [x] Mirrorbright
    - Mirrorbright no longer hinders spell-casting.
    - Healers no longer receive Mirrorbright as a sacrifice gift, they get the Drowsing Rod instead.
- [x] Mortality Dial
    - Was COLD_IRON in splice, but made it silver.
    - Cancels trolls and zombies from reviving.
- [x] Mouser's Scalpel (slashem-up)
    - a rapier which gives +5 to-hit bonus and +1 to-damage. Since rapiers are not randomly generated, one can only find a naturally generated Mouser's Scalpel in Grund's Stronghold.
- [x] Mystic Eyes (Previously known as Lenses of Death Perception)
    - Grants DeathVision
    - As long as the wearer is not blind, they deal double damage, but constantly hallucinate.
    - Grants searching.
    - Deathvision's double damage is applied everywhere it should be (including spells/wands/
- [x] Origin
    - STAFF_OF_ESCAPE
    - While wielded, offers great bonuses for spellcasters.
    - Increases spellcasting ability as a robe would
    - Gives a discount of 5 energy for spells that costs 10 or more.
- [x] Plague
    - confers sickness res while carried (from dnh)
- [x] Poseidon's Trident
- [x] Pridwen
    - Also grants STABLE.
- [x] Quick Blade
- [x] Reaper (Yeoman)
    - Converted to Copper so that elven Yeoman can receive it as a gift.
- [x] Serpent's Tongue (Necro)
    - Change to a parazonium, gets +d5 tohit
- [x] Skullcrusher
    - Replaced Keolewa with Skullcrusher.
- [x] Spear of Light (Renamed from the Holy Spear of Light)
    - Spear of Light causes hostility from undead
    - First sac gift for Undead Slayers
- [x] Sword of Balance
    - Conveys "extreme stability" (hurtling resistance)
- [x] Sunspot
- [x] Sword of Justice
    - Yeoman no longer receive this as a guaranteed sacrifice gift.
    - changed to broadsword
- [x] The End
    - Changed to Chaotic.
- [x] Thiefbane (One-eye sam)
    - Revamped thiefbane: No longer targets humans, so now the player can wield it if lucky enough to get if off of Sam.
    - Now a two-handed sword (from Un), +d5 to-hit, +1 dmg
    - Changed Thiefbane to platinum so elves can wield it.
    - Raised to 10% beheading, will be
    - Cancels thiefs and covetous
    - Protects from thievery and seduction.
    - Moved from chaotic to non-aligned.
- [x] Thunderstruck (previously known as Sonicboom)
    - Increased attack to AD_LOUD(5, 20) - gotta make up for the item loss.
    - Destroys glass, ruby, sapphire, crystal, diamond golems?
    - Added 25% chance of sonic beam shooting out.
- [x] Wallet of Perseus
    - Replaces the Bag of the Hesperides (from Evil)
    - Keeps MC1 and oilskin properties, can wish for.
- [x] Werebane (already in evil)
- [x] Whisperfeet (Tourist sac gift)


### Quest Artifacts:

- [x] The Candle of Eternal Flame (Flame Mage)
    - Changed to Lawful alignment
    - Fixed so it's always lit
    - Removed the #invoke for Fire Elemental ability and moved to Firewall.
    - No longer grants cold resistance, instead grants extrinsic fire resistance.
    - Confers teleport control and warning
    - Also confers faster energy regeneration (like Encanto!)
    - Grants a passive 2d7 fire attack
- [x] The Storm Whistle (Ice Mage)
    - Changed to Neutral alignment
    - No longer grants fire resistance, instead grants extrinsic cold resistance.
    - Can now #invoke for 1 of 10 random "storm" related pets:
- [x] Great Dagger of Glaurgnaa (Necromancer)
    - Only confers MR when wielded.
    - [x] Grants double spirit bonuses when carried
	- [x] Also triples the radius of spirit auto-collection when carried.
- [x] Crown of Saint Edward (Yeoman)
    - Only confers MR when worn.
- [x] Stake of Van Helsing (Undead Slayer)
    - Only confers MR when wielded.
    - Now warns of vampires.
- [x] Iron Spoon of Liberation (Convict)
    - Adapted from dNetHack
    - Replaces the Iron Ball of Liberation
    - Carrying the spoon confers free action
- [x] Staff of rot (Chaotic Quest, chaotic aligned)
    - Base item: Staff of Necromancy
    - Wielded by Acererak. Very strong, but with a cost..
    - Has a special wither attack
    - While wielding, inflicts a passive wither attack (and occasional fear)
    - If you are wielding and withering, you deal double damage.
    - #invoke for withering and aggravate monster
    - Cannot be wished for.
    - 
- [x] Xanathar's Ring of Proof (Neutral Quest, neutral aligned)
    - Base item: Ring of See Invisible
    - Guarded by Xanathar
    - Grants see invisible, warning while carried.
    - Grants magic resistance while worn.
    - Invoke for invisibility
    - Cannot be wished for.
- [x] The Key Of Access (Lawful Quest, Lawful aligned)
    - Guarded by Nebuchadnezzar.
    - Invoke for branchport
    - Cannot be wished for.

### Changes to existing artifacts:
- [x] Demonbane
    - switched from a long sword to a silver heavy mace,
    - first sacrifice artifact gift for Priests.
    - Can be invoked for Flying.
- [x] Dragonbane
    - switched to a pair of dragonhide gloves
    - Now provides reflection, acid resistance, and warns against dragons.
- [x] Excalibur:
    - Only lawful Knights can dip for Excalibur,
    - and there's a danger that the sword will rust away completely when you do (if not fixed).
- [x] Eyes of the Overworld
    - now protect the wearer against many forms of gaze attacks,
    - greatly reducing the need for the player to have to remove them to blind themselves with a blindfold or towel.
    - Take note, Medusa's petrifying gaze attack is too powerful and will overcome the EotO's protection.
- [x] Fire Brand and Frost Brand are now steel short swords.
    - Engraving with Fire Brand burns the text on the floor
- [x] Grimtooth
    - now has a sickness-inducing attack
    - has a 1 in 6 chance of making its target terminally ill.
- [x] Magicbane	
    - no longer an athame, switched to a quarterstaff. 
    - None of its special abilities or functions have otherwise been changed.
    - Putting Magicbane into a bag of holding can explode the bag (and even destroy itself).
- [x] Magic Mirror of Merlin
    - has had magic resistance removed. 
    - In its place is reflection and half spell damage when carried.
- [x] Mitre of Holiness 
    - allows #pray and #turn to function in Gehennom when worn.
- [x] Mjollnir
    - base type is now heavy war hammer
- [x] Ogresmasher
    - base type is now heavy war hammer
- [x] Sceptre of Might:
    - base type changed from a mace to a rod, deals slightly more damage per hit.
- [x] Sting/Orcrist changed to Lawful (same alignment as elves)
- [x] Staff of Aesculapius: also cure afraid and larval infection.
- [x] Sunsword
    - made of gemstone (crystal)
- [x] Tsurugi of Muramasa
    - Raised bisection chance to 15%
    - now confers half physical damage when wielded.
- [x] Vorpal Blade
    - Raised beheading chance to 10%.

- Various artifacts that have a nemesis monster warn against those monsters.
- Almost all of the stock weapon artifacts have had their to-hit and damage modifiers adjusted in the players favor.
- Certain artifacts can either deliver extra damage to their nemesis monsters, or have a small percentage chance of delivering a fatal blow – this applies to the player as well!

## Dungeon Features

### Trap Mechanics:

- [x] Added ice traps (Un)
  - Ice traps only generate after level 8
- [x] Can #untrap rust traps (they turn into fountains)
- [x] Can #untrap fire traps (requires water)
- [x] Untrapping a spear trap has a chance of yielding a spear or stake
- [x] Allow forcefighting of webs with a bladed weapon to destroy them
- [x] The Castle drawbridge does not always close with the passtune
- [x] Invisibility from magic traps is temporary.

#### Breaking wands:
- Breaking wands will usually create related traps (unnethack)
  - wand of cold 		-> ice trap
  - wand of opening 	-> trap door
  - wand of fire 		-> fire trap
  - wand of magic missile -> magic trap
  - wand of teleportation -> teleport trap
  - wand of polymorph 	-> poly trap
  - wand of sleep 		-> sleeping gas trap
  - wand of cancel 		-> antimagic trap
  - wand of water		-> rust trap
  - wand of shock		-> magic beam trap
  - wand of poison		-> magic beam trap
  - wand of acid		-> magic beam trap
  - wand of Sonic		-> magic beam trap

- wand of secret door detection: Detects traps and portals.

### Toilets:

	- I know toilets are a bit silly, but I have enhanced them so they are more
	interesting and useful!

- Toilets can appear separate from sinks, in slashem they only appeared in pairs with sinks.
- Enabled fishing in toilets
- Dropping a ring of poly into a sink can poly into a toilet.
- Prevented toilet prayer while levitating.
- Toilet prayer can now stop the vomiting process (before it could not and you could vomit and still be in the vomiting process)

#### Toilet #kicking:

- Kicking now only breaks the toilet 1/7 chance (was 1/4 in slashem)
- Kicking can generate cockroaches and sewage from kicking (1 in 17 chance)
- Kicking can generate brown puddings (only once per toilet)
- Kicking can generate a random tool, normally this tool will weigh under 15aum, but sometimes you'll get a large tool that bonks against the piping.
  - If a large tool bonks 3x - you'll get that tool no matter now big it is and the toilet is destroyed in the process.		

#### Amulet identification in toilets (From Slex)

- Amulets can be dropped down toilets and possibly identified (similar to sinks and rings)
- Updated amulet feedback messages, blind effects, and hallucination effects.
- Amulets of Flying are always regurgitated from toilets.
- Amulets of Change will polymorph the toilet into a sink

### Forges:

- Forges are introduced, which have the same appearance as fountains but are orange in color. Just like fountains, dipping things into them can have desirable or adverse affects. They can be used to dispose of anything that is flammable, and can also be used to repair corrosion/rust damage to any metallic object (if your luck is greater than 0). There is a rare chance that a lava demon can be summoned via dipping, which will be hostile most times, but can sometimes spawn tame (same odds as getting a wish from a water demon/fountain). Having negative luck can cause a forge to explode when used, which can cause considerable fire damage. Players can also use a forge to remove an attached ball & chain, and can also combine two objects to forge something new using the #forge command.
- [x] Added new recipes for various imported weapons
- [x] Forges don't appear until after level 2.

## Mechanics:

- [x] If the player tries to hit a monster with a weapon they are restricted/unskilled in, or any non-weapon object, they'll never have better than a 75% chance of landing a hit (bare-handed is exempt).
- [x] The player is unable to regenerate hit points while in the Valley of the Dead – healing via other methods (potions, healing spells) is still possible.
- [x] Stoning – monsters being stoned is a slow-stoning process, just as it is for the player.
- [x] Merged scimitar and saber skills.
- [x] Add celibate conduct (slashem)

### Elbereth
- You cannot use Elbereth until you've learned it in-game (via rumor, reading it, etc).
- Conflict negates Elbereth.
- Elves & hobbits start out knowing Elbereth; Orcs can never learn it.

### Extended Commands
- [x] #force can also be used on doors
- [x] #loot can be used at your pet to both give and take away items from its inventory.
- [x] #borrow command (Evil has the "Thievery" skill for Rogues)
- [x] #naming glitches have been nerfed - Players can no longer #name items for identification clues.
- [x] The mysterious force has been removed.

### Resistances/Intrinsics

- [x] Intrinsic resistances are no longer binary, but are percentages instead. The spread is 5% to 50% resistance gained from eating per corpse. Damage reduction from various attacks is determined by how much partial resistance the player has to it.
- [x] Reflection is not 100%; it can lessen the effects of an attack it reflects, but will never negate it.
- [x] The "shower of missiles" monster attack is not completely negated by magic resistance.
- [x] See Invisible is now just temporary from potions, eating stalkers, and fountains (Evil)
- [x] Telepathy from floating eyes is now temporary.
- [x] Restful sleep regenerates hp (we can get restful sleep property from the Amulet of Restful Sleep or a Ring of Sleeping)
- [x] The hunger property halves nutrition from food (so you could potentially eat more giant corpses.) (xnh)
- [x] Extrinsic resistances protect items from elemental damage. For example, wearing a ring of fire resistance will protect your scrolls and potions from fire damage.

- [x] Implemented "Afraid" status; 
    - The player can sometimes become too afraid to control their actions.
    - similar to the effects of being stunned or confused. 
    - Caused by dragon roars and some monster attacks.
    - 
- [x] Add "Stable" property
    - Protects you(or monsters) from clobber and hurtle attacks.
- [x] Added Sonic Resistance property (from Splice)
    - Protects you(or monsters) from sonic attacks.
    - protects you from dragon roars
- [x] Added Psychic Resistance property (from Splice)
    - Protects you from forgetting skills, spells, and other things from potions and scrolls of amnesia, as well as mind flayer and zombie attacks.
    - Protects you from an illithids psionic wave and tentacle attacks.
    - Protects from most fear attacks, including dragon roars.

- [x] Added LarvaCarrier property - some monsters can infect you with eggs!

### Upgrading/Tinker
- [x] Upgrading mechanic (via potions of gain level from Slash'EM)
    - [x] Instead of crystal balls upgrading to magic markers, they upgrade to lenses (going to markers didn't make much sense, and it wasn't too useful since the marker would only have a handful of charges)
    - I removed a lot of the weapon upgrades, since now a lot of those are covered by forge recipes. A few still remain for quarterstaves, boomerangs, and other wooden weapons.
    - I also added upgrade paths for all special new armors
    - The flintstone to healthstone exploit has been fixed; flintstones only upgrade one at a time.

#### Scare Monster: 

- Several more creatures are immune: 
  - any unique monsters including the Wizard of Yendor, quest nemeses, the Riders.
  - lawful minions, Angels (A), Archangels
  - humans (any @), player monsters
  - honey badgers,
  - anything that is mindless (EvilHack added)
- Any monsters affected by conflict are also immune.

#### Altar sacrificing

- Sacrificing for artifacts has been significantly changed in EvilHack/HackEM – there is a chance that your deity will gift you a regular item instead of an artifact. 
- The odds of being gifted an artifact go up as you increase in experience level. 
At XP 4, there's a 10% chance for an artifact gift, and the chances increment exponentially from there. 

- Regular object gifts are either be a weapon or a piece of armor
- For primary spellcasting roles (priest/wizard/etc), there's a chance of receiving a spellbook also. 
The gift received will always be blessed, enchanted, fixed, and there is a 1 in 8 chance that a desirable object property will be added to that object.

- Prayer timeout and the chances of receiving another gift are affected in the same way as if you had received an actual artifact.

#### Twoweaponing

- Twoweaponing has been changed so that the weight of your offhand weapon combined with your skill in twoweaponing has a significant bearing on how successful you'll be in landing a hit. As you increase your skill in twoweaponing, heavier weapons become a viable option to use in your offhand without penalty. That penalty is −30 to-hit, and you'll receive feedback stating that your offhand weapon is too heavy to wield successfully as you try to use it. Below is a listing of what weights and weapons are acceptable per your skill in twoweaponing:

- Unskilled – maximum weight of offhand weapon is 20 aum (can use tridents, javelins, crysknives, daggers, unicorn horns, or anything lighter)
- Basic – maximum weight of offhand weapon is 30 aum (can use short swords, spears, or a mace)
- Skilled – maximum weight of offhand weapon is 40 aum (can use sabers or long swords)
- Expert – maximum weight of offhand weapon is 70 aum (can offhand any one-handed weapon)
- If playing as a giant, or if wearing gauntlets of power, the maximum weight of your offhand weapon can be 200 aum, meaning you can basically offhand any normal object. Bear in mind that object materials can alter an items base weight – at basic twoweaponing skill you could offhand a normal short sword without penalty, but not a silver short sword, as silver adds extra weight compared to iron (33 aum vs 30 aum). But at the same time, a mithril katana (15 aum) would be perfectly acceptable at the same skill level as mithril is much lighter than any other metal in the game.

#### Conflict:

- When causing conflict, it now takes the player's charisma score into account. 
- The formula is 'charisma - monster level + player level' 
- if that value is greater than 19, clamps it to be 19. 
- There's then a percentage chance (rnd(20)) against the value generated on whether conflict kicks in or not. 
- So even with charisma and your xp level maxed out, there's still a 5% chance of conflict not working. 
- Also, the monsters affected have to see you for conflict to work.

#### Dexterity:

- Dexterity can directly affect the player's AC, for better or worse. 
  - being encumbered, or wearing any kind of heavy metallic body armor (read: not mithril) or other rigid material (bone, stone, wood) will negate any favorable armor class bonuses from Dexterity.

#### Amnesia/Forgetting:

- Potions of amnesia can also inflict this effect.
- Can make you lose Elbereth knowledge if you had it.
- Can make you forget forget about shambling horrors.
- Psychic resistance protects from amnesia forgetting. Sources include rings of psychic resistance and being an illithid.

#### Quality-of-Life/Auto-ID:

- [x] Object lookups show more information, including object properties and special effects from materials.
- [x] Artifacts can be looked up in the item database (currently only works if accessed through the inventory, requiring you to have possession of the artifact)

- [x] All roles start out knowing potions of water and scrolls of identify
- [x] Auto-ID dunce cap upon wearing
- [x] Auto-ID jumping boots (from AceHack)
- [x] Auto-ID some potions when inhaling their vapors
- [x] Auto-ID scrolls of scare monster during pickup
- [x] Auto-ID scrolls of teleportation when teleporting any distance after reading one
- [x] Auto-ID wand of teleportation on seeing the successful effect on a monster.
- [x] Auto-ID wands with unambiguous effects and messages
- [x] Auto-ID wand of undead turning if engraving produces a message
- [x] Auto-ID wands of probing and secret door detection when engraved with
- [x] Auto-ID vanishing wands if the other 2 have been identified.
- [x] Auto-ID wands of sleep or death if their counterpart has been identified.
- [x] Auto-ID wands of healing or extra healing if their counterpart has been identified.
- [x] Auto-ID rings dropped into a sink when message is shown
- [x] Auto-ID for ring of regeneration
- [x] Auto-ID whistles on use.
- [x] Audo-ID magic flutes and harps on use
- [x] Auto-ID magic lamps and oil lamps when rubbing
- [x] Auto-ID loadstones upon discovering the inability to drop them.
- [ ] Auto-ID flintstones when rubbed on iron weapons.
- [x] Auto-ID water walking boots
- [x] A blessed scroll of charging will (in addition to charging the item) also reveal the number of charges and the charge counter.
- [x] Non-cursed charging identifies how many charges an item has (from AceHack).
- [x] You can reread a spellbook to refresh your memory at any time
- [x] After a while of using a wielded weapon, you will identify it and it's enchantment
- [x] Blessed stethoscopes can identify eggs (on the floor)
- [x] The player's magic cancellation (MC) is shown in the status bar.
- [x] Show the last turn the player prayed in the ^X screen
- [x] Only blessed scrolls of charging identify the charges of an item.
- [x] Remove "Auto-select every item" when putting into containers
- [x] Default shk sell prompt to N (xnh)
- [x] Regeneration only causes additional hunger when injured.
- [x] Removed two-weaponing restrictions imposed in EvilHack.
- [x] Monks eventually stop receiving the "You feel guilty	message	(xnh)
- [x] Monsters cannot use wands of wishing.
- [x] Chargeable rings may get charged when hit with electricity (3.7)
- [x] Useless +0 rings are generate very rarely (xnh)
- [x] The Astral Plane is always lit. (xnh)
- [x] The passtune range has been extended by 1. (xnh)
- [x] Switching between your main and alternative weapon should not take any turns (from dnh)
- [x] Deliberate level teleporter activation ignores magic resistance (3.7)
- [x] /> < to auto-travel to stairs (from Unnethack)
- [x] Control+Direction now kicks in that direction.
- 

Object lookup has been fleshed out:
- [x] Potions, scrolls, rings, amulets, wands will display lookup info.
- [x] Artifacts will show their info (must have the artifact in possession)
- Weapons show bonus damage and effects resulting from materials.


#### Religion/Prayer/Alters:

- Make temple priests hostile if made on an unaligned altar.
    (This means the VoTD priest is always hostile now - no free donates kiddies!)
- Sacrifice offerings can sometimes bless random objects (From SlashEM)

#### Minions

- Slashem had a mechanic for getting aligned minions as gifts from your god when you sacrifice or pray with low health. I ported most of this mechanic over but added some severe limitations because in my opinion, minions were broken in slashem and too easy to obtain (Lawfuls could get Solars and Planetars!).
- Here are the new limitations; minions are not granted when:
    - You are under level 5.
    - You have negative luck.
    - You have been crowned
    - You have completed the quest
    - You have been expelled from the quest
    - You have pets on the current level.

- The player will also have to hit a 1 on this roll: 
    - 1 in 10 + (4 * number of gifts granted)

- Minion pets cannot betray you.

#### Dungeon growth patch

- Another feature ported over from Slash'EM
- This enables trees to slowly branch out in the dungeon and occasionally drop ripe fruits/leaves.
- It also promotes the growth of herbs left on the ground or water.
- Herbs that are left unattended will occasionally sprout more of the same kind around surrounding tiles: these include garlic, wolfsbane, catnip, carrots, kelp fronds, and mushrooms (technically not herbs but they do spread prolifically). Herbs in the water will move with water currents.
- One strategy to try is leaving these items on the ground early in the game and then coming back after a few thousand turns to see what you have "farmed."

#### alchemy

- Alchemic blast damage is double, but acid resistance halves it (xnh)
- Incorporate Malcolm Ryan's Brewing Patch
    - enables the player to dip mold corpses in potions of fruit juice to start a fermentation timer that will eventually turn the potion into something else dependent on the mold.
    - Dipping green mold into fruit juice produces acid. 
    - Dipping red mold into fruit juice produces booze
    - Dipping brown mold into fruit juice produces sleeping 
    - lichens and shriekers and non-F corpses do not ferment.
    - Dipping cursed molds into fruit juice produces sickness.
    - This also makes the shopkeeper charge you for shop-owned fruit juice
that you begin fermenting. Allows you to buy the new, possibly more
expensive potion at the cost of the fruit juice.

## Dungeon Changes:

- [x] Adjust secret passage frequency and exclude them from DL 1-4 (xnh)
- [x] Random doors are secret less of the time (xnh)
- [x] Kicking a "wall" that is actually a secret of some sort will never injure the player and will always produce an unambiguous message that there's something there. This makes it a valid strategy to kick every wall suspected of hiding something, because a single kick will give a yes/no on whether it's a regular wall or not. It's still not a *great* strategy, because the real walls will still hurt and wound legs like normal. (xnh)


- [x] Extend main dungeon: Now 31-34 levels deep
  - (Slashem was 42-47, but this went over the max)
- [x] Adjusted ludios (appears levels 10-30)
    - Ludios appears on the first eligible level.
- [x] Adjusted oracle (appears levels 5-9)
- [x] Adjusted Nymph level (appears levels 8-30)

- [x] Removed Goblin Town and requirement to defeat Goblin King (Evil)
      Commit: https://github.com/k21971/EvilHack/commit/7829e667eec8aa393821ada29c38b5160845c3e7

- [x] Gnomish Mines:
    - Include new different types of gnomes, dwarves, and thieves.
    - Added dwarf/gnome thieves to most town/minetown variants
    - Removed river/sewage generation from Gnomish Mines
    - Gnomish mines monsters will usually be undead if you are a dwarf, gnome, or hobbit (from SlashEM)
      - This can make the mines a very dangerous place so beware!
      - Update: Instead of making undead mine denizens dependant on your alignment, the chance scales with the dungeon deptch. At the top of the mines, undead should be quite rare, but at mine's end beware.

    - Imported minetown variants: 
        - Mini-Castle (SlashTHEM)
        - Creek Town (SlashTHEM)
        - Ruined & Dangerous Town (SlashTHEM)
        - Lavender Town (SpliceHack)
    - Mine's End levels, added:
        - Gnome King's Apiary (slashthem)
        - Boulder Bonanza (slashthem)
    - Added Gnome King endings:
        - Ruggo the Gnome King's level from slashem
        - Ruggo the Gnome King's level from slashthem
        - Moved The Sewers of Waterdeep into the Gnome King's ending instead, 
            replaced rat king with Ruggo.
        - Create a variant of the dnh drow noble quest level (Ndr-loca")

- [x] Sokoban
    - The last room of Sokoban has more prize variety, with a catch.

- [x] Imported the town branch from SlashTHEM/UnNethack
    - Appears 2-3 levels below the Oracle.
    - Imported Mall levels from SlashEM and added to town variants.
    - Imported Kobold level from SlashEM and added to town variants.
    - Created a Kobold "mall" level that is peaceful and has a lot of shops.
    - Created a "Raided Angband" level that is been ransacked by bandits and thieves.
    
- [x] The Rogue level has been removed.

- [x] Imported the Rat level (levels 10-11)
    - generates in 25% of games, rather than 50%.
    - Contains a guaranteed bag of rats.

- [x] Imported Alignment Quests (appears 15-19, From SlashEM)
    - The alignment quests are now totally optional 
    - no artifact keys, no artifact doors in Vlad'S

    - Lawful Quest
        - has a new boss because we already have nightmares (from Evil)
        - Nebuchadnezzar now guards the 

    - Neutral Quest
        - has a new boss because we already have beholders (from Evil) 
        - Xanathar now guards Xanathar's Ring of Proof

    - Chaotic Quest: 
        - has a new boss because Vecna gets his own branch in Gehennom.
        - Acererak now guards the Staff of Withering

- [x] Imported Grund's Stronghold (appears 20-21)
    - Add a couple rows of spear traps to Grunds.
    - Convert Grunds to a mandatory level at 19/20/21 
    - Grunds downstairs are behind the throne.
    - Added a phase blocking line to middle of Grunds stronghold
    - Made moat 2 tiles wide; can be lava, water, or sewage.
    - Grund must be defeated to take the downstairs
    - Grund must be defeated to take the upstairs on Grunds Stronghold if you have the amulet. (Infidels take notice!)

- [x] Extended Wyrm Caves branch (Appears level 21-22)
    - 8-9 levels deep
    - Has standard mines fill levels - but with the rivers mixed in.
    - Created dynamic narrow passage filler levels that are always random.
    - Created an entrance level (from dnh Erebor.des)
    - Imported the Spider Caves and move here as an optional branch.
    - You'll encounter one "nest" level
    - 66% chance of encountering the Jermlaine village
    - Bottom is a dragon nest (from UnNetHack) with baby and adult dragons, as well as eggs and a treasure horde.
    
- [x] Imported Black Market (appears 23-24):
    - Reduce Black Market prices a bit.
    - Black Market Layout 1: Thinner normal shop.
    - Now undiggable and phaseproof.

- [x] Imported the Storage room (appears 22-30)
    - Added the 4 variations from dnh

- [x] Imported The Lost Tomb (appears 25-29)

- [x] Imported the Sunless Sea

- [x] Imported The Temple of Moloch (appears 26-29)
    
- [x] Lethe Gorge/Gulf of N'Kai:
    - Implemented from the Lethe Patch (without the lethe water)
    - I added this branch just after Medusa's Island.
    - Gulf of nkai levels at combined at the bottom of lethe gorge.
    - Import the lower castle dungeon as a separate mini branch

- [x] Gehennom
    - no longer a series of mazes, but are mines-style levels with lava. 
    - Areas such as the Sanctum and the entrance to the Wizard's Tower have also been adjusted.
    - Cerberus now guards the entrance to Gehennom.
    - Vlad the Impaler's tower is the vanilla version
    - Vibrating square: messages when player is near

- [x] Vecna's Domain
    - completely new and optional side branch has been added, known as  - Vecna is an uber-powerful Lich that is the source of power for all other liches. Because of this, all types of liches (including alhoons) cannot be genocided until Vecna is destroyed.

- [x] Elemental Planes:
    - The first four Planes levels are randomized and appear in a different order every game.
- [x] Additional variants of the Castle, Fort Ludios, Sokoban, Mine Town and Mines End have been added.

### New special rooms

- [x] Fungus Farm (Slash'EM)
- [x] Migo Hive (Slash'EM)
- [x] Dragon Lair (Slash'EM)
- [x] Real Zoo (Slash'EM)
    - Added zoo bats
- [x] Bad Food Shop (Slash'EM)
    - Full of bad food and rotten eggs.
- [x] Giant Court (Slash'EM)
- [x] Lemure Pit
- [x] Mini Guild (Hack'EM)
    - These replace the Guild of Disgruntled Adventurers in Slash'EM. 
    - Much smaller, less densely packed with player monsters
    - other monsters include typical pets.
- [x] Clinic room (SlashTHEM)
- [x] Terror hall (SlashTHEM)
    - Only appears after level 15.

### Themed Rooms:

- Evilhack already had most of the themed rooms but I added more.
- Add any missing flipped versions of rooms (from UnNetHack) for completeness
- Added minimum levels to all themed Rooms
  - The dungeon is pretty clean of themed rooms until level 8. 
  - At levels 8, 11, and 17 we introduce more variety.

## Role Changes:

### New player roles:

- Rebalanced all SlashEM role skills.
- No slashem roles can start with magic markers anymore (to stay in line with Evil)

#### Convict
resistance
- [x] Replaced Iron Ball of Liberation with Iron Spoon of Liberation
- [x] Convicts cannot buy services if banned from a shop.
- [x] Added secret doors to some of the cells on the quest home level, and around the level, so it's easier to navigate.
- [x] Convicts start with a spoon.

#### Infidel

- Infidels no longer start with drain life (Necromancers have the monopoly on that). In it's place, the spell of poison blast has been lowered to a level 2 spell so Infidels can start with that.



#### Flame Mage

- Flame Mage can only be Lawful/Neutral
- Flame Mages no longer gain cold resistance at level 13. (Their quest artifact still provides it though)
- Level 1: Intrinsic fire resistance
- Level 5: Gets "vulnerable to cold"
- Flame Mages also "like fire", so fire usually doesn't have an adverse affect on them.
- Cannot 2-weapon anymore
- Starts with more fire based inventory: 
  - potions of oil, fire bomb, scrolls of fire 
  -  fireproof quarterstaff and fireproof robe.
- Start with Fire Bolt instead of Flame Sphere (Fire Bolt is a Flame Mage exclusive spell)
- Can only specialize in Matter, Attack, and Divination spells.

- Adjusted to cast spells using Wisdom, get spellcasting bonuses are calculated using wisdom.
- Left out the dragon polyself ability.
- Gets spell-hunger reduction (similar to wizards but it's calculated with (int - 2)
- Flame mages get the multishot penalty
- Their special spell is fire bolt.

One major drawback to playing Flame Mages is that they can never attain 100% cold resistance. When they get Vulnerability to Cold - this caps their max cold resistance to 50%.

#### Ice Mage

- Can only be Chaotic/Neutral
- Ice Mages no longer gain fire resistance at level 13. (Their quest artifact does NOT grant fire res anymore)
- Level 1: Intrinsic Cold Resistance
- Level 5: Gains "Vulnerable to Fire"
- Level 15: Gains Water Walking

- Intrinsic Ice armor AC protection (Similar to Slash'EM Monk)
  - This is quite powerful.
  - Calculation is: ACBONUS = -((YOURLEVEL / 2) + 2)
  - Like SlashEM, this bonus caps at -11

- Can now reach expert in riding (Elsa can ride!)
- Starts with more cold based inventory:
  - Replaced their starting quarterstaff with a +2 stilletto
  - Starts with lenses or grappling hook. 
  - Removed studded armor, replaced with robe.
  - Removed starting potions (they might freeze from the cold)
  - Starts with a Frost Horn instead of a wand of cold
  - Ice Mages no longer start with confuse monster (not useful
l for vampires/illithids etc), they always start with slow monster instead.
- Left out the dragon polyself ability.
- Ice Mages can walk on ice without slipping.
- Gets spell-hunger reduction (similar to wizards but it's calculated with (int - 2)
- Ice mages get the multishot penalty
- Their multishot penalty doesn't apply to knives!
- Their special spell is cone of cold.

One major drawback to playing Ice Mages is that they can never attain 100% fire resistance. When they get Vulnerability to Fire - this caps their max fire resistance to 50%.

#### Ice and Flame details

- Each cannot use anything that is decidedly the opposite's domain:
- Flame Mages cannot use Frost Brand/Deep Freeze
- Flame mages cannot read cold based spellbooks.
- Ice mages cannot use Fire Brand/Firewall.
- Ice mages cannot read fire based spellbooks.
- Neither should receive opposite elemental based spellbooks or weapons either.

#### Necromancer


With inspiration from aosdict, the Necromancer role has been overhauled with a new approach.

- Removed starting quarterstaff, starts with 2 daggers.
- Starts with a wand of fear
- Starts with a robe
- Removed starting scrolls
- Only starts with 1 ring.
- Enabled necromancer to reach expert in polearms (for access to scythes)
- Necromancers now receive a +1 alignment bonus when digging graves, and will exercise wisdom if found a corpse.
- The necromancer's starting ghoul has the ghoulname option available.
- Their special spell is summon undead.

##### Spirits

- When playing as a necro, monsters that die drop "spirits". These are auto-collected as you walk over or adjacent to them.
- Spirits show up as a light-blue '~' symbol.
- Nonliving monsters do not leave spirits
- Spirits cannot be picked up by you or other monsters - they will disappear as soon as anything tries to pick them up. If wished for, they will be absorbed immediately into your being.
- Spirits will fade away quickly, so don't let them sit around. On graveyard levels, they get a much higher timeout.
- Very rarely, spirits will sometimes turn into will-o'-the-wisps when they fade. On swamp levels this is much more common. 
- Spirits generate when graves are dug up.
- Spirits generate in morgues (on the corpses of dead players)
- Multiheaded monsters drop more spirits (hydras, ettins, 2-headed trolls)
- Some classes and specific monsters will also want your spirits so try to collect them before they do!
    - L/W/A/&
    - Valkyries, Necromancers, shades, specters, weredemons, aloons
- Tame undead will not try to eat your spirits.

##### Benefits from harvesting spirits
    
Heals HP:
- Level 1: Spirits heal 1HP 
- Level 4: Spirits heal (monster's level / 2) HP + 1 
- Level 8: Spirits heal (monster's level) HP + 1 

Renews magical power:
- Necromancers do not regenerate magical power as other roles do. To renew your MP, you will have to collect spirits or find some other means (potions of gain energy, scrolls of charging, etc)

- [x] Level 1: Spirits grant (monsters level / 2) MP (minimum of 1).
- [x] Level 7: You get the monster's full level value in MP.
- [x] Level 13: You get the 1.5x monster's level value in MP.

##### Necromancer intrinsics changes

- [x] Level 1: Fear Resistance
    - Not afraid of "fear attacks", ie: dragons, banshees, bodaks
    - Not afraid of Noferatu gaze
    - Not afraid of ghosts or temple ghosts.
- [x] Level 1: Drain resistance
- [x] Level 3: Undead warning
- [x] Level 18: Sickness resistance
- [x] Level 18: Poison resistance

##### Necromancer gameplay changes

- [x] Necromancers can eat wraiths with the vanilla success rate.
- [x] Undead will never betray Necromancers (lots of undead are treacherous in HackEM, so charm monster isn't as reliable as it used to be...)
- [x] Like convicts, Necromancers now cannot tame anything other than undead. Instead of taming, the best that occurs will be pacifying the target. 

- Instead of starting with a pile of books or learning techniques, the Necromancer learns special Necromancy spells as they level up. As a holdover from Slash'EM, they still do start with the spell and spellbook of Drain Life.
- When the following spells are learned - the Necromancer gets them forever. This is meant to simulate techniques, but also to encourage use of them throughout the entire game. 

- [x] XP 2: CALL_UNDEAD 
    - (Level 1 spell)
    - When cast, acts like a magic whistle for undead.
- [x] XP 3: RAISE_ZOMBIES 
    - (Level 2 spell)
    - Adapted from the SlashEM Raise Zombies technique
    - Attempts to revive all corpses in adjacent squares and turn them into an appropriate zombies. Other humanoids will become a ghoul 3/4th of the time or a ghast the remaining 1/4th.
	- There is no effect on non-humanoid corpses.
    - An attempt will be made to tame the resulting undead with the same chances as casting charm monster. The undead will always be hostile if not tamed. You will be unable to move for 2 turns even if there was no effect, and the technique will be usable again after 1000-1500 turns.

- [x] XP 7: COMMAND_UNDEAD
    - (Level 4 spell)
    - This spell is equivalent to charm monster, except that it only affects undead.
- [x] XP 9: SUMMON_UNDEAD
    - (Level 5 spell)
    - 72 in 73 chance of creating one undead monster and a 1 in 73 chance of creating five undead. 
	- Necromancer they will automatically attempt to dominate the resulting monster(s). Monster(s) may resist, based on their MR.
- [x] XP 14: ANIMATE_DEAD
    - Adapted from the SlashEM Revivication technique
    - brings a dead corpse back to life, possibily as a pet. (Flagged as undead?)
    - https://nethackwiki.com/wiki/Revivification
- [x] XP 17: SPIRIT_BOMB (Level 5 spell)
    - Adapted from the SlashEM Spirit Bomb technique

#### Undead Slayer

- Starting jacket is always leather
- Can now #turn undead
- Immune to undead fear attacks or ghosts.
- Slayers get an alignment penalty for eating wraith corpses.
- Undead slayers get +2 to-hit bonus against undead/demons and +d4 damage (like hitting as a blessed weapon - this also stacks with the blessed weapon bonus.)
- Undead slayers obliterate zombies to the point that they don't leave corpses. 66% chance of this happening everytime you kill a zombie.
- Level 15: Slayers now get Warning
- Level 21: Slayers now get Clairvoyence
- Their special spell is protection

#### Yeoman
	
- Starts with a green coat that grants MC2. 
- Starts with a horn, a torch, and a blessed whetstone.
- axe - can reach skilled
- Can reach skilled in daggers
- Their special spell is knock.

### Existing role changes

#### Archeologist:

- Starts with 2 spellbooks; can start with torch or lamp
- Club moved from skilled to basic. 
- Spear skill added, can be trained to expert.
- Archeologists can highly enchant fedoras (xnh)

#### Barbarians:

- Barbarians are not immune to poison anymore (unless orcish)
- Barbs start with 2 food rations
- Twoweaponing – Barbarians go from basic to skilled
- riding skill from basic to skilled. 
- Attack spell school removed. Special spell changed to cause fear.

#### Caveman/Cavewoman

- gets nightvision radius 2 (From SlashEM)
– all spell schools removed, has 20% chance of failing to read any spellbook, regardless of its BUC status.
- Can bang rocks together to make flint, can lash flint to arrows, making them more deadly.
- Staying illiterate can increase maximum hit point gain per level-up. 
- Are the only role that can successfully tame and ride saber-toothed tigers.
- Starting pet is a wolf (From UnNethack)
- Brought over the wolfname option, so you can set it in your options file.

#### Healer

- are familiar with more health based potions
- they start the game knowing potions of sickness, regeneration, paralysis, and sleeping.
- Now receives the Drowsing Rod as a first sacrifice gift
- added clerical spell school, can train to skilled. 
- Gets poison resistance intrinsic at experience level 3 instead of experience level 1. 
- Trades warning intrinsic for sick resistance at experience level 15. 
- Can sense how wounded a monster is when attacking it, can also sense the wounded status of its pets.
[x] Healers get +1 blessed scalpel now.

#### Knight

- numerous weapon skills adjusted (axe, broadsword, two-handed sword, 
- polearms and spears from skilled to expert. 
- Mace and morning star moved from skilled to basic). 
- Attack and healing spell schools moved from skilled to basic. 
- Knights can never tame dragons (they are natural enemies). 
- Dwarven knights will start with a dwarvish bearded axe instead of a long sword.

#### Monks

- Can reach SKILLED in lightsabers
- crossbow skill removed, added trident and broadsword;
- tweaked skill advancement for weapons.
- Gains an extra fist attack per round using martial arts skill at grand master level,
- and also gains an extra kick attack (random) using martial arts skill at master level and higher.
- Can break boulders and statues using martial arts skill at skilled level or higher (press shift + f and then a movement key in the desired direction).
- Because of the extra random kick attack at master/grand master level, players will need to be mindful of wearing boots of some sort, as kicking a footrice/basilisk barefoot equals instant petrification.
- Monks now receive extra intrinsics as they level up into the higher experience tiers (telepathy at 20, water walking at 23, stoning resistance at 25, disintegration resistance at 27, and sickness resistance at 30).
- Reverted the starting spell of confuse monster; Monks can again start with the spell of sleep.

#### Priest
    
- gets a +2 shield
- all slash and pierce weapons removed; can only train with blunt-type weapons. 
- Incurs a substantial to-hit penalty, abuses wisdom and can negatively impact alignment when trying to use an edged weapon (wielded or thrown).
- Their crowning gift is Mjollnir.

#### Ranger

- Ranger obtains twoweaponing, up to skilled.
- if playing as a gnome, their quest artifact becomes the Crossbow of Carl. Its properties mirror that of the Longbow of Diana, except that it is a crossbow instead of a bow.
- XL10+ Rangers automatically see ammo enchantments

#### Rogues

- Rogues get scrolls of teleport and gold detection (2 each) and oilskin sack
- Rogues also get darts; 30% that they are replaced with a pistol and bullets.
- new trainable skill known as 'thievery', which allows the rogue to pickpocket a target and steal items from their inventory (must be bare-handed to use, press shift + f and then a movement key in the direction of the target).
- If the target is peaceful, there is a chance the target will not notice depending on the players' skill in thievery.
- Critical strike from behind works while twoweaponing.

#### Samurai

- removed attack and clerical spell schools.

#### Tourist:
    
- Tourists get automatic type identification for shop items
- Tourists get 2 extra magic maps, 2 extra food.	
- Tourists start with all of their optional equipment. 
- The magic marker that a tourist starts with is always 0:0.
- Twoweaponing: Tourists from skilled to basic.

#### Valkyrie

– removed escape spell school.
- Special spell changed to repair armor.
- Dwarven valkyries will start with a dwarvish bearded axe instead of a long sword, and are automatically proficient with it when using it to disarm weapons/shields.

#### Wizard

- removed numerous weapons from being trained (axe, short sword, club, mace, polearms, spear, trident, shuriken). 
- Enchantment spell skill raised from skilled to expert, 
- clerical spell skill lowered from skilled to basic. 
- Can sense if objects are magical in nature, much the same way a priest knows the beatitude of all objects.
- New starting pet, the pseudodragon. 

## Race changes:

### New player races: 

- replete with their own special abilities and restrictions.
- Several role–race combos have been added or unlocked.

#### centaur

#### giant

- Giants can wear mummy wrappings.
- Any giant can wear large splint mail - not just Samurai.

#### Hobbits

- [x] Hobbit race (already in evil)
- [x] Hobbits start out knowing Elbereth


#### illithid

Starts with the Psionic Wave spell:
    - Does 2d6 psychic damage
    - Level 26, this increases to 4d6 psychic damage.

#### Vampires:

- extra attack – drain life bite
- Can only drain corpses and drink blood.
- Added amusing custom messages for draining corpses.
- restricted in the two-weapon skill.
- Start with a small penalty to luck (-1) and alignment (-5)
- Vampires who wear an Opera cloak get a +1 Charisma bonus
- Vampires can wield and wear silver items, but it blocks their natural regeneration
- Can only be chaotic.
- As Vampires, they are vulnerable to silver (watch out!))
- I tried to space out the intrinsics a bit, since it felt like vampires started out overpowered in Slash'EM - but also didn't benefit from Poison or Sleep that vampires usually enjoy.
- Disabled #monster polymorph for the vampiric race. 

| Level | Ability            |
| ------|--------------------|
| Level 1: | Flying             |
| Level 1: | Drain/Death res    |
| Level 1: | Vulnerable to fire |
| Level 3: | Breathless         |
| Level 5: | Regeneration       |
| Level 7: | Poison resistance  |
| Level 9: | Flying             |
| Level 16: | Sleep resistance   |
| Level 21: | Sickness resistance |

### Existing role changes

#### Elves
- Elves always start out knowing Elbereth.
- Elves can use musical instruments without risk of breaking them.
- Elves can always squeeze between two trees
- Elves and rangers get alignment penalty for cutting down trees

#### Dwarves
- Dwarves get an alignment bonus for cutting down trees

#### Orcs
- Orcs can never use Elbereth

## The Quest

- Various changes have been made to each role's Quest to make the experience more varied and interesting for the player.

- The minimum experience level needed to be accepted for the quest has been lowered to 10 (from the default 14).
- If you've made your quest leader angry for whatever reason, this no longer makes the game unwinnable.
- You can engage them in battle, and if you kill your quest leader, this unlocks the quest. You can then complete your quest as you normally would. 
- Take heed, quest leaders are not weak and can prove formidable to a mid-level player. Also note that your deity isn't exactly thrilled with you killing off your quest leader. 
- Your alignment record can still be in the positive, but if you try any helm of opposite alignment shenanigans when it comes time to sacrifice the Amulet of Yendor at the endgame... interesting (read: bad) things can happen.

- Abusing your alignment can have a direct effect on how your quest ends. 
- Once you've defeated the quest nemesis and returned with your quest artifact and the Bell of Opening and speak with your quest leader, there's a chance they may ask you to actually return the quest artifact as they originally alluded to when you first accepted the quest. 
- Alignment abuse is tracked throughout the game – having your alignment negatively adjusted by one point also counts as one abuse point (this can be checked at any time via #conduct, and compared to vanilla, more feedback when abusing alignment is given). 
- Unlike your alignment record, which can be readjusted back into positive standing, your alignment abuse record is PERMANENT.
- Getting one abuse point means there's a 1 in 50 chance your quest leader will ask you to give up the quest artifact. 
- Racking up more abuse points increases those odds, to the point where if you've accumulated 50 or more abuse points, it's virtually guaranteed your quest leader will demand you return the quest artifact. 
- You have two options at this point – either hand over the quest artifact peacefully, or decline. 
- Declining will anger your quest leader, and they will attack. Handing over the quest artifact peacefully will flag the quest as complete. 
- Declining to turn over the quest artifact does not complete the quest – at some point your quest leader will need to be defeated before the quest is flagged as complete. 
- Also, attempting to evade your quest leader (whether they are peaceful or hostile) will do you no good in the long run, as the Bell of Opening has been 'cursed', and the only way to lift the curse is to complete the quest. 
- The Bell will not function for the invocation until this is done.

- Alternative scenarios have been taken into account, such as if you angered your quest leader before even starting the quest and you killed them, you simply need to defeat your quest nemesis to flag the quest as complete. 
- Conversely, if you've killed the quest nemesis and return, but somehow anger your quest leader before you speak to them, killing them also flags the quest as complete. 
- Some peace of mind – if you've never abused your alignment, your quest leader will never ask you for the quest artifact.


## Shops

- Shopkeepers can be a variety of different races; your race versus theirs directly affects pricing.
- I removed the racial shopkeeper price adjustments 
- (personally I love price identification so I couldn't get into this particular Evil change, but I still like the new racial shopkeepers)

### Shopkeeper Services

- Instead of every store offering a general Identify service - every
  shop offers premier identify servies based on their shop type or the race of 
  the shopkeeper.
- Streamlined the service menu interface. Previously navigating the services
  was tedious. Now when you press 'p', you are immediately presented with
  a full menu of all the available services - no submenus within menus. 
- Removed shk stealing all your money when you charge WoW 
- Only Black Market can have the "identify any" service

Firearms training service:
- Instead of the weapon practice technique, this is now a shk service.
- Gun shops always offer this up to expert skill.
- Weapon shops, armor shops offer 20% of the time, up to max skilled.
- General stores offer it 10% of the time, up to max Basic.
- $500 for Basic skill
- $1000 for Skilled
- $2000 for expert?

Tinker service:
- Only gnomes offer this service
- Allows you to upgrade an object to (usually) a better form, works the same as dipping an item into a potion of gain level.
- In Slash'EM this was implemented as a technique, but here the service only takes 1 turn (we'll assume the shopkeepers are very good at tinkering or just cheating and using a hidden potion of gain level behind the counter)
- Base charge is $500
- Non-gnomes are charged double
- Low intelligence (under 18 INT) players are subject to $100 price gouge
- Lower intelligence (under 13 INT) players are subject to $250 price gouge
- See the database on "upgrade" for a full list of eligible upgrades.

#### Identify service details
- All shops:        1 in 3 offer uncursing services.
                    1 in 20 offer rumors

- General stores:   0-2 random identify services,
                    10% chance of having 1 extra identify service.
                    20% offer firearms training
                    25% offer basic charging

- Weapon shops:		75% offer weapon ID
                    20% offer armor ID
                    25% offer weapon fixing
                    25% offer weapon enchanting
                    25% offer weapon poisoning
                    20% offer firearms training
- Armor shops:		75% offer armor ID
                    20% offer weapon ID
                    25% offer armor fixing
                    25% offer armor enchanting
                    20% offer firearms training
- Scroll stores:	75% offer scroll ID
                    20% offer book ID
- Spellbook stores:	75% offer book ID
                    20% offer scroll ID
- Potion stores     50% offer potion ID
- Ring stores       20% offer ring ID
                    20% offer amulet ID
                    20% offer gem ID
                    25% offer basic charging
- Wand shops:		50% offer wand ID
                    20% offer armor ID
                    25% offer basic charging
                    25% offer premium charging
- Tool shops:       50% offer tool ID
                    25% offer basic charging
- Deli/food shops:	100% offer food ID
                    10% offer potion ID
- Pet shops:		50% offer food ID
                    25% offer tool ID
- Lighting shops:	10% offer potion ID

#### Additional race specific services:

- Dwarves:          20% offer weapon ID
                    20% offer armor ID
                    20% offer gem ID
- Orcs:             25% offer weapon poisoning
- Gnomes:           25% offer tool ID
                    25% offer tinker
- Giants:           20% offer gem ID
- Nymphs:           10% offer ring ID
                    20% offer potion ID
- Hobbit:           50% offer food ID
                    10% offer ring ID

- Illithid:         25% offer scroll ID
                    25% offer spellbook ID
- Racial services are offered 25% of the time.

#### New shop types:

- [x] Pet Shops (From unnethack)
- [x] Instrument Shop (From unnethack)
- [x] Tin Shop ("canned food factory") (From unnethack)
- [x] junk shop (from SpliceHack)
    - Added fly swatter, rubber hoses, bag of rats, iron chain, tin opener.
- [x] archery shop (from SpliceHack)
    - Crossbows and bolts can now appear in archery shops.
- [x] mask shop (from SpliceHack)
- [x] Gun shops (from SlashTHEM)

## Credits

### Special thanks to: 
- K2, amateurhour, qt

### Thanks: 
- riker, bhaak, kes, shadowrider, aosdict, paxxed, mobileuser, krm26, Umbire

### Playtesters: 
- nh2465, cbus


==========================