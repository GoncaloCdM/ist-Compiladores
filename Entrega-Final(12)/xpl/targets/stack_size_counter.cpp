#include "targets/stack_size_counter.h"
#include "ast/all.h"

void xpl::stack_size_counter::do_integer_node(cdk::integer_node * const node, int lvl){}
void xpl::stack_size_counter::do_string_node(cdk::string_node * const node, int lvl){}
void xpl::stack_size_counter::do_double_node(cdk::double_node * const node, int lvl){}
void xpl::stack_size_counter::do_neg_node(cdk::neg_node * const node, int lvl){}
void xpl::stack_size_counter::do_add_node(cdk::add_node * const node, int lvl){}
void xpl::stack_size_counter::do_sub_node(cdk::sub_node * const node, int lvl){}
void xpl::stack_size_counter::do_mul_node(cdk::mul_node * const node, int lvl){}
void xpl::stack_size_counter::do_div_node(cdk::div_node * const node, int lvl){}
void xpl::stack_size_counter::do_mod_node(cdk::mod_node * const node, int lvl){}
void xpl::stack_size_counter::do_lt_node(cdk::lt_node * const node, int lvl){}
void xpl::stack_size_counter::do_le_node(cdk::le_node * const node, int lvl){}
void xpl::stack_size_counter::do_ge_node(cdk::ge_node * const node, int lvl){}
void xpl::stack_size_counter::do_gt_node(cdk::gt_node * const node, int lvl){}
void xpl::stack_size_counter::do_ne_node(cdk::ne_node * const node, int lvl){}
void xpl::stack_size_counter::do_eq_node(cdk::eq_node * const node, int lvl){}
void xpl::stack_size_counter::do_or_node(cdk::or_node * const node, int lvl){}
void xpl::stack_size_counter::do_and_node(cdk::and_node * const node, int lvl){}
void xpl::stack_size_counter::do_not_node(cdk::not_node * const node, int lvl){}
void xpl::stack_size_counter::do_identifier_node(cdk::identifier_node * const node, int lvl){}
void xpl::stack_size_counter::do_rvalue_node(cdk::rvalue_node * const node, int lvl){}
void xpl::stack_size_counter::do_assignment_node(cdk::assignment_node * const node, int lvl){}
void xpl::stack_size_counter::do_evaluation_node(xpl::evaluation_node * const node, int lvl){}
void xpl::stack_size_counter::do_print_node(xpl::print_node * const node, int lvl){}
void xpl::stack_size_counter::do_read_node(xpl::read_node * const node, int lvl){}
void xpl::stack_size_counter::do_while_node(xpl::while_node * const node, int lvl){}
void xpl::stack_size_counter::do_if_node(xpl::if_node * const node, int lvl){}
void xpl::stack_size_counter::do_if_else_node(xpl::if_else_node * const node, int lvl){}
void xpl::stack_size_counter::do_return_node(xpl::return_node * const node, int lvl){}
void xpl::stack_size_counter::do_next_node(xpl::next_node * const node, int lvl){}
void xpl::stack_size_counter::do_stop_node(xpl::stop_node * const node, int lvl){}
void xpl::stack_size_counter::do_indexing_node(xpl::indexing_node * const node, int lvl){}
void xpl::stack_size_counter::do_function_decl_node(xpl::function_decl_node * const node, int lvl){}
void xpl::stack_size_counter::do_identity_node(xpl::identity_node * const node, int lvl){}
void xpl::stack_size_counter::do_memory_alloc_node(xpl::memory_alloc_node * const node, int lvl){}
void xpl::stack_size_counter::do_memory_address_node(xpl::memory_address_node * const node, int lvl){}
void xpl::stack_size_counter::do_null_node(xpl::null_node * const node, int lvl){}

void xpl::stack_size_counter::do_sequence_node(cdk::sequence_node * const node, int lvl){
  for(cdk::basic_node * son_node : node->nodes()){
    son_node->accept(this, lvl);
  }
}

void xpl::stack_size_counter::do_function_def_node(xpl::function_def_node * const node, int lvl){
  node->block()->accept(this, lvl);
}

void xpl::stack_size_counter::do_block_node(xpl::block_node * const node, int lvl){
  if(node->declarations()){ node->declarations()->accept(this, lvl); }
  if(node->instructions()){ node->instructions()->accept(this, lvl); }
}

void xpl::stack_size_counter::do_sweep_up_node(xpl::sweep_up_node * const node, int lvl){
  node->init()->accept(this,lvl);
}

void xpl::stack_size_counter::do_sweep_down_node(xpl::sweep_down_node * const node, int lvl){
  node->init()->accept(this,lvl);
}

void xpl::stack_size_counter::do_function_call_node(xpl::function_call_node * const node, int lvl){
  if(node->args()){ node->args()->accept(this, lvl); }
}

void xpl::stack_size_counter::do_variable_decl_node(xpl::variable_decl_node * const node, int lvl){
  _size += node->type()->size();
}
