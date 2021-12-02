// CFW1ColorRGBA.h

#ifndef IncludeGuard__FW1_CFW1ColorRGBA
#define IncludeGuard__FW1_CFW1ColorRGBA

#include "CFW1Object.h"


namespace FW1FontWrapper {


// A color
class CFW1ColorRGBA : public CFW1Object<IFW1ColorRGBA> {
	public:
		// IUnknown
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) {
			if (ppvObject == NULL)
				return E_INVALIDARG;

			if (IsEqualIID(riid, __uuidof(IFW1ColorRGBA))) {
				*ppvObject = static_cast<IFW1ColorRGBA*>(this);
				AddRef();
				return S_OK;
			}

			return CFW1Object::QueryInterface(riid, ppvObject);
		}
		
		// IFW1Color32
		virtual void STDMETHODCALLTYPE SetColor(UINT32 Color) {
			m_color32 = Color;
		}
		virtual void STDMETHODCALLTYPE SetColor(FLOAT Red, FLOAT Green, FLOAT Blue, FLOAT Alpha) {
			UINT32 color32;
			BYTE* colorBytes = reinterpret_cast<BYTE*>(&color32);
			colorBytes[0] = static_cast<BYTE>(Red * 255.0f + 0.5f);
			colorBytes[1] = static_cast<BYTE>(Green * 255.0f + 0.5f);
			colorBytes[2] = static_cast<BYTE>(Blue * 255.0f + 0.5f);
			colorBytes[3] = static_cast<BYTE>(Alpha * 255.0f + 0.5f);

			m_color32 = color32;
		}
		virtual void STDMETHODCALLTYPE SetColor(const FLOAT* pColor) {
			SetColor(pColor[0], pColor[1], pColor[2], pColor[3]);
		}
		virtual void STDMETHODCALLTYPE SetColor(const BYTE* pColor) {
			UINT32 color32;
			BYTE* colorBytes = reinterpret_cast<BYTE*>(&color32);
			for (int i = 0; i < 4; ++i)
				colorBytes[i] = pColor[i];

			m_color32 = color32;
		}
		
		virtual UINT32 STDMETHODCALLTYPE GetColor32() {
			return m_color32;
		}
	
	// Public functions
	public:
		CFW1ColorRGBA() :
			m_color32(0xffffffff)
		{
		}
		
		HRESULT initColor(IFW1Factory* pFW1Factory, UINT32 initialColor32) {
			HRESULT hResult = initBaseObject(pFW1Factory);
			if (FAILED(hResult))
				return hResult;

			m_color32 = initialColor32;

			return S_OK;
		}
	
	// Internal functions
	private:
		virtual ~CFW1ColorRGBA() {
		}

	
	// Internal data
	private:
		UINT32						m_color32;
};


}// namespace FW1FontWrapper


#endif// IncludeGuard__FW1_CFW1ColorRGBA
