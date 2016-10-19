#pragma once
#include <unordered_map>
#include <list>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Constants.h"

namespace ts
{
	class IMonitorableObject
	{
	protected:
		int id { 0 };
	public:
		virtual int GetId () { return id; }
		virtual sf::String Report () const = 0;
		bool operator < (const IMonitorableObject& other) const;
		bool operator == (const IMonitorableObject& other) const;
	};

	class CMonitor
	{
		sf::RenderWindow& window;
		std::list<std::weak_ptr<IMonitorableObject>> monitoredObjects;
		sf::Font font;

		void ClearExpired ();

	public:
		CMonitor (sf::RenderWindow& rwindow);
		void Update ();
		CMonitor& operator << (std::shared_ptr<IMonitorableObject> object);
		CMonitor& operator >> (std::shared_ptr<IMonitorableObject> object);
	};

}