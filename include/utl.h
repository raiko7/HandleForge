#ifndef UTL_H_
#define UTL_H_
//
//    $VER: utl.h 1.0 (04.01.2018)
//
//    foundation utilities
//
//==============================================================================
//
#include <memory>

#include <cstdio>
#include <cstdlib>

#ifdef UTL_HAVE_DIRENT
#include <dirent.h>
#endif

#ifdef UTL_HAVE_RPC
#include <rpc/rpc.h>
#endif

/******************************************************************************/
namespace std {

   template <> struct default_delete<FILE> {
      void operator()(FILE *fp) { if (fp) fclose(fp); }
   };

#ifdef UTL_HAVE_DIRENT
   template <> struct default_delete<DIR> {
      void operator()(DIR *dp) { if (dp) closedir(dp); }
   };
#endif

#ifdef UTL_HAVE_RPC
   template <>	struct default_delete<CLIENT> {
		void operator()(CLIENT *cp) { if (cp) clnt_destroy(cp); }
	};
#endif
}

/******************************************************************************/
namespace utl {

	namespace detail {
		struct free_adapter {
			void operator()(void *p) { free(p); }
		};
	}

	// RAII adapters for leakable C resources
	using file_ptr   = std::unique_ptr<FILE>;
	using malloc_ptr = std::unique_ptr<void, detail::free_adapter>;

	// RAII adapters for leakable POSIX resources
#ifdef UTL_HAVE_DIRENT
	using dir_ptr = std::unique_ptr<DIR>;
#endif

#ifdef UTL_HAVE_RPC
	using client_ptr = std::unique_ptr<CLIENT>;
#endif
}

#endif //UTL_H_
