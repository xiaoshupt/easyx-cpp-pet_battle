#include "SaveData.h"
#include "Battle.h"
#include "Menu.h"
#include <fstream>
#include <sstream>
#include <codecvt>
#include <locale>
#include <cstdio>

bool SaveGame(int slot)
{
    std::locale utf8_loc(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wofstream out;
    out.imbue(utf8_loc);

    SaveData data;

    // 1. 保存宠物数据
    data.petType = g_selectPet;
    if (player_pet != nullptr)
    {
        data.petHp = player_pet->getHp();
        data.petMaxHp = player_pet->getMaxHp();
        data.petMp = player_pet->getMp();
        data.petMaxMp = player_pet->getMaxMp();
    }

    // 2. 保存音量设置
    data.g_bgmVol = g_bgmVol;
    data.g_musicVol = g_musicVol;
    data.g_soundVol = g_soundVol;

    // 3. 拷贝背包道具
    for (auto& item : bag_items)
    {
        SaveItem si;
        si.name = item.name;
        si.count = item.count;
        si.heal = item.heal;
        data.items.push_back(si);
    }

    // 打开存档文件saveX.txt
    std::wstring path = L"save" + std::to_wstring(slot) + L".txt";
    wprintf(L"【保存日志】准备创建文件：%ls\n", path.c_str());
    out.open(path);

    if (!out.is_open())
    {
        wprintf(L"【保存失败】无法创建文件，路径无写入权限/编码错误：%ls\n", path.c_str());
        return false;
    }
    wprintf(L"【保存成功】文件创建完成，开始写入数据\n");

    out << data.petType << std::endl;
    out << data.petHp << L" " << data.petMaxHp << std::endl;
    out << data.petMp << L" " << data.petMaxMp << std::endl;
    out << data.g_bgmVol << L" " << data.g_musicVol << L" " << data.g_soundVol << std::endl;

    // 道具数量
    out << data.items.size() << std::endl;
    for (auto& si : data.items)
    {
        out << si.name << L"|" << si.count << L"|" << si.heal << std::endl;
    }
    out.flush();  // 强制写入磁盘，防止缓冲丢失
    out.close();
    wprintf(L"【保存完成】所有数据写入完毕\n");
    return true;
}

// 读取存档并覆盖全局游戏数据
bool LoadGame(int slot)
{
    std::wstring path = L"save" + std::to_wstring(slot) + L".txt";
    std::wifstream in(path);
    if (!in.is_open())
        return false;

    SaveData data;
    in >> data.petType;
    in >> data.petHp >> data.petMaxHp;
    in >> data.petMp >> data.petMaxMp;
    in >> data.g_bgmVol >> data.g_musicVol >> data.g_soundVol;

    int itemCount;
    in >> itemCount;
    in.ignore();

    // 读取所有道具
    for (int i = 0; i < itemCount; i++)
    {
        std::wstring line;
        std::getline(in, line);
        size_t p1 = line.find(L"|");
        size_t p2 = line.find(L"|", p1 + 1);

        SaveItem si;
        si.name = line.substr(0, p1);
        si.count = std::stoi(line.substr(p1 + 1, p2 - p1 - 1));
        si.heal = std::stoi(line.substr(p2 + 1));
        data.items.push_back(si);
    }
    in.close();

    // 赋值全局音量变量并刷新音量
    g_selectPet = data.petType;
    g_bgmVol = data.g_bgmVol;
    g_musicVol = data.g_musicVol;
    g_soundVol = data.g_soundVol;
    UpdateBgmVolume();
    UpdateBattleBgmVolume();
    UpdateSoundVolume();

    // 覆盖背包道具
    bag_items.clear();
    for (auto& si : data.items)
    {
        Item it{ si.name, si.count, si.heal };
        bag_items.push_back(it);
    }

    // 销毁旧宠物，重建存档对应宠物
    if (player_pet != nullptr)
    {
        delete player_pet;
        player_pet = nullptr;
    }
    if (data.petType == 1)
        player_pet = new WarriorPet(L"小赤锋", &img_WarriorPet);
    else if (data.petType == 2)
        player_pet = new MagePet(L"星琉", &img_MagePet);
    else if (data.petType == 3)
        player_pet = new TankPet(L"盾墩墩", &img_TankPet);

    // 修正宠物血量蓝量
    if (player_pet != nullptr)
    {
        int hpDiff = data.petHp - player_pet->getHp();
        player_pet->addHp(hpDiff);
        player_pet->setMp(data.petMp);
    }

    InitBattleByPet(g_selectPet);
    return true;
}


// 创建空白存档，传入选中的宠物编号
bool CreateEmptySave(int slot, int petType)
{
    std::locale utf8_loc(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wofstream out;
    out.imbue(utf8_loc);
    std::wstring path = L"save" + std::to_wstring(slot) + L".txt";

    SaveData emptyData;
    emptyData.petType = petType; // 写入选中宠物编号
    // 默认初始背包道具
    emptyData.items.push_back({ L"小型回血药",3,30 });
    emptyData.items.push_back({ L"大型回血药",1,60 });
    emptyData.items.push_back({ L"魔力药水",2,30 });

    out.open(path);
    if (!out.is_open())
    {
        wprintf(L"【创建失败】无法生成存档%d\n", slot);
        return false;
    }
    wprintf(L"【创建成功】存档%d，宠物类型%d\n", slot, petType);

    out << emptyData.petType << std::endl;
    out << emptyData.petHp << L" " << emptyData.petMaxHp << std::endl;
    out << emptyData.petMp << L" " << emptyData.petMaxMp << std::endl;
    out << emptyData.g_bgmVol << L" " << emptyData.g_musicVol << L" " << emptyData.g_soundVol << std::endl;
    out << emptyData.items.size() << std::endl;
    for (auto& si : emptyData.items)
    {
        out << si.name << L"|" << si.count << L"|" << si.heal << std::endl;
    }
    out.flush();
    out.close();
    return true;
}


// 删除对应存档txt文件
bool DeleteSaveFile(int slot)
{
    std::wstring path = L"save" + std::to_wstring(slot) + L".txt";
    // Windows删除宽字符文件函数
    int ret = _wremove(path.c_str());
    if (ret == 0)
    {
        wprintf(L"【删除成功】存档%d文件已移除\n", slot);
        return true;
    }
    else
    {
        wprintf(L"【删除失败】存档%d不存在/无权限\n", slot);
        return false;
    }
}