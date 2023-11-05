/*****************************************************************//**
 * @file   OperatingArea.hpp
 * @brief  手牌区&弃牌区
 *
 * @author tanmi
 * @date   November 2023
 *********************************************************************/
#pragma once
#include <vector>
#include <list>
#include"CardsLib.hpp"
using std::list;

class DiscardException : public std::exception
{
	virtual const char* what() const throw()
	{
		return "DiscardArea Error!";
	}
};
class DiscardArea
{
public:
	DiscardArea(size_t master_num) :count(0), masterNum(master_num), isMasterCardUsed(false)
	{}
	~DiscardArea() = default;

	void AddCard(CardBase* card)
	{
		cards.push_back(card);
		count++;
	}
	bool GetRiseStar(CardBase* card)
	{
		if (card == nullptr)
			throw DiscardException();
		if (isMasterCardUsed == false && count >= masterNum)
		{
			if (card->GetStar() == 2)
				return false;
			isMasterCardUsed = true;
			count -= masterNum;
		}
	}
	std::vector<CardBase*> GetDiscardCards()
	{
		std::vector<CardBase*> c = cards;
		cards.clear();
		return c;
	}
private:
	std::vector<CardBase*> cards;
	size_t count;
	int masterNum;
	bool isMasterCardUsed;
};

class OperatingException :public std::exception
{
	virtual const char* what() const throw()
	{
		return "OperatingArea Error!";
	}
};
class OperatingArea
{
public:
	OperatingArea(size_t max_cards_num, size_t init_battle_card_num) 
		:maxCardsNum(max_cards_num), maxBattleCardsNum(init_battle_card_num)
	{}
	~OperatingArea() = default;
	void InstanceDiscardArea(DiscardArea* discard_area)
	{
		discardArea = discard_area;
	}
	bool AddCardsFromPile(CardBase* card)
	{
		if (cards.size() == maxCardsNum)
		{
			return false;
		}
		cards.push_back(card->GetCopy());
		return true;
	}
	bool AddCardsFromBattle(size_t from)
	{
		if (from > battleCards.size())
			throw OperatingException();
		if (cards.size() == maxCardsNum)
		{
			std::cerr << "手牌已满\n";
			return false;
		}
		auto it = battleCards.begin();
		for (size_t i = 0; i < battleCards.size(); ++i)
			++it;
		cards.push_back(*it);
		battleCards.remove(*it);
		return true;
	}
	bool AddBattleCards(size_t from, size_t to)
	{
		if (from > cards.size())
			throw OperatingException();
		if (battleCards.size() == maxBattleCardsNum)
			return false;
		auto it = cards.begin();
		for (size_t i = 0; i < from; ++i)
			++it;
		auto itB = battleCards.begin();
		for (size_t i = 0; i < std::min(battleCards.size(), to); ++i)
			++itB;
		battleCards.insert(itB, *it);
		cards.remove(*it);
		return true;
	}
	void DiscardCard(list<CardBase*>* from, size_t idx)
	{
		if (!discardArea)
		{
			std::cerr << "未定义\n";
			throw OperatingException();
		}
		if (idx > from->size())
		{
			std::cerr << "越界\n";
			throw OperatingException();
		}
		auto it = from->begin();
		for (size_t i = 0; i < idx; ++i)
			++it;
		discardArea->AddCard(*it);
		from->remove(*it);
	}
	void IncreaseBattleCardsNum()
	{
		if (maxBattleCardsNum < 8)
			maxBattleCardsNum++;
	}
	std::list<CardBase*>* GetBattleCardsPtr()
	{
		return &battleCards;
	}
private:
	int maxCardsNum;//<手牌上限
	int maxBattleCardsNum;//<战斗手牌上限
	list<CardBase*> cards;//<手牌区
	list<CardBase*> battleCards;//<战斗手牌区
	DiscardArea* discardArea;//<弃牌区指针
};


