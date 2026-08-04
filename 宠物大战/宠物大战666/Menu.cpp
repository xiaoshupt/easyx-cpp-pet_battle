#include "Menu.h"
#include "Battle.h"
#include <graphics.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include "SaveData.h"
#include <fstream>

//============================
// °´Å¥ÇøÓò
//============================
const int START_X = 504;
const int START_Y = 229; 
const int START_W = 258;
const int START_H = 60;

const int LOAD_X = 504;
const int LOAD_Y = 299;
const int LOAD_W = 258;
const int LOAD_H = 60;

const int SETTING_X = 504;
const int SETTING_Y = 369;
const int SETTING_W = 258;
const int SETTING_H = 60;

const int EXIT_X = 504;
const int EXIT_Y = 436;
const int EXIT_W = 258;
const int EXIT_H = 60;

//============================
// ÉèÖÃ½øÈë½çÃæºóµÄ·µ»Ø°´Å¥
//============================
const int BACK_X = 570;
const int BACK_Y = 540;
const int BACK_W = 140;
const int BACK_H = 50;



GameState state = MENU;


//============================
// È«¾ÖÉèÖÃ×´Ì¬±äÁ¿
//============================
int g_bgmVol = 1;     
int g_musicVol = 1;  
int g_soundVol = 1; 
int g_selectPet = 0;
int createSaveSlot = 0; 


IMAGE menu;
IMAGE start;
IMAGE start_bg;
IMAGE character1;
IMAGE character2;
IMAGE character3;
IMAGE load;
IMAGE setting;
IMAGE quit;
IMAGE defeat;
IMAGE victory;


void LoadMenuResource()
{
    loadimage(&menu, L"res\\menu.png");
    loadimage(&start, L"res\\start.png");
    loadimage(&start_bg, L"res\\start_bg.png"); 
    loadimage(&character1, L"res\\character1.png");
    loadimage(&character2, L"res\\character2.png");
    loadimage(&character3, L"res\\character3.png");
    loadimage(&load, L"res\\load.png");
    loadimage(&setting, L"res\\setting.png");
    loadimage(&quit, L"res\\exit.png");
    loadimage(&defeat, L"res\\defeat.jpg"); 
    loadimage(&victory, L"res\\victory.jpg");
}

void UpdateBgmVolume()
{
    int vol;
    if (g_bgmVol == 2) vol = 550;
    else if (g_bgmVol == 1) vol = 370;
    else vol = 120;
    TCHAR cmd[64];
    _stprintf_s(cmd, _T("setaudio menu_bgm volume to %d"), vol);
    mciSendString(cmd, NULL, 0, NULL);
}

void UpdateBattleBgmVolume()
{
    int vol ;
    if (g_musicVol == 2) vol = 550;
    else if (g_musicVol == 1) vol = 370;
    else vol = 120;
    TCHAR cmd[64];
    _stprintf_s(cmd, _T("setaudio battle_bgm volume to %d"), vol);
    mciSendString(cmd, NULL, 0, NULL);
}

void UpdateSoundVolume()
{
    int vol;
    if (g_soundVol == 2) vol = 550;
    else if (g_soundVol == 1) vol = 370;
    else vol = 120;
    TCHAR cmd[64];
    _stprintf_s(cmd, _T("setaudio click volume to %d"), vol);
    mciSendString(cmd, NULL, 0, NULL);
}

void InitMenu()
{
    LoadMenuResource();
    mciSendString(_T("open res\\menu_bgm.mp3 alias menu_bgm type mpegvideo"), NULL, 0, NULL);
    mciSendString(_T("setaudio menu_bgm volume to 370"), NULL, 0, NULL);
    mciSendString(_T("play menu_bgm repeat"), NULL, 0, NULL);
    mciSendString(_T("open res\\click.mp3 alias click"), NULL, 0, NULL);
    LoadBattleResources();
    mciSendString(_T("open res\\victory_bgm.mp3 alias victory_bgm type mpegvideo"), NULL, 0, NULL);
    mciSendString(_T("open res\\defeat_bgm.mp3 alias defeat_bgm type mpegvideo"), NULL, 0, NULL);
    UpdateSoundVolume();
}


void PlayClickSound()
{
    mciSendString(_T("seek click to start"), NULL, 0, NULL);
    mciSendString(_T("play click"), NULL, 0, NULL);
}

