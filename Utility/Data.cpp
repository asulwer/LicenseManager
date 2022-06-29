#include "stdafx.h"
#include "Data.h"

namespace Utility
{
	Data::Data()
	{
		Key = nullptr;
		Value = nullptr;
	}

	Data::Data(String^ k, String^ v)
	{
		Key = k;
		Value = v;
	}
}