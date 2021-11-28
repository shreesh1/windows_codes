UINT32 resolve_hash_name(LPCSTR cszName)
{
	if (cszName == NULL)
	{
		return 0;
	}

	SIZE_T uNameLen = strnlen_s(cszName, RESOLVE_NAME_MAX);
	if (uNameLen == 0)
	{
		return 0;
	}

	UINT32 u32Hash = 0, u32Buf = 0;
	PBYTE pbData = (PBYTE)cszName;
	INT iRemain = (uNameLen & 3);

	uNameLen >>= 2;

	for (SIZE_T i = uNameLen; i > 0; i--)
	{
		u32Hash += *(const UINT16*)pbData;
		u32Buf = (*(const UINT16*)(pbData + 2) << 11) ^ u32Hash;
		u32Hash = (u32Hash << 16) ^ u32Buf;
		pbData += (2 * sizeof(UINT16));
		u32Hash += u32Hash >> 11;
	}

	switch (iRemain)
	{
	case 1:
		u32Hash += *pbData;
		u32Hash ^= u32Hash << 10;
		u32Hash += u32Hash >> 1;
		break;

	case 2:
		u32Hash += *(const UINT16*)pbData;
		u32Hash ^= u32Hash << 11;
		u32Hash += u32Hash >> 17;
		break;

	case 3:
		u32Hash += *(const UINT16*)pbData;
		u32Hash ^= u32Hash << 16;
		u32Hash ^= pbData[sizeof(UINT16)] << 18;
		u32Hash += u32Hash >> 11;
		break;
	}

	u32Hash ^= u32Hash << 3;
	u32Hash += u32Hash >> 5;
	u32Hash ^= u32Hash << 4;
	u32Hash += u32Hash >> 17;
	u32Hash ^= u32Hash << 25;
	u32Hash += u32Hash >> 6;

	return u32Hash;
	}
