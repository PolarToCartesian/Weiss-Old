#pragma once

#include "../misc/Bindable.h"

template <typename T>
class Shader : public Bindable<T> {
protected:
	const char* m_sourceFilename;

public:
	Shader(const DeviceInfo& deviceInfo, const char* sourceFilename)
		: Bindable<T>(deviceInfo), m_sourceFilename(sourceFilename) {  }

	virtual void Load() = 0;
};