/*****************************************************************//**
 * @file   Main.cpp
 * @brief  Ö÷³ÌÐò
 * 
 * @author tanmi
 * @date   October 2023
 *********************************************************************/
#include "../inlcude/Cards.hpp"

int main()
{
	auto cards = CardLoader::Instance().LoadCard("I:/TanmiGames/cpp/CardsFight/data/cards.txt");
	std::cout << cards.size();
	cards[0].DebugPrint();
	return 0;
}
