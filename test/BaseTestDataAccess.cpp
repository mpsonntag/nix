// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>

#include <nix/hydra/multiArray.hpp>
#include <nix/util/dataAccess.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include "BaseTestDataAccess.hpp"

using namespace nix;
using namespace std;
using namespace boost;

void BaseTestDataAccess::testPositionToIndexRangeDimension() {
    string unit = "ms";
    string invalid_unit = "kV";
    string scaled_unit = "s";

    CPPUNIT_ASSERT_THROW(util::positionToIndex(5.0, invalid_unit, rangeDim), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT(*util::positionToIndex(5.0, unit, PositionMatch::Less, rangeDim) == 3);
    CPPUNIT_ASSERT(*util::positionToIndex(0.005, scaled_unit, PositionMatch::Less, rangeDim) == 3);

    CPPUNIT_ASSERT(!util::positionToIndex(1.0, unit, PositionMatch::Less, rangeDim));
    CPPUNIT_ASSERT(!util::positionToIndex(1.0, unit, PositionMatch::LessOrEqual, rangeDim));
    CPPUNIT_ASSERT(!util::positionToIndex(1.0, unit, PositionMatch::Equal, rangeDim));
    CPPUNIT_ASSERT(*(util::positionToIndex(1.0, unit, PositionMatch::GreaterOrEqual, rangeDim)) == 0);
    CPPUNIT_ASSERT(*(util::positionToIndex(1.0, unit, PositionMatch::Greater, rangeDim)) == 0);

    CPPUNIT_ASSERT(!util::positionToIndex(1.2, unit, PositionMatch::Less, rangeDim));
    CPPUNIT_ASSERT(util::positionToIndex(1.2, unit, PositionMatch::LessOrEqual, rangeDim));
    CPPUNIT_ASSERT(util::positionToIndex(1.2, unit, PositionMatch::Equal, rangeDim));
    CPPUNIT_ASSERT(*(util::positionToIndex(1.2, unit, PositionMatch::GreaterOrEqual, rangeDim)) == 0);
    CPPUNIT_ASSERT(*(util::positionToIndex(1.2, unit, PositionMatch::Greater, rangeDim)) == 1);

    CPPUNIT_ASSERT(*util::positionToIndex(4.5, unit, PositionMatch::Less, rangeDim) == 2);
    CPPUNIT_ASSERT(*util::positionToIndex(4.5, unit, PositionMatch::LessOrEqual, rangeDim) == 3);
    CPPUNIT_ASSERT(*util::positionToIndex(4.5, unit, PositionMatch::Equal, rangeDim) == 3);
    CPPUNIT_ASSERT(*(util::positionToIndex(4.5, unit, PositionMatch::GreaterOrEqual, rangeDim)) == 3);
    CPPUNIT_ASSERT(*(util::positionToIndex(4.5, unit, PositionMatch::Greater, rangeDim)) == 4);

    CPPUNIT_ASSERT(*util::positionToIndex(7.0, unit, PositionMatch::Less, rangeDim) == 4);
    CPPUNIT_ASSERT(*util::positionToIndex(7.0, unit, PositionMatch::LessOrEqual, rangeDim) == 4);
    CPPUNIT_ASSERT(!util::positionToIndex(7.0, unit, PositionMatch::Equal, rangeDim));
    CPPUNIT_ASSERT(!(util::positionToIndex(7.0, unit, PositionMatch::GreaterOrEqual, rangeDim)));
    CPPUNIT_ASSERT(!util::positionToIndex(7.0, unit, PositionMatch::Greater, rangeDim));

    CPPUNIT_ASSERT_THROW(util::positionToIndex({5.0, 1.2}, {1.4}, {unit, unit}, RangeMatch::Inclusive, rangeDim), std::runtime_error);

    vector<optional<pair<ndsize_t, ndsize_t>>> range = util::positionToIndex({0.0}, {7.0}, {unit}, RangeMatch::Inclusive, rangeDim);
    CPPUNIT_ASSERT(range[0] && (*range[0]).first == 0 && (*range[0]).second == 4);
    range = util::positionToIndex({0.0}, {7.0}, {unit}, RangeMatch::Exclusive, rangeDim);
    CPPUNIT_ASSERT(range[0] && (*range[0]).first == 0 && (*range[0]).second == 4);
    
    range = util::positionToIndex({2.0}, {6.7}, {unit}, RangeMatch::Inclusive, rangeDim);
    CPPUNIT_ASSERT(range[0] && (*range[0]).first == 1 && (*range[0]).second == 4);
    range = util::positionToIndex({2.0}, {6.7}, {unit}, RangeMatch::Exclusive, rangeDim);
    CPPUNIT_ASSERT(range[0] && (*range[0]).first == 1 && (*range[0]).second == 3);

    range = util::positionToIndex({1.2}, {1.2}, {unit}, RangeMatch::Inclusive, rangeDim);
    CPPUNIT_ASSERT(range[0] && (*range[0]).first == 0 && (*range[0]).second == 0);
    range = util::positionToIndex({1.2}, {1.2}, {unit}, RangeMatch::Exclusive, rangeDim);
    CPPUNIT_ASSERT(!range[0]);
}

void BaseTestDataAccess::testPositionToIndexRangeDimensionOld() {
    string unit = "ms";
    string invalid_unit = "kV";
    string scaled_unit = "s";

    CPPUNIT_ASSERT_THROW(util::positionToIndex(0.001, invalid_unit, rangeDim), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT_THROW(util::positionToIndex(8.0, unit, rangeDim), nix::OutOfBounds);
    CPPUNIT_ASSERT(util::positionToIndex(0.001, unit, rangeDim) == 0);
    CPPUNIT_ASSERT(util::positionToIndex(0.001, scaled_unit, rangeDim) == 0);
    CPPUNIT_ASSERT_THROW(util::positionToIndex(0.008, scaled_unit, rangeDim), nix::OutOfBounds);
    CPPUNIT_ASSERT(util::positionToIndex(3.4, unit, rangeDim) == 2);
    CPPUNIT_ASSERT(util::positionToIndex(3.6, unit, rangeDim) == 3);
    CPPUNIT_ASSERT(util::positionToIndex(4.0, unit, rangeDim) == 3);
    CPPUNIT_ASSERT(util::positionToIndex(0.0034, scaled_unit, rangeDim) == 2);

    vector<pair<ndsize_t, ndsize_t>> range = util::positionToIndex({0.001}, {3.4}, {unit}, rangeDim);
    CPPUNIT_ASSERT(range[0].first == 0 && range[0].second == 2);
    CPPUNIT_ASSERT_THROW(util::positionToIndex({0.001, 2.0}, {3.4}, {unit}, rangeDim), std::runtime_error);
    CPPUNIT_ASSERT_THROW(util::positionToIndex({7.5}, {8.0}, {unit}, rangeDim), nix::OutOfBounds);
}


void BaseTestDataAccess::testGetDimensionUnit() {
    std::vector<nix::Column> cols = {{"current", "nA", nix::DataType::Double},
                                     {"note", "", nix::DataType::String}};
    DataFrame df = block.createDataFrame("test", "test", cols);

    data_array.appendDataFrameDimension(df);
    data_array.appendDataFrameDimension(df, 1);

    std::string unit = "ms";
    std::string no_unit = "none";

    std::vector<Dimension> dims = data_array.dimensions();
    CPPUNIT_ASSERT(dims.size() == 6);
    CPPUNIT_ASSERT_EQUAL(util::getDimensionUnit(dims[0]), no_unit);
    CPPUNIT_ASSERT_EQUAL(util::getDimensionUnit(dims[1]), unit);
    CPPUNIT_ASSERT_EQUAL(util::getDimensionUnit(dims[2]), unit);
    CPPUNIT_ASSERT_EQUAL(util::getDimensionUnit(dims[3]), cols[0].unit);
    CPPUNIT_ASSERT_EQUAL(util::getDimensionUnit(dims[4]), no_unit);
    CPPUNIT_ASSERT_EQUAL(util::getDimensionUnit(dims[5]), no_unit);
}


void BaseTestDataAccess::testPositionToIndexSampledDimension() {
    string unit = "ms";
    string invalid_unit = "kV";
    string scaled_unit = "s";
    // test incompatible dims
    CPPUNIT_ASSERT_THROW(util::positionToIndex(1.0, invalid_unit, sampledDim), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT_NO_THROW(util::positionToIndex(1.0, unit, sampledDim));
    CPPUNIT_ASSERT_NO_THROW(util::positionToIndex(1.0, scaled_unit, sampledDim));

    sampledDim.unit(nix::none);
    CPPUNIT_ASSERT_THROW(util::positionToIndex(1.0, unit, sampledDim), nix::IncompatibleDimensions);
    sampledDim.unit(unit);

    vector<optional<pair<ndsize_t, ndsize_t>>> ranges = util::positionToIndex({0.0, 2.0, 10.0}, {10.0, 2.0, 5.0}, {unit, unit, unit}, RangeMatch::Inclusive, sampledDim);
    CPPUNIT_ASSERT(ranges.size() == 3);
    CPPUNIT_ASSERT(ranges[0] && (*ranges[0]).first == 0 && (*ranges[0]).second == 10);
    CPPUNIT_ASSERT(ranges[1] && (*ranges[1]).first == 2 && (*ranges[1]).second == 2);
    CPPUNIT_ASSERT(!ranges[2]);// && (*ranges[2]).first == 5 && (*ranges[2]).second == 10);

    ranges = util::positionToIndex({0.0, 2.0, 10.0}, {10.0, 2.0, 5.0}, {unit, unit, unit}, RangeMatch::Exclusive, sampledDim);
    CPPUNIT_ASSERT(ranges.size() == 3);
    CPPUNIT_ASSERT(ranges[0] && (*ranges[0]).first == 0 && (*ranges[0]).second == 9);
    CPPUNIT_ASSERT(!ranges[1]);
    CPPUNIT_ASSERT(!ranges[2]); // && (*ranges[2]).first == 5 && (*ranges[2]).second == 9);
}

void BaseTestDataAccess::testPositionToIndexSampledDimensionOld() {
    string unit = "ms";
    string invalid_unit = "kV";
    string scaled_unit = "s";

    CPPUNIT_ASSERT(util::positionToIndex(-8.0, unit, sampledDim) == 0);
    CPPUNIT_ASSERT_THROW(util::positionToIndex(0.005, invalid_unit, sampledDim), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT(util::positionToIndex(5.0, unit, sampledDim) == 5);
    CPPUNIT_ASSERT(util::positionToIndex(0.005, scaled_unit, sampledDim) == 5);

    CPPUNIT_ASSERT_THROW(util::positionToIndex({},{1.0}, {unit}, sampledDim), std::runtime_error);
    CPPUNIT_ASSERT_THROW(util::positionToIndex({0.0, 1.0}, {3.5, 0.5}, {unit, unit}, sampledDim), nix::OutOfBounds);
    CPPUNIT_ASSERT(util::positionToIndex({0.0, 0.5}, {3.5, 1.0}, {unit, unit}, sampledDim).size() == 2);

    vector<pair<ndsize_t, ndsize_t>> ranges = util::positionToIndex({0.0}, {0.0}, {unit}, sampledDim);
    CPPUNIT_ASSERT(ranges[0].first == 0 && ranges[0].second == 0);
}


void BaseTestDataAccess::testPositionToIndexSetDimensionOld() {
    std::string unit = "ms";
    CPPUNIT_ASSERT_NO_THROW(util::positionToIndex(-5.9, "none", setDim)); // because old style defaults to GreaterOrEqual
    CPPUNIT_ASSERT_THROW(util::positionToIndex(5.8, "none", setDim), nix::OutOfBounds); // labels are set, but too few
    CPPUNIT_ASSERT_NO_THROW(util::positionToIndex(0.5, "none", setDim));
    CPPUNIT_ASSERT(util::positionToIndex(0.5, "none", setDim) == 1);


    std::vector<std::pair<ndsize_t, ndsize_t>> ranges;
    CPPUNIT_ASSERT_THROW(util::positionToIndex({10}, {1.}, {"none"}, setDim), nix::OutOfBounds);
    CPPUNIT_ASSERT_NO_THROW(util::positionToIndex({1}, {10.}, {"none"}, setDim));

    int pos = -1;
    CPPUNIT_ASSERT_NO_THROW(check::converts_to_double(pos, "Does not convert seamlessly to double!"));
    ndsize_t large_pos = pow(10, 16);
    CPPUNIT_ASSERT_NO_THROW(check::converts_to_double(large_pos, "Does not convert seamlessly to double!"));
    large_pos = pow(10, 16);
    large_pos += 1;
    CPPUNIT_ASSERT_THROW(check::converts_to_double(large_pos, "Does not convert seamlessly to double!"), nix::OutOfBounds);
}


void BaseTestDataAccess::testPositionToIndexSetDimension() {
    CPPUNIT_ASSERT(!util::positionToIndex(5.8, PositionMatch::Equal, setDim));

    CPPUNIT_ASSERT_THROW(util::positionToIndex({5.0, 0.}, {10.5}, RangeMatch::Inclusive, setDim), std::runtime_error);
    
    vector<optional<pair<ndsize_t, ndsize_t>>> ranges = util::positionToIndex({5.0, 0.}, {10.5, 1.0}, RangeMatch::Inclusive, setDim);
    CPPUNIT_ASSERT(ranges.size() == 2);
    CPPUNIT_ASSERT(!ranges[0]);
    CPPUNIT_ASSERT(ranges[1] && (*ranges[1]).first == 0 && (*ranges[1]).second == 1);

    ranges = util::positionToIndex({5.0, 0.}, {10.5, 1.0}, RangeMatch::Exclusive, setDim);
    CPPUNIT_ASSERT(ranges.size() == 2);
    CPPUNIT_ASSERT(!ranges[0]);
    CPPUNIT_ASSERT(ranges[1] && (*ranges[1]).first == 0 && (*ranges[1]).second == 0);   
}


void BaseTestDataAccess::testPositionToIndexDataFrameDimension() {
    CPPUNIT_ASSERT_NO_THROW(util::positionToIndex(12.2, PositionMatch::GreaterOrEqual, dfDim)); // no throw even though not valid
    boost::optional<ndsize_t> pos = util::positionToIndex(12.2, PositionMatch::GreaterOrEqual, dfDim);
    CPPUNIT_ASSERT(!pos);

    
    CPPUNIT_ASSERT_THROW(util::positionToIndex({1.0, 2.0}, {0.0}, RangeMatch::Exclusive, dfDim), std::runtime_error);
    vector<optional<pair<ndsize_t, ndsize_t>>> ranges = util::positionToIndex({0.0, 12.0, 5.0}, {9.0, 15.0, 0.0}, RangeMatch::Exclusive, dfDim);
    CPPUNIT_ASSERT(ranges.size() == 3);
    CPPUNIT_ASSERT(ranges[0]);
    CPPUNIT_ASSERT(!ranges[1]);
    CPPUNIT_ASSERT(!ranges[2]);
}


void BaseTestDataAccess::testOffsetAndCount() {
    NDSize offsets, counts;
    /*
     std::vector<double> position {0.0, 2.0, 3.4, 1.0};
     std::vector<double> extent {0.0, 6.0, 2.3, 1.0};
     std::vector<std::string> units {"none", "ms", "ms"};
    */
    // implicitely calls with range = RangeMatch::Inclusive, 
    util::getOffsetAndCount(position_tag, data_array, offsets, counts);
    CPPUNIT_ASSERT(offsets.size() == 4);
    CPPUNIT_ASSERT(counts.size() == 4);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 2 && offsets[2] == 2 && offsets[3] == 1);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 1 && counts[2] == 1 && counts[3] == 1);

    // even if called with RangeMatch::Exclusive it will return count == 1
    util::getOffsetAndCount(position_tag, data_array, offsets, counts, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(offsets.size() == 4);
    CPPUNIT_ASSERT(counts.size() == 4);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 2 && offsets[2] == 2 && offsets[3] == 1);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 1 && counts[2] == 1 && counts[3] == 1);

    position_tag.units(std::vector<std::string>());
    util::getOffsetAndCount(position_tag, data_array, offsets, counts);
    CPPUNIT_ASSERT(position_tag.units().size() == 0);
    CPPUNIT_ASSERT(offsets.size() == 4);
    CPPUNIT_ASSERT(counts.size() == 4);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 2 && offsets[2] == 2 && offsets[3] == 1);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 1 && counts[2] == 1 && counts[3] == 1);

    /*
     Test a tag with position and extents
     position {0.0, 2.0, 3.4, 1.0};
     extent {0.0, 6.0, 2.3, 1.0};
     units {"none", "ms", "ms"};
     ref_dims = {Set (labels:{"label_a", "label_b"}), Sampled(1.0, ms), Range(ticks:{1.2, 2.3, 3.4, 4.5, 6.7}, ms),
     DataFrame(ticks:{0., 2.5, 5.0, 7.5, 10.0, 12.5, 15., 17.5, 20., 22.5})}
    */
    // call with RangeMatch::Inclusive
    util::getOffsetAndCount(segment_tag, data_array, offsets, counts);
    CPPUNIT_ASSERT(offsets.size() == 4);
    CPPUNIT_ASSERT(counts.size() == 4);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 2 && offsets[2] == 2);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 7 && counts[2] == 2);

    // call with RangeMatch::Exclusive
    util::getOffsetAndCount(segment_tag, data_array, offsets, counts, RangeMatch::Exclusive);

    CPPUNIT_ASSERT(offsets.size() == 4);
    CPPUNIT_ASSERT(counts.size() == 4);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 2 && offsets[2] == 2);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 6 && counts[2] == 2);

    /*
    MultiTag:
    positions = [0.0 --> 0.0, 3.0 --> 9.0,  3.4 --> 5.7, 1.0 --> 3.0];  // position 1
                [0.0 --> 0.0, 8.0 --> 11.0, 2.3 --> 4.3, 1.0 --> 7.0];  // position 2

    ref_array {Set, Sample, Range, DataFrame}, as above
    */
    // calling with RangeMatch::Inclusive
    CPPUNIT_ASSERT_THROW(util::getOffsetAndCount(multi_tag, data_array, -1, offsets, counts), nix::OutOfBounds); // not a valid position index
    CPPUNIT_ASSERT_THROW(util::getOffsetAndCount(multi_tag, data_array, 3, offsets, counts), nix::OutOfBounds); // not a valid position index

    util::getOffsetAndCount(multi_tag, data_array, 0, offsets, counts);
    CPPUNIT_ASSERT(offsets.size() == 4);
    CPPUNIT_ASSERT(counts.size() == 4);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 3 && offsets[2] == 2 && offsets[3] == 1);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 7 && counts[2] == 2 && counts[3] == 3);

    util::getOffsetAndCount(multi_tag, data_array, 0, offsets, counts, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(offsets.size() == 4);
    CPPUNIT_ASSERT(counts.size() == 4);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 3 && offsets[2] == 2 && offsets[3] == 1);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 6 && counts[2] == 2 && counts[3] == 2);

    util::getOffsetAndCount(multi_tag, data_array, 1, offsets, counts);
    CPPUNIT_ASSERT(offsets.size() == 4);
    CPPUNIT_ASSERT(counts.size() == 4);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 8 && offsets[2] == 1 && offsets[3] == 1);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 4 && counts[2] == 2 && counts[3] == 7);

    util::getOffsetAndCount(multi_tag, data_array, 1, offsets, counts, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(offsets.size() == 4);
    CPPUNIT_ASSERT(counts.size() == 4);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 8 && offsets[2] == 1 && offsets[3] == 1);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 3 && counts[2] == 2 && counts[3] == 6);
}


