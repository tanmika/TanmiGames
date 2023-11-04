/*****************************************************************//**
 * @file   Skills.hpp
 * @brief  卡牌技能相关
 *
 * @author tanmika
 * @date   October 2023
 *********************************************************************/
#pragma once
#ifndef _TANMI_SKILLS_
#define _TANMI_SKILLS_
#include"Player.hpp"
using std::vector;
class SkillExpection :public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "调用技能时未指定对象";
	}
};

class Skill
{
public:
	Skill() :player(nullptr), enemy(nullptr)
	{}
	~Skill() = default;
	void SetTarget(Player* p1, Player* p2)
	{
		player = p1;
		enemy = p2;
	};
	virtual void Play(size_t) = 0;
protected:
	void isInstance()const
	{
		if (player == nullptr || enemy == nullptr)
			throw SkillExpection();
	}
	Player* player;
	Player* enemy;
};

class SkillDamage final : public Skill
{
public:
	SkillDamage() = default;
	~SkillDamage() = default;
	void Play(size_t t)
	{
		isInstance();

		if (directDamage.size() != 0)
		{
			player->Attack(directDamage[t]);
		}
		if (additionalDamage.size() != 0)
		{
			player->Attack(additionalDamage[t]);
		}
	}
	void SetDirectDamage(size_t dmg)
	{
		directDamage.push_back(dmg);
	}
	void SetAdditionalDamage(size_t dmg)
	{
		additionalDamage.push_back(dmg);
	}
private:
	vector<size_t> directDamage;
	vector<size_t> additionalDamage;
};

class SkillBuff final :public Skill
{
public:
	SkillBuff() = default;
	~SkillBuff() = default;
	void Play(size_t t)
	{
		isInstance();

		if (id < Buff::BuffNum)
		{
			player->GetBuffListPtr()->AddBuff(id, time[t], duration[t]);
		}
		else
		{
			enemy->GetDebuffListPtr()->AddBuff(id, time[t], duration[t]);
		}
	}
	void SetId(size_t _id)
	{
		id = _id;
	}
	void SetDuration(size_t _dra)
	{
		duration.push_back(_dra);
	}
	void SetTime(size_t _t)
	{
		time.push_back(_t);
	}
private:
	size_t id = 0;
	vector<size_t> duration;
	vector<size_t> time;
};
class SkillSpecial final :public Skill
{
public:
	SkillSpecial() = default;
	~SkillSpecial() = default;
	void Play(size_t t)
	{
		isInstance();
		//do nothing yet
	}
	void SetId(size_t _id)
	{
		id = _id;
	}
	void SetTarget(size_t _buff)
	{
		target.push_back(_buff);
	}
private:
	size_t id = 0;
	vector<size_t> target;
};

#endif // !_TANMI_SKILLS_