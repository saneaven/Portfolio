import os
from google import genai
from google.genai import types
from dotenv import load_dotenv
from Manager.PromptManager import game_manager_begin_message, game_manager_system_prompt, game_enemy_system_prompt
from Manager.SchemaManager import ManagerOutputSchema, EnemyOutputSchema
from Manager.DisplayManager import displayResponse
from Manager.ChatLogManager import ChatMessage, GameRole, convertChatLogToContentForUser, convertChatLogToContentForEnemy
from typing import List
import enum


load_dotenv()
api_key = os.getenv("API_KEY") 
client = genai.Client(api_key=api_key)

            
class GameState(enum.Enum):
    USER_TURN = 1
    ENEMY_TURN = 2
    GAME_OVER = 3



# gemini-2.5-pro-exp-03-25

# Only for paid tier api key
## gemini-2.5-flash-preview-04-17 
## gemini-2.5-pro-preview-03-25

LLM_MODEL_ID = "gemini-2.5-pro-exp-03-25"


def userTurn(chatLog : List[ChatMessage]) -> GameState:
    is_turn = True
    while is_turn:

        user_input = input("Write Instruction: (Type 'exit' to end the game): ")
        if user_input == "exit":
            return GameState.GAME_OVER
        
        chatLog.append(
            ChatMessage(
                role=GameRole.USER,
                content=user_input,
            )
        )

        contents = convertChatLogToContentForUser(chatLog)

        print("Loading Game...")
        response = client.models.generate_content(
            model=LLM_MODEL_ID, contents=contents, 
            config=types.GenerateContentConfig(
                system_instruction = game_manager_system_prompt,
                response_mime_type= 'application/json',
                response_schema = ManagerOutputSchema,
                temperature=0.1
            )
        )

        chatLog.append(
            ChatMessage(
                role=GameRole.SYSTEM,
                content=response.text,
            )
        )

        displayResponse(response.parsed)
        print("---")

        if response.parsed.is_turn_changed:
            print("Turn Changed")
            return GameState.ENEMY_TURN
        if response.parsed.current_state.user_status.health <= 0:
            print("User is Dead")
            return GameState.GAME_OVER
        if response.parsed.current_state.enemy_status.health <= 0:
            print("Enemy is Dead! You Win!")
            return GameState.GAME_OVER





def enemyTurn(chatLog : List[ChatMessage]) -> GameState:
    is_turn = True
    while is_turn:
        # gemini-2.0-flash
        # gemini-2.5-pro-exp-03-25
        
        contents = convertChatLogToContentForEnemy(chatLog)

        print("Enemy is Thinking...")
        response = client.models.generate_content(
            model=LLM_MODEL_ID, contents=contents, 
            config=types.GenerateContentConfig(
                system_instruction = game_enemy_system_prompt,
                response_mime_type= 'application/json',
                response_schema = EnemyOutputSchema,
                temperature=0.1
            )
        )

        chatLog.append(
            ChatMessage(
                role=GameRole.ENEMY,
                content=response.text
            )
        )

        print("---")
        print("Enemy Action: ", response.parsed.action)
        print("---")

        contents = convertChatLogToContentForUser(chatLog)
        
        print("Loading Game...")
        response = client.models.generate_content(
            model=LLM_MODEL_ID, contents=contents, 
            config=types.GenerateContentConfig(
                system_instruction = game_manager_system_prompt,
                response_mime_type= 'application/json',
                response_schema = ManagerOutputSchema,
                temperature=0.1
            )
        )

        chatLog.append(
            ChatMessage(
                role=GameRole.SYSTEM,
                content=response.text,
            )
        )

        displayResponse(response.parsed)
        print("---")

        if response.parsed.is_turn_changed:
            print("Turn Changed")
            return GameState.USER_TURN
        if response.parsed.current_state.user_status.health <= 0:
            print("User is Dead...")
            return GameState.GAME_OVER
        if response.parsed.current_state.enemy_status.health <= 0:
            print("Enemy is Dead! You Win!")
            return GameState.GAME_OVER


def runGame():
    chatLog : List[ChatMessage] = [
        ChatMessage(
            role=GameRole.USER,
            content=game_manager_begin_message
        )
    ]

    print("Loading Game...")
    game_state = GameState.USER_TURN

    contents = convertChatLogToContentForUser(chatLog)

    response = client.models.generate_content(
        model=LLM_MODEL_ID, contents=contents, 
        config=types.GenerateContentConfig(
            system_instruction = game_manager_system_prompt,
            response_mime_type= 'application/json',
            response_schema = ManagerOutputSchema,
            temperature=0.1
        )
    )

    chatLog.append(
        ChatMessage(
            role=GameRole.SYSTEM,
            content=response.text,
        )
    )

    
    displayResponse(response.parsed)
    print("---")
    
    while game_state != GameState.GAME_OVER:
        match game_state:
            case GameState.USER_TURN:
                game_state = userTurn(chatLog)
            case GameState.ENEMY_TURN:
                game_state = enemyTurn(chatLog)
            case GameState.GAME_OVER:
                break
        
runGame()