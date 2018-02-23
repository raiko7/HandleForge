#ifndef HANDLE_FORGE_H_
#define HANDLE_FORGE_H_
//
//    $VER: HandleForge.h 1.0 (22.02.2018)
//
//    mapping of pointer resources to checked handles
//
//    (C) Copyright 2018 by Rainer Koppler
//
//==============================================================================
//
#include <memory>
#include <cstdint>
#include <ctime>
#include <typeinfo>

/******************************************************************************/
template <typename T, int size>
class HandleForge {
public:
   using HndType = uint32_t;   // handle type
   using ObjType = T;          // resource type

   HandleForge(void);
   void Reset(void);

   HndType Add(std::unique_ptr<T> p);
   bool Remove(HndType h);

   T *Get(HndType h);

private:
   static_assert(size > 0 && size <= 256, "unsupported table size");

   struct Entry {
      HndType            state;
      std::unique_ptr<T> ptr;
   };
   Entry mEntries[size];

   static const HndType cStateMask = 0xFFFFFF;

   HndType MakeHandle(Entry *p) {
      return ((p->state & cStateMask) << 8) | (p - mEntries);
   }

   bool IsValidHandle(HndType h) {
      return (mEntries[h % size].state & cStateMask) == ((h >> 8) & cStateMask);
   }
};

/******************************************************************************/
/**  initializes empty table
*/
template <typename T, int size>
HandleForge<T, size>::HandleForge(void) {

   // uniqueness/randomness of handles depends on initial state
   mEntries[0].state = static_cast<HndType>((time(nullptr) + typeid(this).hash_code()) << 1);

   for (int i = 1; i != size; ++i) {
      mEntries[i].state = mEntries[i - 1].state + (2 * i);
   }
}

/******************************************************************************/
/**  resets table to empty state, deletes resources
*/
template <typename T, int size>
void HandleForge<T, size>::Reset(void) {

   for (auto &e : mEntries) {
      if (e.state % 2 == 1) {
         e.ptr.reset();
         ++e.state;
      }
   }
}

/******************************************************************************/
/**  adds resource to table and returns handle
*
*  @param[in] p   pointer resource, moved on success, unchanged on failure
*
*  @return handle for checked resource access, 0 on failure
*/
template <typename T, int size>
typename HandleForge<T, size>::HndType HandleForge<T, size>::Add(std::unique_ptr<T> p) {

   for (auto &e : mEntries) {
      if (e.state % 2 == 0) {
         ++e.state;
         e.ptr = std::move(p);
         return MakeHandle(&e);
      }
   }
   return 0;   // table full
}

/******************************************************************************/
/**  removes resource from table, deletes resource
*
*  @param[in] h   resource handle
*
*  @retval true   resource removed and deleted
*  @retval false  invalid handle
*/
template <typename T, int size>
bool HandleForge<T, size>::Remove(HndType h) {

   if (!IsValidHandle(h)) {
      return false;   // invalid handle
   }
   Entry &e = mEntries[h % size];
   e.ptr.reset();
   ++e.state;
   return true;
}

/******************************************************************************/
/**  maps resource handle back to pointer
*
*  @param[in] h   resource handle
*
*  @return resource pointer on success, nullptr if invalid handle
*/
template <typename T, int size>
T *HandleForge<T, size>::Get(HndType h) {

   if (!IsValidHandle(h)) {
      return nullptr;   // invalid handle
   }
   return mEntries[h % size].ptr.get();
}

#endif //HANDLE_FORGE_H_
