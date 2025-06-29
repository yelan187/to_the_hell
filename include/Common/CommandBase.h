#pragma once

#include "Common/ParamBase.h"

namespace ViewModel {
    class ViewModel;
}

namespace View {
    enum class PAGE_STATE;
}

namespace Common {

class CommandParam : public Param {
public:
    virtual ~CommandParam() = default;
};

class CommandBase {
public:
    virtual void execute() {}
    virtual void execute(CommandParam& params) {}
};

template <typename T>
class CommandTypeParam : public CommandParam {
public:
	T value;
};

// Add command param
typedef CommandTypeParam<float> UpdateParam;

typedef struct {
    View::PAGE_STATE new_page_state;
    bool init;
} ChangePageParamValue;
typedef CommandTypeParam<ChangePageParamValue> ChangePageParam;


}