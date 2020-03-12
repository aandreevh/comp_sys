//
// Created by hykar on 10.03.20 г..
//

#ifndef GAMEO_LOGICAL_H
#define GAMEO_LOGICAL_H


namespace base{

    inline namespace logical {

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
    }


}
#endif //GAMEO_LOGICAL_H
