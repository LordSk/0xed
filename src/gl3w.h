/*

    This file is a modified version of gl3w_gen.py, part of gl3w
    (hosted at https://github.com/skaslev/gl3w)

    This is free and unencumbered software released into the public domain.

    Anyone is free to copy, modify, publish, use, compile, sell, or
    distribute this software, either in source code form or as a compiled
    binary, for any purpose, commercial or non-commercial, and by any
    means.

    In jurisdictions that recognize copyright laws, the author or authors
    of this software dedicate any and all copyright interest in the
    software to the public domain. We make this dedication for the benefit
    of the public at large and to the detriment of our heirs and
    successors. We intend this dedication to be an overt act of
    relinquishment in perpetuity of all present and future rights to this
    software under copyright law.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

============================================================================
    You MUST

        #define GL3W_IMPLEMENTATION

    in EXACLY _one_ C or C++ file that includes this header, BEFORE the include,
    like this:

        #define GL3W_IMPLEMENTATION
        #include "gl3w.h"

    All other files should just #include "gl3w.h" without the #define.

*/

#ifndef __gl3w_h_
#define __gl3w_h_

#include "glcorearb.h"

#ifndef __gl_h_
#define __gl_h_
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*GL3WglProc)(void);

/* gl3w api */
int gl3w_init(void);
int gl3w_is_supported(int major, int minor);
GL3WglProc gl3w_get_proc_address(char const *proc);

