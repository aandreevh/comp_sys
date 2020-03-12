#include <iostream>
#include <string_view>
#include <string>

#include "include/base/Class.h"
#include "include/base/Factory.h"

using namespace base::cls;

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

class A : public Class<A>{

};

int main(int arg_c, char** arg_v){
    using ComponentFactory = ClassFactory<Component,std::string>;
    using CFP = ClassFactoryBase::PtrBase<ComponentFactory> ;


    FactroyContext fc;
    fc.template operator()<ComponentFactory>();
    CFP factory = fc.get<ComponentFactory>();
    std::cout<<"Tester"<<std::endl;

    factory->add<BasicComponent>([](const std::string& text){
        return Component::instantiate<BasicComponent>(text);
    });

    factory->instantiate<BasicComponent>("yo!")->printData();


}