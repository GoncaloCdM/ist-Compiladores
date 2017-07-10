#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/stack_size_counter.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  
  ASSERT_SAFE_EXPRESSIONS;

  if(!_is_global){
    _pf.INT(node->value());
  }
  else{  
    chooseSegment(DATA);
    _pf.ALIGN(); 
    if (_var_is_public){ _pf.GLOBAL(_global_var_lbl, _pf.OBJ()); }    
    _pf.LABEL(_global_var_lbl);
    _pf.CONST(node->value());
    chooseSegment(_prevSeg);
  }
}

void xpl::postfix_writer::do_double_node(cdk::double_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;

  if(!_is_global){
    chooseSegment(RODATA);
    _pf.ALIGN();
    _pf.LABEL(mklbl(++_lbl));
    _pf.DOUBLE(node->value());
    chooseSegment(_prevSeg);
    _pf.ADDR(mklbl(_lbl));
    _pf.DLOAD();
  }
  else{
    chooseSegment(DATA);
    _pf.ALIGN();
    if (_var_is_public){ _pf.GLOBAL(_global_var_lbl, _pf.OBJ()); }
    _pf.LABEL(_global_var_lbl);
    _pf.DOUBLE(node->value());
    chooseSegment(_prevSeg);
  }
}

void xpl::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  chooseSegment(RODATA);
  _pf.ALIGN();
  _pf.LABEL(mklbl(++_lbl));
  _pf.STR(node->value());
  chooseSegment(_prevSeg);

  if(!_is_global){
    _pf.ADDR(mklbl(_lbl));
  }
  else{
    chooseSegment(DATA);
    _pf.ALIGN();
    if(_var_is_public){ _pf.GLOBAL(_global_var_lbl, _pf.OBJ()); }
    _pf.LABEL(_global_var_lbl);
    _pf.ID(mklbl(_lbl));
    chooseSegment(_prevSeg);
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);
  if (!isDouble(node->type())){
    _pf.NEG();
  }
  else{ 
    _pf.DNEG();
  }  
}

void xpl::postfix_writer::do_identity_node(xpl::identity_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);
}

void xpl::postfix_writer::do_not_node(cdk::not_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);
  _pf.NOT();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_or_node(cdk::or_node * const node, int lvl){ 
  ASSERT_SAFE_EXPRESSIONS;

  std::string end = mklbl(++_lbl);
  node->left()->accept(this, lvl);

  _pf.DUP();

  _pf.JNZ(end); 
  _pf.TRASH(node->type()->size());
  node->right()->accept(this, lvl);

  _pf.ALIGN();
  _pf.LABEL(end);
}
   
//---------------------------------------------------------------------------

void xpl::postfix_writer::do_and_node(cdk::and_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;

  std::string end = mklbl(++_lbl);
  node->left()->accept(this, lvl);

  _pf.DUP();

  _pf.JZ(end);
  _pf.TRASH(node->type()->size());

  node->right()->accept(this, lvl);

  _pf.ALIGN();
  _pf.LABEL(end);
}

//---------------------------------------------------------------------------
void xpl::postfix_writer::processBinArithExp(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);

  itod(node->type(), node->left()->type());
  
  node->right()->accept(this, lvl);

  itod(node->type(), node->right()->type());

}

void xpl::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);
  
  if(isInt(node->left()->type()) && isPointer(node->type())){
      _pf.INT(node->type()->subtype()->size());
      _pf.MUL();
  }
  else itod(node->type(), node->left()->type());

  node->right()->accept(this, lvl);

  if(isInt(node->right()->type()) && isPointer(node->type())){
      _pf.INT(node->type()->subtype()->size());
      _pf.MUL();
  }
  else itod(node->type(), node->right()->type());

  if(!isDouble(node->type())){
    _pf.ADD();
  }
  else{
    _pf.DADD();
  }
}

void xpl::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  processBinArithExp(node, lvl);

  if (!isDouble(node->type())){ 
    _pf.SUB(); 
  }
  else{ 
    _pf.DSUB(); 
  }
}

void xpl::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  processBinArithExp(node, lvl);

  if (!isDouble(node->type())){ 
    _pf.MUL(); 
  }
  else{ 
    _pf.DMUL(); 
  }
}
void xpl::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  processBinArithExp(node, lvl);

  if (!isDouble(node->type())){ 
    _pf.DIV(); 
  }
  else{ 
    _pf.DDIV(); 
  }
}
void xpl::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);

  _pf.MOD();
}

// ----------------------------------------------------------------------

