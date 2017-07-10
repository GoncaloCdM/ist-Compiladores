
#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void xpl::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void xpl::type_checker::do_double_node(cdk::double_node * const node, int lvl){
  ASSERT_UNSPEC;
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}

void xpl::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

void xpl::type_checker::do_null_node(xpl::null_node * const node, int lvl){
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
  node->type()->_subtype = new basic_type(0, basic_type::TYPE_UNSPEC);
}

//---------------------------------------------------------------------------

inline void xpl::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->argument()->accept(this, lvl + 2);
  typeDealer(node->argument()->type());

  if(isInt(node->argument()->type()) || isDouble(node->argument()->type())){
    node->type(makeType(node->argument()->type())); 
  }
  else throw std::string("wrong type in argument of unary expression");
}

void xpl::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void xpl::type_checker::do_identity_node(xpl::identity_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void xpl::type_checker::do_not_node(cdk::not_node * const node, int lvl){
  ASSERT_UNSPEC;

  node->argument()->accept(this, lvl + 2);

  if(!isInt(node->argument()->type())){ 
    throw std::string("wrong type in not operator");
  }   
}

void xpl::type_checker::do_memory_alloc_node(xpl::memory_alloc_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->argument()->accept(this, lvl);
  typeDealer(node->argument()->type());

  if (!isInt(node->argument()->type())){
    throw std::string("wrong type in argument of alloc expression");
  }

  node->type(new basic_type(4, basic_type::TYPE_POINTER));

  node->type()->_subtype = new basic_type(0, basic_type::TYPE_UNSPEC);
}

void xpl::type_checker::do_memory_address_node(xpl::memory_address_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->argument()->accept(this, lvl);
  typeDealer(node->argument()->type());

  node->type(new basic_type(4, basic_type::TYPE_POINTER));

  node->type()->_subtype=makeType(node->argument()->type());
}

//---------------------------------------------------------------------------

inline void xpl::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);  
  node->right()->accept(this, lvl + 2);
  typeDealer(node->left()->type(), node->right()->type());

  if(!isInt(node->left()->type()) && !isPointer(node->left()->type()) && !isDouble(node->left()->type())){
    throw std::string("wrong type in left argument of binary expression");
  }
  if(!isInt(node->right()->type()) && !isPointer(node->right()->type()) && !isDouble(node->right()->type())){
    throw std::string("wrong type in right argument of binary expression");
  }

}

void xpl::type_checker::processBinAdditiveExp(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;

  processBinaryExpression(node, lvl);

  if(isInt(node->left()->type()) && isPointer(node->right()->type())){
    node->type(makeType(node->right()->type()));
  }
  else if(isPointer(node->left()->type()) && isInt(node->right()->type())){
    node->type(makeType(node->left()->type()));
  }
  else if(isInt(node->left()->type()) && isInt(node->right()->type())){
    node->type(makeType(node->left()->type()));
  }
  else if((isDouble(node->left()->type()) && isInt(node->right()->type())) ||
          (isInt(node->left()->type()) && isDouble(node->right()->type())) ||
          (isDouble(node->left()->type()) && isDouble(node->right()->type()))){
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  }
  else throw std::string("error in additive expression  left");
  
}

void xpl::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_UNSPEC;
  processBinAdditiveExp(node, lvl);
}

void xpl::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_UNSPEC;
  processBinAdditiveExp(node, lvl);

  if(isPointer(node->left()->type()) && isPointer(node->right()->type())){
    if(node->left()->type()->subtype() && node->right()->type()->subtype() && node->left()->type()->subtype()->name()==node->right()->type()->subtype()->name()){
      node->type(new basic_type(4, basic_type::TYPE_INT));  
    }
    else throw std::string("wrong type in sub expression of pointers");
  }
  else if(isInt(node->left()->type()) && isPointer(node->right()->type())){
    throw std::string("cannot subtract a pointer to a int");
  }
}

//---------------------------------------------------------------------------------------------------------------------------

