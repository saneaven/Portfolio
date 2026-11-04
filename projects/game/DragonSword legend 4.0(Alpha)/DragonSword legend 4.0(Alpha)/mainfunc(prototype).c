#include<stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>


int atk = 36, def = 2, hp = 110, mhp = 0, spd = 140, gold = 10000, lv = 1; // 스탯 전역함수

int requireexp = 100, exp = 0;
// 경험치 부분은 그냥 그대로 가져가야됨 (requireexp=100, temp=100, lvl=1, exp=0)

int SP = 6; 
// <==(1렙 SP)  는 6으로 맞춰짐 밸런스 조정사항 있으면 알아서 바꾸셈

int achieve, succeed = -1;//미션 진행도, 미션 성공여부(-1 == 미션 없음, 0 == 미션 성공 못함, 1 == 미션 성공)
int set[7] = { 0 };//장착한 아이템(0==무기,1==투구,2==갑옷,3==신발,4==보석,5==회복(), 6==장착한 아이템)
int inven[7][12][2] = { 0 };//[a(0==무기,1==투구,2==갑옷,3==신발,4==보석,5==기타)][b(최대 저장가능한 아이템 종류)][c(아이템 갯수)]


void textColor(int _colorValue)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _colorValue);
	//#define FOREGROUND_BLUE      0x0001 // 파란색
	//#define FOREGROUND_GREEN     0x0002 // 초록색
	//#define FOREGROUND_RED       0x0004 // 빨간색
	//#define FOREGROUND_INTENSITY 0x0008 // 회색
	//#define BACKGROUND_BLUE      0x0010 // 배경 파란색
	//#define BACKGROUND_GREEN     0x0020 // 배경 초록색
	//#define BACKGROUND_RED       0x0040 // 베걍 빨간색
	//#define BACKGROUND_INTENSITY 0x0080 // 배경 회색
	//#define COMMON_LVB_LEADING_BYTE    0x0100 // Leading Byte of DBCS
	//#define COMMON_LVB_TRAILING_BYTE   0x0200 // Trailing Byte of DBCS
	//#define COMMON_LVB_GRID_HORIZONTAL 0x0400 // 가로 하얀줄
	//#define COMMON_LVB_GRID_LVERTICAL  0x0800 // DBCS: Grid attribute: left vertical.
	//#define COMMON_LVB_GRID_RVERTICAL  0x1000 // 세로 하얀 줄
	//#define COMMON_LVB_REVERSE_VIDEO   0x4000 // DBCS: Reverse fore/back ground attribute.
	//#define COMMON_LVB_UNDERSCORE      0x8000 // DBCS: Underscore.
	//#define COMMON_LVB_SBCSDBCS        0x0300 // SBCS or DBCS flag.
	/*------------색깔 조합 예시 ---------------*/

	//textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
	//textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색
	//textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
	//textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
	//textColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //보라색
	//textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
	//textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
	// 사용 예시 	textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}


typedef struct itm
{
	char name[20];
	int price;
	int atk;
	int spd;
	int def;
	int hp;
	int lv;
	int code;
}itm;

typedef struct monster
{
	char name[40];
	int hp;
	int atk;
	int spd;
	int lv;
	int code;
	int gold;
	int exp;
}monster;

typedef struct qst
{
	char name[40];
	int mission1;
	int mission2;
	int amount1;
	int amount2;
	int gold;
	int exp;
	int item;
}qst;//퀘스트 구조체

itm item(int codenum);
monster mon(int codenum);
qst quest(int codenum);


int wherex() // 커서의 x 좌표를 조사한다.
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.X;
}//밖에서 가져온 x좌표 알아내는 함수
int wherey() // 커서의 y좌표를 조사한다.
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.Y;
}//밖에서 가져온 y좌표 알아내는 함수

void gotoxy(int x, int y) //이동에 필요한 함수
{
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}


