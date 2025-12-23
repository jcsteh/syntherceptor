/*
 * Syntherceptor
 * Main header
 * Author: James Teh <jamie@jantrid.net>
 * Copyright 2025 James Teh
 * License: GNU General Public License version 2.0
 */

#pragma once
#include <windows.h>
#include <sapiddk.h>
#include <atomic>
#include <atlbase.h>
#include <semaphore>
#include <string>
#include <thread>

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
	void speakWorker();

	std::atomic<ULONG> refCount;
	CComPtr<ISpObjectToken> token;
	std::thread speakThread;
	// Used to signal when SSML is ready to be handled by the speak worker.
	std::binary_semaphore ssmlEvent{0};
	// The SSML to be spoken by the speak worker.
	std::wstring ssml;
	// Used to signal when speech has completed or been cancelled.
	std::binary_semaphore doneEvent{0};
};
