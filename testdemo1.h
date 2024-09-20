#pragma once

#include "resource.h"
#include<mftransform.h>
#include<mfapi.h>
#include<wmcodecdsp.h>
#include<codecapi.h>
#include<Mferror.h>
#include<Windows.h>
#include<mfidl.h>
#include<mfreadwrite.h>
#include<d3d11.h>
#include<dxgi1_2.h>


#include<chrono>
#include<thread>


#pragma comment(lib,"mfplat.lib")
#pragma comment(lib,"mfuuid.lib")
#pragma comment(lib,"wmcodecdspuuid.lib")
#pragma comment(lib,"mf.lib")
#pragma comment(lib,"DWrite.lib")
#pragma comment(lib,"Mfreadwrite.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"winmm.lib")


UINT32 width = 0, height = 0;

HWND hwnd;


ID3D11Device* device = NULL;
ID3D11DeviceContext* ctx = NULL;


IDXGIOutputDuplication* dxgiOutputDuplication = NULL;

UINT64 duration;

DXGI_OUTDUPL_DESC dxgi_outdupl_desc{};

HRESULT sink_writer(IMFSinkWriter** ppWriter, DWORD* pStreamIndex, LPCWSTR pwszOutputUrl) {
	*ppWriter = NULL;
	*pStreamIndex = NULL;

	IMFSinkWriter* pSinkWriter = NULL;
	IMFMediaType* pMediaType = NULL;
	IMFMediaType* pMediaType1 = NULL;
	DWORD streamIndex;


	IMFAttributes* attributes = NULL;
	HRESULT hr = S_OK;

	hr = MFCreateAttributes(&attributes, 0);
	if (FAILED(hr)) {
		return hr;
	}
	//hr = attributes->SetGUID(MF_TRANSCODE_CONTAINERTYPE, MFTranscodeContainerType_MPEG4);
	//if (FAILED(hr)) {
	//	return hr;
	//}

	hr = attributes->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, TRUE);

	hr = MFCreateSinkWriterFromURL(pwszOutputUrl, NULL, attributes, &pSinkWriter);
	if (FAILED(hr)) {
		return hr;
	}
	hr = MFCreateMediaType(&pMediaType);
	if (FAILED(hr)) {
		return hr;
	}
	hr = pMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	if (FAILED(hr)) {
		return hr;
	}
	hr = pMediaType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_WMV3);
	//hr = pMediaType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
	if (FAILED(hr)) {
		return hr;
	}
	hr = pMediaType->SetUINT32(MF_MT_AVG_BITRATE, 8000000u);
	if (FAILED(hr)) {
		return hr;
	}
	hr = pMediaType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
	if (FAILED(hr)) {
		return hr;
	}
	hr = MFSetAttributeSize(pMediaType, MF_MT_FRAME_SIZE, width, height);
	if (FAILED(hr)) {
		return hr;
	}
	hr = MFSetAttributeRatio(pMediaType, MF_MT_FRAME_RATE, 30u, 1u);
	if (FAILED(hr)) {
		return hr;
	}
	hr = MFSetAttributeRatio(pMediaType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
	if (FAILED(hr)) {
		return hr;
	}
	//hr = pMediaType->SetUINT32(MF_MT_VIDEO_PROFILE, eAVEncH264VProfile_High10);
	//if (FAILED(hr)) {
	//	return hr;
	//}
	hr = pSinkWriter->AddStream(pMediaType, &streamIndex);
	if (FAILED(hr)) {
		return hr;
	}
	hr = MFCreateMediaType(&pMediaType1);
	if (FAILED(hr)) {
		return hr;
	}
	hr = pMediaType1->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	if (FAILED(hr)) {
		return hr;
	}
	//hr = pMediaType1->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
	hr = pMediaType1->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_ARGB32);
	//hr = pMediaType1->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_NV12);
	if (FAILED(hr)) {
		return hr;
	}
	hr = MFSetAttributeSize(pMediaType1, MF_MT_FRAME_SIZE, width, height);
	if (FAILED(hr)) {
		return hr;
	}
	hr = pMediaType1->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
	if (FAILED(hr)) {
		return hr;
	}
	hr = MFSetAttributeRatio(pMediaType1, MF_MT_FRAME_RATE, 30u, 1u);
	if (FAILED(hr)) {
		return hr;
	}
	hr = MFSetAttributeRatio(pMediaType1, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
	if (FAILED(hr)) {
		return hr;
	}
	//IMFAttributes* attr = NULL;
	//hr = MFCreateAttributes(&attr, 0);
	//if (SUCCEEDED(hr)) {
	//	hr = attr->SetUINT32(CODECAPI_AVEncCommonRateControlMode, eAVEncCommonRateControlMode_GlobalVBR);
	//	hr = attr->SetUINT32(CODECAPI_AVEncCommonMeanBitRate, 5000000);
	//	hr = attr->SetUINT32(CODECAPI_AVEncMPVDefaultBPictureCount, 0);
	//	attr->SetUINT32(CODECAPI_AVEncCommonQuality, 90);
	//	attr->SetUINT32(CODECAPI_AVEncCommonLowLatency, 1);
	//	hr = attr->SetGUID(MF_TRANSCODE_CONTAINERTYPE, MFTranscodeContainerType_FMPEG4);
	//	hr = pSinkWriter->SetInputMediaType(streamIndex, pMediaType1, NULL);
	//}
	//else {
	//	hr = pSinkWriter->SetInputMediaType(streamIndex, pMediaType1, NULL);
	//}
	hr = pSinkWriter->SetInputMediaType(streamIndex, pMediaType1, NULL);
	if (FAILED(hr)) {
		return hr;
	}

	IMFMediaType* mediaType_ = NULL;
	hr = MFCreateMediaType(&mediaType_);
	if (FAILED(hr)) {
		return hr;
	}
	hr = mediaType_->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	if (FAILED(hr)) {
		return hr;
	}
	hr = mediaType_->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_AAC);
	if (FAILED(hr)) {
		return hr;
	}
	hr = mediaType_->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 16);
	if (FAILED(hr)) {
		return hr;
	}
	hr = mediaType_->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, 44100);
	if (FAILED(hr)) {
		return hr;
	}
	hr = mediaType_->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, 2);
	if (FAILED(hr)) {
		return hr;
	}
	hr = mediaType_->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, 12000);
	if (FAILED(hr)) {
		return hr;
	}

	IMFMediaType* mediaType_wav = NULL;
	hr = MFCreateMediaType(&mediaType_wav);
	if (FAILED(hr)) {
		return hr;
	}
	WAVEFORMATEX wav_format{};
	wav_format.wFormatTag = WAVE_FORMAT_PCM;
	wav_format.nChannels = 2;
	wav_format.nSamplesPerSec = 44100;
	wav_format.nAvgBytesPerSec = 176400;
	wav_format.nBlockAlign = 4;
	wav_format.wBitsPerSample = 32;
	wav_format.cbSize = 0;
	hr = MFInitMediaTypeFromWaveFormatEx(mediaType_wav, &wav_format, sizeof(wav_format));
	if (FAILED(hr)) {
		return hr;
	}

	//hr = pSinkWriter->AddStream(mediaType_, 0);
	//hr = pSinkWriter->SetInputMediaType(0, mediaType_wav, NULL);

	hr = pSinkWriter->BeginWriting();
	if (FAILED(hr)) {
		return hr;
	}
	//pSinkWriter->Finalize();
	*ppWriter = pSinkWriter;
	(*ppWriter)->AddRef();
	*pStreamIndex = streamIndex;
	pSinkWriter->Release();
	pSinkWriter = NULL;
	pMediaType->Release();
	pMediaType = NULL;
	pMediaType1->Release();
	pMediaType1 = NULL;

	return S_OK;
}


