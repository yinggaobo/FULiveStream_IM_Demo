//
//  lsMediaCapture.h
//  lsMediaCapture
//
//  Created by NetEase on 15/8/12.
//  Copyright (c) 2015年 NetEase. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "nMediaLiveStreamingDefs.h"
#import "LSVideoParaCtxConfiguration.h"
#import "LSLiveStreamingParaCtxConfiguration.h"

/**
 *  直播中的NSNotificationCenter消息广播
 */
extern NSString * const LS_LiveStreaming_Started;//!< 直播推流已经开始
extern NSString * const LS_LiveStreaming_Finished;//!< 直播推流已经结束
extern NSString * const LS_LiveStreaming_Bad;//!< 直播推流状况不好，建议降低分辨率
extern NSString * const LS_AudioFile_eof;//!< 当前audio文件播放结束

/**
 *  @brief 获取最新一帧视频截图后的回调
 *
 *  @param latestFrameImage 最新一帧视频截图
 */
typedef void(^LSFrameCaptureCompletionBlock)(UIImage *latestFrameImage);

/**
 切换摄像头回调
 */
typedef void(^LSSwitchModuleVideoCameraPositionBlock)();

/**
 切换分辨率回调
 */
typedef void(^LSVideoStreamingQualityBlock)(LSVideoStreamingQuality quality);

/**
 *  @brief 直播类LSMediacapture，用于推流
 */
@interface LSMediaCapture : NSObject

/**
 *
 *  推流地址
 */
@property(nonatomic,copy)NSString* pushUrl;

/**
 *  直播过程中发生错误的回调函数
 *
 *  @param error 具体错误信息
 */
@property (nonatomic,copy) void (^onLiveStreamError)(NSError *error);

/**
 *  得到直播过程中的统计信息
 *
 *  @param statistics 统计信息结构体
 *
 */
@property (nonatomic,copy) void (^onStatisticInfoGot)(LSStatistics* statistics);

/**
 *  初始化mediacapture
 *
 *  @param  liveStreamingURL 推流的url地址
 *
 *  @return LSMediaCapture
 */
- (instancetype)initLiveStream:(NSString *)liveStreamingURL;

/**
 初始化mediacapture
 
 @param liveStreamingURL 推流的url
 @param videoParaCtx 推流视频参数
 @return LSMediaCapture
 */
- (instancetype)initLiveStream:(NSString *)liveStreamingURL withVideoParaCtxConfiguration:(LSVideoParaCtxConfiguration *)videoParaCtx;

/**
 初始化mediacapture
 
 @param liveStreamingURL 推流的url
 @param lsParaCtx 推流参数
 @return LSMediaCapture
 */
- (instancetype)initLiveStream:(NSString *)liveStreamingURL withLivestreamParaCtxConfiguration:(LSLiveStreamingParaCtxConfiguration *)configuration;

/**
 反初始化：释放资源
 */
-(void)unInitLiveStream;

/**
 *  打开视频预览
 *
 *  @param  preview 预览窗口
 */
- (void)startVideoPreview:(UIView*)preview;

/**
 *  @warning 暂停视频预览，如果正在直播，则同时关闭视频预览以及视频推流
 *
 */
- (void)pauseVideoPreview;

/**
 *  @warning 继续视频预览，如果正在直播，则开始视频推流
 *
 */
- (void)resumeVideoPreview;

/**
 *  开始直播
 *
 *  @param completionBlock 具体错误信息
 */
- (void)startLiveStream:(void(^)(NSError *error))completionBlock;

/**
 *  结束推流
 * @warning 只有直播真正开始后，也就是收到LSLiveStreamingStarted消息后，才可以关闭直播,error为nil的时候，说明直播结束，否则直播过程中发生错误
 */
- (void)stopLiveStream:(void(^)(NSError *error))completionBlock;

/**
 *  重启开始视频推流
 *  @warning 需要先启动推流startLiveStreamWithError，开启音视频推流，才可以中断视频推流，重启视频推流，
 */
- (void)resumeVideoLiveStream;

/**
 *  中断视频推流
 *  @warning 需要先启动推流startLiveStreamWithError，开启音视频推流，才可以中断视频推流，重启视频推流，
 */
- (void)pauseVideoLiveStream;

/**
 *  重启音频推流，
 *  @warning：需要先启动推流startLiveStreamWithError，开启音视频推流，才可以中断音频推流，重启音频推流，
 */
