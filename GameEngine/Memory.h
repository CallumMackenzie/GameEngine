#pragma once

#include "ModWin.h"

namespace memory {
	template<typename T> 
	static inline void safe_delete(T*& a) {
		delete a;
		a = NULL;
	}
}