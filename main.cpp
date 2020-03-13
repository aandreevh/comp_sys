#include <iostream>
#include <string_view>
#include <string>

#include "include/base/CompileTime.h"
#include "include/base/class/Class.h"
#include "include/base/Factory.h"
#include "include/base/stream/Stream.h"

using namespace base::cls;
using namespace base::factory;
using namespace base::compile;
using namespace base::stream;

class Component : public Class<Component>{
private:

public:

};


class BasicComponent : public  Component{

    std::string data;
public:

    BasicComponent(){
        this->data = "";
    }
    BasicComponent(const std::string& data){
        this->data = data;
    }

    void printData() const{
        std::cout<<this->data;
    }
};

class TestStream : public base::Stream<int>{

    std::vector<int> data;
    std::size_t cursor =0;
public:
    TestStream(std::initializer_list<int> list):data(list){}

    int next() override {
        return data[cursor++];
    }

    bool empty() const noexcept override {
        return cursor >= data.size();
    }
};


int main(int arg_c, char** arg_v){

    ((TestStream){1,2,3,4}).map<bool>([](int e){
       return (bool)(e%2 ==0);
    }).forEach([](bool b){
        std::cout<<(char)('a'+2*(int)b);
    });

    using ComponentFactory = ClassFactory<Component,std::string>;
    using CFP = ClassFactoryBase::PtrBase<ComponentFactory> ;

    FactoryContext fc;
    fc.template operator()<ComponentFactory>();
    CFP factory = fc.get<ComponentFactory>();
    std::cout<<"Tester"<<std::endl;

    factory->add<BasicComponent>([](const std::string& text){
        return Component::instantiate<BasicComponent>(text);
    });
    factory->instantiate<BasicComponent>("yo!")->printData();


}