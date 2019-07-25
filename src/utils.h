#pragma once
#include "base.h"
#include <vector>
#include <intrin.h>

// TODO: replace this with our own "lsk array"
template<typename T>
struct Array: public std::vector<T>
{
    inline T& push(T elem) {
        push_back(elem);
        return *(end()-1);
    }

    inline void pop() {
        std::vector<T>::pop_back();
    }

    inline void insert(i32 where, T elem) {
        std::vector<T>::insert(begin() + where, elem);
    }

    inline i32 count() const {
        return size();
    }

    inline T& last() {
        return back();
    }
};

/**
 *  List
 *  - growable array
 *  - can't access all elements sequentially (for now) due to buckets
 *  - pointers are not invalidated on resize (grow)
 */
template<typename T, i32 BUCKET_ELT_COUNT=1024>
struct List
{
    struct Bucket
    {
        T* buffer;
		i32 count;
		i32 capacity;
    };

    T stackData[BUCKET_ELT_COUNT];
    Array<Bucket> buckets;

    List() {
        Bucket buck;
        buck.capacity = BUCKET_ELT_COUNT;
        buck.buffer = stackData;
        buck.count = 0;
        buckets.push(buck);
    }

    ~List() {
        const i32 bucketCount = buckets.count();
        for(i32 i = 1; i < bucketCount; ++i) {
            free(buckets[i].buffer);
        }
        buckets.clear();
    }

    inline void _allocNewBucket() {
        const i32 shift = max(0, buckets.count()-1);
        Bucket buck;
        buck.capacity = BUCKET_ELT_COUNT << shift;
        buck.buffer = (T*)malloc(sizeof(T) * buck.capacity);
        buck.count = 0;
        buckets.push(buck);
    }

    inline T& push(T elem) {
        Bucket* buck = &buckets[buckets.count()-1];
        if(buck->count+1 >= buck->capacity) {
            _allocNewBucket();
        }
        buck = &buckets[buckets.count()-1];
        return (buck->buffer[buck->count++] = elem);
    }

    inline void clear() {
        // clear all but the first
        const i32 bucketCount = buckets.count();
        for(i32 i = 1; i < bucketCount; ++i) {
            free(buckets[i].buffer);
        }
        Bucket first = buckets[0];
        first.count = 0;
        buckets.clear();
        buckets.push(first);
    }
};

typedef volatile long int vli32;

struct MutexSpin
{
	vli32 _inUse = 0;

	void lock() {
		while(_InterlockedExchange(&_inUse, 1) == 1) {
			// block
			_mm_pause();
		}
	}

	void unlock() {
		_InterlockedExchange(&_inUse, 0);
	}
};

// Thread safe growable array
template<typename T>
struct ArrayTS
{
	MutexSpin mutexWrite;
	T* bufferRead = nullptr;
	T* buffer = nullptr;
	i32 eltCount = 0;
	i32 capacity;

	~ArrayTS()
	{
		if(buffer) {
			free(buffer);
		}
	}

	inline void reserve(i32 newCapacity)
	{
		if(newCapacity < capacity) return;
		bufferRead = buffer;

		mutexWrite.lock();
		capacity = newCapacity;
		buffer = (T*)malloc(sizeof(T) * capacity);
		memmove(buffer, bufferRead, sizeof(T) * eltCount);
		T* bufferToFree = bufferRead;
		bufferRead = buffer;
		mutexWrite.unlock();

		free(bufferToFree);
	}

	inline T& push(T elt)
	{
		if(eltCount+1 > capacity)
			reserve(capacity * 2);
		return buffer[eltCount++] = elt;
	}

	inline void clear()
	{
		eltCount = 0;
	}

	inline i32 count() const
	{
		return eltCount;
	}

	inline T& operator[](i32 index)
	{
		assert(index < eltCount);
		return bufferRead[index];
	}

	inline const T& operator[](i32 index) const
	{
		assert(index < eltCount);
		return bufferRead[index];
	}
};

struct FileBuffer
{
    u8* data = nullptr;
    i64 size = 0;
};

bool openFileReadAll(const char* path, FileBuffer* out_fb);

inline u32 colorAddAllChannels(u32 color, i32 add)
{
    i32 hoverR = color & 0xff;
    i32 hoverG = (color & 0xff00) >> 8;
    i32 hoverB = (color & 0xff0000) >> 16;
    hoverR = clamp(hoverR + add, 0, 255);
    hoverG = clamp(hoverG + add, 0, 255);
    hoverB = clamp(hoverB + add, 0, 255);
    return (0xff000000 | (hoverB << 16) | (hoverG << 8) | hoverR);
}

