/*****************************************************************//**
 * @file   Player.hpp
 * @brief  玩家相关
 * 
 * @author tanmi
 * @date   October 2023
 *********************************************************************/

#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
using std::vector;
using std::pair;
class Player;
class BuffBase
{
public:
	void SetPlayer(Player* p)
	{
		player = p;
	}
	void Add(size_t n)
	{
		num += n;
	}
	void Sub(size_t n)
	{
		num = num > n ? num - n : 0;
	}
	size_t Sum()const
	{
		return num;
	}
private:
	Player* player;
	size_t num;
};

class BuffList
{
public:
	BuffList(int max_round, vector<BuffBase>& buffs)
	{
		buffVec.resize(max_round +1);
		for (auto& buff : buffs)
			for (auto e: buffVec)
			{
				e = new vector<pair<BuffBase*, size_t>>;
				e->push_back(pair<BuffBase*, size_t>(&buff, 0));
			}
		buffSet.resize(max_round +1);
		for (auto e : buffSet)
			e = new std::unordered_set<size_t>;
	}
	~BuffList() = default;
	void AddBuff(size_t _id, size_t _time =1, size_t duration =0)
	{
		buffVec[duration]->at(_id).first->Add(_time);
		buffVec[duration]->at(_id).second += _time;
		if (duration == 0)return; //永久buff不登记
		for (auto i = 0; i < _time; i++)
		{
			//为每层buff注册id
			auto id = GetId();
			//按回合数登记
			buffSet[duration]->insert(id);
			//按新增顺序登记
			buffMap[id] = _id;
		}
	}
	void Round()
	{
		//当前回合结束消失buff
		auto ptrVec = buffVec[1];
		auto ptrSet = buffSet[1];
		//注销监听
		for (auto e : *ptrSet)
		{
			buffMap.erase(e);
		}
		ptrSet->clear();
		//消除效果
		for (auto& e: *ptrVec)
		{
			e.first->Sub(e.second);
			e.second = 0;
		}
		//其余buff回合数减1
		for (auto i = 1; i < buffVec.size()-1; i++)
		{
			buffVec[i] = buffVec[i + 1];
			buffSet[i] = buffSet[i + 1];
		}
		//最远回合数绑定
		buffVec[buffVec.size() - 1] = ptrVec;
		buffSet[buffSet.size() - 1] = ptrSet;
	}
	void RemoveLasted()
	{
		auto it = buffMap.begin();
		size_t maxId = it->first;
		for (auto iter = buffMap.begin(); iter != buffMap.end(); ++iter)
		{
			if (iter->first > maxId)
			{
				maxId = iter->first;
				it = iter; // id最大的buff
			}
		}
		for (int i = 1; i < buffSet.size(); i++)
		{
			if (buffSet[i]->find(maxId) != buffSet[i]->cend())
			{
				buffSet[i]->erase(maxId);
				buffVec[i]->at(it->second).first->Sub(1);
				buffVec[i]->at(it->second).second--;
				return;
			}
		}
	}
	void RemoveRamdon()
	{
		auto it = buffMap.begin();
		auto num = rand() %( buffMap.size()-1);
		while (num--)
			it++;
		for (int i = 1; i < buffSet.size(); i++)
		{
			if (buffSet[i]->find(it->first) != buffSet[i]->cend())
			{
				buffSet[i]->erase(it->first);
				buffVec[i]->at(it->second).first->Sub(1);
				buffVec[i]->at(it->second).second--;
				return;
			}
		}

	}
private:
	size_t GetId()
	{
		static size_t id=0;
		return id++;
	}
private:
	vector<vector<pair<BuffBase*,size_t>>*> buffVec;//(回合, (buff索引id, (buff 类, 次数)))
	vector<std::unordered_set<size_t>*> buffSet;//(回合, (本buff id))
	std::unordered_map<size_t, size_t> buffMap;//(本buff id, buff索引id)
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
	size_t base_hp;		//基础生命
	size_t base_attack; //基础攻击

	size_t total_hp;	//综合生命
	size_t total_attack;//综合攻击
	size_t shield;		//护盾

	double damageIncrease;	//增伤总计
	double demageReduction;	//减伤总计
private:
	Player* enemy;
	vector<BuffBase> buffs;
	vector<BuffBase> debuffs;
	BuffList buffList;
	BuffList debuffList;
	vector<EffectBase> effects;
};