void xpl::type_checker::processBinMultiplicativeExp(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;

  processBinaryExpression(node, lvl);

  if(isPointer(node->left()->type()) || isPointer(node->right()->type())){
    throw std::string("pointer type in multiplicative expression");
  }
  else if(isInt(node->left()->type()) && isInt(node->right()->type())){
    node->type(makeType(node->left()->type()));
  }
  else if((isDouble(node->left()->type()) && isInt(node->right()->type())) ||
          (isInt(node->left()->type()) && isDouble(node->right()->type())) ||
          (isDouble(node->left()->type()) && isDouble(node->right()->type()))){
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  }
  else throw std::string("wrong type in multiplicative expression");

}

void xpl::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinMultiplicativeExp(node, lvl); 
}
void xpl::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processBinMultiplicativeExp(node, lvl); 
}

void xpl::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(!isInt(node->left()->type()) || !isInt(node->right()->type())){
    throw std::string("wrong type in mod expression");
  }

  node->type(makeType(node->left()->type()));
}

//---------------------------------------------------------------------------------------------------------------------------
void xpl::type_checker::processBinInequalityExp(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(!isInt(node->left()->type()) && !isDouble(node->left()->type())){
    throw std::string("wrong type in left argument of binary inequality expression");
  }
  if(!isInt(node->right()->type()) && !isDouble(node->right()->type())){
    throw std::string("wrong type in right argument of binary inequality expression");
  }

  node->type(new basic_type(4, basic_type::TYPE_INT));
 
}

void xpl::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinInequalityExp(node, lvl);
}
void xpl::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processBinInequalityExp(node, lvl);
}
void xpl::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinInequalityExp(node, lvl);
}
void xpl::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinInequalityExp(node, lvl);
}

//---------------------------------------------------------------------------------------------------------------------------

void xpl::type_checker::processBinEqualityExp(cdk::binary_expression_node * const node, int lvl) {

  processBinaryExpression(node, lvl);

  node->type(new basic_type(4, basic_type::TYPE_INT)); // 0 is false and !0 is true

}

void xpl::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinEqualityExp(node, lvl);
}
void xpl::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinEqualityExp(node, lvl);
}

//---------------------------------------------------------------------------------------------------------------------------
void xpl::type_checker::processBinLogicalExp(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  
  if(!isInt(node->left()->type()) || !isInt(node->right()->type())){
    throw std::string("wrong type in logical expression");
  }
  node->type(makeType(node->left()->type()));
}

void xpl::type_checker::do_or_node(cdk::or_node * const node, int lvl){
  processBinLogicalExp(node, lvl);
}
void xpl::type_checker::do_and_node(cdk::and_node * const node, int lvl){
  processBinLogicalExp(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_UNSPEC;

  std::shared_ptr<xpl::symbol> symbol = _symtab.find(node->name());

  if(symbol != nullptr){
    node->type(makeType(symbol->type()));
  }
  else{
    throw node->name() + "undeclared";
  }
}

void xpl::type_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl);
  
  node->type(makeType(node->lvalue()->type()));
}

void xpl::type_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {

  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl);
  node->rvalue()->accept(this, lvl);

  typeDealer(node->lvalue()->type(), node->rvalue()->type());

  if(!isInt(node->lvalue()->type()) && !isPointer(node->lvalue()->type()) && !isDouble(node->lvalue()->type()) && !isString(node->lvalue()->type())){
    throw std::string("wrong type in left argument of assignment");
  }
  if(!isInt(node->rvalue()->type()) && !isPointer(node->rvalue()->type()) && !isDouble(node->rvalue()->type()) && !isString(node->rvalue()->type())){
    throw std::string("wrong type in right argument of assignment");
  }

  if(node->lvalue()->type()->name() != node->rvalue()->type()->name()){
    if(isDouble(node->lvalue()->type()) && isInt(node->rvalue()->type())){
      node->type(makeType(node->lvalue()->type()));
    }
    else throw std::string("wrong type in left argument of assignment expression - left type: "  + std::to_string(node->lvalue()->type()->name()) + " right type: " + std::to_string(node->rvalue()->type()->name()));
  }
  else if(isPointer(node->lvalue()->type()) && isPointer(node->rvalue()->type()) && node->lvalue()->type()->name()!=node->rvalue()->type()->name()){
    throw std::string("wrong type in left argument of assignment expression - left subtype: "  + std::to_string(node->lvalue()->type()->subtype()->name()) + " right subtype: " + std::to_string(node->rvalue()->type()->subtype()->name()));
  }
  else{
    node->type(makeType(node->lvalue()->type()));
  }
  
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);

  typeDealer(node->argument()->type());
}

