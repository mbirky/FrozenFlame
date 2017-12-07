#ifndef _RENDER_INSTANCE_PARTICLES_H_
#define _RENDER_INSTANCE_PARTICLES_H_

#include "RenderInstance.h"

class CRenderInstanceParticles : public CRenderInstance
{
	// Vertex buffer that contains all the 2d particles
	IDirect3DVertexBuffer9* m_pParticleBuffer;

	// Helper function to creates / recalculates VertexBuffers for particles
	void ParticleBufferWork(void);

public:
	// Constructor
	CRenderInstanceParticles(CBaseObject* pObject);
	// Destructor
	~CRenderInstanceParticles(void);
	// Updates the Render Instance
	void Update(float fElapsedTime);
	// Render the instance with the passed in shader
	void Render(int nCurShader, UINT uiPass);

	// Accessors
	virtual inline EType GetType(void) const { return PARTICLES; }

};

#endif