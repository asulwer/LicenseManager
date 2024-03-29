#pragma once

#include "Data.h"
#include "Binder.h"
#include "Deflate.h"
#include "Encryption.h"

namespace Utility
{
	using namespace System;
	using namespace System::Text;
	using namespace System::Runtime::Serialization::Formatters::Binary; //BinaryFormatter
	
	[Serializable]
	ref class License : ILicense
	{
	public:
		License();
		~License();
		!License();

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
		virtual property array<IData^>^ D
		{
			array<IData^>^ get() override
			{
				return m_D;
			}
			void set(array<IData^>^ value) override
			{
				m_D = value;
			}
		}
		
	private:
		String^ m_Customer;
		String^ m_Product;
		String^ m_Version;
		array<IData^>^ m_D;
		
		[NonSerialized]
		Deflate^ deflate;
		[NonSerialized]
		Encryption^ encryption;		
	};
}
