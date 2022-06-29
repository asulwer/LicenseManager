#pragma once

namespace Utility
{
	using namespace System;
	using namespace System::Runtime::Serialization; //ISerialization
	
	ref class Binder sealed : SerializationBinder
	{
	public:
		virtual Type^ BindToType(String^ assemblyName, String^ typeName) override;
	};
}