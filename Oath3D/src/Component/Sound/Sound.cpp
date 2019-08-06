#include "Common/Common.h"
#include "Sound.h"

#include "AL/al.h"
#include "AL/alc.h"

void Oath::Comp::Emitter::setState(int s) {
	if (!source)
		return;

	ALint source_state;
	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	
	if (source_state == s)
		return;

	if (s == AL_PAUSED)
		alSourcePause(source);
	else if (s == AL_STOPPED)
		alSourceStop(source);
	else if (s == AL_PLAYING)
		alSourcePlay(source);
}