/* OpenGL functions */
extern PFNGLACTIVESHADERPROGRAMPROC                         gl3wActiveShaderProgram;
extern PFNGLACTIVETEXTUREPROC                               gl3wActiveTexture;
extern PFNGLATTACHSHADERPROC                                gl3wAttachShader;
extern PFNGLBEGINCONDITIONALRENDERPROC                      gl3wBeginConditionalRender;
extern PFNGLBEGINQUERYPROC                                  gl3wBeginQuery;
extern PFNGLBEGINQUERYINDEXEDPROC                           gl3wBeginQueryIndexed;
extern PFNGLBEGINTRANSFORMFEEDBACKPROC                      gl3wBeginTransformFeedback;
extern PFNGLBINDATTRIBLOCATIONPROC                          gl3wBindAttribLocation;
extern PFNGLBINDBUFFERPROC                                  gl3wBindBuffer;
extern PFNGLBINDBUFFERBASEPROC                              gl3wBindBufferBase;
extern PFNGLBINDBUFFERRANGEPROC                             gl3wBindBufferRange;
extern PFNGLBINDBUFFERSBASEPROC                             gl3wBindBuffersBase;
extern PFNGLBINDBUFFERSRANGEPROC                            gl3wBindBuffersRange;
extern PFNGLBINDFRAGDATALOCATIONPROC                        gl3wBindFragDataLocation;
extern PFNGLBINDFRAGDATALOCATIONINDEXEDPROC                 gl3wBindFragDataLocationIndexed;
extern PFNGLBINDFRAMEBUFFERPROC                             gl3wBindFramebuffer;
extern PFNGLBINDIMAGETEXTUREPROC                            gl3wBindImageTexture;
extern PFNGLBINDIMAGETEXTURESPROC                           gl3wBindImageTextures;
extern PFNGLBINDPROGRAMPIPELINEPROC                         gl3wBindProgramPipeline;
extern PFNGLBINDRENDERBUFFERPROC                            gl3wBindRenderbuffer;
extern PFNGLBINDSAMPLERPROC                                 gl3wBindSampler;
extern PFNGLBINDSAMPLERSPROC                                gl3wBindSamplers;
extern PFNGLBINDTEXTUREPROC                                 gl3wBindTexture;
extern PFNGLBINDTEXTUREUNITPROC                             gl3wBindTextureUnit;
extern PFNGLBINDTEXTURESPROC                                gl3wBindTextures;
extern PFNGLBINDTRANSFORMFEEDBACKPROC                       gl3wBindTransformFeedback;
extern PFNGLBINDVERTEXARRAYPROC                             gl3wBindVertexArray;
extern PFNGLBINDVERTEXBUFFERPROC                            gl3wBindVertexBuffer;
extern PFNGLBINDVERTEXBUFFERSPROC                           gl3wBindVertexBuffers;
extern PFNGLBLENDCOLORPROC                                  gl3wBlendColor;
extern PFNGLBLENDEQUATIONPROC                               gl3wBlendEquation;
extern PFNGLBLENDEQUATIONSEPARATEPROC                       gl3wBlendEquationSeparate;
extern PFNGLBLENDEQUATIONSEPARATEIPROC                      gl3wBlendEquationSeparatei;
extern PFNGLBLENDEQUATIONSEPARATEIARBPROC                   gl3wBlendEquationSeparateiARB;
extern PFNGLBLENDEQUATIONIPROC                              gl3wBlendEquationi;
extern PFNGLBLENDEQUATIONIARBPROC                           gl3wBlendEquationiARB;
extern PFNGLBLENDFUNCPROC                                   gl3wBlendFunc;
extern PFNGLBLENDFUNCSEPARATEPROC                           gl3wBlendFuncSeparate;
extern PFNGLBLENDFUNCSEPARATEIPROC                          gl3wBlendFuncSeparatei;
extern PFNGLBLENDFUNCSEPARATEIARBPROC                       gl3wBlendFuncSeparateiARB;
extern PFNGLBLENDFUNCIPROC                                  gl3wBlendFunci;
extern PFNGLBLENDFUNCIARBPROC                               gl3wBlendFunciARB;
extern PFNGLBLITFRAMEBUFFERPROC                             gl3wBlitFramebuffer;
extern PFNGLBLITNAMEDFRAMEBUFFERPROC                        gl3wBlitNamedFramebuffer;
extern PFNGLBUFFERDATAPROC                                  gl3wBufferData;
extern PFNGLBUFFERPAGECOMMITMENTARBPROC                     gl3wBufferPageCommitmentARB;
extern PFNGLBUFFERSTORAGEPROC                               gl3wBufferStorage;
extern PFNGLBUFFERSUBDATAPROC                               gl3wBufferSubData;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC                      gl3wCheckFramebufferStatus;
extern PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC                 gl3wCheckNamedFramebufferStatus;
extern PFNGLCLAMPCOLORPROC                                  gl3wClampColor;
extern PFNGLCLEARPROC                                       gl3wClear;
extern PFNGLCLEARBUFFERDATAPROC                             gl3wClearBufferData;
extern PFNGLCLEARBUFFERSUBDATAPROC                          gl3wClearBufferSubData;
extern PFNGLCLEARBUFFERFIPROC                               gl3wClearBufferfi;
extern PFNGLCLEARBUFFERFVPROC                               gl3wClearBufferfv;
extern PFNGLCLEARBUFFERIVPROC                               gl3wClearBufferiv;
extern PFNGLCLEARBUFFERUIVPROC                              gl3wClearBufferuiv;
extern PFNGLCLEARCOLORPROC                                  gl3wClearColor;
extern PFNGLCLEARDEPTHPROC                                  gl3wClearDepth;
extern PFNGLCLEARDEPTHFPROC                                 gl3wClearDepthf;
extern PFNGLCLEARNAMEDBUFFERDATAPROC                        gl3wClearNamedBufferData;
extern PFNGLCLEARNAMEDBUFFERSUBDATAPROC                     gl3wClearNamedBufferSubData;
extern PFNGLCLEARNAMEDFRAMEBUFFERFIPROC                     gl3wClearNamedFramebufferfi;
extern PFNGLCLEARNAMEDFRAMEBUFFERFVPROC                     gl3wClearNamedFramebufferfv;
extern PFNGLCLEARNAMEDFRAMEBUFFERIVPROC                     gl3wClearNamedFramebufferiv;
extern PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC                    gl3wClearNamedFramebufferuiv;
extern PFNGLCLEARSTENCILPROC                                gl3wClearStencil;
extern PFNGLCLEARTEXIMAGEPROC                               gl3wClearTexImage;
extern PFNGLCLEARTEXSUBIMAGEPROC                            gl3wClearTexSubImage;
extern PFNGLCLIENTWAITSYNCPROC                              gl3wClientWaitSync;
extern PFNGLCLIPCONTROLPROC                                 gl3wClipControl;
extern PFNGLCOLORMASKPROC                                   gl3wColorMask;
extern PFNGLCOLORMASKIPROC                                  gl3wColorMaski;
extern PFNGLCOMPILESHADERPROC                               gl3wCompileShader;
extern PFNGLCOMPILESHADERINCLUDEARBPROC                     gl3wCompileShaderIncludeARB;
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC                        gl3wCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC                        gl3wCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC                        gl3wCompressedTexImage3D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC                     gl3wCompressedTexSubImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC                     gl3wCompressedTexSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC                     gl3wCompressedTexSubImage3D;
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC                 gl3wCompressedTextureSubImage1D;
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC                 gl3wCompressedTextureSubImage2D;
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC                 gl3wCompressedTextureSubImage3D;
extern PFNGLCOPYBUFFERSUBDATAPROC                           gl3wCopyBufferSubData;
extern PFNGLCOPYIMAGESUBDATAPROC                            gl3wCopyImageSubData;
extern PFNGLCOPYNAMEDBUFFERSUBDATAPROC                      gl3wCopyNamedBufferSubData;
extern PFNGLCOPYTEXIMAGE1DPROC                              gl3wCopyTexImage1D;
extern PFNGLCOPYTEXIMAGE2DPROC                              gl3wCopyTexImage2D;
extern PFNGLCOPYTEXSUBIMAGE1DPROC                           gl3wCopyTexSubImage1D;
extern PFNGLCOPYTEXSUBIMAGE2DPROC                           gl3wCopyTexSubImage2D;
extern PFNGLCOPYTEXSUBIMAGE3DPROC                           gl3wCopyTexSubImage3D;
extern PFNGLCOPYTEXTURESUBIMAGE1DPROC                       gl3wCopyTextureSubImage1D;
extern PFNGLCOPYTEXTURESUBIMAGE2DPROC                       gl3wCopyTextureSubImage2D;
extern PFNGLCOPYTEXTURESUBIMAGE3DPROC                       gl3wCopyTextureSubImage3D;
extern PFNGLCREATEBUFFERSPROC                               gl3wCreateBuffers;
extern PFNGLCREATEFRAMEBUFFERSPROC                          gl3wCreateFramebuffers;
extern PFNGLCREATEPROGRAMPROC                               gl3wCreateProgram;
extern PFNGLCREATEPROGRAMPIPELINESPROC                      gl3wCreateProgramPipelines;
extern PFNGLCREATEQUERIESPROC                               gl3wCreateQueries;
extern PFNGLCREATERENDERBUFFERSPROC                         gl3wCreateRenderbuffers;
extern PFNGLCREATESAMPLERSPROC                              gl3wCreateSamplers;
extern PFNGLCREATESHADERPROC                                gl3wCreateShader;
extern PFNGLCREATESHADERPROGRAMVPROC                        gl3wCreateShaderProgramv;
extern PFNGLCREATESYNCFROMCLEVENTARBPROC                    gl3wCreateSyncFromCLeventARB;
extern PFNGLCREATETEXTURESPROC                              gl3wCreateTextures;
extern PFNGLCREATETRANSFORMFEEDBACKSPROC                    gl3wCreateTransformFeedbacks;
extern PFNGLCREATEVERTEXARRAYSPROC                          gl3wCreateVertexArrays;
extern PFNGLCULLFACEPROC                                    gl3wCullFace;
extern PFNGLDEBUGMESSAGECALLBACKPROC                        gl3wDebugMessageCallback;
extern PFNGLDEBUGMESSAGECALLBACKARBPROC                     gl3wDebugMessageCallbackARB;
extern PFNGLDEBUGMESSAGECONTROLPROC                         gl3wDebugMessageControl;
extern PFNGLDEBUGMESSAGECONTROLARBPROC                      gl3wDebugMessageControlARB;
extern PFNGLDEBUGMESSAGEINSERTPROC                          gl3wDebugMessageInsert;
extern PFNGLDEBUGMESSAGEINSERTARBPROC                       gl3wDebugMessageInsertARB;
extern PFNGLDELETEBUFFERSPROC                               gl3wDeleteBuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC                          gl3wDeleteFramebuffers;
extern PFNGLDELETENAMEDSTRINGARBPROC                        gl3wDeleteNamedStringARB;
extern PFNGLDELETEPROGRAMPROC                               gl3wDeleteProgram;
extern PFNGLDELETEPROGRAMPIPELINESPROC                      gl3wDeleteProgramPipelines;
extern PFNGLDELETEQUERIESPROC                               gl3wDeleteQueries;
extern PFNGLDELETERENDERBUFFERSPROC                         gl3wDeleteRenderbuffers;
extern PFNGLDELETESAMPLERSPROC                              gl3wDeleteSamplers;
extern PFNGLDELETESHADERPROC                                gl3wDeleteShader;
extern PFNGLDELETESYNCPROC                                  gl3wDeleteSync;
extern PFNGLDELETETEXTURESPROC                              gl3wDeleteTextures;
extern PFNGLDELETETRANSFORMFEEDBACKSPROC                    gl3wDeleteTransformFeedbacks;
extern PFNGLDELETEVERTEXARRAYSPROC                          gl3wDeleteVertexArrays;
extern PFNGLDEPTHFUNCPROC                                   gl3wDepthFunc;
extern PFNGLDEPTHMASKPROC                                   gl3wDepthMask;
extern PFNGLDEPTHRANGEPROC                                  gl3wDepthRange;
extern PFNGLDEPTHRANGEARRAYVPROC                            gl3wDepthRangeArrayv;
extern PFNGLDEPTHRANGEINDEXEDPROC                           gl3wDepthRangeIndexed;
extern PFNGLDEPTHRANGEFPROC                                 gl3wDepthRangef;
extern PFNGLDETACHSHADERPROC                                gl3wDetachShader;
extern PFNGLDISABLEPROC                                     gl3wDisable;
extern PFNGLDISABLEVERTEXARRAYATTRIBPROC                    gl3wDisableVertexArrayAttrib;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC                    gl3wDisableVertexAttribArray;
extern PFNGLDISABLEIPROC                                    gl3wDisablei;
extern PFNGLDISPATCHCOMPUTEPROC                             gl3wDispatchCompute;
extern PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC                 gl3wDispatchComputeGroupSizeARB;
extern PFNGLDISPATCHCOMPUTEINDIRECTPROC                     gl3wDispatchComputeIndirect;
extern PFNGLDRAWARRAYSPROC                                  gl3wDrawArrays;
extern PFNGLDRAWARRAYSINDIRECTPROC                          gl3wDrawArraysIndirect;
extern PFNGLDRAWARRAYSINSTANCEDPROC                         gl3wDrawArraysInstanced;
extern PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC             gl3wDrawArraysInstancedBaseInstance;
extern PFNGLDRAWBUFFERPROC                                  gl3wDrawBuffer;
extern PFNGLDRAWBUFFERSPROC                                 gl3wDrawBuffers;
extern PFNGLDRAWELEMENTSPROC                                gl3wDrawElements;
extern PFNGLDRAWELEMENTSBASEVERTEXPROC                      gl3wDrawElementsBaseVertex;
extern PFNGLDRAWELEMENTSINDIRECTPROC                        gl3wDrawElementsIndirect;
extern PFNGLDRAWELEMENTSINSTANCEDPROC                       gl3wDrawElementsInstanced;
extern PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC           gl3wDrawElementsInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC             gl3wDrawElementsInstancedBaseVertex;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC gl3wDrawElementsInstancedBaseVertexBaseInstance;
extern PFNGLDRAWRANGEELEMENTSPROC                           gl3wDrawRangeElements;
extern PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC                 gl3wDrawRangeElementsBaseVertex;
extern PFNGLDRAWTRANSFORMFEEDBACKPROC                       gl3wDrawTransformFeedback;
extern PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC              gl3wDrawTransformFeedbackInstanced;
extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC                 gl3wDrawTransformFeedbackStream;
extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC        gl3wDrawTransformFeedbackStreamInstanced;
extern PFNGLENABLEPROC                                      gl3wEnable;
extern PFNGLENABLEVERTEXARRAYATTRIBPROC                     gl3wEnableVertexArrayAttrib;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC                     gl3wEnableVertexAttribArray;
extern PFNGLENABLEIPROC                                     gl3wEnablei;
extern PFNGLENDCONDITIONALRENDERPROC                        gl3wEndConditionalRender;
extern PFNGLENDQUERYPROC                                    gl3wEndQuery;
extern PFNGLENDQUERYINDEXEDPROC                             gl3wEndQueryIndexed;
extern PFNGLENDTRANSFORMFEEDBACKPROC                        gl3wEndTransformFeedback;
extern PFNGLFENCESYNCPROC                                   gl3wFenceSync;
extern PFNGLFINISHPROC                                      gl3wFinish;
extern PFNGLFLUSHPROC                                       gl3wFlush;
extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC                      gl3wFlushMappedBufferRange;
extern PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC                 gl3wFlushMappedNamedBufferRange;
extern PFNGLFRAMEBUFFERPARAMETERIPROC                       gl3wFramebufferParameteri;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC                     gl3wFramebufferRenderbuffer;
extern PFNGLFRAMEBUFFERTEXTUREPROC                          gl3wFramebufferTexture;
extern PFNGLFRAMEBUFFERTEXTURE1DPROC                        gl3wFramebufferTexture1D;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC                        gl3wFramebufferTexture2D;
extern PFNGLFRAMEBUFFERTEXTURE3DPROC                        gl3wFramebufferTexture3D;
extern PFNGLFRAMEBUFFERTEXTURELAYERPROC                     gl3wFramebufferTextureLayer;
extern PFNGLFRONTFACEPROC                                   gl3wFrontFace;
extern PFNGLGENBUFFERSPROC                                  gl3wGenBuffers;
extern PFNGLGENFRAMEBUFFERSPROC                             gl3wGenFramebuffers;
extern PFNGLGENPROGRAMPIPELINESPROC                         gl3wGenProgramPipelines;
extern PFNGLGENQUERIESPROC                                  gl3wGenQueries;
extern PFNGLGENRENDERBUFFERSPROC                            gl3wGenRenderbuffers;
extern PFNGLGENSAMPLERSPROC                                 gl3wGenSamplers;
extern PFNGLGENTEXTURESPROC                                 gl3wGenTextures;
extern PFNGLGENTRANSFORMFEEDBACKSPROC                       gl3wGenTransformFeedbacks;
extern PFNGLGENVERTEXARRAYSPROC                             gl3wGenVertexArrays;
extern PFNGLGENERATEMIPMAPPROC                              gl3wGenerateMipmap;
extern PFNGLGENERATETEXTUREMIPMAPPROC                       gl3wGenerateTextureMipmap;
extern PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC              gl3wGetActiveAtomicCounterBufferiv;
extern PFNGLGETACTIVEATTRIBPROC                             gl3wGetActiveAttrib;
extern PFNGLGETACTIVESUBROUTINENAMEPROC                     gl3wGetActiveSubroutineName;
extern PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC              gl3wGetActiveSubroutineUniformName;
extern PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC                gl3wGetActiveSubroutineUniformiv;
extern PFNGLGETACTIVEUNIFORMPROC                            gl3wGetActiveUniform;
extern PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC                   gl3wGetActiveUniformBlockName;
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC                     gl3wGetActiveUniformBlockiv;
extern PFNGLGETACTIVEUNIFORMNAMEPROC                        gl3wGetActiveUniformName;
extern PFNGLGETACTIVEUNIFORMSIVPROC                         gl3wGetActiveUniformsiv;
extern PFNGLGETATTACHEDSHADERSPROC                          gl3wGetAttachedShaders;
extern PFNGLGETATTRIBLOCATIONPROC                           gl3wGetAttribLocation;
extern PFNGLGETBOOLEANI_VPROC                               gl3wGetBooleani_v;
extern PFNGLGETBOOLEANVPROC                                 gl3wGetBooleanv;
extern PFNGLGETBUFFERPARAMETERI64VPROC                      gl3wGetBufferParameteri64v;
extern PFNGLGETBUFFERPARAMETERIVPROC                        gl3wGetBufferParameteriv;
extern PFNGLGETBUFFERPOINTERVPROC                           gl3wGetBufferPointerv;
extern PFNGLGETBUFFERSUBDATAPROC                            gl3wGetBufferSubData;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC                       gl3wGetCompressedTexImage;
extern PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC                   gl3wGetCompressedTextureImage;
extern PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC                gl3wGetCompressedTextureSubImage;
extern PFNGLGETDEBUGMESSAGELOGPROC                          gl3wGetDebugMessageLog;
extern PFNGLGETDEBUGMESSAGELOGARBPROC                       gl3wGetDebugMessageLogARB;
extern PFNGLGETDOUBLEI_VPROC                                gl3wGetDoublei_v;
extern PFNGLGETDOUBLEVPROC                                  gl3wGetDoublev;
extern PFNGLGETERRORPROC                                    gl3wGetError;
extern PFNGLGETFLOATI_VPROC                                 gl3wGetFloati_v;
extern PFNGLGETFLOATVPROC                                   gl3wGetFloatv;
extern PFNGLGETFRAGDATAINDEXPROC                            gl3wGetFragDataIndex;
extern PFNGLGETFRAGDATALOCATIONPROC                         gl3wGetFragDataLocation;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC         gl3wGetFramebufferAttachmentParameteriv;
extern PFNGLGETFRAMEBUFFERPARAMETERIVPROC                   gl3wGetFramebufferParameteriv;
extern PFNGLGETGRAPHICSRESETSTATUSPROC                      gl3wGetGraphicsResetStatus;
extern PFNGLGETGRAPHICSRESETSTATUSARBPROC                   gl3wGetGraphicsResetStatusARB;
extern PFNGLGETIMAGEHANDLEARBPROC                           gl3wGetImageHandleARB;
extern PFNGLGETINTEGER64I_VPROC                             gl3wGetInteger64i_v;
extern PFNGLGETINTEGER64VPROC                               gl3wGetInteger64v;
extern PFNGLGETINTEGERI_VPROC                               gl3wGetIntegeri_v;
extern PFNGLGETINTEGERVPROC                                 gl3wGetIntegerv;
extern PFNGLGETINTERNALFORMATI64VPROC                       gl3wGetInternalformati64v;
extern PFNGLGETINTERNALFORMATIVPROC                         gl3wGetInternalformativ;
extern PFNGLGETMULTISAMPLEFVPROC                            gl3wGetMultisamplefv;
extern PFNGLGETNAMEDBUFFERPARAMETERI64VPROC                 gl3wGetNamedBufferParameteri64v;
extern PFNGLGETNAMEDBUFFERPARAMETERIVPROC                   gl3wGetNamedBufferParameteriv;
extern PFNGLGETNAMEDBUFFERPOINTERVPROC                      gl3wGetNamedBufferPointerv;
extern PFNGLGETNAMEDBUFFERSUBDATAPROC                       gl3wGetNamedBufferSubData;
extern PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC    gl3wGetNamedFramebufferAttachmentParameteriv;
extern PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC              gl3wGetNamedFramebufferParameteriv;
extern PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC             gl3wGetNamedRenderbufferParameteriv;
extern PFNGLGETNAMEDSTRINGARBPROC                           gl3wGetNamedStringARB;
extern PFNGLGETNAMEDSTRINGIVARBPROC                         gl3wGetNamedStringivARB;
extern PFNGLGETOBJECTLABELPROC                              gl3wGetObjectLabel;
extern PFNGLGETOBJECTPTRLABELPROC                           gl3wGetObjectPtrLabel;
extern PFNGLGETPOINTERVPROC                                 gl3wGetPointerv;
extern PFNGLGETPROGRAMBINARYPROC                            gl3wGetProgramBinary;
extern PFNGLGETPROGRAMINFOLOGPROC                           gl3wGetProgramInfoLog;
extern PFNGLGETPROGRAMINTERFACEIVPROC                       gl3wGetProgramInterfaceiv;
extern PFNGLGETPROGRAMPIPELINEINFOLOGPROC                   gl3wGetProgramPipelineInfoLog;
extern PFNGLGETPROGRAMPIPELINEIVPROC                        gl3wGetProgramPipelineiv;
extern PFNGLGETPROGRAMRESOURCEINDEXPROC                     gl3wGetProgramResourceIndex;
extern PFNGLGETPROGRAMRESOURCELOCATIONPROC                  gl3wGetProgramResourceLocation;
extern PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC             gl3wGetProgramResourceLocationIndex;
extern PFNGLGETPROGRAMRESOURCENAMEPROC                      gl3wGetProgramResourceName;
extern PFNGLGETPROGRAMRESOURCEIVPROC                        gl3wGetProgramResourceiv;
extern PFNGLGETPROGRAMSTAGEIVPROC                           gl3wGetProgramStageiv;
extern PFNGLGETPROGRAMIVPROC                                gl3wGetProgramiv;
extern PFNGLGETQUERYBUFFEROBJECTI64VPROC                    gl3wGetQueryBufferObjecti64v;
extern PFNGLGETQUERYBUFFEROBJECTIVPROC                      gl3wGetQueryBufferObjectiv;
extern PFNGLGETQUERYBUFFEROBJECTUI64VPROC                   gl3wGetQueryBufferObjectui64v;
extern PFNGLGETQUERYBUFFEROBJECTUIVPROC                     gl3wGetQueryBufferObjectuiv;
extern PFNGLGETQUERYINDEXEDIVPROC                           gl3wGetQueryIndexediv;
extern PFNGLGETQUERYOBJECTI64VPROC                          gl3wGetQueryObjecti64v;
extern PFNGLGETQUERYOBJECTIVPROC                            gl3wGetQueryObjectiv;
extern PFNGLGETQUERYOBJECTUI64VPROC                         gl3wGetQueryObjectui64v;
extern PFNGLGETQUERYOBJECTUIVPROC                           gl3wGetQueryObjectuiv;
extern PFNGLGETQUERYIVPROC                                  gl3wGetQueryiv;
extern PFNGLGETRENDERBUFFERPARAMETERIVPROC                  gl3wGetRenderbufferParameteriv;
extern PFNGLGETSAMPLERPARAMETERIIVPROC                      gl3wGetSamplerParameterIiv;
extern PFNGLGETSAMPLERPARAMETERIUIVPROC                     gl3wGetSamplerParameterIuiv;
extern PFNGLGETSAMPLERPARAMETERFVPROC                       gl3wGetSamplerParameterfv;
extern PFNGLGETSAMPLERPARAMETERIVPROC                       gl3wGetSamplerParameteriv;
extern PFNGLGETSHADERINFOLOGPROC                            gl3wGetShaderInfoLog;
extern PFNGLGETSHADERPRECISIONFORMATPROC                    gl3wGetShaderPrecisionFormat;
extern PFNGLGETSHADERSOURCEPROC                             gl3wGetShaderSource;
extern PFNGLGETSHADERIVPROC                                 gl3wGetShaderiv;
extern PFNGLGETSTRINGPROC                                   gl3wGetString;
extern PFNGLGETSTRINGIPROC                                  gl3wGetStringi;
extern PFNGLGETSUBROUTINEINDEXPROC                          gl3wGetSubroutineIndex;
extern PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC                gl3wGetSubroutineUniformLocation;
extern PFNGLGETSYNCIVPROC                                   gl3wGetSynciv;
extern PFNGLGETTEXIMAGEPROC                                 gl3wGetTexImage;
extern PFNGLGETTEXLEVELPARAMETERFVPROC                      gl3wGetTexLevelParameterfv;
extern PFNGLGETTEXLEVELPARAMETERIVPROC                      gl3wGetTexLevelParameteriv;
extern PFNGLGETTEXPARAMETERIIVPROC                          gl3wGetTexParameterIiv;
extern PFNGLGETTEXPARAMETERIUIVPROC                         gl3wGetTexParameterIuiv;
extern PFNGLGETTEXPARAMETERFVPROC                           gl3wGetTexParameterfv;
extern PFNGLGETTEXPARAMETERIVPROC                           gl3wGetTexParameteriv;
extern PFNGLGETTEXTUREHANDLEARBPROC                         gl3wGetTextureHandleARB;
extern PFNGLGETTEXTUREIMAGEPROC                             gl3wGetTextureImage;
extern PFNGLGETTEXTURELEVELPARAMETERFVPROC                  gl3wGetTextureLevelParameterfv;
extern PFNGLGETTEXTURELEVELPARAMETERIVPROC                  gl3wGetTextureLevelParameteriv;
extern PFNGLGETTEXTUREPARAMETERIIVPROC                      gl3wGetTextureParameterIiv;
extern PFNGLGETTEXTUREPARAMETERIUIVPROC                     gl3wGetTextureParameterIuiv;
extern PFNGLGETTEXTUREPARAMETERFVPROC                       gl3wGetTextureParameterfv;
extern PFNGLGETTEXTUREPARAMETERIVPROC                       gl3wGetTextureParameteriv;
extern PFNGLGETTEXTURESAMPLERHANDLEARBPROC                  gl3wGetTextureSamplerHandleARB;
extern PFNGLGETTEXTURESUBIMAGEPROC                          gl3wGetTextureSubImage;
extern PFNGLGETTRANSFORMFEEDBACKVARYINGPROC                 gl3wGetTransformFeedbackVarying;
extern PFNGLGETTRANSFORMFEEDBACKI64_VPROC                   gl3wGetTransformFeedbacki64_v;
extern PFNGLGETTRANSFORMFEEDBACKI_VPROC                     gl3wGetTransformFeedbacki_v;
extern PFNGLGETTRANSFORMFEEDBACKIVPROC                      gl3wGetTransformFeedbackiv;
extern PFNGLGETUNIFORMBLOCKINDEXPROC                        gl3wGetUniformBlockIndex;
extern PFNGLGETUNIFORMINDICESPROC                           gl3wGetUniformIndices;
extern PFNGLGETUNIFORMLOCATIONPROC                          gl3wGetUniformLocation;
extern PFNGLGETUNIFORMSUBROUTINEUIVPROC                     gl3wGetUniformSubroutineuiv;
extern PFNGLGETUNIFORMDVPROC                                gl3wGetUniformdv;
extern PFNGLGETUNIFORMFVPROC                                gl3wGetUniformfv;
extern PFNGLGETUNIFORMIVPROC                                gl3wGetUniformiv;
extern PFNGLGETUNIFORMUIVPROC                               gl3wGetUniformuiv;
extern PFNGLGETVERTEXARRAYINDEXED64IVPROC                   gl3wGetVertexArrayIndexed64iv;
extern PFNGLGETVERTEXARRAYINDEXEDIVPROC                     gl3wGetVertexArrayIndexediv;
extern PFNGLGETVERTEXARRAYIVPROC                            gl3wGetVertexArrayiv;
extern PFNGLGETVERTEXATTRIBIIVPROC                          gl3wGetVertexAttribIiv;
extern PFNGLGETVERTEXATTRIBIUIVPROC                         gl3wGetVertexAttribIuiv;
extern PFNGLGETVERTEXATTRIBLDVPROC                          gl3wGetVertexAttribLdv;
extern PFNGLGETVERTEXATTRIBLUI64VARBPROC                    gl3wGetVertexAttribLui64vARB;
extern PFNGLGETVERTEXATTRIBPOINTERVPROC                     gl3wGetVertexAttribPointerv;
extern PFNGLGETVERTEXATTRIBDVPROC                           gl3wGetVertexAttribdv;
extern PFNGLGETVERTEXATTRIBFVPROC                           gl3wGetVertexAttribfv;
extern PFNGLGETVERTEXATTRIBIVPROC                           gl3wGetVertexAttribiv;
extern PFNGLGETNCOMPRESSEDTEXIMAGEPROC                      gl3wGetnCompressedTexImage;
extern PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC                   gl3wGetnCompressedTexImageARB;
extern PFNGLGETNTEXIMAGEPROC                                gl3wGetnTexImage;
extern PFNGLGETNTEXIMAGEARBPROC                             gl3wGetnTexImageARB;
extern PFNGLGETNUNIFORMDVPROC                               gl3wGetnUniformdv;
extern PFNGLGETNUNIFORMDVARBPROC                            gl3wGetnUniformdvARB;
extern PFNGLGETNUNIFORMFVPROC                               gl3wGetnUniformfv;
extern PFNGLGETNUNIFORMFVARBPROC                            gl3wGetnUniformfvARB;
extern PFNGLGETNUNIFORMIVPROC                               gl3wGetnUniformiv;
extern PFNGLGETNUNIFORMIVARBPROC                            gl3wGetnUniformivARB;
extern PFNGLGETNUNIFORMUIVPROC                              gl3wGetnUniformuiv;
extern PFNGLGETNUNIFORMUIVARBPROC                           gl3wGetnUniformuivARB;
extern PFNGLHINTPROC                                        gl3wHint;
extern PFNGLINVALIDATEBUFFERDATAPROC                        gl3wInvalidateBufferData;
extern PFNGLINVALIDATEBUFFERSUBDATAPROC                     gl3wInvalidateBufferSubData;
extern PFNGLINVALIDATEFRAMEBUFFERPROC                       gl3wInvalidateFramebuffer;
extern PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC              gl3wInvalidateNamedFramebufferData;
extern PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC           gl3wInvalidateNamedFramebufferSubData;
extern PFNGLINVALIDATESUBFRAMEBUFFERPROC                    gl3wInvalidateSubFramebuffer;
extern PFNGLINVALIDATETEXIMAGEPROC                          gl3wInvalidateTexImage;
extern PFNGLINVALIDATETEXSUBIMAGEPROC                       gl3wInvalidateTexSubImage;
extern PFNGLISBUFFERPROC                                    gl3wIsBuffer;
extern PFNGLISENABLEDPROC                                   gl3wIsEnabled;
extern PFNGLISENABLEDIPROC                                  gl3wIsEnabledi;
extern PFNGLISFRAMEBUFFERPROC                               gl3wIsFramebuffer;
extern PFNGLISIMAGEHANDLERESIDENTARBPROC                    gl3wIsImageHandleResidentARB;
extern PFNGLISNAMEDSTRINGARBPROC                            gl3wIsNamedStringARB;
extern PFNGLISPROGRAMPROC                                   gl3wIsProgram;
extern PFNGLISPROGRAMPIPELINEPROC                           gl3wIsProgramPipeline;
extern PFNGLISQUERYPROC                                     gl3wIsQuery;
extern PFNGLISRENDERBUFFERPROC                              gl3wIsRenderbuffer;
extern PFNGLISSAMPLERPROC                                   gl3wIsSampler;
extern PFNGLISSHADERPROC                                    gl3wIsShader;
extern PFNGLISSYNCPROC                                      gl3wIsSync;
extern PFNGLISTEXTUREPROC                                   gl3wIsTexture;
extern PFNGLISTEXTUREHANDLERESIDENTARBPROC                  gl3wIsTextureHandleResidentARB;
extern PFNGLISTRANSFORMFEEDBACKPROC                         gl3wIsTransformFeedback;
extern PFNGLISVERTEXARRAYPROC                               gl3wIsVertexArray;
extern PFNGLLINEWIDTHPROC                                   gl3wLineWidth;
extern PFNGLLINKPROGRAMPROC                                 gl3wLinkProgram;
extern PFNGLLOGICOPPROC                                     gl3wLogicOp;
extern PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC               gl3wMakeImageHandleNonResidentARB;
extern PFNGLMAKEIMAGEHANDLERESIDENTARBPROC                  gl3wMakeImageHandleResidentARB;
extern PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC             gl3wMakeTextureHandleNonResidentARB;
extern PFNGLMAKETEXTUREHANDLERESIDENTARBPROC                gl3wMakeTextureHandleResidentARB;
extern PFNGLMAPBUFFERPROC                                   gl3wMapBuffer;
extern PFNGLMAPBUFFERRANGEPROC                              gl3wMapBufferRange;
extern PFNGLMAPNAMEDBUFFERPROC                              gl3wMapNamedBuffer;
extern PFNGLMAPNAMEDBUFFERRANGEPROC                         gl3wMapNamedBufferRange;
extern PFNGLMEMORYBARRIERPROC                               gl3wMemoryBarrier;
extern PFNGLMEMORYBARRIERBYREGIONPROC                       gl3wMemoryBarrierByRegion;
extern PFNGLMINSAMPLESHADINGPROC                            gl3wMinSampleShading;
extern PFNGLMINSAMPLESHADINGARBPROC                         gl3wMinSampleShadingARB;
extern PFNGLMULTIDRAWARRAYSPROC                             gl3wMultiDrawArrays;
extern PFNGLMULTIDRAWARRAYSINDIRECTPROC                     gl3wMultiDrawArraysIndirect;
extern PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC             gl3wMultiDrawArraysIndirectCountARB;
extern PFNGLMULTIDRAWELEMENTSPROC                           gl3wMultiDrawElements;
extern PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC                 gl3wMultiDrawElementsBaseVertex;
extern PFNGLMULTIDRAWELEMENTSINDIRECTPROC                   gl3wMultiDrawElementsIndirect;
extern PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC           gl3wMultiDrawElementsIndirectCountARB;
extern PFNGLNAMEDBUFFERDATAPROC                             gl3wNamedBufferData;
extern PFNGLNAMEDBUFFERPAGECOMMITMENTARBPROC                gl3wNamedBufferPageCommitmentARB;
extern PFNGLNAMEDBUFFERPAGECOMMITMENTEXTPROC                gl3wNamedBufferPageCommitmentEXT;
extern PFNGLNAMEDBUFFERSTORAGEPROC                          gl3wNamedBufferStorage;
extern PFNGLNAMEDBUFFERSUBDATAPROC                          gl3wNamedBufferSubData;
extern PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC                  gl3wNamedFramebufferDrawBuffer;
extern PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC                 gl3wNamedFramebufferDrawBuffers;
extern PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC                  gl3wNamedFramebufferParameteri;
extern PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC                  gl3wNamedFramebufferReadBuffer;
extern PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC                gl3wNamedFramebufferRenderbuffer;
extern PFNGLNAMEDFRAMEBUFFERTEXTUREPROC                     gl3wNamedFramebufferTexture;
extern PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC                gl3wNamedFramebufferTextureLayer;
extern PFNGLNAMEDRENDERBUFFERSTORAGEPROC                    gl3wNamedRenderbufferStorage;
extern PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC         gl3wNamedRenderbufferStorageMultisample;
extern PFNGLNAMEDSTRINGARBPROC                              gl3wNamedStringARB;
extern PFNGLOBJECTLABELPROC                                 gl3wObjectLabel;
extern PFNGLOBJECTPTRLABELPROC                              gl3wObjectPtrLabel;
extern PFNGLPATCHPARAMETERFVPROC                            gl3wPatchParameterfv;
extern PFNGLPATCHPARAMETERIPROC                             gl3wPatchParameteri;
extern PFNGLPAUSETRANSFORMFEEDBACKPROC                      gl3wPauseTransformFeedback;
extern PFNGLPIXELSTOREFPROC                                 gl3wPixelStoref;
extern PFNGLPIXELSTOREIPROC                                 gl3wPixelStorei;
extern PFNGLPOINTPARAMETERFPROC                             gl3wPointParameterf;
extern PFNGLPOINTPARAMETERFVPROC                            gl3wPointParameterfv;
extern PFNGLPOINTPARAMETERIPROC                             gl3wPointParameteri;
extern PFNGLPOINTPARAMETERIVPROC                            gl3wPointParameteriv;
extern PFNGLPOINTSIZEPROC                                   gl3wPointSize;
extern PFNGLPOLYGONMODEPROC                                 gl3wPolygonMode;
extern PFNGLPOLYGONOFFSETPROC                               gl3wPolygonOffset;
extern PFNGLPOPDEBUGGROUPPROC                               gl3wPopDebugGroup;
extern PFNGLPRIMITIVERESTARTINDEXPROC                       gl3wPrimitiveRestartIndex;
extern PFNGLPROGRAMBINARYPROC                               gl3wProgramBinary;
extern PFNGLPROGRAMPARAMETERIPROC                           gl3wProgramParameteri;
extern PFNGLPROGRAMUNIFORM1DPROC                            gl3wProgramUniform1d;
extern PFNGLPROGRAMUNIFORM1DVPROC                           gl3wProgramUniform1dv;
extern PFNGLPROGRAMUNIFORM1FPROC                            gl3wProgramUniform1f;
extern PFNGLPROGRAMUNIFORM1FVPROC                           gl3wProgramUniform1fv;
extern PFNGLPROGRAMUNIFORM1IPROC                            gl3wProgramUniform1i;
extern PFNGLPROGRAMUNIFORM1IVPROC                           gl3wProgramUniform1iv;
extern PFNGLPROGRAMUNIFORM1UIPROC                           gl3wProgramUniform1ui;
extern PFNGLPROGRAMUNIFORM1UIVPROC                          gl3wProgramUniform1uiv;
extern PFNGLPROGRAMUNIFORM2DPROC                            gl3wProgramUniform2d;
extern PFNGLPROGRAMUNIFORM2DVPROC                           gl3wProgramUniform2dv;
extern PFNGLPROGRAMUNIFORM2FPROC                            gl3wProgramUniform2f;
extern PFNGLPROGRAMUNIFORM2FVPROC                           gl3wProgramUniform2fv;
extern PFNGLPROGRAMUNIFORM2IPROC                            gl3wProgramUniform2i;
extern PFNGLPROGRAMUNIFORM2IVPROC                           gl3wProgramUniform2iv;
extern PFNGLPROGRAMUNIFORM2UIPROC                           gl3wProgramUniform2ui;
extern PFNGLPROGRAMUNIFORM2UIVPROC                          gl3wProgramUniform2uiv;
extern PFNGLPROGRAMUNIFORM3DPROC                            gl3wProgramUniform3d;
extern PFNGLPROGRAMUNIFORM3DVPROC                           gl3wProgramUniform3dv;
extern PFNGLPROGRAMUNIFORM3FPROC                            gl3wProgramUniform3f;
extern PFNGLPROGRAMUNIFORM3FVPROC                           gl3wProgramUniform3fv;
extern PFNGLPROGRAMUNIFORM3IPROC                            gl3wProgramUniform3i;
extern PFNGLPROGRAMUNIFORM3IVPROC                           gl3wProgramUniform3iv;
extern PFNGLPROGRAMUNIFORM3UIPROC                           gl3wProgramUniform3ui;
extern PFNGLPROGRAMUNIFORM3UIVPROC                          gl3wProgramUniform3uiv;
extern PFNGLPROGRAMUNIFORM4DPROC                            gl3wProgramUniform4d;
extern PFNGLPROGRAMUNIFORM4DVPROC                           gl3wProgramUniform4dv;
extern PFNGLPROGRAMUNIFORM4FPROC                            gl3wProgramUniform4f;
extern PFNGLPROGRAMUNIFORM4FVPROC                           gl3wProgramUniform4fv;
extern PFNGLPROGRAMUNIFORM4IPROC                            gl3wProgramUniform4i;
extern PFNGLPROGRAMUNIFORM4IVPROC                           gl3wProgramUniform4iv;
extern PFNGLPROGRAMUNIFORM4UIPROC                           gl3wProgramUniform4ui;
extern PFNGLPROGRAMUNIFORM4UIVPROC                          gl3wProgramUniform4uiv;
extern PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC                 gl3wProgramUniformHandleui64ARB;
extern PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC                gl3wProgramUniformHandleui64vARB;
extern PFNGLPROGRAMUNIFORMMATRIX2DVPROC                     gl3wProgramUniformMatrix2dv;
extern PFNGLPROGRAMUNIFORMMATRIX2FVPROC                     gl3wProgramUniformMatrix2fv;
extern PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC                   gl3wProgramUniformMatrix2x3dv;
extern PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC                   gl3wProgramUniformMatrix2x3fv;
extern PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC                   gl3wProgramUniformMatrix2x4dv;
extern PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC                   gl3wProgramUniformMatrix2x4fv;
extern PFNGLPROGRAMUNIFORMMATRIX3DVPROC                     gl3wProgramUniformMatrix3dv;
extern PFNGLPROGRAMUNIFORMMATRIX3FVPROC                     gl3wProgramUniformMatrix3fv;
extern PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC                   gl3wProgramUniformMatrix3x2dv;
extern PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC                   gl3wProgramUniformMatrix3x2fv;
extern PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC                   gl3wProgramUniformMatrix3x4dv;
extern PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC                   gl3wProgramUniformMatrix3x4fv;
extern PFNGLPROGRAMUNIFORMMATRIX4DVPROC                     gl3wProgramUniformMatrix4dv;
extern PFNGLPROGRAMUNIFORMMATRIX4FVPROC                     gl3wProgramUniformMatrix4fv;
extern PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC                   gl3wProgramUniformMatrix4x2dv;
extern PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC                   gl3wProgramUniformMatrix4x2fv;
extern PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC                   gl3wProgramUniformMatrix4x3dv;
extern PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC                   gl3wProgramUniformMatrix4x3fv;
extern PFNGLPROVOKINGVERTEXPROC                             gl3wProvokingVertex;
extern PFNGLPUSHDEBUGGROUPPROC                              gl3wPushDebugGroup;
extern PFNGLQUERYCOUNTERPROC                                gl3wQueryCounter;
extern PFNGLREADBUFFERPROC                                  gl3wReadBuffer;
extern PFNGLREADPIXELSPROC                                  gl3wReadPixels;
extern PFNGLREADNPIXELSPROC                                 gl3wReadnPixels;
extern PFNGLREADNPIXELSARBPROC                              gl3wReadnPixelsARB;
extern PFNGLRELEASESHADERCOMPILERPROC                       gl3wReleaseShaderCompiler;
extern PFNGLRENDERBUFFERSTORAGEPROC                         gl3wRenderbufferStorage;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC              gl3wRenderbufferStorageMultisample;
extern PFNGLRESUMETRANSFORMFEEDBACKPROC                     gl3wResumeTransformFeedback;
extern PFNGLSAMPLECOVERAGEPROC                              gl3wSampleCoverage;
extern PFNGLSAMPLEMASKIPROC                                 gl3wSampleMaski;
extern PFNGLSAMPLERPARAMETERIIVPROC                         gl3wSamplerParameterIiv;
extern PFNGLSAMPLERPARAMETERIUIVPROC                        gl3wSamplerParameterIuiv;
extern PFNGLSAMPLERPARAMETERFPROC                           gl3wSamplerParameterf;
extern PFNGLSAMPLERPARAMETERFVPROC                          gl3wSamplerParameterfv;
extern PFNGLSAMPLERPARAMETERIPROC                           gl3wSamplerParameteri;
extern PFNGLSAMPLERPARAMETERIVPROC                          gl3wSamplerParameteriv;
extern PFNGLSCISSORPROC                                     gl3wScissor;
extern PFNGLSCISSORARRAYVPROC                               gl3wScissorArrayv;
extern PFNGLSCISSORINDEXEDPROC                              gl3wScissorIndexed;
extern PFNGLSCISSORINDEXEDVPROC                             gl3wScissorIndexedv;
extern PFNGLSHADERBINARYPROC                                gl3wShaderBinary;
extern PFNGLSHADERSOURCEPROC                                gl3wShaderSource;
extern PFNGLSHADERSTORAGEBLOCKBINDINGPROC                   gl3wShaderStorageBlockBinding;
extern PFNGLSTENCILFUNCPROC                                 gl3wStencilFunc;
extern PFNGLSTENCILFUNCSEPARATEPROC                         gl3wStencilFuncSeparate;
extern PFNGLSTENCILMASKPROC                                 gl3wStencilMask;
extern PFNGLSTENCILMASKSEPARATEPROC                         gl3wStencilMaskSeparate;
extern PFNGLSTENCILOPPROC                                   gl3wStencilOp;
extern PFNGLSTENCILOPSEPARATEPROC                           gl3wStencilOpSeparate;
extern PFNGLTEXBUFFERPROC                                   gl3wTexBuffer;
extern PFNGLTEXBUFFERRANGEPROC                              gl3wTexBufferRange;
extern PFNGLTEXIMAGE1DPROC                                  gl3wTexImage1D;
extern PFNGLTEXIMAGE2DPROC                                  gl3wTexImage2D;
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC                       gl3wTexImage2DMultisample;
extern PFNGLTEXIMAGE3DPROC                                  gl3wTexImage3D;
extern PFNGLTEXIMAGE3DMULTISAMPLEPROC                       gl3wTexImage3DMultisample;
extern PFNGLTEXPAGECOMMITMENTARBPROC                        gl3wTexPageCommitmentARB;
extern PFNGLTEXPARAMETERIIVPROC                             gl3wTexParameterIiv;
extern PFNGLTEXPARAMETERIUIVPROC                            gl3wTexParameterIuiv;
extern PFNGLTEXPARAMETERFPROC                               gl3wTexParameterf;
extern PFNGLTEXPARAMETERFVPROC                              gl3wTexParameterfv;
extern PFNGLTEXPARAMETERIPROC                               gl3wTexParameteri;
extern PFNGLTEXPARAMETERIVPROC                              gl3wTexParameteriv;
extern PFNGLTEXSTORAGE1DPROC                                gl3wTexStorage1D;
extern PFNGLTEXSTORAGE2DPROC                                gl3wTexStorage2D;
extern PFNGLTEXSTORAGE2DMULTISAMPLEPROC                     gl3wTexStorage2DMultisample;
extern PFNGLTEXSTORAGE3DPROC                                gl3wTexStorage3D;
extern PFNGLTEXSTORAGE3DMULTISAMPLEPROC                     gl3wTexStorage3DMultisample;
extern PFNGLTEXSUBIMAGE1DPROC                               gl3wTexSubImage1D;
extern PFNGLTEXSUBIMAGE2DPROC                               gl3wTexSubImage2D;
extern PFNGLTEXSUBIMAGE3DPROC                               gl3wTexSubImage3D;
extern PFNGLTEXTUREBARRIERPROC                              gl3wTextureBarrier;
extern PFNGLTEXTUREBUFFERPROC                               gl3wTextureBuffer;
extern PFNGLTEXTUREBUFFERRANGEPROC                          gl3wTextureBufferRange;
extern PFNGLTEXTUREPARAMETERIIVPROC                         gl3wTextureParameterIiv;
extern PFNGLTEXTUREPARAMETERIUIVPROC                        gl3wTextureParameterIuiv;
extern PFNGLTEXTUREPARAMETERFPROC                           gl3wTextureParameterf;
extern PFNGLTEXTUREPARAMETERFVPROC                          gl3wTextureParameterfv;
extern PFNGLTEXTUREPARAMETERIPROC                           gl3wTextureParameteri;
extern PFNGLTEXTUREPARAMETERIVPROC                          gl3wTextureParameteriv;
extern PFNGLTEXTURESTORAGE1DPROC                            gl3wTextureStorage1D;
extern PFNGLTEXTURESTORAGE2DPROC                            gl3wTextureStorage2D;
extern PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC                 gl3wTextureStorage2DMultisample;
extern PFNGLTEXTURESTORAGE3DPROC                            gl3wTextureStorage3D;
extern PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC                 gl3wTextureStorage3DMultisample;
extern PFNGLTEXTURESUBIMAGE1DPROC                           gl3wTextureSubImage1D;
extern PFNGLTEXTURESUBIMAGE2DPROC                           gl3wTextureSubImage2D;
extern PFNGLTEXTURESUBIMAGE3DPROC                           gl3wTextureSubImage3D;
extern PFNGLTEXTUREVIEWPROC                                 gl3wTextureView;
extern PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC                 gl3wTransformFeedbackBufferBase;
extern PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC                gl3wTransformFeedbackBufferRange;
extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC                   gl3wTransformFeedbackVaryings;
extern PFNGLUNIFORM1DPROC                                   gl3wUniform1d;
extern PFNGLUNIFORM1DVPROC                                  gl3wUniform1dv;
extern PFNGLUNIFORM1FPROC                                   gl3wUniform1f;
extern PFNGLUNIFORM1FVPROC                                  gl3wUniform1fv;
extern PFNGLUNIFORM1IPROC                                   gl3wUniform1i;
extern PFNGLUNIFORM1IVPROC                                  gl3wUniform1iv;
extern PFNGLUNIFORM1UIPROC                                  gl3wUniform1ui;
extern PFNGLUNIFORM1UIVPROC                                 gl3wUniform1uiv;
extern PFNGLUNIFORM2DPROC                                   gl3wUniform2d;
extern PFNGLUNIFORM2DVPROC                                  gl3wUniform2dv;
extern PFNGLUNIFORM2FPROC                                   gl3wUniform2f;
extern PFNGLUNIFORM2FVPROC                                  gl3wUniform2fv;
extern PFNGLUNIFORM2IPROC                                   gl3wUniform2i;
extern PFNGLUNIFORM2IVPROC                                  gl3wUniform2iv;
extern PFNGLUNIFORM2UIPROC                                  gl3wUniform2ui;
extern PFNGLUNIFORM2UIVPROC                                 gl3wUniform2uiv;
extern PFNGLUNIFORM3DPROC                                   gl3wUniform3d;
extern PFNGLUNIFORM3DVPROC                                  gl3wUniform3dv;
extern PFNGLUNIFORM3FPROC                                   gl3wUniform3f;
extern PFNGLUNIFORM3FVPROC                                  gl3wUniform3fv;
extern PFNGLUNIFORM3IPROC                                   gl3wUniform3i;
extern PFNGLUNIFORM3IVPROC                                  gl3wUniform3iv;
extern PFNGLUNIFORM3UIPROC                                  gl3wUniform3ui;
extern PFNGLUNIFORM3UIVPROC                                 gl3wUniform3uiv;
extern PFNGLUNIFORM4DPROC                                   gl3wUniform4d;
extern PFNGLUNIFORM4DVPROC                                  gl3wUniform4dv;
extern PFNGLUNIFORM4FPROC                                   gl3wUniform4f;
extern PFNGLUNIFORM4FVPROC                                  gl3wUniform4fv;
extern PFNGLUNIFORM4IPROC                                   gl3wUniform4i;
extern PFNGLUNIFORM4IVPROC                                  gl3wUniform4iv;
extern PFNGLUNIFORM4UIPROC                                  gl3wUniform4ui;
extern PFNGLUNIFORM4UIVPROC                                 gl3wUniform4uiv;
extern PFNGLUNIFORMBLOCKBINDINGPROC                         gl3wUniformBlockBinding;
extern PFNGLUNIFORMHANDLEUI64ARBPROC                        gl3wUniformHandleui64ARB;
extern PFNGLUNIFORMHANDLEUI64VARBPROC                       gl3wUniformHandleui64vARB;
extern PFNGLUNIFORMMATRIX2DVPROC                            gl3wUniformMatrix2dv;
extern PFNGLUNIFORMMATRIX2FVPROC                            gl3wUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX2X3DVPROC                          gl3wUniformMatrix2x3dv;
extern PFNGLUNIFORMMATRIX2X3FVPROC                          gl3wUniformMatrix2x3fv;
extern PFNGLUNIFORMMATRIX2X4DVPROC                          gl3wUniformMatrix2x4dv;
extern PFNGLUNIFORMMATRIX2X4FVPROC                          gl3wUniformMatrix2x4fv;
extern PFNGLUNIFORMMATRIX3DVPROC                            gl3wUniformMatrix3dv;
extern PFNGLUNIFORMMATRIX3FVPROC                            gl3wUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX3X2DVPROC                          gl3wUniformMatrix3x2dv;
extern PFNGLUNIFORMMATRIX3X2FVPROC                          gl3wUniformMatrix3x2fv;
extern PFNGLUNIFORMMATRIX3X4DVPROC                          gl3wUniformMatrix3x4dv;
extern PFNGLUNIFORMMATRIX3X4FVPROC                          gl3wUniformMatrix3x4fv;
extern PFNGLUNIFORMMATRIX4DVPROC                            gl3wUniformMatrix4dv;
extern PFNGLUNIFORMMATRIX4FVPROC                            gl3wUniformMatrix4fv;
extern PFNGLUNIFORMMATRIX4X2DVPROC                          gl3wUniformMatrix4x2dv;
extern PFNGLUNIFORMMATRIX4X2FVPROC                          gl3wUniformMatrix4x2fv;
extern PFNGLUNIFORMMATRIX4X3DVPROC                          gl3wUniformMatrix4x3dv;
extern PFNGLUNIFORMMATRIX4X3FVPROC                          gl3wUniformMatrix4x3fv;
extern PFNGLUNIFORMSUBROUTINESUIVPROC                       gl3wUniformSubroutinesuiv;
extern PFNGLUNMAPBUFFERPROC                                 gl3wUnmapBuffer;
extern PFNGLUNMAPNAMEDBUFFERPROC                            gl3wUnmapNamedBuffer;
extern PFNGLUSEPROGRAMPROC                                  gl3wUseProgram;
extern PFNGLUSEPROGRAMSTAGESPROC                            gl3wUseProgramStages;
extern PFNGLVALIDATEPROGRAMPROC                             gl3wValidateProgram;
extern PFNGLVALIDATEPROGRAMPIPELINEPROC                     gl3wValidateProgramPipeline;
extern PFNGLVERTEXARRAYATTRIBBINDINGPROC                    gl3wVertexArrayAttribBinding;
extern PFNGLVERTEXARRAYATTRIBFORMATPROC                     gl3wVertexArrayAttribFormat;
extern PFNGLVERTEXARRAYATTRIBIFORMATPROC                    gl3wVertexArrayAttribIFormat;
extern PFNGLVERTEXARRAYATTRIBLFORMATPROC                    gl3wVertexArrayAttribLFormat;
extern PFNGLVERTEXARRAYBINDINGDIVISORPROC                   gl3wVertexArrayBindingDivisor;
extern PFNGLVERTEXARRAYELEMENTBUFFERPROC                    gl3wVertexArrayElementBuffer;
extern PFNGLVERTEXARRAYVERTEXBUFFERPROC                     gl3wVertexArrayVertexBuffer;
extern PFNGLVERTEXARRAYVERTEXBUFFERSPROC                    gl3wVertexArrayVertexBuffers;
extern PFNGLVERTEXATTRIB1DPROC                              gl3wVertexAttrib1d;
extern PFNGLVERTEXATTRIB1DVPROC                             gl3wVertexAttrib1dv;
extern PFNGLVERTEXATTRIB1FPROC                              gl3wVertexAttrib1f;
extern PFNGLVERTEXATTRIB1FVPROC                             gl3wVertexAttrib1fv;
extern PFNGLVERTEXATTRIB1SPROC                              gl3wVertexAttrib1s;
extern PFNGLVERTEXATTRIB1SVPROC                             gl3wVertexAttrib1sv;
extern PFNGLVERTEXATTRIB2DPROC                              gl3wVertexAttrib2d;
extern PFNGLVERTEXATTRIB2DVPROC                             gl3wVertexAttrib2dv;
extern PFNGLVERTEXATTRIB2FPROC                              gl3wVertexAttrib2f;
extern PFNGLVERTEXATTRIB2FVPROC                             gl3wVertexAttrib2fv;
extern PFNGLVERTEXATTRIB2SPROC                              gl3wVertexAttrib2s;
extern PFNGLVERTEXATTRIB2SVPROC                             gl3wVertexAttrib2sv;
extern PFNGLVERTEXATTRIB3DPROC                              gl3wVertexAttrib3d;
extern PFNGLVERTEXATTRIB3DVPROC                             gl3wVertexAttrib3dv;
extern PFNGLVERTEXATTRIB3FPROC                              gl3wVertexAttrib3f;
extern PFNGLVERTEXATTRIB3FVPROC                             gl3wVertexAttrib3fv;
extern PFNGLVERTEXATTRIB3SPROC                              gl3wVertexAttrib3s;
extern PFNGLVERTEXATTRIB3SVPROC                             gl3wVertexAttrib3sv;
extern PFNGLVERTEXATTRIB4NBVPROC                            gl3wVertexAttrib4Nbv;
extern PFNGLVERTEXATTRIB4NIVPROC                            gl3wVertexAttrib4Niv;
extern PFNGLVERTEXATTRIB4NSVPROC                            gl3wVertexAttrib4Nsv;
extern PFNGLVERTEXATTRIB4NUBPROC                            gl3wVertexAttrib4Nub;
extern PFNGLVERTEXATTRIB4NUBVPROC                           gl3wVertexAttrib4Nubv;
extern PFNGLVERTEXATTRIB4NUIVPROC                           gl3wVertexAttrib4Nuiv;
extern PFNGLVERTEXATTRIB4NUSVPROC                           gl3wVertexAttrib4Nusv;
extern PFNGLVERTEXATTRIB4BVPROC                             gl3wVertexAttrib4bv;
extern PFNGLVERTEXATTRIB4DPROC                              gl3wVertexAttrib4d;
extern PFNGLVERTEXATTRIB4DVPROC                             gl3wVertexAttrib4dv;
extern PFNGLVERTEXATTRIB4FPROC                              gl3wVertexAttrib4f;
extern PFNGLVERTEXATTRIB4FVPROC                             gl3wVertexAttrib4fv;
extern PFNGLVERTEXATTRIB4IVPROC                             gl3wVertexAttrib4iv;
extern PFNGLVERTEXATTRIB4SPROC                              gl3wVertexAttrib4s;
extern PFNGLVERTEXATTRIB4SVPROC                             gl3wVertexAttrib4sv;
extern PFNGLVERTEXATTRIB4UBVPROC                            gl3wVertexAttrib4ubv;
extern PFNGLVERTEXATTRIB4UIVPROC                            gl3wVertexAttrib4uiv;
extern PFNGLVERTEXATTRIB4USVPROC                            gl3wVertexAttrib4usv;
extern PFNGLVERTEXATTRIBBINDINGPROC                         gl3wVertexAttribBinding;
extern PFNGLVERTEXATTRIBDIVISORPROC                         gl3wVertexAttribDivisor;
extern PFNGLVERTEXATTRIBFORMATPROC                          gl3wVertexAttribFormat;
extern PFNGLVERTEXATTRIBI1IPROC                             gl3wVertexAttribI1i;
extern PFNGLVERTEXATTRIBI1IVPROC                            gl3wVertexAttribI1iv;
extern PFNGLVERTEXATTRIBI1UIPROC                            gl3wVertexAttribI1ui;
extern PFNGLVERTEXATTRIBI1UIVPROC                           gl3wVertexAttribI1uiv;
extern PFNGLVERTEXATTRIBI2IPROC                             gl3wVertexAttribI2i;
extern PFNGLVERTEXATTRIBI2IVPROC                            gl3wVertexAttribI2iv;
extern PFNGLVERTEXATTRIBI2UIPROC                            gl3wVertexAttribI2ui;
extern PFNGLVERTEXATTRIBI2UIVPROC                           gl3wVertexAttribI2uiv;
extern PFNGLVERTEXATTRIBI3IPROC                             gl3wVertexAttribI3i;
extern PFNGLVERTEXATTRIBI3IVPROC                            gl3wVertexAttribI3iv;
extern PFNGLVERTEXATTRIBI3UIPROC                            gl3wVertexAttribI3ui;
extern PFNGLVERTEXATTRIBI3UIVPROC                           gl3wVertexAttribI3uiv;
extern PFNGLVERTEXATTRIBI4BVPROC                            gl3wVertexAttribI4bv;
extern PFNGLVERTEXATTRIBI4IPROC                             gl3wVertexAttribI4i;
extern PFNGLVERTEXATTRIBI4IVPROC                            gl3wVertexAttribI4iv;
extern PFNGLVERTEXATTRIBI4SVPROC                            gl3wVertexAttribI4sv;
extern PFNGLVERTEXATTRIBI4UBVPROC                           gl3wVertexAttribI4ubv;
extern PFNGLVERTEXATTRIBI4UIPROC                            gl3wVertexAttribI4ui;
extern PFNGLVERTEXATTRIBI4UIVPROC                           gl3wVertexAttribI4uiv;
extern PFNGLVERTEXATTRIBI4USVPROC                           gl3wVertexAttribI4usv;
extern PFNGLVERTEXATTRIBIFORMATPROC                         gl3wVertexAttribIFormat;
extern PFNGLVERTEXATTRIBIPOINTERPROC                        gl3wVertexAttribIPointer;
extern PFNGLVERTEXATTRIBL1DPROC                             gl3wVertexAttribL1d;
extern PFNGLVERTEXATTRIBL1DVPROC                            gl3wVertexAttribL1dv;
extern PFNGLVERTEXATTRIBL1UI64ARBPROC                       gl3wVertexAttribL1ui64ARB;
extern PFNGLVERTEXATTRIBL1UI64VARBPROC                      gl3wVertexAttribL1ui64vARB;
extern PFNGLVERTEXATTRIBL2DPROC                             gl3wVertexAttribL2d;
extern PFNGLVERTEXATTRIBL2DVPROC                            gl3wVertexAttribL2dv;
extern PFNGLVERTEXATTRIBL3DPROC                             gl3wVertexAttribL3d;
extern PFNGLVERTEXATTRIBL3DVPROC                            gl3wVertexAttribL3dv;
extern PFNGLVERTEXATTRIBL4DPROC                             gl3wVertexAttribL4d;
extern PFNGLVERTEXATTRIBL4DVPROC                            gl3wVertexAttribL4dv;
extern PFNGLVERTEXATTRIBLFORMATPROC                         gl3wVertexAttribLFormat;
extern PFNGLVERTEXATTRIBLPOINTERPROC                        gl3wVertexAttribLPointer;
extern PFNGLVERTEXATTRIBP1UIPROC                            gl3wVertexAttribP1ui;
extern PFNGLVERTEXATTRIBP1UIVPROC                           gl3wVertexAttribP1uiv;
extern PFNGLVERTEXATTRIBP2UIPROC                            gl3wVertexAttribP2ui;
extern PFNGLVERTEXATTRIBP2UIVPROC                           gl3wVertexAttribP2uiv;
extern PFNGLVERTEXATTRIBP3UIPROC                            gl3wVertexAttribP3ui;
extern PFNGLVERTEXATTRIBP3UIVPROC                           gl3wVertexAttribP3uiv;
extern PFNGLVERTEXATTRIBP4UIPROC                            gl3wVertexAttribP4ui;
extern PFNGLVERTEXATTRIBP4UIVPROC                           gl3wVertexAttribP4uiv;
extern PFNGLVERTEXATTRIBPOINTERPROC                         gl3wVertexAttribPointer;
extern PFNGLVERTEXBINDINGDIVISORPROC                        gl3wVertexBindingDivisor;
extern PFNGLVIEWPORTPROC                                    gl3wViewport;
extern PFNGLVIEWPORTARRAYVPROC                              gl3wViewportArrayv;
extern PFNGLVIEWPORTINDEXEDFPROC                            gl3wViewportIndexedf;
extern PFNGLVIEWPORTINDEXEDFVPROC                           gl3wViewportIndexedfv;
extern PFNGLWAITSYNCPROC                                    gl3wWaitSync;

