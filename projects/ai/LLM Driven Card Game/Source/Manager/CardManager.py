from pydantic import BaseModel, Field

# Corrected Card Class Definition
class Card(BaseModel):
    name: str = Field(..., description="Name of the card")
    ability: str = Field(..., description="Ability of the card")
    attack_damage: int = Field(..., description="Attack damage of the card")
    health: int = Field(..., description="Health points of the card")
    mana_cost: int = Field(..., description="Mana cost to play the card")
    lore: str = Field(..., description="Flavor text or lore of the card")

    def __str__(self): # Optional: for better printing of Card objects
        return f"""
[{self.name}]
 mana cost = {self.mana_cost}
 atk = {self.attack_damage}
 hp = {self.health}
 ability = {self.ability}
 lore = {self.lore}
"""
    def toString(self):
        return self.__str__()
    

# Dictionary to store all card objects
card_DB = {}

# --- Card Definitions ---

# Card 1: Sewer Rat
name = "Sewer Rat"
card_DB[name] = Card(
    name=name,
    ability="", # No special ability
    attack_damage=1,
    health=2,
    mana_cost=1,
    lore="A small, swift rat living in the city's dark sewers. Numerous, but usually not threatening... usually."
)

# Card 2: Goblin Raider
name = "Goblin Raider"
card_DB[name] = Card(
    name=name,
    ability="", # No special ability
    attack_damage=3,
    health=2,
    mana_cost=2,
    lore="Low intelligence, but charges recklessly upon command. Can inflict unexpected damage on unwary foes."
)

# Card 3: Shieldbearer
name = "Shieldbearer"
card_DB[name] = Card(
    name=name,
    ability="Taunt", # Ability: Taunt
    attack_damage=1,
    health=4,
    mana_cost=2,
    lore="A dependable soldier hiding behind a sturdy shield, protecting allies. To pass, you must first defeat him."
)

# Card 4: Veteran Mercenary
name = "Veteran Mercenary"
card_DB[name] = Card(
    name=name,
    ability="Battlecry: Deal 1 damage.", # Ability: Battlecry
    attack_damage=3,
    health=3,
    mana_cost=3,
    lore="A seasoned mercenary who has survived countless battlefields. Delivers a sharp blow upon arrival."
)

# Card 5: Seeker of Knowledge
name = "Seeker of Knowledge"
card_DB[name] = Card(
    name=name,
    ability="Battlecry: Draw a card.", # Ability: Battlecry
    attack_damage=2,
    health=2,
    mana_cost=3,
    lore="A scholar searching for ancient knowledge and secrets. His discoveries sometimes hold the key to turning the tide."
)

# Card 6: Charging Boar
name = "Charging Boar"
card_DB[name] = Card(
    name=name,
    ability="Charge", # Ability: Charge
    attack_damage=4,
    health=2,
    mana_cost=4,
    lore="A ferocious beast that rams anything blocking its path. Very dangerous if encountered in the wild."
)

# Card 7: Rock Golem
name = "Rock Golem"
card_DB[name] = Card(
    name=name,
    ability="", # No special ability
    attack_damage=5,
    health=5,
    mana_cost=5,
    lore="A giant mass of rock animated by magic. Possesses great strength but no special abilities. Threatening in its own right."
)

# Card 8: Vampiric Knight
name = "Vampiric Knight"
card_DB[name] = Card(
    name=name,
    ability="Lifesteal", # Ability: Lifesteal
    attack_damage=4,
    health=6,
    mana_cost=6,
    lore="A cursed knight who heals his wounds by absorbing the life force of his enemies. His blade glows crimson."
)

# Card 9: Ancient Guardian
name = "Ancient Guardian"
card_DB[name] = Card(
    name=name,
    ability="Taunt", # Ability: Taunt
    attack_damage=7,
    health=7,
    mana_cost=7,
    lore="A colossal being that has guarded a specific place for ages. When awakened, it blocks the path, daring foes to pass."
)

