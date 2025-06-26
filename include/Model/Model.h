#pragma once
#include "Core/Engine.h"

namespace Model {
class Model {
public:
    Model(Core::Engine &engine) : engine(engine) {};
    
    // 提供空实现以解决虚函数表问题
    virtual void update() {};
    
    // 添加虚析构函数(C++最佳实践)
    virtual ~Model() {}; 
    
protected:
    Core::Engine &engine;

};

};