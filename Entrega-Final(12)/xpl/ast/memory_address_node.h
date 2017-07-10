#ifndef __XPL_MEMORY_ADDRESS_NODE_H__
#define __XPL_MEMORY_ADDRESS_NODE_H__

#include <cdk/ast/unary_expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace xpl {
  /**
   * Class for describing the memory address node
   */
  class memory_address_node: public cdk::unary_expression_node {
    public:
      inline memory_address_node(int lineno, cdk::lvalue_node *arg) :
        cdk::unary_expression_node(lineno, arg) {}

    public:
      void accept(basic_ast_visitor *sp, int level) {
        sp->do_memory_address_node(this, level);
      }

  };

} // xpl

#endif