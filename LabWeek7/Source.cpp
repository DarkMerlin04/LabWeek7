#define _CRT_NO_WARNNINGS 1
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
char cursor(int x, int y) 
{
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))
		return '\0';
	else
		return buf[0];
}
void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
void draw_ship(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(2, 12);
	printf("<-0->");
}
void erase_ship(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(0, 0);
	printf("     ");
}
void draw_bullet(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(4, 4);
	printf("!");
}
void erase_bullet(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(0, 0);
	printf(" ");
}
void draw_comet(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(5, 0);
	printf("*");
}
void erase_comet(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(0, 0);
	printf(" ");
}
void draw_score(int x, int y, int score)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(10, 0);
	printf("Score is %d",score);
}
struct Bullet
{
	bool active = false;
	int x = 0, y = 0;
};
struct Comet
{
	bool active = true;
	int x = 0, y = 0;
};
int main()
{
	srand(time(NULL));
	setcursor(0);
	char ch = ' ';
	int x = 57, y = 25;
	int d = 0;
	int score = 0;
	Bullet bullets[5];
	Comet comet[20];
	draw_ship(x, y);
	do {
		draw_score(80, 2, score);
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 'a') { d = -1; }
			if (ch == 'd') { d = 1; }
			if (ch == 's') { d = 0; }
			if (ch == ' ')
			{
				for (int i = 0; i < 5; i++)
				{
					if (bullets[i].active == false)
					{
						bullets[i].active = true;
						bullets[i].x = x+2;
						bullets[i].y = y;
						Beep(400, 100);
						break;
					}
				}
			}
		}
		for (int j = 0; j < 20; j++)
		{
			if (comet[j].active)
			{
				comet[j].x = (rand() % 60) + 10;
				comet[j].y = (rand() % 4) + 2;
				if (cursor(comet[j].x, comet[j].y) == '*')
				{
					comet[j].active = true;
				}
				else
				{
					draw_comet(comet[j].x, comet[j].y);
					comet[j].active = false;
				}
				if (j > 20) { j = 0; }
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (bullets[i].active)
			{
				erase_bullet(bullets[i].x, bullets[i].y);
				if (bullets[i].y > 0)
				{
					if (cursor(bullets[i].x, bullets[i].y - 1) != '*')
					{
						draw_bullet(bullets[i].x, --bullets[i].y);
					}
					else
					{
						Beep(800, 100);
						erase_bullet(bullets[i].x, --bullets[i].y);
						bullets[i].active = false;
						comet[i].active = true;
						score++;
						draw_score(80, 2, score);
					}
				}
				else
				{
					bullets[i].active = false;
				}
			}
		}
		if (d == -1 && x > 0) { erase_ship(x, y); draw_ship(--x, y); }
		if (d == 1 && x < 115) { erase_ship(x, y); draw_ship(++x, y); }
		if (d == 0) { erase_ship(x, y); draw_ship(x, y); }
		Sleep(10);
	} while (ch != 'x');
	return 0;
}