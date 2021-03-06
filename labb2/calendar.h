#ifndef MIKTOR_CALENDAR
#define MIKTOR_CALENDAR

#include "date.h"
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <stdexcept>

#include <ostream>

#include <cstdio>

namespace lab2 {
	/**
	 * Class T must inherit Date
	 */
	template <typename T>
	class Calendar {
		T _current_date;


		/*
		 * Map each date to an vector with events that date
		*/
		std::map<int, std::vector<std::string> > _events;

		template<typename> friend class Calendar;

		/*
		 * Creates a date object from the given parameters and set date to it
		 * If any parameter is INT_MIN it is set to the 
		 * corresponding value in _current_date
		 * 
		 * Returns true on success and false otherwise
		 */
		bool _create_date(T & date, int year, int month, int day);
		
		public:
			Calendar();

			template <typename T2> Calendar(const Calendar<T2> &calendar);

			template <typename T2>
			Calendar<T> &operator=(const Calendar<T2> &calendar);
			bool set_date(int year, int month, int day);
			bool add_event(std::string event);
			bool add_event(std::string event, int day);
			bool add_event(std::string event, int day, int month);
			bool add_event(std::string event, int day, int month, int year);
			bool remove_event(std::string event);
			bool remove_event(std::string event, int day);
			bool remove_event(std::string event, int day, int month);
			bool remove_event(std::string event, int day, int month, int year);

			const std::map<int, std::vector<std::string> > &events() const;
			const T &current_date() const;
	};
}

template <typename T>
lab2::Calendar<T>::Calendar() : _current_date() { }

template <typename T>
template <typename T2>
lab2::Calendar<T>::Calendar(const Calendar<T2> &calendar) {
	_current_date = calendar._current_date;
	_events = calendar._events;
}

template <typename T>
template <typename T2>
lab2::Calendar<T> &lab2::Calendar<T>::operator=(const lab2::Calendar<T2> &calendar) {
	_current_date = calendar._current_date;
	_events = calendar._events;
	return *this;
}

template<typename T>
bool lab2::Calendar<T>::set_date(int year, int month, int day) {
	try {
		_current_date = T(year, month, day);
		return true;
	} catch (std::out_of_range e) {
		return false;
	}
}

template<typename T>
bool lab2::Calendar<T>::add_event(std::string event, int day, int month) {
	return add_event(event,day,month,_current_date.year());
}

template<typename T>
bool lab2::Calendar<T>::add_event(std::string event, int day) {
	return add_event(event,day,_current_date.month(),_current_date.year());
}

template<typename T>
bool lab2::Calendar<T>::add_event(std::string event) {
	return add_event(event,_current_date.day(),_current_date.month(),_current_date.year());
}

template<typename T>
bool lab2::Calendar<T>::add_event(std::string event, int day, int month, int year) {
	try {
		T date = T(year, month, day);
		//Find events for this date:
		std::vector<std::string> &date_events = _events[date.mod_julian_day()];
		//Exists?
		for(std::vector<std::string>::iterator it=date_events.begin(); it!=date_events.end(); ++it) {
			if(*it == event)
				return false;
		}
		date_events.push_back(event);
		return true;
	} catch (std::out_of_range e) {
		return false;
	}
}

template<typename T>
bool lab2::Calendar<T>::remove_event(std::string event, int day, int month) {
	return remove_event(event, day,month,_current_date.year());
}

template<typename T>
bool lab2::Calendar<T>::remove_event(std::string event, int day) {
	return remove_event(event, day,_current_date.month(),_current_date.year());
}

template<typename T>
bool lab2::Calendar<T>::remove_event(std::string event) {
	return remove_event(event, _current_date.day(),_current_date.month(),_current_date.year());
}

template<typename T>
bool lab2::Calendar<T>::remove_event(std::string event, int day, int month, int year) {
	try {
		T date = T(year, month, day);
		//Find events for this date:
		std::vector<std::string> &date_events = _events[date.mod_julian_day()];
		for(std::vector<std::string>::iterator it=date_events.begin(); it!=date_events.end(); ++it) {
			if(*it == event) {
				date_events.erase(it);
				return true;
			}
		}
		//Nothing found
		return false;
	} catch (std::out_of_range e) {
		return false;
	}
}

template<typename T>
const std::map<int, std::vector<std::string> > &lab2::Calendar<T>::events() const{
	return _events;
}

template<typename T>
const T &lab2::Calendar<T>::current_date() const{
	return _current_date;
}


template <typename T>
std::ostream & operator<<(std::ostream & os, const lab2::Calendar<T> & calendar) {
	std::map<int, std::vector<std::string> >::const_iterator it;
	std::vector<std::string>::const_iterator it2;
	for(it=calendar.events().lower_bound(calendar.current_date().mod_julian_day()+1); it!=calendar.events().end();++it) {
		for(it2=it->second.begin();it2!=it->second.end();++it2) {
			os << T(it->first);
			os << " : ";
			os << (*it2) << std::endl;
		}
	}
	return os;
}

#endif