void DrawMenu()
{
    putimage(0, 0, &menu);

    putimage(504, 229, &start);
    putimage(504, 299, &load);
    putimage(504, 369, &setting);
    putimage(504, 436, &quit);

    setbkmode(TRANSPARENT);
    settextstyle(18, 0, L"Î¢ÈíÑÅºÚ");
    settextcolor(BLACK);
    outtextxy(1129, 699, L"Made by ÀîÊ÷»Ô  ÓÚ³¯¾¸");
    outtextxy(1131, 701, L"Made by ÀîÊ÷»Ô  ÓÚ³¯¾¸");
    outtextxy(1131, 699, L"Made by ÀîÊ÷»Ô  ÓÚ³¯¾¸");
    outtextxy(1129, 701, L"Made by ÀîÊ÷»Ô  ÓÚ³¯¾¸");
    settextcolor(WHITE);
    outtextxy(1130, 700, L"Made by ÀîÊ÷»Ô  ÓÚ³¯¾¸");
    outtextxy(10, 700, L"C++¿Î³ÌÉè¼Æ _ Version 1.0");
}


void DrawStartGame()
{
    cleardevice();
    putimage(0, 0, &menu);
    putimage(350,120,&start_bg);

    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(40, 0,L"Î¢ÈíÑÅºÚ" );
    outtextxy(585,153,L"¿ªÊ¼ÓÎÏ·");
    
    settextstyle(22,0,L"»ªÎªÐÐ¿¬");
    outtextxy(400,215,L"Ñ¡ÔñÄãµÄ³èÎï£º");
    putimage(400, 270, &character1);
    putimage(580, 270, &character2);
    putimage(760, 270, &character3);
    settextstyle(30, 0, L"ËÎÌå");
    settextcolor(RGB(255, 160, 200));
    outtextxy(430, 430, L"Ð¡³à·æ");
    outtextxy(620, 430, L"ÐÇÁð");
    outtextxy(790, 430, L"¶Ü¶Õ¶Õ");

    setfillcolor(RGB(80, 180, 255));
    settextstyle(28, 0, L"»ªÎªÐÐ¿¬");
    solidrectangle(BACK_X,BACK_Y,BACK_X + BACK_W,BACK_Y + BACK_H);
    settextcolor(WHITE);
    outtextxy(615,553,L"·µ»Ø");
}



void DrawLoadGame()
{
    putimage(0, 0, &menu);

    setfillcolor(RGB(0, 0, 0, 130));
    solidrectangle(0, 0, 1280, 720);

    setfillcolor(RGB(240, 240, 240));
    solidrectangle(350, 120, 930, 620);
    setlinecolor(BLACK);
    rectangle(350, 120, 930, 620);

    settextcolor(BLACK);
    settextstyle(40, 0, L"Î¢ÈíÑÅºÚ");
    outtextxy(585, 155, L"¶ÁÈ¡´æµµ");
    settextstyle(30, 0, L"Î¢ÈíÑÅºÚ");
    outtextxy(400, 250, L"´æµµ1");
    settextcolor(RGB(0, 80, 180));
    outtextxy(400, 290, L"´´½¨´æµµ1");
    settextcolor(RGB(180, 0, 0));
    outtextxy(630, 290, L"É¾³ý´æµµ1");

    settextcolor(BLACK);
    outtextxy(400, 350, L"´æµµ2");
    settextcolor(RGB(0, 80, 180));
    outtextxy(400, 390, L"´´½¨´æµµ2");
    settextcolor(RGB(180, 0, 0));
    outtextxy(630, 390, L"É¾³ý´æµµ2");

    settextcolor(BLACK);
    outtextxy(400, 450, L"´æµµ3");
    settextcolor(RGB(0, 80, 180));
    outtextxy(400, 490, L"´´½¨´æµµ3");
    settextcolor(RGB(180, 0, 0));
    outtextxy(630, 490, L"É¾³ý´æµµ3");

    settextcolor(BLACK);
    outtextxy(618, 555, L"·µ»Ø");
}
void DrawSetting()
{
    putimage(0, 0, &menu);
    setfillcolor(RGB(250, 240, 220));
    solidrectangle(360, 120, 920, 620);
    setlinecolor(BLACK);
    rectangle(360, 120, 920, 620);
    setbkmode(TRANSPARENT);
    settextstyle(28, 0, L"Î¢ÈíÑÅºÚ");

    settextcolor(BLACK);
    settextstyle(40, 0, L"Î¢ÈíÑÅºÚ");
    outtextxy(610, 150, L"ÉèÖÃ");
    settextstyle(28, 0, L"Î¢ÈíÑÅºÚ");

    outtextxy(430, 250, L"±³¾°ÒôÀÖ");
    settextcolor(g_bgmVol == 2 ? RED : BLACK);
    outtextxy(600, 250, L"´ó");
    settextcolor(g_bgmVol == 1 ? RED : BLACK);
    outtextxy(700, 250, L"ÖÐ");
    settextcolor(g_bgmVol == 0 ? RED : BLACK);
    outtextxy(800, 250, L"Ð¡");

    settextcolor(BLACK);
    outtextxy(430, 320, L"ÒôÀÖÒôÁ¿");
    settextcolor(g_musicVol == 2 ? RED : BLACK);
    outtextxy(600, 320, L"´ó");
    settextcolor(g_musicVol == 1 ? RED : BLACK);
    outtextxy(700, 320, L"ÖÐ");
    settextcolor(g_musicVol == 0 ? RED : BLACK);
    outtextxy(800, 320, L"Ð¡");

    settextcolor(BLACK);
    outtextxy(430, 390, L"ÒôÐ§ÒôÁ¿");
    settextcolor(g_soundVol == 2 ? RED : BLACK);
    outtextxy(600, 390, L"´ó");
    settextcolor(g_soundVol == 1 ? RED : BLACK);
    outtextxy(700, 390, L"ÖÐ");
    settextcolor(g_soundVol == 0 ? RED : BLACK);
    outtextxy(800, 390, L"Ð¡");

    setfillcolor(RGB(80, 180, 255));
    solidrectangle(BACK_X, BACK_Y, BACK_X + BACK_W, BACK_Y + BACK_H);
    settextcolor(WHITE);
    outtextxy(618, 552, L"·µ»Ø");
}

