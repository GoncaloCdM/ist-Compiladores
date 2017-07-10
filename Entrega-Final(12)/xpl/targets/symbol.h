#ifndef __XPL_SEMANTICS_SYMBOL_H__
#define __XPL_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>
#include <vector>

namespace xpl {

    class symbol {
      basic_type *_type;
      std::string _name;
      int _offset;
      bool _is_global;
      bool _is_public;
      bool _is_use;
      bool _is_function;
      std::vector<basic_type*> _params;
      bool _is_extern;

    public:
      inline symbol(basic_type *type, const std::string &name, int offset, bool is_global, bool is_public, 
                    bool is_use, bool is_function, std::vector<basic_type*> params) :
                    _type(type), _name(name), _offset(offset), _is_global(is_global), _is_public(is_public),
                    _is_use(is_use), _is_extern(is_use||is_public), _is_function(is_function), _params(params) {}

      inline symbol(basic_type *type, const std::string &name, int offset, bool is_global, bool is_public, 
                    bool is_use, bool is_function) :
                    _type(type), _name(name), _offset(offset), _is_global(is_global), _is_public(is_public),
                    _is_use(is_use), _is_extern(is_use || is_public), _is_function(is_function) {}              

      virtual ~symbol() {
        delete _type;
      }

      inline basic_type *type() const {
        return _type;
      }
      inline const std::string &name() const {
        return _name;
      }
      inline int offset() const {
        return _offset;
      }
      inline int offset(int o) {
        return _offset = o;
      }
      inline void is_global(bool global){
        _is_global = global;
      }
      inline bool is_global(){
        return _is_global;
      }
      inline void is_function(bool func){
        _is_function = func;
      }
      inline bool is_function(){
        return _is_function;
      }
      inline void is_public(bool pub){
        _is_public = pub;
      }
      inline bool is_public(){
        return _is_public;
      }
      inline void is_use(bool use){
        _is_use = use;
      }
      inline bool is_use(){
        return _is_use;
      }
      inline void is_extern(bool is_extern){
        _is_extern = is_extern;
      }
      inline bool is_extern(){
        return _is_extern;
      }      
      inline void params(std::vector<basic_type*> sn){
        _params = sn;
      }
      inline std::vector<basic_type*> params(){
        return _params;
      }
    };

} // xpl

#endif
