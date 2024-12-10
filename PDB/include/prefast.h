#ifndef PREFAST_H
#define PREFAST_H

#ifdef _MSC_VER
#define PREFAST_SUPPRESS(NM, MSG) __pragma( disable: NM)
#else
#define PREFAST_SUPPRESS(NM, MSG)
#endif

#endif
