
#include "diff_types.h"
#include <vector>

static const int kMinSingleMatchScore_default = 6;

// create a diff data between oldData and newData
//  out_diff is uncompressed, you can use create_compressed_diff()
//       or create_single_compressed_diff() create compressed diff data
//  recommended always use create_single_compressed_diff() replace create_diff()
//  kMinSingleMatchScore: default 6, bin: 0--4  text: 4--9
//  isUseBigCacheMatch: big cache max used O(oldSize) memory, match speed
//  faster, but build big cache slow
void create_diff(const unsigned char *newData, const unsigned char *newData_end,
                 const unsigned char *oldData, const unsigned char *oldData_end,
                 std::vector<unsigned char> &out_diff,
                 int kMinSingleMatchScore = kMinSingleMatchScore_default,
                 bool isUseBigCacheMatch = false, size_t threadNum = 1);

// return patch(oldData+diff)==newData?
bool check_diff(const unsigned char *newData, const unsigned char *newData_end,
                const unsigned char *oldData, const unsigned char *oldData_end,
                const unsigned char *diff, const unsigned char *diff_end);
bool check_diff(const hpatch_TStreamInput *newData,
                const hpatch_TStreamInput *oldData,
                const hpatch_TStreamInput *diff);

// create a compressed diff data between oldData and newData
//  out_diff compressed by compressPlugin
//  recommended always use create_single_compressed_diff() replace
//  create_compressed_diff() kMinSingleMatchScore: default 6, bin: 0--4  text:
//  4--9 isUseBigCacheMatch: big cache max used O(oldSize) memory, match speed
//  faster, but build big cache slow
void create_compressed_diff(
    const unsigned char *newData, const unsigned char *newData_end,
    const unsigned char *oldData, const unsigned char *oldData_end,
    std::vector<unsigned char> &out_diff,
    const hdiff_TCompress *compressPlugin = 0,
    int kMinSingleMatchScore = kMinSingleMatchScore_default,
    bool isUseBigCacheMatch = false, ICoverLinesListener *listener = 0,
    size_t threadNum = 1);
void create_compressed_diff(
    const unsigned char *newData, const unsigned char *newData_end,
    const unsigned char *oldData, const unsigned char *oldData_end,
    const hpatch_TStreamOutput *out_diff,
    const hdiff_TCompress *compressPlugin = 0,
    int kMinSingleMatchScore = kMinSingleMatchScore_default,
    bool isUseBigCacheMatch = false, ICoverLinesListener *listener = 0,
    size_t threadNum = 1);

// create a compressed diff data by stream:
//  can control memory requires and run speed by different kMatchBlockSize
//  value,
//      but out_diff size is larger than create_compressed_diff()
//  recommended used in limited environment or support large file
//  recommended always use create_single_compressed_diff_stream() replace
//  create_compressed_diff_stream() kMatchBlockSize: recommended (1<<4)--(1<<14)
//    if increase kMatchBlockSize then run faster and require less memory, but
//    out_diff size increase
//  NOTICE: out_diff->write()'s writeToPos may be back to update headData!
//  throw std::runtime_error when I/O error,etc.
static const size_t kMatchBlockSize_default = (1 << 6);
static const size_t kMatchBlockSize_min = 4;
void create_compressed_diff_stream(
    const hpatch_TStreamInput *newData, const hpatch_TStreamInput *oldData,
    const hpatch_TStreamOutput *out_diff,
    const hdiff_TCompress *compressPlugin = 0,
    size_t kMatchBlockSize = kMatchBlockSize_default,
    const hdiff_TMTSets_s *mtsets = 0);

// return patch_decompress(oldData+diff)==newData?
bool check_compressed_diff(const unsigned char *newData,
                           const unsigned char *newData_end,
                           const unsigned char *oldData,
                           const unsigned char *oldData_end,
                           const unsigned char *diff,
                           const unsigned char *diff_end,
                           hpatch_TDecompress *decompressPlugin);
bool check_compressed_diff(const hpatch_TStreamInput *newData,
                           const hpatch_TStreamInput *oldData,
                           const hpatch_TStreamInput *compressed_diff,
                           hpatch_TDecompress *decompressPlugin);
