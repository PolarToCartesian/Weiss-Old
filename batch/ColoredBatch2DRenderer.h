#pragma once

#include "Batch2DRenderer.h"

class ColoredBatch2DRenderer : public Batch2DRenderer<Colored2DVertex>
{
public:
	ColoredBatch2DRenderer(EngineCore& engine)
		: Batch2DRenderer<Colored2DVertex>(engine, WEISS_COLORED_BATCH_2D_RENDERER_IEDS,
			WEISS_COLORED_BATCH_2D_RENDERER_VS_SOURCE,
			WEISS_COLORED_BATCH_2D_RENDERER_PS_SOURCE) {  }
};