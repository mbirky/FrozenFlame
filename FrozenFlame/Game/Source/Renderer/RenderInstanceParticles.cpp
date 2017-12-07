#include "RenderInstanceParticles.h"
#include "Emitter.h"
#include "../States/CVideoOptionsState.h"

CRenderInstanceParticles::CRenderInstanceParticles(CBaseObject* pObject)
	: CRenderInstance(-1, pObject), m_pParticleBuffer(nullptr)
{

}


CRenderInstanceParticles::~CRenderInstanceParticles(void)
{
	if(m_pParticleBuffer)
	{
		m_pParticleBuffer->Release();
		m_pParticleBuffer = nullptr;
	}
}


void CRenderInstanceParticles::Update(float fElapsedTime)
{
	CRenderInstance::Update(fElapsedTime);

	// Handles vertex buffer work for 2d point sprites -JC
	if(GetObjectType() == TWO_D)
	{
		if(static_cast<CEmitter*>(GetObject())->GetActive())
		{		
			// This object is a 2d particle emitter - JC
			ParticleBufferWork();			
		}
	}
}

void CRenderInstanceParticles::Render(int nCurShader, UINT uiPass)
{
	if(!m_pParticleBuffer || !(CVideoOptionsState::GetInstance()->GetRenderFlags() & RF_PARTICLES))
		return;

	CEmitter* pEmitter = static_cast<CEmitter*>(GetObject());	

	// Active check - JC
	if(!pEmitter->GetActive())
		return;		

	// Particles are already in world space - JC
	float scale = pEmitter->GetModifiers().m_fEndScale.x * 50.0f;

	// Clamp scale
	if(scale < 50.0f)
		scale = 50.0f;
	else if(scale > 256.0f)
		scale = 256.0f;		

	int src = pEmitter->GetSourceBlend();
	int dst = pEmitter->GetDestBlend();

	CAssetManager::ShaderSetValue(nCurShader,"sBlend",&src,sizeof(int));
	CAssetManager::ShaderSetValue(nCurShader,"dBlend",&dst,sizeof(int));
	CAssetManager::ShaderSetValue(nCurShader,"gParticleScale",&scale,sizeof(float));
	CAssetManager::ApplyTexture(nCurShader,pEmitter->GetParticles()->GetTextureID());
	CAssetManager::ShaderCommitChanges(nCurShader);

	CView::sm_cRenderer.SetStreamSource(m_pParticleBuffer, sizeof(TParticleVert));
	
	CAssetManager::ShaderBeginPass(nCurShader, uiPass);

	CView::sm_cRenderer.DrawPrimitive(D3DPT_POINTLIST,pEmitter->GetAliveCount());

	CAssetManager::ShaderEndPass(nCurShader);

	return;
}

void CRenderInstanceParticles::ParticleBufferWork(void)
{
	if(!GetObject())
		return;

	CEmitter* pEmitter = static_cast<CEmitter*>(GetObject());

	if(!pEmitter->GetActive() || pEmitter->GetAliveCount() <= 0)
		return;

	TParticleVert* verts = new TParticleVert[pEmitter->GetAliveCount()];
	SecureZeroMemory(verts,sizeof(TParticleVert) * pEmitter->GetAliveCount());

	unsigned int vertIndex = 0;

	// Create raw vert buffer data 
	for(int i = 0; i < pEmitter->GetParticleCount(); ++i)
	{
		if(pEmitter->GetParticles()[i].GetAlive())
		{			
			verts[vertIndex].m_d3dPosition.x = pEmitter->GetParticles()[i].GetWorldMatrix()->axis_pos.x;	
			verts[vertIndex].m_d3dPosition.y = pEmitter->GetParticles()[i].GetWorldMatrix()->axis_pos.y;
			verts[vertIndex].m_d3dPosition.z = pEmitter->GetParticles()[i].GetWorldMatrix()->axis_pos.z;
			verts[vertIndex].m_d3dColor.x = pEmitter->GetParticles()[i].GetParticleColor().r; // Store color in normal data. - JC
			verts[vertIndex].m_d3dColor.y = pEmitter->GetParticles()[i].GetParticleColor().g;
			verts[vertIndex].m_d3dColor.z = pEmitter->GetParticles()[i].GetParticleColor().b;
			verts[vertIndex].m_d3dColor.w = pEmitter->GetParticles()[i].GetParticleColor().a;			
			verts[vertIndex].m_d3dScaleAndRot.x = pEmitter->GetParticles()[i].GetScale(); // Texcoord y = scale - JC
			verts[vertIndex].m_d3dScaleAndRot.y = pEmitter->GetParticles()[i].GetRotation(); // Texcoord x = z rotation. - JC	

			++vertIndex;
		}
	}

	if(!m_pParticleBuffer)
	{
		// Particle buffer only uses [0] of the vert buffer vector - JC
		IDirect3DVertexBuffer9* pVertBuff;
		// This dosen't actually dump any data into it, just creates the buffer by abusing 
		// Dan's create buffer function. - JC
		CView::sm_cRenderer.CreateDynamicBuffer(&pVertBuff,pEmitter->GetParticleCount() * sizeof(TParticleVert));

		m_pParticleBuffer = pVertBuff;
	}

	// Re-writing over top of old verts. - JC
	void* mem = 0;
	m_pParticleBuffer->Lock(0, 0, &mem, D3DLOCK_DISCARD);	
	memcpy(mem,verts,sizeof(TParticleVert) * pEmitter->GetAliveCount());
	m_pParticleBuffer->Unlock();

	// Color of first particle = this emitter's color
	pEmitter->SetColor(pEmitter->GetParticles()->GetColor());

	delete[] verts;
}
