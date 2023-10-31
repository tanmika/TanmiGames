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
#include "Buffs.hpp"
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
	Player* player = nullptr;
	size_t num = 0;
};

class BuffList
{
public:
	BuffList() = default;
	void Ins(int max_round, vector<BuffBase>& buffs)
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
		if (duration == 0)return; //����buff���Ǽ�
		for (auto i = 0; i < _time; i++)
		{
			//Ϊÿ��buffע��id
			auto id = GetId();
			//���غ����Ǽ�
			buffSet[duration]->insert(id);
			//������˳��Ǽ�
			buffMap[id] = _id;
		}
	}
	void Round()
	{
		//��ǰ�غϽ�����ʧbuff
		auto ptrVec = buffVec[1];
		auto ptrSet = buffSet[1];
		//ע������
		for (auto e : *ptrSet)
		{
			buffMap.erase(e);
		}
		ptrSet->clear();
		//����Ч��
		for (auto& e: *ptrVec)
		{
			e.first->Sub(e.second);
			e.second = 0;
		}
		//����buff�غ�����1
		for (auto i = 1; i < buffVec.size()-1; i++)
		{
			buffVec[i] = buffVec[i + 1];
			buffSet[i] = buffSet[i + 1];
		}
		//��Զ�غ�����
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
				it = iter; // id����buff
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
	vector<vector<pair<BuffBase*,size_t>>*> buffVec;//(�غ�, (buff����id, (buff ��, ����)))
	vector<std::unordered_set<size_t>*> buffSet;//(�غ�, (��buff id))
	std::unordered_map<size_t, size_t> buffMap;//(��buff id, buff����id)
};

class EffectBase
{
public:
	void Add(size_t n)
	{
		num += n;
	}
	bool Consume(size_t n)
	{
		if (num < n) return false;
		num -= n;
		return true;
	}
private:
	size_t num;
};

class Player
{
public:
	Player(int _hp, int _attack): base_hp(_hp), base_attack(_attack), total_hp(_hp), total_attack(_attack), 
		shield(0), damageIncrease(0), damageReduction(0), enemy(nullptr)
	{
		buffs.resize(Buff::BuffNum);
		buffList.Ins(5, buffs);
		debuffs.resize(Debuff::DebuffNum);
		debuffList.Ins(5, debuffs);
		effects.resize(Effect::EffectNum);
	}
	~Player() = default;
	void SetEnemy(Player* p)
	{
		enemy = p;
	}
	//����
	void Attack(size_t n);
	void AdditionAttack(size_t n);
	void Defend(size_t n);
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
	double damageReduction;	//�����ܼ�
private:
	Player* enemy;
	vector<BuffBase> buffs;
	vector<BuffBase> debuffs;
	BuffList buffList;
	BuffList debuffList;
	vector<EffectBase> effects;
};
inline void Player::Attack(size_t n)
{
	CalDamageIncrease();
	enemy->Defend((total_attack * n * damageIncrease) / 100);
}

inline void Player::AdditionAttack(size_t n)
{
	CalDamageIncrease();
	enemy->Defend(n * damageIncrease);
}

inline void Player::Defend(size_t n)
{
	CalDamageReduction();
	int damage = std::min(1, int((double)n * damageReduction));
	if (shield > 0)
	{
		if (shield > damage)
		{
			shield -= damage;
			return;
		}
		else
		{
			damage -= shield;
			shield = 0;
		}
	}
	if (total_hp > n)
	{
		total_hp -= n;
		return;
	}
	else
	{
		total_hp = 0;
	}
}

inline void Player::CalDamageIncrease()
{
	total_attack = base_attack * (1 + buffs[1].Sum() * 0.1f) + buffs[0].Sum();
	damageIncrease = 1 + (0.06f * buffs[2].Sum());
}


