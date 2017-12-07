/********************************************************************
* Filename:			CWinState.h
* Date:				11/6/2012
* Mod. Date:		11/6/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the Win state
********************************************************************/

#include "CWinState.h"
#include "../Renderer/View.h"
#include "../Util/CInput.h"
#include "../CGame.h"
#include "CMainMenuState.h"
#include "CCreditsState.h"
#include "../Renderer/AssetManager.h"
#include "../Objects/MenuModelObject.h"
#include "../Renderer/RenderInstance.h" 

void CWinState::Enter(void)
{
	m_nFontID = -1;

	m_nFontID = CView::LoadFont("font example.txt");

	if (CInput::GetInstance()->GetIfUsingController())
	{
		CInput::GetInstance()->SetLeftMotorSpeed(0);
		CInput::GetInstance()->SetRightMotorSpeed(0);
	}

	// Set the background for the state
	m_tBackGround.m_nImageID = CView::LoadSprite("BlackSquare.png", &m_tBackGround.m_nImageWidth, &m_tBackGround.m_nImageHeight);
	m_tDarken.m_nImageID = CView::LoadSprite("Fade.png", &m_tDarken.m_nImageWidth, &m_tDarken.m_nImageHeight);

	CSoundManager::GetInstance()->StopAll();
	CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_MX_VICTORY, GAME_OBJECT_ID_PLAYER);

	m_pMenuPortal = new CMenuModelObject("PortalPlane", vec3f(0.0f, 0.0f, 5.0f));
	m_pMenuPortal->RotateY(D3DXToRadian(135));
	m_pMenuPortal->RotateX(D3DXToRadian(30));

	m_pMenuPlayer = new CMenuModelObject("Player", vec3f(0.0f, -2.5f, -5.0f));
	m_nFireSwordID = CView::PushAttachment(m_pMenuPlayer->GetRenderID(), "FireSword", "weapon_anchor", "", RENDER_MENU_MODEL);
	m_nIceSwordID = CView::PushAttachment(m_pMenuPlayer->GetRenderID(), "IceSword", "weapon_anchor", "", RENDER_MENU_MODEL);
	CView::SetAttachmentEnabled(m_nFireSwordID, false);
	CView::SetAttachmentEnabled(m_nIceSwordID, true);

	unsigned short flags = CView::GetRenderInstance(m_pMenuPlayer->GetRenderID())->GetFlags();
	CView::SetRenderInstanceFlags(m_pMenuPlayer->GetRenderID(), flags | RENDER_TRANSPARENT);
	flags = CView::GetRenderInstance(m_nFireSwordID)->GetFlags();
	CView::SetRenderInstanceFlags(m_nFireSwordID, flags | RENDER_TRANSPARENT);
	flags = CView::GetRenderInstance(m_nIceSwordID)->GetFlags();
	CView::SetRenderInstanceFlags(m_nIceSwordID, flags | RENDER_TRANSPARENT);

	m_pMenuPlayer->LoadAnimation(ANIM_IDLE,			"Player/Player_Idle.anm");
	m_pMenuPlayer->LoadAnimation(ANIM_WALK,			"Player/Player_Movement.anm");

	m_pMenuPlayer->PlayAnimation(ANIM_WALK, true);
	m_fTimer = 0.0f;
}

bool CWinState::Input(void)
{
	if(CView::GetFadeState() != FADE_COMPLETE)
	{
		CView::SetFadeState(FADE_OUT);
		return true;
	}

	if(m_pInput->Select() || m_fTimer > 5.0f)
	{
		CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
		CGame::GetInstance()->ChangeState(CCreditsState::GetInstance());
		CView::ResetFade();
	}

	return true;
}

void CWinState::Update(float _fElapsedTime)
{
	m_fTimer += _fElapsedTime;

	if(m_pMenuPortal) {
		m_pMenuPortal->RotateZ(D3DXToRadian(_fElapsedTime * 90));
		m_pMenuPortal->Update(_fElapsedTime);
	}
	if(m_pMenuPlayer) {
		vec3f camPos(CView::GetCamera().GetPosition().x, CView::GetCamera().GetPosition().y, CView::GetCamera().GetPosition().z);
		m_pMenuPlayer->FullLookAt(camPos);
		m_pMenuPlayer->Update(_fElapsedTime);
		m_pMenuPlayer->SetTransparency(m_pMenuPlayer->GetTransparency() - _fElapsedTime * 0.3f);
	}
	CView::Update(_fElapsedTime);
}

void CWinState::Render(void)
{
	CView::RenderMenuModels(m_tBackGround.m_nImageID);

	float fScaleX = CGame::GetInstance()->GetScreenWidth() / float(g_nDevelopmentResolutionWidth);
	float fScaleY = CGame::GetInstance()->GetScreenHeight() / float(g_nDevelopmentResolutionHeight);

	int nHalfScreenWidth = CGame::GetInstance()->GetScreenWidth() >> 1;
	int nHalfScreenHeight = CGame::GetInstance()->GetScreenHeight() >> 1;

	CView::Draw(m_tDarken.m_nImageID, 0, 0, fScaleX, fScaleY, nullptr, 0.0f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));

	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 1.0f, 1.0f, "Having defeated the Wizard.", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0, -1);
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 1.0f, 1.0f, "The hero continues his journey", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0, 0);
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 1.0f, 1.0f, "to face dangers unknown", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0, 1);
}

void CWinState::Exit(void)
{
	if(m_pMenuPlayer) {
		delete m_pMenuPlayer;
		m_pMenuPlayer = nullptr;
	}
	if(m_pMenuPortal) {
		delete m_pMenuPortal;
		m_pMenuPortal = nullptr;
	}
}