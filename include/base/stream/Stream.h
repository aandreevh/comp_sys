#ifndef PRACT_STREAM_H
#define PRACT_STREAM_H

namespace base{
    inline namespace stream{

        template<typename _El>
        class Stream{

        public:
            using ForEachFunc = std::function<void(const _El&)>;
            using PredFunc = std::function<bool(const _El&)>;
            template<typename T>
            using MapFunc = std::function<T(const _El&)>;

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

            template<typename _Ret>
            MappedStream<_Ret> map(const MapFunc<_Ret>& func){
                return MappedStream<_Ret>(*this,func);
            }

            virtual ~Stream() = default;
        };
    }
}


#endif
