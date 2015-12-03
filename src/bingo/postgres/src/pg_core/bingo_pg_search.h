/* 
 */

#ifndef _BINGO_PG_SEARCH_H__
#define	_BINGO_PG_SEARCH_H__

#include "base_cpp/auto_ptr.h"
#include "base_cpp/array.h"
#include "base_cpp/exception.h"

#include "bingo_postgres.h"
#include "bingo_pg_index.h"
#include "bingo_pg_text.h"

#include "bingo_pg_search_engine.h"

class BingoPgText;
class BingoPgBuffer;
/*
 * Class for searcing molecular structures
 */
class BingoPgSearch {
public:
   BingoPgSearch(PG_OBJECT rel);
   ~BingoPgSearch();
   /*
    * Searches for the next match. Return true if search was successfull
    * Sets up item pointer
    */
   bool next(PG_OBJECT scan_desc_ptr, PG_OBJECT result_item);

   void setItemPointer(PG_OBJECT result_ptr);
   void readCmfItem(indigo::Array<char>& cmf_buf);

   BingoPgIndex& getIndex() {return _bufferIndex;}

   const char* getFuncName() const {return _funcName.ptr();}
   const char* getQuery() {return _queryText.getString();}
   const char* getOptions() {return _optionsText.getString();}

   DEF_ERROR("bingo search engine");

private:
   BingoPgSearch(const BingoPgSearch&); //no implicit copy

   void _initScanSearch();
//   void _defineQueryOptions();

   bool _initSearch;

   PG_OBJECT _indexScanDesc;

   BingoPgIndex _bufferIndex;
   indigo::AutoPtr<BingoPgSearchEngine> _fpEngine;

   BingoPgText _queryText;
   BingoPgText _optionsText;
   indigo::Array<char> _funcName;

};

#endif	/* BINGO_PG_SEARCH_H */

