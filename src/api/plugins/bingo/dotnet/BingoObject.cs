using System;
using System.Collections;

namespace com.epam.indigo
{
	/// <summary>
	/// Bingo search object
	/// </summary>
	public unsafe class BingoObject : IDisposable
	{
		private int _id;
		private Indigo _indigo;
		private BingoLib _bingoLib;
		private IndigoDllLoader _dllLoader;
		private IDisposable _reference;

		internal BingoObject(int id, Indigo indigo, BingoLib bingo_lib)
		{
			this._id = id;
			this._indigo = indigo;
			this._bingoLib = bingo_lib;
			this._dllLoader = IndigoDllLoader.Instance;
		}

		/// <summary>
		/// Destructor
		/// </summary>
		~BingoObject()
		{
			Dispose();
		}
		/// <summary>
		/// </summary>
		public void Dispose()
		{
			if (_id >= 0 && _dllLoader != null && _dllLoader.isValid() && _bingoLib != null && _indigo != null)
			{
			    _indigo.setSessionID();
				Bingo.checkResult(_indigo, _bingoLib.bingoEndSearch(_id));
                _id = -1;
			}
		}

		/// <summary>
		/// Close method
		/// </summary>
		public void close()
		{
			Dispose();
		}

		internal int id
		{
			get { return _id; }
			set { _id = value; }
		}

		/// <summary>
		/// Method to move to the next record
		/// </summary>
		/// <returns>True if there are any more records</returns>
		public bool next()
		{
		   _indigo.setSessionID();
		   return (Bingo.checkResult(_indigo, _bingoLib.bingoNext(_id)) == 1) ? true : false;
		}

		/// <summary>
		/// Method to return current record id. Should be called after next() method.
		/// </summary>
		/// <returns>Record id</returns>
		public int getCurrentId()
		{
		   _indigo.setSessionID();
		   return Bingo.checkResult(_indigo, _bingoLib.bingoGetCurrentId(_id));
		}

		/// <summary>
		/// Method to return current similarity value. Should be called after next() method.
		/// </summary>
		/// <returns>Similarity value</returns>
		public float getCurrentSimilarityValue()
		{
		   _indigo.setSessionID();
		   return Bingo.checkResult(_indigo, _bingoLib.bingoGetCurrentSimilarityValue(_id));
		}

		/// <summary>
		/// Returns a shared IndigoObject for the matched target
		/// </summary>
		/// <returns>Shared Indigo object for the current search operation</returns>
		public IndigoObject getIndigoObject()
		{
		   _indigo.setSessionID();
		   IndigoObject res = new IndigoObject(_indigo, Bingo.checkResult(_indigo, _bingoLib.bingoGetObject(_id)));
		   _reference = res;
		   return res;
		}

		/// <summary>
		/// Method to estimate remaining hits count
		/// </summary>
		/// <returns>Estimated hits count</returns>
		public int estimateRemainingResultsCount ()
		{
		   _indigo.setSessionID();
		   return Bingo.checkResult(_indigo, _bingoLib.bingoEstimateRemainingResultsCount(_id));
		}

		/// <summary>
		/// Method to estimate remaining hits count error
		/// </summary>
		/// <returns>Estimated hits count error</returns>
		public int estimateRemainingResultsCountError ()
		{
		   _indigo.setSessionID();
		   return Bingo.checkResult(_indigo, _bingoLib.bingoEstimateRemainingResultsCountError(_id));
		}

		/// <summary>
		/// Method to estimate remaining search time
		/// </summary>
		/// <returns>Estimated remainin search time</returns>
		public float estimateRemainingTime ()
		{
			float esimated_time;
			_indigo.setSessionID();
			Bingo.checkResult(_indigo, _bingoLib.bingoEstimateRemainingTime(_id, &esimated_time));
			return esimated_time;
		}
	}
}
