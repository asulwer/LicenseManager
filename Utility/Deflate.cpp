#include "stdafx.h"
#include "WinAES.h"
#include "Utility.h"

namespace Utility
{
	using namespace System::IO::Compression; //Deflate

	array<Byte>^ License::Deflate::Compress(array<Byte>^ b)
	{
		array<Byte>^ buffer;

		MemoryStream^ ms = gcnew MemoryStream();
		try
		{
			DeflateStream^ ds = gcnew DeflateStream(ms, CompressionMode::Compress);
			try
			{
				ds->Write(b,0,b->Length);
			}
			finally
			{
				ds->Flush();
				ds->Close();
			}
			buffer = ms->ToArray();
		}
		finally
		{
			ms->Flush();
			ms->Close();
		}

		return buffer;
	}
		
	array<Byte>^ License::Deflate::Decompress(array<Byte>^ b)
	{
		array<Byte>^ buffer;

		DeflateStream^ ds = gcnew DeflateStream(gcnew MemoryStream(b), CompressionMode::Decompress);
		try
		{
			const int size = 4096;
			array<Byte>^ bt = gcnew array<Byte>(size);

			MemoryStream^ memory = gcnew MemoryStream();
			try
			{
				int count=0;
				do
				{
					count = ds->Read(bt,0,size);
					if(count>0)
						memory->Write(bt,0,count);
				} while (count>0);
				buffer = memory->ToArray();
			}
			finally
			{
				memory->Flush();
				memory->Close();
			}
		}
		finally
		{
			ds->Flush();
			ds->Close();
		}
		return buffer;
	}
}