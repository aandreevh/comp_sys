#ifndef PRACT_STREAM_H
#define PRACT_STREAM_H

namespace base{
    inline namespace stream{

        template<typename _El>
        class Stream{

        public:
            using ForEachFunc = std::function<void(const _El&)>;
            using PredFunc = std::function<bool(const _El&)>;
            template<typename _Ret>
            using MapFunc = std::function<_Ret(const _El&)>;
            template<typename _Ret>
            using FoldrFunc = std::function<_Ret(const _El&,_Ret)>;
            template<typename _Ret>
            using FoldlFunc = std::function<_Ret(_Ret,const _El&)>;

            template<typename _Ret>
            class MappedStream : public Stream<_Ret>{
            public:
                using Func =  MapFunc<_Ret>;
            private:
                Stream<_El>& stream;
                Func function;
            public:
                MappedStream(Stream<_El>& str,Func func):stream(str),function(func){}
                _Ret next() override {
                    return function(stream.next());
                }

                bool empty() const noexcept override {
                    return stream.empty();
                }
            };


            virtual _El next() = 0;
            virtual bool empty() const noexcept = 0;

            void forEach(ForEachFunc func){
                while(!empty()) func(next());
            }

            template<typename _Ret,typename _Func> //using template func allows structure op functions with closure
            MappedStream<_Ret> map(const _Func& func){
                return MappedStream<_Ret>(*this,func);
            }

            template<typename _Ret,typename _Func>
            _Ret foldr(_Func func,_Ret zero){
                if(empty()) return zero;
                const _El& e = next(); // argument evaluation is unspecified :/
                return func(e,foldr<_Ret>(func,zero));

            }

            template<typename _Ret,typename _Func>
            _Ret foldl(_Func func,_Ret zero){
                if(empty()) return zero;

                const _El& e = next();
                const _Ret nzero = func(zero,e);
                return foldl<_Ret>(func,nzero); //using tail recursion opt
            }


            virtual ~Stream() = default;
        };
    }
}


#endif
