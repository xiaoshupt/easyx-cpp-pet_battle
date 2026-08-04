#pragma once
#include <vector>
#include <string>

// 道具存档结构
struct SaveItem
{
    std::wstring name = L"";
    int count = 0;
    int heal = 0;
};

// 完整数据容器
struct SaveData
{
    // 宠物信息
    int petType = 0;
    int petHp = 0;
    int petMaxHp = 0;
    int petMp = 0;
    int petMaxMp = 0;

    // 全局音量设置
    int g_bgmVol = 1;
    int g_musicVol = 1;
    int g_soundVol = 1;

    // 背包全部道具
    std::vector<SaveItem> items;
};

bool SaveGame(int slot);
bool LoadGame(int slot);
bool CreateEmptySave(int slot, int petType);
bool DeleteSaveFile(int slot);