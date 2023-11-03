/*****************************************************************//**
 * @file   CardsLib.hpp
 * @brief  牌库
 *
 * @author tanmika
 * @date   November 2023
 *********************************************************************/

#include "Cards.hpp"

using std::pair;
class CardLib
{
public:
	CardLib(vector<CardBase>&& _cards)
	{
		for (auto&& e : _cards)
		{
			cards.emplace_back(std::move(e), true);
		}
	}
	~CardLib() = default;
	void Instance(size_t);///< 将初始化牌库
	bool addCard(size_t idx);
	/**
	 * @attention 不进行合法性检查, 仅可在局中调用
	 */
	void addCards(vector<CardBase*> idxs);
	void removeCard(size_t idx);
	void Shuffle();
	bool Licensing(size_t num, vector<CardBase*>& cards);
	void ClearPile();
	bool EmptyPile()const;
	void Clear();
	bool Empty()const;
	vector<pair<CardBase, bool>>* const GetCardsInfo();
	vector<pair<CardBase*, int>>* const GetPileInfo();
private:
	bool isIns = false;
	size_t pileCardsNum;//<牌库上限(实际需4倍)
	vector<pair<CardBase, bool>> cards;
	vector<pair<CardBase*, int>> cardPile;
	int leftCardsNum = 0;//<牌库剩余牌数
};

inline void CardLib::Instance(size_t num)
{
	isIns = true;
	pileCardsNum = num;
	ClearPile();
}

bool CardLib::addCard(size_t idx)
{
	if (!isIns)
	{
		std::cerr << "牌库未初始化\n";
		return false;
	}
	if (idx >= cards.size() || cards[idx].second == false || cardPile.size() >= pileCardsNum)
		return false;
	cards[idx].second = false;
	cardPile.push_back(pair<CardBase*, int>(&cards[idx].first, 4));
	leftCardsNum += 4;
	return true;
}

inline void CardLib::addCards(vector<CardBase*> idxs)
{
	for (auto e : idxs)
	{
		for (auto& t : cardPile)
		{
			if (t.first == e)
			{
				t.second++;
				leftCardsNum++;
				break;
			}
		}
	}
}

inline void CardLib::removeCard(size_t idx)
{
	leftCardsNum -= 4;
	if (idx == 0 && cardPile.size() == 1) cardPile.pop_back();
	auto card = cardPile[idx].first;
	for (auto& e : cards)
	{
		if (&e.first == card)
		{
			e.second = true;
			break;
		}
	}
	for (size_t i = idx; i < cardPile.size() - 1; i++)
	{
		cardPile[i] = cardPile[i + 1];
	}
	cardPile.pop_back();
}

inline void CardLib::Shuffle()
{
	srand((unsigned)time(0));
	int rnum = 0;
	int size = cardPile.size();
	auto ptemp = cardPile[0].first;
	int temp = 0;
	for (int i = 0; i < size; i++)
	{
		rnum = rand() % (size - i) + i;
		ptemp = cardPile[i].first;
		cardPile[i].first = cardPile[rnum].first;
		cardPile[rnum].first = ptemp;
		temp = cardPile[i].second;
		cardPile[i].second = cardPile[rnum].second;
		cardPile[rnum].second = temp;
	}
}

inline bool CardLib::Licensing(size_t num, vector<CardBase*>& cards)
{
	if (num > leftCardsNum)
		return false;
	if (!cards.empty())
		cards.clear();
	srand((unsigned)time(0));
	for (size_t i = 0; i < num; i++)
	{
		int c = rand() % cardPile.size();
		while (cardPile[c].second == 0)
		{
			c++;
			if (c == cards.size())
				c = 0;
		}
		cards.push_back(cardPile[c].first);
		cardPile[c].second--;
		leftCardsNum--;
	}
	return true;
}

inline void CardLib::ClearPile()
{
	while (!EmptyPile())
	{
		removeCard(cardPile.size() - 1);
	}
	leftCardsNum = 0;
}

inline bool CardLib::EmptyPile() const
{
	return cardPile.empty();
}

inline void CardLib::Clear()
{
	if (leftCardsNum != 0)
		cardPile.clear(), leftCardsNum = 0;
	cards.clear();
}

inline bool CardLib::Empty() const
{
	return cards.empty();
}

inline vector<pair<CardBase, bool>>* const CardLib::GetCardsInfo()
{
	return &cards;
}

inline vector<pair<CardBase*, int>>* const CardLib::GetPileInfo()
{
	return &cardPile;
}