#define glActiveShaderProgram                         gl3wActiveShaderProgram
#define glActiveTexture                               gl3wActiveTexture
#define glAttachShader                                gl3wAttachShader
#define glBeginConditionalRender                      gl3wBeginConditionalRender
#define glBeginQuery                                  gl3wBeginQuery
#define glBeginQueryIndexed                           gl3wBeginQueryIndexed
#define glBeginTransformFeedback                      gl3wBeginTransformFeedback
#define glBindAttribLocation                          gl3wBindAttribLocation
#define glBindBuffer                                  gl3wBindBuffer
#define glBindBufferBase                              gl3wBindBufferBase
#define glBindBufferRange                             gl3wBindBufferRange
#define glBindBuffersBase                             gl3wBindBuffersBase
#define glBindBuffersRange                            gl3wBindBuffersRange
#define glBindFragDataLocation                        gl3wBindFragDataLocation
#define glBindFragDataLocationIndexed                 gl3wBindFragDataLocationIndexed
#define glBindFramebuffer                             gl3wBindFramebuffer
#define glBindImageTexture                            gl3wBindImageTexture
#define glBindImageTextures                           gl3wBindImageTextures
#define glBindProgramPipeline                         gl3wBindProgramPipeline
#define glBindRenderbuffer                            gl3wBindRenderbuffer
#define glBindSampler                                 gl3wBindSampler
#define glBindSamplers                                gl3wBindSamplers
#define glBindTexture                                 gl3wBindTexture
#define glBindTextureUnit                             gl3wBindTextureUnit
#define glBindTextures                                gl3wBindTextures
#define glBindTransformFeedback                       gl3wBindTransformFeedback
#define glBindVertexArray                             gl3wBindVertexArray
#define glBindVertexBuffer                            gl3wBindVertexBuffer
#define glBindVertexBuffers                           gl3wBindVertexBuffers
#define glBlendColor                                  gl3wBlendColor
#define glBlendEquation                               gl3wBlendEquation
#define glBlendEquationSeparate                       gl3wBlendEquationSeparate
#define glBlendEquationSeparatei                      gl3wBlendEquationSeparatei
#define glBlendEquationSeparateiARB                   gl3wBlendEquationSeparateiARB
#define glBlendEquationi                              gl3wBlendEquationi
#define glBlendEquationiARB                           gl3wBlendEquationiARB
#define glBlendFunc                                   gl3wBlendFunc
#define glBlendFuncSeparate                           gl3wBlendFuncSeparate
#define glBlendFuncSeparatei                          gl3wBlendFuncSeparatei
#define glBlendFuncSeparateiARB                       gl3wBlendFuncSeparateiARB
#define glBlendFunci                                  gl3wBlendFunci
#define glBlendFunciARB                               gl3wBlendFunciARB
#define glBlitFramebuffer                             gl3wBlitFramebuffer
#define glBlitNamedFramebuffer                        gl3wBlitNamedFramebuffer
#define glBufferData                                  gl3wBufferData
#define glBufferPageCommitmentARB                     gl3wBufferPageCommitmentARB
#define glBufferStorage                               gl3wBufferStorage
#define glBufferSubData                               gl3wBufferSubData
#define glCheckFramebufferStatus                      gl3wCheckFramebufferStatus
#define glCheckNamedFramebufferStatus                 gl3wCheckNamedFramebufferStatus
#define glClampColor                                  gl3wClampColor
#define glClear                                       gl3wClear
#define glClearBufferData                             gl3wClearBufferData
#define glClearBufferSubData                          gl3wClearBufferSubData
#define glClearBufferfi                               gl3wClearBufferfi
#define glClearBufferfv                               gl3wClearBufferfv
#define glClearBufferiv                               gl3wClearBufferiv
#define glClearBufferuiv                              gl3wClearBufferuiv
#define glClearColor                                  gl3wClearColor
#define glClearDepth                                  gl3wClearDepth
#define glClearDepthf                                 gl3wClearDepthf
#define glClearNamedBufferData                        gl3wClearNamedBufferData
#define glClearNamedBufferSubData                     gl3wClearNamedBufferSubData
#define glClearNamedFramebufferfi                     gl3wClearNamedFramebufferfi
#define glClearNamedFramebufferfv                     gl3wClearNamedFramebufferfv
#define glClearNamedFramebufferiv                     gl3wClearNamedFramebufferiv
#define glClearNamedFramebufferuiv                    gl3wClearNamedFramebufferuiv
#define glClearStencil                                gl3wClearStencil
#define glClearTexImage                               gl3wClearTexImage
#define glClearTexSubImage                            gl3wClearTexSubImage
#define glClientWaitSync                              gl3wClientWaitSync
#define glClipControl                                 gl3wClipControl
#define glColorMask                                   gl3wColorMask
#define glColorMaski                                  gl3wColorMaski
#define glCompileShader                               gl3wCompileShader
#define glCompileShaderIncludeARB                     gl3wCompileShaderIncludeARB
#define glCompressedTexImage1D                        gl3wCompressedTexImage1D
#define glCompressedTexImage2D                        gl3wCompressedTexImage2D
#define glCompressedTexImage3D                        gl3wCompressedTexImage3D
#define glCompressedTexSubImage1D                     gl3wCompressedTexSubImage1D
#define glCompressedTexSubImage2D                     gl3wCompressedTexSubImage2D
#define glCompressedTexSubImage3D                     gl3wCompressedTexSubImage3D
#define glCompressedTextureSubImage1D                 gl3wCompressedTextureSubImage1D
#define glCompressedTextureSubImage2D                 gl3wCompressedTextureSubImage2D
#define glCompressedTextureSubImage3D                 gl3wCompressedTextureSubImage3D
#define glCopyBufferSubData                           gl3wCopyBufferSubData
#define glCopyImageSubData                            gl3wCopyImageSubData
#define glCopyNamedBufferSubData                      gl3wCopyNamedBufferSubData
#define glCopyTexImage1D                              gl3wCopyTexImage1D
#define glCopyTexImage2D                              gl3wCopyTexImage2D
#define glCopyTexSubImage1D                           gl3wCopyTexSubImage1D
#define glCopyTexSubImage2D                           gl3wCopyTexSubImage2D
#define glCopyTexSubImage3D                           gl3wCopyTexSubImage3D
#define glCopyTextureSubImage1D                       gl3wCopyTextureSubImage1D
#define glCopyTextureSubImage2D                       gl3wCopyTextureSubImage2D
#define glCopyTextureSubImage3D                       gl3wCopyTextureSubImage3D
#define glCreateBuffers                               gl3wCreateBuffers
#define glCreateFramebuffers                          gl3wCreateFramebuffers
#define glCreateProgram                               gl3wCreateProgram
#define glCreateProgramPipelines                      gl3wCreateProgramPipelines
#define glCreateQueries                               gl3wCreateQueries
#define glCreateRenderbuffers                         gl3wCreateRenderbuffers
#define glCreateSamplers                              gl3wCreateSamplers
#define glCreateShader                                gl3wCreateShader
#define glCreateShaderProgramv                        gl3wCreateShaderProgramv
#define glCreateSyncFromCLeventARB                    gl3wCreateSyncFromCLeventARB
#define glCreateTextures                              gl3wCreateTextures
#define glCreateTransformFeedbacks                    gl3wCreateTransformFeedbacks
#define glCreateVertexArrays                          gl3wCreateVertexArrays
#define glCullFace                                    gl3wCullFace
#define glDebugMessageCallback                        gl3wDebugMessageCallback
#define glDebugMessageCallbackARB                     gl3wDebugMessageCallbackARB
#define glDebugMessageControl                         gl3wDebugMessageControl
#define glDebugMessageControlARB                      gl3wDebugMessageControlARB
#define glDebugMessageInsert                          gl3wDebugMessageInsert
#define glDebugMessageInsertARB                       gl3wDebugMessageInsertARB
#define glDeleteBuffers                               gl3wDeleteBuffers
#define glDeleteFramebuffers                          gl3wDeleteFramebuffers
#define glDeleteNamedStringARB                        gl3wDeleteNamedStringARB
#define glDeleteProgram                               gl3wDeleteProgram
#define glDeleteProgramPipelines                      gl3wDeleteProgramPipelines
#define glDeleteQueries                               gl3wDeleteQueries
#define glDeleteRenderbuffers                         gl3wDeleteRenderbuffers
#define glDeleteSamplers                              gl3wDeleteSamplers
#define glDeleteShader                                gl3wDeleteShader
#define glDeleteSync                                  gl3wDeleteSync
#define glDeleteTextures                              gl3wDeleteTextures
#define glDeleteTransformFeedbacks                    gl3wDeleteTransformFeedbacks
#define glDeleteVertexArrays                          gl3wDeleteVertexArrays
#define glDepthFunc                                   gl3wDepthFunc
#define glDepthMask                                   gl3wDepthMask
#define glDepthRange                                  gl3wDepthRange
#define glDepthRangeArrayv                            gl3wDepthRangeArrayv
#define glDepthRangeIndexed                           gl3wDepthRangeIndexed
#define glDepthRangef                                 gl3wDepthRangef
#define glDetachShader                                gl3wDetachShader
#define glDisable                                     gl3wDisable
#define glDisableVertexArrayAttrib                    gl3wDisableVertexArrayAttrib
#define glDisableVertexAttribArray                    gl3wDisableVertexAttribArray
#define glDisablei                                    gl3wDisablei
#define glDispatchCompute                             gl3wDispatchCompute
#define glDispatchComputeGroupSizeARB                 gl3wDispatchComputeGroupSizeARB
#define glDispatchComputeIndirect                     gl3wDispatchComputeIndirect
#define glDrawArrays                                  gl3wDrawArrays
#define glDrawArraysIndirect                          gl3wDrawArraysIndirect
#define glDrawArraysInstanced                         gl3wDrawArraysInstanced
#define glDrawArraysInstancedBaseInstance             gl3wDrawArraysInstancedBaseInstance
#define glDrawBuffer                                  gl3wDrawBuffer
#define glDrawBuffers                                 gl3wDrawBuffers
#define glDrawElements                                gl3wDrawElements
#define glDrawElementsBaseVertex                      gl3wDrawElementsBaseVertex
#define glDrawElementsIndirect                        gl3wDrawElementsIndirect
#define glDrawElementsInstanced                       gl3wDrawElementsInstanced
#define glDrawElementsInstancedBaseInstance           gl3wDrawElementsInstancedBaseInstance
#define glDrawElementsInstancedBaseVertex             gl3wDrawElementsInstancedBaseVertex
#define glDrawElementsInstancedBaseVertexBaseInstance gl3wDrawElementsInstancedBaseVertexBaseInstance
#define glDrawRangeElements                           gl3wDrawRangeElements
#define glDrawRangeElementsBaseVertex                 gl3wDrawRangeElementsBaseVertex
#define glDrawTransformFeedback                       gl3wDrawTransformFeedback
#define glDrawTransformFeedbackInstanced              gl3wDrawTransformFeedbackInstanced
#define glDrawTransformFeedbackStream                 gl3wDrawTransformFeedbackStream
#define glDrawTransformFeedbackStreamInstanced        gl3wDrawTransformFeedbackStreamInstanced
#define glEnable                                      gl3wEnable
#define glEnableVertexArrayAttrib                     gl3wEnableVertexArrayAttrib
#define glEnableVertexAttribArray                     gl3wEnableVertexAttribArray
#define glEnablei                                     gl3wEnablei
#define glEndConditionalRender                        gl3wEndConditionalRender
#define glEndQuery                                    gl3wEndQuery
#define glEndQueryIndexed                             gl3wEndQueryIndexed
#define glEndTransformFeedback                        gl3wEndTransformFeedback
#define glFenceSync                                   gl3wFenceSync
#define glFinish                                      gl3wFinish
#define glFlush                                       gl3wFlush
#define glFlushMappedBufferRange                      gl3wFlushMappedBufferRange
#define glFlushMappedNamedBufferRange                 gl3wFlushMappedNamedBufferRange
#define glFramebufferParameteri                       gl3wFramebufferParameteri
#define glFramebufferRenderbuffer                     gl3wFramebufferRenderbuffer
#define glFramebufferTexture                          gl3wFramebufferTexture
#define glFramebufferTexture1D                        gl3wFramebufferTexture1D
#define glFramebufferTexture2D                        gl3wFramebufferTexture2D
#define glFramebufferTexture3D                        gl3wFramebufferTexture3D
#define glFramebufferTextureLayer                     gl3wFramebufferTextureLayer
#define glFrontFace                                   gl3wFrontFace
#define glGenBuffers                                  gl3wGenBuffers
#define glGenFramebuffers                             gl3wGenFramebuffers
#define glGenProgramPipelines                         gl3wGenProgramPipelines
#define glGenQueries                                  gl3wGenQueries
#define glGenRenderbuffers                            gl3wGenRenderbuffers
#define glGenSamplers                                 gl3wGenSamplers
#define glGenTextures                                 gl3wGenTextures
#define glGenTransformFeedbacks                       gl3wGenTransformFeedbacks
#define glGenVertexArrays                             gl3wGenVertexArrays
#define glGenerateMipmap                              gl3wGenerateMipmap
#define glGenerateTextureMipmap                       gl3wGenerateTextureMipmap
#define glGetActiveAtomicCounterBufferiv              gl3wGetActiveAtomicCounterBufferiv
#define glGetActiveAttrib                             gl3wGetActiveAttrib
#define glGetActiveSubroutineName                     gl3wGetActiveSubroutineName
#define glGetActiveSubroutineUniformName              gl3wGetActiveSubroutineUniformName
#define glGetActiveSubroutineUniformiv                gl3wGetActiveSubroutineUniformiv
#define glGetActiveUniform                            gl3wGetActiveUniform
#define glGetActiveUniformBlockName                   gl3wGetActiveUniformBlockName
#define glGetActiveUniformBlockiv                     gl3wGetActiveUniformBlockiv
#define glGetActiveUniformName                        gl3wGetActiveUniformName
#define glGetActiveUniformsiv                         gl3wGetActiveUniformsiv
#define glGetAttachedShaders                          gl3wGetAttachedShaders
#define glGetAttribLocation                           gl3wGetAttribLocation
#define glGetBooleani_v                               gl3wGetBooleani_v
#define glGetBooleanv                                 gl3wGetBooleanv
#define glGetBufferParameteri64v                      gl3wGetBufferParameteri64v
#define glGetBufferParameteriv                        gl3wGetBufferParameteriv
#define glGetBufferPointerv                           gl3wGetBufferPointerv
#define glGetBufferSubData                            gl3wGetBufferSubData
#define glGetCompressedTexImage                       gl3wGetCompressedTexImage
#define glGetCompressedTextureImage                   gl3wGetCompressedTextureImage
#define glGetCompressedTextureSubImage                gl3wGetCompressedTextureSubImage
#define glGetDebugMessageLog                          gl3wGetDebugMessageLog
#define glGetDebugMessageLogARB                       gl3wGetDebugMessageLogARB
#define glGetDoublei_v                                gl3wGetDoublei_v
#define glGetDoublev                                  gl3wGetDoublev
#define glGetError                                    gl3wGetError
#define glGetFloati_v                                 gl3wGetFloati_v
#define glGetFloatv                                   gl3wGetFloatv
#define glGetFragDataIndex                            gl3wGetFragDataIndex
#define glGetFragDataLocation                         gl3wGetFragDataLocation
#define glGetFramebufferAttachmentParameteriv         gl3wGetFramebufferAttachmentParameteriv
#define glGetFramebufferParameteriv                   gl3wGetFramebufferParameteriv
#define glGetGraphicsResetStatus                      gl3wGetGraphicsResetStatus
#define glGetGraphicsResetStatusARB                   gl3wGetGraphicsResetStatusARB
#define glGetImageHandleARB                           gl3wGetImageHandleARB
#define glGetInteger64i_v                             gl3wGetInteger64i_v
#define glGetInteger64v                               gl3wGetInteger64v
#define glGetIntegeri_v                               gl3wGetIntegeri_v
#define glGetIntegerv                                 gl3wGetIntegerv
#define glGetInternalformati64v                       gl3wGetInternalformati64v
#define glGetInternalformativ                         gl3wGetInternalformativ
#define glGetMultisamplefv                            gl3wGetMultisamplefv
#define glGetNamedBufferParameteri64v                 gl3wGetNamedBufferParameteri64v
#define glGetNamedBufferParameteriv                   gl3wGetNamedBufferParameteriv
#define glGetNamedBufferPointerv                      gl3wGetNamedBufferPointerv
#define glGetNamedBufferSubData                       gl3wGetNamedBufferSubData
#define glGetNamedFramebufferAttachmentParameteriv    gl3wGetNamedFramebufferAttachmentParameteriv
#define glGetNamedFramebufferParameteriv              gl3wGetNamedFramebufferParameteriv
#define glGetNamedRenderbufferParameteriv             gl3wGetNamedRenderbufferParameteriv
#define glGetNamedStringARB                           gl3wGetNamedStringARB
#define glGetNamedStringivARB                         gl3wGetNamedStringivARB
#define glGetObjectLabel                              gl3wGetObjectLabel
#define glGetObjectPtrLabel                           gl3wGetObjectPtrLabel
#define glGetPointerv                                 gl3wGetPointerv
#define glGetProgramBinary                            gl3wGetProgramBinary
#define glGetProgramInfoLog                           gl3wGetProgramInfoLog
#define glGetProgramInterfaceiv                       gl3wGetProgramInterfaceiv
#define glGetProgramPipelineInfoLog                   gl3wGetProgramPipelineInfoLog
#define glGetProgramPipelineiv                        gl3wGetProgramPipelineiv
#define glGetProgramResourceIndex                     gl3wGetProgramResourceIndex
#define glGetProgramResourceLocation                  gl3wGetProgramResourceLocation
#define glGetProgramResourceLocationIndex             gl3wGetProgramResourceLocationIndex
#define glGetProgramResourceName                      gl3wGetProgramResourceName
#define glGetProgramResourceiv                        gl3wGetProgramResourceiv
#define glGetProgramStageiv                           gl3wGetProgramStageiv
#define glGetProgramiv                                gl3wGetProgramiv
#define glGetQueryBufferObjecti64v                    gl3wGetQueryBufferObjecti64v
#define glGetQueryBufferObjectiv                      gl3wGetQueryBufferObjectiv
#define glGetQueryBufferObjectui64v                   gl3wGetQueryBufferObjectui64v
#define glGetQueryBufferObjectuiv                     gl3wGetQueryBufferObjectuiv
#define glGetQueryIndexediv                           gl3wGetQueryIndexediv
#define glGetQueryObjecti64v                          gl3wGetQueryObjecti64v
#define glGetQueryObjectiv                            gl3wGetQueryObjectiv
#define glGetQueryObjectui64v                         gl3wGetQueryObjectui64v
#define glGetQueryObjectuiv                           gl3wGetQueryObjectuiv
#define glGetQueryiv                                  gl3wGetQueryiv
#define glGetRenderbufferParameteriv                  gl3wGetRenderbufferParameteriv
#define glGetSamplerParameterIiv                      gl3wGetSamplerParameterIiv
#define glGetSamplerParameterIuiv                     gl3wGetSamplerParameterIuiv
#define glGetSamplerParameterfv                       gl3wGetSamplerParameterfv
#define glGetSamplerParameteriv                       gl3wGetSamplerParameteriv
#define glGetShaderInfoLog                            gl3wGetShaderInfoLog
#define glGetShaderPrecisionFormat                    gl3wGetShaderPrecisionFormat
#define glGetShaderSource                             gl3wGetShaderSource
#define glGetShaderiv                                 gl3wGetShaderiv
#define glGetString                                   gl3wGetString
#define glGetStringi                                  gl3wGetStringi
#define glGetSubroutineIndex                          gl3wGetSubroutineIndex
#define glGetSubroutineUniformLocation                gl3wGetSubroutineUniformLocation
#define glGetSynciv                                   gl3wGetSynciv
#define glGetTexImage                                 gl3wGetTexImage
#define glGetTexLevelParameterfv                      gl3wGetTexLevelParameterfv
#define glGetTexLevelParameteriv                      gl3wGetTexLevelParameteriv
#define glGetTexParameterIiv                          gl3wGetTexParameterIiv
#define glGetTexParameterIuiv                         gl3wGetTexParameterIuiv
#define glGetTexParameterfv                           gl3wGetTexParameterfv
#define glGetTexParameteriv                           gl3wGetTexParameteriv
#define glGetTextureHandleARB                         gl3wGetTextureHandleARB
#define glGetTextureImage                             gl3wGetTextureImage
#define glGetTextureLevelParameterfv                  gl3wGetTextureLevelParameterfv
#define glGetTextureLevelParameteriv                  gl3wGetTextureLevelParameteriv
#define glGetTextureParameterIiv                      gl3wGetTextureParameterIiv
#define glGetTextureParameterIuiv                     gl3wGetTextureParameterIuiv
#define glGetTextureParameterfv                       gl3wGetTextureParameterfv
#define glGetTextureParameteriv                       gl3wGetTextureParameteriv
#define glGetTextureSamplerHandleARB                  gl3wGetTextureSamplerHandleARB
#define glGetTextureSubImage                          gl3wGetTextureSubImage
#define glGetTransformFeedbackVarying                 gl3wGetTransformFeedbackVarying
#define glGetTransformFeedbacki64_v                   gl3wGetTransformFeedbacki64_v
#define glGetTransformFeedbacki_v                     gl3wGetTransformFeedbacki_v
#define glGetTransformFeedbackiv                      gl3wGetTransformFeedbackiv
#define glGetUniformBlockIndex                        gl3wGetUniformBlockIndex
#define glGetUniformIndices                           gl3wGetUniformIndices
#define glGetUniformLocation                          gl3wGetUniformLocation
#define glGetUniformSubroutineuiv                     gl3wGetUniformSubroutineuiv
#define glGetUniformdv                                gl3wGetUniformdv
#define glGetUniformfv                                gl3wGetUniformfv
#define glGetUniformiv                                gl3wGetUniformiv
#define glGetUniformuiv                               gl3wGetUniformuiv
#define glGetVertexArrayIndexed64iv                   gl3wGetVertexArrayIndexed64iv
#define glGetVertexArrayIndexediv                     gl3wGetVertexArrayIndexediv
#define glGetVertexArrayiv                            gl3wGetVertexArrayiv
#define glGetVertexAttribIiv                          gl3wGetVertexAttribIiv
#define glGetVertexAttribIuiv                         gl3wGetVertexAttribIuiv
#define glGetVertexAttribLdv                          gl3wGetVertexAttribLdv
#define glGetVertexAttribLui64vARB                    gl3wGetVertexAttribLui64vARB
#define glGetVertexAttribPointerv                     gl3wGetVertexAttribPointerv
#define glGetVertexAttribdv                           gl3wGetVertexAttribdv
#define glGetVertexAttribfv                           gl3wGetVertexAttribfv
#define glGetVertexAttribiv                           gl3wGetVertexAttribiv
#define glGetnCompressedTexImage                      gl3wGetnCompressedTexImage
#define glGetnCompressedTexImageARB                   gl3wGetnCompressedTexImageARB
#define glGetnTexImage                                gl3wGetnTexImage
#define glGetnTexImageARB                             gl3wGetnTexImageARB
#define glGetnUniformdv                               gl3wGetnUniformdv
#define glGetnUniformdvARB                            gl3wGetnUniformdvARB
#define glGetnUniformfv                               gl3wGetnUniformfv
#define glGetnUniformfvARB                            gl3wGetnUniformfvARB
#define glGetnUniformiv                               gl3wGetnUniformiv
#define glGetnUniformivARB                            gl3wGetnUniformivARB
#define glGetnUniformuiv                              gl3wGetnUniformuiv
#define glGetnUniformuivARB                           gl3wGetnUniformuivARB
#define glHint                                        gl3wHint
#define glInvalidateBufferData                        gl3wInvalidateBufferData
#define glInvalidateBufferSubData                     gl3wInvalidateBufferSubData
#define glInvalidateFramebuffer                       gl3wInvalidateFramebuffer
#define glInvalidateNamedFramebufferData              gl3wInvalidateNamedFramebufferData
#define glInvalidateNamedFramebufferSubData           gl3wInvalidateNamedFramebufferSubData
#define glInvalidateSubFramebuffer                    gl3wInvalidateSubFramebuffer
#define glInvalidateTexImage                          gl3wInvalidateTexImage
#define glInvalidateTexSubImage                       gl3wInvalidateTexSubImage
#define glIsBuffer                                    gl3wIsBuffer
#define glIsEnabled                                   gl3wIsEnabled
#define glIsEnabledi                                  gl3wIsEnabledi
#define glIsFramebuffer                               gl3wIsFramebuffer
#define glIsImageHandleResidentARB                    gl3wIsImageHandleResidentARB
#define glIsNamedStringARB                            gl3wIsNamedStringARB
#define glIsProgram                                   gl3wIsProgram
#define glIsProgramPipeline                           gl3wIsProgramPipeline
#define glIsQuery                                     gl3wIsQuery
#define glIsRenderbuffer                              gl3wIsRenderbuffer
#define glIsSampler                                   gl3wIsSampler
#define glIsShader                                    gl3wIsShader
#define glIsSync                                      gl3wIsSync
#define glIsTexture                                   gl3wIsTexture
#define glIsTextureHandleResidentARB                  gl3wIsTextureHandleResidentARB
#define glIsTransformFeedback                         gl3wIsTransformFeedback
#define glIsVertexArray                               gl3wIsVertexArray
#define glLineWidth                                   gl3wLineWidth
#define glLinkProgram                                 gl3wLinkProgram
#define glLogicOp                                     gl3wLogicOp
#define glMakeImageHandleNonResidentARB               gl3wMakeImageHandleNonResidentARB
#define glMakeImageHandleResidentARB                  gl3wMakeImageHandleResidentARB
#define glMakeTextureHandleNonResidentARB             gl3wMakeTextureHandleNonResidentARB
#define glMakeTextureHandleResidentARB                gl3wMakeTextureHandleResidentARB
#define glMapBuffer                                   gl3wMapBuffer
#define glMapBufferRange                              gl3wMapBufferRange
#define glMapNamedBuffer                              gl3wMapNamedBuffer
#define glMapNamedBufferRange                         gl3wMapNamedBufferRange
#define glMemoryBarrier                               gl3wMemoryBarrier
#define glMemoryBarrierByRegion                       gl3wMemoryBarrierByRegion
#define glMinSampleShading                            gl3wMinSampleShading
#define glMinSampleShadingARB                         gl3wMinSampleShadingARB
#define glMultiDrawArrays                             gl3wMultiDrawArrays
#define glMultiDrawArraysIndirect                     gl3wMultiDrawArraysIndirect
#define glMultiDrawArraysIndirectCountARB             gl3wMultiDrawArraysIndirectCountARB
#define glMultiDrawElements                           gl3wMultiDrawElements
#define glMultiDrawElementsBaseVertex                 gl3wMultiDrawElementsBaseVertex
#define glMultiDrawElementsIndirect                   gl3wMultiDrawElementsIndirect
#define glMultiDrawElementsIndirectCountARB           gl3wMultiDrawElementsIndirectCountARB
#define glNamedBufferData                             gl3wNamedBufferData
#define glNamedBufferPageCommitmentARB                gl3wNamedBufferPageCommitmentARB
#define glNamedBufferPageCommitmentEXT                gl3wNamedBufferPageCommitmentEXT
#define glNamedBufferStorage                          gl3wNamedBufferStorage
#define glNamedBufferSubData                          gl3wNamedBufferSubData
#define glNamedFramebufferDrawBuffer                  gl3wNamedFramebufferDrawBuffer
#define glNamedFramebufferDrawBuffers                 gl3wNamedFramebufferDrawBuffers
#define glNamedFramebufferParameteri                  gl3wNamedFramebufferParameteri
#define glNamedFramebufferReadBuffer                  gl3wNamedFramebufferReadBuffer
#define glNamedFramebufferRenderbuffer                gl3wNamedFramebufferRenderbuffer
#define glNamedFramebufferTexture                     gl3wNamedFramebufferTexture
#define glNamedFramebufferTextureLayer                gl3wNamedFramebufferTextureLayer
#define glNamedRenderbufferStorage                    gl3wNamedRenderbufferStorage
#define glNamedRenderbufferStorageMultisample         gl3wNamedRenderbufferStorageMultisample
#define glNamedStringARB                              gl3wNamedStringARB
#define glObjectLabel                                 gl3wObjectLabel
#define glObjectPtrLabel                              gl3wObjectPtrLabel
#define glPatchParameterfv                            gl3wPatchParameterfv
#define glPatchParameteri                             gl3wPatchParameteri
#define glPauseTransformFeedback                      gl3wPauseTransformFeedback
#define glPixelStoref                                 gl3wPixelStoref
#define glPixelStorei                                 gl3wPixelStorei
#define glPointParameterf                             gl3wPointParameterf
#define glPointParameterfv                            gl3wPointParameterfv
#define glPointParameteri                             gl3wPointParameteri
#define glPointParameteriv                            gl3wPointParameteriv
#define glPointSize                                   gl3wPointSize
#define glPolygonMode                                 gl3wPolygonMode
#define glPolygonOffset                               gl3wPolygonOffset
#define glPopDebugGroup                               gl3wPopDebugGroup
#define glPrimitiveRestartIndex                       gl3wPrimitiveRestartIndex
#define glProgramBinary                               gl3wProgramBinary
#define glProgramParameteri                           gl3wProgramParameteri
#define glProgramUniform1d                            gl3wProgramUniform1d
#define glProgramUniform1dv                           gl3wProgramUniform1dv
#define glProgramUniform1f                            gl3wProgramUniform1f
#define glProgramUniform1fv                           gl3wProgramUniform1fv
#define glProgramUniform1i                            gl3wProgramUniform1i
#define glProgramUniform1iv                           gl3wProgramUniform1iv
#define glProgramUniform1ui                           gl3wProgramUniform1ui
#define glProgramUniform1uiv                          gl3wProgramUniform1uiv
#define glProgramUniform2d                            gl3wProgramUniform2d
#define glProgramUniform2dv                           gl3wProgramUniform2dv
#define glProgramUniform2f                            gl3wProgramUniform2f
#define glProgramUniform2fv                           gl3wProgramUniform2fv
#define glProgramUniform2i                            gl3wProgramUniform2i
#define glProgramUniform2iv                           gl3wProgramUniform2iv
#define glProgramUniform2ui                           gl3wProgramUniform2ui
#define glProgramUniform2uiv                          gl3wProgramUniform2uiv
#define glProgramUniform3d                            gl3wProgramUniform3d
#define glProgramUniform3dv                           gl3wProgramUniform3dv
#define glProgramUniform3f                            gl3wProgramUniform3f
#define glProgramUniform3fv                           gl3wProgramUniform3fv
#define glProgramUniform3i                            gl3wProgramUniform3i
#define glProgramUniform3iv                           gl3wProgramUniform3iv
#define glProgramUniform3ui                           gl3wProgramUniform3ui
#define glProgramUniform3uiv                          gl3wProgramUniform3uiv
#define glProgramUniform4d                            gl3wProgramUniform4d
#define glProgramUniform4dv                           gl3wProgramUniform4dv
#define glProgramUniform4f                            gl3wProgramUniform4f
#define glProgramUniform4fv                           gl3wProgramUniform4fv
#define glProgramUniform4i                            gl3wProgramUniform4i
#define glProgramUniform4iv                           gl3wProgramUniform4iv
#define glProgramUniform4ui                           gl3wProgramUniform4ui
#define glProgramUniform4uiv                          gl3wProgramUniform4uiv
#define glProgramUniformHandleui64ARB                 gl3wProgramUniformHandleui64ARB
#define glProgramUniformHandleui64vARB                gl3wProgramUniformHandleui64vARB
#define glProgramUniformMatrix2dv                     gl3wProgramUniformMatrix2dv
#define glProgramUniformMatrix2fv                     gl3wProgramUniformMatrix2fv
#define glProgramUniformMatrix2x3dv                   gl3wProgramUniformMatrix2x3dv
#define glProgramUniformMatrix2x3fv                   gl3wProgramUniformMatrix2x3fv
#define glProgramUniformMatrix2x4dv                   gl3wProgramUniformMatrix2x4dv
#define glProgramUniformMatrix2x4fv                   gl3wProgramUniformMatrix2x4fv
#define glProgramUniformMatrix3dv                     gl3wProgramUniformMatrix3dv
#define glProgramUniformMatrix3fv                     gl3wProgramUniformMatrix3fv
#define glProgramUniformMatrix3x2dv                   gl3wProgramUniformMatrix3x2dv
#define glProgramUniformMatrix3x2fv                   gl3wProgramUniformMatrix3x2fv
#define glProgramUniformMatrix3x4dv                   gl3wProgramUniformMatrix3x4dv
#define glProgramUniformMatrix3x4fv                   gl3wProgramUniformMatrix3x4fv
#define glProgramUniformMatrix4dv                     gl3wProgramUniformMatrix4dv
#define glProgramUniformMatrix4fv                     gl3wProgramUniformMatrix4fv
#define glProgramUniformMatrix4x2dv                   gl3wProgramUniformMatrix4x2dv
#define glProgramUniformMatrix4x2fv                   gl3wProgramUniformMatrix4x2fv
#define glProgramUniformMatrix4x3dv                   gl3wProgramUniformMatrix4x3dv
#define glProgramUniformMatrix4x3fv                   gl3wProgramUniformMatrix4x3fv
#define glProvokingVertex                             gl3wProvokingVertex
#define glPushDebugGroup                              gl3wPushDebugGroup
#define glQueryCounter                                gl3wQueryCounter
#define glReadBuffer                                  gl3wReadBuffer
#define glReadPixels                                  gl3wReadPixels
#define glReadnPixels                                 gl3wReadnPixels
#define glReadnPixelsARB                              gl3wReadnPixelsARB
#define glReleaseShaderCompiler                       gl3wReleaseShaderCompiler
#define glRenderbufferStorage                         gl3wRenderbufferStorage
#define glRenderbufferStorageMultisample              gl3wRenderbufferStorageMultisample
#define glResumeTransformFeedback                     gl3wResumeTransformFeedback
#define glSampleCoverage                              gl3wSampleCoverage
#define glSampleMaski                                 gl3wSampleMaski
#define glSamplerParameterIiv                         gl3wSamplerParameterIiv
#define glSamplerParameterIuiv                        gl3wSamplerParameterIuiv
#define glSamplerParameterf                           gl3wSamplerParameterf
#define glSamplerParameterfv                          gl3wSamplerParameterfv
#define glSamplerParameteri                           gl3wSamplerParameteri
#define glSamplerParameteriv                          gl3wSamplerParameteriv
#define glScissor                                     gl3wScissor
#define glScissorArrayv                               gl3wScissorArrayv
#define glScissorIndexed                              gl3wScissorIndexed
#define glScissorIndexedv                             gl3wScissorIndexedv
#define glShaderBinary                                gl3wShaderBinary
#define glShaderSource                                gl3wShaderSource
#define glShaderStorageBlockBinding                   gl3wShaderStorageBlockBinding
#define glStencilFunc                                 gl3wStencilFunc
#define glStencilFuncSeparate                         gl3wStencilFuncSeparate
#define glStencilMask                                 gl3wStencilMask
#define glStencilMaskSeparate                         gl3wStencilMaskSeparate
#define glStencilOp                                   gl3wStencilOp
#define glStencilOpSeparate                           gl3wStencilOpSeparate
#define glTexBuffer                                   gl3wTexBuffer
#define glTexBufferRange                              gl3wTexBufferRange
#define glTexImage1D                                  gl3wTexImage1D
#define glTexImage2D                                  gl3wTexImage2D
#define glTexImage2DMultisample                       gl3wTexImage2DMultisample
#define glTexImage3D                                  gl3wTexImage3D
#define glTexImage3DMultisample                       gl3wTexImage3DMultisample
#define glTexPageCommitmentARB                        gl3wTexPageCommitmentARB
#define glTexParameterIiv                             gl3wTexParameterIiv
#define glTexParameterIuiv                            gl3wTexParameterIuiv
#define glTexParameterf                               gl3wTexParameterf
#define glTexParameterfv                              gl3wTexParameterfv
#define glTexParameteri                               gl3wTexParameteri
#define glTexParameteriv                              gl3wTexParameteriv
#define glTexStorage1D                                gl3wTexStorage1D
#define glTexStorage2D                                gl3wTexStorage2D
#define glTexStorage2DMultisample                     gl3wTexStorage2DMultisample
#define glTexStorage3D                                gl3wTexStorage3D
#define glTexStorage3DMultisample                     gl3wTexStorage3DMultisample
#define glTexSubImage1D                               gl3wTexSubImage1D
#define glTexSubImage2D                               gl3wTexSubImage2D
#define glTexSubImage3D                               gl3wTexSubImage3D
#define glTextureBarrier                              gl3wTextureBarrier
#define glTextureBuffer                               gl3wTextureBuffer
#define glTextureBufferRange                          gl3wTextureBufferRange
#define glTextureParameterIiv                         gl3wTextureParameterIiv
#define glTextureParameterIuiv                        gl3wTextureParameterIuiv
#define glTextureParameterf                           gl3wTextureParameterf
#define glTextureParameterfv                          gl3wTextureParameterfv
#define glTextureParameteri                           gl3wTextureParameteri
#define glTextureParameteriv                          gl3wTextureParameteriv
#define glTextureStorage1D                            gl3wTextureStorage1D
#define glTextureStorage2D                            gl3wTextureStorage2D
#define glTextureStorage2DMultisample                 gl3wTextureStorage2DMultisample
#define glTextureStorage3D                            gl3wTextureStorage3D
#define glTextureStorage3DMultisample                 gl3wTextureStorage3DMultisample
#define glTextureSubImage1D                           gl3wTextureSubImage1D
#define glTextureSubImage2D                           gl3wTextureSubImage2D
#define glTextureSubImage3D                           gl3wTextureSubImage3D
#define glTextureView                                 gl3wTextureView
#define glTransformFeedbackBufferBase                 gl3wTransformFeedbackBufferBase
#define glTransformFeedbackBufferRange                gl3wTransformFeedbackBufferRange
#define glTransformFeedbackVaryings                   gl3wTransformFeedbackVaryings
#define glUniform1d                                   gl3wUniform1d
#define glUniform1dv                                  gl3wUniform1dv
#define glUniform1f                                   gl3wUniform1f
#define glUniform1fv                                  gl3wUniform1fv
#define glUniform1i                                   gl3wUniform1i
#define glUniform1iv                                  gl3wUniform1iv
#define glUniform1ui                                  gl3wUniform1ui
#define glUniform1uiv                                 gl3wUniform1uiv
#define glUniform2d                                   gl3wUniform2d
#define glUniform2dv                                  gl3wUniform2dv
#define glUniform2f                                   gl3wUniform2f
#define glUniform2fv                                  gl3wUniform2fv
#define glUniform2i                                   gl3wUniform2i
#define glUniform2iv                                  gl3wUniform2iv
#define glUniform2ui                                  gl3wUniform2ui
#define glUniform2uiv                                 gl3wUniform2uiv
#define glUniform3d                                   gl3wUniform3d
#define glUniform3dv                                  gl3wUniform3dv
#define glUniform3f                                   gl3wUniform3f
#define glUniform3fv                                  gl3wUniform3fv
#define glUniform3i                                   gl3wUniform3i
#define glUniform3iv                                  gl3wUniform3iv
#define glUniform3ui                                  gl3wUniform3ui
#define glUniform3uiv                                 gl3wUniform3uiv
#define glUniform4d                                   gl3wUniform4d
#define glUniform4dv                                  gl3wUniform4dv
#define glUniform4f                                   gl3wUniform4f
#define glUniform4fv                                  gl3wUniform4fv
#define glUniform4i                                   gl3wUniform4i
#define glUniform4iv                                  gl3wUniform4iv
#define glUniform4ui                                  gl3wUniform4ui
#define glUniform4uiv                                 gl3wUniform4uiv
#define glUniformBlockBinding                         gl3wUniformBlockBinding
#define glUniformHandleui64ARB                        gl3wUniformHandleui64ARB
#define glUniformHandleui64vARB                       gl3wUniformHandleui64vARB
#define glUniformMatrix2dv                            gl3wUniformMatrix2dv
#define glUniformMatrix2fv                            gl3wUniformMatrix2fv
#define glUniformMatrix2x3dv                          gl3wUniformMatrix2x3dv
#define glUniformMatrix2x3fv                          gl3wUniformMatrix2x3fv
#define glUniformMatrix2x4dv                          gl3wUniformMatrix2x4dv
#define glUniformMatrix2x4fv                          gl3wUniformMatrix2x4fv
#define glUniformMatrix3dv                            gl3wUniformMatrix3dv
#define glUniformMatrix3fv                            gl3wUniformMatrix3fv
#define glUniformMatrix3x2dv                          gl3wUniformMatrix3x2dv
#define glUniformMatrix3x2fv                          gl3wUniformMatrix3x2fv
#define glUniformMatrix3x4dv                          gl3wUniformMatrix3x4dv
#define glUniformMatrix3x4fv                          gl3wUniformMatrix3x4fv
#define glUniformMatrix4dv                            gl3wUniformMatrix4dv
#define glUniformMatrix4fv                            gl3wUniformMatrix4fv
#define glUniformMatrix4x2dv                          gl3wUniformMatrix4x2dv
#define glUniformMatrix4x2fv                          gl3wUniformMatrix4x2fv
#define glUniformMatrix4x3dv                          gl3wUniformMatrix4x3dv
#define glUniformMatrix4x3fv                          gl3wUniformMatrix4x3fv
#define glUniformSubroutinesuiv                       gl3wUniformSubroutinesuiv
#define glUnmapBuffer                                 gl3wUnmapBuffer
#define glUnmapNamedBuffer                            gl3wUnmapNamedBuffer
#define glUseProgram                                  gl3wUseProgram
#define glUseProgramStages                            gl3wUseProgramStages
#define glValidateProgram                             gl3wValidateProgram
#define glValidateProgramPipeline                     gl3wValidateProgramPipeline
#define glVertexArrayAttribBinding                    gl3wVertexArrayAttribBinding
#define glVertexArrayAttribFormat                     gl3wVertexArrayAttribFormat
#define glVertexArrayAttribIFormat                    gl3wVertexArrayAttribIFormat
#define glVertexArrayAttribLFormat                    gl3wVertexArrayAttribLFormat
#define glVertexArrayBindingDivisor                   gl3wVertexArrayBindingDivisor
#define glVertexArrayElementBuffer                    gl3wVertexArrayElementBuffer
#define glVertexArrayVertexBuffer                     gl3wVertexArrayVertexBuffer
#define glVertexArrayVertexBuffers                    gl3wVertexArrayVertexBuffers
#define glVertexAttrib1d                              gl3wVertexAttrib1d
#define glVertexAttrib1dv                             gl3wVertexAttrib1dv
#define glVertexAttrib1f                              gl3wVertexAttrib1f
#define glVertexAttrib1fv                             gl3wVertexAttrib1fv
#define glVertexAttrib1s                              gl3wVertexAttrib1s
#define glVertexAttrib1sv                             gl3wVertexAttrib1sv
#define glVertexAttrib2d                              gl3wVertexAttrib2d
#define glVertexAttrib2dv                             gl3wVertexAttrib2dv
#define glVertexAttrib2f                              gl3wVertexAttrib2f
#define glVertexAttrib2fv                             gl3wVertexAttrib2fv
#define glVertexAttrib2s                              gl3wVertexAttrib2s
#define glVertexAttrib2sv                             gl3wVertexAttrib2sv
#define glVertexAttrib3d                              gl3wVertexAttrib3d
#define glVertexAttrib3dv                             gl3wVertexAttrib3dv
#define glVertexAttrib3f                              gl3wVertexAttrib3f
#define glVertexAttrib3fv                             gl3wVertexAttrib3fv
#define glVertexAttrib3s                              gl3wVertexAttrib3s
#define glVertexAttrib3sv                             gl3wVertexAttrib3sv
#define glVertexAttrib4Nbv                            gl3wVertexAttrib4Nbv
#define glVertexAttrib4Niv                            gl3wVertexAttrib4Niv
#define glVertexAttrib4Nsv                            gl3wVertexAttrib4Nsv
#define glVertexAttrib4Nub                            gl3wVertexAttrib4Nub
#define glVertexAttrib4Nubv                           gl3wVertexAttrib4Nubv
#define glVertexAttrib4Nuiv                           gl3wVertexAttrib4Nuiv
#define glVertexAttrib4Nusv                           gl3wVertexAttrib4Nusv
#define glVertexAttrib4bv                             gl3wVertexAttrib4bv
#define glVertexAttrib4d                              gl3wVertexAttrib4d
#define glVertexAttrib4dv                             gl3wVertexAttrib4dv
#define glVertexAttrib4f                              gl3wVertexAttrib4f
#define glVertexAttrib4fv                             gl3wVertexAttrib4fv
#define glVertexAttrib4iv                             gl3wVertexAttrib4iv
#define glVertexAttrib4s                              gl3wVertexAttrib4s
#define glVertexAttrib4sv                             gl3wVertexAttrib4sv
#define glVertexAttrib4ubv                            gl3wVertexAttrib4ubv
#define glVertexAttrib4uiv                            gl3wVertexAttrib4uiv
#define glVertexAttrib4usv                            gl3wVertexAttrib4usv
#define glVertexAttribBinding                         gl3wVertexAttribBinding
#define glVertexAttribDivisor                         gl3wVertexAttribDivisor
#define glVertexAttribFormat                          gl3wVertexAttribFormat
#define glVertexAttribI1i                             gl3wVertexAttribI1i
#define glVertexAttribI1iv                            gl3wVertexAttribI1iv
#define glVertexAttribI1ui                            gl3wVertexAttribI1ui
#define glVertexAttribI1uiv                           gl3wVertexAttribI1uiv
#define glVertexAttribI2i                             gl3wVertexAttribI2i
#define glVertexAttribI2iv                            gl3wVertexAttribI2iv
#define glVertexAttribI2ui                            gl3wVertexAttribI2ui
#define glVertexAttribI2uiv                           gl3wVertexAttribI2uiv
#define glVertexAttribI3i                             gl3wVertexAttribI3i
#define glVertexAttribI3iv                            gl3wVertexAttribI3iv
#define glVertexAttribI3ui                            gl3wVertexAttribI3ui
#define glVertexAttribI3uiv                           gl3wVertexAttribI3uiv
#define glVertexAttribI4bv                            gl3wVertexAttribI4bv
#define glVertexAttribI4i                             gl3wVertexAttribI4i
#define glVertexAttribI4iv                            gl3wVertexAttribI4iv
#define glVertexAttribI4sv                            gl3wVertexAttribI4sv
#define glVertexAttribI4ubv                           gl3wVertexAttribI4ubv
#define glVertexAttribI4ui                            gl3wVertexAttribI4ui
#define glVertexAttribI4uiv                           gl3wVertexAttribI4uiv
#define glVertexAttribI4usv                           gl3wVertexAttribI4usv
#define glVertexAttribIFormat                         gl3wVertexAttribIFormat
#define glVertexAttribIPointer                        gl3wVertexAttribIPointer
#define glVertexAttribL1d                             gl3wVertexAttribL1d
#define glVertexAttribL1dv                            gl3wVertexAttribL1dv
#define glVertexAttribL1ui64ARB                       gl3wVertexAttribL1ui64ARB
#define glVertexAttribL1ui64vARB                      gl3wVertexAttribL1ui64vARB
#define glVertexAttribL2d                             gl3wVertexAttribL2d
#define glVertexAttribL2dv                            gl3wVertexAttribL2dv
#define glVertexAttribL3d                             gl3wVertexAttribL3d
#define glVertexAttribL3dv                            gl3wVertexAttribL3dv
#define glVertexAttribL4d                             gl3wVertexAttribL4d
#define glVertexAttribL4dv                            gl3wVertexAttribL4dv
#define glVertexAttribLFormat                         gl3wVertexAttribLFormat
#define glVertexAttribLPointer                        gl3wVertexAttribLPointer
#define glVertexAttribP1ui                            gl3wVertexAttribP1ui
#define glVertexAttribP1uiv                           gl3wVertexAttribP1uiv
#define glVertexAttribP2ui                            gl3wVertexAttribP2ui
#define glVertexAttribP2uiv                           gl3wVertexAttribP2uiv
#define glVertexAttribP3ui                            gl3wVertexAttribP3ui
#define glVertexAttribP3uiv                           gl3wVertexAttribP3uiv
#define glVertexAttribP4ui                            gl3wVertexAttribP4ui
#define glVertexAttribP4uiv                           gl3wVertexAttribP4uiv
#define glVertexAttribPointer                         gl3wVertexAttribPointer
#define glVertexBindingDivisor                        gl3wVertexBindingDivisor
#define glViewport                                    gl3wViewport
#define glViewportArrayv                              gl3wViewportArrayv
#define glViewportIndexedf                            gl3wViewportIndexedf
#define glViewportIndexedfv                           gl3wViewportIndexedfv
#define glWaitSync                                    gl3wWaitSync

