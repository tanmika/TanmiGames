/*****************************************************************//**
 * @file   Main.cpp
 * @brief  Ö÷³ÌÐò
 * 
 * @author tanmi
 * @date   October 2023
 *********************************************************************/
#include "../inlcude/CardsLib.hpp"

int main()
{
	auto cards = CardLoader::Instance()
		.LoadCard("I:/TanmiGames/cpp/CardsFight/data/cards.txt");
	CardLib lib(std::move(cards));
	lib.Instance(20);
	return 0;
}
