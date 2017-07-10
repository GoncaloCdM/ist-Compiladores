// $Id: function_def_node.h,v 1.10 2017/03/24 15:06:55 ist181151 Exp $ -*- c++ -*-
#ifndef __XPL_FUNCTIONDEFNODE_H__
#define __XPL_FUNCTIONDEFNODE_H__

#include <cdk/ast/basic_node.h>

namespace xpl {
  /**
   * Class for describing function_def nodes.
   */
  class function_def_node: public cdk::basic_node {
    bool _is_public;
    basic_type * _type;
    std::string _id;
    cdk::sequence_node * _parameters;
    cdk::expression_node * _default_return;
    block_node * _body;


  public:
    inline function_def_node(int lineno, bool is_public, basic_type* type, std::string* id, 
        cdk::sequence_node* args, cdk::expression_node* value, block_node* block) :
        cdk::basic_node(lineno), _is_public(is_public), _type(type), _id(*id),
        _parameters(args), _default_return(value), _body(block) {}

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_def_node(this, level);
    }

  };

} // xpl

#endif
