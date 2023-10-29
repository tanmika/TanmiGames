/*****************************************************************//**
 * @file   Player.hpp
 * @brief  ������
 * 
 * @author tanmi
 * @date   October 2023
 *********************************************************************/

#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
using std::vector;
class Player;
class BuffBase
{
public:
	void SetPlayer(Player* p)
	{
		player = p;
	}
	void Add(size_t);
	void Sub(size_t);
	size_t Sum()const;
private:
	Player* player;
};

class BuffList
{
public:
	BuffList(int max_round, int buff_num)
	{
		buffVec.resize(max_round);
		for (auto& e : buffVec)
			e.resize(buff_num);
		buffSet.resize(max_round);
	}
	~BuffList() = default;
	void AddBuff(size_t _id, size_t duration, size_t _time);
	void Round();
	void RemoveLasted();
	void RemoveRamdon();
private:
	size_t GetId()
	{
		static size_t id=0;
		return id++;
	}
private:
	vector<vector<BuffBase*>> buffVec;
	vector<std::unordered_set<size_t>> buffSet;
};

class EffectBase
{
public:
	void Add(size_t);
	bool Consume(size_t);
private:
	size_t num;
};

class Player
{
public:
	Player();
	~Player();
	void SetEnemy(Player* p)
	{
		enemy = p;
	}
	void Attack(size_t);
	void AdditionAttack(size_t);
	void Defend(size_t);
	void CalDamageIncrease();
	void CalDamageReduction();
	void Round();
public:
	bool isAlive()const;
	size_t GetHp()const;
private:
	size_t base_hp;		//��������
	size_t base_attack; //��������

	size_t total_hp;	//�ۺ�����
	size_t total_attack;//�ۺϹ���
	size_t shield;		//����

	double damageIncrease;	//�����ܼ�
	double demageReduction;	//�����ܼ�
private:
	Player* enemy;
	vector<BuffBase> buffs;
	vector<BuffBase> debuffs;
	BuffList buffList;
	BuffList debuffList;
	vector<EffectBase> effects;
};
