#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>
#include <process.h>
#include <mmsystem.h>    

#pragma comment(lib, "winmm.lib")

#define TEXT_WITHBG 13+240
#define TEXT_COLOR 5+240
#define TEXT_COLORN 7+240
#define TEXT_COLORS 6+240
#define TEXT_COLORB 1+240
#define FOOD_COLOR 4+240
#define SNAKE_COLOR 2+240
#define BOARDER_COLOR 0+240
#define ITEM_COLOR 4+240
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define DEAD_ZONE_COUNT 50
#define Third_Run "Snake Charmer’s Flute for Sleep.wav"
#define FOOD "food.wav"
#define DEFEAT "buzzer_x.wav"
#define LOSE "boxing_bell.wav"
#define NOSOUND "BOOM.wav"
#define MOVE "button-3.wav"
#define SNAKEBODY "+"
#define ITEM "@"
#define BOARDER "#"


int level = 0;
int foodCount = 0;
int length,apt;
int bend_no;
int len;
char key;
int dead_x[100] = { 0 }, dead_y[100] = { 0 };
void record();
void load();
int life;
void Delay();
void Move();
void Food();
void PrintSnake();
int Score();
void Print();
void gotoxy(int x, int y);
void GotoXY(int x, int y);
void Bend();
void Boarder(int color);
void Down();
void Left();
void Up();
void DeadzoneBuilder();
void DeadzoneChecker(int snk_x, int snk_y);
void SelectLevel();
void Right();
void ExitGame();
int Scoreonly();

struct coordinate
{
    int x;
    int y;
    int direction;
};

typedef struct coordinate coordinate;

coordinate head, bend[500], food, body[30];

int main()
{
    CONSOLE_CURSOR_INFO cursorInfo = { 0, }; cursorInfo.dwSize = 1; cursorInfo.bVisible = FALSE; SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo); // 커서 숨김
    system("color F1");
    PlaySound(TEXT(Third_Run), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    DeadzoneBuilder();
    char key;

    Print();

    system("cls");

    SelectLevel();

    system("cls");

    load();

    system("cls");

    length = 5;

    head.x = 25;

    head.y = 20;

    head.direction = RIGHT;

    Boarder(0);

    life = 3; //number of extra lives

    bend[0] = head;

    
    Move();

    return 0;

}

void Move()
{
    int a, i;

    do
    {
        gotoxy(body[length-1].x, body[length-1].y); printf(" ");
        Food();
        fflush(stdin);

        len = 0;

        for (i = 0; i < 30; i++)

        {

            body[i].x = 0;

            body[i].y = 0;

            if (i == length)

                break;

        }

        Delay();


        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SNAKE_COLOR);
        if (head.direction == RIGHT)
        {
            Right();
        }

        else if (head.direction == LEFT)
        {
            Left();
        }

        else if (head.direction == DOWN)
        {
            Down();
        }
        else if (head.direction == UP)
        {
            Up();
        }

        DeadzoneChecker(head.x, head.y);
        ExitGame();

    } while (!kbhit());

    a = getch();

    if (a == 27)

    {

        system("cls");

        exit(0);

    }
    key = getch();

    if ((key == RIGHT && head.direction != LEFT && head.direction != RIGHT) || (key == LEFT && head.direction != RIGHT && head.direction != LEFT) || (key == UP && head.direction != DOWN && head.direction != UP) || (key == DOWN && head.direction != UP && head.direction != DOWN))

    {

        PlaySound(TEXT(MOVE), NULL, SND_FILENAME | SND_ASYNC);
        bend_no++;

        bend[bend_no] = head;

        head.direction = key;

        if (key == UP)

            head.y--;

        if (key == DOWN)

            head.y++;

        if (key == RIGHT)

            head.x++;

        if (key == LEFT)

            head.x--;

        Move();

    }

    else if (key == 27)

    {

        system("cls");

        exit(0);

    }

    else

    {

        printf("\a");

        Move();

    }
}

void gotoxy(int x, int y)
{

    COORD coord;

    coord.X = x;

    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}
