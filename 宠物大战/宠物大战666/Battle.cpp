#include "Battle.h"
#include <cstdio>
#include "SaveData.h"
#include <fstream>

// 全局标记
bool show_bag = false;
bool is_quit = false;
int enemy_hp = 100;
int enemy_max_hp = 100;
int skill4_round = 0;
bool tank_reflect = false;
bool is_defend = false;
bool showBattleSetting = false;

// 背包道具
std::vector<Item> bag_items = {
    {L"小型回血药", 3, 30},
    {L"大型回血药", 1, 60},
    {L"魔力药水", 2, 30}
};

// 底部技能按钮
std::vector<std::pair<std::wstring, ::RECT>> battle_buttons = {
    {L"撞击", {30, 370, 130, 410}},
    {L"远程", {30, 430, 130, 470}},
    {L"防御", {30, 490, 130, 530}},
    {L"自辅", {30, 550, 130, 590}},
    {L"背包", {30, 610, 130, 650}},
    {L"逃跑", {30, 670, 130, 710}}
};
::RECT bag_close_rect = { 550, 387, 570, 414 };

// 图片资源
IMAGE img_MagePet;
IMAGE img_WarriorPet;
IMAGE img_TankPet;
IMAGE img_Monster;
IMAGE img_bround;
IMAGE img_setting_battle;


PetBase* player_pet = nullptr;

// ===================== Attribute实现 =====================
Attribute::Attribute(int hp_, int atk_, int def_, int sp_, int mp_)
    : hp(hp_), max_hp(hp_), atk(atk_), def(def_), speed(sp_), mp(mp_),max_mp(mp_) {
}
void Attribute::addHp(int val)
{
    hp += val;
    if (hp > max_hp) hp = max_hp;
    if (hp < 0) hp = 0;
}
void Attribute::addMp(int val)
{
    mp += val;
    if (mp < 0) mp = 0;
}
int Attribute::getHp() const { return hp; }
int Attribute::getMaxHp() const { return max_hp; }
int Attribute::getAtk() const { return atk; }
int Attribute::getDef() const { return def; }
int Attribute::getSpeed() const { return speed; }
int Attribute::getMp() const { return mp; }
void Attribute::fullHp() { hp = max_hp; }
int Attribute::getMaxMp() const {
    return max_mp;
}
void Attribute::setMp(int val)
{
    if (val < 0) val = 0;
    if (val > max_mp) val = max_mp;
    mp = val;
}

// ===================== PetBase实现 =====================
PetBase::PetBase(std::wstring name, std::wstring type, IMAGE* img,
    int hp_, int atk_, int def_, int sp_, int mp_)
    : pet_name(name), pet_type(type), pet_img(img), attr(hp_, atk_, def_, sp_, mp_) {
}
void PetBase::addHp(int val) { attr.addHp(val); }
void PetBase::addMp(int val) { attr.addMp(val); }
int PetBase::getHp() const { return attr.getHp(); }
int PetBase::getMaxHp() const { return attr.getMaxHp(); }
int PetBase::getAtk() const { return attr.getAtk(); }
int PetBase::getDef() const { return attr.getDef(); }
int PetBase::getSpeed() const { return attr.getSpeed(); }
int PetBase::getMp() const { return attr.getMp(); }
void PetBase::fullHp() { attr.fullHp(); }
int PetBase::getBaseDamage()
{
    int dmg = attr.getAtk();
    if (skill4_round > 0)
        dmg = dmg * 140 / 100;
    return dmg;
}
void PetBase::drawPet(int x, int y)
{
    putimage(x, y, pet_img);
    //settextcolor(WHITE);
    //settextstyle(22, 0, _T("宋体"));
    //setbkmode(TRANSPARENT);
    //outtextxy(x + 20, y + 130, pet_name.c_str());
    //outtextxy(x + 20, y + 155, pet_type.c_str());
}
void PetBase::setMp(int val)
{
    attr.setMp(val);
}
int PetBase::getMaxMp() const {
    return attr.getMaxMp();
};
std::wstring PetBase::getName() const { return pet_name; }
std::wstring PetBase::getType() const { return pet_type; }

// ===================== 子类实现 =====================
WarriorPet::WarriorPet(std::wstring name, IMAGE* img)
    : PetBase(name, L"小赤锋", img, 100, 20, 8, 12, 60) {
}
MagePet::MagePet(std::wstring name, IMAGE* img)
    : PetBase(name, L"星琉", img, 80, 18, 5, 18, 100) {
}
TankPet::TankPet(std::wstring name, IMAGE* img)
    : PetBase(name, L"盾墩墩", img, 150, 12, 18, 6, 40) {
}
int TankPet::reflectDmg(int enemyDmg)
{
    return enemyDmg * 40 / 100;
}

