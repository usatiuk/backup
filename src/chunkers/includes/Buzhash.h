//
// Created by Stepan Usatiuk on 26.04.2023.
//

#ifndef SEMBACKUP_BUZHASH_H
#define SEMBACKUP_BUZHASH_H

#include <array>
#include <climits>
#include <cstdint>
#include <deque>

/// Cyclic polynomial rolling hash
/** Based on: http://www.serve.net/buz/hash.adt/java.002.html
 *            https://github.com/silvasur/buzhash/blob/master/hash.go
 *            https://en.wikipedia.org/wiki/Rolling_hash#Cyclic_polynomial
 */
class Buzhash {
public:
    /// Constructs a new Buzhash instance
    /// \param blockSize Rolling hash window
    Buzhash(uint32_t blockSize);

    /// Returns current hash value
    uint32_t get() const;

    /// Adds \p   in to the hash
    /// \param in Byte to add
    /// \return   New hash value
    uint32_t feed(uint8_t in);

private:
    uint32_t cur = 0;        ///< Current hash value
    const uint32_t blockSize;///< Hashing window size
    std::deque<uint32_t>
            history;///< Bytes used to calculate current hash, used to compute the hash in a rolling fashion (to remove the oldest byte from the hash when blockSize is reached)

    // Circular shift taken from: https://en.wikipedia.org/wiki/Circular_shift
    /// Shift \p value \p count bits to the right circularly
    /// \param value Value to shift
    /// \param count By how many bytes
    /// \return      Shifted value
    static uint32_t rotr32(uint32_t value, unsigned int count);

