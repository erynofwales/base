/* File.cc
 * vim: set tw=80:
 * Eryn Wells <eryn@erynwells.me>
 */
/**
 * Implementation of file handling.
 */

#include "core/File.hh"


namespace erw {

#pragma mark - File

/*
 * File::File --
 */
File::File(const String& path)
    : mPath(path)
{ }


/*
 * File::~File --
 */
File::~File()
{ }


/*
 * File::path --
 */
String
File::path()
    const noexcept
{
    return mPath;
}


/*
 * File::modeToIOSMode --
 */
std::ios_base::openmode
File::modeToIOSMode(Mode mode)
{
    std::ios_base::openmode openmode = 0;
    if ((mode & In).any()) {
        openmode |= std::ios_base::in;
    }
    if ((mode & Out).any()) {
        openmode |= std::ios_base::out;
    }
    if ((mode & Binary).any()) {
        openmode |= std::ios_base::binary;
    }
    return openmode;
}

#pragma mark - InFile

/*
 * InFile::InFile --
 */
InFile::InFile(const String& path,
               InFile::Mode mode)
    : File(path),
      mStream(path, modeToIOSMode(mode))
{ }


/*
 * InFile::InFile --
 */
InFile::InFile(InFile&& other)
    : File(other.mPath),
      mStream(std::move(other.mStream))
{ }


/*
 * InFile::~InFile --
 */
InFile::~InFile()
{ }


/*
 * InFile::operator= --
 */
InFile&
InFile::operator=(InFile& other)
{
    mStream = std::move(other.mStream);
    return *this;
}


/*
 * InFile::read --
 */
InFile&
InFile::read(char* buffer,
             ssize_t count)
{
    mStream.read(buffer, count);
    return *this;
}


/*
 * InFile::seek --
 */
InFile&
InFile::seek(size_t pos)
{
    mStream.seekg(pos);
    return *this;
}


/*
 * InFile::seek --
 */
InFile&
InFile::seek(ssize_t off,
             File::SeekFrom from)
{
    std::ios_base::seekdir dir;
    switch (from) {
        case File::SeekFrom::Begin:
            dir = std::ios_base::beg;
            break;
        case File::SeekFrom::Here:
            dir = std::ios_base::cur;
            break;
        case File::SeekFrom::End:
            dir = std::ios_base::end;
            break;
    }
    mStream.seekg(off, dir);
    return *this;
}


/*
 * InFile::modeToIOSMode --
 */
std::ios_base::openmode
InFile::modeToIOSMode(File::Mode mode)
{
    // Ensure In flag is always set.
    if (!(mode & File::In).any()) {
        mode |= File::In;
    }
    return File::modeToIOSMode(mode);
}

#pragma mark - OutFile

} /* namespace erw */
