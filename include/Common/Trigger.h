
#pragma once
#include <vector>
#include "Common/NotificationBase.h"

namespace Common {

class Trigger {
private:
	struct Recevier {
		NotificationFunc notification_func{nullptr};
		void *self{nullptr};
	};

public:

	void add(NotificationFunc notification_func, void *self);

	void fire(NotificationParam *param);

private:
	std::vector<Recevier> receviers;
};
}