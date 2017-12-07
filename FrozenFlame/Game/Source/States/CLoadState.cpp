#include "CLoadState.h"
#include "CGameState.h"
#include "../Renderer/View.h"
#include "../Util/CInput.h"
#include "../CGame.h"
#include "CMainMenuState.h"
#include "../Achievements/Header/CAchievementManager.h"
#include "../Objects/MenuModelObject.h"

void CLoadState::Enter()
{
	m_nFontID = CView::LoadFont("font example.txt");

	// Load the menu player
	m_pMenuPlayer = new CMenuModelObject("Player", vec3f(-6.5f, 0.0f, 2.0f));
	m_pMenuPlayer->RotateY(D3DXToRadian(90));
	m_nFireSwordID = CView::PushAttachment(m_pMenuPlayer->GetRenderID(), "FireSword", "weapon_anchor", "", RENDER_MENU_MODEL);
	m_nIceSwordID = CView::PushAttachment(m_pMenuPlayer->GetRenderID(), "IceSword", "weapon_anchor", "", RENDER_MENU_MODEL);
	CView::SetAttachmentEnabled(m_nFireSwordID, false);
	CView::SetAttachmentEnabled(m_nIceSwordID, true);

	m_pMenuPlayer->LoadAnimation(ANIM_IDLE,			"Player/Player_Idle.anm");
	m_pMenuPlayer->LoadAnimation(ANIM_WALK,			"Player/Player_Movement.anm");
	m_pMenuPlayer->LoadAnimation(ANIM_ATTACK,		"Player/Player_AttackUp.anm");
	m_pMenuPlayer->LoadAnimation(ANIM_ATTACK2,		"Player/Player_AttackDown.anm");
	m_pMenuPlayer->LoadAnimation(ANIM_ATTACK3,		"Player/Player_AttackDiagonal.anm");
	m_pMenuPlayer->LoadAnimation(ANIM_DEATH,		"Player/Player_Death.anm");
	m_pMenuPlayer->LoadAnimation(ANIM_DAMAGED,		"Player/Player_Damaged.anm");
	m_pMenuPlayer->LoadAnimation(ANIM_SPECIAL_A,	"Player/Player_Dash.anm");
	m_pMenuPlayer->LoadAnimation(ANIM_SPECIAL_B,	"Player/Player_ActivateRage.anm");

	// Load the menu wizard
	m_pMenuWizard = new CMenuModelObject("Wizard", vec3f(6.5f, 0.0f, 2.0f));
	m_pMenuWizard->RotateY(D3DXToRadian(180));
	m_nWizardStaffID = CView::PushAttachment(m_pMenuWizard->GetRenderID(), "WizardStaff", "weapon_anchor", "", RENDER_MENU_MODEL);

	m_pMenuWizard->LoadAnimation(ANIM_ATTACK,		"Wizard/Wizard_Cast.anm");
	m_pMenuWizard->LoadAnimation(ANIM_IDLE,			"Wizard/Wizard_Idle.anm");
	m_pMenuWizard->LoadAnimation(ANIM_DAMAGED,		"Wizard/Wizard_Rest.anm");
	m_pMenuWizard->LoadAnimation(ANIM_DEATH,		"Wizard/Wizard_Death.anm");
	m_pMenuWizard->LoadAnimation(ANIM_SPECIAL_A,	"Wizard/Wizard_TrapSummon.anm");

	m_nBackgroundID = CView::LoadSprite("BlackSquare.png");
}

bool CLoadState::Input()
{
	if(CInput::GetInstance()->Select())
	{
		// Set the camera up to do the intro
		CView::SetCameraToPan();
		CView::GetCamera().StartPanning();
		m_pInput->DisablePlayerInput();

		CGame::GetInstance()->PopState();
		return true;
	}
	return true;
}