int Inven(int mode, int itemnum) //사용법: set(현재 장착한 아이템 배열), mode(1 == 인벤보기,장착 / 2 == 인벤에서 팔기 / 3 == 배틀시 기타 아이템 보기 / 4 == 아이템 입력하기), itemnum(모드 4일때 넣을 아이템 번호(아니면 0))
{
	int x, y, key;
	x = 3;
	y = 10;
	key = 0;

	int i;

	if (mode <= 3) // '1','2','3'모드 시작
	{
		while (1) // 인벤토리를 보기위한 반복문 시작 / 유저가 인벤토리에서 나올경우 반복문 종료
		{
			//임시로 아이템 코드 박아넣음
			/*
			Inven(0, 4, 1032);
			Inven(0, 4, 2011);
			Inven(0, 4, 3002);
			Inven(0, 4, 4012);
			Inven(0, 4, 5013);
			Inven(0, 4, 6003);
			Inven(0, 4, 6004);
			Inven(0, 4, 6005);
			Inven(0, 4, 6006);
			*/
			//임시끝


			int witem = 0;//변수 두개 선언
			int setitem = 0;
			int temp;
			if (mode == 3)
				witem = 5;

			while (1) // ** 부분 → ## 부분으로 바로 넘어가기 위한 반복문 시작
			{
				system("mode con:cols=105 lines=36");
				//전체UI
				system("cls");
				textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
				printf("┌──────────────────────  인  벤  토  리  ───────────────────┐\n");
				printf("│◆스탯────────────────────────────────────┐┌────────┐│\n");

				printf("││");
				textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
				printf(" 공격력: %-5d", atk);
				textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
				printf(" 체력: %-5d", hp);
				textColor(FOREGROUND_RED | FOREGROUND_GREEN); //밝은 노란색
				printf(" 방어력: %-3d", def);
				textColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //보라색
				printf(" 스피드: %-5d", spd);
				textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
				printf(" 소지골드: %8dG  ", gold);
				textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
				printf("  ││");
				textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
				printf("현재체력: %-5d", hp - mhp);
				textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
				printf(" ││\n");

				printf("│└──────────────────────────────────────┘└────────┘│\n");
				printf("│■──┐ ┌─────────┐  ┌───┐  ┌───┐  ┌──┐  ┌───┐  ┌───┐  ┌──┐ │\n");
				if (witem == 0)
				{
					printf("││무기│ │ 이            름 │  │");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
					printf(" 가격 "); 
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │"); 
					textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
					printf("공격력"); 
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
					printf("체력"); 
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │"); 
					textColor(FOREGROUND_RED | FOREGROUND_GREEN); //밝은 노란색
					printf("방어력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //보라색
					printf("스피드");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
					printf("개수");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│ │\n");
				}
					
				else if (witem == 1)
				{
					printf("││투구│ │ 이            름 │  │");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
					printf(" 가격 ");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
					printf("공격력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
					printf("체력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN); //밝은 노란색
					printf("방어력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //보라색
					printf("스피드");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
					printf("개수");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│ │\n");
				}	
				else if (witem == 2)
				{
					printf("││갑옷│ │ 이            름 │  │");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
					printf(" 가격 ");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
					printf("공격력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
					printf("체력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN); //밝은 노란색
					printf("방어력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //보라색
					printf("스피드");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
					printf("개수");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│ │\n");
				}	
				else if (witem == 3)
				{
					printf("││신발│ │ 이            름 │  │");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
					printf(" 가격 ");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
					printf("공격력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
					printf("체력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN); //밝은 노란색
					printf("방어력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //보라색
					printf("스피드");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
					printf("개수");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│ │\n");
				}
					
				else if (witem == 4)
				{
					printf("││보석│ │ 이            름 │  │");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
					printf(" 가격 ");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
					printf("공격력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
					printf("체력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN); //밝은 노란색
					printf("방어력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //보라색
					printf("스피드");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
					printf("개수");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│ │\n");
				}
					
				else if (witem == 5)
				{
					printf("││포션│ │ 이            름 │  │");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
					printf(" 가격 ");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
					printf("공격력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
					printf("체력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN); //밝은 노란색
					printf("방어력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //보라색
					printf("스피드");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
					printf("개수");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│ │\n");
				}
					
				else if (witem == 6)
				{
					printf("││퀘템│ │ 이            름 │  │");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
					printf(" 가격 ");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
					printf("공격력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
					printf("체력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN); //밝은 노란색
					printf("방어력");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //보라색
					printf("스피드");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│  │");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
					printf("개수");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("│ │\n");
				}
					
				printf("│└──■ └─────────┘  └───┘  └───┘  └──┘  └───┘  └───┘  └──┘ │\n");


				if (witem == 5)
					printf("│◆최근  사용──────────────────────────────────────┐          │\n");
				else if(witem==6)
					printf("│◆최근  얻음──────────────────────────────────────┐          │\n");
				else
					printf("│◆장착─────────────────────────────────────────┐          │\n");
				textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
				printf("││  0.    ");

				printf("%-18s", item(set[witem]).name);
				textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
				printf("      ");
				textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
				printf("%-6d", item(set[witem]).price);
				textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
				printf("       ");
				textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
				printf("%-4d", item(set[witem]).atk);
				textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
				printf("       ");
				textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
				printf("%-4d", item(set[witem]).hp);
				textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
				printf("      ");
				textColor(FOREGROUND_RED | FOREGROUND_GREEN); //밝은 노란색
				printf("%-5d", item(set[witem]).def);
				textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
				printf("       ");
				textColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //보라색
				printf("%-5d", item(set[witem]).spd);
				textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
				printf("   │          │\n");

				printf("│└───────────────────────────────────────────┘          │\n");


				for (i = 0; i < 12; i++)
				{
					printf("│  "); 
					printf("%3d.", i + 1);
					printf("    "); 
					printf("%-18s", item(inven[witem][i][0]).name);
					printf("      ");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
					printf("%-6d", item(inven[witem][i][0]).price);
					printf("       "); 
					textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
					printf("%-4d", item(inven[witem][i][0]).atk);
					printf("       ");
					textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
					printf("%-4d", item(inven[witem][i][0]).hp);
					printf("      ");
					textColor(FOREGROUND_RED | FOREGROUND_GREEN); //밝은 노란색
					printf("%-5d", item(inven[witem][i][0]).def);
					printf("       "); 
					textColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //보라색
					printf("%-5d", item(inven[witem][i][0]).spd);
					printf("        ");
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
					printf("%-4d", inven[witem][i][1]);
					textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
					printf("   │\n");
				}
					

				printf("└──────────────────────────────────────────────────┘\n");
				questfunc(2, 0);

				printf("┌──────────────────────────────────────────────────┐\n");
				printf("│┌── ↖ ──┐  ┌─ ← ─┐ ┌─ → ─┐   ┌─ 」 ─┐  ※인벤토리 순서 ───────────┐│\n");
				if (mode == 1)
					printf("││esc. 나가기 │  │ 이  전 │ │ 다  음 │   │ 장  착 │  │ 무기 투구 갑옷 신발 보석 기타 퀘템 ││\n");
				else if (mode == 2)
					printf("││esc. 나가기 │  │ 이  전 │ │ 다  음 │   │ 판  매 │  │ 무기 투구 갑옷 신발 보석 기타 퀘템 ││\n");
				else if (mode == 3)
					printf("││esc. 나가기 │  │ 이  전 │ │ 다  음 │   │ 사  용 │  │ 무기 투구 갑옷 신발 보석 기타 퀘템 ││\n");
				printf("│└──────┘  └────┘ └────┘   └────┘  └──────────────────┘│\n");
				printf("└──────────────────────────────────────────────────┘\n");
				
				//키보드 이동
				gotoxy(x + 1, y);
				printf("\b▶");
				//scanf("%d", &setitem);
				//witem = witem - 1;
				//do { setitem = _getch(); } while (setitem == 224); // 키입력 받아옴 224인경우 반복해서 224값을 버림 
				// 이부분 문제 있음 : 각 컴퓨터마다 224값이 아닌 다른 값인경우가 있음 -32나 0인경우등등 다양함
				// 이후 이부분 주석 반복
				while (1)
				{
					key = _getch();
					if (key == 75) //←를 누른경우 
					{
						if (mode == 3)
							break;
						y = 10;
						witem--;
						if (witem < 0)
						{
							witem = 6;

						}
						break;
					}
					else if (key == 27) // 키 esc를 누른경우
					{

						return 0; // ** 부분 → ## 부분으로 바로 넘어가기 위한 반복문 종료

								  // 이후 설명 반복
					}

					else if (key == 77)//→를 누른경우
					{
						if (mode == 3)
							break;
						y = 10;
						witem++;
						if (witem > 6)
						{
							witem = 0;
						}
						break;
					}

					else if (key == 72)//↑를 누른경우
					{
						y -= 1;
						if (y < 10)
							y = 21;
						printf("\b\b  ");
						gotoxy(x, y);
						printf("▶");

					}

					else if (key == 80)//↓를 누른경우
					{
						y += 1;
						if (y > 21)
							y = 10;
						printf("\b\b  ");
						gotoxy(x, y);
						printf("▶");
					}

					else if (key == 32 || key == 13)//엔터를 누른경우
					{
						setitem = y - 10;
						if (mode == 1)
						{
							if (witem == 6)
							{
								continue;
							}
							if (witem == 5)	//종류가 기타인경우
							{

								if (mhp == 0)//입은 대미지가 0일경우
								{
									system("cls");
									printf("이미 최고 체력입니다!");
									Sleep(1000);
									break;
								}

								temp = mhp;

								set[witem] = inven[witem][setitem][0];  //사용
								mhp -= item(set[witem]).hp;  //체력회복

								if (mhp < 0)
									mhp = 0;

								inven[witem][setitem][1]--; //갯수 줄이기

								if (inven[witem][setitem][1] <= 0)//갯수가 0이하일경우 그아이템은 사라진다.
								{
									inven[witem][setitem][0] = 0;
									inven[witem][setitem][1] = 0;
								}

								system("cls");
								printf("용사는 %d만큼 회복하였다", temp - mhp);
								Sleep(1000);
								break;
							}

							//제거를 위한 아이템스텟을 주인공스텟에서 빼기
							atk -= item(set[witem]).atk;
							hp -= item(set[witem]).hp;
							def -= item(set[witem]).def;
							spd -= item(set[witem]).spd;


							if (inven[witem][setitem][0] == 0)  //만약 바꾸는 곳이 비어있을 경우 동일한 아이템을 인벤에 추가한뒤 장착한 아이템은 없앤다
							{
								Inven(4, set[witem]);
								set[witem] = 0;
							}

							else  //만약 바꾸는 것이 비어있지 않을경우 동일한 아이템을 인벤에 추가한뒤 장착하려는 아이템을 장착한 아이템란에 복사하고 장착한 아이템의 갯수를 하나 줄이는 데 만약 아이템 갯수가 0개 이하일경우 그 아이템은 사라진다
							{
								Inven(4, set[witem]);
								set[witem] = inven[witem][setitem][0];
								inven[witem][setitem][1]--;
								if (inven[witem][setitem][1] <= 0)
								{
									inven[witem][setitem][0] = 0;
									inven[witem][setitem][1] = 0;
								}
							}
							//추가한 아이템스텟을 주인공 스텟에 더하기
							atk += item(set[witem]).atk;
							hp += item(set[witem]).hp;
							def += item(set[witem]).def;
							spd += item(set[witem]).spd;

							break;

							system("cls");
						}
						else if (mode == 2)//판매할때, 이정도는 이해할것이라 믿는다
						{
							if (witem == 6)
							{
								system("cls");
								printf("이 아이템은 팔지 못합니다!");
								continue;
							}
							gold += inven[witem][setitem][0];
							inven[witem][setitem][0] = 0;

							break;

							system("cls");
						}
						else if (mode == 3)//전투시 아이템을 사용할때
						{
							//mode1의 기타 사용할때랑 같다.
							if (mhp == 0)
							{
								system("cls");
								printf("이미 최고 체력입니다!");
								Sleep(1000);
								break;
							}

							temp = mhp;
							set[witem] = inven[witem][setitem][0];
							mhp -= item(set[witem]).hp;
							if (mhp < 0)
								mhp = 0;
							inven[witem][setitem][1]--;

							if (inven[witem][setitem][1] <= 0)
							{
								inven[witem][setitem][0] = 0;
								inven[witem][setitem][1] = 0;
							}

							system("cls");
							printf("용사는 %d만큼 회복하였다", temp - mhp);
							Sleep(1000);
							return inven[witem][setitem][0];
						}

					}
				}
			}
		}
	}

	else if (mode == 4) // 4 모드 시작
	{

		if (itemnum <= 0)		//아이템번호가 0보다 작을경우 그냥 종료
			return 0;


		int witem = (itemnum / 1000) - 1;     //종류 판별을 위한 식



		for (i = 0; inven[witem][i][0] != 0; i++)//비어있는 공간 탐색을 위해 숫자 증가
		{
			if (inven[witem][i][0] == itemnum)//만약 추가하려는 아이템과 같은 코드의 아이템이 있다면 그곳에 있는 아이템의 갯수를 늘리고 종료
			{
				inven[witem][i][1]++;
				return 0;
			}
		}

		//아닐경우 비어있는 공간에 아이템을 추가하고 갯수를 추가한뒤 종료
		inven[witem][i][0] = itemnum;
		inven[witem][i][1]++;


		if (witem == 6)
		{
			set[witem] = itemnum;
		}
		return 0;
	}
}

int questfunc(int mode, int killmon)//사용법 : mode(1==미션 인트로와 엔딩, 2 == 퀘스트 확인, 3 == 특정 코드가 입력됬을시 진행도 추가), killmon(mode==3일시 코드입력)
{

	static int nowget1;
	static int nowget2;

	printf("\n\n");

	if (mode == 1)
	{
		//여기에 미션 인트로 노가다
		if (succeed == -1)
		{

			if (achieve == 0)
			{
				printf("안녕하신가 용사여\n");
				Sleep(1500);
				printf("갑작스럽게 이렇게 되서 많이 놀랐을 걸세\n");
				Sleep(1500);
				printf("하지만 자네가 용사라는 사실은 사실이고 지금 부활한 흑룡을 없애야 된다는 것도 사실일세 \n");
				Sleep(1500);
				printf("...\n");
				Sleep(1500);
				printf("아직 정신이 완전히 안든 것 같으니\n");
				Sleep(1500);
				printf("간단하게 마을을 돌아보며 가장싼 무기와 포션을 사오는게 어떤가?");
			}

			else if (achieve == 1)
			{
				printf("안녕하세요 용사님 \n");
				Sleep(1500);
				printf("도움이 필요한 사람을 찾고 있다는 소식을 들었는데 \n");
				Sleep(1500);
				printf("혹시 괜찮으시다면 저의 부탁을 들어주실수 있을른지요...\n");
				Sleep(1500);
				printf("사실 요즘 들개가 늘어나서 창고에있는 식량을 훔쳐먹고 아이들을 해치는 일이 늘어나고 있습니다...\n");
				Sleep(1500);
				printf("이들을 어떻게 처리해주실수는 없을까요?");
			}

			else if (achieve == 2)
			{
				printf("하지만 아직 들개들의 횡포는 줄어들기미가 안보이네요\n");
				Sleep(1500);
				printf("마치 대장이 있는것 처럼 조직적으로 돌아 다니고 있어요...");
			}

			else if (achieve == 3)
			{
				printf("흠.. 이제야 정신을 차린것 같군...\n");
				Sleep(1500);
				printf("이제 약초마을로 가서 약초마을의 촌장인 아니카를 찾아가게. \n");
				Sleep(1500);
				printf("그곳으로 가면 필요한것을 얻을수 있을걸세");
			}

			else if (achieve == 4)
			{
				printf("사실 우리도 음식을 대접해 드리고는 싶지만 우리상황이 그럴때가 아니라...\n");
				Sleep(1500);
				printf("사실 최근 마을 뒷산에 몬스터들이 늘어나고 있어서 마을사람들이 불안해하고 있어요!\n");
				Sleep(1500);
				printf("혹시 용사님께서 그 몬스터들을 처치하실수 있으신가요?");
			}

			else if (achieve == 5)
			{
				printf("저희마을에는 싸울 수 있는 전사가 없어요.\n");
				Sleep(1500);
				printf("제발 우리를 몬스터로부터 구해주세요");
			}

			else if (achieve == 6)
			{
				Sleep(1500);
				printf("하지만 이것도 완벽한 해결책은 아닐것 같아요...\n");
				Sleep(1500);
				printf("마을에사는 (이름 아직 정하지 않음)가 이것에대한 자세한 사실을 아는것 같더군요...");
			}

			else if(achieve==7)
			{ 
				printf("만약 몬스터가 몬스터를 조종하고 있다면 믿으시겠습니까? \n");
				Sleep(1500);
				printf("네 저도 처음에는 두눈으로 보고도 믿지 못했습니다! \n");
				Sleep(1500);
				printf("하지만 사실이에요!!\n");
				Sleep(1500);
				printf("몬스터들은 그 조종하는 녀석을 사스콰치라 부르더군요!\n");
				Sleep(1500);
				printf("아마 그녀석을 없앤다면 이 소란도 끝낼수 있을꺼에요!");
			}

			else if (achieve == 8)
			{
				printf("혹시 가시는길에 홀리시티를 들렸다 가실수는 없을까요?\n");
				Sleep(1500);
				printf("그곳도 어떤 사건 때문에 꽤나 시끄럽다고 들었거든요...");
			}

			else if (achieve == 9)
			{
				printf("우리도시는 매우 평화로운 곳이였어요...\n");
				Sleep(1500);
				printf("하지만 어느날 언데드해적단이 나타나서\n");
				Sleep(1500);
				printf("우리도시의 물자를 약탈하고 사람들 멏명을 끌고가 버리고 말았던거에요!\n");
				Sleep(1500);
				printf("그러고나선 그들이 말하길 후에 다시 나타나서 물자를 다시 가져갈테니 준비해놓으라 하더라는거 아니겠어요...\n");
				Sleep(1500);
				printf("하지만 피해가 심각했던 우리는 그정도의 물자를 준비할 수도 없었고 준비하는 것이 가능하더라도 적에겐 굴복하진 않을것이에요!\n");
				Sleep(1500);
				printf("...그렇게 언데드 해적단이 다시 오고있다는 소식이 들리고있어요.\n");
				Sleep(1500);
				printf("용사님! 제발 우리들과 함께 언데드해적단을 막아주세요!\n");
				Sleep(1500);
				printf("...\n");
				Sleep(1500);
				printf("감사해요\n");
				Sleep(1500);
				printf("이제 곧 쳐들어올 언데드들에 대비하여 준비를 하신뒤에 저에게 말해주세요!");
			}

			else if (achieve == 10)
			{
				printf("...!\ 이런! 생각보다 언데드들이 빨리 찾아왔어요!\n");
				Sleep(1500);
				printf("어서 그들의 배로 가서 그들을 처치하고 와주세요!");
			}
			else if (achieve == 11)
			{
				printf("부함장의 위치가 파악되었다고 합니다!\n");
				Sleep(1500);
				printf("시간지체 하지말고 어서 가시죠!");
			}

			else if (achieve == 12)
			{
				printf("전에 느껴지지 않는 굉장한 살기가 느껴진다...?!\n");
				Sleep(1500);
				printf("조심하세요! 흑기사입니다!");
			}

			else if (achieve == 13)
			{
				printf("지금 왕국으로 가는길이라 들었어요\n");
				Sleep(1500);
				printf("가는길에 용사의 고향라는 전설이 있는 마을이 있는데\n");
				Sleep(1500);
				printf("가보시는게 어떨까요?\n");
				Sleep(1500);
				printf("무엇인가 흑룡을 처치할 단서를 얻을수 있잖아요?");
			}

			else if (achieve == 14)
			{
				printf("...자네 이곳에 사람들이 별로 없다는 생각이 않드나?\n");
				Sleep(1500);
				printf("사실 흑룡이 부활한뒤로 이곳에 몬스터들이 많아지면서 찾아오는 발길이 끊기기 시작했네.\n");
				Sleep(1500);
				printf("주로 상업으로 먹고살던 우리들에겐 청천벽력같은 일이였지.\n");
				Sleep(1500);
				printf("그러면서 마을사람들은 먹고살일을 찾기위해 이마을을떠나기 시작했네...\n");
				Sleep(1500);
				printf("혹시 자네가 이마을의 물건을 사면서 경제를 그나마 향상 시켜줄 수는 있나?");
			}

			else if (achieve == 15)
			{
				printf("혹시 자네 한가지 부탁만 더 들어줄 수는 있나?\n");
				Sleep(1500);
				printf("맞네 이근처에 도사리면서 우리들을 굶어죽게 만드는 몬스터들을 없애주게");
			}

			else if (achieve == 16)
			{
				printf("미안하지만 몬스터를 더 잡아줄 수 있겠는가?\n");
				Sleep(1500);
				printf("...\n");
				Sleep(1500);
				printf("고맙네");
			}

			else if (achieve == 17)
			{
				printf("진심으로 미안하네\n");
				Sleep(1500);
				printf("몬스터를 좀 더 잡아주시게");
			}

			else if (achieve == 18)
			{
				printf("좋은소식은\n");
				Sleep(1500);
				printf("이제 몬스터들을 확실하게 없앨 수 있는 방법을 찾은 것이고\n");
				Sleep(1500);
				printf("나쁜소식은\ 자네가 한번더 사냥을 해야한다는 것이지\n");
				Sleep(1500);
				printf("... \n");
				Sleep(1500);
				printf("그래 이 몬스터들을 조종하고 있는 우두머리를 찾았다네\n");
				Sleep(1500);
				printf("이름은 라이칸스로프라 불리는 모양인것 같은데\n");
				Sleep(1500);
				printf("꽤나 악질인것 같으니 조심하게");
			}

			else if (achieve == 19)
			{
				printf("최근 흑룡이 부활하고 나서 레지스탕스가 생겼다는것 같은데\n");
				Sleep(1500);
				printf("최근 꽤 어려움을 겪고 잇다는 소문이 있네\n");
				Sleep(1500);
				printf("서로 목표가 같은것 같은데 \n");
				Sleep(1500);
				printf("서로 도와야 되지 않겠나?");
			}

			Sleep(2000);
			succeed = 0;
		}


		//여기에 미션 크로즈멘트 노가다
		else if (succeed == 1)
		{
			if (achieve == 0)
			{
				printf("가장싼 무기를 사오라는 말은 했어도 부셔져서 못쓰는 무기를 사오란 소리는 않했다만...");
				Sleep(1500);
				printf("\n ... ");
				Sleep(1500);
				printf("\n아직 정신이 덜깬것 같구만");
				Sleep(1500);
				printf("\n일단 그검은 쓰진말게.");
				Sleep(1500);
				printf("\n그래! 몸도 풀겸 마을사람들에게 간단한 부탁을 받고 해결해 주는게 어떤가?");
			}

			else if (achieve == 1)
			{
				printf("감사합니다...");
			}

			else if (achieve == 2)
			{
				printf("이제 들개의 횡포가 많이 줄어든 느낌이 드네요!\n");
				Sleep(1500);
				printf("감사합니다!");
			}

			else if (achieve == 3)
			{
				printf("어서오세요 용사님!\n");
				Sleep(1500);
				printf("소식은 대충 들었어요!");
			}

			else if (achieve == 4)
			{
				printf("음... \n");
				Sleep(1500);
				printf("오히려 몬스터가 모여들고 있는건 저뿐인가요?");
			}

			else if (achieve == 5)
			{
				printf("감사합니다...\n");
				Sleep(1500);
				printf("이것으로 한시름 놨네요. \n");

			}
			else if (achieve == 6)
			{
				printf("용사님이 진짜 오셨어!!!\n");
				Sleep(1500);
				printf("용사님! 일단 제말을 들어보세요!\n");
			}
			else if (achieve == 7)
			{
				printf("신기하네요!\n");
				Sleep(1500);
				printf("몬스터가 모두 사라졌어요!\n");
				Sleep(1500);
				printf("어떻게 하신건가요?\n");
				Sleep(1500);
				printf("이건 변변치 않지만 저희의 작은 성의입니다!");
			}

			else if (achieve == 8)
			{
				printf("오! 드디어 와주셨군요 용사여!\n");
				Sleep(1500);
				printf("우리는 당신이 오는 날만을 기다리며 신께 기도하고 있었소");
			}

			else if (achieve == 9)
			{
				printf("준비가 되신건가요?");
			}

			else if (achieve == 10)
			{
				printf("이걸로 잔챙이들은 왠만큼 해치운것 같군요...");
			}

			else if (achieve == 11)
			{
				printf("....꽤 힘든 전투였습니다.");
			}

			else if (achieve == 12)
			{
				printf("감사해요!\n");
				Sleep(1500);
				printf("덕분에 우리 도시가 평화를 되찾은 것 같아요!\n");
				Sleep(1500);
				printf("변변치 않지만 이건 작은 보상이에요.\n");
				Sleep(1500);
				printf("신의 가호가 함께하시길 빌게요!");
			}

			else if (achieve == 13)
			{
				printf("자네인가?\n");
				Sleep(1500);
				printf("그 분의 후손이라는 사람이?\n");
				Sleep(1500);
				printf("오오 확실히 자화상과 닮앗구만\n");
				Sleep(1500);
				printf("그래 어서오게나 이곳이 바로 그 용사님의 고향일세.");
			}

			else if (achieve == 14)
			{
				printf("고맙네\n");
				Sleep(1500);
				printf("조그만일을 한것 같지만 우리마을에게는 큰도움이 되었네");
			}

			else if (achieve == 15)
			{
				printf("아직 몬스터가 줄어들 기미가 보이지 않는구먼");
			}

			else if (achieve == 16)
			{
				printf("흠... 이정도면 충분할줄 알았더만...");
			}

			else if (achieve == 17)
			{
				printf("용사여 좋은 소식과 나쁜소식이 있는데 무엇을 먼저 들을텐가?");
			}

			else if (achieve == 18)
			{
				printf("진심으로 고맙네\n");
				Sleep(1500);
				printf("이제 다끝난것 같구만\n");
				Sleep(1500);
				printf("이건 전설에서 용사가 혹시 세상이 다시 혼란해져서 새로운용사가 찾아온다면 전해달라고 한 보석일세\n");
				Sleep(1500);
				printf("여기 있네\n");
				Sleep(1500);
				printf("소중히 써주게나.");
				Inven(4, 7002);
			}

			else if (achieve == 19)
			{
				printf("오 신이시어 감사합니다.\n");
				Sleep(1500);
				printf("용사님 마침 좋은때에 찾아와 주셨습니다.");
			}



			Sleep(2000);
			if (quest(achieve).gold != 0)
			{
				gold += quest(achieve).gold;
				printf("\n\n\n\n\n보상으로 %d골드를 받았다!", quest(achieve).gold);
			}
	
			Sleep(500);
			if (quest(achieve).exp != 0)
			{
				printf("\n\n용사는 %d exp 를 얻었다!", quest(achieve).exp);
				Sleep(1000);
				expadd(quest(achieve).exp);
			}

			if (quest(achieve).item != 0)
			{
				Inven(4, quest(achieve).item);
				printf("\n\n용사는 %s를 받았다!", item(quest(achieve).item).name);
				Sleep(1000);
			}
			
			succeed = -1;
			nowget1 = 0;
			nowget2 = 0;
			achieve++;
		}
	}

	if (mode == 2)
	{
		//여기가 인벤에서 연결되는 퀘스트 확인란(UI 개선 필요)

		if (succeed == -1)
		{
			printf("현재 퀘스트 : (없음) \n", quest(achieve).name);
		}

		else
		{
			printf("현재 퀘스트 : %s \n", quest(achieve).name);

			if (quest(achieve).mission1 != 0)
			{
				if (quest(achieve).mission1 >= 1000)
				{
					printf("얻어야 할 아이템 : %s | 얻어야 할 아이템의 수: %d    \n", item(quest(achieve).mission1).name, quest(achieve).amount1);
					printf("현재 얻은 아이템의 수 : %d  \n", nowget1);
				}
				else if (quest(achieve).mission1 < 1000)
				{
					printf("처리해야할 적 : %s | 처리해야 할 적의 수: %d    \n", mon(quest(achieve).mission1).name, quest(achieve).amount1);
					printf("현재 처리한 적의 수 : %d  \n", nowget1);
				}
			}

			if (quest(achieve).mission2 != 0)
			{
				if (quest(achieve).mission2 >= 1000)
				{
					printf("얻어야 할 아이템 : %s | 얻어야 할 아이템의 수: %d    \n", item(quest(achieve).mission2).name, quest(achieve).amount2);
					printf("현재 얻은 아이템의 수 : %d  \n", nowget2);
				}
				else if (quest(achieve).mission2 < 1000)
				{
					printf("처리해야할 적 : %s | 처리해야 할 적의 수: %d    \n", mon(quest(achieve).mission2).name, quest(achieve).amount2);
					printf("현재 처리한 적의 수 : %d  \n", nowget2);
				}
			}

			if (succeed == 1)
				printf("미션완료! \n");
		}
	}

	if (mode == 3)
	{
		//여기가 배틀에서 연결되는 죽인 몬스터 추가란

		if (succeed != -1)
		{
			if (quest(achieve).mission1 == killmon)
			{
				nowget1++;
			}
			if (quest(achieve).mission2 == killmon)
			{
				nowget2++;
			}
			if (nowget1 >= quest(achieve).amount1 && nowget2 >= quest(achieve).amount2)//죽인 몬스터가 필요한 몬스터보다 같거나 클시 성공으로 변환
				succeed = 1;
		}
	}

	return 0;
}


int battle(int esel)
{
	int eatk, ehp, espd;//적 스텟
	int sel = 0;  //고른거
	int miss = -1; //미스(-1:평소, 0:실패, 1:성공)
	int result; //결과
	int temp;  //임시변수
	int battlehp; //현재체력
	int spdnow = 0;
	int espdnow = 0;
	char x = 34;
	char y = 8;
	char ey = 12;

	ehp = mon(esel).hp;
	eatk = mon(esel).atk;
	espd = mon(esel).spd;	//여기까지 몬스터와 용사의 스텟정리

	battlehp = hp - mhp;

	system("mode con:cols=80 lines=25");

	while (1)
	{


		system("cls");
		printf("┌────────────────┐\n");
		printf("│플레이어   의 남은 ");
		textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
		printf(" 체력: %-5d", battlehp);
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
		printf(" │\n");
		printf("│플레이어   의");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
		printf(" 공격력: %-5d", atk);
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
		printf("     │\n");

		printf("│[%-8s] 의 남은 ", mon(esel).name);
		textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
		printf(" 체력: %-5d", ehp);
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
		printf(" │\n");
		printf("│[%-8s] 의 ", mon(esel).name);
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
		printf("공격력: %-5d", eatk);
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
		printf("     │\n");

		printf("└────────────────┘\n");
		printf("플레이어 \n");
	    printf("          ┌──────────┐\n");
		printf("                                  \n");
		printf("          └──────────┘\n");
		printf("[%-8s]\n", mon(esel).name);
		printf("          ┌──────────┐\n");
		printf("                                  \n");
		printf("          └──────────┘\n");



		while (spdnow < 10000 && espdnow < 10000)	//여기서부터 게이지그래픽 노가다
		{
			spdnow += spd;
			espdnow += espd;

			if (spdnow >= 10000)
			{
				gotoxy(0, y);
				printf("완료!!!!! │●●●●●●●●●●│\n");
			}
			else if (spdnow > 9000)
			{
				gotoxy(0, y);
				printf("준비중... │●●●●●●●●●  │\n");
			}
			else if (spdnow > 8000)
			{
				gotoxy(0, y);
				printf("준비중... │●●●●●●●●    │\n");
			}
			else if (spdnow > 7000)
			{
				gotoxy(0, y);
				printf("준비중... │●●●●●●●      │\n");
			}
			else if (spdnow > 6000)
			{
				gotoxy(0, y);
				printf("준비중... │●●●●●●        │\n");
			}
			else if (spdnow > 5000)
			{
				gotoxy(0, y);
				printf("준비중... │●●●●●          │\n");
			}
			else if (spdnow > 4000)
			{
				gotoxy(0, y);
				printf("준비중... │●●●●            │\n");
			}
			else if (spdnow > 3000)
			{
				gotoxy(0, y);
				printf("준비중... │●●●              │\n");
			}
			else if (spdnow > 2000)
			{
				gotoxy(0, y);
				printf("준비중... │●●                │\n");
			}
			else if (spdnow > 1000)
			{
				gotoxy(0, y);
				printf("준비중... │●                  │\n");
			}
			else if (spdnow >= 0)
			{
				gotoxy(0, y);
				printf("체력고갈! │                    │\n");
			}
			else if (spdnow < 0)
			{
				gotoxy(0, y);
				printf("정신혼미! │                    │\n");
			}


			if (espdnow >= 10000)
			{
				gotoxy(0, ey);
				printf("완료!!!!! │●●●●●●●●●●│\n");
			}
			else if (espdnow > 9000)
			{
				gotoxy(0, ey);
				printf("준비중... │●●●●●●●●●  │\n");
			}
			else if (espdnow > 8000)
			{
				gotoxy(0, ey);
				printf("준비중... │●●●●●●●●    │\n");
			}
			else if (espdnow > 7000)
			{
				gotoxy(0, ey);
				printf("준비중... │●●●●●●●      │\n");
			}
			else if (espdnow > 6000)
			{
				gotoxy(0, ey);
				printf("준비중... │●●●●●●        │\n");
			}
			else if (espdnow > 5000)
			{
				gotoxy(0, ey);
				printf("준비중... │●●●●●          │\n");
			}
			else if (espdnow > 4000)
			{
				gotoxy(0, ey);
				printf("준비중... │●●●●            │\n");
			}
			else if (espdnow > 3000)
			{
				gotoxy(0, ey);
				printf("준비중... │●●●              │\n");
			}
			else if (espdnow > 2000)
			{
				gotoxy(0, ey);
				printf("준비중... │●●                │\n");
			}
			else if (espdnow > 1000)
			{
				gotoxy(0, ey);
				printf("준비중... │●                  │\n");
			}
			else if (espdnow >= 0)
			{
				gotoxy(0, ey);
				printf("채력고갈! │                    │\n");
			}
			else if (spdnow < 0)
			{
				gotoxy(0, ey);
				printf("정신혼미! │                    │\n");
			}

			Sleep(35);
		}

		gotoxy(0, ey + 4);

		if (spdnow >= 10000)
		{
			//system("mode con:cols=65 lines=20");
			sel = 0;
			//여기서부터 고르기
			printf("┌───────────────────────────┐\n");
			printf("│     당신의 차례입니다. 무슨 행동을 하시겠습니까?     │\n");
			printf("│          ");
			textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
			printf("1. 공격");
			printf("              ");
			textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색
			printf("  3.회피");
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
			printf("               │\n");

			printf("│         ");
			textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 초록색
			printf(" 2. 아이템");
			printf("              ");
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
			printf("4.도망가기 ");
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
			printf("          │\n");
			printf("└───────────────────────────┘\n");
			printf(" ▶ 행동 : ");
			scanf("%d", &sel);


			if (sel == 1)//공격시
			{
				system("cls");
				temp = ehp;
				Sleep(500);
				printf("\n\n\n\n용사의");
				textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
				printf(" 공격");
				textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
				printf(" !!!!!!\n", atk);
				ehp -= atk;  //이곳에 공격식 써내리면 됨
				if (ehp < 0)
					ehp = 0;
				Sleep(500);
				printf("\n\n 적에게 ");
				textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
				printf(" %d", temp - ehp);
				textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
				printf(" 만큼의 ");
				textColor(FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 빨간색
				printf(" 데미지");
				textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
				printf(" 를 입혔다!\n\n\n\n");
				Sleep(1500);
		
			}

			if (sel == 2)//포션 사용시
			{

				temp = Inven(3, 0);
				if (item(temp).code == 0)
				{
					system("cls");
					printf("아무것도 사용하지 않으셨습니다\n 이전으로 되돌아갑니다\n");
					Sleep(1500);
				
					continue;
				}


			}

			else if (sel == 3)//회피시
			{
				system("cls");
				printf("용사는 공격을 피할 준비를 하고있다!\n");
				srand((int)time(NULL));  //일정확률로 피하게함
				miss = rand() % 4;
				Sleep(1500);
		
			}

			else if (sel == 4)//도망가기시
			{
				system("cls");
				printf("용사는 도망치려 한다!");

				Sleep(500);

				srand((int)time(NULL)); //일정확률로 도망치게 함
				temp = rand() % 2;
				if (temp == 1)
				{
					printf("용사는 무사히 도망쳤다!");
					result = 2;
					Sleep(1500);
					system("cls");

					return result;
				}

				printf("그러나 몬스터에게 발목을 잡혔다!");
				Sleep(1500);


			}

			else if (sel > 4)
			{

				printf("옳바른 숫자 입력 부탁드립니다 \n");
				Sleep(500);
				continue;

			}

			spdnow = 0;
		}


		if (espdnow >= 10000)
		{
			temp = battlehp;
			system("cls");

			printf("\n\n");
			printf("적이 플레이어를 공격하였다! \n\n\n");
			Sleep(500);

			if (miss != -1)
			{
				printf("용사는 회피를 시도하였다!\n\n");
				Sleep(1000);

				if (miss >= 2)
				{
					printf("그러나 실패하였다...\n\n\n");
					Sleep(500);
					miss = -1;
				}

				else if (miss <= 1)
				{
					printf("용사는 공격을 회피하였다!\n");
					Sleep(1500);

					if (miss == 1)
					{
						printf("\n\n용사는 %s의 빈틈을 노렸다!\n", mon(esel).name);
						Sleep(1000);

						temp = ehp;
						ehp -= atk / 2;

						printf("\n\n 적은 %d만큼 대미지를 입었다!", temp - ehp);
						Sleep(1500);
					}
		
					miss = -1;
					system("cls");
					espdnow = 0;
					continue;
				}
			}

			Sleep(500);
			mhp += (100*eatk) / (100+def); //이곳에 공격식

			printf("용사는 %d만큼의 데미지를 입었다!", temp - (hp - mhp));

			Sleep(1500);


			espdnow = 0;
			system("cls");
		}

		battlehp = hp - mhp;

		if (battlehp <= 0)
		{
			system("cls");
			battlehp = 0;
			mhp = hp;
			system("cls");
			printf("플레이어가 쓰러졌습니다!\n");
			Sleep(300);


			temp = gold;
			gold -= mon(esel).gold / 2;

			if (gold < 0)
				gold = 0;

			printf("용사는 몬스터에게 %d골드를 내어 목숨만은 살려내었다!", temp - gold);
			result = 0;

			Sleep(1500);
		

			system("cls");
			mhp = 0;
			return result;
		}

		if (ehp <= 0)
		{
			ehp = 0;
			system("cls");
			printf("%s를 쓰려트렸다!\n", mon(esel).name);
			Sleep(300);

			printf("용사는 전리품으로 %d골드를 얻었다!", mon(esel).gold);
			gold += mon(esel).gold;

			printf("용사는 %d의 경험치를 얻었다!", mon(esel).exp);
			Sleep(1500);

			expadd(mon(esel).exp);
			questfunc(3, esel);
			result = 1;

			Sleep(1500);
		

			system("cls");
			return result;
		}

	}

	return result;
}

int shop(int place, int mode)//사용법 : place(현제장소),mode(상점종류(1 == 무기, 2 == 방어구, 3 == 보석, 4 == 기타))
{
	int shopitem[40] = { 0 };
	int minlv, maxlv;//최소 레벨 제한 , 최대 레벨 제한
	int select;
	int temp1, temp2;
	int accept;
	int a = 0, b = 0, c = 0;
	int i = 0, j = 0;
	int x = 12;
	int y = 0;


	//여기에 최저 레벨 제한과 최고 레벨 제한 넣어야함
	minlv = 1;
	maxlv = 9;

	if (place == 1)
	{
		minlv = 1;
		maxlv = 9;
	}
	else if (place == 2)
	{
		minlv = 10;
		maxlv = 19;
	}
	else if (place == 3)
	{
		minlv = 20;
		maxlv = 29;
	}
	else if (place == 4)
	{
		minlv = 30;
		maxlv = 39;
	}
	else if (place == 5)
	{
		minlv = 40;
		maxlv = 49;
	}
	else if (place == 6)
	{
		minlv = 50;
		maxlv = 59;
	}
	else if (place == 7)
	{
		minlv = 60;
		maxlv = 69;
	}
	else if (place == 9)
	{
		minlv = 70;
		maxlv = 79;
	}
	else if (place == 10)
	{
		minlv = 80;
		maxlv = 89;
	}
	else if (place == 11)
	{
		minlv = 90;
		maxlv = 99;
	}


	if (mode == 1) //무기
	{
		for (a = 0; item(1000 + a).lv < minlv; a++); //무기 //최소 레벨 제한 까지 아이템 제외


		for (a; item(1000 + a).lv < maxlv; a++) //무기 //최대 레벨 제한 까지 아이템 추가
		{
			shopitem[i] = 1000 + a;
			i++;
		}

		if (achieve == 0 && succeed == 0)
			shopitem[0] = 7001;
	}

	//이하 동문

	else if (mode == 2) //방어구
	{
		for (a = 0; item(2000 + a).lv < minlv; a++); //투구
		for (b = 0; item(3000 + b).lv < minlv; b++); //갑옷
		for (c = 0; item(4000 + c).lv < minlv; c++); //신발


		for (a; item(2000 + a).lv < maxlv; a++) //투구
		{
			shopitem[i] = 2000 + a;
			i++;
		}

		temp1 = i;

		for (b; item(3000 + b).lv < maxlv; b++) //갑옷
		{
			shopitem[i] = 3000 + b;
			i++;
		}


		temp2 = i;


		for (c; item(4000 + c).lv < maxlv; c++) //신발
		{
			shopitem[i] = 4000 + c;
			i++;
		}
	}

	if (mode == 3) //보석
	{
		srand((int)time(NULL));

		for (a = 0; a < 6; a++)
		{

			b = rand() % 25;
			shopitem[a] = 5000 + b;
		}

	}
	if (mode == 4) //기타
	{
		for (a = 0; item(6000 + a).lv < minlv; a++);

		for (a; item(6000 + a).lv < maxlv; a++)
		{
			shopitem[i] = 6000 + a;
			i++;
		}
	}



	//여기서 부터 UI적용예정
	while (1)
	{
		system("cls");
		int key = 0;
		i = 0;
		//모드에 따른 이름 출력
		if (mode == 1)
		{
			system("mode con:cols=103 lines=29");
			printf("▤─────  ─────▤────────  상점  ─────────────────────────▤\n");
			printf("│이름 : %-15s│                   ☆                                        ________     │\n", item(shopitem[0]).name);
			printf("│공격력 : %-13d│                   ㅅ                                       /_______│    │\n", item(shopitem[0]).atk);
			printf("│방어력 : %-13d│                  /  ＼                    /＼              ＼    ││    │\n", item(shopitem[0]).def);
			printf("│체력 : %-15d│                 /    ＼                  /  ＼              /＼  ││    │\n", item(shopitem[0]).hp);
			printf("│스피드 : %-13d│                /      ＼                 ＼  ＼            /  /＼│/     │\n", item(shopitem[0]).spd);
			printf("│제한레벨 : %-11d│               │＼    /│               │＼  ＼          /  /           │\n", item(shopitem[0]).lv);
			printf("│가격 : %-15d│               │ ＼  / │               │ ＼  ＼        /  /            │\n", item(shopitem[0]).price);
			printf("▤─────  ─────▤  ┌──┐     │   V   │   ───┐│  │  ＼  ＼      /  /             │\n");
			printf("│이름 : %-15s│  │    │     │   │  │         ││  │   ＼  ＼    /  /              │\n", item(shopitem[1]).name);
			printf("│공격력 : %-13d│  └──┘     │   │  │         ││  │    ＼  ＼  /  /               │\n", item(shopitem[1]).atk);
			printf("│방어력 : %-13d│ ──┬──    │   │  │         ││  │   │＼  ＼/  /                │\n", item(shopitem[1]).def);
			printf("│체력 : %-15d│     │        │   │  │           │☆ ＼  │ ＼  ＼ /                 │\n", item(shopitem[1]).hp);
			printf("│스피드 : %-13d│               │   │  │               ☆＼ │  ＼  ＼                  │\n", item(shopitem[1]).spd);
			printf("│제한레벨 : %-11d│               │   │  │                  ＼│   ＼  ＼                 │\n", item(shopitem[1]).lv);
			printf("│가격 : %-15d│               │   │  │                         /＼  ＼                │\n", item(shopitem[1]).price);
			printf("▤─────  ─────▤               │   │  │                        /  ＼  ＼               │\n");
			printf("│이름 : %-15s│               │   │  │                       /  / ＼  ＼              │\n", item(shopitem[2]).name);
			printf("│공격력 : %-13d│             ┌┴   ┴  ┴┐                    /  /   ＼  ＼             │\n", item(shopitem[2]).atk);
			printf("│방어력 : %-13d│             │           │                   /  /     ＼  ＼            │\n", item(shopitem[2]).def);
			printf("│체력 : %-15d│             └─┬─┬─-┘                  /  /       ＼  /            │\n", item(shopitem[2]).hp);
			printf("│스피드 : %-13d│                 │//│                      /  /         ＼/             │\n", item(shopitem[2]).spd);
			printf("│제한레벨 : %-11d│                 ├─┤                     /__/                          │\n", item(shopitem[2]).lv);
			printf("│가격 : %-15d│                 └─┘                                                   │\n", item(shopitem[2]).price);
			printf("▤───────────▤─────────────────────────────────────▤\n");
			printf("      현재 소지 ");
			textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
			printf("골드:  %-10d", gold);
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
			printf(" / 구매 : ENTER / 나가기 : ESC\n");
			gotoxy(x, y);
			printf("▼");
			while (1)
			{
				key = _getch();
				if (key == 72)
				{
					printf("\b\b ");
					y -= 8;
					if (y < 0)
						y += 8;
					gotoxy(x, y);
					printf("▼");
				}
				else if (key == 27)
					return 0;
				else if (key == 32 || key == 13)
				{
					if (y == 0)
					{
						select = 1;
						break;
					}
					else if (y == 8)
					{
						select = 2;
						break;
					}
					else if (y == 16)
					{
						select = 3;
						break;
					}
				}
				else if (key == 80)
				{
					printf("\b\b ");
					y += 8;
					if (y > 20)
						y -= 8;
					gotoxy(x, y);
					printf("▼");
				}
			}
		}

		else if (mode == 2)
		{
			system("mode con:cols=103 lines=35");
			printf("▤─────  ─────▤────────  상점  ─────────▤─────  ─────▤\n");
			printf("│이름 : %-15s│ ______________           ______________  │이름 : %-15s│\n", item(shopitem[0]).name, item(shopitem[3]).name);
			printf("│공격력 : %-13d││             ＼         /            │ │공격력 : %-13d│\n", item(shopitem[0]).atk, item(shopitem[3]).atk);
			printf("│방어력 : %-13d││              ＼_______/             │ │방어력 : %-13d│\n", item(shopitem[0]).def, item(shopitem[3]).def);
			printf("│체력 : %-15d││               ___  __________       │ │체력 : %-15d│\n", item(shopitem[0]).hp, item(shopitem[3]).hp);
			printf("│스피드 : %-13d│ ＼_____        │＼│＼_______/│ ____/  │스피드 : %-13d│\n", item(shopitem[0]).spd, item(shopitem[3]).spd);
			printf("│제한레벨 : %-11d│       │       │  │  ＼＼＼  ││      │제한레벨 : %-11d│\n", item(shopitem[0]).lv, item(shopitem[3]).lv);
			printf("│가격 : %-15d│       │    _______│  ＼＼  ＼││      │가격 : %-15d│\n", item(shopitem[0]).price, item(shopitem[3]).price);
			printf("▤─────  ─────▤       │   / ┼┼┼│ ＼  ＼＼ ││      ▤─────  ─────▤\n");
			printf("│이름 : %-15s│       │ ┌────┤＼＼＼＼  ││      │이름 : %-15s│\n", item(shopitem[1]).name, item(shopitem[4]).name);
			printf("│공격력 : %-13d│       │ / ┼┼┼   ＼_________││      │공격력 : %-13d│\n", item(shopitem[1]).atk, item(shopitem[4]).atk);
			printf("│방어력 : %-13d│       ││ ┼┼┼┼┼┼┼┼┼┼││      │방어력 : %-13d│\n", item(shopitem[1]).def, item(shopitem[4]).def);
			printf("│체력 : %-15d│       │└──────────-┘│      │체력 : %-15d│\n", item(shopitem[1]).hp, item(shopitem[4]).hp);
			printf("│스피드 : %-13d│       │                         │      │스피드 : %-13d│\n", item(shopitem[1]).spd, item(shopitem[4]).spd);
			printf("│제한레벨 : %-11d│       └───────────-─┘      │제한레벨 : %-11d│\n", item(shopitem[1]).lv, item(shopitem[4]).lv);
			printf("│가격 : %-15d│                 │    │                 │가격 : %-15d│\n", item(shopitem[1]).price, item(shopitem[4]).price);
			printf("▤─────  ─────▤                 ├──┤                 ▤─────  ─────▤\n");
			printf("│이름 : %-15s│                 └-┬-┘                 │이름 : %-15s│\n", item(shopitem[2]).name, item(shopitem[5]).name);
			printf("│공격력 : %-13d│                ──┴──                │공격력 : %-13d│\n", item(shopitem[2]).atk, item(shopitem[5]).atk);
			printf("│방어력 : %-13d│                                          │방어력 : %-13d│\n", item(shopitem[2]).def, item(shopitem[5]).def);
			printf("│체력 : %-15d│       ──┴──        ───┐         │체력 : %-15d│\n", item(shopitem[2]).hp, item(shopitem[5]).hp);
			printf("│스피드 : %-13d│          /＼                  │         │스피드 : %-13d│\n", item(shopitem[2]).spd, item(shopitem[5]).spd);
			printf("│제한레벨 : %-11d│          ＼/           ──┬──        │제한레벨 : %-11d│\n", item(shopitem[2]).lv, item(shopitem[5]).lv);
			printf("│가격 : %-15d│       ──┴──           │            │가격 : %-15d│\n", item(shopitem[2]).price, item(shopitem[5]).price);
			printf("▤─────  ─────┼┬┬┬┬┬─────  ─────┬┬┬┬┬┼─────  ─────▤\n");
			printf("│이름 : %-15s├┼┼┼┼┤이름 : %-15s├┼┼┼┼┤이름 : %-15s│\n", item(shopitem[6]).name, item(shopitem[7]).name, item(shopitem[8]).name);
			printf("│공격력 : %-13d├┼┼┼┼┤공격력 : %-13d├┼┼┼┼┤공격력 : %-13d│\n", item(shopitem[6]).atk, item(shopitem[7]).atk, item(shopitem[8]).atk);
			printf("│방어력 : %-13d├┼┼┼┼┤방어력 : %-13d├┼┼┼┼┤방어력 : %-13d│\n", item(shopitem[6]).def, item(shopitem[7]).def, item(shopitem[8]).def);
			printf("│체력 : %-15d├┼┼┼┼┤체력 : %-15d├┼┼┼┼┤체력 : %-15d│\n", item(shopitem[6]).hp, item(shopitem[7]).hp, item(shopitem[8]).hp);
			printf("│스피드 : %-13d├┼┼┼┼┤스피드 : %-13d├┼┼┼┼┤스피드 : %-13d│\n", item(shopitem[6]).spd, item(shopitem[7]).spd, item(shopitem[8]).spd);
			printf("│제한레벨 : %-11d├┼┼┼┼┤제한레벨 : %-11d├┼┼┼┼┤제한레벨 : %-11d│\n", item(shopitem[6]).lv, item(shopitem[7]).lv, item(shopitem[8]).lv);
			printf("│가격 : %-15d├┼┼┼┼┤가격 : %-15d├┼┼┼┼┤가격 : %-15d│\n", item(shopitem[6]).price, item(shopitem[7]).price, item(shopitem[8]).price);
			printf("▤───────────┴┴┴┴┴┴───────────┴┴┴┴┴┴───────────▤\n");
			printf("      현재 소지 ");
			textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
			printf("골드:  %-10d", gold);
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
			printf(" / 구매 : ENTER / 나가기 : ESC\n");


			gotoxy(x, y);
			printf("▼");
			while (1)
			{
				key = _getch();
				if (key == 72)
				{
					printf("\b\b ");
					y -= 8;
					if (y < 0)
						y += 8;
					gotoxy(x, y);
					printf("▼");
				}
				else if (key == 27)
					return 0;
				else if (key == 32 || key == 13)
				{
					if (y == 0 && x == 12)
					{
						select = 1;
						break;
					}
					else if (y == 8 && x == 12)
					{
						select = 2;
						break;
					}
					else if (y == 16 && x == 12)
					{
						select = 3;
						break;
					}
					else if (y == 0 && x == 80)
					{
						select = 4;
						break;
					}
					else if (y == 8 && x == 80)
					{
						select = 5;
						break;
					}
					else if (y == 16 && x == 80)
					{
						select = 6;
						break;
					}
					else if (y == 24 && x == 12)
					{
						select = 7;
						break;
					}
					else if (y == 24 && x == 46)
					{
						select = 8;
						break;
					}
					else if (y == 24 && x == 80)
					{
						select = 9;
						break;
					}
				}
				else if (key == 80)
				{
					printf("\b\b ");
					y += 8;
					if (y > 26)
						y -= 8;
					gotoxy(x, y);
					printf("▼");
				}
				else if (key == 77)
				{
					printf("\b\b ");
					if (y < 24)
						x += 68;
					else if (y == 24)
						x += 34;

					if (x < 10)
						x -= 34;
					else if (x > 80)
						x -= 68;
					gotoxy(x, y);
					printf("▼");
				}
				else if (key == 75)
				{
					printf("\b\b ");
					if (y < 24)
						x -= 68;
					else if (y == 24)
						x -= 34;

					if (x < 10)
						x += 34;
					gotoxy(x, y);
					printf("▼");
				}
			}



		}

		else if (mode == 3)
		{
			system("mode con:cols=103 lines=27");
			printf("▤─────  ─────▤────────  상점  ──────────▤─────  ─────▤\n");
			printf("│이름 : %-15s│      ■■■■■■■■■■■■■■■■      │이름 : %-15s│\n", item(shopitem[0]).name, item(shopitem[1]).name);
			printf("│공격력 : %-13d│     ■       //■         ■        ■     │공격력 : %-13d│\n", item(shopitem[0]).atk, item(shopitem[1]).atk);
			printf("│방어력 : %-13d│    ■       //■           ■        ■    │방어력 : %-13d│\n", item(shopitem[0]).def, item(shopitem[1]).def);
			printf("│체력 : %-15d│   ■       //■             ■        ■   │체력 : %-15d│\n", item(shopitem[0]).hp, item(shopitem[1]).hp);
			printf("│스피드 : %-13d│  ■■■■■■■■■■■■■■■■■■■■  │스피드 : %-13d│\n", item(shopitem[0]).spd, item(shopitem[1]).spd);
			printf("│                      │   ■        ■            //■      //■   │                      │\n");
			printf("│가격 : %-15d│    ■        ■          //■      //■    │가격 : %-15d│\n", 1, 1);
			printf("▤─────  ─────▤     ■        ■        //■      //■     ▤─────  ─────▤\n");
			printf("│이름 : %-15s│      ■        ■      //■      //■      │이름 : %-15s│\n", item(shopitem[2]).name, item(shopitem[3]).name);
			printf("│공격력 : %-13d│       ■        ■    //■      //■       │공격력 : %-13d│\n", item(shopitem[2]).atk, item(shopitem[3]).atk);
			printf("│방어력 : %-13d│        ■        ■  //■      //■        │방어력 : %-13d│\n", item(shopitem[2]).def, item(shopitem[3]).def);
			printf("│체력 : %-15d│         ■        ■//■      //■         │체력: %-15d │\n", item(shopitem[2]).hp, item(shopitem[3]).hp);
			printf("│스피드 : %-13d│          ■        ■■      //■          │스피드: %-13d │\n", item(shopitem[2]).spd, item(shopitem[3]).spd);
			printf("│                      │           ■■■■■■■■■■■           │                      │\n");
			printf("│가격 : %-15d│                                            │가격 : %-15d│\n", item(shopitem[2]).price, item(shopitem[3]).price);
			printf("▤─────  ─────▤                                            ▤─────  ─────▤\n");
			printf("│이름 : %-15s│                                            │이름: %-15s │\n", item(shopitem[4]).name, item(shopitem[5]).name);
			printf("│공격력 : %-13d│                                            │공격력 : %-12d │\n", item(shopitem[4]).atk, item(shopitem[5]).atk);
			printf("│방어력 : %-13d│         │    │        ㅅ     │          │방어력: %-13d │\n", item(shopitem[4]).def, item(shopitem[5]).def);
			printf("│체력 : %-15d│         ├──┤       /  ＼ ─┤          │체력: %-15d │\n", item(shopitem[4]).hp, item(shopitem[5]).hp);
			printf("│스피드 : %-13d│         └-┬-┘      /    ＼  │          │스피드: %-13d │\n", item(shopitem[4]).spd, item(shopitem[5]).spd);
			printf("│                      │            │           ───┐           │                      │\n");
			printf("│가격 : %-15d│        ──┴──             │           │가격: %-15d │\n", item(shopitem[4]).price, item(shopitem[5]).price);
			printf("▤───────────▤──────────────────────▤───────────▤\n");
			printf("      현재 소지 ");
			textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
			printf("골드:  %-10d", gold);
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
			printf(" / 구매 : ENTER / 나가기 : ESC\n");

			gotoxy(x, y);
			printf("▼");
			while (1)
			{
				key = _getch();
				if (key == 72)
				{
					printf("\b\b ");
					y -= 8;
					if (y < 0)
						y += 8;
					gotoxy(x, y);
					printf("▼");
				}
				else if (key == 27)
					return 0;
				else if (key == 32 || key == 13)
				{
					if (y == 0 && x == 12)
					{
						select = 1;
						break;
					}
					else if (y == 8 && x == 12)
					{
						select = 3;
						break;
					}
					else if (y == 16 && x == 12)
					{
						select = 5;
						break;
					}
					else if (y == 0 && x == 82)
					{
						select = 2;
						break;
					}
					else if (y == 8 && x == 82)
					{
						select = 4;
						break;
					}
					else if (y == 16 && x == 82)
					{
						select = 6;
						break;
					}
				}
				else if (key == 80)
				{
					printf("\b\b ");
					y += 8;
					if (y > 26)
						y -= 8;
					gotoxy(x, y);
					printf("▼");
				}
				else if (key == 77)
				{
					printf("\b\b ");
					x += 70;
					if (x < 10)
						x -= 70;
					else if (x > 85)
						x -= 70;
					gotoxy(x, y);
					printf("▼");
				}
				else if (key == 75)
				{
					printf("\b\b ");
					x -= 68;
					if (x < 10)
						x += 68;
					gotoxy(x, y);
					printf("▼");
				}
			}

		}

		else if (mode == 4)
		{
			system("mode con:cols=103 lines=27");
			printf("▤─────  ─────▤────────────────────────────▤\n");
			printf("│이름:%-17s│                                                        │\n", item(shopitem[0]).name);
			printf("│                      │                                                        │\n");
			printf("│체력회복량:%-11d│                           ┏━━━━━━┓             │\n", item(shopitem[0]).hp);
			printf("│                      │       -┬─┬-           □□□□□□□□□            │\n");
			printf("│                      │        │  │            □□          □□            │\n");
			printf("│가격:%-17d│       -┴┬┴-             □          □              │\n", item(shopitem[0]).price);
			printf("│                      │          │                □━━━━━□              │\n");
			printf("▤─────  ─────▤      ──┴──            □          □              ▤\n");
			printf("│이름:%-17s│                            □          □              │\n", item(shopitem[1]).name);
			printf("│                      │                            □          □              │\n");
			printf("│체력회복량:%-11d│         /      │         □            □             │\n", item(shopitem[1]).hp);
			printf("│                      │        /＼ ──┤        □              □            │\n");
			printf("│                      │       /   ＼ ─┤       □");
			textColor(FOREGROUND_RED);
			printf("■■■■■■■■");
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			printf("□           │\n");
			printf("│가격:%-17d│      /      ＼ │      □", item(shopitem[1]).price);
			textColor(FOREGROUND_RED);
			printf("■■■■■■■■■");
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			printf("□          │\n");
			printf("│                      │                │     □");
			textColor(FOREGROUND_RED);
			printf("■■■■■■■■■■");
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			printf("□         │\n");
			printf("▤─────  ─────▤        │            □");
			textColor(FOREGROUND_RED);
			printf("■■■■■■■■■■■");
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			printf("□        ▤\n");
			printf("│이름:%-17s│        └───     □", item(shopitem[2]).name);
			textColor(FOREGROUND_RED);
			printf("■■■■■■■■■■■■");
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			printf("□       │\n");
			printf("│                      │                     □");
			textColor(FOREGROUND_RED);
			printf("■■■■■■■■■■■■");
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			printf("□       │\n");
			printf("│체력회복량:%-11d│                      □", item(shopitem[2]).hp);
			textColor(FOREGROUND_RED);
			printf("■■■■■■■■■■■");
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			printf("□        │\n");
			printf("│                      │                        □");
			textColor(FOREGROUND_RED);
			printf("■■■■■■■■■");
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			printf("□          │\n");
			printf("│                      │                         □");
			textColor(FOREGROUND_RED);
			printf("■■■■■■■■");
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			printf("□           │\n");
			printf("│가격:%-17d│                           □", item(shopitem[2]).price);
			textColor(FOREGROUND_RED);
			printf("■■■■■■");
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			printf("□             │\n");
			printf("│                      │                            □□□□□□□              │\n");
			printf("▤───────────▤────────────────────────────▤\n");
			printf("      현재 소지 ");
			textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색
			printf("골드:  %-10d", gold);
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
			printf(" / 구매 : ENTER / 나가기 : ESC\n");

			gotoxy(x, y);
			printf("▼");
			while (1)
			{
				key = _getch();
				if (key == 72)
				{
					printf("\b\b ");
					y -= 8;
					if (y < 0)
						y += 8;
					gotoxy(x, y);
					printf("▼");
				}
				else if (key == 27)
					return 0;
				else if (key == 32 || key == 13)
				{
					if (y == 0)
					{
						select = 1;
						break;
					}
					else if (y == 8)
					{
						select = 2;
						break;
					}
					else if (y == 16)
					{
						select = 3;
						break;
					}
				}
				else if (key == 80)
				{
					printf("\b\b ");
					y += 8;
					if (y > 20)
						y -= 8;
					gotoxy(x, y);
					printf("▼");
				}
			}


		}


		system("cls");

		select--;

		if (shopitem[select] % 1000 == 0)//고른아이템이 비어있을때 아무 반응 없음
			continue;

		if (gold < item(shopitem[select]).price)
		{
			printf("이걸 사기엔 골드가 부족합니다...");
			Sleep(1000);
			continue;
		}


		//확인
		printf("%s를 사시겠습니까? (YES = ENTER / NO = ESC)\n\n", item(shopitem[select]).name);
		while (1)
		{
			key = _getch();
			if (key == 13 || key == 32)
			{
				accept = 1;
				break;
			}
			else if (key == 27 || key == 48)
			{
				accept = 0;
				break;
			}
		}



		if (accept == 1) //맞을경우
		{
			gold -= item(shopitem[select]).price;
			Inven(4, shopitem[select]);
			printf("%s를 구매했습니다!", item(shopitem[select]).name);
			questfunc(3, shopitem[select]);
			Sleep(700);
		}

		else if (accept == 0) //아닐경우
		{
			continue;
		}
	}

	return 0;
}

int expadd(int get) // 사용법 : get(얻은exp);
{
	int temp;
	int tempexp;//얻은 exp;

	tempexp = exp;
	exp += get;

	system("cls");

	while (tempexp <= exp)
	{
		printf(" %-6d / %-6d exp", tempexp, requireexp);
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		Sleep(10);


		if (tempexp >= requireexp)
		{
			if (lv < 10)
			{
				temp = (lv * 200) - 100;
				requireexp += temp;
			}
			else
			{
				temp = (lv * 300) - 1100;
				requireexp += temp;
			}

			lv++;
			atk += 3;
			hp += 5;
			spd += 2;
			def += 1;

			if (2 < lv && lv <= 20) // lvl이 2~20일때까지는 1업마다 스탯이 1 오름
			{
				SP += 1;
			}
			else if (20 < lv && lv <= 40)
			{
				SP += 2;
			}
			else if (40 < lv && lv <= 60)
			{
				SP += 3;
			}
			else if (60 < lv && lv <= 80)
			{
				SP += 4;
			}
			else
			{
				SP += 5;
			}

			system("cls");
			printf("lv업!\n");
			Sleep(300);
			printf("◆스탯──────────────────────────────────────────┐\n");
			printf("│레벨: %-3d  공격력 : %-5d   체력 : %-5d   방어력 : %-5d   스피드 : %-5d 현재 SP : %-3d│ \n", lv, atk, hp, def, spd, SP);
			printf("└────────────────────────────────────────────┘\n");

			fflush(stdin);
			getch();
			system("cls");
		}

		if (get < 1000)
			tempexp++;
		else if (get >= 1000 && get < 5000)
			tempexp += 10;
		else if (get >= 5000 && get < 10000)
			tempexp += 50;
		else if (get >= 10000)
			tempexp += 100;
	}

	return;
}

void stat() // SP는 20부터, 나머지는 0부터 시작한다 main함수 만드는 놈아 잘 넣어놔라
{  //필요한것들 선언 스탯4개 스킬 4개 저장용 1개 남은스탯 저장용 1개
	int aatk = 0, ahp = 0;
	int saver = 0;
	int allstat;

	allstat = aatk + ahp;
	system("mode con:lines=26 cols=96");
	system("title 용검 전설");

	while (1) //기본 틀◇⊙▧□
	{
		system("cls");

		printf("\n◇───────스탯 현황──────┬─────────┐◇────────────────┐\n");
		printf("│이 스탯을 찍으려면 1을 눌러 주세요.│힘 = %-13d││힘에 투자한 스탯 : %-11d  │\n", atk, aatk); // 32space, 16글자
		printf("│                                   │                  ││                                │\n");
		printf("│이 스탯을 찍으려면 2를 눌러 주세요.│생명력 = %-9d││생명력에 투자한 스탯 : %-5d    │\n", hp, ahp);
		printf("└───────────────-──┴─────────□└────────────────□");
		printf("\n▧───────┬─────────────  Stat  ───────-▧───────┬────┐\n");
		printf("│남은 SP : %-4d│ ■■■■■■■■■■■■■■■■■■■■■■■■│   용사 레벨  │%-8d│\n", SP, lv); // 91space, 45글자
		printf("├───────▧──┬────-─────────────────┼───────┼────┤\n");
		printf("│    공격력    │%-4d│                                           │ 필요한  경치 │%-8d│\n", atk, requireexp - exp);
		printf("├───────┼──┤                                           ├───────┼────┤\n");
		printf("│     체력     │%-4d│                                           │ 다음 레벨까지│%-8d│\n", hp, requireexp);
		printf("├───────┼──┤                                           ├───────┼────▧\n");
		printf("│    방어력    │%-4d│                                           │ 현재 경험치  │%-8d│\n", def, exp);
		printf("├───────┼──┤※제작자가 알려주는 스탯 공식              └───────┴────▧\n");
		printf("│    스피드    │%-4d│├체력은 1레벨 당 5씩 오르고, 1생명력 당 10 오릅니다.                │\n", spd);
		printf("├───────┼──┤├방어력은 1레벨 당 1씩 오릅니다.                                    │\n");
		printf("│   방어력     │%-4d│├공격력은 1레벨 당 3씩 오르고,1힘 당 7 오릅니다.                    │\n", def);
		printf("├───────┼──┤└스피드는 1레벨 당 2씩 오릅니다.                                    │\n");
		printf("│   스피드     │%-4d│                                                                     │\n", spd);
		printf("└───────┴──▧───────────────────-───────────────▧");

		allstat = aatk + ahp;
		printf("\n지금까지 투자한 모든 스탯은 %d개입니다                                    \n\n\n", allstat);

		//스킬을 찍은후 이부분으로 되돌아 옴으로 남은 스탯 1개 감소


		saver = _getch();

		if (saver == 27)
		{
			return; // ESC키로 프로그램 종료
		}
		else if (saver == 48){}
		else if (saver == 49)
		{
			if (SP <= 0)
			{
				system("cls");
				printf("스텟이 부족합니다...");
				Sleep(500);
				system("cls");
				continue;
			}
			atk += 7;
			aatk++;
			SP--;
			allstat = aatk + ahp;
		} // 1 누르면 힘1찍힘
		else if (saver == 50)
		{
			if (SP <= 0)
			{
				system("cls");
				printf("스텟이 부족합니다...");
				Sleep(500);
				system("cls");
				continue;
			}
			hp += 10;
			ahp++;
			SP--;
			allstat = aatk + ahp;
		} // 2 누르면 체1찍힘
		else if (saver == 51){}
		else if (saver == 52){}
		else if (saver == 53){}
		else if (saver == 54){}
		else if (saver == 55){}
		else if (saver == 56){}
		else if (saver == 1)
		{
			expadd(requireexp - exp); // ctrl+A = 1레벨 업 버그키
			allstat--;
		}
	}
}

int quest_pirate(void)
{
	questfunc(1, 0);
	battle(41);
	system("cls");
	printf("\n\n\n\n\n    쳇... 아직도 몰려오고 있어.");
	Sleep(1000);

	battle(41);
	system("cls");
	printf("\n\n\n\n\n    진짜 많네...");
	Sleep(1000);

	battle(41);
	system("cls");
	printf("\n\n\n\n\n    와...");
	Sleep(1000);
	
	battle(42);
	system("cls");
	printf("\n\n\n\n\n    끝이 안보인다. 미치겠네...");
	Sleep(1000);

	battle(42);
	system("cls");
	printf("\n\n\n\n\n    용사님 괜찮으십니까?");
	Sleep(1000);

	battle(43);
	system("cls");
	printf("\n\n\n\n\n    끝이 보입니다 좀더 힘내십시오!!!");
	Sleep(1000);

	battle(43);
	questfunc(1, 0);

	questfunc(1, 0);
	battle(44);
	questfunc(1, 0);

	questfunc(1, 0);
	battle(45);
	questfunc;
}

void save()
{
	FILE * fp;
	fp = fopen("c:\\save\\SAVE.txt", "w");
	fprintf(fp, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", hp, mhp, def, atk, spd, lv, exp, gold);
	for (int i = 0; i <= 6; i++)
	{
		for (int j = 0; j <= 12; j++)
		{
			for (int k = 0; k <= 2; k++)
				fprintf(fp, "%d ", inven[i][j][k]);
		}
	}

	for (int t = 0; t <= 7; t++)
		fprintf(fp, "%d ", set[t]);
	fprintf(fp, "%d %d ", achieve, succeed);

	fclose(fp);
}

void load()
{
	if (gold < 0)
		gold == 0;
	if (hp < 0)
		hp = 100;

	

	FILE * fp;
	fp = fopen("c:\\save\\SAVE.txt", "r");
	fscanf(fp, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", &hp, &mhp, &def, &atk, &spd, &lv, &exp, &gold);
	for (int i = 0; i <= 6; i++)
	{
		for (int j = 0; j <= 12; j++)
		{
			for (int k = 0; k <= 2; k++)
				fscanf(fp, "%d ", &inven[i][j][k]);
		}
	}

	for (int t = 0; t <= 7; t++)
		fscanf(fp, "%d ", &set[t]);

	fscanf(fp, "%d %d ", &achieve, &succeed);

	fclose(fp);
}
void BATTLEBACKGROUND(int num)

{
	if (num == 1)//들판
	{
		// system("mode con:cols=125 lines=40"); //적용할떄 꼭 사용하기!!


		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	
		printf("▒▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_INTENSITY); //밝은 초록색
		printf("////＼＼");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
		printf("▒▒▒▒▒▒◁▣■■〓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		printf("▒▒▒");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색

		printf("▒");

		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("▒▒▒▒▒▒▒\n");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색

		printf("▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_INTENSITY); //밝은 초록색
		printf("////♣＼＼＼");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
		printf("▒▒▒▒▒▒▒▼▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("▒▒▒▒▒▒▒\n");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색

		printf("▒▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_INTENSITY); //밝은 초록색
		printf("//// ♧♣ ＼＼＼");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▲▒▒▒▒▒▒");

		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("▒▒▒▒▒▒\n");


		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색

		printf("▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_INTENSITY); //밝은 초록색
		printf("//// ♣♧♣   ＼＼＼");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▲▒▒▒▒▒▒▒▒▒▒▒▒▒▒◁▣■■〓▒▒▒▒▒");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		printf("▒▒▒▒▒\n");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색

		printf("▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_INTENSITY); //밝은 초록색
		printf("////  ♣ ♣ ♧ ♣ ＼＼＼");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥tor
		printf("▒▒▒▒▒▒▒▒▒▒▒▒◁▣■■〓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▼▒▒▒▒▒▒▒▒");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		printf("▒▒▒▒\n");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥tor

		printf("▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_INTENSITY); //밝은 초록색
		printf("////♧ ♣  ♣ ♣  ♧  ＼＼＼");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▼▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");
		printf("▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_INTENSITY); //밝은 초록색
		printf("////  ♧  ♣ ♣   ♣  ♣  ＼＼＼");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		printf("▒\n");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색

		printf("▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_INTENSITY); //밝은 초록
		printf("//// ♧  ♣ ♣   ♣  ♣♧     ＼＼＼");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색
		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("┌───────────────────────────────────────────────────────────┐\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│◎                                                                                                                  ◎│\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("└───────────────────────────────────────────────────────────┘\n");

	}


	if (num == 2)//사막
	{
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색	 // ");printf("
		printf("■■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	


		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");
		printf("▒▒▒▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색	 // ");printf("
		printf("■■■■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒▒▒▒▒▒");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색	 // ");printf("

		printf("■■■");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	
		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");

		printf("▒▒▒▒▒▒▒▒▒▒");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색	 // ");printf("


		printf("■■■■■■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색	 // ");printf("

		printf("§");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색	 // ");printf("

		printf("■■■■■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
		printf("▒▒▒▒▒▒▒▒▒▒\n");

		printf("▒▒▒▒▒▒▒▒▒");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색	 // ");printf("

		printf("■■■■■■■■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색	 // ");printf("

		printf("§");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색	 // ");printf("


		printf("■■□■□■■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //밝은 노란색	 // ");printf("

		printf("▒▒▒▒▒▒▒▒▒▒▒▒\n");

		printf("▒▒▒▒▒▒▒▒");

		printf("■■■■■■■■■■");
		printf("§");

		printf("▒▒▒▒");
		printf("■■■■■");
		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");
		printf("▒▒▒▒▒▒▒");

		printf("■■■■■■■■■■■■");
		printf("□□□□□");

		printf("■■■");

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");

		printf("▒▒▒▒▒▒");

		printf("■■■■■■■■■■■■■■");

		printf("□□□■□□□■");

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");

		printf("▒▒▒▒▒");

		printf("■■■■■■■■■■■■■■■■");
		printf("□□");
		printf("■■■");
		printf("□");
		printf("■■■");

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("┌───────────────────────────────────────────────────────────┐\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│◎                                                                                                                  ◎│\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("└───────────────────────────────────────────────────────────┘\n");
	}


	if (num == 3)//항구
	{
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	
		printf("▒▒▒▒▒▒▒▒▒");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
		printf("■■■■■■■■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	
		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");

		printf("▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
		printf("■");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");

		printf("▒▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■■■■■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■■■");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■■■■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");


		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	


		printf("▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■■");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■■■■■");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒▒▒▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■■■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■■■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒\n");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■■");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■■");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■■■");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //밝은 옥색	

		printf("▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■■■");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■■■\n");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색

		printf("■■■■");
		textColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //밝은 파란색

		printf("▒▒▒▒▒▒▒▒▒▒▒▒\n");


		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("┌───────────────────────────────────────────────────────────┐\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│◎                                                                                                                  ◎│\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("└───────────────────────────────────────────────────────────┘\n");

	}


	if (num == 4)// 황야
	{
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("////////////////////");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("/////////////////////////////////");
		textColor(FOREGROUND_RED | BACKGROUND_RED | BACKGROUND_GREEN); //어두운 빨간색 ");printf("


		printf("■■■■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("/////////////////////////////////");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("//");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("////////////////\n");

		printf("////////////////");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("//");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("//");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("/////////////////////////");
		textColor(FOREGROUND_RED | BACKGROUND_RED | BACKGROUND_GREEN); //어두운 빨간색 ");printf("

		printf("■■■■■■■■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("/////////////////////////");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("//");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("//");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("////////////////\n");
		printf("////////////////");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■■■■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("//");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("///////////////////////");
		textColor(FOREGROUND_RED | BACKGROUND_RED | BACKGROUND_GREEN); //어두운 빨간색 ");printf("

		printf("■■■■■■■■■■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("///////////////////////");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("//");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■■■■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("////////////////\n");
		printf("////////////////////");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■■■■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("/////////////////////");
		textColor(FOREGROUND_RED | BACKGROUND_RED | BACKGROUND_GREEN); //어두운 빨간색 ");printf("

		printf("■■■■■■■■■■■■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("/////////////////////");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■■■■");
		textColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 빨간색 ");printf("

		printf("////////////////////\n");

		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 검정색

		printf("■■■■■■■■■■");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■■");
		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 검정색

		printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■■");
		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 검정색

		printf("■■■■■■■■■■\n");

		printf("■■■");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 노란색

		printf("▒▒▒▒▒▒▒");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■■");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 노란색

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 검정색

		printf("■■");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 노란색
		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■■");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 노란색

		printf("▒▒▒▒▒▒▒▒▒▒\n");
		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 검정색

		printf("■■■");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 노란색

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
		textColor(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 초록색

		printf("■■");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 노란색

		printf("▒▒▒");
		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 검정색

		printf("■■■■■");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 노란색

		printf("▒▒\n");
		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 검정색

		printf("■■■■■■");
		textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN); //밝은 노란색

		printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");
		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("┌───────────────────────────────────────────────────────────┐\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│◎                                                                                                                  ◎│\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("└───────────────────────────────────────────────────────────┘\n");
	}

	if (num == 5)//죽음의협곡
	{
		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 배경색



		printf("                                                                                                                           \n");



		printf("                                                                                                                           \n");



		printf("___________");



		printf("||");



		printf("┳┳┳┳");



		printf("▒");



		printf("┳┳┳┳┳┳┳┳┳");



		printf("▒");



		printf("┳┳┳┳┳┳┳");



		printf("▒▒");



		printf("┳┳┳┳┳┳┳");



		printf("▒▒▒▒▒");



		printf("┳┳┳┳┳┳┳┳┳┳┳┳┳");



		printf("||");



		printf("_________ \n");





		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_RED); // 땅부분

		printf("▒▒▒▒▒");



		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 배경색

		printf("||");



		printf("┳┳┳┳┳┳┳┳┳┳┳┳");



		printf("▒▒");



		printf("┳┳┳┳");



		printf("▒▒▒▒");



		printf("┳┳┳┳┳┳┳┳┳┳┳┳┳┳┳");



		printf("▒▒▒");



		printf("┳┳┳┳┳┳┳┳┳");



		printf("||");

		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_RED); // 땅부분



		printf("▒▒▒▒▒ \n");





		printf("■■■■■■");



		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 배경색

		printf("┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻");



		printf("▒▒▒▒▒");



		printf("┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻┻");



		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_RED); // 땅부분

		printf("■■■■■■ \n");





		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_RED); // 땅부분

		printf("■■■");



		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 배경색

		printf("▒▒▒▒");



		printf("         ");



		printf("▒▒▒▒▒");



		printf("                             ");



		printf("▒");



		printf("  ");



		printf("▒▒");



		printf("            ");



		printf("▒▒");



		printf("  ");



		printf("▒");



		printf("  ");



		printf("▒▒");



		printf("          ");



		printf("▒▒▒▒▒");



		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_RED); // 땅부분

		printf("■■■ \n");





		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_RED); // 땅부분

		printf("■");



		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 배경색

		printf("▒▒");



		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_RED); // 땅부분

		printf("■■");



		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 배경색

		printf("                              ");



		printf("▒▒▒▒▒");



		printf("                        ");



		printf("▒▒▒▒");



		printf("                              ");



		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_RED); // 땅부분

		printf("■");



		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 배경색

		printf("▒▒");



		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_RED); // 땅부분

		printf("■■ \n");





		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_RED); // 땅부분

		printf("■■");



		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); // 배경색

		printf("▒▒▒");



		printf("                                                                                                        ");



		textColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_RED); // 땅부분

		printf("■■■■ \n");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("┌───────────────────────────────────────────────────────────┐\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│◎                                                                                                                  ◎│\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("└───────────────────────────────────────────────────────────┘\n");
	}

	else if (num == 0)//왕궁
	{
		textColor(0x0088); // 흰/검 배경색

		printf("▒▒");





		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("┌┐");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■■■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("┌┐");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분┌┐

		printf("■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■■■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("┌┐");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■■■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("┌┐");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분♠

		printf("■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■■■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("┌┐");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■■■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒ \n");





		textColor(0x00FF); // 파/파 배경색

		printf("▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");

		// textColor(0x00FF); // 파/파 배경색

		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("§§");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x00FF); // 파/파 배경색	

		printf("▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("§§");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("§§");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("§§");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("§§");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒ \n");





		textColor(0x0099); // 옅빨/옅빨 배경

		printf("▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분└┘

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("└┘");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0099); // 옅빨/옅빨 배경

		printf("▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x0099); // 옅빨/옅빨 배경

		printf("▒▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("└┘");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0099); // 옅빨/옅빨 배경

		printf("▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x0099); // 옅빨/옅빨 배경

		printf("▒▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("└┘");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0099); // 옅빨/옅빨 배경

		printf("▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x0099); // 옅빨/옅빨 배경

		printf("▒▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("└┘");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0099); // 옅빨/옅빨 배경

		printf("▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x0099); // 옅빨/옅빨 배경

		printf("▒▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0047); // 빨간색 깃발부분 & 복도부분 =||

		printf("└┘");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■");



		textColor(0x0099); // 옅빨/옅빨 배경

		printf("▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x0099); // 옅빨/옅빨 배경

		printf("▒▒ \n");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒▒");



		textColor(0x000E); // 노랑색 깃발부분 =|| 전부분

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x00FF); // 파/파 배경색

		printf("▒▒ \n");





		textColor(0x0088); // 흰/검 배경색

		printf("▒▒▒▒▒▒▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒▒▒▒▒▒▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒▒▒▒▒▒▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒▒▒▒▒▒▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒▒▒▒▒▒▒▒▒");



		textColor(0x0008); // 검/회 기둥색

		printf("■■");



		textColor(0x0088); // 흰/검 배경색

		printf("▒▒ \n");





		textColor(0x0080); // 검/회 기둥색

		printf("┯━━━━━━");



		printf("┯");

		textColor(0x0008); // 검/회 기둥색



		printf("■■■■");



		textColor(0x0080); // 검/회 기둥색
		printf("┯");
		printf("━━━━━━");
		textColor(0x0080); // 검/회 기둥색
		printf("┯");
		textColor(0x0008); // 검/회 기둥색
		printf("■■■■");
		textColor(0x0080); // 검/회 기둥색
		printf("┯");
		printf("━━━━━━");
		textColor(0x0080); // 검/회 기둥색
		printf("┯");
		textColor(0x0008); // 검/회 기둥색
		printf("■■■■");
		textColor(0x0080); // 검/회 기둥색
		printf("┯");
		printf("━━━━━━");
		textColor(0x0080); // 검/회 기둥색
		printf("┯");
		textColor(0x0008); // 검/회 기둥색
		printf("■■■■");
		textColor(0x0080); // 검/회 기둥색
		printf("┯");
		printf("━━━━━━");
		textColor(0x0080); // 검/회 기둥색
		printf("┯");
		textColor(0x0008); // 검/회 기둥색
		printf("■■■■");
		textColor(0x0080); // 검/회 기둥색
		printf("┯ \n");
		textColor(0x00C0); // 빨간색 깃발부분 & 복도부분 =||
		printf("▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧ \n");
		printf("▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧ \n");

		textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("┌───────────────────────────────────────────────────────────┐\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│◎                                                                                                                    │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("│                                                                                                                      │\n");
		printf("└───────────────────────────────────────────────────────────┘\n");
	}

}
void holy_village();
void yakcho_village();
void potion_village();
void home_village();
void res_village();
void kingdom_village();
void field_field();//1
void desert_field();//2
void harbor_field();//3
void outback_field();//4
void death_field();//5
void kingdom_field();//0

int field_Blockcheck(int num, int x, int y)
{
	int i = 0;
	char fieldBlock[100][150] = { 0 };

	for (i = 0; i <= 150; i++)
		fieldBlock[8][i] = 100;//벽
	for (i = 0; i <= 150; i++)
		fieldBlock[30][i] = 100;//벽
	for (i = 0; i <= 31; i++)
		fieldBlock[i][1] = 100;
	for (i = 0; i <= 31; i++)
		fieldBlock[i][119] = 100;


	if (num == 1)//들판

	{
		fieldBlock[19][2] = 101;//포션마을 포탈
		fieldBlock[19][3] = 101;//포션마을 포탈
		fieldBlock[19][117] = 102;//약초마을 포탈
		fieldBlock[19][118] = 102;//약초마을 포탈

	}
	else if (num == 2)//사막
	{
		fieldBlock[19][2] = 103;//약초마을 포탈
		fieldBlock[19][3] = 103;//약초마을 포탈
		fieldBlock[19][117] = 104;//홀리시티 포탈
		fieldBlock[19][118] = 104;//홀리시티 포탈
	}
	else if (num == 3)//항구
	{
		fieldBlock[19][2] = 105;//홀리시티 포탈
		fieldBlock[19][3] = 105;//홀리시티 포탈
		fieldBlock[19][117] = 106;//용사의고향 포탈
		fieldBlock[19][118] = 106;//용사의고향 포탈
	}
	else if (num == 4)//황야
	{
		fieldBlock[19][2] = 107;//용사의고향 포탈
		fieldBlock[19][3] = 107;//용사의고향 포탈
		fieldBlock[19][117] = 108;//전초기지 포탈
		fieldBlock[19][118] = 108;//전초기지 포탈
	}
	else if (num == 5)//죽음의협곡
	{
		fieldBlock[19][2] = 109;//전초기지 포탈
		fieldBlock[19][3] = 109;//전초기지 포탈
		fieldBlock[19][117] = 110;//지배된왕국 포탈
		fieldBlock[19][118] = 110;//지배된왕국 포탈
	}
	else if (num == 0)//왕궁
	{
		fieldBlock[19][2] = 111;//지배된왕국 포탈
		fieldBlock[19][3] = 111;//지배된왕국 포탈

	}

	if (fieldBlock[y][x] == 100)
		return 100;
	if (fieldBlock[y][x] == 101)
		return 101;
	if (fieldBlock[y][x] == 102)
		return 102;
	if (fieldBlock[y][x] == 103)
		return 103;
	if (fieldBlock[y][x] == 104)
		return 104;
	if (fieldBlock[y][x] == 105)
		return 105;
	if (fieldBlock[y][x] == 106)
		return 106;
	if (fieldBlock[y][x] == 107)
		return 107;
	if (fieldBlock[y][x] == 108)
		return 108;
	if (fieldBlock[y][x] == 109)
		return 109;
	if (fieldBlock[y][x] == 110)
		return 110;
	if (fieldBlock[y][x] == 111)
		return 111;

}



//시발롬드라 이렇게 만들면 배틀하고 나오면 다 사라지지 볍신드라
void field_field()
{
	system("mode con:cols=125 lines=40");
	int x, y;
	x = 10; y = 10;
	int footwalk = 0;
	int startx = 0; int starty = 0;
	int endy = 0; int endx = 0;
	int temp;

	int a = 0;

	system("cls");
	BATTLEBACKGROUND(1);

	while (1)
	{


		gotoxy(x, y);
		printf("●");
		a = _getch();

		if (a == 72)
		{
			printf("\b\b  ");
			y -= 1;
			if (field_Blockcheck(1, x, y) == 100)//벽
				y += 1;
			else if (field_Blockcheck(1, x, y) == 101)//포션마을 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n   포션마을로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				potion_village();
				break;
			}
			else if (field_Blockcheck(1, x, y) == 102)//약초마을 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n   약초마을로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				yakcho_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 80)
		{
			printf("\b\b  ");
			y += 1;
			if (field_Blockcheck(1, x, y) == 100)
				y -= 1;
			else if (field_Blockcheck(1, x, y) == 101)//포션마을 포탈
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   포션마을로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				potion_village();
				break;
			}
			else if (field_Blockcheck(1, x, y) == 102)//약초마을 포탈
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   약초마을로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				yakcho_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 75)
		{
			printf("\b\b");
			x -= 1;
			if (field_Blockcheck(1, x, y) == 100)
				x += 1;
			else if (field_Blockcheck(1, x, y) == 101)//포션마을 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n   포션마을로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				potion_village();
				break;
			}
			else if (field_Blockcheck(1, x, y) == 102)//약초마을 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n   약초마을로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				yakcho_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 77)
		{
			printf("\b\b");
			x += 1;
			if (field_Blockcheck(1, x, y) == 100)
				x -= 1;
			else if (field_Blockcheck(1, x, y) == 101)//포션마을 포탈
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n   포션마을로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				potion_village();
				break;
			}
			else if (field_Blockcheck(1, x, y) == 102)//약초마을 포탈
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n   약초마을로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				yakcho_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

	

		}

		/*srand((int)time(NULL));
		temp = rand() % 3;
		if (temp == 1)
		{
			temp = 11 + (rand() % 6);
			battle(temp);
		}*/
	}
}
void desert_field()
{
	system("mode con:cols=125 lines=40");
	int x, y;
	x = 10;
	y = 10;
	int footwalk = 0;
	int startx = 0; int starty = 0;
	int endy = 0; int endx = 0;
	int temp;

	int a = 0;

	BATTLEBACKGROUND(2);

	gotoxy(x, y);
	printf("●");
	while (1)
	{
		a = _getch();

		if (a == 72)
		{
			printf("\b\b  ");
			y -= 1;
			if (field_Blockcheck(1, x, y) == 100)//벽
				y += 1;
			else if (field_Blockcheck(2, x, y) == 103)//약초마을 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n   약초마을로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				yakcho_village();
				break;
			}
			else if (field_Blockcheck(2, x, y) == 104)//홀리시티 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n   홀리시티로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				holy_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 80)
		{
			printf("\b\b  ");
			y += 1;
			if (field_Blockcheck(1, x, y) == 100)
				y -= 1;
			else if (field_Blockcheck(2, x, y) == 103)//약초마을 포탈
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   약초마을로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				yakcho_village();
				break;
			}
			else if (field_Blockcheck(2, x, y) == 104)//홀리시티 포탈
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   홀리시티로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				holy_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 75)
		{
			printf("\b\b");
			x -= 1;
			if (field_Blockcheck(1, x, y) == 100)
				x += 1;
			else if (field_Blockcheck(2, x, y) == 103)//약초마을 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n   약초마을로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				yakcho_village();
				break;
			}
			else if (field_Blockcheck(2, x, y) == 104)//홀리시티 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n   홀리시티로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				holy_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 77)
		{
			printf("\b\b");
			x += 1;
			if (field_Blockcheck(1, x, y) == 100)
				x -= 1;
			else if (field_Blockcheck(2, x, y) == 103)//약초마을 포탈
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n   약초마을로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				yakcho_village();
				break;
			}
			else if (field_Blockcheck(2, x, y) == 104)//홀리시티 포탈
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n   홀리시티로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				holy_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}

		/*srand((int)time(NULL));
		temp = rand() % 3;
		if (temp == 1)
		{
			temp = 21 + (rand() % 8);
			battle(temp);
		}*/
	}
}
void harbor_field()
{
	system("mode con:cols=125 lines=40");
	int x, y;
	x = 10;
	y = 10;
	int footwalk = 0;
	int startx = 0; int starty = 0;
	int endy = 0; int endx = 0;


	int a = 0;

	BATTLEBACKGROUND(3);

	gotoxy(x, y);
	printf("●");
	while (1)
	{
		a = _getch();

		if (a == 72)
		{
			printf("\b\b  ");
			y -= 1;
			if (field_Blockcheck(1, x, y) == 100)//벽
				y += 1;
			else if (field_Blockcheck(3, x, y) == 105)//홀리시티 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n   홀리시티로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				holy_village();
				break;
			}
			else if (field_Blockcheck(3, x, y) == 106)//용사의 고향 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n   용사의 고향으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				home_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 80)
		{
			printf("\b\b  ");
			y += 1;
			if (field_Blockcheck(1, x, y) == 100)
				y -= 1;
			else if (field_Blockcheck(3, x, y) == 105)//홀리시티 포탈
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   홀리시티로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				holy_village();
				break;
			}
			else if (field_Blockcheck(3, x, y) == 106)//용사의 고향 포탈
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   용사의 고향으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				home_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 75)
		{
			printf("\b\b");
			x -= 1;
			if (field_Blockcheck(1, x, y) == 100)
				x += 1;
			else if (field_Blockcheck(3, x, y) == 105)//홀리시티 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n   홀리시티로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				holy_village();
				break;
			}
			else if (field_Blockcheck(3, x, y) == 106)//용사의 고향 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n   용사의 고향으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				home_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 77)
		{
			printf("\b\b");
			x += 1;
			if (field_Blockcheck(1, x, y) == 100)
				x -= 1;
			else if (field_Blockcheck(3, x, y) == 105)//홀리시티 포탈
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n   홀리시티로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				holy_village();
				break;
			}
			else if (field_Blockcheck(3, x, y) == 106)//용사의 고향 포탈
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n   용사의 고향으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				home_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
	}
}
void outback_field()
{
	system("mode con:cols=125 lines=40");
	int x, y;
	x = 10;
	y = 10;
	int footwalk = 0;
	int startx = 0; int starty = 0;
	int endy = 0; int endx = 0;


	int a = 0;

	BATTLEBACKGROUND(4);

	gotoxy(x, y);
	printf("●");
	while (1)
	{
		a = _getch();

		if (a == 72)
		{
			printf("\b\b  ");
			y -= 1;
			if (field_Blockcheck(1, x, y) == 100)//벽
				y += 1;
			else if (field_Blockcheck(4, x, y) == 107)//용사의 고향 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n   용사의 고향으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				home_village();
				break;
			}
			else if (field_Blockcheck(4, x, y) == 108)//전초기지 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n   전초기지로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				res_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 80)
		{
			printf("\b\b  ");
			y += 1;
			if (field_Blockcheck(1, x, y) == 100)
				y -= 1;
			else if (field_Blockcheck(4, x, y) == 107)//용사의 고향 포탈
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   용사의 고향으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				home_village();
				break;
			}
			else if (field_Blockcheck(4, x, y) == 108)//전초기지 포탈
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   전초기지로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				res_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 75)
		{
			printf("\b\b");
			x -= 1;
			if (field_Blockcheck(1, x, y) == 100)
				x += 1;
			else if (field_Blockcheck(4, x, y) == 107)//용사의 고향 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n   용사의 고향으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				home_village();
				break;
			}
			else if (field_Blockcheck(4, x, y) == 108)//전초기지 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n   전초기지로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				res_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 77)
		{
			printf("\b\b");
			x += 1;
			if (field_Blockcheck(1, x, y) == 100)
				x -= 1;
			else if (field_Blockcheck(4, x, y) == 107)//용사의 고향 포탈
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n   용사의 고향으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				home_village();
				break;
			}
			else if (field_Blockcheck(4, x, y) == 108)//전초기지 포탈
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n   전초기지로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				res_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
	}
}
void death_field()
{
	system("mode con:cols=125 lines=40");
	int x, y;
	x = 10;
	y = 10;
	int footwalk = 0;
	int startx = 0; int starty = 0;
	int endy = 0; int endx = 0;


	int a = 0;

	BATTLEBACKGROUND(5);

	gotoxy(x, y);
	printf("●");
	while (1)
	{
		a = _getch();

		if (a == 72)
		{
			printf("\b\b  ");
			y -= 1;
			if (field_Blockcheck(1, x, y) == 100)//벽
				y += 1;
			else if (field_Blockcheck(5, x, y) == 109)//전초기지 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n   레지스탕스 전초기지로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				res_village();
				break;
			}
			else if (field_Blockcheck(5, x, y) == 110)//지배된 왕궁 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n   지배된 왕국으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				kingdom_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 80)
		{
			printf("\b\b  ");
			y += 1;
			if (field_Blockcheck(1, x, y) == 100)
				y -= 1;
			else if (field_Blockcheck(5, x, y) == 109)//전초기지 포탈
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   레지스탕스 전초기지로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				res_village();
				break;
			}
			else if (field_Blockcheck(5, x, y) == 110)//지배된 왕궁 포탈
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   지배된 왕궁으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				kingdom_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 75)
		{
			printf("\b\b");
			x -= 1;
			if (field_Blockcheck(1, x, y) == 100)
				x += 1;
			else if (field_Blockcheck(5, x, y) == 109)//전초기지 포탈
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n   레지스탕스 전초기지로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				res_village();
				break;
			}
			else if (field_Blockcheck(5, x, y) == 110)//지배된 왕궁 포탈
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n   지배된 왕궁으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				kingdom_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 77)
		{
			printf("\b\b");
			x += 1;
			if (field_Blockcheck(1, x, y) == 100)
				x -= 1;
			else if (field_Blockcheck(5, x, y) == 109)//전초기지 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n   레지스탕스 전초기지로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				res_village();
				break;
			}
			else if (field_Blockcheck(5, x, y) == 110)//지배된 왕궁 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n   지배된 왕궁으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				kingdom_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
	}
}
void kingdom_field()
{
	system("mode con:cols=125 lines=40");
	int x, y;
	x = 10;
	y = 10;
	int footwalk = 0;
	int startx = 0; int starty = 0;
	int endy = 0; int endx = 0;


	int a = 0;

	BATTLEBACKGROUND(0);

	gotoxy(x, y);
	printf("●");
	while (1)
	{
		a = _getch();

		if (a == 72)
		{
			printf("\b\b  ");
			y -= 1;
			if (field_Blockcheck(1, x, y) == 100)//벽
				y += 1;
			else if (field_Blockcheck(0, x, y) == 111)//지배된왕국 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n   지배된 왕국으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				kingdom_village();
				break;
			}

			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 80)
		{
			printf("\b\b  ");
			y += 1;
			if (field_Blockcheck(1, x, y) == 100)
				y -= 1;
			else if (field_Blockcheck(0, x, y) == 111)//지배된왕국 포탈
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   지배된 왕국으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				kingdom_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 75)
		{
			printf("\b\b");
			x -= 1;
			if (field_Blockcheck(1, x, y) == 100)
				x += 1;
			else if (field_Blockcheck(0, x, y) == 111)//지배된왕국 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n   지배된 왕국으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				kingdom_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 77)
		{
			printf("\b\b");
			x += 1;
			if (field_Blockcheck(1, x, y) == 100)
				x -= 1;
			else if (field_Blockcheck(0, x, y) == 111)//지배된왕국 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n   지배된 왕국으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				kingdom_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
	}
}


int Potion_Blockcheck(int x, int y)
{
	int i = 0;
	char potionBlock[25][80] = { 0 };

	for (i = 0; i <= 12; i++)
		potionBlock[i][10] = 44;//맨 위 왼쪽 벽
	for (i = 10; i <= 28; i++)
		potionBlock[11][i] = 44;//공원 위쪽 벽
	for (i = 0; i <= 7; i++)
		potionBlock[i][16] = 44;//맨 위 오른쪽 벽
	for (i = 12; i <= 23; i++)
		potionBlock[i][28] = 44;// 공원 오른쪽 벽
	for (i = 17; i <= 75; i++)
		potionBlock[8][i] = 44;//무기상점 아래쪽 벽 전부

	for (i = 13; i <= 17; i++)
		potionBlock[i][35] = 44;//여관 왼쪽 벽
	for (i = 36; i <= 75; i++)
		potionBlock[17][i] = 44;//여관 아래쪽 벽 전부
	for (i = 35; i <= 75; i++)
		potionBlock[12][i] = 44;//여관 위쪽 벽 전부
	for (i = 35; i <= 75; i++)
		potionBlock[20][i] = 44;//마을주민 위쪽 벽 전부
	for (i = 1; i <= 25; i++)
		potionBlock[i][73] = 44;//마을 오른쪽 벽 외곽
	for (i = 10; i <= 15; i++)
		potionBlock[0][i] = 44;
	for (i = 29; i <= 35; i++)
		potionBlock[22][i] = 43;//들판 포탈


	potionBlock[8][46] = 32;//소모아이템 상점 포탈
	potionBlock[8][24] = 33;//무기 상점 포탈
	potionBlock[8][68] = 34;//옷상점 포탈
	potionBlock[8][34] = 35;//보석 상점 포탈
	potionBlock[17][66] = 36;//마을주민 포탈(이장집 옆)
	potionBlock[8][59] = 36;//주민 포탈
	potionBlock[20][41] = 37;//마을주민 포탈(1)
	potionBlock[20][54] = 38;//마을주민 포탈(2)
	potionBlock[20][66] = 39;//마을주민 포탈(3)
	potionBlock[17][42] = 40;//여관 포탈
	potionBlock[17][54] = 41;//이장의 포탈
	potionBlock[12][28] = 44;//공원 모서리
	potionBlock[8][16] = 44;//무기 상점 모서리




							//소모아이템 상점 32
							//무기상점 33
							//방어구 상점 34
							//보석 상점 35
							//마을 주민 36,37,38,39
							//여관 40
							//이장 41
							//마을1(상) 42
							//마을2(하) 43
							//벽   44




	if (potionBlock[y][x] == 32)
		return 32;
	else if (potionBlock[y][x] == 33)
		return 33;
	else if (potionBlock[y][x] == 34)
		return 34;
	else if (potionBlock[y][x] == 35)
		return 35;
	else if (potionBlock[y][x] == 36)
		return 36;
	else if (potionBlock[y][x] == 37)
		return 37;
	else if (potionBlock[y][x] == 38)
		return 38;
	else if (potionBlock[y][x] == 39)
		return 39;
	else if (potionBlock[y][x] == 40)
		return 40;
	else if (potionBlock[y][x] == 41)
		return 41;
	else if (potionBlock[y][x] == 42)
		return 42;
	else if (potionBlock[y][x] == 43)
		return 43;
	else if (potionBlock[y][x] == 44)
		return 44;




}
void drawmap_potion()
{
	system("mode con:cols=80 lines=26");
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf("■       │      │                                                       ■\n");
	printf("■       │      │            ┌──-──┐                              ■\n");
	printf("■       │      │  ◎ = 입구 │포션 마을│                              ■\n");
	printf("■       │      │            └──-──┘                              ■\n");
	printf("■       │      │ ┌───┐┌───┐┌─────┐ ┌───┐┌──┐ ■\n");
	printf("■       │      │ │무  기││보  석││소모아이템│ │ 주민 ││갑옷│ ■\n");
	printf("■       │      │ │      ││      ││상      점│ │      ││상점│ ■\n");
	printf("■       │      └-┴─◎─┴┴─◎─┴┴──◎──┴-┴─◎─┴┴-◎-┴-■\n");
	printf("■       │                                                               ■\n");
	printf("■       │                                                               ■\n");
	printf("■       └────────┐                                             ■\n");
	printf("■┌───────────-┤       ┌─────┬─────┬─────┐■\n");
	printf("■│    공          원     │       │          │          │          │■\n");
	printf("■│           ￥          │       │  여  관  │ 이장의집 │마을 주민 │■\n");
	printf("■│  ￥                   │       │   Save   │          │          │■\n");
	printf("■│         ￥      ￥    │       │          │          │          │■\n");
	printf("■│                       │       └──◎──┴──◎──┴──◎──┘■\n");
	printf("■│     ￥    ￥   ￥     │                                             ■\n");
	printf("■│       ￥              │                                             ■\n");
	printf("■│   ￥         ￥       │       ┌─-◎-─┬┬──◎-─┬┬─-◎-─┬-■\n");
	printf("■│                   ￥  │       │마을주민││마을주민 ││마을주민│ ■\n");
	printf("■│           ￥          │:::::::├────┘└────-┘└────┘ ■\n");
	printf("■└───────────-┤   ◎  │▽들판                              ■\n");
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
}
void potion_village()
{
	int x, y;
	x = 30; y = 10;
	int footwalk = 0;
	int startx = 0; int starty = 0;
	int endy = 0; int endx = 0;


	int a = 0;
	int g, k;
	load();
	drawmap_potion();

	gotoxy(x, y);
	printf("●");
	while (1)
	{
		a = _getch();

		if (a == 72)
		{
			printf("\b\b  ");
			y -= 1;
			if (Potion_Blockcheck(x, y) == 44)//벽
				y += 1;
			else if (Potion_Blockcheck(x, y) == 32)//잡화상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n  잡화 상점으로 이동하셨습니다! \n\n");
				shop(1, 4); //기타   상점 입장
				Sleep(1000);
				system("cls");
				drawmap_potion();
			}
			else if (Potion_Blockcheck(x, y) == 33)//무기상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n  무기 상점으로 이동하셨습니다! \n\n");
				shop(1, 1);
				Sleep(1000);
				system("cls");
				drawmap_potion();
			}
			else if (Potion_Blockcheck(x, y) == 34)//방어구상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n  방어구 상점으로 이동하셨습니다! \n\n");
				shop(1, 2); //방어구 상점 입장
				Sleep(1000);
				system("cls");
				drawmap_potion();
			}
			else if (Potion_Blockcheck(x, y) == 35)//보석상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n  보석 상점으로 이동하셧습니다! \n\n");
				shop(1, 3); //보석   상점 입장
				Sleep(1000);
				system("cls");
				drawmap_potion();
			}
			else if (Potion_Blockcheck(x, y) == 36)//마을 주민
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n  야생의마을주민이 나타났다! \n\n");
				if (achieve == 1)
					questfunc(1, 0);
				if (achieve == 2)
					questfunc(1, 0);
				Sleep(1000);
				system("cls");
				drawmap_potion();
			}
			else if (Potion_Blockcheck(x, y) == 40)//세이브
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n  진행상황을 저장합니다 \n\n");
				save(); // 세이브 함수 호출
				Sleep(1000);
				system("cls");
				drawmap_potion();
			}
			else if (Potion_Blockcheck(x, y) == 41)//이장
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n  이장 \n\n");
				if (achieve == 0)
					questfunc(1, 0);
				if (achieve == 3 && succeed == -1)
					questfunc(1, 0);
				Sleep(1000);
				system("cls");
				drawmap_potion();
			}

			gotoxy(x, y);
			printf("●");
			footwalk++;


		}
		else if (a == 75)
		{
			printf("\b\b");
			x -= 1;
			if (Potion_Blockcheck(x, y) == 44)
				x += 1;
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}

		else if (a == 77)
		{
			printf("\b\b");
			x += 1;
			if (Potion_Blockcheck(x, y) == 44)
				x -= 1;
			else if (Potion_Blockcheck(x, y) == 33)
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n 잡화 상점으로 이동하셨습니다! \n\n");
				shop(1, 4); //기타   상점 입장
				Sleep(1000);
				system("cls");
				drawmap_potion();
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;
		}
		else if (a == 80)
		{
			printf("\b\b  ");
			y += 1;
			if (Potion_Blockcheck(x, y) == 44)
				y -= 1;

			else if (Potion_Blockcheck(x, y) == 37)//주민
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n  주민! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_potion();;
			}
			else if (Potion_Blockcheck(x, y) == 38)//주민
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n  주민! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_potion();;
			}
			else if (Potion_Blockcheck(x, y) == 39)//주민
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   주민! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_potion();
			}
			else if (Potion_Blockcheck(x, y) == 43)//포탈(하)
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   뒷산으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				field_field();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;
		}
		else if (a == 27)
			break;

	}
}

int yakcho_Blockcheck(int x, int y)
{
	int i = 0;
	char yakchoBlock[25][75] = { 0 };

	for (i = 0; i <= 8; i++)
	{
		yakchoBlock[i][8] = 22; yakchoBlock[i][14] = 22; //입구 양쪽 벽 포탈 전까지
	}
	for (i = 8; i <= 12; i++)
		yakchoBlock[15][i] = 22; // 마을 회관 위쪽벽
	for (i = 17; i <= 19; i++)
		yakchoBlock[15][i] = 22; // 마을 회관 위쪽벽
	for (i = 16; i <= 24; i++)
		yakchoBlock[i][19] = 22;// 마을 회관 왼쪽벽
	for (i = 10; i <= 12; i++)
		yakchoBlock[i][8] = 22;// 마을주민4 음식점 사이 벽

	for (i = 15; i <= 74; i++)
		yakchoBlock[11][i] = 22;// 마을 길 위쪽 벽
	for (i = 25; i <= 74; i++)
		yakchoBlock[15][i] = 22;// 마을 길 아래쪽 벽
	for (i = 16; i <= 24; i++)
		yakchoBlock[i][25] = 22;// 주점 왼쪽 벽 전부
	for (i = 13; i <= 16; i++)
		yakchoBlock[15][i] = 23;// 마을회관 포탈
	for (i = 9; i <= 15; i++)
		yakchoBlock[0][i] = 22;// 마을위쪽 벽
	for (i = 20; i <= 25; i++)
		yakchoBlock[22][i] = 25;//마을 아래포탈
	for (i = 12; i <= 15; i++)
		yakchoBlock[i][70] = 26;// 마을 오른쪽  포탈




	yakchoBlock[13][8] = 12;//음식점 포탈
	yakchoBlock[11][53] = 12;// 잡상인 포탈
	yakchoBlock[11][41] = 13;// 무기 상점 포탈
	yakchoBlock[11][27] = 14;// 방어구 상점 포탈
	yakchoBlock[11][64] = 15;// 보석상 포탈

	yakchoBlock[15][44] = 16;//마을주민 1포탈
	yakchoBlock[15][56] = 17;//마을 주민 2 포탈
	yakchoBlock[15][68] = 18;//마을 주민 3 포탈
	yakchoBlock[9][8] = 19;// 마을 주민4 포탈
	yakchoBlock[9][14] = 20;// 마을 주민 5 포탈
	yakchoBlock[15][32] = 21;//주점 포탈

	yakchoBlock[10][14] = 22;// 마을 주민 5 포탈 벽
	yakchoBlock[11][14] = 22;// 마을 주민 5 모서리 벽
	yakchoBlock[14][8] = 22;//음식점 포탈 벽


							//잡상인,음식점 12
							//무기상점 13
							//방어구 상점 14
							//보석 상점 15
							//마을 주민 16,17,18,19,20
							//주점 21(SAVE)
							//벽 22
							//마을회관 23
							//마을포탈(상) 24
							//마을포탈(하) 25
							//마을포탈(우) 26






	if (yakchoBlock[y][x] == 12)
		return 12;
	else if (yakchoBlock[y][x] == 13)
		return 13;
	else if (yakchoBlock[y][x] == 14)
		return 14;
	else if (yakchoBlock[y][x] == 15)
		return 15;
	else if (yakchoBlock[y][x] == 16)
		return 16;
	else if (yakchoBlock[y][x] == 17)
		return 17;
	else if (yakchoBlock[y][x] == 18)
		return 18;
	else if (yakchoBlock[y][x] == 19)
		return 19;
	else if (yakchoBlock[y][x] == 20)
		return 20;
	else if (yakchoBlock[y][x] == 21)
		return 21;
	else if (yakchoBlock[y][x] == 22)
		return 22;
	else if (yakchoBlock[y][x] == 23)
		return 23;
	else if (yakchoBlock[y][x] == 25)
		return 25;

	else if (yakchoBlock[y][x] == 26)
		return 26;


}
void drawmap_yakcho()
{
	system("mode con:cols=80 lines=26");
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf("■     │      │                                                         ■\n");
	printf("■     │      │            ┌──────┐                             ■\n");
	printf("■     │      │            │약초   마을 │                             ■\n");
	printf("■     │      │            └──────┘                             ■\n");
	printf("■     │      ├──┐                                                   ■\n");
	printf("■     │      │  마│                                             사막▷■\n");
	printf("■     │      │  을│                                                   ■\n");
	printf("■──-┤      │  주├─────┐┌─────┐┌───┐ ┌───┐    ■\n");
	printf("■마을 ◎      ◎  민│방어구    ││무기      ││잡상인│ │보석상│    ■\n");
	printf("■주민4│      │   5│      상인││      상인││      │ │      │    ■\n");
	printf("■─-─┤      └──┴──◎──┴┴──◎──┴┴─◎─┴-┴─◎─┴──■\n");
	printf("■음   │                                                              :  ■\n");
	printf("■  식 ◎                                                              :◎■\n");
	printf("■   점│                                                              :  ■\n");
	printf("■──-┴-──◎◎┐      ┌──◎─┬┬──◎──┬┬─◎──┬┬─◎─┐■\n");
	printf("■                │      │주    점││          ││마을    ││마을  │■\n");
	printf("■  마        을  │      │  Save  ││ 마을주민1││   주민2││주민3 │■\n");
	printf("■                │      ├────┘└─────┘└────┘└───┘■\n");
	printf("■                │      │                                              ■\n");
	printf("■                │      │                                              ■\n");
	printf("■  회        관  │      │     ◎ = 입구                                ■\n");
	printf("■                │::::::│                                              ■\n");
	printf("■                │  ◎  │▽뒷산                                        ■\n");
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
}
void yakcho_village()
{
	int x, y;
	x = 10; y = 10;
	int footwalk = 0;
	int startx = 0; int starty = 0;
	int endy = 0; int endx = 0;

	if (achieve == 3 && succeed == 0)
		questfunc(3, 0);

	int a = 0;

	drawmap_yakcho();

	gotoxy(x, y);
	printf("●");
	while (1)
	{
		a = _getch();

		if (a == 72)
		{
			printf("\b\b  ");
			y -= 1;
			if (yakcho_Blockcheck(x, y) == 22)//벽
				y += 1;
			else if (yakcho_Blockcheck(x, y) == 12)//잡화상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 잡화상점으로 이동하셨습니다! \n\n");
				shop(2, 4); //기타   상점 입장
				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}
			else if (yakcho_Blockcheck(x, y) == 13)//무기상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 무기상점으로 이동하셨습니다! \n\n");
				shop(2, 1); //무기   상점 입장
				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}
			else if (yakcho_Blockcheck(x, y) == 14)//방어구상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 방어구상점으로 이동하셨습니다! \n\n");
				shop(2, 2); //방어구 상점 입장
				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}
			else if (yakcho_Blockcheck(x, y) == 15)//보석상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 보석상점으로 이동하셨습니다! \n\n");
				shop(2, 3); //보석   상점 입장
				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}
			else if (yakcho_Blockcheck(x, y) == 24)
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n   포션마을로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				potion_village();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;


		}
		else if (a == 80)
		{
			printf("\b\b  ");
			y += 1;

			if (yakcho_Blockcheck(x, y) == 22)
				y -= 1;

			else if (yakcho_Blockcheck(x, y) == 11)
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}

			else if (yakcho_Blockcheck(x, y) == 23)//마을회관
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 마을회관 \n\n");

				if (achieve == 3 && succeed == 1)
					questfunc(1, 0);
				if (achieve == 4)
					questfunc(1, 0);
				if (achieve == 5)
					questfunc(1, 0);
				if (achieve == 6)
					questfunc(1, 0);
				if (achieve == 7 && succeed == 1)
					questfunc(1, 0);
				if (achieve == 8 && succeed == -1)
					questfunc(1, 0);

				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}

			else if (yakcho_Blockcheck(x, y) == 16)//주민
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 좋은아침! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}

			else if (yakcho_Blockcheck(x, y) == 17)//주민
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 하알ㄺ \n\n");
				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}

			else if (yakcho_Blockcheck(x, y) == 18)//주민
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 핡 \n\n");
				
				if (achieve == 6)
				{
					questfunc(3, 0);
					questfunc(1, 0);
				}
				if (achieve == 7 && succeed == -1)
					questfunc(1, 0);

				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}
			else if (yakcho_Blockcheck(x, y) == 21)//세이브
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 진행상황을 저장하였습니다 \n\n");
				save(); //세이브 함수 호출
				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}
			else if (yakcho_Blockcheck(x, y) == 25)//포탈(하)
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 뒷산으로 이동합니다 \n\n");
				Sleep(1000);
				system("cls");
				field_field();
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 75)
		{
			printf("\b\b");
			x -= 1;
			if (yakcho_Blockcheck(x, y) == 22)
				x += 1;
			else if (yakcho_Blockcheck(x, y) == 11)
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}
			else if (yakcho_Blockcheck(x, y) == 19)//주민
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n 하아아아아아잇 \n\n");
				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}
			else if (yakcho_Blockcheck(x, y) == 12)//잡화상점
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n 잡화상점으로 이동하셨습니다! \n\n");
				shop(1, 4); //기타   상점 입장
				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 77)
		{
			printf("\b\b");
			x += 1;
			if (yakcho_Blockcheck(x, y) == 22)
				x -= 1;
			else if (yakcho_Blockcheck(x, y) == 11)
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}
			else if (yakcho_Blockcheck(x, y) == 20)//주민
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n 좋은아침이네요 \n\n");
				Sleep(1000);
				system("cls");
				drawmap_yakcho();
			}
			else if (yakcho_Blockcheck(x, y) == 26)//포탈(우)
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n 사막으로 이동합니다!\n\n");
				Sleep(1000);
				system("cls");
				desert_field();
				x = 24;
				y = 24;
				break;
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 27)
			break;

	}
}

int Holy_Blockcheck(int x, int y)
{
	int i = 0;
	char holyBlock[25][80] = { 0 };

	for (i = 31; i <= 43; i++)
		holyBlock[16][i] = 61; // 교회가는 포탈
	for (i = 12; i <= 14; i++)
		holyBlock[i][4] = 62; // 사막 가는 포탈
	for (i = 11; i <= 16; i++)
		holyBlock[2][i] = 63; // 항구가는 포탈
	for (i = 12; i <= 14; i++)
		holyBlock[i][71] = 66; // 늪 가는 포탈
	for (i = 0; i <= 9; i++)
		holyBlock[11][i] = 66;// 약초마을 가는 포탈 위쪽 벽
	for (i = 0; i <= 11; i++)
		holyBlock[i][10] = 66;// 포션마을방향 통로 벽 왼쪽
	for (i = 0; i <= 11; i++)
		holyBlock[i][16] = 66;// 포션마을방향 통로 벽 오른쪽
	for (i = 0; i <= 29; i++)
		holyBlock[15][i] = 66;// 옷 무기 상점부분 벽
	for (i = 17; i <= 76; i++)
		holyBlock[11][i] = 66;// 윗쪽 상점부분 벽
	for (i = 43; i <= 76; i++)
		holyBlock[15][i] = 66;// 이장 마을주민 윗 벽
	for (i = 28; i <= 30; i++)
		holyBlock[15][i] = 66;// 잡상인 모서리 벽


	holyBlock[15][27] = 52; //잡템 상점 포탈
	holyBlock[15][15] = 53; //무기 상점 포탈
	holyBlock[15][6] = 54; // 옷 상점 포탈
	holyBlock[11][29] = 55; // 보석 상점 포탈
	holyBlock[15][62] = 56; // 주민1 포탈
	holyBlock[11][55] = 57; // 주민2 포탈
	holyBlock[11][66] = 58; // 주민3 포탈
	holyBlock[11][43] = 60; // 여관 세이브 포탈
	holyBlock[15][50] = 61; // 이장 포탈



							//소모아이템 상점 52
							//무기상점 53
							//방어구 상점 54
							//보석 상점 55
							//마을 주민 56,57,58
							//여관 60
							//이장,교회 61
							// 포탈(약초) 62
							// 포탈(포션) 63
							// 포탈(늪) 64
							//벽   66

	if (holyBlock[y][x] == 52)
		return 52;
	else if (holyBlock[y][x] == 53)
		return 53;
	else if (holyBlock[y][x] == 53)
		return 54;
	else if (holyBlock[y][x] == 55)
		return 55;
	else if (holyBlock[y][x] == 56)
		return 56;
	else if (holyBlock[y][x] == 57)
		return 57;
	else if (holyBlock[y][x] == 58)
		return 58;
	else if (holyBlock[y][x] == 60)
		return 60;
	else if (holyBlock[y][x] == 61)
		return 61;
	else if (holyBlock[y][x] == 62)
		return 62;
	else if (holyBlock[y][x] == 63)
		return 63;
	else if (holyBlock[y][x] == 64)
		return 64;
	else if (holyBlock[y][x] == 66)
		return 66;

}
void drawmap_holy()
{
	system("mode con:cols=80 lines=26");
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf("■       │  ◎  │△항구                                                 ■\n");
	printf("■       │::::::│              ┌──-──┐                            ■\n");
	printf("■       │      │              │홀리 시티│                            ■\n");
	printf("■       │      │              └──-──┘                            ■\n");
	printf("■       │      │                                                       ■\n");
	printf("■       │      │                                                       ■\n");
	printf("■       │      │                                                       ■\n");
	printf("■       │      │    ┌─────┐┌─────┐┌───┐ ┌───┐  ■\n");
	printf("■       │      │    │  보   석 ││  여  관  ││ 주민 │ │ 주민 │  ■\n");
	printf("■ ◁사막│      │    │          ││   Save   ││  NPC │ │ NPC  │  ■\n");
	printf("■─-──┘      └──┴──◎──┴┴──◎──┴┴─◎─┴-┴─◎─┴─■\n");
	printf("■  :                                                                     ■\n");
	printf("■◎:                                                                     ■\n");
	printf("■  :                                                                     ■\n");
	printf("■─┬◎-┬─┬◎─┬─┬─◎┐             ┌──◎─┬┬──◎──┬──■\n");
	printf("■  │옷 │  │무기│  │잡템│:::::::::::::│ 이  장 ││          │ ▷ ■\n");
	printf("■  │   │  │    │  │    ├──-◎───┤        ││ 마을주민1│ 늪 ■\n");
	printf("■  └-─┘  └──┘  └──┤    ┌┐     ├────┘└─────┘    ■\n");
	printf("■                           │ ┌-┘└-┐  │                            ■\n");
	printf("■                           │ └-┐┌-┘  │                            ■\n");
	printf("■                           │    ││     │     ◎ = 입구              ■\n");
	printf("■                           │    └┘     │                            ■\n");
	printf("■                           │    교회     │                            ■\n");
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
}
void holy_village()
{
	int x, y;
	x = 30; y = 13;
	int footwalk = 0;
	int startx = 0; int starty = 0;
	int endy = 0; int endx = 0;

	if (achieve == 8 && succeed == 0)
		questfunc(3, 0);

	int a = 0;
	int g, k;

	drawmap_holy();


	gotoxy(x, y);
	printf("●");
	while (1)
	{
		a = _getch();

		if (a == 72)
		{
			printf("\b\b  ");
			y -= 1;

			if (Holy_Blockcheck(x, y) == 66)//벽
				y += 1;

			else if (Holy_Blockcheck(x, y) == 55)//보석상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 보석상점으로 이동하셨습니다! \n\n");
				shop(3, 3); //보석   상점 입장

				if (achieve == 9 && succeed == 0)
					questfunc(3, 0);

				Sleep(1000);
				system("cls");
				drawmap_holy();
			}

			else if (Holy_Blockcheck(x, y) == 57)//주민
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n  주민! \n\n");
				system("pause");
				Sleep(1000);
				system("cls");
				drawmap_holy();
			}

			else if (Holy_Blockcheck(x, y) == 58)//주민
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 주민! \n\n");
				system("pause");
				Sleep(1000);
				system("cls");
				drawmap_holy();
			}

			else if (Holy_Blockcheck(x, y) == 60)//세이브
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 진행상황을 저장합니다! \n\n");
				save(); //세이브 함수 호출
				Sleep(1000);
				system("cls");
				drawmap_holy();
			}

			else if (Holy_Blockcheck(x, y) == 63)//포탈 (항구)
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 항구로 이동합니다! \n\n");
				Sleep(1000);
				system("cls");
				harbor_field();
				break;
			}

			gotoxy(x, y);
			printf("●");
			footwalk++;


		}

		else if (a == 80)
		{
			printf("\b\b  ");
			y += 1;
			if (Holy_Blockcheck(x, y) == 66)
				y -= 1;

			else if (Holy_Blockcheck(x, y) == 52)// 잡화상점
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 잡화상점으로 이동하셨습니다! \n\n");
				shop(3, 4); //기타   상점 입장

				if (achieve == 9 && succeed == 0)
					questfunc(3, 0);

				Sleep(1000);
				system("cls");
				drawmap_holy();
			}

			else if (Holy_Blockcheck(x, y) == 53)//무기상점
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 무기상점으로 이동하셨습니다! \n\n");
				shop(3, 1); //무기   상점 입장

				if (achieve == 9 && succeed == 0)
					questfunc(3, 0);

				Sleep(1000);
				system("cls");
				drawmap_holy();
			}

			else if (Holy_Blockcheck(x, y) == 54)//방어구상점
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 방어구상점으로 이동하셨습니다! \n\n");
				shop(3, 2); //방어구 상점 입장

				if (achieve == 9 && succeed == 0)
					questfunc(3, 0);

				Sleep(1000);
				system("cls");
				drawmap_holy();
			}

			else if (Holy_Blockcheck(x, y) == 56)//주민
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n  주민! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_holy();
			}

			else if (Holy_Blockcheck(x, y) == 61)//이장 교회
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n  이장 교회! \n\n");

				if (achieve == 8 && succeed == 1)
					questfunc(1, 0);
				if (achieve == 9)
					questfunc(1, 0);
				if (achieve == 10)
					quest_pirate();
				if (achieve == 13 && succeed == -1)
					questfunc(1, 0);
					
				Sleep(1000);
				system("cls");
				drawmap_holy();
			}

			gotoxy(x, y);
			printf("●");
			footwalk++;

		}

		else if (a == 75)
		{
			printf("\b\b");
			x -= 1;

			if (Holy_Blockcheck(x, y) == 66)
				x += 1;

			else if (Holy_Blockcheck(x, y) == 62)//포탈 (약초)
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n 사막으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				desert_field();
				break;
			}

			gotoxy(x, y);
			printf("●");
			footwalk++;

		}

		else if (a == 77)
		{
			printf("\b\b");
			x += 1;

			if (Holy_Blockcheck(x, y) == 66)
				x -= 1;

			else if (Holy_Blockcheck(x, y) == 64)//포탈 우
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n 준비중이라 이동하실수 없습니다! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_holy();
			}

			gotoxy(x, y);
			printf("●");
			footwalk++;

		}
		else if (a == 27)
			break;

	}
}

