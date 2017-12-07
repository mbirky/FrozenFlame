#include "CPopupState.h"
#include "../Renderer/View.h"
#include "../CGame.h"
#include "../Core/CWorldManager.h"

CPopupState::CPopupState(void)
{
	m_nImageID = -1;
	//m_nSelected = - 1;

}

void CPopupState::Enter(void)
{
	m_fFadeTimer = 0.0f;
	m_bFadeOut = false;
	m_bFadeIn = false;
	m_nCurSignID = 0;
	//m_tImage.m_nImageID = CView::LoadSprite("mainmenubg.png", &m_tImage.m_nImageWidth, &m_tImage.m_nImageHeight);
	//m_nImageID = CView::LoadSprite("mainmenubg.png", &m_tBackground.m_nImageWidth, &m_tBackground.m_nImageHeight);

	CObjectManager* pOM = CWorldManager::GetInstance()->GetObjectManager();

	m_pCurrSign = pOM->GetCurrentSign();

	if(m_pCurrSign)
	{
		m_tImage = m_pCurrSign->GetTutorialPopup()[0];
	}
}

bool CPopupState::Input(void)
{
	if(m_pInput->Select())
	{
		if(m_pCurrSign)
		{
			if(m_nCurSignID < m_pCurrSign->GetTutorialPopup().size())
			{
				++m_nCurSignID;
				
			}

			if(m_nCurSignID == m_pCurrSign->GetTutorialPopup().size())
			{
				CGame::GetInstance()->PopState();
			}
			
		}

		//else
		//{
		//	CGame::GetInstance()->PopState();
		//}
		
	}
	if(m_pInput->Menu_Up())
	{
		
	}
	if(m_pInput->Menu_Down())
	{
		
	}
	return true;
}

void CPopupState::Update(float _fElapsedTime)
{
	CView::Update(0.0f);
	
	if(m_nCurSignID < m_pCurrSign->GetTutorialPopup().size())
	{
		m_tImage = m_pCurrSign->GetTutorialPopup()[m_nCurSignID];
	}

	//m_fFlashTimer += _fElapsedTime;
	//
	//if(m_fFlashTimer >= 1.25f)
	//{
	//	m_fFlashTimer = 0.0f;
	//	m_bDisplayControls = !m_bDisplayControls;
	//}
}

void CPopupState::Render(void)
{
	//CView::Draw(m_tImage.m_nImageID
	GetPrevState()->Render();

	if(m_tImage.m_nImageID != -1)
	{
		int nHalfScreenWidth = CGame::GetInstance()->GetScreenWidth() >> 1;
		int nHalfScreenHeight = CGame::GetInstance()->GetScreenHeight() >> 1;

		float fScaleX = CGame::GetInstance()->GetScreenWidth() / float(g_nDevelopmentResolutionWidth);
		float fScaleY = CGame::GetInstance()->GetScreenHeight() / float(g_nDevelopmentResolutionHeight);

		int nHalfWidth =	static_cast<int>((m_tImage.m_nImageWidth * fScaleX )) >> 1;
		int nHalfHeight =	static_cast<int>((m_tImage.m_nImageHeight * fScaleY)) >> 1;
		int nButtonOffsetWidth =	static_cast<int>((m_tImage.m_nImageWidth) * fScaleX);
		int nButtonOffsetHeight =	static_cast<int>((m_tImage.m_nImageHeight) * fScaleY);

		int xPos = nHalfScreenWidth - nHalfWidth;
		CView::Draw(m_tImage.m_nImageID, xPos, 0, fScaleX,
				fScaleY);

		//CView::Draw(m_tImage.m_nImageID, 0, 0, float(CGame::GetInstance()->GetScreenWidth()) / m_tImage.m_nImageWidth,
		//		float(CGame::GetInstance()->GetScreenHeight()) / m_tImage.m_nImageHeight);

		RECT rText;
		CGame* pInst = CGame::GetInstance();

		//rText.left = 0;
		//rText.right = pInst->GetScreenWidth();
		//rText.top = 0;
		//rText.bottom = pInst->GetScreenHeight();
		//CView::Write(m_pCurrSign->GetFontID(), "Placeholder Screen", &rText, true, 1.0f);

		rText.left = 0;
		rText.right = pInst->GetScreenWidth();
		rText.top = long((pInst->GetScreenHeight() * 0.8) - 10 * fScaleY);
		rText.bottom = long(rText.top + 50 * fScaleY);

		if(m_pCurrSign)
		{
			if(m_nCurSignID < m_pCurrSign->GetTutorialPopup().size())
			{
				CView::Write(m_pCurrSign->GetFontID(), "Hit Enter to Continue", &rText, true, 0.5f * fScaleX, 0.5f * fScaleY);
			}

			else
			{
				CView::Write(m_pCurrSign->GetFontID(), "Hit Enter to Exit", &rText, true, 0.5f * fScaleX, 0.5f * fScaleY);
			}
		}
	}
}

void CPopupState::Exit(void)
{
	
}

CPopupState::~CPopupState()
{

}