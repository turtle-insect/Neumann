#include "Font.hpp"

Font& Font::Instance()
{
	static Font instance;
	return instance;
}

Font::Font()
	: mFontLibrary(nullptr)
	, mFontFace(nullptr)
	, mFontFaceState(1)
{
	u64 language = 0;
	Result rc = setInitialize();
	if (R_SUCCEEDED(rc)) rc = setGetSystemLanguage(&language);
	setExit();

	size_t totalCount;
	PlFontData fonts[PlSharedFontType_Total];
	rc = plGetSharedFont(language, fonts, PlSharedFontType_Total, &totalCount);
	if(R_FAILED(rc)) return;

	PlFontData font;
	rc = plGetSharedFontByType(&font, PlSharedFontType_Standard);
	if(R_FAILED(rc)) return;

	FT_Error error = FT_Init_FreeType(&mFontLibrary);
	if(error) return;

	mFontFaceState = FT_New_Memory_Face(mFontLibrary, (const FT_Byte*)font.address, font.size, 0, &mFontFace);
}

Font::~Font()
{
	FT_Done_Face(mFontFace);
	FT_Done_FreeType(mFontLibrary);
}

bool Font::GetGlyph(Glyph& glyph, size_t size, u32 code)
{
	if(mFontLibrary == nullptr) return false;
	if(mFontFace == nullptr) return false;
	if(mFontFaceState) return false;

	FT_Error error = FT_Set_Char_Size(mFontFace, 0, size << 6, 300, 300);
	if(error) return false;

	FT_UInt glyphIndex = FT_Get_Char_Index(mFontFace, code);
	error = FT_Load_Glyph(mFontFace, glyphIndex, FT_LOAD_DEFAULT);
	if(error) return false;

	error = FT_Render_Glyph(mFontFace->glyph, FT_RENDER_MODE_NORMAL);
	if(error) return false;

	FT_Bitmap* bitmap = &mFontFace->glyph->bitmap;
	if(bitmap->pixel_mode != FT_PIXEL_MODE_GRAY) return false;

	glyph.width = bitmap->width;
	glyph.height = bitmap->rows;
	glyph.posX = mFontFace->glyph->bitmap_left;
	glyph.posY = mFontFace->glyph->bitmap_top;
	glyph.advanceX = mFontFace->glyph->advance.x >> 6;
	glyph.pitch = bitmap->pitch;
	glyph.data = bitmap->buffer;
	return true;
}
