#pragma once
#include <windows.h>
#include <sapi.h>
#include <atomic>
#include <atlbase.h>

class Syntherceptor : public ISpTTSEngine, public ISpObjectWithToken {
	public:
	Syntherceptor();

	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override;
	STDMETHODIMP_(ULONG) AddRef() override;
	STDMETHODIMP_(ULONG) Release() override;

	// ISpTTSEngine
	STDMETHODIMP Speak(
		DWORD dwSpeakFlags, REFGUID rguidFormatId,
		const WAVEFORMATEX* pWaveFormatEx, const SPVTEXTFRAG* pTextFragList,
		ISpTTSEngineSite* pOutputSite
	) override;

	STDMETHODIMP GetOutputFormat(
		const GUID* pTargetFormatId, const WAVEFORMATEX* pTargetWaveFormatEx,
		GUID* pOutputFormatId, WAVEFORMATEX** ppCoMemOutputWaveFormatEx
	) override;

	// ISpObjectWithToken
	STDMETHODIMP SetObjectToken(ISpObjectToken* pToken) override;
	STDMETHODIMP GetObjectToken(ISpObjectToken** ppToken) override;

	private:
	std::atomic<ULONG> refCount;
	CComPtr<ISpObjectToken> token;
};
