// $Id: sweep_up_node.h,v 1.7 2017/04/16 23:38:24 ist181151 Exp $ -*- c++ -*-
#ifndef __XPL_SWEEP_UP_NODE_H__
#define __XPL_SWEEP_UP_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/integer_node.h>

namespace xpl {
  /**
   * Class for describing sweep_up-cycle nodes.
   */
  class sweep_up_node: public cdk::basic_node {
    cdk::lvalue_node *_lvalue;
    cdk::expression_node *_init, *_condition, *_increment;
    cdk::basic_node *_block;

  public:
    inline sweep_up_node(int lineno, cdk::lvalue_node *lvalue, cdk::expression_node *init, 
    cdk::expression_node *condition, cdk::expression_node *increment, cdk::basic_node *block) :
    basic_node(lineno), _lvalue(lvalue), _init(init), _condition(condition),
    _increment(increment), _block(block) {}

  public:
    inline cdk::lvalue_node* lvalue() {
      return _lvalue;
    }

    inline cdk::expression_node* init() {
      return _init;
    }

    inline cdk::expression_node* condition() {
      return _condition;
    }

    inline cdk::expression_node* increment() {
      return _increment;
    }

    inline cdk::basic_node* block(){
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_sweep_up_node(this, level);
    }

  };

} // xpl

#endif
