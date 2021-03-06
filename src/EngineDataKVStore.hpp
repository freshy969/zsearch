#ifndef ENGINEDATAKVSTORE_H
#define ENGINEDATAKVSTORE_H

#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <unordered_set>

#include "IKVStore.h"
#include "ZException.hpp"
#include "ZUtil.hpp"
#include "Constants.hpp"

using namespace std;

class EngineDataKVStore
{
	private:

		unsigned long docId;
		unsigned long long wordId;

		shared_ptr<KVStore::IKVStore> store;

	public:

		EngineDataKVStore(shared_ptr<KVStore::IKVStore> store) : docId(1), wordId(1), store(store)
		{
			string strId;

			cerr << "Populating docId from store" << endl;

			if (store->Get(zsearch::DOC_ID_KEY, strId).ok())
			{
				cerr << "got " << strId << endl;
				docId = ZUtil::atoi(strId.c_str());
			}

			cerr << "current docId is " << docId << endl;

			cerr << "Populating wordId from store" << endl;

			if (store->Get(zsearch::WORD_ID_KEY, strId).ok())
			{
				cerr << "got " << strId << endl;
				wordId = ZUtil::atoi(strId.c_str());
			}

			cerr << "current wordId is " << wordId << endl;

			if ((docId <= 1 && wordId > 1) || (docId > 1 && wordId <= 1))
			{
				throw ZException("docId and wordId mismatched! Most likely db is corrupt.");
			}

		}

		~EngineDataKVStore()
		{
			cerr << "Persisting docId " << docId << " to store" << endl;

			string strDocId;
			ZUtil::PutUint64(strDocId, docId);

			if (!store->Put(zsearch::DOC_ID_KEY, strDocId).ok())
			{
				cerr << "Error persisting docId to storage, db might be corrupt or invalid at startup" << endl;
			}

			cerr << "Persisting wordId " << wordId << " to store" << endl;

			string strWordId;
			ZUtil::PutUint64(strWordId, wordId);

			if (!store->Put(zsearch::WORD_ID_KEY, strWordId).ok())
			{
				cerr << "Error persisting wordId to storage, db might be corrupt or invalid at startup" << endl;
			}

			cerr << "Destroyed EngineDataKVStore" << endl;
		}

		unsigned long& getDocId()
		{
			return docId;
		}

		unsigned long long& getWordId()
		{
			return wordId;
		}

};

#endif
