/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * This file was generated by asn1tools version 0.137.4 Thu Dec 13 05:47:38 2018.
 */

#include <string.h>
#include <stdio.h>
#include "uper.h"

struct encoder_t {
    uint8_t *buf_p;
    ssize_t size;
    ssize_t pos;
};

struct decoder_t {
    const uint8_t *buf_p;
    ssize_t size;
    ssize_t pos;
};

static void encoder_init(struct encoder_t *self_p,
                         uint8_t *buf_p,
                         size_t size)
{
    self_p->buf_p = buf_p;
    self_p->size = (8 * size);
    self_p->pos = 0;
}

static ssize_t encoder_get_result(struct encoder_t *self_p)
{
    if (self_p->size >= 0) {
        return ((self_p->pos + 7) / 8);
    } else {
        return (self_p->pos);
    }
}

static void encoder_abort(struct encoder_t *self_p,
                          ssize_t error)
{
    if (self_p->size >= 0) {
        self_p->size = -error;
        self_p->pos = -error;
    }
}

static size_t encoder_alloc(struct encoder_t *self_p,
                            size_t size)
{
    ssize_t pos;

    if (self_p->pos + (ssize_t)size <= self_p->size) {
        pos = self_p->pos;
        self_p->pos += size;
    } else {
        pos = -ENOMEM;
        encoder_abort(self_p, ENOMEM);
    }

    return (pos);
}

static void encoder_append_bit(struct encoder_t *self_p,
                               int value)
{
    ssize_t pos;

    pos = encoder_alloc(self_p, 1);

    if (pos < 0) {
        return;
    }

    if ((pos % 8) == 0) {
        self_p->buf_p[pos / 8] = 0;
    }

    self_p->buf_p[pos / 8] |= (value << (7 - (pos % 8)));
}

static void encoder_append_bits(struct encoder_t *self_p,
                                const uint8_t *buf_p,
                                size_t number_of_bits)
{
    size_t i;

    for (i = 0; i < number_of_bits; i++) {
        encoder_append_bit(self_p, (buf_p[i / 8] >> (7 - (i % 8))) & 1);
    }
}

static void encoder_append_bytes(struct encoder_t *self_p,
                                 const uint8_t *buf_p,
                                 size_t size)
{
    encoder_append_bits(self_p, buf_p, 8 * size);
}

static void encoder_append_uint8(struct encoder_t *self_p,
                                 uint8_t value)
{
    uint8_t buf[1];

    buf[0] = (uint8_t)value;

    encoder_append_bytes(self_p, &buf[0], sizeof(buf));
}

static void encoder_append_uint16(struct encoder_t *self_p,
                                  uint16_t value)
{
    uint8_t buf[2];

    buf[0] = (value >> 8);
    buf[1] = value;

    encoder_append_bytes(self_p, &buf[0], sizeof(buf));
}

static void encoder_append_uint32(struct encoder_t *self_p,
                                  uint32_t value)
{
    uint8_t buf[4];

    buf[0] = (value >> 24);
    buf[1] = (value >> 16);
    buf[2] = (value >> 8);
    buf[3] = value;

    encoder_append_bytes(self_p, &buf[0], sizeof(buf));
}

static void encoder_append_uint64(struct encoder_t *self_p,
                                  uint64_t value)
{
    uint8_t buf[8];

    buf[0] = (value >> 56);
    buf[1] = (value >> 48);
    buf[2] = (value >> 40);
    buf[3] = (value >> 32);
    buf[4] = (value >> 24);
    buf[5] = (value >> 16);
    buf[6] = (value >> 8);
    buf[7] = value;

    encoder_append_bytes(self_p, &buf[0], sizeof(buf));
}

static void encoder_append_int8(struct encoder_t *self_p,
                                int8_t value)
{
    value += 128;
    encoder_append_uint8(self_p, (uint8_t)value);
}

static void encoder_append_int16(struct encoder_t *self_p,
                                 int16_t value)
{
    value += 32768;
    encoder_append_uint16(self_p, (uint16_t)value);
}

static void encoder_append_int32(struct encoder_t *self_p,
                                 int32_t value)
{
    value += 2147483648;
    encoder_append_uint32(self_p, (uint32_t)value);
}

static void encoder_append_int64(struct encoder_t *self_p,
                                 int64_t value)
{
    value += 9223372036854775808ul;
    encoder_append_uint64(self_p, (uint64_t)value);
}

