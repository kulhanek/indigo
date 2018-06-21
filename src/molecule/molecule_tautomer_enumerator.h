/****************************************************************************
* Copyright (C) 2015 EPAM Systems
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

#ifndef __molecule_tautomer_enumerator__
#define __molecule_tautomer_enumerator__

#include "base_cpp/reusable_obj_array.h"
#include "molecule/molecule.h"
#include "molecule/molecule_layered_molecules.h"
#include "molecule/molecule_tautomer.h"

#define USE_DEPRECATED_INCHI

namespace indigo {

class Molecule;

class TautomerEnumerator
{
public:
   TautomerEnumerator(Molecule &molecule, TautomerMethod method);

   void constructMolecule(Molecule &molecule, int layer, bool needAromatize) const;
   bool enumerateLazy();
   void enumerateAll(bool needAromatization);
   bool aromatize();
   int beginNotAromatized();
   int beginAromatized();
   bool isValid(int);
   int next(int);
   void constructMolecule(Molecule &molecule, int n) const;

protected:
   struct Breadcrumps
   {
      Dbitset forwardMask;
      Dbitset backwardMask;
      ObjArray<Dbitset> forwardEdgesHistory;
      ObjArray<Dbitset> backwardEdgesHistory;
      Array<int> nodesHistory;
      Array<int> edgesHistory;
   };

   static bool matchEdge(Graph &subgraph, Graph &supergraph, int sub_idx, int super_idx, void *userdata);
   static bool matchVertex(Graph &subgraph, Graph &supergraph, const int *core_sub, int sub_idx, int super_idx, void *userdata);
   static void edgeAdd(Graph &subgraph, Graph &supergraph, int sub_idx, int super_idx, void *userdata);
   static void vertexAdd(Graph &subgraph, Graph &supergraph, int sub_idx, int super_idx, void *userdata);
   static void vertexRemove(Graph &subgraph, int sub_idx, void *userdata);

   static bool refine_proc(const Molecule &uncleaned_fragments, Molecule &product, Array<int> &mapping, void *userdata);
   static void product_proc(Molecule &product, Array<int> &monomers_indices, Array<int> &mapping, void *userdata);

   bool _performProcedure();
   bool _aromatize(int from, int to);

   Graph _zebraPattern;

public:
   LayeredMolecules layeredMolecules;
#ifdef USE_DEPRECATED_INCHI
   bool _use_deprecated_inchi;
#endif
   int _currentLayer;
   int _currentRule;
   bool _complete;
   int aromatizedRange[2];
   RedBlackSet<unsigned> _enumeratedHistory;
};

}

#endif /* __molecule_tautomer_enumerator__ */
