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

#ifndef __mango_index__
#define __mango_index__

#include "base_cpp/array.h"
#include "base_cpp/output.h"
#include "core/mango_matchers.h"
#include "core/bingo_index.h"

using namespace indigo;

namespace indigo
{
   class Scanner;
}

class BingoContext;

class MangoIndex : public BingoIndex
{
public:
   void prepare (Scanner &molfile, Output &output, OsLock *lock_for_exclusive_access);

   const Array<char> & getCmf () const;
   const Array<char> & getXyz () const;

   const MangoExact::Hash & getHash () const;

   const char * getGrossString () const;
   const char * getCountedElementsString () const;
   const Array<int> & getCountedElements () const;

   const byte * getFingerprint () const;

   const char * getFingerprint_Sim_Str () const;
   
   float getMolecularMass() const;

   int getFpSimilarityBitsCount () const;

   static const int counted_elements[6];

   void clear ();

private:
   // CMF-packed aromatized molecule and coordinates
   Array<char> _cmf;
   Array<char> _xyz;

   // hash for exact match
   MangoExact::Hash _hash; 

   // gross formula
   Array<int> _gross;
   Array<char> _gross_str;

   Array<byte> _fp;
   Array<char> _fp_sim_str;
           
   // comma-separated list of selected counters 
   // (for non-exact gross formula search)
   Array<char> _counted_elems_str;
   Array<int>  _counted_elem_counters;

   // Molecular mass
   float _molecular_mass;

   // Number of one bits in similarity fingerprint
   int _fp_sim_bits_count;
};

#endif
