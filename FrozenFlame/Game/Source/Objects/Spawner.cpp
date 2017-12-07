#include "Spawner.h"
#include "../Core/CWorldManager.h"
#include "../Core/CLevel.h"
#include "../Renderer/CEmitterManager.h"
#include "../Objects/WizardEnemy.h"
#include "../Util/Util.h"

CSpawner::CSpawner(void)
{
	SetType(SPAWNER_OBJECT);
}

CSpawner::~CSpawner()
{
	SetInitialized(false);
}

void CSpawner::Initialize()
{
	CEnemy::Initialize();

	//	Initialization Here
	m_bIsSpawningEnemies = false;
	m_pGaurd = nullptr;
	m_bIsDestructable = true;
	m_bSpawnOnce = false;
	m_bWizardSpawner = false;
	m_bShieldCooldown = false;

	m_nExplosiveType = 1;
	m_fCurrentDelay = 5.0f;
	m_fInternalDelay = 0.0f;
	m_fSpawnDelay = 5.0f;
	SetHealth(25.0f);
	m_nMinimunReinforce = 10;
	m_nExplosiveChance = 0;
	m_fShieldCooldown = 2.0f;

	m_bobbingData.fBobbingSpeed = 0.3f;
	m_bobbingData.fBobbingDistance = 0.7f;
	m_bobbingData.fCurrentFloatDistance = 0.0f + float( rand()% 5 ) * 0.2f;
	m_bobbingData.bBobbingUp = false;

	m_CurrentSpawn = m_vSpawnList.begin();
	m_pLevelOwner = nullptr;

	SetRenderID(CView::Push(this, "Crystal", RENDER_TRANSPARENT | RENDER_RAGE_AFFECTED));
	SetAnimPlayer(CView::GetAnimPlayer(GetRenderID()));

	CView::MakeLight(GetRenderID(), true, vec3f(0, 2, 0), vec4f(0.65f, 0.65f, 0.8f, 1.0f));

	SetAttackedTimer(0.0);
	m_pcBubble = nullptr;

	m_cEmit = nullptr;
	m_pcIceEmitter = nullptr;
	m_pcFireEmitter = nullptr;
	m_pcShatterEmitter = nullptr;
	m_bIsEmitterActive = false;
	m_fElementSwitchDelay = 6.0f;

	m_cEmit = CView::GetEmitterManager().AddEmitter( "Resource/Particles/SpawnerBlocks.txt",vec3f(0.0f,0.0f,0.0f),vec3f(0.0f,0.0f,0.0f),
		0,this);

	SetElementalState(1);
	m_fReformDelay = 20.0f;

	fRotateRadians = -0.3f;
	fRotateDuration = 0.7f;
	fRotateTimer = 0.0f;
}

void CSpawner::Reinitialize(void)
{
	if(GetInitialized()){	
		Uninitialize();
	}
	Initialize();
}

void CSpawner::Uninitialize()
{
	if(m_pcBubble != nullptr)
	{
		m_pcBubble->Uninitialize();
		m_pcBubble = nullptr;
	}
	if(m_cEmit){
		CView::GetEmitterManager().KillEmitter(m_cEmit);
		m_cEmit = nullptr;
	}

	if(m_pcIceEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcIceEmitter);
		m_pcIceEmitter = nullptr;
	}
	if(m_pcFireEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcFireEmitter);
		m_pcFireEmitter = nullptr;
	}
	if(m_pcWizardBeamEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcWizardBeamEmitter);
		m_pcWizardBeamEmitter = nullptr;
	}
	if (m_pcShatterEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcShatterEmitter, false);
		m_pcShatterEmitter = nullptr;
	}	
	CCollidableObject::Uninitialize();
}

