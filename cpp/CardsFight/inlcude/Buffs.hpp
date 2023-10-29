/*****************************************************************//**
 * @file   Buffs.hpp
 * @brief  Buff���
 * 
 * @author tanmi
 * @date   October 2023
 *********************************************************************/
#pragma once

enum Buff
{
	Strength,       // ����10������/��
	Excitement,     // ����10%������/��
	Barrier,        // ���50����ֵ����/��
	Healing,        // �ָ�50����ֵ/��
	Abundance,      // ����25��������/��
	Purification,   // ����1������debuff/��, ����1�ο���/10��
	Resilience,     // ����1�����/��
	Melody,         // ����6%��ɵ��˺�/��
	BuffNum
};
enum Debuff
{
	Bleeding,		// �ִν���ʱ, ���10��׷���˺�/��
	Vulnerability,  // ʹ�ܵ����˺����5%/��
	Fragility,		// ����1�����/��
	Weakness,		// ʹ��ɵ��˺�����5%/��
	Erosion,		// ���ٶԷ�3%��������/��
	Fear,			// ���һ������buff/��
	DebuffNum
};

enum Control
{
	Freeze,			// ʹ�ҷ������ж�, ��������1���ж���
	Taunt,			// ǿ�ƶԷ��»غ�ʹ�ù���, ���100%�������˺�
	ControlNum
};

enum Effect
{
	Mana,			// ��������������Ч��
	Wealth,			// ���1-6��׷���˺�/��
	Counterattack,  // �����ܵ��˺�ʱ, �������100%�������˺�
	Summon,			// ����ٻ���, ӵ�б���50%������, �ٻ�����ɵ��˺���Ϊ׷���˺�
	Charge,			// ��¼����ʱ5��, �غϽ���ʱ����1��, ����ʱ����ʱ���50%�������˺�
	EffectNum
};