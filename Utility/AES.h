#pragma once

#include "WinAES.h"

namespace Utility
{
	using namespace System;
	
	ref class AES
	{
	public:
		AES();
		~AES();
		!AES();

		String^ Encrypt(cli::array<Byte>^ b);
		cli::array<Byte>^ Decrypt(String^ orig);
	private:
		enum class Parameters { KEYSIZE_128 = 16, KEYSIZE_192 = 24, KEYSIZE_256 = 32, BLOCKSIZE = 16 };
		enum class Flags { CREATE_CONTAINER = 1, DELETE_CONTAINER = 8, THROW_EXCEPTION = 16, DEFAULT_FLAGS = CREATE_CONTAINER | DELETE_CONTAINER };
		bool GenerateRandom(cli::array<Byte>^ buffer, int size);
		bool GenerateDefaults();
		bool SetKeyWithIv(cli::array<Byte>^ key, int ksize, cli::array<Byte>^ iv);
		bool SetKeyWithIv(cli::array<Byte>^ key, int ksize, cli::array<Byte>^ iv, int vsize);
		bool MaxCipherTextSize(UInt32 psize, UInt32% csize);
		bool MaxPlainTextSize(UInt32 csize, UInt32% psize);

		WinAES* aes;
	};
}