#ifdef __cplusplus
}
#endif

#endif

#if defined(GL3W_IMPLEMENTATION) && !defined(GL3W_IMPLEMENTATION_DONE)
#define GL3W_IMPLEMENTATION_DONE

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN 1
#define WIN32_MEAN_AND_LEAN 1
#include <windows.h>

static HMODULE gl3w__libgl;

static void gl3w__open_libgl (void) { gl3w__libgl = LoadLibraryA("opengl32.dll"); }
static void gl3w__close_libgl(void) { FreeLibrary(gl3w__libgl); }

static GL3WglProc gl3w__get_proc(char const *proc)
{
	GL3WglProc res;

	res = (GL3WglProc) wglGetProcAddress(proc);
	if (!res)
		res = (GL3WglProc) GetProcAddress(gl3w__libgl, proc);
	return res;
}

#elif defined(__APPLE__) || defined(__APPLE_CC__)

#include <Carbon/Carbon.h>

CFBundleRef gl3w__bundle;
CFURLRef gl3w__bundleURL;

static void gl3w__open_libgl(void)
{
	gl3w__bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
		CFSTR("/System/Library/Frameworks/OpenGL.framework"),
		kCFURLPOSIXPathStyle, true);

	gl3w__bundle = CFBundleCreate(kCFAllocatorDefault, gl3w__bundleURL);
	assert(gl3w__bundle != NULL);
}

static void gl3w__close_libgl(void)
{
	CFRelease(gl3w__bundle);
	CFRelease(gl3w__bundleURL);
}

static GL3WglProc gl3w__get_proc(char const *proc)
{
	GL3WglProc res;

	CFStringRef procname = CFStringCreateWithCString(kCFAllocatorDefault, proc,
		kCFStringEncodingASCII);
	res = (GL3WglProc) CFBundleGetFunctionPointerForName(gl3w__bundle, procname);
	CFRelease(procname);
	return res;
}

#else

#include <dlfcn.h>
#include <GL/glx.h>

static void *gl3w__libgl;
static PFNGLXGETPROCADDRESSPROC gl3w__glx_get_proc_address;

static void gl3w__open_libgl(void)
{
	gl3w__libgl = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL);
	gl3w__glx_get_proc_address = (PFNGLXGETPROCADDRESSPROC) dlsym(gl3w__libgl, "glXGetProcAddressARB");
}

static void gl3w__close_libgl(void) { dlclose(gl3w__libgl); }

static GL3WglProc gl3w__get_proc(char const *proc)
{
	GL3WglProc res;

	res = (GL3WglProc) gl3w__glx_get_proc_address((const GLubyte *) proc);
	if (!res)
		res = (GL3WglProc) dlsym(gl3w__libgl, proc);
	return res;
}

#endif

static struct {
	int major, minor;
} gl3w__version;

static int gl3w__parse_version(void)
{
	if (!glGetIntegerv)
		return -1;

	glGetIntegerv(GL_MAJOR_VERSION, &gl3w__version.major);
	glGetIntegerv(GL_MINOR_VERSION, &gl3w__version.minor);

	if (gl3w__version.major < 3)
		return -1;
	return 0;
}

static void gl3w__load_procs(void);

int gl3w_init(void)
{
	gl3w__open_libgl();
	gl3w__load_procs();
	gl3w__close_libgl();
	return gl3w__parse_version();
}

int gl3w_is_supported(int major, int minor)
{
	if (major < 3)
		return 0;
	if (gl3w__version.major == major)
		return gl3w__version.minor >= minor;
	return gl3w__version.major >= major;
}

GL3WglProc gl3w_get_proc_address(char const *proc)
{
	return gl3w__get_proc(proc);
}

