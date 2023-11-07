/*****************************************************************//**
 * @file   GameManager.hpp
 * @brief  游戏管理类
 *
 * @author tanmi
 * @date   November 2023
 *********************************************************************/
#pragma once

#include <istream>

#include"Player.hpp"
#include"OperatingArea.hpp"
constexpr int MAX_OPERATING_CRAD_NUM = 8; ///< 最大手牌数
constexpr int INIT_BATTLE_CRAD_NUM = 4; ///< 初始对战区牌数
constexpr int MAX_BATTTLE_CRAD_NUM = 8; ///< 最大对战区牌数
constexpr int MASTER_CRAD_DISCARD_NUM = 8; ///< 获得万能牌所需弃牌数
constexpr int ACTION_POINT_PER_BATTLE_CARD = 3; ///< 每张牌所能获得的行动点数
constexpr int INIT_LISTENING_NUM = 6; ///< 初始听牌数
constexpr int ROUND_LISTING_NUM = 4; ///< 轮次听牌数

class PvePlayException :public std::exception
{
	virtual const char* what() const throw()
	{
		return "PvePlayException!";
	}
};

class PveRoundingException :public std::exception
{
	virtual const char* what() const throw()
	{
		return "PveRoundingException!";
	}
};

class PveGame
{
public:
	PveGame(size_t player_hp, size_t player_atk, size_t enemy_hp
		, size_t enemy_atk, vector<CardBase>&& player_cards)
		:round(0), player(player_hp, player_atk), enemy(enemy_hp, enemy_atk),
		cardLib(std::move(player_cards)), discardArea(MASTER_CRAD_DISCARD_NUM),
		operatingArea(MAX_OPERATING_CRAD_NUM, INIT_BATTLE_CRAD_NUM),
		enemyOperatingArea(MAX_OPERATING_CRAD_NUM, MAX_BATTTLE_CRAD_NUM)
	{
		cardLib.Shuffle();
		operatingArea.InstanceDiscardArea(&discardArea);
	}
	~PveGame() = default;
	void SetEnemyCards(vector<CardBase*> enemy_cards); ///<设置敌方对战牌
	void Play(); ///< 单局主循环
	bool Rounding(Player& p1, Player& p2,
		OperatingArea& op1, OperatingArea& op2); ///< 单回合主循环
private:
	void ShowOperatingArea();
	void ShowBattleArea();
	int GetInputInRound(std::string, int); ///< 获得范围内的数字输入
	std::pair<int, int> GetInputInRound(std::string, int, int); ///< 获得范围内的数字输入
	void ShowCardInfo(CardBase*);
private:
	int round;	///< 回合数
	Player player;	///< 玩家
	Player enemy;	///< 敌人
	CardLib cardLib;	///< 卡牌库
	DiscardArea discardArea;	///< 弃牌区
	OperatingArea operatingArea;	///< 手牌区&对战区
	OperatingArea enemyOperatingArea;	///<敌方对战区(仅取对战区)
};

inline void PveGame::SetEnemyCards(vector<CardBase*> enemy_cards)
{
	for (auto card : enemy_cards)
	{
		if (enemyOperatingArea.AddCardsFromPile(card) == false)
			throw PvePlayException();
	}

	for (int i = 0; i < enemy_cards.size(); i++)
	{
		enemyOperatingArea.AddBattleCards(i, i);
		delete enemy_cards[i];
	}
}

