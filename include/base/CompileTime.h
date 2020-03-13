//
// Created by hykar on 10.03.20 г..
//

#ifndef HYKR_LOGICAL_H
#define HYKR_LOGICAL_H


namespace base{

    inline namespace compile {

        template<bool P,bool Q>
        struct _and{
            static constexpr bool value = false;
        };

        template<>
        struct _and<true,true>{
            static constexpr  bool value = true;
        };

        template<bool P,bool Q>
        struct _or{
            static constexpr bool value = true;
        };

        template<>
        struct _or<false,false>{
            static constexpr  bool value = false;
        };

        template<typename... Args>
         struct countArgs {
            static const std::size_t  value = 0;
        };

        template<>
        struct countArgs<> {
            static const std::size_t  value = 0;
        };

        template<typename T, typename... Args>
        struct countArgs<T, Args...> {
            static constexpr std::size_t value = countArgs<Args...>::value+1;
        };
    }


}
#endif //HYKR_LOGICAL_H
