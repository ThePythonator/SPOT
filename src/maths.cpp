#include <spot/maths.hpp>

namespace spot {
	bool colliding(spot::frect rect_a, spot::frect rect_b) {
		return (
			rect_a.pos.x < rect_b.pos.x + rect_b.size.x &&
			rect_b.pos.x < rect_a.pos.x + rect_a.size.x &&
			rect_a.pos.y < rect_b.pos.y + rect_b.size.y &&
			rect_b.pos.y < rect_a.pos.y + rect_a.size.y
		);
	}
}
