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
Copyright (c) 2023 Audiokinetic Inc.
*******************************************************************************/

#pragma once

#include "Wwise/Metadata/WwiseMetadataLoadable.h"

struct WWISEPROJECTDATABASE_API FWwiseMetadataExternalSourceReference : public FWwiseMetadataLoadable
{
	uint32 Cookie;

	FWwiseMetadataExternalSourceReference(FWwiseMetadataLoader& Loader);

	friend uint32 GetTypeHash(const FWwiseMetadataExternalSourceReference& Ref)
	{
		return ::GetTypeHash(Ref.Cookie);
	}
	bool operator==(const FWwiseMetadataExternalSourceReference& Rhs) const
	{
		return Cookie == Rhs.Cookie;
	}
	bool operator<(const FWwiseMetadataExternalSourceReference& Rhs) const
	{
		return Cookie < Rhs.Cookie;
	}
};

struct WWISEPROJECTDATABASE_API FWwiseMetadataExternalSource : public FWwiseMetadataExternalSourceReference
{
	FName Name;
	FName ObjectPath;
	FGuid GUID;

	FWwiseMetadataExternalSource(FWwiseMetadataLoader& Loader);
};
