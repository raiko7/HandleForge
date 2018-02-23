[![Build Status](https://travis-ci.org/raiko7/HandleForge.svg?branch=master)](https://travis-ci.org/raiko7/HandleForge)

# Overview
*HandleForge* is a minimal header-only C++ implementation of a handle table. 
Handle tables free programmers from the drawbacks of using pointers as handles 
in public C/C++ APIs. Robust implementations cannot trust the API client to pass 
valid pointers, therefore some kind of a mapping layer is required that allows 
to check client handles before they can be resolved to pointers in a safe way. 
Such a mapping layer can be provided by a handle table.

The *HandleForge* class template is instantiated with
* the type of the pointer resource to be stored in the table
* the table size

For simplicity reasons the table size is fixed and must be lower or equal 256 
(see *Internals* below).

The *Add* operation creates an association between a pointer and a handle by 
storing the pointer in a table slot and creating a unique slot handle. The original 
pointer can be retrieved by calling the *Get* operation with the corresponding 
handle. The *Remove* operation removes a pointer-handle association and deletes the 
pointer resource using standard deletion.

Prior to destruction, a table can be reset to empty state using the 
*Reset* operation, for example, on behalf of a special API function.


# Internals
Handles are uint32_t values, encoded according to the following scheme:

| Bits    | Information                                              |
|---------|----------------------------------------------------------|
| 0 to 7  | table slot index (0-255)                                 |
| 8       | table slot allocation state (0=free, 1=allocated)        |
| 9 to 31 | table slot allocation generation (incremented on *Remove*) |

The special handle value 0 is used to indicate failure of an *Add* call. A 
successful call to *Add* can never return 0 since Bit 8 is always set in valid 
handles. So even if slot 0 is chosen and the slot's allocation generation is 0 
the handle will be not 0.

The same handle value does not occur until 2^24 deallocations of the same table 
slot.

In order to promote randomness and uniqueness of handles across tables a 
combination of current time and table type information is used (see 
constructor).


# Author
*HandleForge* was written by Rainer Koppler.
