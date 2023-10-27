/*****************************************************************//**
 * @file   Cards.hpp
 * @brief  ������ع�����
 * 
 * @author tanmi
 * @date   October 2023
 *********************************************************************/
#pragma once
#include <vector>
#include <functional>
#include <string>

using Skill = std::pair<std::function<void(size_t)>, size_t>;
enum class CardType
{
	normal, //ͨ�ÿ�
	special, //���⿨
	venue, //���ؿ�
};
class CardFactory;
class CardBase
{
friend class CardFactory;
public:
	CardBase() = default;
	~CardBase() = default;
	bool isPlayCard(size_t cost);
	void ChangeCost(int num);
	void Play();
	const std::string& GetName();
	const std::string& GetDescription();
	void Active(bool);
	bool isActive();
private:
	size_t cost;
	CardType type;
	bool active;
	std::vector<std::pair<size_t, Skill>> skillList;
	std::vector<Skill> specialSkillList;
	std::string name;
	std::string description;
};

class CardFactory
{
	static CardFactory& Instance()
	{
		static CardFactory ins;
		return ins;
	}
	CardBase CreateNormalCard();
	CardBase CreateSpecialCard();
	CardBase CreateVenueCard();
private:
	CardFactory() = default;
	~CardFactory() = default;
};
