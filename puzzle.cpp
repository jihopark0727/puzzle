// puzzle.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <time.h>
#include <stdlib.h>
#include <bangtal.h>
#include <string.h>
#include <stdio.h>

SceneID scene1;
ObjectID picture[9], start_button, end_button;
TimerID timer1;

int pictureSeq[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8}, blank = 8, shuffle_num = 100, ii;
unsigned int start_time;
bool game_started = false, using_chance = false;

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown)
{
    ObjectID object = createObject(image);
    locateObject(object, scene, x, y);

    if (shown)
    {
        showObject(object);
    }
    return object;
}

const char* picture_name(int num)
{
    static char buf[20] = {};
    sprintf_s(buf, "puzzle%d.jpeg", num);
    return buf;
}

void img_set()
{
    int i, garo, sero;
    for (i = 0; i < 9; i++)
    {
        sero = i / 3;
        garo = i % 3;
        locateObject(picture[pictureSeq[i]], scene1, garo * 309, sero * 240);
    }
}

void call_picture()
{
    int i, sero, garo;
    for (i = 0; i < 9; i++)
    {
        sero = i / 3;
        garo = i % 3;
        picture[i] = createObject(picture_name(i), scene1, garo * 309, 480- sero * 240, true);
    }
}

void change_pic_place(int init, int fin)
{
    int temp;
    temp = pictureSeq[init];
    pictureSeq[init] = pictureSeq[fin];
    pictureSeq[fin] = temp;

    img_set();
}

void clear_judge()
{
    char end_message[50];
    int i;

    for (i = 0; i < 8; i++)
    {
        if (pictureSeq[i] != i)
        {
            break;
        }
    }
    if (i == 8)
    {
        game_started = false;
        setObjectImage(start_button, "restart.png");
        showObject(start_button);
        showObject(end_button);
        //showObject(picture[8]);
        sprintf_s(end_message, "%d초 걸렸습니다!", (int)time(NULL) - start_time);
        showMessage(end_message);
    }
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
    int i, j;
    bool change = false;
    char end_message[50] = {};

    if (object == start_button)
    {
        ii = 0;

        hideObject(picture[8]);
        hideObject(start_button);
        hideObject(end_button);

        startTimer(timer1);
        start_time = time(NULL);
    }
    if (object == end_button)
    {
        endGame();
    }
    if (game_started && !using_chance)
    {
        for (i = 0; i < 9; i++)
        {

            if (object == picture[i])
            {
                for (j = 0; j < 9; j++)
                {
                    if (pictureSeq[j] == i)
                        break;
                }
                switch (j)
                {
                case 0:
                    if (j + 1 == blank || j + 3 == blank)
                        change = true;
                    break;
                case 1:
                    if (j - 1 == blank || j + 1 == blank || j + 3 == blank)
                        change = true;
                    break;
                case 2:
                    if (j - 1 == blank || j + 3 == blank)
                        change = true;
                    break;
                case 3:
                    if (j + 1 == blank || j - 3 == blank || j + 3 == blank)
                        change = true;
                    break;
                case 4:
                    if (j + 1 == blank || j - 1 == blank || j - 3 == blank || j + 3 == blank)
                        change = true;
                    break;
                case 5:
                    if (j - 1 == blank || j - 3 == blank || j + 3 == blank)
                        change = true;
                    break;
                case 6:
                    if (j + 1 == blank || j - 3 == blank)
                        change = true;
                    break;
                case 7:
                    if (j + 1 == blank || j - 1 == blank || j - 3 == blank)
                        change = true;
                    break;
                case 8:
                    if (j - 1 == blank || j - 3 == blank)
                        change = true;
                    break;
                default:
                    change = false;
                }

                if (change)
                {
                    change_pic_place(j, blank);
                    blank = j;

                    clear_judge();
                }
                break;
            }
        }

    }
}
    void timerCallback(TimerID timer)
    {
        int next;
        if (timer == timer1)
        {
            if (ii < shuffle_num)
            {
                next = rand() % 9;

                if (next == blank)
                {
                    ii--;
                }

                change_pic_place(next, blank);

                blank = next;

                ii++;

                startTimer(timer1);
            }
            else
            {
                game_started = true;
            }
        }
    }



    int main()
    {
        srand((unsigned int)time(NULL));
        setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
        setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);

        setMouseCallback(mouseCallback);
        setTimerCallback(timerCallback);

        scene1 = createScene("배경", "puzzle.png");
        timer1 = createTimer(0.01f);

        call_picture();
        start_button = createObject("start.png", scene1, 400, 70, true);
        end_button = createObject("end.png", scene1, 400, 30, true);

        startGame(scene1);

    }

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
