#pragma once

using namespace System;
using namespace System::IO; //MemoryStream
using namespace System::Runtime::Serialization; //ISerialization
using namespace System::Runtime::Serialization::Formatters::Binary; //BinaryFormatter
using namespace System::Collections::Generic; //Dictionary

namespace Utility
{
	/*[Serializable]
	ref class Data : IData
	{
	public:
		Data();
		Data(String^ k, String^ v);
			
		virtual property String^ Key
		{
			String^ get() override
			{
				return m_Key;
			}
			void set(String^ value) override
			{
				m_Key = value;
			}
		}
		virtual property String^ Value
		{
			String^ get() override
			{
				return m_Value;
			}
			void set(String^ value) override
			{
				m_Value = value;
			}
		}

	private:
		String^ m_Key;
		String^ m_Value;
	};*/

	[Serializable]
	ref class License : ISerializable, ILicense
	{
	public:
		License();
		//License(String^ customer, String^ product, String^ version, array<IData^>^ data);
	
		virtual String^ Save() override;
		virtual void Open(String^ lic) override;

		virtual property String^ Customer
		{
			String^ get() override
			{
				return m_Customer;
			}
			void set(String^ value) override
			{
				m_Customer = value;
			}
		}
		virtual property String^ Product
		{
			String^ get() override
			{
				return m_Product;
			}
			void set(String^ value) override
			{
				m_Product = value;
			}
		}
		virtual property String^ Version
		{
			String^ get() override
			{
				return m_Version;
			}
			void set(String^ value) override
			{
				m_Version = value;
			}
		}
		/*virtual property array<IData^>^ D
		{
			array<IData^>^ get() override
			{
				return m_D;
			}
			void set(array<IData^>^ value) override
			{
				m_D = value;
			}
		}*/
		virtual property Dictionary<String^,String^>^ Dict
		{
			Dictionary<String^,String^>^ get() override
			{
				return m_Dict;
			}
			void set(Dictionary<String^,String^>^ value) override
			{
				m_Dict = value;
			}
		}

	protected:
		License(SerializationInfo^ info, StreamingContext context);
		virtual void GetObjectData(SerializationInfo^ info, StreamingContext ctxt) = ISerializable::GetObjectData;

	private:
		String^ m_Customer;
		String^ m_Product;
		String^ m_Version;
		//array<IData^>^ m_D;
		Dictionary<String^,String^>^ m_Dict;

		ref class Binder sealed : SerializationBinder
		{
		public:
			virtual Type^ BindToType(String^ assemblyName, String^ typeName) override;
		};

		ref class Deflate
		{
		public:
			Deflate() {};
			~Deflate() {};

			cli::array<Byte>^ Compress(cli::array<Byte>^ b);
			cli::array<Byte>^ Decompress(cli::array<Byte>^ b);
		};

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
			bool GenerateRandom(cli::array<Byte> ^buffer, int size);
			bool GenerateDefaults();
			bool SetKeyWithIv(cli::array<Byte> ^key, int ksize, cli::array<Byte> ^iv);
			bool SetKeyWithIv(cli::array<Byte> ^key, int ksize, cli::array<Byte> ^iv, int vsize);
			bool MaxCipherTextSize(UInt32 psize, UInt32% csize);
			bool MaxPlainTextSize(UInt32 csize, UInt32% psize);		
			
			WinAES* aes;
			Deflate^ deflate;
		};
	};
}
