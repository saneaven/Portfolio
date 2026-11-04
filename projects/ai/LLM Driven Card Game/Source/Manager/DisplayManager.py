from Manager.SchemaManager import ManagerOutputSchema, CurrentStateSchema, FieldCardSchema, HandCardSchema
import itertools
import os

def displayObjectsHorizontal(objects, separator=" | "):
    objects_str = [str(obj) for obj in objects]

    # 1. 각 텍스트 블록을 줄 단위 리스트로 분리
    split_lines = [block.splitlines() for block in objects_str]

    # 2. 각 열(텍스트 블록)의 최대 너비 계산 (정렬을 위해)
    #    - 각 블록 내에서 가장 긴 줄의 길이를 찾음
    #    - 만약 블록이 비어있으면 너비는 0
    col_widths = [max(len(line) for line in block) if block else 0 for block in split_lines]

    # 3. zip_longest를 사용하여 각 행을 묶음 (빈 곳은 ""로 채움)
    #    *split_lines는 리스트의 각 요소를 개별 인자로 풀어 전달합니다.
    zipped_lines = itertools.zip_longest(*split_lines, fillvalue="")
    output = ""
    # 4. 각 행을 순회하며 가로로 결합 및 출력
    for line_parts in zipped_lines:
        output_line = []
        # 각 열(텍스트 블록)에 해당하는 부분을 처리
        for i, part in enumerate(line_parts):
            # 해당 열의 너비에 맞게 왼쪽 정렬 (ljust)
            padded_part = part.ljust(col_widths[i])
            output_line.append(padded_part)
        # 구분자로 각 부분을 연결하여 최종 한 줄 생성 및 출력
        # print(separator.join(output_line) + "\n")
        output += separator.join(output_line) + "\n"

    return output


def displayCurrentState(current_state: CurrentStateSchema):

    outputStructure = f"""

-----------------------------------------------------------------------------------------------------------------------------
[Enemy] {"turn" if current_state.current_turn == "enemy" else ""}
{current_state.enemy_status.dialogue}

HP: {current_state.enemy_status.health} Mana: {current_state.enemy_status.current_mana}/{current_state.enemy_status.max_mana}
Deck: {len(current_state.enemy_status.deck_cards)} remain cards

Hand:
{displayObjectsHorizontal(current_state.enemy_status.hand_cards)}
Field:
{displayObjectsHorizontal(current_state.enemy_status.field_cards)}
-----------------------------------------------------------------------------------------------------------------------------
Field:
{displayObjectsHorizontal(current_state.user_status.field_cards)}
Hand:
{displayObjectsHorizontal(current_state.user_status.hand_cards)}

Deck: {len(current_state.user_status.deck_cards)} remain cards
HP: {current_state.user_status.health} Mana: {current_state.user_status.current_mana}/{current_state.user_status.max_mana}

{current_state.user_status.dialogue}
[User] {"turn" if current_state.current_turn == "user" else ""}
-----------------------------------------------------------------------------------------------------------------------------

"""
    print(outputStructure)

def clear_screen():
  # Windows
  if os.name == 'nt':
    _ = os.system('cls') # 'cls' 명령어 실행
  # POSIX
  else:
    _ = os.system('clear') # 'clear' 명령어 실행

def displayResponse(output: ManagerOutputSchema):
    if output.is_valid:
        clear_screen()
        displayCurrentState(output.current_state)
    else:
        print("Error Message:")
        print(output.error_message)