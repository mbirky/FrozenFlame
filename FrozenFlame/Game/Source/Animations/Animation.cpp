#include "Animation.h"

#include "../Util/Matrix4x4.h"
#include "AnimEvent.h"

#include <fstream>
using std::ifstream;

// Default Constructor - Initialize data members to safe values
CAnimation::CAnimation(void)
{
	m_fDuration = 0.0f;
	m_fActualDuration = 0.0f;
}

// Load - Load animation data from a file
bool CAnimation::Load(const char* szFilename)
{
	if(szFilename)
	{
		ifstream in;

		in.open(szFilename, std::ios_base::in | std::ios_base::binary);

		if(in.is_open())
		{
			// Load animation name
			UINT32 animNameSize;
			in.read((char*)&animNameSize, sizeof(UINT32));
			char animName[128];
			in.read((char*)animName, animNameSize);

			// Load animation duration
			double duration;
			in.read((char*)&duration, sizeof(double));
			m_fActualDuration = m_fDuration = (float)duration;

			UINT32 numBones = 0;
			in.read((char*)&numBones, sizeof(UINT32));

			m_vJoints.reserve(numBones);
			for(UINT32 i = 0; i < numBones; ++i)
			{
				TJoint joint;

				UINT32 numKey = 0;
				in.read((char*)&numKey, sizeof(UINT32));

				joint.m_vKeyFrames.reserve(numKey);
				for(UINT32 j = 0; j < numKey; ++j)
				{
					CKeyFrame keyFrame;

					double keytime;
					in.read((char*)&keytime, sizeof(double));
					keyFrame.SetTime((float)keytime);

					matrix4f localMatrix;
					matrix4f worldMatrix;

					for(int x = 0; x < 16; ++x)
					{
						double d;
						in.read((char*)&d, sizeof(double));
						localMatrix.ma[x] = (float)d;
					}
					localMatrix.axis_z *= -1.0f;
					localMatrix.xz *= -1.0f;
					localMatrix.yz *= -1.0f;
					localMatrix.zz *= -1.0f;
					localMatrix.axis_pos.z *= -1.0f;

					for(int y = 0; y < 16; ++y)
					{
						double d;
						in.read((char*)&d, sizeof(double));
						worldMatrix.ma[y] = (float)d;
					}
					worldMatrix.axis_z *= -1.0f;
					worldMatrix.xz *= -1.0f;
					worldMatrix.yz *= -1.0f;
					worldMatrix.zz *= -1.0f;
					worldMatrix.axis_pos.z *= -1.0f;

					keyFrame.SetLocalTransform(D3DXMATRIX(localMatrix.ma));
					keyFrame.SetWorldTransform(D3DXMATRIX(worldMatrix.ma));

					if(j > 0)
					{
						joint.m_vKeyFrames[j - 1].SetTweenTime(
							keyFrame.GetTime() - joint.m_vKeyFrames[j - 1].GetTime());
					}

					joint.m_vKeyFrames.push_back(keyFrame);
				}

				joint.m_vKeyFrames[numKey - 1].SetTweenTime(
					m_fDuration - joint.m_vKeyFrames[numKey - 1].GetTime());

				m_vJoints.push_back(joint);
			}

			in.close();
			in.clear();
		}
	}
	return true;
}

// Destructor - Clean up
CAnimation::~CAnimation(void)
{
	if(m_vJoints.size())
	{
		m_vJoints.clear();
	}

	for(vector<const IAnimEvent*>::iterator iter = m_vpAnimEvents.begin();
		iter != m_vpAnimEvents.end(); ++iter)
	{
		if(*iter)
		{
			delete (*iter);
			(*iter) = nullptr;
		}
	}
	m_vpAnimEvents.clear();
}

bool CAnimation::LoadEvents(const char* szFilename)
{
	// If the filename is null return false
	if(!szFilename)
		return false;

	// Create an ifstream and try to open the file
	ifstream in;
	in.open(szFilename);

	if(in.is_open())
	{
		// If it's open and while it's good
		while(in.good())
		{
			// Try to get a command
			char buffer[128];
			in >> buffer;

			// If the command is a translate event
			if(strcmp(buffer, "linear_translate") == 0)
			{
				// Get the start and end frames
				int startFrame, endFrame;
				in >> startFrame >> endFrame;

				// Ignore a new line
				in.ignore(INT_MAX, '\n');

				// Get the translation data
				float transX, transY, transZ;
				in >> transX >> transY >> transZ;

				// Convert that into a translation per second based on the animation data
				float startTime = m_vJoints[0].m_vKeyFrames[startFrame].GetTime();
				float endTime = m_vJoints[0].m_vKeyFrames[endFrame].GetTime() + m_vJoints[0].m_vKeyFrames[endFrame].GetTweenTime();
				float delta = endTime - startTime;

				// Create the event and push it to the vector
				CTranslateAnimEvent* pEvent =
					new CTranslateAnimEvent(transX / delta, transY / delta, transZ / delta, startFrame, endFrame);
				m_vpAnimEvents.push_back(pEvent);
			}
			// Else if the command is a speed multiplier event
			else if(strcmp(buffer, "speed_mult") == 0)
			{
				// Get the start and end frames
				int startFrame, endFrame;
				in >> startFrame >> endFrame;

				// Ignore a new line
				in.ignore(INT_MAX, '\n');

				// Get the speed multiplier
				float speedMult;
				in >> speedMult;

				// Create the event and push it to the vector
				CSpeedAnimEvent* pEvent =
					new CSpeedAnimEvent(speedMult, startFrame, endFrame);
				m_vpAnimEvents.push_back(pEvent);
			}

			// If we reached the end of file break
			if(in.eof())
				break;
		}

		in.close();
		in.clear();


		// We opened the file and closed it so we succeeded
		return true;
	}

	// If we didn't reach the other return something went wrong
	return false;
}
