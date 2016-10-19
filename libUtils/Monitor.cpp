#include <iostream>
#include "Monitor.h"

namespace ts
{
	//bool expired (const std::weak_ptr<IMonitorableObject>& value) { return value.expired (); }

	void CMonitor::ClearExpired ()
	{
		/*for (auto deleter = monitoredObjects.begin (); deleter != monitoredObjects.end ();)
		{
			if ((*deleter).expired ())
			{
				deleter = monitoredObjects.erase (deleter);
			}
			else
			{
				++deleter;
			}
		}

		monitoredObjects.remove_if (expired);*/

		monitoredObjects.remove_if ([] (const std::weak_ptr<IMonitorableObject>& value)
		{
			return value.expired ();
		});
	}

	CMonitor::CMonitor (sf::RenderWindow& rwindow)
		: window (rwindow)
	{
		if (!font.loadFromFile ("BKANT.TTF"))
		{
			std::cout << "Can't load font" << std::endl;
		}
	}

	void CMonitor::Update ()
	{
		sf::Text report ("", font, monitorFontSize);
		int index = 0;
		for (auto& object: monitoredObjects)
		{
			if (object.expired())
				continue;

			report.setString (object.lock()->Report ());
			report.setPosition ({ 0.f, float (monitorFontSize * index++) });
			window.draw (report);
		}
	}

	CMonitor& CMonitor::operator << (std::shared_ptr<IMonitorableObject> object)
	{
		if (!object)
		{
			std::cout << "can't start monitoring" << std::endl;
		}
		else
		{
			monitoredObjects.emplace_back (std::weak_ptr<IMonitorableObject> (object));
		}

		return *this;
	}

	CMonitor& CMonitor::operator >> (std::shared_ptr<IMonitorableObject> object)
	{
		if (!object)
		{
			std::cout << "can't start monitoring: empty object" << std::endl;
		}
		else
		{
			ClearExpired ();

			int objId = object->GetId ();
			for (auto deleter = monitoredObjects.begin (); deleter != monitoredObjects.end ();)
			{
				if ((*deleter).lock()->GetId() == objId)
				{
					deleter = monitoredObjects.erase (deleter);
				}
				else
				{
					++deleter;
				}
			}
		}

		return *this;
	}

	bool IMonitorableObject::operator < (const IMonitorableObject& other) const
	{
		return id < other.id;
	}

	bool IMonitorableObject::operator == (const IMonitorableObject& other) const
	{
		return id == other.id;
	}

}