//工具函数

void SettingBattle()
{
    putimage(0, 0, &img_bround);

    setfillcolor(RGB(245, 245, 245));
    solidrectangle(450, 180, 950, 660);
    setlinecolor(BLACK);
    rectangle(450, 180, 950, 660);

    settextcolor(BLACK);
    settextstyle(32, 0, _T("微软雅黑"));
    draw_tip(L"战斗设置", 620, 200, BLACK);

    settextstyle(24, 0, _T("微软雅黑"));

    draw_tip(L"战斗背景音乐：", 480, 260, BLACK);
    settextcolor(g_musicVol == 2 ? RED : BLACK);
    draw_tip(L"大", 660, 260, g_musicVol == 2 ? RED : BLACK);
    settextcolor(g_musicVol == 1 ? RED : BLACK);
    draw_tip(L"中", 720, 260, g_musicVol == 1 ? RED : BLACK);
    settextcolor(g_musicVol == 0 ? RED : BLACK);
    draw_tip(L"小", 780, 260, g_musicVol == 0 ? RED : BLACK);

    settextcolor(BLACK);
    draw_tip(L"技能点击音效：", 480, 340, BLACK);
    settextcolor(g_soundVol == 2 ? RED : BLACK);
    draw_tip(L"大", 660, 340, g_soundVol == 2 ? RED : BLACK);
    settextcolor(g_soundVol == 1 ? RED : BLACK);
    draw_tip(L"中", 720, 340, g_soundVol == 1 ? RED : BLACK);
    settextcolor(g_soundVol == 0 ? RED : BLACK);
    draw_tip(L"小", 780, 340, g_soundVol == 0 ? RED : BLACK);

    draw_tip(L"保存存档：", 480, 430, BLACK);
    draw_tip(L"存档1", 620, 430, RGB(0, 100, 200));
    draw_tip(L"存档2", 700, 430, RGB(0, 100, 200));
    draw_tip(L"存档3", 780, 430, RGB(0, 100, 200));

    setfillcolor(RGB(200, 40, 40));
    solidrectangle(890, 190, 930, 230);
    rectangle(890, 190, 930, 230);
    settextcolor(WHITE);
    draw_tip(L"X", 902, 198, WHITE);
}



void LoadBattleResources()
{
    loadimage(&img_WarriorPet, _T("res\\WarriorPet.png"));
    loadimage(&img_MagePet, _T("res\\MagePet.png"));
    loadimage(&img_TankPet, _T("res\\TankPet.png"));
    loadimage(&img_Monster, _T("res\\Monster.png"));
    loadimage(&img_bround, _T("res\\bround.png"));
    loadimage(&img_setting_battle, _T("res\\setting_battle.jpg"));
}

void draw_hp_bar(int x, int y, int w, int h, int cur, int max)
{
    setfillcolor(RGB(80, 80, 80));
    solidrectangle(x, y, x + w, y + h);
    int hpw = cur * w / max;
    setfillcolor(RGB(220, 60, 60));
    solidrectangle(x, y, x + hpw, y + h);
    setlinecolor(WHITE);
    rectangle(x, y, x + w, y + h);
}
void draw_tip(const wchar_t* text, int x, int y, COLORREF c)
{
    settextcolor(c);
    settextstyle(22, 0, _T("宋体"));
    setbkmode(TRANSPARENT);
    outtextxy(x, y, text);
}
void draw_bag_window()
{
    setfillcolor(RGB(40, 40, 40));
    solidrectangle(380, 380, 580, 530);
    setlinecolor(WHITE);
    rectangle(380,380, 580, 530);
    draw_tip(L"背包", 453, 387, YELLOW);

    setfillcolor(RGB(180, 30, 30));
    solidrectangle(bag_close_rect.left, bag_close_rect.top, bag_close_rect.right, bag_close_rect.bottom);
    rectangle(bag_close_rect.left, bag_close_rect.top, bag_close_rect.right, bag_close_rect.bottom);
    draw_tip(L"X", 555, 390);

    int y = 423;
    for (auto& item : bag_items)
    {
        wchar_t buf[128];
        _stprintf_s(buf, _T("%ls x%d"), item.name.c_str(), item.count);
        draw_tip(buf, 420, y);
        y += 40;
    }
}
void handle_bag_click(int x, int y)
{
    if (player_pet == nullptr)
        return;
    if (x >= bag_close_rect.left && x <= bag_close_rect.right &&
        y >= bag_close_rect.top && y <= bag_close_rect.bottom)
    {
        show_bag = false;
        return;
    }
    int yItem = 423;
    for (auto& item : bag_items)
    {
        if (x >= 420 && x <= 500 && y >= yItem && y <= yItem + 35)
        {
            if (item.count <= 0) break;
            if (item.heal > 0)
            {
                if (item.name.find(L"魔力") != std::wstring::npos)
                    player_pet->addMp(item.heal);
                else
                    player_pet->addHp(item.heal);
                item.count--;
            }
            break;
        }
        yItem += 40;
    }
}
void enemy_attack()
{
    if (player_pet == nullptr)
        return;
    int rawDmg = 25;
    int reduceRate = player_pet->getDef();
    int finalDmg = rawDmg * (100 - reduceRate) / 100;
    if (is_defend) finalDmg /= 2;

    player_pet->addHp(-finalDmg);

    TankPet* tank = dynamic_cast<TankPet*>(player_pet);
    if (tank != nullptr && tank_reflect)
    {
        int reflect = tank->reflectDmg(finalDmg);
        enemy_hp -= reflect;
        if (enemy_hp < 0) enemy_hp = 0;
    }
    is_defend = false;
}