    /// 256 32-bit random numbers used for hashing
    /// Ideally, should have an equal distribution of 0s and 1s, but I didn't bother checking it
    // clang-format off
    static constexpr std::array<uint32_t, 256> randomNumbers{
            0x827f934c, 0xebcd9924, 0x667fdea2, 0x8a8b0997, 0x42af49e8, 0x556cb313, 0x505da41b, 0xb23be60f,
            0xc3901be4, 0xee1d8d4d, 0x4d59795c, 0x8d542ba4, 0x043f073c, 0x2af19a39, 0xb2c4aa36, 0x6e30ff43,
            0x77ad3ef7, 0xd4c077e5, 0x3a1155aa, 0x866b07d3, 0xc16022b2, 0x6d4dad6e, 0x7a69c6dd, 0xd436dc23,
            0x32b64948, 0x1f72475f, 0x129be871, 0x05d46f6e, 0x7e405cd5, 0x31fdd272, 0x84a56b1a, 0xeaf43633,
            0x5f8148d4, 0x6d4bf6d9, 0xc2b4dbd7, 0xaa804cc7, 0xcb3de5ca, 0x6503cdb3, 0xa3c6d727, 0x20e2f098,
            0xd525bb67, 0x37b1b81e, 0xc1f1fd79, 0x4fe91240, 0x6a4ea716, 0x71245e33, 0xdbaab854, 0xfc24600e,
            0xd72dc72f, 0x2d7139ae, 0x075fb38d, 0xb18028a5, 0x9970d103, 0x235ec64b, 0x68645255, 0x352945f0,
            0x7a4b19a1, 0xe17df5f5, 0x676a6644, 0x75aad7aa, 0x63bdfc9a, 0x607586c7, 0x1546400e, 0xfe582141,
            0xb50a199f, 0xb0769910, 0x5d74ab3b, 0x2404799b, 0xa66a3a78, 0x1b6e24aa, 0x630674cc, 0x3272fea4,
            0xd4e9e078, 0xe586d12a, 0x579f8b98, 0xfd16bcb5, 0xd1e4faee, 0xe30953c7, 0x3ac73f87, 0xab66983f,
            0x5fe12f90, 0x10952ef1, 0x5c7ac32a, 0x89ccd941, 0xb82c3fa9, 0xacd374e5, 0x50984746, 0x09f082e8,
            0x11ee3b91, 0x31764e3a, 0xb59df38a, 0x67e94f2d, 0xcceaca68, 0xc68a89d8, 0x5f2e80ac, 0xd5556741,
            0x8c815df6, 0xde71c2b5, 0x7b1f5c49, 0xd64682a4, 0x4fb59748, 0x4968707f, 0x909c0c1a, 0x5f1dd608,
            0x1c601e37, 0x96e01ada, 0xc5582ef8, 0xae6834c1, 0xbe63b0ce, 0xab2aea9f, 0xf13e77c2, 0xe433350b,
            0x17a24a33, 0xc1f31bb6, 0xa23e9de4, 0x7e28ef69, 0x23e0ef42, 0x0796e53f, 0xf9e3045d, 0x7bbacd31,
            0xa48bee27, 0x15f3c3b3, 0x4c320cb4, 0x916429d9, 0xa15ccb3c, 0x82a4a23c, 0xb0cc6a4a, 0xcf8d93fa,
            0x3b18b937, 0xad0488e4, 0xaa568114, 0x80b9b8c7, 0x8f3a9071, 0x818b790d, 0x99c8dbf2, 0x0d23b2a4,
            0x74c81a28, 0x1aa65d76, 0x7168ee7d, 0xc0d40b6c, 0x77c70a0c, 0xd3752839, 0xc2f7981c, 0x83767124,
            0xb881618f, 0xb263d8cf, 0xbbb40400, 0xdb9702eb, 0xaccad841, 0x806af5a7, 0x16f096e3, 0x64bf45d9,
            0x5f7c0a58, 0xdac0c665, 0x1dbebaac, 0xb97027a6, 0xfc934433, 0xfc7b2d06, 0x8871fe4e, 0x0df24135,
            0x6ddf7cc8, 0x32e0d1cd, 0xe88abedd, 0x214af930, 0x90990f97, 0xc7691171, 0xbf7b6ca3, 0x8af6589c,
            0x452c8ee0, 0xbc2c5891, 0xcf8d13b4, 0x698d1f1f, 0x802a011a, 0x19820708, 0x25c79d2f, 0xedf91253,
            0xc93fe5dd, 0xa03a117b, 0x10912ae7, 0xc90d59d0, 0xc3522549, 0x3e4f3e81, 0x494ae40f, 0x2d157b6e,
            0xd7bf06b2, 0x19c5bb2a, 0xa869261c, 0xa80cfd2c, 0x1ea7c6ec, 0x1b36a51f, 0x8bd227cc, 0xad2d2260,
            0x181258c3, 0xbd253a58, 0x3273f94b, 0x9c315309, 0xb2d8d3e3, 0x11ec35a8, 0x384e6475, 0x855a9009,
            0x854cc06a, 0xe7408809, 0xe583ce2a, 0x895fb756, 0x6a8a2072, 0x6598a92b, 0x530f41bb, 0xb1bd57f1,
            0x62d57fa0, 0xe6505776, 0x42fcfe4d, 0x0fbdf1ee, 0x8e3104c4, 0xf11c8a65, 0x5bc51ad9, 0x5f1f8ce9,
            0xab179a87, 0xd5448444, 0x7bd4a26b, 0x658f1963, 0x86db95b8, 0xaba6734e, 0x486fddea, 0x859c3e0b,
            0xebce0106, 0x99c3014e, 0xc151b942, 0x9604aad8, 0xf6ce654b, 0xa1e7982e, 0xf6d8ed14, 0xd4bdf7e2,
            0x13696254, 0x05ec638c, 0x306dbc29, 0x1676eb60, 0xadbf3ce3, 0x966dde56, 0x6d5bea46, 0x719aa10d,
            0x0e65093d, 0x0b1a3c43, 0x0321ea8c, 0xe0ef2cbd, 0x43432ee3, 0x3e62046d, 0x425e7b44, 0x892e119c,
            0xfdec4de5, 0x48c5dd6c, 0x79e6bfcd, 0x8d53372e, 0xe96f6d32, 0x52cddacd, 0x3e99e0eb, 0xa9e5d28f,
    };
    // clang-format on
};


#endif//SEMBACKUP_BUZHASH_H