// check_compressed_diff_stream rename to check_compressed_diff

// resave compressed_diff
//  decompress in_diff and recompress to out_diff
//  throw std::runtime_error when input file error or I/O error,etc.
void resave_compressed_diff(const hpatch_TStreamInput *in_diff,
                            hpatch_TDecompress *decompressPlugin,
                            const hpatch_TStreamOutput *out_diff,
                            const hdiff_TCompress *compressPlugin,
                            hpatch_StreamPos_t out_diff_curPos = 0);

static const size_t kDefaultPatchStepMemSize = 1024 * 256;

// create a diff data between oldData and newData, the diffData saved as single
// compressed stream
//  kMinSingleMatchScore: default 6, bin: 0--4  text: 4--9
//  patchStepMemSize>=hpatch_kStreamCacheSize, default 256k, recommended 64k,2m
//  etc... isUseBigCacheMatch: big cache max used O(oldSize) memory, match speed
//  faster, but build big cache slow
void create_single_compressed_diff(
    const unsigned char *newData, const unsigned char *newData_end,
    const unsigned char *oldData, const unsigned char *oldData_end,
    std::vector<unsigned char> &out_diff,
    const hdiff_TCompress *compressPlugin = 0,
    int kMinSingleMatchScore = kMinSingleMatchScore_default,
    size_t patchStepMemSize = kDefaultPatchStepMemSize,
    bool isUseBigCacheMatch = false, ICoverLinesListener *listener = 0,
    size_t threadNum = 1);
void create_single_compressed_diff(
    const unsigned char *newData, const unsigned char *newData_end,
    const unsigned char *oldData, const unsigned char *oldData_end,
    const hpatch_TStreamOutput *out_diff,
    const hdiff_TCompress *compressPlugin = 0,
    int kMinSingleMatchScore = kMinSingleMatchScore_default,
    size_t patchStepMemSize = kDefaultPatchStepMemSize,
    bool isUseBigCacheMatch = false, ICoverLinesListener *listener = 0,
    size_t threadNum = 1);
// create single compressed diff data by stream:
//  can control memory requires and run speed by different kMatchBlockSize
//  value,
//      but out_diff size is larger than create_single_compressed_diff()
//  recommended used in limited environment or support large file
//  kMatchBlockSize: recommended (1<<4)--(1<<14)
//    if increase kMatchBlockSize then run faster and require less memory, but
//    out_diff size increase
//  NOTICE: out_diff->write()'s writeToPos may be back to update headData!
//  throw std::runtime_error when I/O error,etc.
void create_single_compressed_diff_stream(
    const hpatch_TStreamInput *newData, const hpatch_TStreamInput *oldData,
    const hpatch_TStreamOutput *out_diff,
    const hdiff_TCompress *compressPlugin = 0,
    size_t kMatchBlockSize = kMatchBlockSize_default,
    size_t patchStepMemSize = kDefaultPatchStepMemSize,
    const hdiff_TMTSets_s *mtsets = 0);

// return patch_single_?(oldData+diff)==newData?
bool check_single_compressed_diff(const unsigned char *newData,
                                  const unsigned char *newData_end,
                                  const unsigned char *oldData,
                                  const unsigned char *oldData_end,
                                  const unsigned char *diff,
                                  const unsigned char *diff_end,
                                  hpatch_TDecompress *decompressPlugin);
bool check_single_compressed_diff(const hpatch_TStreamInput *newData,
                                  const hpatch_TStreamInput *oldData,
                                  const hpatch_TStreamInput *diff,
                                  hpatch_TDecompress *decompressPlugin);

// resave single_compressed_diff
//  decompress in_diff and recompress to out_diff
//  throw std::runtime_error when input file error or I/O error,etc.
void resave_single_compressed_diff(
    const hpatch_TStreamInput *in_diff, hpatch_TDecompress *decompressPlugin,
    const hpatch_TStreamOutput *out_diff, const hdiff_TCompress *compressPlugin,
    const hpatch_singleCompressedDiffInfo *diffInfo = 0,
    hpatch_StreamPos_t in_diff_curPos = 0,
    hpatch_StreamPos_t out_diff_curPos = 0);

