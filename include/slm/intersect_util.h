#ifndef SLMATH_INTERSECT_UTIL_H
#define SLMATH_INTERSECT_UTIL_H

#include <slm/vec3.h>

SLMATH_BEGIN()

/**
 * \defgroup intersect_util Intersection testing helper functions.
 * @ingroup slm
 */
/*@{*/

/*
 * Line segment parameters for the second (low-level) intersect_line_box function.
 * Use of this class is not needed but is provided as alternative if more extensive optimization is needed.
 * @see intersect_line_box
 */
class intersect_line_box_line
{
public:
	/** Line segment start point. */
	const vec3	o;
	/** Vector from start point of the line segment to the end point. */
	const vec3	d;
	/** Component-wise one over delta (1/delta.x,1/delta.y,1/delta.z). */
	const vec3	inv_d;
	/** Signs (+-) of the line segment direction per component. */
	const int			signx;
	/** Signs (+-) of the line segment direction per component. */
	const int			signy;
	/** Signs (+-) of the line segment direction per component. */
	const int			signz;

	/** Sets ray origin and direction. Calculates intermediate values. */
	intersect_line_box_line( const vec3& origin, const vec3& direction );

private:
	intersect_line_box_line();
	intersect_line_box_line( const intersect_line_box_line& );
	intersect_line_box_line& operator=( const intersect_line_box_line& );
};

/**
 * Finds intersection between line segment and triangle.
 *
 * See: 'Fast, Minimum Storage Ray-Triangle Intersection' algorithm, 
 * Tomas Möller & Ben Trumbore, Volume 2, Number 1. Journal of Graphics Tools.
 *
 * @param o Starting point of the line segment.
 * @param d Vector from starting point of the line segment to the end point.
 * @param v0 Vertex 0 of the triangle.
 * @param v1 Vertex 1 of the triangle.
 * @param v2 Vertex 2 of the triangle.
 * @param t [out] Relative distance (0,1) to intersection.
 * @return true if intersect.
 */
bool	intersect_line_triangle( const vec3& o, const vec3& d, const vec3& v0, const vec3& v1, const vec3& v2, float* t );

/**
 * Finds if line segment and box intersect.
 *
 * See: Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 * "An Efficient and Robust Ray-Box Intersection Algorithm"
 * Journal of graphics tools, 10(1):49-54, 2005.
 *
 * @param o Starting point of the line segment.
 * @param d Vector from starting point of the line segment to the end point.
 * @param boxmin Minimum coordinates of the box.
 * @param boxmax Maximum coordinates of the box.
 * @return true if intersect.
 */
bool	intersect_line_box( const vec3& o, const vec3& d, const vec3& boxmin, const vec3& boxmax );

/*
 * Finds if line segment and box intersect.
 * Uses pre-calculated information in intersectRayBox_Ray so a bit faster
 * than direct call version of intersect_line_box.
 *
 * See: Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 * "An Efficient and Robust Ray-Box Intersection Algorithm"
 * Journal of graphics tools, 10(1):49-54, 2005.
 *
 * @param line Line segment information.
 * @param boxminmax Minimum and maximum coordinates (so array [2] of vec3) of the box.
 * @return true if intersect.
 */
//bool	intersect_line_box( const intersect_line_box_line* line, const vec3* boxminmax );

/*@}*/

SLMATH_END()

#endif 

// This file is part of 'slm' C++ library. Copyright (C) 2009-2018 Jani Kajala (kajala@gmail.com). Licensed under BSD/MIT license
