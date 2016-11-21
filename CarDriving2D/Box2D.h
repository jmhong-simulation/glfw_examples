#pragma once

template<class TT>
class Box2D
{
public:
	union
	{
		struct{TT x_min_, y_min_,x_max_,y_max_;};
		struct{TT i_start_,j_start_,i_end_,j_end_;};
	};

public:
	Box2D(void)
	{}

	Box2D(const Box2D<TT>& box_input)
		: x_min_(box_input.x_min_), y_min_(box_input.y_min_), x_max_(box_input.x_max_), y_max_(box_input.y_max_)
	{}

	Box2D(const TT& x_min_input, const TT& y_min_input, const TT& x_max_input, const TT& y_max_input)
		: x_min_(x_min_input), y_min_(y_min_input), x_max_(x_max_input), y_max_(y_max_input)
	{}

	~Box2D(void)
	{}

	bool isInside(const TT& x, const TT& y);

};
