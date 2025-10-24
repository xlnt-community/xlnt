// Copyright (c) 2025 xlnt-community
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE
//
// @license: http://www.opensource.org/licenses/mit-license.php
// @author: see AUTHORS file

#ifdef XLNT_USE_MINIZIP_NG

#include <detail/serialization/zip_minizip_ng.hpp>
#include <xlnt/utils/exceptions.hpp>
#include <algorithm>
#include <istream>
#include <ostream>
#include <ios>
#include <iterator>
#include <sstream>
#include <limits>
#include <sstream>

// minizip-ng headers
#include <mz.h>
#include <mz_zip.h>
#include <mz_strm.h>

namespace xlnt {
namespace detail {

// Unified exception mapping for minizip-ng errors
static void throw_minizip_error(int32_t error_code, const std::string &context) {
    std::string message;

    switch (error_code) {
        case MZ_OK:
            return;  // Not an error

        case MZ_STREAM_ERROR:
            message = "ZIP stream error: " + context;
            break;

        case MZ_DATA_ERROR:
            message = "Corrupted ZIP archive: " + context;
            break;

        case MZ_MEM_ERROR:
            message = "Memory allocation failed: " + context;
            break;

        case MZ_END_OF_LIST:
        case MZ_END_OF_STREAM:
            message = "File not found in archive: " + context;
            break;

        case MZ_PARAM_ERROR:
            message = "Invalid parameter: " + context;
            break;

        case MZ_EXIST_ERROR:
            message = "File already exists: " + context;
            break;

        case MZ_OPEN_ERROR:
            message = "Failed to open file in archive: " + context;
            break;

        case MZ_CLOSE_ERROR:
            message = "Failed to close file in archive: " + context;
            break;

        case MZ_SEEK_ERROR:
            message = "Seek error in archive: " + context;
            break;

        case MZ_READ_ERROR:
            message = "Read error from archive: " + context;
            break;

        case MZ_WRITE_ERROR:
            message = "Write error to archive: " + context;
            break;

        case MZ_CRC_ERROR:
            message = "CRC check failed: " + context;
            break;

        case MZ_PASSWORD_ERROR:
            message = "Encrypted archive or incorrect password: " + context;
            break;

        case MZ_SUPPORT_ERROR:
            message =
                "Feature not supported by current minizip build: " + context +
                " (可能未启用解压库，如 zlib；请在构建 minizip-ng 时开启 MZ_ZLIB 或提供可用的解压实现)";
            break;

        default:
            message = "ZIP error " + std::to_string(error_code) + ": " + context;
            break;
    }

    throw xlnt::exception(message);
}

// Custom stream implementation for std::istream/ostream (must match minizip expectations)
struct mz_stream_iostream {
    mz_stream stream;            // MUST be first: minizip casts to mz_stream*
    void *std_stream;            // std::istream* or std::ostream*
    int64_t total_in;
    int64_t total_out;
};

// Forward declare vtbls for create callbacks
extern mz_stream_vtbl iostream_read_vtbl;
extern mz_stream_vtbl iostream_write_vtbl;

// Create/destroy callbacks
static void *iostream_create_read(void) {
    auto *ios = new mz_stream_iostream();
    ios->stream.vtbl = &iostream_read_vtbl;
    ios->stream.base = nullptr;
    ios->std_stream = nullptr;
    ios->total_in = 0;
    ios->total_out = 0;
    return ios;
}

static void *iostream_create_write(void) {
    auto *ios = new mz_stream_iostream();
    ios->stream.vtbl = &iostream_write_vtbl;
    ios->stream.base = nullptr;
    ios->std_stream = nullptr;
    ios->total_in = 0;
    ios->total_out = 0;
    return ios;
}

static void iostream_destroy(void **stream) {
    if (!stream) return;
    auto *ios = static_cast<mz_stream_iostream *>(*stream);
    delete ios;
    *stream = nullptr;
}

int32_t iostream_open(void *stream_data, const char *path, int32_t mode) {
    (void)path;
    (void)mode;
    auto *ios_stream = static_cast<mz_stream_iostream*>(stream_data);
    return (ios_stream && ios_stream->std_stream) ? MZ_OK : MZ_STREAM_ERROR;
}

int32_t iostream_is_open(void *stream_data) {
    auto *ios_stream = static_cast<mz_stream_iostream*>(stream_data);
    return (ios_stream && ios_stream->std_stream) ? MZ_OK : MZ_STREAM_ERROR;
}

int32_t iostream_read(void *stream_data, void *buf, int32_t size) {
    auto *ios_stream = static_cast<mz_stream_iostream*>(stream_data);
    auto *is = static_cast<std::istream*>(ios_stream->std_stream);

    is->read(static_cast<char*>(buf), size);
    int32_t bytes_read = static_cast<int32_t>(is->gcount());

    // Check for read errors (excluding EOF which is normal)
    if (bytes_read == 0 && is->bad()) {
        return MZ_READ_ERROR;
    }

    ios_stream->total_in += bytes_read;
    return bytes_read;
}

int32_t iostream_write(void *stream_data, const void *buf, int32_t size) {
    auto *ios_stream = static_cast<mz_stream_iostream*>(stream_data);
    auto *os = static_cast<std::ostream*>(ios_stream->std_stream);

    os->write(static_cast<const char*>(buf), size);
    if (!os->good()) {
        return MZ_WRITE_ERROR;
    }

    ios_stream->total_out += size;
    return size;
}

int64_t iostream_tell(void *stream_data) {
    auto *ios_stream = static_cast<mz_stream_iostream*>(stream_data);
    auto *is = static_cast<std::istream*>(ios_stream->std_stream);

    return static_cast<int64_t>(is->tellg());
}

int64_t iostream_tell_write(void *stream_data) {
    auto *ios_stream = static_cast<mz_stream_iostream*>(stream_data);
    auto *os = static_cast<std::ostream*>(ios_stream->std_stream);

    return static_cast<int64_t>(os->tellp());
}

int32_t iostream_seek(void *stream_data, int64_t offset, int32_t origin) {
    auto *ios_stream = static_cast<mz_stream_iostream*>(stream_data);
    auto *is = static_cast<std::istream*>(ios_stream->std_stream);

    std::ios_base::seekdir dir;
    switch (origin) {
        case MZ_SEEK_SET: dir = std::ios_base::beg; break;
        case MZ_SEEK_CUR: dir = std::ios_base::cur; break;
        case MZ_SEEK_END: dir = std::ios_base::end; break;
        default: return MZ_PARAM_ERROR;
    }
    
    // Clear EOF/fail state to ensure subsequent seek succeeds
    is->clear();
    is->seekg(offset, dir);
    return is->fail() ? MZ_SEEK_ERROR : MZ_OK;
}

int32_t iostream_seek_write(void *stream_data, int64_t offset, int32_t origin) {
    auto *ios_stream = static_cast<mz_stream_iostream*>(stream_data);
    auto *os = static_cast<std::ostream*>(ios_stream->std_stream);

    std::ios_base::seekdir dir;
    switch (origin) {
        case MZ_SEEK_SET: dir = std::ios_base::beg; break;
        case MZ_SEEK_CUR: dir = std::ios_base::cur; break;
        case MZ_SEEK_END: dir = std::ios_base::end; break;
        default: return MZ_PARAM_ERROR;
    }

    os->clear();
    os->seekp(offset, dir);
    return os->fail() ? MZ_SEEK_ERROR : MZ_OK;
}

int32_t iostream_close(void *stream_data) {
    // Don't close - caller owns the stream
    (void)stream_data;
    return MZ_OK;
}

int32_t iostream_error(void *stream_data) {
    auto *ios_stream = static_cast<mz_stream_iostream*>(stream_data);
    auto *is = static_cast<std::istream*>(ios_stream->std_stream);

    return is->good() ? MZ_OK : MZ_STREAM_ERROR;
}

int32_t iostream_error_write(void *stream_data) {
    auto *ios_stream = static_cast<mz_stream_iostream*>(stream_data);
    auto *os = static_cast<std::ostream*>(ios_stream->std_stream);

    return os->good() ? MZ_OK : MZ_STREAM_ERROR;
}

// Read-only vtbl (for ZIP reading)
mz_stream_vtbl iostream_read_vtbl = {
    iostream_open,
    iostream_is_open,
    iostream_read,
    nullptr,  // write - not used for reading
    iostream_tell,
    iostream_seek,
    iostream_close,
    iostream_error,
    iostream_create_read,  // create
    iostream_destroy,      // destroy
    nullptr,  // get_prop_int64
    nullptr   // set_prop_int64
};

// Write-only vtbl (for ZIP writing)
mz_stream_vtbl iostream_write_vtbl = {
    iostream_open,
    iostream_is_open,
    nullptr,  // read - not used for writing
    iostream_write,
    iostream_tell_write,
    iostream_seek_write,
    iostream_close,
    iostream_error_write,
    iostream_create_write, // create
    iostream_destroy,      // destroy
    nullptr,  // get_prop_int64
    nullptr   // set_prop_int64
};

// Path normalization
std::string normalize_zip_path(const xlnt::path &p) {
    std::string s = p.string();

    // Convert backslash to forward slash
    std::replace(s.begin(), s.end(), '\\', '/');

    // Remove leading slash if present
    if (!s.empty() && s.front() == '/') {
        s = s.substr(1);
    }

    // minizip-ng expects UTF-8, xlnt::path::string() already returns UTF-8
    return s;
}

// =============================================================================
// zip_minizip_reader implementation
// =============================================================================

zip_minizip_reader::zip_minizip_reader(std::istream &stream)
    : source_stream_(stream)
    , zip_handle_(nullptr)
    , stream_handle_(nullptr)
    , ios_stream_(nullptr)
    , index_built_(false)
    , entry_open_(false)
{
    // Validate input stream
    if (!source_stream_.good()) {
        throw xlnt::exception("Invalid or corrupted input stream for ZIP archive");
    }

    // Create minizip handle
    zip_handle_ = mz_zip_create();
    if (!zip_handle_) {
        throw xlnt::exception("Failed to create minizip-ng handle");
    }

    // Create custom stream for std::istream
    stream_handle_ = iostream_create_read();
    if (!stream_handle_) {
        mz_zip_delete(&zip_handle_);
        throw xlnt::exception("Failed to create stream handle");
    }

    // The create callback allocated the stream object; take ownership pointer
    ios_stream_ = static_cast<mz_stream_iostream *>(stream_handle_);

    ios_stream_->std_stream = &source_stream_;
    ios_stream_->total_in = 0;
    ios_stream_->total_out = 0;

    // Open the stream (required by minizip-ng before using it)
    int32_t err = mz_stream_open(stream_handle_, nullptr, MZ_OPEN_MODE_READ);
    if (err != MZ_OK) {
        mz_stream_delete(&stream_handle_);
        ios_stream_ = nullptr;
        mz_zip_delete(&zip_handle_);
        throw_minizip_error(err, "Failed to open stream");
    }

    // Open archive for reading
    // This validates the ZIP file structure and central directory
    err = mz_zip_open(zip_handle_, stream_handle_, MZ_OPEN_MODE_READ);
    if (err != MZ_OK) {
        // Clean up in reverse order
        mz_stream_delete(&stream_handle_);
        ios_stream_ = nullptr;
        mz_zip_delete(&zip_handle_);
        zip_handle_ = nullptr;
        throw_minizip_error(err, "Failed to open ZIP archive");
    }
}

zip_minizip_reader::~zip_minizip_reader() {
    if (zip_handle_) {
        // Close ZIP first so minizip can still access the underlying stream when needed
        // Must close the ZIP; writing the central directory may access the underlying stream
        mz_zip_close(zip_handle_);
        if (stream_handle_) {
            // Destroy callback has already released the iostream object
            mz_stream_delete(&stream_handle_);
            ios_stream_ = nullptr; // destroy
        }
        mz_zip_delete(&zip_handle_);
    }
}

void zip_minizip_reader::build_file_index() {
    if (index_built_) {
        return;
    }

    // Clear any existing index and order
    file_index_.clear();
    file_order_.clear();

    // Get total number of entries for efficient memory allocation
    uint64_t num_entries = 0;
    int32_t err = mz_zip_get_number_entry(zip_handle_, &num_entries);
    if (err == MZ_OK && num_entries > 0) {
        // Pre-allocate both structures to avoid rehashing/reallocation
        file_index_.reserve(static_cast<size_t>(num_entries));
        file_order_.reserve(static_cast<size_t>(num_entries));
    }

    // Iterate through all entries and build index
    err = mz_zip_goto_first_entry(zip_handle_);
    int64_t index = 0;

    while (err == MZ_OK) {
        mz_zip_file *file_info = nullptr;
        err = mz_zip_entry_get_info(zip_handle_, &file_info);

        if (err == MZ_OK && file_info && file_info->filename) {
            // IMPORTANT: Normalize filename consistently before storing
            std::string raw_filename(file_info->filename);
            std::string normalized = normalize_zip_path(xlnt::path(raw_filename));

            // Skip directory entries (end with '/')
            if (!normalized.empty() && normalized.back() != '/') {
                file_index_[normalized] = index;
                file_order_.push_back(normalized);  // Maintain stable order
            }

            index++;
        } else if (err != MZ_OK) {
            // Log warning but continue - partial index is better than none
            // In production, you might want to log this
        }

        err = mz_zip_goto_next_entry(zip_handle_);
    }

    // Mark as built even if we hit an error partway through
    // This prevents infinite retry loops
    index_built_ = true;

    // Verify we got a reasonable number of entries
    if (file_index_.empty() && num_entries > 0) {
        throw xlnt::exception("Failed to build file index: archive may be corrupted");
    }
}

std::unique_ptr<std::streambuf> zip_minizip_reader::open(const path &file) const {
    // To match legacy builtin backend behavior, support opening multiple entries at once
    std::string filename = normalize_zip_path(file);

    // Locate the entry
    int32_t err = mz_zip_locate_entry(zip_handle_, filename.c_str(), 0);
    if (err != MZ_OK) {
        throw_minizip_error(err, "Cannot locate file: " + filename);
    }
    
    // Open for reading
    err = mz_zip_entry_read_open(zip_handle_, 0, nullptr);
    if (err != MZ_OK) {
        throw_minizip_error(err, "Cannot open file for reading: " + filename);
    }
    
    // Read data into std::string (owned by stringbuf)
    std::string out;
    char buffer[64 * 1024];
    for (;;) {
        int32_t n = mz_zip_entry_read(zip_handle_, buffer, sizeof(buffer));
        if (n < 0) {
            mz_zip_entry_close(zip_handle_);
            throw_minizip_error(n, "Error reading from ZIP entry: " + filename);
        }
        if (n == 0) break;
        out.append(buffer, static_cast<size_t>(n));
    }

    mz_zip_entry_close(zip_handle_);

    // Use stringbuf as an immutable input buffer to avoid lifetime issues
    return std::unique_ptr<std::streambuf>(new std::stringbuf(out, std::ios_base::in));
}

std::string zip_minizip_reader::read(const path &file) const {
    auto buf = open(file);
    std::istream stream(buf.get());

    std::string result;
    result.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());

