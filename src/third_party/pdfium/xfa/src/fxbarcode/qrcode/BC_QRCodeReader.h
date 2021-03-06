// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_SRC_FXBARCODE_QRCODE_BC_QRCODEREADER_H_
#define XFA_SRC_FXBARCODE_QRCODE_BC_QRCODEREADER_H_

#include <stdint.h>

#include "core/include/fxcrt/fx_string.h"
#include "xfa/src/fxbarcode/BC_Reader.h"

class CBC_QRDetector;
class CBC_BinaryBitmap;
class CBC_QRCoderDecoder;
class CBC_ResultPoint;
class CBC_ReedSolomonGF256;
class CBC_QRCoderVersion;
class CBC_QRDetector;
class CBC_QRDetectorResult;
class CBC_QRCoderErrorCorrectionLevel;
class CBC_QRCoderMode;
class CBC_QRDataMask;
class CFX_DIBitmap;

class CBC_QRCodeReader : public CBC_Reader {
 private:
  CBC_QRCoderDecoder* m_decoder;

 public:
  CBC_QRCodeReader();
  virtual ~CBC_QRCodeReader();
  CFX_ByteString Decode(CFX_DIBitmap* pBitmap,
                        int32_t hints,
                        int32_t byteModeDecode,
                        int32_t& e);
  CFX_ByteString Decode(const CFX_WideString& filename,
                        int32_t hints,
                        int32_t byteModeDecode,
                        int32_t& e);
  static void ReleaseAll();
  CFX_ByteString Decode(CBC_BinaryBitmap* image, int32_t hints, int32_t& e);
  CFX_ByteString Decode(CBC_BinaryBitmap* image, int32_t& e);
  virtual void Init();
};

#endif  // XFA_SRC_FXBARCODE_QRCODE_BC_QRCODEREADER_H_
