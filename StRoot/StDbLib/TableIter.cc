#include "TableIter.hh"
#include "StDbTable.h"


////////////////////////////////////////////////////////

void
TableIter::init(StDbConfigNode* node){
mnode=node; itr = mnode->mTables.begin();
}

////////////////////////////////////////////////////////

StDbTableI*
TableIter::next(){
StDbTableI* ret = 0;
if(!done()) { 
 ret = *itr;
 char* tableName = (*itr)->getTableName();
 // cout << "Table [" << tableName <<"] is available" << endl;
 delete [] tableName;
 itr++;
}
return ret;
}

////////////////////////////////////////////////////////


StDbTableI*
TableIter::operator++(){
return next();
}

////////////////////////////////////////////////////////

bool
TableIter::done(){
bool retVal = true;
if(itr != mnode->mTables.end())retVal = false;
return retVal;
}