static void encoder_append_float(struct encoder_t *self_p,
                                 float value)
{
    (void)value;

    uint8_t buf[4];

    buf[0] = 0x03;
    buf[1] = 0x80;
    buf[2] = 0x00;
    buf[3] = 0x01;

    encoder_append_bytes(self_p, &buf[0], sizeof(buf));
}

static void encoder_append_double(struct encoder_t *self_p,
                                  double value)
{
    (void)value;

    uint8_t buf[4];

    buf[0] = 0x03;
    buf[1] = 0x80;
    buf[2] = 0x00;
    buf[3] = 0x01;

    encoder_append_bytes(self_p, &buf[0], sizeof(buf));
}

static void encoder_append_bool(struct encoder_t *self_p, bool value)
{
    encoder_append_bit(self_p, value ? 1 : 0);
}

static void encoder_append_non_negative_binary_integer(struct encoder_t *self_p,
                                                       uint64_t value,
                                                       size_t size)
{
    size_t i;

    for (i = 0; i < size; i++) {
        encoder_append_bit(self_p, (value >> (size - i - 1)) & 1);
    }
}

static void decoder_init(struct decoder_t *self_p,
                         const uint8_t *buf_p,
                         size_t size)
{
    self_p->buf_p = buf_p;
    self_p->size = (8 * size);
    self_p->pos = 0;
}

static ssize_t decoder_get_result(struct decoder_t *self_p)
{
    if (self_p->size >= 0) {
        return ((self_p->pos + 7) / 8);
    } else {
        return (self_p->pos);
    }
}

static void decoder_abort(struct decoder_t *self_p,
                          ssize_t error)
{
    if (self_p->size >= 0) {
        self_p->size = -error;
        self_p->pos = -error;
    }
}

static size_t decoder_free(struct decoder_t *self_p,
                           size_t size)
{
    ssize_t pos;

    if (self_p->pos + (ssize_t)size <= self_p->size) {
        pos = self_p->pos;
        self_p->pos += size;
    } else {
        pos = -EOUTOFDATA;
        decoder_abort(self_p, EOUTOFDATA);
    }

    return (pos);
}

static int decoder_read_bit(struct decoder_t *self_p)
{
    ssize_t pos;
    int value;

    pos = decoder_free(self_p, 1);

    if (pos >= 0) {
        value = ((self_p->buf_p[pos / 8] >> (7 - (pos % 8))) & 1);
    } else {
        value = 0;
    }

    return (value);
}

static void decoder_read_bits(struct decoder_t *self_p,
                              uint8_t *buf_p,
                              size_t number_of_bits)
{
    size_t i;

    memset(buf_p, 0, number_of_bits / 8);

    for (i = 0; i < number_of_bits; i++) {
        buf_p[i / 8] |= (decoder_read_bit(self_p) << (7 - (i % 8)));
    }
}

static void decoder_read_bytes(struct decoder_t *self_p,
                               uint8_t *buf_p,
                               size_t size)
{
    decoder_read_bits(self_p, buf_p, 8 * size);
}

static uint8_t decoder_read_uint8(struct decoder_t *self_p)
{
    uint8_t value;

    decoder_read_bytes(self_p, &value, sizeof(value));

    return (value);
}

static uint16_t decoder_read_uint16(struct decoder_t *self_p)
{
    uint8_t buf[2];

    decoder_read_bytes(self_p, &buf[0], sizeof(buf));

    return ((buf[0] << 8) | buf[1]);
}

static uint32_t decoder_read_uint32(struct decoder_t *self_p)
{
    uint8_t buf[4];

    decoder_read_bytes(self_p, &buf[0], sizeof(buf));

    return ((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]);
}

static uint64_t decoder_read_uint64(struct decoder_t *self_p)
{
    uint8_t buf[8];

    decoder_read_bytes(self_p, &buf[0], sizeof(buf));

    return (((uint64_t)buf[0] << 56)
            | ((uint64_t)buf[1] << 48)
            | ((uint64_t)buf[2] << 40)
            | ((uint64_t)buf[3] << 32)
            | ((uint64_t)buf[4] << 24)
            | ((uint64_t)buf[5] << 16)
            | ((uint64_t)buf[6] << 8)
            | (uint64_t)buf[7]);
}

