#include "Common/Trigger.h"

using Common::Trigger;

void Trigger::add(NotificationFunc notification_func, void *self) {
    Recevier receiver;
    receiver.notification_func = notification_func;
    receiver.self = self;
    receviers.push_back(receiver);
}

void Trigger::fire(NotificationParam *param) {
    for (const auto& receiver : receviers) {
        if (receiver.notification_func) {
            receiver.notification_func(param, receiver.self);
        }
    }
    delete param;
}