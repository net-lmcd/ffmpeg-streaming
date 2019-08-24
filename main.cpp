#include <iostream>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/codec2utils.h>
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avio.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

/**
 * av option 세팅
 * @return
 */
AVDictionary* createAVDictionary() {
    AVDictionary *options = NULL;              //create an empty dictionary
    av_dict_set(&options, "video_size", "640x480", 0);  // add an entry
    av_dict_set(&options, "pixel_format", "rgb24", 0);
    av_dict_set(&options, "framerate", "30", 0);
    return options;
}

int main() {
    avcodec_register_all();
    /* av_register_all(); ffmepg 4.0 부터 삭제, 적용 안해도됨.  */
    /* 영상 디코딩 */
    AVCodec *codec;              // 코덱 정보를 나타냄 (ex. MPEG, MJPEG, H264 등)
    AVCodecContext *av_context;      // 스트림을 어떻게 디코딩 할 것인지에 대한 정보
    AVDictionary *av_dict = createAVDictionary();

    codec = avcodec_find_decoder(AV_CODEC_ID_H264);  // H264로 초기화
    if (!codec) {
        std::cout << " cannot find AV_CODEC_ID_H264 " << std::endl;
        exit(-1);
    }

    av_context = avcodec_alloc_context3(codec); // avcodec 정보를 가지고 context를 초기화, 메모리 할당.
    av_context->width = 1280;
    av_context->height = 720;
    av_context->codec_id = AV_CODEC_ID_H264;
    av_context->codec_type = AVMEDIA_TYPE_VIDEO;
    av_context->flags2 |= AV_CODEC_FLAG2_FAST; // 뭘까
    av_context->thread_count = 0; // 0 은 알아서 스레드 생성.

    // AVDictionary options
    if (av_dict == nullptr) {
        std::cout << " AVDictionary Null Pointer " << std::endl;
        exit(-1);
    }

    /* FFMPEG 라이브러리 설정 */
    avcodec_open2(av_context, codec, &av_dict);
    /* 위 작업까지 디코딩 준비 완료, 스트림 데이터가 있는 변수를 ffmpeg 라이브러리에 넘겨주고 디코딩 시키면 됨. */

    const char *filename = "/dev/video0/";
    AVFrame *pFrame, *pFrameRGB;
    AVInputFormat *avi_format = av_find_input_format("v4l2"); // Find AVInputFormat based on the short name of the input format.
    AVFormatContext *av_formatContext = avformat_alloc_context(); // av format context
    AVCodecContext *av_codecContext;
    AVCodecParameters * av_codecParam;
    AVCodec *av_codec;

    /* avformat_open_input(~);
        * Open an input stream and read the header. The codecs are not opened.
        * The stream must be closed with avformat_close_input().
        *
        * avformat_find_stream_info()
        * Read packets of a media file to get stream information. This
        * is useful for file formats with no headers such as MPEG
        *
        * av_dump_format
        * Print detailed information about the input or output format, such as
        * duration, bitrate, streams,
    */

    if (avformat_open_input(&av_formatContext, filename, avi_format, &av_dict) < 0) // open an input stream and read the header.
        abort();
    av_dict_free(&av_dict);

    int find_stream = avformat_find_stream_info(av_formatContext, &av_dict); // read packets of a media file to get stream info
    if (find_stream < 0) {
        std::cout << " avformat_find_stream_info failed " << std::endl;
        exit(-1);
    }

    av_dump_format(av_formatContext, 0, filename, 0); // print detailed information about input/output
    int videoStream = 1;

    for (int i = 0; i < av_formatContext -> nb_streams; i++) {
        AVStream *avStream = av_formatContext -> streams[i]; // get stream
        AVMediaType type = avStream -> codecpar -> codec_type; // get codec type
        if (type == AVMEDIA_TYPE_VIDEO) { // AVMEDIA_TYPE_VIDEO
            videoStream = i;
            avStream = nullptr;
            break;
        }
        avStream = nullptr;
    }

    if (videoStream == -1) {
        std::cout << " videoStream not valied " << std::endl;
        exit(-1);
    }
    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    av_codecParam = av_formatContext -> streams[videoStream] -> codecpar;
    std::cout << " codec Id : " << av_codecParam -> codec_id << std::endl;
    av_codec = avcodec_find_decoder(av_codecParam -> codec_id);
    avcodec_parameters_to_context(av_codecContext, av_codecParam); // av_codecParam to avcodecContext;

    if (av_codec == NULL) {
        std::cout << " codec not found " << std::endl;
        exit(-1);
    }

    if (avcodec_open2(av_codecContext, av_codec, NULL) < 0) {     //  Initialize the AVCodecContext to use the given AVCodec.
        std::cout << " avcodec_open2 failed " << std::endl;
        exit(-1);
    }

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    uint8_t *buffer;
    int size;

    AVPixelFormat av_pixelFormat = AV_PIX_FMT_BGR24;
    size = av_image_get_buffer_size(av_pixelFormat, av_codecContext -> width, av_codecContext -> height, av_codecContext -> block_align); // align 은 뭘까

    return 0;
}




















