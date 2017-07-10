#ifndef __XPL_FUNCTIONDECLNODE_H__
#define __XPL_FUNCTIONDECLNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace xpl {
  /**
   * Class for describing function_decl nodes.
   */
  class function_decl_node: public cdk::basic_node {
    bool _is_public;
    bool _is_use;
    basic_type * _type;
    std::string _id;
    cdk::sequence_node * _parameters;
    
  public:
    inline function_decl_node(int lineno, bool is_public, bool is_use, basic_type * type, 
      std::string* id, cdk::sequence_node * args):
      cdk::basic_node(lineno), _is_public(is_public), _is_use(is_use), _type(type), 
      _id(*id), _parameters(args) {}

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

    inline cdk::sequence_node* params() {
      return _parameters;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_decl_node(this, level);
    }

  };

} // xpl

#endif
