#pragma once

namespace Utility
{
	using namespace System;

	[Serializable]
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
	};
}