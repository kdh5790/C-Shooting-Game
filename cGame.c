#pragma warning(disable:4996)
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <cursor.h>

enum { ESC = 27, LEFT = 75, RIGHT = 77 };

#define HEIGHT 23
#define WIDTH 44
#define TRUE 1
#define FALSE 0


void PrintGame(); // 게임판 전체를 그려주는 함수
void PrintFloor(); // 바닥을 그려주는 함수
void CreateEnemy(); // 적을 생성해주는 함수
void CreateItem();
void MoveEnemy(); // 적을 이동시켜주는 함수
void HitPlayer(); // 플레이어가 데미지를 입었는지 확인해주는 함수
void SaveScore(); // 점수 저장
void LoadScore(); // 점수 불러오기
void GameOver(); // 게임 종료 시 정보를 출력해 줄 함수


struct Player
{
	int px; // 플레이어의 좌우 이동을 위한 x값
	int p_hp;
	BOOL exist; // 플레이어의 생존여부
} player;

struct Enemy
{
	int ex;
	int ey;
	BOOL exist;
} enemy[35];

struct Item
{
	int x;
	int y;
	BOOL exist;
} item;

BOOL IsKeyDown(int Key) // 키입력을 받아오기 위한 함수
{
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
	// GetAsyncKeyState : 현재 무슨 키가 입력되는지 확인
	// 0x8000 : 이전에 누른 적이 없고 호출 시점에는 눌려있는 상태
}

int score = 0;
int bestScore = 0;

int main()
{
	player.exist = TRUE;
	player.px = WIDTH / 2; // 플레이어 초기 x값(위치)
	player.p_hp = 3;

	int speed = 1;
	int eCount = 3;

	srand((unsigned)time(NULL)); // 난수 초기화
	system("cls"); // 화면 지우기
	showcursor(0); //커서 숨기기

	for (int i = 0; i <= 34; i++)
		enemy[i].exist = FALSE;

	gotoxy(player.px, HEIGHT - 2);
	printf("●");
	gotoxy(player.px, HEIGHT - 1);
	printf("■");
	gotoxy(player.px, HEIGHT);
	printf("∥");

	PrintFloor();

	for (int count = 1;; count++)
	{
		if (player.exist)
		{
			if (count % eCount == 0) CreateEnemy();
			if (count % 100 == 0) CreateItem();
			LoadScore();
			if (count % speed == 0)
				MoveEnemy();
			PrintGame();
			HitPlayer();
			if (IsKeyDown(VK_LEFT))
			{
				if (player.px > 0)
				{
					gotoxy(player.px, HEIGHT - 2);
					printf("　");
					gotoxy(player.px, HEIGHT - 1);
					printf("　");
					gotoxy(player.px, HEIGHT);
					printf("　");
					player.px += -1;
					gotoxy(player.px, HEIGHT - 2);
					printf("●");
					gotoxy(player.px, HEIGHT - 1);
					printf("■");
					gotoxy(player.px, HEIGHT);
					printf("∥");
				}
			}

			if (IsKeyDown(VK_RIGHT))
			{
				if (player.px <= 41)
				{
					gotoxy(player.px, HEIGHT - 2);
					printf("　");
					gotoxy(player.px, HEIGHT - 1);
					printf("　");
					gotoxy(player.px, HEIGHT);
					printf("　");
					player.px += 1;
					gotoxy(player.px, HEIGHT - 2);
					printf("●");
					gotoxy(player.px, HEIGHT - 1);
					printf("■");
					gotoxy(player.px, HEIGHT);
					printf("∥");

				}
			}
			/*if (IsKeyDown(ESC))
				player.exist = FALSE;*/

			Sleep(30);
		}
		else if (player.exist == FALSE)
		{
			Sleep(500);
			break;
		}
	}

	SaveScore();
	GameOver();
	getch();

	return 0;
}

void PrintFloor()
{
	for (int i = 0; i < WIDTH + 1; i++)
	{
		gotoxy(i, 24);
		printf("-");
	}

	for (int i = 0; i < HEIGHT + 1; i++)
	{
		gotoxy(44, i);
		printf("|");
	}
}

