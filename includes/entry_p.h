/*
 * Copyright 2011-2021 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#ifndef ENTRY_PRIVATE_H_HEADER_GUARD
#define ENTRY_PRIVATE_H_HEADER_GUARD

#define TINYSTL_ALLOCATOR entry::TinyStlAllocator

#include <bx/spscqueue.h>


#include <bx/bx.h>

namespace bx { struct AllocatorI; }

int _main_();


namespace entry
{
	struct WindowHandle  { uint16_t idx; };
	inline bool isValid(WindowHandle _handle)  { return UINT16_MAX != _handle.idx; }

	bool processEvents(uint32_t& _width, uint32_t& _height, uint32_t& _reset);

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

#ifndef ENTRY_CONFIG_MAX_WINDOWS
#	define ENTRY_CONFIG_MAX_WINDOWS 1
#endif // ENTRY_CONFIG_MAX_WINDOWS

#if !defined(ENTRY_DEFAULT_WIDTH) && !defined(ENTRY_DEFAULT_HEIGHT)
#	define ENTRY_DEFAULT_WIDTH  1280
#	define ENTRY_DEFAULT_HEIGHT 720
#elif !defined(ENTRY_DEFAULT_WIDTH) || !defined(ENTRY_DEFAULT_HEIGHT)
#	error "Both ENTRY_DEFAULT_WIDTH and ENTRY_DEFAULT_HEIGHT must be defined."
#endif // ENTRY_DEFAULT_WIDTH

#define ENTRY_IMPLEMENT_EVENT(_class, _type) \
			_class(WindowHandle _handle) : Event(_type, _handle) {}

namespace entry
{
	struct TinyStlAllocator
	{
		static void* static_allocate(size_t _bytes);
		static void static_deallocate(void* _ptr, size_t /*_bytes*/);
	};

	int main();

	struct Event
	{
		enum Enum
		{
			Exit,
			Size,
			Window,
		};

		Event(Enum _type)
			: m_type(_type)
		{
			m_handle.idx = UINT16_MAX;
		}

		Event(Enum _type, WindowHandle _handle)
			: m_type(_type)
			, m_handle(_handle)
		{
		}

		Event::Enum m_type;
		WindowHandle m_handle;
	};

	struct SizeEvent : public Event
	{
		ENTRY_IMPLEMENT_EVENT(SizeEvent, Event::Size);

		uint32_t m_width;
		uint32_t m_height;
	};

	struct WindowEvent : public Event
	{
		ENTRY_IMPLEMENT_EVENT(WindowEvent, Event::Window);
	};

	const Event* poll();
	void release(const Event* _event);

	class EventQueue
	{
	public:
		EventQueue()
			: m_queue(getAllocator() )
		{
		}

		~EventQueue()
		{
			for (const Event* ev = poll(); NULL != ev; ev = poll() )
			{
				release(ev);
			}
		}
		void postExitEvent()
		{
			Event* ev = BX_NEW(getAllocator(), Event)(Event::Exit);
			m_queue.push(ev);
		}

		void postSizeEvent(WindowHandle _handle, uint32_t _width, uint32_t _height)
		{
			SizeEvent* ev = BX_NEW(getAllocator(), SizeEvent)(_handle);
			ev->m_width  = _width;
			ev->m_height = _height;
			m_queue.push(ev);
		}

		const Event* poll()
		{
			return m_queue.pop();
		}

		void release(const Event* _event) const
		{
			BX_DELETE(getAllocator(), const_cast<Event*>(_event) );
		}

	private:
		bx::SpScUnboundedQueueT<Event> m_queue;
	};

} // namespace entry

#endif // ENTRY_PRIVATE_H_HEADER_GUARD