int home_Blockcheck(int x, int y)
{
	int i = 0;
	char homeBlock[24][96] = { 0 };
	for (i = 3; i <= 45; i++)
		homeBlock[7][i] = 88;//보석 상점 아래 벽
	for (i = 3; i <= 96; i++)
		homeBlock[13][i] = 88;//마을주민 윗벽
	for (i = 0; i <= 7; i++)
		homeBlock[i][45] = 88;//보석 상점 옆 벽
	for (i = 0; i <= 7; i++)
		homeBlock[i][53] = 88;//포션 상점 옆 벽
	for (i = 53; i <= 96; i++)
		homeBlock[7][i] = 88;//포션 상점 옆 벽
	for (i = 8; i <= 12; i++)
		homeBlock[i][93] = 88;//맵 오른쪽 벽
	for (i = 46; i <= 52; i++)
		homeBlock[2][i] = 79;//황야 포탈
	for (i = 8; i <= 12; i++)
		homeBlock[i][4] = 80;//항구 포탈


	homeBlock[7][14] = 70;//무기상점 포탈
	homeBlock[7][24] = 71;//방어구 상점 포탈
	homeBlock[7][38] = 72;//보석상점 포탈
	homeBlock[7][62] = 73;//잡화상점 포탈
	homeBlock[7][82] = 74;//세이브 포탈
	homeBlock[13][18] = 75;//주민 왼 포탈
	homeBlock[13][38] = 76;//주민 중 포탈
	homeBlock[13][54] = 77;//마을 이장님 포탈
	homeBlock[13][80] = 78;//주민 우 포탈





	if (homeBlock[y][x] == 88)
		return 88;
	else if (homeBlock[y][x] == 70)
		return 70;
	else if (homeBlock[y][x] == 71)
		return 71;
	else if (homeBlock[y][x] == 72)
		return 72;
	else if (homeBlock[y][x] == 73)
		return 73;
	else if (homeBlock[y][x] == 74)
		return 74;
	else if (homeBlock[y][x] == 75)
		return 75;
	else if (homeBlock[y][x] == 76)
		return 76;
	else if (homeBlock[y][x] == 77)
		return 77;
	else if (homeBlock[y][x] == 78)
		return 78;
	else if (homeBlock[y][x] == 79)
		return 79;
	else if (homeBlock[y][x] == 80)
		return 80;
}
void drawmap_home()
{
	system("mode con:cols=110 lines=30");
	printf("┌─────────────────────┓        ┏───────────────────┐\n");
	printf("│   용사의 고향                   ￥       ┃   ◎   ┃△황야           ♧                   │\n");
	printf("│                    ￥                    ┃::::::::┃              ￥       ￥       ♧    │\n");
	printf("│        ┏━━━┓┏━━━━┓┏━━━━━┫        ┣━━━━━━┓    ┏━━━━━━┓  ￥│\n");
	printf("│ ♧     ┃ 무기 ┃┃ 방어구 ┃┃   보석   ┃        ┃    포션    ┃  ♧┃  여    관  ┃    │\n");
	printf("│        ┃      ┃┃        ┃┃          ┃        ┃            ┃    ┃    SAVE    ┃ ♧ │\n");
	printf("│    ￥  ┃상  점┃┃상    점┃┃ 상    점 ┃        ┃  상    점  ┃ ￥ ┃            ┃    │\n");
	printf("├────┗━◎━┛┗━◎━━┛┗━━◎━━┛        ┗━━━◎━━┛──┗━━━◎━━┛──┤\n");
	printf("│  :                                                                                         │\n");
	printf("│  :                                                                                         │\n");
	printf("│◎:                                                                                         │\n");
	printf("│  :                                                                                         │\n");
	printf("│  :                                                                                         │\n");
	printf("├───┏━━━━◎━┓──┏━━━━◎━┓─┏━━━◎━━━┓───┏━━━━◎━┓────┤\n");
	printf("│◁항구┃            ┃ ￥ ┃            ┃  ┃              ┃      ┃            ┃    ￥  │\n");
	printf("│      ┃마을주민    ┃    ┃            ┃  ┃  마을 이장님 ┃      ┃  마을주민  ┃  ￥    │\n");
	printf("│      ┃            ┃    ┃ 마을주민   ┃￥┃              ┃      ┃            ┃      ￥│\n");
	printf("│      ┗━━━━━━┛    ┗━━━━━━┛  ┗━━━━━━━┛  ￥  ┗━━━━━━┛        │\n");
	printf("│                                                                                            │\n");
	printf("│           ￥       ♧                                                     ♧               │\n");
	printf("│                             ￥                      ♧                                     │\n");
	printf("│                                   ♧      ￥              ￥        ♧         ￥          │\n");
	printf("│                                                                                            │\n");
	printf("└──────────────────────────────────────────────┘\n");

}
void home_village()
{
	int x, y;
	x = 30; y = 10;
	int footwalk = 0;
	int startx = 0; int starty = 0;
	int endy = 0; int endx = 0;

	if (achieve == 13 && succeed == 0)
		questfunc(3, 0);

	int a = 0;
	int g, k;
	load();
	drawmap_home();

	gotoxy(x, y);
	printf("●");
	while (1)
	{
		a = _getch();

		if (a == 72)
		{
			printf("\b\b  ");
			y -= 1;
			if (home_Blockcheck(x, y) == 88)//벽
				y += 1;
			else if (home_Blockcheck(x, y) == 70)//무기상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 무기상점으로 이동하셨습니다! \n\n");
				shop(1, 1); //무기   상점 입장

				if (achieve == 14 && succeed == 0)
					questfunc(3, 0);

				Sleep(1000);
				system("cls");
				drawmap_home();
			}
			else if (home_Blockcheck(x, y) == 71)//방어구상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 방어구상점으로 이동하셨습니다! \n\n");
				shop(1, 2); //방어구   상점 입장

				if (achieve == 14 && succeed == 0)
					questfunc(3, 0);

				Sleep(1000);
				system("cls");
				drawmap_home();
			}
			else if (home_Blockcheck(x, y) == 72)//보석상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 보석상점으로 이동하셨습니다! \n\n");
				shop(1, 3); //보석 상점 입장

				if (achieve == 14 && succeed == 0)
					questfunc(3, 0);

				Sleep(1000);
				system("cls");
				drawmap_home();
			}
			else if (home_Blockcheck(x, y) == 73)//잡화상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 잡화상점으로 이동하셨습니다! \n\n");
				shop(1, 4); //잡화 상점 입장

				if (achieve == 14 && succeed == 0)
					questfunc(3, 0);

				Sleep(1000);
				system("cls");
				drawmap_home();
			}
			else if (home_Blockcheck(x, y) == 74)//세이브
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 진행상황을 저장합니다! \n\n");
				save(); //세이브 함수 호출
				Sleep(1000);
				system("cls");
				drawmap_home();
			}
			else if (home_Blockcheck(x, y) == 79)//황야 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 황야로 이동합니다! \n\n");
				outback_field();
				Sleep(1000);
				system("cls");
				drawmap_home();
			}




			gotoxy(x, y);
			printf("●");
			footwalk++;


		}
		else if (a == 75)
		{
			printf("\b\b");
			x -= 1;
			if (home_Blockcheck(x, y) == 88)
				x += 1;
			else if (home_Blockcheck(x, y) == 80)
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n 항구로 이동하셨습니다! \n\n");
				//항구 사냥터 함수
				Sleep(1000);
				system("cls");
				drawmap_home();
			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}

		else if (a == 77)
		{
			printf("\b\b");
			x += 1;
			if (home_Blockcheck(x, y) == 88)
				x -= 1;

			gotoxy(x, y);
			printf("●");
			footwalk++;
		}
		else if (a == 80)
		{
			printf("\b\b  ");
			y += 1;
			if (home_Blockcheck(x, y) == 88)
				y -= 1;

			else if (home_Blockcheck(x, y) == 75)//주민왼
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n  주민! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_home();;
			}
			else if (home_Blockcheck(x, y) == 76)//주민중
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n  주민! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_home();;
			}
			else if (home_Blockcheck(x, y) == 78)//주민우
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   주민! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_home();
			}
			else if (home_Blockcheck(x, y) == 77)//마을 이장
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n   이장! \n\n");

				if (achieve == 13 && succeed == 1)
					questfunc(1, 0);
				if (achieve == 14)
					questfunc(1, 0);
				if (achieve == 15)
					questfunc(1, 0);
				if (achieve == 16)
					questfunc(1, 0);
				if (achieve == 17)
					questfunc(1, 0);
				if (achieve == 18)
					questfunc(1, 0);
				if (achieve == 19 && succeed == -1)
					questfunc(1, 0);

				Sleep(1000);
				system("cls");
				drawmap_home();

			}
			gotoxy(x, y);
			printf("●");
			footwalk++;
		}
		else if (a == 27)
			break;

	}
}

