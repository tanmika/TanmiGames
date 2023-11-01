/*****************************************************************//**
 * @file   CardsLib.hpp
 * @brief  ÅÆ¿â
 * 
 * @author tanmika
 * @date   November 2023
 *********************************************************************/

#include "Cards.hpp"


class CardLib
{
public:
	CardLib(vector<CardBase> _cards):
		cards(_cards)
	{}
	~CardLib() = default;
	bool addCard(size_t idx);
	void addCards(vector<CardBase*> idxs);
	bool removeCard(size_t idx);
	void Shuffle();
	bool Licensing(size_t num, vector<CardBase*>& cards);
	void ClearPile();
	bool EmptyPile()const;
	void Clear();
	bool Empty()const;
private:
	vector<CardBase> cards;
	vector<std::pair<CardBase*, int>> cardPile;
};
