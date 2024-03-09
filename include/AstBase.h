//
// Created by jjasmine on 3/9/24.
//

#ifndef SAMMINE_LANG_ASTBASE_H
#define SAMMINE_LANG_ASTBASE_H

namespace sammine_lang {
    namespace AST {

        class ASTVisitor {

        };
        class Visitable {
        public:
            virtual ~Visitable () = default;
            virtual void accept_vis (ASTVisitor &vis) = 0;
        };
        class AstBase {

        };

    }
}
#endif //SAMMINE_LANG_ASTBASE_H
