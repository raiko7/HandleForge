//
//    $VER: testHandleForge.cpp
//
//    HandleForge test applications
//
//==============================================================================
//
#include "HandleForge.h"
#include "utl.h"
#include <cassert>

/******************************************************************************/
int main(int argc, char *argv[]) {

   //--- test package 1 --------------------------------------------------------

   HandleForge<double, 1> tab1;

   auto pd1 = std::make_unique<double>(1.1);
   auto pd2 = std::make_unique<double>(2.2);

   auto h11 = tab1.Add(std::move(pd1));   // named pointers, must be moved
   auto h12 = tab1.Add(std::move(pd2));

   assert(h11 != 0);   // success
   assert(h12 == 0);   // failure (table full)

   assert(tab1.Get(h11) != nullptr);   // valid handle maps to pointer
   assert(tab1.Get(h12) == nullptr);   // invalid handle does not map to anything

   //--- test package 2 --------------------------------------------------------

   HandleForge<unsigned, 1> tab2;

   auto h21 = tab2.Add(std::make_unique<unsigned>(2017));   // unnamed pointer
   assert(h21 != 0);

   assert(*tab2.Get(h21) == 2017);

   //--- test package 3 --------------------------------------------------------

   if (argc < 2) {
      return 1;
   }

   HandleForge<FILE, 1> fTab;

   auto fPtr1 = utl::file_ptr(fopen(argv[1], "r"));
   if (!fPtr1) {
      return 1;
   }

   auto fPtr2 = utl::file_ptr(fopen(argv[1], "r"));
   if (!fPtr2) {
      return 1;
   }

   auto fHnd1 = fTab.Add(std::move(fPtr1));
   auto fHnd2 = fTab.Add(std::move(fPtr2));
   assert(fHnd1 != 0);   // success
   assert(fHnd2 == 0);   // failure (table full)

   auto ok = fTab.Remove(fHnd1);
   assert(ok);   // remove succeeded

   ok = fTab.Remove(fHnd1);
   assert(!ok);   // double-remove failed

   //---------------------------------------------------------------------------

   assert(h11 != fHnd1);

   return (h11 + h12 + h21 + fHnd1 + fHnd2) % 2018;
}
