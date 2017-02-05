/*
 * BVH.cpp
 *
 *  Created on: Nov 30, 2016
 *      Author: bogdan
 */

#include "BVH.h"
#include "RayTracingStatistics.h"

namespace traceraptor {

const float BVH::default_tmin = 0.001f;
const float BVH::default_tmax = MAXFLOAT;

//! Node for storing state information during traversal.
struct BVHTraversal {
  unsigned int i; // Node
  float min_t; // Minimum hit time for this node.
  BVHTraversal() { i = 0; min_t = 0; }
  BVHTraversal(int _i, float _mint) : i(_i), min_t(_mint) { }
};

//! - Compute the nearest intersection of all objects within the tree.
//! - Return true if hit was found, false otherwise.
//! - In the case where we want to find out of there is _ANY_ intersection at all,
//!   set occlusion == true, in which case we exit on the first hit, rather
//!   than find the closest.
bool BVH::getIntersection(const Ray& ray, IntersectionInfo &intersection, bool occlusion) const {
  intersection.t = 9999999.f;
  intersection.hit_something = false;
  float t_min0 = 0, t_min1 = 0;
  float t_max0 = 0, t_max1 = 0;
  int closer, other;

  // Working set
  BVHTraversal todo[64];
  int stackptr = 0;

  // "Push" on the root node to the working set
  todo[stackptr].i = 0;
  todo[stackptr].min_t = -9999999.f;

  while(stackptr >= 0) {
    // Pop off the next node to work on.
    int node_index = todo[stackptr].i;
    float near = todo[stackptr].min_t;
    stackptr--;
    const BVHFlatNode &node(flatTree[node_index]);

    // If this node is further than the closest found intersection, continue
    if(near > intersection.t)
      continue;

    // Is leaf -> Intersect
    if(node.rightOffset == 0) {
//    	Logger::log_debug("num childs " + std::to_string(node.num_primitives));
      for(unsigned int o=0; o<node.num_primitives ;++o) {
    	IntersectionInfo current;

        const std::shared_ptr<Hitable> obj = build_prims[node.start+o];
        bool hit = obj->hit(ray, default_tmin, default_tmax, current);

        if (hit) {
          // If we're only looking for occlusion, then any hit is good enough
          if(occlusion) {
            return true;
          }

          // Otherwise, keep the closest intersection only
          if (current.t < intersection.t) {
            intersection = current;
          }
        }
      }

    } else { // Not a leaf

      bool hitc0 = flatTree[node_index+1].bbox.intersect(ray, t_min0, t_max0);
      INCREMENT_RAY_BBOX_TEST_STATISTICS
      bool hitc1 = flatTree[node_index+node.rightOffset].bbox.intersect(ray, t_min1, t_max1);
      INCREMENT_RAY_BBOX_TEST_STATISTICS

      if(hitc0 && hitc1) {
        // We assume that the left child is a closer hit...
        closer = node_index+1;
        other = node_index+node.rightOffset;

        // ... If the right child was actually closer, swap the relevant values.
        bool right_child_closer = t_min1 < t_min0;
        if(right_child_closer) {
          std::swap(t_min0, t_min1);
          std::swap(closer,other);
        }

        // It's possible that the nearest object is still in the other side, but we'll
        // check the further-awar node later...

        // Push the farther first
        todo[++stackptr] = BVHTraversal(other, t_min1);

        // And now the closer (with overlap test)
        todo[++stackptr] = BVHTraversal(closer, t_min0);
      }

      else if (hitc0) {
        todo[++stackptr] = BVHTraversal(node_index+1, t_min0);
      }

      else if(hitc1) {
        todo[++stackptr] = BVHTraversal(node_index + node.rightOffset, t_min1);
      }

    }
  }

  return intersection.hit_something;
}

BVH::~BVH() {
}

BVH::BVH(std::vector<std::shared_ptr<Hitable>> &objects, unsigned int leafSize)
  : nNodes(0), nLeafs(0), leafSize(leafSize), build_prims(objects) {

	Logger::log_debug("Started building BVH");
    build();
    Logger::log_debug("Ended building BVH - Nodes:" + std::to_string(nNodes) + ", Leafs:" + std::to_string(nLeafs));
  }

struct BVHBuildEntry {
  // If non-zero then this is the index of the parent. (used in offsets)
	unsigned int parent;
  // The range of objects in the object list covered by this node.
	unsigned int start, end;
};

/*! Build the BVH, given an input data set
 *  - Handling our own stack is quite a bit faster than the recursive style.
 *  - Each build stack entry's parent field eventually stores the offset
 *    to the parent of that node. Before that is finally computed, it will
 *    equal exactly three other values. (These are the magic values Untouched,
 *    Untouched-1, and TouchedTwice).
 *  - The partition here was also slightly faster than std::partition.
 */
void BVH::build()
{
  BVHBuildEntry todo[128];
  unsigned int stackptr = 0;
  const unsigned int Untouched    = 0xffffffff;
  const unsigned int TouchedTwice = 0xfffffffd;

  // Push the root
  todo[stackptr].start = 0;
  todo[stackptr].end = build_prims.size();
  todo[stackptr].parent = 0xfffffffc;
  stackptr++;

  BVHFlatNode node;
  std::vector<BVHFlatNode> buildnodes;
  buildnodes.reserve(build_prims.size()*2);
  RayTracingStatistics::getInstance().totalNumberPrimitives.store(build_prims.size());

  while(stackptr > 0) {
    // Pop the next item off of the stack
    BVHBuildEntry &bnode(todo[--stackptr]);
    unsigned int start = bnode.start;
    unsigned int end = bnode.end;
    unsigned int nPrimitives = end - start;

    nNodes++;
    node.start = start;
    node.num_primitives = nPrimitives;
    node.rightOffset = Untouched;

    // Calculate the bounding box for this node
    BBox bb((build_prims[start])->get_bbox());
    BBox bc((build_prims[start])->get_centroid());
    for(unsigned int p = start; p < end; ++p) {
      std::shared_ptr<Hitable> build_prim = (build_prims[p]);
      bb.expandToInclude(build_prim->get_bbox());
      bc.expandToInclude(build_prim->get_centroid());
    }
    node.bbox = bb;

    // If the number of primitives at this point is less than the leaf
    // size, then this will become a leaf. (Signified by rightOffset == 0)
    if(nPrimitives <= leafSize) {
      node.rightOffset = 0;
      nLeafs++;
    }

    buildnodes.push_back(node);

    // Child touches parent...
    // Special case: Don't do this for the root.
    if(bnode.parent != 0xfffffffc) {
      buildnodes[bnode.parent].rightOffset --;

      // When this is the second touch, this is the right child.
      // The right child sets up the offset for the flat tree.
      if(buildnodes[bnode.parent].rightOffset == TouchedTwice) {
        buildnodes[bnode.parent].rightOffset = nNodes - 1 - bnode.parent;
      }
    }

    // If this is a leaf, no need to subdivide.
    if(node.rightOffset == 0)
      continue;

    // Set the split dimensions
    unsigned int split_dim = bc.maxDimension();

    // Split on the center of the longest axis
    float split_coord = .5f * (bc.min[split_dim] + bc.max[split_dim]);

    // Partition the list of objects on this split
    unsigned int mid = start;
    for(unsigned int i=start; i < end ;++i) {
      Vec3f centroid = (build_prims[i])->get_centroid();
      if(centroid[split_dim] < split_coord ) {
        std::swap( (build_prims[i]), (build_prims[mid]) );
        ++mid;
      }
    }

    // If we get a bad split, just choose the center...
    if(mid == start || mid == end) {
      mid = start + (end-start)/2;
    }

    // Push right child
    todo[stackptr].start = mid;
    todo[stackptr].end = end;
    todo[stackptr].parent = nNodes-1;
    stackptr++;

    // Push left child
    todo[stackptr].start = start;
    todo[stackptr].end = mid;
    todo[stackptr].parent = nNodes-1;
    stackptr++;
  }

  // Copy the temp node data to a flat array
  flatTree.reserve(nNodes);
  for(unsigned int n=0; n<nNodes; ++n)
    flatTree[n] = buildnodes[n];
}

} /* namespace traceraptor */
