#include <bgfx/bgfx.h>

#include <time.h>
#include "entry_p.h"


extern "C" int32_t _main_();

namespace entry
{
	static uint32_t s_debug = BGFX_DEBUG_NONE;
	static uint32_t s_reset = BGFX_RESET_NONE;
	static uint32_t s_width = ENTRY_DEFAULT_WIDTH;
	static uint32_t s_height = ENTRY_DEFAULT_HEIGHT;
	static bool s_exit = false;

	extern bx::AllocatorI* getDefaultAllocator();
	bx::AllocatorI* g_allocator = getDefaultAllocator();

#if ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR
	bx::AllocatorI* getDefaultAllocator()
	{
BX_PRAGMA_DIAGNOSTIC_PUSH();
BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459); // warning C4459: declaration of 's_allocator' hides global declaration
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
		static bx::DefaultAllocator s_allocator;
		return &s_allocator;
BX_PRAGMA_DIAGNOSTIC_POP();
	}
#endif // ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR
 
	static AppI*    s_currentApp = NULL;
	static AppI*    s_apps       = NULL;
	static uint32_t s_numApps    = 0;

	static AppI* getCurrentApp(AppI* _set = NULL)
	{
		if (NULL != _set)
		{
			s_currentApp = _set;
		}
		else if (NULL == s_currentApp)
		{
			s_currentApp = getFirstApp();
		}

		return s_currentApp;
	}

	static AppI* getNextWrap(AppI* _app)
	{
		AppI* next = _app->getNext();
		if (NULL != next)
		{
			return next;
		}

		return getFirstApp();
	}

	AppI::AppI()
	{
		m_name        = "_name";
		m_description = "_description";
		m_url         = "_url";
		m_next        = s_apps;

		s_apps = this;
		s_numApps++;
	}

	AppI::~AppI()
	{
		for (AppI* prev = NULL, *app = s_apps, *next = app->getNext()
			; NULL != app
			; prev = app, app = next, next = app->getNext() )
		{
			if (app == this)
			{
				if (NULL != prev)
				{
					prev->m_next = next;
				}
				else
				{
					s_apps = next;
				}

				--s_numApps;

				break;
			}
		}
	}

	const char* AppI::getName() const
	{
		return m_name;
	}

	const char* AppI::getDescription() const
	{
		return m_description;
	}

	const char* AppI::getUrl() const
	{
		return m_url;
	}

	AppI* AppI::getNext()
	{
		return m_next;
	}

	AppI* getFirstApp()
	{
		return s_apps;
	}

	uint32_t getNumApps()
	{
		return s_numApps;
	}

	int runApp(AppI* _app)
	{
		_app->init(s_width, s_height);
		bgfx::frame();

		WindowHandle defaultWindow = { 0 };
		setWindowSize(defaultWindow, s_width, s_height);
		while (_app->update() )
		{
		}
		return _app->shutdown();
	}

	int main()
	{
		entry::WindowHandle defaultWindow = { 0 };

		entry::setWindowTitle(defaultWindow, "BGFX -> SDL Window");
		setWindowSize(defaultWindow, ENTRY_DEFAULT_WIDTH, ENTRY_DEFAULT_HEIGHT);

		int32_t result = bx::kExitSuccess;
		result = ::_main_();

		return result;
	}

	WindowState s_window[ENTRY_CONFIG_MAX_WINDOWS];

	bool processEvents(uint32_t& _width, uint32_t& _height, uint32_t& _debug, uint32_t& _reset)
	{
		bool needReset = s_reset != _reset;

		s_debug = _debug;
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
						WindowState& win = s_window[0];
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

			// inputProcess();

		} while (NULL != ev);

		needReset |= _reset != s_reset;

		if (handle.idx == 0
		&&  needReset)
		{
			_reset = s_reset;
			bgfx::reset(_width, _height, _reset);
		}

		_debug = s_debug;

		s_width = _width;
		s_height = _height;

		return s_exit;
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
