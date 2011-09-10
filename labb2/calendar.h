#include "date.h"
#include <string>
#include <set>
#include <utility>
#include <climits>
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
		std::set< std::pair<int , std::string> > _events;

		template<typename> friend class Calendar;
		template<typename> friend std::ostream & operator<<(std::ostream & os, const lab2::Calendar<T> & cal);

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
			template <typename T2>
			Calendar<T> &operator=(const Calendar<T2> &calendar);
			bool set_date(int year, int month, int day);
			bool add_event(std::string event, int day=INT_MIN, int month=INT_MIN, int year=INT_MIN);
			bool remove_event(std::string event, int day=INT_MIN, int month=INT_MIN, int year=INT_MIN);
	};
}

template<typename T>
bool lab2::Calendar<T>::_create_date(T & date, int year, int month, int day) {
	if(day == INT_MIN)
		day = _current_date.day();
	if(month == INT_MIN)
		month = _current_date.month();
	if(year == INT_MIN)
		year = _current_date.year();

	try {
		date = T(year, month, day);
		return true;
	} catch (std::out_of_range e) {
		return false;
	}
}

template <typename T>
lab2::Calendar<T>::Calendar() : _current_date() { }

template <typename T>
template <typename T2>
lab2::Calendar<T> &lab2::Calendar<T>::operator=(const lab2::Calendar<T2> &calendar) {
	_current_date = calendar._current_date;
	_events = calendar._events;
	return *this;
}

template<typename T>
bool lab2::Calendar<T>::set_date(int year, int month, int day) {
	return _create_date(_current_date,year, month, day);
}

template<typename T>
bool lab2::Calendar<T>::add_event(std::string event, int day, int month, int year) {
	T date;
	if(_create_date(date, year, month, day)) {
		return _events.insert(make_pair(date.mod_julian_day(),event)).second; //Second value in pair is true if insert succeds
	} else {
		printf("Create failed\n");
		return false;
	}
}

template<typename T>
bool lab2::Calendar<T>::remove_event(std::string event, int day, int month, int year) {
	T date;
	if(_create_date(date, year, month, day)) {
		return (_events.erase(make_pair(date.mod_julian_day(), event))==1);
	} else {
		return false;
	}
}


template <typename T>
std::ostream & operator<<(std::ostream & os, const lab2::Calendar<T> & calendar) {
/*	std::set<std::pair<int, std::string> >::iterator it;
	for(it=calendar._events.begin(); it!=calendar._events.end();++it) {
		os << T(
	}*/
	return os;
}