int res_Blockcheck(int x, int y)
{
	int i = 0;
	char resBlock[28][80] = { 0 };

	for (i = 0; i <= 15; i++)
		resBlock[i][24] = 100;//위쪽 왼벽
	for (i = 24; i <= 48; i++)
		resBlock[15][i] = 100;//막사쪽 길 윗벽
	for (i = 16; i <= 19; i++)
		resBlock[i][48] = 100;//막사쪽 길 오른쪽벽
	for (i = 24; i <= 47; i++)
		resBlock[19][i] = 100;//막사쪽 길 아랫벽
	for (i = 20; i <= 26; i++)
		resBlock[i][24] = 100;//막사왼벽
	for (i = 1; i <= 26; i++)
		resBlock[i][15] = 100;//상점가 벽
	for (i = 16; i <= 25; i++)
		resBlock[2][i] = 101;//죽음의협곡 포탈
	for (i = 16; i <= 25; i++)
		resBlock[24][i] = 102;//황야 포탈

	resBlock[4][15] = 103;//무기상점
	resBlock[8][15] = 104;//보석상점
	resBlock[12][15] = 105;//방어구 상점
	resBlock[16][15] = 106;//잡화 상점
	resBlock[20][15] = 107;//저스틴(퀘스트)
	resBlock[19][37] = 108;//세이브
	resBlock[17][48] = 109;//사령관(퀘스트)


	if (resBlock[y][x] == 100)
		return 100;//벽
	else if (resBlock[y][x] == 101)
		return 101;//죽음의협곡 포탈
	else if (resBlock[y][x] == 102)
		return 102;//황야 포탈
	else if (resBlock[y][x] == 103)
		return 103;//무기상점 포탈
	else if (resBlock[y][x] == 104)
		return 104;//보석상점 포탈
	else if (resBlock[y][x] == 105)
		return 105;//방어구상점 포탈
	else if (resBlock[y][x] == 106)
		return 106;//잡화상점 포탈
	else if (resBlock[y][x] == 107)
		return 107;//저스틴(메인퀘스트) 포탈
	else if (resBlock[y][x] == 108)
		return 108;//세이브 포탈
	else if (resBlock[y][x] == 109)
		return 109;//사령관 포탈


}
void drawmap_res()
{
	system("mode con:cols=90 lines=35");
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf("■            │   ◎    │△죽음의협곡                                   ■\n");
	printf("■    ┌───┤:::::::::│                  ┌──────────┐     ■\n");
	printf("■    │      │         │                  │레지스탕스 전초기지 │     ■\n");
	printf("■    │ 무기 ◎         │                  └──────────┘     ■\n");
	printf("■    │      │         │                                               ■\n");
	printf("■    ├───┤         │                                               ■\n");
	printf("■    │      │         │                                               ■\n");
	printf("■    │ 보석 ◎         │                                               ■\n");
	printf("■    │      │         │                                               ■\n");
	printf("■    ├───┤         │                                               ■\n");
	printf("■    │      │         │                                               ■\n");
	printf("■    │방어구◎         │                                               ■\n");
	printf("■    │      │         │                                               ■\n");
	printf("■    ├───┤         │                                               ■\n");
	printf("■    │      │         └───────────┬────┐             ■\n");
	printf("■    │의무실◎                                 │        │             ■\n");
	printf("■    │      │                                 ◎ 사령관 │             ■\n");
	printf("■    ├───┤                                 │        │             ■\n");
	printf("■    │      │         ┌─────◎─────┼────┘             ■\n");
	printf("■    │저스틴◎         │                      │                       ■\n");
	printf("■    │      │         │                      │                       ■\n");
	printf("■    └───┤         │       막  사         │                       ■\n");
	printf("■            │         │       (save)         │                       ■\n");
	printf("■            │:::::::::│                      │                       ■\n");
	printf("■      황야▽│   ◎    │                      │                       ■\n");
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
}
void res_village()
{
	int x, y;
	x = 21; y = 4;
	int footwalk = 0;
	int startx = 0; int starty = 0;
	int endy = 0; int endx = 0;

	if (achieve == 19 && succeed == 0)
		questfunc(3, 0);

	int a = 0;
	int g, k;
	load();
	drawmap_res();

	gotoxy(x, y);
	printf("●");
	while (1)
	{
		a = _getch();

		if (a == 72)
		{
			printf("\b\b  ");
			y -= 1;
			if (res_Blockcheck(x, y) == 100)//벽
				y += 1;
			if (res_Blockcheck(x, y) == 101)// 죽음의협곡 포탈
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 죽음의협곡으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				death_field();
				break;

			}


			gotoxy(x, y);
			printf("●");
			footwalk++;


		}
		else if (a == 75)
		{
			printf("\b\b");
			x -= 1;
			if (res_Blockcheck(x, y) == 100)
				x += 1;
			if (res_Blockcheck(x, y) == 103)// 무기상점 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n 무기상점으로 이동하셨습니다! \n\n");
				shop(1, 1);
				Sleep(1000);
				system("cls");
				drawmap_res();
			}
			if (res_Blockcheck(x, y) == 104)// 보석상점 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n 보석상점으로 이동하셨습니다! \n\n");
				shop(1, 3);
				Sleep(1000);
				system("cls");
				drawmap_res();
			}
			if (res_Blockcheck(x, y) == 105)// 방어구상점 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n 방어구상점으로 이동하셨습니다! \n\n");
				shop(1, 2);
				Sleep(1000);
				system("cls");
				drawmap_res();
			}
			if (res_Blockcheck(x, y) == 106)// 잡화상점 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n 의무실로 이동하셨습니다! \n\n");
				shop(1, 4);
				Sleep(1000);
				system("cls");
				drawmap_res();
			}
			if (res_Blockcheck(x, y) == 107)// 저스틴 포탈
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n (퀘스트)! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_res();
			}

			gotoxy(x, y);
			printf("●");
			footwalk++;

		}

		else if (a == 77)
		{
			printf("\b\b");
			x += 1;
			if (res_Blockcheck(x, y) == 100)
				x -= 1;
			else if (res_Blockcheck(x, y) == 109)//사령관
			{
				x -= 1;
				system("cls");
				printf("\n\n\n\n 메인퀘스트! \n\n");

				if (achieve == 19 && succeed == 1)
					questfunc(1, 0);

				Sleep(1000);
				system("cls");
				drawmap_res();
			}

			gotoxy(x, y);
			printf("●");
			footwalk++;
		}
		else if (a == 80)
		{
			printf("\b\b  ");
			y += 1;
			if (res_Blockcheck(x, y) == 100)
				y -= 1;
			if (res_Blockcheck(x, y) == 102)// 황야포탈
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 황야로 이동하셨습니다! \n\n");
				outback_field();
				Sleep(1000);
				system("cls");
				drawmap_res();
			}
			if (res_Blockcheck(x, y) == 108)// 세이브
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 진행상황을 저장합니다! \n\n");
				save();
				Sleep(1000);
				system("cls");
				drawmap_res();
			}

			gotoxy(x, y);
			printf("●");
			footwalk++;
		}
		else if (a == 27)
			break;

	}
}

