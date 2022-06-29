#include "stdafx.h"
#include "Binder.h"

namespace Utility
{
	using namespace System::Reflection; //Assembly

	Type^ Binder::BindToType(String^ assemblyName, String^ typeName)
	{
		Type^ tyType = nullptr;
				
		String^ sShortAssemblyName = assemblyName->Split(',')[0];
		sShortAssemblyName = sShortAssemblyName->Remove(sShortAssemblyName->Length-3);

		cli::array<Assembly^>^ ayAssemblies = AppDomain::CurrentDomain->GetAssemblies();

		for each(Assembly^ ayAssembly in ayAssemblies)
		{
			if(ayAssembly->FullName->Split(',')[0]->Contains(sShortAssemblyName))
			{
				tyType = ayAssembly->GetType(typeName);
				break;
			}
		}

		return tyType;
	}
}