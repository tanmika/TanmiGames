/*****************************************************************//**
 * @file   Main.cpp
 * @brief  主程序
 *
 * @author tanmi
 * @date   October 2023
 *********************************************************************/
#include "../inlcude/CardsLib.hpp"

int main()
{
	auto cards1 = CardLoader::Instance()
		.LoadCard("I:/TanmiGames/cpp/CardsFight/data/cards.txt");
	CardLib lib1(std::move(cards1));
	lib1.Instance(20);
	auto cardLib1 = lib1.GetCardsInfo();
	std::cout << "//-----------总牌库------------//\n";
	for (auto& e : *cardLib1)
		std::cout << e.first.GetName() << ": " << e.first.GetDescription() << std::endl;
	lib1.addCard(0);
	auto cardPile = lib1.GetPileInfo();
	std::cout << "//-----------牌库------------//\n";
	for (auto& e : *cardPile)
	{
		std::cout << e.first->GetName() << "剩余" << e.second << "张\n";
	}
	std::cout << "//------------玩家-----------//\n";
	Player p1(10000, 100);
	Player p2(10000, 100);

	p1.SetEnemy(&p2);
	p2.SetEnemy(&p1);
	std::cout << "player1 hp:" << p1.GetHp() << std::endl;
	std::cout << "player2 hp:" << p2.GetHp () << std::endl;
	std::cout << "//------------回合------------//\n";
	{
		std::vector<CardBase*> cardsListening;
		if (!lib1.Licensing(1, cardsListening))
			return 0;
		cardsListening[0]->Instance(&p1, &p2);
		cardsListening[0]->Play();
	}
	std::cout << "player1 hp:" << p1.GetHp() << std::endl;
	std::cout << "player2 hp:" << p2.GetHp () << std::endl;
	return 0;
}