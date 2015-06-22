#include "stdafx.h"
#include "WinAES.h"
#include "Utility.h"

namespace Utility
{
	License::License()
	{
		Customer = "";
		Product = "";
		Version = "";
		//D = gcnew array<IData^>(0);
		Dict = gcnew Dictionary<String^,String^>();
	}

	/*License::License(String^ customer, String^ product, String^ version, array<IData^>^ data)
	{
		Customer = customer;
		Product = product;
		Version = version;
		D = data;
	}*/

	License::License(SerializationInfo^ info, StreamingContext context)
	{
		Customer = dynamic_cast<String^>(info->GetValue("Customer", String::typeid));
		Product = dynamic_cast<String^>(info->GetValue("Product", String::typeid));
		Version = dynamic_cast<String^>(info->GetValue("Version", String::typeid));
		//D = dynamic_cast<array<IData^>^>(info->GetValue("D",array<IData^>::typeid));
		Dict = dynamic_cast<Dictionary<String^,String^>^>(info->GetValue("Dict",Dictionary<String^,String^>::typeid));
	}

	void License::GetObjectData(SerializationInfo^ info, StreamingContext ctxt)
	{
		info->AddValue("Customer", Customer);
		info->AddValue("Product", Product);
		info->AddValue("Version", Version);
		//info->AddValue("D", D);
		info->AddValue("Dict",Dict);
	}

	String^ License::Save()
	{
		AES^ crypto = gcnew AES();
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
		return crypto->Encrypt(ms->ToArray());
	}

	void License::Open(String^ lic)
	{
		License^ temp = gcnew License();
		AES^ crypto = gcnew AES();
		array<Byte>^ buffer = nullptr;

		try
		{
			buffer = crypto->Decrypt(lic);
		}
		catch(...)
		{
			throw;
		}
			
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