void GotoXY(int x, int y)
{
    HANDLE a;
    COORD b;
    fflush(stdout);
    b.X = x;
    b.Y = y;
    a = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(a, b);
}
void load()
{
    gotoxy(53, 14); printf("Now Loading...");
    PlaySound(TEXT(NOSOUND), NULL, SND_FILENAME | SND_SYNC);
}
void Down()
{
    int i;
    for (i = 0; i <= (head.y - bend[bend_no].y) && len < length; i++)
    {
        GotoXY(head.x, head.y - i);
        {
            if (len == 0)
            {
                printf("V");
            }
            else
            {
                printf(SNAKEBODY);
            }
        }
        body[len].x = head.x;
        body[len].y = head.y - i;
        len++;
    }
    Bend();
    if (!kbhit())
        head.y++;
}
void Delay()
{
    clock_t oTime, cTime;
    Score();
    long double i;
    oTime = clock();
    while (1) {
        cTime = clock();
        if (cTime - oTime > 200/((float)level/2)) break;
    }
}
void ExitGame()
{
    int i, check = 0;
    for (i = 4; i < length; i++) //starts with 4 because it needs minimum 4 element to touch its own body
    {
        if (body[0].x == body[i].x && body[0].y == body[i].y)
        {
            check++;    //check's value increases as the coordinates of head is equal to any other body coordinate
        }
        if (i == length || check != 0)
            break;
    }
    if (head.x <= 10 || head.x >= 70+40 || head.y <= 10 || head.y >= 29 || check != 0)
    {
        life--;
        if (life > 0)
        {
            Boarder(1);
            PlaySound(TEXT(LOSE), NULL, SND_FILENAME | SND_SYNC);
            system("cls");
            Boarder(0);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOOD_COLOR);
            GotoXY(food.x, food.y); printf(ITEM);
            head.x = 25;
            head.y = 20;
            bend_no = 0;
            head.direction = RIGHT;
            Move();
        }
        else
        {
            system("cls");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4+240);
            GotoXY(14, 13); printf("   #######      ##     ###     ###  ######        #####    ##      ## ######   ###### ");
            GotoXY(14, 14); printf("  ##           ####    ####   ####  ##         ###     ##   ##    ##  ##       ##   ##");
            GotoXY(14, 15); printf(" ##  #####    ##  ##   ## ## ## ##  ######    ##        ##   ##  ##   ######   ###### ");
            GotoXY(14, 16); printf("  ## ## ##   ## #####  ##  ###  ##  ##         ##    ###      ####    ##       ##   ##");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6 + 240);
            GotoXY(14, 17); printf("   ###  ##  ##      ## ##   #   ##  ######      ######         ##     ######   ##    ##");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5 + 240);
            GotoXY(43, 27); printf("     아무 키나 눌러서 점수화면으로");
            PlaySound(TEXT(DEFEAT), NULL, SND_FILENAME | SND_ASYNC);
            record();
            exit(0);
        }
    }
}
void Food()
{
    if (head.x == food.x && head.y == food.y)
    {
        PlaySound(TEXT(FOOD), NULL, SND_FILENAME | SND_ASYNC);
        length++;
        foodCount = 0;
    }
    srand(time(NULL));
    if (foodCount == 1) { return; }
    else {
        foodCount = 1;
        food.x = (rand() % (60 - 10)) + 30 + 5;
        food.y = (rand() % (20 - 10)) + 10 + 5;
        for (int i = 0; i < level * 10; i++) {
            if (food.x == dead_x[i] && food.y == dead_y[i]) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                gotoxy(dead_x[i], dead_y[i]); printf(BOARDER);
                foodCount = 0;
                return;
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOOD_COLOR);
            GotoXY(food.x, food.y); printf(ITEM);
        }


    }

}
void Left()
{
    int i;
    for (i = 0; i <= (bend[bend_no].x - head.x) && len < length; i++)
    {
        GotoXY((head.x + i), head.y);
        {
            if (len == 0)
            {
                printf("<");
            }
            else
            {
                printf(SNAKEBODY);
            }
        }
        body[len].x = head.x + i;
        body[len].y = head.y;
        len++;
    }
    Bend();
    if (!kbhit())
        head.x--;

}
void Right()
{
    int i;
    for (i = 0; i <= (head.x - bend[bend_no].x) && len < length; i++)
    {
        //GotoXY((head.x-i),head.y);
        body[len].x = head.x - i;
        body[len].y = head.y;
        GotoXY(body[len].x, body[len].y);
        {
            if (len == 0)
            {
                printf(">");
            }
            else
            {
                printf(SNAKEBODY);
            }
        }
        /*body[len].x=head.x-i;
        body[len].y=head.y;*/
        len++;
    }
    Bend();
    if (!kbhit())
        head.x++;
}
void Bend()
{
    int i, j, diff;
    for (i = bend_no; i >= 0 && len < length; i--)
    {
        if (bend[i].x == bend[i - 1].x)
        {
            diff = bend[i].y - bend[i - 1].y;
            if (diff < 0)
                for (j = 1; j <= (-diff); j++)
                {
                    body[len].x = bend[i].x;
                    body[len].y = bend[i].y + j;
                    GotoXY(body[len].x, body[len].y);
                    printf(SNAKEBODY);
                    len++;
                    if (len == length)
                        break;
                }
            else if (diff > 0)
                for (j = 1; j <= diff; j++)
                {
                    /*GotoXY(bend[i].x,(bend[i].y-j));
                    printf("*");*/
                    body[len].x = bend[i].x;
                    body[len].y = bend[i].y - j;
                    GotoXY(body[len].x, body[len].y);
                    printf(SNAKEBODY);
                    len++;
                    if (len == length)
                        break;
                }
        }
        else if (bend[i].y == bend[i - 1].y)
        {
            diff = bend[i].x - bend[i - 1].x;
            if (diff < 0)
                for (j = 1; j <= (-diff) && len < length; j++)
                {
                    /*GotoXY((bend[i].x+j),bend[i].y);
                    printf("*");*/
                    body[len].x = bend[i].x + j;
                    body[len].y = bend[i].y;
                    GotoXY(body[len].x, body[len].y);
                    printf(SNAKEBODY);
                    len++;
                    if (len == length)
                        break;
                }
            else if (diff > 0)
                for (j = 1; j <= diff && len < length; j++)
                {
                    /*GotoXY((bend[i].x-j),bend[i].y);
                    printf("*");*/
                    body[len].x = bend[i].x - j;
                    body[len].y = bend[i].y;
                    GotoXY(body[len].x, body[len].y);
                    printf(SNAKEBODY);
                    len++;
                    if (len == length)
                        break;
                }
        }
    }
}


