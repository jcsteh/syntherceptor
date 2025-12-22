#include "syntherceptor.h"

extern "C" {
__declspec(dllimport) int nvdaController_speakText(const wchar_t* text);
__declspec(dllimport) int nvdaController_cancelSpeech();
}

Syntherceptor::Syntherceptor() : refCount(1) {}

STDMETHODIMP Syntherceptor::QueryInterface(REFIID riid, void** ppv) {
	if (!ppv)
		return E_POINTER;
	*ppv = nullptr;

	if (riid == IID_IUnknown || riid == __uuidof(ISpTTSEngine))
		*ppv = static_cast<ISpTTSEngine*>(this);
	else if (riid == __uuidof(ISpObjectWithToken))
		*ppv = static_cast<ISpObjectWithToken*>(this);
	else
		return E_NOINTERFACE;

	AddRef();
	return S_OK;
}

ULONG Syntherceptor::AddRef() {
	return ++refCount;
}

ULONG Syntherceptor::Release() {
	ULONG c = --refCount;
	if (!c)
		delete this;
	return c;
}

STDMETHODIMP Syntherceptor::Speak(
	DWORD flags, REFGUID, const WAVEFORMATEX*, const SPVTEXTFRAG* fragList,
	ISpTTSEngineSite* site
) {
	if (flags & SPF_PURGEBEFORESPEAK)
		nvdaController_cancelSpeech();

	for (auto frag = fragList; frag; frag = frag->pNext) {
		if (site->GetActions() & SPVES_ABORT) {
			nvdaController_cancelSpeech();
			break;
		}

		if (frag->pTextStart && frag->ulTextLen) {
			std::wstring text(frag->pTextStart, frag->ulTextLen);
			nvdaController_speakText(text.c_str());
		}
	}
	return S_OK;
}

STDMETHODIMP Syntherceptor::
	GetOutputFormat(const GUID*, const WAVEFORMATEX*, GUID* formatId, WAVEFORMATEX**) {
	*formatId = SPDFID_Text;
	return S_OK;
}

STDMETHODIMP Syntherceptor::SetObjectToken(ISpObjectToken* pToken) {
	token = pToken;
	return S_OK;
}

STDMETHODIMP Syntherceptor::GetObjectToken(ISpObjectToken** ppToken) {
	if (!ppToken)
		return E_POINTER;
	*ppToken = token;
	if (*ppToken)
		(*ppToken)->AddRef();
	return S_OK;
}