void DrawVictory()
{
    putimage(0, 0, &victory);

    setfillcolor(RGB(255, 255, 255));
    solidrectangle(400, 200, 880, 640);
    rectangle(400, 200, 880, 640);

    settextcolor(RGB(0, 160, 0));
    settextstyle(48, 0, L"Î¢ÈíÑÅºÚ");
    outtextxy(568, 240, L"Õ½¶·Ê¤Àû£¡");

    settextstyle(30, 0, L"Î¢ÈíÑÅºÚ");
    settextcolor(BLACK);
    outtextxy(460, 340, L"Äã³É¹¦»÷°ÜÁËÄ§Îï£¡");

    setfillcolor(RGB(80, 180, 255));
    solidrectangle(BACK_X, BACK_Y, BACK_X + BACK_W, BACK_Y + BACK_H);
    settextcolor(WHITE);
    outtextxy(587, 550, L"·µ»ØÖ÷²Ëµ¥");
}

void DrawDefeat()
{
    putimage(0, 0, &defeat);


    setfillcolor(RGB(255, 255, 255));
    solidrectangle(400, 200, 880, 640);
    rectangle(400, 200, 880, 640);

    settextcolor(RGB(180, 0, 0));
    settextstyle(48, 0, L"Î¢ÈíÑÅºÚ");
    outtextxy(568, 240, L"Õ½¶·Ê§°Ü");

    settextstyle(30, 0, L"Î¢ÈíÑÅºÚ");
    settextcolor(BLACK);
    outtextxy(460, 340, L"ÄãµÄ³èÎï±»Ä§Îï»÷°ÜÁË...");

    setfillcolor(RGB(80, 180, 255));
    solidrectangle(BACK_X, BACK_Y, BACK_X + BACK_W, BACK_Y + BACK_H);
    settextcolor(WHITE);
    outtextxy(587, 550, L"·µ»ØÖ÷²Ëµ¥");
}


