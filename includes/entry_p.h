/*
 * Copyright 2011-2021 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#ifndef ENTRY_PRIVATE_H_HEADER_GUARD
#define ENTRY_PRIVATE_H_HEADER_GUARD

#define TINYSTL_ALLOCATOR entry::TinyStlAllocator

#include <bx/spscqueue.h>

#include "entry.h"

#ifndef ENTRY_CONFIG_MAX_WINDOWS
#	define ENTRY_CONFIG_MAX_WINDOWS 8
#endif // ENTRY_CONFIG_MAX_WINDOWS

#if !defined(ENTRY_DEFAULT_WIDTH) && !defined(ENTRY_DEFAULT_HEIGHT)
#	define ENTRY_DEFAULT_WIDTH  1280
#	define ENTRY_DEFAULT_HEIGHT 720
#elif !defined(ENTRY_DEFAULT_WIDTH) || !defined(ENTRY_DEFAULT_HEIGHT)
#	error "Both ENTRY_DEFAULT_WIDTH and ENTRY_DEFAULT_HEIGHT must be defined."
#endif // ENTRY_DEFAULT_WIDTH

#ifndef ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR
#	define ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR 1
#endif // ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR

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
			// Suspend,
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

		void* m_nwh;
	};

	// struct SuspendEvent : public Event
	// {
	// 	ENTRY_IMPLEMENT_EVENT(SuspendEvent, Event::Suspend);

	// 	Suspend::Enum m_state;
	// };


	const Event* poll();
	const Event* poll(WindowHandle _handle);
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

		void postWindowEvent(WindowHandle _handle, void* _nwh = NULL)
		{
			WindowEvent* ev = BX_NEW(getAllocator(), WindowEvent)(_handle);
			ev->m_nwh = _nwh;
			m_queue.push(ev);
		}

		// void postSuspendEvent(WindowHandle _handle, Suspend::Enum _state)
		// {
		// 	SuspendEvent* ev = BX_NEW(getAllocator(), SuspendEvent)(_handle);
		// 	ev->m_state = _state;
		// 	m_queue.push(ev);
		// }


		const Event* poll()
		{
			return m_queue.pop();
		}

		const Event* poll(WindowHandle _handle)
		{
			if (isValid(_handle) )
			{
				Event* ev = m_queue.peek();
				if (NULL == ev
				||  ev->m_handle.idx != _handle.idx)
				{
					return NULL;
				}
			}

			return poll();
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
