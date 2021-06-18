#ifndef __INSTANCE_HPP_
#define __INSTANCE_HPP_

#include "Engine.hpp"

namespace CbLib::VkEngine{
    class Instance {
    public:
            Instance();

    private:
        VkInstance instance;
    };
}

#endif // __INSTANCE_H_