// same as create?compressed_diff_stream(), but not serialize diffData, only got
// covers
void get_match_covers_by_block(const hpatch_TStreamInput *newData,
                               const hpatch_TStreamInput *oldData,
                               hpatch_TOutputCovers *out_covers,
                               size_t kMatchBlockSize,
                               const hdiff_TMTSets_s *mtsets);
void get_match_covers_by_block(const unsigned char *newData,
                               const unsigned char *newData_end,
                               const unsigned char *oldData,
                               const unsigned char *oldData_end,
                               hpatch_TOutputCovers *out_covers,
                               size_t kMatchBlockSize, size_t threadNum);

// same as create?_diff(), but not serialize diffData, only got covers
void get_match_covers_by_sstring(
    const unsigned char *newData, const unsigned char *newData_end,
    const unsigned char *oldData, const unsigned char *oldData_end,
    hpatch_TOutputCovers *out_covers,
    int kMinSingleMatchScore = kMinSingleMatchScore_default,
    bool isUseBigCacheMatch = false, ICoverLinesListener *listener = 0,
    size_t threadNum = 1, bool isCanExtendCover = true);
void get_match_covers_by_sstring(
    const unsigned char *newData, const unsigned char *newData_end,
    const unsigned char *oldData, const unsigned char *oldData_end,
    std::vector<hpatch_TCover_sz> &out_covers,
    int kMinSingleMatchScore = kMinSingleMatchScore_default,
    bool isUseBigCacheMatch = false, ICoverLinesListener *listener = 0,
    size_t threadNum = 1, bool isCanExtendCover = true);

// -------------------------------------------------------------------------------
// PATCH ---------------------------------------------------------------------//

// generate newData by patch(oldData + serializedDiff)
//  serializedDiff create by create_diff()
hpatch_BOOL patch(unsigned char *out_newData, unsigned char *out_newData_end,
                  const unsigned char *oldData,
                  const unsigned char *oldData_end,
                  const unsigned char *serializedDiff,
                  const unsigned char *serializedDiff_end);

// patch by stream, see patch()
//  used (hpatch_kStreamCacheSize*8 stack memory) for I/O cache
//  if use patch_stream_with_cache(), can passing larger memory cache to
//  optimize speed serializedDiff create by create_diff()
hpatch_BOOL
patch_stream(const hpatch_TStreamOutput *out_newData,    // sequential write
             const hpatch_TStreamInput *oldData,         // random read
             const hpatch_TStreamInput *serializedDiff); // random read

// see patch_stream()
//  can passing more memory for I/O cache to optimize speed
//  note: (temp_cache_end-temp_cache)>=2048
hpatch_BOOL patch_stream_with_cache(
    const hpatch_TStreamOutput *out_newData,   // sequential write
    const hpatch_TStreamInput *oldData,        // random read
    const hpatch_TStreamInput *serializedDiff, // random read
    unsigned char *temp_cache, unsigned char *temp_cache_end);

// get compressedDiff info
//  compressedDiff created by create_compressed_diff() or
//  create_compressed_diff_stream()
hpatch_BOOL getCompressedDiffInfo(hpatch_compressedDiffInfo *out_diffInfo,
                                  const hpatch_TStreamInput *compressedDiff);
// see getCompressedDiffInfo()
hpatch_inline static hpatch_BOOL
getCompressedDiffInfo_mem(hpatch_compressedDiffInfo *out_diffInfo,
                          const unsigned char *compressedDiff,
                          const unsigned char *compressedDiff_end) {
  hpatch_TStreamInput diffStream;
  mem_as_hStreamInput(&diffStream, compressedDiff, compressedDiff_end);
  return getCompressedDiffInfo(out_diffInfo, &diffStream);
}

// patch with decompress plugin
//  used (hpatch_kStreamCacheSize*6 stack memory) + (decompress buffer*4)
//  compressedDiff create by create_compressed_diff() or
//  create_compressed_diff_stream() decompressPlugin can null when no compressed
//  data in compressedDiff if use patch_decompress_with_cache(), can passing
//  larger memory cache to optimize speed
hpatch_BOOL
patch_decompress(const hpatch_TStreamOutput *out_newData,   // sequential write
                 const hpatch_TStreamInput *oldData,        // random read
                 const hpatch_TStreamInput *compressedDiff, // random read
                 hpatch_TDecompress *decompressPlugin);