void CSpawner::Update(float fElapsedTime)
{
	// Intro velocity check hard code height check cause #bads. -JC
	if(GetWorldMatrix()->axis_pos.y < 2.0f && GetVelocity().y < -FLT_EPSILON) {
		vec3f newPos = GetPos();
		newPos.y = 2.0f;
		SetPos(newPos);
		SetVelocity(vec3f(0.0f,0.0f,0.0f));	
	
		CEmitter* tempEmit = CView::GetEmitterManager().AddEmitter( false, true, GetPos(), "Resource/Particles/SpawnerShatter.txt", true);
		tempEmit->SetOffset(vec3f(0.0f,1.0f,0.0f));
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_METEOR_LAND,GAME_OBJECT_ID_PLAYER);
	}

	if(!m_bWizardSpawner){
		if(m_pcBubble && !CView::GetRenderInstance(m_pcBubble->GetRenderID())->IsVisible()){
			if(m_pGaurd != nullptr){
				if(m_pGaurd->GetType() != WIZARD_OBJECT){
					CView::SetIsVisible(m_pcBubble->GetRenderID(), true);
				}
				else if( CView::GetRenderInstance(((CWizardEnemy*)(m_pGaurd))->GetBubble()->GetRenderID())->IsVisible()){
					CView::SetIsVisible(m_pcBubble->GetRenderID(), true);
				}
			}
		}
	}
	//	CSoundManager::GetInstance()->SetObjectWithSoundPos(GetSoundID(), *GetWorldMatrix()); // WORKING CODE DO NOT DELETE
	//	SetSoundPos(GetPos()); // TESTING IF I CAN JUST USE THIS INSTEAD OF THE ABOVE

	if(!CView::GetRenderInstance(GetRenderID())->IsVisible() && CView::GetCamera().isFollowing()){
		CView::SetIsVisible( GetRenderID(), true ); // Appear only after the cutscene completes. - BRG
	}

	if(GetHealth() <= 0){
		if( m_bWizardSpawner ){
			m_fCurrentDelay += fElapsedTime;
			if(m_fCurrentDelay > m_fReformDelay){// REFORM!				
				CWorldManager::GetInstance()->GetObjectManager()->GetWizard()->ReactivateCrystals();
			}
		}
		else{
			
			CView::GetEmitterManager().AddEmitter( "Resource/Particles/SpawnerBlocks.txt",GetPos(),vec3f(0.0f,0.0f,0.0f),
				EMIT_STARTACTIVE | EMIT_KILLONEND);
			
			if (!m_pcShatterEmitter){
				vec3f agentPos = GetPos();
				agentPos.y += 2.0f;
				m_pcShatterEmitter = CView::GetEmitterManager().AddEmitter(false, true, agentPos, "Resource/Particles/SpawnerShatter.txt");
				// play both THUMP and ICEBLOCKSHATTER on top of each other
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_THUMP, GetSoundID());
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_ICEBLOCKSHATTER, GetSoundID());
			}
			Uninitialize();
		}
		return;
	}
	CEnemy::Update(fElapsedTime);
	if(!GetInitialized())
		return;

	SetAttackedTimer(GetAttackedTimer() + fElapsedTime);

	if( m_bWizardSpawner ){
		m_fElementSwitchDelay -= fElapsedTime;
		if(m_fElementSwitchDelay < 2.0f){
			if(!m_bIsEmitterActive){
				if(GetElementalState() == FIRE){
					m_pcIceEmitter->SetActive(true);
				}
				else{
					m_pcFireEmitter->SetActive(true);
				}
				m_bIsEmitterActive = true;
			}
			if(m_fElementSwitchDelay <= 0.0f){
				SetElementalState(-GetElementalState());
				if(GetElementalState() == FIRE){					
					//m_pcIceEmitter->SetActive(false);
					SetColor(vec3f(1.0f, 0.0f, 0.0f));
				}
				else{
					//m_pcFireEmitter->SetActive(false);
					SetColor(vec3f(0.0f, 0.0f, 0.5f));
				}
				m_pcFireEmitter->SetActive(false);
				m_pcIceEmitter->SetActive(false);

				m_fElementSwitchDelay = 6.0f;
				m_bIsEmitterActive = false;
			}

		}
		if(m_bShieldCooldown){
			m_fShieldCooldown -= fElapsedTime;
			if(m_fShieldCooldown <= 0.0f){
				CView::SetIsVisible(m_pcBubble->GetRenderID(),false);
				m_bIsDestructable = true;
				m_bShieldCooldown = false;
				m_fShieldCooldown = 2.0f;
			}
		}
	}

	if( m_bEnergyTrails ) {
		if( m_bWizardSpawner ){
			if(!CView::GetRenderInstance( m_pcBubble->GetRenderID() )->IsVisible() &&
				!m_pcWizardBeamEmitter->GetActive()) {
					m_pcWizardBeamEmitter->SetActive(true);	}
			else if(CView::GetRenderInstance( m_pcBubble->GetRenderID() )->IsVisible() &&
				m_pcWizardBeamEmitter->GetActive()) {
					m_pcWizardBeamEmitter->SetActive(false); }
		}
		else{
			// Check and turn on wizard beam if he's got his shield up. - JC
			if(m_pcBubble && CView::GetRenderInstance( m_pcBubble->GetRenderID() )->IsVisible() &&
				m_pcWizardBeamEmitter && !m_pcWizardBeamEmitter->GetActive()) {
					m_pcWizardBeamEmitter->SetActive(true);	}
			else if(m_pcBubble && !CView::GetRenderInstance( m_pcBubble->GetRenderID() )->IsVisible() &&
				m_pcWizardBeamEmitter && m_pcWizardBeamEmitter->GetActive()) {
					m_pcWizardBeamEmitter->SetActive(false); }
		}
	}

	if(this->GetHitCooldown() >= 0.7f){
		SetIsHit(false);
		SetHitCooldown(0.0f);
	}

	if(this->GetIsHit()){
		SetHitCooldown(this->GetHitCooldown() + fElapsedTime);
	}
	//changed the way this works to allow for shutting off spawners
	//spawners are now activated by CLevel CJM
	if( m_bIsSpawningEnemies ){
		if( m_pGaurd ){ // Are we tied to beasts?
			if( !m_pGaurd->GetInitialized() ){ // Is the Guardian dead?					
				//SetHealth(0); // Kill us // No Longer killing for player satisfaction
				CView::SetIsVisible(m_pcBubble->GetRenderID(), false);
				m_pGaurd = nullptr;
				m_bIsDestructable = true;
				return;
			}
			if( m_pLevelOwner->GetActiveEnemyCount() < m_nMinimunReinforce ){ // Less than amount on field
				m_fCurrentDelay += fElapsedTime;
			}
		}
		else{
			m_fCurrentDelay += fElapsedTime;
		}
		if( m_fCurrentDelay >= m_fSpawnDelay ){				
			m_fInternalDelay += fElapsedTime;
			Spawn();
		}
	}	
	if( m_pcBubble ){
		vec3f vPos = GetPos();
		vPos.y += 3.0f;
		m_pcBubble->SetPos( vPos );
	}

	static float floatDistance = m_bobbingData.fBobbingSpeed * fElapsedTime;
	vec3f currentPos = GetPos();
	if (m_bobbingData.bBobbingUp)
	{
		// float the object up
		m_bobbingData.fCurrentFloatDistance += floatDistance;
		currentPos.y += floatDistance;
		this->SetPos(currentPos);
		// once it has floated as high as it will, float it back down
		if (m_bobbingData.fCurrentFloatDistance > m_bobbingData.fBobbingDistance)
		{
			m_bobbingData.bBobbingUp = false;
			m_bobbingData.fCurrentFloatDistance = m_bobbingData.fBobbingDistance;
		}
	}
	else
	{
		m_bobbingData.fCurrentFloatDistance -= floatDistance;
		currentPos.y -= floatDistance;
		this->SetPos(currentPos);
		if (m_bobbingData.fCurrentFloatDistance < 0)
		{
			m_bobbingData.bBobbingUp = true;
			m_bobbingData.fCurrentFloatDistance = 0;
		}
	}


}

