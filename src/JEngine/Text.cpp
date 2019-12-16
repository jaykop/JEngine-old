#include "Text.h"
#include "Transform.h"
#include "AssetManager.h"
#include "SystemManager.h"
#include "Object.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Text);

std::vector<unsigned> Text::m_pointIndices = { 0, 2, 3, 1, 0, 2 };

Font::Font()
	:fontSize_(0), newline_(0.f)
{}

Text::Text(Object* pOwner)
	:Model(pOwner)
{
	is_ |= IS_TEXT;
}

Text::~Text()
{
	if (pTextBuffer_) {
		delete[] pTextBuffer_;
		pTextBuffer_ = nullptr;
	}

	if (pwTextBuffer) {
		delete[] pwTextBuffer;
		pwTextBuffer = nullptr;
	}

	SYSTEM::pGraphic_->RemoveModel(this);
}

void Text::operator=(const Text & copy)
{
	pFont = copy.pFont;
	text_ = copy.text_;
	wText_ = copy.wText_;
	printWide_ = copy.printWide_;
}

void Text::Register()
{
	SYSTEM::pGraphic_->AddModel(this);
	if (GetOwner()->HasComponent<Transform>())
		pTransform_ = GetOwner()->GetComponent<Transform>();
}

void Text::SetText(const char * pText, ...)
{
	// Set render to print ascii characters
	printWide_ = false;

	// Clear wide character conatiner
	if (!wText_.empty()) {
		wText_.clear();
		delete[] pwTextBuffer;
		pwTextBuffer = nullptr;
	}

	if (pText)
	{
		static unsigned newSize = 0;
		text_.assign(pText); 
		va_list argumens;

		va_start(argumens, pText);
		
		// Get size of new text
		newSize = _vscprintf(pText, argumens) + 1;

		// If the new size is greater than old one,
		// delete existing one and reallocate heap memories
		if (size_ < newSize) {
			size_ = newSize;					// Refresh the size info
			delete[] pTextBuffer_;			// Delete heap
			pTextBuffer_ = nullptr;
			pTextBuffer_ = new char[size_];	// Reallocate memory
		}

		vsprintf_s(pTextBuffer_, size_, pText, argumens);
		va_end(argumens);

		// Refresh the text with additional arguments
		text_ = pTextBuffer_;
	}
}

const std::string& Text::GetText(void) const
{
	return text_;
}

void Text::SetText(const wchar_t* pwText, ...)
{
	// Set render to print wide characters
	printWide_ = true;

	// Clear ascii texts container
	if (!text_.empty()) {
		text_.clear();
		delete[] pTextBuffer_;
		pTextBuffer_ = nullptr;
	}

	if (pwText)
	{
		static size_t newSize = 0;
		wText_.assign(pwText);
		va_list argumens;

		va_start(argumens, pwText);
		// Get new size of the new text
		newSize = _vscwprintf(pwText, argumens) + 1;

		// If new size is greater than one,
		// reallocate the new heap memories
		if (size_ < newSize) {
			size_ = newSize;						// Initialize new size
			delete[] pwTextBuffer;				// Deallocate heap memories
			pwTextBuffer = nullptr;
			pwTextBuffer = new wchar_t[size_];	// Reallocate new memories
		}

		vswprintf_s(pwTextBuffer, size_, pwText, argumens);
		va_end(argumens);

		// Refresh the text with additional arguments
		wText_ = pwTextBuffer;
	}
}

const std::wstring& Text::GetWText() const
{
	return wText_;
}

void Text::Load(CR_RJValue data)
{
	if (data.HasMember("Text")) {
		CR_RJValue loadedText = data["Text"];
		text_.assign(loadedText.GetString());
	}

	if (data.HasMember("Font")) {
		CR_RJValue loadedFont = data["Font"];
		pFont = ASSET::GetFont(loadedFont.GetString());
	}

	if (data.HasMember("Flip")
		&& data["Flip"].GetBool())
		status_ |= IS_FLIPPED;

	if (data.HasMember("Color")) {
		CR_RJValue loadedColor = data["Color"];
		color_.Set(loadedColor[0].GetFloat(), loadedColor[1].GetFloat(),
			loadedColor[2].GetFloat(), loadedColor[3].GetFloat());
	}

	if (data.HasMember("Projection")) {
		CR_RJValue loadedProjection = data["Projection"];

		if (!strcmp("Perspective", loadedProjection.GetString()))
			projection_ = PROJECTION_PERSPECTIVE;

		else if (!strcmp("Orthogonal", loadedProjection.GetString()))
			projection_ = PROJECTION_ORTHOGONAL;

		else
			jeDebugPrint("!Model - Wrong projection type: %s\n", loadedProjection.GetString());
	}

	if (data.HasMember("Bilboard")
		&& data["Bilboard"].GetBool())
		status_ |= IS_BILBOARD;
}

void Text::EditorUpdate(const float /*dt*/)
{
	// TODO
}

jeEnd

