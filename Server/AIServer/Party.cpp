#include "stdafx.h"
#include "Party.h"
#include "User.h"

CParty::CParty()
{
}

CParty::~CParty()
{

}

void CParty::Initialize()
{
}

void CParty::PartyProcess(Packet & pkt)
{
	uint8_t opcode = pkt.read<uint8_t>();
	switch (opcode)
	{
	case PARTY_CREATE:
		PartyCreate(pkt);
		break;
	case PARTY_INSERT:
		PartyInsert(pkt);
		break;
	case PARTY_REMOVE:
		PartyRemove(pkt);
		break;
	case PARTY_DELETE:
		PartyDelete(pkt);
		break;
	}
}

void CParty::PartyCreate(Packet & pkt)
{
	uint16_t sPartyIndex = pkt.read<uint16_t>(), sUid = pkt.read<uint16_t>();
	int16_t sHP = 0, sClass = 0;
	uint8_t  byLevel= 0;
	_PARTY_GROUP* pParty = nullptr;
	CUser* pUser = nullptr;

	pUser = g_pMain->GetUserPtr(sUid);
	if(pUser)	{
		pUser->m_byNowParty = 1;
		pUser->m_sPartyNumber = sPartyIndex;
	}

	pParty = new _PARTY_GROUP;
	pParty->wIndex = sPartyIndex;
	pParty->uid[0] = sUid;

	if( g_pMain->m_arParty.PutData( pParty->wIndex, pParty ) ) {
		TRACE("Party - Create() : Party ����  number = %d, uid=%d, %d \n", sPartyIndex, pParty->uid[0], pParty->uid[1]);
	}
}

void CParty::PartyInsert(Packet & pkt)
{
	uint16_t sPartyIndex = pkt.read<uint16_t>();
	uint8_t byIndex = pkt.read<uint8_t>();
	uint16_t sUid = pkt.read<uint16_t>(), sHP = 0, sClass = 0;
	uint8_t  byLevel= 0;
	_PARTY_GROUP* pParty = nullptr;
	CUser* pUser = nullptr;

	pParty = g_pMain->m_arParty.GetData( sPartyIndex );
	if( !pParty ) {				// �̻��� ���
		return;
	}

	if(byIndex >= 0 && byIndex < 8)	{
		pParty->uid[byIndex] = sUid;

		pUser = g_pMain->GetUserPtr(sUid);
		if(pUser)	{
			pUser->m_byNowParty = 1;
			pUser->m_sPartyNumber = sPartyIndex;
		}
	}
}

void CParty::PartyRemove(Packet & pkt)
{
	uint16_t sPartyIndex = pkt.read<uint16_t>(), sUid = pkt.read<uint16_t>();
	_PARTY_GROUP* pParty = nullptr;
	CUser* pUser = nullptr;

	if (sUid > MAX_USER) return;

	pParty = g_pMain->m_arParty.GetData( sPartyIndex );
	if( !pParty ) {				// �̻��� ���
		return;
	}

	for( int i=0; i<8; i++ ) {
		if( pParty->uid[i] != -1 ) {
			if( pParty->uid[i] == sUid ) {
				pParty->uid[i] = -1;

				pUser = g_pMain->GetUserPtr(sUid);
				if(pUser)	{
					pUser->m_byNowParty = 0;
					pUser->m_sPartyNumber = -1;
				}
			}
		}
	}
}

void CParty::PartyDelete(Packet & pkt)
{
	uint16_t sPartyIndex = pkt.read<uint16_t>();
	_PARTY_GROUP *pParty = g_pMain->m_arParty.GetData(sPartyIndex);
	if (pParty == nullptr)
		return;

	for( int i=0; i<8; i++ ) {
		if( pParty->uid[i] != -1 ) {
			CUser *pUser = g_pMain->GetUserPtr(pParty->uid[i]);
			if(pUser)	{
				pUser->m_byNowParty = 0;
				pUser->m_sPartyNumber = -1;
			}
		}
	}

	g_pMain->m_arParty.DeleteData( pParty->wIndex );
}
