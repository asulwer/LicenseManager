#pragma once

namespace Utility
{
	using namespace System;
	using namespace System::IO::Compression; //Deflate
	using namespace System::IO; //MemoryStream

	ref class Deflate
	{
	public:
		Deflate() {};
		~Deflate() {};

		cli::array<Byte>^ Compress(cli::array<Byte>^ b);
		cli::array<Byte>^ Decompress(cli::array<Byte>^ b);
	};
}