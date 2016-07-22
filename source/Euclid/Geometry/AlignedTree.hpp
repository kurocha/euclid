//
//  Euclid/Geomtry/AlignedTree.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 2/01/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_ALIGNED_TREE_H
#define _EUCLID_GEOMETRY_ALIGNED_TREE_H

#include <vector>
#include <set>

#include "../Numerics/Vector.hpp"
#include "AlignedBox.hpp"

namespace Euclid {
	namespace Geometry {
		// An aligned tree is a generic name for either a quad-tree or an oct-tree
		//	AlignedTree<QuadrantTraits> or AlignedTree<OctantTraits> respectively

		// An aligned tree subdivides a 3 dimensional space into aligned partitions.
		// Each partition can be thus subdivided recursively, and
		// so on. Collision tests only need to be done with objects in all
		// overlapping partitions.

		// Template specialization for 2-dimentional quad-tree
		class Quadrants {
		public:
			enum { Q = 4, D = 2, R = 16 };

			typedef Vector<D> VecT;
			typedef AlignedBox<D> SpaceT;

			template <typename ObjectT>
			static SpaceT calculate_bounding_box (const ObjectT & obj) {
				return obj.bounding_box();
			}

			static unsigned index_for_partition(const Vec2 &point, const Vec2 &center);
			static Vec2 normal_origin_for_partition_index(const std::size_t &i);
			static Vec2 offset_origin_for_partition_index(const std::size_t &i);

			enum PartitionLocation {
				BottomLeft = 0,
				BottomRight = 1,
				TopLeft = 2,
				TopRight = 3
			};

			static PartitionLocation location_for_direction (const Direction &dir);
		};

		// Template specialization for 3-dimentional oct-tree
		class Octants {
		public:
			enum { Q = 8, D = 3, R = 16 };

			typedef Vector<D> VecT;
			typedef AlignedBox<D> SpaceT;

			template <typename ObjectT>
			static SpaceT calculate_bounding_box (const ObjectT & obj) {
				return obj.bounding_box();
			}

			static unsigned index_for_partition(const Vec3 &point, const Vec3 &center);
			static Vec3 normal_origin_for_partition_index(const std::size_t &i);
			static Vec3 offset_origin_for_partition_index(const std::size_t &i);

			enum PartitionLocation {
				BottomLeftNear = 0,
				BottomRightNear = 1,
				TopLeftNear = 2,
				TopRightNear = 3,
				BottomLeftFar = 4,
				BottomRightFar = 5,
				TopLeftFar = 6,
				TopRightFar = 7
			};

			static PartitionLocation location_for_direction (const Direction &dir);
		};

		// An aligned space partitioning tree.
		template <typename _TraitsT, typename ObjectT>
		class AlignedTree {
		public:
			typedef _TraitsT TraitsT;
			typedef typename TraitsT::VecT VecT;
			typedef typename TraitsT::SpaceT SpaceT;
			typedef std::set<ObjectT> ObjectSetT;

			class Partition
			{
			protected:
				ObjectSetT _objects;

				AlignedTree* _base;
				typename TraitsT::PartitionLocation _location;

				Partition* _parent;
				Partition* _children[TraitsT::Q];

				VecT _origin;
				VecT _size;
				unsigned _level;

				void compute_position () {
					_origin = _parent->origin() + (TraitsT::normal_origin_for_partition_index(_location) * _parent->size());
					_size = _parent->size() / 2;
					_level = _parent->level() + 1;
				}

			public:
				Partition (AlignedTree * base, const VecT & origin, const VecT & size) : _origin(origin), _size(size), _level(0)
				{
					_parent = NULL;
					_location = (typename TraitsT::PartitionLocation) 0;
					_base = base;

					std::fill_n(_children, (std::size_t)TraitsT::Q, nullptr);
				}

