//
// Created by hykar on 9.03.20 г..
//

#ifndef HYKR_FACTORY_H
#define HYKR_FACTORY_H

#include <unordered_map>
#include <functional>

#include "class/Class.h"
#include "class/ClassRegistry.h"

namespace base {

    inline namespace factory {

        class ClassFactoryBase : public Class<ClassFactoryBase> {
        public:
            virtual ~ClassFactoryBase() = default;
        };

        template<typename _Base, typename... _Args>
        class ClassFactory : public ClassFactoryBase,
                             public ClassKeyRegistry<_Base, std::function<typename _Base::ClassPtr(_Args...)>> {

        public:
            using ClassBase = Class<_Base>;
            using ClassPtr = typename ClassBase::ClassPtr;
            using Type = typename ClassBase::Type;
            using Registry = ClassKeyRegistry<_Base, std::function<typename _Base::ClassPtr(_Args...)>>;

            using FuncType = std::function<ClassPtr(_Args...)>;

            ClassPtr instantiate(const Type &type, _Args &&...args) const {
                return Registry::get(type)(std::forward<_Args>(args)...);
            }

            template<class _Type>
            typename ClassBase::template PtrBase<_Type> instantiate(_Args &&...args) {
                return ClassBase::template cast<_Type>
                        (instantiate(ClassBase::template type<_Type>(), std::forward<_Args>(args)...));
            }

            virtual ~ClassFactory() = default;
        };


        class FactoryContext : public ClassRegistry<ClassFactoryBase> {};

    }
}

#endif //HYKR_FACTORY_H