static int8_t decoder_read_int8(struct decoder_t *self_p)
{
    int8_t value;

    value = (int8_t)decoder_read_uint8(self_p);
    value -= 128;

    return (value);
}

static int16_t decoder_read_int16(struct decoder_t *self_p)
{
    int16_t value;

    value = (int16_t)decoder_read_uint16(self_p);
    value -= 32768;

    return (value);
}

static int32_t decoder_read_int32(struct decoder_t *self_p)
{
    int32_t value;

    value = (int32_t)decoder_read_uint32(self_p);
    value -= 2147483648;

    return (value);
}

static int64_t decoder_read_int64(struct decoder_t *self_p)
{
    int64_t value;

    value = (int64_t)decoder_read_uint64(self_p);
    value -= 9223372036854775808ul;

    return (value);
}

static float decoder_read_float(struct decoder_t *self_p)
{
    uint8_t buf[4];

    decoder_read_bytes(self_p, &buf[0], sizeof(buf));

    return (1.0f);
}

static double decoder_read_double(struct decoder_t *self_p)
{
    uint8_t buf[4];

    decoder_read_bytes(self_p, &buf[0], sizeof(buf));

    return (1.0);
}

static bool decoder_read_bool(struct decoder_t *self_p)
{
    return (decoder_read_bit(self_p));
}

static uint64_t decoder_read_non_negative_binary_integer(struct decoder_t *self_p,
                                                         size_t size)
{
    size_t i;
    uint64_t value;

    value = 0;

    for (i = 0; i < size; i++) {
        value <<= 1;
        value |= decoder_read_bit(self_p);
    }

    return (value);
}

static void uper_c_source_a_encode_inner(
    struct encoder_t *encoder_p,
    const struct uper_c_source_a_t *src_p)
{
    encoder_append_int8(encoder_p, src_p->a);
    encoder_append_int16(encoder_p, src_p->b);
    encoder_append_int32(encoder_p, src_p->c);
    encoder_append_int64(encoder_p, src_p->d);
    encoder_append_uint8(encoder_p, src_p->e);
    encoder_append_uint16(encoder_p, src_p->f);
    encoder_append_uint32(encoder_p, src_p->g);
    encoder_append_uint64(encoder_p, src_p->h);
    encoder_append_float(encoder_p, src_p->i);
    encoder_append_double(encoder_p, src_p->j);
    encoder_append_bool(encoder_p, &src_p->k);
    encoder_append_bytes(encoder_p, &src_p->l.value[0], 11);
}

static void uper_c_source_a_decode_inner(
    struct decoder_t *decoder_p,
    struct uper_c_source_a_t *dst_p)
{
    dst_p->a = decoder_read_int8(decoder_p);
    dst_p->b = decoder_read_int16(decoder_p);
    dst_p->c = decoder_read_int32(decoder_p);
    dst_p->d = decoder_read_int64(decoder_p);
    dst_p->e = decoder_read_uint8(decoder_p);
    dst_p->f = decoder_read_uint16(decoder_p);
    dst_p->g = decoder_read_uint32(decoder_p);
    dst_p->h = decoder_read_uint64(decoder_p);
    dst_p->i = decoder_read_float(decoder_p);
    dst_p->j = decoder_read_double(decoder_p);
    dst_p->k = decoder_read_bool(decoder_p);
    decoder_read_bytes(decoder_p, &dst_p->l.value[0], 11);
}

static void uper_c_source_b_encode_inner(
    struct encoder_t *encoder_p,
    const struct uper_c_source_b_t *src_p)
{
    switch (src_p->choice) {

    case uper_c_source_b_choice_a_t:
        encoder_append_non_negative_binary_integer(encoder_p, 0, 2);
        encoder_append_int8(encoder_p, src_p->value.a);
        break;

    case uper_c_source_b_choice_b_t:
        encoder_append_non_negative_binary_integer(encoder_p, 1, 2);
        uper_c_source_a_encode_inner(encoder_p, &src_p->value.b);
        break;

    case uper_c_source_b_choice_c_t:
        encoder_append_non_negative_binary_integer(encoder_p, 2, 2);
        break;

    default:
        encoder_abort(encoder_p, EBADCHOICE);
        break;
    }
}

