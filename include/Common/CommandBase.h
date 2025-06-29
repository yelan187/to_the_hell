#pragma once

#include "Common/ParamBase.h"

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
typedef CommandTypeParam<int> ChangePageParam;

}