int kingdom_Blockcheck(int x, int y)
{

	int i = 0;
	char kingdomBlock[26][80] = { 0 };

	for (i = 1; i <= 80; i++)
		kingdomBlock[10][i] = 99;//길 윗쪽 벽
	for (i = 1; i <= 80; i++)
		kingdomBlock[15][i] = 99;//길 아랫쪽 벽
	for (i = 10; i <= 15; i++)
		kingdomBlock[i][72] = 99;//길 오른쪽 벽
	for (i = 10; i <= 15; i++)
		kingdomBlock[i][5] = 98;//죽음의 협곡 포탈

	kingdomBlock[10][8] = 90;//방어구상점
	kingdomBlock[10][19] = 91;//보석상점
	kingdomBlock[10][33] = 92;//잡화상점
	kingdomBlock[10][51] = 93;//무기상점
	kingdomBlock[15][8] = 94;//NPC
	kingdomBlock[15][30] = 95;//왕궁
	kingdomBlock[15][53] = 96;//세이브
	kingdomBlock[15][67] = 97;//이장



	if (kingdomBlock[y][x] == 99)
		return 99;
	else if (kingdomBlock[y][x] == 90)
		return 90;
	else if (kingdomBlock[y][x] == 91)
		return 91;
	else if (kingdomBlock[y][x] == 92)
		return 92;
	else if (kingdomBlock[y][x] == 93)
		return 93;
	else if (kingdomBlock[y][x] == 94)
		return 94;
	else if (kingdomBlock[y][x] == 95)
		return 95;
	else if (kingdomBlock[y][x] == 96)
		return 96;
	else if (kingdomBlock[y][x] == 97)
		return 97;
	else if (kingdomBlock[y][x] == 98)
		return 98;

}
void drawmap_kingdom()
{
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf("■                                                                        ■\n");
	printf("■                                           ┌─────────┐       ■\n");
	printf("■                                           │   지배된 왕국    │       ■\n");
	printf("■                                           └─────────┘       ■\n");
	printf("■                                                                        ■\n");
	printf("■  ┌───┐              ┌────┐                                  ■\n");
	printf("■  │      │┌────┐  │  물약  │    ┌──────┐              ■\n");
	printf("■  │  옷  ││  보석  │  │  상점  │    │  무   기   │              ■\n");
	printf("■  │      ││        │  │        │    │            │              ■\n");
	printf("■─┴─◎─┴┴─-◎-─┴─┴─-◎-─┴──┴──-◎-──┴───────■\n");
	printf("■  ::                                                                    ■\n");
	printf("■◎::                                                                    ■\n");
	printf("■  ::                                                                    ■\n");
	printf("■  ::                                                                    ■\n");
	printf("■─┬─◎─┬─┬──────◎──────┬─┬─-◎-─┬─┬─-◎-─┬■\n");
	printf("■  │ NPC  │  │                          │  │        │  │        │■\n");
	printf("■  │      │  │                          │  │ 여 관  │  │  이 장 │■\n");
	printf("■  └───┘  │     왕           궁      │  │        │  │        │■\n");
	printf("■              │                          │  └────┘  └────┘■\n");
	printf("■ ◁죽음의 협곡│                          │                            ■\n");
	printf("■              │                          │                            ■\n");
	printf("■              │                          │                            ■\n");
	printf("■              └─────────────┘                            ■\n");
	printf("■                                                                        ■\n");
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
}
void kingdom_village()
{
	int x, y;
	x = 30; y = 12;
	int footwalk = 0;
	int startx = 0; int starty = 0;
	int endy = 0; int endx = 0;


	int a = 0;
	int g, k;
	load();
	drawmap_kingdom();

	gotoxy(x, y);
	printf("●");
	while (1)
	{
		a = _getch();

		if (a == 72)
		{
			printf("\b\b  ");
			y -= 1;
			if (kingdom_Blockcheck(x, y) == 99)//벽
				y += 1;
			else if (kingdom_Blockcheck(x, y) == 93)//무기상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 무기상점으로 이동하셨습니다! \n\n");
				shop(1, 1); //무기   상점 입장
				Sleep(1000);
				system("cls");
				drawmap_kingdom();
			}
			else if (kingdom_Blockcheck(x, y) == 90)//방어구상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 방어구상점으로 이동하셨습니다! \n\n");
				shop(1, 2); //방어구   상점 입장
				Sleep(1000);
				system("cls");
				drawmap_kingdom();
			}
			else if (kingdom_Blockcheck(x, y) == 91)//보석상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 보석상점으로 이동하셨습니다! \n\n");
				shop(1, 3); //보석 상점 입장
				Sleep(1000);
				system("cls");
				drawmap_kingdom();
			}
			else if (kingdom_Blockcheck(x, y) == 92)//보석상점
			{
				y += 1;
				system("cls");
				printf("\n\n\n\n 잡화상점으로 이동하셨습니다! \n\n");
				shop(1, 4); //잡화 상점 입장
				Sleep(1000);
				system("cls");
				drawmap_kingdom();
			}

			gotoxy(x, y);
			printf("●");
			footwalk++;


		}
		else if (a == 75)
		{
			printf("\b\b");
			x -= 1;
			if (kingdom_Blockcheck(x, y) == 99)
				x += 1;
			else if (kingdom_Blockcheck(x, y) == 98)
			{
				x += 1;
				system("cls");
				printf("\n\n\n\n 죽음의협곡으로 이동하셨습니다! \n\n");
				Sleep(1000);
				system("cls");
				death_field();
				break;

			}
			gotoxy(x, y);
			printf("●");
			footwalk++;

		}

		else if (a == 77)
		{
			printf("\b\b");
			x += 1;
			if (kingdom_Blockcheck(x, y) == 99)
				x -= 1;

			gotoxy(x, y);
			printf("●");
			footwalk++;
		}
		else if (a == 80)
		{
			printf("\b\b  ");
			y += 1;
			if (kingdom_Blockcheck(x, y) == 99)
				y -= 1;
			else if (kingdom_Blockcheck(x, y) == 94)
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n NPC! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_kingdom();
			}
			else if (kingdom_Blockcheck(x, y) == 95)
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 왕궁으로 이동합니다! \n\n");
				Sleep(1000);
				system("cls");
				kingdom_field();
				break;
			}
			else if (kingdom_Blockcheck(x, y) == 96)
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 진행상황을 저장합니다! \n\n");
				save();
				Sleep(1000);
				system("cls");
				drawmap_kingdom();
			}
			else if (kingdom_Blockcheck(x, y) == 97)
			{
				y -= 1;
				system("cls");
				printf("\n\n\n\n 이장! \n\n");
				Sleep(1000);
				system("cls");
				drawmap_kingdom();
			}



			gotoxy(x, y);
			printf("●");
			footwalk++;
		}
		else if (a == 27)
			break;
		else if (a == 13)
		{
			int o, p;
			wherex();
			wherey();
			o = wherex();
			p = wherey();
			printf("%d %d", p, o);
		}
	}
}




