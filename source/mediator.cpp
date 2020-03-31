#include "main.h"
#include "mediator.h"

map <unsigned int, vector <IEventReceiver *>> Mediator::subscribers;

void Mediator::subscribeEvent(unsigned int eventID, IEventReceiver * receiver)
{
	subscribers[eventID].push_back(receiver);
}

void Mediator::subscribeEvents(const vector <unsigned int> eventIDs, IEventReceiver * receiver)
{
	for (auto & e : eventIDs)
	{
		subscribers[e].push_back(receiver);
	}
}

void Mediator::publishEvent(unsigned int eventID, void * eventData, bool yieldThread)
{
	if (subscribers.find(eventID) == subscribers.end()) return;
	for (auto & v : subscribers[eventID])
	{
		v->onEvent(eventID, eventData);
	}
	if (yieldThread)
	{
		wxTheApp->Yield();
	}
}

void Mediator::unsubscribeAllEvents(IEventReceiver * receiver)
{
	for (auto & kv : subscribers)
	{
		auto & subs = kv.second;
		auto it = find(subs.begin(), subs.end(), receiver);
		if (it != subs.end())
		{
			subs.erase(it);
		}
	}
}