//初始化战斗
void InitBattleByPet(int petId)
{
    enemy_hp = enemy_max_hp;
    skill4_round = 0;
    tank_reflect = false;
    is_defend = false;
    show_bag = false;
}


void DrawBattle()
{
    if (player_pet == nullptr)
        return;
    cleardevice();
    putimage(0, 0, &img_bround);

    settextcolor(WHITE);
    settextstyle(24, 0, _T("宋体"));
    setbkmode(TRANSPARENT);

    TCHAR petBuf[128];
    _stprintf_s(petBuf, _T("%ls HP:%d MP:%d 增伤回合:%d"),
        player_pet->getName().c_str(),
        player_pet->getHp(),
        player_pet->getMp(),
        skill4_round);
    outtextxy(105, 320, petBuf);
    draw_hp_bar(145, 350, 160, 24, player_pet->getHp(), player_pet->getMaxHp());

    TCHAR enBuf[64];
    _stprintf_s(enBuf, _T("魔物 HP:%d"), enemy_hp);
    outtextxy(1060, 190, enBuf);
    draw_hp_bar(1045, 220, 160, 24, enemy_hp, enemy_max_hp);

    player_pet->drawPet(150, 380);
    putimage(1050, 250, &img_Monster);

    // 绘制技能按钮
    for (auto& btn : battle_buttons)
    {
        setfillcolor(RGB(70, 70, 70));
        solidrectangle(btn.second.left, btn.second.top, btn.second.right, btn.second.bottom);
        setlinecolor(WHITE);
        rectangle(btn.second.left, btn.second.top, btn.second.right, btn.second.bottom);
        drawtext(btn.first.c_str(), &btn.second, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    if (show_bag) draw_bag_window();

    // 绘制返回菜单按钮
    putimage(1205, 0, &img_setting_battle);
    if (showBattleSetting)
    {
        SettingBattle();
    }
}

// 鼠标逻辑
void BattleInput(const ExMessage& msg)
{
    if (msg.message != WM_LBUTTONDOWN) return;
    int x = msg.x, y = msg.y;


    if (x >= 1205 && x <= 1205 + 190 && y >= 0 && y <= 190)
    {
        PlayClickSound();
        showBattleSetting = !showBattleSetting;
        return;
    }
    if (showBattleSetting)
    {

        if (x >= 890 && x <= 930 && y >= 190 && y <= 230)
        {
            PlayClickSound();
            showBattleSetting = false;
            return;
        }

        if (x >= 650 && x <= 690 && y >= 250 && y <= 290)
        {
            g_musicVol = 2;
            UpdateBattleBgmVolume();
            PlayClickSound();
            return;
        }

        if (x >= 710 && x <= 750 && y >= 250 && y <= 290)
        {
            g_musicVol = 1;
            UpdateBattleBgmVolume();
            PlayClickSound();
            return;
        }

        if (x >= 770 && x <= 810 && y >= 250 && y <= 290)
        {
            g_musicVol = 0;
            UpdateBattleBgmVolume();
            PlayClickSound();
            return;
        }


        if (x >= 650 && x <= 690 && y >= 330 && y <= 370)
        {
            g_soundVol = 2;
            UpdateSoundVolume();
            PlayClickSound();
            return;
        }
        if (x >= 710 && x <= 750 && y >= 330 && y <= 370)
        {
            g_soundVol = 1;
            UpdateSoundVolume();
            PlayClickSound();
            return;
        }
        if (x >= 770 && x <= 810 && y >= 330 && y <= 370)
        {
            g_soundVol = 0;
            UpdateSoundVolume();
            PlayClickSound();
            return;
        }

        // 存档1保存
        if (x >= 620 && x <= 680 && y >= 420 && y <= 454)
        {
            SaveGame(1);
            PlayClickSound();
            return;
            
        }
        // 存档2保存
        if (x >= 700 && x <= 760 && y >= 420 && y <= 454)
        {
            SaveGame(2);
            PlayClickSound();
            return;
        }
        // 存档3保存
        if (x >= 780 && x <= 840 && y >= 420 && y <= 454)
        {
            SaveGame(3);
            PlayClickSound();
            return;
        }

        // 点击弹窗空白遮罩，不关闭；点击弹窗外直接关闭
        if (!(x >= 450 && x <= 950 && y >= 180 && y <= 620))
        {
            showBattleSetting = false;
            PlayClickSound();
            return;
        }
        
        return;
    }

    // 1. 遍历技能按钮
    bool hitSkill = false;
    for (auto& btn : battle_buttons)
    {
        if (!(x >= btn.second.left && x <= btn.second.right && y >= btn.second.top && y <= btn.second.bottom))
            continue;

        hitSkill = true;
        bool doEnemyTurn = true;
        tank_reflect = false;

        if (btn.first == L"撞击")
        {
            int dmg = player_pet->getBaseDamage();
            enemy_hp -= dmg;
        }
        else if (btn.first == L"远程")
        {
            if (player_pet->getMp() >= 20)
            {
                player_pet->addMp(-20);
                int dmg = player_pet->getBaseDamage() * 160 / 100;
                enemy_hp -= dmg;
            }
            else doEnemyTurn = false;
        }
        else if (btn.first == L"防御")
        {
            tank_reflect = (dynamic_cast<TankPet*>(player_pet) != nullptr);
            is_defend = true;
        }
        else if (btn.first == L"自辅")
        {
            skill4_round = 3;
        }
        else if (btn.first == L"背包")
        {
            show_bag = true;
            doEnemyTurn = false;
        }
        else if (btn.first == L"逃跑")
        {
            mciSendString(_T("stop battle_bgm"), NULL, 0, NULL);
            mciSendString(_T("close battle_bgm"), NULL, 0, NULL);
            state = MENU;
            return;
        }


    if (enemy_hp <= 0)
    {
        mciSendString(_T("stop battle_bgm"), NULL, 0, NULL);
        TCHAR volCmd[128];
        _stprintf_s(volCmd, _T("setaudio victory_bgm volume to %d"),
            g_musicVol == 2 ? 8000 : (g_musicVol == 1 ? 4000 : 1000));
        mciSendString(volCmd, NULL, 0, NULL);
        mciSendString(_T("open res\\victory.mp3 alias victory_bgm type mpegvideo"), NULL, 0, NULL);
        mciSendString(_T("seek victory_bgm to start"), NULL, 0, NULL);
        mciSendString(_T("play victory_bgm"), NULL, 0, NULL);
        state = VICTORY;
        return;
    }

    if (doEnemyTurn && enemy_hp > 0)
    {
        Sleep(300);
        enemy_attack();
        if (skill4_round > 0) skill4_round--;
    }

    if (player_pet->getHp() <= 0)
    {
        mciSendString(_T("stop battle_bgm"), NULL, 0, NULL);
        TCHAR volCmd[128];
        _stprintf_s(volCmd, _T("setaudio defeat_bgm volume to %d"),
            g_musicVol == 2 ? 8000 : (g_musicVol == 1 ? 4000 : 1000));
        mciSendString(volCmd, NULL, 0, NULL);
        mciSendString(_T("open res\\defeat.mp3 alias defeat_bgm type mpegvideo"), NULL, 0, NULL);
        mciSendString(_T("seek defeat_bgm to start"), NULL, 0, NULL);
        mciSendString(_T("play defeat_bgm"), NULL, 0, NULL);
        state = DEFEAT;
        return;
    }
    break;
    }

    if (!hitSkill)
    {
        if (show_bag)
        {
            handle_bag_click(x, y);
            return;
        }
    }


    if (!hitSkill)
    {
        if (x >= BACK_X && x <= BACK_X + BACK_W && y >= BACK_Y && y <= BACK_Y + BACK_H)
        {
            mciSendString(_T("stop battle_bgm"), NULL, 0, NULL);
            mciSendString(_T("close battle_bgm"), NULL, 0, NULL);
            state = MENU;
            return;
        }
    }
}
