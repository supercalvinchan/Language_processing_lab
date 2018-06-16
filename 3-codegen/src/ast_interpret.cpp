#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");

    if( regex_match(program->type, reNum) ){
        return std::atol(program->type.c_str());

    // TODO : Check for things matching reId

  }else if( regex_match(program->type, reId) ){

        auto it = context.bindings.find(program->type);
        if(it != context.bindings.end())
        {
          return it->second;
        }else
        {
          throw std::runtime_error("Variable has not found '"+program->type+"'");
        }
        return 0;

  }else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;

    }else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;

    // TODO: Handle other constructs

    }else if(program->type=="Input"){
       std::string id;
       std::getline(std::cin, id);
        return std::atol(id.c_str());
    }else if(program->type=="Assign"){

       int32_t val=Interpret(context, program->branches.at(0));
       context.bindings[program->value] = val;
       return val;

    }else if(program->type=="Sub"){

        int32_t left=Interpret(context, program->branches.at(0));
        int32_t right=Interpret(context, program->branches.at(1));
        return (left - right);

    }else if(program->type=="Add"){

       int32_t left=Interpret(context, program->branches.at(0));
       int32_t right=Interpret(context, program->branches.at(1));
       return (left + right);

  }else if(program->type=="LessThan"){

     int32_t left=Interpret(context, program->branches.at(0));
     int32_t right=Interpret(context, program->branches.at(1));
     if(left < right)
     {
       return 1;
     }else{
       return 0;
     }
    }else if(program->type=="Seq")
    {
      int32_t val = 0;
       for(int i = 0; i < program->branches.size(); i++)
       {
        val=Interpret(context, program->branches.at(i));

       }
         return val;
    }else if(program->type=="While"){

       while(Interpret(context, program->branches.at(0)))
       {
        Interpret(context, program->branches.at(1));
       }
      return 0;

    }else if(program->type=="If")
    {

     if(Interpret(context, program->branches.at(0)))
     {if(program->branches.size() > 1)
       return Interpret(context, program->branches.at(1));
     }else if(program->branches.size() > 2)
     {
       return Interpret(context, program->branches.at(2));
     } else
     {
       return 0;
     }

   }else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
