//////////////////////////////////////////////////////////////////////
// This file is part of Remere's Map Editor
// (customizations by @dtroitskiy)
//////////////////////////////////////////////////////////////////////

#ifndef RME_MEDIATOR_H
#define RME_MEDIATOR_H

#include <vector>
#include <map>

using namespace std;

typedef vector <unsigned int> EventIDs;

class IEventReceiver
{
public:
	virtual void onEvent(unsigned int eventID, void * eventData) = 0;
};

class Mediator
{
public:
	static void subscribeEvent(unsigned int eventID, IEventReceiver * receiver);
	static void subscribeEvents(const vector <unsigned int> eventIDs, IEventReceiver * receiver);
	static void publishEvent(unsigned int eventID, void * eventData = nullptr, bool yieldThread = false);
	static void unsubscribeAllEvents(IEventReceiver * receiver);

private:
	static map <unsigned int, vector <IEventReceiver *>> subscribers;
};

#endif // RME_MEDIATOR_H