void Boarder(int color)
{
    //system("cls");
    int i;
    GotoXY(food.x, food.y);   /*displaying food*/
    switch (color) {
    case 0:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BOARDER_COLOR);
        break;
    case 1:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOOD_COLOR);
        break;
    }

    for (i = 10; i < 71+40; i++)
    {
        GotoXY(i, 10);
        printf(BOARDER);
        GotoXY(i, 29);
        printf(BOARDER);
    }
    for (i = 10; i < 30; i++)
    {
        GotoXY(10, i);
        printf(BOARDER);
        GotoXY(70+40, i);
        printf(BOARDER);
    }
    for (i = 0; i < level * 10; i++) {
        GotoXY(dead_x[i], dead_y[i]);
        printf(BOARDER);
    }
}
void Print()
{
    //데드존 테스트
    /*for (int i = 0; i < 10; i++) {
        printf("%d %d\n",dead_x[i],dead_y[i]);
    }*/
    //GotoXY(10,12);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240+1);

    GotoXY(7, 6); printf("##     ##     ## ######  ##        ######       #####     ##        ##  ######           ########     ##### ");
    GotoXY(7, 7); printf(" ##   ####   ##  ##      ##      ##          ##      ##   ###      ###  ##                  ##      ##     ## ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240+3);
    GotoXY(7, 8); printf("  ## ##  ## ##   ######  ##     ##          ##        ##  ## ##  ## ##  ######              ##    ##        ##");
    GotoXY(7, 9); printf("   ###    ###    ##      ##      ##          ##     ##    ##   ###  ##  ##                  ##     ##      ##  ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240+5);
    GotoXY(7, 10); printf("    ##    ##     ######  ######   #######     ######      ##   ##   ## ######               ##       ######  ");

    GotoXY(55, 12); printf("  ### #   ###");
    GotoXY(55, 13); printf("  #  ### ##");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240+4);
    GotoXY(55, 14); printf(" #  # # ####");

    GotoXY(38, 16); printf("######  ###   ##     ##     ##   ##  ###### ");
    GotoXY(38, 17); printf("##      ####  ##    ####    ##  ##   ##     ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240+6);
    GotoXY(38, 18); printf("######  ## ## ##   ##  ##   ####     ###### ");
    GotoXY(38, 19); printf("    ##  ##  ####  ########  ## ###   ##     ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240+2);
    GotoXY(38, 20); printf("######  ##   ### ##      ## ##  ###  ###### ");

    GotoXY(43, 22); printf(" ######      ##    ##     ##  ###### ");
    GotoXY(43, 23); printf("##          ## #   ###   ### ### #  ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240+9);
    GotoXY(43, 24); printf("##  ####   ## ###  ## ### ## ## ");
    GotoXY(43, 25); printf(" ###  ##  ##    ## ##  #  ## ######");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_WITHBG);
    GotoXY(43, 27); printf("        아무 키나 눌러서 시작");

    getch();
    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_COLORS);
    GotoXY(38, 9); printf(" [조작키]          [수집품]       [장애물]     ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_COLOR);
    GotoXY(38, 11); printf("    ↑                @              #      ");
    GotoXY(38, 12); printf("  ←↓→                             ");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_COLORS);
    GotoXY(36, 18); printf("[일시정지/재개]        [종료]       [남은 목숨]    ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_COLOR);
    GotoXY(38, 20); printf("   아무 키            ESC             3회        ");
    GotoXY(43, 27); printf("        아무 키나 눌러서 시작");
    if (getch() == 27)
        exit(0);
}
void record()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_WITHBG);
    char plname[20], nplname[20], cha, c;
    int i, j, px, pt[50] = { 0 }, ln = 0;
    FILE* info;
    info = fopen("record.txt", "a+");
    getch();
    system("cls");

    PrintSnake();

    GotoXY(43, 11); printf("          기록한 점수 : %d", apt);
    GotoXY(43, 13); printf("        플레이어 이름 :");
    scanf("%s", &plname);
    PlaySound(TEXT(FOOD), NULL, SND_FILENAME | SND_ASYNC);
    //************************
    for (j = 0; plname[j] != '\0'; j++) //to convert the first letter after space to capital
    {
        nplname[0] = toupper(plname[0]);
        if (plname[j - 1] == ' ')
        {
            nplname[j] = toupper(plname[j]);
            nplname[j - 1] = plname[j - 1];
        }
        else nplname[j] = plname[j];
    }
    nplname[j] = '\0';
    //*****************************
    //sdfprintf(info,"\t\t\tPlayers List\n");
    //fprintf(info,"Player Name :%s\n",nplname);

    fprintf(info, "%d\n", px = Scoreonly());//call score to display score
    //fprintf(info,"\nLevel:%d\n",10);//call level to display level
    fclose(info);
    PrintSnake();
    GotoXY(42, 11); printf("[ 상위 10명을 확인하려면 'Y' 키 입력 ]");
    GotoXY(42, 13); printf("   [ 종료하려면 'Y'이외의 키 입력 ] ");
    cha = getch();
    PlaySound(TEXT(FOOD), NULL, SND_FILENAME | SND_ASYNC);
    system("cls");
    if (cha == 'y')
    {
        info = fopen("record.txt", "r");

        char buf[1024];
        while (!feof(info)) {
            fgets(buf, sizeof(buf), info);
            pt[ln] = atoi(buf);
            ln++;
        }
        int tmp;
        for (int x = 0; x < sizeof(pt) / sizeof(int) - 1; x++) {
            for (int xx = x + 1; xx < sizeof(pt) / sizeof(int); xx++) {
                if (pt[x] < pt[xx]) {
                    tmp = pt[xx];
                    pt[xx] = pt[x];
                    pt[x] = tmp;
                }
            }
        }
        PrintSnake();
        GotoXY(28,7); printf("****개인정보 보호를 위해 플레이어 이름은 익명으로 표기되었습니다.****");
        for (int x = 0; x < 10; x++) {
            GotoXY(48, 9+x); printf("%d등 : %d점  |  플레이어: ", x + 1, pt[x]);
            for (int r = 0; r < (rand() % 3) + 4; r++) {
                printf("*");
            }
        }
        fclose(info);
    }
    GotoXY(43, 27); printf("        아무 키나 눌러서 종료");
    getch();
}

int Score()
{
    int score;
    GotoXY(25, 8);
    score = (length - 5) * level;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_WITHBG);
    printf("점수 : ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_COLORB);
    printf("%d", score);
    score = (length - 5) * level;
    GotoXY(50, 8);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_WITHBG);
    printf("푸드 위치 : ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_COLORB);
    printf("%d : %d", food.x, food.y);
    GotoXY(85, 8);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_WITHBG);
    printf("남은 기회 : ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_COLORB);
    printf("%d", life);
    apt = score;
    return score;
}
int Scoreonly()
{
    int score = Score();
    system("cls");
    return score;
}
void Up()
{
    int i;
    for (i = 0; i <= (bend[bend_no].y - head.y) && len < length; i++)
    {
        GotoXY(head.x, head.y + i);
        {
            if (len == 0)
            {
                printf("A");
            }
            else
            {
                printf(SNAKEBODY);
            }
        }
        body[len].x = head.x;
        body[len].y = head.y + i;
        len++;
    }
    Bend();
    if (!kbhit())
        head.y--;
}

void DeadzoneBuilder() {
    for (int i = 0; i < DEAD_ZONE_COUNT; i++) {
        srand(time(NULL) * (i + 1));
        dead_x[i] = (rand() % 80) + 30;
        dead_y[i] = (rand() % 20) + 10;
    }
}

void DeadzoneChecker(int snk_x, int snk_y) {
    for (int i = 0; i < level * 10; i++) {
        if (snk_x == dead_x[i] && snk_y == dead_y[i])
        {
            life--;
            Boarder(1);
            PlaySound(TEXT(LOSE), NULL, SND_FILENAME | SND_SYNC);
            system("cls");
            Boarder(0);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOOD_COLOR);
            GotoXY(food.x, food.y); printf(ITEM);
            if (life > 0)
            {
                head.x = 25;
                head.y = 20;
                bend_no = 0;
                head.direction = RIGHT;
                Move();
            }
            else
            {
                system("cls");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4 + 240);
                GotoXY(14, 13); printf("   #######      ##     ###     ###  ######        #####    ##      ## ######   ###### ");
                GotoXY(14, 14); printf("  ##           ####    ####   ####  ##         ###     ##   ##    ##  ##       ##   ##");
                GotoXY(14, 15); printf(" ##  #####    ##  ##   ## ## ## ##  ######    ##        ##   ##  ##   ######   ###### ");
                GotoXY(14, 16); printf("  ## ## ##   ## #####  ##  ###  ##  ##         ##    ###      ####    ##       ##   ##");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6 + 240);
                GotoXY(14, 17); printf("   ###  ##  ##      ## ##   #   ##  ######      ######         ##     ######   ##    ##");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5 + 240);
                GotoXY(43, 27); printf("     아무 키나 눌러서 점수화면으로");
                PlaySound(TEXT(DEFEAT), NULL, SND_FILENAME | SND_ASYNC);
                record();
                exit(0);
            }
        }
    }
}

