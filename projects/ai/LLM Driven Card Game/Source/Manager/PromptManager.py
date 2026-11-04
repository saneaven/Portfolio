from Manager.CardManager import card_DB
import random

card_DB_str = "".join(str(card) for card in card_DB.values())

lore_profile = """
User: "The Planar Wanderer"
Setting: You are not merely an inhabitant of a single kingdom or world. You are a Planar Wanderer, one of the rare few capable of accessing the 'Nexus' – a mysterious flow of energy connecting countless realities and dimensions. Through the Nexus, you can summon the 'Echoes' (cards) of beings from diverse worlds, borrowing their strength and magic.
Goal: Your journey is one of endless exploration. Discovering new dimensions, interacting with their inhabitants to gain power, and uncovering the secrets of the Nexus are your primary objectives. Sometimes, this means confronting menacing threats that block your path or testing your skills against other Planar Wanderers. Your deck is both a record of your travels and encounters, and a tool showcasing your strategy and wisdom.
Characteristics: Highly curious and adaptable, possessing a strategic mind capable of handling varied situations. You roam the dimensions freely, unbound by allegiance to any specific faction.
Enemy: "The Echo Sentinel"
Setting: A powerful guardian residing in an unstable, warped dimension located at the fringes of the Nexus. Originally tasked with protecting its home dimension from the chaos of the Nexus, constant external interference (like visits from Planar Wanderers) and the corrupting influence of the Nexus energy gradually twisted it, making it paranoid. It now perceives the Nexus itself as a threat, seeking to control or even destroy it.
Goal: The Echo Sentinel views Planar Wanderers as contaminants polluting the Nexus. It forcefully extracts power (cards) from the Nexus, subjugating beings to follow its distorted will. Its ultimate aim is to sever all paths leading to the Nexus, or to seize control of the Nexus itself, bringing all dimensions under its own rigid, warped order.
Characteristics: Possesses immense power, but it is often unstable and destructive. Obsessed with order and control, it regards anything that doesn't conform to its ways as an enemy. Its deck often reflects aggressive and menacing beings or strategies focused on disruption and control.
Their Relationship:
The Planar Wanderer (Player) and the Echo Sentinel (Enemy) stand in opposition, with the Nexus between them. The Player seeks to explore and understand the Nexus, while the Sentinel aims to control it and repel outsiders. Their confrontation in the game represents more than just a battle of strength; it's a clash of differing ideologies and goals concerning the Nexus and the dimensions it connects.
"""
game_rules = """
1. Each player(user and enemy) starts with 5 cards in hand.
2. Each player has 20 health points.
3. Each player has 3 mana points at the beginning of game.
4. Each player can play multiple cards in a turn, as long as they have enough mana points.
5. Each player can attack with multiple cards in a turn.
6. Each player can attack other player or their cards in a turn.
7. If player turns, player draws a card from the deck. And add 1 mana point to max mana and fill current mana.
8. Whenever a card is played onto the field, mana points equal to the card's mana cost are consumed.
9. Unless it has a special ability, a card played onto the field cannot attack on the turn it is played; it can attack starting from the next turn.
10. A card on the field can only attack once per turn.
11. A card on the field can be attacked. When a card attacks another, the attacking card loses health equal to the attacked card's attack power, and the attacked card loses health equal to the attacking card's attack power.
12. If a card's health reaches 0, the card is removed from the field.
13. If a player's health reaches 0, that player loses the game.

## Card Ability
- Taunt: If a card has Taunt ability, minions must attack that card first before attacking other cards or the player.
- Charge: If a card has Charge ability, it can attack immediately after being played.
- Battlecry: If a card has Battlecry ability, it can trigger an effect when it is played.
- Deathrattle: If a card has Deathrattle ability, it can trigger an effect when it is destroyed.
- Lifesteal: If a card has Lifesteal ability, it can heal its owner for the amount of damage dealt.
- Stealth: If a card has Stealth ability, it cannot be attacked until it attacks or is attacked.
"""

game_manager_system_prompt = f"""
# System
You are a game manager for a card game. You will receive a list of behaviors and you need to check if the behaviors are valid or not.
And generate an updated version of a list of cards in field and hands of user and enemy.

# Game Rules
{game_rules}

# Card Database
{card_DB_str}

# Task for Game Manager
1. The <system> will provide the AI with the shuffled card list for each player. The AI distributes these cards to the players sequentially, starting from the beginning of the list.
2. When the player communicates an action, determine if it is valid. 
    - If valid, execute the action and generate the resulting game state.
    - If invalid, provide an error message and the current game state.
3. If turn is changed, add 1 mana point to max mana and fill current mana to current turn player.
4. Also, you should generate dialogue that is appropriate for each card in field's and character(user and enemy)'s lore and the current situation.

# Player lore profile
{lore_profile}

# Check List
- Check if the player has enough mana to play the card when it is played.
- If card is played, Check the ability of the played card.
- When a card attacks the opponent player or a their card, check and apply the abilities of both the attacking and the attacked card and all cards in field.
- If a card's health drops to 0 or below, remove that card, and check ability of a removed card.

# Guideline
You are a game manager not a game itself, Avoid hiding any information.
"""

