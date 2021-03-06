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

#ifndef CASCADE_SRC_RUNTIME_DATA_PLANE_H
#define CASCADE_SRC_RUNTIME_DATA_PLANE_H

#include <vector>
#include "src/base/bits/bits.h"
#include "src/runtime/ids.h"

namespace cascade {

class Engine;

// The dataplane is the mechanism by which modules communicate with each other
// and the runtime is able to implement the semantics of the verilog reference
// model.  Although the implementation does not enforce it. Conceptually, there
// should be exactly one DataPlane in the runtime.

class DataPlane {
  public:
    // Iterators:
    typedef std::vector<Engine*>::const_iterator reader_iterator;
    typedef std::vector<Engine*>::const_iterator writer_iterator;

    // Constructors
    DataPlane();

    // Id Interface:
    void register_id(VId id);

    // Reader Interface:
    void register_reader(Engine* e, VId id);
    void unregister_reader(Engine* e, VId id);
    reader_iterator reader_find(Engine* e, VId id) const;
    reader_iterator reader_begin(VId id) const;
    reader_iterator reader_end(VId id) const;

    // Writer Interface:
    void register_writer(Engine* e, VId id);
    void unregister_writer(Engine* e, VId id);
    writer_iterator writer_find(Engine* e, VId id) const;
    writer_iterator writer_begin(VId id) const;
    writer_iterator writer_end(VId id) const;

    // Communication Interface:
    void write(VId id, const Bits* bits);
    void write(VId id, bool b);

  private:
    // Registries:
    std::vector<std::vector<Engine*>> readers_;
    std::vector<std::vector<Engine*>> writers_;
    // Buffers:
    std::vector<Bits> write_buf_;
};

} // namespace cascade

#endif
