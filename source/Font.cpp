#include <map>
#include "Font.hpp"

typedef struct
{
	u8 magic[4];
	int version;
	u16 npages;
	u8 height;
	u8 baseline;
} ffnt_header_t;

typedef struct
{
	u32 size, offset;
} ffnt_pageentry_t;

typedef struct
{
	u32 pos[0x100];
	u8 widths[0x100];
	u8 heights[0x100];
	int8_t advances[0x100];
	int8_t posX[0x100];
	int8_t posY[0x100];
} ffnt_pagehdr_t;

typedef struct
{
	ffnt_pagehdr_t hdr;
	u8 data[];
} ffnt_page_t;

extern const ffnt_header_t interuiregular14_nxfnt;
extern const ffnt_header_t interuiregular18_nxfnt;
extern const ffnt_header_t interuiregular20_nxfnt;
extern const ffnt_header_t interuiregular24_nxfnt;

std::map<Font::eType, const ffnt_header_t*> sFontMap;

const ffnt_page_t* getFontPage(Font::eType type, u32 page)
{
	std::map<Font::eType, const ffnt_header_t*>::iterator ite = sFontMap.find(type);
	if (ite == sFontMap.end()) return nullptr;
	const ffnt_header_t* header = ite->second;

	if (page >= header->npages) return nullptr;

	ffnt_pageentry_t* entry = &((ffnt_pageentry_t*)(header + 1))[page];
	if (!entry->size) return nullptr;

	return (const ffnt_page_t*)((const u8*)header + entry->offset);
}

Font& Font::Instance()
{
	static Font instance;
	return instance;
}

Font::Font()
{
	sFontMap.insert(std::map<Font::eType, const ffnt_header_t*>::value_type(eType14, &interuiregular14_nxfnt));
	sFontMap.insert(std::map<Font::eType, const ffnt_header_t*>::value_type(eType18, &interuiregular18_nxfnt));
	sFontMap.insert(std::map<Font::eType, const ffnt_header_t*>::value_type(eType20, &interuiregular20_nxfnt));
	sFontMap.insert(std::map<Font::eType, const ffnt_header_t*>::value_type(eType24, &interuiregular24_nxfnt));
}

Font::~Font()
{
	sFontMap.clear();
}

bool Font::GetGlyph(Glyph& glyph, eType type, u32 code)
{
	const ffnt_page_t* page = getFontPage(type, code >> 8);
	if (!page) return false;

	code &= 0xFF;
	u32 offset = page->hdr.pos[code];
	if (offset == 0xFFFF) return false;

	glyph.width = page->hdr.widths[code];
	glyph.height = page->hdr.heights[code];
	glyph.advance = page->hdr.advances[code];
	glyph.posX = page->hdr.posX[code];
	glyph.posY = page->hdr.posY[code];
	glyph.data = &page->data[offset];

	return true;
}

u8 Font::GetHeight(eType type)
{
	std::map<Font::eType, const ffnt_header_t*>::iterator ite = sFontMap.find(type);
	if (ite == sFontMap.end()) return 0;
	return ite->second->height;
}

u8 Font::GetBaseLine(eType type)
{
	std::map<Font::eType, const ffnt_header_t*>::iterator ite = sFontMap.find(type);
	if (ite == sFontMap.end()) return 0;
	return ite->second->baseline;
}
