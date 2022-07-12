# HackEM

## Design Goals


* The first goal of this project is to become the major successor of Slash'EM and port over most of Slash'EM to a modern platform. I chose EvilHack as a base because it is based on 3.6, it's inheritly more difficult, and it contains many Quality Of Life features already baked in. See here:
	- Nice HP Bar
	- Enhance menu shows max and % progress on skills.
	- Extended commands: #terrain, #annotate, #overview, #tip, #adjust splitting and merging
	- paranoid swim
	- Smart shop autopickup
	- Full bag altar BUC identification
	- Peaceful displacing
	- New conducts: Elbereth, alignment, touched art, petless
	- New room shapes and types (most from 3.7)
	- New soko levels 
	- object materials, quick wand wresting
	
Evil also has some challenging aspects that are well worth keeping:
	- New roles: Infidel, convict
	- New Races: Giant, Tortle, centaur, hobbit, and illithid
	- Elbereth: Can't use unless you found/discovered it.
	- Partial intrinsics
	- Reflection is not 100%
	- No randomly genereated magic markers
	- Zombies ressurect, mummies wither, meaner and improved spellcasters
	- Monster steeds

* The second goal is to apply updates (and applicable bugfixes) to Slash'EM that have been advanced in slashem-up, slashem9, and SlashTHEM. Slash'EM was a great game, but it was far from perfect. It would be nice to polish up things that were left behind (examples: updates to artifacts and quest artifacts, technique bugs, mold mechanics, polymorph bugs (ie: flame mage and ice mage).

* The third goal is to differentiate this fork from EvilHack. Let's use what makes sense - but let Evil have it's own features that make it Evil.

Examples of what to change back to traditional slashem behavior:
	- Don't require defeating Goblin king to access mines
	- Change sokobon prize system back to normal
	- Return shopkeeper pricing to normal levels (for easier price-ID)
	- Allow entering quest at 10 without penalty
	
* The fourth goal is to bring in new and interesting content from the many quality forks out there. I'd like to try not to get too wacky and keep to the spirit of Slashem. 

* The fifth goal is to enhance the existing content from slash'em to make more sense, be more challenging, and use the mechanics in more creative ways. When it makes sense, we'll follow the conventions that Evil has set down, otherwise we might have to invent new mechanics or abilities.

* Goal six is to create some new content that is specific to hackem!  

## Installation

Each OS type found under the `sys` folder has an installation guide for that
particular operating system. Pre-compiled binaries (linux and windows) can be
found here - https://github.com/k21971/EvilHack/releases

For Linux (TL;DR version):
- Dependencies needed: `make` `gcc` `gdb` `flex` `bison` `libncurses-dev`
- From the desired directory, `git clone https://github.com/k21971/EvilHack.git`
- Navigate to the `EvilHack/sys/unix` folder, then `./setup.sh hints/linux` or
  `./setup.sh hints/linux-debug` depending on what you intend to do

  - Using the standard `linux` hints file assumes running as a normal user, and
  game folders and files will reside in `/home/$USER` based on the account used.
  Invoking `sudo` should not be necessary

  - Using the `linux-debug` hints file assumes installing as root, and includes
  extra CFLAGS for debugging in a development scenario

  - With either hints file, edit the install paths to your liking
- Navigate back to the root EvilHack folder, and `make all && make install`
- Execute the `evilhack` binary
- In the home directory of the account used to install EvilHack, create your
  rc config file - `touch .evilhackrc` and then edit as necessary


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.


# Todo

## Goal 1: Porting Slash'EM

Slashem monsters:
	[X] giant tick (AT_MULTIPLY)
	[X] giant flea (AT_MULTIPLY)
		[ ] add message for tick/flea multiplication?
		[ ] Allow ticks and fleas to appear in main dungeon? (They currently only appear in the lawful quest or Gehennom.
	[X] snow ant
	[X] giant louse
	[X] tsetse fly
	[X] migo drone (M3_TRAITOR)
	[X] yellow jacket
	[X] black wasp
	[X] migo warrior
	[X] giant wasp
	[X] spitting beetle
	[X] migo queen
	[X] assassin bug (M3_TRAITOR)
	[X] killer beetle
	[X] jiggling blob
	[X] lava blob
	[X] static blob
	[X] burbling blob
	[X] chicken
	[X] cockatoo
	[X] parrot
	[X] pit bull
	[X] dingo puppy
	[X] large dingo
	[X] death dog
	[X] rabid wolf
	[X] wolverine
	[X] shadow wolf (Removed MR_PLUSONE | MR_HITASTWO)
	[X] mist wolf   (Removed MR_PLUSONE | MR_HITASTWO)
		[ ] Mist wolves leave trail of mist?
	[X] glowing eye
	[X] bloodshot eye
	[X] blinking eye
	[X] werepanther (animal) 	5
	[X] weretiger 	(animal) 	7
	[X] werespider 	(animal) 	11
	[X] weresnake 	(animal) 	9
	[X] werepanther (human) 	5
	[X] werespider 	(human) 	7
		[ ] Werespiders should summon giant spiders, jumping spiders, and recluse spiders, and maybe cave spiders
	[X] weretiger 	(human) 	9
	[X] weresnake 	(human) 	9
	[X] kamadan
	[X] displacer beast (already present in evil)
	[X] caterwaul
	[X] sabre-toothed cat
		(Evil has a sabre-toothed tiger)
		[ ] Set up the sabre-tooth cat to grow up into the tiger?
		[ ] Make sabre-tooth cat a little weaker, or tiger stronger.

	[X] hellcat
	[X] statue gargoyle (Removed MR_PLUSONE and added to nonliving)
		[ ] Put statue gargoyle in immune_death_magic, remove from nonliving list.
	galltrit (deferred)
	jermlaine (deferred)
	
	[X] dwarf thief
		[ ] Check town/minetown for dwarf thief/gnome thief placement
	[X] duergar (removed MR_HITASONE)
	[X] deep one (growup done)
	[X] deeper one (growup done)
	[X] deepest one
	[X] dretch
	[X] rutterkin
	[X] nupperibo
		[ ] Make blood imps, nupperibos drain levels as well?
	[X] blood imp
		[ ] Blood imps should probably have a vampiric attack.
	[X] clear jelly
		[ ] clear jelly has touch acid attack, but not passive
		[ ] add passive acid as well?
	[X] yellow jelly
		[ ] yellow jelly has passive stun, but not active 
		[ ] add passive acid as well?
	[X] orange jelly (Implemented passive sleep attacks)
		[ ] orange jelly has passive sleep, but not active attack
		[ ] add passive acid as well?
	[X] rancid jelly
		[ ] rancid jelly should leave a trail of acid slime?
		[ ] eat organics?
		
		[ ] blue/spotted/ochre jellies seem vulnerable to fire, but new jellies are not in slashem. Should they be? Or vulnerable to something else?
		
	[X] swamp kobold
	[X] rock kobold
	[X] kobold warrior
	[X] Kroo the Kobold King
		[ ] Kobolds use the orc sounds, maybe they should have their own sounds?
		[ ] Add traitor property to all kobolds
		
	[X] leprechaun wizard (Removed MR_HITASONE)
	[X] pixie (removed MR_HITASONE)
	[X] brownie (removed MR_HITASONE)
	[X] quickling (removed MR_HITASONE)
	[e] Aphrodite - Already in evil, higher  level
	[X] war orc
	[X] great orc
	[X] Grund the Orc King
	[X] snow orc
	[X] demon orc
	[X] lamb (Note: grows up)
	[X] sheep (MS_SHEEP)
	[X] goat
	[X] cow (MS_COW)
	[X] bull
	[X] giant badger (changed to rodent class to match honey badger)
		
	
		[ ] Upgrade giant badger to be much stronger, and maybe grow up into honey badger?
		[ ] Badgers ccan usually swim; they release a stink. 
		These animals are usually known as stinky animals due to their ability of release a nasty odor from their well-developed anal scent glands. The body size range may vary from smallest circumboreal weasel that weight about 35 to 250 g, to largest wolverines and the sea otters weighing 32 to 45 kg.
		[ ] Most badgers are solitary, nocturnal animals. (they spawn in very small groups, they like the dark)
		[ ] Honey badgers should love bee hives... and be mortal enemies with bees (or team a)
		
		

	

	
	
	[X] scramper
	[X] squealer
	[X] mangler
	[X] Jumbo the Elephant
	[X] juggernaut
	[X] catoblepas (Removed MR_HITASTWO)
		[ ] catoblepas: The gaze attack doesn't seem to give a message when you die.
			The reason for death is "gaze of death"
		
	[X] rabbit
	[X] black rat
	[X] rabid rabbit
	[X] pack rat
	[X] hellrat
	[X] Rat King
	[X] recluse spider (webmaker)
	[X] barking spider (webmaker)
	[X] carrion crawler
	[X] nickelpede
		[ ] Make centipede grow into nickelpede
		[ ] Make nickelpede grow into giant centipede
	[X] giant scorpion
	[X] phase spider (webmaker)
		[ ] make more powerful and skittish? Make them have phasing?
	[X] Girtab
	[X] Shelob

	Pegasus (evil has?)
	
	[X] larva (Note: grows up)

		
	[X] maggot (Note: grows up)
	[X] dung worm
		[ ] Dung worms should be poisonous?
		[ ] Eating dung worms should cause illness?
		
	
	[X] acid worm
	[X] bloodworm
		[ ] allow "blood worm", "blood-worm" as spelling
	[X] tunnel worm
	[X] rot worm
	[X] spark bug (Removed MR_HITASONE)
	[X] arc bug (Removed MR_HITASONE)
	[X] lightning bug (Removed MR_HITASONE)
	[X] echidna
	[X] platypus
	[X] koala
	[X] wombat
	[X] Tasmanian devil
	[X] wallaby
	[X] wallaroo
	[X] kangaroo
	[X] movanic deva (Removed MR_PLUSONE)
	[X] monadic deva (Removed MR_PLUSONE)
	[X] astral deva (Removed MR_PLUSONE)
	[X] Planetar (Removed MR_PLUSFOUR)
	[X] Solar (Removed MR_PLUSFOUR)
	[X] rhumbat
	[X] athol
	[X] hellbat
		[ ] Hellbat "pecks". SHould be bite.
	[X] mongbat
	[X] mobat
	[X] harpy
	[X] byakhee
	[X] nightgaunt
	[e] baby shimmering dragon
	[e] shimmering dragon
	
		-- adding dragon is tricky, need to maintain some orders
	baby deep dragon
	deep dragon
	
	[X] wyvern (Removed MR_HITASTWO)
	[X] hydra  (Removed MR_HITASTWO)
	[X] disgusting mold
	[X] black mold
	[X] gnome thief
	[X] deep gnome (Removed MR_HITASONE)
	[X] gnome warrior (Removed MR_HITASONE)
	[X] Ruggo the Gnome King (Removed MR_HITASONE)

	[e] gnoll (evil has?) (Note: grows up)
		Evil also has gnoll cleric and gnoll hunter
	gnoll warrior (Note: grows up)
	gnoll chieftain
	gnoll shaman
	
	[X] The Largest Giant (Removed MR_HITASTHREE)
	[X] Father Dagon (deferred)
	[X] Mother Hydra (deferred)
	[e] vorpal jabberwock (already has the beheading attack!)
	[X] troll mummy (Added regen and stalk, revival works fine.)
	[X] ogre mage
	[X] shadow ogre (Removed MR_PLUSONE)
		[ ] Let Shadow Ogres appear in the main dungeon (or at least genennom)
	[X] moldy pudding (Removed MR_HITASONE, added M4_VULNERABLE_FIRE)
		[ ] Moldy pudding: Enhance a bit, only has an engulfing organic attack. No damage.
	[X] shoggoth (Note: grows up)
		[ ] Remove all but one corrosion attack.
	[X] giant shoggoth
		[ ] Remove all but one corrosion attack.
	[e] genetic engineer
	[X] Doctor Frankenstein
	[X] king cobra
	[X] asphynx
	[X] two-headed troll (Removed MR_HITASTWO)
	[X] black troll (Removed MR_HITASTHREE)
	[X] water hulk (Removed MR_HITASONE)
		[ ] Add vulnerability to shock (similar to other water-critters)
		
		
	[X] fire vampire
	[X] star vampire
	[e] vampire mage
	[X] wight
		[ ] Why do we have wights?  Should they not be more powerful than barrow wights?
	[e] zruty
	[X] ghoul mage
	[X] ghast
	[X] ghoul queen
	[X] gug
	[X] wax golem
	[X] plastic golem
	[X] Frankenstein's Monster
	
	[X] ruby golem
	[X] diamond golem
	[X] sapphire golem
	[X] steel golem (Added M4_VULNERABLE_ACID)
	[X] crystal golem
	[X] gibberling
	[X] grimlock
	[X] drow (Added infravisible)
	[X] mugger
	
	[X] gypsy (Added M3_INFRAVISIBLE)
		[ ] Implement gypsy_chat
	
	[X] black marketeer (Removed MR_HITASFOUR, added M3_ACCURATE)
	[X] shadow (Added M3_INFRAVISION)
	[X] spined devil (Removed MR_PLUSONE)
	[X] bearded devil (Removed MR_PLUSONE | MR_HITASTWO)
	[X] bar-lgura (Removed MR_PLUSONE | MR_HITASTWO)
	[X] chasme (Removed MR_PLUSONE | MR_HITASTWO)
	[X] babau (MR_PLUSONE | MR_HITASTWO)
	[X] nabassu (Removed MR_PLUSONE | MR_HITASTHREE)
	
	Cthulhu (deferred)
	
	[X] giant crab
	[X] gila monster
	[X] rhaumbusun
	[e] basilisk
	[X] komodo dragon
		[ ] Should komodo dragon/gila monster be vulnerable to cold?
	[X] bad egg
	[X] killer tripe ration
	[X] killer food ration
	[X] pile of killer coins (Note: grows up)
		[ ] Make evil food vulnerable to fire - "cooking"!
	[X] large pile of killer coins (Note: grows up)
	[X] huge pile of killer coins
	
	Nightmare (evil has nightmares)
	Beholder (evil has beholders)
	Vecna
	
	[X] flame mage
	[X] ice mage
	[X] necromancer
	[X] undead slayer
	[X] yeoman
	
	[X] High Flame Mage (Removed MR_HITASFOUR)
	[X] High Ice Mage (Removed MR_HITASFOUR)
	[X] Dark Lord (Reoved MR_HITASFOUR)
	[X] Van Helsing (Removed MR_HITASFOUR)
	[X] Chief Yeoman Warder
	
	Water Mage
	Earth Mage
	Maugneshaagar
	Count Dracula
	Colonel Blood
	
	igniter
	froster
	embalmer
	exterminator
	Yeoman Warder
	
	
Quality control on monsters:
	[ ] Double-check and redo all difficulty levels for imported monsters.
	[ ] Double check all monsters for secondary references in code (other effects/abilities, etc)
	[ ] Also look at what monsters are normally equipped with.
	[ ] Update database knowledge entries for every new monster
	[ ] Update monsters.map of the tiles
	[ ] Import all tiles for new monsters	
	[ ] Update infravision tags. Only body heat should be visible.
	[ ] For everything that had MARM - maybe use the y value instead? Look into how monsters are armed, this must be related.
	
Other ideas:
	[ ] Make were-critters summon more diversity of monsters
	[ ] Check other monsters for traitor property
	[ ] What can we replace requiresX and hitsAsX with? Forceful hits, more AC/MR

	[ ] Update mines.des to include different types of gnomes, dwarves, and thieves.

Slashem Artifacts:
	Bat from Hell
	Deathsword
	Deep Freeze
	Deluder
	Disrupter
	Doomblade
	Elfrist
	Firewall
	Gauntlets of Defense
	Giantkiller
	Hellfire
	Holy Spear of Light
	Houchou
	Luckblade
	Mirrorbright
	Plague
	Quick Blade
	Reaper
	Serpent's Tongue
	Skullcrusher
	Sword of Balance
	Sword of Justice
	Thiefbane
	Wallet of Perseus
	Werebane
	Whisperfeet
	The Candle of Eternal Flame
	The Storm Whistle
	Great Dagger of Glaurgnaa
	Stake of Van Helsing
	Crown of Saint Edward
	Nighthorn
	Eye of the Beholder
	Hand of Vecna
	Key of Law
	Key of Neutrality
	Key of Chaos


Slashem Items:
	Dark elven dagger
	Wooden stake
	Great dagger
	Silver short sword
	Dark elven short sword
	Silver long sword
	Rapier
	Fly swatter
	Silver mace
	Heavy hammer
	Dark elven bow
	Dark elven arrow
	Pistol				(maybe keep?)
	Submachine gun 		(probably cut)
	Heavy machine gun 	(probably cut)
	Rifle 				(probably cut)
	Assault rifle 		(probably cut)
	Sniper rifle 		(probably cut)
	Shotgun 			(probably cut)
	Auto shotgun 		(probably cut)
	Rocket launcher 	(probably cut)
	Grenade launcher 	(probably cut)
	Bullet 				(maybe keep?)
	Silver bullet		(maybe keep?)
	Shotgun shell		(probably cut)
	Rocket 				(probably cut)
	Frag grenade		(keep but altar?)
	Gas grenade			(keep but altar?)
	Stick of dynamite	(archaeologist specialty?)
	
	Mood ring
	Ring of sleeping
	Ring of gain dexterity
	Ring of gain intelligence
	Ring of gain wisdom
	
	Amulet of flying
	Amulet of drain resistance
	Amulet versus stone
	
	Magic candle
	Torch
	Medical kit			(cut this?)
	Fishing pole
	Green lightsaber	(probably cut)
	Blue lightsaber		(probably cut)
	Red lightsaber		(probably cut)
	Red double lightsaber (probably cut)
	
	Asian pear
	Mushroom
	Sandwich
	Tortilla
	Cheese
	Pill
	Holy wafer
	
	Potion of amnesia
	Potion of blood
	Potion of vampire blood
	Potion of clairvoyance
	Potion of ESP
	Potion of invulnerability
	
	Spellbook of flame sphere
	Spellbook of freeze sphere
	Spellbook of resist poison
	Spellbook of resist sleep
	Spellbook of endure cold
	Spellbook of endure heat
	Spellbook of insulate
	Spellbook of poison blast
	Spellbook of lightning
	Spellbook of acid stream
	Spellbook of enlighten
	Spellbook of command undead
	Spellbook of summon undead
	Spellbook of passwall
	Spellbook of enchant weapon (probably cut)
	Spellbook of enchant armor  (probably cut)
	
	Wand of healing
	Wand of extra healing
	Wand of draining
	Wand of fear
	Wand of create horde
	Wand of fireball
	
	Healthstone
	Whetstone

Slashem Armor:
	Dark elven mithril-coat
	Robe of protection
	Robe of power
	Robe of weakness
	Shimmering dragon scales
	Deep dragon scales
	Shimmering dragon scale mail
	Deep dragon scale mail
	Lab coat
	Poisonous cloak
	Gauntlets of swimming

Roles:
	Flame Mage
	Ice Mage
	Yeoman
	Undead Slayer
	Necromancer
	
	Monk: Implement the monk closer to Slashem
	
Races:
	Doppleganger
	Hobbit
	Vampire
	Lycanthrope		(probably cut this race, it severely sucks as is)
	Drow

Special Levels:
		Mall
	2	Rat level
	3	Kobold level
	4	Mine king level
	5	Grund's Stronghold
	6	Lawful Quest
	7	Neutral Quest
	8	Chaotic Quest
	9	Storage room
	10	Wyrm Caves
	11	The Lost Tomb
	12	One-eyed Sam's Black Market
	13	Nymph level
	14	The Spider Caves
	15	The Guild of Disgruntled Adventurers
	16	The Sunless Sea
	17	The Temple of Moloch
	18	The Giant Caverns
	19	New Demon Prince Lairs
	19.1	Yeenoghu's lair
	19.2	Demogorgon's lair
	19.3	Geryon's lair
	19.4	Dispater's lair
	20	Frankenstein's Lab


New special rooms
	Giant Throne Rooms
	Fungus Farm
	Real Zoo
	Bad Food Shop
	Dragons Lair
	Lemure Pit 		(only appears in Gehennom)
	Migo Hive
	
	
Slashem Mechanics:
	Toilets
	Mold/Fungus ressurection
	Temp polymorph
	Upgrading
	Invisible items
	Poison missiles
	Techniques
	Shopkeeper services
	Minion Summoning (Prayer/Altar Gifts)
	Require-X-to-hit monsters
	Hits-as-a-+X-weapon
	Spell minions/entities
	Gauntlets of power change
	Unicorn Horn mechanic change
	No Mysterious Force
	\#youpoly command
	\#technique command
	\#borrow command
	
	

## Goal 2: Apply slashem updates from slashem-up, slashem9, slashTHEM

		
## Goal 3: Remove/edit Evil content
	[ ] Don't require defeating Goblin king to access mines
	[ ] Change sokobon prize back to normal
	[ ] Return shopkeeper pricing back to normal (no racist pricing!)
	[ ] Fix paranoid swim
	[ ] Remove water from Gnomish Mines (use in a different mines - wyrm or kobold caves?
	[ ] No alignment penalty in quest
	[ ] Can enter quest at 10 - no penalty

	[ ] Revenants should shoot missiles/fireballs (ala Doom)
	[ ] Snowball attack for ice trolls? frost giants?
	[ ] Double check that all nobles are lords, royals are kings.
	[ ] Revert "noble" to "lord"
	[ ] Revert "royal" to "king"
	[ ] Eventually add in the opposite genders from 3.7 (ladies/queens, etc)


	[ ] Keep evil Gnolls or use slashem ones, or ones from Splice/Dnh/Gnollhack?
	[ ] Keep Vecna's lair? 
	
## Goal 4: Import content from other forks.
	3.7
	FIQ
	
	* Bag of tricks patch (from UnNethack)
	* Auto-ID of obvious things (UnNethack)
	* Cartomancer role, Mermaid race (Splicehack)
	* Boots of Stomping (Splice)
	* Sheol branch (Un)
	
## Goal 5: Homemade Ideas/Improvements for Slashem
	[ ] Pokedex for Monsters
	[ ] Pokedex for Items
	[ ] Techniques from SlashEm/Them
	[ ] Add back best roles from SlashThem
		Convict (already in Evil)
		Chef, 
		Drunk, 
		Gladiator, 
		Pirate/Corsair (or copy from Splice) 
		Warrior (revamp to be more in line with Gauntlet)
	[ ] Add back these races from SlashThem
		[ ] Ghoul
		[e] Giant
		[e] Illithid
		[ ] Vampire
	[ ] Allow Giants to benefit from ring of slow digestion
	[ ] Change Gypsy to Fortune Teller and create a "fortune teller wish flag" that once set disallows additional fortune teller wishes.
	[ ] Fix Sokoban so that monsters don't spawn in the hallway and also probably no peaceful spawns in sokoban.  Both are extremely boring when they happen.  One of the variants has this, so you should be able to patch from them.
		
	[ ] Instead of fungus: larva/Maggots grows in some corpses, large ones?
	[ ] Maggots could grow into other things depending on level?
	[ ] Should water monsters have water attack? Spit water?
	[ ] shadow monsters leave trail of darkness
	[ ] Shadow monsters are drain/death/cold resistant?
	[ ] Anything that is "rockish", ie, rock kobolds, rock gnomes, should be stoning resistant and vulnerable to digging. Stone giant.
	
	
	[ ] Regarding temp polymorph - there are pros and cons to keeping and removing it. 
		Temp poly items nerfs polymorph a bit, not so much for potions and scrolls, but moreso for rings and amulets (and wands)
		Temp poly introduces another descriptor "hazy" in the inventory.
		Temp poly opens doors up for strange bugs - like +99 dwarven pick-axes.
		
		Temp poly for monsters is much more powerful - werecritters are annoyingly powerful
		Chameleons/shapechangers can unfairly revert multiple consecutive turns after being killed over and over.
		Pet chameleons are formidable pets, they can get killed over and over, and still come back.
		
		I'm more in favor of NOT re-implementing temp poly and keeping the traditional behavior in vanilla and evil.
	
	[ ] Shopkeeper services
		- the interface needs a revamp, it's really annoying to navigate the shk services in slashem.
		- Implement a nice menu when you press 'p' 
		- (then we can use services even if we own money)
		- General stores should not always allow full identify services for everything. There should be more variety in what they can id - or each one has somem different specialties
		- Slashem stores typically did not have many interesting services. What can we add to make things more interesting?
	

## Goal 6: Hackem specific content
	[ ] Energy Vampires/Psychic Vampires: 
		Inspired by: https://whatwedointheshadows.fandom.com/wiki/Psychic_vampires
		Drain energy merely by talking to them
		bore - sleep
		daywalkers, not affected by sun
		Can drain other EVs
	
	[ ] Giant enhancements:
		* Make giants have more elementals associated with their attacks:
			Fire giants have fire attack/fireballs, passive fire. Fire trail/smoke trail
			Frost giants have cold attack/ice balls, passive cold. Ice trail.
			Storm giants = shock
		* Giants can grab/crush/toss you
		* Giants have bellow/holler attack (like Splice sonic attack)
		* Giants can jump and create pits.
		* Thrown boulders hurt a LOT more.

	[ ] Lord/Prince classes could be able to summon small crowds of lower minions? Or even higher?
	[ ] Is there special handling for multi-headed monsters? Hydras, 2-headed trolled?
		Afffects things, beheading, regeneration, sight, etc...
	[ ] Nagas are a little boring, is there anything we can do to spice them up? They are basically mirroring dragons but weaker.
	[ ] Make see invisible temporary (from potions, eating stalkers, fountains)
		Requires ring to be permanent.

	[ ] Instead of modern firearms - add old-style Muskets?
	[ ] Enemy compounds could even have cannons?...
	
## Goal 8: Rethinking dungeon texture
	Every 5-9 levels, change the theme of the section. 
		* Populate with mostly related monsters, related special rooms.
		
	Don't change the first 9ish levels: But don't allow new special room types until level 8.
	Level 8: We start to see poly traps, but also weirdly shaped rooms.
	
	
	Gardens/plains: sparse trees, fountains, grass, high grass
	Swamps: water, shallow water, swamp, 
	Wastelands/Desert: burnt trees, boulders, 
	
	Courtyard gardens: Pre-castle levels 
	Castle:
	Under castle:
		* Castle storage, kitchen, stores
		* Utility rooms, Sewers
		* Castle Catacombs/dungeons, torture chambers

		* Secret passages/tunnels - to secret chambers.
		* These eventually lead outside to VoTD.
		
	9-17: special rooms, coordinated rooms
	18-27: wilderness? trees, gardens, zoos, 
	
	
## Goal 7: Rethinking Gehennom
	[ ] polluted fountains: For orctown, and deeper levels, swamp levels?
	[ ] Some room and corridor layouts in gehennom - but with a twist?? (This would potentially allow lemure pits)
	[ ] Interspersed in between levels are room and corridor levels that look ripped from SLEX
		(Crazy colors, textures, maybe even take the R&C generator from slex just for this purpose.)
	[ ] No regen in gehennom
	[ ] Limited power regen in nom.
	[ ] Varia suit damage in certain gehennom levels/rooms (very hot/toxic rooms)
	
## Bugs and fixes