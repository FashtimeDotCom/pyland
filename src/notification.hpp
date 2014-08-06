#ifndef NOTIFICATION_H
#define NOTIFICATION_H
#include <list>
#include <string>

///
/// Notification class to hold the cache of notification messages 
///
class Notification {
    private:
        ///
        /// stack list of current and passed messages
        ///
        std::list<std::string> stack;

        ///
        /// iterator to locate the users position in the notification history
        ///
        std::list<std::string>::iterator stack_iterator;

    public:
        Notification();

        ///
        /// add news appends a new notifcation string to the history
        /// @param new_notification
        ///     string on the notification to add 
        ///
        void add_new(std::string new_notifcation);

        ///
        /// moves back in history and return new string to display
        /// @return 
        ///     the new string to display in notification bar, if the movement 
        ///     was invalid, then returns the same string
        ///
        std::string backward();

        ///
        /// moves back in history and return new string to display
        /// @return 
        ///     the new string to display in notification bar, if the movement 
        ///     was invalid, then returns the same string
        ///
        std::string forward();
};

#endif