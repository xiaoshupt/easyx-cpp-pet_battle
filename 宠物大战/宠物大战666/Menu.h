#pragma once
#ifndef MENU_H
#define MENU_H

#include <graphics.h>


enum GameState
{
    MENU,
    SETTING,
    STARTGAME,
    LOADGAME,
    BATTLE,
	VICTORY,
	DEFEAT,
};


extern const int START_X;
extern const int START_Y;
extern const int START_W;
extern const int START_H;

extern const int LOAD_X;
extern const int LOAD_Y;
extern const int LOAD_W;
extern const int LOAD_H;

extern const int SETTING_X;
extern const int SETTING_Y;
extern const int SETTING_W;
extern const int SETTING_H;

extern const int EXIT_X;
extern const int EXIT_Y;
extern const int EXIT_W;
extern const int EXIT_H;

extern const int BACK_X;
extern const int BACK_Y;
extern const int BACK_W;
extern const int BACK_H;


extern IMAGE menu;
extern IMAGE start;
extern IMAGE start_bg;
extern IMAGE character1;
extern IMAGE character2;
extern IMAGE character3;
extern IMAGE load;
extern IMAGE setting;
extern IMAGE quit;

extern GameState state;
extern int g_bgmVol;
extern int g_musicVol;
extern int g_soundVol;
extern int g_selectPet;


void LoadMenuResource();
void UpdateBgmVolume();
void UpdateBattleBgmVolume();
void UpdateSoundVolume();
void InitMenu();
void PlayClickSound();
void DrawMenu();
void DrawStartGame();
void DrawLoadGame();
void DrawSetting();
void ShowMenu();
void DrawBattle();

#endif