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

#ifndef __inchi_parser_h__
#define __inchi_parser_h__

#include "base_cpp/array.h"

namespace indigo {
   DECL_EXCEPTION(InchiParserError);

   // Molecule InChI code constructor class 
   class InChICodeParser
   {
   public:
      enum
      {
         STATIC = 1,
         MOBILE = 2
      };

      explicit InChICodeParser(const char *inchi_code);

      int staticHydrogenPositionBegin()         { return _nextElement(STATIC, -1); }
      int staticHydrogenPositionNext(int index) { return _nextElement(STATIC, index); }
      int staticHydrogenPositionEnd()           { return _hydrogens.size(); }

      int mobileHydrogenPositionBegin()         { return _nextElement(MOBILE, -1); }
      int mobileHydrogenPositionNext(int index) { return _nextElement(MOBILE, index); }
      int mobileHydrogenPositionEnd()           { return _hydrogens.size(); }

      int mobileHydrogenCount() const   { return _mobileCount; }

      int getHydrogen(int index)  { return _hydrogens.at(index); }

      DECL_ERROR;

   private:
      Array<int> _hydrogens;
      Array<int> _types;
      int _mobileCount;

      int _nextElement(int type, int index);
   };

}

#endif // __inchi_parser_h__
