/*****************************************************************//**
 * @file   GameManager.hpp
 * @brief  ��Ϸ������
 *
 * @author tanmi
 * @date   November 2023
 *********************************************************************/
#pragma once

#include <istream>

#include"Player.hpp"
#include"OperatingArea.hpp"
constexpr int MAX_OPERATING_CRAD_NUM = 8; ///< ���������
constexpr int INIT_BATTLE_CRAD_NUM = 4; ///< ��ʼ��ս������
constexpr int MAX_BATTTLE_CRAD_NUM = 8; ///< ����ս������
constexpr int MASTER_CRAD_DISCARD_NUM = 8; ///< �������������������
constexpr int ACTION_POINT_PER_BATTLE_CARD = 3; ///< ÿ�������ܻ�õ��ж�����
constexpr int INIT_LISTENING_NUM = 6; ///< ��ʼ������
constexpr int ROUND_LISTING_NUM = 4; ///< �ִ�������

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
	void SetEnemyCards(vector<CardBase*> enemy_cards); ///<���õз���ս��
	void Play(); ///< ������ѭ��
	bool Rounding(Player& p1, Player& p2,
		OperatingArea& op1, OperatingArea& op2); ///< ���غ���ѭ��
private:
	void ShowOperatingArea();
	void ShowBattleArea();
	int GetInputInRound(std::string, int); ///< ��÷�Χ�ڵ���������
	std::pair<int, int> GetInputInRound(std::string, int, int); ///< ��÷�Χ�ڵ���������
	void ShowCardInfo(CardBase*);
private:
	int round;	///< �غ���
	Player player;	///< ���
	Player enemy;	///< ����
	CardLib cardLib;	///< ���ƿ�
	DiscardArea discardArea;	///< ������
	OperatingArea operatingArea;	///< ������&��ս��
	OperatingArea enemyOperatingArea;	///<�з���ս��(��ȡ��ս��)
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
	// �鿨
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
			cout << "���:";
			ShowCardInfo(card);
			if (operatingArea.AddCardsFromPile(card))
				break;
			cout << "����������:\n";
			ShowOperatingArea();
			auto n = GetInputInRound(
				"��������滻��Ӧ����, ���� -1 �����ÿ���", operatingArea.GetCardsNum() - 1);
			if (n >= 0)
			{
				operatingArea.DiscardCardFrom(operatingArea.GetCardsPtr(), n);
				operatingArea.AddCardsFromPile(card);
				ShowOperatingArea();
			}
		}
	}
	// ��������
	auto mode = GetInputInRound("����0����������, ����1������ս��, ����-1��ɵ���", 1);
	while (mode != -1)
	{
		ShowOperatingArea();
		if (mode == 0)
		{
			auto adjust = GetInputInRound
			("���������Ŷ�, ��-1����������������, �ظ������Է����ս��, ��-1��ͷ����������",
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
				("���������Ŷ�, ��-1����������������, ��-1��ͷ����������",
					operatingArea.GetCardsNum() - 1, operatingArea.GetCardsNum() - 1);
			}
		}
		else
		{
			cout << "��������������, δ���ڿ��ƽ�������������\n";
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
						("������������������������, -1���ñ���",
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
		mode = GetInputInRound("����1����������, ����2������ս��, ����0��ɵ���", 2);
	}

	// ս��
	// �غϽ���
}

class GameManager
{
	//nothing yet
};