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

#ifndef __molecule_tautomer_matcher__
#define __molecule_tautomer_matcher__

#include "graph/embedding_enumerator.h"
#include "molecule/molecule_tautomer.h"
#include "base_cpp/auto_ptr.h"

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4251)
#endif

namespace indigo {

class Molecule;
class AromaticityMatcher;

class DLLEXPORT MoleculeTautomerMatcher
{
public:
   DECL_ERROR;

   bool highlight;

   AromaticityOptions arom_options;

   MoleculeTautomerMatcher (Molecule &target, bool substructure);

   void setQuery (BaseMolecule &query);

   void setRulesList (const PtrArray<TautomerRule> *rules_list);
   void setRules (int rules_set, bool force_hydrogens, bool ring_chain, TautomerMethod method);

   bool find ();

   const int * getQueryMapping ();

   static void parseConditions (const char *tautomer_text, int &rules, bool &force_hydrogens, bool &ring_chain, TautomerMethod &method);

   static int countNonHydrogens (BaseMolecule &molecule);

protected:

   bool _find (bool substructure);

   static bool _checkRules (TautomerSearchContext &context, int first1, int first2, int last1, int last2);

   bool _substructure;
   bool _force_hydrogens;
   bool _ring_chain;
   TautomerMethod _method;
   int  _rules;

   const PtrArray<TautomerRule> *_rules_list;
   AutoPtr<TautomerSearchContext> _context;
   Molecule &_target_src;
   AutoPtr<BaseMolecule> _query;

   Obj<TautomerSuperStructure> _target;
   BaseMolecule *_supermol;

   Obj<GraphDecomposer> _query_decomposer;
   Obj<GraphDecomposer> _target_decomposer;
};

}

#ifdef _WIN32
#pragma warning(pop)
#endif

#endif
