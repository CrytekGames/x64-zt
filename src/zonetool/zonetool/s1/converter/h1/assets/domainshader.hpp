#pragma once

namespace zonetool::s1
{
	namespace converter::h1
	{
		namespace domainshader
		{
			zonetool::h1::MaterialDomainShader* convert(MaterialDomainShader* asset, zone_memory* mem);
			void dump(MaterialDomainShader* asset, zone_memory* mem);
		}
	}
}