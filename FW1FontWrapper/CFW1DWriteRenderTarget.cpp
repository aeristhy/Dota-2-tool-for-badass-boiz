// CFW1DWriteRenderTarget.cpp



#include "CFW1DWriteRenderTarget.h"



namespace FW1FontWrapper {
// Init glyph data
void CFW1DWriteRenderTarget::initGlyphData(
	const DWRITE_FONT_METRICS *fontMetrics,
	const DWRITE_GLYPH_METRICS *glyphMetrics,
	FLOAT fontSize,
	DWGlyphData *outGlyphData
) {
	// Calculate pixel-space coordinates
	FLOAT fscale = fontSize / static_cast<FLOAT>(fontMetrics->designUnitsPerEm);
	
	FLOAT l = static_cast<FLOAT>(glyphMetrics->leftSideBearing) * fscale;
	FLOAT t = static_cast<FLOAT>(glyphMetrics->topSideBearing) * fscale;
	
	FLOAT r = static_cast<FLOAT>(glyphMetrics->rightSideBearing) * fscale;
	FLOAT b = static_cast<FLOAT>(glyphMetrics->bottomSideBearing) * fscale;
	
	FLOAT v = static_cast<FLOAT>(glyphMetrics->verticalOriginY) * fscale;
	
	FLOAT aw = static_cast<FLOAT>(glyphMetrics->advanceWidth) * fscale;
	FLOAT ah = static_cast<FLOAT>(glyphMetrics->advanceHeight) * fscale;
	
	// Set up glyph data
	outGlyphData->offsetX = floor(l);
	outGlyphData->offsetY = floor(t) - floor(v);
	outGlyphData->maxWidth = static_cast<LONG>(aw - r - l + 2.0f);
	outGlyphData->maxHeight = static_cast<LONG>(ah - b - t + 2.0f);
}


}// namespace FW1FontWrapper
