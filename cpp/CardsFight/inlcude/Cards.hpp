/*****************************************************************//**
 * @file   Cards.hpp
 * @brief  卡牌相关功能类
 *
 * @author tanmi
 * @date   October 2023
 *********************************************************************/
#pragma once
#include <vector>
#include <functional>
#include <string>
#include <fstream>
#include <iostream>

#include "Skills.hpp"

enum class CardType
{
	normal, //通用卡
	special, //特殊卡
	venue, //场地卡
};
class CardFactory;
class CardBase
{
	friend class CardFactory;
	friend class CardLoader;
public:
	CardBase(std::string _name) :
		name(_name), active(true), cost(0), type(CardType::normal), specialSkill(nullptr), description("")
	{}
	~CardBase() = default;
	void Instance(Player* player, Player* enemy);
	bool isPlayCard(size_t cost);//<是否可使用
	void ChangeCost(int num);//<改变费用
	void Play();//<使用卡牌
	std::string GetName()const
	{
		return name;
	}
	std::string GetDescription()const
	{
		return description;
	}
	void Active(bool b)
	{
		active = b;
	}
	bool isActive() const
	{
		return active;
	}
#if _DEBUG
	void DebugPrint()
	{
		using std::cout;
		using std::endl;
		cout << "***DEBUG***\n";
		cout << "card name: " << name << endl;
		cout << "cost: " << cost << endl;
		cout << "type: ";
		if (type == CardType::normal)
			cout << "normal\n";
		else if (type == CardType::special)
			cout << "special\n";
		else
			cout << "venue\n";
		cout << "skill num: " << skillList.size() << endl;
		if (specialSkill)
			cout << "exist special skill\n";
		cout << "description: " << description;
	}
#endif // _DEBUG

private:
	Player* player;
	Player* enemy;
	size_t cost;
	CardType type;
	bool active;
	std::vector<std::pair<size_t, Skill*>> skillList;
	Skill* specialSkill;
	std::string name;
	std::string description;
};
inline void CardBase::Instance(Player* player, Player* enemy)
{
	this->player = player;
	this->enemy = enemy;
}
bool CardBase::isPlayCard(size_t cost)
{
	return active && cost >= this->cost;
}
void CardBase::ChangeCost(int num)
{
	cost += num;
}
inline void CardBase::Play()
{
	if (specialSkill)
	{
		specialSkill->Play(1);
	}
	for (auto& e : skillList)
	{
		e.second->Play(e.first);
	}
}
class CardFactory
{
public:
	static CardFactory& Instance()
	{
		static CardFactory ins;
		return ins;
	}
	CardBase CreateCard(std::string _name)
	{
		return CardBase(_name);
	}

private:
	CardFactory() = default;
	~CardFactory() = default;
};

class CardLoader
{
public:
	static CardLoader& Instance()
	{
		static CardLoader ins;
		return ins;
	}
	std::vector<CardBase> LoadCard(std::string path);
private:
	bool Loading(CardBase&, std::fstream&);
};
std::vector<CardBase> CardLoader::LoadCard(std::string path)
{
	using std::fstream;
	fstream file(path);
	if (!file.is_open())
	{
		std::cerr << "card file open failed";
		throw std::runtime_error("CardLoader::LoadCard: file open failed");
	}
	std::vector<CardBase> cardList;
	std::string str;
	while (std::getline(file, str))
	{
		if (str[0] == '#' || str == "")
		{
			continue;
		}
		if (str.find("name: ") == 0)
		{
			std::string name = str.substr(6);
			CardBase card = CardFactory::Instance().CreateCard(name);
			if (Loading(card, file))
				cardList.push_back(card);
		}
		if (file.eof()) break;
	}
	return cardList;
}
class SkillDamage;
class SkillBuff;
class SkillSpecial;
inline bool CardLoader::Loading(CardBase& card, std::fstream& file)
{
	using std::stoi;
	using std::string;

	bool is_description = false;
	bool is_type = false;
	bool is_cost = false;
	bool is_skill = false;
	string str;
	string info;
	while (file >> str)
	{
		if (str == "---")
		{
			if (is_description && is_type
				&& is_cost && is_skill)
				return true;
			else
				return false;
		}
		if (str == "type:")
		{
			file >> info;
			card.type = (info == "normal") ? CardType::normal :
				(info == "special") ? CardType::special :
				(info == "venue") ? CardType::venue : CardType::normal;
			is_type = true;
		}
		else if (str == "description:")
		{
			file >> info;
			card.description = info;
			is_description = true;
		}
		else if (str == "cost:")
		{
			file >> info;
			card.cost = stoi(info);
			is_cost = true;
		}
		else if (str == "damage:")
		{
			file >> info;
			SkillDamage* skill = new SkillDamage();
			size_t found = info.find_last_of("%");
			if (found != string::npos)
				skill->SetAdditionalDamage(stoi(info.substr(0, found)));
			else
				skill->SetDirectDamage(stoi(info));
			card.skillList.push_back(std::pair<size_t, Skill*>(1, skill));
			is_skill = true;
		}
		else if (str == "buff_num:")
		{
			file >> info;
			int n = std::stoi(info);
			while (n--)
			{
				string buffstr;
				SkillBuff* buff = new SkillBuff();
				for (int i = 0; i < 3; i++)
				{
					file >> buffstr;
					if (buffstr == "buff_id:")
					{
						file >> buffstr;
						buff->SetId(stoi(buffstr));
					}
					else if (buffstr == "buff_duration:")
					{
						file >> buffstr;
						buff->SetDuration(stoi(buffstr));
					}
					else if (buffstr == "buff_time:")
					{
						file >> buffstr;
						buff->SetTime(stoi(buffstr));
					}
					else
					{
						std::cerr << "buff info load failed in " << card.name;
						throw std::runtime_error(
							"CardLoader::LoadCard: buff info load failed in " + card.name);
					}
				}
				is_skill = true;
				card.skillList.push_back(std::pair<size_t, Skill*>(1, buff));
			}
		}
		else if (str == "sp_id:")
		{
			file >> str;
			SkillSpecial sp;
			sp.SetId(stoi(str));
			sp.SetTarget(9999);
			is_skill = true;
			file >> str;
			if (str == "sp_target:")
			{
				sp.SetTarget(stoi(str));
				file >> str;
			}
			else if (str == "---")
			{
				if (is_description && is_type
					&& is_cost && is_skill)
					return true;
				else
					return false;
			}
			else
			{
				std::cerr << "sp info load failed in " << card.name;
				throw std::runtime_error(
					"CardLoader::LoadCard: sp info load failed in " + card.name);
			}
		}
		else
		{
			std::cerr << card.name << " load failed";
			throw std::runtime_error(
				"CardLoader::LoadCard: " + card.name + " load failed");
		}
	}
}