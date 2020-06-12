#ifndef _LANGUAGE_SO_
#define _LANGUAGE_SO_

#include "Common.h"
#include "Response.h"

typedef void (*LangDispatchFunc)(Response);

void portuguese_language(LangDispatchFunc *dispatchers);

void english_language(LangDispatchFunc *dispatchers);

void german_language(LangDispatchFunc *dispatchers);

void french_language(LangDispatchFunc *dispatchers);

#endif
