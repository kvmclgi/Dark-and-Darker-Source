#pragma once
inline constexpr DWORD64 _s1 = __TIME__[6] - 48;
inline constexpr DWORD64 _s2 = __TIME__[7] - 48;
inline constexpr DWORD64 _h1 = __TIME__[0] - 48;
inline constexpr DWORD64 _h2 = __TIME__[1] - 48;
inline constexpr DWORD64 _m1 = __TIME__[3] - 48;
inline constexpr DWORD64 _m2 = __TIME__[4] - 48;
inline constexpr DWORD64 __tvtodw(DWORD64 high, DWORD64 low) { return high * 10 + low; }
inline constexpr DWORD64 _HOURS = __tvtodw(_h1, _h2) + 1;
inline constexpr DWORD64 _MINUTES = __tvtodw(_m1, _m2);
inline constexpr DWORD64 _SECONDS = __tvtodw(_s1, _s2);
inline constexpr DWORD64 _TIMEVAL = (_SECONDS + (_MINUTES * 60) + (_HOURS * 60 * 60));
inline bool tv5 = _HOURS % 5 == 5, tv2 = _MINUTES % 2 == 2, tv1 = _SECONDS % 2 == 1;
inline __int64 r0 = _HOURS + _HOURS / 2, r2 = _MINUTES * (_MINUTES - _HOURS), r1 = (_TIMEVAL ^ _h2) / _m1;

__forceinline void RandCode0()
{
	if (r0 > r2)
	{
		r2 ^= r0;
		r2 - r0;
	}

	r1 -= r2;
	if (r1 < r2)
		r1--;
}
#define rCode0 (RandCode0())

__forceinline void RandCode1()
{
	r1++;
	if (tv1 || tv2)
		r1 += r1 ^ r2;
}
#define rCode1 (RandCode1())

inline void RandFunction() {}
#define rFunc (RandFunction())

inline int s1key = 0xe1, s2key = 0x5b;
inline __declspec(noinline) static DWORD64 __UnhideDword0(volatile DWORD64 _param_value)
{
_catch:
	volatile DWORD64 x = _param_value;
	if (!x)
	{
		x += _s2 + _s1 + _m2 + _m1;
		goto _catch;
	}

	volatile DWORD64 part1 = x - _TIMEVAL;
	volatile DWORD64 part2 = part1 - (_s1 + s1key);
	volatile DWORD64 part3 = part2 / (_s2 + s2key);
	volatile DWORD64 partEnd = part3 - (_TIMEVAL * (_s2 + s2key));

	return partEnd;
}

inline  void __fastcall fz0() {}
#define __HiddenDword0(x) ((_s1 + s1key) + ((x + (_TIMEVAL * (_s2 + s2key))) * (_s2 + s2key)) + _TIMEVAL)
#define xor_value_0(x) (__UnhideDword0(__HiddenDword0(x)))

inline  __declspec(noinline) static DWORD64 __UnhideDword1(volatile DWORD64 _param_value)
{
	volatile DWORD64 x = _param_value ^ _SECONDS;
	volatile DWORD64 part1 = _SECONDS;
	volatile DWORD64 part2 = (_MINUTES ^ _m2) * _s2;
	volatile DWORD64 part3 = (_s1 * _MINUTES / _HOURS);
	volatile DWORD64 partEnd1 = (part1 + part2 + 1);
	volatile DWORD64 partEnd2 = x - part3;
	volatile DWORD64 partEnd = partEnd2 / partEnd1;
	return partEnd;
}

#define __HiddenDword1(x) (((x * _SECONDS) + (x * (_MINUTES ^ _m2) * _s2) + (_s1 * _MINUTES / _HOURS) + x) ^ _SECONDS)
#define xor_value_1(x) (__UnhideDword1(__HiddenDword1(x)))


