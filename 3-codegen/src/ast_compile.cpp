#include "ast.hpp"

#include <string>
#include <regex>

static int makeNameUnq=0;

static std::string makeName(std::string base)
{
    return "_"+base+"_"+std::to_string(makeNameUnq++);
}

void CompileRec(
    std::string destReg,    // The name of the register to put the result in
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");

    if( regex_match( program->type, reNum ) ){
        std::cout<<"const "<<destReg<<" "<<program->type<<std::endl;

    }else if( regex_match( program->type, reId ) ){
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout<<"add "<<destReg<<" "<<program->type<<" "<<zero<<std::endl;

    }else if(program->type=="Param"){
        std::cout<<"param "<<destReg<<" "<<program->value<<std::endl;

    }else if(program->type=="Seq"){
        for(unsigned i=0; i<program->branches.size(); i++){
            CompileRec(destReg, program->branches[i]);
        }

    // TODO : handle the others

  }else if(program->type == "Add"){
      std::string srcRegA = makeName("secRegA");
      std::string srcRegB = makeName("secRegB");
      CompileRec(srcRegA, program->branches[0]);
      CompileRec(srcRegB, program->branches[1]);
      std::cout<<"add "<<destReg<<" "<<srcRegA<<" "<<srcRegB<<std::endl;

  }else if(program->type=="Sub"){
  	 std::string srcRegA = makeName("srcRegA");
 	   std::string srcRegB = makeName("srcRegB");
	   CompileRec(srcRegA, program->branches[0]);
 	   CompileRec(srcRegB, program->branches[1]);
 	   std::cout<<"sub "<<destReg<<" "<<srcRegA<<" "<<srcRegB<<std::endl;

	}else if(program->type=="LessThan"){
 	   std::string srcRegA = makeName("srcRegA");
	   std::string srcRegB = makeName("srcRegB");
	   CompileRec(srcRegA, program->branches[0]);
 	   CompileRec(srcRegB, program->branches[1]);
 	   std::cout<<"lt "<<destReg<<" "<<srcRegA<<" "<<srcRegB<<std::endl;
  }else if(program->type=="Assign"){
		    CompileRec(program->value, program->branches[0]);
   }else if(program->type=="Output"){

	    CompileRec(destReg, program->branches[0]);
		std::cout << "output " << destReg<< std::endl;

	}else if(program->type=="If"){
  std::string cond = makeName("cond");
  CompileRec(cond, program->branches[0]);

  std::string zero = makeName("zero");
std::cout<<"const "<<zero<<" 0"<<std::endl;

std::string fail = makeName("fail");
std::string pass = makeName("pass");

  std::cout << "beq " << cond << " " << zero << " " << fail << std::endl;
CompileRec(destReg, program->branches[1]);
std::cout << "bne " << cond << " " << zero << " " << pass << std::endl;
        std::cout << ":" << fail << std::endl;
        CompileRec(destReg, program->branches[2]);
std::cout << ":" << pass << std::endl;

}else if(program->type=="While"){

		std::cout<< "const zero 0" << std::endl;
		std::cout << "const one 1" << std::endl;
		std::cout << ":top" << std::endl;
		 CompileRec("srcRegA", program->branches[0]);

		std::cout << "beq srcRegA zero bottom" << std::endl;
		CompileRec("srcRegA", program->branches[1]);

		//std::cout<< "output srcRegA" << std::endl;
		std::cout<< "beq one one top" << std::endl;
		std::cout << ":bottom" << std::endl;
    std::cout<< "const destReg 0" << std::endl;

   }else if(program->type=="Input"){

		std::cout << "input " << destReg << std::endl;

    }else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}

void Compile(
    TreePtr program
){
    // Create a register to hold the final result
    std::string res=makeName("res");

    // Compile the whole thing
    CompileRec(res, program);

    // Put the result out
    std::cout<<"halt "<<res<<"\n";
}
