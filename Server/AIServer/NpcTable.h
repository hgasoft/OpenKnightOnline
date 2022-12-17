#pragma once

#define MONSTER_SPEED	1500

class CNpcTable  
{
public:
	uint16_t	m_sSid;				// MONSTER(NPC) Serial ID
	std::string	m_strName;		// MONSTER(NPC) Name
	uint16_t	m_sPid;				// MONSTER(NPC) Picture ID
	uint16_t	m_sSize;			// ĳ������ ����(100 �ۼ�Ʈ ����)
	uint32_t	m_iWeapon_1;		// ����迭(������)
	uint32_t	m_iWeapon_2;		// ����迭(�޼�)
	uint8_t	m_byGroup;			// �Ҽ�����
	uint8_t	m_byActType;		// �ൿ����
	uint8_t	m_tNpcType;			// NPC Type
	// 0 : Monster
	// 1 : Normal NPC
	uint8_t	m_byFamilyType;		// ������̿��� �������踦 �����Ѵ�.
	uint8_t	m_byRank;			// ����
	uint8_t	m_byTitle;			// ����
	uint32_t	m_iSellingGroup;	// ������ �׷�(���ǸŸ� ��� NPC�� ��츸)
	uint16_t	m_sLevel;			// level
	uint32_t	m_iExp;				// ����ġ
	uint32_t	m_iLoyalty;			// loyalty

	uint32_t	m_iMaxHP;			// �ִ� HP
	uint16_t	m_sMaxMP;			// �ִ� MP

	uint16_t	m_sAttack;			// ���ݰ�
	uint16_t	m_sDefense;			// ��
	uint16_t	m_sHitRate;			// Ÿ�ݼ�����
	uint16_t	m_sEvadeRate;		// ȸ�Ǽ�����
	uint16_t	m_sDamage;			// �⺻ ������
	uint16_t	m_sAttackDelay;		// ���ݵ�����
	uint16_t	m_sSpeed;			// �̵��ӵ�	
	uint8_t	m_bySpeed_1;		// �⺻ �̵� Ÿ��
	uint8_t	m_bySpeed_2;		// �ٴ� �̵� Ÿ��..
	uint16_t	m_sStandTime;		// ���ִ� �ð�
	uint32_t	m_iMagic1;			// ��븶�� 1
	uint32_t	m_iMagic2;			// ��븶�� 2
	uint32_t	m_iMagic3;			// ��븶�� 3	

	uint16_t	m_byFireR;			// ȭ�� ���׷�
	uint16_t	m_byColdR;			// �ñ� ���׷�
	uint16_t	m_byLightningR;		// ���� ���׷�
	uint16_t	m_byMagicR;			// ���� ���׷�
	uint16_t	m_byDiseaseR;		// ���� ���׷�
	uint16_t	m_byPoisonR;		// �� ���׷�

	float	m_fBulk;

	uint8_t	m_bySearchRange;	// �� Ž�� ����
	uint8_t	m_byAttackRange;	// �����Ÿ�
	uint8_t	m_byTracingRange;	// �߰ݰŸ�

	uint32_t	m_iMoney;			// �������� ��
	uint16_t	m_iItem;			// �������� ������
	uint8_t	m_byDirectAttack;	// ���ݹ��( 0 : ����, 1:�հ���(��������), 2:��,�������� )
	uint8_t	m_byMagicAttack;	// ��������( 0:�������� ����, 1:��������, 2:������, 3:����)
	uint8_t	m_byGroupSpecial;

	CNpcTable() : m_sSpeed(MONSTER_SPEED)
	{
	}
};