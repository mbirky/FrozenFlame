#include "CPickupObject.h"
#include "../Core/CWorldManager.h"
#include "PlayerObject.h"

CPickupObject::CPickupObject(void)
{
	SetType(PICKUP_OBJECT);
	
}


CPickupObject::~CPickupObject()
{
	//SetInitialized(false);
}


void CPickupObject::Initialize(void)
{
	CCollidableObject::Initialize();
	m_nPickupType = -1;
	SetRenderID(CView::Push(this, "Orb", RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED));
	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.scale_post(0.12f, 0.12f, 0.12f);
	SetWorldMatrix(worldMatrix);
	m_nLightID				= -1;
	m_fLifetime				= 15.0f;
	m_nHealthIncrease		= 3;
	m_nRageIncrease			= 7;
	m_fHomingDistance		= 10.0f;
	m_fHomingSpeed			= 50.0f;
	m_fBobbingSpeed			= 1.5f;
	m_fBobbingDistance		= 1.0f;
	m_fCurrentFloatDistance = 0.0f + float( rand()% 5 ) * 0.2f;
	m_bBobbingUp			= true;
}


void CPickupObject::Reinitialize()
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}


void CPickupObject::Update(float fElapsedTime)
{
	vec3f vectorToPlayer(0, 0, 0);
	vec3f playerVector = CWorldManager::GetInstance()->GetObjectManager()->GetPlayer()->GetPos();
	if (false == FloatIsGood( playerVector.x)){
		int x = 10;
	}
	vec3f adjustedPickupPos = this->GetPos();
	// If the pickup is high above the player, the true distance to the player might be too far for the pickups
	// to fly to the player. Adjust the y value down to the player's before calculating distance to the player.
	adjustedPickupPos.y = CWorldManager::GetInstance()->GetObjectManager()->GetPlayer()->GetPos().y;
	vectorToPlayer = CWorldManager::GetInstance()->GetObjectManager()->GetPlayer()->GetPos() - adjustedPickupPos;
		
	if ( vectorToPlayer.magnitude() <= m_fHomingDistance ){// if the distance to the player is within a certain range, have the pickup object home in on the player
		// set the vectorToPlayer to the true vector and not the adjusted vector
		vectorToPlayer = CWorldManager::GetInstance()->GetObjectManager()->GetPlayer()->GetPos() - this->GetPos();
		vectorToPlayer.normalize();
		this->SetPos( GetPos() + vectorToPlayer * m_fHomingSpeed * fElapsedTime );
	}	
	else{// otherwise, have them float up and down
		static float floatDistance = m_fBobbingSpeed * fElapsedTime;
		vec3f currentPos = GetPos();
		if( m_bBobbingUp ){
			// float the object up
			m_fCurrentFloatDistance += floatDistance;
			currentPos.y += floatDistance;
			this->SetPos(currentPos);
			// once it has floated as high as it will, float it back down
			if (m_fCurrentFloatDistance > m_fBobbingDistance)
				m_bBobbingUp = false;
		}
		else{
			m_fCurrentFloatDistance -= floatDistance;
			currentPos.y -= floatDistance;
			this->SetPos( currentPos );
			if ( m_fCurrentFloatDistance < 0 ){
				m_bBobbingUp = true;
				m_fCurrentFloatDistance = 0;
			}
		}
	}
	UpdateCollisionData();
	m_fLifetime -= fElapsedTime;

	if( m_fLifetime <= 0.0f )	{
		Uninitialize();
	}	
}

void CPickupObject::SetPickupType(int nType)
{
	vec4f vColor;
	if( nType == HEALTH_PICKUP ){
		vColor = vec4f( 0.0f, 1.0f, 0.0f, 1.0f );
	}

	else if( nType == RAGE_PICKUP ){
		vColor = vec4f( 1.0f, 0.0f, 0.0f, 1.0f );
	}

	if( -1 == m_nLightID ){
		m_nLightID = CView::MakeLight( GetRenderID(), true, vec3f( 0, 0, 0 ), vColor, 3.0f );
	}
	this->SetColor( vColor.xyz );
	m_nPickupType = nType;
}

void CPickupObject::Uninitialize(void)
{
	if( m_nLightID != -1 ){
		CView::DestroyLight( m_nLightID );
		m_nLightID = -1;
	}
	CCollidableObject::Uninitialize();	
}
//146