void xpl::type_checker::do_print_node(xpl::print_node * const node, int lvl) {
  node->arg()->accept(this, lvl + 2);

  typeDealer(node->arg()->type());
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_read_node(xpl::read_node * const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->type(new basic_type(0, basic_type::TYPE_UNSPEC));
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_while_node(xpl::while_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}
void xpl::type_checker::do_if_node(xpl::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}
void xpl::type_checker::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void xpl::type_checker::do_indexing_node(xpl::indexing_node * const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->pointer()->accept(this, lvl);
  node->index()->accept(this,lvl);
  
  typeDealer(node->pointer()->type(), node->index()->type());

  if(!isPointer(node->pointer()->type()) && !isInt(node->index()->type())){
    throw std::string("wrong type in arguments in" + node->label());
  }
  
  node->type(makeType(node->pointer()->type()->subtype()));

}

void xpl::type_checker::do_sweep_up_node(xpl::sweep_up_node * const node, int lvl){
  if(node->lvalue()){ node->lvalue()->accept(this, lvl); }
  if(node->init()){ node->init()->accept(this, lvl); }
  if(node->condition()){ node->condition()->accept(this, lvl); }
  if(node->increment()){ node->increment()->accept(this, lvl); }  
}

void xpl::type_checker::do_sweep_down_node(xpl::sweep_down_node * const node, int lvl){
  if(node->lvalue()){ node->lvalue()->accept(this, lvl); }
  if(node->init()){ node->init()->accept(this, lvl); }
  if(node->condition()){ node->condition()->accept(this, lvl); }
  if(node->increment()){ node->increment()->accept(this, lvl); } 
}

void xpl::type_checker::do_function_decl_node(xpl::function_decl_node * const node, int lvl) {

  if(node->id() == "xpl"){
    node->id("_main");
  }
  else if(node->id() == "_main"){
    node->id("._main");
  }

  std::shared_ptr<xpl::symbol> symbol = _symtab.find(node->id());
  if(!symbol){
    std::vector<basic_type*> vec;
    size_t i=0;
    while(i<node->params()->size()){
      vec.push_back(makeType(dynamic_cast<xpl::variable_decl_node*>(node->params()->node(i))->type()));
      i++;
    }

    symbol = std::make_shared<xpl::symbol>(makeType(node->type()), node->id(), -node->type()->size(), true, node->is_public(), node->is_use(), 
        true, vec);
    _symtab.insert(node->id(), symbol);
  }
  else throw std::string(node->id() + "redeclared");
}

void xpl::type_checker::do_function_def_node(xpl::function_def_node * const node, int lvl) {

  if(node->return_value()){
    node->return_value()->accept(this, lvl);
  }

  // same as func decl bah - who the hell cares??
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(node->id());
  if(!symbol){
    if(node->id() == "xpl"){
       node->id("_main");
    }   
    else if(node->id() == "_main"){
       node->id("._main");
    }
     
    std::vector<basic_type*> vec;
    size_t i=0;
    while(i<node->params()->size()){
      vec.push_back(makeType(dynamic_cast<xpl::variable_decl_node*>(node->params()->node(i))->type()));
      i++;
    }

    symbol = std::make_shared<xpl::symbol>(makeType(node->type()), node->id(), -node->type()->size(), true, node->is_public(), false, true, vec);
    _symtab.insert(node->id(), symbol);  
  }
  else if( node->params() && (symbol->params()).size()==(node->params())->size() && !(symbol->params()).empty()){
    cdk::sequence_node *args = node->params();
    std::vector<basic_type*> params = symbol->params();

    size_t p_size = 0;

    while(p_size<params.size()){
      xpl::variable_decl_node* arg=dynamic_cast<xpl::variable_decl_node*>(args->node(p_size));
      basic_type* b_type = params[p_size];
      
      if(arg==nullptr || b_type==nullptr){ throw std::string("incompatibility in " + node->id() + " wrong type of arguments"); }

      if(b_type->name()!=arg->type()->name()){ 
        throw std::string("Diff in arguments types  param type: "+ std::to_string(b_type->size()) + " arg type: " + std::to_string(arg->type()->size()));
      }
      else if(isPointer(b_type) && isPointer(arg->type()) && b_type->subtype()->name()!=arg->type()->subtype()->name()){
        throw std::string("Diff in arguments types  param subtype: "+ std::to_string(b_type->subtype()->size()) + " arg subtype: " + std::to_string(arg->type()->subtype()->size())); 
      }

      p_size++;

    }
  } 
  else if(node->type()->name()!=symbol->type()->name() || !symbol->is_function()){
    throw std::string(node->id() + " redefined");
  }
}

void xpl::type_checker::do_function_call_node(xpl::function_call_node * const node, int lvl) {
  ASSERT_UNSPEC;
  
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(node->id());
  if (!symbol){ 
    throw std::string ("symbol " + node->id() + " undeclared");
  }

  if(!symbol->is_function()){
    throw std::string(node->id() + " is not a function");
  }

  if( node->args() && (symbol->params()).size()==(node->args())->size()){
    cdk::sequence_node *args = node->args();
    std::vector<basic_type*> params = symbol->params();

    size_t p_size = params.size()-1, a_size = 0;

    while(p_size>=0 && a_size<args->size()){
      cdk::expression_node* arg=dynamic_cast<cdk::expression_node*>(args->node(a_size));
      basic_type* b_type = params[p_size];
  
      
      if(arg==nullptr || b_type==nullptr){ throw std::string("incompatibility in " + node->id() + " wrong type of arguments"); }

      arg->accept(this, lvl);

      typeDealer(b_type, arg->type());

      if(b_type->name()!=arg->type()->name() && !(isDouble(b_type) && isInt(arg->type()))){ 
        throw std::string("Diff in arguments types  param type: "+ std::to_string(b_type->size()) + " arg type: " + std::to_string(arg->type()->size()));
      }
      else if(isPointer(b_type) && isPointer(arg->type()) && b_type->subtype()->name()!=arg->type()->subtype()->name()){
        throw std::string("Diff in arguments types  param subtype: "+ std::to_string(b_type->subtype()->size()) + " arg subtype: " + std::to_string(arg->type()->subtype()->size())); 
      }

      a_size++;
      p_size--;

    }
  }
  else if(symbol->params().empty() || node->args()!=nullptr){
    throw std::string(node->id() + " failed, incompatibility in types");
  }

  node->type(makeType(symbol->type()));

}

void xpl::type_checker::do_variable_decl_node(xpl::variable_decl_node * const node, int lvl) {

  if(node->initial_value()){ 
    node->initial_value()->accept(this, lvl);
    typeDealer(node->type(), node->initial_value()->type());
  }
  
  std::shared_ptr<xpl::symbol> symbol = _symtab.find_local(node->id());
  if (!symbol){
    symbol = std::make_shared<xpl::symbol>(makeType(node->type()), node->id(), -node->type()->size(), true, node->is_public(), node->is_use(), false);
    _symtab.insert(node->id(), symbol);
  }
  else throw std::string(node->id() + " redeclared");

    
}

void xpl::type_checker::do_return_node(xpl::return_node * const node, int lvl){
}
void xpl::type_checker::do_next_node(xpl::next_node * const node, int lvl){
}
void xpl::type_checker::do_stop_node(xpl::stop_node * const node, int lvl){
}
void xpl::type_checker::do_block_node(xpl::block_node * const node, int lvl){
}