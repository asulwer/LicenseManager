#pragma once

#include "MultiBase.h"
#include "Multi.h"

namespace Utility
{
	using namespace System;
	
	ref class Encryption
	{
	public:
		Encryption();
		~Encryption();
		!Encryption();

		cli::array<Byte>^ Encrypt(cli::array<Byte>^ orig);
		cli::array<Byte>^ Decrypt(cli::array<Byte>^ orig);
	private:
		MULTI_DATA* md = nullptr;
	};
}