- (BOOL)resumeAudioLiveStream;

/**
 *  中断音频推流，
 *  @warning：需要先启动推流startLiveStreamWithError，开启音视频推流，才可以中断音频推流，重启音频推流，
 */
- (BOOL)pauseAudioLiveStream;

/**
 *  设置trace 的level
 *
 *  @param loglevl trace 信息的级别
 */
- (void)setTraceLevel:(LSMediaLog)logLevel;

/**
 *  获取当前sdk的版本号
 *
 */
- (NSString*) getSDKVersionID;

#pragma mark - 直播基础模块相关部分
//!<  本地录制部分
/**
 开始录制并保存本地文件（mp4录制）
 
 @param recordFileName 本地录制的文件全路径
 @return 开启录制成功
 */
- (BOOL)startRecord:(NSString *)recordFileName;

/**
 *  停止本地录制
 */
- (BOOL)stopRecord;

//!<  混音相关部分
/**
 *  开始播放混音文件
 *
 *  @param musicURL 音频文件地址/文件名
 *  @param enableLoop 当前音频文件是否单曲循环
 */
- (BOOL)startPlayMusic:(NSString*)musicURL withEnableSignleFileLooped:(BOOL)enableLoop;

/**
 *  结束播放混音文件，释放播放文件
 */
- (BOOL)stopPlayMusic;

/**
 *  继续播放混音文件
 */
- (BOOL)resumePlayMusic;

/**
 *  中断播放混音文件
 */
- (BOOL)pausePlayMusic;

/**
 *  设置混音强度
 *  @param value 混音强度范围【1-10】
 */
- (void)setMixIntensity:(int )value;

//!< 音视频回调和外部采集相关部分
/**
 采集模块采集的视频数据回调，交由外部进行自定义前处理，处理完后通过externalInputSampleBuffer发送给sdk
 */
@property(nonatomic, copy) void (^externalCaptureSampleBufferCallback)(CMSampleBufferRef sampleBuffer);

/**
 将外部前处理之后的视频数据发送给sdk
 
 @param sampleBuffer 视频数据
 */
-(void)externalInputSampleBuffer:(CMSampleBufferRef)sampleBuffer;

/**
 *  麦克风采集到的原始裸数据的回调，交由外部进行自定义处理，处理完后通过externalInputAudioRawData发送给sdk
 *  param rawData 麦克风采集得到的裸数据，PCM格式，关于裸数据的格式设置 LSAudioParaCtx
 */
@property (nonatomic,copy) void (^externalCaptureAudioRawData)(unsigned char *rawData,unsigned int rawDataSize);

/**
 *  用户可以通过这个接口，将处理完的音频裸数据送回来，裸数据的格式要保持不变，由视频云sdk推流出去
 *
 *  @param rawData     裸数据的buffer
 *  @param rawDataSize 裸数据buffer的大小
 */
-(void)externalInputAudioRawData:(unsigned char*)rawData
                        dataSize:(unsigned int)rawDataSize;

//!< 测速相关部分
/**
 开始测速
 */
-(void)startSpeedCalc:(NSString *)url success:(void(^)(NSMutableArray *array))success fail:(void(^)())fail;

/**
 结束测速
 */
-(void)stopSpeedCalc;
/**
 测速之前设置测速次数和上传数据大小
 
 @param count 测速次数（默认为1次）,测速之后，取平均值返回结果
 @param capacity 上传数据大小(仅限于文件上传类型,经测试，NTS2不能超过500k（含500k)),单位是字节，500k＝500*1024，默认为499k（控制最大不超过10M）
 */
-(void)setSpeedCacl:(NSInteger)count Capacity:(unsigned long long)capacity;

#pragma mark - 视频摄像头相关功能
/**
 *  flash摄像头
 *
 *  @return 打开或者关闭摄像头flash
 */
@property (nonatomic, assign)BOOL flash;

/**
 *  摄像头变焦功能属性：最大拉伸值，系统最大为：videoMaxZoomFactor
 *  @warning iphone4s以及之前的版本，videoMaxZoomFactor＝1；不支持拉伸
 */
@property (nonatomic, assign, readonly) CGFloat maxZoomScale;

/**
 *  摄像头变焦功能属性：拉伸值，［1，maxZoomScale］
 *  @warning iphone4s以及之前的版本，videoMaxZoomFactor＝1；不支持拉伸
 */
