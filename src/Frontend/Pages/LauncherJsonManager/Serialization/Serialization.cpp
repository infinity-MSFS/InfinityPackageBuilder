#include <fstream>

#include "Serialization.hpp"
#include "Util/Errors/Error.hpp"
#include "msgpack.hpp"
#include "zlib.h"

namespace Infinity {
    std::map<std::string, SerializationTypes::Incoming::IncomingBinary> Serialization::m_GroupMap = {};

    std::vector<uint8_t> Serialization::SerializeAndCompress(const SerializationTypes::Outgoing::OutgoingBinary &binary) {
        msgpack::sbuffer sbuffer;
        msgpack::pack(sbuffer, binary);

        z_stream strm{};
        if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 16 + MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
            Error(ErrorType::Fatal, "Failed on deflateInit2()").Dispatch();
        }

        std::vector<uint8_t> compressed_data;
        std::array<uint8_t, 8192> buffer{};

        strm.next_in = reinterpret_cast<Bytef *>(sbuffer.data());
        strm.avail_in = static_cast<uInt>(sbuffer.size());

        int ret;
        do {
            strm.next_out = buffer.data();
            strm.avail_out = static_cast<uInt>(buffer.size());

            ret = deflate(&strm, Z_FINISH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                deflateEnd(&strm);
                Error(ErrorType::Fatal, "Compression error").Dispatch();
            }

            const size_t compressed_size = buffer.size() - strm.avail_out;
            compressed_data.insert(compressed_data.end(), buffer.data(), buffer.data() + compressed_size);
        } while (ret != Z_STREAM_END);

        deflateEnd(&strm);
        return compressed_data;
    }

    std::map<std::string, SerializationTypes::Incoming::IncomingBinary> Serialization::DecompressAndDeserialize(std::vector<uint8_t> &binary) {
        z_stream strm{};
        strm.next_in = binary.data();
        strm.avail_in = static_cast<uInt>(binary.size());

        if (inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK) {
            Error(ErrorType::Fatal, "Failed to initialize zlib for decompression");
        }

        std::vector<uint8_t> decompressed_data;
        std::array<uint8_t, 8191> buffer{};

        int ret;
        do {
            strm.next_out = buffer.data();
            strm.avail_out = static_cast<uInt>(buffer.size());

            ret = inflate(&strm, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                inflateEnd(&strm);
                Error(ErrorType::Fatal, "Decompression error").Dispatch();
            }

            const size_t decompressed_size = buffer.size() - strm.avail_out;
            decompressed_data.insert(decompressed_data.end(), buffer.data(), buffer.data() + decompressed_size);
        } while (ret != Z_STREAM_END);
        inflateEnd(&strm);

        try {
            const msgpack::object_handle object_handle = msgpack::unpack(reinterpret_cast<const char *>(decompressed_data.data()), decompressed_data.size());
            const msgpack::object object = object_handle.get();

            std::map<std::string, SerializationTypes::Incoming::IncomingBinary> incoming_binary;
            object.convert(incoming_binary);
            return incoming_binary;
        } catch (std::exception &e) {
            Error(ErrorType::Fatal, std::string("MessagePack deserialization error") + e.what()).Dispatch();
            return {};
        }
    }
} // namespace Infinity