void BaseTestDataAccess::testPositionInData() {
    NDSize offsets, counts;
    util::getOffsetAndCount(multi_tag, data_array, 0, offsets, counts);
    CPPUNIT_ASSERT(util::positionInData(data_array, offsets));
    CPPUNIT_ASSERT(util::positionAndExtentInData(data_array, offsets, counts));

    util::getOffsetAndCount(multi_tag, data_array, 1, offsets, counts);
    CPPUNIT_ASSERT(util::positionInData(data_array, offsets));
    CPPUNIT_ASSERT(!util::positionAndExtentInData(data_array, offsets, counts));
}


void BaseTestDataAccess::testRetrieveData() {
    std::vector<ndsize_t> position_indices(1, 0);
    CPPUNIT_ASSERT_THROW(util::taggedData(multi_tag, position_indices, 1), nix::OutOfBounds);

    position_indices[0] = 10;
    CPPUNIT_ASSERT_THROW(util::taggedData(multi_tag, position_indices, 0), nix::OutOfBounds);

    position_indices[0] = 0;
    std::vector<DataView> views;
    views = util::taggedData(multi_tag, position_indices, 0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(views.size() == 1);
    nix::DataView v = util::taggedData(multi_tag, 0, 0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT_EQUAL(v.dataExtent(), views[0].dataExtent());

    std::vector<ndsize_t> temp;
    std::vector<DataView> slices = util::taggedData(mtag2, temp, 0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(slices.size() == mtag2.positions().dataExtent()[0]);

    // old-style calls, deprecated
    CPPUNIT_ASSERT_NO_THROW(util::retrieveData(mtag2, 0, 0));
    CPPUNIT_ASSERT_NO_THROW(util::retrieveData(mtag2, 0, mtag2.references()[0]));

    slices = util::taggedData(pointmtag, temp, 0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(slices.size() == pointmtag.positions().dataExtent()[0]);

    DataView data_view = views[0];
    NDSize data_size = data_view.dataExtent();

    CPPUNIT_ASSERT(data_size.size() == 4);
    CPPUNIT_ASSERT(data_size[0] == 1 && data_size[1] == 7 && data_size[2] == 2 && data_size[3] == 3);
    position_indices[0] = 1;
    CPPUNIT_ASSERT_THROW(util::taggedData(multi_tag, position_indices, 0), nix::OutOfBounds);

    data_view = util::taggedData(position_tag, 0, RangeMatch::Inclusive);
    data_size = data_view.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 4);
    CPPUNIT_ASSERT(data_size[0] == 1 && data_size[1] == 1 && data_size[2] == 1 && data_size[3] == 1);

    data_view = util::taggedData(segment_tag, 0, RangeMatch::Inclusive);
    data_size = data_view.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 4);
    CPPUNIT_ASSERT(data_size[0] == 1 && data_size[1] == 7 && data_size[2] == 2 && data_size[3] == 2);

    DataView times_view = util::taggedData(times_tag, 0, RangeMatch::Inclusive);
    data_size = times_view.dataExtent();
    std::vector<double> times(data_size.size());
    times_view.getData(times);
    RangeDimension dim = times_tag.references()[0].dimensions()[0].asRangeDimension();
    CPPUNIT_ASSERT(data_size.size() == 1);
    CPPUNIT_ASSERT(data_size[0] == 77);
}


void BaseTestDataAccess::testTagFeatureData() {
    DataArray number_feat = block.createDataArray("number feature", "test", nix::DataType::Double, {1});
    std::vector<double> number = {10.0};
    number_feat.setData(number);
    DataArray ramp_feat = block.createDataArray("ramp feature", "test", nix::DataType::Double, {10});
    ramp_feat.label("voltage");
    ramp_feat.unit("mV");
    SampledDimension dim = ramp_feat.appendSampledDimension(1.0);
    dim.unit("ms");
    std::vector<double> ramp_data = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    ramp_feat.setData(ramp_data);

    Tag pos_tag = block.createTag("feature test", "test", {5.0});
    pos_tag.units({"ms"});

    Feature f1 = pos_tag.createFeature(number_feat, nix::LinkType::Untagged);
    Feature f2 = pos_tag.createFeature(ramp_feat, nix::LinkType::Tagged);
    Feature f3 = pos_tag.createFeature(ramp_feat, nix::LinkType::Untagged);

    DataView data1 = util::featureData(pos_tag, 0);
    DataView data2 = util::featureData(pos_tag, 1);
    DataView data3 = util::featureData(pos_tag, 2);

    CPPUNIT_ASSERT(pos_tag.featureCount() == 3);
    CPPUNIT_ASSERT(data1.dataExtent().nelms() == 1);
    CPPUNIT_ASSERT(data2.dataExtent().nelms() == 1);
    CPPUNIT_ASSERT(data3.dataExtent().nelms() == ramp_data.size());

    data1 = util::featureData(pos_tag, f1);
    data2 = util::featureData(pos_tag, f2);
    data3 = util::featureData(pos_tag, f3);

    CPPUNIT_ASSERT(pos_tag.featureCount() == 3);
    CPPUNIT_ASSERT(data1.dataExtent().nelms() == 1);
    CPPUNIT_ASSERT(data2.dataExtent().nelms() == 1);
    CPPUNIT_ASSERT(data3.dataExtent().nelms() == ramp_data.size());

    // make tag pointing to a slice
    pos_tag.extent({2.0});
    data1 = util::featureData(pos_tag, 0);
    data2 = util::featureData(pos_tag, 1);
    data3 = util::featureData(pos_tag, 2);

    CPPUNIT_ASSERT(data1.dataExtent().nelms() == 1);
    CPPUNIT_ASSERT(data2.dataExtent().nelms() == 2);
    CPPUNIT_ASSERT(data3.dataExtent().nelms() == ramp_data.size());

    pos_tag.deleteFeature(f1.id());
    pos_tag.deleteFeature(f2.id());
    pos_tag.deleteFeature(f3.id());
    block.deleteDataArray(number_feat.id());
    block.deleteDataArray(ramp_feat.id());
    block.deleteTag(pos_tag);
}


void BaseTestDataAccess::testMultiTagFeatureData() {
    DataArray index_data = block.createDataArray("indexed feature data", "test", nix::DataType::Double, {10, 10});
    SampledDimension dim1 = index_data.appendSampledDimension(1.0);
    dim1.unit("ms");
    SampledDimension dim2 = index_data.appendSampledDimension(1.0);
    dim2.unit("ms");
    typedef boost::multi_array<double, 2> two_d_array;
    typedef two_d_array::index index;
    two_d_array data1(boost::extents[10][10]);
    index value;
    double total = 0.0;
    for(index i = 0; i != 10; ++i) {
        value = 100 * i;
        for(index j = 0; j != 10; ++j) {
            data1[i][j] = static_cast<double>(value++);
            total += data1[i][j];
        }
    }
    index_data.setData(data1);

    DataArray tagged_data = block.createDataArray("tagged feature data", "test", nix::DataType::Double, {10, 20, 10, 10});
    dim1 = tagged_data.appendSampledDimension(1.0);
    dim1.unit("ms");
    dim2 = tagged_data.appendSampledDimension(1.0);
    dim2.unit("ms");
    SampledDimension dim3;
    dim3 = tagged_data.appendSampledDimension(1.0);
    dim3.unit("ms");
    SetDimension dim4 = tagged_data.appendSetDimension();
    typedef boost::multi_array<double, 4> four_d_array;
    typedef four_d_array::index four_d_index;
    four_d_array data2(boost::extents[10][20][10][10]);

    for(four_d_index i = 0; i != 2; ++i) {
        value = 100 * i;
        for(four_d_index j = 0; j != 20; ++j) {
            for(four_d_index k = 0; k != 10; ++k) {
                for (four_d_index l = 0; l != 10; ++l) {
                    data2[i][j][k][l] = static_cast<double>(value++);
                }
            }
        }
    }
    tagged_data.setData(data2);
    Feature index_feature = multi_tag.createFeature(index_data, nix::LinkType::Indexed);
    Feature tagged_feature = multi_tag.createFeature(tagged_data, nix::LinkType::Tagged);
    Feature untagged_feature = multi_tag.createFeature(index_data, nix::LinkType::Untagged);

    // preparations done, actually test
    std::vector<ndsize_t> indices(1, 0);
    CPPUNIT_ASSERT(multi_tag.featureCount() == 3);

    // indexed feature
    // read a single feature, old style function
    DataView data_view = util::retrieveFeatureData(multi_tag, 1, 0);
    CPPUNIT_ASSERT(data_view.dataExtent().size()  == 2);
    CPPUNIT_ASSERT(data_view.dataExtent().nelms() == 10);
    CPPUNIT_ASSERT_THROW(util::retrieveFeatureData(multi_tag, 10, 0), nix::OutOfBounds);
    CPPUNIT_ASSERT_NO_THROW(util::retrieveFeatureData(multi_tag, 1, index_feature));

    // read feature data, multiple indices at once
    data_view = util::featureData(multi_tag, indices, 0)[0];

    NDSize data_size = data_view.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 2);
    CPPUNIT_ASSERT(data_size.nelms() == 10);
    double sum = 0.;
    double temp;
    NDSize offset(data_view.dataExtent().size(), 0);
    for (size_t i = 0; i < data_size[1]; ++i){
        offset[1] = i;
        data_view.getData<double>(temp, offset);
        sum += temp;
    }
    CPPUNIT_ASSERT(sum == 45);

    indices[0] = 1;
    data_view = util::featureData(multi_tag, indices, 0)[0];
    sum = 0;
    for (size_t i = 0; i < data_view.dataExtent()[1]; ++i){
        offset[1] = i;
        data_view.getData<double>(temp, offset);
        sum += temp;
    }
    CPPUNIT_ASSERT(sum == 1045);

    // untagged feature
    indices[0] = 0;
    data_view = util::featureData(multi_tag, indices, 2)[0];
    CPPUNIT_ASSERT(data_view.dataExtent().nelms() == 100);

    indices[0] = 1;
    data_view = util::featureData(multi_tag, indices, 2)[0];
    data_size = data_view.dataExtent();
    CPPUNIT_ASSERT(data_size.nelms() == 100);
    sum = 0;
    for (size_t i = 0; i < data_size[0]; ++i) {
        offset[0] = i;
        for (size_t j = 0; j < data_size[1]; ++j) {
            offset[1] = j;
            data_view.getData<double>(temp, offset);
            sum += temp;
        }
    }
    CPPUNIT_ASSERT(sum == total);

    // tagged feature
    indices[0] = 0;
    data_view = util::featureData(multi_tag, indices, 1)[0];
    data_size = data_view.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 4);

    data_view = util::featureData(multi_tag, indices, tagged_feature)[0];
    data_size = data_view.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 4);

    indices[0] = 1;
    data_view = util::featureData(multi_tag, indices, 1)[0];
    data_size = data_view.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 4);

    indices[0] = 2;
    CPPUNIT_ASSERT_THROW(util::featureData(multi_tag, indices, 1), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::featureData(multi_tag, indices, 3), nix::OutOfBounds);

    // test multiple positions
    std::vector<nix::DataView> views = util::featureData(multi_tag, {0, 1}, 0);
    CPPUNIT_ASSERT(views.size() == 2);
    CPPUNIT_ASSERT(views[0].dataExtent() == NDSize({1, 10}));
    CPPUNIT_ASSERT(views[0].dataExtent() == NDSize({1, 10}));

    // test positions without specifying
    indices.clear();
    views = util::featureData(multi_tag, indices, 0);
    CPPUNIT_ASSERT(views.size() == multi_tag.positionCount());

    // clean up
    multi_tag.deleteFeature(index_feature.id());
    multi_tag.deleteFeature(tagged_feature.id());
    multi_tag.deleteFeature(untagged_feature.id());
    block.deleteDataArray(tagged_data.id());
    block.deleteDataArray(index_data.id());
}