void xpl::postfix_writer::processBinEqualityAndInequalityExp(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);

  if(isDouble(node->right()->type()) && isInt(node->left()->type())){
    _pf.I2D();
  }

  node->right()->accept(this, lvl);

  if(isDouble(node->left()->type()) && isInt(node->right()->type())){
    _pf.I2D();
  }

  if(isDouble(node->left()->type()) || isDouble(node->right()->type())){
    _pf.DCMP();
    _pf.INT(0);
  }
}

void xpl::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinEqualityAndInequalityExp(node, lvl);

  _pf.LT();
}

void xpl::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  processBinEqualityAndInequalityExp(node, lvl);

  _pf.LE();
}

void xpl::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinEqualityAndInequalityExp(node, lvl);

  _pf.GE();
}

void xpl::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinEqualityAndInequalityExp(node, lvl);

  _pf.GT();
}

void xpl::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinEqualityAndInequalityExp(node, lvl);

  _pf.NE();
}

void xpl::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinEqualityAndInequalityExp(node, lvl);

  _pf.EQ();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::shared_ptr<xpl::symbol> symbol = _symtab.find(node->name());

  if(symbol==nullptr) throw std::string(node->name() + " not found!");

  if(symbol->is_global() && !symbol->is_function()){
    _pf.ADDR(symbol->name());
  }
  else{
    _pf.LOCAL(symbol->offset());
  }

}

void xpl::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->lvalue()->accept(this, lvl);
  if(!isDouble(node->type())){ _pf.LOAD(); }
  else{ _pf.DLOAD();  }
}

