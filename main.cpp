#include <iostream>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavfilter/avfilter.h>
}

AVDictionary* createAVDictionary() {
    AVDictionary *opts = NULL;              //create an empty dictionary
    int result = av_dict_set(&opts, "key", "value", 0);  // add an entry
    return (result >= 0) ? opts : nullptr;
}

void webCam() {
    const char *filename = "/dev/video0/";
    AVInputFormat *iformat = av_find_input_format("dshow");
    AVFrame *pFrame,  *pFrameRGB;

}
int main() {
    /* av_register_all(); ffmepg 4.0 부터 삭제, 적용 안해도됨.  */
    /* 영상 디코딩 */
    AVCodec * codec;              // 코덱 정보를 나타냄 (ex. MPEG, MJPEG, H264 등)
    AVCodecContext *av_context;      // 스트림을 어떻게 디코딩 할 것인지에 대한 정보
    AVFormatContext *av_FormatContext = avformat_alloc_context(); // av format
    AVDictionary *av_dict = createAVDictionary();

    codec = avcodec_find_decoder(AV_CODEC_ID_H264);  // H264로 초기화
    if (!codec) {
        std::cout << " cannot find AV_CODEC_ID_H264 " << std::endl;
        exit(-1);
    }

    av_context = avcodec_alloc_context3(codec); // avcodec 정보를 가지고 context를 초기화, 메모리 할당.
    av_context -> width = 1280;
    av_context -> height = 720;
    av_context -> codec_id = AV_CODEC_ID_H264;
    av_context -> codec_type = AVMEDIA_TYPE_VIDEO;
    av_context -> flags2 |= AV_CODEC_FLAG2_FAST; // 뭘까
    av_context -> thread_count = 0; // 0 은 알아서 스레드 생성.

    // AVDictionary options
    if (av_dict == nullptr) {
        std::cout << " AVDictionary Null Pointer " << std::endl;
        exit(-1);
    }

    /* FFMPEG 라이브러리 설정 */
    avcodec_open2(av_context, codec, &av_dict);
    /* 위 작업까지 디코딩 준비 완료, 스트림 데이터가 있는 변수를 ffmpeg 라이브러리에 넘겨주고 디코딩 시키면 됨. */
    return 0;
}