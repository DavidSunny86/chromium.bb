# Copyright (c) 2013 The ANGLE Project Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
    'variables':
    {
        'angle_enable_d3d9%': 1,
        'angle_enable_d3d11%': 1,
        # This file list is shared with the GN build.
        'angle_libegl_sources':
        [
            '../include/EGL/egl.h',
            '../include/EGL/eglext.h',
            '../include/EGL/eglplatform.h',
            '../include/GLES2/gl2.h',
            '../include/GLES2/gl2ext.h',
            '../include/GLES2/gl2platform.h',
            '../include/GLES3/gl3.h',
            '../include/GLES3/gl3ext.h',
            '../include/GLES3/gl3platform.h',
            '../include/GLSLANG/ShaderLang.h',
            '../include/GLSLANG/ShaderVars.h',
            '../include/KHR/khrplatform.h',
            '../include/angle_gl.h',
            'common/RefCountObject.cpp',
            'common/RefCountObject.h',
            'common/angleutils.cpp',
            'common/angleutils.h',
            'common/debug.cpp',
            'common/debug.h',
            'common/event_tracer.cpp',
            'common/event_tracer.h',
            'common/mathutil.cpp',
            'common/mathutil.h',
            'common/platform.h',
            'common/tls.cpp',
            'common/tls.h',
            'common/utilities.cpp',
            'common/utilities.h',
            'libEGL/Config.cpp',
            'libEGL/Config.h',
            'libEGL/Display.cpp',
            'libEGL/Display.h',
            'libEGL/Surface.cpp',
            'libEGL/Surface.h',
            'libEGL/libEGL.cpp',
            'libEGL/main.cpp',
            'libEGL/main.h',
            'libEGL/resource.h',
        ],
    },
    # Everything below this is duplicated in the GN build. If you change
    # anything also change angle/BUILD.gn
    'conditions':
    [
        ['OS=="win"',
        {
            'targets':
            [
                {
                    'target_name': 'libEGL_static',
                    'type': 'static_library',
                    'dependencies': [ 'commit_id' ],
                    'include_dirs':
                    [
                        '.',
                        '../include',
                        'libGLESv2',
                    ],
                    'sources':
                    [
                        '<@(angle_libegl_sources)',
                    ],
                    'defines':
                    [
                        'GL_APICALL=',
                        'GL_GLEXT_PROTOTYPES=',
                        'EGLAPI=',
                    ],
                    'conditions':
                    [
                        ['angle_enable_d3d9==1',
                        {
                            'defines':
                            [
                                'ANGLE_ENABLE_D3D9',
                            ],
                        }],
                        ['angle_enable_d3d11==1',
                        {
                            'defines':
                            [
                                'ANGLE_ENABLE_D3D11',
                            ],
                        }],
                    ],
                    'includes': [ '../build/common_defines.gypi', ],
                    'link_settings':
                    {
                        'libraries':
                        [
                            '-ld3d9.lib',
                        ],
                    },
                    'configurations':
                    {
                        'Debug':
                        {
                            'defines':
                            [
                                'ANGLE_ENABLE_PERF',
                            ],
                        },
                    },
                },
                {
                    'target_name': 'libEGL',
                    'type': 'loadable_module',
                    'dependencies': [ 'libGLESv2_shared', 'libEGL_static', 'commit_id' ],
                    'include_dirs':
                    [
                        '.',
                        '../include',
                    ],
                    'sources':
                    [
                        'common/version.h',
                        'libEGL/dllmain.cpp',
                        'libEGL/libEGL.def',
                        'libEGL/libEGL.rc',
                    ],
                },
            ],
        },
        ],
    ],
}
