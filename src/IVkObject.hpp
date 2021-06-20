#ifndef __IVKOBJECT_H_
#define __IVKOBJECT_H_

namespace CbLib::VkEngine{
    class IVkObject{
        public:
        virtual void destruct() = 0;
    };
}


#endif // __IVKOBJECT_H_