void ShowMenu()
{  
    LoadMenuResource();
    InitMenu();
    BeginBatchDraw();

    ExMessage msg;
    while (true)
    {
        //Êó±êµã»÷ÐÅÏ¢½ÓÊÕ
        if (peekmessage(&msg, EX_MOUSE))
        {
            if (msg.message == WM_LBUTTONDOWN)
            {
                if (state == MENU)
                {
                    if (msg.x >= START_X &&
                        msg.x <= START_X + START_W &&
                        msg.y >= START_Y &&
                        msg.y <= START_Y + START_H)
                    {
                        PlayClickSound();
                        state = STARTGAME;
                    }

                    if (msg.x >= LOAD_X &&
                        msg.x <= LOAD_X + LOAD_W &&
                        msg.y >= LOAD_Y &&
                        msg.y <= LOAD_Y + LOAD_H)
                    {
                        PlayClickSound();
                        state = LOADGAME;
                    }

                    if (msg.x >= SETTING_X &&
                        msg.x <= SETTING_X + SETTING_W &&
                        msg.y >= SETTING_Y &&
                        msg.y <= SETTING_Y + SETTING_H)
                    {
                        PlayClickSound();
                        state = SETTING;
                    }

                    if (msg.x >= EXIT_X &&
                        msg.x <= EXIT_X + EXIT_W &&
                        msg.y >= EXIT_Y &&
                        msg.y <= EXIT_Y + EXIT_H)
                    {
                        EndBatchDraw();
                        mciSendString(_T("stop menu_bgm"),NULL, 0, NULL);
                        mciSendString( _T("close menu_bgm"),NULL, 0, NULL);
                        mciSendString(_T("stop battle_bgm"), NULL, 0, NULL);
                        mciSendString(_T("close battle_bgm"), NULL, 0, NULL);
                        closegraph();
                        exit(0);
                    }
                }

                else if (state == STARTGAME)
                {
                    if (msg.x >= BACK_X && msg.x <= BACK_X + BACK_W && msg.y >= BACK_Y && msg.y <= BACK_Y + BACK_H)
                    {
                        PlayClickSound();
                        createSaveSlot = 0;
                        state = MENU;
                    }

                    // ========== ³èÎï1 Ð¡³à·æ ==========
                    if (msg.x >= 400 && msg.x <= 560 && msg.y >= 270 && msg.y <= 460)
                    {
                        PlayClickSound();
                        g_selectPet = 1;

                        // Èç¹ûÊÇ´´½¨´æµµÁ÷³Ì£¬ÏÈÉú³É´æµµÎÄ¼þ
                        if (createSaveSlot != 0)
                        {
                            CreateEmptySave(createSaveSlot, g_selectPet);
                            createSaveSlot = 0;
                        }

                        if (player_pet != nullptr)
                        {
                            delete player_pet;
                            player_pet = nullptr;
                        }
                        player_pet = new WarriorPet(L"Ð¡³à·æ", &img_WarriorPet);
                        InitBattleByPet(g_selectPet);

                        mciSendString(_T("stop menu_bgm"), NULL, 0, NULL);
                        mciSendString(_T("close menu_bgm"), NULL, 0, NULL);
                        mciSendString(_T("open res\\battle.mp3 alias battle_bgm type mpegvideo"), NULL, 0, NULL);
                        UpdateBattleBgmVolume();
                        mciSendString(_T("play battle_bgm repeat"), NULL, 0, NULL);
                        state = BATTLE;
                    }

                    // ========== ³èÎï2 ÐÇÁð ==========
                    if (msg.x >= 580 && msg.x <= 740 && msg.y >= 270 && msg.y <= 460)
                    {
                        PlayClickSound();
                        g_selectPet = 2;

                        if (createSaveSlot != 0)
                        {
                            CreateEmptySave(createSaveSlot, g_selectPet);
                            createSaveSlot = 0;
                        }

                        if (player_pet != nullptr)
                        {
                            delete player_pet;
                            player_pet = nullptr;
                        }
                        player_pet = new MagePet(L"ÐÇÁð", &img_MagePet);
                        InitBattleByPet(g_selectPet);

                        mciSendString(_T("stop menu_bgm"), NULL, 0, NULL);
                        mciSendString(_T("close menu_bgm"), NULL, 0, NULL);
                        mciSendString(_T("open res\\battle.mp3 alias battle_bgm type mpegvideo"), NULL, 0, NULL);
                        UpdateBattleBgmVolume();
                        mciSendString(_T("play battle_bgm repeat"), NULL, 0, NULL);
                        state = BATTLE;
                    }

                    // ========== ³èÎï3 ¶Ü¶Õ¶Õ ==========
                    if (msg.x >= 760 && msg.x <= 920 && msg.y >= 270 && msg.y <= 460)
                    {
                        PlayClickSound();
                        g_selectPet = 3;

                        if (createSaveSlot != 0)
                        {
                            CreateEmptySave(createSaveSlot, g_selectPet);
                            createSaveSlot = 0;
                        }

                        if (player_pet != nullptr)
                        {
                            delete player_pet;
                            player_pet = nullptr;
                        }
                        player_pet = new TankPet(L"¶Ü¶Õ¶Õ", &img_TankPet);
                        InitBattleByPet(g_selectPet);

                        mciSendString(_T("stop menu_bgm"), NULL, 0, NULL);
                        mciSendString(_T("close menu_bgm"), NULL, 0, NULL);
                        mciSendString(_T("open res\\battle.mp3 alias battle_bgm type mpegvideo"), NULL, 0, NULL);
                        UpdateBattleBgmVolume();
                        mciSendString(_T("play battle_bgm repeat"), NULL, 0, NULL);
                        state = BATTLE;
                    }
                }

                else if (state == BATTLE)
                {
                    PlayClickSound();
                    BattleInput(msg);
                }

                else if (state == LOADGAME)
                {
                    if (msg.x >= BACK_X && msg.x <= BACK_X + BACK_W &&
                        msg.y >= BACK_Y && msg.y <= BACK_Y + BACK_H)
                    {
                        PlayClickSound();
                        state = MENU;
                    }

                    // ===================== ´æµµ1 =====================
                    // ¶ÁÈ¡´æµµ1
                    if (msg.x >= 370 && msg.x <= 520 && msg.y >= 220 && msg.y <= 270)
                    {
                        PlayClickSound();
                        wprintf(L"µã»÷¶ÁÈ¡´æµµ1\n");
                        if (LoadGame(1))
                        {
                            // ÇÐ»»Õ½¶·BGM
                            mciSendString(_T("stop menu_bgm"), NULL, 0, NULL);
                            mciSendString(_T("close menu_bgm"), NULL, 0, NULL);
                            mciSendString(_T("open res\\battle.mp3 alias battle_bgm type mpegvideo"), NULL, 0, NULL);
                            UpdateBattleBgmVolume();
                            mciSendString(_T("play battle_bgm repeat"), NULL, 0, NULL);
                            state = BATTLE;
                        }
                        else
                            wprintf(L"´æµµ1²»´æÔÚ\n");
                    }
                    // ´´½¨´æµµ1£º±ê¼Ç±àºÅ£¬ÌøÑ¡³èÎï½çÃæ
                    if (msg.x >= 370 && msg.x <= 530 && msg.y >= 280 && msg.y <= 315)
                    {
                        PlayClickSound();
                        createSaveSlot = 1;
                        state = STARTGAME;
                    }
                    // É¾³ý´æµµ1
                    if (msg.x >= 610 && msg.x <= 760 && msg.y >= 280 && msg.y <= 315)
                    {
                        PlayClickSound();
                        DeleteSaveFile(1);
                    }

                    // ===================== ´æµµ2 =====================
                    if (msg.x >= 370 && msg.x <= 520 && msg.y >= 320 && msg.y <= 370)
                    {
                        PlayClickSound();
                        wprintf(L"µã»÷¶ÁÈ¡´æµµ2\n");
                        if (LoadGame(2))
                        {
                            mciSendString(_T("stop menu_bgm"), NULL, 0, NULL);
                            mciSendString(_T("close menu_bgm"), NULL, 0, NULL);
                            mciSendString(_T("open res\\battle.mp3 alias battle_bgm type mpegvideo"), NULL, 0, NULL);
                            UpdateBattleBgmVolume();
                            mciSendString(_T("play battle_bgm repeat"), NULL, 0, NULL);
                            state = BATTLE;
                        }
                        else
                            wprintf(L"´æµµ2²»´æÔÚ\n");
                    }
                    if (msg.x >= 370 && msg.x <= 530 && msg.y >= 380 && msg.y <= 415)
                    {
                        PlayClickSound();
                        createSaveSlot = 2;
                        state = STARTGAME;
                    }
                    if (msg.x >= 610 && msg.x <= 760 && msg.y >= 380 && msg.y <= 415)
                    {
                        PlayClickSound();
                        DeleteSaveFile(2);
                    }

                    // ===================== ´æµµ3 =====================
                    if (msg.x >= 370 && msg.x <= 520 && msg.y >= 420 && msg.y <= 470)
                    {
                        PlayClickSound();
                        wprintf(L"µã»÷¶ÁÈ¡´æµµ3\n");
                        if (LoadGame(3))
                        {
                            mciSendString(_T("stop menu_bgm"), NULL, 0, NULL);
                            mciSendString(_T("close menu_bgm"), NULL, 0, NULL);
                            mciSendString(_T("open res\\battle.mp3 alias battle_bgm type mpegvideo"), NULL, 0, NULL);
                            UpdateBattleBgmVolume();
                            mciSendString(_T("play battle_bgm repeat"), NULL, 0, NULL);
                            state = BATTLE;
                        }
                        else
                            wprintf(L"´æµµ3²»´æÔÚ\n");
                    }
                    if (msg.x >= 370 && msg.x <= 530 && msg.y >= 480 && msg.y <= 515)
                    {
                        PlayClickSound();
                        createSaveSlot = 3;
                        state = STARTGAME;
                    }
                    if (msg.x >= 610 && msg.x <= 760 && msg.y >= 480 && msg.y <= 515)
                    {
                        PlayClickSound();
                        DeleteSaveFile(3);
                    }
                }

                
                else if (state == SETTING)
                {
                    if (msg.x >= BACK_X &&
                        msg.x <= BACK_X + BACK_W &&
                        msg.y >= BACK_Y &&
                        msg.y <= BACK_Y + BACK_H)
                    {
                        PlayClickSound();
                        state = MENU;
                    }
                    if (msg.y > 220 && msg.y < 280)
                    {
                        if (msg.x > 570 && msg.x < 630)
                        {
                            g_bgmVol = 2;
                            UpdateBgmVolume();
                            PlayClickSound();
                        }
                        if (msg.x > 670 && msg.x < 730)
                        {
                            g_bgmVol = 1;
                            UpdateBgmVolume();
                            PlayClickSound();
                        }
                        if (msg.x > 770 && msg.x < 830)
                        {
                            g_bgmVol = 0;
                            UpdateBgmVolume();
                            PlayClickSound();
                        }
                    }

                    if (msg.y > 290 && msg.y < 350)
                    {
                        if (msg.x > 570 && msg.x < 630) 
                        { 
                            g_musicVol = 2; 
                            UpdateBattleBgmVolume();
                            PlayClickSound(); 
                        }
                        if (msg.x > 670 && msg.x < 730) 
                        {
                            g_musicVol = 1; 
                            UpdateBattleBgmVolume();
                            PlayClickSound(); }
                        if (msg.x > 770 && msg.x < 830)
                        { 
                            g_musicVol = 0; 
                            UpdateBattleBgmVolume(); 
                            PlayClickSound(); 
                        }
                    }

                    if (msg.y > 360 && msg.y < 420)
                    {
                        if (msg.x > 570 && msg.x < 630)
                        {
                            g_soundVol = 2;
                            UpdateSoundVolume();
                            PlayClickSound();
                        }
                        if (msg.x > 670 && msg.x < 730)
                        {
                            g_soundVol = 1;
                            UpdateSoundVolume();
                            PlayClickSound();
                        }
                        if (msg.x > 770 && msg.x < 830)
                        {
                            g_soundVol = 0;
                            UpdateSoundVolume();
                            PlayClickSound();
                        }
                    }
                   
                }


                else if (state == VICTORY || state == DEFEAT)
                {
                    if (msg.x >= BACK_X && msg.x <= BACK_X + BACK_W &&
                        msg.y >= BACK_Y && msg.y <= BACK_Y + BACK_H)
                    {
                        PlayClickSound();
                        if (state == VICTORY)
                        {  
                            mciSendString(_T("stop victory_bgm"), NULL, 0, NULL);
                            mciSendString(_T("close victory_bgm"), NULL, 0, NULL);
                        }
                        else
                        {
                            mciSendString(_T("stop defeat_bgm"), NULL, 0, NULL);
                            mciSendString(_T("close defeat_bgm"), NULL, 0, NULL);
                        }
                        mciSendString(_T("open res\\menu_bgm.mp3 alias menu_bgm type mpegvideo"), NULL, 0, NULL);
                        UpdateBgmVolume();
                        mciSendString(_T("seek menu_bgm to start"), NULL, 0, NULL);
                        mciSendString(_T("play menu_bgm repeat"), NULL, 0, NULL);
                        state = MENU;
                    }
}
            }
        }

        switch (state)
        {

        case MENU:
            DrawMenu();
            break;

        case SETTING:
            DrawSetting();
            break;


        case STARTGAME:
            DrawStartGame();
            break;

        case LOADGAME:
            DrawLoadGame();
            break;
        case BATTLE:
            DrawBattle();
            break;
        case VICTORY:
            DrawVictory();
            break;
        case DEFEAT:
            DrawDefeat();
            break;
        }
        FlushBatchDraw();
        Sleep(10);
    }
}