inline f32 colorAvgChannel(u32 color)
{
    i32 hoverR = color & 0xff;
    i32 hoverG = (color & 0xff00) >> 8;
    i32 hoverB = (color & 0xff0000) >> 16;
    f32 total = hoverR + hoverG + hoverB;
    return total / 3.0f;
}

inline u32 colorOne(u8 chan1)
{
    return 0xff000000 | (chan1 << 16) | (chan1 << 8) | chan1;
}

inline u32 hash32(const void* data, const u32 dataLen)
{
    assert(dataLen > 0);
    u32 h = 0x811c9dc5;
    for(u32 i = 0; i < dataLen; ++i) {
        h = h ^ ((u8*)data)[i];
        h = h * 16777619;
    }
    return h;
}

template<i32 STR_LEN>
struct StrT
{
    char str[STR_LEN];
    i32 len = 0;

    StrT() = default;

    StrT(const char* _str) {
        set(_str);
    }

    void set(const char* _str) {
        len = strlen(_str);
        assert(len < STR_LEN);
        memmove(str, _str, len);
        str[len] = 0;
    }

    void set(const char* _str, i32 _len) {
        assert(_len < STR_LEN);
        memmove(str, _str, _len);
        str[_len] = 0;
        len = _len;
    }
};

typedef StrT<32> Str32;
typedef StrT<64> Str64;

union Color3
{
    struct { f32 r, g, b; };
    struct { f32 h, s, v; };
    f32 data[3];
};

inline u32 rgbU32Lerp(u32 col1, u32 col2, f32 alpha)
{
    const u8 r1 = col1 & 0xFF;
    const u8 g1 = (col1 & 0xFF00) >> 8;
    const u8 b1 = (col1 & 0xFF0000) >> 16;
    const u8 r2 = col2 & 0xFF;
    const u8 g2 = (col2 & 0xFF00) >> 8;
    const u8 b2 = (col2 & 0xFF0000) >> 16;
    const u8 r = r1 * (1.0f - alpha) + r2 * alpha;
    const u8 g = g1 * (1.0f - alpha) + g2 * alpha;
    const u8 b = b1 * (1.0f - alpha) + b2 * alpha;
    return 0xff000000 | (b << 16) | (g << 8) | r;
}

inline Color3 rgbLerp(const Color3& rgb1, const Color3& rgb2, const f32 alpha)
{
    return Color3 {
        rgb1.r * (1.0f - alpha) + rgb2.r * alpha,
        rgb1.g * (1.0f - alpha) + rgb2.g * alpha,
        rgb1.b * (1.0f - alpha) + rgb2.b * alpha,
    };
}

inline u32 rgbToU32(const Color3& rgb)
{
    return 0xff000000 | (i32(rgb.b * 0xff) << 16) |
           (i32(rgb.g * 0xff) << 8) | i32(rgb.r * 0xff);
}

inline void u32ToRgb(const u32 col, f32* out)
{
    const u8 r = col & 0xFF;
    const u8 g = (col & 0xFF00) >> 8;
    const u8 b = (col & 0xFF0000) >> 16;
    out[0] = r / 255.f;
    out[1] = g / 255.f;
    out[2] = b / 255.f;
}

inline void rgbToCmyk(u32 rbg, f32* cmyk)
{
    const u8 r = rbg & 0xFF;
    const u8 g = (rbg & 0xFF00) >> 8;
    const u8 b = (rbg & 0xFF0000) >> 16;

    const f32 k = min(255 - r, min(255 - g, 255 - b));
    const f32 c = 255 * (255 - r - k) / (255 - k);
    const f32 m = 255 * (255 - g - k) / (255 - k);
    const f32 y = 255 * (255 - b - k) / (255 - k);

    cmyk[0] = c;
    cmyk[1] = m;
    cmyk[2] = y;
    cmyk[3] = k;
}

inline void cmykToRgb(const f32* cmyk, u8* rgb)
{
    const f32 c = cmyk[0];
    const f32 m = cmyk[1];
    const f32 y = cmyk[2];
    const f32 k = cmyk[3];
    rgb[0] = -((c * (255-k)) / 255 + k - 255);
    rgb[1] = -((m * (255-k)) / 255 + k - 255);
    rgb[2] = -((y * (255-k)) / 255 + k - 255);
}