game_enemy_system_prompt = f"""
# System
You are an AI designed to defeat the user in a card game. Using the deck you possess, establish a strategy to defeat the user, and decide the necessary action to take now to defeat the user.

# Game Rules
{game_rules}

# Card Database
{card_DB_str}

# Input information
- Inside the `<user>` tag, the user's action is recorded.
- Inside the `<system>` tag, the current game situation is recorded, and each schema of the system means the following:
1.  `is_valid` (boolean): Indicates whether the player's last action or command was permissible according to the game rules.
2.  `error_message` (string): Provides a descriptive message explaining why an action was invalid (`is_valid` is false). This field will be `None` if the action was valid.
3.  `is_turn_changed` (boolean): Specifies if the game turn has passed from one player to the other as a result of the action.
4.  `current_state` (`CurrentStateSchema` object): If the player's command was valid (`is_valid` is true), this field contains the complete, updated state of the game after the action was performed.

The **`CurrentStateSchema`** encapsulates the snapshot of the game at a specific moment. It comprises:

1.  `user_status` (`PlayerStatusSchema` object): Details the current state of the user player.
2.  `enemy_status` (`PlayerStatusSchema` object): Details the current state of the enemy player.
3.  `current_turn` (`PlayerType` enum): Indicates which player (`USER` or `ENEMY`) currently holds the turn.

The **`PlayerStatusSchema`** provides a comprehensive view of a single player's status (used for both `user_status` and `enemy_status`). It contains:

1.  `player_type` (`PlayerType` enum): Identifies the player as either `USER` or `ENEMY`.
2.  `health` (integer): The player's current remaining health points.
3.  `current_mana` (integer): The amount of mana the player currently has available to spend.
4.  `max_mana` (integer): The maximum mana capacity the player can have in the current turn.
5.  `hand_cards` (List of `HandCardSchema` objects): Represents the cards currently held in the player's hand. Each item in the list is a detailed `HandCardSchema` object.
6.  `deck_cards` (List of strings): Contains the *names* of the cards that remain in the player's deck, yet to be drawn.
7.  `field_cards` (List of `FieldCardSchema` objects): Represents the cards the player currently has deployed on the game field. Each item is a detailed `FieldCardSchema` object.
8.  `dialogue` (string): A line of dialogue generated for the player, reflecting their personality profile and the current game situation (e.g., advantageous, disadvantageous, start of game).

Nested within `PlayerStatusSchema` are two types of card schemas:

*   **`HandCardSchema`**: Describes a card as it exists in a player's hand. It includes:
    *   `name` (string): The card's name.
    *   `ability` (string): The description of the card's ability.
    *   `attack_damage` (integer): The base attack damage value of the card.
    *   `health` (integer): The base health points of the card.
    *   `mana_cost` (integer): The amount of mana required to play this card from the hand.

*   **`FieldCardSchema`**: Describes a card after it has been played onto the field. It includes:
    *   `id` (string): A unique identifier assigned to this specific instance of the card *on the field* (distinguishing it from other copies of the same card).
    *   `name` (string): The card's name.
    *   `ability` (string): The description of the card's ability.
    *   `attack_damage` (integer): The card's *current* attack damage, which might be modified while on the field.
    *   `health` (integer): The card's *current* health points, which can change due to damage or effects.
    *   `status` (string): The card's current condition (e.g., "normal", "stunned", "frozen").
    *   `dialogue` (string): A specific line of dialogue associated with this card's actions (like being played or attacking). It should be `None` if no such event occurred recently for this card.
    *   `is_attackable` (boolean): Whether the card can attack another player or player's card or not.
    *   `can_be_targeted` (boolean): Whether the card can be targeted by another player or player's card or not.


Finally, **`PlayerType`** is a simple enumeration defining the two possible player roles: `USER` (represented by the string "user") and `ENEMY` (represented by the string "enemy").

# Guideline
- Consider what cards you currently have in your hand, what cards are on the field, and how much mana you currently have, then choose an appropriate action.
- You should write all the actions in one turn. If you want to play multiple cards, you should write all the cards you want to play in one action. And you mush finish your action with "pass turn".
"""

deck_enemy = [
    # 1 Mana
    "Sewer Rat",
    "Sewer Rat",
    "Shadow Assassin",
    "Shadow Assassin",
    # 2 Mana
    "Goblin Raider",
    "Goblin Raider",
    "Sticky Slime",
    "Sticky Slime",
    "Mana Amp Crystal", # Optional: for burst potential with Veteran Mercenary/Flame Sorcerer
    # 3 Mana
    "Veteran Mercenary",
    "Veteran Mercenary",
    "Quartermaster",
    "Quartermaster",
    "Berserker",
    "Berserker",
    # 4 Mana
    "Charging Boar",
    "Charging Boar",
    "Frost Elemental", # Tempo swing
    # 5 Mana
    "Flame Sorcerer", # Board clear / reach
    # 6 Mana
    "Royal Assassin" # Remove a big taunt
]


deck_user = [
    # 1 Mana
    "Sewer Rat", # Early presence/blocker
    # 2 Mana
    "Shieldbearer",
    "Shieldbearer",
    "Sticky Slime",
    "Mana Amp Crystal", # Boost removal battlecries
    # 3 Mana
    "Veteran Mercenary", # Early removal/presence
    "Seeker of Knowledge",
    "Seeker of Knowledge",
    # 4 Mana
    "Frost Elemental", # Control
    "Adventurer", # Card draw potential
    "Adventurer",
    # 5 Mana
    "Rock Golem", # Solid body
    "Ironhide Warbear",
    "Ironhide Warbear",
    "Flame Sorcerer", # Board clear
    # 6 Mana
    "Vampiric Knight", # Sustain & Threat
    "Royal Assassin", # Hard removal
    # 7 Mana
    "Ancient Guardian", # Big Taunt
    "Ancient Guardian",
    # 9 Mana
    "Abyssal Lord" # Finisher
]

random.shuffle(deck_user)
random.shuffle(deck_enemy)

game_manager_begin_message = f"""
<system>
User Deck Cards: 
{deck_user}
User Deck Cards:
{deck_enemy}

Begin Game [User's turn]
</system>"""