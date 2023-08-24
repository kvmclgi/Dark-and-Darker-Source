extern "C" { inline PVOID legitTrampoline, legitTrampoline2; void _spoofer_stub(); }

template<typename Ret = void*, typename a1t = void*, typename a2t = void*, typename a3t = void*, typename a4t = void*, typename a5t = void*, typename... Stack>
Ret SpoofCall(PVOID Func, a1t a1 = a1t{}, a2t a2 = a2t{}, a3t a3 = a3t{}, a4t a4 = a4t{}, a5t a5 = a5t{}, Stack... args)
{
	struct callStruct {
		PVOID Fn;
		a5t Arg;

		callStruct(PVOID fn, a5t arg) : Fn(fn), Arg(arg) {}
	} CallCtx(Func, a5);
	typedef Ret(*ShellFn)(a1t, a2t, a3t, a4t, callStruct*, Stack...);
	return ((ShellFn)_spoofer_stub)(a1, a2, a3, a4, &CallCtx, args...);
}
#define SpoofCaller SpoofCall
template<typename Ret = void, typename... Args>
__forceinline Ret VCall(PVOID Class, int Index, Args... AddArgs) {
	return SpoofCaller<Ret>((*(PVOID**)Class)[Index], Class, AddArgs...);
}
#define VCall VCall