void SelectLevel() {

    PrintSnake();
    GotoXY(43, 10); printf("        [ 난이도 선택 ]       ");
    GotoXY(43, 12); printf("[ 1 ~ 10 중 원하는 값을 입력 ]");
    GotoXY(43, 14); printf("       설정 난이도 : ");
    scanf("%d", &level);
    if (level < 1) { level = 1; }
    else if (level > 10) { level = 10; }
    system("cls");
}

void PrintSnake() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6 + 240);
    GotoXY(0, 1); printf("##     ##     ##   ##  ######     ######  ###   ##     ##     ##   ##  ######     ######  ###   ##     ##     ##   ##  #");
    GotoXY(0, 2); printf("##    ####    ##  ##   ##         ##      ####  ##    ####    ##  ##   ##         ##      ####  ##    ####    ##  ##   #");
    GotoXY(0, 3); printf("##   ##  ##   ####     ######     ######  ## ## ##   ##  ##   ####     ######     ######  ## ## ##   ##  ##   ####     #");
    GotoXY(0, 4); printf("##  ########  ## ###   ##             ##  ##  ####  ########  ## ###   ##             ##  ##  ####  ########  ## ###   #");
    GotoXY(0, 5); printf("## ##      ## ##  ###  ######     ######  ##   ### ##      ## ##  ###  ######     ######  ##   ### ##      ## ##  ###  #");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2 + 240);
    GotoXY(0, 10); printf("##     ##     ##   ##  ######     ####                                              ####  ###   ##     ##     ##   ##  #");
    GotoXY(0, 11); printf("##    ####    ##  ##   ##         ##                                                      ####  ##    ####    ##  ##   #");
    GotoXY(0, 12); printf("##   ##  ##   ####     ######     ####                                              ####  ## ## ##   ##  ##   ####     #");
    GotoXY(0, 13); printf("##  ########  ## ###   ##                                                             ##  ##  ####  ########  ## ###   #");
    GotoXY(0, 14); printf("## ##      ## ##  ###  ######     ####                                              ####  ##   ### ##      ## ##  ###  #");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4 + 240);
    GotoXY(0, 19); printf("##     ##     ##   ##  ######     ######  ###   ##     ##     ##   ##  ######     ######  ###   ##     ##     ##   ##  #");
    GotoXY(0, 20); printf("##    ####    ##  ##   ##         ##      ####  ##    ####    ##  ##   ##         ##      ####  ##    ####    ##  ##   #");
    GotoXY(0, 21); printf("##   ##  ##   ####     ######     ######  ## ## ##   ##  ##   ####     ######     ######  ## ## ##   ##  ##   ####     #");
    GotoXY(0, 22); printf("##  ########  ## ###   ##             ##  ##  ####  ########  ## ###   ##             ##  ##  ####  ########  ## ###   #");
    GotoXY(0, 23); printf("## ##      ## ##  ###  ######     ######  ##   ### ##      ## ##  ###  ######     ######  ##   ### ##      ## ##  ###  #");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_WITHBG);
}