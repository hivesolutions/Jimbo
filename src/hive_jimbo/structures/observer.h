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

#pragma once

class JBObservable;
class JBObserver;

class JBObservable {
    private:
        std::map<std::string, std::vector<int(*)(JBObservable &, void *)>> eventHandlersMap;
        std::map<std::string, std::vector<JBObserver *>> observersMap;
    protected:
        void fireEvent(std::string &eventName, void *arguments);
    public:
        JB_EXPORT JBObservable();
        JB_EXPORT ~JBObservable();
        JB_EXPORT void registerForEvent(std::string eventName, int(*callbackFunction)(JBObservable &, void *));
        JB_EXPORT void unregisterForEvent(std::string eventName, int(*callbackFunction)(JBObservable &, void *));
        JB_EXPORT void registerObserverForEvent(std::string eventName, JBObserver &observer);
        JB_EXPORT void unregisterObserverForEvent(std::string eventName, JBObserver &observer);
};

class JBObserver {
    public:
        JB_EXPORT JBObserver();
        JB_EXPORT ~JBObserver();
        JB_EXPORT virtual void update(JBObservable &element, std::string &eventName, void *arguments) {};
};