int scanA(int x, int yy, int x_max, int x_min, int y_max, int y_min)
{
	char key;
	while (1)
	{
		gotoxy(x, yy); printf("\b\b"); printf("▶");
		key = _getch();
		if (key == 72 && yy > y_min)//위쪽 방향키를 누르면서 y가 y_min보다 큰경우
		{
			printf("\b\b"); // ▶ 지우고(두칸크기이기에 2칸 지움)
			printf("  ");//지운 2칸을 매움
			yy -= 1;// 세로칸을 한칸 올림
		}

		if (key == 80 && yy < y_max)// 아래쪽 방향키를 누르면서 y가 y_max보단 작은경우
		{
			printf("\b\b"); // ▶ 지우고(두칸크기이기에 2칸 지움)
			printf("  "); //지운 2칸을 매움
			yy += 1; // 세로칸을 한칸 내림
		}

		if (yy <= y_min) { yy = y_min; }
		if (yy >= y_max) { yy = y_max - 1; }
		if (key == 32 || key == 13)
		{
			return yy;
		}
	}
}//y


void prologue(void)
{
	system("mode con:cols=75 lines=24");
	system("color 0f");
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("\n\n\n");
	printf("			   아주 ");
	Sleep(150);
	printf("아주 ");
	Sleep(150);
	printf("먼 ");
	Sleep(150);
	printf("옛날...\n\n\n");
	Sleep(200);

	textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

	printf("		       사");
	Sleep(250);
	printf("악");
	Sleep(250);
	printf("한");
	Sleep(250);
	textColor(FOREGROUND_INTENSITY);
	printf("흑룡이 ");
	Sleep(150);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("있었습니다.\n\n\n");
	Sleep(200);
	printf("		이 ");
	Sleep(150);
	textColor(FOREGROUND_INTENSITY);
	printf("흑룡은 ");
	Sleep(150);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("너무도 ");
	Sleep(150);

	textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

	printf("사");
	Sleep(250);
	printf("악");
	Sleep(250);

	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);

	printf("하고 ");
	Sleep(150);

	textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

	printf("강");
	Sleep(250);
	printf("력");
	Sleep(250);

	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);

	printf("했기에\n\n\n");
	Sleep(200);
	printf("		       세상은 ");
	Sleep(150);

	textColor(FOREGROUND_INTENSITY);

	printf("암흑으로 ");

	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);

	Sleep(150);
	printf("가득했고...\n\n\n");
	Sleep(200);
	printf("		  수많은 ");
	Sleep(150);
	printf("전사들이 ");
	Sleep(150);
	textColor(FOREGROUND_INTENSITY);
	printf("흑룡에게 ");
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	Sleep(150);
	printf("도전했지만,\n\n\n");
	Sleep(200);

	textColor(FOREGROUND_INTENSITY);
	printf("	        그의 ");
	Sleep(150);
	textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("강력함에 ");
	Sleep(150);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("모두 ");
	Sleep(150);
	printf("무너지고 ");
	Sleep(150);
	textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("말았습니다.\n\n\n");
	Sleep(1500);
	printf("              ");
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);


	system("cls");

	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);

	printf("\n\n\n\n\n");
	printf("		그렇게 ");
	Sleep(250);

	textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

	printf("절망만이 ");
	Sleep(250);

	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);

	printf("가득하던 ");
	Sleep(250);
	printf("세상에 ");
	Sleep(250);

	textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

	printf("어느 ");
	Sleep(150);
	printf("날 ");
	Sleep(150);
	printf("갑자기...\n\n");
	Sleep(300);

	textColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);

	printf("	     용검을 ");
	Sleep(250);
	printf("가진 ");
	Sleep(250);
	printf("용사가 ");
	Sleep(250);
	textColor(FOREGROUND_INTENSITY);
	printf("흑룡을 ");
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	Sleep(250);
	printf("무찌르기 ");
	Sleep(250);
	printf("위해 ");
	Sleep(150);
	printf("나타났습니다.\n\n");
	Sleep(300);
	printf("	          밤낮으로 ");
	Sleep(250);
	printf("이어진 ");
	Sleep(250);
	textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("처절한 ");
	Sleep(250);
	printf("사투");
	Sleep(150);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("끝에 ");
	Sleep(150);
	printf("마침내...\n\n");
	Sleep(300);


	printf("		   용사는 ");
	Sleep(150);
	textColor(FOREGROUND_INTENSITY);
	printf("흑룡의 ");
	Sleep(150);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("머리에 ");
	Sleep(150);
	textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("용검을 ");
	Sleep(150);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("찔러넣어,\n\n");
	Sleep(300);
	textColor(FOREGROUND_INTENSITY);
	printf("			흑룡을 ");
	Sleep(150);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("물리치게 ");
	Sleep(200);
	printf("되었습니다. \n\n");
	Sleep(350);
	printf("		   그렇게 ");
	Sleep(150);
	printf("세상엔 ");
	Sleep(150);
	printf("다시 ");
	Sleep(150);
	textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("평화가 ");
	Sleep(150);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("찾아왔고...\n\n");
	Sleep(300);
	printf("		  용사는 ");
	Sleep(200);
	textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("용검과 ");
	Sleep(200);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("함께 ");
	Sleep(200);
	printf("자취를 ");
	Sleep(200);
	printf("감추었습니다.\n\n");
	Sleep(1500);
	printf("              ");
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);


	system("cls");

	printf("\n\n\n\n\n");
	textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("		  	    하지만 ");
	Sleep(500);
	printf("수 ");
	Sleep(150);
	printf("백년 ");
	Sleep(150);
	printf("후...\n\n");
	Sleep(400);
	printf("		   사악한 ");
	Sleep(150);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("사교의 ");
	Sleep(150);
	printf("무리들이 ");
	Sleep(150);
	textColor(FOREGROUND_INTENSITY);
	printf("흑룡을 ");
	Sleep(150);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("부활시켜 \n\n");
	Sleep(200);
	printf("		  세상을 ");
	Sleep(150);
	printf("다시 ");
	Sleep(150);
	textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("파멸로 ");
	Sleep(200);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("이끌려 ");
	Sleep(200);
	printf("하고 ");
	Sleep(200);
	printf("있습니다.\n\n");
	Sleep(400);
	printf("		   어딘가에 ");
	Sleep(300);
	printf("있을 ");
	Sleep(300);
	textColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("용검은 ");
	Sleep(300);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("다시 ");
	Sleep(300);
	printf("세계를 ");
	Sleep(300);
	printf("구할 \n\n");
	Sleep(300);
	textColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("		      용사의 ");
	Sleep(300);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("후예를 ");
	Sleep(300);
	printf("기다리고");
	Sleep(300);
	printf("있습니다.\n\n");
	Sleep(300);

	printf("			    바로...");
	Sleep(1000);
	printf("당신");
	Sleep(400);
	printf("말입니다.\n\n		    	    ");
	Sleep(2000);

	textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("깨어나세요!");
	Sleep(1000);
	printf("용사여!                               \n\n\n");
	printf("              ");
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	Sleep(2000);

	system("cls");
}