static void uper_c_source_b_decode_inner(
    struct decoder_t *decoder_p,
    struct uper_c_source_b_t *dst_p)
{
    uint8_t choice;

    choice = decoder_read_non_negative_binary_integer(decoder_p, 2);

    switch (choice) {

    case 0:
        dst_p->choice = uper_c_source_b_choice_a_t;
        dst_p->value.a = decoder_read_int8(decoder_p);
        break;

    case 1:
        dst_p->choice = uper_c_source_b_choice_b_t;
        uper_c_source_a_decode_inner(decoder_p, &dst_p->value.b);
        break;

    case 2:
        dst_p->choice = uper_c_source_b_choice_c_t;
        break;

    default:
        decoder_abort(decoder_p, EBADCHOICE);
        break;
    }
}

static void uper_c_source_d_encode_inner(
    struct encoder_t *encoder_p,
    const struct uper_c_source_d_t *src_p)
{
    uint8_t i;
    uint8_t present_mask;

    encoder_append_non_negative_binary_integer(encoder_p,
                                               src_p->length - 1,
                                               4);

    for (i = 0; i < src_p->length; i++) {
        switch (src_p->elements[i].a.b.choice) {

        case uper_c_source_d_a_b_choice_c_t:
            encoder_append_non_negative_binary_integer(encoder_p, 0, 1);
            encoder_append_non_negative_binary_integer(encoder_p,
                                                       src_p->elements[i].a.b.value.c,
                                                       1);
            break;

        case uper_c_source_d_a_b_choice_d_t:
            encoder_append_non_negative_binary_integer(encoder_p, 1, 1);
            encoder_append_bool(encoder_p, src_p->elements[i].a.b.value.d);
            break;

        default:
            encoder_abort(encoder_p, EBADCHOICE);
            break;
        }

        encoder_append_non_negative_binary_integer(encoder_p,
                                                   src_p->elements[i].a.e.length - 3,
                                                   1);
        encoder_append_non_negative_binary_integer(encoder_p,
                                                   src_p->elements[i].g.h,
                                                   2);
        encoder_append_non_negative_binary_integer(encoder_p,
                                                   src_p->elements[i].g.l.length - 1,
                                                   1);
        encoder_append_bytes(encoder_p,
                             &src_p->elements[i].g.l.value[0],
                             src_p->elements[i].g.l.length);
        present_mask = 0;

        if (src_p->elements[i].m.is_n_present) {
            present_mask |= 0x04;
        }

        if (src_p->elements[i].m.o != 3) {
            present_mask |= 0x02;
        }

        if (src_p->elements[i].m.is_p_present) {
            present_mask |= 0x01;
        }

        encoder_append_non_negative_binary_integer(encoder_p,
                                                   present_mask,
                                                   3);

        if (src_p->elements[i].m.is_n_present) {
            encoder_append_bool(encoder_p, src_p->elements[i].m.n);
        }

        if (src_p->elements[i].m.o != 3) {
            encoder_append_non_negative_binary_integer(encoder_p,
                                                       src_p->elements[i].m.o + 3,
                                                       3);
        }

        if (src_p->elements[i].m.is_p_present) {
            present_mask = 0;

            if (src_p->elements[i].m.p.is_r_present) {
                present_mask |= 0x01;
            }

            encoder_append_non_negative_binary_integer(encoder_p,
                                                       present_mask,
                                                       1);
            encoder_append_bytes(encoder_p,
                                 &src_p->elements[i].m.p.q.value[0],
                                 5);

            if (src_p->elements[i].m.p.is_r_present) {
                encoder_append_bool(encoder_p, src_p->elements[i].m.p.r);
            }
        }
    }
}

static void uper_c_source_d_decode_inner(
    struct decoder_t *decoder_p,
    struct uper_c_source_d_t *dst_p)
{
    uint8_t i;
    uint8_t choice;
    uint8_t present_mask;
    bool is_m_o_present;

    dst_p->length = decoder_read_non_negative_binary_integer(decoder_p, 4);
    dst_p->length += 1;

    if (dst_p->length > 10) {
        decoder_abort(decoder_p, EBADLENGTH);

        return;
    }