void CSpawner::Spawn(void)
{
	if(m_fInternalDelay >= SPAWNER_WAIT_TIME)
	{
		if(m_CurrentSpawn != m_vSpawnList.end()) //	We are still in the spawn list
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_CRYSTAL_SPAWN, GetSoundID()); 

			CBaseObject* tempObject = nullptr;
			//	Spawn the current creature
			vec3f levelPos = vec3f( m_pLevelOwner->GetCenterX(), 0.0, m_pLevelOwner->GetCenterZ() );
			vec3f localPos = GetPos();
			vec3f finalPos = vec3f( localPos.x, localPos.y, localPos.z - levelPos.z );

			if( m_cEmit )
			{
				m_cEmit->SetActive(true);				
			}

			//if(( rand() % 101 )  < m_nExplosiveChance )
			//{
			//	tempObject = m_pLevelOwner->PushEnemy(finalPos, EXPLOSIVE_OBJECT, m_nExplosiveType);
			//	m_nExplosiveType = -m_nExplosiveType; // Change the explosive type to alternate
			//}
			//else
			{
				switch ((*m_CurrentSpawn))
				{
				case MELEE_OBJECT:{				
					tempObject = m_pLevelOwner->PushEnemy(finalPos, MELEE_OBJECT);
								  } break;

				case EXPLOSIVE_OBJECT:{
					tempObject = m_pLevelOwner->PushEnemy(finalPos, EXPLOSIVE_OBJECT, m_nExplosiveType);
					m_nExplosiveType = -m_nExplosiveType; // Change the explosive type to alternate
									  } break;
				case RANGED_OBJECT:{
					//	ERROR CANNOT SPAWN RANGED MINIONS
					//CBaseObject* tempObject = CWorldManager::GetInstance()->MakeObject(GetPos(), RANGED_OBJECT);
								   } break;
				case ICE_OBJECT:{
					tempObject = m_pLevelOwner->PushEnemy(finalPos, ICE_OBJECT);
								} break;
				case FIRE_OBJECT:{
					tempObject = m_pLevelOwner->PushEnemy(finalPos, FIRE_OBJECT);
								 } break;
				}
			}
			if(tempObject){
				(( CEnemy* ) tempObject )->GetFSM()->GetIsChasing() = true;
			}
			++m_CurrentSpawn;	//	Increment the spawn
		}
		else{ //	We are outside the spawn list, reset.
			m_CurrentSpawn = m_vSpawnList.begin();

			m_fCurrentDelay = 0.0f;
			if( m_bSpawnOnce ){
				m_bIsSpawningEnemies = false;
				SetHealth(0);
			}
		}
		m_fInternalDelay = 0.0f;
	}
}