inline void PveGame::Play()
{
	using std::vector;
	using std::cout;
	using std::endl;
	// 抽卡
	if (round == 0)
	{
		vector<CardBase*> listening_cards;
		if (!cardLib.Licensing(INIT_LISTENING_NUM, listening_cards))
		{
			std::cerr << "Licensing Error! Init cards num too small\n";
			throw PvePlayException();
		}
		for (auto& card : listening_cards)
		{
			if (!operatingArea.AddCardsFromPile(card))
			{
				std::cerr << "Licensing Error! OperatingArea too small\n";
				throw PvePlayException();
			}
		}
		ShowOperatingArea();
	}
	else
	{
		vector<CardBase*> listening_cards;
		if (!cardLib.Licensing(ROUND_LISTING_NUM, listening_cards))
		{
			cardLib.AddCards(discardArea.GetDiscardCards());
			if (!cardLib.Licensing(ROUND_LISTING_NUM, listening_cards))
			{
				std::cerr << "Licensing Error! Init cards num too small\n";
				throw PvePlayException();
			}
		}
		for (auto& card : listening_cards)
		{
			cout << "获得:";
			ShowCardInfo(card);
			if (operatingArea.AddCardsFromPile(card))
				break;
			cout << "手牌区已满:\n";
			ShowOperatingArea();
			auto n = GetInputInRound(
				"输入序号替换对应卡牌, 输入 -1 舍弃该卡牌", operatingArea.GetCardsNum() - 1);
			if (n >= 0)
			{
				operatingArea.DiscardCardFrom(operatingArea.GetCardsPtr(), n);
				operatingArea.AddCardsFromPile(card);
				ShowOperatingArea();
			}
		}
	}
	// 调整卡牌
	auto mode = GetInputInRound("输入0调整手牌区, 输入1调整对战区, 输入-1完成调整", 1);
	while (mode != -1)
	{
		ShowOperatingArea();
		if (mode == 0)
		{
			auto adjust = GetInputInRound
			("输入调整序号对, 以-1结束将将卡牌弃置, 重复自身以放入对战区, 以-1开头将结束调整",
				operatingArea.GetCardsNum() - 1, operatingArea.GetCardsNum() - 1);
			while (adjust.first != -1)
			{
				if (adjust.second == -1)
				{
					operatingArea.DiscardCardFrom(operatingArea.GetCardsPtr(), adjust.first);
				}
				else if (adjust.first == adjust.second)
				{
					operatingArea.AddBattleCards(adjust.first, operatingArea.GetBattleCardsPtr()->size());
					ShowBattleArea();
				}
				else
				{
					auto cards = operatingArea.GetCardsPtr();
					auto temp = cards[adjust.first];
					cards[adjust.first] = cards[adjust.second];
					cards[adjust.second] = temp;
				}
				ShowOperatingArea();
				adjust = GetInputInRound
				("输入调整序号对, 以-1结束将将卡牌弃置, 以-1开头将结束调整",
					operatingArea.GetCardsNum() - 1, operatingArea.GetCardsNum() - 1);
			}
		}
		else
		{
			cout << "输入调整后卡牌序号, 未存在卡牌将撤回至手牌区\n";
			std::istream is(std::cin.rdbuf());
			auto pre = operatingArea.GetBattleCardsPtr();
			std::vector<CardBase*> temp;
			int idx;
			int num = 0;
			while (is >> idx)
			{
				if(idx < 0 || idx > pre->size())
					throw PvePlayException();
				auto itr = pre->begin();
				for(int i = 0; i < idx; i++)
					itr++;
				if(*itr == nullptr)
					throw PvePlayException();
				temp.push_back(*itr);
				*itr = nullptr;
				num++;
			}
			for (auto e : *pre)
			{
				if(e)
				{
					if (!operatingArea.AddCardsFromBattle(e))
					{
						auto adjust = GetInputInRound
						("输入调整序号弃置手牌区卡牌, -1弃置本身",
							operatingArea.GetCardsNum() - 1);
						ShowCardInfo(e);
						ShowOperatingArea();
						if (adjust == -1)
						{
							discardArea.AddCard(e);
						}
						else
						{
							operatingArea.DiscardCardFrom(operatingArea.GetCardsPtr(), adjust);
							operatingArea.AddCardsFromBattle(e);
						}
					}

				}
			}
			pre->resize(num);
			auto itr = pre->begin();
			for (int i = 0; i < num; i++)
			{
				*itr = temp[i];
			}
			ShowBattleArea();
			ShowOperatingArea();
		}
		mode = GetInputInRound("输入1调整手牌区, 输入2调整对战区, 输入0完成调整", 2);
	}

	// 战斗
	// 回合结束
}

class GameManager
{
	//nothing yet
};