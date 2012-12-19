// Copyright 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CCRenderPass_h
#define CCRenderPass_h

#include "CCDrawQuad.h"
#include "CCOcclusionTracker.h"
#include "CCSharedQuadState.h"
#include "FloatRect.h"
#include "SkColor.h"
#include <public/WebFilterOperations.h>
#include <public/WebTransformationMatrix.h>
#include <wtf/HashMap.h>
#include <wtf/PassOwnPtr.h>
#include <wtf/Vector.h>

namespace cc {

class CCLayerImpl;
class CCRenderSurface;

struct CCAppendQuadsData;

// A list of CCDrawQuad objects, sorted internally in front-to-back order.
class CCQuadList : public Vector<OwnPtr<CCDrawQuad> > {
public:
    typedef reverse_iterator backToFrontIterator;
    typedef const_reverse_iterator constBackToFrontIterator;

    inline backToFrontIterator backToFrontBegin() { return rbegin(); }
    inline backToFrontIterator backToFrontEnd() { return rend(); }
    inline constBackToFrontIterator backToFrontBegin() const { return rbegin(); }
    inline constBackToFrontIterator backToFrontEnd() const { return rend(); }
};

typedef Vector<OwnPtr<CCSharedQuadState> > CCSharedQuadStateList;

class CCRenderPass {
    WTF_MAKE_NONCOPYABLE(CCRenderPass);
public:
    struct Id {
        int layerId;
        int index;

        Id(int layerId, int index)
            : layerId(layerId)
            , index(index)
        {
        }

        bool operator==(const Id& other) const { return layerId == other.layerId && index == other.index; }
        bool operator!=(const Id& other) const { return !(*this == other); }
        bool operator<(const Id& other) const { return layerId < other.layerId || (layerId == other.layerId && index < other.index); }
    };

    static PassOwnPtr<CCRenderPass> create(Id, IntRect outputRect, const WebKit::WebTransformationMatrix& transformToRootTarget);

    // A shallow copy of the render pass, which does not include its quads.
    PassOwnPtr<CCRenderPass> copy(Id newId) const;

    void appendQuadsForLayer(CCLayerImpl*, CCOcclusionTrackerImpl*, CCAppendQuadsData&);
    void appendQuadsForRenderSurfaceLayer(CCLayerImpl*, const CCRenderPass* contributingRenderPass, CCOcclusionTrackerImpl*, CCAppendQuadsData&);
    void appendQuadsToFillScreen(CCLayerImpl* rootLayer, SkColor screenBackgroundColor, const CCOcclusionTrackerImpl&);

    const CCQuadList& quadList() const { return m_quadList; }

    Id id() const { return m_id; }

    // FIXME: Modify this transform when merging the RenderPass into a parent compositor.
    // Transforms from quad's original content space to the root target's content space.
    const WebKit::WebTransformationMatrix& transformToRootTarget() const { return m_transformToRootTarget; }

    // This denotes the bounds in physical pixels of the output generated by this RenderPass.
    const IntRect& outputRect() const { return m_outputRect; }

    FloatRect damageRect() const { return m_damageRect; }
    void setDamageRect(FloatRect rect) { m_damageRect = rect; }

    const WebKit::WebFilterOperations& filters() const { return m_filters; }
    void setFilters(const WebKit::WebFilterOperations& filters) { m_filters = filters; }

    const WebKit::WebFilterOperations& backgroundFilters() const { return m_backgroundFilters; }
    void setBackgroundFilters(const WebKit::WebFilterOperations& filters) { m_backgroundFilters = filters; }

    bool hasTransparentBackground() const { return m_hasTransparentBackground; }
    void setHasTransparentBackground(bool transparent) { m_hasTransparentBackground = transparent; }

    bool hasOcclusionFromOutsideTargetSurface() const { return m_hasOcclusionFromOutsideTargetSurface; }
    void setHasOcclusionFromOutsideTargetSurface(bool hasOcclusionFromOutsideTargetSurface) { m_hasOcclusionFromOutsideTargetSurface = hasOcclusionFromOutsideTargetSurface; }
protected:
    CCRenderPass(Id, IntRect outputRect, const WebKit::WebTransformationMatrix& transformToRootTarget);

    Id m_id;
    CCQuadList m_quadList;
    CCSharedQuadStateList m_sharedQuadStateList;
    WebKit::WebTransformationMatrix m_transformToRootTarget;
    IntRect m_outputRect;
    FloatRect m_damageRect;
    bool m_hasTransparentBackground;
    bool m_hasOcclusionFromOutsideTargetSurface;
    WebKit::WebFilterOperations m_filters;
    WebKit::WebFilterOperations m_backgroundFilters;
};

} // namespace cc

namespace WTF {
template<> struct HashTraits<cc::CCRenderPass::Id> : GenericHashTraits<cc::CCRenderPass::Id> {
    static const bool emptyValueIsZero = false;
    static const bool needsDestruction = false;
    static cc::CCRenderPass::Id emptyValue() { return cc::CCRenderPass::Id(0, 0); }
    static void constructDeletedValue(cc::CCRenderPass::Id& slot) { slot = cc::CCRenderPass::Id(-1, -1); }
    static bool isDeletedValue(cc::CCRenderPass::Id value) { return value.layerId == -1 && value.index == -1; }
};
template<> struct IntHash<cc::CCRenderPass::Id> {
    static unsigned hash(const cc::CCRenderPass::Id& key) { return PairHash<int, int>::hash(std::make_pair(key.layerId, key.index)); }
    static bool equal(const cc::CCRenderPass::Id& a, const cc::CCRenderPass::Id& b) { return a == b; }
    static const bool safeToCompareToEmptyOrDeleted = true;
};
template<> struct DefaultHash<cc::CCRenderPass::Id> {
    typedef IntHash<cc::CCRenderPass::Id> Hash;
};
} // namespace WTF

namespace cc {
typedef Vector<CCRenderPass*> CCRenderPassList;
typedef HashMap<CCRenderPass::Id, OwnPtr<CCRenderPass> > CCRenderPassIdHashMap;
} // namespace cc

#endif
