#pragma once

namespace zonetool::s1
{
	namespace converter::h1
	{
		namespace mapents
		{
			zonetool::h1::MapEnts* convert(MapEnts* asset, zone_memory* mem);
			void dump(MapEnts* asset, zone_memory* mem);
		}
	}
}