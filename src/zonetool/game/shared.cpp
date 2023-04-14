#include <std_include.hpp>

#include "loader/component_loader.hpp"

#include "shared.hpp"

namespace zonetool
{
	namespace
	{
		std::unordered_map<std::uint32_t, void*> x_gfx_globals_map;

		namespace iw6
		{
			void initialize()
			{
				SL_ConvertToString.set(0x1404317F0);

				stream_files.set(0x141E5BF40);
				stream_file_index.set(0x141E5BF14);
			}
		}

		namespace s1
		{
			void initialize()
			{
				SL_ConvertToString.set(0x1403F0F10);

				stream_files.set(0x141E57A30);
				stream_file_index.set(0x141E57A04);
			}
		}

		namespace h1
		{
			void initialize()
			{
				SL_ConvertToString.set(0x14043B170);

				stream_files.set(0x1432748B0);
				stream_file_index.set(0x143274884);
			}
		}

		namespace h2
		{
			void initialize()
			{
				SL_ConvertToString.set(0x1405BFBB0);

				stream_files.set(0x1420B2820);
				stream_file_index.set(0x1420B27F4);
			}
		}
	}

	void* get_x_gfx_globals_for_zone(int zone)
	{
		return x_gfx_globals_map[zone];
	}

	void insert_x_gfx_globals_for_zone(int zone, void* globals)
	{
		x_gfx_globals_map.insert(std::make_pair(zone, globals));
	}

	class shared final : public component_interface
	{ 
	public:
		void post_start() override
		{
			select(iw6::initialize, s1::initialize, h1::initialize, h2::initialize)();
		}
	};

	REGISTER_COMPONENT(shared);
}