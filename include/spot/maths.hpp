#pragma once

#include <linalg.h>

namespace spot {
	using namespace linalg::aliases;

	struct frect {
		float2 pos;
		float2 size;

		float2 center() {
			return { pos.x + size.x / 2, pos.y + size.y / 2 };
		}
	};
}
