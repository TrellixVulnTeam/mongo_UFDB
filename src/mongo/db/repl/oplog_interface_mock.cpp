/**
 *    Copyright (C) 2018-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/oplog_interface_mock.h"

namespace mongo {
namespace repl {

namespace {

class OplogIteratorMock : public OplogInterface::Iterator {
public:
    OplogIteratorMock(OplogInterfaceMock::Operations::const_iterator iterator,
                      OplogInterfaceMock::Operations::const_iterator iteratorEnd);
    StatusWith<Value> next() override;

private:
    OplogInterfaceMock::Operations::const_iterator _iterator;
    OplogInterfaceMock::Operations::const_iterator _iteratorEnd;
};

OplogIteratorMock::OplogIteratorMock(OplogInterfaceMock::Operations::const_iterator iter,
                                     OplogInterfaceMock::Operations::const_iterator iterEnd)
    : _iterator(iter), _iteratorEnd(iterEnd) {}

StatusWith<OplogInterface::Iterator::Value> OplogIteratorMock::next() {
    if (_iterator == _iteratorEnd) {
        return StatusWith<OplogInterface::Iterator::Value>(ErrorCodes::NoSuchKey, "no more ops");
    }
    return *(_iterator++);
}

}  // namespace

OplogInterfaceMock::OplogInterfaceMock(std::initializer_list<Operation> operations)
    : _operations(operations) {}

OplogInterfaceMock::OplogInterfaceMock(const Operations& operations) : _operations(operations) {}

void OplogInterfaceMock::setOperations(const Operations& operations) {
    _operations = operations;
}

std::string OplogInterfaceMock::toString() const {
    return "OplogInterfaceMock";
}

std::unique_ptr<OplogInterface::Iterator> OplogInterfaceMock::makeIterator() const {
    return std::unique_ptr<OplogInterface::Iterator>(
        new OplogIteratorMock(_operations.begin(), _operations.end()));
}

HostAndPort OplogInterfaceMock::hostAndPort() const {
    // Returns a default-constructed HostAndPort, which has an empty hostname and an invalid port.
    return {};
}

}  // namespace repl
}  // namespace mongo