				Partition (Partition *parent, typename TraitsT::PartitionLocation location) : _location(location), _parent(parent)
				{
					assert(_parent->_children[_location] == NULL);

					_parent->_children[_location] = this;
					_base = parent->_base;

					std::fill_n(_children, (std::size_t)TraitsT::Q, nullptr);

					compute_position();
				}

				~Partition () {
					for (unsigned i = 0; i < TraitsT::Q; i += 1)
						if (_children[i])
							delete _children[i];
				}

				VecT origin () const {
					return _origin;
				}

				VecT size () const {
					return _size;
				}

				SpaceT bounding_box() const {
					return SpaceT(_origin, _origin + _size);
				}

				unsigned level () const {
					return _level;
				}

			public:
				// Returns a given child partition.
				Partition* child (unsigned i) { return _children[i]; }
				const Partition* child (unsigned i) const { return _children[i]; }

				// Returns the parent partition.
				Partition* parent () { return _parent; }
				const Partition* parent () const { return _parent; }

				// Returns the objects in this partition.
				ObjectSetT& objects() { return _objects; }
				const ObjectSetT& objects() const { return _objects; }

				/*
				void debug (std::string indent) {
					std::cout << indent << "Partition " << _location << " has " << objects().size() << " objects." << std::endl;

					for (unsigned i = 0; i < TraitsT::Q; i++) {
						if (_children[i] != NULL) {
							_children[i]->debug(indent + "\t");
						}
					}
				}
				*/

				// Visitor pattern.
				template <typename VisitorT>
				void visit (VisitorT & visitor) {
					for (unsigned i = 0; i < TraitsT::Q; i++) {
						if (_children[i] != NULL) {
							_children[i]->visit(visitor);
						}
					}

					visitor.accept(this);
				}

				// Core function to redistribute objects into children subdivisions if possible.
				// Can potentially take a long time to execute. O(QN)
				void redistribute () {
					for (unsigned i = 0; i < TraitsT::Q; i += 1) {
						if (_children[i] == NULL) new Partition(this, (typename TraitsT::PartitionLocation)i);
					}

					ObjectSetT resort;
					std::swap(_objects, resort);

					for (auto object : resort) {
						this->insert(object);
					}
				}

				// Join this partition with its parent.
				void coalesce () {
					//_parent->_children[_location] = NULL;
					//delete this;
				}

				// Insert an object in this partition or a child.
				Partition * insert (ObjectT object)
				{
					Partition * cur = find(object);

					cur->objects().insert(object);

					return cur;
				}

				// Erase an object in this partition or a child.
				Partition * erase (ObjectT object)
				{
					Partition * cur = find(object);

					cur->objects().erase(object);

					return cur;
				}

				// Find an object in this partition or a child.
				Partition * find (ObjectT object) {
					SpaceT b = TraitsT::calculate_bounding_box(object);
					Partition *cur = this;

					unsigned i = 0;
					while (i < TraitsT::Q) {
						Partition *c = cur->child(i);

						if (c && c->bounding_box().contains_box(b)) {
							// Descend into the current partition
							cur = c;
							i = 0;
						} else {
							// Try the next partition
							i += 1;
						}
					}

					return cur;
				}

				// Return the smallest partition for the given point.
				Partition* partition_for_point (const Vec2 &point) {
					if (bounding_box().contains_point(point, true)) {
						// A child potentially contains the point
						Partition *t = NULL;
						for (unsigned i = 0; i < TraitsT::Q; i += 1) {
							if (child(i)) {
								t = child(i).partition_for_point(point);

								if (t) return t;
							}
						}
					}

					// Otherwise, we must contain the point
					return this;
				}

				// Return the smallest partition for the given rect.
				Partition* partition_for_rect (const SpaceT &rect) {
					if (bounding_box().contains_box(rect, true)) {
						// A child potentially contains the point
						Partition *t = NULL;
						for (unsigned i = 0; i < TraitsT::Q; i += 1) {
							if (child(i)) {
								t = child(i)->partition_for_rect(rect);

								if (t) return t;
							}
						}
					}

					return this;
				}