// see patch_decompress()
//  can passing larger memory cache to optimize speed
//  note: (temp_cache_end-temp_cache)>=2048
hpatch_BOOL patch_decompress_with_cache(
    const hpatch_TStreamOutput *out_newData,   // sequential write
    const hpatch_TStreamInput *oldData,        // random read
    const hpatch_TStreamInput *compressedDiff, // random read
    hpatch_TDecompress *decompressPlugin, unsigned char *temp_cache,
    unsigned char *temp_cache_end);

// see patch_decompress()
hpatch_inline static hpatch_BOOL
patch_decompress_mem(unsigned char *out_newData, unsigned char *out_newData_end,
                     const unsigned char *oldData,
                     const unsigned char *oldData_end,
                     const unsigned char *compressedDiff,
                     const unsigned char *compressedDiff_end,
                     hpatch_TDecompress *decompressPlugin) {
  hpatch_TStreamOutput out_newStream;
  hpatch_TStreamInput oldStream;
  hpatch_TStreamInput diffStream;
  mem_as_hStreamOutput(&out_newStream, out_newData, out_newData_end);
  mem_as_hStreamInput(&oldStream, oldData, oldData_end);
  mem_as_hStreamInput(&diffStream, compressedDiff, compressedDiff_end);
  return patch_decompress(&out_newStream, &oldStream, &diffStream,
                          decompressPlugin);
}

// hpatch_TCoverList: open diffData and read coverList
typedef struct hpatch_TCoverList {
  hpatch_TCovers *ICovers;
  // private:
  unsigned char _buf[hpatch_kStreamCacheSize * 4];
} hpatch_TCoverList;

hpatch_inline static void hpatch_coverList_init(hpatch_TCoverList *coverList) {
  assert(coverList != 0);
  memset(coverList, 0, sizeof(*coverList) - sizeof(coverList->_buf));
}
//  serializedDiff create by create_diff()
hpatch_BOOL
hpatch_coverList_open_serializedDiff(hpatch_TCoverList *out_coverList,
                                     const hpatch_TStreamInput *serializedDiff);
//  compressedDiff create by create_compressed_diff() or
//  create_compressed_diff_stream()
hpatch_BOOL
hpatch_coverList_open_compressedDiff(hpatch_TCoverList *out_coverList,
                                     const hpatch_TStreamInput *compressedDiff,
                                     hpatch_TDecompress *decompressPlugin);
hpatch_inline static hpatch_BOOL
hpatch_coverList_close(hpatch_TCoverList *coverList) {
  hpatch_BOOL result = hpatch_TRUE;
  if ((coverList != 0) && (coverList->ICovers)) {
    result = coverList->ICovers->close(coverList->ICovers);
    hpatch_coverList_init(coverList);
  }
  return result;
}

// patch singleCompressedDiff with listener
//	used (stepMemSize memory) + (I/O cache memory) + (decompress buffer*1)
//  every byte in singleCompressedDiff will only be read once in order
//  singleCompressedDiff create by create_single_compressed_diff() or
//  create_single_compressed_diff_stream() you can download&patch diffData at
//  the same time, without saving it to disk same as call
//  getSingleCompressedDiffInfo() + listener->onDiffInfo() +
//  patch_single_compressed_diff()
hpatch_BOOL patch_single_stream(
    sspatch_listener_t *listener,            // call back when got diffInfo
    const hpatch_TStreamOutput *out_newData, // sequential write
    const hpatch_TStreamInput *oldData,      // random read
    const hpatch_TStreamInput
        *singleCompressedDiff, // sequential read every byte
    hpatch_StreamPos_t
        diffInfo_pos, // default 0, begin pos in singleCompressedDiff
    sspatch_coversListener_t
        *coversListener // default NULL, call by on got covers
);
static hpatch_inline hpatch_BOOL patch_single_stream_mem(
    sspatch_listener_t *listener, unsigned char *out_newData,
    unsigned char *out_newData_end, const unsigned char *oldData,
    const unsigned char *oldData_end, const unsigned char *diff,
    const unsigned char *diff_end, sspatch_coversListener_t *coversListener) {
  hpatch_TStreamOutput out_newStream;
  hpatch_TStreamInput oldStream;
  hpatch_TStreamInput diffStream;
  mem_as_hStreamOutput(&out_newStream, out_newData, out_newData_end);
  mem_as_hStreamInput(&oldStream, oldData, oldData_end);
  mem_as_hStreamInput(&diffStream, diff, diff_end);
  return patch_single_stream(listener, &out_newStream, &oldStream, &diffStream,
                             0, coversListener);
}

