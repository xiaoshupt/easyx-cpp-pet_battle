#pragma once
#ifndef BATTLE_H
#define BATTLE_H

#include <graphics.h>
#include <vector>
#include <string>
#include <windows.h>
#include "Menu.h"

extern bool show_bag;
extern bool is_quit;
extern int enemy_hp;
extern int enemy_max_hp;
extern int skill4_round;
extern bool tank_reflect;
extern bool is_defend;
extern void UpdateBgmVolume();
extern void UpdateBattleBgmVolume();
extern void UpdateSoundVolume();


// 背包道具结构体
struct Item
{
    std::wstring name;
    int count;
    int heal;
    Item(std::wstring n, int c, int h) : name(n), count(c), heal(h) {}
};
extern std::vector<Item> bag_items;

// 战斗按钮、背包关闭框
extern std::vector<std::pair<std::wstring, ::RECT>> battle_buttons;
extern ::RECT bag_close_rect;

extern IMAGE img_MagePet;
extern IMAGE img_WarriorPet;
extern IMAGE img_TankPet;
extern IMAGE img_Monster;
extern IMAGE img_bround;
extern IMAGE img_setting_battle;


// 基础属性
class Attribute
{
protected:
    int hp;
    int max_hp;
    int atk;
    int def;
    int speed;
    int mp;
    int max_mp;
public:
    Attribute(int hp_, int atk_, int def_, int sp_, int mp_);
    void addHp(int val);
    void addMp(int val);
    int getHp() const;
    int getMaxHp() const;
    int getAtk() const;
    int getDef() const;
    int getSpeed() const;
    int getMp() const;
    int getMaxMp() const;
    void setMp(int val);
    void fullHp();
    virtual ~Attribute() = default;
};

// 宠物基类
class PetBase
{
protected:
    std::wstring pet_name;
    std::wstring pet_type;
    IMAGE* pet_img;
    Attribute attr;
public:
    PetBase(std::wstring name, std::wstring type, IMAGE* img,
        int hp_, int atk_, int def_, int sp_, int mp_);
    void addHp(int val);
    void addMp(int val);
    int getHp() const;
    int getMaxHp() const;
    int getAtk() const;
    int getDef() const;
    int getSpeed() const;
    int getMp() const;
    void fullHp();
    int getBaseDamage();
    void drawPet(int x, int y);
    void setMp(int val);
    int getMaxMp() const;
    std::wstring getName() const;
    std::wstring getType() const;
    virtual ~PetBase() = default;
};

// 三类宠物子类
class WarriorPet : public PetBase
{
public:
    WarriorPet(std::wstring name, IMAGE* img);
};
class MagePet : public PetBase
{
public:
    MagePet(std::wstring name, IMAGE* img);
};
class TankPet : public PetBase
{
public:
    TankPet(std::wstring name, IMAGE* img);
    int reflectDmg(int enemyDmg);
};

// 玩家宠物全局指针
extern PetBase* player_pet;


void LoadBattleResources();
void draw_hp_bar(int x, int y, int w, int h, int cur, int max);
void draw_tip(const wchar_t* text, int x, int y, COLORREF c = WHITE);
void draw_bag_window();
void handle_bag_click(int x, int y);
void enemy_attack();
void DrawBattle();
void BattleInput(const ExMessage& msg);
void InitBattleByPet(int petId);
void SettingBattle();
#endif