PFNGLACTIVESHADERPROGRAMPROC                         gl3wActiveShaderProgram;
PFNGLACTIVETEXTUREPROC                               gl3wActiveTexture;
PFNGLATTACHSHADERPROC                                gl3wAttachShader;
PFNGLBEGINCONDITIONALRENDERPROC                      gl3wBeginConditionalRender;
PFNGLBEGINQUERYPROC                                  gl3wBeginQuery;
PFNGLBEGINQUERYINDEXEDPROC                           gl3wBeginQueryIndexed;
PFNGLBEGINTRANSFORMFEEDBACKPROC                      gl3wBeginTransformFeedback;
PFNGLBINDATTRIBLOCATIONPROC                          gl3wBindAttribLocation;
PFNGLBINDBUFFERPROC                                  gl3wBindBuffer;
PFNGLBINDBUFFERBASEPROC                              gl3wBindBufferBase;
PFNGLBINDBUFFERRANGEPROC                             gl3wBindBufferRange;
PFNGLBINDBUFFERSBASEPROC                             gl3wBindBuffersBase;
PFNGLBINDBUFFERSRANGEPROC                            gl3wBindBuffersRange;
PFNGLBINDFRAGDATALOCATIONPROC                        gl3wBindFragDataLocation;
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC                 gl3wBindFragDataLocationIndexed;
PFNGLBINDFRAMEBUFFERPROC                             gl3wBindFramebuffer;
PFNGLBINDIMAGETEXTUREPROC                            gl3wBindImageTexture;
PFNGLBINDIMAGETEXTURESPROC                           gl3wBindImageTextures;
PFNGLBINDPROGRAMPIPELINEPROC                         gl3wBindProgramPipeline;
PFNGLBINDRENDERBUFFERPROC                            gl3wBindRenderbuffer;
PFNGLBINDSAMPLERPROC                                 gl3wBindSampler;
PFNGLBINDSAMPLERSPROC                                gl3wBindSamplers;
PFNGLBINDTEXTUREPROC                                 gl3wBindTexture;
PFNGLBINDTEXTUREUNITPROC                             gl3wBindTextureUnit;
PFNGLBINDTEXTURESPROC                                gl3wBindTextures;
PFNGLBINDTRANSFORMFEEDBACKPROC                       gl3wBindTransformFeedback;
PFNGLBINDVERTEXARRAYPROC                             gl3wBindVertexArray;
PFNGLBINDVERTEXBUFFERPROC                            gl3wBindVertexBuffer;
PFNGLBINDVERTEXBUFFERSPROC                           gl3wBindVertexBuffers;
PFNGLBLENDCOLORPROC                                  gl3wBlendColor;
PFNGLBLENDEQUATIONPROC                               gl3wBlendEquation;
PFNGLBLENDEQUATIONSEPARATEPROC                       gl3wBlendEquationSeparate;
PFNGLBLENDEQUATIONSEPARATEIPROC                      gl3wBlendEquationSeparatei;
PFNGLBLENDEQUATIONSEPARATEIARBPROC                   gl3wBlendEquationSeparateiARB;
PFNGLBLENDEQUATIONIPROC                              gl3wBlendEquationi;
PFNGLBLENDEQUATIONIARBPROC                           gl3wBlendEquationiARB;
PFNGLBLENDFUNCPROC                                   gl3wBlendFunc;
PFNGLBLENDFUNCSEPARATEPROC                           gl3wBlendFuncSeparate;
PFNGLBLENDFUNCSEPARATEIPROC                          gl3wBlendFuncSeparatei;
PFNGLBLENDFUNCSEPARATEIARBPROC                       gl3wBlendFuncSeparateiARB;
PFNGLBLENDFUNCIPROC                                  gl3wBlendFunci;
PFNGLBLENDFUNCIARBPROC                               gl3wBlendFunciARB;
PFNGLBLITFRAMEBUFFERPROC                             gl3wBlitFramebuffer;
PFNGLBLITNAMEDFRAMEBUFFERPROC                        gl3wBlitNamedFramebuffer;
PFNGLBUFFERDATAPROC                                  gl3wBufferData;
PFNGLBUFFERPAGECOMMITMENTARBPROC                     gl3wBufferPageCommitmentARB;
PFNGLBUFFERSTORAGEPROC                               gl3wBufferStorage;
PFNGLBUFFERSUBDATAPROC                               gl3wBufferSubData;
PFNGLCHECKFRAMEBUFFERSTATUSPROC                      gl3wCheckFramebufferStatus;
PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC                 gl3wCheckNamedFramebufferStatus;
PFNGLCLAMPCOLORPROC                                  gl3wClampColor;
PFNGLCLEARPROC                                       gl3wClear;
PFNGLCLEARBUFFERDATAPROC                             gl3wClearBufferData;
PFNGLCLEARBUFFERSUBDATAPROC                          gl3wClearBufferSubData;
PFNGLCLEARBUFFERFIPROC                               gl3wClearBufferfi;
PFNGLCLEARBUFFERFVPROC                               gl3wClearBufferfv;
PFNGLCLEARBUFFERIVPROC                               gl3wClearBufferiv;
PFNGLCLEARBUFFERUIVPROC                              gl3wClearBufferuiv;
PFNGLCLEARCOLORPROC                                  gl3wClearColor;
PFNGLCLEARDEPTHPROC                                  gl3wClearDepth;
PFNGLCLEARDEPTHFPROC                                 gl3wClearDepthf;
PFNGLCLEARNAMEDBUFFERDATAPROC                        gl3wClearNamedBufferData;
PFNGLCLEARNAMEDBUFFERSUBDATAPROC                     gl3wClearNamedBufferSubData;
PFNGLCLEARNAMEDFRAMEBUFFERFIPROC                     gl3wClearNamedFramebufferfi;
PFNGLCLEARNAMEDFRAMEBUFFERFVPROC                     gl3wClearNamedFramebufferfv;
PFNGLCLEARNAMEDFRAMEBUFFERIVPROC                     gl3wClearNamedFramebufferiv;
PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC                    gl3wClearNamedFramebufferuiv;
PFNGLCLEARSTENCILPROC                                gl3wClearStencil;
PFNGLCLEARTEXIMAGEPROC                               gl3wClearTexImage;
PFNGLCLEARTEXSUBIMAGEPROC                            gl3wClearTexSubImage;
PFNGLCLIENTWAITSYNCPROC                              gl3wClientWaitSync;
PFNGLCLIPCONTROLPROC                                 gl3wClipControl;
PFNGLCOLORMASKPROC                                   gl3wColorMask;
PFNGLCOLORMASKIPROC                                  gl3wColorMaski;
PFNGLCOMPILESHADERPROC                               gl3wCompileShader;
PFNGLCOMPILESHADERINCLUDEARBPROC                     gl3wCompileShaderIncludeARB;
PFNGLCOMPRESSEDTEXIMAGE1DPROC                        gl3wCompressedTexImage1D;
PFNGLCOMPRESSEDTEXIMAGE2DPROC                        gl3wCompressedTexImage2D;
PFNGLCOMPRESSEDTEXIMAGE3DPROC                        gl3wCompressedTexImage3D;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC                     gl3wCompressedTexSubImage1D;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC                     gl3wCompressedTexSubImage2D;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC                     gl3wCompressedTexSubImage3D;
PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC                 gl3wCompressedTextureSubImage1D;
PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC                 gl3wCompressedTextureSubImage2D;
PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC                 gl3wCompressedTextureSubImage3D;
PFNGLCOPYBUFFERSUBDATAPROC                           gl3wCopyBufferSubData;
PFNGLCOPYIMAGESUBDATAPROC                            gl3wCopyImageSubData;
PFNGLCOPYNAMEDBUFFERSUBDATAPROC                      gl3wCopyNamedBufferSubData;
PFNGLCOPYTEXIMAGE1DPROC                              gl3wCopyTexImage1D;
PFNGLCOPYTEXIMAGE2DPROC                              gl3wCopyTexImage2D;
PFNGLCOPYTEXSUBIMAGE1DPROC                           gl3wCopyTexSubImage1D;
PFNGLCOPYTEXSUBIMAGE2DPROC                           gl3wCopyTexSubImage2D;
PFNGLCOPYTEXSUBIMAGE3DPROC                           gl3wCopyTexSubImage3D;
PFNGLCOPYTEXTURESUBIMAGE1DPROC                       gl3wCopyTextureSubImage1D;
PFNGLCOPYTEXTURESUBIMAGE2DPROC                       gl3wCopyTextureSubImage2D;
PFNGLCOPYTEXTURESUBIMAGE3DPROC                       gl3wCopyTextureSubImage3D;
PFNGLCREATEBUFFERSPROC                               gl3wCreateBuffers;
PFNGLCREATEFRAMEBUFFERSPROC                          gl3wCreateFramebuffers;
PFNGLCREATEPROGRAMPROC                               gl3wCreateProgram;
PFNGLCREATEPROGRAMPIPELINESPROC                      gl3wCreateProgramPipelines;
PFNGLCREATEQUERIESPROC                               gl3wCreateQueries;
PFNGLCREATERENDERBUFFERSPROC                         gl3wCreateRenderbuffers;
PFNGLCREATESAMPLERSPROC                              gl3wCreateSamplers;
PFNGLCREATESHADERPROC                                gl3wCreateShader;
PFNGLCREATESHADERPROGRAMVPROC                        gl3wCreateShaderProgramv;
PFNGLCREATESYNCFROMCLEVENTARBPROC                    gl3wCreateSyncFromCLeventARB;
PFNGLCREATETEXTURESPROC                              gl3wCreateTextures;
PFNGLCREATETRANSFORMFEEDBACKSPROC                    gl3wCreateTransformFeedbacks;
PFNGLCREATEVERTEXARRAYSPROC                          gl3wCreateVertexArrays;
PFNGLCULLFACEPROC                                    gl3wCullFace;
PFNGLDEBUGMESSAGECALLBACKPROC                        gl3wDebugMessageCallback;
PFNGLDEBUGMESSAGECALLBACKARBPROC                     gl3wDebugMessageCallbackARB;
PFNGLDEBUGMESSAGECONTROLPROC                         gl3wDebugMessageControl;
PFNGLDEBUGMESSAGECONTROLARBPROC                      gl3wDebugMessageControlARB;
PFNGLDEBUGMESSAGEINSERTPROC                          gl3wDebugMessageInsert;
PFNGLDEBUGMESSAGEINSERTARBPROC                       gl3wDebugMessageInsertARB;
PFNGLDELETEBUFFERSPROC                               gl3wDeleteBuffers;
PFNGLDELETEFRAMEBUFFERSPROC                          gl3wDeleteFramebuffers;
PFNGLDELETENAMEDSTRINGARBPROC                        gl3wDeleteNamedStringARB;
PFNGLDELETEPROGRAMPROC                               gl3wDeleteProgram;
PFNGLDELETEPROGRAMPIPELINESPROC                      gl3wDeleteProgramPipelines;
PFNGLDELETEQUERIESPROC                               gl3wDeleteQueries;
PFNGLDELETERENDERBUFFERSPROC                         gl3wDeleteRenderbuffers;
PFNGLDELETESAMPLERSPROC                              gl3wDeleteSamplers;
PFNGLDELETESHADERPROC                                gl3wDeleteShader;
PFNGLDELETESYNCPROC                                  gl3wDeleteSync;
PFNGLDELETETEXTURESPROC                              gl3wDeleteTextures;
PFNGLDELETETRANSFORMFEEDBACKSPROC                    gl3wDeleteTransformFeedbacks;
PFNGLDELETEVERTEXARRAYSPROC                          gl3wDeleteVertexArrays;
PFNGLDEPTHFUNCPROC                                   gl3wDepthFunc;
PFNGLDEPTHMASKPROC                                   gl3wDepthMask;
PFNGLDEPTHRANGEPROC                                  gl3wDepthRange;
PFNGLDEPTHRANGEARRAYVPROC                            gl3wDepthRangeArrayv;
PFNGLDEPTHRANGEINDEXEDPROC                           gl3wDepthRangeIndexed;
PFNGLDEPTHRANGEFPROC                                 gl3wDepthRangef;
PFNGLDETACHSHADERPROC                                gl3wDetachShader;
PFNGLDISABLEPROC                                     gl3wDisable;
PFNGLDISABLEVERTEXARRAYATTRIBPROC                    gl3wDisableVertexArrayAttrib;
PFNGLDISABLEVERTEXATTRIBARRAYPROC                    gl3wDisableVertexAttribArray;
PFNGLDISABLEIPROC                                    gl3wDisablei;
PFNGLDISPATCHCOMPUTEPROC                             gl3wDispatchCompute;
PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC                 gl3wDispatchComputeGroupSizeARB;
PFNGLDISPATCHCOMPUTEINDIRECTPROC                     gl3wDispatchComputeIndirect;
PFNGLDRAWARRAYSPROC                                  gl3wDrawArrays;
PFNGLDRAWARRAYSINDIRECTPROC                          gl3wDrawArraysIndirect;
PFNGLDRAWARRAYSINSTANCEDPROC                         gl3wDrawArraysInstanced;
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC             gl3wDrawArraysInstancedBaseInstance;
PFNGLDRAWBUFFERPROC                                  gl3wDrawBuffer;
PFNGLDRAWBUFFERSPROC                                 gl3wDrawBuffers;
PFNGLDRAWELEMENTSPROC                                gl3wDrawElements;
PFNGLDRAWELEMENTSBASEVERTEXPROC                      gl3wDrawElementsBaseVertex;
PFNGLDRAWELEMENTSINDIRECTPROC                        gl3wDrawElementsIndirect;
PFNGLDRAWELEMENTSINSTANCEDPROC                       gl3wDrawElementsInstanced;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC           gl3wDrawElementsInstancedBaseInstance;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC             gl3wDrawElementsInstancedBaseVertex;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC gl3wDrawElementsInstancedBaseVertexBaseInstance;
PFNGLDRAWRANGEELEMENTSPROC                           gl3wDrawRangeElements;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC                 gl3wDrawRangeElementsBaseVertex;
PFNGLDRAWTRANSFORMFEEDBACKPROC                       gl3wDrawTransformFeedback;
PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC              gl3wDrawTransformFeedbackInstanced;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC                 gl3wDrawTransformFeedbackStream;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC        gl3wDrawTransformFeedbackStreamInstanced;
PFNGLENABLEPROC                                      gl3wEnable;
PFNGLENABLEVERTEXARRAYATTRIBPROC                     gl3wEnableVertexArrayAttrib;
PFNGLENABLEVERTEXATTRIBARRAYPROC                     gl3wEnableVertexAttribArray;
PFNGLENABLEIPROC                                     gl3wEnablei;
PFNGLENDCONDITIONALRENDERPROC                        gl3wEndConditionalRender;
PFNGLENDQUERYPROC                                    gl3wEndQuery;
PFNGLENDQUERYINDEXEDPROC                             gl3wEndQueryIndexed;
PFNGLENDTRANSFORMFEEDBACKPROC                        gl3wEndTransformFeedback;
PFNGLFENCESYNCPROC                                   gl3wFenceSync;
PFNGLFINISHPROC                                      gl3wFinish;
PFNGLFLUSHPROC                                       gl3wFlush;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC                      gl3wFlushMappedBufferRange;
PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC                 gl3wFlushMappedNamedBufferRange;
PFNGLFRAMEBUFFERPARAMETERIPROC                       gl3wFramebufferParameteri;
PFNGLFRAMEBUFFERRENDERBUFFERPROC                     gl3wFramebufferRenderbuffer;
PFNGLFRAMEBUFFERTEXTUREPROC                          gl3wFramebufferTexture;
PFNGLFRAMEBUFFERTEXTURE1DPROC                        gl3wFramebufferTexture1D;
PFNGLFRAMEBUFFERTEXTURE2DPROC                        gl3wFramebufferTexture2D;
PFNGLFRAMEBUFFERTEXTURE3DPROC                        gl3wFramebufferTexture3D;
PFNGLFRAMEBUFFERTEXTURELAYERPROC                     gl3wFramebufferTextureLayer;
PFNGLFRONTFACEPROC                                   gl3wFrontFace;
PFNGLGENBUFFERSPROC                                  gl3wGenBuffers;
PFNGLGENFRAMEBUFFERSPROC                             gl3wGenFramebuffers;
PFNGLGENPROGRAMPIPELINESPROC                         gl3wGenProgramPipelines;
PFNGLGENQUERIESPROC                                  gl3wGenQueries;
PFNGLGENRENDERBUFFERSPROC                            gl3wGenRenderbuffers;
PFNGLGENSAMPLERSPROC                                 gl3wGenSamplers;
PFNGLGENTEXTURESPROC                                 gl3wGenTextures;
PFNGLGENTRANSFORMFEEDBACKSPROC                       gl3wGenTransformFeedbacks;
PFNGLGENVERTEXARRAYSPROC                             gl3wGenVertexArrays;
PFNGLGENERATEMIPMAPPROC                              gl3wGenerateMipmap;
PFNGLGENERATETEXTUREMIPMAPPROC                       gl3wGenerateTextureMipmap;
PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC              gl3wGetActiveAtomicCounterBufferiv;
PFNGLGETACTIVEATTRIBPROC                             gl3wGetActiveAttrib;
PFNGLGETACTIVESUBROUTINENAMEPROC                     gl3wGetActiveSubroutineName;
PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC              gl3wGetActiveSubroutineUniformName;
PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC                gl3wGetActiveSubroutineUniformiv;
PFNGLGETACTIVEUNIFORMPROC                            gl3wGetActiveUniform;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC                   gl3wGetActiveUniformBlockName;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC                     gl3wGetActiveUniformBlockiv;
PFNGLGETACTIVEUNIFORMNAMEPROC                        gl3wGetActiveUniformName;
PFNGLGETACTIVEUNIFORMSIVPROC                         gl3wGetActiveUniformsiv;
PFNGLGETATTACHEDSHADERSPROC                          gl3wGetAttachedShaders;
PFNGLGETATTRIBLOCATIONPROC                           gl3wGetAttribLocation;
PFNGLGETBOOLEANI_VPROC                               gl3wGetBooleani_v;
PFNGLGETBOOLEANVPROC                                 gl3wGetBooleanv;
PFNGLGETBUFFERPARAMETERI64VPROC                      gl3wGetBufferParameteri64v;
PFNGLGETBUFFERPARAMETERIVPROC                        gl3wGetBufferParameteriv;
PFNGLGETBUFFERPOINTERVPROC                           gl3wGetBufferPointerv;
PFNGLGETBUFFERSUBDATAPROC                            gl3wGetBufferSubData;
PFNGLGETCOMPRESSEDTEXIMAGEPROC                       gl3wGetCompressedTexImage;
PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC                   gl3wGetCompressedTextureImage;
PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC                gl3wGetCompressedTextureSubImage;
PFNGLGETDEBUGMESSAGELOGPROC                          gl3wGetDebugMessageLog;
PFNGLGETDEBUGMESSAGELOGARBPROC                       gl3wGetDebugMessageLogARB;
PFNGLGETDOUBLEI_VPROC                                gl3wGetDoublei_v;
PFNGLGETDOUBLEVPROC                                  gl3wGetDoublev;
PFNGLGETERRORPROC                                    gl3wGetError;
PFNGLGETFLOATI_VPROC                                 gl3wGetFloati_v;
PFNGLGETFLOATVPROC                                   gl3wGetFloatv;
PFNGLGETFRAGDATAINDEXPROC                            gl3wGetFragDataIndex;
PFNGLGETFRAGDATALOCATIONPROC                         gl3wGetFragDataLocation;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC         gl3wGetFramebufferAttachmentParameteriv;
PFNGLGETFRAMEBUFFERPARAMETERIVPROC                   gl3wGetFramebufferParameteriv;
PFNGLGETGRAPHICSRESETSTATUSPROC                      gl3wGetGraphicsResetStatus;
PFNGLGETGRAPHICSRESETSTATUSARBPROC                   gl3wGetGraphicsResetStatusARB;
PFNGLGETIMAGEHANDLEARBPROC                           gl3wGetImageHandleARB;
PFNGLGETINTEGER64I_VPROC                             gl3wGetInteger64i_v;
PFNGLGETINTEGER64VPROC                               gl3wGetInteger64v;
PFNGLGETINTEGERI_VPROC                               gl3wGetIntegeri_v;
PFNGLGETINTEGERVPROC                                 gl3wGetIntegerv;
PFNGLGETINTERNALFORMATI64VPROC                       gl3wGetInternalformati64v;
PFNGLGETINTERNALFORMATIVPROC                         gl3wGetInternalformativ;
PFNGLGETMULTISAMPLEFVPROC                            gl3wGetMultisamplefv;
PFNGLGETNAMEDBUFFERPARAMETERI64VPROC                 gl3wGetNamedBufferParameteri64v;
PFNGLGETNAMEDBUFFERPARAMETERIVPROC                   gl3wGetNamedBufferParameteriv;
PFNGLGETNAMEDBUFFERPOINTERVPROC                      gl3wGetNamedBufferPointerv;
PFNGLGETNAMEDBUFFERSUBDATAPROC                       gl3wGetNamedBufferSubData;
PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC    gl3wGetNamedFramebufferAttachmentParameteriv;
PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC              gl3wGetNamedFramebufferParameteriv;
PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC             gl3wGetNamedRenderbufferParameteriv;
PFNGLGETNAMEDSTRINGARBPROC                           gl3wGetNamedStringARB;
PFNGLGETNAMEDSTRINGIVARBPROC                         gl3wGetNamedStringivARB;
PFNGLGETOBJECTLABELPROC                              gl3wGetObjectLabel;
PFNGLGETOBJECTPTRLABELPROC                           gl3wGetObjectPtrLabel;
PFNGLGETPOINTERVPROC                                 gl3wGetPointerv;
PFNGLGETPROGRAMBINARYPROC                            gl3wGetProgramBinary;
PFNGLGETPROGRAMINFOLOGPROC                           gl3wGetProgramInfoLog;
PFNGLGETPROGRAMINTERFACEIVPROC                       gl3wGetProgramInterfaceiv;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC                   gl3wGetProgramPipelineInfoLog;
PFNGLGETPROGRAMPIPELINEIVPROC                        gl3wGetProgramPipelineiv;
PFNGLGETPROGRAMRESOURCEINDEXPROC                     gl3wGetProgramResourceIndex;
PFNGLGETPROGRAMRESOURCELOCATIONPROC                  gl3wGetProgramResourceLocation;
PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC             gl3wGetProgramResourceLocationIndex;
PFNGLGETPROGRAMRESOURCENAMEPROC                      gl3wGetProgramResourceName;
PFNGLGETPROGRAMRESOURCEIVPROC                        gl3wGetProgramResourceiv;
PFNGLGETPROGRAMSTAGEIVPROC                           gl3wGetProgramStageiv;
PFNGLGETPROGRAMIVPROC                                gl3wGetProgramiv;
PFNGLGETQUERYBUFFEROBJECTI64VPROC                    gl3wGetQueryBufferObjecti64v;
PFNGLGETQUERYBUFFEROBJECTIVPROC                      gl3wGetQueryBufferObjectiv;
PFNGLGETQUERYBUFFEROBJECTUI64VPROC                   gl3wGetQueryBufferObjectui64v;
PFNGLGETQUERYBUFFEROBJECTUIVPROC                     gl3wGetQueryBufferObjectuiv;
PFNGLGETQUERYINDEXEDIVPROC                           gl3wGetQueryIndexediv;
PFNGLGETQUERYOBJECTI64VPROC                          gl3wGetQueryObjecti64v;
PFNGLGETQUERYOBJECTIVPROC                            gl3wGetQueryObjectiv;
PFNGLGETQUERYOBJECTUI64VPROC                         gl3wGetQueryObjectui64v;
PFNGLGETQUERYOBJECTUIVPROC                           gl3wGetQueryObjectuiv;
PFNGLGETQUERYIVPROC                                  gl3wGetQueryiv;
PFNGLGETRENDERBUFFERPARAMETERIVPROC                  gl3wGetRenderbufferParameteriv;
PFNGLGETSAMPLERPARAMETERIIVPROC                      gl3wGetSamplerParameterIiv;
PFNGLGETSAMPLERPARAMETERIUIVPROC                     gl3wGetSamplerParameterIuiv;
PFNGLGETSAMPLERPARAMETERFVPROC                       gl3wGetSamplerParameterfv;
PFNGLGETSAMPLERPARAMETERIVPROC                       gl3wGetSamplerParameteriv;
PFNGLGETSHADERINFOLOGPROC                            gl3wGetShaderInfoLog;
PFNGLGETSHADERPRECISIONFORMATPROC                    gl3wGetShaderPrecisionFormat;
PFNGLGETSHADERSOURCEPROC                             gl3wGetShaderSource;
PFNGLGETSHADERIVPROC                                 gl3wGetShaderiv;
PFNGLGETSTRINGPROC                                   gl3wGetString;
PFNGLGETSTRINGIPROC                                  gl3wGetStringi;
PFNGLGETSUBROUTINEINDEXPROC                          gl3wGetSubroutineIndex;
PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC                gl3wGetSubroutineUniformLocation;
PFNGLGETSYNCIVPROC                                   gl3wGetSynciv;
PFNGLGETTEXIMAGEPROC                                 gl3wGetTexImage;
PFNGLGETTEXLEVELPARAMETERFVPROC                      gl3wGetTexLevelParameterfv;
PFNGLGETTEXLEVELPARAMETERIVPROC                      gl3wGetTexLevelParameteriv;
PFNGLGETTEXPARAMETERIIVPROC                          gl3wGetTexParameterIiv;
PFNGLGETTEXPARAMETERIUIVPROC                         gl3wGetTexParameterIuiv;
PFNGLGETTEXPARAMETERFVPROC                           gl3wGetTexParameterfv;
PFNGLGETTEXPARAMETERIVPROC                           gl3wGetTexParameteriv;
PFNGLGETTEXTUREHANDLEARBPROC                         gl3wGetTextureHandleARB;
PFNGLGETTEXTUREIMAGEPROC                             gl3wGetTextureImage;
PFNGLGETTEXTURELEVELPARAMETERFVPROC                  gl3wGetTextureLevelParameterfv;
PFNGLGETTEXTURELEVELPARAMETERIVPROC                  gl3wGetTextureLevelParameteriv;
PFNGLGETTEXTUREPARAMETERIIVPROC                      gl3wGetTextureParameterIiv;
PFNGLGETTEXTUREPARAMETERIUIVPROC                     gl3wGetTextureParameterIuiv;
PFNGLGETTEXTUREPARAMETERFVPROC                       gl3wGetTextureParameterfv;
PFNGLGETTEXTUREPARAMETERIVPROC                       gl3wGetTextureParameteriv;
PFNGLGETTEXTURESAMPLERHANDLEARBPROC                  gl3wGetTextureSamplerHandleARB;
PFNGLGETTEXTURESUBIMAGEPROC                          gl3wGetTextureSubImage;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC                 gl3wGetTransformFeedbackVarying;
PFNGLGETTRANSFORMFEEDBACKI64_VPROC                   gl3wGetTransformFeedbacki64_v;
PFNGLGETTRANSFORMFEEDBACKI_VPROC                     gl3wGetTransformFeedbacki_v;
PFNGLGETTRANSFORMFEEDBACKIVPROC                      gl3wGetTransformFeedbackiv;
PFNGLGETUNIFORMBLOCKINDEXPROC                        gl3wGetUniformBlockIndex;
PFNGLGETUNIFORMINDICESPROC                           gl3wGetUniformIndices;
PFNGLGETUNIFORMLOCATIONPROC                          gl3wGetUniformLocation;
PFNGLGETUNIFORMSUBROUTINEUIVPROC                     gl3wGetUniformSubroutineuiv;
PFNGLGETUNIFORMDVPROC                                gl3wGetUniformdv;
PFNGLGETUNIFORMFVPROC                                gl3wGetUniformfv;
PFNGLGETUNIFORMIVPROC                                gl3wGetUniformiv;
PFNGLGETUNIFORMUIVPROC                               gl3wGetUniformuiv;
PFNGLGETVERTEXARRAYINDEXED64IVPROC                   gl3wGetVertexArrayIndexed64iv;
PFNGLGETVERTEXARRAYINDEXEDIVPROC                     gl3wGetVertexArrayIndexediv;
PFNGLGETVERTEXARRAYIVPROC                            gl3wGetVertexArrayiv;
PFNGLGETVERTEXATTRIBIIVPROC                          gl3wGetVertexAttribIiv;
PFNGLGETVERTEXATTRIBIUIVPROC                         gl3wGetVertexAttribIuiv;
PFNGLGETVERTEXATTRIBLDVPROC                          gl3wGetVertexAttribLdv;
PFNGLGETVERTEXATTRIBLUI64VARBPROC                    gl3wGetVertexAttribLui64vARB;
PFNGLGETVERTEXATTRIBPOINTERVPROC                     gl3wGetVertexAttribPointerv;
PFNGLGETVERTEXATTRIBDVPROC                           gl3wGetVertexAttribdv;
PFNGLGETVERTEXATTRIBFVPROC                           gl3wGetVertexAttribfv;
PFNGLGETVERTEXATTRIBIVPROC                           gl3wGetVertexAttribiv;
PFNGLGETNCOMPRESSEDTEXIMAGEPROC                      gl3wGetnCompressedTexImage;
PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC                   gl3wGetnCompressedTexImageARB;
PFNGLGETNTEXIMAGEPROC                                gl3wGetnTexImage;
PFNGLGETNTEXIMAGEARBPROC                             gl3wGetnTexImageARB;
PFNGLGETNUNIFORMDVPROC                               gl3wGetnUniformdv;
PFNGLGETNUNIFORMDVARBPROC                            gl3wGetnUniformdvARB;
PFNGLGETNUNIFORMFVPROC                               gl3wGetnUniformfv;
PFNGLGETNUNIFORMFVARBPROC                            gl3wGetnUniformfvARB;
PFNGLGETNUNIFORMIVPROC                               gl3wGetnUniformiv;
PFNGLGETNUNIFORMIVARBPROC                            gl3wGetnUniformivARB;
PFNGLGETNUNIFORMUIVPROC                              gl3wGetnUniformuiv;
PFNGLGETNUNIFORMUIVARBPROC                           gl3wGetnUniformuivARB;
PFNGLHINTPROC                                        gl3wHint;
PFNGLINVALIDATEBUFFERDATAPROC                        gl3wInvalidateBufferData;
PFNGLINVALIDATEBUFFERSUBDATAPROC                     gl3wInvalidateBufferSubData;
PFNGLINVALIDATEFRAMEBUFFERPROC                       gl3wInvalidateFramebuffer;
PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC              gl3wInvalidateNamedFramebufferData;
PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC           gl3wInvalidateNamedFramebufferSubData;
PFNGLINVALIDATESUBFRAMEBUFFERPROC                    gl3wInvalidateSubFramebuffer;
PFNGLINVALIDATETEXIMAGEPROC                          gl3wInvalidateTexImage;
PFNGLINVALIDATETEXSUBIMAGEPROC                       gl3wInvalidateTexSubImage;
PFNGLISBUFFERPROC                                    gl3wIsBuffer;
PFNGLISENABLEDPROC                                   gl3wIsEnabled;
PFNGLISENABLEDIPROC                                  gl3wIsEnabledi;
PFNGLISFRAMEBUFFERPROC                               gl3wIsFramebuffer;
PFNGLISIMAGEHANDLERESIDENTARBPROC                    gl3wIsImageHandleResidentARB;
PFNGLISNAMEDSTRINGARBPROC                            gl3wIsNamedStringARB;
PFNGLISPROGRAMPROC                                   gl3wIsProgram;
PFNGLISPROGRAMPIPELINEPROC                           gl3wIsProgramPipeline;
PFNGLISQUERYPROC                                     gl3wIsQuery;
PFNGLISRENDERBUFFERPROC                              gl3wIsRenderbuffer;
PFNGLISSAMPLERPROC                                   gl3wIsSampler;
PFNGLISSHADERPROC                                    gl3wIsShader;
PFNGLISSYNCPROC                                      gl3wIsSync;
PFNGLISTEXTUREPROC                                   gl3wIsTexture;
PFNGLISTEXTUREHANDLERESIDENTARBPROC                  gl3wIsTextureHandleResidentARB;
PFNGLISTRANSFORMFEEDBACKPROC                         gl3wIsTransformFeedback;
PFNGLISVERTEXARRAYPROC                               gl3wIsVertexArray;
PFNGLLINEWIDTHPROC                                   gl3wLineWidth;
PFNGLLINKPROGRAMPROC                                 gl3wLinkProgram;
PFNGLLOGICOPPROC                                     gl3wLogicOp;
PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC               gl3wMakeImageHandleNonResidentARB;
PFNGLMAKEIMAGEHANDLERESIDENTARBPROC                  gl3wMakeImageHandleResidentARB;
PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC             gl3wMakeTextureHandleNonResidentARB;
PFNGLMAKETEXTUREHANDLERESIDENTARBPROC                gl3wMakeTextureHandleResidentARB;
PFNGLMAPBUFFERPROC                                   gl3wMapBuffer;
PFNGLMAPBUFFERRANGEPROC                              gl3wMapBufferRange;
PFNGLMAPNAMEDBUFFERPROC                              gl3wMapNamedBuffer;
PFNGLMAPNAMEDBUFFERRANGEPROC                         gl3wMapNamedBufferRange;
PFNGLMEMORYBARRIERPROC                               gl3wMemoryBarrier;
PFNGLMEMORYBARRIERBYREGIONPROC                       gl3wMemoryBarrierByRegion;
PFNGLMINSAMPLESHADINGPROC                            gl3wMinSampleShading;
PFNGLMINSAMPLESHADINGARBPROC                         gl3wMinSampleShadingARB;
PFNGLMULTIDRAWARRAYSPROC                             gl3wMultiDrawArrays;
PFNGLMULTIDRAWARRAYSINDIRECTPROC                     gl3wMultiDrawArraysIndirect;
PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC             gl3wMultiDrawArraysIndirectCountARB;
PFNGLMULTIDRAWELEMENTSPROC                           gl3wMultiDrawElements;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC                 gl3wMultiDrawElementsBaseVertex;
PFNGLMULTIDRAWELEMENTSINDIRECTPROC                   gl3wMultiDrawElementsIndirect;
PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC           gl3wMultiDrawElementsIndirectCountARB;
PFNGLNAMEDBUFFERDATAPROC                             gl3wNamedBufferData;
PFNGLNAMEDBUFFERPAGECOMMITMENTARBPROC                gl3wNamedBufferPageCommitmentARB;
PFNGLNAMEDBUFFERPAGECOMMITMENTEXTPROC                gl3wNamedBufferPageCommitmentEXT;
PFNGLNAMEDBUFFERSTORAGEPROC                          gl3wNamedBufferStorage;
PFNGLNAMEDBUFFERSUBDATAPROC                          gl3wNamedBufferSubData;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC                  gl3wNamedFramebufferDrawBuffer;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC                 gl3wNamedFramebufferDrawBuffers;
PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC                  gl3wNamedFramebufferParameteri;
PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC                  gl3wNamedFramebufferReadBuffer;
PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC                gl3wNamedFramebufferRenderbuffer;
PFNGLNAMEDFRAMEBUFFERTEXTUREPROC                     gl3wNamedFramebufferTexture;
PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC                gl3wNamedFramebufferTextureLayer;
PFNGLNAMEDRENDERBUFFERSTORAGEPROC                    gl3wNamedRenderbufferStorage;
PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC         gl3wNamedRenderbufferStorageMultisample;
PFNGLNAMEDSTRINGARBPROC                              gl3wNamedStringARB;
PFNGLOBJECTLABELPROC                                 gl3wObjectLabel;
PFNGLOBJECTPTRLABELPROC                              gl3wObjectPtrLabel;
PFNGLPATCHPARAMETERFVPROC                            gl3wPatchParameterfv;
PFNGLPATCHPARAMETERIPROC                             gl3wPatchParameteri;
PFNGLPAUSETRANSFORMFEEDBACKPROC                      gl3wPauseTransformFeedback;
PFNGLPIXELSTOREFPROC                                 gl3wPixelStoref;
PFNGLPIXELSTOREIPROC                                 gl3wPixelStorei;
PFNGLPOINTPARAMETERFPROC                             gl3wPointParameterf;
PFNGLPOINTPARAMETERFVPROC                            gl3wPointParameterfv;
PFNGLPOINTPARAMETERIPROC                             gl3wPointParameteri;
PFNGLPOINTPARAMETERIVPROC                            gl3wPointParameteriv;
PFNGLPOINTSIZEPROC                                   gl3wPointSize;
PFNGLPOLYGONMODEPROC                                 gl3wPolygonMode;
PFNGLPOLYGONOFFSETPROC                               gl3wPolygonOffset;
PFNGLPOPDEBUGGROUPPROC                               gl3wPopDebugGroup;
PFNGLPRIMITIVERESTARTINDEXPROC                       gl3wPrimitiveRestartIndex;
PFNGLPROGRAMBINARYPROC                               gl3wProgramBinary;
PFNGLPROGRAMPARAMETERIPROC                           gl3wProgramParameteri;
PFNGLPROGRAMUNIFORM1DPROC                            gl3wProgramUniform1d;
PFNGLPROGRAMUNIFORM1DVPROC                           gl3wProgramUniform1dv;
PFNGLPROGRAMUNIFORM1FPROC                            gl3wProgramUniform1f;
PFNGLPROGRAMUNIFORM1FVPROC                           gl3wProgramUniform1fv;
PFNGLPROGRAMUNIFORM1IPROC                            gl3wProgramUniform1i;
PFNGLPROGRAMUNIFORM1IVPROC                           gl3wProgramUniform1iv;
PFNGLPROGRAMUNIFORM1UIPROC                           gl3wProgramUniform1ui;
PFNGLPROGRAMUNIFORM1UIVPROC                          gl3wProgramUniform1uiv;
PFNGLPROGRAMUNIFORM2DPROC                            gl3wProgramUniform2d;
PFNGLPROGRAMUNIFORM2DVPROC                           gl3wProgramUniform2dv;
PFNGLPROGRAMUNIFORM2FPROC                            gl3wProgramUniform2f;
PFNGLPROGRAMUNIFORM2FVPROC                           gl3wProgramUniform2fv;
PFNGLPROGRAMUNIFORM2IPROC                            gl3wProgramUniform2i;
PFNGLPROGRAMUNIFORM2IVPROC                           gl3wProgramUniform2iv;
PFNGLPROGRAMUNIFORM2UIPROC                           gl3wProgramUniform2ui;
PFNGLPROGRAMUNIFORM2UIVPROC                          gl3wProgramUniform2uiv;
PFNGLPROGRAMUNIFORM3DPROC                            gl3wProgramUniform3d;
PFNGLPROGRAMUNIFORM3DVPROC                           gl3wProgramUniform3dv;
PFNGLPROGRAMUNIFORM3FPROC                            gl3wProgramUniform3f;
PFNGLPROGRAMUNIFORM3FVPROC                           gl3wProgramUniform3fv;
PFNGLPROGRAMUNIFORM3IPROC                            gl3wProgramUniform3i;
PFNGLPROGRAMUNIFORM3IVPROC                           gl3wProgramUniform3iv;
PFNGLPROGRAMUNIFORM3UIPROC                           gl3wProgramUniform3ui;
PFNGLPROGRAMUNIFORM3UIVPROC                          gl3wProgramUniform3uiv;
PFNGLPROGRAMUNIFORM4DPROC                            gl3wProgramUniform4d;
PFNGLPROGRAMUNIFORM4DVPROC                           gl3wProgramUniform4dv;
PFNGLPROGRAMUNIFORM4FPROC                            gl3wProgramUniform4f;
PFNGLPROGRAMUNIFORM4FVPROC                           gl3wProgramUniform4fv;
PFNGLPROGRAMUNIFORM4IPROC                            gl3wProgramUniform4i;
PFNGLPROGRAMUNIFORM4IVPROC                           gl3wProgramUniform4iv;
PFNGLPROGRAMUNIFORM4UIPROC                           gl3wProgramUniform4ui;
PFNGLPROGRAMUNIFORM4UIVPROC                          gl3wProgramUniform4uiv;
PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC                 gl3wProgramUniformHandleui64ARB;
PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC                gl3wProgramUniformHandleui64vARB;
PFNGLPROGRAMUNIFORMMATRIX2DVPROC                     gl3wProgramUniformMatrix2dv;
PFNGLPROGRAMUNIFORMMATRIX2FVPROC                     gl3wProgramUniformMatrix2fv;
PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC                   gl3wProgramUniformMatrix2x3dv;
PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC                   gl3wProgramUniformMatrix2x3fv;
PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC                   gl3wProgramUniformMatrix2x4dv;
PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC                   gl3wProgramUniformMatrix2x4fv;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC                     gl3wProgramUniformMatrix3dv;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC                     gl3wProgramUniformMatrix3fv;
PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC                   gl3wProgramUniformMatrix3x2dv;
PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC                   gl3wProgramUniformMatrix3x2fv;
PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC                   gl3wProgramUniformMatrix3x4dv;
PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC                   gl3wProgramUniformMatrix3x4fv;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC                     gl3wProgramUniformMatrix4dv;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC                     gl3wProgramUniformMatrix4fv;
PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC                   gl3wProgramUniformMatrix4x2dv;
PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC                   gl3wProgramUniformMatrix4x2fv;
PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC                   gl3wProgramUniformMatrix4x3dv;
PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC                   gl3wProgramUniformMatrix4x3fv;
PFNGLPROVOKINGVERTEXPROC                             gl3wProvokingVertex;
PFNGLPUSHDEBUGGROUPPROC                              gl3wPushDebugGroup;
PFNGLQUERYCOUNTERPROC                                gl3wQueryCounter;
PFNGLREADBUFFERPROC                                  gl3wReadBuffer;
PFNGLREADPIXELSPROC                                  gl3wReadPixels;
PFNGLREADNPIXELSPROC                                 gl3wReadnPixels;
PFNGLREADNPIXELSARBPROC                              gl3wReadnPixelsARB;
PFNGLRELEASESHADERCOMPILERPROC                       gl3wReleaseShaderCompiler;
PFNGLRENDERBUFFERSTORAGEPROC                         gl3wRenderbufferStorage;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC              gl3wRenderbufferStorageMultisample;
PFNGLRESUMETRANSFORMFEEDBACKPROC                     gl3wResumeTransformFeedback;
PFNGLSAMPLECOVERAGEPROC                              gl3wSampleCoverage;
PFNGLSAMPLEMASKIPROC                                 gl3wSampleMaski;
PFNGLSAMPLERPARAMETERIIVPROC                         gl3wSamplerParameterIiv;
PFNGLSAMPLERPARAMETERIUIVPROC                        gl3wSamplerParameterIuiv;
PFNGLSAMPLERPARAMETERFPROC                           gl3wSamplerParameterf;
PFNGLSAMPLERPARAMETERFVPROC                          gl3wSamplerParameterfv;
PFNGLSAMPLERPARAMETERIPROC                           gl3wSamplerParameteri;
PFNGLSAMPLERPARAMETERIVPROC                          gl3wSamplerParameteriv;
PFNGLSCISSORPROC                                     gl3wScissor;
PFNGLSCISSORARRAYVPROC                               gl3wScissorArrayv;
PFNGLSCISSORINDEXEDPROC                              gl3wScissorIndexed;
PFNGLSCISSORINDEXEDVPROC                             gl3wScissorIndexedv;
PFNGLSHADERBINARYPROC                                gl3wShaderBinary;
PFNGLSHADERSOURCEPROC                                gl3wShaderSource;
PFNGLSHADERSTORAGEBLOCKBINDINGPROC                   gl3wShaderStorageBlockBinding;
PFNGLSTENCILFUNCPROC                                 gl3wStencilFunc;
PFNGLSTENCILFUNCSEPARATEPROC                         gl3wStencilFuncSeparate;
PFNGLSTENCILMASKPROC                                 gl3wStencilMask;
PFNGLSTENCILMASKSEPARATEPROC                         gl3wStencilMaskSeparate;
PFNGLSTENCILOPPROC                                   gl3wStencilOp;
PFNGLSTENCILOPSEPARATEPROC                           gl3wStencilOpSeparate;
PFNGLTEXBUFFERPROC                                   gl3wTexBuffer;
PFNGLTEXBUFFERRANGEPROC                              gl3wTexBufferRange;
PFNGLTEXIMAGE1DPROC                                  gl3wTexImage1D;
PFNGLTEXIMAGE2DPROC                                  gl3wTexImage2D;
PFNGLTEXIMAGE2DMULTISAMPLEPROC                       gl3wTexImage2DMultisample;
PFNGLTEXIMAGE3DPROC                                  gl3wTexImage3D;
PFNGLTEXIMAGE3DMULTISAMPLEPROC                       gl3wTexImage3DMultisample;
PFNGLTEXPAGECOMMITMENTARBPROC                        gl3wTexPageCommitmentARB;
PFNGLTEXPARAMETERIIVPROC                             gl3wTexParameterIiv;
PFNGLTEXPARAMETERIUIVPROC                            gl3wTexParameterIuiv;
PFNGLTEXPARAMETERFPROC                               gl3wTexParameterf;
PFNGLTEXPARAMETERFVPROC                              gl3wTexParameterfv;
PFNGLTEXPARAMETERIPROC                               gl3wTexParameteri;
PFNGLTEXPARAMETERIVPROC                              gl3wTexParameteriv;
PFNGLTEXSTORAGE1DPROC                                gl3wTexStorage1D;
PFNGLTEXSTORAGE2DPROC                                gl3wTexStorage2D;
PFNGLTEXSTORAGE2DMULTISAMPLEPROC                     gl3wTexStorage2DMultisample;
PFNGLTEXSTORAGE3DPROC                                gl3wTexStorage3D;
PFNGLTEXSTORAGE3DMULTISAMPLEPROC                     gl3wTexStorage3DMultisample;
PFNGLTEXSUBIMAGE1DPROC                               gl3wTexSubImage1D;
PFNGLTEXSUBIMAGE2DPROC                               gl3wTexSubImage2D;
PFNGLTEXSUBIMAGE3DPROC                               gl3wTexSubImage3D;
PFNGLTEXTUREBARRIERPROC                              gl3wTextureBarrier;
PFNGLTEXTUREBUFFERPROC                               gl3wTextureBuffer;
PFNGLTEXTUREBUFFERRANGEPROC                          gl3wTextureBufferRange;
PFNGLTEXTUREPARAMETERIIVPROC                         gl3wTextureParameterIiv;
PFNGLTEXTUREPARAMETERIUIVPROC                        gl3wTextureParameterIuiv;
PFNGLTEXTUREPARAMETERFPROC                           gl3wTextureParameterf;
PFNGLTEXTUREPARAMETERFVPROC                          gl3wTextureParameterfv;
PFNGLTEXTUREPARAMETERIPROC                           gl3wTextureParameteri;
PFNGLTEXTUREPARAMETERIVPROC                          gl3wTextureParameteriv;
PFNGLTEXTURESTORAGE1DPROC                            gl3wTextureStorage1D;
PFNGLTEXTURESTORAGE2DPROC                            gl3wTextureStorage2D;
PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC                 gl3wTextureStorage2DMultisample;
PFNGLTEXTURESTORAGE3DPROC                            gl3wTextureStorage3D;
PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC                 gl3wTextureStorage3DMultisample;
PFNGLTEXTURESUBIMAGE1DPROC                           gl3wTextureSubImage1D;
PFNGLTEXTURESUBIMAGE2DPROC                           gl3wTextureSubImage2D;
PFNGLTEXTURESUBIMAGE3DPROC                           gl3wTextureSubImage3D;
PFNGLTEXTUREVIEWPROC                                 gl3wTextureView;
PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC                 gl3wTransformFeedbackBufferBase;
PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC                gl3wTransformFeedbackBufferRange;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC                   gl3wTransformFeedbackVaryings;
PFNGLUNIFORM1DPROC                                   gl3wUniform1d;
PFNGLUNIFORM1DVPROC                                  gl3wUniform1dv;
PFNGLUNIFORM1FPROC                                   gl3wUniform1f;
PFNGLUNIFORM1FVPROC                                  gl3wUniform1fv;
PFNGLUNIFORM1IPROC                                   gl3wUniform1i;
PFNGLUNIFORM1IVPROC                                  gl3wUniform1iv;
PFNGLUNIFORM1UIPROC                                  gl3wUniform1ui;
PFNGLUNIFORM1UIVPROC                                 gl3wUniform1uiv;
PFNGLUNIFORM2DPROC                                   gl3wUniform2d;
PFNGLUNIFORM2DVPROC                                  gl3wUniform2dv;
PFNGLUNIFORM2FPROC                                   gl3wUniform2f;
PFNGLUNIFORM2FVPROC                                  gl3wUniform2fv;
PFNGLUNIFORM2IPROC                                   gl3wUniform2i;
PFNGLUNIFORM2IVPROC                                  gl3wUniform2iv;
PFNGLUNIFORM2UIPROC                                  gl3wUniform2ui;
PFNGLUNIFORM2UIVPROC                                 gl3wUniform2uiv;
PFNGLUNIFORM3DPROC                                   gl3wUniform3d;
PFNGLUNIFORM3DVPROC                                  gl3wUniform3dv;
PFNGLUNIFORM3FPROC                                   gl3wUniform3f;
PFNGLUNIFORM3FVPROC                                  gl3wUniform3fv;
PFNGLUNIFORM3IPROC                                   gl3wUniform3i;
PFNGLUNIFORM3IVPROC                                  gl3wUniform3iv;
PFNGLUNIFORM3UIPROC                                  gl3wUniform3ui;
PFNGLUNIFORM3UIVPROC                                 gl3wUniform3uiv;
PFNGLUNIFORM4DPROC                                   gl3wUniform4d;
PFNGLUNIFORM4DVPROC                                  gl3wUniform4dv;
PFNGLUNIFORM4FPROC                                   gl3wUniform4f;
PFNGLUNIFORM4FVPROC                                  gl3wUniform4fv;
PFNGLUNIFORM4IPROC                                   gl3wUniform4i;
PFNGLUNIFORM4IVPROC                                  gl3wUniform4iv;
PFNGLUNIFORM4UIPROC                                  gl3wUniform4ui;
PFNGLUNIFORM4UIVPROC                                 gl3wUniform4uiv;
PFNGLUNIFORMBLOCKBINDINGPROC                         gl3wUniformBlockBinding;
PFNGLUNIFORMHANDLEUI64ARBPROC                        gl3wUniformHandleui64ARB;
PFNGLUNIFORMHANDLEUI64VARBPROC                       gl3wUniformHandleui64vARB;
PFNGLUNIFORMMATRIX2DVPROC                            gl3wUniformMatrix2dv;
PFNGLUNIFORMMATRIX2FVPROC                            gl3wUniformMatrix2fv;
PFNGLUNIFORMMATRIX2X3DVPROC                          gl3wUniformMatrix2x3dv;
PFNGLUNIFORMMATRIX2X3FVPROC                          gl3wUniformMatrix2x3fv;
PFNGLUNIFORMMATRIX2X4DVPROC                          gl3wUniformMatrix2x4dv;
PFNGLUNIFORMMATRIX2X4FVPROC                          gl3wUniformMatrix2x4fv;
PFNGLUNIFORMMATRIX3DVPROC                            gl3wUniformMatrix3dv;
PFNGLUNIFORMMATRIX3FVPROC                            gl3wUniformMatrix3fv;
PFNGLUNIFORMMATRIX3X2DVPROC                          gl3wUniformMatrix3x2dv;
PFNGLUNIFORMMATRIX3X2FVPROC                          gl3wUniformMatrix3x2fv;
PFNGLUNIFORMMATRIX3X4DVPROC                          gl3wUniformMatrix3x4dv;
PFNGLUNIFORMMATRIX3X4FVPROC                          gl3wUniformMatrix3x4fv;
PFNGLUNIFORMMATRIX4DVPROC                            gl3wUniformMatrix4dv;
PFNGLUNIFORMMATRIX4FVPROC                            gl3wUniformMatrix4fv;
PFNGLUNIFORMMATRIX4X2DVPROC                          gl3wUniformMatrix4x2dv;
PFNGLUNIFORMMATRIX4X2FVPROC                          gl3wUniformMatrix4x2fv;
PFNGLUNIFORMMATRIX4X3DVPROC                          gl3wUniformMatrix4x3dv;
PFNGLUNIFORMMATRIX4X3FVPROC                          gl3wUniformMatrix4x3fv;
PFNGLUNIFORMSUBROUTINESUIVPROC                       gl3wUniformSubroutinesuiv;
PFNGLUNMAPBUFFERPROC                                 gl3wUnmapBuffer;
PFNGLUNMAPNAMEDBUFFERPROC                            gl3wUnmapNamedBuffer;
PFNGLUSEPROGRAMPROC                                  gl3wUseProgram;
PFNGLUSEPROGRAMSTAGESPROC                            gl3wUseProgramStages;
PFNGLVALIDATEPROGRAMPROC                             gl3wValidateProgram;
PFNGLVALIDATEPROGRAMPIPELINEPROC                     gl3wValidateProgramPipeline;
PFNGLVERTEXARRAYATTRIBBINDINGPROC                    gl3wVertexArrayAttribBinding;
PFNGLVERTEXARRAYATTRIBFORMATPROC                     gl3wVertexArrayAttribFormat;
PFNGLVERTEXARRAYATTRIBIFORMATPROC                    gl3wVertexArrayAttribIFormat;
PFNGLVERTEXARRAYATTRIBLFORMATPROC                    gl3wVertexArrayAttribLFormat;
PFNGLVERTEXARRAYBINDINGDIVISORPROC                   gl3wVertexArrayBindingDivisor;
PFNGLVERTEXARRAYELEMENTBUFFERPROC                    gl3wVertexArrayElementBuffer;
PFNGLVERTEXARRAYVERTEXBUFFERPROC                     gl3wVertexArrayVertexBuffer;
PFNGLVERTEXARRAYVERTEXBUFFERSPROC                    gl3wVertexArrayVertexBuffers;
PFNGLVERTEXATTRIB1DPROC                              gl3wVertexAttrib1d;
PFNGLVERTEXATTRIB1DVPROC                             gl3wVertexAttrib1dv;
PFNGLVERTEXATTRIB1FPROC                              gl3wVertexAttrib1f;
PFNGLVERTEXATTRIB1FVPROC                             gl3wVertexAttrib1fv;
PFNGLVERTEXATTRIB1SPROC                              gl3wVertexAttrib1s;
PFNGLVERTEXATTRIB1SVPROC                             gl3wVertexAttrib1sv;
PFNGLVERTEXATTRIB2DPROC                              gl3wVertexAttrib2d;
PFNGLVERTEXATTRIB2DVPROC                             gl3wVertexAttrib2dv;
PFNGLVERTEXATTRIB2FPROC                              gl3wVertexAttrib2f;
PFNGLVERTEXATTRIB2FVPROC                             gl3wVertexAttrib2fv;
PFNGLVERTEXATTRIB2SPROC                              gl3wVertexAttrib2s;
PFNGLVERTEXATTRIB2SVPROC                             gl3wVertexAttrib2sv;
PFNGLVERTEXATTRIB3DPROC                              gl3wVertexAttrib3d;
PFNGLVERTEXATTRIB3DVPROC                             gl3wVertexAttrib3dv;
PFNGLVERTEXATTRIB3FPROC                              gl3wVertexAttrib3f;
PFNGLVERTEXATTRIB3FVPROC                             gl3wVertexAttrib3fv;
PFNGLVERTEXATTRIB3SPROC                              gl3wVertexAttrib3s;
PFNGLVERTEXATTRIB3SVPROC                             gl3wVertexAttrib3sv;
PFNGLVERTEXATTRIB4NBVPROC                            gl3wVertexAttrib4Nbv;
PFNGLVERTEXATTRIB4NIVPROC                            gl3wVertexAttrib4Niv;
PFNGLVERTEXATTRIB4NSVPROC                            gl3wVertexAttrib4Nsv;
PFNGLVERTEXATTRIB4NUBPROC                            gl3wVertexAttrib4Nub;
PFNGLVERTEXATTRIB4NUBVPROC                           gl3wVertexAttrib4Nubv;
PFNGLVERTEXATTRIB4NUIVPROC                           gl3wVertexAttrib4Nuiv;
PFNGLVERTEXATTRIB4NUSVPROC                           gl3wVertexAttrib4Nusv;
PFNGLVERTEXATTRIB4BVPROC                             gl3wVertexAttrib4bv;
PFNGLVERTEXATTRIB4DPROC                              gl3wVertexAttrib4d;
PFNGLVERTEXATTRIB4DVPROC                             gl3wVertexAttrib4dv;
PFNGLVERTEXATTRIB4FPROC                              gl3wVertexAttrib4f;
PFNGLVERTEXATTRIB4FVPROC                             gl3wVertexAttrib4fv;
PFNGLVERTEXATTRIB4IVPROC                             gl3wVertexAttrib4iv;
PFNGLVERTEXATTRIB4SPROC                              gl3wVertexAttrib4s;
PFNGLVERTEXATTRIB4SVPROC                             gl3wVertexAttrib4sv;
PFNGLVERTEXATTRIB4UBVPROC                            gl3wVertexAttrib4ubv;
PFNGLVERTEXATTRIB4UIVPROC                            gl3wVertexAttrib4uiv;
PFNGLVERTEXATTRIB4USVPROC                            gl3wVertexAttrib4usv;
PFNGLVERTEXATTRIBBINDINGPROC                         gl3wVertexAttribBinding;
PFNGLVERTEXATTRIBDIVISORPROC                         gl3wVertexAttribDivisor;
PFNGLVERTEXATTRIBFORMATPROC                          gl3wVertexAttribFormat;
PFNGLVERTEXATTRIBI1IPROC                             gl3wVertexAttribI1i;
PFNGLVERTEXATTRIBI1IVPROC                            gl3wVertexAttribI1iv;
PFNGLVERTEXATTRIBI1UIPROC                            gl3wVertexAttribI1ui;
PFNGLVERTEXATTRIBI1UIVPROC                           gl3wVertexAttribI1uiv;
PFNGLVERTEXATTRIBI2IPROC                             gl3wVertexAttribI2i;
PFNGLVERTEXATTRIBI2IVPROC                            gl3wVertexAttribI2iv;
PFNGLVERTEXATTRIBI2UIPROC                            gl3wVertexAttribI2ui;
PFNGLVERTEXATTRIBI2UIVPROC                           gl3wVertexAttribI2uiv;
PFNGLVERTEXATTRIBI3IPROC                             gl3wVertexAttribI3i;
PFNGLVERTEXATTRIBI3IVPROC                            gl3wVertexAttribI3iv;
PFNGLVERTEXATTRIBI3UIPROC                            gl3wVertexAttribI3ui;
PFNGLVERTEXATTRIBI3UIVPROC                           gl3wVertexAttribI3uiv;
PFNGLVERTEXATTRIBI4BVPROC                            gl3wVertexAttribI4bv;
PFNGLVERTEXATTRIBI4IPROC                             gl3wVertexAttribI4i;
PFNGLVERTEXATTRIBI4IVPROC                            gl3wVertexAttribI4iv;
PFNGLVERTEXATTRIBI4SVPROC                            gl3wVertexAttribI4sv;
PFNGLVERTEXATTRIBI4UBVPROC                           gl3wVertexAttribI4ubv;
PFNGLVERTEXATTRIBI4UIPROC                            gl3wVertexAttribI4ui;
PFNGLVERTEXATTRIBI4UIVPROC                           gl3wVertexAttribI4uiv;
PFNGLVERTEXATTRIBI4USVPROC                           gl3wVertexAttribI4usv;
PFNGLVERTEXATTRIBIFORMATPROC                         gl3wVertexAttribIFormat;
PFNGLVERTEXATTRIBIPOINTERPROC                        gl3wVertexAttribIPointer;
PFNGLVERTEXATTRIBL1DPROC                             gl3wVertexAttribL1d;
PFNGLVERTEXATTRIBL1DVPROC                            gl3wVertexAttribL1dv;
PFNGLVERTEXATTRIBL1UI64ARBPROC                       gl3wVertexAttribL1ui64ARB;
PFNGLVERTEXATTRIBL1UI64VARBPROC                      gl3wVertexAttribL1ui64vARB;
PFNGLVERTEXATTRIBL2DPROC                             gl3wVertexAttribL2d;
PFNGLVERTEXATTRIBL2DVPROC                            gl3wVertexAttribL2dv;
PFNGLVERTEXATTRIBL3DPROC                             gl3wVertexAttribL3d;
PFNGLVERTEXATTRIBL3DVPROC                            gl3wVertexAttribL3dv;
PFNGLVERTEXATTRIBL4DPROC                             gl3wVertexAttribL4d;
PFNGLVERTEXATTRIBL4DVPROC                            gl3wVertexAttribL4dv;
PFNGLVERTEXATTRIBLFORMATPROC                         gl3wVertexAttribLFormat;
PFNGLVERTEXATTRIBLPOINTERPROC                        gl3wVertexAttribLPointer;
PFNGLVERTEXATTRIBP1UIPROC                            gl3wVertexAttribP1ui;
PFNGLVERTEXATTRIBP1UIVPROC                           gl3wVertexAttribP1uiv;
PFNGLVERTEXATTRIBP2UIPROC                            gl3wVertexAttribP2ui;
PFNGLVERTEXATTRIBP2UIVPROC                           gl3wVertexAttribP2uiv;
PFNGLVERTEXATTRIBP3UIPROC                            gl3wVertexAttribP3ui;
PFNGLVERTEXATTRIBP3UIVPROC                           gl3wVertexAttribP3uiv;
PFNGLVERTEXATTRIBP4UIPROC                            gl3wVertexAttribP4ui;
PFNGLVERTEXATTRIBP4UIVPROC                           gl3wVertexAttribP4uiv;
PFNGLVERTEXATTRIBPOINTERPROC                         gl3wVertexAttribPointer;
PFNGLVERTEXBINDINGDIVISORPROC                        gl3wVertexBindingDivisor;
PFNGLVIEWPORTPROC                                    gl3wViewport;
PFNGLVIEWPORTARRAYVPROC                              gl3wViewportArrayv;
PFNGLVIEWPORTINDEXEDFPROC                            gl3wViewportIndexedf;
PFNGLVIEWPORTINDEXEDFVPROC                           gl3wViewportIndexedfv;
PFNGLWAITSYNCPROC                                    gl3wWaitSync;

