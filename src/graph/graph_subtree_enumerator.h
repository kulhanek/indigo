/****************************************************************************
 * Copyright (C) 2009-2015 EPAM Systems
 * 
 * This file is part of Indigo toolkit.
 * 
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 3 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ***************************************************************************/

#ifndef __graph_subtree_enumerator__
#define __graph_subtree_enumerator__

#include "base_cpp/tlscont.h"
#include "graph/graph.h"
#include "base_cpp/list.h"
#include "base_cpp/obj_array.h"

namespace indigo {

class Filter;

class GraphSubtreeEnumerator
{
public:
   explicit GraphSubtreeEnumerator (Graph &graph);
   ~GraphSubtreeEnumerator ();

   Filter *vfilter;

   void (*callback)(Graph &graph, const Array<int> &vertices, const Array<int> &edges, void *context);

   // Callback function that returns some value for subgraph.
   // Graph is treated to be maximal by this criteria if one of its supergraph 
   // has different value from value for current subgraph. If there is no 
   // supergraphs for some graphs (cutted by max_vertices constraint) 
   // then such graph is treated to be maximal too.
   int (*maximal_critera_value_callback)(Graph &graph, const Array<int> &vertices,
      const Array<int> &edges, void *context);

   // Call main callback function only for maximal subgraphs (by maximal 
   // criteria callback or by size).
   bool handle_maximal;

   int   min_vertices;
   int   max_vertices;
   void *context;

   void process ();

protected:
   Graph &_graph;

   struct VertexEdgeParent
   {
      int v;
      int e;
      int parent;

      void reset () { v = e = parent = -1; }
   };

   CP_DECL;
   TL_CP_DECL(Array<VertexEdgeParent>, _front); // array with current front

   TL_CP_DECL(Array<int>, _vertices); // array with subgraph vertices
   TL_CP_DECL(Array<int>, _edges);    // array with subgraph edges

   TL_CP_DECL(Array<int>, _v_processed); // from _graph to _subtree

   void _reverseSearch (int front_idx, int cur_maximal_criteria_value);

   VertexEdge _m1, _m2;
};

}

#endif