void BaseTestDataAccess::testMultiTagUnitSupport() {
    std::vector<std::string> valid_units{"none","ms","ms"};
    std::vector<std::string> invalid_units{"mV", "Ohm", "muV"};
    std::vector<ndsize_t> position_indices(1);

    MultiTag testTag = block.createMultiTag("test", "testTag", multi_tag.positions());
    testTag.units(valid_units);
    testTag.addReference(data_array);
    
    position_indices[0] = 0;
    CPPUNIT_ASSERT_NO_THROW(util::retrieveData(testTag, position_indices, 0));
    
    testTag.units(nix::none);
    CPPUNIT_ASSERT_NO_THROW(util::retrieveData(testTag, position_indices, 0));
    
    testTag.units(invalid_units);
    CPPUNIT_ASSERT_THROW(util::taggedData(testTag, position_indices, 0), nix::IncompatibleDimensions);
}


void BaseTestDataAccess::testDataView() {
    NDSize zcount = {2, 5, 2, 5};
    NDSize zoffset = {0, 5, 2, 2};

    DataView io = DataView(data_array, zcount, zoffset);

    CPPUNIT_ASSERT_EQUAL(zcount, io.dataExtent());
    CPPUNIT_ASSERT_EQUAL(data_array.dataType(), io.dataType());

    typedef boost::multi_array<double, 4> array_type;
    array_type data(boost::extents[2][5][2][5]);
    io.getData(data);

    const array_type::size_type *ext = data.shape();
    for (size_t i = 0; i < 4; i++) {
        CPPUNIT_ASSERT_EQUAL(static_cast<array_type::size_type >(zcount[i]), ext[i]);
    }

    array_type ref;
    data_array.getData(ref);

    for(size_t i = 0; i < zcount[0]; ++i) {
        for(size_t j = 0; j < zcount[1]; ++j) {
            for(size_t k = 0; k < zcount[2]; ++k) {
                for (size_t l = 0; l < zcount[3]; ++l) {
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(ref[i + 0][j + 5][k + 2][l + 2],
                                                 data[i][j][k][l],
                                                 std::numeric_limits<double>::epsilon());
                    data[i][j][k][l] = 0.0;
                }
            }
        }
    }

    io.setData(data, {0, 0, 0, 0});
    data_array.getData(ref);

    for(size_t i = 0; i < zcount[0]; ++i) {
        for(size_t j = 0; j < zcount[1]; ++j) {
            for(size_t k = 0; k < zcount[2]; ++k) {
                for (size_t l = 0; l < zcount[3]; ++l) {
                    CPPUNIT_ASSERT_DOUBLES_EQUAL(ref[i + 0][j + 5][k + 2][l + 2],
                                                 0.0,
                                                 std::numeric_limits<double>::epsilon());
                }
            }
        }
    }

    double val = 0.0;
    CPPUNIT_ASSERT_THROW(io.getData(val, {}, {0, 0, 3, 2}), OutOfBounds);

    array_type r2d2(boost::extents[3][3][3][3]);
    CPPUNIT_ASSERT_THROW(io.getData(r2d2, {3, 3, 3, 3}, {}), OutOfBounds);

    CPPUNIT_ASSERT_THROW(io.dataExtent(zcount), std::runtime_error);

    CPPUNIT_ASSERT_THROW(DataView(data_array, {0, 0, 0, 0, 0}, {1, 1, 1}), IncompatibleDimensions);
    CPPUNIT_ASSERT_THROW(DataView(data_array, {0, 0, 0}, {1, 1}), IncompatibleDimensions);
}


