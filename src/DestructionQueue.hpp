#ifndef __DESTRUCTIONQUEUE_HPP_
#define __DESTRUCTIONQUEUE_HPP_

#include <deque>
#include <memory>
#include "IVkObject.hpp"

namespace CbLib::VkEngine{
    class DestructionQueue{
        public:
            template <typename VkObject>
            void push(VkObject& to_destruct){
                destructQueue.push_back(std::make_unique<VkObject>(to_destruct));
            }

            void flush(){
                while(!destructQueue.empty()){
                    auto& to_destruct = destructQueue.front();
                    to_destruct->destruct();
                    destructQueue.pop_front();
                }
                destructQueue.clear();
            }

        private:
            std::deque<std::unique_ptr<IVkObject>> destructQueue;
    };
}



#endif // __DESTRUCTIONQUEUE_H_
