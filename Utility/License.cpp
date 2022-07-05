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
		License^ temp = gcnew License();
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

				temp = (License^)bf->Deserialize(ms);
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

		this->Customer = temp->Customer;
		this->Product = temp->Product;
		this->Version = temp->Version;
		this->D = temp->D;
	}
}