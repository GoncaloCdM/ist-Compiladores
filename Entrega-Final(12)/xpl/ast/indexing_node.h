#ifndef __XPL_INDEXING_NODE_H__
#define __XPL_INDEXING_NODE_H__

#include "cdk/ast/lvalue_node.h"
#include "cdk/ast/expression_node.h"

namespace xpl {
  /**
   * Class for describing the indexing node
   */
  class indexing_node: public cdk::lvalue_node {
    cdk::expression_node *_ptr, *_index;

    public:
      inline indexing_node(int lineno, cdk::expression_node *ptr, 
        cdk::expression_node *index) :
        cdk::lvalue_node (lineno),  _ptr(ptr), _index(index) {}

    public:
      inline cdk::expression_node * pointer() { 
        return _ptr; 
      }
      
      inline cdk::expression_node * index() {
        return _index; 
      }

      void accept(basic_ast_visitor *sp, int level) {
        sp->do_indexing_node(this, level);
      }

  };

} // xpl

#endif