    return result;
}

std::vector<path> zip_minizip_reader::files() const {
    const_cast<zip_minizip_reader*>(this)->build_file_index();

    // Return files in stable order (central directory order)
    // Using file_order_ instead of iterating unordered_map
    std::vector<path> result;
    result.reserve(file_order_.size());

    for (const auto &filename : file_order_) {
        result.emplace_back(filename);
    }

    return result;
}

bool zip_minizip_reader::has_file(const path &file) const {
    const_cast<zip_minizip_reader*>(this)->build_file_index();

    std::string normalized = normalize_zip_path(file);
    return file_index_.count(normalized) > 0;
}

void zip_minizip_reader::mark_entry_opened() const {
    if (entry_open_) {
        throw xlnt::exception("Cannot open multiple ZIP entries simultaneously");
    }
    entry_open_ = true;
}

void zip_minizip_reader::mark_entry_closed() const {
    entry_open_ = false;
}

// =============================================================================
// minizip_streambuf implementation
// =============================================================================

minizip_streambuf::minizip_streambuf(void *zip_handle, const std::string &filename, zip_minizip_reader *reader)
    : zip_handle_(zip_handle)
    , reader_(reader)
    , buffer_(buffer_size)
    , eof_reached_(false)
{
    // Check for concurrent entry access
    if (reader_) {
        reader_->mark_entry_opened();
    }

    // Locate file entry
    int32_t err = mz_zip_locate_entry(zip_handle_, filename.c_str(), 0);
    if (err != MZ_OK) {
        if (reader_) {
            reader_->mark_entry_closed();
        }
        throw_minizip_error(err, "Cannot locate file: " + filename);
    }

    // Open file for reading
    err = mz_zip_entry_read_open(zip_handle_, 0, nullptr);
    if (err != MZ_OK) {
        if (reader_) {
            reader_->mark_entry_closed();
        }
        throw_minizip_error(err, "Cannot open file for reading: " + filename);
    }

    // Initialize buffer pointers (empty initially)
    setg(buffer_.data(), buffer_.data(), buffer_.data());
}

minizip_streambuf::~minizip_streambuf() {
    if (zip_handle_) {
        mz_zip_entry_close(zip_handle_);
    }
    if (reader_) {
        reader_->mark_entry_closed();
    }
}

std::streambuf::int_type minizip_streambuf::underflow() {
    if (gptr() < egptr()) {
        return traits_type::to_int_type(*gptr());
    }

    if (eof_reached_) {
        return traits_type::eof();
    }

    // Read more data
    int32_t bytes_read = mz_zip_entry_read(zip_handle_, buffer_.data(), static_cast<int32_t>(buffer_size));

    if (bytes_read < 0) {
        throw_minizip_error(bytes_read, "Error reading from ZIP entry");
    }

    if (bytes_read == 0) {
        eof_reached_ = true;
        return traits_type::eof();
    }

    setg(buffer_.data(), buffer_.data(), buffer_.data() + bytes_read);

    return traits_type::to_int_type(*gptr());
}

// =============================================================================
// zip_minizip_writer implementation
// =============================================================================

zip_minizip_writer::zip_minizip_writer(std::ostream &stream)
    : destination_stream_(stream)
    , zip_handle_(nullptr)
    , stream_handle_(nullptr)
    , ios_stream_(nullptr)
    , entry_open_(false)
{
    // Validate output stream
    if (!destination_stream_.good()) {
        throw xlnt::exception("Invalid or corrupted output stream for ZIP archive");
    }

    // Create minizip handle
    zip_handle_ = mz_zip_create();
    if (!zip_handle_) {
        throw xlnt::exception("Failed to create minizip-ng handle");
    }

    // Create custom stream for std::ostream
    stream_handle_ = iostream_create_write();
    if (!stream_handle_) {
        mz_zip_delete(&zip_handle_);
        throw xlnt::exception("Failed to create stream handle");
    }

    // The create callback allocated the stream object; take ownership pointer
    ios_stream_ = static_cast<mz_stream_iostream *>(stream_handle_);

    ios_stream_->std_stream = &destination_stream_;
    ios_stream_->total_in = 0;
    ios_stream_->total_out = 0;

    // Open the stream (required by minizip-ng before using it)
    int32_t err = mz_stream_open(stream_handle_, nullptr, MZ_OPEN_MODE_WRITE);
    if (err != MZ_OK) {
        mz_stream_delete(&stream_handle_);
        ios_stream_ = nullptr;
        mz_zip_delete(&zip_handle_);
        throw_minizip_error(err, "Failed to open stream");
    }
    
    // Open for writing and create a new archive
    err = mz_zip_open(zip_handle_, stream_handle_, MZ_OPEN_MODE_WRITE | MZ_OPEN_MODE_CREATE);
    if (err != MZ_OK) {
        // Clean up in reverse order
        mz_stream_delete(&stream_handle_);
        ios_stream_ = nullptr;
        mz_zip_delete(&zip_handle_);
        zip_handle_ = nullptr;
        throw_minizip_error(err, "Failed to create ZIP archive");
    }
}

zip_minizip_writer::~zip_minizip_writer() {
    if (zip_handle_) {
        mz_zip_close(zip_handle_);

        // CRITICAL: Force flush to disk before cleaning up
        // Ensures central directory is fully written
        if (destination_stream_.good()) {
            destination_stream_.flush();
        }

        if (stream_handle_) {
            // Destroy callback has already released the iostream object
            mz_stream_delete(&stream_handle_);
            ios_stream_ = nullptr; // destroy 回调已释放对象
        }
        mz_zip_delete(&zip_handle_);
    }
}

std::unique_ptr<std::streambuf> zip_minizip_writer::open(const path &file) {
    return std::unique_ptr<std::streambuf>(new minizip_write_streambuf(zip_handle_, normalize_zip_path(file), this));
}

void zip_minizip_writer::mark_entry_opened() {
    if (entry_open_) {
        throw xlnt::exception("Cannot open multiple ZIP entries simultaneously");
    }
    entry_open_ = true;
}

void zip_minizip_writer::mark_entry_closed() {
    entry_open_ = false;
}

// =============================================================================
// minizip_write_streambuf implementation
// =============================================================================

minizip_write_streambuf::minizip_write_streambuf(void *zip_handle, const std::string &filename, zip_minizip_writer *writer)
    : zip_handle_(zip_handle)
    , writer_(writer)
    , buffer_(buffer_size)
{
    // Check for concurrent entry access
    if (writer_) {
        writer_->mark_entry_opened();
    }

    // Create file info
    mz_zip_file file_info = {};
    file_info.filename = filename.c_str();
    file_info.compression_method = MZ_COMPRESS_METHOD_DEFLATE;
    file_info.flag = MZ_ZIP_FLAG_UTF8;

    // CRITICAL: Disable Zip64 for Excel 2007/2010 compatibility
    // Excel has poor Zip64 support for small files (< 4GB)
    file_info.zip64 = MZ_ZIP64_DISABLE;  // = 2

    // Open file for writing
    int32_t err = mz_zip_entry_write_open(zip_handle_, &file_info, MZ_COMPRESS_LEVEL_DEFAULT, 0, nullptr);
    if (err != MZ_OK) {
        if (writer_) {
            writer_->mark_entry_closed();
        }
        throw_minizip_error(err, "Cannot open file for writing: " + filename);
    }

    // Initialize buffer
    setp(buffer_.data(), buffer_.data() + buffer_size);
}

minizip_write_streambuf::~minizip_write_streambuf() {
    try {
        sync();
        if (zip_handle_) {
            mz_zip_entry_close(zip_handle_);
        }
        if (writer_) {
            writer_->mark_entry_closed();
        }
    } catch (...) {
        // Ignore exceptions in destructor
    }
}

std::streambuf::int_type minizip_write_streambuf::overflow(std::streambuf::int_type c) {
    // Fix buffer overflow: when the buffer is full, flush before writing the current character,
    // otherwise writing past epptr() would corrupt ZIP data.
    if (c != traits_type::eof()) {
        if (pptr() == epptr()) {
            flush_buffer();
        }
        *pptr() = static_cast<char>(c);
        pbump(1);
        return c;
    }
    
    // At EOF, just flush any pending buffer
    // Flush pending buffer first, then write directly to ZIP to avoid overflow-induced state issues
    flush_buffer();
    return traits_type::not_eof(c);
}

int minizip_write_streambuf::sync() {
    flush_buffer();
    return 0;
}

void minizip_write_streambuf::flush_buffer() {
    std::ptrdiff_t n = pptr() - pbase();
    if (n > 0) {
        int32_t written = mz_zip_entry_write(zip_handle_, pbase(), static_cast<int32_t>(n));
        if (written != n) {
            throw xlnt::exception("Failed to write to ZIP entry");
        }
        // Reset buffer pointers only AFTER successful write
        setp(buffer_.data(), buffer_.data() + buffer_size);
    }
}

std::streamsize minizip_write_streambuf::xsputn(const char* s, std::streamsize n) {
    flush_buffer();

    std::streamsize total = 0;
    while (total < n) {
        int32_t chunk = static_cast<int32_t>(std::min<std::streamsize>(n - total,
            static_cast<std::streamsize>(std::numeric_limits<int32_t>::max())));
        int32_t written = mz_zip_entry_write(zip_handle_, s + total, chunk);
        if (written <= 0) {
            // Return partial write so the upper layer can set failbit
            return total;
        }
        total += written;
    }

    return total;
}

} // namespace detail
} // namespace xlnt

#endif // XLNT_USE_MINIZIP_NG
