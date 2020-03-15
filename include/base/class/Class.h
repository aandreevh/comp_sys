#ifndef HYKR_CLASS_H
#define HYKR_CLASS_H

#include <string>
#include <typeindex>
#include <memory>
#include <functional>

#include "../CompileTime.h"

namespace base {

inline namespace cls{

    namespace {

        template<typename _Base>
        class _InternType{  //type must be _Base specific for compile time Type mismatch checks
        public:
            using Intern = std::type_info;
            using Ident = std::size_t;

            using hash = struct{
                using Wrapper = std::hash<Ident>;

                Wrapper  wrapper;
                std::size_t operator ()(const _InternType& type) const{
                    return wrapper(type.ident());
                }
            };

        private:
            Ident _ident;
            constexpr static Ident _map(const Intern& intern){
                return  intern.hash_code();
            }
        public:

            constexpr _InternType():_ident(Ident()){}
            constexpr _InternType(const Intern& info):_ident(_map(info)){} //intended implicit constructor

            constexpr Ident ident()const noexcept {
                return this->_ident;
            }

            constexpr static _InternType<_Base> base() noexcept{
                return typeid(_Base);
            }

            constexpr bool operator==(const _InternType& other) const noexcept {
                return other.ident() == ident();
            }
        };

    }


    template<typename _Base = void>
    class Class {

    public:


        using TrueBase = typename std::conditional<
                std::is_same<_Base, void>::value, Class<void>, _Base>::type;
        template<typename T>
        using PtrBase = std::shared_ptr<T>;
        using ClassPtr =  PtrBase<_Base>;
        using Type = _InternType<_Base>;
        template<typename T,typename... _Args>
        static std::function<PtrBase<T>(_Args...)> PtrInstantiate(){
            return std::make_shared<T,_Args...>;
        }

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
        static PtrBase<T> instantiate(_Args&&... args){
            return PtrInstantiate<T,_Args...>()(std::forward<_Args>(args)...);
        }


    };

}

}


#endif //HYKR_CLASS_H
