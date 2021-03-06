TL;DR
===========
* Turn based
* Order of action in turns are decided by initiative and a D10(OP10)
* Given a number of action points and weapon actions each turn
* action points (AP) generic, used for all actions.
* weapon actions (WA) are the number of actions (attacks & blocks) you can do with that weapon each turn.
* For attacks and blocks you chose a number of AP to use. If a D20 is less or equal to that you succed
** 1 is always critical hit/ perfect block
** 20 is always fatal miss
* When you are prompted to block you may only block or not block.
** If you succed with the block damage are only applied on the weapon
** If you chose not to block you have a chance for passive protection if you have a shield
* Initiative is important! Hight initiative means you get to attack instead of using up all your WA on blocks

The info line above the prompt looks like this:
[ AP: 10 | HP: 24 | AM: 0 | ARM: 55/5 || RH - WA: 2 | DUR: 100/10 || LH - WA: 1 | DUR 15/1 ]
AP: Action Points
HP: Life
AM: Action Mod (number added to all D20 rolls)
ARM: Armor, first number is durability, second is damage reduction
RH: Right Hand
LH: Left Hand
WH: Weapon Action
DUR: Durability (works in same way as armor)

Basic Commands
=============
The order of commands etc never matter, so you can write things in what order you want.
You can tab-complete all commands as long as you are in the first word on the line, in all other context
all items and characters are tab-completed.

## Most used commands
* help - show all commands
* go north/south/east/west - go in direction.
	You can also type just n/s/e/w or north/south/east/west
* attack [action points] [character] [left/right hand]
	Attack the specified character with the given amount of points and the specified hand.
	Just typing attack will try to attack:
** The Character you are in a fight with atm (if you don't are in a fight you have to specify character)
** Your defaultattack settings points (set with defaultattack [points]), if you don't have that set you will have to specify each time
** The hand which have a weapon and have weapon actions left
* inventory/backpack - show your inventory
* equipment - show your equipment
* stats/show attributes - show your attributes (named show attr.. and not just attributes due to completion collision with attack, which is far more common)
* talk to [character]
* pick up/take [item]
* next - End your turn
* look around - Describe surroundings
* use [item] - use item, mostly with health potions
* descripe/inspect [item] (if item is not given it's an alias for look around)
* open/close [chest]
* drop [item]
* [un]equip [item]
* defaultattack [points] - set default number of points for attack
* defaultblock [points] - set default number of points for block

When specifying a npc or an item you need to type something that matches any whole word in the npc's/item's name.
If you can not uniquely specify a item (due to two items having the same name) you may type a number after
* inspect longsword 2 - would inspect the second match for the word longsword

Game system
==========
You and all other characters have a given set of attributes, these specify your abilities (mostly in battle).
The primary attributes are: (for all except damage_overpower a higher number is better)

* action_points - used for actions during turn
* initiative - order of attack
* strength - decides how much you can carry and extra damage (carry weight = strength * 10, extra dmg = strength/10)
* life - your life, loose it all and you die.

All equipment have attributes to (the sum of these and your attributes are show with stats/show attributes, but
for many weapons it is the individual stat per item that counts (marked with [item specific] below)

* armor
* backpack_volume - Decides how much you volume you can keep in your inventory
* damage_extra [item specific]
* damage_overpower [item specific]
* damage_dices [item specific]
* durability [item specific]
* weapon_actions [item specific]
* passive_protection [item specific]
* action_mod - added to each attack/block roll

## The dices
The following dices are used in the game
* D5
* D10
* D20

Overpower is specified below as (OPX) which means that a roll above or equal to X gives an extra roll
(Ex: With a D10(OP9): First roll: 9 (get to reroll), second roll: 2, total value: 11)

### A D20-check
A D20 check is a roll with a D20 against a given number of action points (X)
* A 1: critical roll
* 20: fatal (even if X >= 20)
* <= X: success
* > X: failure

## The game rules
__Cursive text below marks atttributes__
The rules are based on the RPG "Drakar och Demoner: Trudvagn" (which are loosly based on DnD).

The game is turn based. The order in which the characters get to act is decided by their _initiative_ plus a D10(OP10)

Each turn the player have a number of _action points_ to use, decided by their attribute for that.

Action points are used for all (most) actions. Some common costs are:
* movement cost: Varies from area to area, the cost of moving into an area. This is the reason why you might get "need more action points" when you try go in a direction. If so you need to find items that boosts your AP.
* picking up items: 5
* equiping item: 5
* using an item: 5
* opening an chest: 5
* attack/block: Decided by player

If you try a action that you don't have enough points for and you are not in a fight the game will automaticly end your turn,
and retry the action next turn.

After each turn
--
Is printed, to make sense of what actions belong to which turns

### The battle
When a player attack (A) another player (B) , the following happens

1. Player A decides how many action points to use on the attack, and which weapon to use
1. _weapon_actions_ decides how many actions each weapon can do each turn (including blocks)
1. A D20-check is rolled for Player A against the spent number of action points.
* Critical roll: An extra damage dice and can't be blocked with passive protection
* Fatal roll: -5 on next attack or block
1. If the attack hit damage dices are rolled (_damage_dices_ (or 1 if it's not present)  + 1 if critical hit), D10(OP _damage_overpower_ )
1. Player A's _strength_/10 is added to the damage (rounded down)
1. Without knowing how much damage was rolled Player B get to decide if he want to block or not.
* If B blocks, he rolls a D20 check if he succeed.
** If he succeds the damage is reduced by  _durability_ of the weapon (or shield) he blocked with divided by 10 is (rounded down, but not less than 1)
	and then taken from the items durability.
** If he lands a perfect block all damage is absorbed (no durability loss), and he get +5 on next attack or block
** If he decide not to block, a D20 is rolled against the weapon with best passive protection (mostly shields).
	A passive block can never be perfect or fatal, but if it succed it counts as a normal block.
	A passive block don't occur if the player tries to block (but fails)
** If he fails to block the damage is reduced with _armor_/10 and then applied to BOTH armor and life.
1. If Player A still have action points left he may continue to attack

Blocking: most of blocking have been described in the previous text, but your decition to block is prompted when you are attacked,
you may then do one of the following:
* Enter a black row (just press enter) - Don't block (try for a passive block if you have a shield)
* Enter block [left/right] [action points]
** Just as action you can set default action points with defaultblock, and you don't need to specify a hand to use
* You must have weapon actions left for the weapon to block


Items
=======

Items are randomly generated. Some chests and enemies have higher chance of better items.
Most item's base values are slightly random, but in a tight range. The importart part are the affixes.
An item can have up to 3 affixes:
* 0 affixes: Normal (white)
* 1 affix: Magic (blue)
* 2 affix: Rare (purple)
* 3 affix: Legendary (orange)

The prefixes adds different attributes, and the name of the prefix hint's at what it does.

Some items are red, these are not better in any way, but are unique/quest items or
