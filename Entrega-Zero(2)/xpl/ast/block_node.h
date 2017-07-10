#ifndef __XPL_BLOCK_NODE_H__
#define __XPL_BLOCK_NODE_H__

#include <cdk/ast/sequence_node.h>

namespace xpl {
  /**
   * Class for describing block nodes.
   */
  class block_node: public cdk::basic_node {
    cdk::sequence_node *_decls, *_instrs;

    public:
      inline block_node(int lineno, cdk::sequence_node *decls, 
        cdk::sequence_node *instrs) :
        cdk::basic_node(lineno), _decls(decls), _instrs(instrs) {}

    public:
      void accept(basic_ast_visitor *sp, int level) {
        sp->do_block_node(this, level);
      }

  };

} // xpl

#endif