				// Return all objets in the given partition including children.
				ObjectSetT all_objects () {
					ObjectSetT objects;
					Partition * cur = this;

					while (cur != NULL) {
						objects.insert(cur->_objects.begin(), cur->_objects.end());

						cur = cur->_parent;
					}

					return objects;
				};

				// Return the set of objects in a given rectangle.
				ObjectSetT objects_in_rect (const SpaceT & _rect) {
					SpaceT rect(_rect);
					rect.clip_to_box(this->bounding_box());

					ObjectSetT selection;

					for(auto object : _objects) {
						SpaceT b = TraitsT::calculate_bounding_box(object);

						if (b.intersects_with(rect)) {
							selection.insert(object);
						}
					}

					for (unsigned i = 0; i < TraitsT::Q; i += 1) {
						if (child(i) == NULL) continue;

						if (child(i)->bounding_box().intersects_with(rect)) {
							ObjectSetT child_selection = child(i)->objects_in_rect(rect);

							selection.insert(child_selection.begin(), child_selection.end());
						}
					}

					return selection;
				}

				// This function probably needs to be fixed
				ObjectSetT objects_along_line (const LineSegment<TraitsT::D> &l) {
					ObjectSetT selection;

					for (unsigned i = 0; i < TraitsT::Q; i += 1) {
						if (child(i) == NULL) continue;

						if (child(i)->bounding_box().intersects_with(l, true)) {
							// Add all current-level objects
							selection.insert(child(i)->objects().begin(), child(i)->objects().end());

							// Add any more children objects
							ObjectSetT objs = child(i)->objects_along_line(l);
							selection.insert(objs.begin(), objs.end());
						}
					}

					return selection;
				}
			};

		protected:
			SpaceT _bounds;
			bool _expanding;
			Partition _top;

			void expand (const unsigned & dir) {
				if (dir && LEFT) {
					if (dir && TOP) {
					} else {
					}
				}
			}

		public:
			AlignedTree (const VecT & origin, const VecT & size) : _bounds(origin, origin + size), _expanding(true), _top(this, origin, size) {
			}

			// The top partition in the tree.
			Partition * top () {
				return &_top;
			}

			const Partition * top () const {
				return &_top;
			}

			bool expanding () const { return _expanding; }
			void set_expanding (bool expanding) { _expanding = expanding; }

			// Copy objects from another STL container.
			template <typename IteratorT>
			void insert (IteratorT begin, IteratorT end) {
				for (; begin != end; ++begin) {
					insert(*begin);
				}
			}

			// Insert an object if it exists, redistribute the tree if appropriate.
			Partition * insert (ObjectT o, bool redistribute = true) {
				SpaceT b = TraitsT::calculate_bounding_box(o);

				if (!_top.bounding_box().contains_box(b))
					return NULL;

				Partition * p = _top.insert(o);

				// TraitsT::R is the threshold at which we redistribute objects:
				if (redistribute && p->objects().size() > TraitsT::R) {
					p->redistribute();
				}

				return p;
			}

			// Find an object if it exists.
			Partition * find (ObjectT o) {
				SpaceT b = TraitsT::calculate_bounding_box(o);

				if (!_top.bounding_box().intersects_with(b))
					return NULL;

				return _top.find(o);
			}

			// Erase an object if it exists.
			Partition * erase (ObjectT o, bool coalesce = true) {
				Partition * p = find(o);

				if (p) {
					p->objects().erase(o);
				}

				if (p && p->objects().size() == 0) {
					p->coalesce();
				}

				return p;
			}

			// Find the smallest partition which encloses the given point.
			Partition* partition_for_point (const Vec2 &point) {
				return _top.partition_for_point(point);
			}

			// Find the smallest partition which encloses the given rectangle.
			Partition* partition_for_rect (const SpaceT &rect) {
				return _top.partition_for_rect(rect);
			}
		};
	}
}

#endif