void PrintGame()
{
	gotoxy(0, 27);
	printf("점수 : %d", score);

	if (score >= bestScore)
		bestScore = score;

	gotoxy(20, 27);
	printf("최고 점수 : %d", bestScore);

	gotoxy(0, 29);
	switch (player.p_hp)
	{
	case 3:
		printf("HP ♥ ♥ ♥");
		break;
	case 2:
		printf("HP ♥ ♥ ♡");
		break;
	case 1:
		printf("HP ♥ ♡ ♡");
		break;
	case 0:
		printf("HP ♡ ♡ ♡");
		break;
	}

	for (int i = 0; i < 35; i++)
	{
		if (enemy[i].exist)
		{
			gotoxy(enemy[i].ex, enemy[i].ey - 1);
			printf("　");

			gotoxy(enemy[i].ex, enemy[i].ey);
			printf("＠");
		}
	}

	if (item.exist)
	{
		gotoxy(item.x, item.y - 1);
		printf("　");

		gotoxy(item.x, item.y);
		printf("♥");
	}

}

void CreateEnemy()
{
	int i;

	for (i = 0; i < 35; i++)
	{
		//해당 인덱스[i]에 적이 없으면 (FALSE 이면 실행)
		if (!enemy[i].exist)
		{
			//가로 (x축) 무작위로 적 출현, 세로(y축)은 출현 위치 항상 일치
			enemy[i].ex = rand() % 41;
			enemy[i].ey = 1;
			//적이 출현한 인덱스 [i]의 상태를 TRUE로 변경
			enemy[i].exist = TRUE;
			return;
		}
	}
}

void CreateItem()
{
	item.x = rand() % 41;
	item.y = 1;
	item.exist = TRUE;
	return;
}

void MoveEnemy()
{
	for (int i = 0; i < WIDTH; i++)
	{
		if (enemy[i].exist == TRUE) // 해당 인덱스에 적이 있다면 실행
		{
			enemy[i].ey++;
			if (enemy[i].ey == HEIGHT + 1 && enemy[i].exist == TRUE)
			{
				gotoxy(enemy[i].ex, HEIGHT);
				printf("　");
				enemy[i].exist = FALSE;
				score += 50;
			}
		}
	}
	if (item.exist == TRUE)
	{
		item.y++;
		if (item.y == HEIGHT + 1 && item.exist == TRUE)
		{
			gotoxy(item.x, HEIGHT);
			printf("　");
			item.exist = FALSE;
		}
	}
}

void HitPlayer()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		if (enemy[i].exist == FALSE) // 적이 아직 살아있다면 for문 탈출
			continue;
		if (enemy[i].ey == HEIGHT && abs(enemy[i].ex - player.px) <= 1) // 적의 y값이 플레이어의 y값과 같고 (적의 x값 - 플레이어의 x값)의 절대값이 1보다 작거나 같다면
		{
			enemy[i].exist = FALSE; // 적을 없앤다.
			gotoxy(enemy[i].ex, enemy[i].ey); // 적의 위치로 커서 이동
			puts(" "); // 공백 출력
			player.p_hp -= 1;
			if (player.p_hp <= 0)
			{
				player.p_hp = 0;
				player.exist = FALSE;
			}
		}
	}

	if (item.exist == TRUE)
	{
		if (item.y == HEIGHT && abs(item.x - player.px) <= 1)
		{
			item.exist = FALSE;
			gotoxy(item.x, item.y);
			puts("　");
			if (player.p_hp < 3)
				player.p_hp += 1;
		}

	}

}

void SaveScore()
{
	FILE* fFile = fopen("score.dat", "wb");
	if (fFile != NULL)
	{
		fprintf(fFile, "%d", bestScore);
		fclose(fFile);
	}
}

void LoadScore()
{
	FILE* fFile = fopen("score.dat", "rb");
	if (fFile != NULL)
	{
		fscanf(fFile, "%d", &bestScore);
		fclose(fFile);
	}
}

void GameOver()
{
	system("cls");
	gotoxy(20, 5);
	puts("GAME OVER");
	gotoxy(20, 6);
	printf("점수 : %d\n\n\n\n", score);
}
