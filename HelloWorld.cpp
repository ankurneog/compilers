//===-- HelloWorld.cpp - Example Transformations --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Utils/HelloWorld.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/NoFolder.h"


using namespace llvm;


PreservedAnalyses HelloWorldPass::run(Function &F,
                                      FunctionAnalysisManager &AM) {
  std::string Basicblock;
  size_t InstCount=0;
  errs() << " Function : "<<F.getName() << "\n";


  for (BasicBlock &BB : F){


      errs() << "     Basic block (name=" << BB.getName() << ") has "
             << BB.size() << " instructions.\n";
      for(Instruction &I : BB){
          errs()<<"             Instructions : "<<I<<"\n";
      }
      for(BasicBlock* Pred: predecessors(&BB)){
        errs()<<"         Predecessor : "<<Pred->getName()<<"\n";
      }
      for(BasicBlock* Succ: successors(&BB)){
        errs()<<"         Successors  : "<<Succ->getName()<<"\n";
      }
      errs() << "\n";


      if(BB.size()>InstCount){
        Basicblock = BB.getName().str();
        InstCount = BB.size();
      }
  }


  errs()<<"Block with Most instructions : "<<Basicblock<<" ,Instruction Count: "<<InstCount<<"\n";

  LLVMContext& context = F.getContext();
  auto module = F.getParent();

  errs()<<"-----Global Variable----- "<<'\n';

  auto type = IntegerType::getInt32Ty(context);
  auto constant = module->getOrInsertGlobal("my_global", type);
  constant->setName("global_x");
  module->print(llvm::errs(), nullptr);

  errs()<<"------Add new Instruction---------"<<'\n';

  for (BasicBlock &BB : F) {
    
    if(BB.getName().str()=="entry"){
       for (Instruction& I : BB){
         if(StoreInst* Si =dyn_cast<StoreInst>(&I)){
            errs()<<" Found store instruction "<<I<<"\n";
            LLVMContext& context = I.getContext();
            //IRBuilder Builder(&BB,BB.getFirstInsertionPt());
            Value* lhs = ConstantInt::get(Type::getInt32Ty(context), 4);
            errs()<<" Value lhs : "<<lhs->getName()<<"\n";
            Value* rhs = ConstantInt::get(Type::getInt32Ty(context), 6);
            IRBuilder<llvm::NoFolder> builder(&I); // To prevent constant folding
            builder.SetInsertPoint(&I);
            auto my_result=builder.CreateMul(lhs,rhs,"my_mul");
            errs()<<"Created builder.CreateMul(lhs,rhs,my_mul)....... "<<"\n";
            break;

         } 
       }       



      /*
       errs()<<" Found basic block "<<BB.getName().str()<<'\n';
       auto bb_iterator=BB.begin();
      // error: call to deleted constructor of 'LLVMContext'
       LLVMContext& llvm_context= bb_iterator->getContext();
       Value* lhs = ConstantInt::get(Type::getInt32Ty(llvm_context), 4);
       Value* rhs = ConstantInt::get(Type::getInt32Ty(llvm_context), 6);

       Instruction &I =*bb_iterator;
       errs()<<" Instruction : "<<I.getName().str()<<"\n";
       IRBuilder<> builder(&I);
       DebugLoc dl = builder.getCurrentDebugLocation();
       builder.CreateMul(lhs,rhs,"my_mul");
       builder.SetInsertPoint(I.getNextNode());
       //errs()<<" Builder : "<<builder.getName
       builder.SetCurrentDebugLocation(dl);
       */

    } 
  }
  
  return PreservedAnalyses::all();


}