@property (nonatomic, assign) CGFloat zoomScale;

/**
 *  摄像头变焦功能属性：拉伸值变化回调block
 *
 *  摄像头响应uigesture事件，而改变了拉伸系数反馈
 *  @warning iphone4s以及之前的版本，videoMaxZoomFactor＝1；不支持拉伸
 */
@property (nonatomic,copy) void (^onZoomScaleValueChanged)(CGFloat value);

/**
 *  切换前后摄像头
 *
 *  @return 当前摄像头的位置，前或者后
 */
- (LSCameraPosition)switchCamera:(LSSwitchModuleVideoCameraPositionBlock)cameraPostionBlock;

/**
 切换分辨率，支持直播过程中切换分辨率，切换分辨率，水印将自动清除，需要外部根据分辨率，再次设置水印大小
 
 @param videoResolution
 @param videoResolutionBlock
 */
- (void)switchVideoStreamingQuality:(LSVideoStreamingQuality)videoResolution block:(LSVideoStreamingQualityBlock)videoResolutionBlock;

/**
 直播推流之前设置如下参数
 
 @param bitrate 推流码率 default会按照分辨率设置
 @param fps 采集帧率 default ＝ 15
 @param cameraOrientation 摄像头采集方向（一般不变）
 */
- (void)setBitrate:(int)bitrate
               fps:(int)fps
 cameraOrientation:(LSCameraOrientation) cameraOrientation;

/**
 切换本地预览镜像
 
 @return 当前是否镜像
 */
-(BOOL)changeIsFrontPreViewMirrored;

/**
 切换编码镜像（针对拉流端观众）
 
 @return 当前是否镜像
 */
-(BOOL)changeIsFrontCodeMirrored;

/**
 *  获取视频截图，
 *
 *  @param  LSFrameCaptureCompletionBlock 获取最新一幅视频图像的回调
 *
 */
- (void)snapShotWithCompletionBlock:(LSFrameCaptureCompletionBlock)completionBlock;

#pragma mark - 视频前处理模块处理
//!< 滤镜相关部分
/**
 *  设置滤镜类型
 *
 *  @param filterType 滤镜类型，目前支持4种滤镜，参考 LSGpuImageFilterType 描述
 *
 */
- (void)setFilterType:(LSGpuImageFilterType)filterType;

//!< 美颜功能
/**
 设置磨皮强度【0-1】
 
 @param value 值
 */
- (void)setSmoothFilterIntensity:(float)value;

/**
 设置美白强度【0-1】
 
 @param value 值
 */
- (void)setWhiteningFilterIntensity:(float)value;

/**
 调节曝光度（-10.0 - 10.0 ，默认为0.0）
 
 @param exposure 曝光度
 */
-(void)adjustExposure:(CGFloat)exposure;

//!< 水印相关部分
/**
 添加涂鸦
 
 @param image 涂鸦静态图像
 @param rect 具体位置和大小（x，y根据location位置，计算具体的位置信息）
 @param location 位置
 */
- (void)addGraffiti:(UIImage*)image
               rect:(CGRect)rect
           location:(LSWaterMarkLocation)location;

/**
 添加静态视频水印
 
 @param image 静态图像
 @param rect 具体位置和大小（x，y根据location位置，计算具体的位置信息）
 @param location 位置
 */
- (void) addWaterMark: (UIImage*) image
                 rect: (CGRect) rect
             location: (LSWaterMarkLocation) location;

/**
 关闭本地预览静态水印
 */
- (void)closePreviewWaterMark:(BOOL)isClosed;

/**
 添加动态视频水印
 
 @param imageArray 动态图像数组
 @param count 播放速度的快慢:count代表count帧显示同一张图
 @param looped 是否循环，不循环就显示一次
 @param rect 具体位置和大小（x，y根据location位置，计算具体的位置信息）
 @param location 位置
 */
- (void) addDynamicWaterMarks: (NSArray*) imageArray
                     fpsCount: (unsigned int)count
                         loop: (BOOL)looped
                         rect: (CGRect) rect
                     location: (LSWaterMarkLocation) location;

/**
 关闭本地预览动态水印
 */
- (void)closePreviewDynamicWaterMark:(BOOL)isClosed;

/**
 清除水印
 */
- (void)cleanWaterMark;

@end




