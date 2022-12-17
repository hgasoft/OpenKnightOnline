// GrassMng.h: interface for the CGrassMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRASSMNG_H__3B6D6FD3_4213_40EE_A8E9_F50412B49EC2__INCLUDED_)
#define AFX_GRASSMNG_H__3B6D6FD3_4213_40EE_A8E9_F50412B49EC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameBase.h"
//#include "GameProcedure.h"
#include <list>
#include "GrassBoard.h"

#define GRASS_MAX 11 //	(40/4(TILE_SIZE)) + 1
#define GRASS_TILENUM 11
#define GRASS_MAKENEW	1
#define	GRASS_MAKEUSE	2

class CGameProcedure;
class CN3Terrain;

//typedef std::list<class CGrassBoard*>::iterator it_GrassBoard;
class CGrassMng   : public CGameBase
{
protected:

//	std::list<class CGrassBoard*>	m_Grasses;
	CGrassBoard		m_pGrasses[GRASS_TILENUM][GRASS_TILENUM];	//	Ǯ �ѷ����� ����
//	__Vector3		m_pCount[GRASS_MAX];
	float			m_fChkRange[4];	//	ī�޶���� ��ġ�˻���

//	__Vector3		m_vCamPo;
	bool			m_bChkZoneChange;
	uint16_t	m_usDrawIndex;

	CN3Texture*		m_txTexture[8];
#ifdef _DEBUG
	char			m_strFileName[8][_MAX_PATH];	//	���߿� ����Ÿ� ���� �׸��� �̸��� ��� �޴´�
#endif
	int				m_iFileMaxNum;

protected:
	void	LoadFromFile(const char* szFileName,__Vector3 CamPo);
	int		SetFile(int iTexIndex,uint8_t ucTexOrgIndex,__Vector3 CamPo);
	float	RandomGenf(float max, float min);
	void	ChkThisZoneUseGrass(int nGrassUseOrder);

	void	FindGrassIndex(uint8_t GrassIndex,int& nFineGrass,uint8_t& ucFineIndex);
	void	FindGrassIndex(const uint8_t uCGrassMngOrder,int* pnInputGrass,uint8_t* GrassIndex,int& nGrassTotNum);

	void	ChkTileRange(float fCamX,float fCamZ);	//	Ÿ�ϰ������� äũ,tick
	void	FineNewTile(uint16_t* Tile,int& iCount,float* ChkRange,float* LargeRange);

public:
	void	Init(__Vector3 CamPo);
	void	Tick(CGameProcedure* pProc);
	void	Render();
	void	Release();

	bool	ChangeZone();

	bool	IsInRect(float fPoX,float fPoY,float* fRange);	//	�������� �ִ��� Ȯ��
public:
	CGrassMng();
	virtual ~CGrassMng();
};

#endif // !defined(AFX_GRASS_H__3B6D6FD3_4213_40EE_A8E9_F50412B49EC2__INCLUDED_)
