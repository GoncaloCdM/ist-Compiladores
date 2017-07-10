#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

std::string ttos(basic_type *type) {
  if(type->name()==basic_type::TYPE_INT) return "int";
  if(type->name()==basic_type::TYPE_STRING) return "string";
  if(type->name()==basic_type::TYPE_VOID) return "void";
  if(type->name()==basic_type::TYPE_DOUBLE) return "real";
  if(type->name()==basic_type::TYPE_POINTER){ return (ttos(type->subtype()) + "*"); }
  return "invalid";
}

std::string btos(bool b) {
  if(0) return "false";
  if(1) return "true";
  return "invalid";
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  
    for (size_t i = 0; i < node->size(); i++)
        node->node(i)->accept(this, lvl + 2);
  
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  process_literal(node, lvl);
}


void xpl::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  process_literal(node, lvl);
}


void xpl::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

inline void xpl::xml_writer::do_unary_expression(cdk::unary_expression_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}


void xpl::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  do_unary_expression(node, lvl);
}


void xpl::xml_writer::do_not_node(cdk::not_node * const node, int lvl){
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  do_unary_expression(node, lvl);
}


void xpl::xml_writer::do_identity_node(xpl::identity_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  do_unary_expression(node, lvl);
}


void xpl::xml_writer::do_memory_alloc_node(xpl::memory_alloc_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  do_unary_expression(node, lvl);
}


void xpl::xml_writer::do_memory_address_node(xpl::memory_address_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  do_unary_expression(node, lvl);
}


void xpl::xml_writer::do_indexing_node(xpl::indexing_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  openTag("pointer", lvl+2);
  if(node->pointer() != nullptr){ node->pointer()->accept(this, lvl+4);}
  closeTag("pointer", lvl+2);
  openTag("index", lvl+2);
  if(node->index() != nullptr){ node->index()->accept(this, lvl+4);}
  closeTag("index", lvl+2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

inline void xpl::xml_writer::do_binary_expression(cdk::binary_expression_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_or_node(cdk::or_node * const node, int lvl){
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_and_node(cdk::and_node * const node, int lvl){
  do_binary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl);
  reset_new_symbol();
  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------
void xpl::xml_writer::do_variable_decl_node(xpl::variable_decl_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */

  os() << std::string(lvl, ' ') << "<" << node->label() 
  << " is_public='" << btos(node->is_public()) << "' is_use='" 
  << btos(node->is_use()) << "' type='" << ttos(node->type()) 
  << "' id='" << node->id() << "'>" << std::endl;

  openTag("value", lvl+2);
  if(node->initial_value() != nullptr) { node->initial_value()->accept(this, lvl+4);}
  closeTag("value", lvl+2);

  closeTag(node, lvl);
}

void xpl::xml_writer::do_function_decl_node(xpl::function_decl_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */

  os() << std::string(lvl, ' ') << "<" << node->label() 
  << " is_public='" << btos(node->is_public()) << "' is_use='" 
  << btos(node->is_use()) << "' type='" << ttos(node->type()) 
  << "' id='" << node->id() << "'>" << std::endl;
  openTag("params", lvl+2);
  if(node->params() != nullptr) { node->params()->accept(this, lvl+4);}
  closeTag("params", lvl+2);  
  closeTag(node, lvl);
}

void xpl::xml_writer::do_function_def_node(xpl::function_def_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */

os() << std::string(lvl, ' ') << "<" << node->label() 
  << " is_public='" << btos(node->is_public()) 
  << "' type='" << ttos(node->type()) 
  << "' id='" << node->id() << "'>" << std::endl;
  openTag("params", lvl+2); 
  if(node->params()!=nullptr) { node->params()->accept(this, lvl + 4);}
  closeTag("params", lvl+2);
  openTag("return_value", lvl+2);
  if(node->return_value()!=nullptr) { node->return_value()->accept(this, lvl + 4);}
  closeTag("return_value", lvl+2);
  openTag("block", lvl+2); node->block()->accept(this, lvl + 4); closeTag("block", lvl+2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_function_call_node(xpl::function_call_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */

  os() << std::string(lvl, ' ') << "<" << node->label() << " id='" << node->id() << "'>" << std::endl;
  openTag("args", lvl+2);
  if(node->args()!=nullptr) { node->args()->accept(this, lvl+4);}
  closeTag("args", lvl+2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_print_node(xpl::print_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */

  os() << std::string(lvl, ' ') << "<" << node->label() 
  << " is_newline='" << btos(node->is_newline()) << "'>" << std::endl;
  openTag("arg", lvl+2);
  if(node->arg()!=nullptr) { node->arg()->accept(this, lvl + 4);}
  closeTag("arg", lvl+2); 
  closeTag(node, lvl);
}

void xpl::xml_writer::do_block_node(xpl::block_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  openTag("decls", lvl + 2);
  if(node->declarations() != nullptr) { node->declarations()->accept(this, lvl + 4);}
  closeTag("decls", lvl + 2);
  openTag("instrs", lvl + 2); 
  if(node->instructions() != nullptr) { node->instructions()->accept(this, lvl + 4);}
  closeTag("instrs", lvl + 2); 
  closeTag(node, lvl);
}

void xpl::xml_writer::do_if_node(xpl::if_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}


void xpl::xml_writer::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_while_node(xpl::while_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}


void xpl::xml_writer::do_sweep_up_node(xpl::sweep_up_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  openTag("leftval", lvl+2);
  node->lvalue()->accept(this, lvl+4);
  closeTag("leftval", lvl+2);
  openTag("initiation", lvl+2);
  node->init()->accept(this, lvl+4);
  closeTag("initiation", lvl+2);
  openTag("condition", lvl+2); 
  node->condition()->accept(this, lvl+4);
  closeTag("condition", lvl+2);
  openTag("increment", lvl+2);
  node->increment()->accept(this, lvl+4);
  closeTag("increment", lvl+2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}


void xpl::xml_writer::do_sweep_down_node(xpl::sweep_down_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  openTag("leftval", lvl+2);
  node->lvalue()->accept(this, lvl+4);
  closeTag("leftval", lvl+2);
  openTag("initiation", lvl+2);
  node->init()->accept(this, lvl+4);
  closeTag("initiation", lvl+2);
  openTag("condition", lvl+2); 
  node->condition()->accept(this, lvl+4);
  closeTag("condition", lvl+2);
  openTag("increment", lvl+2);
  node->increment()->accept(this, lvl+4);
  closeTag("increment", lvl+2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_read_node(xpl::read_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  closeTag(node, lvl);
}
void xpl::xml_writer::do_return_node(xpl::return_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_stop_node(xpl::stop_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_null_node(xpl::null_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_next_node(xpl::next_node * const node, int lvl) {
  /* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */
  openTag(node, lvl);
  closeTag(node, lvl);
}

/* TODO 3rd deliver ASSERT_SAFE_EXPRESSIONS; */

