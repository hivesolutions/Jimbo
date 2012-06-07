// Hive Jimbo Library
// Copyright (C) 2008 Hive Solutions Lda.
//
// This file is part of Hive Jimbo Library.
//
// Hive Jimbo Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Hive Jimbo Library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Hive Jimbo Library. If not, see <http://www.gnu.org/licenses/>.
//
// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#include "stdafx.h"

#include "observer.h"

JBObservable::JBObservable() {
}

JBObservable::~JBObservable() {
}

void JBObservable::FireEvent(std::string &event_name, void *arguments) {
    // retrieves both the vector containing the various callback functions
    // for the current event and the vector containing the various observer
    // to be notified about the event
    std::vector<int(*)(JBObservable &, void *)> &callback_functions = this->event_handlers_map[event_name];
    std::vector<JBObserver *> &observers = this->observers_map[event_name];

    // iterates over all the callback functions to call them notifying
    // the handler of the function about the "fired" event
    for(unsigned int index = 0; index < callback_functions.size(); index++) {
        // retrieves the current callback function and calls it with the
        // current observable reference and the event arguments
        int(*callback_function)(JBObservable &, void *) = callback_functions[index];
        callback_function(*this, arguments);
    }

    // iterates over all the observers to Update them notifying
    // the observer about the "fired" event
    for(unsigned int index = 0; index < observers.size(); index++) {
        // retrieves the current observer object and "updates" it notifiying
        // it about the event (send the observable the event name and the
        // arguments to the Update call as parameters)
        JBObserver *observer = observers[index];
        observer->Update(*this, event_name, arguments);
    }
}

void JBObservable::RegisterForEvent(std::string event_name, int(*callback_function)(JBObservable &, void *)) {
    // in case the event list of handlers is not yet set, on must be created
    // to hold the references to the various callback functions
    if(this->event_handlers_map.find(event_name) == this->event_handlers_map.end()) {
        // creates the new vector to hold the various callback functions
        this->event_handlers_map[event_name] = std::vector<int(*)(JBObservable &, void*)>();
    }

    // appends the callback function to the back of the callback
    // functions list for the current event (name)
    this->event_handlers_map[event_name].push_back(callback_function);
}

void JBObservable::UnregisterForEvent(std::string event_name, int(*callback_function)(JBObservable &, void *)) {
    std::vector<int(*)(JBObservable &, void *)> &callback_functions = this->event_handlers_map[event_name];

    for(unsigned int index = 0; index < callback_functions.size(); index++) {
        // retrieves the current callback function and checks if it is
        // the one to be found in case it's not continues the loop (continue
        // finding more elements, otherwise erases it from the vector
        int(*_callback_function)(JBObservable &, void *) = callback_functions[index];
        if(_callback_function != callback_function) continue;

        // erases the current element from the vector and break the loop
        // unregister is complete
        callback_functions.erase(callback_functions.begin() + index);
        break;
    }
}

void JBObservable::RegisterObserverForEvent(std::string event_name, JBObserver &observer) {
    if(this->observers_map.find(event_name) == this->observers_map.end()) {
        this->observers_map[event_name] = std::vector<JBObserver *>();
    }

    this->observers_map[event_name].push_back(&observer);
}

void JBObservable::UnregisterObserverForEvent(std::string event_name, JBObserver &observer) {
}

JBObserver::JBObserver() {
}

JBObserver::~JBObserver() {
}
