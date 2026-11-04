import os
from google import genai
from google.genai import types
from dotenv import load_dotenv
from PromptManager import GameManagerSystemPrompt, GameManagerBeginMessage, ManagerOutputSchema

client = genai.Client(api_key="GEMINI_API_KEY")
api_key = os.getenv("API_KEY")

client = genai.Client(api_key=api_key)

chat = client.chats.create(model="gemini-2.0-flash",
        config=types.GenerateContentConfig(
            system_instruction = GameManagerSystemPrompt,
            response_mime_type= 'application/json',
            response_schema = ManagerOutputSchema,
        ))
