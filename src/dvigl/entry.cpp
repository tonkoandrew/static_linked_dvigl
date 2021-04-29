#include <bgfx/bgfx.h>

#include <time.h>
#include "entry_p.h"


int32_t _main_();

namespace entry
{
	static uint32_t s_reset = BGFX_RESET_NONE;
	static uint32_t s_width = ENTRY_DEFAULT_WIDTH;
	static uint32_t s_height = ENTRY_DEFAULT_HEIGHT;

	extern bx::AllocatorI* getDefaultAllocator();
	bx::AllocatorI* g_allocator = getDefaultAllocator();

	bx::AllocatorI* getDefaultAllocator()
	{
BX_PRAGMA_DIAGNOSTIC_PUSH();
BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459); // warning C4459: declaration of 's_allocator' hides global declaration
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
		static bx::DefaultAllocator s_allocator;
		return &s_allocator;
BX_PRAGMA_DIAGNOSTIC_POP();
	}
 
	AppI::AppI()
	{
	}

	AppI::~AppI()
	{
	}

	int runApp(AppI* _app)
	{
		_app->init(s_width, s_height);
		bgfx::frame();

		while (_app->update() )
		{
		}
		return _app->shutdown();
	}

	int main()
	{
		return _main_();
	}

	WindowState s_window;

	bool processEvents(uint32_t& _width, uint32_t& _height, uint32_t& _reset)
	{
		bool needReset = s_reset != _reset;

		s_reset = _reset;

		WindowHandle handle = { UINT16_MAX };

		const Event* ev;
		do
		{
			struct SE { const Event* m_ev; SE() : m_ev(poll() ) {} ~SE() { if (NULL != m_ev) { release(m_ev); } } } scopeEvent;
			ev = scopeEvent.m_ev;

			if (NULL != ev)
			{
				switch (ev->m_type)
				{
				case Event::Exit:
					return true;

				case Event::Size:
					{
						const SizeEvent* size = static_cast<const SizeEvent*>(ev);
						WindowState& win = s_window;
						win.m_handle = size->m_handle;
						win.m_width  = size->m_width;
						win.m_height = size->m_height;

						handle  = size->m_handle;
						_width  = size->m_width;
						_height = size->m_height;

						needReset = true;
					}
					break;

				default:
					break;
				}
			}
		} while (NULL != ev);

		needReset |= _reset != s_reset;

		if (handle.idx == 0
		&&  needReset)
		{
			_reset = s_reset;
			bgfx::reset(_width, _height, _reset);
		}

		s_width = _width;
		s_height = _height;

		return false;
	}

	bx::AllocatorI* getAllocator()
	{
		if (NULL == g_allocator)
		{
			g_allocator = getDefaultAllocator();
		}

		return g_allocator;
	}

	void* TinyStlAllocator::static_allocate(size_t _bytes)
	{
		return BX_ALLOC(getAllocator(), _bytes);
	}

	void TinyStlAllocator::static_deallocate(void* _ptr, size_t /*_bytes*/)
	{
		if (NULL != _ptr)
		{
			BX_FREE(getAllocator(), _ptr);
		}
	}

} // namespace entry
