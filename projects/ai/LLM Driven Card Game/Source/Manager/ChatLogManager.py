from google.genai import types
from pydantic import BaseModel, Field
from typing import List
import enum


class GameRole(enum.Enum):
    USER = 0
    SYSTEM = 1
    ENEMY = 2

class Content(BaseModel):
    text: str = Field(..., description="Content of the message")

class ChatMessage(BaseModel):
    role: GameRole = Field(..., description="Role of the message sender")
    content: str = Field(..., description="Content of the message")

def convertChatLogToContentForUser(chat_log: List[ChatMessage]) -> List[types.Content]:
    content = []
    user_role = "user"
    enemy_role = "user"
    system_role = "model"

    for chat in chat_log:
        match chat.role:
            case GameRole.USER:
                content.append(
                    types.Content(
                        role=user_role,
                        parts=[types.Part(
                            text=chat.content
                        )]
                    )
                )
            case GameRole.ENEMY:
                content.append(
                    types.Content(
                        role=enemy_role,
                        parts=[types.Part(
                            text=chat.content
                        )]
                    )
                )
            case GameRole.SYSTEM:
                content.append(
                    types.Content(
                        role=system_role,
                        parts=[types.Part(
                            text=chat.content
                        )]
                    )
                )
            
    return content

def convertChatLogToContentForEnemy(chat_log: List[ChatMessage]) -> List[types.Content]:
    content: List[types.Content] = []
    user_role = "user"
    enemy_role = "model"
    system_role = "user"

    for chat in chat_log:
        match chat.role:
            case GameRole.USER:
                modified_chat = f"""
<user>
User action: {chat.content}
</user>
"""
                content.append(
                    types.Content(
                        role=user_role,
                        parts=[types.Part(
                            text=modified_chat
                        )]
                    )
                )
            case GameRole.ENEMY:
                content.append(
                    types.Content(
                        role=enemy_role,
                        parts=[types.Part(
                            text=chat.content
                        )]
                    )
                )
            case GameRole.SYSTEM:
                modified_chat = f"""
<system>
Current Status: {chat.content}
</system>
"""
                content.append(
                    types.Content(
                        role=system_role,
                        parts=[types.Part(
                            text=modified_chat
                        )]
                    )
                )
            
    content_final:List[types.Content] = []
    index_current = 0
    index_next = 1
    content_text = content[index_current].parts[0].text
    while index_next < len(content):
        if content[index_current].role == content[index_next].role:
            content_text += content[index_next].parts[0].text + "\n"
            index_next += 1

            if index_next == len(content): # last element
                content_final.append(
                    types.Content(
                        role=content[index_current].role,
                        parts=[types.Part(
                            text=content_text
                        )]
                    )
                )
        else:
            content_final.append(
                types.Content(
                    role=content[index_current].role,
                    parts=[types.Part(
                        text=content_text
                    )]
                )
            )
            index_current = index_next
            index_next += 1

            content_text = content[index_current].parts[0].text

    # for c in content_final:
    #     print(c.role)
    #     print(c.parts[0].text)
    #     print("---")

    return content_final