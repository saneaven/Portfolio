from pydantic import BaseModel, Field
from typing import Union, Optional, List, Dict, Any
import enum

class EnemyOutputSchema(BaseModel):
    action: str = Field(
        ...,
        description="""Order of the enemy's action. It should be a string that describes the order of the enemy's action like "attack A With B" or "play C". If there is no available action, output should be "pass turn".""",
    )

class FieldCardSchema(BaseModel):
    id: str = Field(..., description="Unique identifier of the card in field")
    name: str = Field(..., description="Name of the card")
    ability: str = Field(..., description="Ability of the card")
    attack_damage: int = Field(..., description="Current state of attack damage of the card")
    health: int = Field(..., description="Current state of health points of the card")
    status: str = Field(..., description="Current state of the card like normal, stunned, frozen, etc.")
    dialogue: str = Field(..., description="Dialogue of the card when it is played or attacked. If it is not played or attacked, it should be None.")
    is_attackable: bool = Field(..., description="Whether the card can attack another player(user or enemy) or player's card or not.")
    can_be_targeted: bool = Field(..., description="Whether the card can be targeted by another player(user or enemy) or player's card or not. Example: If a card has the stealth ability and it is active, it cannot be targeted; if there is a friendly minion on the field with the Taunt ability, only minions with Taunt can be targeted.")

    def __str__(self):
        return f"""
[{self.name}] {"@" if not self.is_attackable else ""} {"//" if not self.can_be_targeted else ""}
atk: {self.attack_damage} hp: {self.health}
{self.ability}
{self.dialogue}
"""
    
class HandCardSchema(BaseModel):
    name: str = Field(..., description="Name of the card")
    ability: str = Field(..., description="Ability of the card")
    attack_damage: int = Field(..., description="Current state of attack damage of the card")
    health: int = Field(..., description="Current state of health points of the card")
    mana_cost: int = Field(..., description="Mana cost to play the card")

    def __str__(self):
        return f"""
[{self.name}] cost: {self.mana_cost}
atk: {self.attack_damage} hp: {self.health}
{self.ability}
"""

class PlayerType(enum.Enum):
    USER = "user"
    ENEMY = "enemy"

class PlayerStatusSchema(BaseModel):
    player_type: PlayerType = Field(
        ...,
        description="Type of the player.",
    )
    health: int = Field(
        ...,
        description="Current health points of the player.",
    )
    current_mana: int = Field(
        ...,
        description="Current mana points of the player.",
    )
    max_mana: int = Field(
        ...,
        description="Max mana points of the player.",
    )
    hand_cards: List[HandCardSchema] = Field(
        ...,
        description="List of the name of cards in hand of the player.",
    )
    deck_cards: List[str] = Field(
        ...,
        description="List of the name of remain cards in deck of the player.",
    )
    field_cards: List[FieldCardSchema] = Field(
        ...,
        description="List of the cards in field of the player.",
    )
    dialogue: str = Field(
        ...,
        description="Dialogue of the player. Dialogue should be generated based on their profile and reflecting the current situation, such as whether it's the beginning, advantageous, disadvantageous, etc.",
    )

class CurrentStateSchema(BaseModel):
    user_status: PlayerStatusSchema = Field(
        ...,
        description="Current state of the user.",
    )
    enemy_status: PlayerStatusSchema = Field(
        ...,
        description="Current state of the enemy.",
    )
    current_turn: PlayerType = Field(
        ...,
        description="Current turn of the game.",
    )

class ManagerOutputSchema(BaseModel):
    is_valid: bool = Field(
        ...,
        description="Whether the behaviors are valid or not.",
    )
    error_message: str = Field(
        ...,
        description="If player's command is invalid, generate error message. If valid, it should be None.",
    )
    is_turn_changed: bool = Field(
        ...,
        description="Whether the turn is changed or not.",
    )
    current_state: CurrentStateSchema = Field(
        ...,
        description="If player's command is valid, Generate current state of the game.",
    )
