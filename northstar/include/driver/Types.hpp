#pragma once

namespace northstar {
    namespace driver {
        namespace types {
            enum class EHand {
                Left,
                Right
            };

            enum class EHandeSkeletonBone : vr::BoneIndex_t
            {
                Root = 0,
                Wrist,
                Thumb0,
                Thumb1,
                Thumb2,
                Thumb3,
                IndexFinger0,
                IndexFinger1,
                IndexFinger2,
                IndexFinger3,
                IndexFinger4,
                MiddleFinger0,
                MiddleFinger1,
                MiddleFinger2,
                MiddleFinger3,
                MiddleFinger4,
                RingFinger0,
                RingFinger1,
                RingFinger2,
                RingFinger3,
                RingFinger4,
                PinkyFinger0,
                PinkyFinger1,
                PinkyFinger2,
                PinkyFinger3,
                PinkyFinger4,
                Aux_Thumb,
                Aux_IndexFinger,
                Aux_MiddleFinger,
                Aux_RingFinger,
                Aux_PinkyFinger,
                Count
            };
        }
    }
}
