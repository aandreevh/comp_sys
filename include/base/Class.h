#ifndef GAMEO_CLASS_H
#define GAMEO_CLASS_H

#include <string>
#include <typeindex>
#include <memory>

#include "compile_time/logical.h"

namespace base {

inline namespace cls{

    template<typename _Base = void>
    class Class {

    public:


        using TrueBase = typename std::conditional<
                std::is_same<_Base, void>::value, Class<void>, _Base>::type;
        template<typename T>
        using PtrBase = std::shared_ptr<T>;
        using ClassPtr =  PtrBase<_Base>;
        using TypeInfo = std::type_info;
        class Type{
        public:
            using Intern = std::type_info;
            using Ident = std::size_t;

            using hash = struct{
                using Wrapper = std::hash<Ident>;
                Wrapper  wrapper;
                std::size_t operator ()(const Type& type) const{
                    return wrapper(type.ident());
                }
            };

        private:
            std::size_t _ident;

            constexpr static Ident _map(const Intern& intern){
                return  intern.hash_code();
            }
        public:

            constexpr Type():_ident(Ident()){}
            constexpr Type(const Intern& info):_ident(_map(info)){}

            constexpr Ident ident()const noexcept {
                return this->_ident;
            }

            constexpr static Type base() noexcept{
                return typeid(_Base);
            }

            constexpr bool operator==(const Type& other) const noexcept {
                return other.ident() == ident();
            }
        };


    private:

        template<typename T>
        static constexpr bool _Valid() noexcept {
            return std::is_base_of<Class<_Base>, T>::value;
        }

    public:

        inline  Type type() const noexcept { return typeid(*this); }

        operator Type() const{
            return type();
        }

        virtual ~Class() {
            static_assert(_Valid<_Base>(), "_Base must be either void or should inherit Class<_Base>");
        }

        template<typename T, typename = typename std::enable_if<_Valid<T>()>::type>
        static PtrBase<T> cast(const ClassPtr &ptr) {
            return std::static_pointer_cast<T, _Base>(ptr);
        }

        template<typename T = _Base, typename = typename std::enable_if<_Valid<T>()>::type>
        constexpr static Type type() noexcept {
            return typeid(T);
        }

        template<typename T = _Base,typename... _Args,typename = typename std::enable_if<_Valid<T>()>::type>
        static ClassPtr instantiate(_Args&&... args){
            return std::make_shared<T>(std::forward<_Args>(args)...);
        }


    };

}

}
#endif //GAMEO_CLASS_H