void CLoadState::Update(float _fElapsedTime)
{
	static float fWizardAnimTimer = (rand() % 10) / 10.0f;
	static float fPlayerAnimTimer = (rand() % 10) / 10.0f;
	static float fAnimCooldown = 2.0f;
	fWizardAnimTimer += _fElapsedTime + (rand() % 10) / 1000.0f;
	fPlayerAnimTimer += _fElapsedTime + (rand() % 10) / 1000.0f;

	if(m_pMenuWizard) {
		// Animate the wizard based on time and randomness
		if(fWizardAnimTimer >= fAnimCooldown) {
			m_pMenuWizard->PlayAnimation(rand() % 3 ? ANIM_ATTACK : rand() % 2 ? ANIM_SPECIAL_A : ANIM_DEATH, false);
			fWizardAnimTimer = 0.0f;
		}
	
		m_pMenuWizard->Update(_fElapsedTime);
	}

	if(m_pMenuPlayer) {
		// Animate the player based on time and randomness
		if(fPlayerAnimTimer >= fAnimCooldown) {
			if(rand() % 2)
				m_pMenuPlayer->PlayAnimation(EAnimType(rand() % (ANIM_ATTACK3 - ANIM_ATTACK) + ANIM_ATTACK), false);
			else
				m_pMenuPlayer->PlayAnimation(EAnimType(rand() % (ANIM_SPECIAL_B - ANIM_SPECIAL_A) + ANIM_SPECIAL_A), false);
			fPlayerAnimTimer = 0.0f;
		}
		m_pMenuPlayer->Update(_fElapsedTime);
	}
	CView::Update(_fElapsedTime);
}

void CLoadState::Render()
{
	CView::RenderMenuModels(m_nBackgroundID);

#pragma region Text
	RECT tTextRect;
	
	int nScreenWidth	= CGame::GetInstance()->GetScreenWidth();
	int nScreenHeight	= CGame::GetInstance()->GetScreenHeight();

	float fScaleX = nScreenWidth / float(g_nDevelopmentResolutionWidth);
	float fScaleY = nScreenHeight / float(g_nDevelopmentResolutionHeight);
	
	string szText = "His destroyed homeland behind him,";
	tTextRect.left = 0;
	tTextRect.right = CGame::GetInstance()->GetScreenWidth();
	tTextRect.top = LONG((nScreenHeight / 2) - (20 * fScaleY));
	tTextRect.bottom = tTextRect.top;
	CView::Write(m_nFontID, szText, &tTextRect, true, 0.5f * fScaleX, 0.5f * fScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	
	szText = "the hero sets out to avenge his ";
	tTextRect.top += (LONG)(nScreenHeight * (0.05f * fScaleY));
	tTextRect.bottom = tTextRect.top;
	CView::Write(m_nFontID, szText, &tTextRect, true, 0.5f * fScaleX, 0.5f * fScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	
	szText = "people. Wielding only his magical";
	tTextRect.top += (LONG)(nScreenHeight * (0.05f * fScaleY));
	tTextRect.bottom = tTextRect.top;
	CView::Write(m_nFontID, szText, &tTextRect, true, 0.5f * fScaleX, 0.5f * fScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	
	szText = "elemental blade and unending courage";
	tTextRect.top += (LONG)(nScreenHeight * (0.05f * fScaleY));
	tTextRect.bottom = tTextRect.top;
	CView::Write(m_nFontID, szText, &tTextRect, true, 0.5f * fScaleX, 0.5f * fScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	
	szText = "he must defeat the Wizard.";
	tTextRect.top += (LONG)(nScreenHeight * (0.05f * fScaleY));
	tTextRect.bottom = tTextRect.top;
	CView::Write(m_nFontID, szText, &tTextRect, true, 0.5f * fScaleX, 0.5f * fScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	szText = "Press Enter to continue";
	tTextRect.top = LONG(nScreenHeight - (150 * fScaleY));
	tTextRect.bottom = tTextRect.top;
	CView::Write(m_nFontID, szText, &tTextRect, true, 0.5f * fScaleX, 0.5f * fScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#pragma endregion
}

void CLoadState::Exit()
{
	if(m_pMenuPlayer) {
		delete m_pMenuPlayer;
		m_pMenuPlayer = nullptr;
	}
	if(m_pMenuWizard) {
		delete m_pMenuWizard;
		m_pMenuWizard = nullptr;
	}
}
