/*****************************************************************//**
 * @file   Buffs.hpp
 * @brief  Buff相关
 * 
 * @author tanmi
 * @date   October 2023
 *********************************************************************/
#pragma once

enum Buff
{
	Strength,       // 提升10攻击力/层
	Excitement,     // 提升10%攻击力/层
	Melody,         // 提升6%造成的伤害/层
	Healing,        // 恢复50生命值/层
	Abundance,      // 提升25生命上限/层
	Resilience,     // 提升1点防御/层
	Purification,   // 抵消1个最新debuff/层, 抵消1次控制/10层 // 未完成
	BuffNum
};
enum Debuff
{
	Weakness,		// 使造成的伤害降低5%/层
	Erosion,		// 减少对方3%生命上限/层
	Vulnerability,  // 使受到的伤害提高5%/层
	Fragility,		// 降低1点防御/层
	Bleeding,		// 轮次结束时, 造成10点追加伤害/层
	Fear,			// 清除一个最新buff/层  // 未完成
	DebuffNum
};

enum Control
{
	Freeze,			// 使我方立即行动, 并额外获得1个行动点
	Taunt,			// 强制对方下回合使用攻击, 造成100%攻击力伤害
	ControlNum
};

enum Effect
{
	Mana,			// 消耗以提升卡牌效果
	Wealth,			// 造成1-6点追加伤害/层
	Barrier,        // 获得1生命值护盾/层
	Counterattack,  // 护盾受到伤害时, 立即造成100%攻击力伤害
	Summon,			// 获得召唤物, 拥有本体50%攻击力, 召唤物造成的伤害视为追加伤害
	Charge,			// 记录倒计时5次, 回合结束时减少1次, 倒计时结束时造成50%攻击力伤害
	EffectNum
};