static void gl3w__load_procs(void)
{
	gl3wActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC) gl3w__get_proc("glActiveShaderProgram");
	gl3wActiveTexture = (PFNGLACTIVETEXTUREPROC) gl3w__get_proc("glActiveTexture");
	gl3wAttachShader = (PFNGLATTACHSHADERPROC) gl3w__get_proc("glAttachShader");
	gl3wBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC) gl3w__get_proc("glBeginConditionalRender");
	gl3wBeginQuery = (PFNGLBEGINQUERYPROC) gl3w__get_proc("glBeginQuery");
	gl3wBeginQueryIndexed = (PFNGLBEGINQUERYINDEXEDPROC) gl3w__get_proc("glBeginQueryIndexed");
	gl3wBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC) gl3w__get_proc("glBeginTransformFeedback");
	gl3wBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) gl3w__get_proc("glBindAttribLocation");
	gl3wBindBuffer = (PFNGLBINDBUFFERPROC) gl3w__get_proc("glBindBuffer");
	gl3wBindBufferBase = (PFNGLBINDBUFFERBASEPROC) gl3w__get_proc("glBindBufferBase");
	gl3wBindBufferRange = (PFNGLBINDBUFFERRANGEPROC) gl3w__get_proc("glBindBufferRange");
	gl3wBindBuffersBase = (PFNGLBINDBUFFERSBASEPROC) gl3w__get_proc("glBindBuffersBase");
	gl3wBindBuffersRange = (PFNGLBINDBUFFERSRANGEPROC) gl3w__get_proc("glBindBuffersRange");
	gl3wBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC) gl3w__get_proc("glBindFragDataLocation");
	gl3wBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC) gl3w__get_proc("glBindFragDataLocationIndexed");
	gl3wBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) gl3w__get_proc("glBindFramebuffer");
	gl3wBindImageTexture = (PFNGLBINDIMAGETEXTUREPROC) gl3w__get_proc("glBindImageTexture");
	gl3wBindImageTextures = (PFNGLBINDIMAGETEXTURESPROC) gl3w__get_proc("glBindImageTextures");
	gl3wBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC) gl3w__get_proc("glBindProgramPipeline");
	gl3wBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) gl3w__get_proc("glBindRenderbuffer");
	gl3wBindSampler = (PFNGLBINDSAMPLERPROC) gl3w__get_proc("glBindSampler");
	gl3wBindSamplers = (PFNGLBINDSAMPLERSPROC) gl3w__get_proc("glBindSamplers");
	gl3wBindTexture = (PFNGLBINDTEXTUREPROC) gl3w__get_proc("glBindTexture");
	gl3wBindTextureUnit = (PFNGLBINDTEXTUREUNITPROC) gl3w__get_proc("glBindTextureUnit");
	gl3wBindTextures = (PFNGLBINDTEXTURESPROC) gl3w__get_proc("glBindTextures");
	gl3wBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC) gl3w__get_proc("glBindTransformFeedback");
	gl3wBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) gl3w__get_proc("glBindVertexArray");
	gl3wBindVertexBuffer = (PFNGLBINDVERTEXBUFFERPROC) gl3w__get_proc("glBindVertexBuffer");
	gl3wBindVertexBuffers = (PFNGLBINDVERTEXBUFFERSPROC) gl3w__get_proc("glBindVertexBuffers");
	gl3wBlendColor = (PFNGLBLENDCOLORPROC) gl3w__get_proc("glBlendColor");
	gl3wBlendEquation = (PFNGLBLENDEQUATIONPROC) gl3w__get_proc("glBlendEquation");
	gl3wBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC) gl3w__get_proc("glBlendEquationSeparate");
	gl3wBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC) gl3w__get_proc("glBlendEquationSeparatei");
	gl3wBlendEquationSeparateiARB = (PFNGLBLENDEQUATIONSEPARATEIARBPROC) gl3w__get_proc("glBlendEquationSeparateiARB");
	gl3wBlendEquationi = (PFNGLBLENDEQUATIONIPROC) gl3w__get_proc("glBlendEquationi");
	gl3wBlendEquationiARB = (PFNGLBLENDEQUATIONIARBPROC) gl3w__get_proc("glBlendEquationiARB");
	gl3wBlendFunc = (PFNGLBLENDFUNCPROC) gl3w__get_proc("glBlendFunc");
	gl3wBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC) gl3w__get_proc("glBlendFuncSeparate");
	gl3wBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC) gl3w__get_proc("glBlendFuncSeparatei");
	gl3wBlendFuncSeparateiARB = (PFNGLBLENDFUNCSEPARATEIARBPROC) gl3w__get_proc("glBlendFuncSeparateiARB");
	gl3wBlendFunci = (PFNGLBLENDFUNCIPROC) gl3w__get_proc("glBlendFunci");
	gl3wBlendFunciARB = (PFNGLBLENDFUNCIARBPROC) gl3w__get_proc("glBlendFunciARB");
	gl3wBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC) gl3w__get_proc("glBlitFramebuffer");
	gl3wBlitNamedFramebuffer = (PFNGLBLITNAMEDFRAMEBUFFERPROC) gl3w__get_proc("glBlitNamedFramebuffer");
	gl3wBufferData = (PFNGLBUFFERDATAPROC) gl3w__get_proc("glBufferData");
	gl3wBufferPageCommitmentARB = (PFNGLBUFFERPAGECOMMITMENTARBPROC) gl3w__get_proc("glBufferPageCommitmentARB");
	gl3wBufferStorage = (PFNGLBUFFERSTORAGEPROC) gl3w__get_proc("glBufferStorage");
	gl3wBufferSubData = (PFNGLBUFFERSUBDATAPROC) gl3w__get_proc("glBufferSubData");
	gl3wCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) gl3w__get_proc("glCheckFramebufferStatus");
	gl3wCheckNamedFramebufferStatus = (PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC) gl3w__get_proc("glCheckNamedFramebufferStatus");
	gl3wClampColor = (PFNGLCLAMPCOLORPROC) gl3w__get_proc("glClampColor");
	gl3wClear = (PFNGLCLEARPROC) gl3w__get_proc("glClear");
	gl3wClearBufferData = (PFNGLCLEARBUFFERDATAPROC) gl3w__get_proc("glClearBufferData");
	gl3wClearBufferSubData = (PFNGLCLEARBUFFERSUBDATAPROC) gl3w__get_proc("glClearBufferSubData");
	gl3wClearBufferfi = (PFNGLCLEARBUFFERFIPROC) gl3w__get_proc("glClearBufferfi");
	gl3wClearBufferfv = (PFNGLCLEARBUFFERFVPROC) gl3w__get_proc("glClearBufferfv");
	gl3wClearBufferiv = (PFNGLCLEARBUFFERIVPROC) gl3w__get_proc("glClearBufferiv");
	gl3wClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC) gl3w__get_proc("glClearBufferuiv");
	gl3wClearColor = (PFNGLCLEARCOLORPROC) gl3w__get_proc("glClearColor");
	gl3wClearDepth = (PFNGLCLEARDEPTHPROC) gl3w__get_proc("glClearDepth");
	gl3wClearDepthf = (PFNGLCLEARDEPTHFPROC) gl3w__get_proc("glClearDepthf");
	gl3wClearNamedBufferData = (PFNGLCLEARNAMEDBUFFERDATAPROC) gl3w__get_proc("glClearNamedBufferData");
	gl3wClearNamedBufferSubData = (PFNGLCLEARNAMEDBUFFERSUBDATAPROC) gl3w__get_proc("glClearNamedBufferSubData");
	gl3wClearNamedFramebufferfi = (PFNGLCLEARNAMEDFRAMEBUFFERFIPROC) gl3w__get_proc("glClearNamedFramebufferfi");
	gl3wClearNamedFramebufferfv = (PFNGLCLEARNAMEDFRAMEBUFFERFVPROC) gl3w__get_proc("glClearNamedFramebufferfv");
	gl3wClearNamedFramebufferiv = (PFNGLCLEARNAMEDFRAMEBUFFERIVPROC) gl3w__get_proc("glClearNamedFramebufferiv");
	gl3wClearNamedFramebufferuiv = (PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC) gl3w__get_proc("glClearNamedFramebufferuiv");
	gl3wClearStencil = (PFNGLCLEARSTENCILPROC) gl3w__get_proc("glClearStencil");
	gl3wClearTexImage = (PFNGLCLEARTEXIMAGEPROC) gl3w__get_proc("glClearTexImage");
	gl3wClearTexSubImage = (PFNGLCLEARTEXSUBIMAGEPROC) gl3w__get_proc("glClearTexSubImage");
	gl3wClientWaitSync = (PFNGLCLIENTWAITSYNCPROC) gl3w__get_proc("glClientWaitSync");
	gl3wClipControl = (PFNGLCLIPCONTROLPROC) gl3w__get_proc("glClipControl");
	gl3wColorMask = (PFNGLCOLORMASKPROC) gl3w__get_proc("glColorMask");
	gl3wColorMaski = (PFNGLCOLORMASKIPROC) gl3w__get_proc("glColorMaski");
	gl3wCompileShader = (PFNGLCOMPILESHADERPROC) gl3w__get_proc("glCompileShader");
	gl3wCompileShaderIncludeARB = (PFNGLCOMPILESHADERINCLUDEARBPROC) gl3w__get_proc("glCompileShaderIncludeARB");
	gl3wCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC) gl3w__get_proc("glCompressedTexImage1D");
	gl3wCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC) gl3w__get_proc("glCompressedTexImage2D");
	gl3wCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC) gl3w__get_proc("glCompressedTexImage3D");
	gl3wCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) gl3w__get_proc("glCompressedTexSubImage1D");
	gl3wCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) gl3w__get_proc("glCompressedTexSubImage2D");
	gl3wCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) gl3w__get_proc("glCompressedTexSubImage3D");
	gl3wCompressedTextureSubImage1D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC) gl3w__get_proc("glCompressedTextureSubImage1D");
	gl3wCompressedTextureSubImage2D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC) gl3w__get_proc("glCompressedTextureSubImage2D");
	gl3wCompressedTextureSubImage3D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC) gl3w__get_proc("glCompressedTextureSubImage3D");
	gl3wCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC) gl3w__get_proc("glCopyBufferSubData");
	gl3wCopyImageSubData = (PFNGLCOPYIMAGESUBDATAPROC) gl3w__get_proc("glCopyImageSubData");
	gl3wCopyNamedBufferSubData = (PFNGLCOPYNAMEDBUFFERSUBDATAPROC) gl3w__get_proc("glCopyNamedBufferSubData");
	gl3wCopyTexImage1D = (PFNGLCOPYTEXIMAGE1DPROC) gl3w__get_proc("glCopyTexImage1D");
	gl3wCopyTexImage2D = (PFNGLCOPYTEXIMAGE2DPROC) gl3w__get_proc("glCopyTexImage2D");
	gl3wCopyTexSubImage1D = (PFNGLCOPYTEXSUBIMAGE1DPROC) gl3w__get_proc("glCopyTexSubImage1D");
	gl3wCopyTexSubImage2D = (PFNGLCOPYTEXSUBIMAGE2DPROC) gl3w__get_proc("glCopyTexSubImage2D");
	gl3wCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC) gl3w__get_proc("glCopyTexSubImage3D");
	gl3wCopyTextureSubImage1D = (PFNGLCOPYTEXTURESUBIMAGE1DPROC) gl3w__get_proc("glCopyTextureSubImage1D");
	gl3wCopyTextureSubImage2D = (PFNGLCOPYTEXTURESUBIMAGE2DPROC) gl3w__get_proc("glCopyTextureSubImage2D");
	gl3wCopyTextureSubImage3D = (PFNGLCOPYTEXTURESUBIMAGE3DPROC) gl3w__get_proc("glCopyTextureSubImage3D");
	gl3wCreateBuffers = (PFNGLCREATEBUFFERSPROC) gl3w__get_proc("glCreateBuffers");
	gl3wCreateFramebuffers = (PFNGLCREATEFRAMEBUFFERSPROC) gl3w__get_proc("glCreateFramebuffers");
	gl3wCreateProgram = (PFNGLCREATEPROGRAMPROC) gl3w__get_proc("glCreateProgram");
	gl3wCreateProgramPipelines = (PFNGLCREATEPROGRAMPIPELINESPROC) gl3w__get_proc("glCreateProgramPipelines");
	gl3wCreateQueries = (PFNGLCREATEQUERIESPROC) gl3w__get_proc("glCreateQueries");
	gl3wCreateRenderbuffers = (PFNGLCREATERENDERBUFFERSPROC) gl3w__get_proc("glCreateRenderbuffers");
	gl3wCreateSamplers = (PFNGLCREATESAMPLERSPROC) gl3w__get_proc("glCreateSamplers");
	gl3wCreateShader = (PFNGLCREATESHADERPROC) gl3w__get_proc("glCreateShader");
	gl3wCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC) gl3w__get_proc("glCreateShaderProgramv");
	gl3wCreateSyncFromCLeventARB = (PFNGLCREATESYNCFROMCLEVENTARBPROC) gl3w__get_proc("glCreateSyncFromCLeventARB");
	gl3wCreateTextures = (PFNGLCREATETEXTURESPROC) gl3w__get_proc("glCreateTextures");
	gl3wCreateTransformFeedbacks = (PFNGLCREATETRANSFORMFEEDBACKSPROC) gl3w__get_proc("glCreateTransformFeedbacks");
	gl3wCreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC) gl3w__get_proc("glCreateVertexArrays");
	gl3wCullFace = (PFNGLCULLFACEPROC) gl3w__get_proc("glCullFace");
	gl3wDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC) gl3w__get_proc("glDebugMessageCallback");
	gl3wDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC) gl3w__get_proc("glDebugMessageCallbackARB");
	gl3wDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC) gl3w__get_proc("glDebugMessageControl");
	gl3wDebugMessageControlARB = (PFNGLDEBUGMESSAGECONTROLARBPROC) gl3w__get_proc("glDebugMessageControlARB");
	gl3wDebugMessageInsert = (PFNGLDEBUGMESSAGEINSERTPROC) gl3w__get_proc("glDebugMessageInsert");
	gl3wDebugMessageInsertARB = (PFNGLDEBUGMESSAGEINSERTARBPROC) gl3w__get_proc("glDebugMessageInsertARB");
	gl3wDeleteBuffers = (PFNGLDELETEBUFFERSPROC) gl3w__get_proc("glDeleteBuffers");
	gl3wDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) gl3w__get_proc("glDeleteFramebuffers");
	gl3wDeleteNamedStringARB = (PFNGLDELETENAMEDSTRINGARBPROC) gl3w__get_proc("glDeleteNamedStringARB");
	gl3wDeleteProgram = (PFNGLDELETEPROGRAMPROC) gl3w__get_proc("glDeleteProgram");
	gl3wDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC) gl3w__get_proc("glDeleteProgramPipelines");
	gl3wDeleteQueries = (PFNGLDELETEQUERIESPROC) gl3w__get_proc("glDeleteQueries");
	gl3wDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) gl3w__get_proc("glDeleteRenderbuffers");
	gl3wDeleteSamplers = (PFNGLDELETESAMPLERSPROC) gl3w__get_proc("glDeleteSamplers");
	gl3wDeleteShader = (PFNGLDELETESHADERPROC) gl3w__get_proc("glDeleteShader");
	gl3wDeleteSync = (PFNGLDELETESYNCPROC) gl3w__get_proc("glDeleteSync");
	gl3wDeleteTextures = (PFNGLDELETETEXTURESPROC) gl3w__get_proc("glDeleteTextures");
	gl3wDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC) gl3w__get_proc("glDeleteTransformFeedbacks");
	gl3wDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) gl3w__get_proc("glDeleteVertexArrays");
	gl3wDepthFunc = (PFNGLDEPTHFUNCPROC) gl3w__get_proc("glDepthFunc");
	gl3wDepthMask = (PFNGLDEPTHMASKPROC) gl3w__get_proc("glDepthMask");
	gl3wDepthRange = (PFNGLDEPTHRANGEPROC) gl3w__get_proc("glDepthRange");
	gl3wDepthRangeArrayv = (PFNGLDEPTHRANGEARRAYVPROC) gl3w__get_proc("glDepthRangeArrayv");
	gl3wDepthRangeIndexed = (PFNGLDEPTHRANGEINDEXEDPROC) gl3w__get_proc("glDepthRangeIndexed");
	gl3wDepthRangef = (PFNGLDEPTHRANGEFPROC) gl3w__get_proc("glDepthRangef");
	gl3wDetachShader = (PFNGLDETACHSHADERPROC) gl3w__get_proc("glDetachShader");
	gl3wDisable = (PFNGLDISABLEPROC) gl3w__get_proc("glDisable");
	gl3wDisableVertexArrayAttrib = (PFNGLDISABLEVERTEXARRAYATTRIBPROC) gl3w__get_proc("glDisableVertexArrayAttrib");
	gl3wDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) gl3w__get_proc("glDisableVertexAttribArray");
	gl3wDisablei = (PFNGLDISABLEIPROC) gl3w__get_proc("glDisablei");
	gl3wDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC) gl3w__get_proc("glDispatchCompute");
	gl3wDispatchComputeGroupSizeARB = (PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC) gl3w__get_proc("glDispatchComputeGroupSizeARB");
	gl3wDispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECTPROC) gl3w__get_proc("glDispatchComputeIndirect");
	gl3wDrawArrays = (PFNGLDRAWARRAYSPROC) gl3w__get_proc("glDrawArrays");
	gl3wDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC) gl3w__get_proc("glDrawArraysIndirect");
	gl3wDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC) gl3w__get_proc("glDrawArraysInstanced");
	gl3wDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC) gl3w__get_proc("glDrawArraysInstancedBaseInstance");
	gl3wDrawBuffer = (PFNGLDRAWBUFFERPROC) gl3w__get_proc("glDrawBuffer");
	gl3wDrawBuffers = (PFNGLDRAWBUFFERSPROC) gl3w__get_proc("glDrawBuffers");
	gl3wDrawElements = (PFNGLDRAWELEMENTSPROC) gl3w__get_proc("glDrawElements");
	gl3wDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC) gl3w__get_proc("glDrawElementsBaseVertex");
	gl3wDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC) gl3w__get_proc("glDrawElementsIndirect");
	gl3wDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC) gl3w__get_proc("glDrawElementsInstanced");
	gl3wDrawElementsInstancedBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC) gl3w__get_proc("glDrawElementsInstancedBaseInstance");
	gl3wDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC) gl3w__get_proc("glDrawElementsInstancedBaseVertex");
	gl3wDrawElementsInstancedBaseVertexBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC) gl3w__get_proc("glDrawElementsInstancedBaseVertexBaseInstance");
	gl3wDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC) gl3w__get_proc("glDrawRangeElements");
	gl3wDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC) gl3w__get_proc("glDrawRangeElementsBaseVertex");
	gl3wDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACKPROC) gl3w__get_proc("glDrawTransformFeedback");
	gl3wDrawTransformFeedbackInstanced = (PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC) gl3w__get_proc("glDrawTransformFeedbackInstanced");
	gl3wDrawTransformFeedbackStream = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC) gl3w__get_proc("glDrawTransformFeedbackStream");
	gl3wDrawTransformFeedbackStreamInstanced = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC) gl3w__get_proc("glDrawTransformFeedbackStreamInstanced");
	gl3wEnable = (PFNGLENABLEPROC) gl3w__get_proc("glEnable");
	gl3wEnableVertexArrayAttrib = (PFNGLENABLEVERTEXARRAYATTRIBPROC) gl3w__get_proc("glEnableVertexArrayAttrib");
	gl3wEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) gl3w__get_proc("glEnableVertexAttribArray");
	gl3wEnablei = (PFNGLENABLEIPROC) gl3w__get_proc("glEnablei");
	gl3wEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC) gl3w__get_proc("glEndConditionalRender");
	gl3wEndQuery = (PFNGLENDQUERYPROC) gl3w__get_proc("glEndQuery");
	gl3wEndQueryIndexed = (PFNGLENDQUERYINDEXEDPROC) gl3w__get_proc("glEndQueryIndexed");
	gl3wEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC) gl3w__get_proc("glEndTransformFeedback");
	gl3wFenceSync = (PFNGLFENCESYNCPROC) gl3w__get_proc("glFenceSync");
	gl3wFinish = (PFNGLFINISHPROC) gl3w__get_proc("glFinish");
	gl3wFlush = (PFNGLFLUSHPROC) gl3w__get_proc("glFlush");
	gl3wFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC) gl3w__get_proc("glFlushMappedBufferRange");
	gl3wFlushMappedNamedBufferRange = (PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC) gl3w__get_proc("glFlushMappedNamedBufferRange");
	gl3wFramebufferParameteri = (PFNGLFRAMEBUFFERPARAMETERIPROC) gl3w__get_proc("glFramebufferParameteri");
	gl3wFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) gl3w__get_proc("glFramebufferRenderbuffer");
	gl3wFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC) gl3w__get_proc("glFramebufferTexture");
	gl3wFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) gl3w__get_proc("glFramebufferTexture1D");
	gl3wFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) gl3w__get_proc("glFramebufferTexture2D");
	gl3wFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) gl3w__get_proc("glFramebufferTexture3D");
	gl3wFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC) gl3w__get_proc("glFramebufferTextureLayer");
	gl3wFrontFace = (PFNGLFRONTFACEPROC) gl3w__get_proc("glFrontFace");
	gl3wGenBuffers = (PFNGLGENBUFFERSPROC) gl3w__get_proc("glGenBuffers");
	gl3wGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) gl3w__get_proc("glGenFramebuffers");
	gl3wGenProgramPipelines = (PFNGLGENPROGRAMPIPELINESPROC) gl3w__get_proc("glGenProgramPipelines");
	gl3wGenQueries = (PFNGLGENQUERIESPROC) gl3w__get_proc("glGenQueries");
	gl3wGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) gl3w__get_proc("glGenRenderbuffers");
	gl3wGenSamplers = (PFNGLGENSAMPLERSPROC) gl3w__get_proc("glGenSamplers");
	gl3wGenTextures = (PFNGLGENTEXTURESPROC) gl3w__get_proc("glGenTextures");
	gl3wGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC) gl3w__get_proc("glGenTransformFeedbacks");
	gl3wGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) gl3w__get_proc("glGenVertexArrays");
	gl3wGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) gl3w__get_proc("glGenerateMipmap");
	gl3wGenerateTextureMipmap = (PFNGLGENERATETEXTUREMIPMAPPROC) gl3w__get_proc("glGenerateTextureMipmap");
	gl3wGetActiveAtomicCounterBufferiv = (PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC) gl3w__get_proc("glGetActiveAtomicCounterBufferiv");
	gl3wGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC) gl3w__get_proc("glGetActiveAttrib");
	gl3wGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAMEPROC) gl3w__get_proc("glGetActiveSubroutineName");
	gl3wGetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC) gl3w__get_proc("glGetActiveSubroutineUniformName");
	gl3wGetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC) gl3w__get_proc("glGetActiveSubroutineUniformiv");
	gl3wGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC) gl3w__get_proc("glGetActiveUniform");
	gl3wGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) gl3w__get_proc("glGetActiveUniformBlockName");
	gl3wGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC) gl3w__get_proc("glGetActiveUniformBlockiv");
	gl3wGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC) gl3w__get_proc("glGetActiveUniformName");
	gl3wGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC) gl3w__get_proc("glGetActiveUniformsiv");
	gl3wGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC) gl3w__get_proc("glGetAttachedShaders");
	gl3wGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) gl3w__get_proc("glGetAttribLocation");
	gl3wGetBooleani_v = (PFNGLGETBOOLEANI_VPROC) gl3w__get_proc("glGetBooleani_v");
	gl3wGetBooleanv = (PFNGLGETBOOLEANVPROC) gl3w__get_proc("glGetBooleanv");
	gl3wGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC) gl3w__get_proc("glGetBufferParameteri64v");
	gl3wGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC) gl3w__get_proc("glGetBufferParameteriv");
	gl3wGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC) gl3w__get_proc("glGetBufferPointerv");
	gl3wGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC) gl3w__get_proc("glGetBufferSubData");
	gl3wGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC) gl3w__get_proc("glGetCompressedTexImage");
	gl3wGetCompressedTextureImage = (PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC) gl3w__get_proc("glGetCompressedTextureImage");
	gl3wGetCompressedTextureSubImage = (PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC) gl3w__get_proc("glGetCompressedTextureSubImage");
	gl3wGetDebugMessageLog = (PFNGLGETDEBUGMESSAGELOGPROC) gl3w__get_proc("glGetDebugMessageLog");
	gl3wGetDebugMessageLogARB = (PFNGLGETDEBUGMESSAGELOGARBPROC) gl3w__get_proc("glGetDebugMessageLogARB");
	gl3wGetDoublei_v = (PFNGLGETDOUBLEI_VPROC) gl3w__get_proc("glGetDoublei_v");
	gl3wGetDoublev = (PFNGLGETDOUBLEVPROC) gl3w__get_proc("glGetDoublev");
	gl3wGetError = (PFNGLGETERRORPROC) gl3w__get_proc("glGetError");
	gl3wGetFloati_v = (PFNGLGETFLOATI_VPROC) gl3w__get_proc("glGetFloati_v");
	gl3wGetFloatv = (PFNGLGETFLOATVPROC) gl3w__get_proc("glGetFloatv");
	gl3wGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC) gl3w__get_proc("glGetFragDataIndex");
	gl3wGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC) gl3w__get_proc("glGetFragDataLocation");
	gl3wGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) gl3w__get_proc("glGetFramebufferAttachmentParameteriv");
	gl3wGetFramebufferParameteriv = (PFNGLGETFRAMEBUFFERPARAMETERIVPROC) gl3w__get_proc("glGetFramebufferParameteriv");
	gl3wGetGraphicsResetStatus = (PFNGLGETGRAPHICSRESETSTATUSPROC) gl3w__get_proc("glGetGraphicsResetStatus");
	gl3wGetGraphicsResetStatusARB = (PFNGLGETGRAPHICSRESETSTATUSARBPROC) gl3w__get_proc("glGetGraphicsResetStatusARB");
	gl3wGetImageHandleARB = (PFNGLGETIMAGEHANDLEARBPROC) gl3w__get_proc("glGetImageHandleARB");
	gl3wGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC) gl3w__get_proc("glGetInteger64i_v");
	gl3wGetInteger64v = (PFNGLGETINTEGER64VPROC) gl3w__get_proc("glGetInteger64v");
	gl3wGetIntegeri_v = (PFNGLGETINTEGERI_VPROC) gl3w__get_proc("glGetIntegeri_v");
	gl3wGetIntegerv = (PFNGLGETINTEGERVPROC) gl3w__get_proc("glGetIntegerv");
	gl3wGetInternalformati64v = (PFNGLGETINTERNALFORMATI64VPROC) gl3w__get_proc("glGetInternalformati64v");
	gl3wGetInternalformativ = (PFNGLGETINTERNALFORMATIVPROC) gl3w__get_proc("glGetInternalformativ");
	gl3wGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC) gl3w__get_proc("glGetMultisamplefv");
	gl3wGetNamedBufferParameteri64v = (PFNGLGETNAMEDBUFFERPARAMETERI64VPROC) gl3w__get_proc("glGetNamedBufferParameteri64v");
	gl3wGetNamedBufferParameteriv = (PFNGLGETNAMEDBUFFERPARAMETERIVPROC) gl3w__get_proc("glGetNamedBufferParameteriv");
	gl3wGetNamedBufferPointerv = (PFNGLGETNAMEDBUFFERPOINTERVPROC) gl3w__get_proc("glGetNamedBufferPointerv");
	gl3wGetNamedBufferSubData = (PFNGLGETNAMEDBUFFERSUBDATAPROC) gl3w__get_proc("glGetNamedBufferSubData");
	gl3wGetNamedFramebufferAttachmentParameteriv = (PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC) gl3w__get_proc("glGetNamedFramebufferAttachmentParameteriv");
	gl3wGetNamedFramebufferParameteriv = (PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC) gl3w__get_proc("glGetNamedFramebufferParameteriv");
	gl3wGetNamedRenderbufferParameteriv = (PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC) gl3w__get_proc("glGetNamedRenderbufferParameteriv");
	gl3wGetNamedStringARB = (PFNGLGETNAMEDSTRINGARBPROC) gl3w__get_proc("glGetNamedStringARB");
	gl3wGetNamedStringivARB = (PFNGLGETNAMEDSTRINGIVARBPROC) gl3w__get_proc("glGetNamedStringivARB");
	gl3wGetObjectLabel = (PFNGLGETOBJECTLABELPROC) gl3w__get_proc("glGetObjectLabel");
	gl3wGetObjectPtrLabel = (PFNGLGETOBJECTPTRLABELPROC) gl3w__get_proc("glGetObjectPtrLabel");
	gl3wGetPointerv = (PFNGLGETPOINTERVPROC) gl3w__get_proc("glGetPointerv");
	gl3wGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC) gl3w__get_proc("glGetProgramBinary");
	gl3wGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) gl3w__get_proc("glGetProgramInfoLog");
	gl3wGetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIVPROC) gl3w__get_proc("glGetProgramInterfaceiv");
	gl3wGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOGPROC) gl3w__get_proc("glGetProgramPipelineInfoLog");
	gl3wGetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIVPROC) gl3w__get_proc("glGetProgramPipelineiv");
	gl3wGetProgramResourceIndex = (PFNGLGETPROGRAMRESOURCEINDEXPROC) gl3w__get_proc("glGetProgramResourceIndex");
	gl3wGetProgramResourceLocation = (PFNGLGETPROGRAMRESOURCELOCATIONPROC) gl3w__get_proc("glGetProgramResourceLocation");
	gl3wGetProgramResourceLocationIndex = (PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC) gl3w__get_proc("glGetProgramResourceLocationIndex");
	gl3wGetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAMEPROC) gl3w__get_proc("glGetProgramResourceName");
	gl3wGetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIVPROC) gl3w__get_proc("glGetProgramResourceiv");
	gl3wGetProgramStageiv = (PFNGLGETPROGRAMSTAGEIVPROC) gl3w__get_proc("glGetProgramStageiv");
	gl3wGetProgramiv = (PFNGLGETPROGRAMIVPROC) gl3w__get_proc("glGetProgramiv");
	gl3wGetQueryBufferObjecti64v = (PFNGLGETQUERYBUFFEROBJECTI64VPROC) gl3w__get_proc("glGetQueryBufferObjecti64v");
	gl3wGetQueryBufferObjectiv = (PFNGLGETQUERYBUFFEROBJECTIVPROC) gl3w__get_proc("glGetQueryBufferObjectiv");
	gl3wGetQueryBufferObjectui64v = (PFNGLGETQUERYBUFFEROBJECTUI64VPROC) gl3w__get_proc("glGetQueryBufferObjectui64v");
	gl3wGetQueryBufferObjectuiv = (PFNGLGETQUERYBUFFEROBJECTUIVPROC) gl3w__get_proc("glGetQueryBufferObjectuiv");
	gl3wGetQueryIndexediv = (PFNGLGETQUERYINDEXEDIVPROC) gl3w__get_proc("glGetQueryIndexediv");
	gl3wGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC) gl3w__get_proc("glGetQueryObjecti64v");
	gl3wGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC) gl3w__get_proc("glGetQueryObjectiv");
	gl3wGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC) gl3w__get_proc("glGetQueryObjectui64v");
	gl3wGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC) gl3w__get_proc("glGetQueryObjectuiv");
	gl3wGetQueryiv = (PFNGLGETQUERYIVPROC) gl3w__get_proc("glGetQueryiv");
	gl3wGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) gl3w__get_proc("glGetRenderbufferParameteriv");
	gl3wGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC) gl3w__get_proc("glGetSamplerParameterIiv");
	gl3wGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC) gl3w__get_proc("glGetSamplerParameterIuiv");
	gl3wGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC) gl3w__get_proc("glGetSamplerParameterfv");
	gl3wGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC) gl3w__get_proc("glGetSamplerParameteriv");
	gl3wGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) gl3w__get_proc("glGetShaderInfoLog");
	gl3wGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMATPROC) gl3w__get_proc("glGetShaderPrecisionFormat");
	gl3wGetShaderSource = (PFNGLGETSHADERSOURCEPROC) gl3w__get_proc("glGetShaderSource");
	gl3wGetShaderiv = (PFNGLGETSHADERIVPROC) gl3w__get_proc("glGetShaderiv");
	gl3wGetString = (PFNGLGETSTRINGPROC) gl3w__get_proc("glGetString");
	gl3wGetStringi = (PFNGLGETSTRINGIPROC) gl3w__get_proc("glGetStringi");
	gl3wGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEXPROC) gl3w__get_proc("glGetSubroutineIndex");
	gl3wGetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC) gl3w__get_proc("glGetSubroutineUniformLocation");
	gl3wGetSynciv = (PFNGLGETSYNCIVPROC) gl3w__get_proc("glGetSynciv");
	gl3wGetTexImage = (PFNGLGETTEXIMAGEPROC) gl3w__get_proc("glGetTexImage");
	gl3wGetTexLevelParameterfv = (PFNGLGETTEXLEVELPARAMETERFVPROC) gl3w__get_proc("glGetTexLevelParameterfv");
	gl3wGetTexLevelParameteriv = (PFNGLGETTEXLEVELPARAMETERIVPROC) gl3w__get_proc("glGetTexLevelParameteriv");
	gl3wGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC) gl3w__get_proc("glGetTexParameterIiv");
	gl3wGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC) gl3w__get_proc("glGetTexParameterIuiv");
	gl3wGetTexParameterfv = (PFNGLGETTEXPARAMETERFVPROC) gl3w__get_proc("glGetTexParameterfv");
	gl3wGetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC) gl3w__get_proc("glGetTexParameteriv");
	gl3wGetTextureHandleARB = (PFNGLGETTEXTUREHANDLEARBPROC) gl3w__get_proc("glGetTextureHandleARB");
	gl3wGetTextureImage = (PFNGLGETTEXTUREIMAGEPROC) gl3w__get_proc("glGetTextureImage");
	gl3wGetTextureLevelParameterfv = (PFNGLGETTEXTURELEVELPARAMETERFVPROC) gl3w__get_proc("glGetTextureLevelParameterfv");
	gl3wGetTextureLevelParameteriv = (PFNGLGETTEXTURELEVELPARAMETERIVPROC) gl3w__get_proc("glGetTextureLevelParameteriv");
	gl3wGetTextureParameterIiv = (PFNGLGETTEXTUREPARAMETERIIVPROC) gl3w__get_proc("glGetTextureParameterIiv");
	gl3wGetTextureParameterIuiv = (PFNGLGETTEXTUREPARAMETERIUIVPROC) gl3w__get_proc("glGetTextureParameterIuiv");
	gl3wGetTextureParameterfv = (PFNGLGETTEXTUREPARAMETERFVPROC) gl3w__get_proc("glGetTextureParameterfv");
	gl3wGetTextureParameteriv = (PFNGLGETTEXTUREPARAMETERIVPROC) gl3w__get_proc("glGetTextureParameteriv");
	gl3wGetTextureSamplerHandleARB = (PFNGLGETTEXTURESAMPLERHANDLEARBPROC) gl3w__get_proc("glGetTextureSamplerHandleARB");
	gl3wGetTextureSubImage = (PFNGLGETTEXTURESUBIMAGEPROC) gl3w__get_proc("glGetTextureSubImage");
	gl3wGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC) gl3w__get_proc("glGetTransformFeedbackVarying");
	gl3wGetTransformFeedbacki64_v = (PFNGLGETTRANSFORMFEEDBACKI64_VPROC) gl3w__get_proc("glGetTransformFeedbacki64_v");
	gl3wGetTransformFeedbacki_v = (PFNGLGETTRANSFORMFEEDBACKI_VPROC) gl3w__get_proc("glGetTransformFeedbacki_v");
	gl3wGetTransformFeedbackiv = (PFNGLGETTRANSFORMFEEDBACKIVPROC) gl3w__get_proc("glGetTransformFeedbackiv");
	gl3wGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC) gl3w__get_proc("glGetUniformBlockIndex");
	gl3wGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC) gl3w__get_proc("glGetUniformIndices");
	gl3wGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) gl3w__get_proc("glGetUniformLocation");
	gl3wGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIVPROC) gl3w__get_proc("glGetUniformSubroutineuiv");
	gl3wGetUniformdv = (PFNGLGETUNIFORMDVPROC) gl3w__get_proc("glGetUniformdv");
	gl3wGetUniformfv = (PFNGLGETUNIFORMFVPROC) gl3w__get_proc("glGetUniformfv");
	gl3wGetUniformiv = (PFNGLGETUNIFORMIVPROC) gl3w__get_proc("glGetUniformiv");
	gl3wGetUniformuiv = (PFNGLGETUNIFORMUIVPROC) gl3w__get_proc("glGetUniformuiv");
	gl3wGetVertexArrayIndexed64iv = (PFNGLGETVERTEXARRAYINDEXED64IVPROC) gl3w__get_proc("glGetVertexArrayIndexed64iv");
	gl3wGetVertexArrayIndexediv = (PFNGLGETVERTEXARRAYINDEXEDIVPROC) gl3w__get_proc("glGetVertexArrayIndexediv");
	gl3wGetVertexArrayiv = (PFNGLGETVERTEXARRAYIVPROC) gl3w__get_proc("glGetVertexArrayiv");
	gl3wGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC) gl3w__get_proc("glGetVertexAttribIiv");
	gl3wGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC) gl3w__get_proc("glGetVertexAttribIuiv");
	gl3wGetVertexAttribLdv = (PFNGLGETVERTEXATTRIBLDVPROC) gl3w__get_proc("glGetVertexAttribLdv");
	gl3wGetVertexAttribLui64vARB = (PFNGLGETVERTEXATTRIBLUI64VARBPROC) gl3w__get_proc("glGetVertexAttribLui64vARB");
	gl3wGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC) gl3w__get_proc("glGetVertexAttribPointerv");
	gl3wGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC) gl3w__get_proc("glGetVertexAttribdv");
	gl3wGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC) gl3w__get_proc("glGetVertexAttribfv");
	gl3wGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC) gl3w__get_proc("glGetVertexAttribiv");
	gl3wGetnCompressedTexImage = (PFNGLGETNCOMPRESSEDTEXIMAGEPROC) gl3w__get_proc("glGetnCompressedTexImage");
	gl3wGetnCompressedTexImageARB = (PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC) gl3w__get_proc("glGetnCompressedTexImageARB");
	gl3wGetnTexImage = (PFNGLGETNTEXIMAGEPROC) gl3w__get_proc("glGetnTexImage");
	gl3wGetnTexImageARB = (PFNGLGETNTEXIMAGEARBPROC) gl3w__get_proc("glGetnTexImageARB");
	gl3wGetnUniformdv = (PFNGLGETNUNIFORMDVPROC) gl3w__get_proc("glGetnUniformdv");
	gl3wGetnUniformdvARB = (PFNGLGETNUNIFORMDVARBPROC) gl3w__get_proc("glGetnUniformdvARB");
	gl3wGetnUniformfv = (PFNGLGETNUNIFORMFVPROC) gl3w__get_proc("glGetnUniformfv");
	gl3wGetnUniformfvARB = (PFNGLGETNUNIFORMFVARBPROC) gl3w__get_proc("glGetnUniformfvARB");
	gl3wGetnUniformiv = (PFNGLGETNUNIFORMIVPROC) gl3w__get_proc("glGetnUniformiv");
	gl3wGetnUniformivARB = (PFNGLGETNUNIFORMIVARBPROC) gl3w__get_proc("glGetnUniformivARB");
	gl3wGetnUniformuiv = (PFNGLGETNUNIFORMUIVPROC) gl3w__get_proc("glGetnUniformuiv");
	gl3wGetnUniformuivARB = (PFNGLGETNUNIFORMUIVARBPROC) gl3w__get_proc("glGetnUniformuivARB");
	gl3wHint = (PFNGLHINTPROC) gl3w__get_proc("glHint");
	gl3wInvalidateBufferData = (PFNGLINVALIDATEBUFFERDATAPROC) gl3w__get_proc("glInvalidateBufferData");
	gl3wInvalidateBufferSubData = (PFNGLINVALIDATEBUFFERSUBDATAPROC) gl3w__get_proc("glInvalidateBufferSubData");
	gl3wInvalidateFramebuffer = (PFNGLINVALIDATEFRAMEBUFFERPROC) gl3w__get_proc("glInvalidateFramebuffer");
	gl3wInvalidateNamedFramebufferData = (PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC) gl3w__get_proc("glInvalidateNamedFramebufferData");
	gl3wInvalidateNamedFramebufferSubData = (PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC) gl3w__get_proc("glInvalidateNamedFramebufferSubData");
	gl3wInvalidateSubFramebuffer = (PFNGLINVALIDATESUBFRAMEBUFFERPROC) gl3w__get_proc("glInvalidateSubFramebuffer");
	gl3wInvalidateTexImage = (PFNGLINVALIDATETEXIMAGEPROC) gl3w__get_proc("glInvalidateTexImage");
	gl3wInvalidateTexSubImage = (PFNGLINVALIDATETEXSUBIMAGEPROC) gl3w__get_proc("glInvalidateTexSubImage");
	gl3wIsBuffer = (PFNGLISBUFFERPROC) gl3w__get_proc("glIsBuffer");
	gl3wIsEnabled = (PFNGLISENABLEDPROC) gl3w__get_proc("glIsEnabled");
	gl3wIsEnabledi = (PFNGLISENABLEDIPROC) gl3w__get_proc("glIsEnabledi");
	gl3wIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) gl3w__get_proc("glIsFramebuffer");
	gl3wIsImageHandleResidentARB = (PFNGLISIMAGEHANDLERESIDENTARBPROC) gl3w__get_proc("glIsImageHandleResidentARB");
	gl3wIsNamedStringARB = (PFNGLISNAMEDSTRINGARBPROC) gl3w__get_proc("glIsNamedStringARB");
	gl3wIsProgram = (PFNGLISPROGRAMPROC) gl3w__get_proc("glIsProgram");
	gl3wIsProgramPipeline = (PFNGLISPROGRAMPIPELINEPROC) gl3w__get_proc("glIsProgramPipeline");
	gl3wIsQuery = (PFNGLISQUERYPROC) gl3w__get_proc("glIsQuery");
	gl3wIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) gl3w__get_proc("glIsRenderbuffer");
	gl3wIsSampler = (PFNGLISSAMPLERPROC) gl3w__get_proc("glIsSampler");
	gl3wIsShader = (PFNGLISSHADERPROC) gl3w__get_proc("glIsShader");
	gl3wIsSync = (PFNGLISSYNCPROC) gl3w__get_proc("glIsSync");
	gl3wIsTexture = (PFNGLISTEXTUREPROC) gl3w__get_proc("glIsTexture");
	gl3wIsTextureHandleResidentARB = (PFNGLISTEXTUREHANDLERESIDENTARBPROC) gl3w__get_proc("glIsTextureHandleResidentARB");
	gl3wIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC) gl3w__get_proc("glIsTransformFeedback");
	gl3wIsVertexArray = (PFNGLISVERTEXARRAYPROC) gl3w__get_proc("glIsVertexArray");
	gl3wLineWidth = (PFNGLLINEWIDTHPROC) gl3w__get_proc("glLineWidth");
	gl3wLinkProgram = (PFNGLLINKPROGRAMPROC) gl3w__get_proc("glLinkProgram");
	gl3wLogicOp = (PFNGLLOGICOPPROC) gl3w__get_proc("glLogicOp");
	gl3wMakeImageHandleNonResidentARB = (PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC) gl3w__get_proc("glMakeImageHandleNonResidentARB");
	gl3wMakeImageHandleResidentARB = (PFNGLMAKEIMAGEHANDLERESIDENTARBPROC) gl3w__get_proc("glMakeImageHandleResidentARB");
	gl3wMakeTextureHandleNonResidentARB = (PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC) gl3w__get_proc("glMakeTextureHandleNonResidentARB");
	gl3wMakeTextureHandleResidentARB = (PFNGLMAKETEXTUREHANDLERESIDENTARBPROC) gl3w__get_proc("glMakeTextureHandleResidentARB");
	gl3wMapBuffer = (PFNGLMAPBUFFERPROC) gl3w__get_proc("glMapBuffer");
	gl3wMapBufferRange = (PFNGLMAPBUFFERRANGEPROC) gl3w__get_proc("glMapBufferRange");
	gl3wMapNamedBuffer = (PFNGLMAPNAMEDBUFFERPROC) gl3w__get_proc("glMapNamedBuffer");
	gl3wMapNamedBufferRange = (PFNGLMAPNAMEDBUFFERRANGEPROC) gl3w__get_proc("glMapNamedBufferRange");
	gl3wMemoryBarrier = (PFNGLMEMORYBARRIERPROC) gl3w__get_proc("glMemoryBarrier");
	gl3wMemoryBarrierByRegion = (PFNGLMEMORYBARRIERBYREGIONPROC) gl3w__get_proc("glMemoryBarrierByRegion");
	gl3wMinSampleShading = (PFNGLMINSAMPLESHADINGPROC) gl3w__get_proc("glMinSampleShading");
	gl3wMinSampleShadingARB = (PFNGLMINSAMPLESHADINGARBPROC) gl3w__get_proc("glMinSampleShadingARB");
	gl3wMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC) gl3w__get_proc("glMultiDrawArrays");
	gl3wMultiDrawArraysIndirect = (PFNGLMULTIDRAWARRAYSINDIRECTPROC) gl3w__get_proc("glMultiDrawArraysIndirect");
	gl3wMultiDrawArraysIndirectCountARB = (PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC) gl3w__get_proc("glMultiDrawArraysIndirectCountARB");
	gl3wMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC) gl3w__get_proc("glMultiDrawElements");
	gl3wMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC) gl3w__get_proc("glMultiDrawElementsBaseVertex");
	gl3wMultiDrawElementsIndirect = (PFNGLMULTIDRAWELEMENTSINDIRECTPROC) gl3w__get_proc("glMultiDrawElementsIndirect");
	gl3wMultiDrawElementsIndirectCountARB = (PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC) gl3w__get_proc("glMultiDrawElementsIndirectCountARB");
	gl3wNamedBufferData = (PFNGLNAMEDBUFFERDATAPROC) gl3w__get_proc("glNamedBufferData");
	gl3wNamedBufferPageCommitmentARB = (PFNGLNAMEDBUFFERPAGECOMMITMENTARBPROC) gl3w__get_proc("glNamedBufferPageCommitmentARB");
	gl3wNamedBufferPageCommitmentEXT = (PFNGLNAMEDBUFFERPAGECOMMITMENTEXTPROC) gl3w__get_proc("glNamedBufferPageCommitmentEXT");
	gl3wNamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGEPROC) gl3w__get_proc("glNamedBufferStorage");
	gl3wNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC) gl3w__get_proc("glNamedBufferSubData");
	gl3wNamedFramebufferDrawBuffer = (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC) gl3w__get_proc("glNamedFramebufferDrawBuffer");
	gl3wNamedFramebufferDrawBuffers = (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC) gl3w__get_proc("glNamedFramebufferDrawBuffers");
	gl3wNamedFramebufferParameteri = (PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC) gl3w__get_proc("glNamedFramebufferParameteri");
	gl3wNamedFramebufferReadBuffer = (PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC) gl3w__get_proc("glNamedFramebufferReadBuffer");
	gl3wNamedFramebufferRenderbuffer = (PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC) gl3w__get_proc("glNamedFramebufferRenderbuffer");
	gl3wNamedFramebufferTexture = (PFNGLNAMEDFRAMEBUFFERTEXTUREPROC) gl3w__get_proc("glNamedFramebufferTexture");
	gl3wNamedFramebufferTextureLayer = (PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC) gl3w__get_proc("glNamedFramebufferTextureLayer");
	gl3wNamedRenderbufferStorage = (PFNGLNAMEDRENDERBUFFERSTORAGEPROC) gl3w__get_proc("glNamedRenderbufferStorage");
	gl3wNamedRenderbufferStorageMultisample = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC) gl3w__get_proc("glNamedRenderbufferStorageMultisample");
	gl3wNamedStringARB = (PFNGLNAMEDSTRINGARBPROC) gl3w__get_proc("glNamedStringARB");
	gl3wObjectLabel = (PFNGLOBJECTLABELPROC) gl3w__get_proc("glObjectLabel");
	gl3wObjectPtrLabel = (PFNGLOBJECTPTRLABELPROC) gl3w__get_proc("glObjectPtrLabel");
	gl3wPatchParameterfv = (PFNGLPATCHPARAMETERFVPROC) gl3w__get_proc("glPatchParameterfv");
	gl3wPatchParameteri = (PFNGLPATCHPARAMETERIPROC) gl3w__get_proc("glPatchParameteri");
	gl3wPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACKPROC) gl3w__get_proc("glPauseTransformFeedback");
	gl3wPixelStoref = (PFNGLPIXELSTOREFPROC) gl3w__get_proc("glPixelStoref");
	gl3wPixelStorei = (PFNGLPIXELSTOREIPROC) gl3w__get_proc("glPixelStorei");
	gl3wPointParameterf = (PFNGLPOINTPARAMETERFPROC) gl3w__get_proc("glPointParameterf");
	gl3wPointParameterfv = (PFNGLPOINTPARAMETERFVPROC) gl3w__get_proc("glPointParameterfv");
	gl3wPointParameteri = (PFNGLPOINTPARAMETERIPROC) gl3w__get_proc("glPointParameteri");
	gl3wPointParameteriv = (PFNGLPOINTPARAMETERIVPROC) gl3w__get_proc("glPointParameteriv");
	gl3wPointSize = (PFNGLPOINTSIZEPROC) gl3w__get_proc("glPointSize");
	gl3wPolygonMode = (PFNGLPOLYGONMODEPROC) gl3w__get_proc("glPolygonMode");
	gl3wPolygonOffset = (PFNGLPOLYGONOFFSETPROC) gl3w__get_proc("glPolygonOffset");
	gl3wPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC) gl3w__get_proc("glPopDebugGroup");
	gl3wPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC) gl3w__get_proc("glPrimitiveRestartIndex");
	gl3wProgramBinary = (PFNGLPROGRAMBINARYPROC) gl3w__get_proc("glProgramBinary");
	gl3wProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC) gl3w__get_proc("glProgramParameteri");
	gl3wProgramUniform1d = (PFNGLPROGRAMUNIFORM1DPROC) gl3w__get_proc("glProgramUniform1d");
	gl3wProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DVPROC) gl3w__get_proc("glProgramUniform1dv");
	gl3wProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC) gl3w__get_proc("glProgramUniform1f");
	gl3wProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FVPROC) gl3w__get_proc("glProgramUniform1fv");
	gl3wProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC) gl3w__get_proc("glProgramUniform1i");
	gl3wProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IVPROC) gl3w__get_proc("glProgramUniform1iv");
	gl3wProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UIPROC) gl3w__get_proc("glProgramUniform1ui");
	gl3wProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIVPROC) gl3w__get_proc("glProgramUniform1uiv");
	gl3wProgramUniform2d = (PFNGLPROGRAMUNIFORM2DPROC) gl3w__get_proc("glProgramUniform2d");
	gl3wProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DVPROC) gl3w__get_proc("glProgramUniform2dv");
	gl3wProgramUniform2f = (PFNGLPROGRAMUNIFORM2FPROC) gl3w__get_proc("glProgramUniform2f");
	gl3wProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC) gl3w__get_proc("glProgramUniform2fv");
	gl3wProgramUniform2i = (PFNGLPROGRAMUNIFORM2IPROC) gl3w__get_proc("glProgramUniform2i");
	gl3wProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IVPROC) gl3w__get_proc("glProgramUniform2iv");
	gl3wProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UIPROC) gl3w__get_proc("glProgramUniform2ui");
	gl3wProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIVPROC) gl3w__get_proc("glProgramUniform2uiv");
	gl3wProgramUniform3d = (PFNGLPROGRAMUNIFORM3DPROC) gl3w__get_proc("glProgramUniform3d");
	gl3wProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DVPROC) gl3w__get_proc("glProgramUniform3dv");
	gl3wProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC) gl3w__get_proc("glProgramUniform3f");
	gl3wProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC) gl3w__get_proc("glProgramUniform3fv");
	gl3wProgramUniform3i = (PFNGLPROGRAMUNIFORM3IPROC) gl3w__get_proc("glProgramUniform3i");
	gl3wProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IVPROC) gl3w__get_proc("glProgramUniform3iv");
	gl3wProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UIPROC) gl3w__get_proc("glProgramUniform3ui");
	gl3wProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIVPROC) gl3w__get_proc("glProgramUniform3uiv");
	gl3wProgramUniform4d = (PFNGLPROGRAMUNIFORM4DPROC) gl3w__get_proc("glProgramUniform4d");
	gl3wProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DVPROC) gl3w__get_proc("glProgramUniform4dv");
	gl3wProgramUniform4f = (PFNGLPROGRAMUNIFORM4FPROC) gl3w__get_proc("glProgramUniform4f");
	gl3wProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC) gl3w__get_proc("glProgramUniform4fv");
	gl3wProgramUniform4i = (PFNGLPROGRAMUNIFORM4IPROC) gl3w__get_proc("glProgramUniform4i");
	gl3wProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IVPROC) gl3w__get_proc("glProgramUniform4iv");
	gl3wProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UIPROC) gl3w__get_proc("glProgramUniform4ui");
	gl3wProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIVPROC) gl3w__get_proc("glProgramUniform4uiv");
	gl3wProgramUniformHandleui64ARB = (PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC) gl3w__get_proc("glProgramUniformHandleui64ARB");
	gl3wProgramUniformHandleui64vARB = (PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC) gl3w__get_proc("glProgramUniformHandleui64vARB");
	gl3wProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DVPROC) gl3w__get_proc("glProgramUniformMatrix2dv");
	gl3wProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC) gl3w__get_proc("glProgramUniformMatrix2fv");
	gl3wProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC) gl3w__get_proc("glProgramUniformMatrix2x3dv");
	gl3wProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC) gl3w__get_proc("glProgramUniformMatrix2x3fv");
	gl3wProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC) gl3w__get_proc("glProgramUniformMatrix2x4dv");
	gl3wProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC) gl3w__get_proc("glProgramUniformMatrix2x4fv");
	gl3wProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DVPROC) gl3w__get_proc("glProgramUniformMatrix3dv");
	gl3wProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC) gl3w__get_proc("glProgramUniformMatrix3fv");
	gl3wProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC) gl3w__get_proc("glProgramUniformMatrix3x2dv");
	gl3wProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC) gl3w__get_proc("glProgramUniformMatrix3x2fv");
	gl3wProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC) gl3w__get_proc("glProgramUniformMatrix3x4dv");
	gl3wProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC) gl3w__get_proc("glProgramUniformMatrix3x4fv");
	gl3wProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DVPROC) gl3w__get_proc("glProgramUniformMatrix4dv");
	gl3wProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC) gl3w__get_proc("glProgramUniformMatrix4fv");
	gl3wProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC) gl3w__get_proc("glProgramUniformMatrix4x2dv");
	gl3wProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC) gl3w__get_proc("glProgramUniformMatrix4x2fv");
	gl3wProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC) gl3w__get_proc("glProgramUniformMatrix4x3dv");
	gl3wProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC) gl3w__get_proc("glProgramUniformMatrix4x3fv");
	gl3wProvokingVertex = (PFNGLPROVOKINGVERTEXPROC) gl3w__get_proc("glProvokingVertex");
	gl3wPushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC) gl3w__get_proc("glPushDebugGroup");
	gl3wQueryCounter = (PFNGLQUERYCOUNTERPROC) gl3w__get_proc("glQueryCounter");
	gl3wReadBuffer = (PFNGLREADBUFFERPROC) gl3w__get_proc("glReadBuffer");
	gl3wReadPixels = (PFNGLREADPIXELSPROC) gl3w__get_proc("glReadPixels");
	gl3wReadnPixels = (PFNGLREADNPIXELSPROC) gl3w__get_proc("glReadnPixels");
	gl3wReadnPixelsARB = (PFNGLREADNPIXELSARBPROC) gl3w__get_proc("glReadnPixelsARB");
	gl3wReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILERPROC) gl3w__get_proc("glReleaseShaderCompiler");
	gl3wRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) gl3w__get_proc("glRenderbufferStorage");
	gl3wRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) gl3w__get_proc("glRenderbufferStorageMultisample");
	gl3wResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACKPROC) gl3w__get_proc("glResumeTransformFeedback");
	gl3wSampleCoverage = (PFNGLSAMPLECOVERAGEPROC) gl3w__get_proc("glSampleCoverage");
	gl3wSampleMaski = (PFNGLSAMPLEMASKIPROC) gl3w__get_proc("glSampleMaski");
	gl3wSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC) gl3w__get_proc("glSamplerParameterIiv");
	gl3wSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC) gl3w__get_proc("glSamplerParameterIuiv");
	gl3wSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC) gl3w__get_proc("glSamplerParameterf");
	gl3wSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC) gl3w__get_proc("glSamplerParameterfv");
	gl3wSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC) gl3w__get_proc("glSamplerParameteri");
	gl3wSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC) gl3w__get_proc("glSamplerParameteriv");
	gl3wScissor = (PFNGLSCISSORPROC) gl3w__get_proc("glScissor");
	gl3wScissorArrayv = (PFNGLSCISSORARRAYVPROC) gl3w__get_proc("glScissorArrayv");
	gl3wScissorIndexed = (PFNGLSCISSORINDEXEDPROC) gl3w__get_proc("glScissorIndexed");
	gl3wScissorIndexedv = (PFNGLSCISSORINDEXEDVPROC) gl3w__get_proc("glScissorIndexedv");
	gl3wShaderBinary = (PFNGLSHADERBINARYPROC) gl3w__get_proc("glShaderBinary");
	gl3wShaderSource = (PFNGLSHADERSOURCEPROC) gl3w__get_proc("glShaderSource");
	gl3wShaderStorageBlockBinding = (PFNGLSHADERSTORAGEBLOCKBINDINGPROC) gl3w__get_proc("glShaderStorageBlockBinding");
	gl3wStencilFunc = (PFNGLSTENCILFUNCPROC) gl3w__get_proc("glStencilFunc");
	gl3wStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC) gl3w__get_proc("glStencilFuncSeparate");
	gl3wStencilMask = (PFNGLSTENCILMASKPROC) gl3w__get_proc("glStencilMask");
	gl3wStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC) gl3w__get_proc("glStencilMaskSeparate");
	gl3wStencilOp = (PFNGLSTENCILOPPROC) gl3w__get_proc("glStencilOp");
	gl3wStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC) gl3w__get_proc("glStencilOpSeparate");
	gl3wTexBuffer = (PFNGLTEXBUFFERPROC) gl3w__get_proc("glTexBuffer");
	gl3wTexBufferRange = (PFNGLTEXBUFFERRANGEPROC) gl3w__get_proc("glTexBufferRange");
	gl3wTexImage1D = (PFNGLTEXIMAGE1DPROC) gl3w__get_proc("glTexImage1D");
	gl3wTexImage2D = (PFNGLTEXIMAGE2DPROC) gl3w__get_proc("glTexImage2D");
	gl3wTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC) gl3w__get_proc("glTexImage2DMultisample");
	gl3wTexImage3D = (PFNGLTEXIMAGE3DPROC) gl3w__get_proc("glTexImage3D");
	gl3wTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC) gl3w__get_proc("glTexImage3DMultisample");
	gl3wTexPageCommitmentARB = (PFNGLTEXPAGECOMMITMENTARBPROC) gl3w__get_proc("glTexPageCommitmentARB");
	gl3wTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC) gl3w__get_proc("glTexParameterIiv");
	gl3wTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC) gl3w__get_proc("glTexParameterIuiv");
	gl3wTexParameterf = (PFNGLTEXPARAMETERFPROC) gl3w__get_proc("glTexParameterf");
	gl3wTexParameterfv = (PFNGLTEXPARAMETERFVPROC) gl3w__get_proc("glTexParameterfv");
	gl3wTexParameteri = (PFNGLTEXPARAMETERIPROC) gl3w__get_proc("glTexParameteri");
	gl3wTexParameteriv = (PFNGLTEXPARAMETERIVPROC) gl3w__get_proc("glTexParameteriv");
	gl3wTexStorage1D = (PFNGLTEXSTORAGE1DPROC) gl3w__get_proc("glTexStorage1D");
	gl3wTexStorage2D = (PFNGLTEXSTORAGE2DPROC) gl3w__get_proc("glTexStorage2D");
	gl3wTexStorage2DMultisample = (PFNGLTEXSTORAGE2DMULTISAMPLEPROC) gl3w__get_proc("glTexStorage2DMultisample");
	gl3wTexStorage3D = (PFNGLTEXSTORAGE3DPROC) gl3w__get_proc("glTexStorage3D");
	gl3wTexStorage3DMultisample = (PFNGLTEXSTORAGE3DMULTISAMPLEPROC) gl3w__get_proc("glTexStorage3DMultisample");
	gl3wTexSubImage1D = (PFNGLTEXSUBIMAGE1DPROC) gl3w__get_proc("glTexSubImage1D");
	gl3wTexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC) gl3w__get_proc("glTexSubImage2D");
	gl3wTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC) gl3w__get_proc("glTexSubImage3D");
	gl3wTextureBarrier = (PFNGLTEXTUREBARRIERPROC) gl3w__get_proc("glTextureBarrier");
	gl3wTextureBuffer = (PFNGLTEXTUREBUFFERPROC) gl3w__get_proc("glTextureBuffer");
	gl3wTextureBufferRange = (PFNGLTEXTUREBUFFERRANGEPROC) gl3w__get_proc("glTextureBufferRange");
	gl3wTextureParameterIiv = (PFNGLTEXTUREPARAMETERIIVPROC) gl3w__get_proc("glTextureParameterIiv");
	gl3wTextureParameterIuiv = (PFNGLTEXTUREPARAMETERIUIVPROC) gl3w__get_proc("glTextureParameterIuiv");
	gl3wTextureParameterf = (PFNGLTEXTUREPARAMETERFPROC) gl3w__get_proc("glTextureParameterf");
	gl3wTextureParameterfv = (PFNGLTEXTUREPARAMETERFVPROC) gl3w__get_proc("glTextureParameterfv");
	gl3wTextureParameteri = (PFNGLTEXTUREPARAMETERIPROC) gl3w__get_proc("glTextureParameteri");
	gl3wTextureParameteriv = (PFNGLTEXTUREPARAMETERIVPROC) gl3w__get_proc("glTextureParameteriv");
	gl3wTextureStorage1D = (PFNGLTEXTURESTORAGE1DPROC) gl3w__get_proc("glTextureStorage1D");
	gl3wTextureStorage2D = (PFNGLTEXTURESTORAGE2DPROC) gl3w__get_proc("glTextureStorage2D");
	gl3wTextureStorage2DMultisample = (PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC) gl3w__get_proc("glTextureStorage2DMultisample");
	gl3wTextureStorage3D = (PFNGLTEXTURESTORAGE3DPROC) gl3w__get_proc("glTextureStorage3D");
	gl3wTextureStorage3DMultisample = (PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC) gl3w__get_proc("glTextureStorage3DMultisample");
	gl3wTextureSubImage1D = (PFNGLTEXTURESUBIMAGE1DPROC) gl3w__get_proc("glTextureSubImage1D");
	gl3wTextureSubImage2D = (PFNGLTEXTURESUBIMAGE2DPROC) gl3w__get_proc("glTextureSubImage2D");
	gl3wTextureSubImage3D = (PFNGLTEXTURESUBIMAGE3DPROC) gl3w__get_proc("glTextureSubImage3D");
	gl3wTextureView = (PFNGLTEXTUREVIEWPROC) gl3w__get_proc("glTextureView");
	gl3wTransformFeedbackBufferBase = (PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC) gl3w__get_proc("glTransformFeedbackBufferBase");
	gl3wTransformFeedbackBufferRange = (PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC) gl3w__get_proc("glTransformFeedbackBufferRange");
	gl3wTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC) gl3w__get_proc("glTransformFeedbackVaryings");
	gl3wUniform1d = (PFNGLUNIFORM1DPROC) gl3w__get_proc("glUniform1d");
	gl3wUniform1dv = (PFNGLUNIFORM1DVPROC) gl3w__get_proc("glUniform1dv");
	gl3wUniform1f = (PFNGLUNIFORM1FPROC) gl3w__get_proc("glUniform1f");
	gl3wUniform1fv = (PFNGLUNIFORM1FVPROC) gl3w__get_proc("glUniform1fv");
	gl3wUniform1i = (PFNGLUNIFORM1IPROC) gl3w__get_proc("glUniform1i");
	gl3wUniform1iv = (PFNGLUNIFORM1IVPROC) gl3w__get_proc("glUniform1iv");
	gl3wUniform1ui = (PFNGLUNIFORM1UIPROC) gl3w__get_proc("glUniform1ui");
	gl3wUniform1uiv = (PFNGLUNIFORM1UIVPROC) gl3w__get_proc("glUniform1uiv");
	gl3wUniform2d = (PFNGLUNIFORM2DPROC) gl3w__get_proc("glUniform2d");
	gl3wUniform2dv = (PFNGLUNIFORM2DVPROC) gl3w__get_proc("glUniform2dv");
	gl3wUniform2f = (PFNGLUNIFORM2FPROC) gl3w__get_proc("glUniform2f");
	gl3wUniform2fv = (PFNGLUNIFORM2FVPROC) gl3w__get_proc("glUniform2fv");
	gl3wUniform2i = (PFNGLUNIFORM2IPROC) gl3w__get_proc("glUniform2i");
	gl3wUniform2iv = (PFNGLUNIFORM2IVPROC) gl3w__get_proc("glUniform2iv");
	gl3wUniform2ui = (PFNGLUNIFORM2UIPROC) gl3w__get_proc("glUniform2ui");
	gl3wUniform2uiv = (PFNGLUNIFORM2UIVPROC) gl3w__get_proc("glUniform2uiv");
	gl3wUniform3d = (PFNGLUNIFORM3DPROC) gl3w__get_proc("glUniform3d");
	gl3wUniform3dv = (PFNGLUNIFORM3DVPROC) gl3w__get_proc("glUniform3dv");
	gl3wUniform3f = (PFNGLUNIFORM3FPROC) gl3w__get_proc("glUniform3f");
	gl3wUniform3fv = (PFNGLUNIFORM3FVPROC) gl3w__get_proc("glUniform3fv");
	gl3wUniform3i = (PFNGLUNIFORM3IPROC) gl3w__get_proc("glUniform3i");
	gl3wUniform3iv = (PFNGLUNIFORM3IVPROC) gl3w__get_proc("glUniform3iv");
	gl3wUniform3ui = (PFNGLUNIFORM3UIPROC) gl3w__get_proc("glUniform3ui");
	gl3wUniform3uiv = (PFNGLUNIFORM3UIVPROC) gl3w__get_proc("glUniform3uiv");
	gl3wUniform4d = (PFNGLUNIFORM4DPROC) gl3w__get_proc("glUniform4d");
	gl3wUniform4dv = (PFNGLUNIFORM4DVPROC) gl3w__get_proc("glUniform4dv");
	gl3wUniform4f = (PFNGLUNIFORM4FPROC) gl3w__get_proc("glUniform4f");
	gl3wUniform4fv = (PFNGLUNIFORM4FVPROC) gl3w__get_proc("glUniform4fv");
	gl3wUniform4i = (PFNGLUNIFORM4IPROC) gl3w__get_proc("glUniform4i");
	gl3wUniform4iv = (PFNGLUNIFORM4IVPROC) gl3w__get_proc("glUniform4iv");
	gl3wUniform4ui = (PFNGLUNIFORM4UIPROC) gl3w__get_proc("glUniform4ui");
	gl3wUniform4uiv = (PFNGLUNIFORM4UIVPROC) gl3w__get_proc("glUniform4uiv");
	gl3wUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC) gl3w__get_proc("glUniformBlockBinding");
	gl3wUniformHandleui64ARB = (PFNGLUNIFORMHANDLEUI64ARBPROC) gl3w__get_proc("glUniformHandleui64ARB");
	gl3wUniformHandleui64vARB = (PFNGLUNIFORMHANDLEUI64VARBPROC) gl3w__get_proc("glUniformHandleui64vARB");
	gl3wUniformMatrix2dv = (PFNGLUNIFORMMATRIX2DVPROC) gl3w__get_proc("glUniformMatrix2dv");
	gl3wUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC) gl3w__get_proc("glUniformMatrix2fv");
	gl3wUniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DVPROC) gl3w__get_proc("glUniformMatrix2x3dv");
	gl3wUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC) gl3w__get_proc("glUniformMatrix2x3fv");
	gl3wUniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DVPROC) gl3w__get_proc("glUniformMatrix2x4dv");
	gl3wUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC) gl3w__get_proc("glUniformMatrix2x4fv");
	gl3wUniformMatrix3dv = (PFNGLUNIFORMMATRIX3DVPROC) gl3w__get_proc("glUniformMatrix3dv");
	gl3wUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC) gl3w__get_proc("glUniformMatrix3fv");
	gl3wUniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DVPROC) gl3w__get_proc("glUniformMatrix3x2dv");
	gl3wUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC) gl3w__get_proc("glUniformMatrix3x2fv");
	gl3wUniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DVPROC) gl3w__get_proc("glUniformMatrix3x4dv");
	gl3wUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC) gl3w__get_proc("glUniformMatrix3x4fv");
	gl3wUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DVPROC) gl3w__get_proc("glUniformMatrix4dv");
	gl3wUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) gl3w__get_proc("glUniformMatrix4fv");
	gl3wUniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DVPROC) gl3w__get_proc("glUniformMatrix4x2dv");
	gl3wUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC) gl3w__get_proc("glUniformMatrix4x2fv");
	gl3wUniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DVPROC) gl3w__get_proc("glUniformMatrix4x3dv");
	gl3wUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC) gl3w__get_proc("glUniformMatrix4x3fv");
	gl3wUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIVPROC) gl3w__get_proc("glUniformSubroutinesuiv");
	gl3wUnmapBuffer = (PFNGLUNMAPBUFFERPROC) gl3w__get_proc("glUnmapBuffer");
	gl3wUnmapNamedBuffer = (PFNGLUNMAPNAMEDBUFFERPROC) gl3w__get_proc("glUnmapNamedBuffer");
	gl3wUseProgram = (PFNGLUSEPROGRAMPROC) gl3w__get_proc("glUseProgram");
	gl3wUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC) gl3w__get_proc("glUseProgramStages");
	gl3wValidateProgram = (PFNGLVALIDATEPROGRAMPROC) gl3w__get_proc("glValidateProgram");
	gl3wValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINEPROC) gl3w__get_proc("glValidateProgramPipeline");
	gl3wVertexArrayAttribBinding = (PFNGLVERTEXARRAYATTRIBBINDINGPROC) gl3w__get_proc("glVertexArrayAttribBinding");
	gl3wVertexArrayAttribFormat = (PFNGLVERTEXARRAYATTRIBFORMATPROC) gl3w__get_proc("glVertexArrayAttribFormat");
	gl3wVertexArrayAttribIFormat = (PFNGLVERTEXARRAYATTRIBIFORMATPROC) gl3w__get_proc("glVertexArrayAttribIFormat");
	gl3wVertexArrayAttribLFormat = (PFNGLVERTEXARRAYATTRIBLFORMATPROC) gl3w__get_proc("glVertexArrayAttribLFormat");
	gl3wVertexArrayBindingDivisor = (PFNGLVERTEXARRAYBINDINGDIVISORPROC) gl3w__get_proc("glVertexArrayBindingDivisor");
	gl3wVertexArrayElementBuffer = (PFNGLVERTEXARRAYELEMENTBUFFERPROC) gl3w__get_proc("glVertexArrayElementBuffer");
	gl3wVertexArrayVertexBuffer = (PFNGLVERTEXARRAYVERTEXBUFFERPROC) gl3w__get_proc("glVertexArrayVertexBuffer");
	gl3wVertexArrayVertexBuffers = (PFNGLVERTEXARRAYVERTEXBUFFERSPROC) gl3w__get_proc("glVertexArrayVertexBuffers");
	gl3wVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC) gl3w__get_proc("glVertexAttrib1d");
	gl3wVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC) gl3w__get_proc("glVertexAttrib1dv");
	gl3wVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC) gl3w__get_proc("glVertexAttrib1f");
	gl3wVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC) gl3w__get_proc("glVertexAttrib1fv");
	gl3wVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC) gl3w__get_proc("glVertexAttrib1s");
	gl3wVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC) gl3w__get_proc("glVertexAttrib1sv");
	gl3wVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC) gl3w__get_proc("glVertexAttrib2d");
	gl3wVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC) gl3w__get_proc("glVertexAttrib2dv");
	gl3wVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC) gl3w__get_proc("glVertexAttrib2f");
	gl3wVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC) gl3w__get_proc("glVertexAttrib2fv");
	gl3wVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC) gl3w__get_proc("glVertexAttrib2s");
	gl3wVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC) gl3w__get_proc("glVertexAttrib2sv");
	gl3wVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC) gl3w__get_proc("glVertexAttrib3d");
	gl3wVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC) gl3w__get_proc("glVertexAttrib3dv");
	gl3wVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC) gl3w__get_proc("glVertexAttrib3f");
	gl3wVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC) gl3w__get_proc("glVertexAttrib3fv");
	gl3wVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC) gl3w__get_proc("glVertexAttrib3s");
	gl3wVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC) gl3w__get_proc("glVertexAttrib3sv");
	gl3wVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC) gl3w__get_proc("glVertexAttrib4Nbv");
	gl3wVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC) gl3w__get_proc("glVertexAttrib4Niv");
	gl3wVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC) gl3w__get_proc("glVertexAttrib4Nsv");
	gl3wVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC) gl3w__get_proc("glVertexAttrib4Nub");
	gl3wVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC) gl3w__get_proc("glVertexAttrib4Nubv");
	gl3wVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC) gl3w__get_proc("glVertexAttrib4Nuiv");
	gl3wVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC) gl3w__get_proc("glVertexAttrib4Nusv");
	gl3wVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC) gl3w__get_proc("glVertexAttrib4bv");
	gl3wVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC) gl3w__get_proc("glVertexAttrib4d");
	gl3wVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC) gl3w__get_proc("glVertexAttrib4dv");
	gl3wVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC) gl3w__get_proc("glVertexAttrib4f");
	gl3wVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC) gl3w__get_proc("glVertexAttrib4fv");
	gl3wVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC) gl3w__get_proc("glVertexAttrib4iv");
	gl3wVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC) gl3w__get_proc("glVertexAttrib4s");
	gl3wVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC) gl3w__get_proc("glVertexAttrib4sv");
	gl3wVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC) gl3w__get_proc("glVertexAttrib4ubv");
	gl3wVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC) gl3w__get_proc("glVertexAttrib4uiv");
	gl3wVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC) gl3w__get_proc("glVertexAttrib4usv");
	gl3wVertexAttribBinding = (PFNGLVERTEXATTRIBBINDINGPROC) gl3w__get_proc("glVertexAttribBinding");
	gl3wVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC) gl3w__get_proc("glVertexAttribDivisor");
	gl3wVertexAttribFormat = (PFNGLVERTEXATTRIBFORMATPROC) gl3w__get_proc("glVertexAttribFormat");
	gl3wVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC) gl3w__get_proc("glVertexAttribI1i");
	gl3wVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC) gl3w__get_proc("glVertexAttribI1iv");
	gl3wVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC) gl3w__get_proc("glVertexAttribI1ui");
	gl3wVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC) gl3w__get_proc("glVertexAttribI1uiv");
	gl3wVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC) gl3w__get_proc("glVertexAttribI2i");
	gl3wVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC) gl3w__get_proc("glVertexAttribI2iv");
	gl3wVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC) gl3w__get_proc("glVertexAttribI2ui");
	gl3wVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC) gl3w__get_proc("glVertexAttribI2uiv");
	gl3wVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC) gl3w__get_proc("glVertexAttribI3i");
	gl3wVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC) gl3w__get_proc("glVertexAttribI3iv");
	gl3wVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC) gl3w__get_proc("glVertexAttribI3ui");
	gl3wVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC) gl3w__get_proc("glVertexAttribI3uiv");
	gl3wVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC) gl3w__get_proc("glVertexAttribI4bv");
	gl3wVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC) gl3w__get_proc("glVertexAttribI4i");
	gl3wVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC) gl3w__get_proc("glVertexAttribI4iv");
	gl3wVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC) gl3w__get_proc("glVertexAttribI4sv");
	gl3wVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC) gl3w__get_proc("glVertexAttribI4ubv");
	gl3wVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC) gl3w__get_proc("glVertexAttribI4ui");
	gl3wVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC) gl3w__get_proc("glVertexAttribI4uiv");
	gl3wVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC) gl3w__get_proc("glVertexAttribI4usv");
	gl3wVertexAttribIFormat = (PFNGLVERTEXATTRIBIFORMATPROC) gl3w__get_proc("glVertexAttribIFormat");
	gl3wVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC) gl3w__get_proc("glVertexAttribIPointer");
	gl3wVertexAttribL1d = (PFNGLVERTEXATTRIBL1DPROC) gl3w__get_proc("glVertexAttribL1d");
	gl3wVertexAttribL1dv = (PFNGLVERTEXATTRIBL1DVPROC) gl3w__get_proc("glVertexAttribL1dv");
	gl3wVertexAttribL1ui64ARB = (PFNGLVERTEXATTRIBL1UI64ARBPROC) gl3w__get_proc("glVertexAttribL1ui64ARB");
	gl3wVertexAttribL1ui64vARB = (PFNGLVERTEXATTRIBL1UI64VARBPROC) gl3w__get_proc("glVertexAttribL1ui64vARB");
	gl3wVertexAttribL2d = (PFNGLVERTEXATTRIBL2DPROC) gl3w__get_proc("glVertexAttribL2d");
	gl3wVertexAttribL2dv = (PFNGLVERTEXATTRIBL2DVPROC) gl3w__get_proc("glVertexAttribL2dv");
	gl3wVertexAttribL3d = (PFNGLVERTEXATTRIBL3DPROC) gl3w__get_proc("glVertexAttribL3d");
	gl3wVertexAttribL3dv = (PFNGLVERTEXATTRIBL3DVPROC) gl3w__get_proc("glVertexAttribL3dv");
	gl3wVertexAttribL4d = (PFNGLVERTEXATTRIBL4DPROC) gl3w__get_proc("glVertexAttribL4d");
	gl3wVertexAttribL4dv = (PFNGLVERTEXATTRIBL4DVPROC) gl3w__get_proc("glVertexAttribL4dv");
	gl3wVertexAttribLFormat = (PFNGLVERTEXATTRIBLFORMATPROC) gl3w__get_proc("glVertexAttribLFormat");
	gl3wVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC) gl3w__get_proc("glVertexAttribLPointer");
	gl3wVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC) gl3w__get_proc("glVertexAttribP1ui");
	gl3wVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC) gl3w__get_proc("glVertexAttribP1uiv");
	gl3wVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC) gl3w__get_proc("glVertexAttribP2ui");
	gl3wVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC) gl3w__get_proc("glVertexAttribP2uiv");
	gl3wVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC) gl3w__get_proc("glVertexAttribP3ui");
	gl3wVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC) gl3w__get_proc("glVertexAttribP3uiv");
	gl3wVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC) gl3w__get_proc("glVertexAttribP4ui");
	gl3wVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC) gl3w__get_proc("glVertexAttribP4uiv");
	gl3wVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) gl3w__get_proc("glVertexAttribPointer");
	gl3wVertexBindingDivisor = (PFNGLVERTEXBINDINGDIVISORPROC) gl3w__get_proc("glVertexBindingDivisor");
	gl3wViewport = (PFNGLVIEWPORTPROC) gl3w__get_proc("glViewport");
	gl3wViewportArrayv = (PFNGLVIEWPORTARRAYVPROC) gl3w__get_proc("glViewportArrayv");
	gl3wViewportIndexedf = (PFNGLVIEWPORTINDEXEDFPROC) gl3w__get_proc("glViewportIndexedf");
	gl3wViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFVPROC) gl3w__get_proc("glViewportIndexedfv");
	gl3wWaitSync = (PFNGLWAITSYNCPROC) gl3w__get_proc("glWaitSync");
}


#endif /* GL3W_IMPLEMENTATION */