inline f32 vec3Dot(const f32* v1, const f32* v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

inline void rgbToXyz(const f32* normRgb, f32* xyz)
{
    // Reference:
    // RGB/XYZ Matrices
    // http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
    const f32 vx[3] = {0.4124564f, 0.3575761f, 0.1804375f};
    const f32 vy[3] = {0.2126729f, 0.7151522f, 0.0721750f};
    const f32 vz[3] = {0.0193339f, 0.1191920f, 0.9503041f};
    xyz[0] = vec3Dot(vx, normRgb);
    xyz[1] = vec3Dot(vy, normRgb);
    xyz[2] = vec3Dot(vz, normRgb);
}

inline void xyzToRgb(const f32* xyz, f32* normRgb)
{
    const f32 vx[3] = {3.2404542f, -1.5371385f, -0.4985314f};
    const f32 vy[3] = {-0.9692660f, 1.8760108f, 0.0415560f};
    const f32 vz[3] = {0.0556434f, -0.2040259f, 1.0572252f};
    normRgb[0] = vec3Dot(vx, xyz);
    normRgb[1] = vec3Dot(vy, xyz);
    normRgb[2] = vec3Dot(vz, xyz);
}

inline void rgbToHsv(const Color3& rgb, Color3* hsv)
{
    f32 r = rgb.r;
    f32 g = rgb.g;
    f32 b = rgb.b;

    f32 K = 0.f;
    if(g < b) {
        swap(&g, &b);
        K = -1.f;
    }
    if(r < g) {
        swap(&r, &g);
        K = -2.f / 6.f - K;
    }

    const f32 chroma = r - (g < b ? g : b);
    const f32 h = fabsf(K + (g - b) / (6.f * chroma + 1e-20f));
    const f32 s = chroma / (r + 1e-20f);
    const f32 v = r;
    *hsv = { h, s, v };
}

inline void hsvToRgb(const Color3& hsv, Color3* rgb)
{
    f32 h = hsv.h;
    f32 s = hsv.s;
    f32 v = hsv.v;
    f32& out_r = rgb->r;
    f32& out_g = rgb->g;
    f32& out_b = rgb->b;

    if(s == 0.0f) {
        // gray
        out_r = out_g = out_b = v;
        return;
    }

    h = fmodf(h, 1.0f) / (60.0f/360.0f);
    i32 i = h;
    f32 f = h - (f32)i;
    f32 p = v * (1.0f - s);
    f32 q = v * (1.0f - s * f);
    f32 t = v * (1.0f - s * (1.0f - f));

    switch(i) {
        case 0: out_r = v; out_g = t; out_b = p; break;
        case 1: out_r = q; out_g = v; out_b = p; break;
        case 2: out_r = p; out_g = v; out_b = t; break;
        case 3: out_r = p; out_g = q; out_b = v; break;
        case 4: out_r = t; out_g = p; out_b = v; break;
        case 5: default: out_r = v; out_g = p; out_b = q; break;
    }
}

inline void hsvLerp(const f32* hsv1, const f32* hsv2, f32 alpha, f32* hsvOut)
{
    f32 h1 = hsv1[0];
    f32 h2 = hsv2[0];

    // hue interpolation
    f32 d = h2 - h1;
    if(d > 0.5f) {
        h1 += 1.0f;
    }
    else if(d < -0.5f) {
        h2 += 1.0f;
    }

    hsvOut[0] = fmodf(lerp(h1, h2, alpha), 1.0f);
    hsvOut[1] = lerp(hsv1[1], hsv2[1], alpha);
    hsvOut[2] = lerp(hsv1[2], hsv2[2], alpha);
}

inline f32 rgbGetBrightness(const Color3& rgbIn)
{
    return 0.299f * rgbIn.r + 0.587f * rgbIn.g + 0.114f * rgbIn.b;
}

inline Color3 rgbGetLighterColor(const Color3& rgbIn, const f32 lightVal)
{
    Color3 hsv, rgbOut;
    rgbToHsv(rgbIn, &hsv);
    hsv.s = max(0.0f, hsv.s - lightVal);
    hsv.v = min(1.0f, hsv.v + lightVal);
    hsvToRgb(hsv, &rgbOut);
    return rgbOut;
}

template<typename T>
struct GrowableBufferT
{
	T* data = nullptr;
	i32 capacity = 0;
	i32 size = 0;

	~GrowableBufferT() {
		release();
	}

	inline void init(i32 size_) {
		assert(data == nullptr);
		data = (T*)malloc(size_);
		capacity = size_;
		size = 0;
	}

	inline void reserve(i32 size_) {
		if(capacity < size_) {
			i32 newCapacity = max(capacity * 2, size_);
			data = (T*)realloc(data, newCapacity);
			capacity = newCapacity;
		}
	}

	inline void release() {
		if(data) {
			free(data);
			data = nullptr;
		}
	}
};

typedef GrowableBufferT<void> GrowableBuffer;
