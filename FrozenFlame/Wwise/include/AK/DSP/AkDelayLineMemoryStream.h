/***********************************************************************
The content of this file includes source code for the sound engine
portion of the AUDIOKINETIC Wwise Technology and constitutes "Level
Two Source Code" as defined in the Source Code Addendum attached
with this file.  Any use of the Level Two Source Code shall be
subject to the terms and conditions outlined in the Source Code
Addendum and the End User License Agreement for Wwise(R).

Version: <VERSION>  Build: <BUILDNUMBER>
Copyright (c) <COPYRIGHTYEAR> Audiokinetic Inc.
***********************************************************************/

// Length of delay line is mapped on 4 frames boundary (i.e. may not be suited for reverberation for example)
// Handling code for triple buffering processing on position independent code platforms is provided
// This is not a delay line implementation, but rather just some services for memory managment related 
// to specific delay line execution needs as detailed by clients
#include <AK/AkPlatforms.h>
#ifdef AK_PS3
#include "PS3/AkDelayLineMemoryStream.h"
#else
//Default implementation

#ifndef _AKDSP_DELAYLINEMEMORYSTREAM_
#define _AKDSP_DELAYLINEMEMORYSTREAM_

#include <AK/DSP/AkDelayLineMemory.h>

namespace AK
{
	namespace DSP
	{
		template < class T_SAMPLETYPE, AkUInt32 T_MAXNUMCHANNELS > 
		class CAkDelayLineMemoryStream : public CAkDelayLineMemory< T_SAMPLETYPE, T_MAXNUMCHANNELS > 
		{
		public:

			T_SAMPLETYPE * GetCurrentPointer( AkUInt32 in_uOffset, AkUInt32 in_uChannelIndex )
			{
				return this->m_pDelay[in_uChannelIndex] + in_uOffset;
				}
		};
	} // namespace DSP
} // namespace AK

#endif // _AKDSP_DELAYLINEMEMORYSTREAM_
#endif //AK_PS3