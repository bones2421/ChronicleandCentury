/*******************************************************************************
The content of this file includes portions of the proprietary AUDIOKINETIC Wwise
Technology released in source code form as part of the game integration package.
The content of this file may not be used without valid licenses to the
AUDIOKINETIC Wwise Technology.
Note that the use of the game engine is subject to the Unreal(R) Engine End User
License Agreement at https://www.unrealengine.com/en-US/eula/unreal
 
License Usage
 
Licensees holding valid licenses to the AUDIOKINETIC Wwise Technology may use
this file in accordance with the end user license agreement provided with the
software or, alternatively, in accordance with the terms contained
in a written agreement between you and Audiokinetic Inc.
Copyright (c) 2022 Audiokinetic Inc.
*******************************************************************************/

#pragma once

#include "GenericPlatform/GenericPlatformFile.h"
#include "AkInclude.h"
#include "Wwise/WwiseExecutionQueue.h"

class FWwiseAsyncCycleCounter;
class FWwiseFileCacheHandle;
class IAsyncReadRequest;

using FWwiseFileOperationDone = TUniqueFunction<void(bool bResult)>;
using FWwiseAkFileOperationDone = TUniqueFunction<void(AkAsyncIOTransferInfo* TransferInfo, AKRESULT AkResult)>;

/**
 * Wwise File Cache manager.
 *
 * This is a simple Wwise version of Unreal's complex FFileCache.
 *
 * WwiseFileHandler module already opens any file only once, so we don't need a global cache.
 * We also only need preloading the file start, which makes it easier to handle.
 *
 * We only want to keep "n" values per file in the cache to reduce the amount of storage
 * reading, as well as permanently keeping the preloaded data.
 *
 * Finally, compared to Unreal's FFileCache, we want to process everything asynchronously,
 * including file opening in the unlikely possibility the file is not accessible or present.
 */
class WWISEFILEHANDLER_API FWwiseFileCache
{
public:
	static FWwiseFileCache* Get();

	FWwiseFileCache();
	virtual ~FWwiseFileCache();
	virtual void CreateFileCacheHandle(FWwiseFileCacheHandle*& OutHandle, const FString& Pathname, int64 PreloadSize, int64 Granularity, FWwiseFileOperationDone&& OnDone);

	virtual EAsyncIOPriorityAndFlags PreloadPriority() const { return AIOP_BelowNormal; }

	FQueuedThreadPool* FileCacheThreadPool = nullptr;
	FWwiseExecutionQueue* ExecutionQueue = nullptr;

private:
	void InitializeFileCacheThreadPool();
};

class WWISEFILEHANDLER_API FWwiseFileCacheHandle
{
public:
	FWwiseFileCacheHandle(const FString& Pathname, int64 PreloadSize, EAsyncIOPriorityAndFlags PreloadPriority, int64 Granularity);
	virtual ~FWwiseFileCacheHandle();

	virtual void Open(FWwiseFileOperationDone&& OnDone);

	virtual void ReadData(uint8* OutBuffer, int64 Offset, int64 BytesToRead, EAsyncIOPriorityAndFlags Priority, FWwiseFileOperationDone&& OnDone);
	void ReadAkData(uint8* OutBuffer, int64 Offset, int64 BytesToRead, int8 AkPriority, FWwiseFileOperationDone&& OnDone);
	void ReadAkData(const AkIoHeuristics& Heuristics, AkAsyncIOTransferInfo& TransferInfo, FWwiseAkFileOperationDone&& Callback);

	const FString& GetPathname() const { return Pathname; }
	int64 GetFileSize() const { return FileSize; }

protected:
	FString Pathname;

	IAsyncReadFileHandle* FileHandle;
	int64 FileSize;

	int64 PreloadSize;
	EAsyncIOPriorityAndFlags PreloadPriority;
	uint8* PreloadData;

	int64 Granularity;

	FWwiseFileOperationDone InitializationDone;
	FWwiseAsyncCycleCounter* InitializationStat;

	TAtomic<int32> ReadDataInProcess { 0 };

	virtual void OnSizeRequestDone(bool bWasCancelled, IAsyncReadRequest* Request);
	virtual void ProcessPreload();
	virtual void OnPreloadRequestDone(bool bResult, uint8* TempPreloadData);
	virtual void OnReadDataDone(bool bWasCancelled, IAsyncReadRequest* Request, FWwiseFileOperationDone&& OnDone);
	virtual void OnReadDataDone(bool bResult, FWwiseFileOperationDone&& OnDone);
	virtual void CallDone(bool bResult, FWwiseFileOperationDone&& OnDone);
};
