#pragma once

#include "BatchRenderer.h"

class ColoredBatch2DRenderer : public BatchRenderer<Colored2DVertex>
{
public:
	ColoredBatch2DRenderer(EngineCore& engine)
		: BatchRenderer<Colored2DVertex>(engine, WEISS_COLORED_BATCH_2D_RENDERER_IEDS,
			WEISS_COLORED_BATCH_2D_RENDERER_VS_SOURCE,
			WEISS_COLORED_BATCH_2D_RENDERER_PS_SOURCE) {  }
};