void drawmap(int type, int getsu) // 맵 가로크기, 세로크기, 시작x, 시작y, 끝x, 끝y 순서 앞에 2개만 설정하면됨
{
	int grassx = 0; // 풀을 그리기 위한 변수 두개
	int grassy = 0;
	if (type == 1)
	{
		system("mode con:cols=50 lines=30");
		for (int d = 0; d < getsu; d++) // 풀 그리는 반복문
		{
			grassx = rand() % (50); // 0 ~ 가로크기 - 2 만큼을 랜덤으로 구해서 풀 x좌표에 선언
			grassy = rand() % (30); // "
			gotoxy(grassx + 3, grassy + 1); // 설정한 좌표에 가서
			textColor(FOREGROUND_GREEN); //초록색
			printf("\b,"); // 한칸 지우고 풀 이모티콘인 ","을 작성
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
			grassx = 0; // 다시 초기화 하고 반복
			grassy = 0;
		}
	}
	else if (type == 2)
	{
		system("mode con:cols=70 lines=15");
		for (int d = 0; d < getsu; d++) // 풀 그리는 반복문
		{
			grassx = rand() % (70); // 0 ~ 가로크기 - 2 만큼을 랜덤으로 구해서 풀 x좌표에 선언
			grassy = rand() % (15); // "
			gotoxy(grassx + 3, grassy + 1); // 설정한 좌표에 가서
			textColor(FOREGROUND_RED | FOREGROUND_GREEN); //노란색
			printf("\b."); // 한칸 지우고 풀 이모티콘인 ","을 작성
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
			grassx = 0; // 다시 초기화 하고 반복
			grassy = 0;
		}
	}
	else if (type == 3)
	{
		system("mode con:cols=50 lines=24");
		for (int d = 0; d < getsu; d++) // 풀 그리는 반복문
		{
			grassx = rand() % (50); // 0 ~ 가로크기 - 2 만큼을 랜덤으로 구해서 풀 x좌표에 선언
			grassy = rand() % (24); // "
			gotoxy(grassx + 3, grassy + 1); // 설정한 좌표에 가서
			textColor(FOREGROUND_RED | FOREGROUND_GREEN); //노란색
			printf("\b."); // 한칸 지우고 풀 이모티콘인 ","을 작성
			textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); //밝은 흰색
			grassx = 0; // 다시 초기화 하고 반복
			grassy = 0;
		}
	}
}

