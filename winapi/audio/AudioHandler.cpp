#include "AudioHandler.h"

void AudioHandler::playSoundFromFile(const char* filename)
{
	HRESULT_ERROR(mciSendString(("play " + std::string(filename)).c_str(), 0, NULL, NULL), "Could Not Play Audio File");
}