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

class Skill 
{
public:
	Skill():player(nullptr), enemy(nullptr)
	{}
	~Skill() =default;
	void SetTarget(Player* p1, Player* p2)
	{
		player = p1;
		enemy = p2;
	};
	virtual void Play(size_t) = 0;
private:
	Player* player;
	Player* enemy;
};

class SkillDamage final: public Skill
{
public:
	SkillDamage()
	{}
	~SkillDamage() = default;
	void Play(size_t)
	{}
	void SetDirectDamage(size_t dmg)
	{
		directDamage = dmg;
	}
	void SetAdditionalDamage(size_t dmg)
	{
		additionalDamage = dmg;
	}
private:
	size_t directDamage;
	size_t additionalDamage;
};

class SkillBuff final :public Skill
{
public:
	SkillBuff()
	{}
	~SkillBuff() = default;
	void Play(size_t)
	{}
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
	size_t id;
	size_t duration;
	size_t time;
};
class SkillSpecial final :public Skill
{
public:
	SkillSpecial(){}
	~SkillSpecial() = default;
	void Play(size_t)
	{}
	void SetId(size_t _id)
	{
		id = _id;
	}
	void SetTarget(size_t _buff)
	{
		target = _buff;
	}
private:
	size_t id;
	size_t target;
};

#endif // !_TANMI_SKILLS_