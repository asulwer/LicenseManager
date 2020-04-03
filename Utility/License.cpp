#include "stdafx.h"
#include "WinAES.h"
#include "Utility.h"

namespace Utility
{
	License::License()
	{
		this->Customer = String::Empty;
		this->Product = String::Empty;
		this->Version = String::Empty;
		//this->D = gcnew array<IData^>(0);
		this->Dict = gcnew Dictionary<String^,String^>();
		this->aes = gcnew AES();
		this->deflate = gcnew Deflate();
	}
	License::~License()
	{
		this->!License();
	}
	License::!License()
	{
		delete aes;
		delete deflate;
	}

	License::License(SerializationInfo^ info, StreamingContext ctxt)
	{
		//called when we Deserialize the data in Open
		this->Customer = dynamic_cast<String^>(info->GetValue("Customer", String::typeid));
		this->Product = dynamic_cast<String^>(info->GetValue("Product", String::typeid));
		this->Version = dynamic_cast<String^>(info->GetValue("Version", String::typeid));
		//this->D = dynamic_cast<array<IData^>^>(info->GetValue("D",array<IData^>::typeid));
		this->Dict = dynamic_cast<Dictionary<String^,String^>^>(info->GetValue("Dict",Dictionary<String^,String^>::typeid));
	}	
	void License::GetObjectData(SerializationInfo^ info, StreamingContext ctxt)
	{
		//called when we Serialize the data in Save
		info->AddValue("Customer", this->Customer);
		info->AddValue("Product", this->Product);
		info->AddValue("Version", this->Version);
		//info->AddValue("D", this->D);
		info->AddValue("Dict", this->Dict);
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
		
		buffer = deflate->Compress(ms->ToArray());
		return aes->Encrypt(buffer);
	}
	void License::Open(String^ lic)
	{
		License^ temp = gcnew License();
		cli::array<Byte>^ buffer = nullptr;

		try
		{
			buffer = aes->Decrypt(lic);
			buffer = deflate->Decompress(buffer);
		}
		catch(...)
		{
			throw;
		}
		
		//deserialize the decrypted/decompressed object
		if(buffer != nullptr)
		{
			MemoryStream^ ms = gcnew MemoryStream(buffer);
			try
			{
				BinaryFormatter^ bf = gcnew BinaryFormatter();
				bf->Binder = gcnew Binder;

				temp = (License^)bf->Deserialize(ms);
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
		//this->D = temp->D;
		this->Dict = temp->Dict;
	}
}