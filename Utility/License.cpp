#include "stdafx.h"
#include "License.h"

namespace Utility
{
	License::License()
	{
		this->Customer = String::Empty;
		this->Product = String::Empty;
		this->Version = String::Empty;
		this->D = gcnew array<Data^>(0);		
		
		this->encryption = gcnew Encryption();
		this->deflate = gcnew Deflate();
	}
	License::~License()
	{
		this->!License();
	}
	License::!License()
	{
		//delete aes; //not necessary to delete as this is a managed resource
		//delete deflate;
	}

	String^ License::Save()
	{
		cli::array<Byte>^ buffer = nullptr;

		MemoryStream^ ms = gcnew MemoryStream();
		try
		{
			BinaryFormatter^ bf = gcnew BinaryFormatter();
			bf->Binder = gcnew Binder;
			bf->Serialize(ms, this);
		}
		finally
		{
			ms->Flush();
			ms->Close();
		}
		
		buffer = ms->ToArray(); //redundant
		buffer = encryption->Encrypt(buffer); //encrypt
		buffer = deflate->Compress(buffer); //compress
		return Convert::ToBase64String(buffer); //convert to base64
	}
	void License::Open(String^ lic)
	{
		cli::array<Byte>^ buffer = nullptr;

		try
		{
			buffer = Convert::FromBase64String(lic); //convert from base64
			buffer = deflate->Decompress(buffer); //decompress
			buffer = encryption->Decrypt(buffer); //decrypt
		}
		catch(...)
		{
			throw;
		}
		
		//deserialize the decrypted/decompressed object
		if(buffer != nullptr && buffer->Length > 0)
		{
			MemoryStream^ ms = gcnew MemoryStream(buffer);
			try
			{
				BinaryFormatter^ bf = gcnew BinaryFormatter();
				bf->Binder = gcnew Binder;
				
				//a new temporary License to hold the Deserialized license isnt necessary
				this->Customer = ((License^)bf->Deserialize(ms))->Customer;
				ms->Position = 0; //reset
				this->Product = ((License^)bf->Deserialize(ms))->Product;
				ms->Position = 0; //reset
				this->Version = ((License^)bf->Deserialize(ms))->Version;
				ms->Position = 0; //reset
				this->D = ((License^)bf->Deserialize(ms))->D;
			}
			catch (...)
			{
				throw;
			}
			finally
			{
				ms->Flush();
				ms->Close();
			}
		}
	}
}