void CSpawner::OnAttack( int nDamage, int nElementType )
{
	// If spawner can take damage
	if ( GetAttackedTimer() >= DAMAGE_COOLDOWN ){
		if( m_bIsDestructable){ // Make sure not invulnerable
			if(m_bWizardSpawner){
				if(nElementType == GetElementalState()){ // Stun if attacking with wrong element
					CWorldManager::GetInstance()->GetObjectManager()->GetPlayer()->StunThePlayer();
					return;
				}
				SetHealth( GetHealth() - nDamage );
				if(GetHealth() <= 0){
					CWorldManager::GetInstance()->GetObjectManager()->GetWizard()->CrystalDisabled();
					m_fCurrentDelay = 0.0f;
				}
			}
			else{
				SetHealth( GetHealth() - nDamage );
			}
			SetAttackedTimer( 0.0 );
			// I wrote this line of code, and I'm proud of it - Dan
			CView::FlashRenderInstance(GetRenderID(), nElementType == FIRE ? vec4f(1, 0.25f, 0, 1) 
				: nElementType == ICE ? vec4f(0, 0.8f, 1, 1) : vec4f(1, 1, 1, 1));
		}
	}
}

void CSpawner::AddBubble()
{
	if(m_pcBubble == nullptr)
	{
		m_pcBubble = (CSceneryObject*) CWorldManager::GetInstance()->GetObjectManager()->NewObject(SCENERY_OBJECT, 0, "WizardShield");
		CView::SetRenderInstanceFlags(m_pcBubble->GetRenderID(), RENDER_FULLY_TRANSLUCENT | RENDER_NOT_SHADOWCASTER | RENDER_SCROLLING_UVS | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED); // Now this is real code!
		m_pcBubble->SetTransparency(0.25f);
		m_pcBubble->SetColor(vec3f(1.0f,0.0f,1.0f));
		matrix4f mMat = *m_pcBubble->GetWorldMatrix();
		mMat.scale_post(0.5f,0.5f,0.5f);
		m_pcBubble->SetWorldMatrix(mMat);

		CView::SetIsVisible(m_pcBubble->GetRenderID(), false); // To prevent the effect playing before the crystal lands.
	}
}
