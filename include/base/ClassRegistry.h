//
// Created by hykar on 11.03.20 г..
//

#ifndef GAMEO_CLASSREGISTRY_H
#define GAMEO_CLASSREGISTRY_H

#include "Class.h"

namespace base {

    class BaseClassRegistry : public Class<BaseClassRegistry>{
    public:
       virtual ~BaseClassRegistry() = default;
    };

    template<typename _Base, typename _El,
             typename _Cont = std::unordered_map<typename _Base::Type, _El, typename _Base::Type::hash>>
    class ClassKeyRegistry : public BaseClassRegistry{
    public:
        using ClassBase = Class<_Base>;
        using Type = typename ClassBase::Type;

        class TypeNotFoundException : public std::exception {
        public:

            static constexpr const char *base() {
                return typeid(_Base).name();
            }

            const char *what() const noexcept override {
                return base();
            }


        };

    private:
        _Cont storage;

    public:
        const _El& get(const Type &type) const {
            if (!exists(type)) throw TypeNotFoundException();
            return storage.find(type)->second;
        }

        template<class _Type>
        const _El& get() const{
            return get(ClassBase::template type<_Type>());
        }


        bool exists( Type type) const {
            return storage.find(type) != storage.end();
        }

        template<class _Type>
        bool exists() const {
            return exists(ClassBase::template type<_Type>());
        }


        bool remove(const Type &type) {
            if (!exists(type)) return false;
            storage.erase(storage.find(type));
            return true;
        }

        template<class _Type>
        bool remove() {
            return remove(ClassBase::template type<_Type>());
        }

        bool add(const Type &type,const _El& el) {
            if (exists(type)) return false;
            storage[type] = el;
            return true;
        }

        template<class _Type>
        bool add(const _El& el) {
            return add(ClassBase::template type<_Type>(), el);
        }

        virtual ~ClassKeyRegistry() = default;

    };

    template<typename _Base>
    class ClassRegistry : public BaseClassRegistry{
    public:
        using ClassBase = Class<_Base>;
        using ClassPtr = typename ClassBase::ClassPtr;
        using Type = typename ClassBase::Type;
    private:
        using Registry  = ClassKeyRegistry<_Base,ClassPtr>;
        Registry registry;

    public:

        const ClassPtr& get(const Type &type) const {
            return registry.get(type);
        }

        template<class _Type>
        typename ClassBase::template PtrBase<_Type> get() const{
            return ClassBase::template cast<_Type>(registry.template get<_Type>());
        }


        bool exists( Type type) const {
            return registry.exists(type);
        }

        template<class _Type>
        bool exists() const {
            return registry.template exists<_Type>();
        }


        bool remove(const Type &type) {
            return registry.remove(type);
        }

        template<class _Type>
        bool remove() {
            return registry.template remove<_Type>();
        }

        bool add(const ClassPtr& el) {
            return registry.add(el->type(),el);
        }


        template<class _Type,typename... _Args>
        bool add(_Args&&... args) {
            if (exists(Registry::ClassBase::template type<_Type>())) return false;
            return registry.add(Registry::ClassBase::template type<_Type>(),
                       Registry::ClassBase::template instantiate<_Type>(std::forward<_Args>(args)...));
        }

        template<class _Type,typename... _Args>
        bool operator()(_Args&&... args) {
            return add<_Type,_Args...>(std::forward<_Args>(args)...);
        }

    };
}

#endif //GAMEO_CLASSREGISTRY_H
