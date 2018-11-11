// Copyright 2017-2018 VMware, Inc.
// SPDX-License-Identifier: BSD-2-Clause
//
// The BSD-2 license (the License) set forth below applies to all parts of the
// Cascade project.  You may not use this file except in compliance with the
// License.
//
// BSD-2 License
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "src/verilog/transform/dead_code_eliminate.h"

#include "src/verilog/ast/ast.h"
#include "src/verilog/analyze/resolve.h"

namespace cascade {

DeadCodeEliminate::DeadCodeEliminate() : Editor() { }

void DeadCodeEliminate::run(ModuleDeclaration* md) {
  Index idx(this);
  md->accept(&idx);
  md->accept(this);
}

DeadCodeEliminate::Index::Index(DeadCodeEliminate* dce) {
  dce_ = dce;
}

void DeadCodeEliminate::Index::visit(const Attributes* a) {
  // Does nothing
  (void) a;
}

void DeadCodeEliminate::Index::visit(const Identifier* i) {
  Visitor::visit(i);
  const auto r = Resolve().get_resolution(i);
  if (r != nullptr) {
    dce_->use_.insert(r);
  }
}

void DeadCodeEliminate::Index::visit(const IntegerDeclaration* id) {
  // Does nothing
  (void) id;
}

void DeadCodeEliminate::Index::visit(const LocalparamDeclaration* ld) {
  // Does nothing
  (void) ld;
}

void DeadCodeEliminate::Index::visit(const NetDeclaration* nd) {
  // Does nothing
  (void) nd;
}

void DeadCodeEliminate::Index::visit(const ParameterDeclaration* pd) {
  // Does nothing
  (void) pd;
}

void DeadCodeEliminate::Index::visit(const RegDeclaration* rd) {
  // Does nothing
  (void) rd;
}

void DeadCodeEliminate::edit(ModuleDeclaration* md) {
  for (auto i = md->get_items()->begin(), ie = md->get_items()->end(); i != ie;) {
    if (auto d = dynamic_cast<Declaration*>(*i)) {
      if (use_.find(d->get_id()) == use_.end()) {
        i = md->get_items()->purge(i);
        continue;
      }
    }
    ++i;
  }
}

void DeadCodeEliminate::edit(ParBlock* pb) {
  for (auto i = pb->get_decls()->begin(), ie = pb->get_decls()->end(); i != ie; ) {
    if (use_.find((*i)->get_id()) == use_.end()) {
      i = pb->get_decls()->purge(i);
    } else {
      ++i;
    }
  }
}

void DeadCodeEliminate::edit(SeqBlock* sb) {
  for (auto i = sb->get_decls()->begin(), ie = sb->get_decls()->end(); i != ie; ) {
    if (use_.find((*i)->get_id()) == use_.end()) {
      i = sb->get_decls()->purge(i);
    } else {
      ++i;
    }
  }
}

} // namespace cascade