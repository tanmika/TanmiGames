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
		while (t--)
		{
			if (directDamage != 0)
			{
				player->Attack(directDamage);
			}
			if (additionalDamage != 0)
			{
				player->Attack(additionalDamage);
			}
		}
	}
	void SetDirectDamage(size_t dmg)
	{
		directDamage = dmg;
	}
	void SetAdditionalDamage(size_t dmg)
	{
		additionalDamage = dmg;
	}
private:
	size_t directDamage = 0;
	size_t additionalDamage = 0;
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
			player->GetBuffListPtr()->AddBuff(id, time, duration);
		}
		else
		{
			enemy->GetDebuffListPtr()->AddBuff(id, time, duration);
		}
	}
	void SetId(size_t _id)
	{
		id = _id;
	}
	void SetDuration(size_t _dra)
	{
		duration = _dra;
	}
	void SetTime(size_t _t)
	{
		time = _t;
	}
private:
	size_t id = 0;
	size_t duration = 0;
	size_t time = 0;
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
		target = _buff;
	}
private:
	size_t id = 0;
	size_t target = 0;
};

#endif // !_TANMI_SKILLS_