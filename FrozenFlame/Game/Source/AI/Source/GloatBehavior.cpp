#include "../Header/GloatBehavior.h"
#include "../Header/CFSM.h"
#include "../../Animations/AnimPlayer.h"
#include "../../Animations/Animation.h"
#include "../../Core/CWorldManager.h"


void CGloatBehavior::Enter()
{
	m_fCountdown = 3.0f;
}

void CGloatBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	if(CInput::GetInstance()->GetIsInputEnabled() && GetOwner()->GetIsChasing()){
		CInput::GetInstance()->DisablePlayerInput();
		GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_SPECIAL_A);
		
		int currentGameStage = CWorldManager::GetInstance()->GetCurrentSection();
		CWorldManager::GetInstance()->GetObjectManager()->GetPlayer()->SetVelocity(vec3f(0.0f, 0.0f, 0.0f));

		switch(currentGameStage)
		{
		case FIRST_STAGE:
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARTTAUNT_SURVIVED, GAME_OBJECT_ID_PLAYER);
			}
			break;

		case SECOND_STAGE:
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARTTAUNT_VERMIN, GAME_OBJECT_ID_PLAYER);
			}
			break;

		case THIRD_STAGE:
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARTTAUNT_STRONGER, GAME_OBJECT_ID_PLAYER);
			}
			break;

		case FOURTH_STAGE:
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARTTAUNT_DONEWELL, GAME_OBJECT_ID_PLAYER);
			}
			break;

		case FINAL_BOSS:
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARTTAUNT_FULLPOWER, GAME_OBJECT_ID_PLAYER);
			}
			break;
		}

		CView::GetCamera().SetPlayer(GetOwner()->GetAgent());		
		CGame::GetInstance()->StopTimer();
	}

	if(!CInput::GetInstance()->GetIsInputEnabled()){
		m_fCountdown -= _fElapsedTime;
	}
	
	GetOwner()->GetAgent()->GetAnimationPlayer()->Update(_fElapsedTime);


	if(m_fCountdown <= 0.0f && GetOwner()->GetAgent()->GetAnimationPlayer()->GetLastPlayedAnimType() == ANIM_SPECIAL_A){

		int currentGameStage = CWorldManager::GetInstance()->GetCurrentSection();

		if(currentGameStage == FINAL_BOSS){				
				GetOwner()->TransitionTo(TRAP_SPAWN_BEHAVIOR);
		}
		else{
				GetOwner()->TransitionTo(FLOAT_BEHAVIOR);
		}
	}
}

void CGloatBehavior::Exit()
{
	CGame::GetInstance()->StartTimer();
	CInput::GetInstance()->EnablePlayerInput();
	CView::SetIsVisible( ((CWizardEnemy*)(GetOwner()->GetAgent()))->GetBubble()->GetRenderID(), true);
	CView::GetCamera().SetPlayer(CWorldManager::GetInstance()->GetObjectManager()->GetPlayer());
}
