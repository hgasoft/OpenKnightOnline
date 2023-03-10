// GameProcMain.cpp: implementation of the CGameProcMain class.
//
//////////////////////////////////////////////////////////////////////
#include <io.h>

//#include "stdafx.h"
#include "resource.h"

#include "GameEng.h"
#include "GameProcMain.h"
#include "LocalInput.h"

#include "APISocket.h"
#include "PacketDef.h"

#include "PlayerMySelf.h"
#include "PlayerOtherMgr.h"
#include "ServerMesh.h"
#include "N3FXMgr.h"

#include "UIManager.h"
#include "UILoading.h"
#include "UIChat.h"
#include "UIInventory.h"
#include "UICmd.h"
#include "UIVarious.h"
#include "UIStateBar.h"
#include "UITargetBar.h"
#include "UITransactionDlg.h"
#include "UIHelp.h"
#include "UIMessageWnd.h"
#include "UINotice.h"
#include "UIDroppedItemDlg.h"
#include "UIPerTradeDlg.h"
#include "UIPartyOrForce.h"
#include "UISkillTreeDlg.h"
#include "UICmdList.h"
#include "UICmdEdit.h"
#include "UIHotKeyDlg.h"
#include "UIClassChange.h"
#include "UINpcEvent.h"
#include "UIItemExchange.h"
#include "UIRepairTooltipDlg.h"
#include "UINpcTalk.h"
#include "UIKnightsOperation.h"
#include "UIPartyBBS.h"
#include "UIWareHouseDlg.h"
#include "UINPCChangeEvent.h"
#include "UIWarp.h"
#include "UIInn.h"
#include "UICreateClanName.h"
#include "UIPartyBBS.h"
#include "UITradeSellBBS.h"
#include "UITradeBBSSelector.h"
#include "UITradeBBSEditDlg.h"
#include "UIQuestMenu.h"
#include "UIQuestTalk.h"
#include "UIDead.h"

#include "SubProcPerTrade.h"
#include "CountableItemEditDlg.h"
#include "MagicSkillMng.h"
#include "WarMessage.h"
#include "GameCursor.h"

#include "N3WorldManager.h"

#include "LightMgr.h"

#include "N3SkyMng.h"
#include "N3ShapeExtra.h"
#include "N3Camera.h"
#include "N3SndObj.h"
#include "N3SndObjStream.h"
#include "N3SndMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

enum e_ChatCmd
{
	CMD_WHISPER, CMD_TOWN, CMD_EXIT, CMD_GREETING, CMD_GREETING2, CMD_GREETING3,
	CMD_PROVOKE, CMD_PROVOKE2, CMD_PROVOKE3, CMD_GAME_SAVE, CMD_RECOMMEND, CMD_INDIVIDUAL_BATTLE,
	CMDSIT_STAND, CMD_WALK_RUN, CMD_LOCATION,
	
	CMD_TRADE, CMD_FORBIDTRADE, CMD_PERMITTRADE, CMD_MERCHANT,

	CMD_PARTY, CMD_LEAVEPARTY, CMD_RECRUITPARTY, CMD_FORBIDPARTY, CMD_PERMITPARTY,

	CMD_JOINCLAN, CMD_WITHDRAWCLAN, CMD_FIRECLAN, CMD_COMMAND, CMD_CLAN_WAR,
	CMD_SURRENDER, CMD_APPOINTVICECHIEF, CMD_CLAN_CHAT, CMD_CLAN_BATTLE,

	CMD_CONFEDERACY, CMD_BAN_KNIGHTS, CMD_QUIT_KNIGHTS, CMD_BASE, CMD_DECLARATION,

	CMD_VISIBLE, CMD_INVISIBLE, CMD_CLEAN, CMD_RAINING, CMD_SNOWING, CMD_TIME, CMD_CU_COUNT,
	CMD_NOTICE, CMD_ARREST, CMD_FORBIDCONNECT, CMD_FORBIDCHAT, CMD_PERMITCHAT, CMD_NOTICEALL,
	CMD_CUTOFF, CMD_VIEW, CMD_UNVIEW, CMD_FORBIDUSER, CMD_SUMMONUSER,
	CMD_ATTACKDISABLE, CMD_ATTACKENABLE, CMD_PLC,

	CMD_COUNT,
	CMD_UNKNOWN = 0xffffffff
};
static std::string s_szCmdMsg[CMD_COUNT]; // ?????? ??????



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcMain::CGameProcMain()				// r???? ??????.. ?? ?????? ?????? ???? ????..
{	
	m_fLBClickTime = 0.0f;
	m_bLoadComplete	= FALSE;
	m_fRequestGameSave = 300.0f;

	//sound obj...
	m_pSnd_Town = NULL;
	m_pSnd_Battle = NULL;

	m_iJoinReqClan = 0;
	m_iJoinReqClanRequierID = 0;

	//UI
	m_pUIMsgDlg = new CUIMessageWnd();
	m_pUIMsgDlg2 = new CUIMessageWnd2();
	m_pUIChatDlg = new CUIChat();
	m_pUIChatDlg2 = new CUIChat2();
	m_pUIStateBarAndMiniMap = new CUIStateBar();
	m_pUIVar = new CUIVarious();
	m_pUICmd = new CUICmd();
	m_pUITargetBar = new CUITargetBar();
	m_pUIHelp = new CUIHelp();
	m_pUINotice = new CUINotice();
	m_pUIClassChange = new CUIClassChange();
	m_pUINpcEvent = new CUINPCEvent();
	m_pUIItemREDlg = new CUIItemExchange();
	m_pUIRepairTooltip = new CUIRepairTooltipDlg();
	m_pUIDroppedItemDlg = new CUIDroppedItemDlg();
	m_pUITransactionDlg = new CUITransactionDlg();
	m_pUIInventory = new CUIInventory();
	m_pUIPartyOrForce = new CUIPartyOrForce();
	m_pUISkillTreeDlg = new CUISkillTreeDlg();
	m_pUICmdListDlg = new CUICmdList();
	m_pUICmdEditDlg = new CUICmdEdit();
	m_pUIHotKeyDlg = new CUIHotKeyDlg();
	m_pUINpcTalk = new CUINpcTalk();
	m_pUIKnightsOp = new CUIKnightsOperation();			// ?????? ?????? ????, ????, ??...
	m_pUIPartyBBS = new CUIPartyBBS(); // ???? ???? ?????? ????????..
	m_pUIWareHouseDlg = new CUIWareHouseDlg();
	m_pUINpcChange = new CUINPCChangeEvent();	
	m_pUIWarp = new CUIWarp();
	m_pUIInn = new CUIInn();
	m_pUICreateClanName = new CUICreateClanName();
	m_pUITradeBBS = new CUITradeSellBBS();
	m_pUITradeBBSSelector = new CUITradeBBSSelector();	
	m_pUITradeBBSEdit = new CUITradeBBSEditDlg();
	m_pUIQuestMenu = new CUIQuestMenu();
	m_pUIQuestTalk = new CUIQuestTalk();
	m_pUIDead = new CUIDead();

	m_pSubProcPerTrade = new CSubProcPerTrade();
	m_pMagicSkillMng = new CMagicSkillMng(this);
	m_pTargetSymbol = new CN3Shape(); // ?????????? ???????? ???? ???????? ???????? ?????? ????..
	m_pWarMessage = new CWarMessage;

	m_pLightMgr = new CLightMgr;
}

CGameProcMain::~CGameProcMain()
{
	this->Release();

	//UI
	delete m_pUIMsgDlg;
	delete m_pUIMsgDlg2;
	delete m_pUIChatDlg;
	delete m_pUIChatDlg2;
	delete m_pUIStateBarAndMiniMap;
	delete m_pUIVar;
	delete m_pUICmd;
	delete m_pUITargetBar;
	delete m_pUIHelp;
	delete m_pUINotice;
	delete m_pUIClassChange;
	delete m_pUINpcEvent;
	delete m_pUIItemREDlg;
	delete m_pUIRepairTooltip;
	delete m_pUIDroppedItemDlg;
	delete m_pUITransactionDlg;
	delete m_pUIInventory;
	delete m_pUIPartyOrForce;
	delete m_pUISkillTreeDlg;
	delete m_pUICmdListDlg;
	delete m_pUICmdEditDlg;
	delete m_pUIHotKeyDlg;
	delete m_pUINpcTalk;
	delete m_pUIKnightsOp;
	delete m_pUIPartyBBS;
	delete m_pUIWareHouseDlg;
	delete m_pUINpcChange;
	delete m_pUIWarp;
	delete m_pUIInn;
	delete m_pUICreateClanName;
	delete m_pUITradeBBS;
	delete m_pUITradeBBSSelector;
	delete m_pUITradeBBSEdit;
	delete m_pUIQuestMenu;
	delete m_pUIQuestTalk;
	delete m_pUIDead;

	delete m_pSubProcPerTrade;
	delete m_pMagicSkillMng;
	delete m_pWarMessage;
	delete m_pTargetSymbol; // ?????????? ???????? ???? ???????? ???????? ?????? ????..

	delete m_pLightMgr;
}

void CGameProcMain::Release()
{
	this->ReleaseSound();
	this->ReleaseUIs();

	m_pSubProcPerTrade->Release();
	m_pMagicSkillMng->Release();
	m_pWarMessage->Release();
	m_pTargetSymbol->Release();
	m_pLightMgr->Release();

	CGameProcedure::Release();
}

void CGameProcMain::ReleaseUIs()
{
	m_pUIChatDlg->Release();
	m_pUIChatDlg2->Release();
	m_pUIMsgDlg->Release();
	m_pUIMsgDlg2->Release();
	m_pUICmd->Release();
	m_pUIVar->Release();
	m_pUIStateBarAndMiniMap->Release();
	m_pUITargetBar->Release();
	m_pUIHelp->Release();
	m_pUINotice->Release();
	m_pUIClassChange->Release();
	m_pUINpcEvent->Release();
	m_pUIItemREDlg->Release();
	m_pUIRepairTooltip->Release();
	m_pUIPartyOrForce->Release();
	m_pUISkillTreeDlg->Release();
	m_pUICmdListDlg->Release();
	m_pUICmdEditDlg->Release();
	m_pUIHotKeyDlg->Release();
	m_pUINpcTalk->Release();
//	m_pUITradeList->Release();
	m_pUIKnightsOp->Release();			// ?????? ?????? ????, ????, ??...
	m_pUIPartyBBS->Release();
	m_pUIWareHouseDlg->Release();
	m_pUINpcChange->Release();
	m_pUIWarp->Release();
	m_pUIInn->Release();
	m_pUICreateClanName->Release();

	CN3UIBase::DestroyTooltip();
}

void CGameProcMain::Init()
{
	CGameProcedure::Init();
	m_pLightMgr->Release();
	s_pEng->SetDefaultLight(m_pLightMgr->Light(0), m_pLightMgr->Light(1), m_pLightMgr->Light(2));

	int i = 0;
	for (uint32_t resource = IDS_CMD_WHISPER; resource <= IDS_CMD_LOCATION; resource++)
		::_LoadStringFromResource(resource, s_szCmdMsg[i++]);

	for (uint32_t resource = IDS_CMD_TRADE; resource <= IDS_CMD_MERCHANT; resource++)
		::_LoadStringFromResource(resource, s_szCmdMsg[i++]);

	for (uint32_t resource = IDS_CMD_PARTY; resource <= IDS_CMD_PERMITPARTY; resource++)
		::_LoadStringFromResource(resource, s_szCmdMsg[i++]);

	for (uint32_t resource = IDS_CMD_JOINCLAN; resource <= IDS_CMD_CLAN_BATTLE; resource++)
		::_LoadStringFromResource(resource, s_szCmdMsg[i++]);

	for (uint32_t resource = IDS_CMD_CONFEDERACY; resource <= IDS_CMD_DECLARATION; resource++)
		::_LoadStringFromResource(resource, s_szCmdMsg[i++]);

	for (uint32_t resource = IDS_CMD_VISIBLE; resource <= IDS_CMD_PLC; resource++)
		::_LoadStringFromResource(resource, s_szCmdMsg[i++]);

	s_SndMgr.ReleaseStreamObj(&(CGameProcedure::s_pSnd_BGM));

	if(m_pWarMessage) m_pWarMessage->InitFont();
	this->InitUI(); // ?????? ???? ???? UI ????...
	this->InitZone(s_pPlayer->m_InfoExt.iZoneCur, s_pPlayer->Position()); // ?? ????..

	//sound obj...
	if(m_pSnd_Battle==NULL)
	{
		int iIDSndBattle = ((NATION_KARUS == s_pPlayer->m_InfoBase.eNation) ? ID_SOUND_BGM_KA_BATTLE : ID_SOUND_BGM_EL_BATTLE);
		m_pSnd_Battle = s_pEng->s_SndMgr.CreateStreamObj(iIDSndBattle);	// ???????? ID
		if(m_pSnd_Battle) 
		{
			m_pSnd_Battle->Looping(true);
			m_pSnd_Battle->Stop();
		}
	}
	if(m_pSnd_Town==NULL)
	{
		m_pSnd_Town = s_pEng->s_SndMgr.CreateStreamObj(ID_SOUND_BGM_TOWN);	// ???????? ID
		if(m_pSnd_Town)
		{
			m_pSnd_Town->Looping(true);
			m_pSnd_Town->Play(NULL, 3.0f);
		}
	}

	if(s_pUILoading) s_pUILoading->Render("Loading Character Data...", 0);

	// ???? ????..
	char szPathOld[_MAX_PATH], szPathFind[_MAX_PATH];
	::GetCurrentDirectory(_MAX_PATH, szPathOld);

	_finddata_t fi;
	long hFind = -1;

	// ?????? ?? ????..
	// ?????????? ?? ????..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Chr");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.N3Anim", &fi);
	if(hFind)
	{
		std::string szFN = "Chr\\";
		szFN += fi.name;
		CN3AnimControl* pObjTmp = s_MngAniCtrl.Get(szFN.c_str());
		while(_findnext(hFind, &fi) != -1)
		{
			szFN = "Chr\\";
			szFN += fi.name;
			pObjTmp = s_MngAniCtrl.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if(s_pUILoading) s_pUILoading->Render("Loading Character Data... 10 %", 10);

	// ?????? ?? ????..
	// ?????? ?? ????..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Item");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.dxt", &fi);
	if(hFind)
	{
		std::string szFN = "Item\\";
		szFN += fi.name;
		CN3Texture* pObjTmp = s_MngTex.Get(szFN.c_str());
		while(_findnext(hFind, &fi) != -1)
		{
			szFN = "Item\\";
			szFN += fi.name;
			pObjTmp = s_MngTex.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if(s_pUILoading) s_pUILoading->Render("Loading Character Data... 25 %", 25);
	
	// ?????? ?? ????..
	// ?????? ?? ????..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Chr");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.N3Joint", &fi);
	if(hFind)
	{
		std::string szFN = "Chr\\";
		szFN += fi.name;
		CN3Joint* pObjTmp = s_MngJoint.Get(szFN.c_str());
		while(_findnext(hFind, &fi) != -1)
		{
			szFN = "Chr\\";
			szFN += fi.name;
			pObjTmp = s_MngJoint.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if(s_pUILoading) s_pUILoading->Render("Loading Character Data... 50 %", 50);

	// ?????? ?? ????..
	// ???? ????..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Item");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.N3CSkins", &fi);
	if(hFind)
	{
		std::string szFN = "Item\\";
		szFN += fi.name;
		CN3CPartSkins* pObjTmp = s_MngSkins.Get(szFN.c_str());
		while(_findnext(hFind, &fi) != -1)
		{
			szFN = "Item\\";
			szFN += fi.name;
			pObjTmp = s_MngSkins.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if(s_pUILoading) s_pUILoading->Render("Loading Character Data... 75 %", 75);
	
	// ?????? ?? ????..
	// PMesh ????..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Item");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.N3PMesh", &fi);
	if(hFind)
	{
		std::string szFN = "Item\\";
		szFN += fi.name;
		CN3PMesh* pObjTmp = s_MngPMesh.Get(szFN.c_str());
		while(_findnext(hFind, &fi) != -1)
		{
			szFN = "Item\\";
			szFN += fi.name;
			pObjTmp = s_MngPMesh.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if(s_pUILoading) s_pUILoading->Render("Loading Character Data... 100 %", 100);

	this->MsgSend_GameStart();

	// ???? ??????..
	::SetCurrentDirectory(szPathOld);
}

void CGameProcMain::InitPlayerPosition(const __Vector3& vPos) // ???????? ???? ??????.. ?????? ??????, ?????????? ?????? ????.
{
	__Vector3 vPosFinal = vPos;
	float fYTerrain = ACT_WORLD->GetHeightWithTerrain(vPos.x, vPos.z);	// ?????? ?????? ????..
	float fYObject = ACT_WORLD->GetHeightNearstPosWithShape(vPos, 1.0f); // ???????????? ???? ?????? ?????? ????..
	if (!s_pWorldMgr->IsIndoor())
	{
		if (T_Abs(vPos.y - fYObject) < T_Abs(vPos.y - fYTerrain)) vPosFinal.y = fYObject; // ???? ?????? ???? ??????..
		else vPosFinal.y = fYTerrain;
	}
	else
	{
		if (fYObject > fYTerrain)
			vPosFinal.y = fYObject;
		else
			vPosFinal.y = fYTerrain;
	}

	s_pPlayer->PositionSet(vPosFinal, true);	// ?????? ???? ????..	
	s_pPlayer->m_vPosFromServer = vPos;
	m_vPlayerPosSended = vPos;					// ?????? ???? ???? ????..
	m_fMsgSendTimeMove = 0;						// ?????? ????????..

	this->CommandSitDown(false, false, true); // ?????? ??????.. ???????? ???????? ????????.. ?????? ????..
	this->TargetSelect(-1, false); // ???? ????..
	this->UpdateCameraAndLight(); // ???????? ?????? ???? ????..

	s_pPlayer->Action(PSA_BASIC, true, NULL, true); // ?????? ???? ????..
}

void CGameProcMain::Tick()
{
	CGameProcedure::Tick();	// ??, ?????? ???? ????..

	if ( FALSE == m_bLoadComplete ) return;				// ?????? ??????????.. ????????.
	if(!s_pSocket->IsConnected()) return;

#ifdef _DEBUG
	if(s_pLocalInput->IsKeyPressed(SDL_SCANCODE_F11))
	{
		uint8_t byBuff[32];
		int iOffset = 0;
		CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_TEST_PACKET);
		s_pSocket->Send(byBuff, iOffset);


		int iNPC = s_pOPMgr->m_NPCs.size();
		char szBuff[1024];
		sprintf(szBuff, "NPC Region Test : %d", iNPC);
		CLogWriter::Write(szBuff);
		it_NPC it = s_pOPMgr->m_NPCs.begin(), itEnd = s_pOPMgr->m_NPCs.end();
		for(; it != itEnd; it++)
		{
			CPlayerNPC* pNPC = it->second;

			sprintf(szBuff, "    ID(%d) Name(%s) Pos(%.1f, %.1f)",
				pNPC->IDNumber(),
				pNPC->IDString().c_str(), 
				pNPC->m_vPosFromServer.x,
				pNPC->m_vPosFromServer.z );
			CLogWriter::Write(szBuff);
		}

	}
#endif

	uint32_t dwMouseFlags = s_pLocalInput->MouseGetFlag();	// ?????? ???? ?????? - LocalInput.h ????
	this->ProcessLocalInput(dwMouseFlags);					// ???????? ?????? ?????? UI ?????? ????...

	MsgSend_Continous();									// ???? ???????? ???????? ??????, ???????? ???????? ???? ?????? ????..

	s_pPlayer->Tick();									// ???????? ??(????)
	s_pWorldMgr->Tick();
	s_pOPMgr->Tick(s_pPlayer->Position());				// ???? ???? ?????? ??(????)
//	s_pFX->Tick(); //???????? ?????? ???? ?? ???? ?????? ?????? ???? Render()???? ?????? ????...

	__Vector3 ListenerPos = s_pPlayer->Position();
	__Vector3 ListenerDir = s_pPlayer->Direction();
	__Vector3 ListenerUp(0,1,0);

	// Sound Tick...
	CN3SndObj::SetListenerPos(&ListenerPos);
	CN3SndObj::SetListenerOrientation(&ListenerDir, &ListenerUp);

	this->UpdateUI_MiniMap(); // ?????? ????????..
	this->UpdateUI_TargetBar(); // ?????? ????..
	this->UpdateBGM(); // ?????????? ?????? ???? ????..
	this->UpdateCameraAndLight(); // ???????? ?????? ????..
	
//	ProcessPlayerInclination();							// ???? ????..(?????? ?????? ?????? ?????? ????????..).
#ifdef _N3_64GRID_
	m_SMesh.Tick(s_pPlayer, &m_Terrain);				// ???? ???? ??.(????)
#endif

	m_pUIStateBarAndMiniMap->UpdatePosition(s_pPlayer->Position(), s_pPlayer->Yaw()); // ???? ????????.

	if(m_pMagicSkillMng) m_pMagicSkillMng->Tick();
	if(m_pWarMessage) m_pWarMessage->Tick();
	if(m_pLightMgr) m_pLightMgr->Tick();
	
	////////////////////////////////////////////////////////////////////////////////////
	// ???? ?????? ?????????? 2???? ???? WIZ_TIME_NOTIFY ??????..
	float fTime = CN3Base::TimeGet();
	static float fTimePrev = fTime;
	
	static float fTimeInterval1 = 0;
	if(0 == s_pSocket->m_iSendByteCount)
	{
		fTimeInterval1 += fTime - fTimePrev;
		if(fTimeInterval1 >= 2.0f)
		{
			uint8_t byCmd = WIZ_TIMENOTIFY;
			s_pSocket->Send(&byCmd, 1);
			s_pSocket->m_iSendByteCount = 0;
			fTimeInterval1 = 0;
		}
	}
	else
	{
		s_pSocket->m_iSendByteCount = 0;
		fTimeInterval1 = 0;
	}
	// ???? ?????? ?????????? 2???? ???? WIZ_TIME_NOTIFY ??????..
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// ?????? ?????? ????..
	static float fInterval2 = 0, fInterval3 = 0, fInterval4 = 0, fInterval5 = 0;
	fInterval2 += fTime - fTimePrev;
	fInterval3 += fTime - fTimePrev;
	fInterval4 += fTime - fTimePrev;
	fInterval5 += fTime - fTimePrev;
	m_fRequestGameSave += fTime - fTimePrev;
	if(fInterval2 > 1200.0f) // ???? ????..
	{
		uint8_t byBuff[4];												// ????.. 
		int iOffset=0;												// ????..
		s_pSocket->MP_AddByte(byBuff, iOffset, WIZ_DATASAVE);	// ???? ???? ??????..
		s_pSocket->Send(byBuff, iOffset);				// ????..

		fInterval2 = 0.0f;
	}
	if(fInterval3 > 10.0f) // ???????? ????.. 
	{
		MsgSend_SpeedCheck();										// ???????? ???? ????

		fInterval3 = 0.0f;
	}
	if(s_pPlayer->m_InfoBase.iLevel < 12 && fInterval4 > 20.0f) // ?????? ?????? ?? ?????? ????..
	{
		std::string szMsg;
		::_LoadStringFromResource(IDS_HELP_TIP_ALL, szMsg);
		this->m_pUIMsgDlg->AddMsg(szMsg, 0xffffff00);

		::_LoadStringFromResource(IDS_HELP_TIP1 + (rand() % 33), szMsg);
		this->m_pUIMsgDlg->AddMsg(szMsg, 0xffffff00); // ???? ????..
		fInterval4 = 0;
	}
	if(fInterval5 > 5.0f) // ?????? ?????? ?? ?????? ????..
	{
//		m_pUIChatDlg->ChangeChattingMode(N3_CHAT_CONTINUE); // ???????? ?????? ??????...
		m_pUIChatDlg->ShowContinueMsg();
		fInterval5 = 0;
	}
	fTimePrev = fTime;
	// ?????? ?????? ????..
	////////////////////////////////////////////////////////////////////////////////////
}

void CGameProcMain::Render()
{
	if ( FALSE == m_bLoadComplete )	return; 		// ?????? ????????

	D3DCOLOR crSky = ACT_WORLD->GetSkyColorWithSky();
	s_pEng->Clear(crSky); // ???? ?????? ?????? ??????.. -> ?????????? ?????? ???? ?????? ?????? ??????..
	s_pEng->s_lpD3DDev->BeginScene();			// ?? ???? ????...
	
	ACT_WORLD->RenderSky();								// ???? ??????..
	float fSunAngle = ACT_WORLD->GetSunAngleByRadinWithSky(); // ???? ?????? ????????..

	uint32_t dwFilter = D3DTEXF_LINEAR;
	CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MINFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MAGFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MIPFILTER, dwFilter);

	ACT_WORLD->RenderTerrain();						// ???? ????..
	ACT_WORLD->RenderShape();						// ???? ????..
	s_pOPMgr->Render(fSunAngle);				// ???? ???????? ????..
	s_pPlayer->Render(fSunAngle);			// ???????? ????..

	// NOTE(srmeier): uncomment to render the collision meshes
#ifdef _DEBUG
	ACT_WORLD->RenderCollisionWithShape(s_pPlayer->Position());				// ???? ???? ????..
#endif

#ifdef _N3_64GRID_
	m_SMesh.Render();							// ???? ???? ????..
#endif

	this->RenderTarget();						// ???????? ???? ?????? ???? ???????? ??????..

	ACT_WORLD->RenderGrass();						//	?? ???? (asm)
	s_pFX->Tick();
	s_pFX->Render();
	ACT_WORLD->RenderBirdMgr();

	CN3Base::s_AlphaMgr.Render(); // ???? ?????? ???????? ??????..
	
	ACT_WORLD->RenderSkyWeather();							// ???? ??????..
	
	CGameProcedure::Render(); // UI ?? ?????? ???????? ???? ??????..
	if(m_pWarMessage) m_pWarMessage->RenderMessage();
	if(s_pGameCursor) s_pGameCursor->Render();

	s_pEng->s_lpD3DDev->EndScene();
	s_pEng->Present(CN3Base::s_hWndBase);
}

void CGameProcMain::RenderTarget()
{
	if(NULL == m_pTargetSymbol) return;

	// ?????????? ???????? ???? ???????? ???????? ?????? ????..
	CPlayerBase* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);//?????? ???????? ???????? ?????? ???????? ????.
	if(NULL == pTarget && NULL == s_pPlayer->m_pObjectTarget) return;

	float fScale = 1;
	float fYScale = 1;
	__Vector3 vPos(0,0,0);

	if(pTarget)
	{
		fYScale = pTarget->Height() * 1.3f;
		fScale = pTarget->Radius() * 2.0f;
		vPos = pTarget->Position();
	}
	else 
	{
		__Vector3 vTmp = s_pPlayer->m_pObjectTarget->Max() - s_pPlayer->m_pObjectTarget->Min();
		fYScale = vTmp.y;
		vTmp.y = 0;
		fScale = vTmp.Magnitude();
		vPos = s_pPlayer->m_pObjectTarget->Pos();
	}

	m_pTargetSymbol->ScaleSet(fScale, fYScale, fScale);
	m_pTargetSymbol->PosSet(vPos);
	m_pTargetSymbol->Tick();
	if(m_pTargetSymbol->Part(1)) // ?????? ?????? ?????? ???? ??????..
	{
		CN3PMesh* pPMesh = m_pTargetSymbol->Part(1)->Mesh();
		if(pPMesh && pPMesh->GetMaxNumVertices() == 4)
		{
			__VertexT1* pVs = m_pTargetSymbol->Part(1)->Mesh()->GetVertices();
			for(int i = 0; i < 4; i++)
			{
				pVs[i].y =	ACT_WORLD->GetHeightWithTerrain(vPos.x + (pVs[i].x * fYScale), vPos.z + (pVs[i].z * fYScale));
				pVs[i].y -= vPos.y;
				pVs[i].y /= fYScale;
				pVs[i].y += 0.1f / fYScale;
			}
		}
	}
	m_pTargetSymbol->Render();

	// NOTE(srmeier): uncomment to render the collision meshes
#ifdef _DEBUG
	if(pTarget) pTarget->RenderCollisionMesh();
	if(s_pPlayer->m_pObjectTarget) s_pPlayer->m_pObjectTarget->RenderCollisionMesh();
#endif
}

bool CGameProcMain::ProcessPacket(Packet& pkt)
{
	size_t rpos = pkt.rpos();
	if (CGameProcedure::ProcessPacket(pkt))
		return true;

	pkt.rpos(rpos);

	int iCmd = pkt.read<uint8_t>();		// ?????? ????..

	switch ( iCmd )										// ???????? ?????? ????..
	{
#ifdef _DEBUG
	case WIZ_TEST_PACKET:
		{
			int iNPC = pkt.read<int16_t>();
			char szBuff[32];
			sprintf(szBuff, "NPC Region Test : %d -> ", iNPC);
			std::string szLog = szBuff;
			for(int i = 0; i < iNPC; i++)
			{
				int iID = pkt.read<int16_t>();
				sprintf(szBuff, "%d, ", iID);
				szLog += szBuff;
			}
			CLogWriter::Write(szLog.c_str());
		}
		return true;
#endif
		case WIZ_ZONEABILITY: {
			// NOTE(srmeier): this is a custom packet used to set terrain zoneability
			Uint8 opcode = pkt.read<uint8_t>();

			switch (opcode) {
				case 0x03://0x01:
					uint16_t zoneFlags = pkt.read<uint16_t>();
					ZoneAbilityType zoneType = (ZoneAbilityType)pkt.read<uint8_t>();
					uint8_t zoneTariff = pkt.read<uint8_t>();
					uint8_t minLevel = pkt.read<uint8_t>();
					uint8_t maxLevel = pkt.read<uint8_t>();

					ACT_WORLD->m_zoneFlags  = zoneFlags;
					ACT_WORLD->m_zoneType   = zoneType;
					ACT_WORLD->m_byTariff   = zoneTariff;
					ACT_WORLD->m_byMinLevel = minLevel;
					ACT_WORLD->m_byMaxLevel = maxLevel;
				break;
			}
		} return true;

		case WIZ_DEBUG_STRING_PACKET: {
			// NOTE(srmeier): testing this debug string functionality

			int iLen = pkt.read<int16_t>();

			std::string szDebugString;
			pkt.readString(szDebugString, iLen);

			MsgOutput("DEBUG: "+szDebugString, D3DCOLOR_ARGB(255, 255, 255, 0));

		} return true;

		case WIZ_EVENT:
		case WIZ_MERCHANT_INOUT: {
			// need to implement these
		} return true;

		case WIZ_GAMESTART: {
			// NOTE(srmeier): send for the second half of the gamestart process

			uint8_t byBuff[32];
			int iOffset = 0;

			CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_GAMESTART);
			CAPISocket::MP_AddByte(byBuff, iOffset, 0x02);

			s_pSocket->Send(byBuff, iOffset);

		} return true;

		case WIZ_MYINFO:									// ???? ???? ??????..
			this->MsgRecv_MyInfo_All(pkt);
			return true;
		case WIZ_HP_CHANGE:
			this->MsgRecv_MyInfo_HP(pkt);
			return true;
		case WIZ_MSP_CHANGE:
			this->MsgRecv_MyInfo_MSP(pkt);
			return true;
		case WIZ_EXP_CHANGE:
			this->MsgRecv_MyInfo_EXP(pkt);
			return true;
		case WIZ_LOYALTY_CHANGE:
			this->MsgRecv_MyInfo_RealmPoint(pkt);
			return true;
		case WIZ_LEVEL_CHANGE:
			this->MsgRecv_MyInfo_LevelChange(pkt);
			return true;
		case WIZ_POINT_CHANGE:
			this->MsgRecv_MyInfo_PointChange(pkt);
			return true;
		case WIZ_CHAT:														// ???? ??????..	
			this->MsgRecv_Chat(pkt);
			return true;
		case WIZ_WARP:
			{
				float fX = (pkt.read<uint16_t>())/10.0f;
				float fZ = (pkt.read<uint16_t>())/10.0f;

				float fY = ACT_WORLD->GetHeightWithTerrain(fX, fZ, true);
				float fYObject = ACT_WORLD->GetHeightWithShape(fX, fZ);
				if(fYObject > fY) fY = fYObject;
				this->InitPlayerPosition(__Vector3(fX, fY, fZ)); // ???????? ???? ??????.. ?????? ??????, ?????????? ?????? ????.
				s_pFX->TriggerBundle(
					s_pPlayer->IDNumber(),
					-1,
					s_pPlayer->m_InfoBase.eNation == NATION_KARUS
						? FXID_WARP_KARUS
						: FXID_WARP_ELMORAD,
					s_pPlayer->IDNumber(),
					-1,
					0);
			}
			return true;
		case WIZ_MOVE:
			this->MsgRecv_UserMove(pkt);
			return true;
		case WIZ_ROTATE:												// ???? ??????..
			this->MsgRecv_Rotation(pkt);
			return true;
		case WIZ_REGENE:
			{
//				if(m_pUIDead) m_pUIDead->MsgRecv_Revival(pkt);
				this->MsgRecv_Regen(pkt);
				std::string szMsg = "Press OK to teleport back to the re-spawn point."; //IDS_REGENERATION (3701) in 1.298 client.
				MessageBoxClose(szMsg);
				m_pUITargetBar->SetVisible(false);
			}
			return true;
		case WIZ_DEAD:
			this->MsgRecv_Dead(pkt);
			return true;
		case WIZ_TIME:
			this->MsgRecv_Time(pkt);
			return true;
		case WIZ_WEATHER:
			this->MsgRecv_Weather(pkt);
			return true;
		case WIZ_USER_INOUT:												// ???? ???? ??/????..
			this->MsgRecv_UserInOut(pkt);
			return true;
		case WIZ_REGIONCHANGE:										// ???? ?????????? ?? ???? ?????? ???????? ????????...
			this->MsgRecv_UserInAndRequest(pkt);
			return true;
		case WIZ_REQ_USERIN:										// ?????? ?????? UserIn ?? ???? ?????? ???? ????..
			this->MsgRecv_UserInRequested(pkt);						// 
			return true;
		case WIZ_NPC_REGION:										// ???? ?????????? ?? ???? ?????? ???????? ????????...
			this->MsgRecv_NPCInAndRequest(pkt);
			return true;
		case WIZ_REQ_NPCIN:											// ?????? ?????? UserIn ?? ???? ?????? ???? ????..
			this->MsgRecv_NPCInRequested(pkt);						// 
			return true;
		case WIZ_NPC_INOUT:												// NPC ??/????..
			this->MsgRecv_NPCInOut(pkt);
			return true;
		case WIZ_ATTACK:
			this->MsgRecv_Attack(pkt);
			return true;
		case WIZ_NPC_MOVE:												// NPC ?????? ????..
			this->MsgRecv_NPCMove(pkt);
			return true;
		case WIZ_TARGET_HP:
			this->MsgRecv_TargetHP(pkt);
			return true;
		case WIZ_ITEM_MOVE:
			this->MsgRecv_ItemMove(pkt);				// Item Move?? ???? ????..
			return true;
		case WIZ_ITEM_DROP:
			this->MsgRecv_ItemBundleDrop(pkt);
			return true;
		case WIZ_BUNDLE_OPEN_REQ:
			this->MsgRecv_ItemBundleOpen(pkt);
			return true;
		case WIZ_TRADE_NPC:
			this->MsgRecv_ItemTradeStart(pkt);
			return true;
		case WIZ_ITEM_TRADE:
			this->MsgRecv_ItemTradeResult(pkt);
			return true;
		case WIZ_ITEM_GET:
			this->MsgRecv_ItemDroppedGetResult(pkt);					// ???? ?????? ?????? ???? ????..
			return true;
		case WIZ_REPAIR_NPC:
			this->MsgRecv_NpcEvent(pkt);
			return true;
		case WIZ_ITEM_REPAIR:
			this->MsgRecv_ItemRepair(pkt);
			return true;
		case WIZ_ITEM_COUNT_CHANGE:
			this->MsgRecv_ItemCountChange(pkt);
			return true;
		case WIZ_ITEM_REMOVE:
			this->MsgRecv_ItemDestroy(pkt);
			return true;
		case WIZ_WEIGHT_CHANGE:
			this->MsgRecv_ItemWeightChange(pkt);
			return true;
		case WIZ_USERLOOK_CHANGE:
			this->MsgRecv_UserLookChange(pkt);
			return true;
		case WIZ_ZONE_CHANGE:
			this->MsgRecv_ZoneChange(pkt);
			return true;
		case WIZ_STATE_CHANGE:
			this->MsgRecv_UserState(pkt);
			return true;
		case WIZ_NOTICE:
			this->MsgRecv_Notice(pkt);
			return true;
		case WIZ_PARTY:
			this->MsgRecv_PartyOrForce(pkt);
			return true;
		case WIZ_EXCHANGE:
			this->MsgRecv_PerTrade(pkt);
			return true;
		case WIZ_SKILLPT_CHANGE:
			this->MsgRecv_SkillChange(pkt);
			return true;
		case WIZ_MAGIC_PROCESS:
			this->MsgRecv_MagicProcess(pkt);
			return true;
		case WIZ_CLASS_CHANGE:
			this->MsgRecv_NpcChangeOpen(pkt);
			return true;
		case WIZ_OBJECT_EVENT:
			this->MsgRecv_ObjectEvent(pkt);
			return true;
		case WIZ_CHAT_TARGET:
			{
				Uint8 type = pkt.read<uint8_t>();
				int err = pkt.read<int16_t>();

				std::string szID, szMsg;
				int iLen = pkt.read<int16_t>();		
				pkt.readString(szID, iLen);	

				e_ChatMode eCM = N3_CHAT_UNKNOWN;
				if(szID.empty())
				{
					::_LoadStringFromResource(IDS_CHAT_SELECT_TARGET_FAIL, szMsg);
					eCM = N3_CHAT_NORMAL;
				}
				else
				{
					::_LoadStringFromResource(IDS_CHAT_SELECT_TARGET_SUCCESS, szMsg);
					eCM = N3_CHAT_PRIVATE;
				}
				
				this->MsgOutput(szID + " " + szMsg, 0xffffff00);
				m_pUIChatDlg->ChangeChattingMode(eCM); 
			}
			return true;
		case WIZ_CONCURRENTUSER: // ???? ???????? ...
			{
				int iUserCount = pkt.read<int16_t>();		// ID ?????? ????..

				std::string szFmt;
				::_LoadStringFromResource(IDS_FMT_CONCURRENT_USER_COUNT, szFmt);
				char szBuff[128] = "";
				sprintf(szBuff, szFmt.c_str(), iUserCount);
				this->MsgOutput(szBuff, D3DCOLOR_ARGB(255,255,255,0));
			}
			return true;
		case WIZ_DURATION:
			this->MsgRecv_DurabilityChange(pkt);
			return true;
		case WIZ_KNIGHTS_PROCESS:
			this->MsgRecv_Knights(pkt);
			return true;
		case WIZ_KNIGHTS_LIST:
			this->MsgRecv_KnightsListBasic(pkt);
			return true;
		case WIZ_CONTINOUS_PACKET: // ?????? ?????? ????... ???? ?? ???????? ????!!!
			this->MsgRecv_ContinousPacket(pkt);
			return true;
		case WIZ_WAREHOUSE:	// ??????..
			this->MsgRecv_WareHouse(pkt);			// ?????? ???? ????..
			return true;
		case WIZ_FRIEND_PROCESS:
			if(m_pUIVar->m_pPageFriends) m_pUIVar->m_pPageFriends->MsgRecv_MemberInfo(pkt);
			return true;
		case WIZ_GOLD_CHANGE:
			this->MsgRecv_NoahChange(pkt);
			return true;
		case WIZ_WARP_LIST:
			this->MsgRecv_WarpList(pkt);
			return true;
//		case WIZ_VIRTUAL_SERVER:
//			this->MsgRecv_ServerCheckAndRequestConcurrentUserCount(pkt);
//			return true;
//		case WIZ_ZONE_CONCURRENT:
//			this->MsgRecv_ConcurrentUserCountAndSendServerCheck(pkt);
//			return true;
		case WIZ_CORPSE: //regen?? ???? ???? ???????? ???????? ??????.
			this->MsgRecv_Corpse(pkt);
			return true;
		case WIZ_PARTY_BBS:
			if(m_pUIPartyBBS) m_pUIPartyBBS->MsgRecv_RefreshData(pkt);
			return true;
		case WIZ_MARKET_BBS:
			if(m_pUITradeBBS) m_pUITradeBBS->MsgRecv_TradeBBS(pkt);
			return true;
		case WIZ_SELECT_MSG:
			if(m_pUIQuestMenu) m_pUIQuestMenu->Open(pkt);
			return true;
		case WIZ_NPC_SAY:
			if(m_pUIQuestTalk) m_pUIQuestTalk->Open(pkt);
			return true;
//		case N3_CLAN:
//			this->MsgRecv_Clan(pkt);
//			return true;
		case WIZ_QUEST:
			uint8_t start = pkt.read<uint8_t>();
			uint16_t questId = pkt.read<uint16_t>();
			uint8_t state = pkt.read<uint8_t>();
			if (state == 1)
			{				
				char buff[100];
				sprintf(buff, "You have started quest:%d", questId);
				//::_LoadStringFromResource(IDS_SKILL_FAIL_CASTING, buff);
				this->MsgOutput(buff, D3DCOLOR_ARGB(255, 255, 255, 255));
				return true;
			}
			break;
	}

#ifdef _DEBUG
	if (!m_pUIChatDlg)
		return false;

	char szBuffer[128];
	sprintf(szBuffer, "Unhandled packet. Opcode: 0x%02x", iCmd);

	std::string szMessage = szBuffer;
	m_pUIChatDlg->AddChatMsg(N3_CHAT_NORMAL, szMessage, 0xffff0000);
#endif
	return false;
}

// ???????? ?????? ???????? ????????..
void CGameProcMain::ProcessLocalInput(uint32_t dwMouseFlags)
{
	// Loading?? ?? ??..
	if ( FALSE == m_bLoadComplete ) return; 

	if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE)
		return;

	if (!s_bIsWindowInFocus)
		return;

	if (s_bWindowHasMouseFocus)
	{
		POINT ptPrev = s_pLocalInput->MouseGetPosOld();
		POINT ptCur = s_pLocalInput->MouseGetPos();

		OnMouseMove(ptCur, ptPrev);

		//static POINT ptPrev_RB ={};

		if (dwMouseFlags & MOUSE_RBCLICK)
		{
			// NOTE: right click on NPCs, interactable shapes, item boxes, etc.
			OnMouseRBtnPress(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_RBDOWN)
		{
			// NOTE: this is where the right click rotation and zoom out occur
			//if(!SDL_GetRelativeMouseMode()) ptPrev_RB = ptCur;
			//else {
			//	int x, y;
			//	SDL_GetWindowPosition(s_hWndBase, &x, &y);
			//	SetCursorPos(ptPrev_RB.x+x, ptPrev_RB.y+y);
			//}
			OnMouseRbtnDown(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_RBCLICK)
		{
			OnMouseRBtnPressd(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_RBDBLCLK)
		{
			OnMouseRDBtnPress(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_LBCLICK)
		{
			// NOTE: move on click
			OnMouseLBtnPress(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_LBDOWN)
		{
			// NOTE: move on held down click
			OnMouseLbtnDown(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_LBCLICKED)
		{
			OnMouseLBtnPressd(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_LBDBLCLK)
		{
			OnMouseLDBtnPress(ptCur, ptPrev);
		}
		if (dwMouseFlags & MOUSE_MBCLICK)
		{
			OnMouseMBtnPress(ptCur, ptPrev);
		}

		// reset mouse visibility
		if (!(dwMouseFlags&MOUSE_RBDOWN) && SDL_ShowCursor(SDL_QUERY) == SDL_DISABLE) {
			SDL_ShowCursor(SDL_ENABLE);
		}
		//if(!(dwMouseFlags&MOUSE_RBDOWN) && SDL_GetRelativeMouseMode()) {
		//	SDL_SetRelativeMouseMode(SDL_FALSE);
		//	int x, y;
		//	SDL_GetWindowPosition(s_hWndBase, &x, &y);
		//	SetCursorPos(ptPrev_RB.x+x, ptPrev_RB.y+y);
		//	s_pLocalInput->MouseSetPos(ptPrev_RB.x+x, ptPrev_RB.y+y);
		//}


		// Moves camera when mouse is on the borders of the screen. For both X & Y
		if (!(dwMouseFlags & MOUSE_RBDOWN))
		{
			float fRotY = 0, fRotX = 0;
			if (0 == ptCur.x) fRotY = -2.0f;
			else if ((CN3Base::s_CameraData.vp.Width - 1) == ptCur.x) fRotY = 2.0f;
			if (0 == ptCur.y) fRotX = -1.0f;
			else if ((CN3Base::s_CameraData.vp.Height - 1) == ptCur.y) fRotX = 1.0f;
			if (fRotY)
			{
				if (VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->CameraYawAdd(fRotY);
				else s_pPlayer->RotAdd(fRotY);
			}
			if (fRotX && VP_THIRD_PERSON != s_pEng->ViewPoint()) s_pEng->CameraPitchAdd(fRotX);
		}
	}

	int iHotKey = -1;
	if( s_pLocalInput->IsKeyPress(KM_HOTKEY1) ) iHotKey = 0;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY2) ) iHotKey = 1;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY3) ) iHotKey = 2;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY4) ) iHotKey = 3;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY5) ) iHotKey = 4;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY6) ) iHotKey = 5;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY7) ) iHotKey = 6;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY8) ) iHotKey = 7;
		
	if ((iHotKey >= 0 && iHotKey < 8) &&
		CN3UIBase::GetFocusedEdit() == NULL && 
		m_pSubProcPerTrade->m_ePerTradeState == PER_TRADE_STATE_NONE )
	{
		m_pUIHotKeyDlg->EffectTriggerByHotKey(iHotKey);
	}
	// ????
	//////////////////////////////////////////

	if(s_pLocalInput->IsKeyPress(KM_CAMERA_CHANGE))												// ???? ????..
	{
		this->CommandCameraChange(); // ?????? ???? ??????..
	}

	// ?????????? ??, ???????? ?????? ?????? ??????..
	if(s_pEng->ViewPoint() == VP_THIRD_PERSON)
	{
		float fPitch = 0;
		if(s_pLocalInput->IsKeyDown(SDL_SCANCODE_HOME)) fPitch = D3DXToRadian(45.0f);		// home ???? ??????..
		else if(s_pLocalInput->IsKeyDown(SDL_SCANCODE_END)) fPitch = D3DXToRadian(-45.0f);	// End ???? ??????..
		if(fPitch) s_pEng->CameraPitchAdd(fPitch);
	}

	if (!IsUIKeyOperated() && NULL == CN3UIBase::GetFocusedEdit() )			// ?????????? ?????? 
	{
		if(s_pPlayer->m_InfoBase.iAuthority == AUTHORITY_MANAGER) //???? ???????? ?? ?????? ???????? ????.
		{
			if ( s_pLocalInput->IsKeyDown(SDL_SCANCODE_Q) ) s_pPlayer->m_bTempMoveTurbo = true; // ???? ???? ???????? ????..  // ???? ????.. ?????? ??????..
			else s_pPlayer->m_bTempMoveTurbo = false; // ???? ???? ???????? ????..  // ???? ????.. ?????? ??????..
		}

		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_ATTACK))
			this->CommandToggleAttackContinous();		// ???? ????..}
		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_RUN))
			this->CommandToggleWalkRun();				// ???? / ???? ????	
		if(s_pLocalInput->IsKeyPress(KM_TARGET_NEARST_ENEMY))
			this->CommandTargetSelect_NearstEnemy();	// ???? ?????? ?? ???? ????..
		if(s_pLocalInput->IsKeyPress(KM_TARGET_NEARST_PARTY))
			this->CommandTargetSelect_NearstOurForce(); // ???? ?????? ???? ????????..

		float fRotKeyDelta = D3DXToRadian(60); // ???? 60 ?? ????..
		if(s_pLocalInput->IsKeyDown(KM_ROTATE_LEFT) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_LEFT))	
		{
			if(s_pPlayer->IsAlive()) s_pPlayer->RotAdd(-fRotKeyDelta); // ???? 180 ?? ???????? ????.
			if(m_pUIDroppedItemDlg->IsVisible()) m_pUIDroppedItemDlg->LeaveDroppedState();	
		}
		if(s_pLocalInput->IsKeyDown(KM_ROTATE_RIGHT) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_RIGHT))	
		{
			if(s_pPlayer->IsAlive()) s_pPlayer->RotAdd(fRotKeyDelta); // ???? 180 ?? ?????????? ????.
			if(m_pUIDroppedItemDlg->IsVisible()) m_pUIDroppedItemDlg->LeaveDroppedState();	
		}
		
		if(s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_UP))
		{
			bool bStart = false;
			if(s_pLocalInput->IsKeyPress(KM_MOVE_FOWARD) || s_pLocalInput->IsKeyPress(SDL_SCANCODE_UP))
			{
				if(VP_THIRD_PERSON == s_pEng->ViewPoint())
				{
					if(!s_pPlayer->m_bTargetOrPosMove) bStart = true;
					s_pPlayer->m_bTargetOrPosMove = false;
					if(s_pPlayer->m_bAttackContinous)
						CommandToggleAttackContinous();
				}
				else
					bStart = true;// ?????? ????????
			}
			this->CommandMove(MD_FOWARD, bStart); // ?????? ????..
		}
		else if(s_pLocalInput->IsKeyDown(KM_MOVE_BACKWARD) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_DOWN))
		{
			bool bStart = false;
			if(s_pLocalInput->IsKeyPress(KM_MOVE_BACKWARD) || s_pLocalInput->IsKeyPress(SDL_SCANCODE_DOWN)) 
			{
				if(VP_THIRD_PERSON == s_pEng->ViewPoint())
				{
					if(!s_pPlayer->m_bTargetOrPosMove) bStart = true;
					s_pPlayer->m_bTargetOrPosMove = false;
					if(s_pPlayer->m_bAttackContinous)
						CommandToggleAttackContinous();
				}
				else
					bStart = true;// ?????? ????????
			}
			this->CommandMove(MD_BACKWARD, bStart); // ???? ????..
		}
		else if(s_pLocalInput->IsKeyPress(KM_TOGGLE_MOVE_CONTINOUS))
		{
			this->CommandToggleMoveContinous();
		}

		if(	s_pLocalInput->IsKeyPressed(KM_MOVE_FOWARD) || s_pLocalInput->IsKeyPressed(SDL_SCANCODE_UP) || 
			s_pLocalInput->IsKeyPressed(KM_MOVE_BACKWARD) || s_pLocalInput->IsKeyPressed(SDL_SCANCODE_DOWN) ) // ????/???? ???? ???? ????. 
		{
			this->CommandMove(MD_STOP, true);
		}

		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_INVENTORY)) this->CommandToggleUIInventory();
		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_STATE)) this->CommandToggleUIState();
		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_SKILL)) this->CommandToggleUISkillTree();
		if (s_pLocalInput->IsKeyPress(KM_TOGGLE_CMDLIST)) this->CommandToggleCmdList();
		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_SITDOWN)) this->CommandSitDown(true, !s_pPlayer->m_bSitDown);

		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_HELP)) 
		{
			if(m_pUIHelp->IsVisible()) m_pUIHelp->SetVisible(false);
			else m_pUIHelp->SetVisible(true);
		}

		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_MINIMAP))
		{
			this->CommandToggleUIMiniMap();
		}

		if (s_pLocalInput->IsKeyPress(SDL_SCANCODE_PAGEUP))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->PageUp();
		
		if (s_pLocalInput->IsKeyPress(SDL_SCANCODE_PAGEDOWN))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->PageDown();

		if (s_pLocalInput->IsKeyPress(KM_SKILL_PAGE_1))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(0);
		if (s_pLocalInput->IsKeyPress(KM_SKILL_PAGE_2))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(1);
		if (s_pLocalInput->IsKeyPress(KM_SKILL_PAGE_3))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(2);
		if (s_pLocalInput->IsKeyPress(KM_SKILL_PAGE_4))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(3);
		if (s_pLocalInput->IsKeyPress(KM_SKILL_PAGE_5))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(4);
		if (s_pLocalInput->IsKeyPress(KM_SKILL_PAGE_6))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(5);
		if (s_pLocalInput->IsKeyPress(KM_SKILL_PAGE_7))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(6);
		if (s_pLocalInput->IsKeyPress(KM_SKILL_PAGE_8))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->SetHotKeyPage(7);

	} // end of if ( !m_UIChatDlg.IsChatMode() )

	// ..... ?????? ?????? ????..
#if _DEBUG
	if(s_pLocalInput->IsKeyPress(SDL_SCANCODE_F12)) // ?????? ??????..
		s_pEng->Lightning(); // ???? ????..
#endif
}

void CGameProcMain::ProcessPlayerInclination()											// ?????? ?? ??????..
{
/*	float fFrm = GetIndepTimeFrame();

	__Vector3 vNorm, vNormXZ; 
	s_pTerrain->GetNormal( s_pEng->m_matPlayer.Pos().x, s_pEng->m_matPlayer.Pos().z, vNorm );

	vNorm.Normalize();
	vNormXZ = vNorm;
	vNormXZ.y = 0.0f;

	if ( vNormXZ.Magnitude() > 0.8f )
	{
		vNormXZ.x *= 0.01f*fFrm;
		vNormXZ.z *= 0.01f*fFrm;
		s_pPlayer->m_vPlayerIncline = vNormXZ;
	}*/
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

void CGameProcMain::MsgSend_Continous()						// ???? ????(?)?????? ???????? ?????????? ???????? ????..
{
	float fTime = s_pEng->TimeGet();

	if ( fTime >= m_fMsgSendTimeMove + PACKET_INTERVAL_MOVE )					// 1???? ????????..	
	{
		__Vector3 vPos = s_pPlayer->Position();
		if(m_vPlayerPosSended != vPos) this->MsgSend_Move(true, true); // ?????????? ?????????? ?????? ???? ????..
	}

	if( false == s_pPlayer->m_bMoveContinous && fTime >= m_fMsgSendTimeRot + PACKET_INTERVAL_ROTATE ) // ?????????? ?????? ????.. 2???? ????????..
	{
		float fYaw = s_pPlayer->Yaw();
		if( fYaw != m_fPlayerYawSended ) MsgSend_Rotation(); // ?????????? ?????????? ???? ???? ????
	}
}



//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

void CGameProcMain::MsgSend_Attack(int iTargetID, float fInterval, float fDistance) // ???? ???? ?????? - ???????? ???? ?????? ???? ???? ?????? ??????.
{
	if(s_pPlayer->m_fTimeAfterDeath > 0 || s_pPlayer->IsDead())	return; // ???? ??????..

	uint8_t byBuff[32];												// ????.. 
	int iOffset=0;													// ????..

	uint8_t bySuccess = true;

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_ATTACK);						// ???? ??????..
	CAPISocket::MP_AddByte(byBuff, iOffset, 0x01);							// ??? ????????
	CAPISocket::MP_AddByte(byBuff, iOffset, bySuccess);						// ???? ????.. - ???? ???????? ??????.

	fInterval += 0.1f;

	CAPISocket::MP_AddShort(byBuff, iOffset, iTargetID );					// ?????? ??????..
	CAPISocket::MP_AddShort(byBuff, iOffset, (int)(fInterval * 100));	// ?????? ????
	CAPISocket::MP_AddShort(byBuff, iOffset, (int)(fDistance * 10));	// ?????? ????

	s_pSocket->Send(byBuff, iOffset);										// ????..
}

void CGameProcMain::MsgSend_Move(bool bMove, bool bContinous)
{
	__Vector3 vPos(0,0,0);
	float fSpeed = s_pPlayer->MoveSpeed();
	
	uint8_t byMoveFlag = 0;

	if(true == bMove) // ???????? ?????????? ??????????
	{
		if(s_pPlayer->m_fTimeAfterDeath > 0 || s_pPlayer->IsDead())	return; // ???? ??????..

		vPos = s_pPlayer->NextPos(PACKET_INTERVAL_MOVE); // ???? 1?????? ?????? ????????.
		byMoveFlag |= 0x01;
	}
	else // ?????????? 
	{
		s_pPlayer->m_bTargetOrPosMove	= false;
		s_pPlayer->m_iMoveTarget		= -1;
		vPos = s_pPlayer->Position();
		fSpeed = 0;
	}

	if(true == bContinous) // ?????????? ???????? ??????..
	{
		byMoveFlag |= 0x02;
	}

	uint8_t byBuff[64];											// ???? ????..
	int iOffset=0;											// ????..

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_MOVE);			// ??????..
	CAPISocket::MP_AddWord(byBuff, iOffset, (uint16_t)(vPos.x*10));			// ???? ????
	CAPISocket::MP_AddWord(byBuff, iOffset, (uint16_t)(vPos.z*10));
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)(vPos.y*10));
	CAPISocket::MP_AddWord(byBuff, iOffset, (uint16_t)(fSpeed*10));			// ???? 
	CAPISocket::MP_AddByte(byBuff, iOffset, byMoveFlag );		// ?????? ??????..
	s_pSocket->Send(byBuff, iOffset);							// ?????? ????..

	m_vPlayerPosSended = s_pPlayer->Position(); // ?????? ???? ???? ????..
	
	if(true == bMove) m_fMsgSendTimeMove = CN3Base::TimeGet(); // ?????? ????????..
}

void CGameProcMain::MsgSend_Rotation()
{
	if(s_pPlayer->IsDead()) return; // ???? ???? ???? ????!!

	uint8_t byBuff[8];
	int iOffset=0;

	float fYaw = s_pPlayer->Yaw(); // ????..
	
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_ROTATE);
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)(fYaw*100));

	s_pSocket->Send(byBuff, iOffset);

	m_fPlayerYawSended = fYaw; // ???????? ????
	m_fMsgSendTimeRot = CN3Base::TimeGet(); // ?????? ????????..
}


void CGameProcMain::MsgSend_Chat(e_ChatMode eMode, const std::string& szChat)
{
	if(szChat.empty() || szChat.size() >= 128) return;
	if(eMode==N3_CHAT_CLAN && s_pPlayer->m_InfoExt.iKnightsID <=0) return;

	uint8_t byBuff[512];
	int iOffset=0;
	
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_CHAT);
	CAPISocket::MP_AddByte(byBuff, iOffset, eMode);
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)szChat.size());
	CAPISocket::MP_AddString(byBuff, iOffset, szChat);

	__ASSERT(iOffset<512, "Send Buffer OverFlow");
	s_pSocket->Send(byBuff, iOffset); // ??????..
}

void CGameProcMain::MsgSend_ChatSelectTarget(const std::string& szTargetID)
{
	if(szTargetID.empty() || szTargetID.size() > 20) return;

	int iOffset = 0;
	uint8_t byBuff[32];

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_CHAT_TARGET);

	// TEMP(srmeier): testing private messages
	CAPISocket::MP_AddByte(byBuff, iOffset, 0x01);

	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)szTargetID.size() );
	CAPISocket::MP_AddString(byBuff, iOffset, szTargetID );

	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_Regen()
{
	if(s_pPlayer->m_iSendRegeneration >= 2) return; // ???? ?????? ???? ?????????? ???????? ??????

	uint8_t byBuff[4];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_REGENE);
	CAPISocket::MP_AddByte(byBuff, iOffset, 1); //1: ?????? ????????..
	
	CLogWriter::Write("Send Regeneration");

	s_pSocket->Send(byBuff, iOffset); // ??????..

	s_pPlayer->m_iSendRegeneration = 2; // ???? ?????? ???? ?????????? ???????? ??????
	//TRACE("???? - ???? ????????\n");
}

bool CGameProcMain::MsgSend_RequestItemBundleOpen(CPlayerNPC* pCorpse)
{
	if( NULL == pCorpse || pCorpse->m_iDroppedItemID <= 0) return false;

	float fDistTmp = (pCorpse->Position() - s_pPlayer->Position()).Magnitude();
	if(	fDistTmp >= (pCorpse->Radius() * 2.0f + 6.0f)) return false;

	int iItemBundleID = pCorpse->m_iDroppedItemID;
	m_pUIDroppedItemDlg->m_iItemBundleID = pCorpse->m_iDroppedItemID;
		
	s_pOPMgr->CorpseRemove(pCorpse, false); // ???? ???????? ??????..
	
	uint8_t byBuff[8];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_BUNDLE_OPEN_REQ);
	CAPISocket::MP_AddDword(byBuff, iOffset, iItemBundleID);

	s_pSocket->Send(byBuff, iOffset); // ??????..

	return true;
}

void CGameProcMain::MsgSend_PartyOrForcePermit(int iPartyOrForce, bool bYesNo)
{
	uint8_t byBuff[4];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_PARTY);
//	CAPISocket::MP_AddByte(byBuff, iOffset, iPartyOrForce);
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_OR_FORCE_PERMIT);
	CAPISocket::MP_AddByte(byBuff, iOffset, bYesNo);

	s_pSocket->Send(byBuff, iOffset); // ??????..
}

bool CGameProcMain::MsgSend_PartyOrForceCreate(int iPartyOrForce, const std::string& szID)
{
	if(szID.empty() || szID.size() > 20) return false;

	bool bIAmLeader, bIAmMember;
	int iMemberIndex = -1;
	CPlayerBase* pTarget = NULL;
	this->PartyOrForceConditionGet(bIAmLeader, bIAmMember, iMemberIndex, pTarget);
	if(true == bIAmMember && false == bIAmLeader) return false; // ???? ?????? ???? ???? ?????? ?????? ????..

	uint8_t byBuff[32];
	int iOffset=0;

	e_SubPacket_Party eCmdParty = N3_SP_PARTY_OR_FORCE_CREATE;
	if(m_pUIPartyOrForce->MemberCount() >= 2)
	{
		eCmdParty = N3_SP_PARTY_OR_FORCE_INSERT;
	}

	m_pUIPartyOrForce->m_iPartyOrForce = iPartyOrForce;

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_PARTY);
//	CAPISocket::MP_AddByte(byBuff, iOffset, iPartyOrForce);
	CAPISocket::MP_AddByte(byBuff, iOffset, eCmdParty);
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)szID.size());
	CAPISocket::MP_AddString(byBuff, iOffset, szID);

	s_pSocket->Send(byBuff, iOffset); // ??????..
	
	if(m_pUIPartyOrForce->MemberCount() <= 0) // ???? ???????? ????...
	{
		m_pUIPartyOrForce->MemberAdd(
			s_pPlayer->IDNumber(),
			s_pPlayer->IDString(),
			s_pPlayer->m_InfoBase.iLevel, 
			s_pPlayer->m_InfoBase.eClass, 
			s_pPlayer->m_InfoBase.iHP, 
			s_pPlayer->m_InfoBase.iHPMax);  // ???? ???? ???? ??????..
	}

	//TRACE ("Party or Force ???? ???? - Target ID(%s)\n", szID.c_str());

	return true;
}

void CGameProcMain::MsgSend_PartyOrForceLeave(int iPartyOrForce)
{
	if(m_pUIPartyOrForce->MemberCount() <= 0) return; // ???????? ????..

	CPlayerBase* pTarget = NULL;
	bool bIAmLeader, bIAmMember;
	int iMemberIndex = -1;
	this->PartyOrForceConditionGet(bIAmLeader, bIAmMember, iMemberIndex, pTarget);

	uint8_t byBuff[8];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_PARTY);
//	CAPISocket::MP_AddByte(byBuff, iOffset, iPartyOrForce);
	if(bIAmLeader) // ???? ??????????..
	{
		if(iMemberIndex > 0 && pTarget) // ??????????.. ????????..
		{
			CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_OR_FORCE_REMOVE);
			CAPISocket::MP_AddShort(byBuff, iOffset, pTarget->IDNumber());
		}
		else // ?????? ???????? ?????? ???? ??????..
		{
			CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_OR_FORCE_DESTROY);
		}
	}
	else if(bIAmMember) // ?????? ?????? ???? ???????? ??????..
	{
		CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_OR_FORCE_REMOVE);
		CAPISocket::MP_AddShort(byBuff, iOffset, s_pPlayer->IDNumber());
	}
	s_pSocket->Send(byBuff, iOffset); // ??????..
}

void CGameProcMain::MsgSend_ObjectEvent(int iEventID, int iNPCID)
{
	uint8_t byBuff[8];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_OBJECT_EVENT);
	CAPISocket::MP_AddShort(byBuff, iOffset, iEventID);	// Index
	CAPISocket::MP_AddShort(byBuff, iOffset, iNPCID);	// Parameter

	s_pSocket->Send(byBuff, iOffset); // ??????..
}

void CGameProcMain::MsgSend_Weather(int iWeather, int iPercent)
{
	if(iWeather < 1 || iWeather > 3) return;
	if(iPercent < 0 || iPercent > 100) return;

	uint8_t byBuff[8];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_WEATHER); // -> byte - ????.... 0x01 - ????.. 0x02 -  ?? 0x03
	CAPISocket::MP_AddByte(byBuff, iOffset, iWeather); // -> byte - ????.... 0x01 - ????.. 0x02 -  ?? 0x03
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)iPercent); // int16_t -> ?????? ????, ??, ?? ?? ?? ???????? 

	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_Time(int iHour, int iMin)
{
	uint8_t byBuff[12];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_TIME); 
	CAPISocket::MP_AddShort(byBuff, iOffset, 0);		// year
	CAPISocket::MP_AddShort(byBuff, iOffset, 0);		// month
	CAPISocket::MP_AddShort(byBuff, iOffset, 0);		// day
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)iHour);
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)iMin);

	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_Administrator(e_SubPacket_Administrator eSP, const std::string& szID)
{
	if(szID.empty() || szID.size() >= 20) return;

	uint8_t byBuff[64];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_OPERATOR); // ?????? ????????..
	CAPISocket::MP_AddByte(byBuff, iOffset, eSP);
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)szID.size());
	CAPISocket::MP_AddString(byBuff, iOffset, szID);	

	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsJoinReq(bool bJoin)
{
	uint8_t byBuff[8];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_KNIGHTS_PROCESS); // ?????? ????????..
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_JOIN_REQ);
	CAPISocket::MP_AddByte(byBuff, iOffset, (uint8_t)bJoin);
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)m_iJoinReqClanRequierID);
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)m_iJoinReqClan);
	
	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsJoin(int iTargetID)
{
	uint8_t byBuff[4];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_KNIGHTS_PROCESS); // ?????? ????????..
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_JOIN);
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)iTargetID);
	
	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsLeave(std::string& szName)
{
	uint8_t byBuff[64];
	int iOffset=0;

	int iLen = szName.size();

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_KNIGHTS_PROCESS); // ?????? ????????..
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_MEMBER_REMOVE);
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)iLen);
	CAPISocket::MP_AddString(byBuff, iOffset, szName);	// ?????? ?????? ?????? ????..
	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsWithdraw()
{
	uint8_t byBuff[2];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_KNIGHTS_PROCESS); 
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_WITHDRAW);
	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsAppointViceChief(std::string& szName)
{
	uint8_t byBuff[64];
	int iOffset=0;

	int iLen = szName.size();

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_KNIGHTS_PROCESS); // ?????? ????????..
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_APPOINT_VICECHIEF);
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)iLen);
	CAPISocket::MP_AddString(byBuff, iOffset, szName);	// ?????? ?????? ?????? ????..
	s_pSocket->Send(byBuff, iOffset);
}

bool CGameProcMain::MsgRecv_MyInfo_All(Packet& pkt)
{
	int iZone = s_pPlayer->m_InfoExt.iZoneCur;
	s_pPlayer->Release(); // ???? ???? ?? ???? ????....
	s_pPlayer->m_InfoExt.iZoneCur = iZone;

	int iID = pkt.read<int16_t>();
	int iLen = pkt.read<uint8_t>();

	std::string szID;
	pkt.readString(szID, iLen);
	s_pPlayer->IDSet(iID, szID, D3DCOLOR_XRGB(100, 210, 255)); // ???? ???????? ?????? ????..

	float fX = (pkt.read<uint16_t>())/10.0f;
	float fZ = (pkt.read<uint16_t>())/10.0f;
	float fY = (pkt.read<int16_t>())/10.0f;
	
	s_pPlayer->m_InfoBase.eNation = (e_Nation)pkt.read<uint8_t>();
	s_pPlayer->m_InfoBase.eRace = (e_Race)pkt.read<uint8_t>();
	s_pPlayer->m_InfoBase.eClass = (e_Class)pkt.read<int16_t>();
	s_pPlayer->m_InfoExt.iFace = pkt.read<uint8_t>(); // ???? ????..
	s_pPlayer->m_InfoExt.iHair = pkt.read<uint8_t>(); // ????????

	__TABLE_PLAYER_LOOKS* pLooks = s_pTbl_UPC_Looks.Find(s_pPlayer->m_InfoBase.eRace);	// User Player Character Skin ?????? ??????..
	if(NULL == pLooks) CLogWriter::Write("CGameProcMain::MsgRecv_MyInfo_All : failed find character resource data (Race : %d)", s_pPlayer->m_InfoBase.eRace);
	__ASSERT(pLooks, "failed find character resource data");
	s_pPlayer->InitChr(pLooks); // ???? ????..

	s_pPlayer->m_InfoExt.iRank = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iTitle = pkt.read<uint8_t>();
	s_pPlayer->m_InfoBase.iLevel = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iLevelPrev = s_pPlayer->m_InfoBase.iLevel;
	s_pPlayer->m_InfoExt.iBonusPointRemain = pkt.read<uint8_t>(); // ???? ?????? ??????..

	s_pPlayer->m_InfoExt.iExpNext = pkt.read<uint32_t>(); 
	s_pPlayer->m_InfoExt.iExp = pkt.read<uint32_t>(); 
	s_pPlayer->m_InfoExt.iRealmPoint = pkt.read<uint32_t>();

	// @Demircivi, implemented monthly np system.
	s_pPlayer->m_InfoExt.iRealmPointMonthly = pkt.read<uint32_t>();

	s_pPlayer->m_InfoExt.iCity = pkt.read<uint8_t>();
	
	std::string szKnightsName = "";
	int iKnightsID = pkt.read<int16_t>(); // ???? ?????? ID
	e_KnightsDuty eKnightsDuty = (e_KnightsDuty)pkt.read<uint8_t>(); // ???????????? ????..
	
	// NOTE(srmeier): adding alliance ID and knight's byFlag
	int iAllianceID = pkt.read<int16_t>();
	uint8_t byFlag = pkt.read<uint8_t>();
	
	int iKnightNameLen = pkt.read<uint8_t>(); // ???? ?????? ???? ????.
	pkt.readString(szKnightsName, iKnightNameLen);
	int iKnightsGrade = pkt.read<uint8_t>(); // ???? ?????? ????
	int	iKnightsRank = pkt.read<uint8_t>(); // ???? ?????? ????
	
	// NOTE(srmeier): adding mark version and cape ID
	int16_t sMarkVersion = pkt.read<int16_t>();
	int16_t sCapeID = pkt.read<int16_t>();

	// ?????? ???? ????..
	s_pPlayer->m_InfoExt.eKnightsDuty = eKnightsDuty; // ???????????? ????..
	s_pPlayer->KnightsInfoSet(iKnightsID, szKnightsName, iKnightsGrade, iKnightsRank);
	m_pUIVar->UpdateKnightsInfo();
	
	s_pPlayer->m_InfoBase.iHPMax = pkt.read<int16_t>();	
	s_pPlayer->m_InfoBase.iHP = pkt.read<int16_t>(); 
	s_pPlayer->m_InfoExt.iMSPMax = pkt.read<int16_t>();
	s_pPlayer->m_InfoExt.iMSP = pkt.read<int16_t>();
	s_pPlayer->m_InfoExt.iWeightMax = pkt.read<int16_t>(); 
	s_pPlayer->m_InfoExt.iWeight = pkt.read<int16_t>(); 

	s_pPlayer->m_InfoExt.iStrength = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iStrength_Delta = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iStamina = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iStamina_Delta = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iDexterity = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iDexterity_Delta = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iIntelligence = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iIntelligence_Delta = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iMagicAttak = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iMagicAttak_Delta = pkt.read<uint8_t>();
	
	s_pPlayer->m_InfoExt.iAttack = pkt.read<int16_t>();
//	s_pPlayer->m_InfoExt.iAttack_Delta		= pkt.read<int16_t>();
	s_pPlayer->m_InfoExt.iGuard = pkt.read<int16_t>();
//	s_pPlayer->m_InfoExt.iGuard_Delta		= pkt.read<int16_t>();
	s_pPlayer->m_InfoExt.iRegistFire = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iRegistCold = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iRegistLight = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iRegistMagic = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iRegistCurse = pkt.read<uint8_t>();
	s_pPlayer->m_InfoExt.iRegistPoison = pkt.read<uint8_t>();

	s_pPlayer->m_InfoExt.iGold = pkt.read<uint32_t>();
	s_pPlayer->m_InfoBase.iAuthority = pkt.read<uint8_t>(); //????.. 

	// NOTE(srmeier): adding national rank and leader rank
	uint8_t bKnightsRank = pkt.read<uint8_t>();
	uint8_t bPersonalRank = pkt.read<uint8_t>();

	// ???? UI ????..
	for ( int i = 0; i < 9; i++ )
	{
		m_pUISkillTreeDlg->m_iSkillInfo[i] = pkt.read<uint8_t>();
	}
	m_pUISkillTreeDlg->InitIconUpdate();
	m_pUIHotKeyDlg->ReleaseItem();
	m_pUIHotKeyDlg->InitIconUpdate();			// ?????? ???????? ???????? ???????? ?????????????? ????????..

	// ???????? ???? ??..
	int iItemIDInSlots[ITEM_SLOT_COUNT]; memset(iItemIDInSlots, -1, sizeof(iItemIDInSlots));
	int iItemDurabilityInSlots[ITEM_SLOT_COUNT]; memset(iItemDurabilityInSlots, -1, sizeof(iItemDurabilityInSlots));
	int iItemCountInSlots[ITEM_SLOT_COUNT]; memset(iItemCountInSlots, -1, sizeof(iItemCountInSlots));

	for ( int i = 0; i < ITEM_SLOT_COUNT; i++ )				// ???? ????????..
	{
		iItemIDInSlots[i] = pkt.read<uint32_t>();
		iItemDurabilityInSlots[i] = pkt.read<int16_t>();
		iItemCountInSlots[i] = pkt.read<int16_t>();

		// NOTE(srmeier): adding rental flag and remaining time
		uint8_t bRentFlag = pkt.read<uint8_t>();
		int16_t sRemainingRentalTime = pkt.read<int16_t>();
	}

	m_fMsgSendTimeMove		= 0;						// Network ReQuest ?????? ??????..
	m_fMsgSendTimeRot		= 0;
	m_fPlayerYawSended		= 0;						// ?????? ???????? ???? ?????? ???????? y ?? ??????.
	m_vPlayerPosSended		= s_pPlayer->Position();	// ?????? ???????? ???? ?????? ???????? ????.

	// ?????? ?????? ???? ????
	if(m_pUIVar->m_pPageState) m_pUIVar->m_pPageState->UpdateID(szID); // ???? ????.
	m_pUIVar->UpdateAllStates(&(s_pPlayer->m_InfoBase), &(s_pPlayer->m_InfoExt));

	//__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(s_pPlayer->m_InfoExt.iKnightsID);
	//if(pKIB) m_pUIVar->m_pPageKnights->UpdateKnightsName(pKIB->szName);
	
	// ???? ?? ????
	m_pUIStateBarAndMiniMap->UpdateExp(s_pPlayer->m_InfoExt.iExp, s_pPlayer->m_InfoExt.iExpNext, true);
	m_pUIStateBarAndMiniMap->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax, true);
	m_pUIStateBarAndMiniMap->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax, true);

	m_pUIPartyOrForce->MemberInfoReInit(); // ???? ??.. ????..
	
	__TABLE_ITEM_BASIC* pItem = NULL;								// ?????? ?????? ?????? ??????..	
	__TABLE_ITEM_EXT* pItemExt = NULL;								// ?????? ?????? ?????? ??????..	

	int iItemIDInInventorys[MAX_ITEM_INVENTORY]; memset(iItemIDInInventorys, -1, sizeof(iItemIDInInventorys));
	int iItemCountInInventorys[MAX_ITEM_INVENTORY]; memset(iItemCountInInventorys, -1, sizeof(iItemCountInInventorys));
	int iItemDurabilityInInventorys[MAX_ITEM_INVENTORY]; memset(iItemDurabilityInInventorys, -1, sizeof(iItemDurabilityInInventorys));

	for ( int i = 0; i < MAX_ITEM_INVENTORY; i++ )				// ???? ????????..
	{
		iItemIDInInventorys[i] = pkt.read<uint32_t>();
		iItemDurabilityInInventorys[i] = pkt.read<int16_t>();
		iItemCountInInventorys[i] = pkt.read<int16_t>();

		// NOTE(srmeier): adding rental flag and remaining time
		uint8_t bRentFlag = pkt.read<uint8_t>();
		int16_t sRemainingRentalTime = pkt.read<int16_t>();
	}


	// NOTE(srmeier): adding is chicken and manner points
	uint8_t bIDK; int16_t sIDK;
	bIDK = pkt.read<uint8_t>();
	bIDK = pkt.read<uint8_t>();
	sIDK = pkt.read<int16_t>();
	uint8_t bIsChicken = pkt.read<uint8_t>();
	int iMannerPoints = pkt.read<uint32_t>();


	m_pUIInventory->ReleaseItem();

	std::string szResrcFN, szIconFN;
	for ( int i = 0; i < ITEM_SLOT_COUNT; i++ )				// ???? ????????..
	{
		if(0 == iItemIDInSlots[i]) continue;

		pItem = s_pTbl_Items_Basic.Find(iItemIDInSlots[i]/1000*1000);	// ?? ?????? ????..
		if(pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION)
			pItemExt = s_pTbl_Items_Exts[pItem->byExtIndex].Find(iItemIDInSlots[i]%1000);	// ?? ?????? ????..
		else
			pItemExt = NULL;

		if ( NULL == pItem || NULL == pItemExt )
		{
			__ASSERT(0, "NULL Item!!!");
			CLogWriter::Write("MyInfo - slot - Unknown Item %d, IDNumber", iItemIDInSlots[i]);
			continue; // ???????? ??????..
		}

		e_PartPosition ePart;
		e_PlugPosition ePlug;
		e_ItemType eType = CGameProcedure::MakeResrcFileNameForUPC(pItem, &szResrcFN, &szIconFN, ePart, ePlug, s_pPlayer->m_InfoBase.eRace); // ???????? ???? ???? ?????? ????????
		if(ITEM_TYPE_UNKNOWN == eType) CLogWriter::Write("MyInfo - slot - Unknown Item");
		__ASSERT(ITEM_TYPE_UNKNOWN != eType, "Unknown Item Type");
		e_ItemSlot eSlot = (e_ItemSlot)i;

		switch(eSlot)
		{
		case ITEM_SLOT_EAR_RIGHT:
		case ITEM_SLOT_EAR_LEFT:
		case ITEM_SLOT_NECK:
		case ITEM_SLOT_RING_RIGHT:
		case ITEM_SLOT_RING_LEFT:
			{
				if(ITEM_TYPE_ICONONLY != eType) CLogWriter::Write("MyInfo - slot - Invalid Item");
				__ASSERT(ITEM_TYPE_ICONONLY == eType, "Invalid Item");
			}
			break;
		case ITEM_SLOT_UPPER: // Parts - ????
		case ITEM_SLOT_LOWER: // Parts - ????
		case ITEM_SLOT_GLOVES: // ????
		case ITEM_SLOT_SHOES: // ????
		case ITEM_SLOT_HEAD: // ???????? ?? -> ???? ??
			{
				if(ITEM_TYPE_PART != eType)  CLogWriter::Write("MyInfo - slot - Invalid Item");
				__ASSERT(ITEM_TYPE_PART == eType, "Invalid Item");
				s_pPlayer->PartSet(ePart, szResrcFN, pItem, pItemExt);	// ?????? ????..
			}
			break;
		case ITEM_SLOT_HAND_RIGHT: // ??????
		case ITEM_SLOT_HAND_LEFT: // ????
			{
				if(ITEM_TYPE_PLUG != eType) CLogWriter::Write("MyInfo - slot - Invalid Item");
				__ASSERT(ITEM_TYPE_PLUG == eType, "Invalid Item");
				
				e_PlugPosition ePlugPos;
				if(ITEM_SLOT_HAND_RIGHT == eSlot) ePlugPos = PLUG_POS_RIGHTHAND;
				else ePlugPos = PLUG_POS_LEFTHAND;
				s_pPlayer->PlugSet(ePlugPos, szResrcFN, pItem, pItemExt);	// ?????? ????..
			}
			break;
		case ITEM_SLOT_SHOULDER: // ????
			{
			}
			break;
		case ITEM_SLOT_BELT:
			{
			}
			break;
		}

		s_pPlayer->DurabilitySet(eSlot, iItemDurabilityInSlots[eSlot]);	// ???? ???? ????..

		__IconItemSkill* spItem = new __IconItemSkill;
		spItem->pItemBasic	= pItem;
		spItem->pItemExt	= pItemExt;
		spItem->szIconFN	= szIconFN; // ?????? ???? ???? ????..
		spItem->iCount		= iItemCountInSlots[i];
		spItem->iDurability = iItemDurabilityInSlots[i];

		// ???????? ?????? ??????.
		m_pUIInventory->m_pMySlot[i] = spItem;
		//TRACE("Init Inv Msg Slot %d \n", iItemIDInSlots[i]);
	}

	// ????????..
	int iItemCount = 0;
	for ( int i = 0; i < MAX_ITEM_INVENTORY; i++ )				// ???????? ????????..	
	{
		if(!iItemIDInInventorys[i]) continue;

		pItem = s_pTbl_Items_Basic.Find(iItemIDInInventorys[i]/1000*1000);	// ?? ?????? ????..
		pItemExt = NULL;
		if(pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION)
			pItemExt = s_pTbl_Items_Exts[pItem->byExtIndex].Find(iItemIDInInventorys[i]%1000);	// ?? ?????? ????..
		if ( NULL == pItem || NULL == pItemExt )
		{
			__ASSERT(0, "NULL Item");
			CLogWriter::Write("MyInfo - Inv - Unknown Item %d, IDNumber", iItemIDInInventorys[i]);
			continue; // ???????? ??????..
		}

		e_PartPosition ePart;
		e_PlugPosition ePlug;
		e_ItemType eType = CGameProcedure::MakeResrcFileNameForUPC(pItem, NULL, &szIconFN, ePart, ePlug, s_pPlayer->m_InfoBase.eRace); // ???????? ???? ???? ?????? ????????
		if(ITEM_TYPE_UNKNOWN == eType) CLogWriter::Write("MyInfo - slot - Unknown Item");
		__ASSERT(ITEM_TYPE_UNKNOWN != eType, "Unknown Item");
		
		__IconItemSkill* spItem = new __IconItemSkill;
		spItem->pItemBasic	= pItem;
		spItem->pItemExt	= pItemExt;
		spItem->szIconFN = szIconFN; // ?????? ???? ???? ????..
		spItem->iCount	= iItemCountInInventorys[i];
		spItem->iDurability = iItemDurabilityInInventorys[i];

		m_pUIInventory->m_pMyInvWnd[i] = spItem; 
		//TRACE("Init Inv Msg Inve %d, iOrder %d \n", iItemIDInInventorys[i], i);
	}

	// Inventory & Slot Icon Operation.. ^^
	m_pUIInventory->InitIconUpdate();

	if (m_pUISkillTreeDlg) m_pUISkillTreeDlg->UpdateDisableCheck();
	if (m_pUIHotKeyDlg) m_pUIHotKeyDlg->UpdateDisableCheck();

	if(s_pPlayer->Part(PART_POS_UPPER)->FileName().empty()) // ???????? ?????????? ?????? ???? ?????? ????..
		s_pPlayer->PartSet(PART_POS_UPPER, pLooks->szPartFNs[PART_POS_UPPER], NULL, NULL);	// ????..
	if(s_pPlayer->Part(PART_POS_LOWER)->FileName().empty()) // ???????? ?????????? ?????? ???? ?????? ????..
		s_pPlayer->PartSet(PART_POS_LOWER, pLooks->szPartFNs[PART_POS_LOWER], NULL, NULL);	// ????..
	if(s_pPlayer->Part(PART_POS_HANDS)->FileName().empty()) // ???????? ?????????? ?????? ???? ?????? ????..
		s_pPlayer->PartSet(PART_POS_HANDS,  pLooks->szPartFNs[PART_POS_HANDS], NULL, NULL);	// ??..
	if(s_pPlayer->Part(PART_POS_FEET)->FileName().empty()) // ???????? ?????????? ?????? ???? ?????? ????..
		s_pPlayer->PartSet(PART_POS_FEET,  pLooks->szPartFNs[PART_POS_FEET], NULL, NULL);	// ????..
	if(s_pPlayer->Part(PART_POS_FACE)->FileName().empty()) // ???????? ?????????? ?????? ???? ?????? ????..
		s_pPlayer->InitFace();
	if(s_pPlayer->Part(PART_POS_HAIR_HELMET)->FileName().empty()) // ???????? ?????????? ?????? ???? ?????? ????..
		s_pPlayer->InitHair();

	s_pPlayer->InventoryChrAnimationInitialize();
	s_pPlayer->SetSoundAndInitFont();

//	s_pPlayer->SettingCollisionCheckPoint();

	////////////////////////////////////////////////////////////
	// ?????? ????..
	int iRun = 1;
//	if(false == CGameProcedure::RegGetSetting("UserRun", &iRun, 4)) iRun = 1; // ???? ???? ?????? ?????????????? ????.. ???????? ???? ????????..
//	if(1 == iRun)
	this->CommandToggleWalkRun(); // ???? ??????..

	e_ViewPoint eVP = VP_THIRD_PERSON;
	if(false == CGameProcedure::RegGetSetting("CameraMode", &eVP, 4)) eVP = VP_THIRD_PERSON; // ?????? ???? ????
	s_pEng->ViewPointChange(eVP);
	// ?????? ????..
	////////////////////////////////////////////////////////////

	this->InitPlayerPosition(__Vector3(fX, fY, fZ)); // ???????? ???? ??????.. ?????? ??????, ?????????? ?????? ????.

	// berserk temp
	//s_pPlayer->PlugSet(PLUG_POS_BACK, "item/babacloak.n3cplug_cloak", NULL);	// ?????? ????..
	// end berserk temp

	// berserk
	//s_pPlayer->AttachCloak();

	//..
	s_pOPMgr->Release();							// ???? ???? ???? ?????? ??????..
	
	m_bLoadComplete = TRUE;						// ???? ??..

	return true;
}

bool CGameProcMain::MsgRecv_Chat(Packet& pkt)
{
	std::string szChat;				// ????..
	e_ChatMode eCM	=	(e_ChatMode)pkt.read<uint8_t>();	// ???? ????
	e_Nation eNation =	(e_Nation)pkt.read<uint8_t>();	// ???????? ????
	int iID =			pkt.read<int16_t>();			// ????????

	std::string szName;
	int iNameLen = pkt.read<uint8_t>();
	pkt.readString(szName, iNameLen);

	std::string szMsg;
	int iMsgLen = pkt.read<int16_t>();
	pkt.readString(szMsg, iMsgLen);

	if (szName.empty())
		szChat = szMsg;
	else
		szChat = szName + " : " + szMsg;

	int iChatLen = szChat.size();
	
	if(eCM == N3_CHAT_CONTINUE_DELETE)
	{//???? ???? ????...
		m_pUIChatDlg->DeleteContinueMsg();
		return true;
	}
	else if(eCM == N3_CHAT_TITLE_DELETE)
	{//?????? ???? ????...
		m_pUIChatDlg->SetNoticeTitle("", 0xffffffff);
		return true;
	}

	D3DCOLOR crChat = 0xffffffff;
	switch(eCM)
	{
	case N3_CHAT_NORMAL:	crChat = D3DCOLOR_ARGB(255,255,255,255);	break;
	case N3_CHAT_PRIVATE:	crChat = D3DCOLOR_ARGB(255, 128, 255, 255);/*D3DCOLOR_ARGB(255,192,192,0);*/		break;
	case N3_CHAT_PARTY:		crChat = D3DCOLOR_ARGB(255,0,192,192);		break;
	case N3_CHAT_FORCE:		crChat = D3DCOLOR_ARGB(255,0,192,192);		break;
	//case N3_CHAT_SHOUT:	crChat = D3DCOLOR_ARGB(255,255,0,0);		break;
	case N3_CHAT_SHOUT:		crChat = D3DCOLOR_ARGB(255,0xf8,0x66,0x05);	break;
	case N3_CHAT_CLAN:		crChat = D3DCOLOR_ARGB(255,0,255,0);		break;
	case N3_CHAT_PUBLIC:	crChat = D3DCOLOR_ARGB(255,255,255,0);		break;
	case N3_CHAT_WAR:		crChat = D3DCOLOR_ARGB(255,255,255,0);		break;
	case N3_CHAT_TITLE:		crChat = D3DCOLOR_ARGB(255,255,255,0);		break;
	case N3_CHAT_CONTINUE:	crChat = D3DCOLOR_ARGB(255,255,255,0);		break;
	}

	if(eCM == N3_CHAT_TITLE)
	{
		m_pUIChatDlg->SetNoticeTitle(szChat, crChat);
		return true;
	}
	else if(N3_CHAT_WAR == eCM)
	{
		if(m_pWarMessage) m_pWarMessage->SetMessage(szChat, D3DFONT_BOLD, crChat);
		return true;
	}
	else if(N3_CHAT_CONTINUE == eCM)
	{
		m_pUIChatDlg->AddContinueMsg(eCM, szChat, crChat);
		return true;
	}


	// ???? ??????...           ??,.?? a
	if( N3_CHAT_NORMAL == eCM || 
		N3_CHAT_PRIVATE == eCM || 
		N3_CHAT_SHOUT == eCM )
	{
		// TEMP(srmeier): this again depends on zoneability and will need to be looked at
		/*
		if(eNation != s_pPlayer->m_InfoBase.eNation)
		{
			CPlayerBase* pTalker = s_pOPMgr->UPCGetByID(iID, false);
			bool bIamManager = (0 == s_pPlayer->m_InfoBase.iAuthority) ? true : false;
			bool bTalkerIsManager = (pTalker && 0 == pTalker->m_InfoBase.iAuthority)  ? true : false;
				
			if(!(bIamManager || bTalkerIsManager)) // ???? ???????? ?????? ???????? ???????? ??????
			{
				int i = szChat.find(':');
				if(i >= 0)
				{
					for(; i < iChatLen; i++)
					{
						szChat[i] = '!' + rand()%10; // ?????? ???? ??????..
					}
				}
			}
		}
		*/
	}
	
	// ?????? ????..
	CPlayerBase* pBPC = NULL;
	if(iID == s_pPlayer->IDNumber()) pBPC = s_pPlayer;
	else pBPC = s_pOPMgr->CharacterGetByID(iID, false);
	if(pBPC && s_pPlayer->Distance(pBPC->Position()) < 4.0f &&
		(N3_CHAT_NORMAL == eCM || N3_CHAT_SHOUT == eCM) ) // ???? ???? ???? ????????????..
		pBPC->BalloonStringSet(szChat, crChat);

	// ???????? ????..
	m_pUIChatDlg->AddChatMsg(eCM, szChat, crChat);

	return true;
}

bool CGameProcMain::MsgRecv_UserMove(Packet& pkt)
{
	int iID			= pkt.read<int16_t>();
	float fX		= (pkt.read<uint16_t>())/10.0f; // ?????????? ?????????????? ???? ????. ?????????? ???? ?????? ??????.
	float fZ		= (pkt.read<uint16_t>())/10.0f;
	float fY		= (pkt.read<int16_t>())/10.0f;
	float fSpeed	= (pkt.read<int16_t>())/10.0f; // ?????????? ?????????????? ???????? ????. ?????????? 0 ?? ????.
	uint8_t byMoveFlag = pkt.read<uint8_t>(); // ???????? ??????.. 0 ???? 1 ????, 2 ???? ??????

	// ?????? ???? ?? ????.. ^^
	// ??????, ???????? ???? 1, ???????? ???? 2, ???? xzy ????, ???? xzy ????, 1???? x, z, y dnlcl..

	if(0xff == byMoveFlag) // ???????? ???????? ????????..
	{
		if(iID == s_pPlayer->IDNumber())	// ?? ????????..
		{
			this->InitPlayerPosition(__Vector3(fX, fY, fZ)); // ???????? ???? ??????.. ?????? ??????, ?????????? ?????? ????.
		}
		else
		{
			CPlayerNPC* pBPC = s_pOPMgr->CharacterGetByID(iID, true);	// ???? ?????????? ??????..
			if (NULL == pBPC)
			{
				this->MsgSend_UserInRequest(iID); // User ?????? ???? ???? ????????..
				return false;
			}
			else if(pBPC->IsDead())
				return false;
			
			pBPC->MoveTo(fX, fY, fZ, 0, 0);	// ???? ????..
			pBPC->PositionSet(__Vector3(fX, fY, fZ), true);
		}
		return false;
	}

	if(iID == s_pPlayer->IDNumber())	// ?? ????????..
	{
		s_pPlayer->m_vPosFromServer.Set(fX, fY, fZ);
		return false;
	}

	CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(iID, true);	// ???? ?????????? ??????..
	if (NULL == pUPC) return false;

//	if(0.0f != fSpeed) TRACE("Move - %4.1f ( %d : Mode %d ) %.2f??\n", fSpeed, iID, byMoveFlag, CN3Base::TimeGet());
//	else TRACE("Stop - %4.1f ( %d : Mode %d )  %.2f??\n", fSpeed, iID, byMoveFlag, CN3Base::TimeGet());

	//	?????? ?????? ???? ?????? ????????..
	pUPC->MoveTo(fX, fY, fZ, fSpeed, byMoveFlag);	// ???? ????..

	return true;
}

bool CGameProcMain::MsgRecv_Rotation(Packet& pkt)
{
	int iID			= pkt.read<int16_t>();
	float fYaw		= (pkt.read<int16_t>())/100.0f;

	if(s_pPlayer->IDNumber() == iID) return false;

	CPlayerNPC* pBPC = NULL;
	pBPC = s_pOPMgr->CharacterGetByID(iID, true);	// ???? ???? ??????..
	if ( NULL == pBPC ) return false;

	pBPC->RotateTo(fYaw, false);	// ?????? ???? ???? x, y, z?????? 

	return true;
}

/*
bool CGameProcMain::MsgRecv_Dead(Packet& pkt)
{
	int iID	= pkt.read<int16_t>();
//	TRACE("Dead message Receive.. !!!!!!!! %d \n", iID);

	if ( iID == s_pPlayer->IDNumber() )
	{
		s_pPlayer->Action(PSA_PRE_DYING, true, NULL, true); // ???????? ??????.
	}
	else
	{
		CPlayerOther* pReadyToDead = s_pOPMgr->PlayerGetByID(iID, true); // ???? ???? ?????????? ??????????..
		if(pReadyToDead) // ?????? ???????????? ??????.
		{
			pReadyToDead->Acttion(PSA_PRE_DYING, true, NULL, true);
		}
	}

	return true;
}
*/


bool CGameProcMain::MsgRecv_Regen(Packet& pkt)
{
	__Vector3 vPosPlayer;
	vPosPlayer.x = (pkt.read<uint16_t>())/10.0f;
	vPosPlayer.z = (pkt.read<uint16_t>())/10.0f;
	vPosPlayer.y = (pkt.read<int16_t>())/10.0f;
	
	this->InitPlayerPosition(vPosPlayer); // ???????? ???? ??????.. ?????? ??????, ?????????? ?????? ????.
	s_pPlayer->RegenerateCollisionMesh(); // ???? ?????? ???? ??????..

	s_pPlayer->m_iSendRegeneration = 0; // ???? ?????? ???? ?????????? ???????? ??????
	s_pPlayer->m_fTimeAfterDeath = 0; // ???? ?????? ???? ?????????? ???????? ??????
	//TRACE("???? - ???? ????????(%.1f, %.1f)\n", vPosPlayer.x, vPosPlayer.z);

	//
	//???? & ???? ??????..
	if(m_pUIStateBarAndMiniMap) m_pUIStateBarAndMiniMap->ClearMagic();
	if(m_pMagicSkillMng) m_pMagicSkillMng->ClearDurationalMagic();
	if(CGameProcedure::s_pFX) s_pFX->StopMine();

	CLogWriter::Write("Receive Regeneration");

	if(s_pPlayer->Nation()==NATION_KARUS) CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_KARUS, s_pPlayer->IDNumber(), -1);
	else if(s_pPlayer->Nation()==NATION_ELMORAD) CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_ELMORAD, s_pPlayer->IDNumber(), -1);

	return true;
}

bool CGameProcMain::MsgRecv_Time(Packet& pkt)
{
	int16_t year	= pkt.read<int16_t>();
	int16_t month = pkt.read<int16_t>();
	int16_t day	= pkt.read<int16_t>();
	int16_t hour	= pkt.read<int16_t>();
	int16_t min	= pkt.read<int16_t>();

	ACT_WORLD->SetGameTimeWithSky(year, month, day, hour, min);

	return true;
}

bool CGameProcMain::MsgRecv_Weather(Packet& pkt)
{
	int iWeather = pkt.read<uint8_t>(); // -> byte - ????.... 0x01 - ????.. 0x02 -  ?? 0x03
	int iPercent = pkt.read<int16_t>(); // int16_t -> ?????? ????, ??, ?? ?? ?? ???????? 

	
	////////////////////////////////////////////////////
	// ?????? ?? ?????? ????....
/*	float fTimeFromServer = pkt.read<float>(); // float -> ?????????? ???? ????...
	float fTimeLocal = CN3Base::TimeGet();
	
	static float fTimeFromServerPrev = fTimeFromServer;
	static float fTimeLocalPrev = fTimeLocal;
	static int iTSH = 0; // Trying SpeedHacking...
	if(fTimeFromServer != 0.0f)
	{
		float fTDS = fTimeFromServer - fTimeFromServerPrev;
		if(fTDS > 355.0f && fTDS < 365.0f) // 6???? ???? ?????? ???? ????..
		{
			float fTDL = fTimeLocal - fTimeLocalPrev;
			if(fTDL < (fTDS * 0.8f) || fTDL > (fTDS * 1.2f)) // ???? ?????? ???? ???????? 20?????? ???? ????..
			{
				iTSH++; // Try Speed Hacking ... ++
			}
			else 
			{
				iTSH = 0;
			}

			if(iTSH >= 3) CGameProcedure::s_bUseSpeedHack = true; // ?????? ????....
		}

		fTimeFromServerPrev = fTimeFromServer;
		fTimeLocalPrev = fTimeLocal;
	}
	// ?????? ?? ?????? ????....
	////////////////////////////////////////////////////
*/
	
	if(iPercent < 20) iPercent = 20;
	if(iPercent > 100) iPercent = 100;
	float fPercent = iPercent / 100.0f;

//enum {	GEW_CLEAR=0,		// ????
//		GEW_DRIZZLE_RAIN=1,		// ??????
//		GEW_RAINY=2,			// ???????? ??
//		GEW_HEAVY_RAIN=3,		// ????
//		GEW_SNOW1=11,			// ?????? ??
//		GEW_SNOW2=12,			// ?????? ??
//		GEW_HEAVY_SNOW=13		// ????
	//TRACE("???? - ????(%d - %d)\n", iWeather, iPercent);

	ACT_WORLD->SetWeatherWithSky((CN3SkyMng::eSKY_WEATHER)iWeather,iPercent);	// ???? ????????
	float fDelta = 1.0f;
	if(0x1 == iWeather) fDelta = 0.5f + (1.0f-fPercent)*0.5f; // ???? ????. ???????? ????...
	else if(0x02 == iWeather) fDelta = 0.25f + (1.0f - fPercent)*0.75f; // ???? .
	else if(0x03 == iWeather) fDelta = 0.25f + (1.0f - fPercent)*0.75f; // ????..

	s_pEng->FarPlaneDeltaSet(fDelta, false);

	return true;
}

bool CGameProcMain::MsgRecv_UserInOut(Packet& pkt)
{
	int iType = pkt.read<uint8_t>();//pkt.read<uint8_t>();

	if ( 0x01 == iType ) // ???? ????????.(???? ???????? ???? ???? ?????????? ????..)
		this->MsgRecv_UserIn(pkt, false);
	else if ( 0x02 == iType ) // User ??????.
		this->MsgRecv_UserOut(pkt);
	else if ( 0x03 == iType ) // ???? ???????? (?????? ??????????, game start???? ??????.)
		this->MsgRecv_UserIn(pkt, true);
	else if ( 0x04 == iType ) // ???? ???????? (warp)
		this->MsgRecv_UserIn(pkt, false);
	
	return true;
}

bool CGameProcMain::MsgRecv_UserIn(Packet& pkt, bool bWithFX)
{
	int iID			= pkt.read<int16_t>();

	std::string szName;
	int iNameLen	= pkt.read<uint8_t>();
	pkt.readString(szName, iNameLen );

	e_Nation eNation =				(e_Nation)pkt.read<uint8_t>(); // ???? ????. 0 ???? ????. 1

	// ?????? ????
	int iKnightsID =					pkt.read<int16_t>(); // ?????? ID
	e_KnightsDuty eKnightsDuty =	(e_KnightsDuty)pkt.read<uint8_t>(); // ???? ????. 0 ???? ????. 1

	// NOTE(srmeier): adding alliance ID
	int16_t sAllianceID = pkt.read<int16_t>();

	int iKnightNameLen = pkt.read<uint8_t>(); // ???? ?????? ???? ????.
	std::string szKnightsName;
	pkt.readString(szKnightsName, iKnightNameLen);
	int iKnightsGrade = pkt.read<uint8_t>();			// ????
	int iKnightsRank = pkt.read<uint8_t>();			// ????

	// NOTE(srmeier): adding mark version and cape ID
	int16_t sMarkVersion = pkt.read<int16_t>();
	int16_t sCapeID = pkt.read<int16_t>();

	int iLevel =					pkt.read<uint8_t>(); // ????...
	e_Race eRace =					(e_Race)pkt.read<uint8_t>();
	e_Class eClass =				(e_Class)pkt.read<int16_t>();
	float fXPos	=					(pkt.read<uint16_t>())/10.0f;
	float fZPos	=					(pkt.read<uint16_t>())/10.0f;
	float fYPos	=					(pkt.read<int16_t>())/10.0f;
	
	float fYTerrain = ACT_WORLD->GetHeightWithTerrain(fXPos, fZPos);	// ?????? ?????? ????..
	float fYObject = ACT_WORLD->GetHeightNearstPosWithShape(__Vector3(fXPos, fYPos, fZPos), 1.0f); // ???????????? ???? ?????? ?????? ????..
	if (fYObject > fYTerrain) fYPos = fYObject;
	else fYPos = fYTerrain;

	int iFace = pkt.read<uint8_t>(); // ????????..
	int iHair = pkt.read<uint8_t>(); // ???? ????

	int iStatus = pkt.read<uint8_t>(); // 1 - ??????.. 2 - ????????.. 3 ... ????????..

	// NOTE(srmeier): updating status
	//int iStatusSize = pkt.read<uint8_t>(); // 0 - ???? ????, 1 - ???? ????. 2 - ????????..
	int iStatusSize = pkt.read<uint32_t>();

	int iRecruitParty = pkt.read<uint8_t>(); // 1 - ????. 2 - ???? ????..
	uint8_t byAuthority = pkt.read<uint8_t>(); // ????...

	// NOTE(srmeier): adding is party leader, invisibilitytype, direction, chicken, rank, knight rank, personal rank
	bool bPartyLeader = pkt.read<bool>();
	uint8_t bInvisibilityType = pkt.read<uint8_t>();
	int16_t sDirection = pkt.read<int16_t>();
	bool bIsChicken = pkt.read<bool>();
	uint8_t bRank = pkt.read<uint8_t>();
	uint8_t m_bKnightsRank = pkt.read<uint8_t>();
	uint8_t m_bPersonalRank = pkt.read<uint8_t>();


	uint32_t	dwItemIDs[MAX_ITEM_SLOT_OPC];  // ???? ?????? - ???? ????????(NPC ????) 0 ~ 4 ????,????,????,??,?? 5 ???? 6 ?????? 7 ????
	int		iItemDurabilities[MAX_ITEM_SLOT_OPC]; // ???? ???????? ??????..
	for(int i = 0; i < MAX_ITEM_SLOT_OPC; i++)
	{
		dwItemIDs[i] = pkt.read<uint32_t>();			// ???????? ???? ?????????? ID
		iItemDurabilities[i] = pkt.read<int16_t>();	// ???????? ???? ?????????? ???? ??????

		// NOTE(srmeier): adding bFlag, probably the rental thing
		uint8_t bFlag = pkt.read<uint8_t>();
	}

	// NOTE(srmeier): adding the zone id
	uint8_t bZoneID = pkt.read<uint8_t>();


	if ( iID == s_pPlayer->IDNumber()) 
		return false; 									// ?? ???????? .. // ????????..

	CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(iID, false);
	if(pUPC) // ???? ?????? ???? ?????? ??????..
	{
		CLogWriter::Write("User In - Duplicated ID (%d, %s) Pos(%.2f,%.2f,%.2f)", iID, szName.c_str(), fXPos, fYPos, fZPos);
		//TRACE("User In - Duplicated ID (%d, %s) Pos(%.2f,%.2f,%.2f)\n", iID, szName.c_str(), fXPos, fYPos, fZPos);

		// TEMP(srmeier): need to figure out what is going on here and how to fix it
		// commenting out what's below to keep the OtherPlayer in the manager...

		// NOTE(srmeier): probably shouldn't be requesting this player's info if it's already in the manager...

		//pUPC->Action(PSA_BASIC, true, NULL, true); // ?????? ??????..
		//pUPC->m_fTimeAfterDeath = 0;
		//pUPC->PositionSet(__Vector3(fXPos, fYPos, fZPos), true);

		return false;
	}
	
	D3DCOLOR crID = (eNation != s_pPlayer->m_InfoBase.eNation) ? D3DCOLOR_XRGB(255,96,96) : D3DCOLOR_XRGB(128,128,255); // ?????? ???? ?????? ????

	pUPC = new CPlayerOther();
	pUPC->IDSet(iID, szName, crID);
	pUPC->m_InfoBase.eNation = eNation;
	pUPC->m_InfoBase.eClass = eClass;
	pUPC->m_InfoBase.iLevel = iLevel;
	pUPC->m_InfoBase.iAuthority = byAuthority;
	pUPC->Init(eRace, iFace, iHair, dwItemIDs, iItemDurabilities);
	pUPC->RotateTo(D3DXToRadian(rand()%360), true);
	pUPC->KnightsInfoSet(iKnightsID, szKnightsName, iKnightsGrade, iKnightsRank);

	//__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(iKightsID);
	//if(pKIB) pUPC->KnightsNameSet(pKIB->szName, 0xffff0000);

	pUPC->PositionSet(__Vector3(fXPos, fYPos, fZPos), true);			// ???? ???????? ???? ???? ????..
	pUPC->MoveTo(fXPos, fYPos, fZPos, 0, 0);					// ???? ????..
	pUPC->RotateTo(D3DXToRadian(rand()%360), true);
	s_pOPMgr->UPCAdd(pUPC);										// ?????? ????...

	//if(bWithFX)
	if(bWithFX && pUPC->m_InfoBase.iAuthority != AUTHORITY_MANAGER)
	{
		if(eNation==NATION_KARUS) CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_REGEN_KARUS, iID, -1);
		else if(eNation==NATION_ELMORAD) CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_REGEN_ELMORAD, iID, -1);
	}

	// ???? ????.., ?????? ????..
	if(1 == iStatus) // ??????..
	{
		pUPC->Action(PSA_BASIC, true, NULL, true);
		pUPC->ActionMove(PSM_STOP);
	}
	else if(2 == iStatus) // ???? ????..
	{
		pUPC->Action(PSA_SITDOWN, true, NULL, true);
		pUPC->ActionMove(PSM_STOP);
	}
	else if(3 == iStatus) // ???? ????..
	{
		s_pOPMgr->MoveToCorpsesForcely(pUPC, false); // ?????? ??????..
	}

	if(0x01 == iStatusSize)
	{
		pUPC->ScaleSetGradually(1.0f); // ???? ????..
		pUPC->FlickerFactorSet(1.0f); // ???????? ??????.. ??????????..
	}
	else if(0x02 == iStatusSize) pUPC->ScaleSetGradually(2.0f); // ??????..
	else if(0x03 == iStatusSize) pUPC->ScaleSetGradually(0.5f);// ????????.
	else if(0x04 == iStatusSize) pUPC->FlickerFactorSet(0.7f); // ???????? ??????.. ??????????..

	if(1 == iRecruitParty)
	{
		pUPC->InfoStringSet("", 0);
	}
	else if(2 == iRecruitParty) // ???? ????..
	{
		int iLevel = pUPC->m_InfoBase.iLevel;
		int iLMin = iLevel - 8;
		if(iLMin < 0) iLMin = 0;
		int iLMax = iLevel + 8;
		if(iLMax > 60) iLMax = 60;

		char szBuff[128];
		std::string szMsg;
		::_LoadStringFromResource(IDS_WANT_PARTY_MEMBER, szMsg);
		sprintf(szBuff, szMsg.c_str(), iLMin, iLMax);
		pUPC->InfoStringSet(szBuff, 0xff00ff00);
	}

	return true;
}

bool CGameProcMain::MsgRecv_UserOut(Packet& pkt)
{
	int iID	= pkt.read<int16_t>();

	//???? ?????? ???????? ?????? ??????.
	CPlayerNPC* pUPC = s_pOPMgr->UPCGetByID(iID, false);
	if( pUPC == NULL ) return false;
	if( pUPC->IsDead() )
	{//???????? ???? ???????? UserOut?? ???? ???? ?????? ????????.
		s_pOPMgr->CorpseAdd(iID);
		return true;
	}

	return s_pOPMgr->UPCDelete(iID);					// ?????? ????...	
}

// ???? ?????? ???? ???????? ?????????? ??????... ????.. ?????????? ?????? ???? ???????? ????..
bool CGameProcMain::MsgRecv_UserInAndRequest(Packet& pkt)
{
	int iUPCCountReceived = pkt.read<int16_t>();
//	TRACE("UPC region update : %d\n", iUPCCountReceived);
	
	if(0 == iUPCCountReceived) return false;
	if(iUPCCountReceived < 0 || iUPCCountReceived >= 1000)
	{
		char szErr[256];
		sprintf(szErr, "???? ???? ???? - ???????? ???? %d", iUPCCountReceived);
		CGameProcedure::ReportDebugStringAndSendToServer(szErr);
		__ASSERT(0, szErr);
		return false;
	}

	int iID = 0;
	m_SetUPCID.clear(); // ?? ??????...
	it_ID itID, itIDEnd = m_SetUPCID.end();
	pair_ID pairID;

	for ( int i = 0; i < iUPCCountReceived; i++ )
	{
		iID = pkt.read<int16_t>();
//		TRACE("               ID : %d\n", iID);
		if(iID == s_pPlayer->IDNumber()) continue; // ?? ??????????.. ???? ?????? ??????.

		pairID = m_SetUPCID.insert(iID);
	}

	if(m_SetUPCID.empty()) // ???? ?????? ?????? ?????? ???? ??????..
	{
		s_pOPMgr->ReleaseUPCs();
		return false;
	}

	// ???? ???? ???????? ???????? ???? UPC ID ?? ????????..
	CPlayerOther* pUPC = NULL;
	it_UPC itUPC = s_pOPMgr->m_UPCs.begin(), itUPCEnd = s_pOPMgr->m_UPCs.end();
	for(; itUPC != itUPCEnd; )
	{
		pUPC = itUPC->second;
		iID = pUPC->IDNumber();
		
		itID = m_SetUPCID.find(iID);
		if(itID != itIDEnd) // ???? ?????? ???????? ??????.. 
		{
			m_SetUPCID.erase(itID); // ?????? ?????????? ????..
			itUPC++;
		}
		else  // ???? ?????????? ?????? ??????..
		{
//			TRACE("           delete : %d\n", iID);

			if(pUPC && pUPC->IsDead()) 
			{
				itUPC++;
			}
			else
			{
				delete pUPC; pUPC = NULL;
				itUPC = s_pOPMgr->m_UPCs.erase(itUPC);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// ???? ???? ?????? ?????? ??????..
	int iNewUPCCount = m_SetUPCID.size();
	if(iNewUPCCount > 0)
	{
		int iOffset=0;														// ?????? ??????..
		std::vector<uint8_t> byBuff(iNewUPCCount * 2 + 10, 0);					// ???? ????..
		CAPISocket::MP_AddByte(&(byBuff[0]), iOffset, WIZ_REQ_USERIN);	// ??????.
		CAPISocket::MP_AddShort(&(byBuff[0]), iOffset, iNewUPCCount);		// ?????? ????..
		
		itID = m_SetUPCID.begin(); itIDEnd = m_SetUPCID.end();
		for(int i = 0; itID != itIDEnd; itID++, i++)
		{
			iID = *itID;
			CAPISocket::MP_AddShort(&(byBuff[0]), iOffset, iID);			// ?????? ?????? ?????? ????????..
		}
		s_pSocket->Send(&(byBuff[0]), iOffset); // ??????
	}
	// ???? ???? ?????? ?????? ??????..
	////////////////////////////////////////////////////////////////////////////

	return true;
}

bool CGameProcMain::MsgRecv_UserInRequested(Packet& pkt)
{
	int iPlayerCount = pkt.read<int16_t>();

	if(0 == iPlayerCount) return false;
	if(iPlayerCount < 0 || iPlayerCount >= 1000)
	{
		char szErr[256];
		sprintf(szErr, "???????? ???? ???? - ???????? ???? %d", iPlayerCount);
		CGameProcedure::ReportDebugStringAndSendToServer(szErr);
		__ASSERT(0, szErr);
		return false;
	}

#ifdef _DEBUG
	float fTime = CN3Base::TimeGet();
#endif

	//	int iOffset2 = iOffset;
	for ( int i = 0; i < iPlayerCount; i++ )
	{
		this->MsgRecv_UserIn(pkt); // ???????? ???? ???? ???? ??...
	}

#ifdef _DEBUG
	//TRACE("  User In Requested (Count:%d) : Avg(%.4f Sec)\n", iPlayerCount, (CN3Base::TimeGet() - fTime)/iPlayerCount);
#endif

	return true;
}


bool CGameProcMain::MsgRecv_NPCInOut(Packet& pkt)
{
	uint8_t byType		= pkt.read<uint8_t>();
	if ( byType == 0x01 ) // NPC ????????
	{
		return this->MsgRecv_NPCIn(pkt); // NPC In ????
	}
	else // NPC ??????.
	{
		return this->MsgRecv_NPCOut(pkt);
	}

	return true;
}

bool CGameProcMain::MsgRecv_NPCIn(Packet& pkt)
{
	int		iID			= pkt.read<int16_t>(); // Server???? ???????? ???? ID
	int		iIDResrc	= pkt.read<int16_t>(); // ?????? ID
	int		iType		= pkt.read<uint8_t>();	// NPC Type - 0x05 : ????
	int		iItemTrdeID	= pkt.read<uint32_t>();	// ?????? ?????? ???? ID ?????? ?????? ID
	int		iScale		= pkt.read<int16_t>(); // ?????? 100 ?? 1.0 
	int		iItemID0	= pkt.read<uint32_t>(); // ?????? ID
	int		iItemID1	= pkt.read<uint32_t>(); // ?????? ID
	int 	iNameLen	= pkt.read<uint8_t>();
	std::string szName;									// NPC ??????..
	if(iNameLen > 0) pkt.readString(szName, iNameLen );
	else szName = "";

#ifdef _DEBUG
	CLogWriter::Write("NPC In - ID(%d) Name(%s) Time(%.1f)", iID, szName.c_str(), CN3Base::TimeGet()); // ???? ????..
#endif

	e_Nation eNation = (e_Nation)pkt.read<uint8_t>(); // ???? ????. 0 ???? ????. 1
	int iLevel		= pkt.read<uint8_t>();

	float fXPos	= (pkt.read<uint16_t>())/10.0f;
	float fZPos	= (pkt.read<uint16_t>())/10.0f;
	float fYPos	= (pkt.read<int16_t>())/10.0f;

	float fYTerrain = ACT_WORLD->GetHeightWithTerrain(fXPos, fZPos);	// ?????? ?????? ????..
	float fYObject = ACT_WORLD->GetHeightNearstPosWithShape(__Vector3(fXPos, fYPos, fZPos), 1.0f); // ???????????? ???? ?????? ?????? ????..
	if (fYObject > fYTerrain) fYPos = fYObject;
	else fYPos = fYTerrain;

	uint32_t dwStatus =	pkt.read<uint32_t>(); // ????... ?????????? or ???????? ????. 0 ?? ????, 1 ????. 2, 4, 8, 16 ....
	uint32_t dwType =		pkt.read<uint8_t>(); // ????... 0 ???? ?????? ???? NPC, 1 ???? ???????? ???? NPC


	// NOTE(srmeier): adding the unknown bytes and the direction
	int16_t sIDK0 = pkt.read<int16_t>();
	int16_t sIDK1 = pkt.read<int16_t>();
	uint8_t byDirection = pkt.read<uint8_t>();


	CPlayerNPC* pNPC = s_pOPMgr->NPCGetByID(iID, false);
	if(pNPC) // ???? ?????? ???? ?????? ??????..
	{
		CLogWriter::Write("NPC In - Duplicated ID (%d, %s) Pos(%.2f,%.2f,%.2f)", iID, szName.c_str(), fXPos, fYPos, fZPos);
		//TRACE("NPC In - Duplicated ID (%d, %s) Pos(%.2f,%.2f,%.2f)\n", iID, szName.c_str(), fXPos, fYPos, fZPos);
		pNPC->Action(PSA_BASIC, true, NULL, true); // ?????? ??????..
		pNPC->m_fTimeAfterDeath = 0;
		pNPC->PositionSet(__Vector3(fXPos, fYPos, fZPos), true);
		return false;
	}

	D3DCOLOR crID = (eNation != s_pPlayer->m_InfoBase.eNation) ? D3DCOLOR_XRGB(255,128,128) : D3DCOLOR_XRGB(192,192,255);

	pNPC = new CPlayerNPC();
	pNPC->IDSet(iID, szName, crID);				// ??????.. ?? ID ????.
	pNPC->m_InfoBase.eNation = eNation;
	pNPC->m_InfoBase.eRace = RACE_NPC;			// NPC ???? ?? ??????.
	pNPC->m_InfoBase.iLevel = iLevel;
	pNPC->m_InfoBase.iAuthority = AUTHORITY_NPC;// ???? NPC?? ?????? ????..
	
	s_pOPMgr->NPCAdd(pNPC);						// ?????? ????...

	// ???? ?????? ???? ???????? ????..(?) ????????.. ??????????, ???? ??????...
	CN3Shape* pShape = NULL;
	__TABLE_PLAYER_LOOKS* pLooks = NULL;
	if(0 == dwType)
	{
		pLooks = s_pTbl_NPC_Looks.Find(iIDResrc);	// ???? ????..
		if(NULL == pLooks) // ?????? ???? ???? ???????? ??????... 
		{
			pLooks = s_pTbl_NPC_Looks.GetIndexedData(0);
			char szBuff[256];
			sprintf(szBuff, "Normal NPC In : [Name(%s), ResourceID(%d)]", szName.c_str(), iIDResrc);
			this->MsgOutput(szBuff, 0xffff00ff);
			CLogWriter::Write(szBuff);
		}
	}
	else
	{
		pShape = ACT_WORLD->ShapeGetByIDWithShape(iIDResrc); // Object NPC ???? ???? ????...
		if(NULL == pShape)
		{
			char szBuff[256];
			sprintf(szBuff, "Object NPC In : [Name(%s), ResourceID(%d)]", szName.c_str(), iIDResrc);
			this->MsgOutput(szBuff, 0xffff00ff);
			CLogWriter::Write(szBuff);
			pLooks = s_pTbl_NPC_Looks.GetIndexedData(0);
		}
	}
		
	if(pLooks)
	{
		pNPC->InitChr(pLooks);	// ???? ????..
		pNPC->ScaleSet(iScale / 100.0f);

		std::string szPartFN;
		e_PartPosition eParts[6] = { PART_POS_UPPER, PART_POS_LOWER, PART_POS_FACE, PART_POS_HANDS, PART_POS_FEET, PART_POS_HAIR_HELMET };
		for(int i = 0; i < 6; i++)
		{
			pNPC->PartSet(eParts[i], pLooks->szPartFNs[eParts[i]], NULL, NULL);
		}

		if(iItemID0)
		{
			__TABLE_ITEM_BASIC* pItem0 = s_pTbl_Items_Basic.Find(iItemID0/1000*1000);
			__TABLE_ITEM_EXT* pItemExt0 = NULL;
			if(pItem0 && pItem0->byExtIndex >= 0 && pItem0->byExtIndex < MAX_ITEM_EXTENSION)
				pItemExt0 = s_pTbl_Items_Exts[pItem0->byExtIndex].Find(iItemID0%1000);	// ?? ?????? ????..
			if(pItem0 && pItemExt0)
			{
				e_PartPosition ePart;
				e_PlugPosition ePlug;
				std::string szItemFN;
				CGameProcedure::MakeResrcFileNameForUPC(pItem0, &szItemFN, NULL, ePart, ePlug, s_pPlayer->m_InfoBase.eRace);
				pNPC->PlugSet(PLUG_POS_RIGHTHAND, szItemFN, pItem0, pItemExt0);
			}
			else
			{
				__ASSERT(0, "Invalid Item ID And Extension");
			}
		}

		if(iItemID1)
		{
			__TABLE_ITEM_BASIC* pItem1 = s_pTbl_Items_Basic.Find(iItemID1/1000*1000);
			__TABLE_ITEM_EXT* pItemExt1 = NULL;
			if(pItem1 && pItem1->byExtIndex >= 0 && pItem1->byExtIndex < MAX_ITEM_EXTENSION)
				pItemExt1 = s_pTbl_Items_Exts[pItem1->byExtIndex].Find(iItemID1%1000);	// ?? ?????? ????..
			if(pItem1 && pItemExt1)
			{
				e_PartPosition ePart;
				e_PlugPosition ePlug;
				std::string szItemFN;
				CGameProcedure::MakeResrcFileNameForUPC(pItem1, &szItemFN, NULL, ePart, ePlug, s_pPlayer->m_InfoBase.eRace);
				pNPC->PlugSet(PLUG_POS_LEFTHAND, szItemFN, pItem1, pItemExt1);
			}
			else
			{
				__ASSERT(0, "Invalid Item ID And Extension");
			}
		}
	}
	else if(pShape && (pShape->Type() & OBJ_SHAPE_EXTRA))
	{
		CN3ShapeExtra* pSE = (CN3ShapeExtra*)pShape;
		pNPC->m_pShapeExtraRef = pSE; // ???? ???????? ????..
		pSE->m_bVisible = true; // ?????? ?????????? ???? ???? ?????? ????..
		
		bool bShouldBeRotate = true;
		__Vector3 vAxis(0,1,0);
		float fRadian = 0, fRadian2 = 0;
		bool bVisible = false;

		if(OBJECT_TYPE_DOOR_LEFTRIGHT == pSE->m_iEventType) // ???????? ????
		{
			vAxis.Set(0,1,0);
			fRadian = D3DXToRadian(80);
			fRadian2 = 0;
		} 
		else if(OBJECT_TYPE_DOOR_TOPDOWN == pSE->m_iEventType)
		{
			vAxis.Set(0,0,1);
			fRadian = D3DXToRadian(90);
			fRadian2 = 0;
		} // ???????? ????
		else if(OBJECT_TYPE_LEVER_TOPDOWN == pSE->m_iEventType)
		{
			vAxis.Set(1,0,0);
			fRadian = D3DXToRadian(45);
			fRadian2 = D3DXToRadian(-45);
		} // ???? ????
		else if(OBJECT_TYPE_FLAG == pSE->m_iEventType)
		{
			bShouldBeRotate = false;
		}

		if(0x01 == dwStatus) // ?????? ???????? ?????? ???? ???? ????..
		{
			if(bShouldBeRotate) // ????
			{
				pSE->RotateTo(0, vAxis, fRadian, 1, true); // ???? ????.
				pSE->RotateTo(1, vAxis, -fRadian, 1, true); // ???? ????.
			}
			else // ??????
			{
				pSE->m_bVisible = true;
			}
		}
		else if(0x00 == dwStatus) // ???????? ???????? ????????..
		{
			if(bShouldBeRotate) // ????
			{
				pSE->RotateTo(0, vAxis, fRadian2, 1, true); // ???? ????.
				pSE->RotateTo(1, vAxis, -fRadian2, 1, true); // ???? ????.
			}
			else // ????????.
			{
				pSE->m_bVisible = false;
			}
		}
	}
	else
	{
		char szBuff[256];
		sprintf(szBuff, "No Resource NPC In : [Name(%s), ResourceID(%d)]", szName.c_str(), iIDResrc);
		CLogWriter::Write(szBuff);
	}

	pNPC->PositionSet(__Vector3(fXPos, fYPos, fZPos), true);	// ???? ???? ????..
	pNPC->MoveTo(fXPos, fYPos, fZPos, 0, 0);					// ???? ????..
	pNPC->RotateTo(D3DXToRadian(rand()%360), false);
	pNPC->Action(PSA_BASIC, true, NULL, true);
	pNPC->ActionMove(PSM_STOP);

	return true;
}

bool CGameProcMain::MsgRecv_NPCOut(Packet& pkt)
{
	int  iID		= pkt.read<int16_t>(); // Server???? ???????? ???? ID
	return s_pOPMgr->NPCDelete(iID);										// ?????? ????...	
}


// ???? ?????? ???? ???????? ?????????? ??????... ????.. ?????????? ?????? ???? ???????? ????..
bool CGameProcMain::MsgRecv_NPCInAndRequest(Packet& pkt)
{
	int iNPCCountReceived = pkt.read<int16_t>();
	//TRACE("NPC region update : %d\n", iNPCCountReceived);

	if(0 == iNPCCountReceived) return false;
	if(iNPCCountReceived < 0 || iNPCCountReceived >= 1000)
	{
		char szErr[256];
		sprintf(szErr, "???????? ???? ???? - NPC ???? %d", iNPCCountReceived);
		CGameProcedure::ReportDebugStringAndSendToServer(szErr);
		__ASSERT(0, szErr);
		return false;
	}

	int iID = 0;
	m_SetNPCID.clear(); // ?? ??????...
	it_ID itID, itIDEnd = m_SetNPCID.end();
	pair_ID pairID;

	for ( int i = 0; i < iNPCCountReceived; i++ )
	{
		iID = pkt.read<int16_t>();
		pairID = m_SetNPCID.insert(iID);
		//TRACE("               ID : %d\n", iID);
	}

	if(m_SetNPCID.empty()) // ???? ?????? ?????? ?????? ???? ??????..
	{
		s_pOPMgr->ReleaseNPCs();
		return false;
	}

	// ???? ???? ???????? ???????? ???? NPC ID ?? ????????..
	CPlayerNPC* pNPC = NULL;
	it_NPC itNPC = s_pOPMgr->m_NPCs.begin(), itNPCEnd = s_pOPMgr->m_NPCs.end();
	for(; itNPC != itNPCEnd; )
	{
		pNPC = itNPC->second;
		iID = pNPC->IDNumber();
		
		itID = m_SetNPCID.find(iID);
		if(itID != itIDEnd) // ???? ?????? ???????? ??????.. 
		{
			m_SetNPCID.erase(itID); // ?????? ?????????? ????..
			itNPC++;
		}
		else  // ???? ?????????? ?????? ??????..
		{
			//TRACE("           delete : %d\n", iID);

			if(pNPC && pNPC->IsDead()) 
			{
				itNPC++;
			}
			else
			{
				delete pNPC; pNPC = NULL;
				itNPC = s_pOPMgr->m_NPCs.erase(itNPC);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// ???? ???? ?????? ?????? ??????..
	int iNewNPCCount = m_SetNPCID.size();
	if(iNewNPCCount > 0)
	{
		int iOffset=0;														// ?????? ??????..
		std::vector<uint8_t> byBuff(iNewNPCCount * 2 + 10, 0);					// ???? ????..
		CAPISocket::MP_AddByte(&(byBuff[0]), iOffset, WIZ_REQ_NPCIN);	// ??????.
		CAPISocket::MP_AddShort(&(byBuff[0]), iOffset, iNewNPCCount);		// ?????? ????..
		
		itID = m_SetNPCID.begin(); itIDEnd = m_SetNPCID.end();
		for(int i = 0; itID != itIDEnd; itID++, i++)
		{
			iID = *itID;
			CAPISocket::MP_AddShort(&(byBuff[0]), iOffset, iID);			// ?????? ?????? ?????? ????????..
		}
		s_pSocket->Send(&(byBuff[0]), iOffset); // ??????
	}
	// ???? ???? ?????? ?????? ??????..
	////////////////////////////////////////////////////////////////////////////

	return true;
}

bool CGameProcMain::MsgRecv_NPCInRequested(Packet& pkt)
{
	int iNPCCount = pkt.read<int16_t>();

	if(0 == iNPCCount) return false;
	if(iNPCCount < 0 || iNPCCount >= 1000)
	{
		char szErr[256];
		sprintf(szErr, "???????? ???????? - NPC ???? %d", iNPCCount);
		CGameProcedure::ReportDebugStringAndSendToServer(szErr);
		__ASSERT(0, szErr);
		return false;
	}

#ifdef _DEBUG
	float fTime = CN3Base::TimeGet();
#endif

	for ( int i = 0; i < iNPCCount; i++ )
	{
		this->MsgRecv_NPCIn(pkt); // ???????? ???? ???? ???? ??...
	}
	
#ifdef _DEBUG
	//TRACE("  NPC In Requested (Count:%d) : Avg(%.4f Sec)\n", iNPCCount, (CN3Base::TimeGet() - fTime)/iNPCCount);
#endif

	return true;
}

bool CGameProcMain::MsgRecv_NPCMove(Packet& pkt)
{
	int iID			= pkt.read<int16_t>();
	float fXPos		= (pkt.read<uint16_t>())/10.0f;
	float fZPos		= (pkt.read<uint16_t>())/10.0f;
	float fYPos		= (pkt.read<int16_t>())/10.0f;
	float fSpeed	= (pkt.read<int16_t>())/10.0f;

	// ?????? ???? ?? ????.. ^^
	CPlayerNPC* pNPC = NULL;
	pNPC = s_pOPMgr->NPCGetByID(iID, true);				// NPC?? ID???? ????..
	if ( NULL == pNPC )
	{
		this->MsgSend_NPCInRequest(iID);
		return false;		// ???????? NPC?? ??????..
	}

	float fY = ACT_WORLD->GetHeightWithTerrain(fXPos, fZPos);		// ???? ??????..
	int iMoveMode = (fSpeed > 0) ? 2 : 0;
	pNPC->MoveTo(fXPos, fY, fZPos, fSpeed, iMoveMode);	// NPC ???? ????..

	__ASSERT(!(iMoveMode && fSpeed == 0), "Invalid NPC Move Packet");

	return true;
}

bool CGameProcMain::MsgRecv_Attack(Packet& pkt)
{
	int iType		= pkt.read<uint8_t>(); // 0x01 - ???????? ????, 0x02 ???? ???? 0x03, ???? ???? ????
	int iResult		= pkt.read<uint8_t>(); // 0x00 ????, 0x01 ????
	int iIDAttacker = pkt.read<int16_t>(); // ?????? ??
	int iIDTarget	= pkt.read<int16_t>(); // ?????? ???? ??????????.

	if ( iIDAttacker == iIDTarget ) return false;		// ???????? ???????? ???? ????????

	bool bIAmTarget = (iIDTarget == s_pPlayer->IDNumber()) ? true : false;
	bool bIAmAttacker = (iIDAttacker == s_pPlayer->IDNumber()) ? true : false;
	
	CPlayerBase* pAttacker = NULL;
	if ( bIAmAttacker ) pAttacker = s_pPlayer;	// ???????? ???? ?? ????????
	else pAttacker = s_pOPMgr->CharacterGetByID(iIDAttacker, true);
	if(NULL == pAttacker) // ???? ???????? ???? ??????
	{
		if(iIDAttacker > 10000) // NPC ?? 1000 ????????.
			this->MsgSend_NPCInRequest(iIDAttacker); // NPC ?????? ???? ???? ????????..
		else if(iIDAttacker < 3000)
			this->MsgSend_UserInRequest(iIDAttacker); // NPC ?????? ???? ???? ????????..
		return false; // ???????? ???? ?????? ????????.
	}

	CPlayerBase* pTarget = NULL;
	if(bIAmTarget) // ?????? ??????..
	{
		this->CommandSitDown(false, false); // ?????? ??????.
		if(m_pMagicSkillMng->IsCasting())
		{
			__TABLE_UPC_SKILL* pSkill = s_pTbl_Skill.Find(s_pPlayer->m_dwMagicID);
			if(pSkill)
			{
				int SuccessValue = rand()%100;
				if(SuccessValue >= pSkill->iPercentSuccess) // ???? ???????? ???? ???????? ????????..
					s_pPlayer->Action(PSA_BASIC, false, NULL, true); // ?????? ????, ???????????? ???? ????..
			}
		}
		pTarget = s_pPlayer;
	}
	else // ?????? ??????????..
	{
		pTarget = s_pOPMgr->CharacterGetByID(iIDTarget, true); //  ???? ???????? ?????????? ??????????.. 
		if(NULL == pTarget) pTarget = s_pOPMgr->CharacterGetByID(iIDTarget, false); // ?????? ???????? ???? ???????? ???????? ????
	}

	if(NULL == pTarget) return false; // ?????? ????!!!!

//	pTarget->m_iIDTargetMe = iIDAttacker; // ???? ???? ???????? ??????

	if(pAttacker != s_pPlayer && pAttacker && pAttacker->IsAlive()) // ???????? ???? ???? ?????? ???? ???????? ??????..
	{
		((CPlayerNPC*)pAttacker)->RotateTo(pTarget); // ?????? ?????? ???? ????
		pAttacker->m_iIDTarget = iIDTarget; // ???? ID ????..
		if(0x01 == iType) pAttacker->Action(PSA_ATTACK, false, pTarget); // ???????? ???? ????..
		else if(0x02 == iType) pAttacker->Action(PSA_SPELLMAGIC, false, pTarget); // ???? ????..
//		else if(0x03 == iType) pAttacker->Action(PSA_SPELLMAGIC, false, pTarget); // ???? ???? ????..
	}

	pTarget->m_bGuardSuccess = false; // ?????? ???????????? ???? ??????..
	if(0x0 == iResult) // ???? ????
	{
		if(pTarget->IsAlive() && 0 == pTarget->m_iSkillStep) // ???? ???? ?????? ?????? ???? ???? ?????? ???? ?????? ????..
		{
			pTarget->m_bGuardSuccess = true;
			pTarget->Action(PSA_GUARD, false);
		}

		if(pAttacker == s_pPlayer) 
		{
			char szBuf[128] = "";
			std::string szFmt; ::_LoadStringFromResource(IDS_MSG_FMT_TARGET_ATTACK_FAILED, szFmt);
			sprintf(szBuf, szFmt.c_str(), pTarget->IDString().c_str());
			MsgOutput(szBuf, 0xffffffff);
		}
	}
	else if(0x2 == iResult) // Attack And Dead 
	{
		if(pTarget == s_pPlayer)
		{
//			if(m_pUIDead) m_pUIDead->SetVisible(true);
			std::string szMsg; ::_LoadStringFromResource(IDS_REGENERATION, szMsg);
			CGameProcedure::MessageBoxPost(szMsg, "", MB_OK, BEHAVIOR_REGENERATION); // ???? ???? ?????? ????..
		}
		
		pTarget->m_fTimeAfterDeath = 0.1f; // ????????..
		if(pTarget->m_pShapeExtraRef) s_pOPMgr->CharacterDelete(pTarget->IDNumber()); // ???????? ?????? ??????????..!!
	}

	if(pTarget == s_pPlayer) 
	{
		//TRACE("player is under of attack (%d)\n", iIDAttacker);
	}

	return true;
}

bool CGameProcMain::MsgRecv_Dead(Packet& pkt)
{
	int iIDTarget	= pkt.read<int16_t>(); // ?????? ???? ??????????.

	
	CPlayerBase* pTarget = NULL;
	if(iIDTarget == s_pPlayer->IDNumber())
	{
		pTarget = s_pPlayer;

//		if(m_pUIDead) m_pUIDead->SetVisible(true);
		std::string szMsg; ::_LoadStringFromResource(IDS_REGENERATION, szMsg);
		CGameProcedure::MessageBoxPost(szMsg, "", MB_OK, BEHAVIOR_REGENERATION);
		CLogWriter::Write("Dead!!!");
	}
	else
	{
		pTarget = this->CharacterGetByID(iIDTarget, false);
	}

	if(pTarget)
	{
		if(pTarget->m_pShapeExtraRef) s_pOPMgr->CharacterDelete(pTarget->IDNumber());
		else
		{
			pTarget->m_fTimeAfterDeath = 0.1f; // ????????..
			pTarget->ActionDying(PSD_KEEP_POSITION, __Vector3(0,0,1));
		}

		return true;
	}

	return false;
}

bool CGameProcMain::MsgRecv_ItemMove(Packet& pkt)
{
	__InfoPlayerMySelf* pInfoExt = &(s_pPlayer->m_InfoExt);
	__InfoPlayerBase* pInfoBase = &(s_pPlayer->m_InfoBase);

	uint8_t bResult =		pkt.read<uint8_t>();	// 0x01 : true, 0x00 : false..
	if (bResult)
	{
		pInfoExt->iAttack = pkt.read<int16_t>();
		pInfoExt->iGuard =	pkt.read<int16_t>();
		pInfoExt->iWeightMax = pkt.read<int16_t>();
		
		pInfoBase->iHPMax = pkt.read<int16_t>();
		pInfoExt->iMSPMax = pkt.read<int16_t>();
		
		// ???????? ???? ????????????..
		pInfoExt->iStrength_Delta =		pkt.read<uint16_t>();
		pInfoExt->iStamina_Delta	=	pkt.read<uint16_t>();
		pInfoExt->iDexterity_Delta =	pkt.read<uint16_t>();
		pInfoExt->iIntelligence_Delta =	pkt.read<uint16_t>();
		pInfoExt->iMagicAttak_Delta =	pkt.read<uint16_t>();

		pInfoExt->iRegistFire =		pkt.read<uint16_t>();
		pInfoExt->iRegistCold =		pkt.read<uint16_t>();
		pInfoExt->iRegistLight =	pkt.read<uint16_t>();
		pInfoExt->iRegistMagic =	pkt.read<uint16_t>();
		pInfoExt->iRegistCurse =	pkt.read<uint16_t>();
		pInfoExt->iRegistPoison	=	pkt.read<uint16_t>();

		if(pInfoBase->iHP > pInfoBase->iHPMax) pInfoBase->iHP = pInfoBase->iHPMax; // ????????..
		if(pInfoExt->iMSP > pInfoExt->iMSPMax) pInfoExt->iMSP = pInfoExt->iMSPMax; // ????????..

		m_pUIVar->m_pPageState->UpdateHP(pInfoBase->iHP, pInfoBase->iHPMax);
		m_pUIVar->m_pPageState->UpdateMSP(pInfoExt->iMSP, pInfoExt->iMSPMax);

		m_pUIStateBarAndMiniMap->UpdateHP(pInfoBase->iHP, pInfoBase->iHPMax, false);
		m_pUIStateBarAndMiniMap->UpdateMSP(pInfoExt->iMSP, pInfoExt->iMSPMax, false);
		
		m_pUIVar->m_pPageState->UpdateAttackPoint(pInfoExt->iAttack, pInfoExt->iAttack_Delta);
		m_pUIVar->m_pPageState->UpdateGuardPoint(pInfoExt->iGuard, pInfoExt->iGuard_Delta);
		m_pUIVar->m_pPageState->UpdateWeight(pInfoExt->iWeight, pInfoExt->iWeightMax);
		
		m_pUIVar->m_pPageState->UpdateStrength(pInfoExt->iStrength, pInfoExt->iStrength_Delta);
		m_pUIVar->m_pPageState->UpdateStamina(pInfoExt->iStamina, pInfoExt->iStamina_Delta);
		m_pUIVar->m_pPageState->UpdateDexterity(pInfoExt->iDexterity, pInfoExt->iDexterity_Delta);
		m_pUIVar->m_pPageState->UpdateIntelligence(pInfoExt->iIntelligence, pInfoExt->iIntelligence_Delta);
		m_pUIVar->m_pPageState->UpdateMagicAttak(pInfoExt->iMagicAttak, pInfoExt->iMagicAttak_Delta);

		m_pUIVar->m_pPageState->UpdateRegistFire(pInfoExt->iRegistFire, pInfoExt->iRegistFire_Delta);
		m_pUIVar->m_pPageState->UpdateRegistCold(pInfoExt->iRegistCold, pInfoExt->iRegistCold_Delta);
		m_pUIVar->m_pPageState->UpdateRegistLight(pInfoExt->iRegistLight, pInfoExt->iRegistLight_Delta);
		m_pUIVar->m_pPageState->UpdateRegistMagic(pInfoExt->iRegistMagic, pInfoExt->iRegistMagic_Delta);
		m_pUIVar->m_pPageState->UpdateRegistCurse(pInfoExt->iRegistCurse, pInfoExt->iRegistCurse_Delta);
		m_pUIVar->m_pPageState->UpdateRegistPoison(pInfoExt->iRegistPoison, pInfoExt->iRegistPoison_Delta);
	}

	m_pUIInventory->ReceiveResultFromServer(bResult);
	//TRACE("Init Inv Move Res %d \n", (int)bResult);

	return true;
}

bool CGameProcMain::MsgRecv_ItemWeightChange(Packet& pkt)		// ?????? ???? ????..
{
	__InfoPlayerMySelf* pInfoExt = &(s_pPlayer->m_InfoExt);
	pInfoExt->iWeight = pkt.read<int16_t>();
	m_pUIVar->m_pPageState->UpdateWeight(pInfoExt->iWeight, pInfoExt->iWeightMax);

	return true;
}

bool CGameProcMain::MsgRecv_UserLookChange(Packet& pkt)
{
	int iID				= pkt.read<int16_t>();
	e_ItemSlot eSlot	= (e_ItemSlot)pkt.read<uint8_t>();
	uint32_t dwItemID = pkt.read<uint32_t>();
	int iDurability = pkt.read<int16_t>();

	if(iID == s_pPlayer->IDNumber()) return false;
	
	CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(iID, false);
	if(NULL == pUPC) return false;

	__TABLE_ITEM_BASIC* pItem = s_pTbl_Items_Basic.Find(dwItemID/1000*1000);

	__TABLE_ITEM_EXT* pItemExt = NULL;
	if(pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION)
		pItemExt = s_pTbl_Items_Exts[pItem->byExtIndex].Find(dwItemID%1000);	// ?? ?????? ????..
	if(dwItemID && (NULL == pItem || NULL == pItemExt))
	{
		__ASSERT(0, "NULL Item!!!");
		return false;
	}

	e_PartPosition ePartPos = PART_POS_UNKNOWN, ePartPos2 = PART_POS_UNKNOWN;
	e_PlugPosition ePlugPos = PLUG_POS_UNKNOWN, ePlugPos2 = PLUG_POS_UNKNOWN;

	if(ITEM_SLOT_HEAD == eSlot) ePartPos = PART_POS_HAIR_HELMET;
	else if(ITEM_SLOT_UPPER == eSlot) ePartPos = PART_POS_UPPER;
	else if(ITEM_SLOT_LOWER == eSlot) ePartPos = PART_POS_LOWER;
	else if(ITEM_SLOT_GLOVES == eSlot) ePartPos = PART_POS_HANDS;
	else if(ITEM_SLOT_SHOES == eSlot) ePartPos = PART_POS_FEET;
	else if(ITEM_SLOT_HAND_RIGHT == eSlot) ePlugPos = PLUG_POS_RIGHTHAND;
	else if(ITEM_SLOT_HAND_LEFT == eSlot) ePlugPos = PLUG_POS_LEFTHAND;

	if(ePartPos != PART_POS_UNKNOWN) 
	{
		if(dwItemID) // ???????? ???? ????
		{
			std::string szItemFN;
			CGameProcedure::MakeResrcFileNameForUPC(pItem, &szItemFN, NULL, ePartPos2, ePlugPos2, s_pPlayer->m_InfoBase.eRace);
			pUPC->PartSet(ePartPos, szItemFN, pItem, pItemExt); // ?????? ??????..
			pUPC->DurabilitySet(eSlot, iDurability);
		}
		else
		{
			__TABLE_PLAYER_LOOKS* pLooks = s_pTbl_UPC_Looks.Find(pUPC->m_InfoBase.eRace);	// User Player Character Skin ?????? ??????..
			if(NULL == pLooks)
			{
				CLogWriter::Write("CGameProcMain::MsgRecv_UserLookChange() - failed find table : Race (%d)", pUPC->m_InfoBase.eRace);
				__ASSERT(pLooks, "failed find table");
			}
			else
			{
				if(PART_POS_HAIR_HELMET == ePartPos) pUPC->InitHair(); // ???????? ???? ??????..
				else pUPC->PartSet(ePartPos, pLooks->szPartFNs[ePartPos], NULL, NULL);
			}
		}

		return true;
	}
	else if(ePlugPos != PLUG_POS_UNKNOWN)
	{
		if(dwItemID)
		{
			std::string szItemFN;
			CGameProcedure::MakeResrcFileNameForUPC(pItem, &szItemFN, NULL, ePartPos2, ePlugPos2, s_pPlayer->m_InfoBase.eRace);
			pUPC->PlugSet(ePlugPos, szItemFN, pItem, pItemExt);
			pUPC->DurabilitySet(eSlot, iDurability);
		}
		else
		{
			pUPC->PlugSet(ePlugPos, "", NULL, NULL);
			pUPC->DurabilitySet(eSlot, 0);
		}

		// Sound Range ???????? ???????? ??????????..
		__Vector3 vPosPlayer = s_pPlayer->Position();
		float fDist = pUPC->Distance(vPosPlayer);
		if(fDist < SOUND_RANGE_TO_SET) pUPC->SetSoundPlug(pItem); // SOUND_RANGE ???? ??????.

		return true;
	}
	else
	{
		CLogWriter::Write("CGameProcMain::MsgRecv_UserLookChange() - Unknown Slot(%d) Item(%d)", eSlot, dwItemID);
		return false;
	}

	return false;
}

bool CGameProcMain::MsgRecv_ItemBundleDrop(Packet& pkt)		// Item ?? ?????? ???????????? ???? ????
{
	int iID = pkt.read<int16_t>();
	int iItemID = pkt.read<uint32_t>();

	CPlayerBase* pCorpse = s_pOPMgr->NPCGetByID(iID, false);
	if(NULL == pCorpse) pCorpse = s_pOPMgr->CorpseGetByID(iID);
	
	if(pCorpse)
		pCorpse->m_iDroppedItemID = iItemID; // ???????? ?????? ?????? ????

	return true;
}

bool CGameProcMain::MsgRecv_ItemBundleOpen(Packet& pkt)		// ?????? ?????? ?????? ?????? ??????..
{
	uint32_t dwItemID = 0;
	int iItemCount = 0;

	POINT ptCur = s_pLocalInput->MouseGetPos();
	m_pUIDroppedItemDlg->EnterDroppedState(ptCur.x, ptCur.y);
	//TRACE ("?????? ?????? ????.. \n");

	for(int i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++)
	{
		dwItemID = pkt.read<uint32_t>();
		iItemCount = pkt.read<int16_t>();

		// ???????? ?????? ?????????? ???? ????????..
		if ( dwItemID )
			m_pUIDroppedItemDlg->AddToItemTable(dwItemID, iItemCount, i);
	}

	m_pUIDroppedItemDlg->InitIconUpdate();

	return true;
}

void CGameProcMain::MsgRecv_ItemRepair(Packet& pkt)			// Item Repair Result.. 
{
	int iResult = pkt.read<uint8_t>();		// Trade id
	int iGold   = pkt.read<uint32_t>();		// Trade id
	m_pUIInventory->ReceiveResultFromServer( iResult, iGold );
}

void CGameProcMain::MsgRecv_ItemCountChange(Packet& pkt)		// Item Count Change..
{
	int iTotalCount = pkt.read<int16_t>();		// Trade id

	for( int i = 0; i < iTotalCount; i++ )
	{
		int iDistrict	= pkt.read<uint8_t>();
		int iIndex		= pkt.read<uint8_t>();
		int iID			  = pkt.read<uint32_t>();
		int	iCount		= pkt.read<uint32_t>();
		int iNewItem	= pkt.read<uint8_t>(); // 100 for new items, 0 otherwise
		int iDurability = pkt.read<uint16_t>();
		m_pUIInventory->ItemCountChange(iDistrict, iIndex, iCount, iID, iDurability);
		if (iNewItem == 100)
		{
			char szBuf[256] = { 0 };
			__TABLE_ITEM_BASIC* pItem = CGameProcedure::s_pTbl_Items_Basic.Find(iID / 1000 * 1000);
			if (pItem != nullptr)
			{
				std::string szFmt;  ::_LoadStringFromResource(IDS_ITEM_RECEIVED, szFmt);
				snprintf(szBuf, sizeof(szBuf), szFmt.c_str(), pItem->szName.c_str());
				MsgOutput(szBuf, 0xFFFFFF00);
			}
		}
	}	

	if (m_pUISkillTreeDlg) m_pUISkillTreeDlg->UpdateDisableCheck();
	if (m_pUIHotKeyDlg) m_pUIHotKeyDlg->UpdateDisableCheck();
}

void CGameProcMain::MsgRecv_ItemDestroy(Packet& pkt)			// Item Count Change..
{
	int	iResult		= pkt.read<uint8_t>();		// Trade id
	m_pUIInventory->ReceiveResultItemRemoveFromServer(iResult);
};

void CGameProcMain::MsgRecv_MyInfo_HP(Packet& pkt)
{
	int iHPMax =	pkt.read<int16_t>();	
	int iHP =		pkt.read<int16_t>();	

	int iHPChange = iHP - s_pPlayer->m_InfoBase.iHP;
	char szBuf[256] = "";
	if(iHPChange < 0)
	{
		std::string szFmt; ::_LoadStringFromResource(IDS_MSG_FMT_HP_LOST, szFmt);
		sprintf(szBuf, szFmt.c_str(), -iHPChange);
		MsgOutput(szBuf, 0xffff3b3b);
	}
	else if(iHPChange > 0)
	{
		std::string szFmt; ::_LoadStringFromResource(IDS_MSG_FMT_HP_RECOVER, szFmt);
		sprintf(szBuf, szFmt.c_str(), iHPChange);
		MsgOutput(szBuf, 0xff6565ff);
	}

	s_pPlayer->m_InfoBase.iHPMax = iHPMax;
	s_pPlayer->m_InfoBase.iHP = iHP;

	m_pUIVar->m_pPageState->UpdateHP(iHP, iHPMax);
	m_pUIStateBarAndMiniMap->UpdateHP(iHP, iHPMax, false);
}

void CGameProcMain::MsgRecv_MyInfo_MSP(Packet& pkt)
{
	int iMSPMax = pkt.read<int16_t>();	
	int iMSP = pkt.read<int16_t>();
	
	int iMSPChange = iMSP - s_pPlayer->m_InfoExt.iMSP;
	
	e_Class_Represent eCR = CGameProcedure::GetRepresentClass(s_pPlayer->m_InfoBase.eClass);
	bool bUseMP = (CLASS_REPRESENT_WARRIOR != eCR && CLASS_REPRESENT_ROGUE != eCR) ? true : false;
	
	char szBuf[256] = "";
	if(iMSPChange < 0)
	{
		std::string szFmt;
		if (bUseMP) ::_LoadStringFromResource(IDS_MSG_FMT_MP_USE, szFmt);
		else ::_LoadStringFromResource(IDS_MSG_FMT_SP_USE, szFmt); //IDS_MSG_FMT_SP_USE
		sprintf(szBuf, szFmt.c_str(), -iMSPChange);
		MsgOutput(szBuf, 0xffff3b3b);
	}
	else if(iMSPChange > 0)
	{
		std::string szFmt;
		if (bUseMP) ::_LoadStringFromResource(IDS_MSG_FMT_MP_RECOVER, szFmt);
		else ::_LoadStringFromResource(IDS_MSG_FMT_SP_RECOVER, szFmt); //IDS_MSG_FMT_SP_RECOVER
		sprintf(szBuf, szFmt.c_str(), iMSPChange);
		MsgOutput(szBuf, 0xff6565ff);
	}

	s_pPlayer->m_InfoExt.iMSPMax = iMSPMax;
	s_pPlayer->m_InfoExt.iMSP = iMSP;

	m_pUIVar->m_pPageState->UpdateMSP(iMSP, iMSPMax);
	m_pUIStateBarAndMiniMap->UpdateMSP(iMSP, iMSPMax, false);

	if (m_pUISkillTreeDlg) m_pUISkillTreeDlg->UpdateDisableCheck();
	if (m_pUIHotKeyDlg) m_pUIHotKeyDlg->UpdateDisableCheck();
}

void CGameProcMain::MsgRecv_MyInfo_EXP(Packet& pkt)
{
	Uint64 iExp = pkt.read<uint32_t>();
	Uint64 iOldExp = s_pPlayer->m_InfoExt.iExp;

	s_pPlayer->m_InfoExt.iExp = iExp;
	m_pUIVar->m_pPageState->UpdateExp(iExp, s_pPlayer->m_InfoExt.iExpNext);
	m_pUIStateBarAndMiniMap->UpdateExp(iExp, s_pPlayer->m_InfoExt.iExpNext, false);

	if(s_pPlayer->m_InfoExt.iLevelPrev == s_pPlayer->m_InfoBase.iLevel && iExp != iOldExp)
	{
		char szBuf[256] = "";
		if (iExp > iOldExp)
		{
			std::string szFmt; ::_LoadStringFromResource(IDS_MSG_FMT_EXP_GET, szFmt);
			sprintf(szBuf, szFmt.c_str(), iExp - iOldExp);
		}
		else if (iExp < iOldExp)
		{
			std::string szFmt; ::_LoadStringFromResource(IDS_MSG_FMT_EXP_LOST, szFmt);
			sprintf(szBuf, szFmt.c_str(), iOldExp - iExp);
		}
		MsgOutput(szBuf, 0xffffff00);
	}

	s_pPlayer->m_InfoExt.iLevelPrev = s_pPlayer->m_InfoBase.iLevel;
}

bool CGameProcMain::MsgRecv_MyInfo_LevelChange(Packet& pkt)
{
	int iID = pkt.read<int16_t>();
	int iLevel = pkt.read<uint8_t>();
	if(iID == s_pPlayer->IDNumber())
	{
		__InfoPlayerBase*	pInfoBase = &(s_pPlayer->m_InfoBase);
		__InfoPlayerMySelf*	pInfoExt = &(s_pPlayer->m_InfoExt);
		
		int iLevelPrev = pInfoBase->iLevel;
		pInfoBase->iLevel = iLevel;
		pInfoExt->iBonusPointRemain = pkt.read<uint8_t>(); // ???? ?????? ??????..

		uint8_t	bExtraSkillPoint		= pkt.read<uint8_t>();	// ???? ??????
		//TRACE("Skill change Extra value %d\n", bExtraSkillPoint);

		Uint64 iExpNext	= pkt.read<uint32_t>(); 
		Uint64 iExp		= pkt.read<uint32_t>();
			
		pInfoExt->iExpNext	= iExpNext; 
		pInfoExt->iExp		= iExp; 

		pInfoBase->iHPMax =		pkt.read<int16_t>();	
		pInfoBase->iHP =		pkt.read<int16_t>();

		pInfoExt->iMSPMax =		pkt.read<int16_t>();
		pInfoExt->iMSP =		pkt.read<int16_t>();

		pInfoExt->iWeightMax =	pkt.read<int16_t>();
		pInfoExt->iWeight	=	pkt.read<int16_t>();

		m_pUIVar->UpdateAllStates(&(s_pPlayer->m_InfoBase), &(s_pPlayer->m_InfoExt)); // ???? ???? ????????..

		m_pUIStateBarAndMiniMap->UpdateExp(pInfoExt->iExp, pInfoExt->iExpNext, true);
		m_pUIStateBarAndMiniMap->UpdateHP(pInfoBase->iHP, pInfoBase->iHPMax, false);
		m_pUIStateBarAndMiniMap->UpdateMSP(pInfoExt->iMSP, pInfoExt->iMSPMax, false);

		m_pUISkillTreeDlg->m_iSkillInfo[0] = bExtraSkillPoint;
		m_pUISkillTreeDlg->InitIconUpdate();		// ?????? ?????????????? .. ?????? ?????? ?? ????..

		if(iLevel > iLevelPrev) // ?????? Level Up ?????? ????????..
		{
			if(s_pPlayer->Nation()==NATION_KARUS) CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_LEVELUP_KARUS, iID, -1);
			else if(s_pPlayer->Nation()==NATION_ELMORAD) CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_LEVELUP_ELMORAD, iID, -1);
		}
	}
	else // ???? ??????..
	{
		CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(iID, false);
		if(pUPC)
		{
			if(iLevel > pUPC->m_InfoBase.iLevel)
			{
				if(pUPC->Nation()==NATION_KARUS) CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_LEVELUP_KARUS, iID, -1);
				else if(pUPC->Nation()==NATION_ELMORAD) CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_LEVELUP_ELMORAD, iID, -1);
			}
			pUPC->m_InfoBase.iLevel = iLevel;
		}
	}

	return TRUE;
}

void CGameProcMain::MsgRecv_MyInfo_RealmPoint(Packet& pkt)
{
	uint8_t bType = pkt.read<uint8_t>();

	if (bType == 1) // TODO: @Demircivi: LOYALTY_NATIONAL_POINTS is: 1, define it in header. 
	{
		// TODO: @Demircivi, implement missing

		uint32_t iLoyalty = pkt.read<uint32_t>();
		uint32_t iLoyaltyMonthly = pkt.read<uint32_t>();
		uint32_t iUnk = pkt.read<uint32_t>();				// Clan donations(? Donations made by this user? For the clan overall?)
		uint32_t iClanLoyaltyAmount = pkt.read<uint32_t>();	// Premium NP(? Additional NP gained?)

		int32_t iLoyaltyDelta = iLoyalty - s_pPlayer->m_InfoExt.iRealmPoint;
		if (iLoyaltyDelta > 0) // Got NP.
		{
			char szBuf[128]; std::string szFmt; ::_LoadStringFromResource(IDS_LOYALTY_CHANGE_GET, szFmt);
			sprintf(szBuf, szFmt.c_str(), iLoyaltyDelta);
			MsgOutput(szBuf, 0xffa2a0c8);
		}
		else // Lost NP.
		{
			char szBuf[128]; std::string szFmt; ::_LoadStringFromResource(IDS_LOYALTY_CHANGE_LOST, szFmt);
			sprintf(szBuf, szFmt.c_str(), -iLoyaltyDelta);
			MsgOutput(szBuf, 0xffff3b3b);
		}

		s_pPlayer->m_InfoExt.iRealmPoint = iLoyalty;
		s_pPlayer->m_InfoExt.iRealmPointMonthly = iLoyaltyMonthly;

		if (m_pUIVar->m_pPageState)
			m_pUIVar->m_pPageState->UpdateRealmPoint(s_pPlayer->m_InfoExt.iRealmPoint, s_pPlayer->m_InfoExt.iRealmPointMonthly); // ???? ???????? 10?? ???????? ????
	}
	else if (bType == 2)
	{
		// TODO: @Demircivi, after implementing Manner feature call its update method from here.
		uint32_t iNewManner = pkt.read<uint32_t>(); // IDS_MANNER_CHANGE_GET / IDS_MANNER_CHANGE_LOST
		CLogWriter::Write("Got manner update packet but didn't update form since there is no manner feature New Manner: %d.", iNewManner);
	}
	else
	{
		CLogWriter::Write("Unhandled MsgRecv_MyInfo_RealmPoint subOpcode: %02x(hex).", bType);
	}
}

void CGameProcMain::MsgRecv_MyInfo_PointChange(Packet& pkt)
{
	int iType = pkt.read<uint8_t>();
	int iVal = pkt.read<int16_t>();

	s_pPlayer->m_InfoBase.iHPMax =		pkt.read<int16_t>();
	s_pPlayer->m_InfoExt.iMSPMax =		pkt.read<int16_t>();
	s_pPlayer->m_InfoExt.iAttack =		pkt.read<int16_t>();
	s_pPlayer->m_InfoExt.iWeightMax =	pkt.read<int16_t>();

	m_pUIVar->m_pPageState->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax);
	m_pUIStateBarAndMiniMap->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax, false);
	
	m_pUIVar->m_pPageState->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax);
	m_pUIStateBarAndMiniMap->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax, false);

	m_pUIVar->m_pPageState->UpdateAttackPoint(s_pPlayer->m_InfoExt.iAttack, s_pPlayer->m_InfoExt.iAttack_Delta);
	m_pUIVar->m_pPageState->UpdateWeight(s_pPlayer->m_InfoExt.iWeight, s_pPlayer->m_InfoExt.iWeightMax);


// ?????? ?????? -	 ?????? b1(1?? 2???? 3???? 4???? 5????) s(-1 +1)
//					 ?????? b1(1?? 2???? 3???? 4???? 5????) s(????????)

	if(0x01 == iType) // Strength
	{
		s_pPlayer->m_InfoExt.iStrength = iVal;
		m_pUIVar->m_pPageState->UpdateStrength(s_pPlayer->m_InfoExt.iStrength, s_pPlayer->m_InfoExt.iStrength_Delta);
	}
	else if(0x02 == iType) // Stamina
	{
		s_pPlayer->m_InfoExt.iStamina = iVal;
		m_pUIVar->m_pPageState->UpdateStamina(s_pPlayer->m_InfoExt.iStamina, s_pPlayer->m_InfoExt.iStamina_Delta);
	}
	else if(0x03 == iType) // Dex
	{
		s_pPlayer->m_InfoExt.iDexterity = iVal;
		m_pUIVar->m_pPageState->UpdateDexterity(s_pPlayer->m_InfoExt.iDexterity, s_pPlayer->m_InfoExt.iDexterity_Delta);
	}
	else if(0x04 == iType) // Intelligence
	{
		s_pPlayer->m_InfoExt.iIntelligence = iVal;
		m_pUIVar->m_pPageState->UpdateIntelligence(s_pPlayer->m_InfoExt.iIntelligence, s_pPlayer->m_InfoExt.iIntelligence_Delta);
	}
	else if(0x05 == iType) // Magic Attack
	{
		s_pPlayer->m_InfoExt.iMagicAttak = iVal;
		m_pUIVar->m_pPageState->UpdateMagicAttak(s_pPlayer->m_InfoExt.iMagicAttak, s_pPlayer->m_InfoExt.iMagicAttak_Delta);
	}

	if(iType >= 1 && iType <= 5)
	{
		s_pPlayer->m_InfoExt.iBonusPointRemain--;
		m_pUIVar->m_pPageState->UpdateBonusPointAndButtons(s_pPlayer->m_InfoExt.iBonusPointRemain); // ?????? ?????? ?????? ??????????
	}
}

void CGameProcMain::InitUI()
{
	const int iW = CN3Base::s_CameraData.vp.Width;
	const int iH = CN3Base::s_CameraData.vp.Height;

	RECT rc;
	int iX = 0, iY = 0;

	e_Nation eNation = s_pPlayer->m_InfoBase.eNation; // ????....

	__TABLE_UI_RESRC* pTbl = s_pTbl_UI.Find(eNation);
	if(NULL == pTbl) return;
	
	m_pUICmd->Init(s_pUIMgr);
	m_pUICmd->LoadFromFile(pTbl->szCmd);
	rc = m_pUICmd->GetRegion();
	m_pUICmd->SetPos((iW - (rc.right - rc.left))/2, iH - (rc.bottom - rc.top));
	m_pUICmd->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);

	m_pUIChatDlg->Init(s_pUIMgr);					//Manager ???????? ???????? ???? 
	m_pUIChatDlg->LoadFromFile(pTbl->szChat);
	rc = m_pUIChatDlg->GetRegion();
	RECT rcCmd = m_pUICmd->GetRegion(); rcCmd.top += 5; // .. ???? ????..
	iX = 0;
	iY = iH - ((rc.bottom - rc.top) + (rcCmd.bottom - rcCmd.top));
	CGameProcedure::UIPostData_Read(UI_POST_WND_CHAT, m_pUIChatDlg, iX, iY);
	m_pUIChatDlg->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);
	m_pUIChatDlg->SetVisibleWithNoSound(true);

	m_pUIChatDlg2->Init(s_pUIMgr);
	m_pUIChatDlg2->LoadFromFile(pTbl->szChat2);
	m_pUIChatDlg2->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);
	m_pUIChatDlg2->SetVisibleWithNoSound(false);

	m_pUIMsgDlg->Init(s_pUIMgr);
	m_pUIMsgDlg->LoadFromFile(pTbl->szMsgOutput);
	CGameProcedure::UIPostData_Read(UI_POST_WND_INFO, m_pUIMsgDlg, rc.right, rc.top);
	m_pUIMsgDlg->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);
	m_pUIMsgDlg->SetVisibleWithNoSound(true);

	m_pUIMsgDlg2->Init(s_pUIMgr);
	m_pUIMsgDlg2->LoadFromFile(pTbl->szMsgOutput2);
	m_pUIMsgDlg2->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);
	m_pUIMsgDlg2->SetVisibleWithNoSound(false);

	// ???????? ?????? ?? ???? ??????..
	m_pUIChatDlg->MoveOffset(0, -1);
	m_pUIMsgDlg->MoveOffset(0, -1);

	m_pUIStateBarAndMiniMap->Init(s_pUIMgr);
	m_pUIStateBarAndMiniMap->LoadFromFile(pTbl->szStateBar);
	m_pUIStateBarAndMiniMap->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);
#ifdef _DEBUG
	m_pUIStateBarAndMiniMap->SetPos(0, 70); // ?????? ???? ?????????? ???? ??????....
#else
	m_pUIStateBarAndMiniMap->SetPos(0, 0);
#endif

	// ?????? UI - ????, ??????????, ??????, ???? ??????...
	m_pUIVar->Init(s_pUIMgr);
	m_pUIVar->LoadFromFile(pTbl->szVarious);
	m_pUIVar->SetVisibleWithNoSound(false);
	m_pUIVar->m_pPageState->LoadFromFile(pTbl->szState);
	m_pUIVar->m_pPageState->SetVisibleWithNoSound(true);
	m_pUIVar->m_pPageKnights->LoadFromFile(pTbl->szKnights);
	m_pUIVar->m_pPageKnights->SetVisibleWithNoSound(false);
	m_pUIVar->m_pPageFriends->LoadFromFile(pTbl->szFriends);
	m_pUIVar->m_pPageFriends->SetVisibleWithNoSound(false);
	m_pUIVar->SetStyle(m_pUIVar->GetStyle() | UISTYLE_POS_LEFT);

	// TODO(srmeier): need to implement this
	m_pUIVar->m_pPageQuest->LoadFromFile(pTbl->szQuest);
	m_pUIVar->m_pPageQuest->SetVisibleWithNoSound(false);


	
	m_pUITargetBar->Init(s_pUIMgr);
	m_pUITargetBar->LoadFromFile(pTbl->szTargetBar);
	m_pUITargetBar->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);
	rc = m_pUITargetBar->GetRegion();
	m_pUITargetBar->SetPos((iW - (rc.right - rc.left))/2, 0);
	m_pUITargetBar->SetVisibleWithNoSound(false);

	m_pUIHelp->Init(s_pUIMgr);
	m_pUIHelp->LoadFromFile(pTbl->szHelp);
	CGameProcedure::UIPostData_Read(UI_POST_WND_HELP, m_pUIHelp, 0, 0);
	rc = m_pUIHelp->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIHelp->SetPos(iX, iY);
//	m_pUIHelp->SetStyle(UISTYLE_SHOW_ME_ALONE);
	
	// ????????..
	m_pUINotice->Init(s_pUIMgr);
	m_pUINotice->LoadFromFile(pTbl->szNotice);
	m_pUINotice->SetVisibleWithNoSound(false);

	m_pUIClassChange->Init(s_pUIMgr);
	m_pUIClassChange->LoadFromFile(pTbl->szClassChange);
	m_pUIClassChange->SetVisibleWithNoSound(false);
	rc = m_pUIClassChange->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIClassChange->SetPos(iX, iY);
	m_pUIClassChange->SetStyle(UISTYLE_USER_MOVE_HIDE);

	m_pUINpcChange->Init(s_pUIMgr);
	m_pUINpcChange->LoadFromFile(pTbl->szChangeClassInit);
	m_pUINpcChange->SetVisibleWithNoSound(false);
	rc = m_pUINpcChange->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUINpcChange->SetPos(iX, iY);
	m_pUINpcChange->SetStyle(UISTYLE_SHOW_ME_ALONE | UISTYLE_USER_MOVE_HIDE);

	m_pUINpcEvent->Init(s_pUIMgr);
	m_pUINpcEvent->LoadFromFile(pTbl->szNpcEvent);
	m_pUINpcEvent->SetVisibleWithNoSound(false);
	m_pUINpcEvent->SetStyle(UISTYLE_SHOW_ME_ALONE | UISTYLE_USER_MOVE_HIDE);
	rc = m_pUINpcEvent->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUINpcEvent->SetPos(iX, iY);

	m_pUINpcTalk->Init(s_pUIMgr);
	m_pUINpcTalk->LoadFromFile(pTbl->szNpcTalk);
	m_pUINpcTalk->SetVisibleWithNoSound(false);
	rc = m_pUINpcTalk->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUINpcTalk->SetPos(iX, iY);

	m_pUIWarp->Init(s_pUIMgr);
	m_pUIWarp->LoadFromFile(pTbl->szZoneChangeOrWarp);
	m_pUIWarp->SetVisibleWithNoSound(false);
	rc = m_pUIWarp->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIWarp->SetPos(iX, iY);
	m_pUIWarp->SetStyle(UISTYLE_USER_MOVE_HIDE | UISTYLE_SHOW_ME_ALONE);

//	m_pUITradeList->Init(s_pUIMgr);
//	m_pUITradeList->LoadFromFile(pTbl->szNpcExchangeList);
//	m_pUITradeList->SetVisibleWithNoSound(false);
//	rc = m_pUITradeList->GetRegion();
//	iX = (iW - (rc.right - rc.left))/2;
//	iY = (iH - (rc.bottom - rc.top))/2;
//	m_pUITradeList->SetPos(iX, iY);

	m_pUIItemREDlg->Init(s_pUIMgr);
	m_pUIItemREDlg->LoadFromFile(pTbl->szExchangeRepair);
	m_pUIItemREDlg->SetVisibleWithNoSound(false);
	rc = m_pUIItemREDlg->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIItemREDlg->SetPos(iX, iY);
	m_pUIItemREDlg->InitIconWnd(UIWND_EXCHANGE_REPAIR);
	m_pUIItemREDlg->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUIItemREDlg->SetState(UI_STATE_COMMON_NONE);

	m_pUIRepairTooltip->Init(s_pUIMgr);
	m_pUIRepairTooltip->LoadFromFile(pTbl->szRepairTooltip);
	m_pUIRepairTooltip->SetVisibleWithNoSound(false);
	m_pUIRepairTooltip->InitPos();
	m_pUIRepairTooltip->SetStyle(UISTYLE_ALWAYSTOP|UISTYLE_FOCUS_UNABLE);

	m_pUIPartyOrForce->Init(s_pUIMgr);
	m_pUIPartyOrForce->LoadFromFile(pTbl->szPartyOrForce);
	m_pUIPartyOrForce->SetStyle(UISTYLE_HIDE_UNABLE);
	rc = m_pUIPartyOrForce->GetRegion();
	iX = iW - (rc.right - rc.left);
	m_pUIPartyOrForce->SetPos(iX, 0);
	m_pUIPartyOrForce->SetVisible(false); // ?????? ????????~
//	CGameProcedure::UIPostData_Read(UI_POST_WND_PARTY, m_pUIPartyOrForce, iX, 0);

	// Dropped Item Dlg.. 
	m_pUIDroppedItemDlg->Init(s_pUIMgr);
	m_pUIDroppedItemDlg->LoadFromFile(pTbl->szDroppedItem);
	m_pUIDroppedItemDlg->SetVisibleWithNoSound(false);
	m_pUIDroppedItemDlg->InitIconWnd(UIWND_DROPITEM);
	m_pUIDroppedItemDlg->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUIDroppedItemDlg->SetState(UI_STATE_COMMON_NONE);
	m_pUIDroppedItemDlg->SetStyle(m_pUIDroppedItemDlg->GetStyle() | UISTYLE_USER_MOVE_HIDE);

	m_pUITransactionDlg->Init(s_pUIMgr);
	m_pUITransactionDlg->LoadFromFile(pTbl->szTransaction);
	rc = m_pUITransactionDlg->GetRegion();
	m_pUITransactionDlg->SetPos(iW - (rc.right-rc.left), 10);
	m_pUITransactionDlg->SetVisibleWithNoSound(false);
	m_pUITransactionDlg->InitIconWnd(UIWND_TRANSACTION);
	m_pUITransactionDlg->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUITransactionDlg->SetState(UI_STATE_COMMON_NONE);
	m_pUITransactionDlg->SetStyle(m_pUITransactionDlg->GetStyle() | UISTYLE_POS_RIGHT | UISTYLE_USER_MOVE_HIDE);

	m_pSubProcPerTrade->InitPerTradeDlg(s_pUIMgr);

	m_pUIInventory->Init(s_pUIMgr);
	m_pUIInventory->LoadFromFile(pTbl->szInventory);
	m_pUIInventory->SetVisibleWithNoSound(false);
	m_pUIInventory->SetPos(465,10);
	m_pUIInventory->InitIconWnd(UIWND_INVENTORY);
	m_pUIInventory->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUIInventory->SetState(UI_STATE_COMMON_NONE);
	m_pUIInventory->SetStyle(m_pUIInventory->GetStyle() | UISTYLE_POS_RIGHT);

	// Countable Item Edit Dlg..
	CN3UIWndBase::m_pCountableItemEdit = new CCountableItemEditDlg;
	CN3UIWndBase::m_pCountableItemEdit->Init(s_pUIMgr);
	CN3UIWndBase::m_pCountableItemEdit->LoadFromFile(pTbl->szPersonalTradeEdit);
	CN3UIWndBase::m_pCountableItemEdit->SetStyle(UISTYLE_ALWAYSTOP);
	// ???? ???? ..
	rc = CN3UIWndBase::m_pCountableItemEdit->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	CN3UIWndBase::m_pCountableItemEdit->SetPos(iX, iY);	
	CN3UIWndBase::m_pCountableItemEdit->SetVisibleWithNoSound(false);
	CN3UIWndBase::m_pCountableItemEdit->SetUIType(UI_TYPE_BASE);
	CN3UIWndBase::m_pCountableItemEdit->SetState(UI_STATE_COMMON_NONE);

	m_pUISkillTreeDlg->Init(s_pUIMgr);
	m_pUISkillTreeDlg->LoadFromFile(pTbl->szSkillTree);
	m_pUISkillTreeDlg->SetVisibleWithNoSound(false);
	rc = m_pUISkillTreeDlg->GetRegion();
	m_pUISkillTreeDlg->SetPos(iW - (rc.right-rc.left), 10);
	m_pUISkillTreeDlg->InitIconWnd(UIWND_SKILL_TREE);
	m_pUISkillTreeDlg->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUISkillTreeDlg->SetState(UI_STATE_COMMON_NONE);
	m_pUISkillTreeDlg->SetStyle(m_pUISkillTreeDlg->GetStyle() | UISTYLE_POS_RIGHT);

#if __VERSION > 1098
	m_pUICmdListDlg->Init(s_pUIMgr);
	m_pUICmdListDlg->LoadFromFile(pTbl->szCmdList);
	m_pUICmdListDlg->SetVisibleWithNoSound(false);
	rc = m_pUICmdListDlg->GetRegion();
	m_pUICmdListDlg->SetPos(iW - (rc.right - rc.left), 10);
	m_pUICmdListDlg->SetUIType(UI_TYPE_BASE);
	m_pUICmdListDlg->SetState(UI_STATE_COMMON_NONE);
	m_pUICmdListDlg->SetStyle(m_pUISkillTreeDlg->GetStyle() | UISTYLE_POS_RIGHT);

	m_pUICmdEditDlg->Init(s_pUIMgr);
	m_pUICmdEditDlg->LoadFromFile(pTbl->szCmdEdit);
	m_pUICmdEditDlg->SetVisibleWithNoSound(false);
	rc = m_pUICmdEditDlg->GetRegion();
	iX = (iW - (rc.right - rc.left)) / 2;
	iY = (iH - (rc.bottom - rc.top)) / 2;
	m_pUICmdEditDlg->SetPos(iX, iY);
	m_pUICmdEditDlg->SetStyle(UISTYLE_USER_MOVE_HIDE);
#endif
	
	// default ui pos ..	???????? ????????.. ???? ?????? ?????? ????.. by ecli666
	rc = m_pUIStateBarAndMiniMap->GetRegion();
	m_pUIHotKeyDlg->Init(s_pUIMgr);
	m_pUIHotKeyDlg->LoadFromFile(pTbl->szHotKey);
	m_pUIHotKeyDlg->SetStyle(UISTYLE_HIDE_UNABLE);
	CGameProcedure::UIPostData_Read(UI_POST_WND_HOTKEY, m_pUIHotKeyDlg, rc.left, rc.bottom);
	m_pUIHotKeyDlg->SetVisibleWithNoSound(true); // ?????? ??????!!!
	m_pUIHotKeyDlg->InitIconWnd(UIWND_HOTKEY);
	m_pUIHotKeyDlg->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUIHotKeyDlg->SetState(UI_STATE_COMMON_NONE);

	m_pUIKnightsOp->Init(s_pUIMgr);	// ?????? ?????? ????, ????, ??...
	m_pUIKnightsOp->LoadFromFile(pTbl->szKnightsOperation);
	m_pUIKnightsOp->SetVisibleWithNoSound(false);

	// ???? ???? ??????..
	m_pUIPartyBBS->Init(s_pUIMgr);
	m_pUIPartyBBS->LoadFromFile(pTbl->szPartyBBS);//, N3FORMAT_VER_1298);
	m_pUIPartyBBS->SetVisibleWithNoSound(false);
	rc = m_pUIPartyBBS->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIPartyBBS->SetPos(iX, iY);


	m_pUIWareHouseDlg->Init(s_pUIMgr);
	m_pUIWareHouseDlg->LoadFromFile(pTbl->szWareHouse);
	rc = m_pUIWareHouseDlg->GetRegion();
	m_pUIWareHouseDlg->SetPos(iW - (rc.right-rc.left), 10);
	m_pUIWareHouseDlg->SetVisibleWithNoSound(false);
	m_pUIWareHouseDlg->InitIconWnd(UIWND_WARE_HOUSE);
	m_pUIWareHouseDlg->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUIWareHouseDlg->SetState(UI_STATE_COMMON_NONE);
	m_pUIWareHouseDlg->SetStyle(UISTYLE_USER_MOVE_HIDE | UISTYLE_POS_RIGHT);

	m_pTargetSymbol->LoadFromFile(pTbl->szTargetSymbolShape); // ?????????? ???????? ???? ???????? ???????? ?????? ????..

	m_pUIInn->Init(s_pUIMgr);
	m_pUIInn->LoadFromFile(pTbl->szInn);
	m_pUIInn->SetVisibleWithNoSound(false);
	m_pUIInn->SetStyle(UISTYLE_SHOW_ME_ALONE | UISTYLE_USER_MOVE_HIDE);
	rc = m_pUIInn->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIInn->SetPos(iX, iY);
	
	m_pUICreateClanName->Init(s_pUIMgr);
	m_pUICreateClanName->LoadFromFile(pTbl->szInputClanName);
	m_pUICreateClanName->SetVisibleWithNoSound(false);
	rc = m_pUICreateClanName->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUICreateClanName->SetPos(iX, iY);
	m_pUICreateClanName->SetStyle(UISTYLE_USER_MOVE_HIDE);

	// Quest Menu
	m_pUIQuestMenu->Init(s_pUIMgr);
	m_pUIQuestMenu->LoadFromFile(pTbl->szQuestMenu);
	m_pUIQuestMenu->SetVisibleWithNoSound(false);
	m_pUIQuestMenu->SetStyle(UISTYLE_USER_MOVE_HIDE);

	// Quest Talk
	m_pUIQuestTalk->Init(s_pUIMgr);
	m_pUIQuestTalk->LoadFromFile(pTbl->szQuestTalk);
	m_pUIQuestTalk->SetVisibleWithNoSound(false);
	m_pUIQuestTalk->SetStyle(UISTYLE_USER_MOVE_HIDE);
	rc = m_pUIQuestTalk->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIQuestTalk->SetPos(iX, iY);

	// dead ui
	m_pUIDead->Init(s_pUIMgr);
	m_pUIDead->LoadFromFile(pTbl->szDead);
	m_pUIDead->SetVisibleWithNoSound(false);
	rc = m_pUIDead->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIDead->SetPos(iX, iY);


	// ?????? ??????
	m_pUITradeBBS->Init(s_pUIMgr);
	m_pUITradeBBS->LoadFromFile(pTbl->szTradeBBS);
	m_pUITradeBBS->SetVisibleWithNoSound(false);
	m_pUITradeBBS->SetStyle(UISTYLE_USER_MOVE_HIDE);
	rc = m_pUITradeBBS->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUITradeBBS->SetPos(iX, iY);

	// ?????? ???? ???? 
	m_pUITradeBBSSelector->Init(s_pUIMgr);
	m_pUITradeBBSSelector->LoadFromFile(pTbl->szTradeBBSSelector);
	m_pUITradeBBSSelector->SetVisibleWithNoSound(false);
	rc = m_pUITradeBBSSelector->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUITradeBBSSelector->SetPos(iX, iY);
	m_pUITradeBBSSelector->SetStyle(UISTYLE_USER_MOVE_HIDE);

	// ?????? ???? ????
	m_pUITradeBBSEdit->LoadFromFile(pTbl->szTradeExplanation);
	m_pUITradeBBSEdit->SetVisibleWithNoSound(false);
	rc = m_pUITradeBBSEdit->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUITradeBBSEdit->SetPos(iX, iY);
}

void CGameProcMain::MsgSend_RequestTargetHP(int16_t siIDTarget, uint8_t byUpdateImmediately)
{
	uint8_t byBuff[4];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_TARGET_HP);
	CAPISocket::MP_AddShort(byBuff, iOffset, siIDTarget);
	CAPISocket::MP_AddByte(byBuff, iOffset, byUpdateImmediately); // 0x00 - ???? ??????????.. 0x01 - ???? ????????..

	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgRecv_TargetHP(Packet& pkt)
{
	int iID				= pkt.read<int16_t>();	// SID
	uint8_t byUpdateImmediately = pkt.read<uint8_t>();	// 0x00 - ???? ??????????.. 0x01 - ???? ????????..
	int iTargetHPMax	= pkt.read<uint32_t>();  // HP 
	int iTargetHPCur	= pkt.read<uint32_t>();  // HP 
	int iTargetHPChange	= pkt.read<int16_t>();  // HP 

	if(iTargetHPMax <= 0)
	{
		CPlayerNPC* pTarget = this->s_pOPMgr->CharacterGetByID(iID, true);
		std::string szBuff("Invalid HP Change - 0 ID(");
		if(pTarget) szBuff += pTarget->IDString();
		szBuff += ')';
		CLogWriter::Write(szBuff.c_str());
	}
	__ASSERT(iTargetHPMax > 0, "???? ???? ?????? 0?????????? ??????.");

	if(iID == s_pPlayer->m_iIDTarget) // ???? ???????? ??????..
	{
		bool bUI = false;
		if(byUpdateImmediately) bUI = true;
		m_pUITargetBar->UpdateHP(iTargetHPCur, iTargetHPMax, bUI);
	//	TRACE("Target HP Update %d %d\n", iID, iTargetHPPercent);
	}

	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(iID, true);
	if(pTarget)
	{
		pTarget->m_InfoBase.iHP = iTargetHPCur;
		pTarget->m_InfoBase.iHPMax = iTargetHPMax;

		char szBuf[256] = "";
		if(iTargetHPChange < 0)
		{
			std::string szFmt; ::_LoadStringFromResource(IDS_MSG_FMT_TARGET_HP_LOST, szFmt);
			sprintf(szBuf, szFmt.c_str(), pTarget->IDString().c_str(), -iTargetHPChange);
			MsgOutput(szBuf, 0xffffffff);
		}
		else if(iTargetHPChange > 0)
		{
			std::string szFmt; ::_LoadStringFromResource(IDS_MSG_FMT_TARGET_HP_RECOVER, szFmt);
			sprintf(szBuf, szFmt.c_str(), pTarget->IDString().c_str(), iTargetHPChange);
			MsgOutput(szBuf, 0xff6565ff);
		}
	}
}

// ??????..................
bool CGameProcMain::MsgSend_NPCEvent(int16_t siIDTarget)
{
	uint8_t byBuff[4];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_NPC_EVENT);
	CAPISocket::MP_AddShort(byBuff, iOffset, siIDTarget);

	s_pSocket->Send(byBuff, iOffset);

	return true;
}

void CGameProcMain::MsgSend_NPCInRequest(int iID) // NPC ?????? ???? ???? ????????..
{
	//TRACE("found NPC ghost (ID:%d)\n", iID);

	int iOffset=0;													// ?????? ??????..
	uint8_t byBuff[32];;												// ???? ????..
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_REQ_NPCIN);		// ??????.
	CAPISocket::MP_AddShort(byBuff, iOffset, 1);					// ?????? ????..
	CAPISocket::MP_AddShort(byBuff, iOffset, iID);					// ?????? ?????? ?????? ????????..

	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_UserInRequest(int iID) // User ?????? ???? ???? ????????..
{
	//TRACE("found User ghost (ID:%d)\n", iID);

	int iOffset=0;													// ?????? ??????..
	uint8_t byBuff[32];;												// ???? ????..
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_REQ_USERIN);	// ??????.
	CAPISocket::MP_AddShort(byBuff, iOffset, 1);					// ?????? ????..
	CAPISocket::MP_AddShort(byBuff, iOffset, iID);					// ?????? ?????? ?????? ????????..

	s_pSocket->Send(byBuff, iOffset);
}


void CGameProcMain::MsgSend_Warp() // ???? - ???????? ?????? ????..
{	
	__WarpInfo WI;
	if (!m_pUIWarp->InfoGetCur(WI)
		|| WI.szName.empty())
		return;

	uint8_t byBuff[8];
	int iOffset = 0;
	
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_WARP_LIST);
	CAPISocket::MP_AddShort(byBuff, iOffset, WI.iID); // ???? ?????? ??????...
	s_pSocket->Send(byBuff, iOffset);

	/*
	__Vector3 vec3;
	vec3.x = 361.278503f;
	vec3.y = 2.822370f;
	vec3.z = 137.339859f;

	InitZone(WI.iZone, vec3);
	*/
}

void CGameProcMain::DoCommercialTransaction(int iTradeID)
{
	if (m_pUITransactionDlg->IsVisible())
		return;

	m_pUITransactionDlg->m_iTradeID = iTradeID;

	if ( !m_pUITransactionDlg->IsVisible() )
		m_pUITransactionDlg->SetVisible(true);

	if (m_pUIInventory->IsVisible()) // ?????????? ?????? ??????..
		this->CommandToggleUIInventory();

	if (m_pUISkillTreeDlg->IsVisible())
		m_pUISkillTreeDlg->Close();

	m_pUITransactionDlg->EnterTransactionState();
}

bool CGameProcMain::MsgRecv_ItemTradeStart(Packet& pkt)			// ?????? ??????..
{
	int iTradeID = pkt.read<uint32_t>();		// Trade id
	CPlayerNPC* pNPC = s_pOPMgr->NPCGetByID(s_pPlayer->m_iIDTarget, true);
	if (!pNPC)	return true;
	m_pUINpcEvent->Open(NPC_EVENT_ITEM_TRADE, iTradeID, pNPC->GetNPCOriginID());

	return true;
}

bool CGameProcMain::MsgRecv_ItemTradeResult(Packet& pkt)			// ?????? ?????? ????..
{
	byte bfType = 0x00;	int	iMoney = 0;
	byte bResult = pkt.read<uint8_t>();		// Trade id

	switch ( bResult )
	{
		case 0x00:
			bfType = pkt.read<uint8_t>();		// Trade id
			m_pUITransactionDlg->ReceiveResultTradeFromServer( bResult, bfType, iMoney );
			break;

		case 0x01:
			iMoney = pkt.read<uint32_t>();		// Trade id
			m_pUITransactionDlg->ReceiveResultTradeFromServer( bResult, bfType, iMoney );
			break;

		case 0x03:
			m_pUITransactionDlg->ReceiveResultTradeMoveSuccess();
			break;

		case 0x04:
			m_pUITransactionDlg->ReceiveResultTradeMoveFail();
			break;
	}
	
	return true;
}

void CGameProcMain::InitZone(int iZone, const __Vector3& vPosPlayer)
{
	if(m_pSnd_Battle) m_pSnd_Battle->Stop(0.0f); // ???? ??????..
	if(m_pSnd_Town) m_pSnd_Town->Stop(0.0f);

	MsgSend_SpeedCheck(true);		// ???????? ???? ???????? ??????
	static int iZonePrev = -1;
	if(iZonePrev != iZone) // ???? ???? ???????? ????..
	{
		m_pLightMgr->Release();
		s_pEng->SetDefaultLight(m_pLightMgr->Light(0), m_pLightMgr->Light(1), m_pLightMgr->Light(2));

		if(m_pUIChatDlg) m_pUIChatDlg->ChangeChattingMode(N3_CHAT_NORMAL); //?? ?????? ???? ???? ?????????? ????????.
		if(m_pUIPartyOrForce) m_pUIPartyOrForce->MemberDestroy();	//?? ?????? ???? ?????? ????...
		this->UpdateUI_PartyOrForceButtons(); // ?????? ???? ???? ???? ?????? ?????? ???? ???????? ??????.

		s_pPlayer->m_bMoveContinous = true; // ??????..
		this->CommandToggleMoveContinous();

		CLogWriter::Write("CGameProcMain::InitZone -> Zone Change (%d -> %d) Position(%.1f, %.1f, %.1f)", iZonePrev, iZone, vPosPlayer.x, vPosPlayer.y, vPosPlayer.z);

		m_bLoadComplete = false; // ???? ????..
		CLogWriter::Write("%d->ClearDurationalMagic()",m_pMagicSkillMng); // TmpLog1122
		m_pMagicSkillMng->ClearDurationalMagic();
		CLogWriter::Write("%d->ClearAll()", s_pFX); // TmpLog1122
		s_pFX->ClearAll();

		if(s_pUILoading)
		{
			CLogWriter::Write("s_pUILoading->Render()"); // TmpLog1122
			s_pUILoading->Render("", 0);
		}
		
		s_pPlayer->m_InfoExt.iZoneCur = iZone;
		iZonePrev = iZone; // ?????? ???? ?? ?????? ??????????.

		CLogWriter::Write("%d->Find(s_pPlayer->m_InfoExt.iZoneCur)",s_pTbl_Zones); // TmpLog1122
		__TABLE_ZONE* pZoneData = s_pTbl_Zones.Find(s_pPlayer->m_InfoExt.iZoneCur);
		if(NULL == pZoneData) {
			CLogWriter::Write("can't find zone data. (zone : %d)", s_pPlayer->m_InfoExt.iZoneCur);
			__ASSERT(0, "Zone Data Not Found!");
			system("pause");
			return;
		}

		CLogWriter::Write("%d->Release()",s_pOPMgr); // TmpLog1122

		s_pOPMgr->Release(); // ???? ???? ?? ??????..
		CLogWriter::Write("%d->InitWorld()",s_pWorldMgr); // TmpLog1122
		s_pWorldMgr->InitWorld(iZone, vPosPlayer);

		// ?????? ????..
		CLogWriter::Write("%d->GetWidthByMeterWithTerrain()",ACT_WORLD); // TmpLog1122
		float fWidth = ACT_WORLD->GetWidthByMeterWithTerrain();
		CLogWriter::Write("%d->LoadMap()",m_pUIStateBarAndMiniMap); // TmpLog1122
		CLogWriter::Write("%d->szMiniMapFNszMiniMapFN",pZoneData); // TmpLog1122
		m_pUIStateBarAndMiniMap->LoadMap(pZoneData->szMiniMapFN, fWidth, fWidth);

		CLogWriter::Write("GetRepresentClass()"); // TmpLog1122
		// ?? ???? ??????..
		float fZoom = 6.0f;
		e_Class_Represent eCR = CGameProcedure::GetRepresentClass(s_pPlayer->m_InfoBase.eClass);
		if(CLASS_REPRESENT_ROGUE == eCR) fZoom = 3.0f; // ???? ?????? ???? ???? ???? ?????? ??????..
		CLogWriter::Write("%d->ZoomSet()",m_pUIStateBarAndMiniMap); // TmpLog1122
		m_pUIStateBarAndMiniMap->ZoomSet(fZoom);

		CLogWriter::Write("%d->szTerrainFN.c_str()",pZoneData); // TmpLog1122
		//char szBuf[256];
		char szFName[_MAX_PATH];
		_splitpath(pZoneData->szTerrainFN.c_str(), NULL, NULL, szFName, NULL);
		char szFName2[_MAX_PATH];
		char szFullPathName[_MAX_PATH];
		sprintf(szFName2,"%s_Bird",szFName);
		_makepath(szFullPathName, NULL, "misc\\bird", szFName2, "lst");

		m_pLightMgr->LoadZoneLight(pZoneData->szLightObjFN.c_str());

		m_bLoadComplete = true; // ???? ????..
	}
		
	// ?????? ????..
	CN3Camera* pCamera		= s_pEng->CameraGetActive();		// ???????? ?????? ????..
	if(pCamera)
	{
		__Vector3 vPosPlayer = s_pPlayer->Position();
		pCamera->m_Data.fFOV	= D3DXToRadian(70);				// Field of View ..
		pCamera->m_Data.fFP		= 512.0f;						// Far Plane..
		pCamera->m_Data.fNP		= 0.5f;							// Near Plane..
		CLogWriter::Write("pCamera->LookAt()"); // TmpLog1122
		pCamera->LookAt(vPosPlayer + __Vector3(0,0,-1), vPosPlayer, __Vector3(0,1,0));
		CLogWriter::Write("pCamera->Tick()"); // TmpLog1122
		pCamera->Tick();
		CLogWriter::Write("pCamera->Apply()"); // TmpLog1122
		pCamera->Apply();
	}
	// ???????? ???????????? ?????? ???? ????..
	////////////////////////////////////////////////////////////////////////////////

	CLogWriter::Write("InitPlayerPosition() Position(%.1f, %.1f, %.1f)",vPosPlayer.x, vPosPlayer.y, vPosPlayer.z); // TmpLog1122
	this->InitPlayerPosition(vPosPlayer); // ???????? ???? ??????.. ?????? ??????, ?????????? ?????? ????.
	CLogWriter::Write("%d->Release()",s_pOPMgr); // TmpLog1122
	s_pOPMgr->Release(); // ???? ???????? ????...
}


void CGameProcMain::MsgSend_GameStart()
{
	uint8_t byBuff[32];															// ???? ????..
	int iOffset=0;															// ???? ??????..

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_GAMESTART);						// ???? ?????? ???? ??????..
	//CAPISocket::MP_AddByte(byBuff, iOffset, s_pPlayer->IDString().size());		// ?????? ???? ?????? ????..
	//CAPISocket::MP_AddString(byBuff, iOffset, s_pPlayer->IDString());			// ?????? ?????? ?????? ????..

	// NOTE(srmeier): start the first part of the login process
	CAPISocket::MP_AddByte(byBuff, iOffset, 0x01);

	s_pSocket->Send(byBuff, iOffset);
}

bool CGameProcMain::CommandToggleWalkRun()
{
	bool bRun = s_pPlayer->ToggleRunMode();
	bool bMove = s_pPlayer->m_bMoveContinous;

	// ?????? ???? ????????..
	if(m_pUICmd->m_pBtn_Act_Walk)
	{
		m_pUICmd->m_pBtn_Act_Walk->SetVisible(bRun);
		if(bMove) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_DOWN);
		else m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);
	}
	if(m_pUICmd->m_pBtn_Act_Run)
	{
		m_pUICmd->m_pBtn_Act_Run->SetVisible(!bRun);
		if(bMove) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_DOWN);
		else m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
	}

	return bRun;
}

bool CGameProcMain::CommandToggleAttackContinous()
{
	CPlayerBase* pTarget = s_pPlayer->TargetPointerCheck(false);
	this->CommandEnableAttackContinous(!s_pPlayer->m_bAttackContinous, pTarget);

	return s_pPlayer->m_bAttackContinous;
}

bool CGameProcMain::CommandToggleMoveContinous()
{
	if(s_pPlayer->IsDead()) return false;

	s_pPlayer->ToggleMoveMode();				// ???? ???? ????.. 
	if(s_pPlayer->m_bMoveContinous)
	{
		this->CommandMove(MD_FOWARD, true);
		if(m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_DOWN);
		if(m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_DOWN);
	}
	else
	{
		this->CommandMove(MD_STOP, true); // ?????? ???? ??????..
		if(m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
		if(m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);
	}

	return s_pPlayer->m_bMoveContinous;
}

void CGameProcMain::CommandMove(e_MoveDirection eMD, bool bStartOrEnd)
{
	if(bStartOrEnd) this->CloseUIs(); // ???? ??????, ????????... UI ????..

	if(s_pPlayer->IsDead()) return; // ???? ???? ???? ????!!

	if(MD_FOWARD == eMD || MD_BACKWARD == eMD)
	{
		s_pUIMgr->UserMoveHideUIs();
		this->CommandSitDown(false, false, true); // ?????? ??????..
		if(s_pPlayer->m_bStun) return; // ?????? ???? ???????? ????..
		if(MD_FOWARD == eMD)
		{
			if(s_pPlayer->IsRunning()) s_pPlayer->ActionMove(PSM_RUN); // ????????..
			else s_pPlayer->ActionMove(PSM_WALK); // ????????..
		}
		else
		{
			s_pPlayer->ActionMove(PSM_WALK_BACKWARD); // ????..
		}

		if( bStartOrEnd ) // ???????? ???????????? ?????????? ???? ??????..
		{
			float fSpeed = s_pPlayer->MoveSpeedCalculationAndCheckCollision(); // ?????? ?????? ?? ?????? ???? ?????? ????. ???????? 0 ???? ????????..
			if(0 == fSpeed) // ?????????? ????????..
			{
				s_pPlayer->ActionMove(PSM_STOP); // ????..
			}
			else
			{
				this->MsgSend_Move(true, false); // ???????? ????????.. ?????? ???? ???? ??????.
			}

			if(m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_DOWN);
			if(m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_DOWN);
		}
	}
	else if(MD_STOP == eMD)
	{
		s_pPlayer->m_bMoveContinous = false; // ???? ???? ????..
		s_pPlayer->ActionMove(PSM_STOP);
		
		if(m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
		if(m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);

		this->MsgSend_Move(false, false); // ?????? ???? ???? ??????.
	}

	if(s_pPlayer->m_bAttackContinous) // ??????????..
	{
		CPlayerBase* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);
		if(s_pPlayer->IsAttackableTarget(pTarget)) // ???? ????????..
			s_pPlayer->Action(PSA_ATTACK, false, pTarget); // ????
	}
}

void CGameProcMain::CommandEnableAttackContinous(bool bEnable, CPlayerBase* pTarget)
{
	if(bEnable == s_pPlayer->m_bAttackContinous) return;
	if(bEnable)
	{
		this->CloseUIs(); // ???? ??????, ????????... UI ????..
		s_pUIMgr->UserMoveHideUIs();

		if(s_pPlayer->m_bStun) return; // ?????? ???? ???? ????..
		if(NULL == pTarget) return;
		s_pPlayer->RotateTo(pTarget); // ?????? ??????.
		if(pTarget->m_InfoBase.eNation == s_pPlayer->m_InfoBase.eNation) return; // ?????? ?????? ????????..

		//-------------------------------------------------------------------------
		/*
		// TODO(srmeier): need to use ZoneAbilityType here
		// NOTE(srmeier): using zoneability information to determine if target is attackable
		if (!ACT_WORLD->canAttackSameNation() && (pTarget->m_InfoBase.eNation == s_pPlayer->m_InfoBase.eNation))
			return;
		if (!ACT_WORLD->canAttackOtherNation() && (s_pPlayer->m_InfoBase.eNation == NATION_ELMORAD && pTarget->m_InfoBase.eNation == NATION_KARUS))
			return;
		if (!ACT_WORLD->canAttackOtherNation() && (s_pPlayer->m_InfoBase.eNation == NATION_KARUS && pTarget->m_InfoBase.eNation == NATION_ELMORAD))
			return;
		*/
		//-------------------------------------------------------------------------
	}
	s_pPlayer->m_bAttackContinous = bEnable; // ?????? ????????..

	if(bEnable)
		SetGameCursor(s_hCursorAttack);
	else
	{
		e_Nation eNation = s_pPlayer->m_InfoBase.eNation;
		SetGameCursor(((NATION_ELMORAD == eNation) ? s_hCursorNormal1 : s_hCursorNormal));
	}

	if(m_pUICmd->m_pBtn_Act_Attack)
	{
		if(bEnable) m_pUICmd->m_pBtn_Act_Attack->SetState(UI_STATE_BUTTON_DOWN);
		else m_pUICmd->m_pBtn_Act_Attack->SetState(UI_STATE_BUTTON_NORMAL);
	}

	if(	bEnable ) // ???? ????!
	{
		char szMsg[128] = { 0 };
		std::string szFmt;
		::_LoadStringFromResource(IDS_MSG_ATTACK_START, szFmt);
		sprintf(szMsg, szFmt.c_str(), pTarget->IDString().c_str());

		this->PlayBGM_Battle();
		
		if(s_pPlayer->IsAttackableTarget(pTarget))
			s_pPlayer->Action(PSA_BASIC, true, pTarget);

		this->MsgOutput(szMsg, 0xff00ffff);
	}
	else // ???? ???? ????.
	{
		std::string szMsg;
		::_LoadStringFromResource(IDS_MSG_ATTACK_STOP, szMsg);
		s_pPlayer->Action(PSA_BASIC, true, pTarget);
		this->MsgOutput(szMsg, 0xff00ffff);
	}

	if(	bEnable && false == s_pPlayer->IsAttackableTarget(pTarget)) // ????, ???? ?? ???? ???????? ???? ?????????? ????????..
	{
		std::string szMsg; ::_LoadStringFromResource(IDS_MSG_ATTACK_DISABLE, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
	}
}

void CGameProcMain::CommandToggleUIChat()
{
	bool visible = m_pUIChatDlg->IsVisible();

	m_pUIChatDlg->SetVisibleWithNoSound(!visible);
	m_pUIChatDlg2->SetVisibleWithNoSound(visible);
}

void CGameProcMain::CommandToggleUIMsgWnd()
{
	bool visible = m_pUIMsgDlg->IsVisible();

	m_pUIMsgDlg->SetVisibleWithNoSound(!visible);
	m_pUIMsgDlg2->SetVisibleWithNoSound(visible);
}

bool CGameProcMain::CommandToggleUIState()
{
	bool bNeedOpen = !(m_pUIVar->IsVisible());
	if(bNeedOpen)
	{
		s_pUIMgr->SetFocusedUI(m_pUIVar);
		m_pUIVar->Open();
	}
	else m_pUIVar->Close();

	// ?????? ???? ????????..
//	if(m_pUICmd->m_pBtn_Character)
//	{
//		if(bNeedOpen) m_pUICmd->m_pBtn_Character->SetState(UI_STATE_BUTTON_DOWN); // ???? ???? ????..
//		else m_pUICmd->m_pBtn_Character->SetState(UI_STATE_BUTTON_NORMAL); // ???? ????
//	}

	return bNeedOpen;
}

bool CGameProcMain::CommandToggleUIInventory()
{
	bool bNeedOpen = false;

	// ???? ??????????..
	if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE)
		return bNeedOpen;

	if (m_pUINpcEvent->IsVisible())
		return bNeedOpen;

	if( m_pUIInn->IsVisible()) return bNeedOpen;
	if( m_pUICreateClanName->IsVisible()) return bNeedOpen;

	if (m_pUIWareHouseDlg->IsVisible())
	{
		if (m_pUIInventory->IsVisible())
			m_pUIInventory->Close(true);
		return bNeedOpen;
	}

	if (m_pUITransactionDlg->IsVisible())
	{
		if (m_pUIInventory->IsVisible())
			m_pUIInventory->Close(true);
		return bNeedOpen;
	}
	
	if (m_pUIInventory->IsVisible())
	{
		m_pUIInventory->Close(true);
		return bNeedOpen;
	}
	else
	{
		bNeedOpen = true;
		if(m_pUISkillTreeDlg->IsVisible()) m_pUISkillTreeDlg->Close();
		
		s_pUIMgr->SetFocusedUI(m_pUIInventory);
		m_pUIInventory->Open();
	}

	return bNeedOpen;
}

bool CGameProcMain::CommandToggleUISkillTree()
{
	bool bNeedOpen = !(m_pUISkillTreeDlg->IsVisible());

	// ???? ??????????..
	if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE)
		return bNeedOpen;

	if (bNeedOpen)
	{
		if (m_pUIInventory->IsVisible())
			m_pUIInventory->Close();
		if (m_pUITransactionDlg->IsVisible())
			m_pUITransactionDlg->LeaveTransactionState();
		if (m_pUIWareHouseDlg->IsVisible())
			m_pUIWareHouseDlg->LeaveWareHouseState();

		s_pUIMgr->SetFocusedUI(m_pUISkillTreeDlg);
		m_pUISkillTreeDlg->Open();
	}
	else
	{
		m_pUISkillTreeDlg->Close();
	}

	return bNeedOpen;
}

bool CGameProcMain::CommandToggleUIMiniMap()
{
	return m_pUIStateBarAndMiniMap->ToggleMiniMap();
}

bool CGameProcMain::CommandToggleCmdList()
{
	
	bool bNeedOpen = !(m_pUICmdListDlg->IsVisible());

	if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE)
		return bNeedOpen;

	if (bNeedOpen)
	{
		if (m_pUIInventory->IsVisible())
			m_pUIInventory->Close();
		if (m_pUITransactionDlg->IsVisible())
			m_pUITransactionDlg->LeaveTransactionState();
		if (m_pUIWareHouseDlg->IsVisible())
			m_pUIWareHouseDlg->LeaveWareHouseState();

		s_pUIMgr->SetFocusedUI(m_pUICmdListDlg);
		m_pUICmdListDlg->Open();
	}
	else
	{
		m_pUICmdListDlg->Close();
	}

	return bNeedOpen;
}

bool CGameProcMain::OpenCmdEdit(std::string msg)
{

	bool bNeedOpen = !(m_pUICmdEditDlg->IsVisible());

	if (bNeedOpen)
	{
		s_pUIMgr->SetFocusedUI(m_pUICmdEditDlg);
		m_pUICmdEditDlg->Open(msg);
	}

	return bNeedOpen;
}

void CGameProcMain::CommandCameraChange() // ?????? ???? ??????..
{
	if(VP_THIRD_PERSON == s_pEng->ViewPoint() && s_pPlayer->m_bTargetOrPosMove)
	{
		s_pPlayer->m_bMoveContinous = false;
		s_pPlayer->m_bTargetOrPosMove = false;
		CommandToggleMoveContinous();
	}

	s_pEng->ViewPointChange(VP_UNKNOWN); // ???????? ?????? ??????..
}

void CGameProcMain::MsgOutput(const std::string& szMsg, D3DCOLOR crMsg)
{
	m_pUIMsgDlg->AddMsg(szMsg, crMsg);
}

bool CGameProcMain::MsgRecv_ItemDroppedGetResult(Packet& pkt)	// ???? ?????? ?????? ???? ????..
{
	uint8_t	bResult; 
	uint8_t	bPos = 0;
	int		iItemID = 0;
	int		iGoldID = 0;
	int		iStrLen = 0;
	int16_t	sItemCount = 0;
	std::string szString;

	bResult = pkt.read<uint8_t>();
	if ( (bResult == 0x01) || (bResult == 0x02) || (bResult == 0x05) )
	{
		bPos	= pkt.read<uint8_t>();
		iItemID = pkt.read<uint32_t>();
			if ( (bResult == 0x01) || (bResult == 0x05) )
			{
				sItemCount = pkt.read<int16_t>();
			}
		iGoldID = pkt.read<uint32_t>();
	}


	if (bResult == 0x03)
	{
		iItemID = pkt.read<uint32_t>();
		iStrLen = (int)pkt.read<int16_t>();
		pkt.readString(szString, iStrLen);
	}

	//TRACE("???? - Item Get %d %d\n", bResult, iGoldID);

	if ( m_pUIDroppedItemDlg )
		m_pUIDroppedItemDlg->GetItemByIDToInventory(bResult, iItemID, iGoldID, bPos, sItemCount, iStrLen, szString);
	return true;
}

void CGameProcMain::MsgRecv_ZoneChange(Packet& pkt)
{
	uint8_t ZoneChangeFlag = pkt.read<uint8_t>();

	switch (ZoneChangeFlag) {

		case ZoneChangeTeleport: {
			int iZone = -1;
			if(N3FORMAT_VER_DEFAULT & N3FORMAT_VER_1264) {
				iZone = 10 * pkt.read<int16_t>();
			} else {
				iZone = pkt.read<int16_t>();
			}

			float fX = (pkt.read<uint16_t>()) / 10.0f;
			float fZ = (pkt.read<uint16_t>()) / 10.0f;
			float fY = (pkt.read<int16_t>()) / 10.0f;
			int iVictoryNation = pkt.read<uint8_t>();
			CGameProcedure::LoadingUIChange(iVictoryNation);


			__Vector3 vPosPlayer;
			vPosPlayer.x = fX;
			vPosPlayer.y = fY;
			vPosPlayer.z = fZ;
			this->InitPlayerPosition(vPosPlayer); // ???????? ???? ??????.. ?????? ??????, ?????????? ?????? ????.
			s_pPlayer->RegenerateCollisionMesh(); // ???? ?????? ???? ??????..
			s_pPlayer->m_iSendRegeneration = 0; // ???? ?????? ???? ?????????? ???????? ??????
			s_pPlayer->m_fTimeAfterDeath = 0; // ???? ?????? ???? ?????????? ???????? ??????

			if (s_pPlayer->IsDead())
			{
				//TRACE("ZoneChange - ???? ????????(%.1f, %.1f)\n", fX, fZ);

				//???? & ???? ??????..
				if (m_pUIStateBarAndMiniMap) m_pUIStateBarAndMiniMap->ClearMagic();
				if (m_pMagicSkillMng) m_pMagicSkillMng->ClearDurationalMagic();
				if (CGameProcedure::s_pFX) s_pFX->StopMine();

				if (s_pPlayer->Nation() == NATION_KARUS) CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_KARUS, s_pPlayer->IDNumber(), -1);
				else if (s_pPlayer->Nation() == NATION_ELMORAD) CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_ELMORAD, s_pPlayer->IDNumber(), -1);
			}

			this->InitZone(iZone, __Vector3(fX, fY, fZ)); // Zone Update

			uint8_t byBuff[4];
			int iOffset_send = 0;
			CAPISocket::MP_AddByte(byBuff, iOffset_send, WIZ_ZONE_CHANGE);
			CAPISocket::MP_AddByte(byBuff, iOffset_send, (Uint8)ZoneChangeLoading);
			s_pSocket->Send(byBuff, iOffset_send);
		} break;

		case ZoneChangeLoaded: {
			uint8_t byBuff[4];
			int iOffset_send = 0;
			CAPISocket::MP_AddByte(byBuff, iOffset_send, WIZ_ZONE_CHANGE);
			CAPISocket::MP_AddByte(byBuff, iOffset_send, (Uint8)ZoneChangeLoaded);
			s_pSocket->Send(byBuff, iOffset_send);
		} break;

	}
}

void CGameProcMain::MsgRecv_UserState(Packet& pkt)
{
	int iID = pkt.read<int16_t>();
	e_SubPacket_State eSP = (e_SubPacket_State)pkt.read<uint8_t>(); // 0x01
	int iState = pkt.read<uint32_t>();//int iState = pkt.read<uint8_t>();

	CPlayerBase* pBPC = NULL;
	if ( s_pPlayer->IDNumber() == iID )
		pBPC = s_pPlayer;
	else
		pBPC = s_pOPMgr->UPCGetByID(iID, false); 
	
	if(NULL == pBPC) return;

	if(N3_SP_STATE_CHANGE_SITDOWN == eSP) // ????,????,????...
	{
		if(pBPC != s_pPlayer) // ?????????? ??????..
		{
			if(0x01 == iState) 
				pBPC->Action(PSA_BASIC, true);
			else if(0x02 == iState) 
				pBPC->Action(PSA_SITDOWN, true);
			else if(0x03 == iState) 
				pBPC->Action(PSA_DYING, false, NULL, true);
		}
	}
	else if(N3_SP_STATE_CHANGE_RECRUIT_PARTY == eSP) // ???? ????..
	{
		if(0x01 == iState) 
		{
			pBPC->InfoStringSet("", 0);
		}
		else if(0x02 == iState) 
		{
			int iLevel = pBPC->m_InfoBase.iLevel;
			int iLMin = iLevel - 8;
			if(iLMin < 0) iLMin = 0;
			int iLMax = iLevel + 8;
			if(iLMax > 60) iLMax = 60;

			char szBuff[128];
			std::string szMsg;
			::_LoadStringFromResource(IDS_WANT_PARTY_MEMBER, szMsg);
			sprintf(szBuff, szMsg.c_str(), iLMin, iLMax);
			pBPC->InfoStringSet(szBuff, 0xff00ff00);
		}
	}
	else if(N3_SP_STATE_CHANGE_SIZE == eSP) // ???? ????
	{
		if(0x01 == iState)
		{
			pBPC->ScaleSetGradually(1.0f); // ???? ????..
			pBPC->FlickerFactorSet(1.0f); // ???????? ??????.. ??????????..
		}
		else if(0x02 == iState) pBPC->ScaleSetGradually(2.0f); // ??????..
		else if(0x03 == iState) pBPC->ScaleSetGradually(0.5f);// ????????.
		else if(0x04 == iState) pBPC->FlickerFactorSet(0.7f); // ???????? ??????.. ??????????..
	}
	else if(N3_SP_STATE_CHANGE_ACTION == eSP) // ???? ????
	{
		if (1 == iState) pBPC->AnimationAdd(ANI_GREETING0, true); // ????
		else if (2 == iState) pBPC->AnimationAdd(ANI_GREETING1, true); // ????
		else if (3 == iState) pBPC->AnimationAdd(ANI_GREETING2, true); // ????
		else if (11 == iState) pBPC->AnimationAdd(ANI_WAR_CRY1, true); // ????
		else if (12 == iState) pBPC->AnimationAdd(ANI_WAR_CRY2, true); // ????
		else if (13 == iState) pBPC->AnimationAdd(ANI_WAR_CRY3, true); // ????
	}
	else if (N3_SP_STATE_CHANGE_VISIBLE == eSP)
	{
		if (pBPC->m_InfoBase.iAuthority == AUTHORITY_MANAGER) {
			if (0 == iState)
				pBPC->m_bVisible = true;
			if (255 == iState && (s_pPlayer->m_InfoBase.iAuthority != AUTHORITY_MANAGER))
				pBPC->m_bVisible = false;
		}
	}
}

void CGameProcMain::MsgRecv_Notice(Packet& pkt)
{
	if(m_pUINotice) m_pUINotice->RemoveNotice();

	int iNoticeCount = pkt.read<uint8_t>();
	for(int i = 0; i < iNoticeCount; i++)
	{
		int iStrLen = pkt.read<uint8_t>();
		if(iStrLen <= 0) continue;

		std::string szNotice;
		pkt.readString(szNotice, iStrLen);
		if(m_pUINotice) m_pUINotice->m_Texts.push_back(szNotice);
	}

	if(m_pUINotice && iNoticeCount > 0)
	{
		m_pUINotice->GenerateText();
		
		// NOTE(srmeier): set where on the screen the notice appears
		RECT rc = m_pUINotice->GetRegion();
		int x = CN3Base::s_CameraData.vp.Width - (rc.right - rc.left);//(CN3Base::s_CameraData.vp.Width/2) - (rc.right - rc.left)/2;
		int y = 10;//(CN3Base::s_CameraData.vp.Height/2) - (rc.bottom - rc.top)/2;
		m_pUINotice->SetPos(x, y);
		m_pUINotice->SetVisible(true);
	}
}

void CGameProcMain::MsgRecv_PartyOrForce(Packet& pkt)
{
//	int iPartyOrForce = pkt.read<uint8_t>();
	int iSubCmd	= pkt.read<uint8_t>();
	
	switch(iSubCmd)
	{
		case N3_SP_PARTY_OR_FORCE_PERMIT:			// 0x02	// Send - b1(YesNo) | Recv - s1(ID) ?????? ?????? ID
		{
			int iID			= pkt.read<int16_t>();
			int iStrLen		= pkt.read<int16_t>();
			std::string szID;
			pkt.readString(szID, iStrLen);
			
			if(iID >= 0)
			{
				std::string szMsg; ::_LoadStringFromResource(IDS_PARTY_PERMIT, szMsg);
				CGameProcedure::MessageBoxPost(szID + szMsg, "", MB_YESNO, BEHAVIOR_PARTY_PERMIT);
			}
		}
		break;

		case N3_SP_PARTY_OR_FORCE_INSERT:			// 0x02	// Send - s1(ID) | Recv - s3(ID, HPMax, HP) b2(Level, Class) - ???????? ID ?? ????????..
		{
			int iID = pkt.read<int16_t>();
			int iErrorCode = pkt.read<uint8_t>();

			if(iErrorCode >= 0)
			{
				int iIDLength	= pkt.read<int16_t>();
				std::string szID; pkt.readString(szID, iIDLength);
				int iHPMax		= pkt.read<int16_t>();
				int iHP			= pkt.read<int16_t>();
				int iLevel		= pkt.read<uint8_t>();
				e_Class eClass	= (e_Class)(pkt.read<int16_t>());

				// NOTE: these parts where added to this packet at some later point and will need to be
				// implemented...
				int iMPMax		= pkt.read<int16_t>();
				int iMP			= pkt.read<int16_t>();
				e_Nation eNation = (e_Nation)pkt.read<uint8_t>();

				m_pUIPartyOrForce->MemberAdd(iID, szID, iLevel, eClass, iHP, iHPMax); // ?????? ??????????..
				if(iID != s_pPlayer->IDNumber()) // ???? ?????? ???? ???? ?????? ????.
				{
					std::string szMsg; ::_LoadStringFromResource(IDS_PARTY_INSERT, szMsg);
					this->MsgOutput(szID + szMsg, D3DCOLOR_ARGB(255,255,255,255));
				}
			}
			else // ?????? ???????? ????..
			{
				std::string szMsg;

				if (-1 == iErrorCode) ::_LoadStringFromResource(IDS_PARTY_INSERT_ERR_REJECTED, szMsg); // ???????? ?????? ?????????? ???? ??????..
				else if (-2 == iErrorCode) ::_LoadStringFromResource(IDS_PARTY_INSERT_ERR_LEVEL_DIFFERENCE, szMsg); // ???? ?????? ???? ????...
				else if (-3 == iErrorCode) ::_LoadStringFromResource(IDS_PARTY_INSERT_ERR_INVALID_NATION, szMsg); // ?????? ???? ?? ???? ????????.
				else ::_LoadStringFromResource(IDS_PARTY_INSERT_ERR, szMsg); // ???????? ?????? ?????????? ???? ??????..

				this->MsgOutput(szMsg, D3DCOLOR_ARGB(255,255,255,255));
				if(m_pUIPartyOrForce->MemberCount() == 1) m_pUIPartyOrForce->MemberDestroy(); // ?????? ???????? ???? ?????? ???? ??????.
			}
			
			this->UpdateUI_PartyOrForceButtons(); // ?????? ???? ???? ???? ?????? ?????? ???? ???????? ??????.
		}
		break;
	
		case N3_SP_PARTY_OR_FORCE_REMOVE:			// 0x03	// Send - s1(ID) | Recv - s1(ID) - 
		{
			int iID			= pkt.read<int16_t>();

			if(iID == s_pPlayer->IDNumber())
			{
				std::string szMsg; ::_LoadStringFromResource(IDS_PARTY_DESTROY, szMsg); // ?????? ??????..
				this->MsgOutput(szMsg, D3DCOLOR_ARGB(255,255,255,255));  // ???? ???? ??????
				m_pUIPartyOrForce->MemberDestroy(); // ???? ????????.. ?????? ??????..
			}
			else
			{
				int iMemberIndex = -1;
				const __InfoPartyOrForce* pInfo = m_pUIPartyOrForce->MemberInfoGetByID(iID, iMemberIndex);
				if(pInfo)
				{
					std::string szMsg; ::_LoadStringFromResource(IDS_PARTY_LEAVE, szMsg); // ?????? ??????..
					this->MsgOutput(pInfo->szID + szMsg, D3DCOLOR_ARGB(255,255,255,255)); // ???? ???????? ???????? ??????..
					m_pUIPartyOrForce->MemberRemove(iID); // ??????..
				}
			}

			this->UpdateUI_PartyOrForceButtons(); // ?????? ???? ???? ???? ?????? ?????? ???? ???????? ??????.
		}
		break;
		
		case N3_SP_PARTY_OR_FORCE_DESTROY:			// 0x04	// Send
		{
			m_pUIPartyOrForce->MemberDestroy(); // ???? ??????..
			std::string szMsg; ::_LoadStringFromResource(IDS_PARTY_DESTROY, szMsg);
			this->MsgOutput(szMsg, D3DCOLOR_ARGB(255,255,255,255));

			this->UpdateUI_PartyOrForceButtons(); // ?????? ???? ???? ???? ?????? ?????? ???? ???????? ??????.
		}
		break;

		case N3_SP_PARTY_OR_FORCE_HP_CHANGE:		// 0x05	// Recv - s3(ID, HPMax, HP) - ???? ???????? ?????? ???? ????..
		{
			int iID			= pkt.read<int16_t>();
			int iHPMax		= pkt.read<int16_t>();
			int iHP			= pkt.read<int16_t>();

			m_pUIPartyOrForce->MemberHPChange(iID, iHP, iHPMax);
		}
		break;
		
		case N3_SP_PARTY_OR_FORCE_LEVEL_CHANGE:		// 0x06	// Recv - s1(ID), b1(Level)
		{
			int iID			= pkt.read<int16_t>();
			int iLevel		= pkt.read<uint8_t>();

			m_pUIPartyOrForce->MemberLevelChange(iID, iLevel);
		}
		break;
		
		case N3_SP_PARTY_OR_FORCE_CLASS_CHANGE:		// 0x07	// Recv - s1(ID), b1(Class)???????? ????????...
		{
			int iID			= pkt.read<int16_t>();
			e_Class eClass	= (e_Class)(pkt.read<int16_t>());

			m_pUIPartyOrForce->MemberClassChange(iID, eClass);
		}
		break;
		
		case N3_SP_PARTY_OR_FORCE_STATUS_CHANGE:	// 0x08	// Recv - s1(ID), b1(Status)...??, ????, ??????????, ????
		{
			int iID	=			pkt.read<int16_t>();
			e_PartyStatus ePS =	(e_PartyStatus)pkt.read<uint8_t>();
			int iSuffer = 		pkt.read<uint8_t>();
			bool bSuffer = (iSuffer) ? true : false; 

			m_pUIPartyOrForce->MemberStatusChange(iID, ePS, bSuffer);
		}
		break;
	}
}

void CGameProcMain::CommandSitDown(bool bLimitInterval, bool bSitDown, bool bImmediately)
{
	if(bSitDown == s_pPlayer->m_bSitDown) return; // ?????? ?????? ????????..

	// ???? ???? ????..
	if(bLimitInterval)
	{
		float fTime = CN3Base::TimeGet();
		static float fTimePrev = fTime - 4.0f;
		if(fTime - fTimePrev < 3.0f) return;
		fTimePrev = fTime;
	}

	e_StateAction eSA = s_pPlayer->State();
	e_StateMove eSM = s_pPlayer->StateMove();

	if(bSitDown)
	{
		if(eSM != PSM_STOP) // ???? ?????? ??????.
			return;

		if(this->m_pMagicSkillMng->IsCasting()) // ???? ?????? ?????? ???? ??????..
			return;

		if(s_pPlayer->m_bAttackContinous) // ???? ???????? ??????..
			this->CommandEnableAttackContinous(false, NULL); // ???? ???? ????..
	}

	bool bVBs[2] = { false, false };
	int iState = 0;
	if(bSitDown)
	{
		eSA = PSA_SITDOWN;
		bVBs[0] = false; bVBs[1] = true;
		iState = 0x02;
	} // ???? ????
	else
	{
		eSA = PSA_BASIC;
		bVBs[0] = true; bVBs[1] = false;
		iState = 0x01;
	} // ???????? ????
	
	s_pPlayer->m_bSitDown = bSitDown;
	s_pPlayer->Action(eSA, true, NULL, bImmediately);

	if(m_pUICmd->m_pBtn_Act_SitDown)
	{
		m_pUICmd->m_pBtn_Act_SitDown->SetVisible(bVBs[0]);
		m_pUICmd->m_pBtn_Act_SitDown->SetState(UI_STATE_BUTTON_DOWN); // ???? ????
	}
	if(m_pUICmd->m_pBtn_Act_StandUp)
	{
		m_pUICmd->m_pBtn_Act_StandUp->SetVisible(bVBs[1]);
		m_pUICmd->m_pBtn_Act_StandUp->SetState(UI_STATE_BUTTON_DOWN); // ???? ????
	}

	this->MsgSend_StateChange(N3_SP_STATE_CHANGE_SITDOWN, iState); // ??????.. ????..
}

void CGameProcMain::CommandTargetSelect_NearstEnemy() // ???? ?????? ?? ???? ????..
{
	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByNearstEnemy(s_pPlayer->m_InfoBase.eNation, s_pPlayer->Position());
	this->TargetSelect(pTarget);
	s_pPlayer->RotateTo(pTarget);
}

void CGameProcMain::CommandTargetSelect_NearstOurForce() // ???? ?????? ???? ????????..
{
	CPlayerOther* pTarget = m_pUIPartyOrForce->MemberGetByNearst(s_pPlayer->Position());
	this->TargetSelect(pTarget);
	s_pPlayer->RotateTo(pTarget);
}

void CGameProcMain::CloseUIs()
{
}

void CGameProcMain::MsgSend_StateChange(e_SubPacket_State eSP, int iState)
{
	uint8_t byBuff[4];											// ???? ????..
	int iOffset=0;											// ???? ??????..

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_STATE_CHANGE);	// ???? ????..
	CAPISocket::MP_AddByte(byBuff, iOffset, eSP);
	CAPISocket::MP_AddShort(byBuff, iOffset, iState);//CAPISocket::MP_AddByte(byBuff, iOffset, iState);

	s_pSocket->Send(byBuff, iOffset);	
}

void CGameProcMain::MsgSend_PerTradeReq(int iDestID, bool bNear)
{
	uint8_t byBuff[4];											// ???? ????..
	int iOffset=0;											// ???? ??????..

	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_EXCHANGE);			
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PER_TRADE_REQ);		
	CAPISocket::MP_AddShort(byBuff, iOffset, (int16_t)iDestID );			// ?????? ??????..
	if(bNear)
		CAPISocket::MP_AddByte(byBuff, iOffset, 1);
	else
		CAPISocket::MP_AddByte(byBuff, iOffset, 2);
	//1:???? ????
	//2:?????? ?????? ????

	s_pSocket->Send(byBuff, iOffset);									// ????..

	//TRACE("??????: %d, ?????? ???? ???? ???? ????.. \n", iDestID);
}

void CGameProcMain::MsgRecv_PerTrade(Packet& pkt)
{
	uint8_t	bSubCom = pkt.read<uint8_t>();
	int16_t	sOtherID, sItemCount, sCount, sDurability;
	uint8_t	bResult, bItemPos;
	int		iItemID, iCount, iTotalGold;

	switch ( bSubCom )
	{
		case N3_SP_PER_TRADE_REQ:
			//TRACE("?????? ???? ???? ???? ????.. \n");
			sOtherID = pkt.read<int16_t>();	

			if (m_pUITransactionDlg->IsVisible()) 	/* ?????? ??????????.. */
			{
				// ????????..
				m_pUITransactionDlg->LeaveTransactionState();
				m_pSubProcPerTrade->LeavePerTradeState(PER_TRADE_RESULT_MY_DISAGREE);
				break;
			}

			if (m_pUIWareHouseDlg->IsVisible()) 	/* ???????? ??????????.. */
			{
				// ????????..
				m_pUIWareHouseDlg->LeaveWareHouseState();
				m_pSubProcPerTrade->LeavePerTradeState(PER_TRADE_RESULT_MY_DISAGREE);
				break;
			}

			if ( m_pUIInventory->IsVisible() )
				m_pUIInventory->Close();

			if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE)
			{
				std::string stdMsg;
				::_LoadStringFromResource(IDS_PER_TRADEING_OTHER, stdMsg);
				MsgOutput(stdMsg, 0xff9b9bff);
				break;
			}

			if (m_pUIQuestMenu->IsVisible())
				m_pUIQuestMenu->SetVisible(false);

			if (m_pUIQuestTalk->IsVisible())
				m_pUIQuestTalk->SetVisible(false);

			if (m_pUINpcEvent->IsVisible())
				m_pUINpcEvent->Close();

			if (m_pUITradeBBSSelector->IsVisible())
				m_pUITradeBBSSelector->SetVisible(false);

			if(m_pUITradeBBS->IsVisible())
				m_pUITradeBBS->SetVisible(false);

			m_pSubProcPerTrade->ReceiveMsgPerTradeReq(sOtherID);
			break;


		case N3_SP_PER_TRADE_AGREE:
			bResult = pkt.read<uint8_t>();
			m_pSubProcPerTrade->ReceiveMsgPerTradeAgree(bResult);
			break;

		case N3_SP_PER_TRADE_ADD:
			bResult = pkt.read<uint8_t>();
			m_pSubProcPerTrade->ReceiveMsgPerTradeAdd(bResult);
			break;

		case N3_SP_PER_TRADE_OTHER_ADD:
			iItemID = pkt.read<uint32_t>();
			iCount  = pkt.read<uint32_t>();
			sDurability = pkt.read<int16_t>();
			m_pSubProcPerTrade->ReceiveMsgPerTradeOtherAdd(iItemID, iCount, (int)sDurability);			
			break;

		case N3_SP_PER_TRADE_OTHER_DECIDE:
			m_pSubProcPerTrade->ReceiveMsgPerTradeOtherDecide();
			break;

		case N3_SP_PER_TRADE_DONE:
			bResult = pkt.read<uint8_t>();
			if ( bResult == 0x01 )		// ????????..
			{	
				iTotalGold = pkt.read<uint32_t>();
				m_pSubProcPerTrade->ReceiveMsgPerTradeDoneSuccessBegin(iTotalGold);
				sItemCount = pkt.read<int16_t>();
				for( int i = 0; i < sItemCount; i++ )
				{
					bItemPos = pkt.read<uint8_t>();	
					iItemID = pkt.read<uint32_t>();
					sCount  = pkt.read<int16_t>();	
					sDurability = pkt.read<int16_t>();
					m_pSubProcPerTrade->ReceiveMsgPerTradeDoneItemMove(bItemPos, iItemID, sCount, sDurability);					
				}
				m_pSubProcPerTrade->ReceiveMsgPerTradeDoneSuccessEnd();
			}
			else						// ????????..
				m_pSubProcPerTrade->ReceiveMsgPerTradeDoneFail();
			break;

		case N3_SP_PER_TRADE_CANCEL:
				m_pSubProcPerTrade->ReceiveMsgPerTradeCancel();
			break;
	}
}

void CGameProcMain::TargetSelect(int iID, bool bMustAlive)
{
	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(iID, bMustAlive);
	this->TargetSelect(pTarget);
}

void CGameProcMain::TargetSelect(CPlayerNPC* pTarget)
{
	if(PSA_SPELLMAGIC == s_pPlayer->State()) // ???? ???????? ???? ???????? ?????? ?? ??????..
		return;

	int iIDTargetPrev = s_pPlayer->m_iIDTarget; // ???? ???? ????..
	s_pPlayer->m_iIDTarget = -1;
	s_pPlayer->m_pObjectTarget = NULL;

	if(pTarget && pTarget->m_InfoBase.iAuthority != AUTHORITY_MANAGER)
	{ //???????? ???????? ???? ???? ???? ??????...
		s_pPlayer->m_iIDTarget = pTarget->IDNumber();
		if(iIDTargetPrev != s_pPlayer->m_iIDTarget) // ???? ?????? ??????
		{
			this->CommandEnableAttackContinous(false, NULL);	// ???? ???? ????..

			D3DCOLOR crID = 0xffffffff;
			if(	pTarget->PlayerType() == PLAYER_OTHER) // User..
			{
				if(pTarget->m_InfoBase.eNation != s_pPlayer->m_InfoBase.eNation) crID = 0xffff4040; // ???? ????????
				else crID = 0xff6b9fff;
			}
			else // NPC
			{
				if(pTarget->m_InfoBase.eNation != s_pPlayer->m_InfoBase.eNation) crID = 0xffff6060; // ???? ????????
				else crID = 0xff1064ff;
			}
			
			if( pTarget->IsAlive() )//???????? ?????? target bar?? ????????.
			{
				m_pUITargetBar->SetVisible(true);
				m_pUITargetBar->SetIDString(pTarget->IDString(), crID); // ?????? ????..
				this->MsgSend_RequestTargetHP(s_pPlayer->m_iIDTarget, 0x01);	// ???????? HP?????? ????????..
			}
			else
			{
				m_pUITargetBar->SetVisible(false); // ???? ?? ??????..
			}
		}
	}
	else
	{
		s_pPlayer->m_iIDTarget = -1; // ???? ???????? ????..
		m_pUITargetBar->SetVisible(false); // ???? ?? ??????..

		this->CommandEnableAttackContinous(false, NULL); // ???? ???? ????..
	}

	if(	PSA_SITDOWN != s_pPlayer->State() && 
		PSM_STOP == s_pPlayer->StateMove() &&
		PSA_BASIC == s_pPlayer->State()) s_pPlayer->Action(PSA_BASIC, true); // ???? ???? ??????..

	this->UpdateUI_PartyOrForceButtons(); // ?????? ???? ???? ???? ?????? ?????? ???? ???????? ??????.
}

void CGameProcMain::MsgRecv_SkillChange(Packet& pkt)			// ???? ????..
{
	int iType	= pkt.read<uint8_t>();
	int iValue	= pkt.read<uint8_t>();

	m_pUISkillTreeDlg->m_iSkillInfo[iType] = iValue;
	m_pUISkillTreeDlg->m_iSkillInfo[0]++;
	m_pUISkillTreeDlg->InitIconUpdate();		// ???? ???????? ?????????????? .. ?????? ?????? ?? ????..
}

void CGameProcMain::MsgRecv_MagicProcess(Packet& pkt)
{
	e_SubPacket_Magic eSP = (e_SubPacket_Magic)pkt.read<uint8_t>();
	
	switch ( eSP )
	{
	case N3_SP_MAGIC_CASTING:
		m_pMagicSkillMng->MsgRecv_Casting(pkt);
		break;
	case N3_SP_MAGIC_FLYING:
		m_pMagicSkillMng->MsgRecv_Flying(pkt);
		break;
	case N3_SP_MAGIC_EFFECTING:
		m_pMagicSkillMng->MsgRecv_Effecting(pkt);
		break;
	case N3_SP_MAGIC_FAIL:
		m_pMagicSkillMng->MsgRecv_Fail(pkt);
		break;
	case N3_SP_MAGIC_TYPE4BUFFTYPE:
		m_pMagicSkillMng->MsgRecv_BuffType(pkt);
		break;
	}	
}

void CGameProcMain::MsgRecv_ClassChange(Packet& pkt)			// ???? ????..
{
	e_SubPacket_ClassChange eSP = (e_SubPacket_ClassChange)pkt.read<uint8_t>();

	switch ( eSP )
	{
		case N3_SP_CLASS_CHANGE_SUCCESS:		// Success..
			m_pUIClassChange->Open(N3_SP_CLASS_CHANGE_SUCCESS);
			break;

		case N3_SP_CLASS_CHANGE_NOT_YET:		// Not Yet..
			m_pUIClassChange->Open(N3_SP_CLASS_CHANGE_NOT_YET);
			break;

		case N3_SP_CLASS_CHANGE_ALREADY:		// Already..
			m_pUIClassChange->Open(N3_SP_CLASS_CHANGE_ALREADY);
			break;

		case N3_SP_CLASS_CHANGE_FAILURE:		// Failure..
			m_pUIClassChange->RestorePrevClass();
			break;
	}
}

void CGameProcMain::MsgRecv_ObjectEvent(Packet& pkt)
{
	int iType = pkt.read<uint8_t>();		// Event Type
	int iResult = pkt.read<uint8_t>();	

	if(OBJECT_TYPE_BINDPOINT == iType) 
	{
		std::string szMsg;
		if (0x01 == iResult) ::_LoadStringFromResource(IDS_BIND_POINT_FAILED, szMsg);
		this->MsgOutput(szMsg, 0xff00ff00);
	}
	else if(OBJECT_TYPE_DOOR_LEFTRIGHT == iType ||
			OBJECT_TYPE_DOOR_TOPDOWN == iType ||
			OBJECT_TYPE_LEVER_TOPDOWN == iType ||
			OBJECT_TYPE_FLAG == iType) 
	{
		int iID = pkt.read<int16_t>();	// ???? ???? ???? ID
		int iActivate = pkt.read<uint8_t>();	// ???? ????..

		CPlayerNPC* pNPC = s_pOPMgr->NPCGetByID(iID, true);
		__ASSERT(pNPC, "Invalid NPC ID");
		if(pNPC)
		{
			__ASSERT(pNPC->m_pShapeExtraRef && (pNPC->m_pShapeExtraRef->Type() & OBJ_SHAPE_EXTRA), "Invalid object!!");
			std::string szMsg = "Object Event - invalid object!!";
			if(pNPC->m_pShapeExtraRef)
			{
				__Vector3 vAxis(0,1,0);
				float fRadian = D3DXToRadian(90);
				bool bShouldBeRotate = true; // ?????? ????????
				if(OBJECT_TYPE_DOOR_LEFTRIGHT == iType) // ???????? ????
				{
					if(0x01 == iActivate) {
						fRadian = D3DXToRadian(80);
						::_LoadStringFromResource(IDS_DOOR_OPENED, szMsg);
					}
					else {
						fRadian = D3DXToRadian(0);
						::_LoadStringFromResource(IDS_DOOR_CLOSED, szMsg);
					}
					vAxis.Set(0,1,0);
				} 
				else if(OBJECT_TYPE_DOOR_TOPDOWN == iType) // ???????? ????
				{
					if(0x01 == iActivate) {
						fRadian = D3DXToRadian(90);
						::_LoadStringFromResource(IDS_DOOR_OPENED, szMsg);
					}
					else  {
						D3DXToRadian(0);
						::_LoadStringFromResource(IDS_DOOR_CLOSED, szMsg);
					}
					vAxis.Set(0,0,1);
				}
				else if(OBJECT_TYPE_LEVER_TOPDOWN == iType) // ???? ????
				{
					if(0x01 == iActivate) {
						fRadian = D3DXToRadian(-45);
						::_LoadStringFromResource(IDS_LEVER_ACTIVATE, szMsg);
					}
					else  {
						fRadian = D3DXToRadian(45);
						::_LoadStringFromResource(IDS_LEVER_DEACTIVATE, szMsg);
					}
					vAxis.Set(1,0,0);
				}
				else if(OBJECT_TYPE_FLAG == iType)
				{
					bShouldBeRotate = false; // ?????? ????????
				}

				if(0x01 == iActivate)
				{
				}
				else // if(0x00 == iActivate);
				{
					::_LoadStringFromResource(IDS_DOOR_CLOSED, szMsg);
				}

				CN3ShapeExtra* pSE = (CN3ShapeExtra*)pNPC->m_pShapeExtraRef;
				if(bShouldBeRotate)
				{
					pSE->RotateTo(0, vAxis, fRadian, 1);
					pSE->RotateTo(1, vAxis, -fRadian, 1);
				}
				else 
				{
					if(iActivate) pSE->m_bVisible = true;
					else pSE->m_bVisible = false;
				}
			}
			this->MsgOutput(szMsg, 0xff00ff00);
		}
	}
	else
	{
		__ASSERT(0, "Unknown Object Event");
	}
}

void CGameProcMain::ParseChattingCommand(const std::string& szCmd)
{
	static char szCmds[4][1024] = { "", "", "", "" };
	static uint8_t byBuff[1024] = "";
	sscanf(szCmd.c_str(), "/%s %s %s %s", szCmds[0], szCmds[1], szCmds[2], szCmds[3]);

	if(0 == lstrcmp(szCmds[0], "goto"))
	{
		float fX = (float)atof(szCmds[1]);
		float fZ = (float)atof(szCmds[2]);
		
		int iOffset = 0;
		CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_WARP);
		CAPISocket::MP_AddWord(byBuff, iOffset, (uint16_t)(fX * 10));
		CAPISocket::MP_AddWord(byBuff, iOffset, (uint16_t)(fZ * 10));

		s_pSocket->Send(byBuff, iOffset);
	}
	
	e_ChatCmd eCmd = CMD_UNKNOWN;
	for(int i = 0; i < CMD_COUNT; i++)
	{
		if(0 == lstrcmpi(szCmds[0], s_szCmdMsg[i].c_str()))
		{
			eCmd = (e_ChatCmd)i;
			break;
		}
	}

	switch(eCmd)
	{
		case CMD_WHISPER:
		{
			this->MsgSend_ChatSelectTarget(szCmds[1]); // ?????? ???? ???? ??????.
		}
		break;

		case CMD_TOWN:
		{
			if(s_pPlayer->m_bStun) return; // ?????? ???? ????..
			if(s_pPlayer->m_InfoBase.iHP * 2 >= s_pPlayer->m_InfoBase.iHPMax) // HP?? ?? ???? ?????? ????.
			{
				// NOTE(srmeier): currently there is an issue where OtherPlayers may get
				// duplicated in the player manager if they where there before the TP

				int iOffset = 0;
				CAPISocket::MP_AddWord(byBuff, iOffset, WIZ_HOME);		// ?????? ????...
				s_pSocket->Send(byBuff, iOffset);
			}
			else // HP?? ?? ???? ?????? ????.
			{
				std::string szMsg;
				::_LoadStringFromResource(IDS_ERR_GOTO_TOWN_OUT_OF_HP, szMsg);
				this->MsgOutput(szMsg, 0xffff00ff);
			}
		}
		break;

		case CMD_TRADE:
		{
			CPlayerOther* pOPC = s_pOPMgr->UPCGetByID(s_pPlayer->m_iIDTarget, true);
			if(	pOPC &&
				(pOPC->Position() - s_pPlayer->Position()).Magnitude() < (pOPC->Height() + 5.0f) && 
				!m_pUITransactionDlg->IsVisible() ) // ???????? ???? ?????????? ????????..  ?????? ??????.. // ?????? ?????? ????.. // ?????? ???? ??????..
			{
				std::string szMsg; ::_LoadStringFromResource(IDS_PERSONAL_TRADE_REQUEST, szMsg);
				MsgOutput(pOPC->IDString() + szMsg, 0xffffff00);

				MsgSend_PerTradeReq(pOPC->IDNumber());
				
				if (m_pUINpcEvent->IsVisible())
					m_pUINpcEvent->Close();

				if (m_pUIQuestTalk->IsVisible())
					m_pUIQuestTalk->SetVisible(false);

				if (m_pUIQuestMenu->IsVisible())
					m_pUIQuestMenu->SetVisible(false);

				m_pSubProcPerTrade->EnterWaitMsgFromServerStatePerTradeReq();
			}
		}
		break;

		case CMD_EXIT:
		{
			//PostQuitMessage(0);
			CGameBase::s_bRunning = false;
		}
		break;

		case CMD_PARTY:
		{
			CPlayerBase* pTarget = s_pOPMgr->UPCGetByID(s_pPlayer->m_iIDTarget, true);
			if(pTarget)
			{
				std::string szMsg;
				if (this->MsgSend_PartyOrForceCreate(0, pTarget->IDString()))
					::_LoadStringFromResource(IDS_PARTY_INVITE, szMsg); // ???? ????.. 
				else
					::_LoadStringFromResource(IDS_PARTY_INVITE_FAILED, szMsg); // ???? ???? ????
				this->MsgOutput(pTarget->IDString() + szMsg, 0xffffff00);
			}
		}
		break;

		case CMD_LEAVEPARTY:
		{
			this->MsgSend_PartyOrForceLeave(0); // ???? ????..
		}
		break;

		case CMD_RECRUITPARTY:
		{
			if(m_pUIPartyBBS)
			{
				if(s_pPlayer->m_bRecruitParty)
				{
					m_pUIPartyBBS->MsgSend_RegisterCancel();
				}
				else
				{
					m_pUIPartyBBS->MsgSend_Register();
				}
			}

//			if(m_pUIPartyBBS && !m_pUIPartyBBS->IsVisible())
//			m_pUIPartyBBS->MsgSend_RefreshData(0);
		}
		break;

		case CMD_JOINCLAN:
		{
			if(s_pPlayer->m_InfoExt.eKnightsDuty == KNIGHTS_DUTY_CHIEF ||
				s_pPlayer->m_InfoExt.eKnightsDuty == KNIGHTS_DUTY_VICECHIEF)
			{			
				this->MsgSend_KnightsJoin(s_pPlayer->m_iIDTarget);
			}
		}
		break;

		case CMD_WITHDRAWCLAN:
		{
			this->MsgSend_KnightsWithdraw();
		}
		break;

		case CMD_FIRECLAN:
		{
			if(s_pPlayer->m_InfoExt.eKnightsDuty == KNIGHTS_DUTY_CHIEF)
			{
				std::string szName = szCmds[1];
				MsgSend_KnightsLeave(szName);			
			}
		}
		break;

		case CMD_APPOINTVICECHIEF:
		{
			if(s_pPlayer->m_InfoExt.eKnightsDuty == KNIGHTS_DUTY_CHIEF)
			{
				std::string szName = szCmds[1];
				MsgSend_KnightsAppointViceChief(szName);
			}
		}
		break;

		case CMD_GREETING:
		case CMD_GREETING2:
		case CMD_GREETING3:
		{
			if(	s_pPlayer->State() == PSA_BASIC && 
				s_pPlayer->StateMove() == PSM_STOP )
			{
				this->MsgSend_StateChange(N3_SP_STATE_CHANGE_ACTION, 1 + (eCmd - CMD_GREETING));
			}
		}
		break;

		case CMD_PROVOKE:
		case CMD_PROVOKE2:
		case CMD_PROVOKE3:
		{
			if(	s_pPlayer->State() == PSA_BASIC && 
				s_pPlayer->StateMove() == PSM_STOP )
			{
				this->MsgSend_StateChange(N3_SP_STATE_CHANGE_ACTION, 11 + (eCmd - CMD_PROVOKE));
			}
		}
		break;

		case CMD_VISIBLE:
		{
			this->MsgSend_StateChange(N3_SP_STATE_CHANGE_VISIBLE, 0);
		}
		break;

		case CMD_INVISIBLE:
		{
			this->MsgSend_StateChange(N3_SP_STATE_CHANGE_VISIBLE, 255);
		}
		break;

		case CMD_CLEAN:
		{
			int iPercent = atoi(szCmds[1]);
			this->MsgSend_Weather(1, iPercent);
		}
		break;

		case CMD_RAINING:
		{
			int iPercent = atoi(szCmds[1]);
			this->MsgSend_Weather(2, iPercent);
		}
		break;

		case CMD_SNOWING:
		{
			int iPercent = atoi(szCmds[1]);
			this->MsgSend_Weather(3, iPercent);
		}
		break;

		case CMD_TIME:
		{
			int iHour = atoi(szCmds[1]);
			int iMin = atoi(szCmds[2]);
			this->MsgSend_Time(iHour, iMin);
		}
		break;

		case CMD_CU_COUNT:
		{
			int iOffset=0;
			CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_CONCURRENTUSER); 
			s_pSocket->Send(byBuff, iOffset);
		}
		break;

		case CMD_NOTICE:
		{
			if(szCmd.size() >= (s_szCmdMsg[CMD_NOTICE].size()+2))//7)
			{
				std::string szChat = szCmd.substr(s_szCmdMsg[CMD_NOTICE].size()+2); // "/???? "?? ?????? ?????? ??????
				this->MsgSend_Chat(N3_CHAT_PUBLIC, szChat);
			}
		}
		break;

		case CMD_ARREST:
		{
			this->MsgSend_Administrator(N3_SP_ADMINISTRATOR_ARREST, szCmds[1]); //????		
		}
		break;

		case CMD_FORBIDCONNECT:
		{
			this->MsgSend_Administrator(N3_SP_ADMINISTRATOR_FORBID_CONNECT, szCmds[1]); //????????		
		}
		break;
		
		case CMD_FORBIDCHAT:
		{
			this->MsgSend_Administrator(N3_SP_ADMINISTRATOR_CHAT_FORBID, szCmds[1]); //????????		
		}
		break;
		
		case CMD_PERMITCHAT:
		{
			this->MsgSend_Administrator(N3_SP_ADMINISTRATOR_CHAT_PERMIT, szCmds[1]); //????????		
		}
		break;
		
		case CMD_GAME_SAVE:
		{
			if(m_fRequestGameSave > 300.0f)
			{
				uint8_t byBuff[4];												// ????.. 
				int iOffset=0;												// ????..
				s_pSocket->MP_AddByte(byBuff, iOffset, WIZ_DATASAVE);	// ???? ???? ??????..
				s_pSocket->Send(byBuff, iOffset);				// ????..
				m_fRequestGameSave = 0.0f;

				std::string szMsg;
				::_LoadStringFromResource(IDS_REQUEST_GAME_SAVE, szMsg);
				this->MsgOutput(szMsg, 0xffffff00);
			}
			else
			{
				char szBuf[256];
				std::string szMsg;
				::_LoadStringFromResource(IDS_DELAY_GAME_SAVE, szMsg);
				sprintf(szBuf, szMsg.c_str(), 5);
				this->MsgOutput(szBuf, 0xffffff00);
			}
		}
		break;

		default:
		break;
	} // end of switch(eCmd)

	// Clears out the strings from the szCmds so we won't re-send same values on the next command execution.
	memset(szCmds, 0, sizeof(szCmds));
}

void CGameProcMain::UpdateUI_PartyOrForceButtons()
{
	// ???? ???? ???? ??????..
	CPlayerBase* pTarget = NULL;
	bool bIAmLeader = false, bIAmMemberOfParty = false;
	int iMemberIndex = -1;
	this->PartyOrForceConditionGet(bIAmLeader, bIAmMemberOfParty, iMemberIndex, pTarget);
	m_pUIPartyOrForce->MemberSelect(iMemberIndex);

	m_pUICmd->UpdatePartyButtons(bIAmLeader, bIAmMemberOfParty, iMemberIndex, pTarget);
}

const __InfoPartyOrForce* CGameProcMain::PartyOrForceConditionGet(bool& bIAmLeader, bool& bIAmMember, int& iMemberIndex, class CPlayerBase*& pTarget)
{
	// ???? ???? ???? ??????..
	bIAmLeader = false;
	iMemberIndex = -1;
	bIAmMember = false;
	pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, true);

	if(m_pUIPartyOrForce->MemberCount() >= 2)
	{
		bIAmMember = true;
		if(m_pUIPartyOrForce->MemberInfoGetByIndex(0)->iID == s_pPlayer->IDNumber()) bIAmLeader = true;
		return m_pUIPartyOrForce->MemberInfoGetByID(s_pPlayer->m_iIDTarget, iMemberIndex);
	}

	return NULL;
}

void CGameProcMain::UpdateUI_MiniMap()
{
	if(NULL == m_pUIStateBarAndMiniMap || !m_pUIStateBarAndMiniMap->IsVisible()) return;


	m_pUIStateBarAndMiniMap->PositionInfoClear();

	D3DCOLOR crType = 0xffffffff;
	e_Nation eNation = s_pPlayer->m_InfoBase.eNation;

	it_NPC it = s_pOPMgr->m_NPCs.begin(), itEnd = s_pOPMgr->m_NPCs.end();
	CPlayerNPC* pNPC = NULL;
	for(; it != itEnd; it++) // NPC
	{
		pNPC = it->second;

		if(eNation != pNPC->m_InfoBase.eNation) crType = 0xff800000; // ???? ???? NPC ???? ?????? ??????
		else crType = 0xff00a0ff; // ???? ???? NPC ??????

		m_pUIStateBarAndMiniMap->PositionInfoAdd(pNPC->IDNumber(), pNPC->Position(), crType, false);
	}

	it_UPC it2 = s_pOPMgr->m_UPCs.begin(), itEnd2 = s_pOPMgr->m_UPCs.end();
	CPlayerOther* pUPC = NULL;
	__TABLE_ZONE* pZoneInfo = s_pTbl_Zones.Find(s_pPlayer->m_InfoExt.iZoneCur);
	for(; it2 != itEnd2; it2++) // User
	{
		pUPC = it2->second;

		bool bDrawTop = false;
		if(eNation != pUPC->m_InfoBase.eNation) // ??????????
		{
			if(pUPC->State() == PSA_SITDOWN)
			{
				pUPC->m_InfoBase.bRenderID = false; // ?????? ???????? ????.
				continue; // ??????????.. ????????..
			}
			else
			{
				pUPC->m_InfoBase.bRenderID = true; // ?????? ??????.
			}

			if(pZoneInfo && FALSE == pZoneInfo->bIndicateEnemyPlayer) continue; // ?????? ???? ???? ?????? ??????.. ????????..
			
			crType = 0xffff4040; // ???? ????  - ???? ??????
		}
		else // ???? ??????..
		{
			int iMemberIndex = -1;
			const __InfoPartyOrForce* pPI = m_pUIPartyOrForce->MemberInfoGetByID(pUPC->IDNumber(), iMemberIndex);
			if(pPI)
			{
				bDrawTop = true; // ?? ???? ??????.
				crType = 0xffffff00; // ???? ?????? // ??????
			}
			else
			{
				crType = 0xff0040ff; // ???? ?????? ?????? ???? ??????
			}
		}

		if(pUPC && pUPC->m_InfoBase.iAuthority != AUTHORITY_MANAGER)	// ???????? ?????????? ???????? ???????? ????????.
			m_pUIStateBarAndMiniMap->PositionInfoAdd(pUPC->IDNumber(), pUPC->Position(), crType, bDrawTop);
	}
}

void CGameProcMain::UpdateUI_TargetBar()
{
	if(NULL == m_pUITargetBar || !m_pUITargetBar->IsVisible()) return;

	//???? ???????? ?????????????? target bar?? ???????? ??????.
	CPlayerNPC* t_pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);
	if( t_pTarget && t_pTarget->State() == PSA_DEATH )
	{
		m_pUITargetBar->SetVisible(false);
		return;
	}

	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, true);
	if(pTarget) // ?????? ????????..
	{
		float fTimeTmp = CN3Base::TimeGet();
		if(fTimeTmp > m_pUITargetBar->m_fTimeSendPacketLast + PACKET_INTERVAL_REQUEST_TARGET_HP) // 1???? ?????? ???? ???? ????
		{
			this->MsgSend_RequestTargetHP(s_pPlayer->m_iIDTarget, false);
			m_pUITargetBar->m_fTimeSendPacketLast = fTimeTmp;
		}
	}
	else // ?????? ??????..
	{
		s_pPlayer->m_iIDTarget = -1;
		m_pUITargetBar->SetVisible(false);
	}
}

void CGameProcMain::UpdateBGM()
{
	if(	NULL == m_pSnd_Battle || !m_pSnd_Battle->IsPlaying() ) return;

//	if(s_pPlayer->pTarget && s_pPlayer->pTarget->IsAlive()) 
	__Vector3 vPosPlayer = s_pPlayer->Position();
	e_Nation eNationPlayer = s_pPlayer->m_InfoBase.eNation;

	bool bStopBattleBgm = true;
	CPlayerBase* pBPC;
	it_NPC it = s_pOPMgr->m_NPCs.begin(), itEnd = s_pOPMgr->m_NPCs.end();
	for(; it != itEnd && bStopBattleBgm; it++)
	{
		pBPC = it->second;
		if(eNationPlayer == pBPC->m_InfoBase.eNation) continue;

		if((vPosPlayer - pBPC->Position()).Magnitude() < 12.0f)
			bStopBattleBgm = false;
	}

	CPlayerOther* pUPC;
	it_UPC it2 = s_pOPMgr->m_UPCs.begin(), itEnd2 = s_pOPMgr->m_UPCs.end();
	for(; it2 != itEnd2 && bStopBattleBgm; it2++)
	{
		pUPC = it2->second;
		if(eNationPlayer == pUPC->m_InfoBase.eNation) continue;

		if((vPosPlayer - pUPC->Position()).Magnitude() < 12.0f)
			bStopBattleBgm = false;
	}

	if(bStopBattleBgm) this->PlayBGM_Town();
}

void CGameProcMain::UpdateCameraAndLight()
{
	__Vector3 vPosPlayer = s_pPlayer->Position();
	if(s_pPlayer->State() == PSA_SITDOWN)
	{
		float fRootY = s_pPlayer->RootPosition().y;
		float fH = s_pPlayer->Height();
		vPosPlayer.y += fRootY - (fH / 2.0f);
	}

	D3DCOLOR crDiffuses[MAX_GAME_LIGHT];
	D3DCOLOR crAmbients[MAX_GAME_LIGHT];
	for(int i = 0; i < MAX_GAME_LIGHT; i++)
	{
		crDiffuses[i] = ACT_WORLD->GetLightDiffuseColorWithSky(i);
		crAmbients[i] = ACT_WORLD->GetLightAmbientColorWithSky(i);
	}
	
	s_pEng->Tick(	crDiffuses, crAmbients, ACT_WORLD->GetFogColorWithSky(), 
					vPosPlayer, s_pPlayer->Rotation(),
					s_pPlayer->Height(), ACT_WORLD->GetSunAngleByRadinWithSky() );		// ?????? ?????? ???? ?????? ????????..
	s_pEng->ApplyCameraAndLight();	// ???????? ???????? ?????? ???? D3D Device ?? ????????.
}

void CGameProcMain::MsgRecv_DurabilityChange(Packet& pkt)		// ?????? ????..
{
	e_ItemSlot eSlot = (e_ItemSlot)pkt.read<uint8_t>();	
	int iCurValue  = pkt.read<int16_t>();	

	s_pPlayer->DurabilitySet(eSlot, iCurValue);
	m_pUIInventory->DurabilityChange(eSlot, iCurValue);
}

void CGameProcMain::MsgRecv_NpcEvent(Packet& pkt)				// Npc Event(Exchange, Repair both).. 
{
	int iTradeID = pkt.read<uint32_t>();		// Trade id
	CPlayerNPC* pNPC = s_pOPMgr->NPCGetByID(s_pPlayer->m_iIDTarget, true);
	if (!pNPC)	return;

	m_pUINpcEvent->Open(NPC_EVENT_TRADE_REPAIR, iTradeID, pNPC->GetNPCOriginID());
}


void CGameProcMain::MsgRecv_Knights(Packet& pkt)
{
	e_SubPacket_Knights eSP = (e_SubPacket_Knights)(pkt.read<uint8_t>());	// Sub Packet

	switch(eSP)
	{
	case N3_SP_KNIGHTS_CREATE: // ????..
		this->MsgRecv_Knights_Create(pkt);
		break;
	case N3_SP_KNIGHTS_WITHDRAW: //????
		this->MsgRecv_Knights_Withdraw(pkt);
		break;
	case N3_SP_KNIGHTS_JOIN: //????
		this->MsgRecv_Knights_Join(pkt);
		break;
	case N3_SP_KNIGHTS_MEMBER_REMOVE: //???? ???? - 
		this->MsgRecv_Knights_Leave(pkt);
		break;
	case N3_SP_KNIGHTS_APPOINT_VICECHIEF: //?????? ???? - ?????????? ????
		this->MsgRecv_Knights_AppointViceChief(pkt);
 	break;
	case N3_SP_KNIGHTS_MEMBER_INFO_ALL:
		this->MsgRecv_Knights_MemberInfoAll(pkt);
		break;
	case N3_SP_KNIGHTS_GRADE_CHANGE_ALL:
		this->MsgRecv_Knights_GradeChangeAll(pkt);
		break;
	case N3_SP_KNIGHTS_DESTROY: // ?????? Send - | Recv - b1(1:???? 0:????)
		{
			uint8_t	bSubCom = pkt.read<uint8_t>();
			
			std::string szMsg;
			switch ( (e_SubPacket_KNights_Common)bSubCom )
			{
			case N3_SP_KNIGHTS_COMMON_SUCCESS: //???????? ????
				::_LoadStringFromResource(IDS_CLAN_WITHDRAW_SUCCESS, szMsg);
				m_pUIKnightsOp->KnightsInfoDelete(s_pPlayer->m_InfoExt.iKnightsID);
				this->MsgOutput(szMsg, 0xffffff00);
				break;
			case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB???? ????..
			case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER:	//???? ????..
				break;
			}
			s_pPlayer->m_InfoExt.eKnightsDuty = KNIGHTS_DUTY_UNKNOWN;
			s_pPlayer->KnightsInfoSet(0, "", 0, 0);
			m_pUIVar->UpdateKnightsInfo();
		}
		break;
	case N3_SP_KNIGHTS_DUTY_CHANGE: // ???? ????..
		MsgRecv_Knights_Duty_Change(pkt);
		break;
	case N3_SP_KNIGHTS_JOIN_REQ:
		MsgRecv_Knigts_Join_Req(pkt);
		break;

/*	case N3_SP_KNIGHTS_APPOINT_CHIEF: //???? ???? - ?????????? ????
		{
			::_LoadStringFromResource(IDS_KNIGHTS_APPOINT_CHIEF_SUCCESS, szMsg); // ????
		}
		break;
	case N3_SP_KNIGHTS_DESTROY: // ?????? Send - | Recv - b1(1:???? 0:????)
		{
			::_LoadStringFromResource(IDS_KNIGHTS_DESTROY_SUCCESS, szMsg); // ????

			s_pPlayer->m_InfoExt.iKnightsID = 0;
			s_pPlayer->m_InfoExt.eKnightsDuty = KNIGHTS_DUTY_UNKNOWN;

			// ?????????? ????..
			m_pUIVar->m_pPageKnights->UpdateKnightsName("");
			m_pUIVar->m_pPageKnights->UpdateKnightsChiefName("");
			m_pUIVar->m_pPageKnights->UpdateKnightsDuty(KNIGHTS_DUTY_UNKNOWN);

			m_pUIKnightsOp->MsgSend_KnightsList(0); // ???? ?????? ???????? ????????..
		}
		break;

	case N3_SP_KNIGHTS_MEMBER_JOIN_ADMIT: //???? ???? ???? Send - s1(Knights ID) | Recv - b1(1:???? 0:????)
		{
			::_LoadStringFromResource(IDS_KNIGHTS_ADMIT_SUCCESS, szMsg); // ????
		}
		break;
	case N3_SP_KNIGHTS_MEMBER_JOIN_REJECT: //???? ???? ???? - ?????????? ????
		{
			::_LoadStringFromResource(IDS_KNIGHTS_REJECT_SUCCESS, szMsg); // ????
		}
		break;
	case N3_SP_KNIGHTS_MEMBER_PUNISH: //???? ???? - ?????????? ????
		{
			::_LoadStringFromResource(IDS_KNIGHTS_PUNISH_SUCCESS, szMsg); // ????
		}
		break;
	case N3_SP_KNIGHTS_APPOINT_OFFICER: // ???????? - ?????????? ????
		{
			::_LoadStringFromResource(IDS_KNIGHTS_APPOINT_OFFICER_SUCCESS, szMsg); // ????
		}
		break;
	case N3_SP_KNIGHTS_LIST: //???? ?????? ???? Send - | s1(Knights Count) Loop { s1(Knights ID) s1(Name Length) str1 (Name) }
		{
			if(false == m_pUIKnightsOp->IsVisible())
			{
				m_pUIKnightsOp->Open(s_pPlayer->m_InfoExt.eKnightsDuty); // UI ???? ???????? ??????..
			}
			else 
			{
				m_pUIKnightsOp->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty); // UI ???? ???????? ??????..
			}
			m_pUIKnightsOp->MsgRecv_KnightsList(pkt);
		}
		break;
	case N3_SP_KNIGHTS_MEMBER_INFO_ALL: // ???? ???? Send - s1(page) | s1(Member Count) Loop { s1(Name Length) str1 (Name) }
		{
			m_pUIVar->m_pPageKnights->MsgRecv_MemberInfo(pkt);
		}
		break;
	case N3_SP_KNIGHTS_MEMBER_INFO_ONLINE: //???? ???? ?????? Send - s1(page) | s1(Member Count) Loop { s1(Name Length) str1 (Name) }
		{
			m_pUIVar->m_pPageKnights->MsgRecv_MemberInfo(pkt);
		}
		break;
	case N3_SP_KNIGHTS_STASH: //?????? ????
		{
		}
		break;
	case N3_SP_KNIGHTS_DUTY_CHANGE: // ???? ????..
		{
			int iID = pkt.read<int16_t>();
			e_KnightsDuty eDuty = (e_KnightsDuty)pkt.read<uint8_t>();

			if(iID == s_pPlayer->IDNumber()) // ?? ???? ????..
			{
				m_pUIVar->m_pPageKnights->UpdateKnightsDuty(eDuty);
			}
			else
			{
				CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(iID, false);
				if(pUPC) // ?????? ???? ????..
				{
				}
			}
		}
		break;
	default:
		__ASSERT(0, "Invalid Knights SubPacket");
		break;
*/	}
}

void CGameProcMain::MsgRecv_KnightsListBasic(Packet& pkt) // ?????? ???? ???? ????..
{
	e_SubPacket_KnightsList eSP = (e_SubPacket_KnightsList)(pkt.read<uint8_t>());	// Sub Packet
	switch(eSP)
	{
	case N3_SP_KNIGHTS_LIST_BASIC_ALL: // Receive - s1(knights Count) { s21(id, ????????), str1(????) }
		{
			int iCount = pkt.read<int16_t>();	// ?????? ????
			for(int i = 0; i < iCount; i++)
			{
				std::string szID;
				int iID = pkt.read<int16_t>();		// ?????? ID
				int iLen = pkt.read<int16_t>();		// ID ?????? ????..
				pkt.readString(szID, iLen);	// ID ??????..

				m_pUIKnightsOp->KnightsInfoInsert(iID, szID); // ?????? ???? ???? ?? ??????..
			}
		}
		break;
	case N3_SP_KNIGHTS_LIST_BASIC_INSERT: // Receive - s2(id, ????????), str1(????)
		{
			std::string szID;
			int iID = pkt.read<int16_t>();		// ?????? ID
			int iLen = pkt.read<int16_t>();		// ID ?????? ????..
			pkt.readString(szID, iLen);	// ID ??????..

			m_pUIKnightsOp->KnightsInfoInsert(iID, szID); // ?????? ???? ????..
		}
		break;
	case N3_SP_KNIGHTS_LIST_BASIC_REMOVE: // Receive - s1(id)
		{
			int iID = pkt.read<int16_t>();		// ?????? ID
			m_pUIKnightsOp->KnightsInfoDelete(iID); // ?????? ???? ??????..
		}
		break;
	}
}

void CGameProcMain::MsgRecv_ContinousPacket(Packet& pkt) // ?????? ?????? ????... ???? ?? ???????? ????!!!
{
	uint16_t iWholeSize;
	pkt >> iWholeSize;

	while (pkt.rpos() < iWholeSize)
	{
		uint16_t iSizeThisPacket;
		pkt >> iSizeThisPacket;

		if (iSizeThisPacket <= 0 || iSizeThisPacket >= iWholeSize)
		{
			__ASSERT(0, "Invalid continous packet");
			break; // ??????!!
		}

		Packet tempPacket;
		tempPacket.readFrom(pkt, iSizeThisPacket);
		ProcessPacket(tempPacket);
	}
}

void CGameProcMain::MsgRecv_WareHouse(Packet& pkt)			// ?????? ???? ????..
{
	uint8_t	bResult, bSubCom = pkt.read<uint8_t>();

	switch ( (e_SubPacket_WareHouse)bSubCom )
	{
		case N3_SP_WARE_INN:
			m_pUIInn->SetVisible(true);
			break;

		case N3_SP_WARE_OPEN:
			MsgRecv_WareHouseOpen(pkt);
			break;

		case N3_SP_WARE_GET_IN:
			bResult = pkt.read<uint8_t>();	// 0x01 : true, 0x00 : false..
			if (m_pUIWareHouseDlg) m_pUIWareHouseDlg->ReceiveResultToWareMsg(bResult);
			break;

		case N3_SP_WARE_GET_OUT:
			bResult = pkt.read<uint8_t>();	// 0x01 : true, 0x00 : false..
			if (m_pUIWareHouseDlg) m_pUIWareHouseDlg->ReceiveResultFromWareMsg(bResult);
			break;

		case N3_SP_WARE_WARE_MOVE:
			bResult = pkt.read<uint8_t>();	// 0x01 : true, 0x00 : false..
			if (m_pUIWareHouseDlg) m_pUIWareHouseDlg->ReceiveResultWareToWareMsg(bResult);
			break;

		case N3_SP_WARE_INV_MOVE:
			bResult = pkt.read<uint8_t>();	// 0x01 : true, 0x00 : false..
			if (m_pUIWareHouseDlg) m_pUIWareHouseDlg->ReceiveResultInvToInvMsg(bResult);
			break;
	}
}

void CGameProcMain::MsgRecv_WareHouseOpen(Packet& pkt)		// ?????? ????..
{
	if (m_pUIWareHouseDlg->IsVisible())
		return;

	Uint8 idk = pkt.read<uint8_t>();

	int iWareGold, iItemID, iItemDurability, iItemCount;
	iWareGold		= pkt.read<uint32_t>();
	m_pUIWareHouseDlg->EnterWareHouseStateStart(iWareGold);

	for ( int i = 0; i < MAX_ITEM_WARE_PAGE*MAX_ITEM_TRADE; i++ )				// ???? ????????..
	{
		iItemID			= pkt.read<uint32_t>();
		iItemDurability	= pkt.read<int16_t>();
		iItemCount		= pkt.read<int16_t>();
		m_pUIWareHouseDlg->AddItemInWare( iItemID, iItemDurability, iItemCount, i );
	}

	if (!m_pUIWareHouseDlg->IsVisible())
		m_pUIWareHouseDlg->SetVisible(true);

	if (m_pUIInventory->IsVisible())		// ?????????? ?????? ??????..
		this->CommandToggleUIInventory();

	if (m_pUISkillTreeDlg->IsVisible())
		m_pUISkillTreeDlg->Close();

	m_pUIWareHouseDlg->EnterWareHouseStateEnd();
}

void CGameProcMain::PlayBGM_Town()
{
	if(m_pSnd_Battle) m_pSnd_Battle->Stop(3.0f);
	if(NULL == m_pSnd_Town || m_pSnd_Town->IsPlaying()) return;
	m_pSnd_Town->SetMaxVolume(60);
	m_pSnd_Town->Play(NULL, 3.0f); // ???? ???? ????.. ?????? ?????? ???????? ?????? ????..
}

void CGameProcMain::PlayBGM_Battle()
{
	if(m_pSnd_Town) m_pSnd_Town->Stop(3.0f);
	if(NULL == m_pSnd_Battle || m_pSnd_Battle->IsPlaying()) return;
	m_pSnd_Battle->SetMaxVolume(80);
	m_pSnd_Battle->Play(NULL, 3.0f); // ???? ???? ????.. ?????? ?????? ???????? ?????? ????..
}

void CGameProcMain::ReleaseSound()
{
	if(ACT_WORLD && ACT_WORLD->GetSkyRef()) ACT_WORLD->GetSkyRef()->ReleaseSound();
	CN3Base::s_SndMgr.ReleaseStreamObj(&m_pSnd_Town);
	CN3Base::s_SndMgr.ReleaseStreamObj(&m_pSnd_Battle);
}

void CGameProcMain::MsgRecv_NpcChangeOpen(Packet& pkt)		// Class Change?? ??????..
{
	uint8_t	bSubCom = pkt.read<uint8_t>();

	switch (bSubCom)
	{
		case N3_SP_CLASS_CHANGE_PURE:
			m_pUINpcChange->Open();
			break;

		case N3_SP_CLASS_CHANGE_REQ:
			MsgRecv_ClassChange(pkt);
			break;

		case N3_SP_CLASS_ALL_POINT:
			MsgRecv_AllPointInit(pkt);
			break;

		case N3_SP_CLASS_SKILL_POINT:
			MsgRecv_SkillPointInit(pkt);
			break;

		case N3_SP_CLASS_POINT_CHANGE_PRICE_QUERY:
			MsgRecv_PointChangePriceQueryRequest(pkt);
			break;

		case N3_SP_CLASS_PROMOTION:
			MsgRecv_ClassPromotion(pkt);
			break;
	}
}

void CGameProcMain::MsgRecv_AllPointInit(Packet& pkt)			// All Point ??????..
{
	uint8_t	bType		= pkt.read<uint8_t>();	
	uint32_t	dwGold		= pkt.read<uint32_t>();	

	char szBuf[256] = "";
	std::string szMsg; 

	switch (bType)
	{
		case 0x00:	// ???? ????..
			::_LoadStringFromResource(IDS_POINTINIT_NOT_ENOUGH_NOAH, szMsg);
			sprintf(szBuf, szMsg.c_str(), dwGold);
			MsgOutput(szBuf, 0xffff3b3b);
			break;

		case 0x01:	// ????..
			s_pPlayer->m_InfoExt.iStrength = pkt.read<int16_t>();
			m_pUIVar->m_pPageState->UpdateStrength(s_pPlayer->m_InfoExt.iStrength, s_pPlayer->m_InfoExt.iStrength_Delta);

			s_pPlayer->m_InfoExt.iStamina = pkt.read<int16_t>();
			m_pUIVar->m_pPageState->UpdateStamina(s_pPlayer->m_InfoExt.iStamina, s_pPlayer->m_InfoExt.iStamina_Delta);

			s_pPlayer->m_InfoExt.iDexterity = pkt.read<int16_t>();
			m_pUIVar->m_pPageState->UpdateDexterity(s_pPlayer->m_InfoExt.iDexterity, s_pPlayer->m_InfoExt.iDexterity_Delta);

			s_pPlayer->m_InfoExt.iIntelligence = pkt.read<int16_t>();
			m_pUIVar->m_pPageState->UpdateIntelligence(s_pPlayer->m_InfoExt.iIntelligence, s_pPlayer->m_InfoExt.iIntelligence_Delta);

			s_pPlayer->m_InfoExt.iMagicAttak = pkt.read<int16_t>();
			m_pUIVar->m_pPageState->UpdateMagicAttak(s_pPlayer->m_InfoExt.iMagicAttak, s_pPlayer->m_InfoExt.iMagicAttak_Delta);

			s_pPlayer->m_InfoBase.iHPMax =		pkt.read<int16_t>();
			s_pPlayer->m_InfoExt.iMSPMax =		pkt.read<int16_t>();
			s_pPlayer->m_InfoExt.iAttack =		pkt.read<int16_t>();
			s_pPlayer->m_InfoExt.iWeightMax =	pkt.read<int16_t>();

			m_pUIVar->m_pPageState->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax);
			m_pUIStateBarAndMiniMap->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax, false);
			
			m_pUIVar->m_pPageState->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax);
			m_pUIStateBarAndMiniMap->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax, false);

			m_pUIVar->m_pPageState->UpdateAttackPoint(s_pPlayer->m_InfoExt.iAttack, s_pPlayer->m_InfoExt.iAttack_Delta);
			m_pUIVar->m_pPageState->UpdateWeight(s_pPlayer->m_InfoExt.iWeight, s_pPlayer->m_InfoExt.iWeightMax);

			s_pPlayer->m_InfoExt.iBonusPointRemain = pkt.read<int16_t>(); // ???? ?????? ??????..
			m_pUIVar->m_pPageState->UpdateBonusPointAndButtons(s_pPlayer->m_InfoExt.iBonusPointRemain); // ?????? ?????? ?????? ??????????

			// ?? ????.. ?????????? ?????? ??????.. ???? ???????? ??????..
			s_pPlayer->m_InfoExt.iGold = dwGold;
			if (m_pUIInventory->IsVisible())
				m_pUIInventory->GoldUpdate();
			if (m_pUITransactionDlg->IsVisible())
				m_pUITransactionDlg->GoldUpdate();
			if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible())
				m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();

			break;

		case 0x02:	// Already..
			::_LoadStringFromResource(IDS_POINTINIT_ALREADY, szMsg);
			sprintf(szBuf, szMsg.c_str());
			MsgOutput(szBuf, 0xffff3b3b);
			break;
	}
}

void CGameProcMain::MsgRecv_SkillPointInit(Packet& pkt)		// Skill Point ??????..
{
	uint8_t	bType		= pkt.read<uint8_t>();	
	uint32_t	dwGold		= pkt.read<uint32_t>();	
	int i;
	char szBuf[256] = "";
	std::string szMsg; 

	switch (bType)
	{
		case 0x00:	// ???? ????..
			::_LoadStringFromResource(IDS_POINTINIT_NOT_ENOUGH_NOAH, szMsg);
			sprintf(szBuf, szMsg.c_str(), dwGold);
			MsgOutput(szBuf, 0xffff3b3b);
			break;

		case 0x01:	// ????..
			m_pUISkillTreeDlg->m_iSkillInfo[0] = pkt.read<uint8_t>();
			for ( i = 1; i < 9; i++ )
				m_pUISkillTreeDlg->m_iSkillInfo[i] = 0;
			m_pUISkillTreeDlg->InitIconUpdate();

			// ?? ????.. ?????????? ?????? ??????.. ???? ???????? ??????..
			s_pPlayer->m_InfoExt.iGold = dwGold;
			if (m_pUIInventory->IsVisible())
				m_pUIInventory->GoldUpdate();
			if (m_pUITransactionDlg->IsVisible())
				m_pUITransactionDlg->GoldUpdate();
			if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible())
				m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();

			if (m_pUIHotKeyDlg)
				m_pUIHotKeyDlg->AllFactorClear();
			break;

		case 0x02:	// Already..
			::_LoadStringFromResource(IDS_POINTINIT_ALREADY, szMsg);
			sprintf(szBuf, szMsg.c_str());
			MsgOutput(szBuf, 0xffff3b3b);
			break;
	}
}

void CGameProcMain::MsgRecv_PointChangePriceQueryRequest(Packet& pkt)		// ?????? ???? ???? ????..
{
	uint32_t	dwGold		= pkt.read<uint32_t>();	
	m_pUINpcChange->ReceivePriceFromServer(dwGold);
}

void CGameProcMain::MsgRecv_NoahChange(Packet& pkt)		// ???? ????..
{
	uint8_t	bType				= pkt.read<uint8_t>();	
	uint32_t	dwGoldOffset	= pkt.read<uint32_t>();		
	uint32_t	dwGold			= pkt.read<uint32_t>();	

	char szBuf[256] = "";
	std::string szMsg; 

	switch (bType)
	{
		case N3_SP_NOAH_GET:
			::_LoadStringFromResource(IDS_NOAH_CHANGE_GET, szMsg);
			sprintf(szBuf, szMsg.c_str(), dwGoldOffset);
			MsgOutput(szBuf, 0xff6565ff);
			break;

		case N3_SP_NOAH_LOST:
			::_LoadStringFromResource(IDS_NOAH_CHANGE_LOST, szMsg);
			sprintf(szBuf, szMsg.c_str(), dwGoldOffset);
			MsgOutput(szBuf, 0xffff3b3b);
			break;

		case N3_SP_NOAH_SPEND:
			::_LoadStringFromResource(IDS_NOAH_CHANGE_SPEND, szMsg);
			sprintf(szBuf, szMsg.c_str(), dwGoldOffset);
			MsgOutput(szBuf, 0xffff3b3b);
			break;
	}
	
	s_pPlayer->m_InfoExt.iGold = dwGold;
	if (m_pUIInventory->IsVisible())
		m_pUIInventory->GoldUpdate();
	if (m_pUITransactionDlg->IsVisible())
		m_pUITransactionDlg->GoldUpdate();
	if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible())
		m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();
}

void CGameProcMain::MsgRecv_WarpList(Packet& pkt)		// ???? ?????? - ?? ???????? ?? ???? ????..
{
	int iByte = pkt.read<uint8_t>();

	m_pUIWarp->Reset();

	int iStrLen = 0;

	int iListCount = pkt.read<int16_t>();

	// if there are no warp info (if m_bZoneChangeSameZone is true) - No need to show empty list. 
	if (iListCount == 0)
		return;

	for(int i = 0; i < iListCount; i++)
	{
		__WarpInfo WI;
		
		WI.iID = pkt.read<int16_t>(); // ???? ID
		iStrLen = pkt.read<int16_t>(); // ???? ????
		pkt.readString(WI.szName, iStrLen); // ????
		iStrLen = pkt.read<int16_t>(); // ?????? ????
		pkt.readString(WI.szAgreement, iStrLen); // ??????
		WI.iZone = pkt.read<int16_t>();				// ??????
		WI.iMaxUser = pkt.read<int16_t>();			// ???? ???? ??????.
		WI.iGold = pkt.read<uint32_t>();				// ??
		WI.vPos.x = (pkt.read<int16_t>())/10.0f;	// ???? 
		WI.vPos.z = (pkt.read<int16_t>())/10.0f;	//
		WI.vPos.y = (pkt.read<int16_t>())/10.0f;	// 

		m_pUIWarp->InfoAdd(WI);
	}

	m_pUIWarp->UpdateList();
	m_pUIWarp->SetVisible(true);
}

/*
void CGameProcMain::MsgRecv_ServerCheckAndRequestConcurrentUserCount(Packet& pkt)	// ???? IP ?? ?????? ???? ???????? ?????? ????..
{
	std::string szIP;
	int iStrLen = pkt.read<int16_t>(); // IP..
	pkt.readString(szIP, iStrLen);
	uint32_t dwPort = pkt.read<int16_t>(); // Port

	__WarpInfo WI;
	if(m_pUIWarp->InfoGetCur(WI) < 0) return;

	bool bNeedConnectSubSocket = (szIP != s_pSocket->GetCurrentIP() || dwPort != s_pSocket->GetCurrentPort()); // ???????? ?? IP ?? ?????? ????????

	if(bNeedConnectSubSocket) // ???? ???????? ???????? ????..
	{
		int iErr = s_pSocketSub->Connect(s_hWndSubSocket, szIP.c_str(), dwPort); // ???? ???????? ????????..
		if(iErr)
		{
			this->ReportServerConnectionFailed(WI.szName, iErr, false);
			return;
		}
	}

	// ?????? ????..
	int iOffsetSend = 0;
	uint8_t byBuff[8];
	
	CAPISocket::MP_AddByte(byBuff, iOffsetSend, WIZ_ZONE_CONCURRENT);
	CAPISocket::MP_AddShort(byBuff, iOffsetSend, WI.iZone);
	CAPISocket::MP_AddByte(byBuff, iOffsetSend, s_pPlayer->m_InfoBase.eNation); // ?????? ??????..

	if(bNeedConnectSubSocket) s_pSocketSub->Send(byBuff, iOffsetSend); // ???? ???????? ??????.
	else s_pSocket->Send(byBuff, iOffsetSend); // ?? ???????? ??????..
}


void CGameProcMain::MsgRecv_ConcurrentUserCountAndSendServerCheck(Packet& pkt)			// ???????? ???? ?????? ???????????? ?????? ??????.
{
	int iConcurrentUser = pkt.read<int16_t>(); // IP..
	if(s_pSocketSub->IsConnected()) s_pSocketSub->Disconnect();

	__WarpInfo WI;
	if(m_pUIWarp->InfoGetCur(WI) < 0) return;

	if(iConcurrentUser < WI.iMaxUser) // ???? ???????? ??????..
	{
		int iOffsetSend = 0;
		uint8_t byBuff[8];
		
		CAPISocket::MP_AddByte(byBuff, iOffsetSend, WIZ_VIRTUAL_SERVER);
		CAPISocket::MP_AddShort(byBuff, iOffsetSend, WI.iID);

		s_pSocket->Send(byBuff, iOffsetSend);
	}
	else
	{
		std::string szMsg; ::_LoadStringFromResource(IDS_MSG_CONCURRENT_USER_OVERFLOW, szMsg); // ???? ???? ???? ????..
		this->MsgOutput(szMsg, 0xffff0000);
	}
}
*/

void CGameProcMain::MsgRecv_Knights_Create(Packet& pkt)
{
	uint8_t	bSubCom = pkt.read<uint8_t>();

	switch ( (e_SubPacket_KNights_Create)bSubCom )
	{
		case N3_SP_KNIGHTS_CREATE_SUCCESS:
			{
				int sid = pkt.read<int16_t>();
				std::string szID;
				int iID = pkt.read<int16_t>();		// ?????? ID
				int iLen = pkt.read<int16_t>();		// ID ?????? ????..
				pkt.readString(szID, iLen);	// ID ??????..
				int iGrade = pkt.read<uint8_t>();	// ????
				int iRank = pkt.read<uint8_t>();		// ????
				uint32_t dwGold = pkt.read<uint32_t>();

				if(s_pPlayer->IDNumber()==sid)
				{
					m_pUIInn->Message(/*IDS_CLAN_MAKE_SUCCESS*/6505);

					// ?? ????.. ?????????? ?????? ??????.. ???? ???????? ??????..
					s_pPlayer->m_InfoExt.iGold = dwGold;
					if (m_pUIInventory->IsVisible()) m_pUIInventory->GoldUpdate();
					if (m_pUITransactionDlg->IsVisible()) m_pUITransactionDlg->GoldUpdate();
					if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible())
						m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();

					//??????(????)UI????????...????...
					s_pPlayer->m_InfoExt.eKnightsDuty = KNIGHTS_DUTY_CHIEF;
					s_pPlayer->KnightsInfoSet(iID, szID, iGrade, iRank);
					m_pUIVar->UpdateKnightsInfo();

					if(m_pUIVar->m_pPageKnights->IsVisible())
					{
						m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
						m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
					}

					//m_pUIKnightsOp->KnightsInfoInsert(iID, szID); // ?????? ???? ????..
				}
				else
				{
					CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(sid, true);
					if(pUPC)
						pUPC->KnightsInfoSet(iID, szID, iGrade, iRank);
				}
			}
			break;
		case N3_SP_KNIGHTS_CREATE_FAIL_LOWLEVEL:
			m_pUIInn->Message(/*IDS_CLAN_DENY_LOWLEVEL*/6500);
			break;
		case N3_SP_KNIGHTS_CREATE_FAIL_DUPLICATEDNAME:
			m_pUICreateClanName->Open(/*IDS_CLAN_REINPUT_NAME*/6508);
			break;

		case N3_SP_KNIGHTS_CREATE_FAIL_LOWMONEY:
			m_pUIInn->Message(/*IDS_CLAN_DENY_LOWGOLD*/6501);
			break;
		case N3_SP_KNIGHTS_CREATE_FAIL_ALREADYJOINED:
			m_pUIInn->Message(/*IDS_CLAN_DENY_ALREADYJOINED*/6503);
			break;
		case N3_SP_KNIGHTS_CREATE_FAIL_INVALIDDAY:
			m_pUIInn->Message(/*IDS_CLAN_DENY_INVALIDDAY*/6502);
			break;
		case N3_SP_KNIGHTS_CREATE_FAIL_DBFAIL:
		case N3_SP_KNIGHTS_CREATE_FAIL_UNKNOWN:
			m_pUIInn->Message(/*IDS_CLAN_DENY_UNKNOWN*/6504);
			break;
		case N3_SP_KNIGHTS_CREATE_FAIL_INVALIDSERVER:
			m_pUIInn->Message(/*IDS_CLAN_DENY_INVALID_SERVER*/6527);
			break;
	}
}

void CGameProcMain::MsgRecv_Knights_Withdraw(Packet& pkt)
{
	uint8_t	bSubCom = pkt.read<uint8_t>();

	std::string szMsg;
	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_DBFAIL:
		break;
	case N3_SP_KNIGHTS_COMMON_SUCCESS:
		{
			int sid = pkt.read<int16_t>();
			if(s_pPlayer->IDNumber()==sid)
			{
				s_pPlayer->m_InfoExt.iKnightsID = pkt.read<int16_t>();
				s_pPlayer->m_InfoExt.eKnightsDuty = (e_KnightsDuty)pkt.read<uint8_t>();
				m_pUIVar->UpdateKnightsInfo();

				s_pPlayer->KnightsInfoSet(s_pPlayer->m_InfoExt.iKnightsID, "", 0, 0);
				::_LoadStringFromResource(IDS_CLAN_WITHDRAW_SUCCESS, szMsg);
				this->MsgOutput(szMsg, 0xffffff00);

				if(m_pUIVar->m_pPageKnights->IsVisible())
				{
					m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
					m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
				}
			}
			else
			{
				int iKnightsID = pkt.read<int16_t>();
				e_KnightsDuty eKnightsDuty = (e_KnightsDuty)pkt.read<uint8_t>();

				CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(sid, true);
				if(pUPC)
				{
					//__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(iKnightsID);
					//if(pKIB) pUPC->KnightsNameSet(pKIB->szName, 0xffff0000);
					//else pUPC->KnightsNameSet("", 0xffff0000);
					pUPC->KnightsInfoSet(iKnightsID, "", 0, 0);
				}
			}
		}
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER:
	case N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER:
	case N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER:
	case N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER:
	case N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT:
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN:
	case N3_SP_KNIGHTS_COMMON_FAIL_FULL:
	case N3_SP_KNIGHTS_COMMON_FAIL_ME:
	case N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED:
		::_LoadStringFromResource(IDS_CLAN_WITHDRAW_FAIL, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE:
		::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_BATTLEZONE, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	}
}

void CGameProcMain::MsgRecv_Knights_Join(Packet& pkt)
{
	uint8_t	bSubCom = pkt.read<uint8_t>();

	std::string szMsg;
	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB???? ????..
		break;
	case N3_SP_KNIGHTS_COMMON_SUCCESS: //???????? ????
		{
			int sid = pkt.read<int16_t>();
			int iID = pkt.read<int16_t>();
			e_KnightsDuty eDuty = (e_KnightsDuty)pkt.read<uint8_t>();
			int iL = pkt.read<int16_t>(); // ???? ?????? ???? ????.
			std::string szKnightsName;
			pkt.readString(szKnightsName, iL);
			int iGrade = pkt.read<uint8_t>();	// ????
			int iRank = pkt.read<uint8_t>();		// ????

			if(s_pPlayer->IDNumber()==sid)
			{
				s_pPlayer->m_InfoExt.eKnightsDuty = eDuty;
				s_pPlayer->KnightsInfoSet(iID, szKnightsName, iGrade, iRank);
				m_pUIVar->UpdateKnightsInfo();

				::_LoadStringFromResource(IDS_CLAN_JOIN_SUCCESS, szMsg);
				this->MsgOutput(szMsg, 0xffffff00);

				if(m_pUIVar->m_pPageKnights->IsVisible())
				{
					m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
					m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
				}
			}
			else
			{
				CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(sid, true);								
				if(pUPC)
					pUPC->KnightsInfoSet(iID, szKnightsName, iGrade, iRank);
			}
		}
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER:	//???? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER:	//?????????? ???? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_DEAD_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER: //?????????? ?????? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_ENEMY_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER: //?????????? ???? ???? ???????? ???????? ???????? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_OTHER_CLAN_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT: //?????? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_INVALIDRIGHT, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN:	//???????? ???? ??????..									
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_CLAN, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_FULL:	//?????? ??..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_CLAN_FULL, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_ME:
		::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_ME, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED:
		::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_NOTJOINED, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_REJECT:
		::_LoadStringFromResource(IDS_CLAN_JOIN_REJECT, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE:
		::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_BATTLEZONE, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	}	
}

void CGameProcMain::MsgRecv_Knights_Leave(Packet& pkt)
{
	uint8_t	bSubCom = pkt.read<uint8_t>();

	std::string szMsg;
	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB???? ????..
		break;
	case N3_SP_KNIGHTS_COMMON_SUCCESS: //???????? ????
		{
			int sid = pkt.read<int16_t>();
			int iID = pkt.read<int16_t>();
			e_KnightsDuty eDuty = (e_KnightsDuty)pkt.read<uint8_t>();
			int iL = pkt.read<int16_t>(); // ???? ?????? ???? ????.
			std::string szKnightsName;
			pkt.readString(szKnightsName, iL);
			int iGrade = pkt.read<uint8_t>();	// ????
			int iRank = pkt.read<uint8_t>();		// ????

			if(s_pPlayer->IDNumber()==sid)
			{
				s_pPlayer->m_InfoExt.eKnightsDuty = eDuty;
				s_pPlayer->KnightsInfoSet(iID, szKnightsName, iGrade, iRank);
				m_pUIVar->UpdateKnightsInfo();
				
				::_LoadStringFromResource(IDS_CLAN_JOIN_SUCCESS, szMsg);
				this->MsgOutput(szMsg, 0xffffff00);

				if(m_pUIVar->m_pPageKnights->IsVisible())
				{
					m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
					m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
				}
			}
			else
			{
				CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(sid, true);								
				if(pUPC)
					pUPC->KnightsInfoSet(iID, "", 0, 0);
			}
		}
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER:	//???? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER:	//?????????? ???? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_DEAD_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER: //?????????? ?????? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_ENEMY_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER: //?????????? ???? ???? ???????? ???????? ???????? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_OTHER_CLAN_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT: //?????? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_INVALIDRIGHT, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN:	//???????? ???? ??????..									
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_CLAN, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_FULL:	//?????? ??..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_CLAN_FULL, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_ME:
		::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_ME, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED:
		::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_NOTJOINED, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE:
		::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_BATTLEZONE, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	}	
}

void CGameProcMain::MsgRecv_Knights_AppointViceChief(Packet& pkt)
{
	uint8_t	bSubCom = pkt.read<uint8_t>();

	std::string szMsg;
	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB???? ????..
		break;
	case N3_SP_KNIGHTS_COMMON_SUCCESS: //???????? ????
		{
			int iID = pkt.read<int16_t>();
			e_KnightsDuty eDuty = (e_KnightsDuty)pkt.read<uint8_t>();

			s_pPlayer->m_InfoExt.iKnightsID = iID;
			s_pPlayer->m_InfoExt.eKnightsDuty = eDuty;
			m_pUIVar->UpdateKnightsInfo();

			::_LoadStringFromResource(IDS_CLAN_JOIN_SUCCESS, szMsg);
			this->MsgOutput(szMsg, 0xffffff00);

			if(m_pUIVar->m_pPageKnights->IsVisible())
			{
				m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
				m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
			}
		}
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER:	//???? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER:	//?????????? ???? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_DEAD_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER: //?????????? ?????? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_ENEMY_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER: //?????????? ???? ???? ???????? ???????? ???????? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_OTHER_CLAN_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT: //?????? ????..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_INVALIDRIGHT, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN:	//???????? ???? ??????..									
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_CLAN, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_FULL:	//?????? ??..
		::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_CLAN_FULL, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_ME:
		::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_ME, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED:
		::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_NOTJOINED, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE:
		::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_BATTLEZONE, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	}	
}

void CGameProcMain::MsgRecv_Knights_MemberInfoAll(Packet& pkt)
{
	uint8_t	bSubCom = pkt.read<uint8_t>();

	std::string szMsg;
	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_SUCCESS:
		m_pUIVar->m_pPageKnights->MsgRecv_MemberInfo(pkt);
		break;
	case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB???? ????..
	default:
		break;		
	}
}

void CGameProcMain::MsgRecv_Knights_GradeChangeAll(Packet& pkt)
{
	int iCount = pkt.read<int16_t>();
	if(iCount <= 0) return;
	
	std::vector<int> iIDs(iCount, 0);
	std::vector<int> iGrades(iCount, 0);
	std::vector<int> iRanks(iCount, 0);

	for(int i = 0; i < iCount; i++)
	{
		iIDs[i] = pkt.read<int16_t>();
		iGrades[i] = pkt.read<uint8_t>();
		iRanks[i] = pkt.read<uint8_t>();
	}

	it_UPC it = s_pOPMgr->m_UPCs.begin(), itEnd = s_pOPMgr->m_UPCs.end();
	for(; it != itEnd; it++)
	{
		CPlayerOther* pUPC = it->second;
		if(NULL == pUPC) continue;

		int iIDTmp = pUPC->m_InfoExt.iKnightsID;
		if(iIDTmp <= 0) continue;

		for(int i = 0; i < iCount; i++)
		{
			if(iIDs[i] == iIDTmp)
			{
				pUPC->KnightsInfoSet(iIDTmp, pUPC->m_InfoExt.szKnights, iGrades[i], iRanks[i]);
				break;
			}
		}
	}

}

void CGameProcMain::MsgRecv_Knights_Duty_Change(Packet& pkt)
{
	uint8_t bSubCom = pkt.read<uint8_t>();

	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_SUCCESS:
		{
			int sid = pkt.read<int16_t>();
			int iID = pkt.read<int16_t>();
			e_KnightsDuty eDuty = (e_KnightsDuty)pkt.read<uint8_t>();

			if(s_pPlayer->IDNumber()==sid)
			{
				s_pPlayer->m_InfoExt.iKnightsID = iID;
				s_pPlayer->m_InfoExt.eKnightsDuty = eDuty;
				m_pUIVar->UpdateKnightsInfo();
				if(s_pPlayer->m_InfoExt.iKnightsID == 0)
					s_pPlayer->KnightsInfoSet(0, "", 0, 0);
				//std::string szName;
				//__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(s_pPlayer->m_InfoExt.iKnightsID);
				//if(pKIB) m_pUIVar->m_pPageKnights->UpdateKnightsName(pKIB->szName);
				//else m_pUIVar->m_pPageKnights->UpdateKnightsName("");
			}
			else
			{
				CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(sid, true);								
				if(pUPC)
					if(iID == 0) pUPC->KnightsInfoSet(0, "", 0, 0);
			}
		}
		break;
	}
}

void CGameProcMain::MsgRecv_Knigts_Join_Req(Packet& pkt)
{
	uint8_t bSubCom = pkt.read<uint8_t>();

	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_SUCCESS:
		{
			m_iJoinReqClanRequierID = pkt.read<int16_t>();
			m_iJoinReqClan = pkt.read<int16_t>();

			int iL = pkt.read<int16_t>(); // ???? ?????? ???? ????.
			std::string szKnightsName;
			pkt.readString(szKnightsName, iL);

//			std::string szName;
//			__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(m_iJoinReqClan);
//			if(!pKIB)
//			{
//				MsgSend_KnightsJoinReq(false);
//				break;
//			}

			std::string szMsg;
			::_LoadStringFromResource(IDS_CLAN_JOIN_REQ, szMsg);

			char szTmp[256];
			sprintf(szTmp, szMsg.c_str(), szKnightsName.c_str());
			
			CGameProcedure::s_pProcMain->MessageBoxPost(szTmp, "", MB_YESNO, BEHAVIOR_CLAN_JOIN);			
		}
		break;
	}
}

int CGameProcMain::MsgRecv_VersionCheck(Packet& pkt) // virtual
{
	int iVersion = CGameProcedure::MsgRecv_VersionCheck(pkt);
	this->MsgSend_CharacterSelect(); // virtual

	return iVersion;
}

bool CGameProcMain::MsgRecv_CharacterSelect(Packet& pkt) // virtual
{
	bool bSuccess = CGameProcedure::MsgRecv_CharacterSelect(pkt);

	//?????????? ?????? ???? ?????? ???? ?????? ???? ???? ??????????.
	if(s_pPlayer->IsDead())
	{
		this->InitPlayerPosition(s_pPlayer->Position()); // ???????? ???? ??????.. ?????? ??????, ?????????? ?????? ????.
		s_pPlayer->RegenerateCollisionMesh(); // ???? ?????? ???? ??????..
		s_pPlayer->m_iSendRegeneration = 0; // ???? ?????? ???? ?????????? ???????? ??????
		s_pPlayer->m_fTimeAfterDeath = 0; // ???? ?????? ???? ?????????? ???????? ??????

		//
		//???? & ???? ??????..
		if(m_pUIStateBarAndMiniMap) m_pUIStateBarAndMiniMap->ClearMagic();
		if(m_pMagicSkillMng) m_pMagicSkillMng->ClearDurationalMagic();
		if(CGameProcedure::s_pFX) s_pFX->StopMine();

		CLogWriter::Write("Receive Regeneration");

		if(s_pPlayer->Nation()==NATION_KARUS) CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_KARUS, s_pPlayer->IDNumber(), -1);
		else if(s_pPlayer->Nation()==NATION_ELMORAD) CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_ELMORAD, s_pPlayer->IDNumber(), -1);
	}
			

	this->InitZone(s_pPlayer->m_InfoExt.iZoneCur, s_pPlayer->Position()); // ???? ??????..
	s_pProcMain->MsgSend_GameStart(); // ???????? ?????? ???? ????..

	return bSuccess;
}

void CGameProcMain::MsgRecv_Corpse(Packet& pkt)
{
	int iID	= pkt.read<int16_t>();//regen?????? ???? ?????? ??????

	if( s_pPlayer->IDNumber() != iID )
	{
		s_pOPMgr->CorpseAdd( iID );

		if( s_pPlayer->m_iIDTarget == iID )
			s_pPlayer->m_iIDTarget = -1; //???? ???? regen?? ???????? ???????? ?????? ?????? ?????? ??????.
	}
}

void CGameProcMain::MsgSend_PerTradeBBSReq(std::string szName, int iDestID)
{
	if(	!m_pUITransactionDlg->IsVisible() ) //// ?????? ?????? ????.. // ?????? ???? ??????..
	{
		std::string szMsg;
		::_LoadStringFromResource(IDS_PERSONAL_TRADE_REQUEST, szMsg);
		MsgOutput(szName + szMsg, 0xffffff00);

		MsgSend_PerTradeReq(iDestID, false);

		if (m_pUINpcEvent->IsVisible())
			m_pUINpcEvent->Close();
		m_pSubProcPerTrade->EnterWaitMsgFromServerStatePerTradeReq(szName);
	}
}

void CGameProcMain::MsgSend_CharacterSelect() // virtual
{
	CGameProcedure::MsgSend_CharacterSelect();
	
	m_pMagicSkillMng->ClearDurationalMagic();
	s_pFX->ClearAll();
}

void CGameProcMain::ControlViewVDegree(int16_t sValue)
{
	if(s_pEng->ViewPoint() == VP_THIRD_PERSON)
	{
		float fPitch = 0;
		if(sValue > 0)	fPitch = D3DXToRadian(45.0f)*3;
		else			fPitch = D3DXToRadian(-45.0f)*3;
		if(fPitch) s_pEng->CameraPitchAdd(fPitch);
	}
}
/*
bool CGameProcMain::OnMouseMove(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	if(ptCur.x != ptPrev.x || ptCur.y != ptPrev.y)
	{
		if(m_pMagicSkillMng->m_dwRegionMagicState==1)
		{
			__Vector3 vPick;
			CPlayerNPC*	 pTarget = NULL;
		
			int iID = -1;
			pTarget = s_pOPMgr->Pick(ptCur.x, ptCur.y, iID, &vPick); // ?????? ???????? ????..
			this->TargetSelect(iID, false); // ?????? ??????..
			if(NULL == pTarget) // ?????? ??????..
			{
				s_pPlayer->m_pObjectTarget = ACT_WORLD->PickWithShape(ptCur.x, ptCur.y, true, &vPick); // ???? ?????? ????????..
				if(NULL == s_pPlayer->m_pObjectTarget) // ?????? ??????..
				{
					ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vPick); // ?????? ????????..
				}
			}
			s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, vPick);
		}
	}

	return true;
}
*/
bool CGameProcMain::OnMouseMove(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	// NOTE: check if cursor position has changed
	if(ptCur.x != ptPrev.x || ptCur.y != ptPrev.y)
	{
		// NOTE: check if something has been selected by the cursor
		if(m_pMagicSkillMng->m_dwRegionMagicState==1)
		{
			__Vector3 vNormal, vMyPos, vGap, vDir;

			vMyPos = s_pPlayer->Position();
			vMyPos.y += s_pPlayer->Height() / 2;

			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, m_vMouseSkillPos); // ?????? ????????..

			vDir = m_vMouseSkillPos - vMyPos;
			vGap = vDir;
			vDir.Normalize();

			bool bColShape = ACT_WORLD->CheckCollisionWithShape(vMyPos, vDir, vGap.Magnitude(), &m_vMouseSkillPos, &vNormal);

			if(!bColShape) // ?????? ??????..
			{
				int iID = -1;
				s_pOPMgr->Pick(ptCur.x, ptCur.y, iID, &m_vMouseSkillPos); // ?????? ???????? ????..
				this->TargetSelect(iID, false); // ?????? ??????..
			}
			else
			{
				m_vMouseSkillPos.y = ACT_WORLD->GetHeightWithTerrain(m_vMouseSkillPos.x, m_vMouseSkillPos.z);
			}
			s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, m_vMouseSkillPos);
		}
	}

	return true;
}

// ???? ???? ????
bool CGameProcMain::OnMouseLDBtnPress(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, true);

	if(pTarget && pTarget->m_InfoBase.iAuthority == AUTHORITY_MANAGER)
	{
		s_pPlayer->m_iIDTarget = -1;
		pTarget = NULL;
	}

	if(VP_THIRD_PERSON == s_pEng->ViewPoint())
	{
		if(s_pPlayer->IsAttackableTarget(pTarget, false))
		{
			this->CommandMove(MD_STOP, true);
			this->CommandEnableAttackContinous(true, pTarget); // ???? ????
		}
		else if(pTarget && VP_THIRD_PERSON == s_pEng->ViewPoint())
		{
			this->CommandMove(MD_FOWARD, true);
			s_pPlayer->SetMoveTargetID(s_pPlayer->m_iIDTarget);
		}
	}
	else
	{
		s_pPlayer->m_bAttackContinous = false;
		CommandToggleAttackContinous();
	}
	return true;
}

// ???? ????
bool CGameProcMain::OnMouseLBtnPress(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	POINT ptPlayer = ::_Convert3D_To_2DCoordinate(	s_pPlayer->Position(), 
													CN3Base::s_CameraData.mtxView,
													CN3Base::s_CameraData.mtxProjection,
													CN3Base::s_CameraData.vp.Width,
													CN3Base::s_CameraData.vp.Height);
	
	__Vector3 vDir((float)(ptCur.x - ptPlayer.x), 0, (float)(ptPlayer.y - ptCur.y));
	__Matrix44 mtxTmp; mtxTmp.RotationY(s_pEng->CameraYaw());
	vDir *= mtxTmp;
	vDir.Normalize();
	float fYaw = ::_Yaw2D(vDir.x, vDir.z);

	CPlayerNPC*	 pTarget			= NULL;
	BOOL		 bFindCorpse		= false;

	int iID = -1;
	pTarget = s_pOPMgr->PickPrecisely(ptCur.x, ptCur.y, iID, &m_vMouseLBClickedPos); // ?????? ???????? ????..
	this->TargetSelect(iID, false); // ?????? ??????..
	if(NULL == pTarget) // ?????? ??????..
	{
		if(s_pPlayer->m_bAttackContinous) // ???? ???????? ??????..
			this->CommandEnableAttackContinous(false, NULL); // ???? ???? ????..

		s_pPlayer->m_pObjectTarget = ACT_WORLD->PickWithShape(ptCur.x, ptCur.y, true, &m_vMouseLBClickedPos); // ???? ?????? ????????..
		if(NULL == s_pPlayer->m_pObjectTarget) // ?????? ??????..
		{
			// ???? ?????? ?????? ???? ????..
			CPlayerNPC* pCorpse = s_pOPMgr->PickCorpse(ptCur.x, ptCur.y, iID); // ????..
			if(false == this->MsgSend_RequestItemBundleOpen(pCorpse)) // ???? ?????? ????..
			{
				ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, m_vMouseLBClickedPos); // ?????? ????????..
			}
			else
				bFindCorpse = true;
		}
	}

	if(m_pMagicSkillMng->m_dwRegionMagicState==1)
	{
//		s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, m_vMouseLBClickedPos);
		s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, m_vMouseSkillPos);
		m_pMagicSkillMng->m_dwRegionMagicState = 2;
	}

	if(!s_pPlayer->IsDead() && VP_THIRD_PERSON == s_pEng->ViewPoint())
	{
		m_fLBClickTime = CN3Base::TimeGet();
		this->CommandSitDown(false, false); // ???? ?????? ??????..

		if(pTarget)
		{
			if(s_pLocalInput->IsKeyDown(SDL_SCANCODE_LCTRL) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_RCTRL))
			{
				if(s_pPlayer->IsAttackableTarget(pTarget, false))
				{
					if(s_pPlayer->m_bTargetOrPosMove)
						this->CommandMove(MD_STOP, true);

					this->CommandEnableAttackContinous(true, pTarget); // ???? ????
				}
				else
				{
					this->CommandMove(MD_FOWARD, true);
					s_pPlayer->SetMoveTargetID(s_pPlayer->m_iIDTarget);
				}
			}
			else
			{
				if(s_pPlayer->m_bTargetOrPosMove && s_pPlayer->m_iMoveTarget != pTarget->IDNumber())
					this->CommandMove(MD_STOP, true);
			}
		}
		else if(!bFindCorpse)
		{
			s_pPlayer->RotateTo(fYaw, true);
			__Vector3 vMovePoint;
			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // ?????? ????????..

			float fDist = (vMovePoint - s_pPlayer->Position()).Magnitude();

			if(!s_pPlayer->m_bTargetOrPosMove && fDist > 1.5f)
				this->CommandMove(MD_FOWARD, true);

			if(fDist > 1.5f)
			{
				s_pPlayer->SetMoveTargetPos(vMovePoint);
			}
			else if(s_pPlayer->m_bTargetOrPosMove)
			{
				this->CommandMove(MD_STOP, true);
			}
		}
	}
	else if(!s_pPlayer->IsDead())
	{
		if(pTarget && (s_pLocalInput->IsKeyDown(SDL_SCANCODE_LCTRL) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_RCTRL)))
		{
			if(s_pPlayer->IsAttackableTarget(pTarget, false))
			{
				if(s_pPlayer->m_bTargetOrPosMove)
					this->CommandMove(MD_STOP, true);

				this->CommandEnableAttackContinous(true, pTarget); // ???? ????
			}
			else
			{
				s_pPlayer->m_bAttackContinous = false;
				CommandToggleAttackContinous();
			}
		}
	}

	return true;
}

// ???? ??????
bool CGameProcMain::OnMouseLBtnPressd(POINT ptCur, POINT ptPrev)
{
	if(!s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD))
	{
//		if(!s_pPlayer->m_bTargetOrPosMove)
//			this->CommandMove(MD_STOP, true);
	}
	return true;
}

// ???? ?????? ??????
bool CGameProcMain::OnMouseLbtnDown(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	POINT ptPlayer = ::_Convert3D_To_2DCoordinate(	s_pPlayer->Position(), 
													CN3Base::s_CameraData.mtxView,
													CN3Base::s_CameraData.mtxProjection,
													CN3Base::s_CameraData.vp.Width,
													CN3Base::s_CameraData.vp.Height);
	
	__Vector3 vDir((float)(ptCur.x - ptPlayer.x), 0, (float)(ptPlayer.y - ptCur.y));
	__Matrix44 mtxTmp; mtxTmp.RotationY(s_pEng->CameraYaw());
	vDir *= mtxTmp;
	vDir.Normalize();
	float fYaw = ::_Yaw2D(vDir.x, vDir.z);

	if(!s_pPlayer->IsDead() && VP_THIRD_PERSON == s_pEng->ViewPoint() && !s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD)) // ?????? ????????.. UI ?? ?????? ????????..
	{
		__Vector3 vMovePoint;
		float fDist;
		float fNowTime = CN3Base::TimeGet();

		if( fNowTime - m_fLBClickTime > 0.1f && s_pPlayer->m_bTargetOrPosMove)
		{
			if(s_pPlayer->m_bAttackContinous)
				CommandToggleAttackContinous();


			this->CommandSitDown(false, false); // ???? ?????? ??????..
			s_pPlayer->RotateTo(fYaw, true);
			this->CommandMove(MD_FOWARD, false);

			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // ?????? ????????..
			fDist = (vMovePoint - s_pPlayer->Position()).Magnitude();

			s_pPlayer->SetMoveTargetPos(vMovePoint);
		}
		else if( fNowTime - m_fLBClickTime > 0.1f && !s_pPlayer->m_bTargetOrPosMove)
		{
			if(s_pPlayer->m_bAttackContinous)
				CommandToggleAttackContinous();

			s_pPlayer->RotateTo(fYaw, true);
			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // ?????? ????????..

			fDist = (vMovePoint - s_pPlayer->Position()).Magnitude();

			if(fDist > 1.5f)
			{
				this->CommandMove(MD_FOWARD, true);
				s_pPlayer->SetMoveTargetPos(vMovePoint);
			}
		}
	}
	return true;
}

// ?????? ????
bool CGameProcMain::OnMouseRBtnPress(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	if(s_pLocalInput->IsKeyDown(SDL_SCANCODE_LCTRL) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_RCTRL))
	{
		m_pUIHotKeyDlg->EffectTriggerByMouse();
		return true;
	}

	int iID = -1;
	CPlayerNPC* pNPC = s_pOPMgr->PickNPC(ptCur.x, ptCur.y, iID); // ????..

	if(NULL == pNPC)
	{
		// NOTE: sending the packet to pick up items
		CPlayerNPC* pCorpse = s_pOPMgr->PickCorpse(ptCur.x, ptCur.y, iID); // ????..
		if(false == this->MsgSend_RequestItemBundleOpen(pCorpse)) // ???? ?????? ?????? ???? ????..
		{
			// NOTE: if no corpse and item box then check if the player clicked on an iteractable shape
			CN3Shape* pShape = ACT_WORLD->PickWithShape(ptCur.x, ptCur.y, true);
			if(	pShape && pShape == s_pPlayer->m_pObjectTarget && pShape->m_iEventID) // Event ?? ??????..
			{
				float fD = (s_pPlayer->Position() - pShape->Pos()).Magnitude(); 
				float fDLimit = (s_pPlayer->Radius() + pShape->Radius()) * 2.0f;
				if(fD > fDLimit) // ?????? ????
				{
					std::string szMsg; 
					if (OBJECT_TYPE_BINDPOINT == pShape->m_iEventType)
						::_LoadStringFromResource(IDS_BIND_POINT_REQUEST_FAIL, szMsg);
					else
						::_LoadStringFromResource(IDS_ERR_REQUEST_OBJECT_EVENT_SO_FAR, szMsg);
					this->MsgOutput(szMsg, 0xffff8080);
				}
				else
				{
					if(OBJECT_TYPE_BINDPOINT == pShape->m_iEventType)
					{
						std::string szMsg; ::_LoadStringFromResource(IDS_REQUEST_BINDPOINT, szMsg);
						this->MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_REQUEST_BINDPOINT); // ?????? ?????? ???? ?????? ????
					}
					else if(OBJECT_TYPE_WARP_POINT == pShape->m_iEventType)
					{
						this->MsgSend_ObjectEvent(pShape->m_iEventID, pShape->m_iNPC_ID); // ???????? ?????? ??????..
					}
				}
			}
		}
	}
	else if(pNPC->IDNumber() == s_pPlayer->m_iIDTarget) // ???????? ???? ???????? ?????? ???????? ???? ..) 
	{
		if(	pNPC->m_pShapeExtraRef ) // ???????? ?????? NPC ????.. ?????? ?? NPC?? ID ?? ??????..
		{
			if(pNPC->m_pShapeExtraRef->m_iNPC_ID > 0)
			{
				float fD = (s_pPlayer->Position() - pNPC->m_pShapeExtraRef->Pos()).Magnitude();
				float fDLimit = (s_pPlayer->Radius() + pNPC->m_pShapeExtraRef->Radius()) * 2.0f;
				if(fD > fDLimit) // ?????? ????
				{
					std::string szMsg;
					::_LoadStringFromResource(IDS_ERR_REQUEST_OBJECT_EVENT_SO_FAR, szMsg);
					this->MsgOutput(szMsg, 0xffff8080);
				}
				else
				{
					this->MsgSend_ObjectEvent(pNPC->m_pShapeExtraRef->m_iEventID, pNPC->IDNumber());
				}
			}
		}
		else // ???? NPC ????..
		{
			// NOTE: an NPC has been clicked on
			// TODO(srmeier): need to use ZoneAbilityType here
			// NOTE(srmeier): using the zone type to decide if you can talk with NPC
			if(ACT_WORLD->GetZoneType()==ZoneAbilityNeutral || (pNPC->m_InfoBase.eNation == s_pPlayer->m_InfoBase.eNation)) // ???? ???? ??????..
			{
				float fD = (s_pPlayer->Position() - pNPC->Position()).Magnitude();
				float fDLimit = (s_pPlayer->Radius() + pNPC->Radius()) * 3.0f;
				if(fD > fDLimit) // ?????? ????
				{
					std::string szMsg;
					::_LoadStringFromResource(IDS_ERR_REQUEST_NPC_EVENT_SO_FAR, szMsg);
					this->MsgOutput(szMsg, 0xffff8080);
				}
				else
				{
					s_pPlayer->ActionMove(PSM_STOP); // ???? ??????..
					pNPC->RotateTo(s_pPlayer); // ?????? ???????? ?????? ??????.
					this->MsgSend_NPCEvent(iID); // ?????? ????..
					if (m_pUITransactionDlg) m_pUITransactionDlg->m_iNpcID = pNPC->IDNumber();
				}
			}
		}
	}
	return true;
}

// ?????? ??????
bool CGameProcMain::OnMouseRBtnPressd(POINT ptCur, POINT ptPrev)
{
	return true;
}

// ?????? ?????? ??????
bool CGameProcMain::OnMouseRbtnDown(POINT ptCur, POINT ptPrev)
{
	float fMouseSensivity = 0.02f;//0.05f;//
	
	float fRotY = D3DXToRadian(180.0f) * ((ptCur.x - ptPrev.x) * fMouseSensivity); // ?????? ???? ????????..
	float fRotX = D3DXToRadian(180.0f) * ((ptCur.y - ptPrev.y) * fMouseSensivity);
	if(fRotY && s_pPlayer->IsAlive())
	{
		if(VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->CameraYawAdd(fRotY);
		else if(false == s_pPlayer->m_bStun) s_pPlayer->RotAdd(fRotY); // ?????? ???? ????????..
	}
	if(fRotX)
	{
		if(VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->CameraZoom(-fRotX); // ?????? ????
		else s_pEng->CameraPitchAdd(fRotX); // ?????? ????
	}

	if(fRotY || fRotX)
	{
		//SDL_SetRelativeMouseMode(SDL_TRUE);
		if(SDL_ShowCursor(SDL_QUERY)==SDL_ENABLE) {
			SDL_ShowCursor(SDL_DISABLE);
		}

		int x, y;
		SDL_GetWindowPosition(s_pWindow, &x, &y);
		SetCursorPos(ptPrev.x+x, ptPrev.y+y);
		s_pLocalInput->MouseSetPos(ptPrev.x, ptPrev.y);

		//SetGameCursor(NULL);
		//::SetCursorPos(ptPrev.x, ptPrev.y);
		//s_pLocalInput->MouseSetPos(ptPrev.x, ptPrev.y);
	}

	return true;
}

// ?????? ???? ????
bool CGameProcMain::OnMouseRDBtnPress(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	//???? ?????? ????????????....
	m_pUIHotKeyDlg->EffectTriggerByMouse();

	return true;
}

// When Mouse Middle Button Pressed - This will need to be change to 180* Camera Turn
bool CGameProcMain::OnMouseMBtnPress(POINT ptCur, POINT ptPrev)
{
	if (s_pPlayer->IsAlive())
	{
		if (VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->CameraYawAdd(100);
	}

	return true;
}

void CGameProcMain::ProcessUIKeyInput(bool bEnable)
{
	if(m_pUIChatDlg && !m_pUIChatDlg->IsChatMode())
	{
		CGameProcedure::ProcessUIKeyInput();
		if(s_pLocalInput->IsKeyPress(SDL_SCANCODE_RETURN) && !s_bKeyPress)
		{
			m_pUIChatDlg->SetFocus();
		}
	}
	else if(m_pUIChatDlg && m_pUIChatDlg->IsChatMode())
	{
		s_bKeyPress = false;
		if(m_pUIChatDlg->GetEnableKillFocus())
		{
			m_pUIChatDlg->SetEnableKillFocus(false);
			m_pUIChatDlg->KillFocus();
		}
	}
}

void CGameProcMain::MsgSend_SpeedCheck(bool bInit)
{
	uint8_t	byBuff[10];											// ????.. 
	int		iOffset=0;											// ????..
	float	fTime = CN3Base::TimeGet();							// ?????????? ????

	s_pSocket->MP_AddByte(byBuff, iOffset, WIZ_SPEEDHACK_CHECK);	// ???????? ???? ????..
	s_pSocket->MP_AddByte(byBuff, iOffset, bInit);				// ?????? ???? ???????? ?? ???? true ???? ???????? false?? ????????
	s_pSocket->MP_AddFloat(byBuff, iOffset, fTime);				// ?????????? ????
	s_pSocket->Send(byBuff, iOffset);							// ????..
}

void CGameProcMain::MsgRecv_ClassPromotion(Packet& pkt)
{
	uint16_t sClass, socketID;
	pkt >> sClass >> socketID;

	// TODO: Clean this up when CPlayerMySelf is derived properly so we can share this logic in a much nicer fashion.
	if (socketID == s_pPlayer->IDNumber())
	{
		s_pPlayer->m_InfoBase.eClass = (e_Class)sClass;
		m_pUIVar->UpdateAllStates(&s_pPlayer->m_InfoBase, &s_pPlayer->m_InfoExt);
		m_pUIHotKeyDlg->ClassChangeHotkeyFlush();
		m_pUISkillTreeDlg->SetPageInCharRegion();
		m_pUISkillTreeDlg->InitIconUpdate();
	}
	else
	{
		auto pUPC = s_pOPMgr->UPCGetByID(socketID, false);
		if (pUPC != nullptr)
			pUPC->m_InfoBase.eClass = (e_Class)sClass;
	}

	s_pFX->TriggerBundle(socketID, -1, FXID_CLASS_CHANGE, socketID, -1);
}


void CGameProcMain::NoahTrade(uint8_t bType, uint32_t dwGoldOffset, uint32_t dwGold)
{
	char szBuf[256] = "";
	std::string szMsg;

	switch (bType)
	{
	case N3_SP_NOAH_GET:
		::_LoadStringFromResource(IDS_TRADE_COIN_RECV, szMsg);
		sprintf(szBuf, szMsg.c_str(), dwGoldOffset);
		MsgOutput(szBuf, 0xff6565ff);
		break;

	case N3_SP_NOAH_LOST:
		::_LoadStringFromResource(IDS_TRADE_COIN_PAID, szMsg);
		sprintf(szBuf, szMsg.c_str(), dwGoldOffset);
		MsgOutput(szBuf, 0xffff3b3b);
		break;
	}

	//s_pPlayer->m_InfoExt.iGold = dwGold;
	if (m_pUIInventory->IsVisible())
		m_pUIInventory->GoldUpdate();
	if (m_pUITransactionDlg->IsVisible())
		m_pUITransactionDlg->GoldUpdate();
	if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible())
		m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();
}