    for (i = 0; i < dst_p->length; i++) {
        choice = decoder_read_non_negative_binary_integer(decoder_p, 1);

        switch (choice) {

        case 0:
            dst_p->elements[i].a.b.choice = uper_c_source_d_a_b_choice_c_t;
            dst_p->elements[i].a.b.value.c = decoder_read_non_negative_binary_integer(decoder_p, 1);
            break;

        case 1:
            dst_p->elements[i].a.b.choice = uper_c_source_d_a_b_choice_d_t;
            dst_p->elements[i].a.b.value.d = decoder_read_bool(decoder_p);
            break;

        default:
            decoder_abort(decoder_p, EBADCHOICE);
            break;
        }

        dst_p->elements[i].a.e.length = decoder_read_non_negative_binary_integer(decoder_p, 1);
        dst_p->elements[i].a.e.length += 3;
        dst_p->elements[i].g.h = decoder_read_non_negative_binary_integer(decoder_p, 2);
        dst_p->elements[i].g.l.length = decoder_read_non_negative_binary_integer(decoder_p, 1);
        dst_p->elements[i].g.l.length += 1;

        if (dst_p->elements[i].g.l.length > 2) {
            decoder_abort(decoder_p, EBADLENGTH);

            return;
        }

        decoder_read_bytes(decoder_p,
                           &dst_p->elements[i].g.l.value[0],
                           dst_p->elements[i].g.l.length);
        present_mask = decoder_read_non_negative_binary_integer(decoder_p, 3);
        dst_p->elements[i].m.is_n_present = ((present_mask & 0x04) == 0x04);
        is_m_o_present = ((present_mask & 0x02) == 0x02);
        dst_p->elements[i].m.is_p_present = ((present_mask & 0x01) == 0x01);

        if (dst_p->elements[i].m.is_n_present) {
            dst_p->elements[i].m.n = decoder_read_bool(decoder_p);
        }

        if (is_m_o_present) {
            dst_p->elements[i].m.o = decoder_read_non_negative_binary_integer(decoder_p, 3);
            dst_p->elements[i].m.o -= 3;
        } else {
            dst_p->elements[i].m.o = 3;
        }

        if (dst_p->elements[i].m.is_p_present) {
            present_mask = decoder_read_non_negative_binary_integer(decoder_p, 1);
            dst_p->elements[i].m.p.is_r_present = ((present_mask & 0x01) == 0x01);
            decoder_read_bytes(decoder_p,
                               &dst_p->elements[i].m.p.q.value[0],
                               5);

            if (dst_p->elements[i].m.p.is_r_present) {
                dst_p->elements[i].m.p.r = decoder_read_bool(decoder_p);
            }
        }
    }
}

ssize_t uper_c_source_a_encode(
    uint8_t *dst_p,
    size_t size,
    const struct uper_c_source_a_t *src_p)
{
    struct encoder_t encoder;

    encoder_init(&encoder, dst_p, size);
    uper_c_source_a_encode_inner(&encoder, src_p);

    return (encoder_get_result(&encoder));
}

ssize_t uper_c_source_a_decode(
    struct uper_c_source_a_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    struct decoder_t decoder;

    decoder_init(&decoder, src_p, size);
    uper_c_source_a_decode_inner(&decoder, dst_p);

    return (decoder_get_result(&decoder));
}

ssize_t uper_c_source_b_encode(
    uint8_t *dst_p,
    size_t size,
    const struct uper_c_source_b_t *src_p)
{
    struct encoder_t encoder;

    encoder_init(&encoder, dst_p, size);
    uper_c_source_b_encode_inner(&encoder, src_p);

    return (encoder_get_result(&encoder));
}

ssize_t uper_c_source_b_decode(
    struct uper_c_source_b_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    struct decoder_t decoder;

    decoder_init(&decoder, src_p, size);
    uper_c_source_b_decode_inner(&decoder, dst_p);

    return (decoder_get_result(&decoder));
}

ssize_t uper_c_source_d_encode(
    uint8_t *dst_p,
    size_t size,
    const struct uper_c_source_d_t *src_p)
{
    struct encoder_t encoder;

    encoder_init(&encoder, dst_p, size);
    uper_c_source_d_encode_inner(&encoder, src_p);

    return (encoder_get_result(&encoder));
}

ssize_t uper_c_source_d_decode(
    struct uper_c_source_d_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    struct decoder_t decoder;

    decoder_init(&decoder, src_p, size);
    uper_c_source_d_decode_inner(&decoder, dst_p);

    return (decoder_get_result(&decoder));
}
