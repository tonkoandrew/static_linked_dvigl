/*
 * Copyright 2011-2021 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#ifndef ENTRY_H_HEADER_GUARD
#define ENTRY_H_HEADER_GUARD

#include <bx/bx.h>

namespace bx { struct AllocatorI; }

extern "C" int _main_();

#define ENTRY_WINDOW_FLAG_NONE         UINT32_C(0x00000000)
#define ENTRY_WINDOW_FLAG_ASPECT_RATIO UINT32_C(0x00000001)
#define ENTRY_WINDOW_FLAG_FRAME        UINT32_C(0x00000002)


namespace entry
{
	struct WindowHandle  { uint16_t idx; };
	inline bool isValid(WindowHandle _handle)  { return UINT16_MAX != _handle.idx; }

	struct Suspend
	{
		enum Enum
		{
			WillSuspend,
			DidSuspend,
			WillResume,
			DidResume,

			Count
		};
	};

	bool processEvents(uint32_t& _width, uint32_t& _height, uint32_t& _debug, uint32_t& _reset);

	bx::AllocatorI*  getAllocator();

	WindowHandle createWindow(int32_t _x, int32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags = ENTRY_WINDOW_FLAG_NONE, const char* _title = "");
	void destroyWindow(WindowHandle _handle);
	void setWindowSize(WindowHandle _handle, uint32_t _width, uint32_t _height);
	void setWindowTitle(WindowHandle _handle, const char* _title);
	void setWindowFlags(WindowHandle _handle, uint32_t _flags, bool _enabled);

	struct WindowState
	{
		WindowState()
			: m_width(0)
			, m_height(0)
			, m_nwh(NULL)
		{
			m_handle.idx = UINT16_MAX;
		}

		WindowHandle m_handle;
		uint32_t     m_width;
		uint32_t     m_height;
		void*        m_nwh;
	};

	bool processWindowEvents(WindowState& _state, uint32_t& _debug, uint32_t& _reset);

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