void BaseTestDataAccess::testDataSlice() {
    nix::Block b = file.createBlock("slicing data", "nix.test");

    // 1D SampledDimension
    std::vector<double> time(1000);
    std::vector<double> voltage;
    double interval = 0.01;
    double pi = 3.1415;
    double freq = 1.5;
    std::iota(time.begin(), time.end(), 0.);
    std::transform(time.begin(), time.end(), time.begin(),
                   [interval](double t){ return t * interval; });
    std::transform(time.begin(), time.end(), std::back_inserter(voltage),
                   [pi, freq](double t) { return std::sin(t * freq * 2 * pi); });

    // create a DataArray
    nix::DataArray oned_array = b.createDataArray("sinewave", "nix.sampled", nix::DataType::Double, {voltage.size()});
    oned_array.setData(voltage);
    oned_array.label("voltage");
    oned_array.unit("mV");

    nix::SampledDimension dim = oned_array.appendSampledDimension(interval);
    dim.label("time");
    dim.unit("s");

    // 2d data: Set - Sample
    typedef boost::multi_array<double, 2> array_type_2d;
    typedef array_type_2d::index index;

    array_type_2d data(boost::extents[10][time.size()]);
    for(index i = 0; i < 10; ++i) {
        for (size_t j = 0; j < time.size(); ++j) {
            data[i][j] = std::sin(time[j] * freq * 2 * pi + (i*2*pi/10));
        }
    }

    nix::NDSize data_shape(2, 10);  // NDSize object with rank two  10 elements per dim, for now
    data_shape[1] = time.size();
    // create the DataArray and store the data.
    nix::DataArray twod_array = b.createDataArray("2d sinewaves", "nix.test", nix::DataType::Double, data_shape);
    twod_array.setData(data);
    twod_array.label("voltage");
    twod_array.unit("mV");
    twod_array.appendSetDimension();
    dim = twod_array.appendSampledDimension(interval);
    dim.label("time");
    dim.unit("s");

    // 2d data: Range - Sample
    std::vector<double> ticks(10);
    for(size_t i = 0; i < 10; ++i) {
        ticks[i] = i * pi;
    }
    nix::DataArray twod_array2 = b.createDataArray("2d sinewaves 2", "nix.test", nix::DataType::Double, data_shape);
    twod_array2.setData(data);
    twod_array2.label("voltage");
    twod_array2.unit("mV");
    RangeDimension rangeDim = twod_array2.appendRangeDimension(ticks);
    rangeDim.label("time");
    rangeDim.unit("s");
    dim = twod_array2.appendSampledDimension(interval);
    dim.label("time");
    dim.unit("s");

    // do the tests!
    nix::DataArray no_array;
    CPPUNIT_ASSERT_THROW(util::dataSlice(no_array, {1, 2}, {2,3}), nix::UninitializedEntity);
    CPPUNIT_ASSERT_THROW(util::dataSlice(oned_array, {1, 2}, {2, 3}), std::invalid_argument); // 1d data but called with 2d segment 
    CPPUNIT_ASSERT_THROW(util::dataSlice(oned_array, {1}, {2}, {"ms", "mV"}), std::invalid_argument); // 1d positions, but two units
    CPPUNIT_ASSERT_THROW(util::dataSlice(oned_array, {1, 2, 3}, {1, 2}), std::invalid_argument); // different size of start and end
    CPPUNIT_ASSERT_THROW(util::dataSlice(oned_array, {0.0}, {1.0}, {"mV"}), nix::IncompatibleDimensions); // sampledDimension represents time
    CPPUNIT_ASSERT_THROW(util::dataSlice(oned_array, {0.0}, {1.0}, {"ks"}), nix::OutOfBounds); // well beyond data
    CPPUNIT_ASSERT_THROW(util::dataSlice(oned_array, {1.0}, {0.0}), std::invalid_argument); // start > end

    CPPUNIT_ASSERT_NO_THROW(util::dataSlice(oned_array, {0.0}, {1.0}));
    CPPUNIT_ASSERT_NO_THROW(util::dataSlice(oned_array, {0.0}, {1.0}, {"s"}));
    CPPUNIT_ASSERT_NO_THROW(util::dataSlice(oned_array, {0.0}, {1.0}, {"ms"}));
    CPPUNIT_ASSERT_NO_THROW(util::dataSlice(oned_array, {0.0}, {0.001}, {"ks"}));
 
    // test with RangeMatch::Inclusive
    nix::DataView slice = util::dataSlice(oned_array, {0.0}, {1.0}, {}, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(slice.dataExtent().size() == 1);
    CPPUNIT_ASSERT(slice.dataExtent()[0] == 101);

    slice = util::dataSlice(twod_array2, {3.14, 1.0}, {9.6, 2.0}, {}, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(slice.dataExtent()[0] == 3 && slice.dataExtent()[1] == 101);

    slice = util::dataSlice(twod_array, {0., 0.0}, {9.0, 1.0}, {"none", "s"}, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(slice.dataExtent()[0] == 10 && slice.dataExtent()[1] == 101);

    // test with RangeMatch::Exclusive
    slice = util::dataSlice(oned_array, {0.0}, {1.0}, {}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(slice.dataExtent().size() == 1);
    CPPUNIT_ASSERT(slice.dataExtent()[0] == 100);

    slice = util::dataSlice(oned_array, {1.0}, {1.0}, {}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(slice.dataExtent().size() == 1);
    CPPUNIT_ASSERT(slice.dataExtent()[0] == 1);

    slice = util::dataSlice(twod_array2, {3.14, 1.0}, {9.6, 2.0}, {}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(slice.dataExtent()[0] == 3 && slice.dataExtent()[1] == 100);

    slice = util::dataSlice(twod_array, {0., 0.0}, {9.0, 1.0}, {"none", "s"}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(slice.dataExtent()[0] == 9 && slice.dataExtent()[1] == 100);

    b.deleteDataArray(oned_array);
    b.deleteDataArray(twod_array);
    file.deleteBlock(b);
}

void BaseTestDataAccess::testFlexibleTagging() {
    nix::Block b = file.createBlock("flexible tagging", "nix.test");

    // create dummy data
    std::vector<double> data(1000, 0.0);
    for (size_t i = 0; i <data.size(); ++i) {
        data[i] = 3.14 * 1;
    }

    typedef boost::multi_array<int, 2> array_type_2d;
    typedef array_type_2d::index index;
    nix::NDSize data_shape_2d(2, 0);
    data_shape_2d[0] = 100;
    data_shape_2d[1] = 10;
    array_type_2d data2d(boost::extents[data_shape_2d[0]][data_shape_2d[1]]);
    for(index i = 0; i < 100; ++i) {
        for(index j = 0; j < 10; ++j) {
            data2d[i][j] = std::rand() % 100 + 1;
        }
    }

    typedef boost::multi_array<int, 3> array_type_3d;
    typedef array_type_3d::index index3;
    nix::NDSize data_shape_3d(3, 0);
    data_shape_3d[0] = 100;
    data_shape_3d[1] = 10;
    data_shape_3d[2] = 5;

    array_type_3d data3d(boost::extents[100][10][5]);
    for(index3 i = 0; i < 100; ++i) {
        for(index3 j = 0; j < 10; ++j) {
            for(index3 k = 0; k < 5; ++k) {
                data3d[i][j][k] = std::rand() % 100 + 1;
            }
        }
    }
    // Create the DataArrays and store the data.
    nix::DataArray array = b.createDataArray("1d random data", "nix.sampled", data);
    nix::SampledDimension dim = array.appendSampledDimension(0.1);
    dim.label("time");
    dim.unit("s");

    nix::DataArray array2d = b.createDataArray("2d random data", "nix.sampled.2d", data2d);
    dim = array2d.appendSampledDimension(1.);
    dim.label("time");
    dim.unit("s");
    array2d.appendSetDimension();

    nix::DataArray array3d = b.createDataArray("3d random data", "nix.sampled.3d", data3d);
    dim = array3d.appendSampledDimension(1.);
    dim.label("time");
    dim.unit("s");
    array3d.appendSetDimension();
    array3d.appendSetDimension();

    // Tag, tagging 2 dimensions
    nix::Tag tag = b.createTag("1stTag", "nix.segment", {25, 0});
    tag.extent({50, 5});
    tag.units({"s"});
    tag.addReference(array);
    tag.addReference(array2d);
    tag.addReference(array3d);

    nix::DataView view = tag.taggedData("1d random data");
    nix::NDSize exp_shape({500});
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);

    view = tag.taggedData("2d random data");
    exp_shape = {50, 5};
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);

    view = tag.taggedData("3d random data");
    exp_shape = {50, 5, 4};
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);

    // Tag, tagging 3 dims without extents, i.e. a point
    nix::Tag ndTag = b.createTag("2ndTag", "nix.points", {25, 0, 0});
    ndTag.addReference(array);
    ndTag.addReference(array2d);
    ndTag.addReference(array3d);

    view = ndTag.taggedData("1d random data");
    exp_shape = {1};
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);

    view = ndTag.taggedData("2d random data");
    exp_shape = {1, 1};
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);

    view = ndTag.taggedData("3d random data");
    exp_shape = {1, 1, 1};
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);

    //Tag, tagging 3d dims but with explicit zero extents
    nix::Tag rdTag = b.createTag("3rdTag11", "nix.points", {25, 0, 0});
    rdTag.extent({0.0, 0.0, 0.0});
    rdTag.addReference(array);
    rdTag.addReference(array2d);
    rdTag.addReference(array3d);

    view = rdTag.taggedData("1d random data");
    exp_shape = {1};
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);

    view = rdTag.taggedData("2d random data");
    exp_shape = {1, 1};
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);

    view = rdTag.taggedData("3d random data");
    exp_shape = {1, 1, 1};
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);

    nix::Tag failTag = b.createTag("failing tag", "nix.points", {25, 0, 0});
    failTag.extent({0.0, 0.0}); // this is invalid!
    failTag.addReference(array);
    failTag.addReference(array2d);
    failTag.addReference(array3d);

    CPPUNIT_ASSERT_THROW(failTag.taggedData("3d random data"), nix::IncompatibleDimensions);

    // MultiTag
    typedef boost::multi_array<double, 2> pos_type;
    typedef pos_type::index index4;

    pos_type pos_data(boost::extents[5][2]);
    pos_type ext_data(boost::extents[5][2]);
    for(index4 i = 0; i < 5; ++i) {
        pos_data[i][0] = i * 15.0 + 5.00;
        ext_data[i][0] = 10.0;
        pos_data[i][1] = 1.0;
        ext_data[i][1] = 2.0;
    }
    nix::DataArray positions = b.createDataArray("mtag positions", "nix.positions.2d", pos_data);
    positions.appendSetDimension();
    positions.appendSetDimension();

    nix::DataArray extents = b.createDataArray("mtag_extents", "nix.extents.2d", ext_data);
    extents.appendSetDimension();
    extents.appendSetDimension();

    nix::MultiTag mtag = b.createMultiTag("mtag", "segments", positions);
    mtag.extents(extents);

    mtag.addReference(array);
    mtag.addReference(array2d);
    mtag.addReference(array3d);

    view = mtag.taggedData(0, "1d random data");
    exp_shape = {100};
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);

    view = mtag.taggedData(0, "2d random data");
    exp_shape = {10, 2};
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);

    view = mtag.taggedData(0, "3d random data");
    exp_shape = {10, 2, 4};
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);


    // Test DataFrameDim
    std::vector<nix::Column> cols = {{"current", "nA", nix::DataType::Double},
                                     {"note", "", nix::DataType::String}};
    nix::DataFrame df = b.createDataFrame("conditions", "test", cols);
    std::vector<nix::Variant> vals(2);
    df.rows(10);
    for (int i = 0; i < 10; ++i) {
        vals[0].set(i * 2.5);
        vals[1].set("test");
        df.writeRow(i, vals);
    }
    array3d.deleteDimensions();
    dim = array3d.appendSampledDimension(1.);
    dim.label("time");
    dim.unit("s");
    array3d.appendDataFrameDimension(df);
    array3d.appendSetDimension();

    nix::Tag dfTag = b.createTag("dftest", "dftest", {25, 0});
    dfTag.extent({50, 5});
    dfTag.addReference(array3d);

    view = dfTag.taggedData("3d random data");
    exp_shape = {50, 5, 4};
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);

    dfTag.position({25});
    dfTag.extent({50});

    view = dfTag.taggedData("3d random data");
    exp_shape = {50, 9, 4};
    CPPUNIT_ASSERT(view.dataExtent() == exp_shape);

    file.deleteBlock(b);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
