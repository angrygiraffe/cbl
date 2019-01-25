#pragma once

namespace cbl {
namespace filesystem{

class path;
class file_status;

class filesystem {
 public:
   virtual file_status stat(const path& name) = 0;
   virtual file create(const path& name) = 0;
   virtual exists(const path& name);
   virtual void rename(const path& old_path, const path& new_path) = 0;
   virtual void mkdir_all(const path& name, file_mode perm) = 0;
   virtual void chtimes(string name, chrone
};
}//namespace filesystem
}//namespace cbl
