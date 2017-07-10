#ifndef __XPL_VARIABLEDECLNODE_H__
#define __XPL_VARIABLEDECLNODE_H__

#include <cdk/ast/expression_node.h>

namespace xpl {
  /**
   * Class for describing variable declaration nodes.
   */
  class variable_decl_node: public cdk::basic_node {
    bool _is_public;
    bool _is_use;
    basic_type * _type;
    std::string _id;
    cdk::expression_node * _initial_value;
    
  public:
    inline variable_decl_node(int lineno, basic_type * type, bool is_public, bool is_use,
      std::string * id, cdk::expression_node * value):
      cdk::basic_node(lineno), _is_public(is_public), _is_use(is_use), 
      _type(type), _id(*id), _initial_value(value) {}

  public:
    inline bool is_public() {
      return _is_public;
    }

    inline bool is_use() {
      return _is_use;
    }

    inline basic_type* type() {
      return _type;
    } 

    inline std::string id() {
      return _id;
    }

    inline cdk::expression_node* initial_value() {
      return _initial_value;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_decl_node(this, level);
    }

  };

} // xpl

#endif
