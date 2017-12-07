#include "Signpost.h"
#include "../Core/CWorldManager.h"

CSignpost::CSignpost()
{
	SetType(SIGN_OBJECT);
	this->m_nLevel = -1;
	this->m_nSection = -1;
	this->m_nLightID = -1;

}

CSignpost::~CSignpost()
{
	SetInitialized(false);

}

void CSignpost::Initialize(void)
{
	CEnvironmentObject::Initialize();
	m_fPopupScale = 0.8f;
	SetRenderID(CView::Push(this,"SignPost",0));
	this->m_nLevel = CWorldManager::GetInstance()->GetCurrLoadLevel();
	this->m_nFontID = CView::LoadFont("font example.txt");
	this->m_tPopup.m_nImageID = CView::LoadSprite("message_box.png", &this->m_tPopup.m_nImageWidth, &this->m_tPopup.m_nImageHeight);
	this->m_nSection = CWorldManager::GetInstance()->GetCurrentSection();
	int nRand = rand()% 26;
	int nChance = rand()%100;

	if(nChance >= 50)
	{
		nRand = -nRand;
	}
	
	matrix4f mWorldMat = *this->GetWorldMatrix();
	mWorldMat.rotate_y_pre(D3DXToRadian(120 + nRand/2.0f));
	mWorldMat.rotate_x_pre(D3DXToRadian(30 + nRand/1.5f));
	mWorldMat.scale_pre(vec3f(1.0f, 0.8f, 1.0f));
	this->SetWorldMatrix(mWorldMat);
	vec3f vPos = this->GetPos();
	vPos.y += 1.5f;
	
	// New light system - Dan
	//CView::AttachLight(this->GetRenderID(), vec4f(0.0f, 1.0f, 0.0f, 1.0f), 8.0f, vec4f(0.1f, 0.1f, 0.1f, 0.1f), vec4f(1.0f, 1.0f, 1.0f, 1.0f), 16.0f, 3.0f, vec3f(0.1f, 0.015f, 0.035f), vec3f(0, vPos.y, 0) );
	if(m_nLightID == -1)
	{
		m_nLightID = CView::MakeLight(GetRenderID(), true, vec3f(1, 1, 1), vec4f(1, 1, 1, 1), 8.0f);
	}
	
	
//	char buffer[128];

	switch(m_nSection)
	{
	case 0:
		{
			switch(m_nLevel)
			{

			case 0:
				{
					this->m_tTutorial.m_nImageID = CView::LoadSprite("controls_keyboard.png", &this->m_tTutorial.m_nImageWidth, &this->m_tTutorial.m_nImageHeight);
					m_vtTutorial.push_back(this->m_tTutorial);

					//TImage tTutorialB;
					this->m_tTutorial.m_nImageID = CView::LoadSprite("controls_xbox.png", &this->m_tTutorial.m_nImageWidth, &this->m_tTutorial.m_nImageHeight);
					m_vtTutorial.push_back(this->m_tTutorial);

					break;
				}

			case 1:
				{
					this->m_tTutorial.m_nImageID = CView::LoadSprite("sign_swordattacks.png", &this->m_tTutorial.m_nImageWidth, &this->m_tTutorial.m_nImageHeight);
					m_vtTutorial.push_back(this->m_tTutorial);
					break;

				}

			case 2:
				{
					//this->m_tTutorial.m_nImageID = CView::LoadSprite("sign_dash.png", &this->m_tTutorial.m_nImageWidth, &this->m_tTutorial.m_nImageHeight);
					//m_vtTutorial.push_back(this->m_tTutorial);
					this->m_tTutorial.m_nImageID = CView::LoadSprite("sign_rage.png", &this->m_tTutorial.m_nImageWidth, &this->m_tTutorial.m_nImageHeight);
					m_vtTutorial.push_back(this->m_tTutorial);

					break;
				}

			case 3:
				{
					this->m_tTutorial.m_nImageID = CView::LoadSprite("sign_deflect.png", &this->m_tTutorial.m_nImageWidth, &this->m_tTutorial.m_nImageHeight);
					m_vtTutorial.push_back(this->m_tTutorial);
					//m_fPopupScale = 3.0f;
					break;
				}

			case 5:
				{
					this->m_tTutorial.m_nImageID = CView::LoadSprite("sign_explodeminion.png", &this->m_tTutorial.m_nImageWidth, &this->m_tTutorial.m_nImageHeight);
					m_vtTutorial.push_back(this->m_tTutorial);
					//m_fPopupScale = 2.0f;
					break;
				}
			}
			break;
		}

	case 1:
		{
			switch(m_nLevel)
			{

			case 1:
				{
					this->m_tTutorial.m_nImageID = CView::LoadSprite("sign_dash.png", &this->m_tTutorial.m_nImageWidth, &this->m_tTutorial.m_nImageHeight);
					m_vtTutorial.push_back(this->m_tTutorial);
					//m_fPopupScale = 2.0f;
					break;
				}
				case 2:
				{
					this->m_tTutorial.m_nImageID = CView::LoadSprite("sign_icebeastattack.png", &this->m_tTutorial.m_nImageWidth, &this->m_tTutorial.m_nImageHeight);
					m_vtTutorial.push_back(this->m_tTutorial);
					//m_fPopupScale = 2.0f;
					break;
				}
			}
			
			break;
		}

	case 2:
		{

			switch(m_nLevel)
			{

			case 0:
				{
					this->m_tTutorial.m_nImageID = CView::LoadSprite("sign_firebeastattack.png", &this->m_tTutorial.m_nImageWidth, &this->m_tTutorial.m_nImageHeight);
					m_vtTutorial.push_back(this->m_tTutorial);
					//m_fPopupScale = 2.0f;
					break;

				}
			}
			break;
		}
	};


}

void CSignpost::Reinitialize(void)
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}

void CSignpost::Update(float fElapsedTime)
{
	UpdateCollisionData();
}

void CSignpost::Uninitialize(void)
{
	CEnvironmentObject::Uninitialize();

	if(m_nLightID != -1)
	{
		// New light system - Dan
		CView::DestroyLight(m_nLightID);
		m_nLightID = -1;
	}

	if(m_vtTutorial.size())
	{
		//unsigned size = m_vtTutorial.size();
		//
		//for(unsigned i = 0; i < size; ++i)
		//{
			m_vtTutorial.clear();

		//}
	}
}
void CSignpost::SetDescription(char* _szDesc) 
{
	/*char* buffer = _szDesc;*/
	char buffer[128];
	//buffer = _szDesc;
	strcpy_s(buffer, strlen(_szDesc)+1, _szDesc);
	m_szDescription = buffer;
	//strcpy_s(m_szDescription, strlen(_szDesc)+1, _szDesc);
}