int Monster_check() // 몬스터 만나는 랜덤
{
	int a = 0;
	srand(time(NULL)); // 시드 설정, time(NULL)은 1970년 1월1일 부터 지금까지의 시간을 초로 환산해줌 즉 랜덤
	a = rand() % 4; // % 7으로 총 7자리의 수, 0~6 까지의 수를 랜덤으로 뽑아냄
	if (a == 0)
		return 1; //몬스터를 만난경우
	if (a > 0)
		return 2; // 아닌경우
}

int Monster_select()
{
	int a = 0;
	srand(time(NULL));
	a = rand() % 5;
	if (a == 0)
		return 11;
	else if (a == 1)
		return 12;
	else if (a == 2)
		return 13;
	else if (a == 3)
		return 14;
	else if (a == 4)
		return 15;
	else if (a == 5)
		return 16;
}

int battlefiled_test(void)
{
	system("cls");
	int x = 10;
	int y = 10;
	int g, k;
	int footwalk = 0; // 발걸음 카운트용 변수

	int startx = 0; int starty = 0;
	int endy = 0; int endx = 0;

	int *sx = &startx; int *sy = &starty;
	int *ex = &endx; int *ey = &endy;

	int result;
	int key = 0;
	srand(time(NULL));
	key = rand() % 3;
	drawmap(key, 80);
	gotoxy(x, y);
	printf("●");
	while (1)
	{
		key = _getch();

		if (key == 72)
		{
			printf("\b\b  ");
			y -= 1;
			gotoxy(x, y);
			printf("●");
			footwalk++;
			if (Monster_check() == 1)
			{
			system("cls");
			printf("%d걸음만에 몬스터를 만났습니다!!!\n", footwalk);
			system("cls");
			battle(Monster_select());
			Sleep(1000);
			break;
			}

		}
		else if (key == 80)
		{
			printf("\b\b  ");
			y += 1;
			gotoxy(x, y);
			printf("●");
			footwalk++;
			if (Monster_check() == 1)
			{
				system("cls");
				printf("%d걸음만에 몬스터를 만났습니다!!!\n", footwalk);
				system("cls");
				battle(Monster_select());
				Sleep(1000);
				break;
			}
		}

		else if (key == 75)
		{
			printf("\b\b");
			x -= 1;
			gotoxy(x, y);
			printf("●");
			footwalk++;
			if (Monster_check() == 1)
			{
				system("cls");
				printf("%d걸음만에 몬스터를 만났습니다!!!\n", footwalk);
				system("cls");
				battle(Monster_select());
				Sleep(1000);
				break;
			}
		}

		else if (key == 77)
		{
			printf("\b\b");
			x += 1;
			gotoxy(x, y);
			printf("●");
			footwalk++;
			if (Monster_check() == 1)
			{
				system("cls");
				printf("%d걸음만에 몬스터를 만났습니다!!!\n", footwalk);
				system("cls");
				battle(Monster_select());
				Sleep(1000);
				break;
			}
		}
		else if (key == 32) // 스페이스바 누르면 현재 좌표 표시
		{
			g = wherex();
			k = wherey();
			printf("%d %d", g, k);
			Sleep(1000);
			printf("\b\b\b\b\b");
		}

	}
};


int dev(void)//임시 메인
{
	//prologue();
	textColor(0x000f); //밝은 흰색
	int select;
	int result;
	int temp;

	while (1)
	{
		system("cls");
		printf("1.싸운다 \n2.인벤 \n3.상점 \n4.퀘스트 \n5.마을 입장 \n6.배틀테스트 \n7.스탯 \n8.세이브 \n9.로드 \n10.exp추가 \n0.나가기\n");
		scanf("%d", &select);

		if (select == 1)
		{
			printf("어떤 몬스터?");
			scanf("%d", &select);
			result = battle(select);
		}

		else if (select == 2)
		{
			printf("모드 : ");
			scanf("%d", &select);
			if (select <= 3)
				Inven(select, 0);
			else if (select == 4)
			{
				printf("아이템번호 : ");
				scanf("%d", &temp);
				Inven(4, temp);
			}
			else
			{
				printf("지원되지 않는 항목입니다");
			}

		}

		else if (select == 3)
		{
			printf("모드 : ");
			scanf("%d", &select);
			shop(1, select);
		}

		else if (select == 4)
		{
			printf("모드 : ");
			scanf("%d", &select);
			temp = 0;
			if (select == 3)
			{
				printf("죽인 몬스터 : ");
				scanf("%d", &temp);
			}
			questfunc(select, temp);
		}

		else if (select == 5)
		{
			system("cls");
			potion_village();
		}

		else if (select == 6)
		{
			battlefiled_test();
		}

		else if (select == 7)
			stat();

		else if (select == 8)
			save();
		else if (select == 9)
			load();
		else if (select == 10)
		{
			printf("추가 exp : ");
			scanf("%d", &temp);
			expadd(temp);
		}
		else if (select == 0)
			break;
	}

	return 0;
}//임시메인
     

int main(void)
{
	//if (achieve == 0)
	//	prologue();

	int mainnum = 0;

	system("mode con:cols=100 lines=25");

	//좀더 화려한 매인화면 필요
	printf("                                                                                                \n");
	Sleep(50);
	printf("      ■■■■■■        ■■■■■■    ■        ■■■■■■■  ■            ■        ■  \n");
	Sleep(50);
	printf("    ■            ■                ■    ■              ■        ■            ■        ■  \n");
	Sleep(50);
	printf("    ■            ■                ■■■■              ■    ■■■            ■  ■■■■  \n");
	Sleep(50);
	printf("      ■■■■■■                ■      ■            ■  ■      ■          ■  ■      ■  \n");
	Sleep(50);
	printf("      ■        ■              ■        ■          ■      ■    ■        ■      ■    ■  \n");
	Sleep(50);
	printf("  ■■■■■■■■■■      ■■          ■         ■        ■   ■      ■          ■  ■  \n");
	Sleep(50);
	printf("                                                                                                \n");
	Sleep(50);
	printf("      ■■■■■■          ■■■■■■■■          ■                      ■■■■■■■■  \n");
	Sleep(50);
	printf("    ■            ■        ■            ■          ■                                    ■  \n");
	Sleep(50);
	printf("    ■            ■        ■            ■          ■                      ■■■■■■■■  \n");
	Sleep(50);
	printf("    ■            ■        ■            ■          ■                      ■                \n");
	Sleep(50);
	printf("      ■■■■■■          ■■■■■■■■          ■■■■■■■■        ■■■■■■■■  \n");
	printf("                                                                                \n");
	printf("                                                                                                \n");
	Sleep(50);
	printf("                                           Main Menu                                            \n");
	printf("\n");
	printf("\n");
	printf("                                       1. 게 임 시 작\n");
	Sleep(140);
	printf("                                       2. 설       정\n");
	Sleep(130);
	printf("                                       3. 개발자의 말\n");
	Sleep(120);
	printf("                                       4. 개발자 모드\n");
	Sleep(120);
	printf("                                       5. 종       료\n");


	system("color 7e");
	Sleep(200);
	system("color 0f");

	scanf("%d", &mainnum);

	switch (mainnum)
	{
	case 1:
		potion_village();
		break;
	case 2:
		printf("아직 안만들음");
		Sleep(1000);
		break;
	case 3:
		printf("내생에 가장 후회하는건 왜 이걸 만들자고 했을까");
		Sleep(1000);
		break;
	case 4:
		dev();
		break;
	case 5:
		return 0;
	}

	/*int x = 36;
	int y = 18;
	int keye = 0;

	keye = scanA(x, y, 36, 36, 17, 22);
	if (keye == 18)
	{
		prologue();
	}*/
	return 0;
}