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
		buffVec.resize(max_round + 1);
		for (auto& buff : buffs)
			for (auto e : buffVec)
			{
				e = new vector<pair<BuffBase*, size_t>>;
				e->push_back(pair<BuffBase*, size_t>(&buff, 0));
			}
		buffSet.resize(max_round + 1);
		for (auto e : buffSet)
			e = new std::unordered_set<size_t>;
	}
	~BuffList() = default;
	void AddBuff(size_t _id, size_t _time = 1, size_t duration = 0)
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
		for (auto& e : *ptrVec)
		{
			e.first->Sub(e.second);
			e.second = 0;
		}
		//其余buff回合数减1
		for (auto i = 1; i < buffVec.size() - 1; i++)
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
		auto num = rand() % (buffMap.size() - 1);
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
		static size_t id = 0;
		return id++;
	}
private:
	vector<vector<pair<BuffBase*, size_t>>*> buffVec;//(回合, (buff索引id, (buff 类, 次数)))
	vector<std::unordered_set<size_t>*> buffSet;//(回合, (本buff id))
	std::unordered_map<size_t, size_t> buffMap;//(本buff id, buff索引id)
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
	size_t num = 0;
};

class Player
{
public:
	Player(int _hp, int _attack) : base_hp(_hp), base_attack(_attack), total_hp(_hp), total_attack(_attack), hp(_hp), defence(0),
		shield(0), damageIncrease(0), damageReduction(0), enemy(nullptr)
	{
		buffs.resize(Buff::BuffNum, BuffBase());
		buffList.Ins(5, buffs);
		debuffs.resize(Debuff::DebuffNum, BuffBase());
		debuffList.Ins(5, debuffs);
		effects.resize(Effect::EffectNum, EffectBase());
	}
	~Player() = default;
	void SetEnemy(Player* p)
	{
		enemy = p;
	}
	//待定
	void Attack(size_t n);
	void AdditionAttack(size_t n);
	void Defend(size_t n);
	void CalDamageIncrease();
	void CalDamageReduction();
	void Round(); // 回合结算发生于一方进入回合时
public:
	bool isAlive()const;
	size_t GetTotalHp(int change, bool revover = true); // 计算并返回当前生命值上限
	void Heal(size_t);
	size_t GetHp()const
	{
		return hp;
	}
	BuffList* GetBuffListPtr()
	{
		return &buffList;
	}
	BuffList* GetDebuffListPtr()
	{
		return &debuffList;
	}
private:
	size_t base_hp;		//基础生命
	size_t base_attack; //基础攻击
	int defence;	//防御

	size_t total_hp;	//综合生命
	size_t total_attack;//综合攻击
	size_t shield;		//护盾
	size_t hp;			//当前生命

	double damageIncrease;	//增伤总计
	double damageReduction;	//减伤总计
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
			effects[Effect::Barrier].Consume(damage);
			return;
		}
		else
		{
			damage -= shield;
			effects[Effect::Barrier].Consume(shield);
			shield = 0;
		}
	}
	if (hp > damage)
	{
		hp -= damage;
		return;
	}
	else
	{
		hp = 0;
	}
}

inline void Player::CalDamageIncrease()
{
	total_attack = base_attack * (1 + buffs[Buff::Excitement].Sum() * 0.1f) +
		buffs[Buff::Strength].Sum();
	damageIncrease = (1 + (0.06f * buffs[Buff::Melody].Sum())) *
		std::pow(0.95f, debuffs[Debuff::Weakness].Sum());
}

inline void Player::CalDamageReduction()
{
	damageReduction = (1 + (0.05f * debuffs[Debuff::Vulnerability].Sum()));
	defence = buffs[Buff::Resilience].Sum() - debuffs[Debuff::Fragility].Sum();
	if (defence > 0)
	{
		damageReduction *= (20.0f / (20.0f + defence));
	}
	else
	{
		damageReduction *= (1 + 0.05f * defence);
	}
}

inline void Player::Round()
{
	Defend(debuffs[Debuff::Bleeding].Sum() * 10);
	int erosion = debuffs[Debuff::Erosion].Sum();
	debuffList.Round();
	GetTotalHp((erosion - debuffs[Debuff::Erosion].Sum()) * 0.03f * base_hp, false);
	if (!isAlive()) return;
	int abundance = buffs[Buff::Abundance].Sum();
	Heal(buffs[Buff::Healing].Sum() * 50);
	buffList.Round();
	GetTotalHp((abundance - buffs[Buff::Abundance].Sum()) * 25);
}

inline bool Player::isAlive() const
{
	return hp > 0;
}

inline size_t Player::GetTotalHp(int change, bool recover)
{
	if (change >= 0)
	{
		total_hp += change;
		if (recover)
			hp += change;
	}
	else
	{
		total_hp = std::max(1, (int)total_hp + change);
		hp = std::min(hp, total_hp);
	}
	return total_hp;
}

inline void Player::Heal(size_t _hp)
{
	hp = std::min(total_hp, hp + _hp);
}