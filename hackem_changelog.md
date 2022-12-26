```
   O                                             ,  /\  .  
   8                                            //`-||-'\\
,--8--,  _    _            _    ______ __  __  (| -=||=- |)
  |:|   | |  | |          | |  |  ____|  \/  |  \\,-||-.//
  |˙|   | |__| | __ _  ___| | _| |__  | \  / |   `  ||  '  
  |˙|   |  __  |/ _` |/ __| |/ /  __| | |\/| |      || 
  | |   | |  | | (_| | (__|   <| |____| |  | |      || 
  | |   |_|  |_|\__,_|\___|_|\_\______|_|  |_|      || 
  \ /                                               ()
   .
```                                    
# Hack'EM

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
    - Reduced to level 2.
- [x] spellbook of reflection
    - Creates a temporary reflective bubble around the caster, 
    - acts exactly the same as an amulet of reflection (or the like)
- [x] spellbook of repair armor
    - repairs one level of damage to any worn piece of armor. 
    - Will not make armor items fixed. 
    - Works as a foil to the new monster spell 'destroy armor'
- [x] spellbook of sonicboom
- [x] spellbook of summon undead
- [x] spellbook of invisiblity - now a level 2 spell

- [x] spellbook of fire bolt (new Hack'EM special!)
  - Fire Bolt is only available to the Flame Mage and will not generate randomly.
  - 1d10 fire damage (avg 5.5)
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

### Spell changes:

- [x] spellbook of confuse monster – level 2 → 1
- [x] spellbook of slow monster 
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
- [x] spellbook of Invisibility – level 4 → 2
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

- [x] Updated nasties lists with slashem monsters.
    - Added gugs, slaughter wights, devas, revenants
    - 
### Monster Clerical spells:

- [x] Summon minion	(new)
    - Summon a type of minion based on its alignment.
    - Demons that can cast clerical spells are not given access to this spell (they can already demon-gate).
- [x] Geyser
    - A powerful spell that causes a large geyser of water to slam into its target, causing physical damage (8d6).
    - What's new: can now rust any unprotected worn armor.
- [x] Vulnerability	 (new)
    - It's target temporarily becomes vulnerable to either fire, cold, shock or acid-based attacks, dropping its resistance (if any exists) by 50%.


## Artifacts:

**Design decision #1:**

    In SlashEM, artifacts grant a base damage bonus - should we adjust so that they deal closer to that on average? Currently, we'll say no and see how things play out. (Except for Doomblade which I specifically want close to slashem levels.)

**Design decision #2:** 

    Any roles that had 2 guaranteed altar sacrifice gifts have been reduced to just 1. Having 2 just seems excessive, and most of the time, the second gift was just a barrier to overcome to get to better stuff (example: Deathsword for Barbarian, or Deluder for Wizards)
    
- [x] Players can twoweapon with an artifact in each hand, if those artifacts happen to get along with each other.
- [x] When sacrificing, your deity may gift you a regular piece of gear instead of an artifact.
- [x] Added much stronger artifact blasting (6d6 to 8d10) (from Slashem)
- [x] If wielding/wearing any of the 'banes, taming becomes impossible */
- [x] Wishing for artifacts: 
    - Instead of being based off of how many artifacts exist in the game total (vanilla NetHack method), success is based off of how many artifacts the player has wished for (successful or not). 
    - ALSO - the odds are high that it comes with its owner...

### New artifacts
- [x] Balmung
    - Now gold. Destruction proof, but destroys armor like a boss
- [x] Bat from Hell
    - Rogues no longer receive this as a guaranteed sacrifice gift.
- [x] Bradamante's Fury
- [x] Carnwennan
- [x] Chains of Malcanthet
    - Changed to target Angels instead of demons
- [x] Circe's Witchstaff
    - Changed to a STAFF_OF_MATTER;
    - poly attack upped to 10%; can change enemies into pigs or feral hogs.
- [x] Deathsword
    - Barbs no longer receive this as a guaranteed sacrifice gift.
    - Deathsword causes hostility from humans
    - Capable of inflicting a paralyzing hit 1 in 4 attacks.
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
    - (Copper Staff of Healing) +d5 to-hit, +d8 dmg
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
    - When a player creates Gungnir, it now converts the spear held to an atgeir (type of spear)
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
    - Cannot be wished for. Now has a 75% chance of appearing in the Town branch.
- [x] Mirrorbright
    - Mirrorbright no longer hinders spell-casting.
    - Healers no longer receive Mirrorbright as a sacrifice gift, they get the Drowsing Rod instead.
- [x] Mortality Dial
    - Was COLD_IRON in splice, made it IRON.
    - Prevents trolls and zombies from reviving.
- [x] Mouser's Scalpel (slashem-up)
    - a rapier which gives +5 to-hit bonus and +1 to-damage.
- [x] Mystic Eyes (splice)
    - (Previously known as Lenses of Death Perception)
    - Grants DeathVision
    - As long as the wearer is not blind, they deal double damage, but constantly hallucinate. Deathvision's double damage is applied everywhere  (including spells/wands)
    - Grants searching.
    - Made out of gemstone
- [x] Origin
    - STAFF_OF_ESCAPE
    - While wielded, confers teleport control and great bonuses for spellcasters.
    - Increases spellcasting ability as a robe would, plus it makes escape spells cost less.
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
    - Upgraded to a parazonium, gets +d5 tohit
- [x] Skullcrusher
    - Replaced Keolewa with Skullcrusher.
    - Upgraded to +1d12
- [x] Holy Spear of Light
    - Holy Spear of Light causes hostility from undead
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
    - Now a non-aligned two-handed sword (from Un), +d5 to-hit, +1 dmg
    - Changed Thiefbane to platinum so elves can wield it.
    - Raised to 10% beheading
    - Cancels thieves and covetous monsters.
    - Protects from thievery and seduction.
- [x] Thunderstruck (previously known as Sonicboom)
    - Increased attack to AD_LOUD(5, 20).
    - Destroys glass golems
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
    - Can now #invoke for 1 of 10 random "storm" related pets.
- [x] Great Dagger of Glaurgnaa (Necromancer)
    - Only confers MR when wielded.
    - Grants double spirit bonuses when carried
	- Also triples the radius of spirit auto-collection when carried.
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
    -  Magicbane no longer cancels inventory items
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
- [x] Staff of Aesculapius: also cures afraid and larval infection.
- [x] Sunsword
    - made of gemstone (crystal)
- [x] Tsurugi of Muramasa
    - Raised bisection chance to 15%
    - now confers half physical damage when wielded.
- [x] Vorpal Blade
    - Raised beheading chance to 10%.
    - Wielding Vorpal Blade protects against decapitation attacks.

- Various artifacts that have a nemesis monster warn against those monsters.
- Almost all of the stock weapon artifacts have had their to-hit and damage modifiers adjusted in the players favor.
- Certain artifacts can either deliver extra damage to their nemesis monsters, or have a small percentage chance of delivering a fatal blow – this applies to the player as well!

## Dungeon Features

### Trap Mechanics:

- [x] Added ice traps (Un)
  - Ice traps only generate after level 8
- [x] Added magic beam traps (evil)
    - Magic beam traps cannot generate above level 16 (SlashTHEM)
    - Magic beam traps now generate a random beam type on every trigger; before they were hardcoded to a single beam when created on the level.
- [x] Can #untrap rust traps (they turn into fountains)
- [x] Can #untrap fire traps (requires water)
- [x] Untrapping a spear trap has a chance of yielding a spear or stake
- [x] Allow forcefighting of webs with a bladed weapon to destroy them
- [x] The Castle drawbridge does not always close with the passtune
- [x] Invisibility from magic traps is temporary.
- [x] 1/7 chance a polytrap disappears after polymorphing a monster. (splice)
- [x] Thrown items can get stuck in webs (3.7)

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

### Grass

    - Pulled from SpliceHack/xNetHack, we now have lush grass in the dungeon!
    - Grass appears sparsely in the early levels, peaks around level 15, and then tapers off after level 22. 
    - Hidey monsters can use grass as concealment.
    - Fire and death rays scorch away grass and revert it to normal floor

### Blood (splice)

    - When monsters are killed, they have a chance to spray blood in a random direction and color a tile red. No real effect on gameplay, purely aesthetic.
    - The amount of blood depends on monster size.
    - Spraying bloody tiles with a ray of water (ie: from a wand of water) will clear the tile of it's blood.
    - When bloody tile is walked over, there is a 1 in 20 chance it will erode away.
    - Orc blood is black.

### Vents (splice)

    - Vents appear after level 3. 
    - After level 6, 66% of vents will emit poison gas instead of harmless mist.

### Toilets (SlashEM)

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

#### Amulet identification in toilets (slex)

- Amulets can be dropped down toilets and possibly identified (similar to sinks and rings)
- Updated amulet feedback messages, blind effects, and hallucination effects.
- Amulets of Flying are always regurgitated from toilets.
- Amulets of Change will polymorph the toilet into a sink

### Forges (evil)

- Forges are introduced, which have the same appearance as fountains but are orange in color. Just like fountains, dipping things into them can have desirable or adverse affects. They can be used to dispose of anything that is flammable, and can also be used to repair corrosion/rust damage to any metallic object (if your luck is greater than 0). There is a rare chance that a lava demon can be summoned via dipping, which will be hostile most times, but can sometimes spawn tame (same odds as getting a wish from a water demon/fountain). Having negative luck can cause a forge to explode when used, which can cause considerable fire damage. Players can also use a forge to remove an attached ball & chain, and can also combine two objects to forge something new using the #forge command.
- [x] Added new recipes for various imported weapons
- [x] Forges don't appear until after level 2.
- [x] Forges can be blown up by rays of water.

## Mechanics Changes

- [x] If the player tries to hit a monster with a weapon they are restricted/unskilled in, or any non-weapon object, they'll never have better than a 75% chance of landing a hit (bare-handed is exempt).
- [x] Strength bonus for two-handed weaponry is x1.5 (EvilHack)
    - The standard damage bonus for strength is multiplied 1.5 times when wielding a weapon using two hands, or if you're a giant one-handing a two-handed weapon (they're heavy)
- [x] The player is unable to regenerate hit points while in the Valley of the Dead – healing via other methods (potions, healing spells) is still possible.
- [x] Stoning – monsters being stoned is a slow-stoning process, just as it is for the player.
- [x] Merged scimitar and saber skills.
- [x] Add celibate conduct (slashem)
- [x] Galloping duration depends on riding skills (idea by Beolach, code by Pasi Kallinen)

### Mold/Fungus ressurection
- (ported from Splicehack, which ported from xnh)
- Corpses have a chance to mold and grow a random F.
- Petrifying monster corpses only grow green mold.
- The ressurection rates have been tapered back a bit, so it's not as annoying as in Slash'EM - but there are more F monsters that can spawn (including moldiers at deeper levels)
- Moldy corpses can be used with different potions (see the brewing patch details)

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
- [x] See Invisible is now just temporary from potions, eating stalkers, and fountains (Evil). 
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
- [x] Added Psychic Resistance property (from Splice)
    - Illithids start with psychic res.
    - Protects you from forgetting skills, spells, and other things from potions and scrolls of amnesia, as well as mind flayer and zombie attacks.
    - Protects you from an illithids psionic wave and tentacle attacks.
    - Protects from most fear attacks, including dragon roars.
    - Wearing items that give psychic resistance stop you from being afraid.
- [x] Added LarvaCarrier property - some monsters can infect you with eggs!

### Upgrading/Tinker

- Upgrading mechanic (via potions of gain level from Slash'EM)
- Instead of crystal balls upgrading to magic markers, they upgrade to lenses (going to markers didn't make much sense, and it wasn't too useful since the marker would only have a handful of charges)
- I removed a lot of the weapon upgrades, since now a lot of those are covered by forge recipes. A few still remain for quarterstaves and boomerangs
- I also added upgrade paths for all special new armors
- The flintstone to healthstone exploit has been fixed; flintstones only upgrade one at a time.

### Gem Alchemy

    - [x] Acid is no longer a useless endgame potion!
    - [x] Gems dissolve in acid to produce new potions.
    - [x] If the acid is cursed there will be an alchemical explosion, otherwise there is always a 1 in 30 chance of an explosion.
    - [x] Use the object lookup (on a gem) to see what potion is created from a particular gem.

#### Scare Monster: 

- Several more creatures are immune: 
  - any unique monsters including the Wizard of Yendor, quest nemeses, the Riders.
  - lawful minions, Angels (A), Archangels
  - humans (any @), player monsters
  - honey badgers
  - anything that is mindless (EvilHack added)
  - Any monsters affected by conflict or in berserk mode are also immune.

#### Altar sacrificing

- Sacrificing for artifacts has been significantly changed in EvilHack/HackEM – there is a chance that your deity will gift you a regular item instead of an artifact. 
- The odds of being gifted an artifact go up as you increase in experience level. 
At XP 4, there's a 10% chance for an artifact gift, and the chances increment exponentially from there. 

- Regular object gifts are either be a weapon or a piece of armor
- For primary spellcasting roles (priest/wizard/etc), there's a chance of receiving a spellbook also. 
The gift received will always be blessed, enchanted, fixed, and there is a 1 in 8 chance that a desirable object property will be added to that object.

- Prayer timeout and the chances of receiving another gift are affected in the same way as if you had received an actual artifact.

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
- Can make you lose intrinsic telepathy.
- Psychic resistance protects from amnesia forgetting. Sources include rings of psychic resistance and being an illithid.

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
- Herbs that are left unattended will occasionally sprout more of the same kind around surrounding tiles.
- These include:
    - garlic, 
    - wolfsbane, 
    - catnip, 
    - carrots, 
    - mushrooms 
    - pineapples
    - kelp fronds,
    
- Some of these are not herbs but they still do spread prolifically. 
- Herbs in the water will move with water currents.
- One strategy to try is leaving these items on the ground early in the game and then coming back after a few thousand turns to see what you have "farmed."
- Level with trees have a chance of an additional axe (to aid with potential overgrowth)

#### Alchemy

- Alchemic blast damage is doubled, but acid resistance halves it (xnh)

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

- Potions can now be poured down into sinks (xnh). Allows for safer identification of potions by dumping them down the drain, leaving the hero to suffer only vapor effects. This is done by selecting a potion to #dip while
standing on top of a sink. If a potion of polymorph is poured down a sink, it polymorphs it.

## Dungeon Changes:

- [x] Adjust secret passage frequency and exclude them from DL 1-4 (xnh)
- [x] Random doors are secret less of the time (xnh)
- [x] Kicking a "wall" that is actually a secret of some sort will never injure the player and will always produce an unambiguous message that there's something there. This makes it a valid strategy to kick every wall suspected of hiding something, because a single kick will give a yes/no on whether it's a regular wall or not. It's still not a *great* strategy, because the real walls will still hurt and wound legs like normal. (xnh)

- [x] Extended the main dungeon: Now 35-39 levels deep
- [x] Ludios appears on the first eligible level.
- [x] Adjusted oracle (appears levels 5-9)
    - The fountains in Delphi can sometimes cause hallucination (splice)

- [x] Gnomish Mines:
    - Lengthened Gnomish Mines depth by 2 levels.
    - Added new different types of gnomes, dwarves, and thieves.
    - Added dwarf/gnome thieves to most town/minetown variants. Evil added
    - Monsters will usually be undead if you are a dwarf, gnome, or hobbit (from SlashEM) Instead of making undead dependant on your alignment, the chance scales with the dungeon depth. At the top of the mines, undead should be quite rare, but at mine's end beware.
    
    - Imported minetown variants from SlashTHEM and SpliceHack: 
    - Imported Mine's End levels from SlashTHEM
    - Imported Gnome King ending frmo SlashEM, SlashTHEM:

- [x] Sokoban
    - The last room of Sokoban has more prize variety, with a catch.

- [x] Imported the town branch from SlashTHEM/UnNethack
    - Appears 2-3 levels below the Oracle.
    - Imported Mall levels from SlashEM and added to town variants.
    - Imported Kobold level from SlashEM and added to town variants.
    - Created a Kobold "mall" level that is peaceful and has a lot of shops.
    - Created a "Raided Angband" level that is been ransacked by bandits and thieves.
    
- [x] Randomized "Themed Denizen" room (appears 10-14)
    - picked from:
    - The Rat Level (Slash'EM)
    - The Rat Level (SlashTHEM)
    - The Nymph Level
    - The Jermlaine Village (new in Hack'EM)

- [x] The Rogue level has been removed.

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
        - Acererak now guards the Staff of Rot

- [x] Imported Grund's Stronghold (appears 20-21)
    - Added more traps and moat, the stronghold wall is phaseproof.
    - Grund must be defeated to take the downstairs. 
    - If you have the amulet, he also must be defeated to take the upstairs (Infidels take notice!)

- [x] Extended Wyrm Caves branch (Appears level 21-22)
    - 5-6 levels deep
    - Created an entrance level (from dnh Erebor.des)
    - Created dynamic narrow passage filler levels that are always random.
    - At the bottom is a dangerous dragon nest a massive treasure horde.(from UnNetHack)
    
- [x] Imported Black Market (appears 23-24):
    - Reduce Black Market prices a bit.
    - Black Market Layout 1: Thinner normal shop.
    - Now undiggable and phaseproof.

- [x] Imported the Storage Room (appears 22-30)
    - Added 4 variations from dnh

- [x] Randomized "Lost SLASH'EM Levels Branch" (appears 25-30)
    - Instead of cluttering the dungeon with all the one-level branches from Slash'EM, a single one will be picked from these.
        - The Lost Tomb
        - Spider Caves
        - The Sunless Sea
        - The Giant Caves

- [x] Imported The Temple of Moloch (appears 26-29)

- [x] Gehennom
    - no longer a series of mazes, but are mines-style levels with lava. 
    - Areas such as the Sanctum and the entrance to the Wizard's Tower have also been adjusted.
    - Cerberus now guards the entrance to Gehennom.
    - Vlad the Impaler's tower is the vanilla version
    - Vibrating square: messages when player is near

- [x] Vecna's Domain
    - completely new and optional side branch has been added. Vecna is an uber-powerful Lich that is the source of power for all other liches. Because of this, all types of liches (including alhoons) cannot be genocided until Vecna is destroyed.

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
- [x] Convicts start with a spoon (which uses knife skill). Convicts get backstab bonuses (similar to Rogue) when wielding a spoon.
- [x] Town watchguards will get angry if they witness a convict on the loose - however, now you can wear a blindfold or towel (or be polymorphed) to disguise yourself.
- [x] For balance, vampiric convicts do not start on the edge of hunger - they get a little head start and start on the verge of satiated instead.

#### Infidel

- Infidels no longer start with drain life (Necromancers have the monopoly on that). In it's place, the spell of poison blast has been lowered to a level 2 spell so Infidels can start with that.

#### Flame Mage

- Flame Mage can only be Lawful/Neutral
- Flame Mages no longer gain cold resistance at level 13. (Their quest artifact still provides it though)
- Level 1: Intrinsic fire resistance
- Level 5: Gets "vulnerable to cold"
- Flame Mages also "like fire", so fire usually doesn't have an adverse affect on them.
- Cannot 2-weapon anymore
- Starts with more fire based inventory: potions of oil, fire bomb, scrolls of fire, fireproof quarterstaff and fireproof robe.
- Start with Fire Bolt instead of Flame Sphere (Fire Bolt is a Flame Mage exclusive spell)
- Can only specialize in Matter, Attack, and Divination spells.
- Casts spells using Wisdom; spellcasting bonuses are also calculated using wisdom.
- Left out the dragon polyself ability.
- Gets spell-hunger reduction (similar to wizards but it's calculated with (int - 2)
- Flame mages have multishot penalty.
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
  - Ice Mages no longer start with confuse monster, they always start with slow monster instead.
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
- Starts with a robe and wand of fear
- Removed starting scrolls; only starts with 1 ring.
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
- A new necromancer starts with 0 energy, but a high max of 99, so you can stock up on spirits first.

- [x] Level 1: Spirits grant (monsters level / 2) MP (minimum of 1).
- [x] Level 7: You get the monster's full level value in MP.
- [x] Level 13: You get the 1.5x monster's level value in MP.

##### Necromancer intrinsics changes

- [x] Level 1: Fear Resistance
    - Not afraid of "fear attacks", ie: banshees, bodaks
    - Not afraid of Noferatu gaze
    - Not afraid of ghosts or temple ghosts.
    - Note: They still can be afraid of dragon roars though.
- [x] Level 1: Drain resistance
- [x] Level 3: Undead warning
- [x] Level 18: Sickness resistance
- [x] Level 18: Poison resistance

##### Necromancer gameplay changes

- [x] Necromancers can eat wraiths with the vanilla success rate.
- [x] Undead will never betray Necromancers (lots of undead are treacherous in HackEM, so charm monster isn't as reliable as it used to be...)
- [x] Like convicts, Necromancers now cannot tame anything other than undead. Instead of taming, the best that occurs will be pacifying the target. 

- Instead of starting with a pile of books or learning techniques, the Necromancer learns special Necromancy spells as they level up. As a holdover from Slash'EM, they still start with the spell and spellbook of Drain Life.
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
- Their guaranteed sacrifice gift is the Holy Spear of Light.
- Elven slayers can start with silver elven daggers.
- Undead slayers do NOT have a multishot penalty.

#### Yeoman
	
- Starts with a green coat that grants MC2. 
- Starts with a horn, a torch, and a blessed whetstone.
- axe - can reach skilled
- Can reach skilled in daggers
- Their special spell is knock.

### Existing role changes

#### Archeologist

- Starts with 2 spellbooks; can start with torch or lamp
- Club moved from skilled to basic. 
- Spear skill added, can be trained to expert.
- Archeologists can highly enchant fedoras (xnh)
- Only archeologists get a luck bonus from wearing fedoras.

#### Barbarian

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
- they start the game knowing potions related to healing: sickness, regeneration, paralysis, sleeping, restore ability, blood, and all healing potions. They also recognize healthstones.
- Now receives the Drowsing Rod as a first sacrifice gift
- added clerical spell school, can train to skilled. 
- Gets poison resistance intrinsic at experience level 3 instead of experience level 1. 
- Trades warning intrinsic for sick resistance at experience level 15. 
- Can sense how wounded a monster is when attacking it, can also sense the wounded status of its pets.
- Healers get +1 blessed scalpel now.

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
- Monks get a 1AC bonus for being drunk (confused).
- Reverted vanilla changes to starting spells. Monks start with sleep instead of confuse monster.

#### Priest
    
- gets a +2 shield
- all slash and pierce weapons removed; can only train with blunt-type weapons. 
- Incurs a substantial to-hit penalty, abuses wisdom and can negatively impact alignment when trying to use an edged weapon (wielded or thrown).
- Their crowning gift is Mjollnir.

#### Ranger

- Ranger obtains twoweaponing, up to skilled.
- if playing as a gnome, their quest artifact becomes the Crossbow of Carl. Its properties mirror that of the Longbow of Diana, except that it is a crossbow instead of a bow.
- XL10+ Rangers automatically see ammo enchantments

#### Rogue

- Rogues get scrolls of teleport and gold detection (2 each) and oilskin sack
- Rogues also get darts; 30% chance that they are replaced with a pistol and bullets.
- new trainable skill known as 'thievery', which allows the rogue to pickpocket a target and steal items from their inventory (must be bare-handed to use, press shift + f and then a movement key in the direction of the target).
- If the target is peaceful, there is a chance the target will not notice depending on the players' skill in thievery.
- Critical strike from behind works while twoweaponing.
- Grimtooth is the first sacrifice gift for Rogues.
- Give Rogues backstab bonus on a whole bunch of other monster statuses (xnh)
- The backstab formula has been adjusted: Bonus based on skill with a cap.(xnh)

#### Samurai

- removed attack and clerical spell schools.

#### Tourist
    
- Tourists get automatic type identification for shop items
- Tourists get 2 extra magic maps, 2 extra food.	
- Tourists start with all of their optional equipment. 
- The magic marker that a tourist starts with is always 0:0.
- Twoweaponing: Tourists from skilled to basic.
- Tourists receive at most 1 PW per level up (they do not have any innate magic ability.)

#### Valkyrie

– removed escape spell school.
- Special spell changed to repair armor.
- Dwarven valkyries will start with a dwarvish bearded axe instead of a long sword, and are automatically proficient with it when using it to disarm weapons/shields.
- Since Valkyries don't have access to Excalibur - they are able to get the powerful Gungnir as compensation. Simply get a spear, enchant it up to +5, then pray while you have "pious" alignment. Your god will turn the spear into Gungnir for you.

#### Wizard

- removed numerous weapons from being trained (axe, short sword, club, mace, polearms, spear, trident, shuriken). 
- Enchantment spell skill raised from skilled to expert, 
- clerical spell skill lowered from skilled to basic. 
- Can sense if objects are magical in nature, much the same way a priest knows the beatitude of all objects.
- Wizards start with magic whistles, harps, flutes, and drums identified. Wizards can see the magical nature of tools, so by process of elimination a wizard should know these objects. Otherwise we get an awkward situation of seeing a magical whistle, then it auto-identifies when used to a magic whistle.
- New starting pet, the pseudodragon. 

## Race changes:

### New player races: 

#### Centaurs

#### Giants

- Giants can wear mummy wrappings.
- Any giant can wear large splint mail - not just Samurai.

#### Hobbits

- [x] Hobbit race (already in evil)
- [x] Hobbits start out knowing Elbereth


#### Illithids

Starts with the Psionic Wave spell:
    - Does 2d6 psychic damage
    - Level 26, this increases to 4d6 psychic damage.

#### Vampires

- Vampirics get an extra attack – a drain life bite
- Can only drain corpses and drink blood.
- From Splice: Vampirics get almost double the nutrition from feeding on life blood, they start with both blood potions known.
- Added amusing custom messages for draining corpses.
- Vampires who wear an Opera cloak get a +1 Charisma bonus
- Can be chaotic or neutral.

- I tried to space out the intrinsics a bit, since it felt like vampires started out overpowered in Slash'EM - but also didn't benefit from Poison or Sleep that vampires usually enjoy.
| Level | Ability             |
| ------|---------------------|
| Level 1: | Drain resistance    |
| Level 1: | Wither resistance(*)    |
| Level 1: | Vulnerable to fire  |
| Level 3: | Breathless          |
| Level 5: | Regeneration        |
| Level 7: | Poison resistance   |
| Level 9: | Flying              |
| Level 16: | Sleep resistance    |
| Level 21: | Sickness resistance |

(*) mummies instead inflict regular physical damage to withering resistant monsters.

Weaknesses:
- Vulnerable to silver.
- Vampires can wield and wear silver items, but it damages them and blocks their natural regeneration
- Restricted in two-weapon skill.
- Start with a small penalty to luck (-1) and alignment (-5)
- Disabled #monster polymorph for the vampiric race.

### Existing role changes

#### Elves

- Elves always start out knowing Elbereth.
- Elves can use musical instruments without risk of breaking them.
- Elves can always squeeze between two trees
- Elves and rangers get alignment penalty for cutting down trees
- Elves can only be lawful.

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
                    (Note: Armor shops will refuse to work on dragon scales.)
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

#### Misc shop changes

- [x] Lighting shops can now carry wands/scrolls/spellbooks of light, as well as the rare wand of lightning.
- [x] Delis can carry meat sticks, royal jelly, and huge chunks of meat.
- [x] Hardware stores stock potions of oil, and rarely touchstones (xnh)


## Credits

### Special thanks to:
- My wife - for being endlessly patient with this time-consuming endeavor!
- K2, amateurhour, qt

### Thanks: 
- riker, bhaak, kes, shadowrider, aosdict, paxxed, mobileuser, krm26, Umbire

### Playtesters: 
- nh2465, cbus


==========================

