//*-- Author :    Valery Fine   26/01/99  (E-mail: fine@bnl.gov)
// 
//  

//*-- Author :    Valery Fine   26/01/99  (E-mail: fine@bnl.gov)
// 
// 
#ifndef STAR_St_TableSorter
#define STAR_St_TableSorter

#include "TNamed.h"
// #include "TString.h"

////////////////////////////////////////////////////////////////////////////////////////
//
//  St_TableSorter  - Is an "observer" class to sort the St_Table objects
//                    The class provides an interface to the standard "C/C++"
//
// qsort and bsearch subroutine (for further information see your local C/C++ docs)
// =====     =======
//
//  - This class DOESN'T change / touch the "host" table  itself
//    For any St_Table object one can create as many different "sorter"
//    as he/she finds useful for his/her code
//  - Any instance of this class is meaningful as long as the "host" object
//    "St_Table" does exist and is not changed
//  - Any attempt to access this St_TableSorter after the "host" object deleted
//    causes the program abnormal termination
//  - Any attempt to access this St_TableSorter after the "host" object been changed
//    causes an unpredictable result
//  - Any instance (object) of this class is NOT deleted "by automatic" just
//    the "host object "St_Table" deleted. It is the responsibility of the user's code
//    keeping St_TableSorter and the the "host" St_Table objects consistent.
//
////////////////////////////////////////////////////////////////////////////////////////

class St_Table;
class St_TableSorter : public TNamed {
 protected:
    typedef Int_t (*COMPAREMETHOD)(const void **, const void **);
    typedef Int_t (*SEARCHMETHOD)(const void *, const void **);
    typedef Int_t (*CALLQSORT)(const void *, const void *);
    enum EColumnType {kNAN, kFloat, kInt, kLong, kShort, kDouble, kUInt
                           ,kULong, kUShort, kUChar, kChar };
    void    **m_SortIndex;    // Array of pointers to columns of the sorted table
    Int_t     m_firstRow;     // first row of the table to be sorted
    Int_t     m_numberOfRows; // number of rows of ttable to be sorted
    TString   m_colName;      //
    Int_t     m_colOffset;    //
    Int_t     m_colSize;      // The size of the selected columen in bytes
    Int_t    *m_IndexArray;   // "parsed" indecis
    Int_t     m_colDimensions;// The number of the dimensions for array
    const St_Table &m_ParentTable;  // the back pointer to the sorted table
    SEARCHMETHOD m_searchMethod; // Function selected to serach values
    EColumnType m_colType;   // data type of the selected column

    Int_t  BSearch(const void *value);
    void   FillIndexArray();
    void   SortArray();
    void   LearnTable();
    Int_t  LFind(const void *){ return -1;}

 public:
    St_TableSorter();
    St_TableSorter(const St_Table &table, TString &colName, Int_t firstRow=0,Int_t numbeRows=0);
    virtual ~St_TableSorter();
    
    Int_t BSearch(Float_t value);
    Int_t BSearch(Int_t value);
    Int_t BSearch(Double_t value);
    Int_t BSearch(const Char_t *value);
    Int_t BSearch(TString &value);
 
    static int CompareFloat_t     (const void **, const void **);
    static int CompareInt_t       (const void **, const void **);
    static int CompareLong_t      (const void **, const void **);
    static int CompareULong_t     (const void **, const void **);
    static int CompareUInt_t      (const void **, const void **);
    static int CompareShort_t     (const void **, const void **);
    static int CompareDouble_t    (const void **, const void **);
    static int CompareUShort_t    (const void **, const void **);
    static int CompareUChar_t     (const void **, const void **);
    static int CompareChar_t      (const void **, const void **);

    virtual       Int_t   GetIndex(UInt_t index) const;
    virtual const Text_t *GetColumnName() const { return m_colName.Data();}
    virtual const Text_t *GetTableName()  const;
    virtual const Text_t *GetTableTitle() const;
    virtual const Text_t *GetTableType() const;
    virtual       Int_t   GetNRows()      const { return m_numberOfRows;}
    virtual       Int_t   GetFirstRow()   const { return m_firstRow;}

    static int SearchFloat_t     (const void *, const void **);
    static int SearchInt_t       (const void *, const void **);
    static int SearchULong_t     (const void *, const void **);
    static int SearchLong_t      (const void *, const void **);
    static int SearchUInt_t      (const void *, const void **);
    static int SearchShort_t     (const void *, const void **);
    static int SearchDouble_t    (const void *, const void **);
    static int SearchUShort_t    (const void *, const void **);
    static int SearchUChar_t     (const void *, const void **);
    static int SearchChar_t      (const void *, const void **);

    Int_t  LFind(Float_t value){ return LFind(&value);}
    Int_t  LFind(Int_t value){ return LFind(&value);}
    Int_t  LFind(Double_t value){ return LFind(&value);}
    Int_t  LFind(const Char_t *value){ return LFind(&value);}
    Int_t  LFind(TString &value){ return LFind(&value);}

    Int_t operator[](Float_t value) { return BSearch(value); }
    Int_t operator[](Int_t value)   { return BSearch(value); }
    Int_t operator[](Double_t value){ return BSearch(value); } 
    Int_t operator[](const Char_t *value) { return BSearch(value); }
    Int_t operator[](TString &value) { return BSearch(value); }

    Int_t operator()(Float_t value)  { return LFind(value); }
    Int_t operator()(Int_t value)    { return LFind(value); }
    Int_t operator()(Double_t value) { return LFind(value); }
    Int_t operator()(const Char_t *value) { return LFind(value); }
    Int_t operator()(TString &value) { return LFind(value); }

    ClassDef(St_TableSorter,0)
};
#endif
