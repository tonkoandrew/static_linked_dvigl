/*
 * Copyright 2011-2021 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#ifndef ENTRY_H_HEADER_GUARD
#define ENTRY_H_HEADER_GUARD

#include <bx/bx.h>

namespace bx { struct AllocatorI; }

int _main_();


namespace entry
{
	struct WindowHandle  { uint16_t idx; };
	inline bool isValid(WindowHandle _handle)  { return UINT16_MAX != _handle.idx; }

	bool processEvents(uint32_t& _width, uint32_t& _height, uint32_t& _debug, uint32_t& _reset);

	bx::AllocatorI*  getAllocator();

	struct WindowState
	{
		WindowState()
			: m_width(0)
			, m_height(0)
		{
			m_handle.idx = UINT16_MAX;
		}

		WindowHandle m_handle;
		uint32_t     m_width;
		uint32_t     m_height;
	};

	class BX_NO_VTABLE AppI
	{
	public:
		///
		AppI();

		///
		virtual ~AppI() = 0;

		///
		virtual void init(uint32_t _width, uint32_t _height) = 0;

		///
		virtual int  shutdown() = 0;

		///
		virtual bool update() = 0;
	};

	///
	int runApp(AppI* _app);

} // namespace entry

#endif // ENTRY_H_HEADER_GUARD
