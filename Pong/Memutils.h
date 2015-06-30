namespace Memory
{
	template <class pClass> static inline void SafeRelease(pClass **ppClass)
	{
		if (*ppClass)
		{
			(*ppClass)->Release();
			*ppClass = nullptr;
		}
	}
}
