/** @file ascii85.c
 *
 * @brief Ascii85 encoder and decoder
 *
 * @par
 * @copyright Copyright © 2017 Doug Currie, Londonderry, NH, USA. All rights reserved.
 * 
 * @par
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without 
 * restriction, including without limitation the rights to use, copy, modify, merge, publish, 
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**/

#include "ascii85.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// From Wikipedia re: Ascii85 length...
// Adobe adopted the basic btoa encoding, but with slight changes, and gave it the name Ascii85.
// The characters used are the ASCII characters 33 (!) through 117 (u) inclusive (to represent
// the base-85 digits 0 through 84), together with the letter z (as a special case to represent
// a 32-bit 0 value), and white space is ignored. Adobe uses the delimiter "~>" to mark the end
// of an Ascii85-encoded string, and represents the length by truncating the final group: If the
// last block of source bytes contains fewer than 4 bytes, the block is padded with up to three
// null bytes before encoding. After encoding, as many bytes as were added as padding are
// removed from the end of the output.
// The reverse is applied when decoding: The last block is padded to 5 bytes with the Ascii85
// character "u", and as many bytes as were added as padding are omitted from the end of the
// output (see example).
// NOTE: The padding is not arbitrary. Converting from binary to base 64 only regroups bits and
// does not change them or their order (a high bit in binary does not affect the low bits in the
// base64 representation). In converting a binary number to base85 (85 is not a power of two)
// high bits do affect the low order base85 digits and conversely. Padding the binary low (with
// zero bits) while encoding and padding the base85 value high (with 'u's) in decoding assures
// that the high order bits are preserved (the zero padding in the binary gives enough room so
// that a small addition is trapped and there is no "carry" to the high bits).

// NOTE: ths implementation does not ignore white space!
//
// The motivation for this implementation is as a binary message wrapper for serial
// communication; in that application, white space is used for message framing.

static const uint8_t base_char = 33u; // '!' -- note that (85 + 33) < 128

static const int32_t ascii85_in_length_max = 65536;

static const bool ascii85_decode_z_for_zero  = true;
static const bool ascii85_encode_z_for_zero  = true;

static const bool ascii85_check_decode_chars = true;

#if 0
static inline bool ascii85_char_ok (uint8_t c)
{
    return ((c >= 33u) && (c <= 117u));
}
#endif

static inline bool ascii85_char_ng (uint8_t c)
{
    return ((c < 33u) || (c > 117u));
}

/*!
 * @brief encode_ascii85: encode binary input into Ascii85
 * @param[in] inp pointer to a buffer of unsigned bytes 
 * @param[in] in_length the number of bytes at inp to encode
 * @param[in] outp pointer to a buffer for the encoded data
 * @param[in] out_max_length available space at outp in bytes; must be >= 5 * ceiling(in_length/4)
 * @return number of bytes in the encoded value at outp if non-negative; error code from
 * ascii85_errs_e if negative
 * @par Possible errors include: ascii85_err_in_buf_too_large, ascii85_err_out_buf_too_small
 */
int32_t encode_ascii85 (const uint8_t *inp, int32_t in_length, uint8_t *outp, int32_t out_max_length)
{
    // Note that (in_length + 3) below may overflow, but this is inconsequental
    // since ascii85_in_length_max is < (INT32_MAX - 3), and we check in_length before
    // using the calculated out_length.
    //
    int32_t out_length = (((in_length + 3) / 4) * 5); // ceiling

    if (in_length > ascii85_in_length_max)
    {
        out_length = (int32_t )ascii85_err_in_buf_too_large;
    }
    else if (out_length > out_max_length)
    {
        out_length = (int32_t )ascii85_err_out_buf_too_small;
    }
    else
    {
        int32_t in_rover = 0;

        out_length = 0; // we know we can increment by 5 * ceiling(in_length/4)

        while (in_rover < in_length)
        {
            uint32_t chunk;
            int32_t chunk_len = in_length - in_rover;

            if (chunk_len >= 4)
            {
                chunk  = (((uint32_t )inp[in_rover++]) << 24u);
                chunk |= (((uint32_t )inp[in_rover++]) << 16u);
                chunk |= (((uint32_t )inp[in_rover++]) <<  8u);
                chunk |= (((uint32_t )inp[in_rover++])       );
            }
            else
            {
                chunk  =                           (((uint32_t )inp[in_rover++]) << 24u);
                chunk |= ((in_rover < in_length) ? (((uint32_t )inp[in_rover++]) << 16u) : 0u);
                chunk |= ((in_rover < in_length) ? (((uint32_t )inp[in_rover++]) <<  8u) : 0u);
                chunk |= ((in_rover < in_length) ? (((uint32_t )inp[in_rover++])       ) : 0u);
            }

            if (/*lint -e{506} -e{774}*/ascii85_encode_z_for_zero && (0u == chunk) && (chunk_len >= 4))
            {
                outp[out_length++] = (uint8_t )'z';
            }
            else
            {
                outp[out_length + 4] = (chunk % 85u) + base_char;
                chunk /= 85u;
                outp[out_length + 3] = (chunk % 85u) + base_char;
                chunk /= 85u;
                outp[out_length + 2] = (chunk % 85u) + base_char;
                chunk /= 85u;
                outp[out_length + 1] = (chunk % 85u) + base_char;
                chunk /= 85u;
                outp[out_length    ] = (uint8_t )chunk + base_char;
                // we don't need (chunk % 85u) on the last line since (((((2^32 - 1) / 85) / 85) / 85) / 85) = 82.278

                if (chunk_len >= 4)
                {
                    out_length += 5;
                }
                else
                {
                    out_length += (chunk_len + 1); // see note above re: Ascii85 length
                }
            }
        }
    }

    // Shouldn't need to do this -Victor
    int true_out_length;
    true_out_length = (in_length/4)*5 + (in_length%4==0 ? 0 : in_length%4+1);
    outp[true_out_length] = '\0';
    return out_length;
}
