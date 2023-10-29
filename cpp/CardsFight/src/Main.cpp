/*****************************************************************//**
 * @file   Main.cpp
 * @brief  ������
 * 
 * @author tanmi
 * @date   October 2023
 *********************************************************************/
#include "../inlcude/Cards.hpp"

int main()
{
	auto cards = CardLoader::Instance()
		.LoadCard("I:/TanmiGames/cpp/CardsFight/data/cards.txt");
	cards[0].DebugPrint();
	return 0;
}
