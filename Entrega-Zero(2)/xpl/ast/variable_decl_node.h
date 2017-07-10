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

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_decl_node(this, level);
    }

  };

} // xpl

#endif
