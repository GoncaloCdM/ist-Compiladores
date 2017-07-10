// $Id: function_def_node.h,v 1.15 2017/05/22 14:06:00 ist181151 Exp $ -*- c++ -*-
#ifndef __XPL_FUNCTIONDEFNODE_H__
#define __XPL_FUNCTIONDEFNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

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
    inline bool is_public() {
      return _is_public;
    }

    inline basic_type* type() {
      return _type;
    } 

    inline std::string id() {
      return _id;
    }

    inline void id(std::string id){
      _id=id;
    }

    inline cdk::sequence_node* params() {
      return _parameters;
    }

    inline cdk::expression_node* return_value() {
      return _default_return;
    }

    inline block_node* block() {
      return _body;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_def_node(this, level);
    }

  };

} // xpl

#endif
