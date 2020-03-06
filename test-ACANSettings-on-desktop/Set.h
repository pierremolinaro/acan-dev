#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <vector>

//----------------------------------------------------------------------------------------------------------------------

class Set {
//--- Default constructor
  public: Set (const uint32_t inMaxValue) ;

//--- Destructor
  public: ~Set (void) ;

//--- Accessors
  public : std::vector <uint32_t> values (void) const ;

//--- Set operations
  public : bool insert (const uint32_t inItem) ;

//--- Private properties
  private: size_t mMaxValue ;
  private: uint64_t * mArray ;

//--- No copy
  private: Set (const Set &inOperand) ;
  private: Set & operator = (const Set & inOperand) ;
};

//----------------------------------------------------------------------------------------------------------------------