void xpl::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) { 
  ASSERT_SAFE_EXPRESSIONS;

  node->rvalue()->accept(this, lvl);
  if(!isDouble(node->rvalue()->type())){ _pf.DUP(); }
  else{ _pf.DDUP(); }

  itod(node->lvalue()->type(), node->rvalue()->type());

  node->lvalue()->accept(this, lvl);
  if(!isDouble(node->lvalue()->type())){  _pf.STORE();  }
  else{ _pf.DSTORE(); }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_function_def_node(xpl::function_def_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;


  _is_global = false;

  _actual_function = _symtab.find(node->id());
  if(_actual_function==nullptr) throw std::string(node->id() + " not found!");

  if(_actual_function->is_extern()){  
    popUsedFunc(node->id());
    _actual_function->is_extern(false);
  }

  xpl::stack_size_counter s_cont(_compiler);
  node->accept(&s_cont, lvl);

  size_t size_var_ret = node->type()->size();
  size_t total_size = s_cont.size() + size_var_ret;
  _func_offset = -size_var_ret;
  _arg_offset = 8;
  
  _symtab.push();

  chooseSegment(TEXT);
  _pf.ALIGN();

  if(node->is_public()){ _pf.GLOBAL(node->id(), _pf.FUNC()); }

  _pf.ALIGN();
  _pf.LABEL(node->id());
  _pf.ENTER(total_size);

  if(node->return_value()){
    node->return_value()->accept(this, lvl);
    itod(node->type(), node->return_value()->type());
    
    if(!isDouble(node->type())) {
      _pf.LOCAL(_func_offset);
      _pf.STORE();
    }
    else{
      _pf.LOCAL(_func_offset);
      _pf.DSTORE();
    }
  }

  if(node->params()){ 
    is_argument=true;
    node->params()->accept(this, lvl); 
    is_argument=false;
  }
  
  node->block()->accept(this, lvl);

  if(!isVoid(node->type())){
    if(!isDouble(node->type())){
      _pf.LOCAL(-size_var_ret);
      _pf.LOAD();
      _pf.POP();
    }
    else{
      _pf.LOCAL(-size_var_ret);
      _pf.DLOAD();
      _pf.DPOP();
    }
  }

  _pf.LEAVE();
  _pf.RET();

  _is_global = true;
  _actual_function = nullptr;
  _symtab.pop();
}


//---------------------------------------------------------------------------

void xpl::postfix_writer::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  if (isInt(node->argument()->type())) {
    _pf.TRASH(4);
  } 
  else if(isString(node->argument()->type())) {
    _pf.TRASH(4);
  } 
  else if(isDouble(node->argument()->type())){
    _pf.TRASH(8);
  }
  else if(isPointer(node->argument()->type())){
    _pf.TRASH(4);
  }
  else if(isVoid(node->argument()->type())){}
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void xpl::postfix_writer::do_print_node(xpl::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->arg()->accept(this, lvl);

  if(isInt(node->arg()->type())){
    _pf.CALL("printi");
    pushUsedFunc("printi");
    _pf.TRASH(4);
  }
  else if(isDouble(node->arg()->type())){
    _pf.CALL("printd");
    pushUsedFunc("printd");
    _pf.TRASH(8);
  }
  else if(isString(node->arg()->type())){
    _pf.CALL("prints");
    pushUsedFunc("prints");
    _pf.TRASH(4);
  }
  else{
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }

  if(node->is_newline()) {
    _pf.CALL("println");
    pushUsedFunc("println");
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_read_node(xpl::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if(isInt(node->type())){ 
    _pf.CALL("readi"); 
    pushUsedFunc("readi");
    _pf.PUSH();
  }
  else if(isDouble(node->type())){ 
    _pf.CALL("readd"); 
    pushUsedFunc("readd");
    _pf.DPUSH();
  }
  else{ 
    std::cout << "foi no read no postfix";
    throw std::string("wrong type in reading"); }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_while_node(xpl::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::string cond = mklbl(++_lbl);
  _next.push_back(cond);
  std::string end = mklbl(++_lbl);
  _stop.push_back(end);

  _pf.LABEL(cond);
  node->condition()->accept(this, lvl);
  _pf.JZ(end);
  node->block()->accept(this, lvl + 2);
  _pf.JMP(cond);
  _pf.LABEL(end);

  _next.pop_back();
  _stop.pop_back();

}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_if_node(xpl::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::string ifend = mklbl(++_lbl);

  node->condition()->accept(this, lvl);
  _pf.JZ(ifend);
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(ifend);

}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_block_node(xpl::block_node * const node, int lvl) {
  _symtab.push();
  if(node->declarations()){ node->declarations()->accept(this, lvl+2); }
  if(node->instructions()){ node->instructions()->accept(this, lvl+2); }
  _symtab.pop();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_indexing_node(xpl::indexing_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->pointer()->accept(this, lvl);

  node->index()->accept(this, lvl);
  
  _pf.INT(node->type()->size());
  _pf.MUL();
  _pf.ADD();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_sweep_up_node(xpl::sweep_up_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::string cond = mklbl(++_lbl);
  std::string inc = mklbl(++_lbl);
  _next.push_back(inc);
  std::string end = mklbl(++_lbl);
  _stop.push_back(end);

  cdk::assignment_node* assign =  new cdk::assignment_node(node->lineno(),node->lvalue(), node->init());
  assign->accept(this, lvl);
  _pf.TRASH(assign->type()->size());

  cdk::rvalue_node* rvalue = new cdk::rvalue_node(node->lineno(), node->lvalue());

  _pf.ALIGN();
  _pf.LABEL(cond);

  cdk::le_node* le = new cdk::le_node(node->lineno(), rvalue, node->condition());
  le->accept(this, lvl);

  _pf.JZ(end);
  node->block()->accept(this, lvl);

  _pf.ALIGN();
  _pf.LABEL(inc);

  cdk::add_node* add = new cdk::add_node(node->lineno(), rvalue, node->increment());
  add->accept(this, lvl);

  assign = new cdk::assignment_node(node->lineno(), node->lvalue(), add);
  assign->accept(this, lvl);  
  _pf.TRASH(assign->type()->size());

  _pf.JMP(cond);
  _pf.ALIGN();
  _pf.LABEL(end);
  _next.pop_back();
  _stop.pop_back();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_sweep_down_node(xpl::sweep_down_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::string cond = mklbl(++_lbl);
  std::string inc = mklbl(++_lbl);
  _next.push_back(inc);
  std::string end = mklbl(++_lbl);
  _stop.push_back(end);

  cdk::assignment_node* assign =  new cdk::assignment_node(node->lineno(),node->lvalue(), node->init());
  assign->accept(this, lvl);
  _pf.TRASH(assign->type()->size());

  cdk::rvalue_node* rvalue = new cdk::rvalue_node(node->lineno(), node->lvalue());

  _pf.ALIGN();
  _pf.LABEL(cond);

  cdk::ge_node* ge = new cdk::ge_node(node->lineno(), rvalue, node->condition());
  ge->accept(this, lvl);
  
  _pf.JZ(end);
  node->block()->accept(this, lvl);

  _pf.ALIGN();
  _pf.LABEL(inc);

  cdk::sub_node* sub = new cdk::sub_node(node->lineno(), rvalue, node->increment());
  sub->accept(this, lvl);

  assign = new cdk::assignment_node(node->lineno(), node->lvalue(), sub);  
  assign->accept(this, lvl);
  _pf.TRASH(assign->type()->size());

  _pf.JMP(cond);
  _pf.ALIGN();
  _pf.LABEL(end);
  
  _next.pop_back();
  _stop.pop_back();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_function_decl_node(xpl::function_decl_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_function_call_node(xpl::function_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  xpl::stack_size_counter* s_cnt = new xpl::stack_size_counter(_compiler);
  size_t func_size = s_cnt->size();

  const std::string & id = node->id();
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(id);

  if(symbol==nullptr) throw std::string(node->id() + " not found!");

  if(node->type()->name()!=symbol->type()->name() || node->id()!=symbol->name() || !symbol->is_function()){
     throw std::string(id + " is not compatible");
  }

  if(node->args() && (symbol->params()).size()==(node->args())->size()){
    std::vector<basic_type*> params = symbol->params();
    cdk::sequence_node *args = node->args();
    size_t p_size = params.size()-1, a_size = 0;

    while(p_size>=0 && a_size<args->size()){
      cdk::expression_node* arg=dynamic_cast<cdk::expression_node*>(args->node(a_size));
      basic_type* b_type = params[p_size];
       
      if(arg==nullptr || b_type==nullptr){ 
        throw std::string("incompatibility in " + node->id() + " wrong type of arguments"); 
      }

      arg->accept(this, lvl);
      itod(b_type, arg->type());

      a_size++;
      p_size--;
    }
  }

  _pf.CALL(symbol->name());
  _pf.TRASH(func_size);

  if(symbol->is_extern()){ pushUsedFunc(symbol->name()); }

  if(isDouble(node->type())){
    _pf.DPUSH();
  }
  else if(!isVoid(node->type())){
    _pf.PUSH();
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_variable_decl_node(xpl::variable_decl_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _var_is_public = node->is_public();
  std::shared_ptr<xpl::symbol> symbol = _symtab.find_local(node->id());

  if(symbol==nullptr) throw std::string(node->id() + " not found!");

  if(is_argument){
    symbol->offset(_arg_offset);
    _arg_offset += symbol->type()->size();
    symbol->is_global(false);
  }      
  else if(_is_global){
    _global_var_lbl = node->id();
    if (!node->initial_value()) {
        chooseSegment(BSS);
        _pf.ALIGN();
        _pf.LABEL(node->id());
        _pf.BYTE(node->type()->size());
        chooseSegment(_prevSeg);
    } 
    else{
      if (isDouble(node->type()) && isInt(node->initial_value()->type())){
        cdk::integer_node *int_node=dynamic_cast<cdk::integer_node*>(node->initial_value());

        cdk::double_node* i2d_node = new cdk::double_node(int_node->lineno(), int_node->value());

        i2d_node->accept(this, lvl);
      }
      else {
        node->initial_value()->accept(this, lvl);
      }
    }
    if(node->is_use()){ pushUsedFunc(_global_var_lbl); }
  }
  else{
      symbol->is_global(false);
      _func_offset -= node->type()->size();
      symbol->offset(_func_offset);
    if (node->initial_value()){

      node->initial_value()->accept(this, lvl);
      itod(node->type(), node->initial_value()->type());

      if(!isDouble(node->type())){
        _pf.DUP();
        _pf.LOCAL(symbol->offset());
        _pf.STORE();
      }
      else{
       _pf.DDUP();
       _pf.LOCAL(symbol->offset()); 
       _pf.DSTORE();
      }
    }
    else if(node->is_use()){ pushUsedFunc(node->id()); }
  }
}


//---------------------------------------------------------------------------

void xpl::postfix_writer::do_memory_alloc_node(xpl::memory_alloc_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);

  _pf.INT(node->type()->subtype()->size()); 
  _pf.MUL();

  _pf.ALLOC();
  _pf.SP();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_memory_address_node(xpl::memory_address_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_null_node(xpl::null_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.INT(0);
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_return_node(xpl::return_node * const node, int lvl) {
  if(_actual_function != nullptr){
    if(!isVoid(_actual_function->type())){

      _pf.LOCAL(_actual_function->offset());

      if(!isDouble(_actual_function->type())){
        _pf.LOAD();
        _pf.POP();
      }
      else{
        _pf.DLOAD();
        _pf.DPOP();
      }
    }
    _pf.LEAVE();
    _pf.RET();
  }  
  else throw std::string("return instruction outside a fuction");
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_next_node(xpl::next_node * const node, int lvl) {
  if(_next.empty()){
    throw std::string("next instruction not in a cycle");
  }  
  else{ 
    _pf.JMP(_next.back());
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_stop_node(xpl::stop_node * const node, int lvl) {
  if(_stop.empty()){
    throw std::string("stop instruction not in a cycle");
  }  
  else{ 
    _pf.JMP(_stop.back());  
  }  
}