void get(HWND h) {
	hwnd = h;

	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;
	//flags = D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_10_1,D3D_FEATURE_LEVEL_10_0 };
	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) {
		return;
	}

	hr = CoInitialize(NULL);

	D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE,D3D_DRIVER_TYPE_WARP,D3D_DRIVER_TYPE_REFERENCE };
	for (DWORD i = 0;i < ARRAYSIZE(driverTypes);i++) {
		hr = D3D11CreateDevice(NULL, driverTypes[i], NULL, flags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &device, &featureLevel, &ctx);
		if (SUCCEEDED(hr)) {
			break;
		}
	}

	hr = MFStartup(MF_VERSION, 0);


	IDXGIDevice1* dxgiDevice = NULL;
	hr = device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
	if (FAILED(hr)) {
		return;
	}
	IDXGIAdapter* adapter = NULL;
	hr = dxgiDevice->GetAdapter(&adapter);
	if (FAILED(hr)) {
		return;
	}
	dxgiDevice->Release();
	dxgiDevice = NULL;
	IDXGIOutput* output = NULL;
	hr = adapter->EnumOutputs(0, &output);
	if (FAILED(hr)) {
		return;
	}
	adapter->Release();
	adapter = NULL;

	DXGI_OUTPUT_DESC desc;
	output->GetDesc(&desc);
	IDXGIOutput1* output1 = NULL;
	hr = output->QueryInterface(__uuidof(IDXGIOutput1), (void**)&output1);
	if (FAILED(hr)) {
		return;
	}
	hr = output1->DuplicateOutput(device, &dxgiOutputDuplication);
	if (FAILED(hr)) {
		return;
	}
	output->Release();
	output1->Release();
	output = NULL;
	output1 = NULL;

	/*DXGI_OUTDUPL_DESC desc1;*/
	dxgiOutputDuplication->GetDesc(&dxgi_outdupl_desc);
	width = dxgi_outdupl_desc.ModeDesc.Width;
	height = dxgi_outdupl_desc.ModeDesc.Height;

	hr = MFFrameRateToAverageTimePerFrame(30, 1, &duration);
	if (FAILED(hr)) {
		duration = 10000 / 3;
	}

	DXGI_OUTDUPL_FRAME_INFO frame_info{};
	IDXGIResource* resource = NULL;

	IMFSinkWriter* writer = NULL;


	DWORD streamIndex = 0;
	hr = sink_writer(&writer, &streamIndex, L"e:\\11test11111.wmv");
	if (FAILED(hr)) {
		return;
	}
	IMFPresentationClock* presentation = NULL;
	IMFPresentationTimeSource* m_pTimeSrc = NULL;
	hr = MFCreatePresentationClock(&presentation);
	if (FAILED(hr)) {
		return;
	}
	hr = MFCreateSystemTimeSource(&m_pTimeSrc);
	if (FAILED(hr)) {
		return;
	}
	hr = presentation->SetTimeSource(m_pTimeSrc);
	if (FAILED(hr)) {
		return;
	}
	hr = presentation->Start(0);
	LONGLONG sampleTime = 0;


	D3D11_TEXTURE2D_DESC desc1{};
	int o = 0;
	while (true) {
		hr = dxgiOutputDuplication->AcquireNextFrame(300, &frame_info, &resource);
		if (SUCCEEDED(hr)) {
			ID3D11Texture2D* texture = NULL;
			hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&texture);
			texture->GetDesc(&desc1);
			ID3D11Texture2D* texture1 = NULL;
			hr = device->CreateTexture2D(&desc1, NULL, &texture1);
			if (SUCCEEDED(hr)) {
				ctx->CopyResource(texture1, texture);
			}
			else {
				continue;
			}

			if (SUCCEEDED(hr)) {
				IDXGISurface* dxgiSurface = NULL;
				hr = texture1->QueryInterface(__uuidof(IDXGISurface), (void**)&dxgiSurface);
				if (SUCCEEDED(hr)) {
					IMFSample* sample = NULL;
					hr = MFCreateSample(&sample);
					if (SUCCEEDED(hr)) {
						IMFMediaBuffer* buffer = NULL;
						hr = MFCreateDXGISurfaceBuffer(__uuidof(ID3D11Texture2D), dxgiSurface, 0, FALSE, &buffer);
						if (SUCCEEDED(hr)) {
							hr = sample->AddBuffer(buffer);
							hr = sample->SetSampleDuration(duration);
							hr = presentation->GetTime(&sampleTime);
							hr = sample->SetSampleTime(sampleTime);
							//sampleTime += duration * 10;

							DWORD length = 0;
							IMF2DBuffer* imf_buffer = NULL;
							hr = buffer->QueryInterface(__uuidof(IMF2DBuffer), (void**)&imf_buffer);
							if (SUCCEEDED(hr)) {
								hr = imf_buffer->GetContiguousLength(&length);
								hr = buffer->SetCurrentLength(length);

								hr = writer->WriteSample(0, sample);

								if (FAILED(hr)) {
									OutputDebugString(L"FAILED WRITE SAMPLE\n");
								}

								o++;
								if (o > 1920) {
									writer->Finalize();

									OutputDebugString(L"OOOOOOOOOO");
									break;
								}
								imf_buffer->Release();
								imf_buffer = NULL;
							}

							buffer->Release();
							buffer = NULL;
						}
						sample->Release();
						sample = NULL;
					}
					dxgiSurface->Release();
					dxgiSurface = NULL;
				}
				texture->Release();
				texture = NULL;
				texture1->Release();
				texture1 = NULL;
			}
			resource->Release();
			resource = NULL;
		}
		dxgiOutputDuplication->ReleaseFrame();

		std::this_thread::sleep_for(std::chrono::milliseconds(333));
	}
}