// get singleCompressedDiff info
//  singleCompressedDiff create by create_single_compressed_diff() or
//  create_single_compressed_diff_stream()
hpatch_BOOL getSingleCompressedDiffInfo(
    hpatch_singleCompressedDiffInfo *out_diffInfo,
    const hpatch_TStreamInput *singleCompressedDiff, // sequential read
    hpatch_StreamPos_t
        diffInfo_pos // default 0, begin pos in singleCompressedDiff
);
hpatch_inline static hpatch_BOOL
getSingleCompressedDiffInfo_mem(hpatch_singleCompressedDiffInfo *out_diffInfo,
                                const unsigned char *singleCompressedDiff,
                                const unsigned char *singleCompressedDiff_end) {
  hpatch_TStreamInput diffStream;
  mem_as_hStreamInput(&diffStream, singleCompressedDiff,
                      singleCompressedDiff_end);
  return getSingleCompressedDiffInfo(out_diffInfo, &diffStream, 0);
}

// patch singleCompressedDiff with diffInfo
//	used (stepMemSize memory) + (I/O cache memory) + (decompress buffer*1)
//	note: (I/O cache memory) >= hpatch_kStreamCacheSize*3
//  temp_cache_end-temp_cache == stepMemSize + (I/O cache memory)
//  singleCompressedDiff create by create_single_compressed_diff() or
//  create_single_compressed_diff_stream() decompressPlugin can null when no
//  compressed data in singleCompressedDiff same as call
//  compressed_stream_as_uncompressed() + patch_single_stream_diff()
hpatch_BOOL patch_single_compressed_diff(
    const hpatch_TStreamOutput *out_newData,         // sequential write
    const hpatch_TStreamInput *oldData,              // random read
    const hpatch_TStreamInput *singleCompressedDiff, // sequential read
    hpatch_StreamPos_t
        diffData_pos, // diffData begin pos in singleCompressedDiff
    hpatch_StreamPos_t uncompressedSize, hpatch_StreamPos_t compressedSize,
    hpatch_TDecompress *decompressPlugin, hpatch_StreamPos_t coverCount,
    hpatch_size_t stepMemSize, unsigned char *temp_cache,
    unsigned char *temp_cache_end,
    sspatch_coversListener_t
        *coversListener // default NULL, call by on got covers
);

hpatch_BOOL compressed_stream_as_uncompressed(
    hpatch_TUncompresser_t *uncompressedStream,
    hpatch_StreamPos_t uncompressedSize, hpatch_TDecompress *decompressPlugin,
    const hpatch_TStreamInput *compressedStream,
    hpatch_StreamPos_t compressed_pos, hpatch_StreamPos_t compressed_end);
void close_compressed_stream_as_uncompressed(
    hpatch_TUncompresser_t *uncompressedStream);

hpatch_BOOL patch_single_stream_diff(
    const hpatch_TStreamOutput *out_newData,         // sequential write
    const hpatch_TStreamInput *oldData,              // random read
    const hpatch_TStreamInput *uncompressedDiffData, // sequential read
    hpatch_StreamPos_t
        diffData_pos, // diffData begin pos in uncompressedDiffData
    hpatch_StreamPos_t
        diffData_posEnd, // diffData end pos in uncompressedDiffData
    hpatch_StreamPos_t coverCount, hpatch_size_t stepMemSize,
    unsigned char *temp_cache, unsigned char *temp_cache_end,
    sspatch_coversListener_t *coversListener);