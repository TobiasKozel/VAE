#ifndef TKLB_ASSERT
#ifdef TKLB_NO_ASSERT
	#define TKLB_ASSERT(condition);
	#define TKLB_ASSERT_STATE(condition);
#else
	#include <cassert>
	#define TKLB_ASSERT(condition) assert((condition));

	/**
	 * @brief This can be used to write some additional code
	 * only needed when doing assertions
	 */
	#define TKLB_ASSERT_STATE(condition) condition;

#endif // TKLB_NO_ASSERT
#endif // TKLB_ASSERT
