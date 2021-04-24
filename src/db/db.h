#ifndef STORAGE_RANGEDB_DB_H_
#define STORAGE_RANGEDB_DB_H_

#include <cstdint>
#include <cstdio>

namespace rangedb {

struct Options;
struct ReadOptions;
struct WriteOptions;
class WriteBatch;

class DB {
 public:
  static Status Open(const Options& options, const std::string& name,
                     DB** dbptr);

  DB() = default;

  DB(const DB&) = delete;
  DB& operator=(const DB&) = delete;

  virtual ~DB();

  virtual Status Put(const WriteOptions& options, const Slice& key,
                     const Slice& value) = 0;

  virtual Status Delete(const WriteOptions& options, const Slice& key) = 0;

  virtual Status Write(const WriteOptions& options, WriteBatch* updates) = 0;
  virtual Status Get(const ReadOptions& options, const Slice& key,
                     std::string* value) = 0;
  virtual Iterator* NewIterator(const ReadOptions& options) = 0;
}  // namespace rangedb

#endif  // STORAGE_RANGEDB_DB_H_
