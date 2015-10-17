/* File.hh
 * vim: set tw=80:
 * Eryn Wells <eryn@erynwells.me>
 */
/**
 * File interface.
 */

#include <bitset>
#include <fstream>
#include <memory>

#include "String.hh"


namespace erw {
namespace core {

struct File
{
    enum class SeekFrom {
        /** Seek from the beginning of the file. */
        Begin,
        /** Seek from the current file offset. */
        Here,
        /** Seek from the end of the file. */
        End,
    };

    typedef std::bitset<3> Mode;

    static constexpr Mode In = Mode(1);
    static constexpr Mode Out = Mode(2);
    static constexpr Mode Binary = Mode(4);

    /** Destructor. */
    virtual ~File();

    /** Seek to an absolute position in the file. */
    virtual File& seek(size_t pos) = 0;

    /**
     * Seek to an `offset` from the given anchor point in the file.
     * @see SeekFrom
     */
    virtual File& seek(ssize_t offset, SeekFrom from) = 0;

protected:
    /** Convert a File::Mode to an iostream openmode bitset. */
    virtual std::ios_base::openmode modeToIOSMode(Mode mode);
};


/** File handle for reading. */
struct InFile
    : public File
{
    /** Open a file at `path` for reading. */
    InFile(const String& path, File::Mode mode);
    
    /** Deleted copy constructor. File handles cannot be copied. */
    InFile(const InFile& other) = delete;

    /** Move `other` to this InFile. */
    InFile(InFile&& other);

    virtual ~InFile();

    /** Move `other` to this InFile. File handles cannot be copied. */
    InFile& operator=(InFile& other);

    /** Read up to `count` characters into the provided `buffer`. */
    InFile& read(char* buffer, ssize_t count);

    /** @see File::seek */
    InFile& seek(size_t pos) override;

    /** @See File::seek */
    InFile& seek(ssize_t pos, File::SeekFrom from) override;

private:
    std::ifstream mStream;

    std::ios_base::openmode modeToIOSMode(File::Mode mode) override;
};


struct OutFile
    : public File
{
    /** Write `count` characters from the provided `buffer` into this file. */
    InFile& write(char* buffer, ssize_t count);

    /** @see File::seek */
    InFile& seek(size_t pos) override;

    /** @See File::seek */
    InFile& seek(ssize_t pos, File::SeekFrom from) override;

private:
    std::ofstream stream;
};

} /* namespace core */
} /* namespace erw */
