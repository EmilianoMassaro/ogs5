/*
 * PolyLine.h
 *
 *  Created on: Jan 14, 2010
 *      Author: TF
 * \copyright
 * Copyright (c) 2018, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 */

#ifndef POLYLINE_H_
#define POLYLINE_H_

// GEOLIB
#include "GeoObject.h"
#include "Point.h"

// MathLib
#include "MathTools.h"

#include <cmath>
#include <vector>

namespace GEOLIB
{
class Location
{
public:
	enum type
	{
		LEFT,
		RIGHT,
		BEYOND,
		BEHIND,
		BETWEEN,
		SOURCE,
		DESTINATION
	};
};

/**
 * \ingroup GEOLIB
 *
 * \brief Class Polyline consists mainly of a reference to a point vector and
 * a vector that stores the indices in the point vector.
 * A polyline consists of at least one line segment. The polyline is specified by the points
 * of the line segments. The class Polyline stores ids of pointers to the points in the
 * _ply_pnt_ids vector.
 * */
class Polyline : public GeoObject
{
public:
	/** constructor
	 * \param pnt_vec a reference to the point vector
	 */
	Polyline(const std::vector<Point*>& pnt_vec);
	/**
	 * Copy constructor
	 * @param ply Polyline
	 */
	Polyline(const Polyline& ply);

	virtual ~Polyline() {}
	/** write the points to the stream */
	void write(std::ostream& os) const;

	/**
	 * Adds an id of a point at the end of the polyline. The id have to be inside
	 * the (internal) _ply_pnts vector the polyline is based on.
	 * @param pnt_id
	 */
	virtual void addPoint(size_t pnt_id);

	/**
	 * Method inserts a new point (that have to be inside the _ply_pnts vector)
	 * at the given position in the polyline.
	 * @param pos the position in the polyline, pos have to be a value into the interval [0, number of points)
	 * @param pnt_id the id of the new point in the vector of points the polyline is based on
	 */
	virtual void insertPoint(size_t pos, size_t pnt_id);

	/**
	 * Closes a polyline by adding a line segment that connects start- and end-point.
	 * \param ply A Polyline containing at least three points.
	 * \return A polygon.
	 */
	static Polyline* closePolyline(const Polyline& ply);

	/// Constructs one polyline from a vector of connected polylines.
	/// All polylines in this vector need to reference the same point vector.
	static Polyline* constructPolylineFromSegments(const std::vector<Polyline*>& ply_vec, double prox = 0.0);

	/**
	 * returns the number of points,
	 * the number of segments is about one smaller
	 * */
	size_t getNumberOfPoints() const;

	/** returns true if the polyline is closed */
	bool isClosed() const;

	/**
	 * Method tests if the given id of a point (within the vector of points the polyline is
	 * based on) is inside the polyline.
	 * @param pnt_id the id of the point
	 * @return true if the point is part of the polyline, else false
	 */
	bool isPointIDInPolyline(size_t pnt_id) const;

	/**
	 * returns the index of the i-th polyline point
	 * in the point vector
	 */
	size_t getPointID(size_t i) const;

	/**
	 * Changes a point index for one point in a line
	 * @param idx Index of point in line
	 * @param id ID of point in PointVec object
	 */
	void setPointID(size_t idx, size_t id);

	/** \brief const access operator for the access to the i-th point of the polyline.
	 */
	const Point* operator[](size_t i) const;

	/**
	 * \brief returns the i-th point contained in the polyline
	 * */
	const Point* getPoint(size_t i) const;

	std::vector<Point*> const& getPointsVec() const;

	/**
	 * returns the length of the polyline until the k-th line segment
	 * @param k the k-th line segment
	 * @return the length of the polyline until the k-th line segment
	 */
	double getLength(size_t k) const;

	/**
	 * get the complete length vector
	 * @return the length vector of the polyline
	 */
	const std::vector<double>& getLengthVec() const;

	friend bool operator==(Polyline const& lhs, Polyline const& rhs);

protected:
	/**
	 * 2D method - ignores z coordinate. It calculates the location
	 * of the point relative to the k-th line segment of the polyline.
	 * (literatur reference:
	 * Computational Geometry and Computer Graphics in C++; Michael J. Laszlo)
	 * @param k the number of line segment
	 * @param pnt the point
	 * @return a value of enum LOCATION
	 */
	Location::type getLocationOfPoint(size_t k, GEOLIB::Point const& pnt) const;

	static bool pointsAreIdentical(const std::vector<Point*>& pnt_vec, size_t i, size_t j, double prox);

	/** a reference to the vector of pointers to the geometric points */
	const std::vector<Point*>& _ply_pnts;
	/** position of pointers to the geometric points */
	std::vector<size_t> _ply_pnt_ids;
	/**
	 * the k-th element of the vector contains the length of the polyline until the k-th segment
	 */
	std::vector<double> _length;
};

/** overload the output operator for class Polyline */
std::ostream& operator<<(std::ostream& os, Polyline const& pl);

bool containsEdge(const Polyline& ply, size_t id0, size_t id1);

bool isLineSegmentIntersecting(const Polyline& ply, GEOLIB::Point const& s0, GEOLIB::Point const& s1);

/**
 * comparison operator
 * @param lhs first polyline
 * @param rhs second polyline
 * @return true, if the polylines consists of the same sequence of line segments
 */
bool operator==(Polyline const& lhs, Polyline const& rhs);

} // end namespace

#endif /* POLYLINE_H_ */
