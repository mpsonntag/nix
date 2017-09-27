// Copyright (c) 2017, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <ckellner@redhat.com>

#ifndef NIX_DATA_FRAME_H
#define NIX_DATA_FRAME_H

#include <nix/Platform.hpp>
#include <nix/DataType.hpp>

#include <nix/base/EntityWithSources.hpp>
#include <nix/base/IDataFrame.hpp>

#include <string>
#include <vector>

namespace nix {

class NIXAPI DataFrame : public base::EntityWithSources<base::IDataFrame> {
public:

    DataFrame()
        : EntityWithSources()
        {}

    DataFrame(const std::shared_ptr<base::IDataFrame> &df)
        : EntityWithSources(df)
        {}

    DataFrame(std::shared_ptr<base::IDataFrame> &&ptr)
        : EntityWithSources(std::move(ptr))
        {}

    ndsize_t rows() const {
        return backend()->rows();
    }

    void rows(ndsize_t n) {
        return backend()->rows(n);
    }

    std::vector<Column> columns() const {
        return backend()->columns();
    }

    void writeRow(ndsize_t row, const std::vector<Variant> &v) {
        return backend()->writeRow(row, v);
    }

    void writeCell(ndsize_t row, ndsize_t col, const Variant &v) {
        return backend()->writeCell(row, col, v);
    }

};


template<>
struct objectToType<nix::DataFrame> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::DataFrame;
    typedef nix::base::IDataFrame backendType;
};


}

#endif
