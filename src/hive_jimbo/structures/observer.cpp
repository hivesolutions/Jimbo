/*
 Hive Jimbo Library
 Copyright (C) 2008 Hive Solutions Lda.

 This file is part of Hive Jimbo Library.

 Hive Jimbo Library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Hive Jimbo Library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Hive Jimbo Library. If not, see <http://www.gnu.org/licenses/>.

 __author__    = João Magalhães <joamag@hive.pt>
 __version__   = 1.0.0
 __revision__  = $LastChangedRevision$
 __date__      = $LastChangedDate$
 __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
 __license__   = GNU General Public License (GPL), Version 3
*/

#include "stdafx.h"

#include "observer.h"

JBObservable::JBObservable() {
}

JBObservable::~JBObservable() {
}

void JBObservable::fireEvent(std::string &eventName, void *arguments) {
    // retrieves both the vector containing the various callback functions
    // for the current event and the vector containing the various observer
    // to be notified about the event
    std::vector<int(*)(JBObservable &, void *)> &callbackFunctions = this->eventHandlersMap[eventName];
    std::vector<JBObserver *> &observers = this->observersMap[eventName];

    // iterates over all the callback functions to call them notifying
    // the handler of the function about the "fired" event
    for(unsigned int index = 0; index < callbackFunctions.size(); index++) {
        // retrieves the current callback function and calls it with the
        // current observable reference and the event arguments
        int(*callbackFunction)(JBObservable &, void *) = callbackFunctions[index];
        callbackFunction(*this, arguments);
    }

    // iterates over all the observers to update them notifying
    // the observer about the "fired" event
    for(unsigned int index = 0; index < observers.size(); index++) {
        // retrieves the current observer object and "updates" it notifiying
        // it about the event (send the observable the event name and the
        // arguments to the update call as parameters)
        JBObserver *observer = observers[index];
        observer->update(*this, eventName, arguments);
    }
}

void JBObservable::registerForEvent(std::string eventName, int(*callbackFunction)(JBObservable &, void *)) {
    // in case the event list of handlers is not yet set, on must be created
    // to hold the references to the various callback functions
    if(this->eventHandlersMap.find(eventName) == this->eventHandlersMap.end()) {
        // creates the new vector to hold the various callback functions
        this->eventHandlersMap[eventName] = std::vector<int(*)(JBObservable &, void*)>();
    }

    // appends the callback function to the back of the callback
    // functions list for the current event (name)
    this->eventHandlersMap[eventName].push_back(callbackFunction);
}

void JBObservable::unregisterForEvent(std::string eventName, int(*callbackFunction)(JBObservable &, void *)) {
    std::vector<int(*)(JBObservable &, void *)> &callbackFunctions = this->eventHandlersMap[eventName];

    for(unsigned int index = 0; index < callbackFunctions.size(); index++) {
        // retrieves the current callback function and checks if it is
        // the one to be found in case it's not continues the loop (continue
        // finding more elements, otherwise erases it from the vector
        int(*_callbackFunction)(JBObservable &, void *) = callbackFunctions[index];
        if(_callbackFunction != callbackFunction) continue;

        // erases the current element from the vector and break the loop
        // unregister is complete
        callbackFunctions.erase(callbackFunctions.begin() + index);
        break;
    }
}

void JBObservable::registerObserverForEvent(std::string eventName, JBObserver &observer) {
    if(this->observersMap.find(eventName) == this->observersMap.end()) {
        this->observersMap[eventName] = std::vector<JBObserver *>();
    }

    this->observersMap[eventName].push_back(&observer);
}

void JBObservable::unregisterObserverForEvent(std::string eventName, JBObserver &observer) {
}

JBObserver::JBObserver() {
}

JBObserver::~JBObserver() {
}
