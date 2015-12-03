/****************************************************************************
 * Copyright (C) 2009-2011 GGA Software Services LLC
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

#include "layout/molecule_layout_graph.h"

using namespace indigo;

void MoleculeLayoutGraph::_setChainType (const Array<int> &chain, const Array<int> &mapping, int type)
{
   for (int i = 0; i < chain.size() - 1; i++)
   {
      if (i > 0)
         _layout_vertices[mapping[chain[i]]].type = type;

      const Vertex &vert = getVertex(mapping[chain[i]]);
      int edge_idx = vert.neiEdge(vert.findNeiVertex(mapping[chain[i + 1]]));

      _layout_edges[edge_idx].type = type;
   }
}

bool MoleculeLayoutGraph::_splitCycle (const Cycle &cycle, const Array<int> &cycle_vertex_types, bool check_boundary,
                                       Array<int> &chain_ext, Array<int> &chain_int, int &c_beg, int &c_end) const
{
   int i, j, k;

   // 1. First vertex is drawn
   if  (cycle_vertex_types[0] != ELEMENT_NOT_DRAWN)
   {
      // 1. Find first not drawn 
      for (i = 0; i < cycle.vertexCount(); i++)
         if (cycle_vertex_types[i] == ELEMENT_NOT_DRAWN)
            break;

      // 2. Find the last not drawn
      for (j = i; j < cycle.vertexCount(); j++)
         if (cycle_vertex_types[j] != ELEMENT_NOT_DRAWN)
            break;
      j--;

      // 3. Check other are drawn
      for (k = j + 1; k < cycle.vertexCount(); k++)
         if (cycle_vertex_types[k] == ELEMENT_NOT_DRAWN)
            return false;

      // 4. Check boundary vertices are marked as boundary
      if (check_boundary)
         if (cycle_vertex_types[i - 1] != ELEMENT_BOUNDARY ||
             cycle_vertex_types[(j + 1) % cycle.vertexCount()] != ELEMENT_BOUNDARY)
            return false;

      // 5. Make internal and external chains
      c_beg = cycle.getVertex(i - 1);
      c_end = cycle.getVertex((j + 1) % cycle.vertexCount());

      chain_ext.clear();
      chain_int.clear();

      for (k = i - 1; k < j + 2; k++)
         chain_ext.push(cycle.getVertex(k % cycle.vertexCount()));
      for (k = i - 1; k >= 0; k--)
         chain_int.push(cycle.getVertex(k));
      for (k = cycle.vertexCount() - 1; k > j; k--)
         chain_int.push(cycle.getVertex(k));
   } else // First vertex is not drawn
   {
      // 1. Find first vertex after the last not drawn from the beginning
      for (i = 0; i < cycle.vertexCount(); i++)
         if (cycle_vertex_types[i] != ELEMENT_NOT_DRAWN)
            break;

      // 2. Find first vertex after the last not drawn from the end
      for (j = cycle.vertexCount() - 1; j >= 0; j--)
         if (cycle_vertex_types[j] != ELEMENT_NOT_DRAWN)
            break;

      // 3. Check other are drawn
      for (k = i; k < j + 1; k++)
         if (cycle_vertex_types[k] == ELEMENT_NOT_DRAWN)
            return false;

      // 4. Check boundary vertices are marked as boundary
      if (check_boundary)
         if (cycle_vertex_types[i] != ELEMENT_BOUNDARY || cycle_vertex_types[j] != ELEMENT_BOUNDARY)
            return false;

      // 5. Make internal and external chains
      c_beg = cycle.getVertex(i);
      c_end = cycle.getVertex(j);

      chain_int.clear();
      chain_ext.clear();
      for (k = i; k < j + 1; k++)
         chain_int.push(cycle.getVertex(k));
      for (k = i; k >= 0; k--)
         chain_ext.push(cycle.getVertex(k));
      for (k = cycle.vertexCount() - 1; k > j - 1; k--)
         chain_ext.push(cycle.getVertex(k));
   }

   return true;
}

// Split cycle into separate chains which are not drawn
void MoleculeLayoutGraph::_splitCycle2 (const Cycle &cycle, const Array<int> &cycle_vertex_types, ObjArray < Array<int> > &chains_ext) const
{
   int i;
   
   chains_ext.clear();
   
   i = 0;
   
   while (cycle_vertex_types[i] == ELEMENT_NOT_DRAWN)
      i++;

   while (i < cycle.vertexCount())
   {
      for (; i < cycle.vertexCount(); i++)
         if (cycle_vertex_types[i] == ELEMENT_NOT_DRAWN)
            break;
      
      if (i == cycle.vertexCount())
         break;
      
      Array<int> &chain_ext = chains_ext.push();

      chain_ext.push(cycle.getVertex(i - 1));

      for (; i < cycle.vertexCount() && cycle_vertex_types[i] == ELEMENT_NOT_DRAWN; i++)
          chain_ext.push(cycle.getVertex(i));

      if (i < cycle.vertexCount() || cycle_vertex_types[0] != ELEMENT_NOT_DRAWN)
         chain_ext.push(cycle.getVertex(i % cycle.vertexCount()));
   }
   
   if (cycle_vertex_types[0] == ELEMENT_NOT_DRAWN)
   {
      i = cycle.vertexCount() - 1;
      
      Array<int> *chain_ext = 0;
      
      if (cycle_vertex_types[i] != ELEMENT_NOT_DRAWN)
      {
         chain_ext = &chains_ext.push();
         chain_ext->push(cycle.getVertex(i));
         
      } else
         chain_ext = &chains_ext.top();

      for (i = 0; cycle_vertex_types[i] == ELEMENT_NOT_DRAWN; i++)
         chain_ext->push(cycle.getVertex(i));
      
      chain_ext->push(cycle.getVertex(i));
   }
}

// Attach cycle outside component border. Component must have given number of common edges or any (if 0)
bool MoleculeLayoutGraph::_attachCycleOutside (const Cycle &cycle, float length, int n_common_edges)
{
   MoleculeLayoutGraph new_bc;
   int i, j, k;
   int n_common_e = 0, n_common_v = 0;
   bool is_attached = false;
   QS_DEF(Array<int>, cycle_vertex_types);
   QS_DEF(Array<int>, border1v);
   QS_DEF(Array<int>, border1e);
   QS_DEF(Array<int>, border2v);
   QS_DEF(Array<int>, border2e);
   Vec2f p;

   cycle_vertex_types.clear_resize(cycle.vertexCount());
   cycle_vertex_types.zerofill();

   for (i = 0; i < cycle.vertexCount(); i++)
   {
      cycle_vertex_types[i] = _layout_vertices[cycle.getVertex(i)].type;

      if (cycle_vertex_types[i] > 0)
         n_common_v++;
      if (_layout_edges[cycle.getEdge(i)].type > 0)
         n_common_e++;
   }

   if (n_common_edges  > 0 && n_common_e != n_common_edges)
      return false;

   // Everything is drawn
   if (n_common_e == cycle.vertexCount())
      return true;

   // If all vertices are drawn then draw edges without intersections with already drawn
   // Find new border
   // If then all edges are drawn return true, else return false
   if  (n_common_v == cycle.vertexCount())
   {
      is_attached = true;

      for (i = 0; i < cycle.vertexCount(); i++)
      {
         if (_layout_edges[cycle.getEdge(i)].type == ELEMENT_NOT_DRAWN)
         {
            for (j = edgeBegin(); j < edgeEnd(); j = edgeNext(j))
               if (_layout_edges[j].type > ELEMENT_NOT_DRAWN)
                  if ((_calcIntersection(i, j) % 10) != 1)
                  {
                     is_attached = false;
                     break;
                  }

            if (!is_attached)
               continue;

            if  (cycle_vertex_types[i] == ELEMENT_INTERNAL ||
                 cycle_vertex_types[(i + 1) % cycle.vertexCount()] == ELEMENT_INTERNAL)
            {
               _layout_edges[cycle.getEdge(i)].type = ELEMENT_INTERNAL;
            } else
            {  // Both vertices are boundary.
               // Check if edge is boundary by its center
               p.lineCombin2(_layout_vertices[cycle.getVertex(i)].pos, 0.9f,
                  _layout_vertices[cycle.getVertexC(i + 1)].pos, 0.1f);

               if  (_isPointOutside(p))
               {
                  _splitBorder(cycle.getVertex(i), cycle.getVertexC(i + 1), border1v, border1e, border2v, border2e);

                  _layout_edges[cycle.getEdge(i)].type = ELEMENT_BOUNDARY;

                  // Ignore border1 and check if vertices are inside new bound
                  // if no then restore Border1 and ignore Border2
                  for (j = 0; j < border1e.size(); j++)
                  {
                     if (j > 0)
                        _layout_vertices[border1v[j]].type = ELEMENT_IGNORE;
                     _layout_edges[border1e[j]].type = ELEMENT_IGNORE;
                  }

                  p.lineCombin2(_layout_vertices[border1v[1]].pos, 0.9f, _layout_vertices[border1v[2]].pos, 0.1f);

                  if (!_isPointOutside(p))
                  {
                     for (j = 0; j < border1e.size(); j++)
                     {
                        if (j > 0)
                           _layout_vertices[border1v[j]].type = ELEMENT_INTERNAL;
                        _layout_edges[border1e[j]].type = ELEMENT_INTERNAL;
                     }
                  } else {
                     for (j = 0; j < border1e.size(); j++)
                     {
                        if (j > 0)
                           _layout_vertices[border1v[j]].type = ELEMENT_BOUNDARY;
                        _layout_edges[border1e[j]].type = ELEMENT_BOUNDARY;
                     }
                     for (j = 0; j < border2e.size(); j++)
                     {
                        if (j > 0)
                           _layout_vertices[border2v[j]].type = ELEMENT_INTERNAL;
                        _layout_edges[border2e[j]].type = ELEMENT_INTERNAL;
                     }
                  }
               } else
                   _layout_edges[cycle.getEdge(i)].type = ELEMENT_INTERNAL;
            }
         }
      }
      return is_attached;
   }

   // Attach cycle of two parts:
   //   chain_int - internal and boundary vertices of component
   //   chain_ext - not drawn vertices and two boundary
   // If number of common vertices is less than 2 then skip cycle
   if (n_common_v < 2)
      return false;

   QS_DEF(Array<int>, chain_ext);
   QS_DEF(Array<int>, chain_int);
   int c_beg, c_end;

   if (!_splitCycle(cycle, cycle_vertex_types, true, chain_ext, chain_int, c_beg, c_end))
      return false;

   // Make Border1, Border2 from component border (borders have two common vertices)
   _splitBorder(c_beg, c_end, border1v, border1e, border2v, border2e);

   QS_DEF(MoleculeLayoutGraph, next_bc);
   QS_DEF(Array<int>, mapping);

   for (int n_try = 0; n_try < 2 && !is_attached; n_try++)
   {
      // Complete regular polygon by chain_ext (on the one side if n_try == 1 and other side if n_try == 2
      next_bc.cloneLayoutGraph(*this, &mapping);

      if  (length > 1)
      {
         k = chain_ext.size() - 2;
         float dist = Vec2f::dist(_layout_vertices[c_beg].pos, _layout_vertices[c_end].pos);

         if (dist > (k + 1) * length)
            length = 0.2f + dist / (k + 1);
      }

      if (n_try == 0)
      {
         if (!next_bc._drawRegularCurveEx(chain_ext, c_beg, c_end, length, true, ELEMENT_BOUNDARY, mapping))
            return false;
      } else // (n_try == 1)
      {
         if (!next_bc._drawRegularCurveEx(chain_ext, c_beg, c_end, length, false, ELEMENT_BOUNDARY, mapping))
            return false;
      }

      bool bad_try = false;

      // Check chain_ext is outside bound
      for (i = 1; i < chain_ext.size() - 1; i++)
      {
         if (!_isPointOutside(next_bc._layout_vertices[mapping[chain_ext[i]]].pos))
         {
            bad_try = true;
            break;
         }
      }

      if (bad_try)
         continue;

      // Check edges from chain_ext intersect previous border other than in the ends
      for (i = 0; i < chain_ext.size() - 1 && !bad_try; i++)
         for (j = next_bc.edgeBegin(); j < next_bc.edgeEnd(); j = next_bc.edgeNext(j))
         {
            if (_layout_edges[next_bc._layout_edges[j].ext_idx].type == ELEMENT_BOUNDARY)
            {
               const Vertex &vert = next_bc.getVertex(mapping[chain_ext[i]]);
               int edge1_idx = vert.neiEdge(vert.findNeiVertex(mapping[chain_ext[i + 1]]));
               int intersect = next_bc._calcIntersection(edge1_idx, j);

               const Edge &edge1 = next_bc.getEdge(edge1_idx);
               const Edge &edge2 = next_bc.getEdge(j);

               // Check if edges intersect
               if ((intersect % 10) != 1 || (intersect == 21 && edge1.beg != edge2.beg && edge1.beg != edge2.end
                  && edge1.end != edge2.beg && edge1.end != edge2.end))
               {
                  bad_try = true;
                  break;
               }
            }
         }

      if (bad_try)
         continue;

      // If Border1 lays inside cycle [chain_ext,border2] than it becomes internal and Border2 becomes boundary
      // If Border2 lays inside cycle [chain_ext,border1] than it becomes internal and Border1 becomes boundary
      // In both cases chain_ext becomes external.
      // Ignore border1 and check if vertices are inside new bound
      // if no then restore Border1 and ignore Border2
      next_bc._setChainType(border1v, mapping, ELEMENT_IGNORE);

      p.lineCombin2(next_bc._layout_vertices[mapping[border1v[0]]].pos, 0.9f,
         next_bc._layout_vertices[mapping[border1v[1]]].pos, 0.1f);

      if (!next_bc._isPointOutside(p))
         next_bc._setChainType(border1v, mapping, ELEMENT_INTERNAL);
      else
      {
         next_bc._setChainType(border1v, mapping, ELEMENT_BOUNDARY);
         next_bc._setChainType(border2v, mapping, ELEMENT_INTERNAL);
      }
      // Replace chain_ext by single edge and check if chain_ext is outside (try to draw convex polygon)
      if (n_try == 0 && chain_ext.size() > 2)
      {
         next_bc._setChainType(chain_ext, mapping, ELEMENT_IGNORE);

         const Vertex &vert = next_bc.getVertex(mapping[c_beg]);
         int edge_idx;
         int type = -1;

         if ((edge_idx = vert.findNeiVertex(mapping[c_end])) != -1)
         {
            edge_idx = vert.neiEdge(edge_idx);
            type = next_bc._layout_edges[edge_idx].type;
            next_bc._layout_edges[edge_idx].type = ELEMENT_BOUNDARY;
         } else 
            edge_idx = next_bc.addLayoutEdge(mapping[c_beg], mapping[c_end], -1, ELEMENT_BOUNDARY);

         if (!next_bc._isPointOutside(next_bc._layout_vertices[mapping[chain_ext[1]]].pos))
            continue;

         next_bc._setChainType(chain_ext, mapping, ELEMENT_BOUNDARY);

         if (type < 0)
            next_bc.removeEdge(edge_idx);
         else
            next_bc._layout_edges[edge_idx].type = type;
      }
      // Check if border1, border2 are outside cycle
      // (draw cycle outside not inside)
      if (n_try == 0)
      {
         for (i = 1; i < border1v.size() - 1 && !is_attached; i++)
            if (next_bc._isPointOutsideCycleEx(cycle, next_bc._layout_vertices[mapping[border1v[i]]].pos, mapping))
               is_attached = true;

         for (i = 1; i < border2v.size() - 1 && !is_attached; i++)
            if (next_bc._isPointOutsideCycleEx(cycle, next_bc._layout_vertices[mapping[border2v[i]]].pos, mapping))
               is_attached = true;
      } else
         is_attached = true;
   }

   // Copy new layout
   if (is_attached)
      next_bc.copyLayoutTo(*this, mapping);

   return is_attached;
}

// Attach cycle inside component border.
// Everything can be attached outside is already attached.
bool MoleculeLayoutGraph::_attachCycleInside (const Cycle &cycle, float length)
{
   int n_common_e = 0, n_common_v = 0;
   int i, j;

   QS_DEF(Array<int>, cycle_vertex_types);

   cycle_vertex_types.clear_resize(cycle.vertexCount());
   cycle_vertex_types.zerofill();

   for (i = 0; i < cycle.vertexCount(); i++)
   {
      cycle_vertex_types[i] = _layout_vertices[cycle.getVertex(i)].type;

      if (cycle_vertex_types[i] > 0)
         n_common_v++;
      if (_layout_edges[cycle.getEdge(i)].type > 0)
         n_common_e++;
   }

   // Everything is drawn
   if (n_common_e  == cycle.vertexCount())
      return true;

   bool attached = false;

   // If all vertices are drawn then draw edges without intersections with already drawn
   // Find new border
   // If then all edges are drawn return true, else return false
   if (n_common_v == cycle.vertexCount())
   {
      attached = true;

      for (i = 0; i < cycle.vertexCount(); i++) 
      {
         if (_layout_edges[cycle.getEdge(i)].type == ELEMENT_NOT_DRAWN)
         {
            for (j = edgeBegin(); j < edgeEnd(); j = edgeNext(j))
               if (_layout_edges[j].type > ELEMENT_NOT_DRAWN)
                  if ((_calcIntersection(i, j) % 10) != 1)
                  {
                     attached = false;
                     break;
                  }

             if (!attached)
                continue;
            
             _layout_edges[cycle.getEdge(i)].type = ELEMENT_INTERNAL;
         }
      }

      return attached;
   }

   // Attach cycle of two parts:
   //   chain_int - internal and boundary vertices of component
   //   chain_ext - not drawn vertices and two boundary
   // If number of common vertices is less than 2 then skip cycle
   if (n_common_v < 2) 
      return false;

   QS_DEF(Array<int>, chain_ext);
   QS_DEF(Array<int>, chain_int);
   int c_beg, c_end;

   if (!_splitCycle(cycle, cycle_vertex_types, false, chain_ext, chain_int, c_beg, c_end))
      return false;

   QS_DEF(MoleculeLayoutGraph, next_bc);
   QS_DEF(Array<int>, mapping);

   for (int n_try = 0; n_try < 2 && !attached; n_try++)
   {
      // Complete regular polygon by chain_ext (on the one side if n_try == 1 and other side if n_try == 2
      next_bc.cloneLayoutGraph(*this, &mapping);

      if (n_try == 0)
      {
         if (!next_bc._drawRegularCurveEx(chain_ext, c_beg, c_end, length, true, ELEMENT_INTERNAL, mapping))
            return false;
      } else // (n_try == 1)
      {
         if (!next_bc._drawRegularCurveEx(chain_ext, c_beg, c_end, length, false, ELEMENT_INTERNAL, mapping))
            return false;
      }

      bool bad_try = false;

      // Check edges from chain_ext intersect previous border other than in the ends
      for (i = 0; i < chain_ext.size() - 1 && !bad_try; i++)
         for (j = next_bc.edgeBegin(); j < next_bc.edgeEnd(); j = next_bc.edgeNext(j))
         {
            if (_layout_edges[next_bc._layout_edges[j].ext_idx].type != ELEMENT_NOT_DRAWN)
            {
               const Vertex &vert = next_bc.getVertex(mapping[chain_ext[i]]);
               int edge1_idx = vert.neiEdge(vert.findNeiVertex(mapping[chain_ext[i + 1]]));
               int intersect = next_bc._calcIntersection(edge1_idx, j);

               const Edge &edge1 = next_bc.getEdge(edge1_idx);
               const Edge &edge2 = next_bc.getEdge(j);

               // Check if edges intersect
               if ((intersect % 10) != 1 || (intersect == 21 && edge1.beg != edge2.beg && edge1.beg != edge2.end
                  && edge1.end != edge2.beg && edge1.end != edge2.end))
               {
                  bad_try = true;
                  break;
               }
            }
         }

      if (!bad_try)
         attached = true;
   }

   // Copy new layout
   if (attached)
      next_bc.copyLayoutTo(*this, mapping);

   return attached;
}

// Attach cycle with intersections.
// Everything can be attached w/o intersections is already attached.
// Not all cycle vertices are drawn
bool MoleculeLayoutGraph::_attachCycleWithIntersections (const Cycle &cycle, float length)
{
   int n_common_e = 0, n_common_v = 0;
   int i, j, k;

   QS_DEF(Array<int>, cycle_vertex_types);

   cycle_vertex_types.clear_resize(cycle.vertexCount());
   cycle_vertex_types.zerofill();

   for (i = 0; i < cycle.vertexCount(); i++)
   {
      cycle_vertex_types[i] = _layout_vertices[cycle.getVertex(i)].type;

      if (cycle_vertex_types[i] > 0)
         n_common_v++;
      if (_layout_edges[cycle.getEdge(i)].type > 0)
         n_common_e++;
   }

   // All vertices are drawn - return false
   if (n_common_v == cycle.vertexCount())
      return false;

   // Attach cycle of two parts:
   //   chain_int - internal and boundary vertices of component
   //   chain_ext - not drawn vertices and two boundary
   // If number of common vertices is less than 2 then skip cycle
   if (n_common_v < 2)
      throw Error("attaching cycle with only one vertex drawn"); //return false;

   QS_DEF(ObjArray< Array<int> >, chains_ext);

   // Split cycle into separate external (not drawn) chains
   _splitCycle2(cycle, cycle_vertex_types, chains_ext);
   
   // Attach each chain separately
   for (int chain_idx = 0; chain_idx < chains_ext.size(); chain_idx++)
   {
      Array<int> &chain_ext = chains_ext[chain_idx];
      int c_beg = chain_ext[0], c_end = chain_ext[chain_ext.size() - 1];

      float max_length = length * 4; // to avoid infinite values
      
      // Complete regular polygon by chain_ext (on the one side if n_try == 1 and other side if n_try == 2
      // Mark new vertices and edges as not planar
      k = chain_ext.size() - 2;
      float dist = Vec2f::dist(getPos(c_beg), getPos(c_end));

      if (dist > (k + 1) * length)
      {   
         length = 0.2f + dist / (k + 1);
         max_length = __max(max_length, length * 1.5f);  // update max length if needed
      }

      bool attached = false;

      while (!attached && length < max_length)
      {
         attached = true;

         while (!_drawRegularCurve(chain_ext, c_beg, c_end, length, true, ELEMENT_NOT_PLANAR))
            length *= 1.2f;

         // Choose position with minimal energy
         Vec2f &pos1 = getPos(chain_ext[1]);
         float s = 0;

         for (i = vertexBegin(); i < vertexEnd(); i = vertexNext(i))
            if (_layout_vertices[i].type == ELEMENT_INTERNAL || _layout_vertices[i].type == ELEMENT_BOUNDARY)  
            {
               Vec2f &pos = getPos(i);
               s += Vec2f::distSqr(pos, pos1);
            }

         _drawRegularCurve(chain_ext, c_beg, c_end, length, false, ELEMENT_NOT_PLANAR);
         
         float sn = 0;

         for (i = vertexBegin(); i < vertexEnd(); i = vertexNext(i))
         {
            int type = _layout_vertices[i].type;

            if (type == ELEMENT_INTERNAL || type == ELEMENT_BOUNDARY)  
            {
               Vec2f &pos = getPos(i);
               sn += Vec2f::distSqr(pos, pos1);
            }
         }

         if (sn < s - 0.001)
            _drawRegularCurve(chain_ext, c_beg, c_end, length, true, ELEMENT_NOT_PLANAR);
         
         // Try to change edge length to avoid bad layout
         for (i = 1; i < chain_ext.size() - 1; i++)
         {
            if (_isVertexOnSomeEdge(chain_ext[i]))
            {
               length *= 1.2f;
               attached = false;
               break;
            }
         }

         if (!attached)
            continue;

         for (j = 0; j < chain_ext.size() - 1 && attached; j++)
         {
            for (i = vertexBegin(); i < vertexEnd(); i = vertexNext(i))
            {
               int type = _layout_vertices[i].type;

               if (i != chain_ext[j] && i != chain_ext[j + 1] && (type == ELEMENT_INTERNAL || type == ELEMENT_BOUNDARY))
               {
                  if (_isVertexOnEdge(i, chain_ext[j], chain_ext[j + 1]))
                  {
                     length *= 1.2f;
                     attached = false;
                     break;
                  }
               }
            }
         }
      }
   }

   return true;
}

// Attach set of trivial components
void MoleculeLayoutGraph::_attachDandlingVertices (int vert_idx, Array<int> &adjacent_list)
{
   int i, j;
   int n_pos = 0, not_drawn_idx = 0, drawn_idx = -1;
   float phi;
   Vec2f v1, v2;
   QS_DEF(Array<Vec2f>, positions);
   int parity = 0;

   const Vertex &vert = getVertex(vert_idx);

   // Calculate number of drawn edges
   for (i = vert.neiBegin(); i < vert.neiEnd(); i = vert.neiNext(i))
   {
      if (getVertexType(vert.neiVertex(i)) != ELEMENT_NOT_DRAWN &&
          getEdgeType(vert.neiEdge(i)) != ELEMENT_NOT_DRAWN)
      {
         n_pos++;
         drawn_idx = i;
      } else
         not_drawn_idx = i;
   }

   if (n_pos > 1 && adjacent_list.size() == 1)
   {
      // n_pos of drawn edges and one not drawn 
      positions.clear_resize(n_pos);

      Vec2f p0;

      QS_DEF(Array<float>, angles); // polar angles of drawn edges
      QS_DEF(Array<int>, edges); // edge indices in CCW order

      angles.clear();
      edges.clear();

      // find angles
      for (i = vert.neiBegin(); i < vert.neiEnd(); i = vert.neiNext(i))
      {
         if (i == not_drawn_idx)
            continue;

         edges.push(i);
         p0.diff(getPos(vert.neiVertex(i)), getPos(vert_idx));
         angles.push(p0.tiltAngle2());
      }

      // sort
      for (i = 0; i < n_pos; i++)
         for (j = i + 1; j < n_pos; j++)
            if (angles[i] > angles[j])
            {
               angles.swap(i, j);
               edges.swap(i, j);
            }

      // place new edge between drawn
      v1 = getPos(vert_idx);

      for (i = 0; i < n_pos - 1; i++)
      {
         v2 = getPos(vert.neiVertex(edges[i]));
         phi = (angles[i + 1] - angles[i]) / 2;
         _calculatePos(phi, v1, v2, positions[i]);      
      }

      v2 = getPos(vert.neiVertex(edges.top()));
      phi = (2 * PI + angles[0] - angles.top()) / 2;
      _calculatePos(phi, v1, v2, positions.top());
   } else 
   {
      // Single drawn edge
      // Split 2pi to n_pos+1 parts
      // Place vertices like regular polygon
      // Drawn is first vertex, other in CCW order
      if (adjacent_list.size() > 1)
      {
         if (n_pos == 1 && adjacent_list.size() == 3) // to avoid four bonds to be drawn like cross
         {
            n_pos = 5;
            
            for (i = 0; i < adjacent_list.size(); i++)
               if (getVertex(adjacent_list[i]).degree() != 1)
               {
                  n_pos = adjacent_list.size();
                  break;
               }
                  
         } else
            n_pos = adjacent_list.size();
      } else 
      {
         int type1 = 0, type2 = 0;

         if (_molecule != 0)
         {
            int first_nei = vert.neiBegin();
            type1 = _molecule->getBondOrder(_molecule_edge_mapping[_layout_edges[vert.neiEdge(first_nei)].ext_idx]);
            type2 = _molecule->getBondOrder(_molecule_edge_mapping[_layout_edges[vert.neiEdge(vert.neiNext(first_nei))].ext_idx]);
         }
         if (n_pos != 1 || (!(type1 == BOND_TRIPLE || type2 == BOND_TRIPLE) && !(type1 == BOND_DOUBLE && type2 == BOND_DOUBLE)))
            n_pos = 2;
      }

      positions.clear_resize(n_pos);

      phi = 2 * PI / (n_pos + 1);
      v1 = getPos(vert_idx);

      for (i = vert.neiBegin(); i < vert.neiEnd(); i = vert.neiNext(i))
      {
         if (getVertexType(vert.neiVertex(i)) != ELEMENT_NOT_DRAWN &&
            getEdgeType(vert.neiEdge(i)) != ELEMENT_NOT_DRAWN)
         {
            v2 = getPos(vert.neiVertex(i));
            break;
         }
      }

      _calculatePos(phi, v1, v2, positions[0]);

      for (i = 1; i < n_pos; i++)
      {
         v2 = positions[i - 1];
         _calculatePos(phi, v1, v2, positions[i]);
      }

      // Check cis/trans
      if (_molecule != 0 && n_pos == 2)
      {
         parity = _molecule->cis_trans.getParity(_molecule_edge_mapping[_layout_edges[vert.neiEdge(drawn_idx)].ext_idx]);

         if (parity != 0)
         {
            int substituents[4];
            _molecule->cis_trans.getSubstituents_All(_molecule_edge_mapping[_layout_edges[vert.neiEdge(drawn_idx)].ext_idx], substituents);
           
            int to_draw_substituent = -1;

            for (i = 0; i < 4; i++)
               if (substituents[i] == _layout_vertices[adjacent_list.top()].ext_idx)
               {
                  to_draw_substituent = i;
                  break;
               }

            const Vertex &drawn_vert = getVertex(vert.neiVertex(drawn_idx));

            int drawn_substituent = -1;
            int drawn_substituent_idx = -1;
    
            for (i = drawn_vert.neiBegin(); i < drawn_vert.neiEnd(); i = drawn_vert.neiNext(i))
               if (drawn_vert.neiVertex(i) != vert_idx) // must be drawn
               {
                  for (j = 0; j < 4; j++)
                     if (substituents[j] == _layout_vertices[drawn_vert.neiVertex(i)].ext_idx)
                     {
                        drawn_substituent_idx = drawn_vert.neiVertex(i);
                        drawn_substituent = j;
                        break;
                     }
                  break;
               }
           
            bool same_side = false;

            if ((parity == MoleculeCisTrans::CIS) == (abs(to_draw_substituent - drawn_substituent) == 2))
               same_side = true;
          
            int side_sign = MoleculeCisTrans::sameside(Vec3f(_layout_vertices[vert.neiVertex(drawn_idx)].pos), 
               Vec3f(_layout_vertices[vert_idx].pos), Vec3f(_layout_vertices[drawn_substituent_idx].pos), Vec3f(positions[0]));

            if (same_side)
            {
               if (side_sign == -1)
                  positions.swap(0, 1);
            } else if (side_sign == 1)
               positions.swap(0, 1);
         }
      }
   }

   // Calculate energy
   if (parity == 0)
   {
      QS_DEF(Array<double>, energies);
      QS_DEF(Array<double>, norm_a);
      double norm = 0.0;
      float r = 0.f;
      Vec2f p0;

      energies.clear_resize(n_pos);
      norm_a.clear_resize(vertexEnd());
      energies.zerofill();

      for (i = vertexBegin(); i < vertexEnd(); i = vertexNext(i))
         if (getVertexType(i) != ELEMENT_NOT_DRAWN)
         {
            norm_a[i] = _layout_vertices[i].morgan_code;
            norm += norm_a[i] * norm_a[i];
         }

      norm = sqrt(norm);

      for (i = 0; i < n_pos; i++) 
         for (j = vertexBegin(); j < vertexEnd(); j = vertexNext(j))
            if (getVertexType(j) != ELEMENT_NOT_DRAWN)
            {
               p0.diff(positions[i], getPos(j));
               r = p0.lengthSqr();

               if  (r < EPSILON)
               {
                  energies[i] = 1E+20f;
                  continue;
               }

               energies[i] += ((norm_a[j] / norm + 0.5) / r);
            }

      // Sort by energies
      for (i = 0; i < n_pos; i++)
         for (j = i + 1; j < n_pos; j++)
            if (energies[j] < energies[i])
            {
               energies.swap(i, j);
               positions.swap(i, j);
            }
   }

   // Assign coordinates
   j = 0;
   while (adjacent_list.size() > 0)
   {
      i = adjacent_list.pop();

      _layout_vertices[i].pos = positions[j];
      _layout_vertices[i].type = ELEMENT_BOUNDARY;
      _layout_edges[vert.neiEdge(vert.findNeiVertex(i))].type = ELEMENT_BOUNDARY;
      j++;
   }
}