# Card 10: Flame Sorcerer
name = "Flame Sorcerer"
card_DB[name] = Card(
    name=name,
    ability="Battlecry: Deal 2 damage to all enemy minions.", # Ability: Battlecry
    attack_damage=3,
    health=3,
    mana_cost=5,
    lore="A sorcerer who commands the power of flame at will. Fire engulfs the battlefield upon his arrival."
)

# Card 11: Shadow Assassin
name = "Shadow Assassin"
card_DB[name] = Card(
    name=name,
    ability="Stealth", # Ability: Stealth
    attack_damage=2,
    health=1,
    mana_cost=1,
    lore="An assassin hiding in the shadows, aiming for the enemy's vitals. Most dangerous when unseen."
)

# Card 12: Sticky Slime
name = "Sticky Slime"
card_DB[name] = Card(
    name=name,
    ability="Deathrattle: Summon a 1/1 Slime.", # Ability: Deathrattle
    attack_damage=1,
    health=1,
    mana_cost=2,
    lore="A sticky and annoying creature. Even when dealt with, a small piece remains, squirming."
)

# Card 13: Quartermaster
name = "Quartermaster"
card_DB[name] = Card(
    name=name,
    ability="Battlecry: Give a friendly minion +1/+1.", # Ability: Battlecry
    attack_damage=2,
    health=3,
    mana_cost=3,
    lore="An officer managing supplies and boosting soldiers' morale. His support makes allies stronger."
)

# Card 14: Berserker
name = "Berserker"
card_DB[name] = Card(
    name=name,
    ability="While damaged, this minion has +2 Attack.", # Ability: Enrage (described)
    attack_damage=2,
    health=4,
    mana_cost=3,
    lore="A warrior who turns pain into fury. The more wounded, the fiercer they become."
)

# Card 15: Frost Elemental
name = "Frost Elemental"
card_DB[name] = Card(
    name=name,
    ability="Battlecry: Freeze an enemy character.", # Ability: Battlecry (Freeze)
    attack_damage=3,
    health=4,
    mana_cost=4,
    lore="An elemental that commands chilling cold. Its breath freezes the enemy's movements."
)

# Card 16: Adventurer
name = "Adventurer"
card_DB[name] = Card(
    name=name,
    ability="Whenever this attacks and destroys a minion, draw a card.", # Ability
    attack_damage=3,
    health=4,
    mana_cost=4,
    lore="One who braves dangers to explore the unknown. Finds new opportunities amidst the fray."
)

# Card 17: Mana Amp Crystal
name = "Mana Amp Crystal"
card_DB[name] = Card(
    name=name,
    ability="Your Battlecry damage effects deal +1 damage.", # Ability: Aura/Passive
    attack_damage=0,
    health=3,
    mana_cost=2,
    lore="A mystic crystal that amplifies nearby mana. Enhances the power of magic."
)

# Card 18: Ironhide Warbear
name = "Ironhide Warbear"
card_DB[name] = Card(
    name=name,
    ability="Taunt", # Ability: Taunt
    attack_damage=4,
    health=6,
    mana_cost=5,
    lore="A massive bear armored in thick iron plates. Serves as a sturdy shield on the battlefield."
)

# Card 19: Royal Assassin
name = "Royal Assassin"
card_DB[name] = Card(
    name=name,
    ability="Battlecry: Destroy a minion.", # Ability: Battlecry
    attack_damage=4,
    health=4,
    mana_cost=6,
    lore="A secret assassin acting under royal command. Eliminates any target with certainty."
)

# Card 20: Abyssal Lord
name = "Abyssal Lord"
card_DB[name] = Card(
    name=name,
    ability="Battlecry: Give your other minions +2/+2.", # Ability: Battlecry
    attack_damage=7,
    health=7,
    mana_cost=9,
    lore="A powerful demon lord risen from the depths of the abyss. His arrival empowers allies and plunges foes into despair."
)


# # --- Verification ---
# print(f"Total cards in database: {len(card_DB)}")

# # Print details of one card as an example
# print("\nExample Card (Veteran Mercenary):")

# # Print all card names and basic stats using the __repr__ method
# print("\n--- All Cards ---")
# for card_name, card_obj in card_DB.items():
#     print(card_obj)