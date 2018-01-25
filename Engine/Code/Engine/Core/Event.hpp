#pragma once
#include <vector>

// An Event/Delegate at its core is just a list of function callbacks
//
// Events can be registered with some identifying information [user_arg].
// And calling the event can take some event specific data [event_arg]
typedef void(*basic_event_cb)(void *user_arg, void *event_arg);

class EventV0
{
	// subscription - when subscribing this is the identifying 
	// information (what to call, and what to call with)
	struct event_sub_t
	{
		basic_event_cb cb;
		void *user_arg;
	};

public:
	EventV0()
	{
	}

	~EventV0()
	{
		m_subscriptions.clear();
	}

	void subscribe(void *user_arg, basic_event_cb cb)
	{
		// Good safeguard in debug to add is to make sure
		// you're not double subscribing to an event
		// with a similar pair. 
		// TODO - ASSERT AGAINST ABOVE

		// Add subscriptoin
		event_sub_t sub;
		sub.cb = cb;
		sub.user_arg = user_arg;
		m_subscriptions.push_back(sub);
	}

	void unsubscribe(void *user_arg, basic_event_cb cb)
	{
		for (unsigned int i = 0; i < m_subscriptions.size(); ++i) {
			event_sub_t &sub = m_subscriptions[i];
			if ((sub.cb == cb) && (sub.user_arg == user_arg)) {
				m_subscriptions.erase(m_subscriptions.begin() + i);
				return; // should be unique, so return immeidately
			}
		}
	}

	void trigger(void *event_arg)
	{
		for (unsigned int i = 0; i < m_subscriptions.size(); ++i) {
			event_sub_t &sub = m_subscriptions[i];
			sub.cb(sub.user_arg, event_arg);
		}
	}

public:
	std::vector<event_sub_t> m_subscriptions;
};

//--------------------------------------------------------------------
// EVENT VERSION 1
// - Thread Safety around 
// - Add support for callbacks without user data
// - Adds supports for methods
//--------------------------------------------------------------------
// TODO

//--------------------------------------------------------------------
// EVENT VERSION 2
// - Templated argument types
//--------------------------------------------------------------------
// TODO

//--------------------------------------------------------------------
// EVENT VERSION 3
// - Thread Safety [ReaderWriter Locks]
// - 
//--------------------------------------------------------------------
// Thread Safe


